QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Snake
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    main.cpp \
    gamewindow.cpp \
    gameboard.cpp \
    gameconfigdialog.cpp

HEADERS += \
    gamewindow.h \
    gameboard.h \
    gameconfigdialog.h