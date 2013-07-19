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

#include <QMatrix>
#include <QPainter>
#include "Widget.h"
#include <QDebug>

unsigned long Doodle::Widget::s_id = 0;

Doodle::Widget::Widget(Widget* parent = 0)
	: _parent(parent)
{
	_id = s_id++;
	if(_parent != 0) {
		_parent->addChild(this);
	}
	_flags = RenderChildren;
	_application = NULL;
	_scale = 1.0;
	_angle = 0.0;
	_pinchable = true;
	_visible = true;

	// makeVisual();
}

Doodle::Widget::~Widget() {
	QList<Widget*>::const_iterator wi;
	for(wi = _children.begin(); wi != _children.end(); ++wi) {
		if((*wi) != 0) {
			// FIXME: Dangling pointers ahead.
			delete (*wi);
		}
	}
}

void Doodle::Widget::setPosition(QPointF pos) {
	QPointF disp = pos - _position;
	_position = pos;
	foreach(Widget* w, _children) {
		w->transform(disp, 0, 0);
	}
}

void Doodle::Widget::setAngle(float degree) {
	_angle = degree;
}

void Doodle::Widget::setScale(float scale) {
	if(pinchable()) {
		_scale = scale;
	} else {
		_scale = 1.0;
	}
}

bool Doodle::Widget::contains(const QPointF& p) const {
	if(!visible()) {
		return false;
	}
	QMatrix m;
	// FIXME
	m.translate(position().x()*800, position().y()*800);
	m.scale(_scale*_size, _scale*_size);
	m.rotate(180*angle()/M_PI);
	QPolygonF shape = m.map(_shape);
	m.reset();
	m.scale(800, 800);
	QPointF q = m.map(p);
	return shape.containsPoint(q, Qt::OddEvenFill);
}

void Doodle::Widget::addChild(Widget* child) {
	if(!_children.contains(child)) {
		_children.append(child);
		child->setParent(this);
		emit childAdded(child);
	}
}

void Doodle::Widget::removeChild(Widget* child) {
	if(_children.contains(child)) {
		_children.removeAll(child);
	}
}

void Doodle::Widget::transform(QPointF translation, float zoom, float rotation) {
	QPointF parentOldPosition = position();
	if(pinchable()) {
		_scale += zoom;
	}
	_position += translation;
	_angle += rotation;
	foreach(Widget* w, _children) {
		QMatrix m;
		m.rotate(180*rotation/M_PI);
		QPointF childOldPosition = w->position() - parentOldPosition;
		QPointF childNewPosition = m.map(childOldPosition);
		if(pinchable()) {
			// FIXME: zoom*(translation + ... ) ?
			w->transform(translation + (childNewPosition - childOldPosition), zoom, rotation);
		} else {
			w->transform(translation + (childNewPosition - childOldPosition), 0, rotation);
		}
	}
}

void Doodle::Widget::renderChildren(QPainter* painter) {
//	if(_flags & RenderChildren) {
//		foreach(Widget* w, _children) {
//			painter->save();
//			painter->translate(w->position().x()*_scale, w->position().y()*_scale);
//			w->render(painter);
//			painter->restore();
//		}
//	}
}

void Doodle::Widget::tap(int n) {
	emit tapped(n);
}
