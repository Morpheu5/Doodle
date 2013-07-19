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
#include <DoodleSDK/includes/Cursor.h>
#include <DoodleSDK/includes/Trace.h>
#include <DoodleSDK/includes/Widget.h>
#include "Pinch.h"

#include <QDebug>

void Doodle::Pinch::init(QString, QHash<QString, QString>) {
	return;
}

void Doodle::Pinch::recognize() {
	// FIXME: There's a lot to fix here...
	// FIXME: Probably endemic rotation fluctuation even when cursors are moving together.
	QStringList resultPacket;
	foreach(Group* group, _pGroups) {
		if(group->progressiveTraces().size() != 2) {
			continue;
		}
		Trace* firstTrace = group->progressiveTraces()[0];
		if(!firstTrace->isOnWidget()) {
			continue;
		}
		Widget* firstParent = firstTrace->widget();
		while(firstParent->parent() != NULL) {
			Widget* w = firstParent->parent();
			firstParent = w;
		}

		Trace* secondTrace = group->progressiveTraces()[1];
		if(!secondTrace->isOnWidget()) {
			return;
		}
		Widget* secondParent = secondTrace->widget();
		while(secondParent->parent() != NULL) {
			Widget* w = secondParent->parent();
			secondParent = w;
		}
//		if(secondTrace->widget()->id() != firstTrace->widget()->id()) {
//			return;
//		}
		if(firstParent->id() != secondParent->id()) {
			return;
		}
		Trace::State fs = firstTrace->state();
		Trace::State ss = secondTrace->state();
		if(fs != Trace::ADDED && fs != Trace::UPDATED) {
			return;
		}

		resultPacket.clear();
		// The empty string accounts for the objects
		resultPacket << "Pinch" << "" << QString::number(firstParent->id()) << "6";
		Cursor fp = firstTrace->lastPoint();
		Cursor sp = secondTrace->lastPoint();
		QPointF center((fp.x()+sp.x())/2, (fp.y()+sp.y())/2);
		float distance = sqrt(fp.squareDistance(sp));
		float dx = fp.x() - sp.x();
		float dy = fp.y() - sp.y();
		float angle = asin(dx/distance)+M_PI_2;
		if(dy < 0) {
			angle = 2*M_PI - angle;
		}
		if(ss == Trace::ADDED) {
			_lastPosition = center;
			_lastAngle = angle;
			_lastZoom = distance;
			return;
		}
		QPointF displacement = (center - _lastPosition)/2;
		_lastPosition += displacement;
		float rotation = angle - _lastAngle;
		_lastAngle += rotation;
		float zoom = distance - _lastZoom;
		_lastZoom += zoom;

		// Format: widget_id:group_id:x_displacement:y_displacement:zoom:rotation
		resultPacket << QString("%1:%2:%3:%4:%5:%6").arg(firstParent->id()).arg(group->id()).arg(displacement.x()).arg(displacement.y()).arg(8*zoom).arg(rotation).split(':');
		emit result(resultPacket);
	}
}

Q_EXPORT_PLUGIN2(Pinch, Doodle::Pinch);
