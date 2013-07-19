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

#ifndef TARGET_H_
#define TARGET_H_

#include <QObject>
#include <QPointF>
#include <TUIO/TuioTime.h>

namespace Doodle {

/*!
 * A class for dealing with TUIO objects.
 * A Target is any TUIO objects. It has the common properties of all TUIO
 * objects. This is a base class that is mostly intended to be extended.
 *
 * \see Cursor
 * \see Tangible
 */
class Target {
public:
	/*!
	 * Standard constructor.
	 */
	Target();

	/*!
	 * Clone constructor. This is used to build a Target that clones a specific
	 * TUIO object.
	 *
	 * \param	sessionID	Unique session ID , according to TUIO protocol.
	 * \param	x			x coordinate.
	 * \param	y			y coordinate.
	 * \param	ttime		Timestamp, according to TUIO protocol.
	 * \param	xSpeed		Linear speed along the x coordinate.
	 * \param	ySpeed		Linear speed along the y coordinate.
	 * \param	motionSpeed	Linear speed of the Cursor.
	 * \param	motionAccel	Acceleration of the Cursor.
	 * \param	state		State of the Cursor, according to TUIO protocol.
	 *
	 */
	Target(long sessionID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, int state);

	/*!
	 * Standard destructor.
	 */
	~Target();

	/*!
	 * Computes the square distance between this Target and a given one.
	 *
	 * \param	t	A reference to the other Target.
	 *
	 * \return	Square distance.
	 */
	float squareDistance(const Target& t);

	/*!
	 * \return	TUIO Session ID of this Target.
	 */
	long sessionID() const { return _sessionID; }

	/*!
	 * \return	\f$ x \f$ coordinate of this Target.
	 *
	 * \sa y()
	 */
	float x() const { return _position.x(); }

	/*!
	 * \return	\f$ y \f$ coordinate of this Target.
	 *
	 * \sa x()
	 */
	float y() const { return _position.y(); }

	/*!
	 * \return	Coordinates of this Target as a QPointF reference.
	 *
	 * \sa x()
	 * \sa y()
	 */
	const QPointF& position() const { return _position; }

	/*!
	 * \return	Timestamp of this Target according to TUIO protocol.
	 */
	TUIO::TuioTime currentTime() const { return _currentTime; }

	/*!
	 * \return	Linear speed along the \f$ x \f$ coordinate.
	 *
	 * \sa motionSpeed()
	 */
	float xSpeed() const { return _xSpeed; }

	/*!
	 * \return	Linear speed along the \f$ y \f$ coordinate.
	 *
	 * \sa motionSpeed()
	 */
	float ySpeed() const { return _ySpeed; }

	/*!
	 * \return	Linear speed of this Target.
	 *
	 * \sa xSpeed()
	 * \sa ySpeed()
	 * \sa motionAccel()
	 */
	float motionSpeed() const { return _motionSpeed; }

	/*!
	 * \return Acceleration of this Target.
	 *
	 * \sa motionSpeed()
	 */
	float motionAccel() const { return _motionAccel; }

	/*!
	 * The state of this Target according to TUIO protocol.
	 *
	 * \sa state()
	 * \sa setState()
	 */
	enum State { INVALID = -1, ADDED, ACCELERATING, DECELERATING, STOPPED, REMOVED };

	/*!
	 * \return	The state of this Target according to TUIO protocol.
	 *
	 * \sa setState()
	 */
	int state() const { return _state; }

	/*!
	 * Sets the state of this Target.
	 *
	 * \param	state	State.
	 *
	 * \sa state()
	 */
	void setState(State state) { _state = state; }

protected:
	long _sessionID;
	QPointF _position;
	TUIO::TuioTime _currentTime;
	float _xSpeed;
	float _ySpeed;
	float _motionSpeed;
	float _motionAccel;
	int _state;
};
}

#endif /* TARGET_H_ */
