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
#include <cmath>

#include <QDebug>
#include <QList>
#include <QTimer>
#include <QString>

#include <DoodleSDK/widgets/PushButtonWidget.h>
#include <DoodleSDK/widgets/CheckButtonWidget.h>

#include <DoodleSDK/includes/OSCHelper.h>

#include "MetronomeWidget.h"
#include "Metronome.h"

Doodle::Metronome::Metronome() {
}

void Doodle::Metronome::init(QString rcPath, QHash<QString, QString> params) {
	Q_UNUSED(rcPath);
	_bpm = params["bpm"].toDouble();
	_step_16 = _bpm/16;
	_counter = 0;
}

QStringList Doodle::Metronome::declareObjects() const {
	QStringList l;
	l << "Metronome";
	return l;
}

QStringList Doodle::Metronome::declareGestureRecognizers() const {
	QStringList l;
	return l;
}

QStringList Doodle::Metronome::declareWidgets() const {
	QStringList l;
	return l;
}

void Doodle::Metronome::processResults(QStringList results) {
	Q_UNUSED(results);
}

void Doodle::Metronome::processMessages(QStringList messages) {
	Q_UNUSED(messages);
}

void Doodle::Metronome::addTangible(Tangible* t) {
	_tangibles.insert(t->fiducialID(), t);
	if(_objects[t->fiducialID()] == "Metronome") {
		MetronomeWidget* w = new MetronomeWidget;
		w->setMetronome(t);
		if(!_widgets.contains(w)) {
			_widgets.append(w);
		}
		_metronomeWidget = w;
		emit makeWidget(w);

		float angle = t->angleDeg();
		_bpm = 100*angle/360 + 40;
		_step_16 = (double)60000/(16*_bpm);
		_timer = new QTimer;
		connect(_timer, SIGNAL(timeout()), SLOT(beat()));
		_timer->start(_step_16);
	}
}

void Doodle::Metronome::updateTangible(Tangible* t) {
	_tangibles.insert(t->fiducialID(), t);
	if(_objects[t->fiducialID()] == "Metronome") {
		if(_metronomeWidget != NULL) {
			if(_metronomeWidget->metronome()->fiducialID() == t->fiducialID() && _metronomeWidget->metronome()->sessionID() == t->sessionID()) {
				_metronomeWidget->setMetronome(t);
				float angle = t->angleDeg();
				_bpm = 100*angle/360 + 40;
				_step_16 = (double)60000/(16*_bpm);
				_timer->setInterval(_step_16);
//				_timer->start();
			}
		}
	}
}

void Doodle::Metronome::removeTangible(Tangible* t) {
	// TODO: Save chord list
	_tangibles.remove(t->fiducialID());
	if(_objects[t->fiducialID()] == "Metronome") {
		QSet<Widget*> sub;
		QList<Widget*>::const_iterator wli;
		for(wli = _widgets.begin(); wli != _widgets.end(); ++wli) {

		}
		if(_metronomeWidget != NULL) {
			if(_metronomeWidget->metronome()->fiducialID() == t->fiducialID() && _metronomeWidget->metronome()->sessionID() == t->sessionID()) {
				sub.insert((Widget*)_metronomeWidget);
				_metronomeWidget = NULL;
			}
		}
		QSet<Widget*>::const_iterator wsi;
		for(wsi = sub.begin(); wsi != sub.end(); ++wsi) {
			_widgets.removeAll(*wsi);
			emit destroyWidget(*wsi);
		}

		delete _timer;
	}
}

void Doodle::Metronome::beat() {
	QStringList message;
	message << "Metronome" << "" << "1" << "beat";
	emit deliverMessages(message);
}

Q_EXPORT_PLUGIN2(Metronome, Doodle::Metronome);
