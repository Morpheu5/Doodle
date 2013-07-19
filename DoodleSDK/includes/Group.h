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

#ifndef GROUP_H_
#define GROUP_H_

#include <QList>
#include "Trace.h"

namespace Doodle {

/*!
 * A class for grouping finger traces.
 * Traces are considered a group when they are somewhat related. This can happen
 * when they're near in some sense, or even if they're not but they're
 * associated with the same object.
 */
class Group {
public:
	/*!
	 * Standard constructor.
	 */
	Group();

	/*!
	 * Removes a trace from the temporary list and puts it in the list of traces
	 * that are to be considered part of this group.
	 *
	 * \param	t	Pointer to the trace to be finalized.
	 */
	void finalizeTrace(Trace* t);

	/*!
	 * Continues a trace that hasn't yet been finalized.
	 *
	 * \param	t	Pointer to the trace that has to be continued.
	 */
	void appendTrace(Trace* t);

	/*!
	 * \return The unique ID of this group.
	 */
	long id() const { return _id; }

	/*!
	 * Checks if there are traces in the temporary list.
	 */
	bool isEmpty() const { return _tempTraces.isEmpty(); }

	/*!
	 * Checks if the last trace in the temporary list is associated with a
	 * widget.
	 */
	bool isOnWidget() const { return _tempTraces.last()->widget() != NULL; }

	/*!
	 * \return	List of IDs of the tangible objects the traces in this group are
	 * 			associated with.
	 */
	QList<long> objects() const;

	/*!
	 * \return	A reference to the list of finalized traces in this group.
	 */
	QList<Trace>& finalizedTraces() { return _traces; }

	/*!
	 * \return	A reference to the list of traces that are in the temporary list.
	 * 			This is used for working with those traces that must be analyzed
	 * 			in a continuous fashion, e.g. those performing drag or pinch
	 * 			gestures.
	 */
	QList<Trace*>& progressiveTraces() { return _tempTraces; }

private:
	long _id;
	QList<Trace> _traces;
	QList<Trace*> _tempTraces;
	static long s_id;
};
}

#endif /* GROUP_H_ */
