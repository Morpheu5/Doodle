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

#ifndef CURSOR_H_
#define CURSOR_H_

#include "Target.h"

namespace Doodle {

/*!
 * A Cursor is a finger tip.
 *
 * \see Target
 */
class Cursor : public Target {
public:
	/*!
	 * Standard constructor.
	 */
	Cursor();

	/*!
	 * Clone constructor.
	 *
	 * \param	sessionID	Unique session ID , according to TUIO protocol.
	 * \param	fingerID	Unique finger ID, according to TUIO protocol.
	 * \param	x			x coordinate.
	 * \param	y			y coordinate.
	 * \param	ttime		Timestamp, according to TUIO protocol.
	 * \param	xSpeed		Linear speed along the x coordinate.
	 * \param	ySpeed		Linear speed along the y coordinate.
	 * \param	motionSpeed	Linear speed of the Cursor.
	 * \param	motionAccel	Acceleration of the Cursor.
	 * \param	state		State of the Cursor, according to TUIO protocol.
	 *
	 * \see Target
	 */
	Cursor(long sessionID, int fingerID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, int state);

	/*!
	 * Standard destructor.
	 */
	~Cursor();

	/*!
	 * \return	The ID of this finger tip according to TUIO protocol.
	 */
	int fingerID() const { return _fingerID; }

protected:
	int _fingerID;
};
}

#endif /* CURSOR_H_ */
