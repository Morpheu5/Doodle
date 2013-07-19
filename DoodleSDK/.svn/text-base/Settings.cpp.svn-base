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

#include <stdexcept>

#include "Settings.h"

#include <QtXmlPatterns>
#include <QtXml>

#include <QFile>

#include <QObject>
#include <QDebug>

Doodle::Settings* Doodle::Settings::s_instance = 0;

Doodle::Settings::Settings(QObject* parent = 0) : QObject(parent) {
	Q_UNUSED(parent);
}

Doodle::Settings::~Settings() {
}

void Doodle::Settings::loadConfig(QString configFile) {
	int i, j, k;

	QFile configXml(configFile);
	QDomDocument doc;
	if(!configXml.open(QIODevice::ReadOnly)) {
		QString e(tr("Unable to open %1."));
		throw std::runtime_error(e.arg(configFile).toAscii().data());
	}
	if(!doc.setContent(&configXml)) {
		configXml.close();
		QString e(tr("Unable to parse %1."));
		throw std::runtime_error(e.arg(configFile).toAscii().data());
	}

	QDomNodeList sections = doc.elementsByTagName("section");
	for(i = 0; i < sections.size(); i++) {
		QDomElement sectionElement = sections.at(i).toElement();
		if(sectionElement.attribute("id") == "system") {
			QDomNodeList params = sectionElement.elementsByTagName("param");
			for(j = 0; j < params.size(); j++) {
				QDomElement param = params.at(j).toElement();
				_system.insert(param.attribute("name").simplified(), param.text().simplified());
			}
		}
		if(sectionElement.attribute("id") == "graphics") {
			QDomNodeList params = sectionElement.elementsByTagName("param");
			for(j = 0; j < params.size(); j++) {
				QDomElement param = params.at(j).toElement();
				_graphics.insert(param.attribute("name").simplified(), param.text().simplified());
			}
		}
		if(sectionElement.attribute("id") == "audio") {
			QDomNodeList params = sectionElement.elementsByTagName("param");
			for(j = 0; j < params.size(); j++) {
				QDomElement param = params.at(j).toElement();
				_audio.insert(param.attribute("name").simplified(), param.text().simplified());
			}
		}
	}
}

QString Doodle::Settings::retrieveConfigParam(QString section, QString param, QString configFile = "config.xml") const {
	QXmlQuery query;
	query.bindVariable("configFile", QVariant(configFile));
	query.bindVariable("section", QVariant(section));
	query.bindVariable("param", QVariant(param));
	query.setQuery("for $i in doc($configFile)/doodle/section[@id=$section]/param[@name=$param]/text() return $i");
	QString result;
	query.evaluateTo(&result);
	return result.trimmed();
}

QString Doodle::Settings::get(QString section, QString param) {
	if(section == "system") {
		if(_system.contains(param)) {
			return _system[param];
		}
	} else if(section == "graphics") {
		if(_graphics.contains(param)) {
			return _graphics[param];
		}
	} else if(section == "audio") {
		if(_audio.contains(param)) {
			return _audio[param];
		}
	} else {
		QString e(tr("Couldn't find param %1 in section %2.").arg(param).arg(section));
		throw std::runtime_error(e.toAscii().data());
	}
}
