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

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>

namespace Doodle {

/*!
 * A class for management of XML configuration files. For practical purposes,
 * this class is a QObject.
 */
class Settings : public QObject {
public:
	/*!
	 * Standard constructor.
	 *
	 * \param	parent	Parent of the object.
	 */
	Settings(QObject* parent);

	/*!
	 * Standard destructor.
	 */
	virtual ~Settings();

	/*!
	 * Implementation of Singleton pattern.
	 *
	 * \return	Instance of this class.
	 */
	static Settings* instance() {
		if(!s_instance) {
			s_instance = new Settings(0);
		}
		return s_instance;
	}

	/*!
	 * Loads configuration from an XML file.
	 *
	 * \param	configFile	Name of the configuration file.
	 */
	void loadConfig(QString configFile);

	/*!
	 * Returns a parameter.
	 *
	 * \param	section	The section in which the parameter is supposed to be.
	 * \param	param	The name of the parameter.
	 *
	 * \return	The parameter.
	 */
	QString get(QString section, QString param);

protected:
	QHash<QString, QString> _system;
	QHash<QString, QString> _graphics;
	QHash<QString, QString> _audio;

private:
	QString retrieveConfigParam(QString, QString, QString) const;

	static Settings* s_instance;
};

}

#endif /* SETTINGS_H_ */
