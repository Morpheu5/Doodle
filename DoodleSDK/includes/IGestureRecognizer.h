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

#ifndef IGESTURERECOGNIZER_H_
#define IGESTURERECOGNIZER_H_
#include <QList>
#include <QSet>
#include <QHash>
#include <QStringList>
#include "Group.h"

#define DOODLE_STATIC_GESTURERECOGNIZER \
public: \
	void feedGroup(Group& group) { _sGroups.append(group); recognize(); } \
	void feedGroup(Group*) { } \
	bool isProgressive() const { return false; } \
private:

#define DOODLE_PROGRESSIVE_GESTURERECOGNIZER \
public: \
	void feedGroup(Group&) { } \
	void feedGroup(Group* group) { _pGroups.insert(group); recognize(); } \
	bool isProgressive() const { return true; } \
private:

#define DOODLE_GESTURERECOGNIZER \
public: \
	int priority() const { return _priority; } \
	void setPriority(int priority) { _priority = priority; } \
private:

namespace Doodle {

/*!
 * Qt Plugin Interface for Gesture Recognizers.
 * A Doodle Application is a process that gathers information from the
 * environment and produces corresponding output. For example a waveform
 * generator is a Doodle Application.
 *
 * See http://doc.trolltech.com/plugins-howto.html for more information.
 */
class IGestureRecognizer {
public:
	/*!
	 * Standard constructor.
	 */
	virtual ~IGestureRecognizer() { }

	/*!
	 * Checks if a gesture is progressive. A progressive gesture is a gesture
	 * that has to be continuously processes and whose effects may be continuous
	 * as well.
	 *
	 * \return	True if the gesture is progressive.
	 */
	virtual bool isProgressive() const = 0;

	/*!
	 * \return	The name of this Gesture Recognizer.
	 */
	virtual QString name() const = 0;

	/*!
	 * Initializes this Gesture Recognizer.
	 *
	 * \param	rcPath	Path to resources.
	 * \param	params	A set of specific parameters.
	 */
	virtual void init(QString rcPath, QHash<QString, QString> params) = 0;

	/*!
	 * \return	Priority, used to sort the Gesture Recognizers in the processing
	 * 			pipeline.
	 */
	virtual int priority() const = 0;

	/*!
	 * Sets priority for this Gesture Recognizer.
	 *
	 * \param	priority	Priority level.
	 */
	virtual void setPriority(int priority) = 0; // FIXME: This is a thing one cannot trust...

	/*!
	 * Sets the group of traces to be analyzed.
	 *
	 * \param	group	A group of traces, passed by reference.
	 */
	virtual void feedGroup(Group& group) = 0;

	/*!
	 * Sets the group of traces to be analyzed.
	 *
	 * \param	group	A group of traces, passed by pointer.
	 */
	virtual void feedGroup(Group* group) = 0;

public slots:
	/*!
	 * Slot responsible for gesture recognition, either progressive or not. This
	 * also delivers results.
	 *
	 * \sa result()
	 */
	virtual void recognize() = 0;

signals:
	/*!
	 * Signal emitted when a result is ready for consumption.
	 *
	 * \param	result	The result.
	 */
	void result(QStringList result);

protected:
	int _priority;
	QString _rcPath;
	QList<Group> _sGroups;
	QSet<Group*> _pGroups;
};

}

Q_DECLARE_INTERFACE(Doodle::IGestureRecognizer, "Doodle.IGestureRecognizer/1.0");

#endif /* IGESTURERECOGNIZER_H_ */
