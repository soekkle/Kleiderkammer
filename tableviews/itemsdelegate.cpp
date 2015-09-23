/*
 * Copyright (C) 2015 Sören Krecker
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

#include "itemsdelegate.h"

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
