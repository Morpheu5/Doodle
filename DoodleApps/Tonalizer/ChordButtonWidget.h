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

#ifndef CHORDBUTTONWIDGET_H_
#define CHORDBUTTONWIDGET_H_

#include <DoodleSDK/widgets/CheckButtonWidget.h>
#include "ChordWidget.h"
#include "Chord.h"

namespace Doodle {

/*!
 * The slots around the Tonalizer representing chords. These can be active or
 * inactive, and can also be used to open the corresponding ChordWidget.
 */
class ChordButtonWidget : public CheckButtonWidget {
public:
	virtual bool pinchable() const { return false; }
	virtual void setPinchable(bool) { _pinchable = false; }

	/*!
	 * Sets corresponding ChordWidget.
	 *
	 * \param	chordWidget	The ChordWidget.
	 */
	void setChordWidget(ChordWidget* chordWidget) { _chordWidget = chordWidget; }

	/*!
	 * \return	The ChordWidget associated with the ChordButtonWidget.
	 */
	ChordWidget* chordWidget() { return _chordWidget; }

	/*!
	 * \return	The name of Chord's tonic.
	 */
	QString text() const { return _chordWidget->chord()->tonic(); }

protected:
	ChordWidget* _chordWidget;
};

}

#endif /* CHORDBUTTONWIDGET_H_ */
