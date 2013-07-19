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
#include <DoodleSDK/includes/Widget.h>
#include "MultiTap.h"

#include <QDebug>

void Doodle::MultiTap::init(QString, QHash<QString, QString>) {
	return;
}

void Doodle::MultiTap::recognize() {
	QStringList resultPacket;
	int n = 0;
	QList<QPointF> taps;
	Group g = _sGroups.takeFirst();
	QList<long> widgets;
	foreach(Trace t, g.finalizedTraces()) {
		foreach(Stroke s, t.strokes()) {
			long interval = (s.last().currentTime() - s.first().currentTime()).getTotalMilliseconds();
			if(interval < 1500) {
				QPointF d = s.first().position() - s.last().position();
				if(d.x()*d.x()+d.y()*d.y() < 0.01*0.01) {
					n++;
					taps.append((s.first().position()+s.last().position())/2);
					if(t.isOnWidget()) {
						if(!widgets.contains(t.widget()->id())) {
							widgets.append(t.widget()->id());
						}
					}
				} else {
					return;
				}
			} else {
				return;
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
	resultPacket << "MultiTap" << objects.join(",") << widgetStr.join(",") << QString::number(1 + taps.size()*2) << QString::number(n);
	foreach(QPointF p, taps) {
		resultPacket.append(QString("%1:%2").arg(p.x()).arg(p.y()).split(":"));
	}
	emit result(resultPacket);
}

Q_EXPORT_PLUGIN2(MultiTap, Doodle::MultiTap);
