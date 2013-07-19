TEMPLATE = lib
TARGET = $$qtLibraryTarget(MusicTouch)
QT += core \
    gui
HEADERS += MusicTouch.h
SOURCES += MusicTouch.cpp
INCLUDEPATH += /usr/include
CONFIG += plugin
LIBS += -lDoodleSDK
