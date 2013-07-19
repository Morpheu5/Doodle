TEMPLATE = lib
TARGET = $$qtLibraryTarget(PolyGlyphEngine)
QT += core \
    gui \
    xml
HEADERS += PolyGlyphEngine.h
SOURCES += PolyGlyphEngine.cpp
CONFIG += plugin
INCLUDEPATH += ../Glyph
LIBS += -lGlyph
