#-------------------------------------------------
#
# Project created by QtCreator 2014-05-06T10:14:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjektPJC
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cell.cpp \
    track.cpp \
    game.cpp \
    player.cpp \
    usercar.cpp \
    computercar.cpp \
    abstractcar.cpp \
    trackgui.cpp \
    playerselectwidget.cpp

HEADERS  += mainwindow.h \
    cell.h \
    track.h \
    game.h \
    player.h \
    usercar.h \
    computercar.h \
    abstractcar.h \
    trackgui.h \
    playerselectwidget.h

FORMS    += mainwindow.ui \
    playerselectwidget.ui

RESOURCES += \
    Tracks.qrc \
    Textures.qrc

OTHER_FILES += \
    trackOne.txt
