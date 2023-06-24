greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += \
    src/board.h \
    src/fontmanager.h \
    src/gamescene.h \
    src/tile.h \
    src/utils.h \
    src/view.h

SOURCES += \
    src/board.cpp \
    src/fontmanager.cpp \
    src/gamescene.cpp \
    src/main.cpp \
    src/tile.cpp \
    src/view.cpp

RESOURCES += \
    resource.qrc
