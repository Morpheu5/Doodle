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

#ifndef WIDGET_H_
#define WIDGET_H_

#include <cmath>

#include <QObject>
#include <QList>
#include <QPointF>
#include <QMatrix>
#include <QPolygonF>
class QPainter;

#include "IApplication.h"

namespace Doodle {

/*!
 * A Widget is a graphical object used to give visual feedback. For practical
 * purposes this is a QObject.
 */
class Widget : public QObject {
	Q_OBJECT

public:
	/*!
	 * QObject constructor.
	 *
	 * \param	parent	Parent of the Widget.
	 */
	Widget(Widget* parent);

	/*!
	 * Standard destructor.
	 */
	virtual ~Widget();

	/*!
	 * \return	Unique ID of the Widget.
	 */
	unsigned long id() const { return _id; }

	/*!
	 * \return	Name of the Widget.
	 */
	virtual const QString name() const = 0;

	/*!
	 * Flags used to choose whether children of the Widget should be rendered or
	 * not.
	 */
	enum RenderFlags { NoFlag = 0x0, RenderChildren = 0x1, AllFlags = 0xff };

	/*!
	 * \return	Render flags.
	 */
	RenderFlags renderFlags() const { return _flags; }

	/*!
	 * Sets the render flags.
	 *
	 * \param	flags	Render flags.
	 */
	void setRenderFlags(RenderFlags flags) { _flags = flags; }

	/*!
	 * \return	Position of the Widget.
	 */
	virtual const QPointF position() const { return _position; }

	/*!
	 * Sets the position of the Widget.
	 *
	 * \param	pos		Position of the Widget.
	 */
	virtual void setPosition(QPointF pos);

	/*!
	 * \return	Angle of the Widget in degrees.
	 */
	virtual float angle() const { return _angle; }

	/*!
	 * Sets the angle of the Widget in degrees.
	 *
	 * \param	degree	Angle of the Widget in degrees.
	 */
	virtual void setAngle(float degree);

	/*!
	 * Size is actually a multiplier but its semantics is different different
	 * from that of scale().
	 *
	 * \return	Size of the widget.
	 *
	 * \sa scale()
	 */
	virtual float size() const { return _size; }

	/*!
	 * Sets the size of the Widget.
	 *
	 * \param	size	Size of the Widget.
	 *
	 * \sa size()
	 * \sa scale()
	 */
	virtual void setSize(float size) { _size = size; }

	/*!
	 * A multiplier which is used when the Widget is pinchable.
	 *
	 * \return	Scale factor.
	 */
	virtual float scale() const { return _scale; }

	/*!
	 * Sets the scale factor.
	 *
	 * \param	scale	Scale factor.
	 */
	virtual void setScale(float scale);

	/*!
	 * \return	Shape of the Widget.
	 */
	virtual const QPolygonF& shape() const { return _shape; }

	/*!
	 * Sets the shape of the Widget.
	 */
	virtual void setShape(QPolygonF shape) { _shape = shape; }

	/*!
	 * \return	True if the Widget is pinchable (zoomable).
	 */
	virtual bool pinchable() const { return _pinchable; }

	/*!
	 * Makes the Widget pinchable (zoomable).
	 *
	 * \param	pinchable	False if the Widget shall not be pinchable. True by
	 * 						default.
	 */
	virtual void setPinchable(bool pinchable = true) { _pinchable = pinchable; }

	/*!
	 * \return	True if the Widget is visible and should be rendered.
	 */
	virtual bool visible() const { return _visible; }

	/*!
	 * Makes the Widget visibile.
	 *
	 * \param	visible	False if the Widget shall be hidden. True by default.
	 */
	virtual void setVisible(bool visible = true) { _visible = visible; }

	/*!
	 * Paints itself on the given QPainter.
	 *
	 * \param	painter	The QPainter that handles the rendering.
	 */
	virtual void render(QPainter* painter) = 0;

	/*!
	 * Paints children of the Widget.
	 *
	 * \param	painter	The QPainter that handles the rendering.
	 */
	virtual void renderChildren(QPainter* painter);

	/*!
	 * \param	p	The point.
	 *
	 * \return	True if the point is contained in the Widget.
	 */
	virtual bool contains(const QPointF& p) const;

	/*!
	 * \return	Parent of the Widget.
	 */
	Widget* parent() { return _parent; }

	/*!
	 * Makes the Widget child of another Widget.
	 *
	 * \param	parent	The parent Widget.
	 */
	void setParent(Widget* parent) { _parent = parent; }

	/*!
	 * \return	True if the Widget has no parent.
	 */
	bool isTopLevel() { return _parent == 0; }

	/*!
	 * Makes the Widget parent of another Widget.
	 *
	 * \param	child	The other Widget.
	 */
	void addChild(Widget* child);

	/*!
	 * Removes a Widget from the children of this Widget.
	 *
	 * \param	child	The Widget to be removed.
	 */
	void removeChild(Widget* child);

	/*!
	 * \return	List of children of the Widget.
	 */
	const QList<Widget*>& children() const { return _children; }

	/*!
	 * Applies a linear transform to the Widget.
	 *
	 * \param	translation	Translation
	 * \param	zoom		Scaling
	 * \param	rotation	Rotation
	 */
	void transform(QPointF translation, float zoom, float rotation);

	/*!
	 * \return	The Application the Widget belongs to.
	 */
	IApplication* application() { return _application; }

	/*!
	 * Sets the Application to which the Widget belongs to.
	 *
	 * \param	application	The Application.
	 */
	void setApplication(IApplication* application) { _application = application; }

	/*!
	 * \return	List of Tangible objects (IDs) associated with the Widget.
	 */
	const QList<long>& objects() { return _objects; }

	/*!
	 * Associates the Widget with a list of Tangible objects.
	 *
	 * \param	objects	List of objects (IDs).
	 */
	void setObjects(QList<long> objects) { _objects = objects; }

	/*!
	 * Associates one Tangible object (ID) with this Widget.
	 *
	 * \param	object	ID of the object.
	 */
	void appendObject(long object) { _objects.append(object); }

signals:
	/*!
	 * Signal emitted when the Widget received a number of taps, the number
	 * being emitted as a parameter.
	 *
	 * \param	n	Number of taps.
	 */
	void tapped(int n);

	/*!
	 * Signal emitted when a Widget becomes a child of this Widget.
	 *
	 * \param	child	The newly added Widget.
	 */
	void childAdded(Widget* child);

public slots:
	/*!
	 * Reaction of the Widget to an arbitrary number of taps.
	 *
	 * \param	n	Number of taps.
	 */
	virtual void tap(int n);

protected:
	unsigned long _id;
	RenderFlags _flags;
	QPointF _position;
	double _angle;
	double _size;
	double _scale;

	QPolygonF _shape;

	bool _visible;
	bool _pinchable;

	Widget* _parent;
	QList<Widget*> _children;

	IApplication* _application;
	QList<long> _objects;

private:
	static unsigned long s_id;
};

}

#endif /* WIDGET_H_ */
