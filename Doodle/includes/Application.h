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

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <QApplication>
#include <QString>
#include <QMap>

#include <DoodleSDK/includes/Tangible.h>
#include <DoodleSDK/includes/IApplication.h>
#include <DoodleSDK/includes/IGestureRecognizer.h>
#include <DoodleSDK/includes/Widget.h>

#include "Grid.h"

class QTimer;

namespace Doodle {
class TuioProxy;
class Painter;
class Viewport;
class JackDriver;

/*!
 * The core of Doodle. This is both a QApplication and a Doodle Application
 * (IApplication) for practical purposes. As a QApplication, it inherits all the
 * benefits of Qt Framework, and as a Doodle Application it can handle Tangible
 * objects, Cursors and Widgets like any other Doodle Application, despite being
 * a special one.
 *
 * It is a special application because it acts like a supervisor, making all
 * the pieces work together.
 */
class Application : public QApplication, public IApplication {
	Q_OBJECT

	DOODLE_APPLICATION

public:
	/*!
	 * QApplication constructor.
	 *
	 * \param	argc	Command line arguments count.
	 * \param	argv	Command line arguments.
	 */
	Application(int& argc, char** argv);

	/*!
	 * Standard destructor.
	 */
	~Application();

	void init(QString rcPath, QHash<QString, QString> params) { }

	/*!
	 * Retrieves a configuration parameter from a given section in a given file.
	 *
	 * \param	section		Name of the section.
	 * \param	param		Name of the parameter.
	 * \param	configFile	Name of the configuration file.
	 */
	QString retrieveConfigParam(QString section, QString param, QString configFile) const;

	QString name() const { return QString("Doodle"); }
	QStringList declareObjects() const { return QStringList(); }
	QStringList declareGestureRecognizers() const { return QStringList(); }
	QStringList declareWidgets() const { return QStringList(); }

public slots:
	/*!
	 * Adds a Widget to a the global list of Widgets.
	 *
	 * \param	widget	The Widget to be added.
	 */
	void addWidget(Widget* widget) {
		if(!_widgets.contains(widget)) {
			_widgets.append(widget);
		}
	}

	/*!
	 * Removes a Widget from the global list of Widgets.
	 *
	 * \param	widget	The Widget to be removed.
	 */
	void removeWidget(Widget* widget) {
		_widgets.removeAll(widget);
	}

	/*!
	 * Adds a Tangible object to the global list of Tangible objects.
	 *
	 * For a description of parameters, see Tangible.
	 *
	 * \sa Tangible
	 */
	void addTangible(long sessionID, int fiducialID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, float angle, float rotationSpeed, float rotationAccel, int state) {
		_addingTangibles.append(Tangible(sessionID, fiducialID, x, y, ttime, xSpeed, ySpeed, motionSpeed, motionAccel, angle, rotationSpeed, rotationAccel, state));
	}

	/*!
	 * Updates a Tangible object already in the global list of Tangible objects.
	 *
	 * For a description of parameters, see Tangible.
	 *
	 * \sa Tangible
	 */
	void updateTangible(long sessionID, int fiducialID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, float angle, float rotationSpeed, float rotationAccel, int state) {
		_updatingTangibles.append(Tangible(sessionID, fiducialID, x, y, ttime, xSpeed, ySpeed, motionSpeed, motionAccel, angle, rotationSpeed, rotationAccel, state));
	}

	/*!
	 * Removes a Tangible object from the global list of Tangible objects.
	 *
	 * For a description of parameters, see Tangible.
	 *
	 * \sa Tangible
	 */
	void removeTangible(long sessionID, int fiducialID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, float angle, float rotationSpeed, float rotationAccel, int state) {
		_removingTangibles.append(Tangible(sessionID, fiducialID, x, y, ttime, xSpeed, ySpeed, motionSpeed, motionAccel, angle, rotationSpeed, rotationAccel, state));
	}

	/*!
	 * Adds a cursor to the global list of Cursors.
	 *
	 * For a description of parameters, see Cursor.
	 *
	 * \sa Cursor
	 */
	void addCursor(long sessionID, int fingerID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, int state) {
		_addingCursors.append(Cursor(sessionID, fingerID, x, y, ttime, xSpeed, ySpeed, motionSpeed, motionAccel, state));
	}

	/*!
	 * Updates a cursor already in the global list of Cursors.
	 *
	 * For a description of parameters, see Cursor.
	 *
	 * \sa Cursor
	 */
	void updateCursor(long sessionID, int fingerID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, int state) {
		_updatingCursors.append(Cursor(sessionID, fingerID, x, y, ttime, xSpeed, ySpeed, motionSpeed, motionAccel, state));
	}

	/*!
	 * Removes a cursor from the global list of Cursors.
	 *
	 * For a description of parameters, see Cursor.
	 *
	 * \sa Cursor
	 */
	void removeCursor(long sessionID, int fingerID, float x, float y, TUIO::TuioTime ttime, float xSpeed, float ySpeed, float motionSpeed, float motionAccel, int state) {
		_removingCursors.append(Cursor(sessionID, fingerID, x, y, ttime, xSpeed, ySpeed, motionSpeed, motionAccel, state));
	}

	/*!
	 * Processes events occurred in the latest TUIO cycle.
	 * reacTIVision samples the video stream at regular intervals. At the
	 * beginning of each sampling phase, a TUIO cycle is started. A cycle
	 * finishes at the following sampling phase. In the meanwhile, reacTIVision
	 * analyzes the current frame and delivers the results (Tangible objects and
	 * Cursors) to TuioListeners. These listeners can directly work with this
	 * information (simple applications that don't need sophisticated
	 * processing) or pass them to more complex applications.
	 *
	 * In Doodle the latter approach is chosen.
	 *
	 * \sa TuioListener
	 * \sa TuioProxy
	 *
	 * \param	ttime	TUIO timestamp.
	 *
	 */
	void tuioCycle(TUIO::TuioTime ttime);

	/*!
	 * Removes dead Traces from the temporary queue and delivers them to
	 * Gesture Recognizers.
	 *
	 * When a dead Trace is removed, it is finalized in its Group. If the
	 * Group is empty (that is, if its temporary list of Traces is empty) then
	 * it is fed to Gesture Recognizers, and their results are delivered to
	 * relevant Applications.
	 */
	void purgeOldTraces();

	void processResults(QStringList results);
	void processMessages(QStringList messages);
	void addTangible(Tangible* t);
	void updateTangible(Tangible* t);
	void removeTangible(Tangible* t);

signals:
	/*!
	 * \internal
	 */
	void staticResults(QStringList results);

	/*!
	 * Signal emitted to notify interested parties that a list of messages is
	 * available.
	 *
	 * \param	messages	List of messages.
	 */
	void deliverMessages(QStringList messages);

private:
	/*!
	 * Processes Tangible objects that were added in the latest TUIO Cycle.
	 *
	 * \sa tuioCycle
	 */
	void processAddingTangibles();

	/*!
	 * Processes Tangible objects that were updated in the latest TUIO Cycle.
	 *
	 * \sa tuioCycle
	 */
	void processUpdatingTangibles();

	/*!
	 * Processes Tangible objects that were removed in the latest TUIO Cycle.
	 */
	void processRemovingTangibles();

	/*!
	 * Processes Cursors that were added in the latest TUIO Cycle.
	 */
	void processAddingCursors();

	/*!
	 * Processes Cursors that were updated in the latest TUIO Cycle.
	 */
	void processUpdatingCursors();

	/*!
	 * Processes Cursors that were removed in the latest TUIO Cycle.
	 */
	void processRemovingCursors();

	/*!
	 * Resumes a Trace. If a cursor happens near the last point of a Trace
	 * within a time interval, then that trace is brought back to life and
	 * another Stroke is created.
	 *
	 * \param	c	The new Cursor.
	 *
	 * \return	The resumed Trace.
	 */
	Trace* resumeTrace(Cursor& c);

	/*!
	 * Tries to associate a Trace with a nearby Group. If a suitable Group is
	 * not found, then a new one is created.
	 *
	 * \param	t	The Trace to be associated with a Group.
	 *
	 * \return	The Group that the Trace is now associated with.
	 */
	Group* findNearbyGroup(Trace* t);

	/*!
	 * Associates Tangible objects that are near a given Trace.
	 *
	 * \param	t	The Trace.
	 */
	void associateNearbyTangibles(Trace* t);

	QList<Tangible> _addingTangibles;
	QList<Tangible> _updatingTangibles;
	QList<Tangible> _removingTangibles;

	QList<Cursor> _addingCursors;
	QList<Cursor> _updatingCursors;
	QList<Cursor> _removingCursors;

	// FIXME: There are several hash tables and maps for which the "multi"
	// variant would be more appropriate and meaningful. Make this correction.
	QHash<Trace*, QTimer*> _timers;

	QHash<long, Trace*> _traces;
	QList<Trace*> _removedTraces;

	QHash<long, Tangible*> _tangibles;
	QHash<long, Cursor*> _cursors;
	QHash<long, Group*> _groups;

	Grid<Tangible> _grid;

	QHash<long, QString> _objectsTable;
	QHash<QString, QHash<QString, QString> > _grParams;
	QHash<QString, QHash<QString, QString> > _appParams;

	QHash<QString, QObject*> _grob;
	QHash<QString, QObject*> _appob;

	QHash<QString, IGestureRecognizer*> _gestureRecognizers;
	QHash<QString, IGestureRecognizer*> _staticGRs;
	QHash<QString, IGestureRecognizer*> _progressiveGRs;
	QMap<long, IGestureRecognizer*> _grPriorities;

	QHash<QString, IApplication*> _applications;
	QHash<QString, IApplication*> _objectToApplications;
	QHash<QString, IApplication*> _gestureRecognizerToApplications;

	// TODO: May be better with application's name?
	QHash<IApplication*, QStringList> _requestedGRs;

	/* Settings.
	 * TODO: Provide a better scheme, maybe a separate class. */
	int _fps;
	float _traceTimeGap;
	float _traceSpaceGap;
	float _groupSpaceGap;
	bool _multistroke;

	bool _mute;

	/* Service classes. */
	TuioProxy* _tuioProxy;

	Painter* _painter;
	Viewport* _viewport;

	QList<Widget*> _widgets;

	QMap<long, QStringList> _sortedResults;
	QStringList _resultsBuffer;

	JackDriver* _jackDriver;
};
}

#endif /* APPLICATION_H_ */
