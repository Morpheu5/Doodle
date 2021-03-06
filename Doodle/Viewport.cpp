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

#include <QPainter>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QFontMetrics>
#include <QtOpenGL>

#include <QDebug>

#include "Painter.h"
#include "Viewport.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

Doodle::Viewport::Viewport(Painter* painter)
	: QGLWidget((QWidget*)NULL), _painter(painter) {
	makeCurrent();
	_fbo = new QGLFramebufferObject(800, 800);

	QGLFormat fmt = format();
	fmt.setSampleBuffers(true);
	setFormat(fmt);

	resizeGL(800, 800);
	resize(800, 800);
	_painter->setSize(800, 800);
	_painter->setBackground(QBrush(QColor::fromHsv(210,127,63,255)));
	_fullscreen = false;
	_calibration = false;
	setAutoFillBackground(false);

	_hasShift = false;
	_xScale = 1.0;
	_yScale = 1.0;
	_xRotation = 0.0;
	_zRotation = 0.0;
	_xPosition = 0.0;
	_yPosition = 0.0;
	_zPosition = 0.0;
}

QSize Doodle::Viewport::minimumSizeHint() const {
	return QSize(400, 400);
}

QSize Doodle::Viewport::sizeHint() const {
	return QSize(800, 800);
}

Doodle::Viewport::~Viewport() {
	makeCurrent();
	glDeleteLists(_object, 1);
	delete _fbo;
}

void Doodle::Viewport::initializeGL() {
	double x = 1.0;
	double y = 1.0;
	_object = glGenLists(1);
	glNewList(_object, GL_COMPILE);
	glBegin(GL_QUADS);
	{
		qglColor(Qt::white);
		glTexCoord2d(0.0, 0.0); glVertex3d(-x,  y,  0.0);
		glTexCoord2d(1.0, 0.0); glVertex3d( x,  y,  0.0);
		glTexCoord2d(1.0, 1.0); glVertex3d( x, -y,  0.0);
		glTexCoord2d(0.0, 1.0); glVertex3d(-x, -y,  0.0);
	}
	glEnd();
	glEndList();
}

void Doodle::Viewport::resizeGL(int width, int height) {
	glViewport(0, 0, width, height);

	glMatrixMode(GL_MODELVIEW);
}

void Doodle::Viewport::paintEvent(QPaintEvent* event) {
	QPainter p(this);
	saveGLState();

	QPainter fboPainter(_fbo);
	_painter->paint(&fboPainter, event);
	if(_calibration) {
		fboPainter.save();
		QPen pen(Qt::white);
		pen.setWidth(2);
		fboPainter.setPen(pen);
		// horizontals
		fboPainter.drawLine(0, 0, 0, _painter->height());
		fboPainter.drawLine(_painter->height()/4, 0, _painter->height()/4, _painter->height());
		fboPainter.drawLine(_painter->height()/2, 0, _painter->height()/2, _painter->height());
		fboPainter.drawLine(3*_painter->height()/4, 0, 3*_painter->height()/4, _painter->height());
		fboPainter.drawLine(_painter->height(), 0, _painter->height(), _painter->height());
		// verticals
		fboPainter.drawLine(0, 0, _painter->width(), 0);
		fboPainter.drawLine(0, _painter->width()/4, _painter->width(), _painter->width()/4);
		fboPainter.drawLine(0, _painter->width()/2, _painter->width(), _painter->width()/2);
		fboPainter.drawLine(0, 3*_painter->width()/4, _painter->width(), 3*_painter->width()/4);
		fboPainter.drawLine(0, _painter->width(), _painter->width(), _painter->width());
		// diagonals
		fboPainter.drawLine(0,0,_painter->height(),_painter->height());
		fboPainter.drawLine(_painter->height(),0,0,_painter->height());
		fboPainter.restore();
	}
	fboPainter.end();

	qglClearColor(_painter->background().color());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float aspect = (float)width()/height();
	gluPerspective(15, aspect, 4, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, width(), height());
	glBindTexture(GL_TEXTURE_2D, _fbo->texture());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);

//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslated(_xPosition, _yPosition, _zPosition - 6.5);
	glRotated(_xRotation, 1.0, 0.0, 0.0);
	glRotated(_zRotation, 0.0, 0.0, 1.0);
	glScaled(_xScale, _yScale, 1.0);
	glCallList(_object);

	restoreGLState();
	if(_calibration) {
		p.setPen(Qt::white);
		QFont font("Helvetica Neue", 16, QFont::Bold);
		p.setFont(font);
		QFontMetrics fm(p.font());
		QString s = QString("Calibration");
		p.drawText((width() - fm.width(s))/2, fm.lineSpacing()*7, s);
		s = QString("scale: (%1, %2)").arg(_xScale).arg(_yScale);
		p.drawText((width() - fm.width(s))/2, fm.lineSpacing()*8, s);
		s = QString("rotation: (%1, %2)").arg(_xRotation).arg(_zRotation);
		p.drawText((width() - fm.width(s))/2, fm.lineSpacing()*9, s);
		s = QString("translation: (%1, %2, %3)").arg(_xPosition).arg(_yPosition).arg(_zPosition);
		p.drawText((width() - fm.width(s))/2, fm.lineSpacing()*10, s);
	}
	p.end();
}

void Doodle::Viewport::animate() {
	update();
}

void Doodle::Viewport::keyPressEvent(QKeyEvent* event) {
	int key = event->key();
	if(key == Qt::Key_T) {
		_painter->toggleOnTable();
	} else if(key == Qt::Key_F) {
		if(_fullscreen) {
			showNormal();
		} else {
			showFullScreen();
		}
		_fullscreen = !_fullscreen;
	} else if(key == Qt::Key_C) {
		_calibration = !_calibration;
	} else if(key == Qt::Key_Shift && _calibration) {
		_hasShift = true;
	} else if(key == Qt::Key_W && _calibration) {
		_xScale += _hasShift? 0.001 : 0.01;
		_painter->setScale(_xScale);
	} else if(key == Qt::Key_Q && _calibration) {
		_xScale -= _hasShift? 0.001 : 0.01;
		_painter->setScale(_xScale);
	} else if(key == Qt::Key_S && _calibration) {
		_yScale += _hasShift? 0.001 : 0.01;
	} else if(key == Qt::Key_X && _calibration) {
		_yScale -= _hasShift? 0.001 : 0.01;
	} else if(key == Qt::Key_A && _calibration) {
		_xRotation += _hasShift? 0.1 : 1.0;
	} else if(key == Qt::Key_Z && _calibration) {
		_xRotation -= _hasShift? 0.1 : 1.0;
	} else if(key == Qt::Key_R && _calibration) {
		_zRotation += _hasShift? 0.1 : 1.0;
	} else if(key == Qt::Key_E && _calibration) {
		_zRotation -= _hasShift? 0.1 : 1.0;
	} else if(key == Qt::Key_Plus && _calibration) {
		_zPosition += _hasShift? 0.01 : 0.1;
	} else if(key == Qt::Key_Minus && _calibration) {
		_zPosition -= _hasShift? 0.01 : 0.1;
	} else if(key == Qt::Key_Down && _calibration) {
		_yPosition += _hasShift? 0.001 : 0.01;
	} else if(key == Qt::Key_Up && _calibration) {
		_yPosition -= _hasShift? 0.001 : 0.01;
	} else if(key == Qt::Key_Right && _calibration) {
		_xPosition += _hasShift? 0.001 : 0.01;
	} else if(key == Qt::Key_Left && _calibration) {
		_xPosition -= _hasShift? 0.001 : 0.01;
	}
}

void Doodle::Viewport::keyReleaseEvent(QKeyEvent* event) {
	int key = event->key();
	if(key == Qt::Key_Shift && _calibration) {
		_hasShift = false;
	}
}

void Doodle::Viewport::saveGLState() {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
}

void Doodle::Viewport::restoreGLState() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}
