#include <QtGui>

#include "GlyphView.h"
#include "GlyphPainter.h"

GlyphView::GlyphView(GlyphPainter* glyphPainter, QWidget* parent)
	: QGLWidget(parent), _glyphPainter(glyphPainter) {
	elapsed = 0;
	setFixedSize(400,400);
	setFocusPolicy(Qt::StrongFocus);
}

void GlyphView::animate() {
	elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
	repaint();
}

void GlyphView::paintEvent(QPaintEvent* event) {
	QPainter painter;
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);
	_glyphPainter->paint(&painter, event, elapsed);
	painter.end();
}

void GlyphView::mousePressEvent(QMouseEvent* event) {
	emit(pointClicked(event->pos()));
}

void GlyphView::mouseMoveEvent(QMouseEvent* event) {
	emit(mouseDrag(event->pos()));
}

void GlyphView::mouseReleaseEvent(QMouseEvent* event) {
	emit(endMotion(event->pos()));
}

void GlyphView::keyPressEvent(QKeyEvent* event) {
	emit(keyPressed(event->key()));
}

void GlyphView::keyReleaseEvent(QKeyEvent* event) {
	emit(keyReleased(event->key()));
}
