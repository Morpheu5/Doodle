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

#ifndef METRONOME_H_
#define METRONOME_H_

#include <QtPlugin>
#include <QList>
#include <QHash>
#include <DoodleSDK/includes/IApplication.h>
#include <DoodleSDK/includes/Widget.h>

namespace Doodle {
class MetronomeWidget;

/*!
 * This application repeats a message at a given rate.
 */
class Metronome : public QObject, public IApplication {
	Q_OBJECT
	Q_INTERFACES(Doodle::IApplication)

	DOODLE_APPLICATION

public:
	Metronome();

	QString name() const { return QString("Metronome"); }
	virtual void init(QString rcPath, QHash<QString, QString> params);
	QStringList declareObjects() const;
	QStringList declareGestureRecognizers() const;
	QStringList declareWidgets() const;

public slots:
	void processResults(QStringList results);
	void processMessages(QStringList messages);
	void addTangible(Tangible* t);
	void updateTangible(Tangible* t);
	void removeTangible(Tangible* t);

	/*!
	 * Slot connected with QTimer expiration. Each time the QTimer expires, it
	 * sends a signal that triggers this slot that, in turn, broadcasts its
	 * beat message.
	 */
	void beat();

signals:
	void makeWidget(Widget* widget);
	void destroyWidget(Widget* widget);
	void deliverMessages(QStringList messages);

private:
	MetronomeWidget* _metronomeWidget;

	double _bpm;
	double _step_16;

	int _counter;

	QTimer* _timer;

};

}

#endif /* SEQUENCER_H_ */
