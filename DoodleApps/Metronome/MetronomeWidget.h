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

#ifndef METRONOMEWIDGET_H_
#define METRONOMEWIDGET_H_

class QPainter;

#include <DoodleSDK/includes/Widget.h>
#include <DoodleSDK/includes/Tangible.h>

namespace Doodle {

/*!
 * Visual representation of the Metronome Tangible object.
 */
class MetronomeWidget : public Widget {
	Q_OBJECT

public:
	MetronomeWidget();
	MetronomeWidget(Widget*);

	/*!
	 * Helper routine to build the visual representation.
	 */
	void makeVisual();

	const QString name() const { return QString("MetronomeWidget"); }

	virtual bool pinchable() const { return false; }
	virtual void setPinchable(bool) { _pinchable = false; }

	void render(QPainter*);

	/*!
	 * \return	The Metronome Tangible object.
	 */
	Tangible* metronome() { return _metronome; }

	/*!
	 * Sets the Metronome Tangible object.
	 *
	 * \param	metronome	The Metronome.
	 */
	void setMetronome(Tangible* metronome) {
		_metronome = metronome;
		_objects.clear();
		_objects.append(metronome->fiducialID());
	}

	virtual const QPointF position() const {
		return _metronome->position();
	}
	virtual float angle() const { return _metronome->angle(); }
	virtual const QPolygonF& shape() const { return _metronome->shape(); }
	virtual float size() const { return _metronome->size(); }

protected:
	Tangible* _metronome;

	int _sides;
};

}

#endif /* SEQUENCERWIDGET_H_ */
