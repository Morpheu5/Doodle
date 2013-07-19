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

#ifndef BEZIERGLYPHENGINE_H_
#define BEZIERGLYPHENGINE_H_

#include <QObject>
#include "IGlyphEngine.h"

namespace Doodle {

/*!
 * A Glyph Recognizer Engine that implements a Bézier approximation method.
 */
class BezierGlyphEngine : public QObject, public IGlyphEngine {
	Q_OBJECT
	Q_INTERFACES(Doodle::IGlyphEngine)

public:
	void init(QString rcPath);

	/*!
	 * This is the core of the engine. Here's a brief sketch of the algorithm.
	 *
	 * -#	Take the first Glyph.
	 * 		-	Note: in current implementation, it actually takes just the
	 * 			first Trace of the Glyph. This is because the models to match
	 * 			against are made of a single Bézier curve. It should be trivial
	 * 			to extend this to examine all the Traces.
	 * -#	If necessary, decimate the number of points to less than 52 (see
	 * 		comment in the code).
	 * -#	Let \f$n\f$ be the number of points \f$ - 1\f$ after decimation. Use
	 * 		them to make Bézier curve of degree \f$n\f$.
	 * -#	Compute \f$N\f$ constant time samples of user's Glyph.
	 * 		-	Note: \f$N\f$ is an arbitrary number, still it's fixed to 100 in
	 * 			the current implementation. This is deliberate and there's no
	 * 			valid reason for it being a 100. \f$M\f$ is also the number of
	 * 			constant time samples the models are made of. Ideally this
	 * 			number should be fed via configuration file but still it needs
	 * 			to be the same for both pre-computed models and run-time
	 * 			computed glyphs, at least using the metric defined in this
	 * 			implementation.
	 * -#	Normalize the constant time samples inside the unitary square.
	 * -#	Compute the orientation of the Glyph.
	 * 		-	Note: orientation is defined as the vector starting at the first
	 * 			point and ending at the last point of the Glyph.
	 * -#	Load the models. For each model, align them according to input's
	 * 		orientation and compare one by one the points, one from the model
	 * 		and one from the Glyph to be recognized. Keep count of how many such
	 * 		pairs are within a given distance to each other. The model with the
	 * 		higher score is the best match.
	 */
	QString recognize(QList<QList<QPointF> > glyph) const;

	/*!
	 * The Bezier Engine doesn't need any bounding box.
	 *
	 * \return	Always false.
	 */
	bool needsPad() const { return false; }

private:
	/*!
	 * Simple binomia coefficient implementation.
	 *
	 * \return	choose(n,k)
	 */
	unsigned long long choose(unsigned n, unsigned k) const;

	/*!
	 * Computes the distance between two points.
	 *
	 * \param	a	First point.
	 * \param	b	Second point.
	 */
	float distance(QPointF a, QPointF b) const;

	/*!
	 * Computes the distance between a point and QPointF(0,0), that is the
	 * length of the vector identified by the point.
	 *
	 * \param	a	The vector.
	 *
	 * \return	Length of vector a.
	 */
	float length(QPointF a) const;

	/*!
	 * Dot product between two vectors identified by points.
	 *
	 * \param	a	First vector.
	 * \param	b	Second vector.
	 *
	 * \return	Their dot product.
	 */
	float dot(QPointF a, QPointF b) const;
};

}

#endif /* BEZIERGLYPHENGINE_H_ */
