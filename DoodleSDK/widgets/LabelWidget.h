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

#ifndef LABELWIDGET_H_
#define LABELWIDGET_H_

#include <QFont>

class QPainter;

#include "../includes/Widget.h"

namespace Doodle {
class Widget;

/*!
 * A Widget to display a line of text.
 */
class LabelWidget : public Widget {
	Q_OBJECT

public:
	/*!
	 * \copydoc Widget::Widget()
	 */
	LabelWidget();

	/*!
	 * \copydoc Widget::Widget(Widget*)
	 */
	LabelWidget(Widget* parent);

	/*!
	 * \copydoc Widget::name()
	 */
	const QString name() const { return QString("LabelWidget"); }

	void render(QPainter* painter);

	/*!
	 * \return	Text of the Label.
	 */
	const QString& text() const { return _text; }

	/*!
	 * Sets the text of the Label.
	 */
	void setText(QString text);

	/*!
	 * Sets the font to be used for rendering the Label.
	 */
	virtual void setFont(QFont font) { _font = font; }

protected:
	QFont _font;
	QString _text;
};

}

#endif /* LABELWIDGET_H_ */
