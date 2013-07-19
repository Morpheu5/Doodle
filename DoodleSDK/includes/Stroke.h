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

#ifndef STROKE_H_
#define STROKE_H_

#include <stdexcept>

#include <QList>
#include "Cursor.h"

namespace Doodle {

/*!
 * A Stroke is the list of points that describes the motion of a cursor, from
 * its creation to its destruction.
 */
class Stroke {
public:
	/*!
	 * \return	Unique ID of a Stroke.
	 */
	long id() const;

	/*!
	 * \return	First point in the Stroke.
	 */
	const Cursor& first() const;

	/*!
	 * \return	Last point in the Stroke.
	 */
	const Cursor& last() const;

	/*!
	 * Indexed access to any point in the list.
	 *
	 * \param	i	Index.
	 *
	 * \return	i-th point in the list.
	 */
	const Cursor& operator[](int i) const;

	/*!
	 * \return	True if the Stroke is empty.
	 */
	bool isEmpty() const { return _stroke.isEmpty(); }

	/*!
	 * \return	Number of points in the Stroke.
	 */
	int size() const { return _stroke.size(); }

	/*!
	 * Appends a new point to the Stroke.
	 *
	 * \param	c	The new point.
	 */
	void append(Cursor c) { _stroke.append(c); }

private:
	QList<Cursor> _stroke;
};
}

#endif /* STROKE_H_ */
