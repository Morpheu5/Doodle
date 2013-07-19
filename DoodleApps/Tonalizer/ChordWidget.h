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

#ifndef CHORDWIDGET_H_
#define CHORDWIDGET_H_

#include <QFont>

#include <DoodleSDK/includes/Widget.h>
#include <DoodleSDK/widgets/PushButtonWidget.h>
#include <DoodleSDK/widgets/CheckButtonWidget.h>

namespace Doodle {
class Chord;

/*!
 * This is an alternate way to edit chords. Glyph Recognition is ok but it is
 * wise to provide alternatives to users.
 */
class ChordWidget : public Widget {
	Q_OBJECT

public:
	ChordWidget();
	ChordWidget(Widget* parent);

	/*!
	 * Helper routine to create the visuals of this widget.
	 */
	void makeVisual();

	const QString name() const { return QString("ChordWidget"); }

	/*!
	 * Not pinchable.
	 */
	virtual bool pinchable() const { return false; }

	/*!
	 * Not pinchable.
	 */
	virtual void setPinchable(bool) { _pinchable = false; }

	void render(QPainter* painter);

	/*!
	 * Sets the Chord that this widget is going to edit.
	 *
	 * \param	chord	The Chord.
	 */
	void setChord(Chord* chord) { _chord = chord; }

	/*!
	 * \return	The Chord at its current state.
	 */
	Chord* chord() { return _chord; }

	/*!
	 * Makes the ChordWidget active (i.e. visible).
	 */
	void setActive(bool active);

	/*!
	 * \return	True if it is active.
	 */
	bool active() const { return _active; }

	/*!
	 * A PushButtonWidget to make the tonic a half-tone higher.
	 */
	PushButtonWidget* sharp() { return _sharp; }

	/*!
	 * A PushButtonWidget to make the tonic a half-tone lower.
	 */
	PushButtonWidget* flat() { return _flat; }

	/*!
	 * Minor/major control.
	 */
	CheckButtonWidget* minor() { return _minor; }

	/*!
	 * Major seventh control.
	 */
	CheckButtonWidget* sevenMaj() { return _sevenMaj; }

	/*!
	 * Ninth control.
	 */
	CheckButtonWidget* nine() { return _nine; }

	/*!
	 * Suspended fourth control.
	 */
	CheckButtonWidget* four() { return _four; }

	/*!
	 * Sixth control.
	 */
	CheckButtonWidget* six() { return _six; }

	/*!
	 * Minor seventh control.
	 */
	CheckButtonWidget* seven() { return _seven; }

public slots:
	void tap(int n);

protected:
	/*!
	 * Used to update the visual representation when necessary.
	 */
	void updateVisual();

	Chord* _chord;
	bool _active;

	QFont _font;

	PushButtonWidget* _sharp;
	PushButtonWidget* _flat;
	CheckButtonWidget* _minor;
	CheckButtonWidget* _sevenMaj;
	CheckButtonWidget* _four;
	CheckButtonWidget* _six;
	CheckButtonWidget* _seven;
	CheckButtonWidget* _nine;
};

}

#endif /* CHORDWIDGET_H_ */
