#ifndef COMBOBOXGROESSENDELEGATE_H
#define COMBOBOXGROESSENDELEGATE_H

#include <QItemDelegate>
#include <QComboBox>

#include "datenquelle.h"

class ComboboxGroessenDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ComboboxGroessenDelegate(DatenQuelle *Daten, QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    DatenQuelle *Daten;
signals:

public slots:

};

#endif // COMBOBOXGROESSENDELEGATE_H
