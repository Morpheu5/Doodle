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
#include <TUIO/TuioPoint.h>
#include <TUIO/TuioContainer.h>
#include <TUIO/TuioTime.h>
#include "Target.h"

#include <QDebug>

Doodle::Target::Target() {
	_state = INVALID;
}

Doodle::Target::Target(long sessionID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, int state) {
	_position.setX(x);
	_position.setY(y);
	_currentTime = ttime;
	_sessionID = sessionID;
	_xSpeed = xSpeed;
	_ySpeed = ySpeed;
	_motionSpeed = motionSpeed;
	_motionAccel = motionAccel;
	_state = state;
}

Doodle::Target::~Target() {
}

float Doodle::Target::squareDistance(const Target& t) {
	float dx = _position.x() - t._position.x();
	float dy = _position.y() - t._position.y();
	return dx*dx + dy*dy;
}
