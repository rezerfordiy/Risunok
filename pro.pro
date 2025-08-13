
QT       += core gui widgets

TARGET = test
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    src/test.cpp \
    src/Graph.cpp \
    src/AStar.cpp \

HEADERS += \
    include/Graph.h \
    include/AStar.h \

INCLUDEPATH += include
