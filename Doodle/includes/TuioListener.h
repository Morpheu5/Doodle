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

#ifndef TUIOLISTENER_H_
#define TUIOLISTENER_H_

#include <stdexcept>

#include <TUIO/TuioListener.h>
#include <TUIO/TuioClient.h>

namespace Doodle {
class TuioProxy;

/*!
 * TuioListener used to receive TUIO objects from TUIO library and deliver them
 * to the TuioProxy that will deliver them to the application in an appropriate
 * format.
 *
 * For more information about TUIO C++ API, see http://www.tuio.org/?cpp
 */
class TuioListener : public TUIO::TuioListener {
public:
	/*!
	 * Constructor.
	 *
	 * \param	port	UPD Port on which to listen.
	 * \param	parent	The TuioProxy to which deliver the objects.
	 */
	TuioListener(int port, TuioProxy* parent);

	/*!
	 * Standard destructor.
	 */
	~TuioListener();

	/*!
	 * Called when a tangible object is put on the table.
	 *
	 * \param	tobj	Pointer to the object.
	 */
	void addTuioObject(TUIO::TuioObject* tobj);

	/*!
	 * Called when a tangible object on the table is updated.
	 *
	 * \param	tobj	Pointer to the object.
	 */
	void updateTuioObject(TUIO::TuioObject* tobj);

	/*!
	 * Called when a tangible object is removed from the table.
	 *
	 * \param	tobj	Pointer to the object.
	 */
	void removeTuioObject(TUIO::TuioObject* tobj);

	/*!
	 * Called when a finger touches the table.
	 *
	 * \param	tcur	Pointer to the cursor.
	 */
	void addTuioCursor(TUIO::TuioCursor* tcur);

	/*!
	 * Called when a finger moves on the table.
	 *
	 * \param	tcur	Pointer to the cursor.
	 */
	void updateTuioCursor(TUIO::TuioCursor* tcur);

	/*!
	 * Called when a finger leaves the table.
	 *
	 * \param	tcur	Pointer to the cursor.
	 */
	void removeTuioCursor(TUIO::TuioCursor* tcur);

	/*!
	 * Signals that the sampling and analysis phase of a TUIO Cycle is completed
	 * and processing can begin.
	 */
	void refresh(TUIO::TuioTime ttime);

private:
	TuioProxy* _tuioProxy;
	TUIO::TuioClient* _tuioClient;
};
}

#endif /* TUIOLISTENER_H_ */
