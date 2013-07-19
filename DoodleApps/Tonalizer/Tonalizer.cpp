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

#include <typeinfo>
#include <stdexcept>
#include <cmath>

#include <QDebug>
#include <QList>
#include <QTimer>
#include <QString>

#include <DoodleSDK/widgets/PushButtonWidget.h>
#include <DoodleSDK/widgets/CheckButtonWidget.h>

#include "TonalizerWidget.h"
#include "PadWidget.h"
#include "ChordWidget.h"
#include "ChordButtonWidget.h"
#include "Tonalizer.h"

Doodle::Tonalizer::Tonalizer() {
	_padNeeded = true;
	_havePad = false;
	_buildingChord = false;
	_currentChordWidget = NULL;
}

void Doodle::Tonalizer::init(QString rcPath, QHash<QString, QString> params) {
	Q_UNUSED(rcPath);
	if(params["padNeeded"].simplified() == "true") {
		_padNeeded = true;
	} else {
		_padNeeded = false;
	}
}

QStringList Doodle::Tonalizer::declareObjects() const {
	QStringList l;
	l << "Tonalizer";
	return l;
}

QStringList Doodle::Tonalizer::declareGestureRecognizers() const {
	QStringList l;
	l << "Glyph" << "MultiTap" << "StraightStroke";
	return l;
}

QStringList Doodle::Tonalizer::declareWidgets() const {
	QStringList l;
	l << "PadWidget";
	return l;
}

void Doodle::Tonalizer::processResults(QStringList results) {
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

		// To make everything local we want to check if the result is associated
		// with at least one of our objects or if the result is associated with
		// one of our widgets.
		bool accept = false;
		if(!_padNeeded) {
			accept = acceptable(objects, widgets);
		} else {
			foreach(Widget* w, _widgets) {
				foreach(unsigned long id, widgets) {
					if(w->id() == id) {
						if(qobject_cast<PadWidget*>(w)) {
							accept = true;
						}
					}
				}
			}
		}
		if(!accept) {
			QStringList::iterator i = results.begin();
			results.erase(i, i + skip);
			continue;
		}

		if(name == "Glyph") {
			QString glyph = results.takeFirst();
			if(_currentChordWidget == NULL) {
				// Start building chord
				if(glyph == "C" || glyph == "D" || glyph == "E" || glyph == "F" || glyph == "G" || glyph == "A" || glyph == "B") {
					Chord* chord = new Chord(glyph);
					_chords.append(chord);
					ChordWidget* chordWidget = new ChordWidget;
					chordWidget->setChord(chord);
					chordWidget->setPosition(QPointF(0.5, 0.5));
					chordWidget->setAngle(0);
					_currentChordWidget = chordWidget;
					connect(chordWidget, SIGNAL(tapped(int)), SLOT(chordWidgetTapped(int)));
					_widgets.append(chordWidget);
					emit makeWidget(chordWidget);

					ChordButtonWidget* tcb = new ChordButtonWidget;
					tcb->setChordWidget(chordWidget);
					tcb->setText(chord->tonic());
					tcb->setSize(15, 10);
					tcb->setChecked(true);
					connect(tcb, SIGNAL(checked()), SLOT(chordButtonChecked()));
					connect(tcb, SIGNAL(unchecked()), SLOT(chordButtonUnchecked()));
					_tonalizerWidget->chordList().append(tcb);
					_widgets.append(tcb);
					emit makeWidget(tcb);

					// FIXME: Maybe a more specific approach may be more
					// approriate and also gets rid of sender().
					foreach(Widget* w, chordWidget->children()) {
						if(CheckButtonWidget* cb = qobject_cast<CheckButtonWidget*>(w)) {
							connect(cb, SIGNAL(checked()), SLOT(buttonChecked()));
							connect(cb, SIGNAL(unchecked()), SLOT(buttonUnchecked()));
						}
						if(PushButtonWidget* pb = qobject_cast<PushButtonWidget*>(w)) {
							connect(pb, SIGNAL(pushed()), SLOT(buttonPushed()));
						}
						_widgets.append(w);
						emit makeWidget(w);
					}
					QStringList message;
					message << "Tonalizer" << "" << "NewChord" << chord->tonic();
					foreach(int n, chord->notes()) {
						message << QString::number(n);
					}
					message.insert(2, QString::number(message.length() - 2));
					emit deliverMessages(message);
				}
			} else { /* _currentChordWidget != NULL */
				if(_chords.isEmpty()) {
					throw std::runtime_error("INVISIBLE CHORD LIST");
				}

				Chord* chord = _currentChordWidget->chord();
				chord->process(glyph);

				QStringList message;
				message << "Tonalizer" << "" << "EditChord" << QString::number(_chords.indexOf(chord)) << chord->tonic();
				foreach(int n, chord->notes()) {
					message << QString::number(n);
				}
				message.insert(2, QString::number(message.length() - 2));
				emit deliverMessages(message);
			}
		} else if(name == "MultiTap") {
			int n = results.takeFirst().toInt();
			switch(n) {
			case 1:
				// TODO: Make the tap local (that is: check if the tap involved
				// the tonalizer or not).
				if(!_havePad && _padNeeded) {
					foreach(Tangible* t, _tangibles) {
						if(_objects[t->fiducialID()] == "Tonalizer") {
							// NOTE: You can't do this inline in the construction of
							// p because C/C++ makes the calls from the rightmost to
							// the leftmost.
							float x = results.takeFirst().toFloat();
							float y = results.takeFirst().toFloat();
							// TODO: Move the pad far from the tonalizer and rotate
							// it according to its relative position.
							QPointF p(x, y);
							PadWidget* w = new PadWidget;
							w->setApplication(this);
							w->appendObject(t->fiducialID());
							w->setAngle(0);
							w->setPosition(p);
							if(!_widgets.contains(w)) {
								_widgets.append(w);
							}
							connect(w, SIGNAL(tapped(int)), SLOT(padTapped(int)));
							emit makeWidget(w);
							_havePad = true;
						}
					}
				} else {
					QStringList::iterator i = results.begin();
					results.erase(i, i + n*2);
				}
				break;
			default:
				QStringList::iterator i = results.begin();
				results.erase(i, i + n*2);
			}
			// Make MultiTap blocker.
			return;
		} else if(name == "StraightStroke") {
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
					foreach(ChordButtonWidget* w, _tonalizerWidget->chordList()) {
						if(w->id() == id) {
							ChordWidget* cw = w->chordWidget();
							if(!cw->visible() && !cw->active()) {
								cw->setVisible(true);
								cw->setActive(false);
								cw->setPosition(line.p2());
							}
						}
					}
				}
				break;
			}
			default: {
				QStringList::iterator i = results.begin();
				results.erase(i, i + skip - 1);
				break;
			}
			}
			// Make StraightStroke blocker.
			return;
		} else {
			qDebug() << this << "Skipping" << skip << "params because I don't know" << name;
			QStringList::iterator i = results.begin();
			results.erase(i, i + skip);
		}
	}
	return;
}

void Doodle::Tonalizer::processMessages(QStringList messages) {
	Q_UNUSED(messages);
}

void Doodle::Tonalizer::addTangible(Tangible* t) {
	_tangibles.insert(t->fiducialID(), t);
	if(_objects[t->fiducialID()] == "Tonalizer") {
		TonalizerWidget* w = new TonalizerWidget;
		w->setTonalizer(t);
		if(!_widgets.contains(w)) {
			_widgets.append(w);
		}
		_tonalizerWidget = w;
		emit makeWidget(w);
	}
	// TODO: Resume chord list
}

void Doodle::Tonalizer::updateTangible(Tangible* t) {
	_tangibles.insert(t->fiducialID(), t);
	if(_objects[t->fiducialID()] == "Tonalizer") {
		if(_tonalizerWidget != NULL) {
			if(_tonalizerWidget->tonalizer()->fiducialID() == t->fiducialID() && _tonalizerWidget->tonalizer()->sessionID() == t->sessionID()) {
				_tonalizerWidget->setTonalizer(t);
			}
		}
	}
}

void Doodle::Tonalizer::removeTangible(Tangible* t) {
	// TODO: Save chord list
	_tangibles.remove(t->fiducialID());
	if(_objects[t->fiducialID()] == "Tonalizer") {
		QSet<Widget*> sub;
		QList<Widget*>::const_iterator wli;
		for(wli = _widgets.begin(); wli != _widgets.end(); ++wli) {
			PadWidget* pad;
			if((pad = dynamic_cast<PadWidget*>(*wli)) != NULL) {
				sub.insert(*wli);
				_havePad = false;
				continue;
			}
		}
		if(_tonalizerWidget != NULL) {
			if(_tonalizerWidget->tonalizer()->fiducialID() == t->fiducialID() && _tonalizerWidget->tonalizer()->sessionID() == t->sessionID()) {
				sub.insert((Widget*)_tonalizerWidget);
				_tonalizerWidget = NULL;
			}
		}
		QSet<Widget*>::const_iterator wsi;
		for(wsi = sub.begin(); wsi != sub.end(); ++wsi) {
			_widgets.removeAll(*wsi);
			emit destroyWidget(*wsi);
		}
	}
}

void Doodle::Tonalizer::padTapped(int n) {
	Q_UNUSED(n);
	if(_buildingChord) {
		_buildingChord = false;
	}
}

void Doodle::Tonalizer::buttonChecked() {
	if(CheckButtonWidget* b = qobject_cast<CheckButtonWidget*>(sender())) {
		ChordWidget* p = (ChordWidget*)b->parent();
		Chord* chord = p->chord();
		chord->process(b->text());

		QStringList message;
		message << "Tonalizer" << "" << "EditChord" << QString::number(_chords.indexOf(chord)) << chord->tonic();
		foreach(int n, chord->notes()) {
			message << QString::number(n);
		}
		message.insert(2, QString::number(message.length() - 2));
		emit deliverMessages(message);
	}
}

void Doodle::Tonalizer::buttonUnchecked() {
	if(CheckButtonWidget* b = qobject_cast<CheckButtonWidget*>(sender())) {
		ChordWidget* p = (ChordWidget*)b->parent();
		Chord* chord = p->chord();
		chord->process(b->text());

		QStringList message;
		message << "Tonalizer" << "" << "EditChord" << QString::number(_chords.indexOf(chord)) << chord->tonic();
		foreach(int n, chord->notes()) {
			message << QString::number(n);
		}
		message.insert(2, QString::number(message.length() - 2));
		emit deliverMessages(message);
	}
}

void Doodle::Tonalizer::buttonPushed() {
	if(PushButtonWidget* b = qobject_cast<PushButtonWidget*>(sender())) {
		ChordWidget* p = (ChordWidget*)b->parent();
		Chord* chord = p->chord();
		chord->process(b->text());

		QStringList message;
		message << "Tonalizer" << "" << "EditChord" << QString::number(_chords.indexOf(chord)) << chord->tonic();
		foreach(int n, chord->notes()) {
			message << QString::number(n);
		}
		message.insert(2, QString::number(message.length() - 2));
		emit deliverMessages(message);
	}
}

void Doodle::Tonalizer::chordWidgetTapped(int n) {
	ChordWidget *chordWidget = qobject_cast<ChordWidget*>(sender());
	switch(n) {
	case 1:
		qDebug() << _currentChordWidget << chordWidget;
		if(_currentChordWidget == NULL) {
			_currentChordWidget = chordWidget;
			_currentChordWidget->setActive(true);
		} else {
			_currentChordWidget->setActive(false);
			if(_currentChordWidget == chordWidget) {
				_currentChordWidget = NULL;
			} else {
				chordWidget->setActive(true);
				_currentChordWidget = chordWidget;
			}
		}
		break;
	case 2:
		chordWidget->setActive(false);
		chordWidget->setVisible(false);
		if(_currentChordWidget == chordWidget) {
			_currentChordWidget = NULL;
		}
		break;
	}
}

void Doodle::Tonalizer::chordButtonChecked() {

}

void Doodle::Tonalizer::chordButtonUnchecked() {

}

Q_EXPORT_PLUGIN2(Tonalizer, Doodle::Tonalizer);
