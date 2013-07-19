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

#ifndef MULTITAP_H_
#define MULTITAP_H_

#include <QtPlugin>
#include <DoodleSDK/includes/IGestureRecognizer.h>

namespace Doodle {

/*!
 * A Gesture Recognizer for detecting multiple taps.
 *
 * This is a static Gesture Recognizer.
 */
class MultiTap : public QObject, public IGestureRecognizer {
	Q_OBJECT
	Q_INTERFACES(Doodle::IGestureRecognizer)

	DOODLE_GESTURERECOGNIZER
	DOODLE_STATIC_GESTURERECOGNIZER

public:
	QString name() const { return QString("MultiTap"); }
	void init(QString rcPath, QHash<QString, QString> params);

public slots:
	/*!
	 * A Tap is detected when a Trace is made of few points or it is contained
	 * within a bounding box that is almost square and small.
	 *
	 * If a Group is made of \f$N\f$ Traces that are Taps, then \f$N\f$ taps are
	 * detected.
	 */
	void recognize();

signals:
	void result(QStringList results);
};

}

#endif /* MULTITAP_H_ */
