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
    Oberflaeche/mainwindow.cpp \
    DatenQuellen/datenquelle.cpp \
    DatenQuellen/sqlitequelle.cpp \
    Oberflaeche/kleidungstypenverwaltung.cpp \
    Oberflaeche/gruppenverwaltung.cpp \
    bericht.cpp \
    tableviews/kleidungstableview.cpp \
    tableviews/kleidungsgroessentableview.cpp \
    Oberflaeche/kleidersuche.cpp \
    tableviews/itemsdelegate.cpp \
    Oberflaeche/personbearbeitendialog.cpp

HEADERS  += Oberflaeche/mainwindow.h \
    DatenQuellen/datenquelle.h \
    DatenQuellen/sqlitequelle.h \
    Oberflaeche/kleidungstypenverwaltung.h \
    Oberflaeche/gruppenverwaltung.h \
    bericht.h \
    tableviews/kleidungstableview.h \
    tableviews/kleidungsgroessentableview.h \
    Oberflaeche/kleidersuche.h \
    versioninfo.h \
    tableviews/itemsdelegate.h \
    Oberflaeche/personbearbeitendialog.h

FORMS    += Oberflaeche/mainwindow.ui \
    Oberflaeche/kleidungstypenverwaltung.ui \
    Oberflaeche/gruppenverwaltung.ui \
    Oberflaeche/kleidersuche.ui \
    Oberflaeche/personbearbeitendialog.ui
# Setzen der VersionsInfos
VERSION= 0.3.0

# Da unter Qt 5 Probleme mit dem Drucken von WebWidgeds auftreten wird es hier deaktivirt
equals(QT_MAJOR_VERSION,5):{
    DEFINES +=NOPRINT
}
