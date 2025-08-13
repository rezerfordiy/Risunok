#!/bin/bash


qmake -makefile Make.pro

make && make clean




#if [ $# -eq 0 ]; then
#    echo "Ошибка: Не указаны параметры"
#    exit 1
#fi
#
#QT_PATH=$(brew --prefix qt)
#
#clang++ -std=c++17 \
#  -I"$QT_PATH"/include/qt \
#  -I./include \
#  -F"$QT_PATH"/lib \
#  -framework QtWidgets \
#  -framework QtGui \
#  -framework QtCore \
#  src/Graph.cpp \
#  src/GraphLoader.cpp \
#  src/AStar.cpp \
#  src/MainWindow.cpp \
#  src/main.cpp \
#  -o "$1" && ./"$1"
#

# qmake -makefile pro.pro

