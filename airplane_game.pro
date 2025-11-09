QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = AirplaneGame
TEMPLATE = app

SOURCES += \
    main.cpp \
    gamewindow.cpp \
    game.cpp \
    player.cpp \
    enemy.cpp \
    bullet.cpp

HEADERS += \
    gamewindow.h \
    game.h \
    player.h \
    enemy.h \
    bullet.h

RESOURCES += resources.qrc