TEMPLATE = lib
TARGET = $$qtLibraryTarget(Sequencer)
QT += core \
    gui
HEADERS += SequenceButtonWidget.h \
    KeyboardWidget.h \
    SequencerWidget.h \
    Sequencer.h
SOURCES += KeyboardWidget.cpp \
    SequencerWidget.cpp \
    Sequencer.cpp
INCLUDEPATH += /usr/include
CONFIG += plugin
LIBS += -lDoodleSDK
