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
#include "TangibleWidget.h"

#include <QDebug>

Doodle::TangibleWidget::TangibleWidget() : Widget(0) {
	_shape << QPointF( 0.0, -0.5);
	_shape << QPointF( 0.25, 0.5);
	_shape << QPointF(-0.25, 0.5);
	_shape << QPointF( 0.0, -0.5);

	_size = 45;
}

Doodle::TangibleWidget::TangibleWidget(Widget* parent = 0) : Widget(parent) {
	_shape << QPointF( 0.0, -0.5);
	_shape << QPointF( 0.25, 0.5);
	_shape << QPointF(-0.25, 0.5);
	_shape << QPointF( 0.0, -0.5);

	_size = 45;
}

void Doodle::TangibleWidget::render(QPainter* painter) {
	painter->save();
	painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
	QPen pen(Qt::white);
	pen.setCosmetic(true);
	painter->setPen(pen);
	QMatrix m;
	m.scale(_size, _size);
	QPolygonF shape = m.map(_shape);
	painter->drawPolygon(shape);
	painter->restore();
}
