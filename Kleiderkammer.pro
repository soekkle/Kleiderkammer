#-------------------------------------------------
#
# Project created by QtCreator 2014-11-18T19:34:51
#
#-------------------------------------------------

QT       += core gui sql webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport webkitwidgets

TARGET = Kleiderkammer
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    datenquelle.cpp \
    sqlitequelle.cpp \
    kleidungstypenverwaltung.cpp \
    gruppenverwaltung.cpp \
    bericht.cpp \
    tableviews/kleidungstableview.cpp \
    tableviews/kleidungsgroessentableview.cpp \
    kleidersuche.cpp \
    tableviews/itemsdelegate.cpp

HEADERS  += mainwindow.h \
    datenquelle.h \
    sqlitequelle.h \
    kleidungstypenverwaltung.h \
    gruppenverwaltung.h \
    bericht.h \
    tableviews/kleidungstableview.h \
    tableviews/kleidungsgroessentableview.h \
    kleidersuche.h \
    versioninfo.h \
    tableviews/itemsdelegate.h

FORMS    += mainwindow.ui \
    kleidungstypenverwaltung.ui \
    gruppenverwaltung.ui \
    kleidersuche.ui
# Setzen der VersionsInfos
VERSION= 0.0.1

# Da unter Qt 5 Probleme mit dem Drucken von WebWidgeds auftreten wird es hier deaktivirt
equals(QT_MAJOR_VERSION,5):{
    DEFINES +=NOPRINT
}
