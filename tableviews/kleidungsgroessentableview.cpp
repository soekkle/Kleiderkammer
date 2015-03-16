#include "kleidungsgroessentableview.h"

/*!
 * \brief KleidungsGroessenTableView::KleidungstypenTableView
 * \param Daten Zeiger auf ein Objekt der Klasse DatenQuelle
 * \param parent Zeiger auf den Patent in Qt.
 */
KleidungsGroessenTableView::KleidungsGroessenTableView(DatenQuelle* Daten, QObject *parent):
    QAbstractTableModel(parent)
{
    this->Daten=Daten;
}

void KleidungsGroessenTableView::clear()
{
    delete Groessen;
    Groessen=NULL;
    Typ=-1;
}

int KleidungsGroessenTableView::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant KleidungsGroessenTableView::data(const QModelIndex &index, int role) const
{
    if (Groessen==NULL)
        return QVariant();
    if (role == Qt::DisplayRole)
    {
        if (index.row()<Groessen->Anzahl)
            switch (index.column()) {
            case 0:
                return QVariant(Groessen->Namen[index.row()]);
                break;
            case 1:
                return QVariant(Groessen->Rang[index.row()]);
                break;
            default:
                return QVariant();
                break;
            }
    }
    return QVariant();
}

Qt::ItemFlags KleidungsGroessenTableView::flags(const QModelIndex &index) const
{
    if (index.column()==1)
        return Qt::ItemIsEditable|Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}

QVariant KleidungsGroessenTableView::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role==Qt::DisplayRole)
    {
        if(orientation==Qt::Horizontal)
        {
            switch(section)
            {
                case 0:
                    return QString("Name");
                case 1:
                    return QString("Rang");
            }
        }
    }
    return QAbstractTableModel::headerData(section,orientation,role);
}

int KleidungsGroessenTableView::rowCount(const QModelIndex &) const
{
    if (Groessen==NULL)
        return 0;
    return Groessen->Anzahl;
}

bool KleidungsGroessenTableView::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.column()!=1)
        return false;
    if (role==Qt::EditRole)
    {
        int Rang=value.toInt();
        Daten->setRangGroesse(Groessen->IDs[index.row()],Rang);
    }
    update();
    return true;
}

void KleidungsGroessenTableView::setKleidungsTyp(int Typ)
{
    if (Typ<0)
    {
        delete Groessen;
        Groessen=NULL;
    }
    this->Typ=Typ;
    update();
}

void KleidungsGroessenTableView::update()
{
    if (Groessen!=NULL)
    {
        int anz_Alt=Groessen->Anzahl-1;
        beginRemoveRows(QModelIndex(),0,anz_Alt);
        delete Groessen;
        endRemoveRows();
    }
    if (Typ<0)
        return;
    int anz=1;
    if (Typ==0)
        anz=0;
    Groessen=Daten->getGroessen(&Typ,anz);
    beginInsertRows(QModelIndex(),0,Groessen->Anzahl-1);
    endInsertRows();
}
