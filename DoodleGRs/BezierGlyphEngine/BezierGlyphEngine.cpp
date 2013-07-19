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

#include <cmath>
#include <climits>
#include <stdexcept>

#include <QDir>
#include <QFile>
#include <QDomDocument>

#include "BezierGlyphEngine.h"

#include <QDebug>

void Doodle::BezierGlyphEngine::init(QString rcPath) {
	_rcPath = rcPath;
}

QString Doodle::BezierGlyphEngine::recognize(QList<QList<QPointF> > glyph) const {
	// FIXME: Generalize to more than one trace
	if(glyph.isEmpty()) {
		return QString();
	}
	QList<QPointF> trace = glyph.at(0);
	int n = trace.size();
	float t = 0;
	int i,j;
	QList<QPointF> r;
	if(n > 52) {
		/* There seem to be a problem: using 64 bit unsigned integers, it is
		 * possible to handle at most 52 binomial coefficients, that is to say
		 * that choose(52,26) is the biggest coefficient that fits into unsigned
		 * long long.
		 * If by any chance the trace is made of more than 52 points, we have to
		 * shirnk it down and this is done by simply subsampling it at a ratio
		 * that puts in safely under the limit. More sophisticated strategies
		 * can be developed but this is quick and good enough in this case.
		 */
		r.append(trace.first());
		int ratio = (int)ceil((float)((float)n/(float)52));
		for(i = 1; i < n; i+=ratio) {
			r.append(trace.at(i));
		}
		if(r.last() != trace.last()) {
			r.append(trace.last());
		}
	} else {
		r = trace;
	}
	n = r.size()-1;

	// Compute binomial coefficients
	unsigned long long* b = new unsigned long long[n+1];
	for(i = 0; i <= n; i++) {
		try {
			b[i] = choose(n,i);
		} catch (std::runtime_error e) {
			throw e;
		}
	}

	// Compute the constant time samples.
	// TODO: Configure the step
	float t_step = (float)1/(float)100;
	QList<QPointF> B;
	// Smarter techniques may be used (such as Horner's formula), but for the
	// moment, the direct approach will suffice.
	for(t = 0; t < 1-t_step; t += t_step) {
		float x = 0, y = 0;
		for(i = 0; i <= n; i++) {
			x += b[i]*pow(1-t, (n-i))*pow(t, i)*r.at(i).x();
			y += b[i]*pow(1-t, (n-i))*pow(t, i)*r.at(i).y();
		}
		B.append(QPointF(x,y));
	}
	// Normalize B inside unitary square centered in (0,0).
	float mX = 1, MX = 0, mY = 1, MY = 0;
	for(i = 0; i < B.size(); i++) {
		mX = mX < B.at(i).x()? mX : B.at(i).x();
		MX = MX > B.at(i).x()? MX : B.at(i).x();
		mY = mY < B.at(i).y()? mY : B.at(i).y();
		MY = MY > B.at(i).y()? MY : B.at(i).y();
	}
	for(i = 0; i < B.size(); i++) {
		B.replace(i, QPointF((B.at(i).x()-mX)*(1/(MX-mX))-0.5f, (B.at(i).y()-mY)*(1/(MY-mY))-0.5f));
	}

	// Compute the orientation for the given glyph.
	QPointF uO = B.last() - B.first();

	// Load the models.
	// FIXME: Preload the models
	QDomDocument glyphsXml;
	QDir dir(_rcPath);
	QFile glyphsFile(dir.canonicalPath()+"/BezierGlyphs.xml");
	if(!glyphsFile.open(QIODevice::ReadOnly)) {
		QString e(tr("[BezierGlyphEngine] Couldn't load glyphs definition file (%1: %2)").arg(glyphsFile.fileName()).arg(glyphsFile.errorString()));
		throw std::runtime_error(e.toAscii().data());
	}
	if(!glyphsXml.setContent(&glyphsFile)) {
		QString e(tr("[BezierGlyphEngine] Couldn't parse glyphs definition file (%1: %2").arg(glyphsFile.fileName()).arg(glyphsFile.errorString()));
		glyphsFile.close();
		throw std::runtime_error(e.toAscii().data());
	}
	glyphsFile.close();
	// Compare the given glyph with the models.
	QDomNodeList modelGlyph = glyphsXml.elementsByTagName("glyph");
	int score, oldScore = INT_MIN;
	QString resultingGlyph;
	for(i = 0; i < modelGlyph.size(); i++) {
		QList<QPointF> G = B;

		// Build a list of the points of the model.
		QDomNodeList modelPoints = modelGlyph.at(i).firstChild().childNodes();
		QList<QPointF> M;
		for(j = 0; j < modelPoints.size(); j++) {
			M.append(QPointF(modelPoints.at(j).attributes().namedItem("x").nodeValue().toFloat(), modelPoints.at(j).attributes().namedItem("y").nodeValue().toFloat()));
		}
		// It is important that the model and the input have the same number of
		// points. This is a restriction, but it allows for a straightforward
		// and fast approach.
		if(G.size() != M.size()) {
			QString e(tr("[BezierGlyphEngine] Input has %1 points while Model %2 has %3 points.").arg(G.size()).arg(modelGlyph.at(i).attributes().namedItem("id").nodeValue()).arg(M.size()));
			throw std::runtime_error(e.toAscii().data());
		}

		// Compute the orientation vector for the current model.
		QPointF mO = M.last() - M.first();

		// Determine whether the rotation has to be ccw or cw.
		int ccw = 1;
		if(mO.x()*uO.y() - mO.y()*uO.x() < 0) {
			ccw = -1;
		}
		// Compute the angle.
		float theta = ccw*acos(dot(uO, mO)/(length(uO)*length(mO)));
		float cosTheta = cos(theta);
		float sinTheta = sin(theta);
		// Apply the rotation.
		for(j = 0; j < G.size(); j++) {
			float ox = G.at(j).x();
			float oy = G.at(j).y();
			float nx = ox*cosTheta + oy*sinTheta;
			float ny =-ox*sinTheta + oy*cosTheta;
			G.replace(j, QPointF(nx, ny));
		}

		/* Finally test user's input against the model.
		 * Since the two glyphs have the same number of points, the score is
		 * simply the sum of the corresponding points that are within a given
		 * threshold. */
		score = 0;
		QList<QPointF> matches;
		for(j = 0; j < G.size(); j++) {
			// TODO: Configure the threshold
			float d = distance(G.at(j), M.at(j));
			if(d < 0.2) {
				score += 1;
				matches.append(G.at(j));
			} else {
				score -= 1;
			}
			if(d < 0.1) {
				score += 4;
				matches.append(G.at(j));
			} else {
				score -= 1;
			}
		}
		// The higher the score, the most likely is the given model to match
		// user's input.
		if(score > oldScore) {
			oldScore = score;
			resultingGlyph = modelGlyph.at(i).attributes().namedItem("id").nodeValue();
		}
//		qDebug() << score << modelGlyph.at(i).attributes().namedItem("id").nodeValue();
//		dumpImage(G, M, matches, "comp_" + modelGlyph.at(i).attributes().namedItem("id").nodeValue() + "-" + modelGlyph.at(i).attributes().namedItem("variant").nodeValue());
	}
//	dumpImage(B, "input");
//	std::cout << "Bezier: " << resultingGlyph.toAscii().data() << std::endl;
	return resultingGlyph;
}

unsigned long long Doodle::BezierGlyphEngine::choose(unsigned n, unsigned k) const {
	// FIXME: Check if this is the best version
	if(k > n) {
		throw std::runtime_error("[Glyph] k can never be greater than n in choose(n,k)");
	}

	if(k > n/2) {
		k = n-k;
	}

	long double a = 1;
	unsigned i;
	for(i = 1; i <= k; i++) {
		a = a * (n-k+i) / i;
	}
	return (unsigned long long)ceil(a);
}

float Doodle::BezierGlyphEngine::distance(QPointF a, QPointF b) const {
	return sqrt(pow(a.x()-b.x(), 2) + pow(a.y()-b.y(), 2));
}

float Doodle::BezierGlyphEngine::length(QPointF a) const {
	return distance(a, QPointF(0,0));
}

float Doodle::BezierGlyphEngine::dot(QPointF a, QPointF b) const {
	return (a.x() * b.x()) + (a.y() * b.y());
}

Q_EXPORT_PLUGIN2(BezierGlyphEngine, Doodle::BezierGlyphEngine);
