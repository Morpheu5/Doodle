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
#include <QPaintDevice>
#include <QPaintEngine>
#include <QMatrix>
#include "SequencerWidget.h"

#include <QDebug>

Doodle::SequencerWidget::SequencerWidget() : Widget(0) {
	makeVisual();
}

Doodle::SequencerWidget::SequencerWidget(Widget* parent = 0) : Widget(parent) {
	makeVisual();
}

void Doodle::SequencerWidget::makeVisual() {
	_sides = 8;
	int i;
	for(i = 0; i < _sides; i++) {
		QPointF q;
		q.setX(cos(2*i*M_PI/_sides));
		q.setY(sin(2*i*M_PI/_sides));
		_shape << q;
	}
	_shape << _shape[0];

	_size = 45;
}

void Doodle::SequencerWidget::render(QPainter* painter) {
	painter->save();
	int width = painter->device()->width();
	int height = painter->device()->height();
	QPointF newPos(position().x()*width, position().y()*height);
	QRadialGradient gradient(newPos, _scale*(10+_size), newPos);
	QColor matte;
	matte.setRgb(255,255,255,255);
	gradient.setColorAt(0, matte);
	matte.setAlpha(0);
	gradient.setColorAt(1, matte);
	QBrush brush(gradient);
	painter->setPen(Qt::NoPen);
	painter->setBrush(brush);
	QMatrix m;
	m.translate(newPos.x(), newPos.y());
	m.rotate(180*angle()/M_PI);
	m.scale(_scale*_size, _scale*_size);
	QPolygonF shape = m.map(_shape);
	painter->drawPolygon(shape);

	QPen pen(Qt::white);
	pen.setWidth(10);
	painter->setPen(pen);

	int i;
	for(i = 0; i < _sequenceList->size(); i++) {
		m.reset();
		m.rotate(180*(angle()-(M_PI/_sides)+(i*2*M_PI/_sides))/M_PI);
		QPointF w = m.map(QPointF(0.075, 0.0));
		SequenceButtonWidget* sbw = _sequenceList->at(i);
		sbw->setPosition(w + position());
		sbw->setAngle(angle() + (i*2 - 1)*M_PI/_sides + 3*M_PI_2);
	}

	painter->restore();
}
