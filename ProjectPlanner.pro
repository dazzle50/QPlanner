#-------------------------------------------------
#
# Project created by QtCreator 2014-06-10T19:39:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjectPlanner
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
    gui/mainwindow.cpp \
    gui/maintabwidget.cpp \
    gui/ganttview.cpp \
    gui/xtableview.cpp \
    model/plan.cpp \
    model/day.cpp \
    model/daysmodel.cpp \
    model/datetime.cpp

HEADERS  += \
    gui/mainwindow.h \
    gui/maintabwidget.h \
    gui/ganttview.h \
    gui/xtableview.h \
    gui/propertieswidget.h \
    model/plan.h \
    model/day.h \
    model/daysmodel.h \
    model/datetime.h

FORMS += \
    gui/mainwindow.ui \
    gui/maintabwidget.ui
