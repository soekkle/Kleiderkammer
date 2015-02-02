#ifndef KLEIDUNGSTABLEVIEW_H
#define KLEIDUNGSTABLEVIEW_H

#include <QAbstractTableModel>
#include "datenquelle.h"
#include <QDebug>
/*!
 * \brief The KleidungsTableview class
 */

class KleidungsTableview : public QAbstractTableModel
{
    Q_OBJECT
public:
    KleidungsTableview(DatenQuelle* Daten,int Modus,QObject *parent = 0);
    int getKleidungsId(int row);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    void setFilterTyp(int Typ);
    void setFilterPerson(int Person);
    void setFilterGroesse(int Groesse);
    void update();
signals:
    
public slots:
    
private:
    KleiderTabelle* Kleidung;
    int FilterTyp, FilterGroesse, FilterPerson;
    int Modus;
    DatenQuelle* Daten;
};

#endif // KLEIDUNGSTABLEVIEW_H
