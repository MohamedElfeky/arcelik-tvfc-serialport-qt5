#-------------------------------------------------
#
# Project created by QtCreator 2019-01-23T13:29:57
#
#-------------------------------------------------

QT       += core gui
QT       += serialport         # sudo apt-get install libqt5serialport5-dev

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ArcelikTV_FactoryCommand
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
