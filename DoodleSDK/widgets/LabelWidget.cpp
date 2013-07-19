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
#include "LabelWidget.h"

#include <QDebug>

Doodle::LabelWidget::LabelWidget() : Widget(0) {
	_shape << QPointF(-0.5, -0.5);
	_shape << QPointF( 0.5, -0.5);
	_shape << QPointF( 0.5,  0.5);
	_shape << QPointF(-0.5,  0.5);
	_shape << QPointF(-0.5, -0.5);
	_size = 50;

	_font = QFont("Helvetica Neue", 14);
}

Doodle::LabelWidget::LabelWidget(Widget* parent = 0) : Widget(parent) {
	_shape << QPointF(-0.5, -0.5);
	_shape << QPointF( 0.5, -0.5);
	_shape << QPointF( 0.5,  0.5);
	_shape << QPointF(-0.5,  0.5);
	_shape << QPointF(-0.5, -0.5);
	_size = 50;

	_font = QFont("Helvetica Neue", 14);
}

void Doodle::LabelWidget::setText(QString text) {
	_text = text.trimmed();
	QFontMetrics fm(_font);
	float width = fm.width(_text);
	float height = fm.height();
	float max = width > height? width : height;
	float nw = 0.5;
	float nh = width/(2*height);
	_shape[0] = QPointF(-nw, -nh);
	_shape[1] = QPointF( nw, -nh);
	_shape[2] = QPointF( nw,  nh);
	_shape[3] = QPointF(-nw,  nh);
	_shape[4] = QPointF(-nw, -nh);
	_size = width;
}

void Doodle::LabelWidget::render(QPainter* painter) {
	painter->save();
	int width = painter->device()->width();
	int height = painter->device()->height();
	QMatrix m;
	m.translate(_position.x()*width, _position.y()*height);
	m.rotate(180*_angle/M_PI);
	m.scale(_scale*_size, _scale*_size);
	QPolygonF shape = m.map(_shape);
	QRectF shapeBound = shape.boundingRect();
	QBrush brush(QColor::fromRgb(255,255,255,63));
	QPen pen(Qt::white);
	pen.setWidth(2);
	painter->setBrush(brush);
	painter->setPen(pen);
	painter->drawPolygon(shape);
	painter->setFont(_font);
	QFontMetrics fm(_font);
	m.reset(); m.rotate(180*_angle/M_PI);
	QPointF d = m.map(QPointF(-(float)fm.width(_text)/2, (float)fm.height()/4));

	QPointF pos((_position).x()*width + d.x(), (_position).y()*height + d.y());
	painter->translate(pos.x(), pos.y());
	painter->rotate(180*_angle/M_PI);
	painter->drawText(0, 0, _text);
	painter->restore();
}
