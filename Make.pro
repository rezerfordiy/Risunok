QT += core gui widgets

TARGET = MyApp
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/Graph.cpp \
    src/AStar.cpp \
    src/GraphLoader.cpp

HEADERS += \
    include/MainWindow.h \
    include/Graph.h \
    include/AStar.h \
    include/GraphLoader.h

INCLUDEPATH += include

# Копирование input.txt к бинарнику только для мака
macx {
    QMAKE_POST_LINK += $$quote(mkdir -p $$OUT_PWD/$${TARGET}.app/Contents/MacOS && cp -f $$PWD/input.txt $$OUT_PWD/$${TARGET}.app/Contents/MacOS/)
}
