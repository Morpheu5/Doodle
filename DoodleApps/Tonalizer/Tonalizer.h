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

#ifndef TONALIZER_H_
#define TONALIZER_H_

#include <QtPlugin>
#include <QList>
#include <QHash>
#include <DoodleSDK/includes/IApplication.h>
#include <DoodleSDK/includes/Widget.h>

#include "ChordWidget.h"

#include "Chord.h"

namespace Doodle {
class TonalizerWidget;

/*!
 * The Tonalizer Application. It is responsible for all the editing actions
 * about chords.
 */
class Tonalizer : public QObject, public IApplication {
	Q_OBJECT
	Q_INTERFACES(Doodle::IApplication)

	DOODLE_APPLICATION

public:
	/*!
	 * Standard constructor.
	 */
	Tonalizer();

	QString name() const { return QString("Tonalizer"); }
	virtual void init(QString rcPath, QHash<QString, QString> params);
	QStringList declareObjects() const;
	QStringList declareGestureRecognizers() const;
	QStringList declareWidgets() const;

public slots:
	/*!
	 * This manages three Gesture Recognizers.
	 *
	 * -	Glyph: creates a new chord or edits an existing one, depending on
	 * 		the message.
	 * -	MultiTap: when tapped once, creates a PadWidget if none exists.
	 * -	StraightStroke: used to access the ChordWidget for a particular
	 * 		Chord slot.
	 */
	void processResults(QStringList);
	void processMessages(QStringList);
	void addTangible(Tangible* t);
	void updateTangible(Tangible* t);
	void removeTangible(Tangible* t);

	/*!
	 * If a PadWidget is tapped and we are in building mode, enter editing
	 * mode.
	 *
	 * \param	n	Unused.
	 */
	void padTapped(int n);

	/*!
	 * If a CheckButtonWidget belonging to a ChordWidget is checked, perform the
	 * the corresponding action on the corresponding Chord.
	 */
	void buttonChecked();

	/*!
	 * If a CheckButtonWidget belonging to a ChordWidget is unchecked, perform
	 * the corresponding action on the corresponding Chord.
	 */
	void buttonUnchecked();

	/*!
	 * If a PushButtonWidget belonging to a ChordWidget is pushed, perform the
	 * corresponding action on the corresponding Chord.
	 */
	void buttonPushed();

	/*!
	 * A tap on the ChordWidget toggles its enabled/visible status. If tapped
	 * once, it is enabled/disabled, if tapped twice (or with two simultaneous
	 * fingers) it is hid.
	 */
	void chordWidgetTapped(int);

	/*!
	 * If a Chord slot is checked, then it is active and used for playing.
	 */
	void chordButtonChecked();

	/*!
	 * If a Chord slot is unchecked, the it is inactive and not used for
	 * playing.
	 */
	void chordButtonUnchecked();

signals:
	void makeWidget(Widget* widget);
	void destroyWidget(Widget* widget);
	void deliverMessages(QStringList messages);

private:
	TonalizerWidget* _tonalizerWidget;

	bool _padNeeded;
	bool _havePad;
	bool _buildingChord;

	QList<Chord*> _chords;

	ChordWidget* _currentChordWidget;
};

}

#endif /* TONALIZER_H_ */
