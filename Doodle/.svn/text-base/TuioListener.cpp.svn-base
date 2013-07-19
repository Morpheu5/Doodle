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

#include "TuioListener.h"
#include "TuioProxy.h"

Doodle::TuioListener::TuioListener(int port, TuioProxy* parent) {
	_tuioProxy = parent;

	_tuioClient = new TUIO::TuioClient(port);
	_tuioClient->addTuioListener(this);
	_tuioClient->connect();

	if(!_tuioClient->isConnected()) {
		throw std::runtime_error("Couldn't start TUIO Client on port " + port);
	}
}

Doodle::TuioListener::~TuioListener() {
	_tuioClient->disconnect();
	delete _tuioClient;
}

void Doodle::TuioListener::addTuioObject(TUIO::TuioObject* tobj) { _tuioProxy->addTuioObject(tobj); }
void Doodle::TuioListener::updateTuioObject(TUIO::TuioObject* tobj) { _tuioProxy->updateTuioObject(tobj); }
void Doodle::TuioListener::removeTuioObject(TUIO::TuioObject* tobj) { _tuioProxy->removeTuioObject(tobj); }

void Doodle::TuioListener::addTuioCursor(TUIO::TuioCursor* tcur) { _tuioProxy->addTuioCursor(tcur); }
void Doodle::TuioListener::updateTuioCursor(TUIO::TuioCursor* tcur) { _tuioProxy->updateTuioCursor(tcur); }
void Doodle::TuioListener::removeTuioCursor(TUIO::TuioCursor* tcur) { _tuioProxy->removeTuioCursor(tcur); }

void Doodle::TuioListener::refresh(TUIO::TuioTime ttime) { _tuioProxy->refresh(ttime); }
