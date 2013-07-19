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
#include <stdexcept>

#include <QDebug>
#include <QDir>
#include <QVariant>
#include <QPluginLoader>

#include <DoodleSDK/includes/Widget.h>

#include "Glyph.h"
#include "IGlyphEngine.h"

void Doodle::Glyph::init(QString rcPath, QHash<QString, QString> params) {
	_rcPath = rcPath;
	_engineName = params["engine"];
	_enginesPath = params["enginesPath"];
	QPluginLoader loader;
	loader.setLoadHints(QLibrary::ExportExternalSymbolsHint);
	loader.setFileName(_enginesPath+"/lib"+_engineName+"GlyphEngine.so");
	QObject* plugin = loader.instance();
	if(plugin) {
		_engine = qobject_cast<IGlyphEngine*>(plugin);
		if(_engine) {
			_engine->init(rcPath);
		}
	} else {
		QString e(tr("[Glyph] Unable to load desired engine: %1.").arg(loader.errorString()));
		throw std::runtime_error(e.toAscii().data());
	}
}

void Doodle::Glyph::recognize() {
	Group g = _sGroups.takeFirst();
	double angle = 0.0;
	double size = 1.0;
	QPointF position(0.0, 0.0);
	if(_engine->needsPad()) {
		if(!g.finalizedTraces()[0].isOnWidget()) {
			return;
		}
		// FIXME: Use qobject_cast, maybe?
		Widget* w = g.finalizedTraces()[0].widget();
		if(w->name() != "PadWidget") {
			return;
		}
		angle = - w->angle();
		size = w->size();
		QPointF position = w->position();
	}
	QList<QList<QPointF> > input;

	foreach(Trace t, g.finalizedTraces()) {
		QList<QPointF> trace;
		foreach(Stroke s, t.strokes()) {
			int i;
			for(i = 0; i < s.size(); i++) {
				QPointF p = (s[i].position() - position)/size;
				double costheta = cos(angle); double sintheta = sin(angle);
				QPointF q;
				q.setX((p.x()*costheta - p.y()*sintheta));
				q.setY((p.x()*sintheta + p.y()*costheta));
				trace.append(q);
			}
		}
		input.append(trace);
	}
	QStringList resultPacket;
	QStringList objects;
	foreach(long l, g.objects()) {
		objects.append(QString::number(l));
	}
	QString r = _engine->recognize(input);
	if(_engine->needsPad()) {
		resultPacket << "Glyph" << objects.join(",") << QString::number(g.finalizedTraces()[0].widget()->id()) << "1" << r;
	} else {
		resultPacket << "Glyph" << objects.join(",") << "" << "1" << r;
	}
	emit result(resultPacket);
};

Q_EXPORT_PLUGIN2(Glyph, Doodle::Glyph);
