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

#include <QRectF>
#include <QPolygonF>
#include <QList>
#include <QLineF>
#include <DoodleSDK/includes/Widget.h>
#include "StraightStroke.h"

#include <QDebug>

void Doodle::StraightStroke::init(QString, QHash<QString, QString>) {
	return;
}

void Doodle::StraightStroke::recognize() {
	QStringList resultPacket;
	int n = 0;
	QList<QPointF> taps;
	Group g = _sGroups.takeFirst();
	QList<unsigned long> widgets;
	QList<QLineF> lines;
	foreach(Trace t, g.finalizedTraces()) {
		QPointF first = t.strokes().first().first().position();
		QPointF last = t.strokes().last().last().position();
		QPointF diff = first - last;
		double length = sqrt(diff.x()*diff.x() + diff.y()*diff.y());
		if(length < 0.01) {
			return;
		}
		QLineF line(first, last);
		foreach(Stroke s, t.strokes()) {
			int i;
			for(i = 0; i < s.size(); i++) {
//				QLineF l(first, s[i].position());
//				double angle = line.angleTo(l);
//				if(angle >= 10 && angle <= 350) {
//					return;
//				}
				if(pointToSegmentDistance(s[i].position(), line) > 0.01) {
					return;
				}
			}
		}
		lines.append(line);
		if(t.isOnWidget()) {
			if(!widgets.contains(t.widget()->id())) {
				widgets.append(t.widget()->id());
			}
		}
	}
	QStringList objects;
	foreach(long l, g.objects()) {
		objects.append(QString::number(l));
	}
	QStringList widgetStr;
	foreach(long l, widgets) {
		widgetStr.append(QString::number(l));
	}
	resultPacket << "StraightStroke" << objects.join(",") << widgetStr.join(",") << QString::number(1 + lines.size()*5) << QString::number(lines.size());
	foreach(QLineF l, lines) {
		resultPacket.append(QString("%1:%2:%3:%4:%5").arg(l.length()).arg(l.x1()).arg(l.y1()).arg(l.x2()).arg(l.y2()).split(":"));
	}
	emit result(resultPacket);
}

double Doodle::StraightStroke::pointToSegmentDistance(QPointF p, QLineF line) {
	if(line.length() == 0) {
		QPointF d = p - line.p1();
		return sqrt(d.x()*d.x() + d.y()*d.y());
	} else {
		QPointF d = line.p2() - line.p1();
		double r = ((p.x() - line.p1().x())*d.x() + (p.y() - line.p1().y())*d.y()) / (d.x()*d.x() + d.y()*d.y());
		if(r < 0) {
			QPointF e = p - line.p1();
			return sqrt(e.x()*e.x() + e.y()*e.y());
		} else if(r > 1) {
			QPointF e = p - line.p2();
			return sqrt(e.x()*e.x() + e.y()*e.y());
		} else {
			QPointF e = p - QPointF((1-r)*line.p1().x() + r*line.p2().x(), (1-r)*line.p1().y() + r*line.p2().y());
			return sqrt(e.x()*e.x() + e.y()*e.y());
		}
	}
}

Q_EXPORT_PLUGIN2(StraightStroke, Doodle::StraightStroke);
