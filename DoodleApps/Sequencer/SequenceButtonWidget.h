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

#ifndef SEQUENCEBUTTONWIDGET_H_
#define SEQUENCEBUTTONWIDGET_H_

#include <DoodleSDK/widgets/CheckButtonWidget.h>

#include "KeyboardWidget.h"

namespace Doodle {

class SequenceButtonWidget : public CheckButtonWidget {
public:
	SequenceButtonWidget(Widget* parent = 0) : CheckButtonWidget(parent) { }

	virtual bool pinchable() const { return false; }
	virtual void setPinchable(bool) { _pinchable = false; }

	QString text() const { return _tonic; }

	void setTonic(QString tonic) { _tonic = tonic; }

	KeyboardWidget& keyboard() { return _keyboard; }

	QHash<QString, QSet<int> >& scales() { return _scales; }
	void setScales(QHash<QString, QSet<int> >& scales) { _scales = scales; _keyboard.setScales(&_scales); }

protected:
	QHash<QString, QSet<int> > _scales;
	QString _scale;
	QString _tonic;

	KeyboardWidget _keyboard;
};

}

#endif /* SEQUENCEBUTTONWIDGET_H_ */
