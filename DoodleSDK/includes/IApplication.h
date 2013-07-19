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

#ifndef IAPPLICATION_H_
#define IAPPLICATION_H_
#include <QStringList>
#include <QSet>
#include <QDebug>

#include "Tangible.h"

#define DOODLE_APPLICATION \
public: \
	virtual void setObjects(QHash<long, QString> objects) { _objects = objects; } \
protected: \
	virtual bool acceptable(QList<long> objects, QList<unsigned long> widgets) { \
		QSet<unsigned long> r = widgets.toSet(); \
		int i; \
		for(i = 0; i < _widgets.size(); i++) { \
			if(r.contains(_widgets.at(i)->id())) { \
				return true; \
			} \
		} \
		QSet<long> s = objects.toSet(); \
		s.intersect(_objects.keys().toSet()); \
		return !s.isEmpty(); \
	} \
private:

namespace Doodle {
class Widget;
class Tangible;

/*!
 * Qt Plugin Interface class for Doodle Applications.
 * A Doodle Application is a process that gathers information from the
 * environment and produces corresponding output. For example a waveform
 * generator is a Doodle Application.
 *
 * See http://doc.trolltech.com/plugins-howto.html for more information.
 */
class IApplication {
public:
	/*!
	 * \return	Name of this Application.
	 */
	virtual QString name() const = 0;

	/*!
	 * Initializes this Application.
	 *
	 * \param	rcPath	Path to resources.
	 * \param	params	A set of Application-specific parameters.
	 */
	virtual void init(QString rcPath, QHash<QString, QString> params) = 0;

	/*!
	 * \return	A list of objects (tangibles) this Application deals with.
	 */
	virtual QStringList declareObjects() const = 0;

	/*!
	 * \return	A list of gesture recognizers this Application listens to.
	 */
	virtual QStringList declareGestureRecognizers() const = 0;

	/*!
	 * \return	A list of widgets this Application produces.
	 */
	virtual QStringList declareWidgets() const = 0;

	/*!
	 * Sets the list of objects that this application has requested.
	 *
	 * \param	objects	List of objects.
	 */
	virtual void setObjects(QHash<long, QString> objects) = 0;

public slots:
	/*!
	 * Processes a result coming from a Gesture Recognizer.
	 *
	 * \param	results	Result packet.
	 */
	virtual void processResults(QStringList results) = 0;

	/*!
	 * Processes a message coming from another Application.
	 *
	 * \param	messages	List of messages.
	 */
	virtual void processMessages(QStringList messages) = 0;

	/*!
	 * When a Tangible object enters the system, each Application can either add
	 * it to their pool of objects, or reject it.
	 *
	 * \param	t	A pointer to the object.
	 */
	virtual void addTangible(Tangible* t) = 0;

	/*!
	 * When a Tangible object is updated (i.e. moved, rotated, etc), each
	 * Application should update their references according to the new state.
	 *
	 * \param	t	A pointer to the object.
	 */
	virtual void updateTangible(Tangible* t) = 0;

	/*!
	 * When a Tangible exits the system, each Application should remove it from
	 * their pools.
	 *
	 * \param	t	A pointer to the object.
	 */
	virtual void removeTangible(Tangible* t) = 0;

signals:
	/*!
	 * Signal emitted to publish a widget for display.
	 *
	 * \param	A pointer to the Widget.
	 */
	void makeWidget(Widget* w);

	/*!
	 * Signal emitted to remove a Widget from display.
	 *
	 * \param	w	A pointer to the Widget.
	 */
	void destroyWidget(Widget* w);

	/*!
	 * Signal emitted to deliver a message to all subscribed Applications.
	 *
	 * \param	messages	A list of QStrings bearing the messages.
	 */
	void deliverMessages(QStringList message);

protected:
	/*!
	 * Used to check whether a result can be accepted based on its associated
	 * objects and widgets.
	 *
	 * \param	objects	List of objects (IDs).
	 * \param	widgets	List of widgets (IDs).
	 */
	virtual bool acceptable(QList<long> objects, QList<unsigned long> widgets) = 0;

	QHash<long, QString> _objects;
	QHash<long, Tangible*> _tangibles;
	QList<Widget*> _widgets;
};

}

Q_DECLARE_INTERFACE(Doodle::IApplication, "Doodle.IApplication/1.0");

#endif /* IAPPLICATION_H_ */
