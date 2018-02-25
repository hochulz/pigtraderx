#-------------------------------------------------
#
# Project created by QtCreator 2018-01-27T18:01:09
#
#-------------------------------------------------

QT       += core gui
QT += axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pigtraderX
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    stock.cpp \
    kiwoom.cpp \
    etf.cpp

HEADERS += \
        mainwindow.h \
    cpcybos.h \
    stockmst2.h \
    stockcur.h \
    stockjpbid.h \
    cpsvr7244.h \
    cpsvrnew7244s.h \
    stock.h \
    cptdnew5331a.h \
    cptdutil.h \
    kiwoom.h \
    cpcodemgr.h \
    etf.h \
    futuremst.h \
    futurecuronly.h \
    cptd6033.h \
    cpconclusion.h \
    cptd0311.h

FORMS += \
        mainwindow.ui
