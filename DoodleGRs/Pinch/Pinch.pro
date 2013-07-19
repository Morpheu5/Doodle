TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(Pinch)
QT += core \
    gui
HEADERS += Pinch.h
SOURCES += Pinch.cpp
INCLUDEPATH += /usr/include
LIBS += -lDoodleSDK
