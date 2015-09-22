#include "kleidungstableview.h"

KleidungsTableview::KleidungsTableview(DatenQuelle *Daten, int Modus, QObject *parent) :
    QAbstractTableModel(parent)
{
    this->Daten=Daten;
    this->Modus=Modus;
    FilterTyp=0;
    FilterGroesse=-1;
    FilterPerson=0;
    Kleidung=Daten->getKleiderinKammer(0,0);
}

int KleidungsTableview::columnCount(const QModelIndex &) const
{
    return 6;
}

QVariant KleidungsTableview::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.row()<Kleidung->Anzahl)
            switch(index.column())
            {
            case 0:
                return QVariant(Kleidung->Nummer[index.row()]);break;
            case 1:
                return QVariant(Kleidung->Typ[index.row()]);break;
            case 2:
            {
                if (Kleidung->Groesseunbekannt[index.row()])
                    return QVariant("Unbekannt");
                else
                    return QVariant(Kleidung->Groesse[index.row()]);
                break;
            }
            case 3:
            {
                int alter=QDate::currentDate().year() - Kleidung->Anschaffung[index.row()].date().year();
                return QVariant(alter);break;
            }
            case 4:
                return QVariant(Kleidung->AnzahlAusleihen[index.row()]);break;
            case 5:
                return QVariant(Kleidung->Bemerkung[index.row()]);break;
            }
    }
    if (role == Qt::EditRole)
        if (index.row()<Kleidung->Anzahl)
        {
            if (index.column()==5)
            {
                return QVariant(Kleidung->Bemerkung[index.row()]);
            }
        }
    return QVariant();
}

Qt::ItemFlags KleidungsTableview::flags(const QModelIndex &index) const
{
    if (index.column()==5)
        return Qt::ItemIsEditable|Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (index.column()==2)
        if (Kleidung->Groesseunbekannt[index.row()])
        return Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable;
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

int KleidungsTableview::getKleidungsId(int row)
{
    if (row<Kleidung->Anzahl)
        return Kleidung->ID[row];
    return -1;
}

QVariant KleidungsTableview::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role==Qt::DisplayRole)
    {
        if(orientation==Qt::Horizontal)
        {
            switch(section)
            {
                case 0:
                    return QString("Nummer");
                case 1:
                    return QString("Typ");
                case 2:
                    return QString().fromUtf8("Größe");
                case 3:
                    return QString("Alter");
                case 4:
                    return QString("Ausleihen");
                case 5:
                   return QString("Bemerkung");

            }
        }
    }
    return QAbstractTableModel::headerData(section,orientation,role);
}

int KleidungsTableview::rowCount(const QModelIndex &) const
{
    return Kleidung->Anzahl;
}

bool KleidungsTableview::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role==Qt::EditRole)
    {
        int row=index.row();
        switch (index.column())
        {
            case 5:
            {
                int ID=Kleidung->ID[row];
                QString Text=value.toString();
                Daten->setKleidungsKommentar(ID,Text);
                Kleidung->Bemerkung[index.row()]=Text;
                dataChanged(index,index);
            }break;
            case 2:
            {if (Kleidung->Groesseunbekannt[row])
            {
                QString Eingabe=value.toString();
                int TID=Daten->getKleidungsTypID(Kleidung->Typ[row]);
                int GID=Daten->getGroessenID(Eingabe,TID);
                if (GID==-1)
                    return false;
                Daten->setKleidungsGroesse(Kleidung->ID[row],GID);
                Kleidung->Groesse[index.row()]=Eingabe;
                Kleidung->Groesseunbekannt[index.row()]=false;
                dataChanged(index,index);
            }
            }break;
        }
    }
    return true ;
}

void KleidungsTableview::setFilterGroesse(int Groesse)
{
    if (Groesse>0)
        FilterGroesse=Groesse;
    else
        FilterGroesse=-1;
    update();
}

void KleidungsTableview::setFilterPerson(int Person)
{
    if (Person>=0)
        FilterPerson=Person;
    update();
}

void KleidungsTableview::setFilterTyp(int Typ)
{
    if (Typ>=0)
        FilterTyp=Typ;
    update();
}

void KleidungsTableview::update()
{
    int anz_Alt=0;
    if (Kleidung!=NULL)
    {
        anz_Alt=Kleidung->Anzahl-1;
        beginRemoveRows(QModelIndex(),0,anz_Alt);
        delete Kleidung;
        endRemoveRows();
    }
    if (Modus==0)
        Kleidung=Daten->getKleiderinKammer(FilterTyp,FilterGroesse);
    if (Modus==1)
        Kleidung=Daten->getKleidervonPerson(FilterPerson,FilterTyp);
    beginInsertRows(QModelIndex(),0,Kleidung->Anzahl-1);
    endInsertRows();
}
