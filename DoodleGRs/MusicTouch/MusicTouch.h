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

#ifndef MUSICTOUCH_H_
#define MUSICTOUCH_H_

#include <QtPlugin>
#include <DoodleSDK/includes/IGestureRecognizer.h>
#include <QPointF>
#include <QLineF>

namespace Doodle {

/*!
 * The Gesture Recognizer that implements the drawing gestures for composing
 * melodies.
 *
 * This is a static Gesture Recognizer.
 */
class MusicTouch : public QObject, public IGestureRecognizer {
	Q_OBJECT
	Q_INTERFACES(Doodle::IGestureRecognizer)

	DOODLE_GESTURERECOGNIZER
	DOODLE_STATIC_GESTURERECOGNIZER

public:
	QString name() const { return QString("MusicTouch"); }
	void init(QString rcPath, QHash<QString, QString> params);

public slots:
	/*!
	 * The current implementation recognizes both taps and strokes. Taps are
	 * simply matched against a KeyboardWidget and the corresponding keys are
	 * activated.
	 *
	 * Recognition of strokes is a different story. At the current state, it
	 * recognizes where the stroke as zero derivative with respect to the
	 * orientation of the KeyboardWidget it is drawn over. More sophisticated
	 * clue points can be implemented, such as slowing down or changes in size
	 * of the Cursor (when and if TUIO protocol will support this feature).
	 *
	 * As it is easily noted, the current implementation is quite naïve, so the
	 * recognition is not very accurate. Smoothing, Spline approximation and
	 * other sophisticated methods can be applied to make recognition work
	 * better.
	 */
	void recognize();

signals:
	void result(QStringList result);
};

}

#endif /* MUSICTOUCH_H_ */
