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

#ifndef STRAIGHTSTROKE_H_
#define STRAIGHTSTROKE_H_

#include <QtPlugin>
#include <DoodleSDK/includes/IGestureRecognizer.h>
#include <QPointF>
#include <QLineF>

namespace Doodle {

/*!
 * A Gesture Recognizer to detect straight strokes. A straight stroke is a
 * single linear segment, as expected.
 *
 * This is a static Gesture Recognizer.
 */
class StraightStroke : public QObject, public IGestureRecognizer {
	Q_OBJECT
	Q_INTERFACES(Doodle::IGestureRecognizer)

	DOODLE_GESTURERECOGNIZER
	DOODLE_STATIC_GESTURERECOGNIZER

public:
	QString name() const { return QString("StraightStroke"); }
	void init(QString rcPath, QHash<QString, QString> params);

public slots:
	/*!
	 * This takes each Trace in a Group and see if they are linear segments. If
	 * at least one is not, then recognition fails. This is done by computing
	 * the linear segment that connects the first and the last point of the
	 * Trace and checking if each point in the Trace is within a fixed threshold
	 * from this segment. The first that is not makes the whole recognition
	 * fail.
	 */
	void recognize();

signals:
	void result(QStringList result);

private:
	double pointToSegmentDistance(QPointF, QLineF);
};

}

#endif /* StraightStroke_H_ */
