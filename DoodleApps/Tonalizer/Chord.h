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

#ifndef CHORD_H_
#define CHORD_H_

#include <QVector>
#include <QSet>
#include <QString>
#include <QMap>

namespace Doodle {

/*!
 * A Chord is made of a tonic and a set of intervals from the tonic. This way
 * we can express many different chords, from simple majors and minors to
 * uncommon ones like mM9 (that is minor third, major seventh and added ninth).
 */
class Chord {
public:
	/*!
	 * Sets the tonic and constructs a major chord.
	 */
	Chord(QString& tonic);

	/*!
	 * Standard destructor.
	 */
	virtual ~Chord();

	/*!
	 * \return	List of the intervals of the Chord.
	 */
	QList<int> notes() { return _notes.uniqueKeys(); }

	/*!
	 * \return	Name of the tonic note.
	 */
	QString tonic() { return _tonic; }

	/*!
	 * Sets the name of the tonic note.
	 */
	void setTonic(QString& tonic) { _tonic = tonic; }

	/*!
	 * This routine processes a message coming from a Glyph Recognizer and
	 * updates the Chord accordingly.
	 *
	 * \param	s	The message.
	 */
	void process(QString s);

	/*!
	 * \return	Names of the notes the Chord is made of.
	 */
	QString toPitches();

	/*!
	 * \return	Formal notation of the Chord, for example C, D7, Em add9, Fsus4
	 * 			and so on. This is for display on the ChordWidget.
	 */
	QString toNotation();

private:
	int _base;
	QMap<int, int> _notes;
	QString _tonic;
	int _last;
	bool _add;
	QString _buffer;

	QVector<QString> _chromaticScale;
	QVector<QString> _labels;
};

}

#endif /* CHORD_H_ */
