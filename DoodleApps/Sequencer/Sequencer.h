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

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include <QtPlugin>
#include <QList>
#include <QHash>
#include <DoodleSDK/includes/IApplication.h>
#include <DoodleSDK/includes/Widget.h>

#include "SequenceButtonWidget.h"

namespace Doodle {
class SequencerWidget;

/*!
 * This application takes the notes on all associated KeyboardWidgets and sends
 * them out via MIDI messages incapsulated into OSC packets.
 *
 * More than this, it also manages the different scales that can fit the Chords
 * from the Tonalizer.
 *
 * Note: this implementation is based on western tonal harmony. It should be
 * just a matter of additional, hopefully trivial, work to extend it to other
 * harmony systems.
 */
class Sequencer : public QObject, public IApplication {
	Q_OBJECT
	Q_INTERFACES(Doodle::IApplication)

	DOODLE_APPLICATION

public:
	/*!
	 * This constructor prepares the _chromaticScale with the twelve half-tones
	 * and also creates a list of available scales, which are subsets of the
	 * chromatic scale.
	 */
	Sequencer();

	QString name() const { return QString("Sequencer"); }
	virtual void init(QString rcPath, QHash<QString, QString> params);
	QStringList declareObjects() const;
	QStringList declareGestureRecognizers() const;
	QStringList declareWidgets() const;

public slots:
	/*!
	 * This routine processes two gestures.
	 *
	 * -	StraightStroke: used to open the KeyboardWidget corresponding to a
	 * 		sequence slot.
	 * -	MusicTouch: used to trigger notes on a KeyboardWidget.
	 *
	 * Both are blocking and StraightStroke has priority over MusicTouch. This
	 * means that if a StraightStroke comes, then the results processing is
	 * stopped and if any MusicTouch result comes after that, it is ignored.
	 */
	void processResults(QStringList results);

	/*!
	 * Gathers information from Tonalizer and Metronome.
	 *
	 * -	Tonalizer: if a new Chord is created, the a new sequence slot is
	 * 		created. If an existing Chord is modified, then the corresponding
	 * 		sequence slot and KeyboardWidget are updated accordingly.
	 * -	Metronome: when a beat comes, advance the current sequence or, if
	 * 		the current sequence is over, start the next one. For each active
	 * 		note, a MIDI over OSC message is sent.
	 */
	void processMessages(QStringList messages);
	void addTangible(Tangible* t);
	void updateTangible(Tangible* t);
	void removeTangible(Tangible* t);

signals:
	void makeWidget(Widget* widget);
	void destroyWidget(Widget* widget);
	void deliverMessages(QStringList messages);

private:
	QVector<QString> _chromaticScale;
	QHash<QString, QSet<int> > _scales;

	SequencerWidget* _sequencerWidget;
	QList<SequenceButtonWidget*> _sequenceList;

	int _sequenceListIndex;
	int _sequenceIndex;

	int _counter;

private slots:
	void publishWidget(Widget* widget);
	void unpublishWidget(Widget* widget);
};

}

#endif /* SEQUENCER_H_ */
