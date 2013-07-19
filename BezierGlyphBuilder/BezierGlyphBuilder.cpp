#include <stdexcept>

#include "BezierGlyphBuilder.h"
#include <QtGui>
#include "GlyphView.h"
#include "GlyphPainter.h"

#include <QtXml>

BezierGlyphBuilder::BezierGlyphBuilder(QWidget *parent)
    : QDialog(parent)
{
	int i;
	ui.setupUi(this);
	ui.inputFileNameLineEdit->setText("glyphs.xml");
	ui.outputFileNameLineEdit->setText("output.xml");
	_oldPoint = NULL;
	connect(ui.glyphComboBox, SIGNAL(activated(QString)), this, SLOT(setVariants(QString)));
	connect(ui.variantComboBox, SIGNAL(activated(QString)), this, SLOT(loadGlyph(QString)));
	connect(ui.samplesSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setSamples(int)));
	connect(ui.loadButton, SIGNAL(clicked()), this, SLOT(loadGlyphsFromFile()));

	connect(ui.applyButton, SIGNAL(clicked()), this, SLOT(saveVariant()));
	connect(ui.buildButton, SIGNAL(clicked()), this, SLOT(buildGlyphs()));

	_view = new GlyphView(&_glyphPainter, this);
	QGLFormat fmt;
	fmt.setAlpha(true);
	fmt.setAccum(true);
	fmt.setStencil(true);
	fmt.setOverlay(true);
	_view->setFormat(fmt);

	ui.glyphViewLayout->addWidget(_view, 1);
	QTimer* timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), _view, SLOT(animate()));
	timer->start(50);

	setWindowTitle(tr("Bezier Glyph Builder"));

	_glyphPainter.setTStep(1/(double)ui.samplesSpinBox->value());
	_glyphPainter.setGlyph(&_currentGlyph);
	_glyphPainter.setActivePoints(&_activePoints);
	connect(_view, SIGNAL(pointClicked(QPoint)), this, SLOT(pointClicked(QPoint)));
	connect(_view, SIGNAL(mouseDrag(QPoint)), this, SLOT(movePoints(QPoint)));
	connect(_view, SIGNAL(endMotion(QPoint)), this, SLOT(endMotion(QPoint)));
	connect(_view, SIGNAL(keyPressed(int)), this, SLOT(keyPressed(int)));
	connect(_view, SIGNAL(keyReleased(int)), this, SLOT(keyReleased(int)));

	loadGlyphsFromFile();
}

void BezierGlyphBuilder::loadGlyphsFromFile() {
	int i;
	QString glyphsFilename = ui.inputFileNameLineEdit->text();

	QFile glyphsFile(glyphsFilename);
	if(!glyphsFile.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(this, this->windowTitle(), tr("Couldn't load %1.").arg(glyphsFilename));
		return;
	}
	if(!_glyphsXml.setContent(&glyphsFile)) {
		glyphsFile.close();
		QMessageBox::critical(this, this->windowTitle(), tr("Couldn't parse %2.").arg(glyphsFilename));
		return;
	}
	glyphsFile.close();

	QDomElement glyphsRoot = _glyphsXml.documentElement();
	QDomNodeList glyphs = glyphsRoot.elementsByTagName("glyph");
	for(i = 0; i < glyphs.size(); i++) {
		QString id = glyphs.at(i).attributes().namedItem("id").nodeValue();
		if(ui.glyphComboBox->findText(id) == -1) {
			ui.glyphComboBox->addItem(glyphs.at(i).attributes().namedItem("id").nodeValue());
		}
	}
	setVariants(ui.glyphComboBox->currentText());
}

void BezierGlyphBuilder::setVariants(QString id) {
	int i;
	ui.variantComboBox->clear();
	QDomElement glyphsRoot = _glyphsXml.documentElement();
	QDomNodeList glyphs = glyphsRoot.elementsByTagName("glyph");
	for(i = 0; i < glyphs.size(); i++) {
		if(glyphs.at(i).attributes().namedItem("id").nodeValue() != id) {
			continue;
		}
		ui.variantComboBox->addItem(glyphs.at(i).attributes().namedItem("variant").nodeValue());
	}
	loadGlyph(ui.variantComboBox->currentText());
}

void BezierGlyphBuilder::loadGlyph(QString variant) {
	int i,j;
	_currentGlyph.clear();
	QString id = ui.glyphComboBox->currentText();
	QDomElement glyphsRoot = _glyphsXml.documentElement();
	QDomNodeList glyphs = glyphsRoot.elementsByTagName("glyph");
	for(i = 0; i < glyphs.size(); i++) {
		if(glyphs.at(i).attributes().namedItem("id").nodeValue() == id && glyphs.at(i).attributes().namedItem("variant").nodeValue() == variant) {
			QDomNodeList points = glyphs.at(i).childNodes().at(0).childNodes();
			for(j = 0; j < points.size(); j++) {
				_currentGlyph.append(QPointF(points.at(j).attributes().namedItem("x").nodeValue().toFloat(),points.at(j).attributes().namedItem("y").nodeValue().toFloat()));
			}
			break;
		} else {
			continue;
		}
	}
}

void BezierGlyphBuilder::setSamples(int i) {
	_glyphPainter.setTStep(1/(double)i);
}

void BezierGlyphBuilder::pointClicked(QPoint p) {
	if(_oldPoint == NULL) {
		_oldPoint = new QPoint(p);
	}
	int i;
	for(i = 0; i < _currentGlyph.size(); i++) {
		QPoint q((200*(_currentGlyph.at(i)+QPointF(1,1))).toPoint());
		if((p-q).manhattanLength() <= 5) {
			if(!_ctrl_pressed) {
				_activePoints.clear();
			}
			if(!_activePoints.contains(_currentGlyph.at(i))) {
				_activePoints.append(_currentGlyph.at(i));
			} else {
				_activePoints.removeAll(_currentGlyph.at(i));
			}
			return;
		}
	}
	_activePoints.clear();
}

void BezierGlyphBuilder::movePoints(QPoint p) {
	if(!_activePoints.empty()) {
		QPointF np = QPointF(p);
		QPointF d = (*_oldPoint - np)/200;
		int i;
		for(i = 0; i < _activePoints.size(); i++) {
			QPointF a = _activePoints.at(i);
			int j = _currentGlyph.indexOf(a);
			QPointF q = _currentGlyph.at(j);
			_currentGlyph[j].rx() = (q-d).x();
			_currentGlyph[j].ry() = (q-d).y();
			_activePoints.replace(i, _currentGlyph.at(j));
		}
		delete _oldPoint;
		_oldPoint = new QPoint(p);
		ui.applyButton->setEnabled(true);
	}
}

void BezierGlyphBuilder::endMotion(QPoint p) {
	delete _oldPoint;
	_oldPoint = NULL;
}

void BezierGlyphBuilder::keyPressed(int k) {
	switch(k) {
	case Qt::Key_A: {
		if(_activePoints.size() > 1) {
			int i;
			for(i = 0; i < _activePoints.size()-1; i++) {
				// TODO: Fix some quirk here...
				QPointF a((_activePoints.at(i)+_activePoints.at(i+1))/2);
				int j = _currentGlyph.indexOf(_activePoints.at(i+1));
				_currentGlyph.insert(j, a);
			}
		}
		break;
	}
	case Qt::Key_X: {
		int i;
		for(i = 0; i < _activePoints.size(); i++) {
			int j = _currentGlyph.indexOf(_activePoints.at(i));
			_currentGlyph.removeAt(j);
		}
		break;
	}
	case Qt::Key_Control: {
		_ctrl_pressed = true;
		break;
	}
	}
}

void BezierGlyphBuilder::keyReleased(int k) {
	switch(k) {
	case Qt::Key_Control: {
		_ctrl_pressed = false;
		break;
	}
	}
}

void BezierGlyphBuilder::saveVariant() {
	int i,j;
	QString id = ui.glyphComboBox->currentText();
	QString variant = ui.variantComboBox->currentText();

	QDomElement glyphsRoot = _glyphsXml.documentElement();
	QDomNodeList glyphs = glyphsRoot.elementsByTagName("glyph");
	for(i = 0; i < glyphs.size(); i++) {
		if(glyphs.at(i).attributes().namedItem("id").nodeValue() == id && glyphs.at(i).attributes().namedItem("variant").nodeValue() == variant) {
			QDomElement points = _glyphsXml.createElement("points");
			for(j = 0; j < _currentGlyph.size(); j++) {
				QDomElement point = _glyphsXml.createElement("point");
				point.setAttribute("x", round(_currentGlyph.at(j).x()*1000)/1000);
				point.setAttribute("y", round(_currentGlyph.at(j).y()*1000)/1000);
				points.appendChild(point);
			}
			glyphs.at(i).removeChild(glyphs.at(i).firstChild());
			glyphs.at(i).appendChild(points);
			break;
		} else {
			continue;
		}
	}
	QFile glyphsFile(ui.inputFileNameLineEdit->text());
	if(!glyphsFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		throw std::runtime_error("Couldn't write on " + ui.inputFileNameLineEdit->text().toStdString());
	}
	QTextStream out(&glyphsFile);
	out << _glyphsXml.toString();
	glyphsFile.close();
	ui.applyButton->setEnabled(false);
}

void BezierGlyphBuilder::buildGlyphs() {
	QString glyphsFilename = ui.inputFileNameLineEdit->text();
	QString outputFilename = ui.outputFileNameLineEdit->text();
	double t, t_step = _glyphPainter.tStep();
	int i,j,k,l,m;

	QDomDocument outputXml;
	QDomElement outputRoot = outputXml.createElement("glyphs");
	outputXml.appendChild(outputRoot);

	QDomElement glyphsRoot = _glyphsXml.documentElement();
	// TODO: Cannot append attributes to the root. Fix this.
	glyphsRoot.setAttribute("step", t_step);
	QDomNodeList glyphs = glyphsRoot.elementsByTagName("glyph");
	for(i = 0; i < glyphs.size(); i++) {
		QDomElement newGlyph = outputXml.createElement("glyph");
		outputRoot.appendChild(newGlyph);
		newGlyph.setAttribute("id", glyphs.at(i).attributes().namedItem("id").nodeValue());
		newGlyph.setAttribute("variant", glyphs.at(i).attributes().namedItem("variant").nodeValue());
		QDomElement newPoints = outputXml.createElement("points");
		newGlyph.appendChild(newPoints);

		QDomNodeList points = glyphs.at(i).childNodes();
		for(j = 0; j < points.size(); j++) {
			QDomNodeList point = points.at(j).childNodes();
			QList<QPointF> cp;
			for(k = 0; k < point.size(); k++) {
				cp.append(QPointF(point.at(k).attributes().namedItem("x").nodeValue().toFloat(), point.at(k).attributes().namedItem("y").nodeValue().toFloat()));
			}

			int n = cp.size()-1;

			unsigned long long* b = new unsigned long long[n+1];
			for(k = 0; k <= n; k++) {
				b[k] = _glyphPainter.choose(n,k);
			}
			QList<QPointF> r;
			for(t = 0; t < 1; t += t_step) {
				double x = 0, y = 0;
				for(k = 0; k <= n; k++) {
					x += b[k]*pow(1-t, (n-k))*pow(t, k)*cp.at(k).x();
					y += b[k]*pow(1-t, (n-k))*pow(t, k)*cp.at(k).y();
				}
				r.append(QPointF(x,y));
			}

			double mX = 1, MX = 0, mY = 1, MY = 0;
			for(k = 0; k < r.size(); k++) {
				mX = mX < r.at(k).x()? mX : r.at(k).x();
				MX = MX > r.at(k).x()? MX : r.at(k).x();
				mY = mY < r.at(k).y()? mY : r.at(k).y();
				MY = MY > r.at(k).y()? MY : r.at(k).y();
			}
			for(k = 0; k < r.size(); k++) {
				QPointF n = r.at(k);
				n.rx() = (n.rx() - mX) * (1/(MX-mX)) -0.5f;
				n.ry() = (n.ry() - mY) * (1/(MY-mY)) -0.5f;
				r.replace(k, n);

				QDomElement newPoint = outputXml.createElement("point");
				newPoints.appendChild(newPoint);
				newPoint.setAttribute("x", round(1000*n.x())/1000);
				newPoint.setAttribute("y", round(1000*n.y())/1000);
			}
			QPointF o = r.last() - r.first();
			newPoints.setAttribute("ox", round(1000*o.x())/1000);
			newPoints.setAttribute("oy", round(1000*o.y())/1000);
		}
	}
	QFile outputFile(outputFilename);
	if(!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::critical(this, this->windowTitle(), tr("Couldn't open %1 for writing.").arg(outputFilename));
		return;
	}
	QTextStream out(&outputFile);
	out << outputXml.toString();
	outputFile.close();
}
