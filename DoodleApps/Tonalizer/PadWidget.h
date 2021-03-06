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

#ifndef PADWIDGET_H_
#define PADWIDGET_H_

class QPainter;

#include <DoodleSDK/includes/Widget.h>

namespace Doodle {

/*!
 * A writing pad widget for writing chord. This is needed by the
 * PolyGlyphEngine.
 */
class PadWidget : public Widget {
	Q_OBJECT

public:
	PadWidget();
	PadWidget(Widget* parent);

	const QString name() const { return QString("PadWidget"); }

	void render(QPainter* painter);
};

}

#endif /* PADWIDGET_H_ */
