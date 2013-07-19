TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(Glyph)
QT += core \
    gui
HEADERS += IGlyphEngine.h \
    Glyph.h
SOURCES += Glyph.cpp
INCLUDEPATH += /usr/include
LIBS += -lDoodleSDK
