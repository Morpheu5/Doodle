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

#include <QPainter>
#include <QMatrix>
#include <QPolygonF>
#include <QFontMetrics>
#include <DoodleSDK/widgets/CheckButtonWidget.h>
#include "ChordWidget.h"

#include "Chord.h"

#include <QDebug>

Doodle::ChordWidget::ChordWidget() : Widget(0) {
	makeVisual();
	_active = true;
}

Doodle::ChordWidget::ChordWidget(Widget* parent = 0) : Widget(parent) {
	makeVisual();
	_active = true;
}

void Doodle::ChordWidget::makeVisual() {
	_shape << QPointF(-0.56, -0.25);
	_shape << QPointF( 0.56, -0.25);
	_shape << QPointF( 0.56,  0.35);
	_shape << QPointF(-0.56,  0.35);
	_shape << QPointF(-0.56, -0.25);

	_size = 100;
	_font = QFont("Helvetica Neue", 14);
	_font.setStretch(QFont::Condensed);
	_font.setWeight(QFont::Bold);

	_sharp = new PushButtonWidget(this);
	_sharp->setPinchable(false);
	_sharp->setText("#");
	_sharp->setSize(15, 15);
	_sharp->setPosition(QPointF(0.09, -0.0125));

	_flat = new PushButtonWidget(this);
	_flat->setPinchable(false);
	_flat->setText("b");
	_flat->setSize(15, 15);
	// FIXME: This is spooky.
	_flat->setPosition(QPointF(0.09, 0.0250));

	_minor = new CheckButtonWidget(this);
	_minor->setPinchable(false);
	_minor->setText("m");
	_minor->setSize(15, 15);
	_minor->setPosition(QPointF(0.0515, -0.0500));

	_sevenMaj = new CheckButtonWidget(this);
	_sevenMaj->setPinchable(false);
	_sevenMaj->setText("M");
	_sevenMaj->setSize(15, 15);
	_sevenMaj->setPosition(QPointF(0.09, -0.0500));

	_four = new CheckButtonWidget(this);
	_four->setPinchable(false);
	_four->setText("4");
	_four->setSize(15,15);
	_four->setPosition(QPointF(-0.0515, -0.0500));

	_six = new CheckButtonWidget(this);
	_six->setPinchable(false);
	_six->setText("6");
	_six->setSize(15,15);
	_six->setPosition(QPointF(-0.09, -0.0500));

	_seven = new CheckButtonWidget(this);
	_seven->setPinchable(false);
	_seven->setText("7");
	_seven->setSize(15,15);
	_seven->setPosition(QPointF(-0.09, -0.0125));

	_nine = new CheckButtonWidget(this);
	_nine->setPinchable(false);
	_nine->setText("9");
	_nine->setSize(15,15);
	_nine->setPosition(QPointF(-0.09, 0.0250));
}

void Doodle::ChordWidget::render(QPainter* painter) {
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
	pen.setCosmetic(true);
	pen.setWidth(2);
	painter->setPen(pen);
	QBrush brush(QColor::fromRgb(255,255,255,63));
	if(_active) {
		painter->setBrush(brush);
	}
	painter->drawPolygon(shape);
	painter->setFont(_font);
	QString notation = _chord->toNotation();
	QFontMetrics fm(_font);
	m.reset(); m.rotate(180*_angle/M_PI);
	QPointF d = m.map(QPointF(-(float)fm.width(notation)/2, (float)fm.height()/4 + 5));

	QPointF pos((_position).x()*width + d.x(), (_position).y()*height + d.y());
	painter->translate(pos.x(), pos.y());
	painter->rotate(180*_angle/M_PI);
	painter->drawText(0, 0, notation);
	painter->restore();
}

void Doodle::ChordWidget::updateVisual() {
	QList<int> notes = _chord->notes();
	if(notes.contains(14)) {
		_nine->setChecked(true);
	} else {
		_nine->setChecked(false);
	}

	if(notes.contains(3)) {
		_minor->setChecked(true);
		_four->setChecked(false);
	} else if(notes.contains(4)) {
		_minor->setChecked(false);
		_four->setChecked(false);
	}

	if(notes.contains(5)) {
		_minor->setChecked(false);
		_four->setChecked(true);
	}

	if(notes.contains(9)) {
		_six->setChecked(true);
	} else {
		_six->setChecked(false);
	}

	if(!notes.contains(10) && !notes.contains(11)) {
		_seven->setChecked(false);
		_sevenMaj->setChecked(false);
	} else if(notes.contains(10) && !notes.contains(11)) {
		_seven->setChecked(true);
		_sevenMaj->setChecked(false);
	} else if(!notes.contains(10) && notes.contains(11)) {
		_seven->setChecked(false);
		_sevenMaj->setChecked(true);
	} else {
		// This should never happen.
		_seven->setChecked(true);
		_sevenMaj->setChecked(true);
	}
}

void Doodle::ChordWidget::tap(int n) {
	emit tapped(n);
}

void Doodle::ChordWidget::setActive(bool active = true) {
	// TODO: This only allows one chord at a time to be modified.
	_active = active;
	if(_active) {
		foreach(Widget* w, _children) {
			w->setVisible(true);
		}
	} else {
		foreach(Widget* w, _children) {
			w->setVisible(false);
		}
	}
}
