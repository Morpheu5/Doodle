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

#ifndef PAINTER_H_
#define PAINTER_H_

#include <QBrush>
#include <QPen>
#include <QHash>

#include <QMutex>

class QPainter;
class QPaintEvent;

namespace Doodle {
class Tangible;
class Trace;
class Widget;

static QMutex paintMutex;

/*!
 * QPainter helper used to draw visual feedback of the application.
 */
class Painter {
public:
	/*!
	 * Standard constructor.
	 */
	Painter();

	/*!
	 * Standard destructor.
	 */
	~Painter();

	/*!
	 * Qt Painting routine. All the drawing takes place here.
	 *
	 * See http://doc.trolltech.com/qpainter.html and
	 * http://doc.trolltech.com/4.6/examples-painting.html
	 *
	 * \param	painter	An instance of QPainter.
	 * \param	event	The event associated with the paint request.
	 */
	void paint(QPainter* painter, QPaintEvent* event);

	/*!
	 * \return	Width of the Viewport.
	 */
	int width() const { return _width; }

	/*!
	 * Sets width of the Viewport.
	 *
	 * \param	width	Width.
	 */
	void setWidth(int width) { _width = width; }

	/*!
	 * \return	Height of the Viewport.
	 */
	int height() const { return _height; }

	/*!
	 * Sets height of the Viewport.
	 *
	 * \param	height	Height.
	 */
	void setHeight(int height) { _height = height; }

	/*!
	 * \return	Scale factor of the Viewport.
	 */
	float scale() const { return _scale; }

	/*!
	 * Sets scale factor of the Viewport.
	 */
	void setScale(float scale) { _scale = scale; }

	/*!
	 * Sets size (width x height) of the Viewport, and computes its aspect
	 * ratio.
	 *
	 * \param	width	Width.
	 * \param	height	Height.
	 */
	void setSize(int width, int height) { _width = width; _height = height; _aspectRatio = (float)_height/_width; _scale = (float)_height/800; }

	/*!
	 * \return	Viewport's aspect ratio.
	 */
	float aspectRatio() const { return _aspectRatio; }

	/*!
	 * \return	QBrush used to paint the background.
	 */
	const QBrush& background() const { return _background; }

	/*!
	 * Sets the QBrush used to paint the background.
	 *
	 * \param	background	QBrush to paint the background.
	 */
	void setBackground(QBrush background) { _background = background; }

	/*!
	 * Background colour is constantly changing. This sets the steps to walk
	 * around the colour wheel.
	 *
	 * \param	hueStep	Step.
	 */
	void setHueStep(float hueStep) { _hueStep = hueStep; }

	/*!
	 * Sets the set of Tangible objects present on the table.
	 *
	 * \param	tangibles	The set of Tangible objects.
	 */
	void setTangibles(QHash<long, Tangible*>* tangibles) { _tangibles = tangibles; }

	/*!
	 * Sets the set of active Traces.
	 *
	 * \param	traces	The set of Traces.
	 */
	void setTraces(QHash<long, Trace*>* traces) { _traces = traces; }

	/*!
	 * Sets the set of active Widgets.
	 *
	 * \param	widgets	The set of Widgets.
	 */
	void setWidgets(QList<Widget*>* widgets) { _widgets = widgets; }

	/*!
	 * \internal Sets the space gap within which two Strokes are considered part of the
	 * same Trace. This is for visual debugging.
	 *
	 * \param	traceSpaceGap	The gap.
	 */
	void setTraceSpaceGap(float* traceSpaceGap) { _traceSpaceGap = traceSpaceGap; }

	/*!
	 * \internal
	 */
	void setGroupSpaceGap(float* groupSpaceGap) { _groupSpaceGap = groupSpaceGap; }

	/*!
	 * \internal Toggles a boolean variable to help distinguish whether we're
	 * working on the simulator or on the Table. This variable should be true
	 * for real world production applications.
	 */
	void toggleOnTable() { _onTable = !_onTable; }

private:
	float _hue;
	float _hueStep;

	QBrush _background;
	QPen _pen;
	QBrush _fingerBrush;
	QPen _fingerPen;

	int _width;
	int _height;
	float _scale;
	float _aspectRatio; // _height / _width

	QHash<long, Tangible*>* _tangibles;
	QHash<long, Trace*>* _traces;
	QList<Widget*>* _widgets;
	float* _traceSpaceGap;
	float* _groupSpaceGap;

	bool _onTable;
};
}

//static QMutex s_paintMutex;

#endif /* PAINTER_H_ */
