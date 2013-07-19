/* (C) 2009 by Andrea Franceschini <andrea.franceschini@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cmath>

#include <QtAlgorithms>

#include <QPainter>
#include <QMatrix>
#include <QPolygonF>
#include <QFontMetrics>

#include <DoodleSDK/widgets/CheckButtonWidget.h>
#include <DoodleSDK/widgets/PushButtonWidget.h>
#include "KeyboardWidget.h"

#include <QDebug>

Doodle::KeyboardWidget::KeyboardWidget() : Widget(0) {
	makeVisual();
	_active = true;
}

Doodle::KeyboardWidget::KeyboardWidget(Widget* parent = 0) : Widget(parent) {
	makeVisual();
	_active = true;
}

void Doodle::KeyboardWidget::makeVisual() {
	_shape << QPointF(0,0);
	_shape << QPointF(0,0);

	_size = 300;
	_font = QFont("Helvetica Neue", 10);
	_font.setStretch(QFont::Condensed);
	_font.setWeight(QFont::Bold);

	int i;
	for(i = 0; i < 8; i++) {
		_columns.append(20);
	}
}

void Doodle::KeyboardWidget::init() {
	CheckButtonWidget* w;
	int i, j;
	double xp, yp = 0;
	qDebug() << *_currentScale;
	for(i = 0; i < floor(_currentScale->size()/2); i++) {
		xp = 0;
		for(j = 0; j < _columns.size(); j++) {
			w = new CheckButtonWidget(this);
			w->setText("");
			w->setPosition(QPointF(xp-((float)(_columns.size()-1)*_columns[j]/800), yp-((_currentScale->size()/2 - 1)*10.0/800)));
			qDebug() << w->position();
			w->setSize(_columns[j] ,10);
			emit childAdded(w);
			xp += (double)2*_columns[j]/800;
		}
		yp += (double)2*10/800;
	}
	PushButtonWidget* closebw = new PushButtonWidget(this);
	closebw->setText("X");
	closebw->setPosition(QPointF(0.275, -(_currentScale->size()/2 + 1)*10.0/800));
	closebw->setSize(20,15);
	connect(closebw, SIGNAL(pushed()), SLOT(hide()));
	emit childAdded(closebw);

	PushButtonWidget* cleanbw = new PushButtonWidget(this);
	cleanbw->setText("CLEAR");
	cleanbw->setPosition(QPointF(0.225, -(_currentScale->size()/2 + 1)*10.0/800));
	cleanbw->setSize(20,15);
	cleanbw->setFont(_font);
	connect(cleanbw, SIGNAL(pushed()), SLOT(clean()));
	emit childAdded(cleanbw);

	setVisible(false);
}

void Doodle::KeyboardWidget::updateKeyboard() {
	foreach(Widget* c, _children) {
		emit childDestroyed(c);
		delete c;
	}
	_children.clear();
//	_scaleButtons.clear();

	CheckButtonWidget* w;
	int i, j;
	double xp, yp = 0;
	for(i = 0; i < floor(_currentScale->size()/2); i++) {
		xp = 0;
		for(j = 0; j < _columns.size(); j++) {
			w = new CheckButtonWidget(this);
			w->setText("");
			w->setPosition(_position + QPointF(xp-((float)(_columns.size()-1)*_columns[j]/800), yp-((_currentScale->size()/2 - 1)*10.0/800)));
			w->setSize(_columns[j], 10);
			emit childAdded(w);
			xp += (double)2*_columns[j]/800;
		}
		yp += (double)2*10/800;
	}

//	float k = -(_currentScale->size()/2 + 2)*10.0/800;
//	foreach(QString scale, _scales->uniqueKeys()) {
//		w = new CheckButtonWidget(this);
//		w->setText(scale);
//		w->setPosition(_position + QPointF((float)(_columns.size() + 3)*20.0/800, k+((_currentScale->size()/2)*10.0/800)));
//		w->setSize(50, 10);
//		w->setFont(_font);
//		_scaleButtons.append(w);
//		connect(w, SIGNAL(checked()), SLOT(changeScale()));
//		emit childAdded(w);
//		k += 2*10.0/800;
//	}

	PushButtonWidget* closebw = new PushButtonWidget(this);
	closebw->setText("X");
	closebw->setPosition(_position + QPointF(0.175, -(_currentScale->size()/2 + 2)*10.0/800));
	closebw->setSize(20,15);
	connect(closebw, SIGNAL(pushed()), SLOT(hide()));
	emit childAdded(closebw);

	PushButtonWidget* cleanbw = new PushButtonWidget(this);
	cleanbw->setText("CLEAR");
	cleanbw->setPosition(_position + QPointF(0.125, -(_currentScale->size()/2 + 2)*10.0/800));
	cleanbw->setSize(20,15);
	cleanbw->setFont(_font);
	connect(cleanbw, SIGNAL(pushed()), SLOT(clean()));
	emit childAdded(cleanbw);

	if(_visible) {
		setVisible(true);
	} else {
		setVisible(false);
	}
}

void Doodle::KeyboardWidget::render(QPainter* painter) {
	painter->save();
	updateVisual();
	int width = painter->device()->width();
	int height = painter->device()->height();
	QMatrix m;
	m.translate(_position.x()*width, _position.y()*height);
	m.rotate(180*_angle/M_PI);
	m.scale(_scale*_size, _scale*_size);
	QPolygonF shape = m.map(_shape);
	QRectF shapeBound = shape.boundingRect();
	QPen pen(Qt::white);
	pen.setCosmetic(false);
	pen.setWidth(2);
	painter->setPen(pen);
	QBrush brush(QColor::fromRgb(255,255,255,63));
	if(_active) {
		painter->setBrush(brush);
	}
	painter->drawPolygon(shape);
	pen.setWidth(10);
	painter->setPen(pen);
	QPointF from(-0.55,-(_currentScale->size() + 2)*10.0/800);
	QPointF to(-0.55, (_currentScale->size() + 2)*10.0/800);
	painter->drawLine(m.map(from), m.map(to));
	painter->restore();
}

void Doodle::KeyboardWidget::updateVisual() {
//	QList<int> notes = _chord->notes();
}

void Doodle::KeyboardWidget::tap(int n) {
	emit tapped(n);
}

void Doodle::KeyboardWidget::hide() {
	setVisible(false);
}

void Doodle::KeyboardWidget::clean() {
	foreach(Widget* w, _children) {
		if(CheckButtonWidget* cbw = qobject_cast<CheckButtonWidget*>(w)) {
			cbw->uncheck();
		}
	}
}

void Doodle::KeyboardWidget::changeScale() {
	qDebug() << sender();
	if(CheckButtonWidget* w = qobject_cast<CheckButtonWidget*>(sender())) {
		qDebug() << w->name();
		foreach(CheckButtonWidget* cbw, _scaleButtons) {
			cbw->setChecked(false);
		}
		w->setChecked(true);
		_currentScale = &((*_scales)[w->text()]);
		updateKeyboard();
	}
}

void Doodle::KeyboardWidget::setVisible(bool visible = true) {
	_visible = visible;
	if(_visible) {
		foreach(Widget* w, _children) {
			w->setVisible(true);
		}
	} else {
		foreach(Widget* w, _children) {
			w->setVisible(false);
		}
	}
}

QList<int> Doodle::KeyboardWidget::notes(int index) {
	QList<int> notesList;
	int i = 0;
	int m = _columns.size();
	foreach(Widget* w, _children) {
		if(CheckButtonWidget* cbw = qobject_cast<CheckButtonWidget*>(w)) {
			if(i % _columns.size() == index) {
				if(cbw->isChecked()) {
					QList<int> nl = _currentScale->toList();
					qSort(nl);
					notesList.append(nl[_currentScale->size() - 1 - i/m]);
				}
			}
			i++;
		}
	}

	return notesList;
}
