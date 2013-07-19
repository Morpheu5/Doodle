TEMPLATE = lib
TARGET = $$qtLibraryTarget(BezierGlyphEngine)
QT += core \
    gui \
    xml
HEADERS += BezierGlyphEngine.h
SOURCES += BezierGlyphEngine.cpp
CONFIG += plugin
INCLUDEPATH += ../Glyph
LIBS += -lGlyph
