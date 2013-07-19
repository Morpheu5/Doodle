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

#include "Tangible.h"
#include <cmath>

#include <QDebug>

Doodle::Tangible::Tangible() : Target() {
	makeShape();
	_size = 0.2f;
	_associated = false;
}

Doodle::Tangible::Tangible(long sessionID, int fiducialID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, float angle, float rotationSpeed, float rotationAccel, int state) : Target(sessionID, x, y, ttime, xSpeed, ySpeed, motionSpeed, motionAccel, state) {
	_fiducialID = fiducialID;
	_angle = angle;
	_rotationSpeed = rotationSpeed;
	_rotationAccel = rotationAccel;
	_size = 0.2f;
	makeShape();
	_associated = false;
}

Doodle::Tangible::~Tangible() {
}

void Doodle::Tangible::makeShape() {
	// TODO: Make the size factor configurable
	int i, n = 24;
	for(i = 0; i < n; i++) {
		double theta = M_PI * ((double)i/((double)n/2));
		_shape.append(QPointF(cos(theta), sin(theta)));
	}
	_shape.append(QPointF(1, 0));
}
