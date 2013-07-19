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

#ifndef GLYPH_H_
#define GLYPH_H_

#include <QtPlugin>
#include <DoodleSDK/includes/IGestureRecognizer.h>

namespace Doodle {
class IGlyphEngine;

/*!
 * This is the Gesture Recognizer that recognizes glyph handwriting.
 *
 * It takes a Group of Traces and uses an engine to analyze it. Such engine is
 * a plugin itself, so any kind of different and more or less sophisticated
 * recognition algorithms can be implemented.
 *
 * This is a static Gesture Recognizer.
 */
class Glyph : public QObject, public IGestureRecognizer {
	Q_OBJECT
	Q_INTERFACES(Doodle::IGestureRecognizer)

	DOODLE_GESTURERECOGNIZER
	DOODLE_STATIC_GESTURERECOGNIZER

public:
	QString name() const { return QString("Glyph"); }
	void init(QString rcPath, QHash<QString, QString> params);

public slots:
	void recognize();

signals:
	void result(QStringList result);

private:
	QString _engineName;
	QString _enginesPath;
	IGlyphEngine* _engine;
};

}

#endif /* GLYPH_H_ */
