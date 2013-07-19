#ifndef GLYPHVIEW_H_
#define GLYPHVIEW_H_

#include <QGLWidget>

class GlyphPainter;
class QPaintEvent;

/*!
 * An OpenGL 2D viewport and editing area.
 */
class GlyphView : public QGLWidget {
	Q_OBJECT

public:
	/*!
	 * Standard constructor.
	 *
	 * \param	glyphPainter	GlyphPainter helper instance.
	 * \param	parent			Parent widget.
	 */
	GlyphView(GlyphPainter* glyphPainter, QWidget* parent);

signals:
	/*!
	 * Signal emitted when the editing area receives a mouse click event.
	 *
	 * \param	p	Coordinates of the click.
	 */
	void pointClicked(QPoint p);

	/*!
	 * Signal emitted when the editing area receives a mouse drag event (button
	 * press and motion).
	 *
	 * \param	p	Coordinates of the drag event.
	 */
	void mouseDrag(QPoint p);

	/*!
	 * Signal emitted when the mouse stops moving on the editing area.
	 */
	void endMotion(QPoint p);

	/*!
	 * Signal emitted when the editing area receives a key pressed event.
	 *
	 * \param	k	The pressed key.
	 */
	void keyPressed(int k);

	/*!
	 * Signal emitted when the editing area receives a key released event.
	 *
	 * \param	k	The released key.
	 */
	void keyReleased(int k);

public slots:
	/*!
	 * Updates the visuals.
	 */
	void animate();

protected:
	/*!
	 * Emits pointClicked.
	 *
	 * \param	event	The mouse click event.
	 */
    void mousePressEvent(QMouseEvent* event);

    /*!
     * Emits mouseDrag.
     *
     * \param	event	The mouse motion event.
     */
    void mouseMoveEvent(QMouseEvent* event);

    /*!
     * Emits endMotion.
     *
     * \param	event	The mouse release event.
     */
    void mouseReleaseEvent(QMouseEvent* event);

    /*!
     * Calls the GlyphPainter paint routine.
     *
     * \param	event	The QPaintEvent.
     */
	void paintEvent(QPaintEvent* event);

	/*!
	 * Emits keyPressed.
	 *
	 * \param	event	The key pressed event.
	 */
	void keyPressEvent(QKeyEvent* event);

	/*!
	 * Emits keyReleased.
	 *
	 * \param	event	The key released event.
	 */
	void keyReleaseEvent(QKeyEvent* event);

private:
	GlyphPainter* _glyphPainter;
	int elapsed;
};

#endif /* GLYPHVIEW_H_ */
