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

#include <QString>
#include <QTimer>

#include <QDebug>

#include <cstdarg>
#include <stdexcept>

#include <oscpack/osc/OscOutboundPacketStream.h>
#include <oscpack/ip/UdpSocket.h>

#include "OSCHelper.h"

Doodle::OSCHelper* Doodle::OSCHelper::s_instance = 0;

#define OSC_BUFFER_SIZE 1024

Doodle::OSCHelper::OSCHelper()
:	QObject(),
	_transmitSocket(IpEndpointName("127.0.01", 4444))
{
	_buffer = new char[OSC_BUFFER_SIZE];
}

Doodle::OSCHelper::~OSCHelper() {
	delete _buffer;
}

void Doodle::OSCHelper::send(QString receiver, QString types, QList<QVariant> params) {
	if(types.length() != params.length()) {
		throw std::runtime_error("[OscHelper] Different amount of types and params passed.");
	}

	osc::OutboundPacketStream p(_buffer, OSC_BUFFER_SIZE);
	p.Clear();
	p << osc::BeginMessage(receiver.toAscii().data());
	int i;
	for(i = 0; i < types.length(); i++) {
		switch(types[i].toAscii()) {
		case 'i':
			p << params[i].toInt();
			break;
		case 'f':
			p << (float)params[i].toDouble();
			break;
		case 's':
			p << params[i].toString().toUtf8().data();
			break;
		case 'd':
			p << params[i].toDouble();
			break;
		case 'T':
		case 'F':
			p << params[i].toBool();
			break;
		}
	}
	p << osc::EndMessage;

	_transmitSocket.Send(p.Data(), p.Size());
}
