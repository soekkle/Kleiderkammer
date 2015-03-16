#ifndef KLEIDUNGSGROESSENTABLEVIEW_H
#define KLEIDUNGSGROESSENTABLEVIEW_H

#include <QAbstractTableModel>
#include <datenquelle.h>
/*!
 * \brief Die KleidungsGroessenTableView Klasse bereitet die Daten zum anzeigenin der Größen einer Tablewiev auf.
 * Und bietet die möglichkeit den Rang nach dem die Größen sortiert werden anzupassen.
 * \author Sören Krecker
 * \version 0.1
 */
class KleidungsGroessenTableView : public QAbstractTableModel
{
public:
    KleidungsGroessenTableView(DatenQuelle* Daten, QObject *parent = 0);
    int columnCount(const QModelIndex &) const;
    void clear();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &) const;
    //bool setData(const QModelIndex &index, const QVariant &value, int role=Qt::EditRole);
    void setKleidungsTyp(int Typ);

public slots:
    void update();

private:
    //! Zeiger Auf die Datenquelle aus der Die Daten Entnommen Werden.
    DatenQuelle *Daten;
    //! Zeiger Auf die Zwieschengespeicherten ergebnisse die Angezeigt werden.
    GroessenTabelle *Groessen;
    //! Speicher für den Kleidungstyp für den die Größe angezeigt werden soll.
    int Typ;
};

#endif // KLEIDUNGSGROESSENTABLEVIEW_H
