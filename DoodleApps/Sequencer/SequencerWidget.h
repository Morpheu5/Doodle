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

#ifndef SEQUENCERWIDGET_H_
#define SEQUENCERWIDGET_H_

class QPainter;

#include <DoodleSDK/includes/Widget.h>
#include <DoodleSDK/includes/Tangible.h>

#include "SequenceButtonWidget.h"

namespace Doodle {
class KeyboardWidget;

/*!
 * Visual representation of the Sequencer Tangible object.
 */
class SequencerWidget : public Widget {
	Q_OBJECT

public:
	SequencerWidget();
	SequencerWidget(Widget*);

	/*!
	 * Helper routine to build the visual representation.
	 */
	void makeVisual();

	const QString name() const { return QString("SequencerWidget"); }

	virtual bool pinchable() const { return false; }
	virtual void setPinchable(bool pinchable) { _pinchable = false; }

	void render(QPainter* painter);

	/*!
	 * \return	The Sequencer Tangible object.
	 */
	Tangible* sequencer() { return _sequencer; }

	/*!
	 * Sets the Sequencer Tangible object.
	 */
	void setSequencer(Tangible* sequencer) {
		_sequencer = sequencer;
		_objects.clear();
		_objects.append(sequencer->fiducialID());
	}
	virtual const QPointF position() const {
		return _sequencer->position();
	}
	virtual float angle() const { return _sequencer->angle(); }
	virtual const QPolygonF& shape() const { return _sequencer->shape(); }
	virtual float size() const { return _sequencer->size(); }

	/*!
	 * Sets the list of sequence slots that must be visualized around the
	 * SequencerWidget.
	 */
	void setSequenceList(QList<SequenceButtonWidget*>* sequenceList) { _sequenceList = sequenceList; }

protected:
	Tangible* _sequencer;

	int _sides;

	QList<SequenceButtonWidget*>* _sequenceList;
};

}

#endif /* SEQUENCERWIDGET_H_ */
