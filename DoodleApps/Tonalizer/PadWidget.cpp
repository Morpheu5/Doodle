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
#include "PadWidget.h"

#include <QDebug>

Doodle::PadWidget::PadWidget() : Widget(0) {
	_shape << QPointF(-0.5, -0.5);
	_shape << QPointF( 0.5, -0.5);
	_shape << QPointF( 0.5,  0.5);
	_shape << QPointF(-0.5,  0.5);
	_shape << QPointF(-0.5, -0.5);

	_size = 100;
}

Doodle::PadWidget::PadWidget(Widget* parent = 0) : Widget(parent) {
	_shape << QPointF(-0.5, -0.5);
	_shape << QPointF( 0.5, -0.5);
	_shape << QPointF( 0.5,  0.5);
	_shape << QPointF(-0.5,  0.5);
	_shape << QPointF(-0.5, -0.5);

	_size = 100;
}

void Doodle::PadWidget::render(QPainter* painter) {
	painter->save();
	int width = painter->device()->width();
	int height = painter->device()->height();
	QMatrix m;
	m.translate(_position.x()*width, _position.y()*height);
	m.rotate(180*_angle/M_PI);
	m.scale(_scale*_size, _scale*_size);
	QPen pen(Qt::white);
	pen.setCosmetic(false);
	pen.setWidth(2);
	painter->setPen(pen);
	QBrush brush(QColor::fromHsv(200,255,255,63), Qt::SolidPattern);
	painter->setBrush(brush);
	QPolygonF shape = m.map(_shape);
	painter->drawPolygon(shape);
	pen.setWidth(10);
	pen.setCapStyle(Qt::FlatCap);
	QLineF line(m.map(QPointF(-0.5, 0.5)), m.map(QPointF(0.5, 0.5)));
	painter->setPen(pen);
	painter->drawLine(line);
	painter->restore();
}
