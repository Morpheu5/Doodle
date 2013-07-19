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
#include <limits>

#include <QRectF>
#include <QPolygonF>
#include <QList>
#include <QLineF>
#include <DoodleSDK/includes/Widget.h>
#include "MusicTouch.h"

#include <QDebug>

void Doodle::MusicTouch::init(QString rcPath, QHash<QString, QString> params) {
	return;
}

void Doodle::MusicTouch::recognize() {
	QStringList resultPacket;
	int n = 0;
	QList<QPointF> taps;
	Group g = _sGroups.takeFirst();
	QList<unsigned long> widgets;
	QList<QPointF> triggers;
	if(!g.finalizedTraces().first().isOnWidget()) {
		return;
	} else {
		widgets.append(g.finalizedTraces().first().widget()->id());
	}
	foreach(Trace t, g.finalizedTraces()) {
		if(!t.isOnWidget()) {
			continue;
		}
		float angle = t.widget()->angle();
		float tangent = tan(angle);
		triggers.append(t.firstPoint().position());
		foreach(Stroke s, t.strokes()) {
			int l = s.size() - 1;
			int i;
			QList<QPointF> cluster;
			for(i = 0; i < l; i++) {
				// TODO: motion speed.
				float diff = (s[i+1].y() - s[i].y()) / (s[i+1].x() - s[i].x());
				if(abs(diff - tangent) <= 1.0) {
					cluster.append((s[i].position() + s[i+1].position())/2);
				} else {
					// Clusterize and clear
					QPointF t(0,0);
					foreach(QPointF p, cluster) {
						t += p;
					}
					t /= cluster.size();
					if(!std::isnan(t.x()) && !std::isnan(t.y()) && !std::isinf(t.x()) && !std::isinf(t.y())) {
						triggers.append(t);
					}
					cluster.clear();
				}
			}
		}
		triggers.append(t.lastPoint().position());
	}
	QStringList objects;
	foreach(long l, g.objects()) {
		objects.append(QString::number(l));
	}
	QStringList widgetStr;
	foreach(long l, widgets) {
		widgetStr.append(QString::number(l));
	}
	resultPacket << "MusicTouch" << objects.join(",") << widgetStr.join(",") << QString::number(1 + triggers.size()*2) << QString::number(triggers.size());
	foreach(QPointF tp, triggers) {
		resultPacket.append(QString("%1:%2").arg(tp.x()).arg(tp.y()).split(":"));
	}
	emit result(resultPacket);
}

Q_EXPORT_PLUGIN2(MusicTouch, Doodle::MusicTouch);
