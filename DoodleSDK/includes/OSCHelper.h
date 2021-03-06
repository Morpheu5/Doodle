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

#ifndef OSCHELPER_H_
#define OSCHELPER_H_

#include <QObject>
#include <QList>
#include <QVariant>
#include <QString>
#include <oscpack/ip/UdpSocket.h>

namespace Doodle {

/*!
 * A helper class for sending Open Sound Control messages.
 */
class OSCHelper : public QObject {
	Q_OBJECT

public:
	/*!
	 * Standard constructor.
	 */
	OSCHelper();

	/*!
	 * Standard destructor.
	 */
	virtual ~OSCHelper();

	/*!
	 * Implementation of Singleton pattern.
	 *
	 * \return	Instance of this class.
	 */
	static OSCHelper* instance() {
		if(!s_instance) {
			s_instance = new OSCHelper;
		}
		return s_instance;
	}

	/*!
	 * Sends the packet.
	 *
	 * \param	receiver	OSC receiver path.
	 * \param	types		OSC types.
	 * \param	params		List of parameters.
	 */
	void send(QString receiver, QString types, QList<QVariant> params);

protected:
	UdpTransmitSocket _transmitSocket;
	char* _buffer;

private:
	static OSCHelper* s_instance;

};

}

#endif /* OSCHELPER_H_ */
