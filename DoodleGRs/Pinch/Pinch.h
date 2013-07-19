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

#ifndef PINCH_H_
#define PINCH_H_

#include <QtPlugin>
#include <DoodleSDK/includes/IGestureRecognizer.h>

namespace Doodle {

/*!
 * This recognizes the well know Pinch gesture which is used to zoom in and out
 * objects as well as to drag them around.
 *
 * It recognizes a pair of Cursors over the same Widget and, if it is pinchable,
 * then zooming and draggin are applied.
 *
 * This is a progressive Gesture Recognizer.
 */
class Pinch : public QObject, public IGestureRecognizer {
	Q_OBJECT
	Q_INTERFACES(Doodle::IGestureRecognizer)

	DOODLE_GESTURERECOGNIZER
	DOODLE_PROGRESSIVE_GESTURERECOGNIZER

public:
	QString name() const { return QString("Pinch"); }
	void init(QString rcPath, QHash<QString, QString> params);

public slots:
	void recognize();

signals:
	void result(QStringList result);

private:
	bool _first;
	QPointF _lastPosition;
	float _lastAngle;
	float _lastZoom;
};

}

#endif /* GLYPH_H_ */
