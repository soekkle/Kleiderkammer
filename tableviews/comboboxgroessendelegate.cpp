#include "comboboxgroessendelegate.h"

ComboboxGroessenDelegate::ComboboxGroessenDelegate(DatenQuelle *Daten, QObject *parent) :
    QItemDelegate(parent)
{
    this->Daten=Daten;
}

QWidget *ComboboxGroessenDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    QComboBox *editor = new QComboBox(parent);
    return editor;
}

void ComboboxGroessenDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString Wert =index.data(Qt::DisplayRole).toString();
    QComboBox *Box=static_cast<QComboBox*>(editor);
    QModelIndex IndexTyp=index.child(index.row(),1);
    const QAbstractItemModel *model=index.model();
    QString Typ=model->data(IndexTyp).toString();
    int TypID=Daten->getKleidungsTypID(Typ);
    GroessenTabelle *Groessen=Daten->getGroessen(&TypID,1);
    for (int i=0;i<Groessen->Anzahl;++i)
    {
        Box->addItem(Groessen->Namen[i]);
    }
    delete Groessen;
    Box->setEditText(Wert);
}

void ComboboxGroessenDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *Box=static_cast<QComboBox*>(editor);
    model->setData(index,Box->currentText(),Qt::EditRole);
}

void ComboboxGroessenDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}
