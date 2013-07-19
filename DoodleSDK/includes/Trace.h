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

#ifndef TRACE_H_
#define TRACE_H_

#include <QList>
#include "Stroke.h"

namespace Doodle {

class Tangible;
class Widget;

/*!
 * A Trace is a sequence of related Strokes. Two strokes are related when the
 * last point of the first Stroke and the first point of the second Stroke are
 * nearby. This way a Trace can be composed of multiple subsequent Strokes. This
 * helps when a cursor is so fast that the vision engine loses its track for a
 * short time, thus creating unwanted gaps.
 */
class Trace {
public:
	/*!
	 * Standard constructor.
	 */
	Trace();

	/*!
	 * Standard destructor.
	 */
	~Trace();

	/*!
	 * \return	List of Strokes in the Trace.
	 */
	const QList<Stroke>& strokes() const;

	/*!
	 * \return	Unique Trace ID.
	 */
	long id() const;

	/*!
	 * \return
	 */
	long prevId() const;

	/*!
	 * \return	Unique Group ID the Trace is part of.
	 */
	long groupId() const { return _groupId; }

	/*!
	 * Sets the ID of the Groups the Trace is part of.
	 *
	 * \param	groupId	Unique Group ID the Trace is part of.
	 */
	void setGroupId(long groupId) { _groupId = groupId; }

	/*!
	 * \return	First point of the (first Stroke in the) Trace.
	 */
	const Cursor& firstPoint() const;

	/*!
	 * \return	Last point of the (last Stroke in the) Trace.
	 */
	const Cursor& lastPoint() const;

	/*!
	 * Appends a point to the Trace.
	 *
	 * \param	c	The point.
	 */
	void append(Cursor c);

	/*!
	 * Associates a Tangible object with the Trace.
	 */
	void addTangible(Tangible* t);

	/*!
	 * \return	List of Tangible objects associated with the Trace.
	 */
	const QList<Tangible*>& tangibles() const { return _tangibles; }

	/*!
	 * \return	The Widget the Trace is associated with. TODO: Which one?
	 */
	Widget* widget() { return _widget; }

	/*!
	 * Associate the Trace with a Widget.
	 *
	 * \param	widget	Widget to be associated with the Trace.
	 */
	void setWidget(Widget* widget) { _widget = widget; }

	/*!
	 * State of the Trace. Used for grouping Strokes.
	 */
	enum State { ADDED, UPDATED, REMOVED, RESUMED, DEAD };

	/*!
	 * \return	State of the Treace.
	 */
	enum State state() const { return _state; }

	/*!
	 * Sets the state of the Trace.
	 *
	 * \param	state	The state.
	 */
	void setState(State state) { _state = state; }

	/*!
	 * \return	True if the Trace started inside a widget.
	 */
	bool isOnWidget() const { return _widget != NULL; }

private:
	long _groupId;

	QList<Stroke> _strokes;
	QList<Tangible*> _tangibles;
	Widget* _widget;

	State _state;
};
}

#endif /* TRACE_H_ */
