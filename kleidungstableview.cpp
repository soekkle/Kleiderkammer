#include "kleidungstableview.h"

KleidungsTableview::KleidungsTableview(DatenQuelle *Daten, int Modus, QObject *parent) :
    QAbstractTableModel(parent)
{
    this->Daten=Daten;
    this->Modus=Modus;
    FilterTyp=0;
    FilterGroesse=0;
    FilterPerson=0;
    Kleidung=Daten->getKleiderinKammer(0,0);
}

int KleidungsTableview::columnCount(const QModelIndex &parent) const
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
                return QVariant(Kleidung->Groesse[index.row()]);break;
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
    return QVariant();
}

Qt::ItemFlags KleidungsTableview::flags(const QModelIndex &index) const
{
    if (index.column()==5)
        return Qt::ItemIsEditable|Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
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
                    return QString().fromLocal8Bit("Größe");
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

int KleidungsTableview::rowCount(const QModelIndex &parent) const
{
    return Kleidung->Anzahl-1;
}

bool KleidungsTableview::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role==Qt::EditRole)
    {
        int ID=Kleidung->ID[index.row()];
        QString Text=value.toString();
        Daten->setKleidungsKommentar(ID,Text);
        Kleidung->Bemerkung[index.row()]=Text;
        dataChanged(index,index);
    }
    return true ;
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
        delete Kleidung;
    }
    if (Modus==0)
        Kleidung=Daten->getKleiderinKammer(FilterTyp,FilterGroesse);
    if (Modus==1)
        Kleidung=Daten->getKleidervonPerson(FilterPerson,FilterTyp);
    int anz=Kleidung->Anzahl-1;
    qDebug()<<anz_Alt<<anz;
    if (anz_Alt>anz)
    {
        beginRemoveRows(QModelIndex(),anz,anz_Alt);
        endRemoveRows();
    }
    else if (anz>anz_Alt)
    {
        beginInsertRows(QModelIndex(),anz_Alt+1,anz);
        endInsertRows();
    }
}
