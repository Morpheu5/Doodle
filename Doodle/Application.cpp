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

#include <stdexcept>
#include <cmath>

#include <QTimer>
#include <QDebug>
#include <QHash>
#include <QMap>
#include <QStringList>

#include <QtXml>
#include <QtXmlPatterns>
#include <QPluginLoader>

#include <TUIO/TuioTime.h>

#include "Application.h"
#include "Painter.h"
#include "Viewport.h"

#include <DoodleSDK/includes/Settings.h>
#include <DoodleSDK/includes/OSCHelper.h>
#include <DoodleSDK/includes/Tangible.h>
#include <DoodleSDK/includes/Cursor.h>
#include <DoodleSDK/includes/Trace.h>
#include <DoodleSDK/widgets/LabelWidget.h>
#include "Grid.h"

#include "TuioProxy.h"

Q_DECLARE_METATYPE(TUIO::TuioTime)

Doodle::Application::Application(int& argc, char** argv) : QApplication(argc, argv) {
	QStringList args = arguments();
	QString configFile;
	QStringList::iterator ai;
	for(ai = args.begin()+1; ai != args.end(); ++ai) {
		if(*ai == "-help" || *ai == "--help" || *ai == "-h") {
			QString help(tr("Usage: %1 [-config <configfilename.xml>]"));
			throw std::runtime_error(help.arg(args[0]).toAscii().data());
		} else if(*ai == "-config") {
			configFile = *(++ai);
		} else {
			QString warning(tr("Warning: %1 is an unknown option."));
			qWarning() << warning.arg(*ai).toAscii().data();
		}
	}
	if(configFile.isEmpty()) {
		configFile = "config.xml";
	}

	Settings::instance()->loadConfig(configFile);

	QString mute = Settings::instance()->get("audio", "mute");
	if(mute == "true") {
		_mute = true;
	} else {
		_mute = false;
	}

	QFile configXml(configFile);
	QDomDocument doc;
	if(!configXml.open(QIODevice::ReadOnly)) {
		QString e(tr("Unable to open %1."));
		throw std::runtime_error(e.arg(configFile).toAscii().data());
	}
	if(!doc.setContent(&configXml)) {
		configXml.close();
		QString e(tr("Unable to parse %1."));
		throw std::runtime_error(e.arg(configFile).toAscii().data());
	}

	QStringList gesturerecognizersTable;
	QStringList applicationsTable;

	QDomNodeList sections = doc.elementsByTagName("section");
	QDomNodeList associations;
	int i,j,k;
	for(i = 0; i < sections.size(); i++) {
		QDomElement sectionElement = sections.at(i).toElement();
		if(sectionElement.attribute("id") == "objects") {
			QDomNodeList objects = sectionElement.elementsByTagName("object");
			for(j = 0; j < objects.size(); j++) {
				QDomElement objectElement = objects.at(j).toElement();
				_objectsTable.insertMulti(objectElement.attribute("symbol").toInt(), objectElement.attribute("name").trimmed());
			}
		}
		if(sectionElement.attribute("id") == "gesturerecognizers") {
			QDomNodeList gesturerecognizers = sectionElement.elementsByTagName("gesturerecognizer");
			for(j = 0; j < gesturerecognizers.size(); j++) {
				QDomElement gesturerecognizerElement = gesturerecognizers.at(j).toElement();
				QString grName = gesturerecognizerElement.attribute("name").trimmed();
				gesturerecognizersTable.append(grName);
				QDomNodeList grParams = gesturerecognizerElement.elementsByTagName("param");
				for(k = 0; k < grParams.size(); k++) {
					QDomElement grParam = grParams.at(k).toElement();
					_grParams[grName].insert(grParam.attribute("name"), grParam.text().trimmed());
				}
			}
		}
		if(sectionElement.attribute("id") == "applications") {
			QDomNodeList applications = sectionElement.elementsByTagName("application");
			for(j = 0; j < applications.size(); j++) {
				QDomElement applicationElement = applications.at(j).toElement();
				QString appName = applicationElement.attribute("name").trimmed();
				QDomNodeList appParams = applicationElement.elementsByTagName("param");
				for(k = 0; k < appParams.size(); k++) {
					QDomElement appParam = appParams.at(k).toElement();
					_appParams[appName].insert(appParam.attribute("name"), appParam.text().trimmed());
				}
				applicationsTable.append(applicationElement.attribute("name").trimmed());
			}
		}
	}

	QString appsPath = retrieveConfigParam("system", "applicationspath", configFile);
	QString grsPath = retrieveConfigParam("system", "gesturerecognizerspath", configFile);
	QString rcPath = retrieveConfigParam("system", "resourcespath", configFile);

	foreach(QString grName, gesturerecognizersTable) {
		QPluginLoader loader(grsPath+"/lib"+grName+".so");
		QObject* plugin = loader.instance();
		if(plugin) {
			_grob.insert(grName, plugin);
			IGestureRecognizer* iGR = qobject_cast<IGestureRecognizer*>(plugin);
			if(iGR) {
				iGR->init(rcPath, _grParams[grName]);
				iGR->setPriority(_grParams[grName]["priority"].toInt());
				_gestureRecognizers.insert(grName, iGR);
				if(iGR->isProgressive()) {
					_progressiveGRs.insert(grName, iGR);
				} else {
					_staticGRs.insert(grName, iGR);
				}
				_grPriorities.insertMulti(iGR->priority(), iGR);
			} else {
				QString e(tr("[Doodle:internal] Unable to cast QObject* into IGestureRecognizer*."));
				throw std::runtime_error(e.toAscii().data());
			}
		} else {
			QString e(tr("[Doodle:internal] Unable to load %1 gesture recognizer: %2").arg(grName).arg(loader.errorString()));
			throw std::runtime_error(e.toAscii().data());
		}
	}
	foreach(QString appName, applicationsTable) {
		QPluginLoader loader(appsPath+"/lib"+appName+".so");
		QObject* plugin = loader.instance();
		if(plugin) {
			_appob.insert(appName, plugin);
			IApplication* iApp = qobject_cast<IApplication*>(plugin);
			if(iApp) {
				_applications.insert(appName, iApp);
				connect(plugin, SIGNAL(makeWidget(Widget*)), this, SLOT(addWidget(Widget*)));
				connect(plugin, SIGNAL(destroyWidget(Widget*)), this, SLOT(removeWidget(Widget*)));
				iApp->init(rcPath, _appParams[appName]);
			} else {
				QString e(tr("[Doodle:internal] Unable to cast QObject* into IApplication*."));
				throw std::runtime_error(e.toAscii().data());
			}
		} else {
			QString e(tr("[Doodle:internal] Unable to load %1 application: %2").arg(appName).arg(loader.errorString()));
			throw std::runtime_error(e.toAscii().data());
		}
	}

	QStringList l;
	foreach(IApplication* a, _applications) {
		l = a->declareObjects();
		QHash<long, QString> ol;
		foreach(QString o, l) {
			_objectToApplications.insertMulti(o, a);
			ol.insert(_objectsTable.key(o), o);
		}
		a->setObjects(ol);

		l = a->declareGestureRecognizers();
		_requestedGRs.insert(a, l);
		foreach(QString gr, l) {
			_gestureRecognizerToApplications.insertMulti(gr, a);
			// Other applications only receive the progressive GRs directly.
			if(_progressiveGRs.contains(gr)) {
				connect(_grob[gr], SIGNAL(result(QStringList)), _appob[a->name()], SLOT(processResults(QStringList)));
			}
		}
	}
	foreach(IGestureRecognizer* iGR, _gestureRecognizers) {
		QString gr = iGR->name();
		_gestureRecognizerToApplications.insertMulti(gr, this);
		if(_progressiveGRs.contains(gr)) {
			connect(_grob[gr], SIGNAL(result(QStringList)), SLOT(processResults(QStringList)));
		} else {
			connect(_grob[gr], SIGNAL(result(QStringList)), SLOT(processResults(QStringList)), Qt::DirectConnection);
		}
	}

	foreach(QObject* a, _appob) {
		foreach(QObject* b, _appob) {
			if(a != b) {
				connect(a, SIGNAL(deliverMessages(QStringList)), b, SLOT(processMessages(QStringList)));
			}
		}
		connect(a, SIGNAL(deliverMessages(QStringList)), SLOT(processMessages(QStringList)));
	}

	_grob.clear();
	_appob.clear();
	/* Here's where black magic ends. KTHXBYE. */

	// TODO: Provide a better settings scheme (maybe a Settings class)
	_fps = retrieveConfigParam("graphics", "fps", configFile).toInt();
	_traceTimeGap = retrieveConfigParam("trace", "timegap", configFile).toInt();
	_traceSpaceGap = retrieveConfigParam("trace", "spacegap", configFile).toFloat();
	_groupSpaceGap = retrieveConfigParam("group", "spacegap", configFile).toFloat();
	_multistroke = retrieveConfigParam("group", "multistroke", configFile) == "true" ? true : false;

	_grid.resize(32, 24);

	_painter = new Painter();
	_painter->setTangibles(&_tangibles);
	_painter->setTraces(&_traces);
	_painter->setWidgets(&_widgets);
	_painter->setTraceSpaceGap(&_traceSpaceGap);
	_painter->setGroupSpaceGap(&_groupSpaceGap);
	_painter->setHueStep(1.0/(_fps*retrieveConfigParam("graphics", "colorCycle", configFile).toFloat()));
	_viewport = new Viewport(_painter);
	_viewport->setXScale(retrieveConfigParam("graphics", "xScale", configFile).toDouble());
	_viewport->setYScale(retrieveConfigParam("graphics", "yScale", configFile).toDouble());
	_viewport->setXRotation(retrieveConfigParam("graphics", "xRotation", configFile).toDouble());
	_viewport->setZRotation(retrieveConfigParam("graphics", "zRotation", configFile).toDouble());
	_viewport->setXPosition(retrieveConfigParam("graphics", "xPosition", configFile).toDouble());
	_viewport->setYPosition(retrieveConfigParam("graphics", "yPosition", configFile).toDouble());
	_viewport->setZPosition(retrieveConfigParam("graphics", "zPosition", configFile).toDouble());
	QTimer* timer = new QTimer();
	connect(timer, SIGNAL(timeout()), _viewport, SLOT(animate()));
	timer->start(1000/_fps);
	_viewport->show();

	_tuioProxy = new TuioProxy(3333);
	TUIO::TuioTime::initSession();

	qRegisterMetaType<TUIO::TuioTime>("TUIO::TuioTime");

	connect(_tuioProxy, SIGNAL(addTangible(long, int, float, float, TUIO::TuioTime, float, float, float, float, float, float, float, int)), this, SLOT(addTangible(long, int, float, float, TUIO::TuioTime, float, float, float, float, float, float, float, int)));
	connect(_tuioProxy, SIGNAL(updateTangible(long, int, float, float, TUIO::TuioTime, float, float, float, float, float, float, float, int)), this, SLOT(updateTangible(long, int, float, float, TUIO::TuioTime, float, float, float, float, float, float, float, int)));
	connect(_tuioProxy, SIGNAL(removeTangible(long, int, float, float, TUIO::TuioTime, float, float, float, float, float, float, float, int)), this, SLOT(removeTangible(long, int, float, float, TUIO::TuioTime, float, float, float, float, float, float, float, int)));

	connect(_tuioProxy, SIGNAL(addCursor(long, int, float, float, TUIO::TuioTime, float, float, float, float, int)), this, SLOT(addCursor(long, int, float, float, TUIO::TuioTime, float, float, float, float, int)));
	connect(_tuioProxy, SIGNAL(updateCursor(long, int, float, float, TUIO::TuioTime, float, float, float, float, int)), this, SLOT(updateCursor(long, int, float, float, TUIO::TuioTime, float, float, float, float, int)));
	connect(_tuioProxy, SIGNAL(removeCursor(long, int, float, float, TUIO::TuioTime, float, float, float, float, int)), this, SLOT(removeCursor(long, int, float, float, TUIO::TuioTime, float, float, float, float, int)));

	connect(_tuioProxy, SIGNAL(tuioCycle(TUIO::TuioTime)), this, SLOT(tuioCycle(TUIO::TuioTime)));
}

Doodle::Application::~Application() {
	// FIXME: Thoroughly clean everything. Memory leaks make bunny cry!
	qDebug() << ">> ~Application()";

	delete OSCHelper::instance();

	delete _tuioProxy;
	delete _painter;
	delete _viewport;

	_objectToApplications.clear();

	foreach(IApplication* a, _applications) {
		QString name = a->name();
		_applications.remove(name);
	}

	foreach(IGestureRecognizer* gr, _gestureRecognizers) {
		QString name = gr->name();
		_gestureRecognizers.remove(name);
	}

	qDebug() << "<< ~Application()";
}

QString Doodle::Application::retrieveConfigParam(QString section, QString param, QString configFile = "config.xml") const {
	QXmlQuery query;
	query.bindVariable("configFile", QVariant(configFile));
	query.bindVariable("section", QVariant(section));
	query.bindVariable("param", QVariant(param));
	query.setQuery("for $i in doc($configFile)/doodle/section[@id=$section]/param[@name=$param]/text() return $i");
	QString result;
	query.evaluateTo(&result);
	return result.trimmed();
}

void Doodle::Application::processAddingTangibles() {
	QList<Tangible>::Iterator i;
	for(i = _addingTangibles.begin(); i != _addingTangibles.end(); ++i) {
		Tangible* t = new Tangible(*i);
		_tangibles.insert(i->sessionID(), t);
//		_grid.insert(t);
		QList<IApplication*> al = _objectToApplications.values(_objectsTable[t->fiducialID()]);
		if(!al.isEmpty()) {
			int j;
			for(j = 0; j < al.size(); j++) {
				al[j]->addTangible(t);
			}
		}
	}
}

void Doodle::Application::processUpdatingTangibles() {
	// TODO: Stress test for dangling pointers
	QList<Tangible>::Iterator i;
	for(i = _updatingTangibles.begin(); i != _updatingTangibles.end(); ++i) {
		Tangible* t = _tangibles[i->sessionID()];
		Tangible* u = new Tangible(*i);
		_tangibles.insert(i->sessionID(), u);
//		_grid.update(u);
		QList<IApplication*> al = _objectToApplications.values(_objectsTable[u->fiducialID()]);
		if(!al.isEmpty()) {
			int j;
			for(j = 0; j < al.size(); j++) {
				al[j]->updateTangible(u);
			}
		}
		delete t;
	}
}

void Doodle::Application::processRemovingTangibles() {
	QList<Tangible>::Iterator i;
	for(i = _removingTangibles.begin(); i != _removingTangibles.end(); ++i) {
		Tangible* t = _tangibles[i->sessionID()];
		_tangibles.remove(i->sessionID());
//		_grid.remove(t);
		QList<IApplication*> al = _objectToApplications.values(_objectsTable[t->fiducialID()]);
		if(!al.isEmpty()) {
			int j;
			for(j = 0; j < al.size(); j++) {
				al[j]->removeTangible(t);
			}
		}
		delete t;
	}
}

void Doodle::Application::processAddingCursors() {
	QList<Cursor>::Iterator i;
	for(i = _addingCursors.begin(); i != _addingCursors.end(); ++i) {
		Trace* t = resumeTrace(*i);
		if(t != NULL) {
			t->setState(Trace::RESUMED);
			t->append(*i);
			_timers[t]->stop();
			_timers.remove(t);
			_removedTraces.removeAll(t);
			if(_traces.contains(t->prevId())) {
				_traces.remove(t->prevId());
			}
		} else {
			t = new Trace();
			t->append(*i);
			Group* g = NULL;
			int j;
			// First: if the cursor is on a widget, assign its trace to the
			// group of the traces possibly already on the widget
			for(j = _widgets.size()-1; j >= 0; j--) {
				if((_widgets.at(j))->contains((*i).position())) {
					t->setWidget(_widgets.at(j));
					foreach(Trace* ot, _traces) {
						if(t != ot) {
							if(ot->widget() == t->widget()) {
								g = _groups[ot->groupId()];
								t->setGroupId(g->id());
								break;
							}
						}
					}
					_widgets.move(j, _widgets.size()-1);
					break;
				}
				if(g != NULL) {
					break;
				}
			}
			// Second: if the cursor is not on a widget, look for nearby groups
			if(g == NULL) {
				g = findNearbyGroup(t);
			}
			// Third: if no nearby group has been found, create a new one.
			if(g == NULL) {
				g = new Group;
				_groups.insert(g->id(), g);
			}
			g->appendTrace(t);
			t->setGroupId(g->id());
			t->setState(Trace::ADDED);
		}
		associateNearbyTangibles(t);
		_traces.insert(i->sessionID(), t);
		foreach(IGestureRecognizer* gr, _progressiveGRs) {
			gr->feedGroup(_groups[t->groupId()]);
		}
	}
}

void Doodle::Application::processUpdatingCursors() {
	QList<Cursor>::Iterator i;
	for(i = _updatingCursors.begin(); i != _updatingCursors.end(); ++i) {
		Trace* t = _traces[i->sessionID()];
		t->setState(Trace::UPDATED);
		t->append(*i);
		associateNearbyTangibles(t);
		foreach(IGestureRecognizer* gr, _progressiveGRs) {
			gr->feedGroup(_groups[t->groupId()]);
		}
	}
}

void Doodle::Application::processRemovingCursors() {
	QList<Cursor>::Iterator i;
	for(i = _removingCursors.begin(); i != _removingCursors.end(); ++i) {
		Trace* t = _traces[i->sessionID()];
//		t->append(*i); // TuioTime reports incorrectly
		_timers[t] = new QTimer();
		_timers[t]->setSingleShot(true);
		connect(_timers[t], SIGNAL(timeout()), SLOT(purgeOldTraces()));
		_traces[i->sessionID()]->setState(Trace::REMOVED);
		_removedTraces.append(_traces[i->sessionID()]);
		_timers[t]->start(_traceTimeGap);
	}
}

Doodle::Trace* Doodle::Application::resumeTrace(Cursor& c) {
	float maxDist = _traceSpaceGap * _traceSpaceGap;
	float dist;
	int i;
	Trace* t = NULL;
	for(i = 0; i < _removedTraces.size(); i++) {
		Cursor d = _removedTraces.at(i)->lastPoint();
		dist = c.squareDistance(d);
		if(dist < maxDist) {
			maxDist = dist;
			t = _removedTraces[i];
		}
	}
	return t;
}

Doodle::Group* Doodle::Application::findNearbyGroup(Trace* t) {
	Group* g = NULL;
	Cursor c = t->lastPoint();
	QHash<long, Trace*>::ConstIterator ti;

	// First: try to associate with a group on the same widget, if the trace is
	// on a widget.
	if(t->isOnWidget()) {
		for(ti = _traces.begin(); ti != _traces.end(); ++ti) {
			if((*ti)->isOnWidget()) {
				Widget* firstParent = t->widget();
				while(firstParent->parent() != NULL) {
					firstParent = firstParent->parent();
				}
				Widget* secondParent = (*ti)->widget();
				while(secondParent->parent() != NULL) {
					secondParent = secondParent->parent();
				}
				if(firstParent == secondParent) {
					g = _groups[(*ti)->groupId()];
					break;
				}
			}
		}
	}
	if(g) {
		return g;
	}

	// Second: try to find the nearest group.
	float maxDist = _groupSpaceGap * _groupSpaceGap;
	float dist;
	for(ti = _traces.begin(); ti != _traces.end(); ++ti) {
		if(((*ti)->isOnWidget() && !t->isOnWidget()) || (!(*ti)->isOnWidget() && t->isOnWidget())) {
			continue;
		}
		Cursor d = (*ti)->lastPoint();
		dist = c.squareDistance(d);
		if(dist < maxDist) {
			maxDist = dist;
			g = _groups[(*ti)->groupId()];
		}
	}
	return g;
}

void Doodle::Application::associateNearbyTangibles(Trace* t) {
	// FIXME: This commented snippet takes advantage of the Grid.
//	const Cursor& c = t->lastPoint();
//	QSet<Tangible*> s = _grid.at(c.x(), c.y());
//	foreach(Tangible* u, s) {
//		t->addTangible(u);
//	}
	if(t->isOnWidget()) {
		return;
	}
	float maxDist = _groupSpaceGap * _groupSpaceGap;
	QHash<long, Tangible*>::ConstIterator ti;
	for(ti = _tangibles.begin(); ti != _tangibles.end(); ++ti) {
		if((*ti)->squareDistance(t->lastPoint()) < maxDist) {
			t->addTangible(*ti);
		}
	}
}

void Doodle::Application::purgeOldTraces() {
	if(_removedTraces.isEmpty()) {
		// Prevent funny dangling pointers issues. Not so funny, actually.
		return;
	}
	// FIXME: Not sure the first one is the one to purge: find out.
	Trace* t = _removedTraces.takeFirst();
	// Declare the trace dead.
	t->setState(Trace::DEAD);
	// Make it a concrete object inside the group and take it away from the
	// pointers of temporary still alive traces.
	_groups[t->groupId()]->finalizeTrace(t);
	// When there are no more alive traces in the group, start the delivery.
	if(_groups[t->groupId()]->isEmpty()) {
		QList<long> objects;
		QStringList objNames;
		QStringList grNames;
		QStringList results;
		foreach(IGestureRecognizer* gr, _grPriorities) {
			Group* g = _groups[t->groupId()];
			gr->feedGroup(*g);
		}
//		foreach(QStringList sl, _sortedResults) {
//			_resultsBuffer << sl;
//		}
		// Now deliver the results to the applications
		foreach(IApplication* a, _applications) {
			foreach(QStringList result, _sortedResults) {
				if(_requestedGRs[a].contains(result[0])) {
					a->processResults(result);
					break;
				}
			}
//			QStringList result = _sortedResults[keys.first()];
//			if(_requestedGRs[a].contains(result[0])) {
//				a->processResults(result);
//			}
		}
		_sortedResults.clear();
		_resultsBuffer.clear();
		// Finally, throw away this group...
		_groups.remove(t->groupId());
	}
	// ... and the dead trace.
	_traces.remove(t->id());
	delete t;
}

void Doodle::Application::tuioCycle(TUIO::TuioTime ttime) {
	QMutexLocker locker(&paintMutex);
	processRemovingTangibles();
	processUpdatingTangibles();
	processAddingTangibles();
	processRemovingCursors();
	processUpdatingCursors();
	processAddingCursors();

	_addingTangibles.clear();
	_updatingTangibles.clear();
	_removingTangibles.clear();
	_addingCursors.clear();
	_updatingCursors.clear();
	_removingCursors.clear();
}

/* IApplication */

void Doodle::Application::processResults(QStringList results) {
	if(results.isEmpty()) {
		return;
	}
	QString name = results.takeFirst();
	if(_staticGRs.contains(name)) {
		results.prepend(name);
		_sortedResults.insertMulti(_staticGRs[name]->priority(), results);
		if(name == "MultiTap") {
			QList<unsigned long> wids;
			QStringList strWids = results[2].split(',', QString::SkipEmptyParts);
			foreach(QString s, strWids) {
				wids.append(s.toULong());
			}
			foreach(Widget* w, _widgets) {
				if(wids.contains(w->id())) {
					w->tap((results[3].toInt()-1)/2);
					break;
				}
			}
		}
	} else {
		if(name == "Pinch") {
			results.erase(results.begin(), results.begin() + 3);
			unsigned long widgetID = results.takeFirst().toInt();
			results.takeFirst(); // Waste the group id for now.
			QList<Widget*>::const_iterator wi;
			for(wi = _widgets.begin(); wi != _widgets.end(); ++wi) {
				if((*wi)->id() == widgetID) {
					float x = results.takeFirst().toFloat();
					float y = results.takeFirst().toFloat();
					QPointF displacement(x,y);
					float zoom = results.takeFirst().toFloat();
					float rotation = - results.takeFirst().toFloat();
					if((*wi)->pinchable()) {
						(*wi)->transform(displacement, zoom/8, rotation);
					} else {
						(*wi)->transform(displacement, 0, rotation);
					}
				}
			}
		}
	}
}

void Doodle::Application::processMessages(QStringList messages) {
	Q_UNUSED(messages);
}

void Doodle::Application::addTangible(Tangible* t) {
	Q_UNUSED(t)
}

void Doodle::Application::updateTangible(Tangible* t) {
	Q_UNUSED(t)
}

void Doodle::Application::removeTangible(Tangible* t) {
	Q_UNUSED(t)
}
