TEMPLATE = lib
TARGET = $$qtLibraryTarget(StraightStroke)
QT += core \
    gui
HEADERS += StraightStroke.h
SOURCES += StraightStroke.cpp
INCLUDEPATH += /usr/include
CONFIG += plugin
LIBS += -lDoodleSDK
