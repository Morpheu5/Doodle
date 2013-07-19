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

#ifndef KEYBOARDWIDGET_H_
#define KEYBOARDWIDGET_H_

#include <QFont>

#include <DoodleSDK/includes/Widget.h>
#include <DoodleSDK/widgets/CheckButtonWidget.h>

namespace Doodle {

/*!
 * This is the Keyboard on which users compose melodies.
 */
class KeyboardWidget : public Widget {
	Q_OBJECT

public:
	KeyboardWidget();
	KeyboardWidget(Widget* parent);

	void init();

	/*!
	 * Helper routine to build the visual representation. This is a grid of
	 * CheckButtonWidgets, so that each one can be turned on and off at will.
	 * Those that are on are notes that will be played.
	 */
	void makeVisual();

	const QString name() const { return QString("KeyboardWidget"); }

	virtual bool pinchable() const { return false; }
	virtual void setPinchable(bool pinchable) { _pinchable = false; }

	void render(QPainter* painter);

	void setVisible(bool visible = true);

	/*!
	 * Sets the scales that fit the current Chord and chooses the first one by
	 * default to be the current scale.
	 *
	 * In the current implementation there's not yet a mechanism to choose
	 * another scale.
	 */
	void setScales(QHash<QString, QSet<int> >* scales) {
		_scales = scales;
		_currentScale = &(*(scales->begin()));
		updateKeyboard();
	}

	/*!
	 * \return	The grid of notes on the KeyboardWidget.
	 */
	QList<int>& columns() { return _columns; }

	/*!
	 * \return	List of the notes that are active on a given column.
	 */
	QList<int> notes(int);

signals:
	void childAdded(Widget* widget);

	/*!
	 * Signal emitted when the KeyboardWidget needs to change its layout and so
	 * all its children (e.g. CheckButtonWidgets) are destroyed.
	 */
	void childDestroyed(Widget* widget);

public slots:
	void tap(int n);

	/*!
	 * Hides the KeyboardWidget.
	 */
	void hide();

	/*!
	 * Turns off all the notes.
	 */
	void clean();

	/*!
	 * Selects a different scale from those available.
	 */
	void changeScale();

protected:
	/*!
	 * \internal
	 */
	void updateVisual();

	/*!
	 * Routine to change KeyboardWidget layout.
	 */
	void updateKeyboard();

	bool _active;

	QFont _font;

	QList<int> _columns;
	QHash<QString, QSet<int> >* _scales;

	QSet<int>* _currentScale;

//	QList<QList<bool> > _pianoRoll;
	QList<CheckButtonWidget*> _scaleButtons;
};

}

#endif /* KEYBOARDWIDGET_H_ */
