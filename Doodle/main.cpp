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

#include <QtGui>
#include <QtOpenGL>
#include "Application.h"
using namespace Doodle;

int main(int argc, char *argv[]) {
	Application* a;
	try {
		a = new Application(argc, argv);
	} catch (std::runtime_error e) {
		qCritical() << e.what();
		return -1;
	} catch (std::exception e) {
		qCritical() << e.what();
		return -1;
	}

	if (!QGLFormat::hasOpenGL() || !QGLFramebufferObject::hasOpenGLFramebufferObjects()) {
		qCritical() << "This system doesn't support OpenGL and/or Framebuffer Objects";
	}

	int r = a->exec();
	delete a;
    return r;
}
