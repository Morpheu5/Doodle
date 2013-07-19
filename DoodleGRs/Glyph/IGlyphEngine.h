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

#ifndef IENGINE_H_
#define IENGINE_H_

#include <QtPlugin>
#include <QList>
#include <QPointF>
#include <QString>

namespace Doodle {

/*!
 * Qt Plugin Interface for Glyph Recognition Engines.
 *
 * A Glyph Recognition Engine takes a list of Strokes and tries to find which
 * glyph approximates it best.
 *
 * It must be noted that, in order to help development of different algorithms,
 * the recognition function doesn't take a Group of Traces as input. Instead it
 * takes a list of lists of points, thus effectively decoupling development of
 * glyph recognition algorithms from that of Doodle.
 */
class IGlyphEngine {
public:
	/*!
	 * Initializes the Glyph Recognition Engine.
	 *
	 * \param	rcPath	Path of resource files.
	 */
	virtual void init(QString rcPath) = 0;

	/*!
	 * This is the core of the engine. It takes a glyph and tries to recognize
	 * which one it is. The key point is that it is not important how the
	 * recognition step is done. What matters is that the message returned is
	 * either a meaningful glyph or an error.
	 */
	virtual QString recognize(QList<QList<QPointF> > glyph) const = 0;

	/*!
	 * In case the algorithm needs a boundary space as a reference, some sort of
	 * pad Widget in which the users can draw glyphs should be provided.
	 *
	 * \return	True if a boundary Widget is needed.
	 */
	virtual bool needsPad() const = 0;

protected:
	QString _rcPath;
};

}

Q_DECLARE_INTERFACE(Doodle::IGlyphEngine, "Doodle.IGlyphEngine/1.0");

#endif /* IENGINE_H_ */
