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

#ifndef POLYGLYPHENGINE_H_
#define POLYGLYPHENGINE_H_

#include <QObject>
#include <QList>
#include <QPolygonF>
#include <QHash>
#include "IGlyphEngine.h"

namespace Doodle {

/*!
 * This is another Glyph Recognition Engine based on polygonal models.
 *
 * It doesn't work very well and it needs the user to draw over a PadWidget, so
 * it's included just to demonstrate that the Glyph Recognition Engines are
 * plugins.
 */
class PolyGlyphEngine : public QObject, public IGlyphEngine {
	Q_OBJECT
	Q_INTERFACES(Doodle::IGlyphEngine)

public:
	void init(QString);

	/*!
	 * The algorithm is quite simple.
	 *
	 * -#	Take user's Glyph, which is assumed to be drawn on a PadWidget.
	 * -#	For each model, try to match user's input with a model by
	 * 		subsequently rotating it of 90 degrees.
	 * 		-	Note: this way we match it four times each because we don't know
	 * 			in which direction the user has drawn the glyph.
	 * -#	The model that contains the highest number of input's points is the
	 * 		best match.
	 *
	 * It is clear that at present moment this is a poor method. It may be
	 * faster than Bézier Engine in theory, but it is much less effective, so it
	 * was never used for practical purposes.
	 */
	QString recognize(QList<QList<QPointF> > glyph) const;

	/*!
	 * This needs a writing pad.
	 */
	bool needsPad() const { return true; }

private:
	unsigned long long choose(unsigned n, unsigned k) const;
	float distance(QPointF a, QPointF b) const;
	float length(QPointF a) const;
	float dot(QPointF a, QPointF b) const;

	QHash<QString, QList<QPolygonF> > _glyphTable;
};

}

#endif /* POLYGLYPHENGINE_H_ */
