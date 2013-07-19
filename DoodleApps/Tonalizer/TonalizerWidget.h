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

#ifndef TONALIZERWIDGET_H_
#define TONALIZERWIDGET_H_

class QPainter;

#include <DoodleSDK/includes/Widget.h>
#include "ChordButtonWidget.h"
#include <DoodleSDK/includes/Tangible.h>

namespace Doodle {

/*!
 * The visual representation of the Tonalizer Tangible object.
 */
class TonalizerWidget : public Widget {
	Q_OBJECT

public:
	TonalizerWidget();
	TonalizerWidget(Widget* parent);

	/*!
	 * Helper routine to make visuals.
	 */
	void makeVisual();

	const QString name() const { return QString("TonalizerWidget"); }

	virtual bool pinchable() const { return false; }
	virtual void setPinchable(bool pinchable) { _pinchable = false; }

	void render(QPainter* painter);

	/*!
	 * \return	Associated Tonalizer Tangible object.
	 */
	Tangible* tonalizer() { return _tonalizer; }

	/*!
	 * Sets the Tonalizer Tangible object.
	 *
	 * \param	tonalizer	The Tonalizer.
	 */
	void setTonalizer(Tangible* tonalizer) {
		_tonalizer = tonalizer;
		_objects.clear();
		_objects.append(tonalizer->fiducialID());
	}
	virtual const QPointF position() const {
		return _tonalizer->position();
	}
	virtual float angle() const { return _tonalizer->angle(); }
	virtual const QPolygonF& shape() const { return _tonalizer->shape(); }
	virtual float size() const { return _tonalizer->size(); }

	/*!
	 * \return	List of chords, either active or not.
	 */
	QList<ChordButtonWidget*>& chordList() { return _chordList; }

protected:
	Tangible* _tonalizer;

	int _sides;

	QList<ChordButtonWidget*> _chordList;
};

}

#endif /* TONALIZERWIDGET_H_ */
