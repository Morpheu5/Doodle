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

#ifndef TANGIBLE_H_
#define TANGIBLE_H_

#include <qmath.h>
#include <QPolygonF>
#include "Target.h"

namespace Doodle {
/*!
 * A Tangible is a puck with a fiduciary marker.
 * This is a base class that can be extended to create different types of
 * objects, mostly useful to give special visual feedback.
 *
 * \see Target
 */
class Tangible : public Target {
public:
	/*!
	 * Standard constructor.
	 */
	Tangible();

	/*!
	 * Clone constructor.
	 *
	 * \param	sessionID		Unique session ID , according to TUIO protocol.
	 * \param	fiducialID		Unique fiducial ID, according to TUIO protocol.
	 * \param	x				x coordinate.
	 * \param	y				y coordinate.
	 * \param	ttime			Timestamp, according to TUIO protocol.
	 * \param	xSpeed			Linear speed along the x coordinate.
	 * \param	ySpeed			Linear speed along the y coordinate.
	 * \param	motionSpeed		Linear speed of the Cursor.
	 * \param	motionAccel		Acceleration of the Cursor.
	 * \param	angle			Angle in radians.
	 * \param	rotationSpeed	Angular speed in radians per second.
	 * \param	rotationAccel	Angular acceleration in radians per second
	 * 							squared.
	 * \param	state			State of the Cursor, according to TUIO protocol.
	 *
	 * \see Target
	 */
	Tangible(long sessionID, int fiducialID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, float angle, float rotationSpeed, float rotationAccel, int state);

	/*!
	 * Standard destructor.
	 */
	~Tangible();

	/*!
	 * \return	ID of the fiduciary marker attached to this object.
	 */
	int fiducialID() const { return _fiducialID; }

	/*!
	 * \return	Rotation angle of this object in radians.
	 */
	float angle() const { return _angle; }

	/*!
	 * \return	Rotation angle of this object in degrees.
	 */
	float angleDeg() const { return _angle*180/M_PI; }

	/*!
	 * \return	Angular speed of this object.
	 */
	float rotationSpeed() const { return _rotationSpeed; }

	/*!
	 * \return	Angular acceleration of this object.
	 */
	float rotationAccel() const { return _rotationAccel; }

	/*!
	 * \return	Size of this object, used to paint its visual representation.
	 */
	float size() const { return _size; }

	/*!
	 * \return	True if this tangible is associated with a Trace.
	 */
	bool associated() const { return _associated; }

	/*!
	 * Sets Trace association status of this object.
	 *
	 * \param	associated	True if the Tangible is associated with a Trace.
	 */
	void setAssociated(bool associated) { _associated = associated; }

	/*!
	 * Makes the visual shape for this object.
	 */
	void makeShape();

	/*!
	 * \return The shape for this object.
	 */
	const QPolygonF& shape() const { return _shape; }

protected:
	int _fiducialID;
	float _angle;
	float _rotationSpeed;
	float _rotationAccel;
	float _size;
	bool _associated;

	QPolygonF _shape;
};
}

#endif /* TANGIBLE_H_ */
