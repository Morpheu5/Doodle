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
#include <QFont>
#include "CheckButtonWidget.h"

#include <QDebug>

Doodle::CheckButtonWidget::CheckButtonWidget() : Widget(0) {
	_shape << QPointF(-0.5, -0.5);
	_shape << QPointF( 0.5, -0.5);
	_shape << QPointF( 0.5,  0.5);
	_shape << QPointF(-0.5,  0.5);
	_shape << QPointF(-0.5, -0.5);
	_size = 50;

	_font = QFont("Helvetica Neue", 14);
	_checked = false;
}

Doodle::CheckButtonWidget::CheckButtonWidget(Widget* parent = 0) : Widget(parent) {
	_shape << QPointF(-0.5, -0.5);
	_shape << QPointF( 0.5, -0.5);
	_shape << QPointF( 0.5,  0.5);
	_shape << QPointF(-0.5,  0.5);
	_shape << QPointF(-0.5, -0.5);
	_size = 50;

	_font = QFont("Helvetica Neue", 14);
	_checked = false;
}

void Doodle::CheckButtonWidget::setSize(float width, float height) {
	float nw = 0.5;
	float nh = height/(2*width);
	_shape[0] = QPointF(-nw, -nh);
	_shape[1] = QPointF( nw, -nh);
	_shape[2] = QPointF( nw,  nh);
	_shape[3] = QPointF(-nw,  nh);
	_shape[4] = QPointF(-nw, -nh);
	_size = 2*width;
}

void Doodle::CheckButtonWidget::setText(QString text) {
	_text = text.trimmed();
}

void Doodle::CheckButtonWidget::render(QPainter* painter) {
	painter->save();
	int width = painter->device()->width();
	int height = painter->device()->height();
	QMatrix m;
	m.translate(_position.x()*width, _position.y()*height);
	m.rotate(180*_angle/M_PI);
	m.scale(_scale*_size, _scale*_size);
	QPolygonF shape = m.map(_shape);
	QRectF shapeBound = shape.boundingRect();
	QBrush brush;
	if(_checked) {
		brush = QBrush(QColor::fromRgb(255,255,255,63));
	} else {
		brush = QBrush(QColor::fromRgb(255,255,255,0));
	}
	QPen pen(Qt::white);
	pen.setWidth(2);
	painter->setBrush(brush);
	painter->setPen(pen);
	painter->drawPolygon(shape);
	painter->setFont(_font);
	QFontMetrics fm(_font);
	m.reset(); m.rotate(180*_angle/M_PI);
	QPointF d = m.map(QPointF(-(float)fm.width(text())/2, (float)fm.height()/4));

	QPointF pos((_position).x()*width + d.x(), (_position).y()*height + d.y());
	painter->translate(pos.x(), pos.y());
	painter->rotate(180*_angle/M_PI);
	painter->drawText(0, 0, text());
	painter->restore();
}

void Doodle::CheckButtonWidget::tap(int n) {
	int m = n%2;
	if(m != 0) {
		_checked = !_checked;
		if(_checked) {
			emit checked();
		} else {
			emit unchecked();
		}
	}
//	emit tapped(n);
}

void Doodle::CheckButtonWidget::check() {
	if(!_checked) {
		_checked = true;
		emit checked();
	}
}

void Doodle::CheckButtonWidget::uncheck() {
	if(_checked) {
		_checked = false;
		emit unchecked();
	}
}
