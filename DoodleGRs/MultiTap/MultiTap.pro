TEMPLATE = lib
CONFIG += plugin
TARGET = $$qtLibraryTarget(MultiTap)
QT += core \
    gui
HEADERS += MultiTap.h
SOURCES += MultiTap.cpp
INCLUDEPATH += /usr/include
LIBS += -lDoodleSDK
