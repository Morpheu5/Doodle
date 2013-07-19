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

#include <QImage>
#include <QPainter>

#include <QDir>
#include <QFile>
#include <QDomDocument>
#include <QHash>
#include <QSet>

#include "PolyGlyphEngine.h"

#include <QDebug>

void Doodle::PolyGlyphEngine::init(QString rcPath) {
	_rcPath = rcPath;
	QDomDocument glyphsXml;
	QDir dir(_rcPath);
	QFile glyphsFile(dir.canonicalPath()+"/PolyGlyphs.xml");
	if(!glyphsFile.open(QIODevice::ReadOnly)) {
		QString e(tr("[PolyGlyphEngine] Couldn't load glyphs definition file (%1: %2)").arg(glyphsFile.fileName()).arg(glyphsFile.errorString()));
		throw std::runtime_error(e.toAscii().data());
	}
	if(!glyphsXml.setContent(&glyphsFile)) {
		QString e(tr("[PolyGlyphEngine] Couldn't parse glyphs definition file (%1: %2").arg(glyphsFile.fileName()).arg(glyphsFile.errorString()));
		glyphsFile.close();
		throw std::runtime_error(e.toAscii().data());
	}
	glyphsFile.close();
	QDomNodeList glyphList = glyphsXml.elementsByTagName("glyph");
	int i;
	for(i = 0; i < glyphList.size(); i++) {
		QList<QPolygonF> glyph;
		QDomNodeList polygonList = glyphList.at(i).toElement().elementsByTagName("polygon");
		int j;
		for(j = 0; j < polygonList.size(); j++) {
			QPolygonF polygon;
			QDomNodeList vertexList = polygonList.at(j).toElement().elementsByTagName("vertex");
			int k;
			for(k = 0; k < vertexList.size()+1; k++) {
				QDomElement vertexElement = vertexList.at(k%3).toElement();
				float x = vertexElement.attribute("x").toFloat();
				float y = vertexElement.attribute("y").toFloat();
				polygon << QPointF(x,-y);
			}
			glyph.append(polygon);
		}
		_glyphTable.insertMulti(glyphList.at(i).toElement().attribute("name"), glyph);
	}
}

QString Doodle::PolyGlyphEngine::recognize(QList<QList<QPointF> > glyph) const {
	int score, oldScore = INT_MIN;
	QString resultingGlyph;

	foreach(QString key, _glyphTable.uniqueKeys()) {
		foreach(QList<QPolygonF> model, _glyphTable.values(key)) {
			score = 0;
			foreach(QPolygonF polygon, model) {
				bool hit = false;
				foreach(QList<QPointF> stroke, glyph) {
					foreach(QPointF point, stroke) {
						if(polygon.containsPoint(point, Qt::OddEvenFill)) {
							score += 1;
							hit = true;
						} else {

						}
					}
				}
				if(!hit) {
					// Assign a penalty
				}
			}
			if(score > oldScore) {
				oldScore = score;
				resultingGlyph = key;
			}
		}
	}

	return resultingGlyph;
}

unsigned long long Doodle::PolyGlyphEngine::choose(unsigned n, unsigned k) const {
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

float Doodle::PolyGlyphEngine::distance(QPointF a, QPointF b) const {
	return sqrt(pow(a.x()-b.x(), 2) + pow(a.y()-b.y(), 2));
}

float Doodle::PolyGlyphEngine::length(QPointF a) const {
	return distance(a, QPointF(0,0));
}

float Doodle::PolyGlyphEngine::dot(QPointF a, QPointF b) const {
	return (a.x() * b.x()) + (a.y() * b.y());
}

Q_EXPORT_PLUGIN2(PolyGlyphEngine, Doodle::PolyGlyphEngine);
