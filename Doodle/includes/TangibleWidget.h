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

#ifndef TANGIBLEWIDGET_H_
#define TANGIBLEWIDGET_H_

class QPainter;

#include <DoodleSDK/includes/Widget.h>
#include <DoodleSDK/includes/Tangible.h>

namespace Doodle {

/*!
 * A special Widget that visually represents a Tangible object. This is
 * typically placed at the same position of the Tangible object, thus ending up
 * under the object.
 */
class TangibleWidget : public Widget {
public:
	/*!
	 * \copydoc Widget::Widget()
	 */
	TangibleWidget();

	/*!
	 * \copydoc Widget::Widget(Widget*)
	 */
	TangibleWidget(Widget*);

	const QString name() const { return QString("TangibleWidget"); }

	void render(QPainter* painter);
};

}

#endif /* TANGIBLEWIDGET_H_ */
