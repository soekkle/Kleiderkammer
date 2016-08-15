/*
 * Copyright (C) 2014-2016 Sören Krecker
 *
 * This file is part of Kleiderkammer.
 *
 * Kleiderkammer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation version 3 of the License.
 *
 * Kleiderkammer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Kleiderkammer.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Diese Datei ist Teil von Kleiderkammer.
 *
 * Kleiderkammer ist Freie Software: Sie können es unter den Bedingungen
 * der GNU Affero General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz, weiterverbreiten und/oder modifizieren.
 *
 * Kleiderkammer wird in der Hoffnung, dass es nützlich sein wird, aber
 * OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
 * Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN
 * ZWECK.Siehe die GNU Affero General Public License für weitere Details.
 *
 * Sie sollten eine Kopie der GNU Affero General Public License zusammen mit
 * diesem Programm erhalten haben. Wenn nicht, siehe
 * <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//#if __linux__||__unix__
    Ort=QDir::homePath();//Setzt den Pfard der Anwendung unter Linux
    Ort=Ort.append("/.config");
/*#elif __WIN32__||_MSC_VER
    PWSTR *localAppData=new PWSTR[256];
    _GUID Local;//Ersetzrt die _GUID FOLDERID_LocalAppData ist nicht schön aber läuft.
    Local.Data1=0xF1B32785;
    Local.Data2=0x6FBA;
    Local.Data3=0x4FCF;
    Local.Data4[0]=0x9D;
    Local.Data4[1]=0x55;
    Local.Data4[2]=0x7B;
    Local.Data4[3]=0x8E;
    Local.Data4[4]=0x7F;
    Local.Data4[5]=0x15;
    Local.Data4[6]=0x70;
    Local.Data4[7]=0x91;
    SHGetKnownFolderPath(Local,0,NULL,localAppData);
    Ort=QString::fromWCharArray(*localAppData);//setzt den Pfard der Anwendung unter Windows
    delete localAppData;

#endif*/
    Ort.append("/Kleiderkammer/");
    {
        QDir Appdir(Ort);
        if(!Appdir.exists())
        {
            Appdir.mkpath(Ort);
        }
    }
    QString Ort=this->Ort;
    Ort.append("Daten.sqlite");
    std::clog <<Ort.toStdString()<<std::endl;
    Daten=new SQLiteQuelle(Ort);//Stellt die verbindung zur Datenbank her.
    Typen=new KleidungsTypenVerwaltung(Daten,this);//Erzeugt die Modelle zur Anzeige der Daten.
    Gruppen=new Gruppenverwaltung(Daten,this);
    KleiderInfoSuchen=new KleiderSuche(Daten,this);

    // Einsetzen der ausgelagreten Tabs
    PersonenTab=new WidgetPersonenTab(Daten,0);
    ui->tabWidget->addTab(PersonenTab,"Jugendliche");

    KleidungTab=new WidgetKleidungTab(Daten,0);
    ui->tabWidget->addTab(KleidungTab,QString::fromUtf8("Kleidungsstücke"));

    EinkleidenTab=new WidgetEinkleidenTab(Daten,0);
    EinkleidenTab->setEnabled(false);
    ui->tabWidget->addTab(EinkleidenTab,"Einkleiden");

    BerichtTab=new WidgetBerichtTab(Daten,Ort,0);
    ui->tabWidget->addTab(BerichtTab,"Bericht");

    ComboboxFuellen();//Inizalisiert alle Comboboxen.

    //Stellt alle Eventverbindungen her.
    connect(ui->actionKleidungstypen_verwalten,SIGNAL(triggered()),Typen,SLOT(exec()));
    connect(Typen,SIGNAL(datenGeaendert()),this,SLOT(ComboboxFuellen()));
    connect(ui->actionGruppen_Verwalten,SIGNAL(triggered()),Gruppen,SLOT(exec()));
    connect(Gruppen,SIGNAL(datenGeaendert()),this,SLOT(ComboboxFuellen()));
    connect(KleiderInfoSuchen,SIGNAL(PersonGewaehlt(int)),this,SLOT(ZeigePersonKleider(int)));
    connect(PersonenTab,SIGNAL(showPerson(int)),this,SLOT(ZeigePersonKleider(int)));
    // Verbinden der Mainmenü einträgen
    connect(ui->actionBeenden,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionKleidungsst_ck,SIGNAL(triggered()),KleiderInfoSuchen,SLOT(exec()));
    connect(ui->action_ber,SIGNAL(triggered()),this,SLOT(ZeigeInfo()));
    connect(ui->action_ber_QT,SIGNAL(triggered()),this,SLOT(ZeigeQTInfo()));
    // Verbinden der Tabs untereinander
    connect(EinkleidenTab,SIGNAL(KleidungVerschoben()),KleidungTab,SLOT(refrashTable()));
    connect(PersonenTab,SIGNAL(PersonGewaehlt(int)),EinkleidenTab,SLOT(showPerson(int)));
#ifdef NOPRINT
    ui->pushButton_BeDr->close();
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
    delete Daten;
    delete BerichtTab;
    delete EinkleidenTab;
    QString DBAktuell,DBBack;
    DBAktuell=Ort;
    DBAktuell.append("Daten.sqlite");
    DBBack=Ort;
    DBBack.append("Backup/");
    QDir BackDir(DBBack);
    if (!BackDir.exists())
    {
        BackDir.mkpath(DBBack);
    }
    QStringList Datein=BackDir.entryList(QDir::Files,QDir::Name);
    for (int i=0;i<Datein.length()-9;++i)
    {
        QString BackAlt=DBBack;
        BackAlt.append(Datein[i]);
        QFile Alt(BackAlt);
        Alt.remove();
    }
    DBBack.append(QDateTime::currentDateTime().toString("yyMMddhhmmss")).append(".db");
    QFile::copy(DBAktuell,DBBack);
}

void MainWindow::ComboboxFuellen()
{
    BerichtTab->DatenGeaendert();
    EinkleidenTab->DatenGeaendert();
    KleidungTab->DatenGeaendert();
    PersonenTab->DatenGeaendert();
}

void MainWindow::ZeigeInfo()
{
    QString UberText;
    UberText=QString::fromUtf8("<html><head></head><body><h1>Kleiderkammer</h1><p>Version: %1</p><p>Diese Anwendung dient der Verwaltung einer Kleiderkammer.</p><p>Kleiderkammer ist Freie Software: Sie können es unter den Bedingungen der GNU Affero General Public License, wie von der Free Software Foundation,Version 3 der Lizenz, weiterverbreiten und/oder modifizieren.</p><p>Kleiderkammer wird in der Hoffnung, dass es nützlich sein wird, aber OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK. Siehe die GNU Affero General Public License für weitere Details.</p><p>Sie sollten eine Kopie der GNU Affero General Public License zusammen mit diesem Programm erhalten haben. Wenn nicht, siehe <a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>.</p></bod></html>");
    UberText=UberText.arg(VER_NUMBER_STRING);
    QMessageBox::about(this,QString::fromUtf8("Über Kleiderkammer"),UberText);
}

void MainWindow::ZeigePersonKleider(int ID)
{
    ui->tabWidget->setCurrentIndex(2);
    EinkleidenTab->showPerson(ID);
}

void MainWindow::ZeigeQTInfo()
{
    QMessageBox::aboutQt(this);
}
