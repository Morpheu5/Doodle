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

#include <stdexcept>

#include "Chord.h"
#include <QDebug>
#include <QtAlgorithms>

Doodle::Chord::Chord(QString& tonic) {
	_chromaticScale << "C" << "C#" << "D" << "D#" << "E" << "F" << "F#" << "G" << "G#" << "A" << "A#" << "B";
	if(_chromaticScale.lastIndexOf(tonic) == -1) {
		throw std::runtime_error("Couldn't create chord " + tonic.toStdString());
	}
	_base = 4;
	_tonic = tonic;
	_notes.insert(0,0);
	_notes.insert(4,4);
	_notes.insert(7,7);
}

Doodle::Chord::~Chord() {

}

void Doodle::Chord::process(QString s) {
	if(s == "b" || s == "B" || s == "F") {
		int i = _chromaticScale.lastIndexOf(_tonic);
		if(i != -1) {
			if(i == 0) {
				_tonic = _chromaticScale[11];
			} else {
				_tonic = _chromaticScale[(i-1)%12];
			}
		}
	} else if(s == "#") {
		int i = _chromaticScale.lastIndexOf(_tonic);
		_tonic = _chromaticScale[(i+1)%12];
	} else if(s == "m") {
		if(_notes.contains(5)) {
			_notes.remove(4);
			_notes.insert(3,3);
		} else if(_notes.contains(4)) {
			_notes.remove(4);
			_notes.insert(3,3);
		} else if(_notes.contains(3)) {
			_notes.remove(3);
			_notes.insert(4,4);
		}
		_notes.remove(5);
	} else if(s == "4") {
		if(_notes.contains(5)) {
			_notes.remove(5);
			_notes.insert(4,4);
		} else {
			_notes.remove(3);
			_notes.remove(4);
			_notes.insert(5,5);
		}
	} else if(s == "M" || s == "maj") {
		if(_notes.contains(10)) {
			_notes.remove(10);
			_notes.insert(11,11);
		} else if(_notes.contains(11)) {
			_notes.remove(11);
		} else {
			_notes.insert(11,11);
		}
	} else if(s == "9") {
		if(_notes.contains(14)) {
			_notes.remove(14);
		} else {
			if(!_notes.contains(10) && !_notes.contains(11)) {
				_notes.insert(10,10);
			}
			_notes.insert(14,14);
		}
	} else if(s == "6") {
		if(_notes.contains(9)) {
			_notes.remove(9);
		} else {
			_notes.insert(9,9);
		}
	} else if(s == "7") {
		if(_notes.contains(11)) {
			_notes.remove(11);
			_notes.insert(10,10);
		} else if(_notes.contains(10)) {
			_notes.remove(10);
		} else {
			_notes.insert(10,10);
		}
	}
}

QString Doodle::Chord::toPitches() {
	QList<int> notes = _notes.uniqueKeys();
	qSort(notes);
	int octave = _base;
	QString result;
	int i = _chromaticScale.lastIndexOf(_tonic);
	foreach(int n, notes) {
		if(n+i > 11) {
			octave = _base + 1;
		}
		result.append(_chromaticScale[(n+i)%12]).append(QString::number(octave)).append(" ");
	}
	return result;
}

QString Doodle::Chord::toNotation() {
	QList<int> notes = _notes.uniqueKeys();
	QString result;
	result.append(_tonic);
	if(notes.contains(3)) {
		result.append("m");
	}
	if(notes.contains(5)) {
		result.append("sus4 ");
	}
	if(notes.contains(9)) {
		result.append("6");
	}
	if(notes.contains(10) && !notes.contains(14)) {
		result.append("7");
	}
	if(notes.contains(11)) {
		result.append("M");
	}
	if(notes.contains(14)) {
		if(!notes.contains(10) && !notes.contains(11)) {
			result.append("add");
		}
		if(notes.contains(9) && notes.contains(10) && !notes.contains(11)) {
			result.append("/");
		}
		result.append("9");
	}
	return result;
}
