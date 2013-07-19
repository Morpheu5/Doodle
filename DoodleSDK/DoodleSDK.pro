TEMPLATE = lib
TARGET = DoodleSDK
QT += core \
    gui \
    xmlpatterns \
    xml
HEADERS += includes/OSCHelper.h \
    includes/Settings.h \
    widgets/PushButtonWidget.h \
    widgets/CheckButtonWidget.h \
    widgets/LabelWidget.h \
    includes/Widget.h \
    includes/IApplication.h \
    includes/IGestureRecognizer.h \
    includes/Target.h \
    includes/Cursor.h \
    includes/Tangible.h \
    includes/Trace.h \
    includes/Stroke.h \
    includes/Group.h
SOURCES += OSCHelper.cpp \
    Settings.cpp \
    widgets/PushButtonWidget.cpp \
    widgets/CheckButtonWidget.cpp \
    widgets/LabelWidget.cpp \
    Widget.cpp \
    Cursor.cpp \
    Group.cpp \
    Stroke.cpp \
    Tangible.cpp \
    Target.cpp \
    Trace.cpp
FORMS += 
RESOURCES += 
INCLUDEPATH += includes/
QMAKE_CXXFLAGS += -finline-functions \
    -ffast-math \
    -funroll-all-loops \
    -fomit-frame-pointer
LIBS += -ljack \
    -lpthread \
    -lrt \
    -loscpack
