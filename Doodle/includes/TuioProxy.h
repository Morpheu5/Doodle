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

#ifndef TUIOPROXY_H_
#define TUIOPROXY_H_

#include <QObject>
#include <TUIO/TuioObject.h>
#include <TUIO/TuioCursor.h>
#include "TuioListener.h"

namespace Doodle {

/*!
 * This class implements the cloning process. It's been decided that cloning
 * the objects was safer than using pointers that may unexpectedly disappear.
 * Even if TUIO library has been checked multiple times, there are still
 * occasional dangling pointers leading to null pointer accesses and consequent
 * crashes. By cloning every incoming object, the number of crashes was
 * dramatically reduced, at the cost of the time and memory space needed for
 * copying all the parameters.
 */
class TuioProxy : public QObject {
	Q_OBJECT

public:
	/*!
	 * Standard constructor.
	 *
	 * \param	port	UDP Port number for TuioListener. This is set by Doodle
	 * 					according to the configuration file.
	 */
	TuioProxy(int port);

	/*!
	 * Called by the listener when a tangible object is put on the table.
	 *
	 * \param	tobj	Pointer to the object.
	 */
	void addTuioObject(TUIO::TuioObject* tobj);

	/*!
	 * Called by the listener when a tangible object on the table is updated.
	 *
	 * \param	tobj	Pointer to the object.
	 */
	void updateTuioObject(TUIO::TuioObject* tobj);

	/*!
	 * Called by the listener when a tangible object  is removed from the table.
	 *
	 * \param	tobj	Pointer to the object.
	 */
	void removeTuioObject(TUIO::TuioObject* tobj);

	/*!
	 * Called by the listener when a finger touches the table.
	 *
	 * \param	tcur	Pointer to the cursor.
	 */
	void addTuioCursor(TUIO::TuioCursor* tcur);

	/*!
	 * Called by the listener when a finger is moved on the table.
	 *
	 * \param	tcur	Pointer to the cursor.
	 */
	void updateTuioCursor(TUIO::TuioCursor* tcur);

	/*!
	 * Called by the listener when a finger leaves the table.
	 *
	 * \param	tcur	Pointer to the cursor.
	 */
	void removeTuioCursor(TUIO::TuioCursor* tcur);

	/*!
	 * Emits the tuioCycle signal to make the processing phase of the Cycle
	 * begin.
	 *
	 * \param	ttime	TUIO Timestamp.
	 */
	void refresh(TUIO::TuioTime ttime);

signals:
	/*!
	 * Emitted from addTuioObject with the list of parameters.
	 *
	 * For information on the parameters, see Tangible.
	 *
	 * \sa Tangible
	 */
	void addTangible(long sessionID, int fiducialID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, float angle, float rotationSpeed, float rotationAccel, int state);

	/*!
	 * Emitted from updateTuioObject with the list of parameters.
	 *
	 * For information on the parameters, see Tangible.
	 *
	 * \sa Tangible
	 */
	void updateTangible(long sessionID, int fiducialID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, float angle, float rotationSpeed, float rotationAccel, int state);

	/*!
	 * Emitted from removeTuioObject with the list of parameters.
	 *
	 * For information on the parameters, see Tangible.
	 *
	 * \sa Tangible
	 */
	void removeTangible(long sessionID, int fiducialID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, float angle, float rotationSpeed, float rotationAccel, int state);

	/*!
	 * Emitted from addTuioCursor with the list of parameters.
	 *
	 * For information on the parameters, see Tangible.
	 *
	 * \sa Cursor
	 */
	void addCursor(long sessionID, int fingerID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, int state);

	/*!
	 * Emitted from updateTuioCursor with the list of parameters.
	 *
	 * For information on the parameters, see Tangible.
	 *
	 * \sa Cursor
	 */
	void updateCursor(long sessionID, int fingerID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, int state);

	/*!
	 * Emitted from removeTuioCursor with the list of parameters.
	 *
	 * For information on the parameters, see Tangible.
	 *
	 * \sa Cursor
	 */
	void removeCursor(long sessionID, int fingerID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, int state);

	/*!
	 * Emitted by refresh when the recognition phase is finished and the
	 * processing phase shall begin.
	 *
	 * \param	ttime	TUIO timestamp.
	 */
	void tuioCycle(TUIO::TuioTime ttime);

private:
	TuioListener* _tuioListener;
};
}

#endif /* TUIOPROXY_H_ */
