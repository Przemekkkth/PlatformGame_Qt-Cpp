greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    src/heroanim.cpp \
    src/game.cpp \
    src/gamescene.cpp \
    src/main.cpp \
    src/view.cpp

HEADERS += \
    src/heroanim.h \
    src/game.h \
    src/gamescene.h \
    src/view.h

RESOURCES += \
    resource.qrc
