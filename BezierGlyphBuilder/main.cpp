#include "BezierGlyphBuilder.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BezierGlyphBuilder w;
    w.show();
    return a.exec();
}
