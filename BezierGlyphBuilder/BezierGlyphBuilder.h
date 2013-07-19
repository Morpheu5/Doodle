#ifndef BEZIERGLYPHBUILDER_H
#define BEZIERGLYPHBUILDER_H

#include <stdexcept>

#include <QtGui/QDialog>
#include <QString>
#include <QList>
#include <QPointF>
#include <QtXml>

#include "ui_BezierGlyphBuilder.h"

#include "GlyphView.h"
#include "GlyphPainter.h"

/*!
 * Main window.
 */
class BezierGlyphBuilder : public QDialog
{
	Q_OBJECT

public:
	/*!
	 * Standard constructor.
	 */
	BezierGlyphBuilder(QWidget *parent = 0);

public slots:
	/*!
	 * Loads a set of glyphs from a file. These are Bézier curves of low degree.
	 */
	void loadGlyphsFromFile();

	/*!
	 * For each glyph there can be variants to account for different ways users
	 * can write them.
	 */
	void setVariants(QString id);

	/*!
	 * Loads a particular variant of a glyph in the editing area.
	 */
	void loadGlyph(QString variant);

	/*!
	 * Sets the number of constant time samples for output.
	 */
	void setSamples(int i);

	/*!
	 * Selection (either single or multiple) of control points.
	 */
	void pointClicked(QPoint p);

	/*!
	 * Move control points.
	 */
	void movePoints(QPoint);

	/*!
	 * When the movement of control points finishes.
	 */
	void endMotion(QPoint);

	/*!
	 * Keyboard commands.
	 *
	 * -	A: adds a control point between selected control points.
	 * -	X: deletes a control point.
	 * -	CTRL: makes _ctrl_pressed true.
	 */
	void keyPressed(int k);

	/*!
	 * Makes _ctrl_pressed false when CTRL is released.
	 */
	void keyReleased(int k);

	/*!
	 * Saves editings done to a glyph variant.
	 */
	void saveVariant();

	/*!
	 * Computes constant time samples of each variant and writes them to an XML
	 * file.
	 */
	void buildGlyphs();

private:
	Ui::BezierGlyphBuilderClass ui;

	GlyphView* _view;
	GlyphPainter _glyphPainter;

	QDomDocument _glyphsXml;
	QList<QPointF> _currentGlyph;
	QList<QPointF> _activePoints;

	QPoint* _oldPoint;

	bool _ctrl_pressed;
};

#endif // BEZIERGLYPHBUILDER_H
