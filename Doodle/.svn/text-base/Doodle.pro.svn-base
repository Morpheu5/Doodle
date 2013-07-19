TEMPLATE = app
TARGET = Doodle
QT += core \
    gui \
    opengl \
    xml \
    xmlpatterns
HEADERS += includes/TangibleWidget.h \
    includes/Viewport.h \
    includes/Grid.h \
    includes/TuioProxy.h \
    includes/TuioListener.h \
    includes/Painter.h \
    includes/Application.h
SOURCES += TangibleWidget.cpp \
    Viewport.cpp \
    TuioListener.cpp \
    TuioProxy.cpp \
    Painter.cpp \
    Application.cpp \
    main.cpp
FORMS += 
RESOURCES += 
INCLUDEPATH += includes/
LIBS += -lTUIO \
    -lDoodleSDK
QMAKE_CXXFLAGS += -finline-functions \
    -ffast-math \
    -funroll-all-loops \
    -fomit-frame-pointer
