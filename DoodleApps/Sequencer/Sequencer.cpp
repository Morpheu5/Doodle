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

#include "SequencerWidget.h"
#include "KeyboardWidget.h"
#include "Sequencer.h"

Doodle::Sequencer::Sequencer() {
	_chromaticScale << "C" << "C#" << "D" << "D#" << "E" << "F" << "F#" << "G" << "G#" << "A" << "A#" << "B";

	QSet<int> scale;
	scale << 0 << 2 << 4 << 5 << 7 << 9 << 11;
	_scales.insert("diatonic major", scale);
	scale.clear();

	scale << 0 << 2 << 3 << 5 << 7 << 8 << 10;
	_scales.insert("diatonic minor", scale);
	scale.clear();

	scale << 0 << 2 << 4 << 7 << 9;
	_scales.insert("pentatonic major", scale);
	scale.clear();

	scale << 0 << 3 << 5 << 7 << 10;
	_scales.insert("pentatonic minor", scale);
	scale.clear();

	scale << 0 << 2 << 4 << 6 << 8 << 10;
	_scales.insert("whole tone", scale);
	scale.clear();

	scale << 0 << 2 << 3 << 5 << 6 << 8 << 9 << 11;
	_scales.insert("diminished whole", scale);
	scale.clear();

	scale << 0 << 1 << 3 << 4 << 6 << 7 << 9 << 10;
	_scales.insert("diminished half", scale);
	scale.clear();

	scale << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11;
	_scales.insert("chromatic", scale);
	scale.clear();

	QHash<QString, QSet<int> >::iterator it;
	for(it = _scales.begin(); it != _scales.end(); ++it) {
		QSet<int> s = *it;
		foreach(int i, s) {
			it->insert(i + 12);
		}
	}

	_counter = 0;
	_sequenceListIndex = 0;
	_sequenceIndex = 0;
}

void Doodle::Sequencer::init(QString rcPath, QHash<QString, QString> params) {
	Q_UNUSED(rcPath);
	Q_UNUSED(params);

//	KeyboardWidget* keyb = new KeyboardWidget;
//	connect(keyb, SIGNAL(childAdded(Widget*)), SLOT(publishWidget(Widget*)));
//	keyb->init();
//	keyb->setPosition(QPointF(0.5, 0.5));
//	keyb->setVisible(true);
//	_widgets.append(keyb);
//	emit makeWidget(keyb);
}

QStringList Doodle::Sequencer::declareObjects() const {
	QStringList l;
	l << "Sequencer";
	return l;
}

QStringList Doodle::Sequencer::declareGestureRecognizers() const {
	QStringList l;
	l << "MultiTap" << "StraightStroke" << "MusicTouch" << "_RawStrokes";
	return l;
}

QStringList Doodle::Sequencer::declareWidgets() const {
	QStringList l;
	l << "KeyboardWidget";
	return l;
}

void Doodle::Sequencer::processResults(QStringList results) {
	QString name;
	QList<long> objects;
	QList<unsigned long> widgets;
	int skip;
	while(!results.isEmpty()) {
		name = results.takeFirst();
		objects.clear();
		QString l = results.takeFirst();
		foreach(QString s, l.split(',', QString::SkipEmptyParts)) {
			objects << s.toLong();
		}
		widgets.clear();
		l = results.takeFirst();
		foreach(QString s, l.split(',', QString::SkipEmptyParts)) {
			widgets << s.toULong();
		}
		skip = results.takeFirst().toInt();

		bool accept = acceptable(objects, widgets);

		if(!accept) {
			QStringList::iterator i = results.begin();
			results.erase(i, i + skip);
			continue;
		}

		if(name == "MultiTap") {
			int n = results.takeFirst().toInt();
			switch(n) {
			case 1:
				break;
			default:
				QStringList::iterator i = results.begin();
				results.erase(i, i + n*2);
			}
			// Make MultiTap blocker.
			return;
		} else if(name == "StraightStroke") {
			// Make StraightStroke blocker.
			int n = results.takeFirst().toInt();
			switch(n) {
			case 1: {
				float length = results.takeFirst().toFloat();
				Q_UNUSED(length);
				float x1 = results.takeFirst().toFloat();
				float y1 = results.takeFirst().toFloat();
				float x2 = results.takeFirst().toFloat();
				float y2 = results.takeFirst().toFloat();
				QLineF line(x1,y1,x2,y2);
				foreach(unsigned long id, widgets) {
					foreach(SequenceButtonWidget* w, _sequenceList) {
						if(w->id() == id) {
							KeyboardWidget& kw = w->keyboard();
							kw.setVisible(true);
							kw.setPosition(line.p2());
						}
					}
				}
			}
			}
			return;
		} else if(name == "MusicTouch") {
			int n = results.takeFirst().toInt();
			int i;
			for(i = 0; i < n; i++) {
				float x = results.takeFirst().toFloat();
				float y = results.takeFirst().toFloat();
				QPointF p(x,y);
				foreach(SequenceButtonWidget* sbw, _sequenceList) {
					foreach(Widget* w, sbw->keyboard().children()) {
						if(w->contains(p)) {
							if(CheckButtonWidget* cbw = qobject_cast<CheckButtonWidget*>(w)) {
								cbw->tap(1);
							}
						}
					}
				}
			}
			return;
		} else {
			qDebug() << this << "Skipping" << skip << "params because I don't know" << name;
			QStringList::iterator i = results.begin();
			results.erase(i, i + skip);
		}
	}
	return;
}

void Doodle::Sequencer::processMessages(QStringList messages) {
	while(!messages.isEmpty()) {
		QString sender = messages[0];
		QString receiver = messages[1];
		int params = messages[2].toInt() + 3;
		QString method = messages[3];
		if(sender == "Tonalizer") {
			if(method == "NewChord") {
				int b = _chromaticScale.lastIndexOf(messages[4]);
				QSet<int> chord;
				QStringList::iterator i;
				for(i = messages.begin() + 5; i != messages.begin() + params; ++i) {
					chord.insert((*i).toInt());
				}
				SequenceButtonWidget* w = new SequenceButtonWidget;
				w->setSize(20,10);
				w->setChecked(true);
				w->setTonic(messages[4]);
//				connect(w, SIGNAL(checked()), SLOT(sequenceButtonChecked()));
//				connect(w, SIGNAL(unchecked()), SLOT(sequencerButtonUnchecked()));
				_sequenceList.append(w);
				_widgets.append(w);
				emit makeWidget(w);

				connect(&w->keyboard(), SIGNAL(childAdded(Widget*)), SLOT(publishWidget(Widget*)));
				connect(&w->keyboard(), SIGNAL(childDestroyed(Widget*)), SLOT(unpublishWidget(Widget*)));
				_widgets.append(&w->keyboard());
				emit makeWidget(&w->keyboard());

				QHash<QString, QSet<int> > scales;
				foreach(QString name, _scales.keys()) {
					foreach(QSet<int> scale, _scales.values(name)) {
						QSet<int> ts = scale;
						if(ts.intersect(chord) == chord) {
							scales.insert(name, scale);
						}
					}
				}
				w->setScales(scales);
				w->keyboard().setVisible(false);
			}
			if(method == "EditChord") {
				int b = _chromaticScale.lastIndexOf(messages[5]);
				QSet<int> chord;
				QStringList::iterator i;
				for(i = messages.begin() + 6; i != messages.begin() + params; ++i) {
					chord.insert((*i).toInt());
				}

				SequenceButtonWidget* sbw = _sequenceList[messages[4].toInt()];
				sbw->setTonic(messages[5]);

				QHash<QString, QSet<int> > scales;
				foreach(QString name, _scales.keys()) {
					foreach(QSet<int> scale, _scales.values(name)) {
						QSet<int> ts = scale;
						if(ts.intersect(chord) == chord) {
							scales.insert(name, scale);
						}
					}
				}
				sbw->setScales(scales);
			}
		} else if(sender == "Metronome") {
			_counter = ++_counter % 2;
			if(!_counter) {
				if(!_sequenceList.isEmpty()) {
					_sequenceIndex = ++_sequenceIndex % _sequenceList[_sequenceListIndex]->keyboard().columns().size();
					if(!_sequenceIndex) {
						_sequenceListIndex = ++_sequenceListIndex % _sequenceList.size();
					}
					// Play sequence(index).notes
					QList<int> notes = _sequenceList[_sequenceListIndex]->keyboard().notes(_sequenceIndex);
					OSCHelper* osc = OSCHelper::instance();
					int b = _chromaticScale.lastIndexOf(_sequenceList[_sequenceListIndex]->text());
					foreach(int n, notes) {
						QList<QVariant> vl;
						vl << 48 + b + n;
						vl << 100;
						vl << 500;
						osc->send("/midi/note", "iii", vl);
					}
				}
			}
		}
		QStringList::iterator i = messages.begin();
		messages.erase(i, i + params);
	}
}

void Doodle::Sequencer::addTangible(Tangible* t) {
	_tangibles.insert(t->fiducialID(), t);
	if(_objects[t->fiducialID()] == "Sequencer") {
		SequencerWidget* w = new SequencerWidget;
		w->setSequenceList(&_sequenceList);
		w->setSequencer(t);
		if(!_widgets.contains(w)) {
			_widgets.append(w);
		}
		_sequencerWidget = w;
		emit makeWidget(w);
	}
}

void Doodle::Sequencer::updateTangible(Tangible* t) {
	_tangibles.insert(t->fiducialID(), t);
	if(_objects[t->fiducialID()] == "Sequencer") {
		if(_sequencerWidget != NULL) {
			if(_sequencerWidget->sequencer()->fiducialID() == t->fiducialID() && _sequencerWidget->sequencer()->sessionID() == t->sessionID()) {
				_sequencerWidget->setSequencer(t);
			}
		}
	}
}

void Doodle::Sequencer::removeTangible(Tangible* t) {
	// TODO: Save chord list
	_tangibles.remove(t->fiducialID());
	if(_objects[t->fiducialID()] == "Sequencer") {
		QSet<Widget*> sub;
		QList<Widget*>::const_iterator wli;
		for(wli = _widgets.begin(); wli != _widgets.end(); ++wli) {
//			PadWidget* pad;
//			if((pad = dynamic_cast<PadWidget*>(*wli)) != NULL) {
//				sub.insert(*wli);
//				_havePad = false;
//				continue;
//			}
		}
		if(_sequencerWidget != NULL) {
			if(_sequencerWidget->sequencer()->fiducialID() == t->fiducialID() && _sequencerWidget->sequencer()->sessionID() == t->sessionID()) {
				sub.insert((Widget*)_sequencerWidget);
				_sequencerWidget = NULL;
			}
		}
		QSet<Widget*>::const_iterator wsi;
		for(wsi = sub.begin(); wsi != sub.end(); ++wsi) {
			_widgets.removeAll(*wsi);
			emit destroyWidget(*wsi);
		}
	}
}

void Doodle::Sequencer::publishWidget(Widget* w) {
	if(!_widgets.contains(w)) {
		_widgets.append(w);
	}
	emit makeWidget(w);
}

void Doodle::Sequencer::unpublishWidget(Widget* w) {
	if(_widgets.contains(w)) {
		_widgets.removeAll(w);
	}
	emit destroyWidget(w);
}

Q_EXPORT_PLUGIN2(Sequencer, Doodle::Sequencer);
