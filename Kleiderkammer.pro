#-------------------------------------------------
#
# Project created by QtCreator 2014-11-18T19:34:51
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kleiderkammer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    datenquelle.cpp \
    sqlitequelle.cpp \
    kleidungstypenverwaltung.cpp \
    gruppenverwaltung.cpp \
    bericht.cpp

HEADERS  += mainwindow.h \
    datenquelle.h \
    sqlitequelle.h \
    kleidungstypenverwaltung.h \
    gruppenverwaltung.h \
    bericht.h

FORMS    += mainwindow.ui \
    kleidungstypenverwaltung.ui \
    gruppenverwaltung.ui
