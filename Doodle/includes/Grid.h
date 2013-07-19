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

#ifndef GRID_H_
#define GRID_H_

#include <QVector>
#include <QSet>
#include <QDebug>
#include <cmath>

namespace Doodle {

/*!
 * A Grid for quick search and location of objects in Viewport (GL_QUAD) space.
 * This is used to quickly find neightbors of a given object (point).
 *
 * Template parameter T is a Target.
 */
template <class T>
class Grid {
public:
	/*!
	 * Standard constructor.
	 */
	Grid() { }

	/*!
	 * Standard destructor.
	 */
	virtual ~Grid() { }

	/*!
	 * Subdivides the space in rows and columns.
	 *
	 * \param	rows	Number of rows.
	 * \param	cols	Number of columns.
	 */
	void resize(int rows, int cols) {
		_grid.resize(rows);
		int i;
		for(i= 0; i < _grid.size(); i++) {
			_grid[i].resize(cols);
		}
	}

	/*!
	 * \param	row		Row coordinate.
	 * \param	col		Column coordinate.
	 *
	 * \return	The objects in the cell (row,col).
	 */
	const QSet<T*>& at(int row, int col) const { return _grid[row][col]; }

	/*!
	 * Converts a position into a Grid cell and returns the objects in that
	 * cell.
	 *
	 * \param	x	x coordinate.
	 * \param	y	y coordinate.
	 *
	 * \return The set of objects in the cell containing (x,y).
	 */
	const QSet<T*>& at(float x, float y) const {
		int nx = (int)floor(((x-0.1666)/1.3333)*_grid[0].size())-1;
		int ny = (int)floor(y*_grid.size())-1;
		return at(ny, nx);
	}

	/*!
	 * \param	t	A Target, passed by pointer.
	 *
	 * \return	The set of objects in the same cell of t.
	 */
	const QSet<T*>& at(T* t) const { return at(t->x(), t->y()); }

	/*!
	 * \param	t	A Target, passed by reference.
	 *
	 * \return	The set of objects in the same cell of t.
	 */
	const QSet<T*>& at(const T& t) const { return at(t.x(), t.y()); }

	/*!
	 * Computes the Grid cell that the Target falls into. Then stores the Target
	 * in that cell.
	 *
	 * \param	t	A Target.
	 */
	void insert(T* t) {
		int nx = (int)floor(((t->x()-0.1666)/1.3333)*(_grid[0].size())-1);
		int ny = (int)floor(t->y()*(_grid.size())-1);
		_grid[ny][nx].insert(t);
	}

	/*!
	 * Updates a Target that is already into the Grid.
	 *
	 * \param	t	A Target.
	 */
	void update(T* t) {
		int nx = (int)floor(((t->x()-0.1666)/1.3333)*(_grid[0].size())-1);
		int ny = (int)floor(t->y()*(_grid.size())-1);
		bool found = false;
		foreach(T* u, _grid[ny][nx]) {
			if(u->sessionID() == t->sessionID()) {
				found = true;
			}
		}

		if(!found) {
			remove(t);
		}
		_grid[ny][nx].insert(t);
	}

	/*!
	 * Removes a Target from the Grid.
	 *
	 * \param	t	A Target.
	 */
	void remove(T* t) {
		foreach(QVector<QSet<T*> > v, _grid) {
			foreach(QSet<T*> s, v) {
				foreach(T* u, s) {
					if(u->sessionID() == t->sessionID()) {
						s.remove(u);
					}
				}
			}
		}
	}

private:
	QVector<QVector<QSet<T*> > > _grid;
};
}

#endif /* GRID_H_ */
