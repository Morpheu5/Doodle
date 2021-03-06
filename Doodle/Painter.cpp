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

#include <QHash>
#include <QPointF>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>

#include <QMutexLocker>

#include <DoodleSDK/includes/Tangible.h>
#include <DoodleSDK/includes/Trace.h>
#include <DoodleSDK/includes/Widget.h>

#include "TangibleWidget.h"
#include "Painter.h"

#include <cmath>
#include <list>

Doodle::Painter::Painter() {
	_background = QBrush(QColor::fromHsv(210,127,63,255));
	_pen.setColor(Qt::white);
	_pen.setWidth(2);
	_fingerBrush.setColor(Qt::yellow);
	_fingerBrush.setStyle(Qt::SolidPattern);
	_fingerPen.setBrush(_fingerBrush);
	_fingerPen.setWidth(5);

	_hue = 210.0/360.0;
	_hueStep = 1.0/450.0;

	_onTable = false;
}

Doodle::Painter::~Painter() {

}

void Doodle::Painter::paint(QPainter* painter, QPaintEvent* event) {
	int i,k;
	painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
	// "She changes every time you look" [Steven Wilson]
	QColor bgColor = QColor::fromHsvF(_hue, 63.0/255, 63.0/255, 255.0/255);
	_background.setColor(bgColor);
	_hue += _hueStep;
	if(_hue >= 1.0) {
		_hue -= 1.0;
	}
	painter->fillRect(0, 0, _width, _height, _background);
	painter->save();

	QPen trace(Qt::green);
	trace.setWidth(5);
	trace.setCapStyle(Qt::RoundCap);
	trace.setJoinStyle(Qt::RoundJoin);

	painter->setPen(_pen);
	painter->drawEllipse((_width-_height)/2, 0, _height, _height);

	QMutexLocker locker(&paintMutex);

	if(_widgets->size() > 0) {
		for(i = 0; i < _widgets->size(); i++) {
			Widget* w = _widgets->at(i);
			if(!w->visible()) {
				continue;
			}
			painter->save();
			if(_onTable) {
				painter->scale(1.3333, 1.0);
				painter->translate(-_width*0.125,0);
			}
			w->render(painter);
			painter->restore();

		}
	}

	if(_traces->size() > 0) {
		painter->save();
		if(_onTable) {
			painter->scale(1.3333, 1.0);
			painter->translate(-_width*0.125,0);
		}
		QHash<long, Trace*>::ConstIterator j;
		for(j = _traces->begin(); j != _traces->end(); ++j) {
			switch((*j)->state()) {
			case Trace::ADDED:
			case Trace::UPDATED:
			case Trace::RESUMED:
				trace.setColor(QColor::fromRgb(255,255,255,127));
				break;
			case Trace::REMOVED:
				trace.setColor(QColor::fromRgb(255,255,255,63));
				break;
			case Trace::DEAD:
				trace.setColor(Qt::darkRed);
				break;
			}

			Cursor dot;
			try {
				dot = (*j)->lastPoint();
			} catch(std::runtime_error e) {
				continue;
			}
			QPen f;
			QColor fc = QColor::fromHsv((60*(*j)->groupId())%360, 255, 255, 127);
			QBrush b;
			b.setColor(fc);
			b.setStyle(Qt::SolidPattern);
			painter->setPen(Qt::NoPen);
			painter->setBrush(b);
			int fingerSize = (int)round(*_traceSpaceGap*_width);
			int groupSize = (int)round(*_groupSpaceGap*2*_width);
			painter->drawEllipse(dot.x()*_width-fingerSize/2, dot.y()*_height-fingerSize/2, fingerSize, fingerSize);
			QPointF newPos(dot.x()*_width, dot.y()*_height);
			QRadialGradient gradient(newPos, groupSize/2, newPos);
			gradient.setColorAt(0, QColor::fromRgb(255,255,255,30));
			gradient.setColorAt(1, QColor::fromRgb(255,255,255,0));
			b = QBrush(gradient);
			painter->setBrush(b);
			painter->drawEllipse(newPos, groupSize/2, groupSize/2);
			const QList<Stroke>& sl = ((*j)->strokes());
			painter->setPen(trace);
			QVector<QPointF> polyline;
			for(i = 0; i < sl.size(); i++) {
				const Stroke s = sl.at(i);
				for(k = 0; k < s.size(); k++) {
					QPointF u(s[k].x()*_width, s[k].y()*_height);
					polyline.append(u);
				}
			}
			painter->drawPolyline(polyline.data(), polyline.size());
		}
		painter->restore();
	}
	painter->restore();
}
