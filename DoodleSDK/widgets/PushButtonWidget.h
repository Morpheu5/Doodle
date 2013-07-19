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

#ifndef PUSHBUTTONWIDGET_H_
#define PUSHBUTTONWIDGET_H_

#include <QFont>
#include <QString>

#include "../includes/Widget.h"

namespace Doodle {

/*!
 * A push button, a button with no states.
 */
class PushButtonWidget : public Widget {
	Q_OBJECT

public:
	PushButtonWidget();
	PushButtonWidget(Widget* parent);

	const QString name() const { return QString("PushButtonWidget"); }

	virtual void render(QPainter* painter);

	/*!
	 * Sets width and height, thus allowing any aspect ratio to be used.
	 *
	 * \param	width	Width.
	 * \param	height	Height.
	 */
	virtual void setSize(float width, float height);

	/*!
	 * \return	Label of the button.
	 */
	virtual QString text() const { return _text; }

	/*!
	 * Sets the label of the button.
	 *
	 * \param	text	Label.
	 */
	virtual void setText(QString text);

	/*!
	 * Sets the font to be used for rendering the label.
	 *
	 * \param	font	Font.
	 */
	virtual void setFont(QFont font) { _font = font; }

signals:
	/*!
	 * Signal emitted when the button receives a number of taps.
	 *
	 * \param	n	Number of taps received.
	 */
	void tapped(int n);

	/*!
	 * Signal emitted when the button is tapped once.
	 */
	void pushed();

public slots:
	/*!
	 * Button reaction to a number of received taps.
	 *
	 * \param	n	Number of taps received.
	 */
	void tap(int n);

	/*!
	 * Button reaction when pushed.
	 */
	void push();

protected:
	QFont _font;
	QString _text;
};

}

#endif /* PUSHBUTTONWIDGET_H_ */
