#ifndef GLYPHPAINTER_H_
#define GLYPHPAINTER_H_

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QList>
#include <QPointF>
class QPainter;
class QPaintEvent;

/*!
 * Painter helper used to a Bézier curve and its control points in the editing
 * viewport.
 */
class GlyphPainter {
public:
	GlyphPainter();

	/*!
	 * Qt Painting routine. All the drawing takes place here.
	 *
	 * See http://doc.trolltech.com/qpainter.html and
	 * http://doc.trolltech.com/4.6/examples-painting.html
	 *
	 * \param	painter	An instance of QPainter.
	 * \param	event	The event associated with the paint request.
	 * \param	elapsed	Elapsed time since the initialization of the painting
	 * 					engine.
	 */
	void paint(QPainter* painter, QPaintEvent* event, int elapsed);

	/*!
	 * \return	List of control points of the Bézier curve representing the
	 * 			glyph.
	 */
	QList<QPointF>& glyph() { return *_glyph; }

	/*!
	 * \return	Constant time step.
	 */
	double tStep() { return _t_step; }

	/*!
	 * Sets the glyph to be painted.
	 *
	 * \param	The glyph.
	 */
	void setGlyph(QList<QPointF>* glyph) { _glyph = glyph; }

	/*!
	 * Sets the active control points that will be painted differently.
	 *
	 * \param	activePoints	List of active control points.
	 */
	void setActivePoints(QList<QPointF>* activePoints) { _activePoints = activePoints; }

	/*!
	 * Sets constant time interval.
	 *
	 * \param	t_step	Constant time interval.
	 */
	void setTStep(double t_step) { _t_step = t_step; }

	/*!
	 * \return	choose(n,k)
	 */
	unsigned long long choose(unsigned n, unsigned k);

private:
	QBrush background;
	QPen pen;

	QList<QPointF>* _glyph;
	QList<QPointF>* _activePoints;
	double _t_step;
};

#endif /* GLYPHPAINTER_H_ */
