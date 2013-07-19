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

#include <QSet>

#include "Group.h"
#include "Tangible.h"

long Doodle::Group::s_id = 0;

Doodle::Group::Group() {
	_id = s_id++;
}

void Doodle::Group::finalizeTrace(Trace* t) {
	_traces.append(*t);
	_tempTraces.removeAll(t);
}

void Doodle::Group::appendTrace(Trace* t) {
	if(!_tempTraces.contains(t)) {
		_tempTraces.append(t);
	}
}

QList<long> Doodle::Group::objects() const {
	// FIXME: Potential flaw with tangibles having the same fiducial.
	QList<long> l;
	QSet<long> s;
	QList<Trace>::ConstIterator i;
	for(i = _traces.begin(); i != _traces.end(); ++i) {
		const QList<Tangible*>& o = i->tangibles();
		QList<Tangible*>::ConstIterator tli;
		for(tli = o.begin(); tli != o.end(); ++tli) {
			s.insert((*tli)->fiducialID());
		}
	}
	l = s.toList();
	return l;
}
