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
#include "Trace.h"
#include "Widget.h"

Doodle::Trace::Trace() {
	Stroke s;
	_strokes.append(s);
	_groupId = -1;
	_widget = NULL;
}

Doodle::Trace::~Trace() {
}

const QList<Doodle::Stroke>& Doodle::Trace::strokes() const {
	if(_strokes.isEmpty()) {
		throw std::runtime_error("Empty strokes");
	}
	return _strokes;
}

long Doodle::Trace::id() const {
	if(_strokes.isEmpty()) {
		throw std::runtime_error("Empty trace");
	}
	return _strokes.last().id();
}

long Doodle::Trace::prevId() const {
	if(!_strokes.isEmpty()) {
		if(_strokes.size() < 2) {
			return _strokes.at(0).id();
		} else {
			return _strokes.at(_strokes.size()-2).id();
		}
	} else {
		throw std::runtime_error("Empty trace");
	}
}

const Doodle::Cursor& Doodle::Trace::firstPoint() const {
	if(_strokes.isEmpty()) {
		throw std::runtime_error("Trace empty");
	}
	return _strokes.first().first();
}

const Doodle::Cursor& Doodle::Trace::lastPoint() const {
	if(_strokes.isEmpty()) {
		throw std::runtime_error("Trace empty");
	}
	return _strokes.last().last();
}

void Doodle::Trace::append(Cursor c) {
	if(_state == RESUMED) {
		Stroke s;
		_strokes.append(s);
	}
	_strokes.last().append(c);
}

void Doodle::Trace::addTangible(Tangible* t) {
	if(!_tangibles.isEmpty()) {
		if(_tangibles.last()->fiducialID() == t->fiducialID()) {
			return;
		}
	}
	_tangibles.append(t);
	t->setAssociated(true);
}
