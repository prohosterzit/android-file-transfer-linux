#-------------------------------------------------
#
# Project created by QtCreator 2015-03-13T21:22:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt
TEMPLATE = app

QMAKE_CXXFLAGS += -pthread -std=c++14
INCLUDEPATH += ..
LIBS += -L.. -lmtp-ng -lusb-1.0

CONFIG += link_pkgconfig
PKGCONFIG += libusb

SOURCES += main.cpp\
        mainwindow.cpp \
    mtpobjectsmodel.cpp \
    progressdialog.cpp \
    createdirectorydialog.cpp

HEADERS  += mainwindow.h \
    mtpobjectsmodel.h \
    progressdialog.h \
    createdirectorydialog.h

FORMS    += mainwindow.ui \
    progressdialog.ui \
    createdirectorydialog.ui
