#include "spinboxdelegate.h"

SpinBoxDelegate::SpinBoxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
 {
     QSpinBox *editor = new QSpinBox(parent);
     editor->setMinimum(0);
     editor->setMaximum(100);

     return editor;
 }

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int Wert =index.data(Qt::DisplayRole).toInt();
    QSpinBox *Box=static_cast<QSpinBox*>(editor);
    Box->setValue(Wert);
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *Box=static_cast<QSpinBox*>(editor);
    model->setData(index,Box->value(),Qt::EditRole);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const
 {
     editor->setGeometry(option.rect);
 }
