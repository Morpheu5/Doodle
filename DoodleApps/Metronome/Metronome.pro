TEMPLATE = lib
TARGET = $$qtLibraryTarget(Metronome)
QT += core \
    gui
INCLUDEPATH += /usr/include
HEADERS += MetronomeWidget.h \
    Metronome.h
SOURCES += MetronomeWidget.cpp \
    Metronome.cpp
CONFIG += plugin
LIBS += -lDoodleSDK
