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

#include "TuioProxy.h"

Doodle::TuioProxy::TuioProxy(int port) {
	_tuioListener = new TuioListener(port, this);
}

void Doodle::TuioProxy::addTuioObject(TUIO::TuioObject* tobj) {
	emit(addTangible(
			tobj->getSessionID(),
			tobj->getSymbolID(),
			tobj->getX()*1.3333 - 0.1666,
			tobj->getY(),
			tobj->getTuioTime(),
			tobj->getXSpeed(),
			tobj->getYSpeed(),
			tobj->getMotionSpeed(),
			tobj->getMotionAccel(),
			tobj->getAngle(),
			tobj->getRotationSpeed(),
			tobj->getRotationAccel(),
			tobj->getTuioState()
			));
}

void Doodle::TuioProxy::updateTuioObject(TUIO::TuioObject* tobj) {
	emit(updateTangible(
			tobj->getSessionID(),
			tobj->getSymbolID(),
			tobj->getX()*1.3333 - 0.1666,
			tobj->getY(),
			tobj->getTuioTime(),
			tobj->getXSpeed(),
			tobj->getYSpeed(),
			tobj->getMotionSpeed(),
			tobj->getMotionAccel(),
			tobj->getAngle(),
			tobj->getRotationSpeed(),
			tobj->getRotationAccel(),
			tobj->getTuioState()
			));
}

void Doodle::TuioProxy::removeTuioObject(TUIO::TuioObject* tobj) {
	emit(removeTangible(
			tobj->getSessionID(),
			tobj->getSymbolID(),
			tobj->getX()*1.3333 - 0.1666,
			tobj->getY(),
			tobj->getTuioTime(),
			tobj->getXSpeed(),
			tobj->getYSpeed(),
			tobj->getMotionSpeed(),
			tobj->getMotionAccel(),
			tobj->getAngle(),
			tobj->getRotationSpeed(),
			tobj->getRotationAccel(),
			tobj->getTuioState()
			));
}

void Doodle::TuioProxy::addTuioCursor(TUIO::TuioCursor* tcur) {
	emit(addCursor(
			tcur->getSessionID(),
			tcur->getCursorID(),
			tcur->getX()*1.3333 - 0.1666,
			tcur->getY(),
			tcur->getTuioTime(),
			tcur->getXSpeed(),
			tcur->getYSpeed(),
			tcur->getMotionSpeed(),
			tcur->getMotionAccel(),
			tcur->getTuioState()
			));
}
void Doodle::TuioProxy::updateTuioCursor(TUIO::TuioCursor* tcur) {
	emit(updateCursor(
			tcur->getSessionID(),
			tcur->getCursorID(),
			tcur->getX()*1.3333 - 0.1666,
			tcur->getY(),
			tcur->getTuioTime(),
			tcur->getXSpeed(),
			tcur->getYSpeed(),
			tcur->getMotionSpeed(),
			tcur->getMotionAccel(),
			tcur->getTuioState()
			));
}

void Doodle::TuioProxy::removeTuioCursor(TUIO::TuioCursor* tcur) {
	emit(removeCursor(
			tcur->getSessionID(),
			tcur->getCursorID(),
			tcur->getX()*1.3333 - 0.1666,
			tcur->getY(),
			tcur->getTuioTime(),
			tcur->getXSpeed(),
			tcur->getYSpeed(),
			tcur->getMotionSpeed(),
			tcur->getMotionAccel(),
			tcur->getTuioState()
			));
}

void Doodle::TuioProxy::refresh(TUIO::TuioTime ttime) { emit(tuioCycle(ttime)); }
