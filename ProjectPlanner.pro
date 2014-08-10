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
    model/datetime.cpp \
    model/calendar.cpp \
    model/calendarsmodel.cpp \
    model/resource.cpp \
    model/resourcesmodel.cpp \
    model/task.cpp \
    model/tasksmodel.cpp \
    delegate/daysdelegate.cpp \
    delegate/calendarsdelegate.cpp \
    delegate/resourcesdelegate.cpp \
    delegate/tasksdelegate.cpp \
    delegate/xtimeedit.cpp \
    gui/ganttchart.cpp \
    gui/ganttscale.cpp \
    delegate/xdateedit.cpp

HEADERS  += \
    gui/mainwindow.h \
    gui/maintabwidget.h \
    gui/ganttview.h \
    gui/xtableview.h \
    gui/propertieswidget.h \
    model/plan.h \
    model/day.h \
    model/daysmodel.h \
    model/datetime.h \
    model/calendar.h \
    model/calendarsmodel.h \
    model/resource.h \
    model/resourcesmodel.h \
    model/task.h \
    model/tasksmodel.h \
    model/timespan.h \
    command/commanddaysetdata.h \
    command/commandcalendarsetdata.h \
    command/commandresourcesetdata.h \
    command/commandtasksetdata.h \
    delegate/daysdelegate.h \
    delegate/calendarsdelegate.h \
    delegate/resourcesdelegate.h \
    delegate/tasksdelegate.h \
    delegate/xtimeedit.h \
    gui/ganttchart.h \
    gui/ganttscale.h \
    delegate/xdateedit.h

FORMS += \
    gui/mainwindow.ui \
    gui/maintabwidget.ui
