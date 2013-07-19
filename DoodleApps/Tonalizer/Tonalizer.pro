TEMPLATE = lib
TARGET = $$qtLibraryTarget(Tonalizer)
QT += core \
    gui
HEADERS += ChordButtonWidget.h \
    ChordWidget.h \
    Chord.h \
    PadWidget.h \
    TonalizerWidget.h \
    Tonalizer.h
SOURCES += ChordWidget.cpp \
    Chord.cpp \
    PadWidget.cpp \
    TonalizerWidget.cpp \
    Tonalizer.cpp
INCLUDEPATH += /usr/include
CONFIG += plugin
LIBS += -lDoodleSDK
