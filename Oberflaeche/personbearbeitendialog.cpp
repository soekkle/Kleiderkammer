/*
 * Copyright (C) 2016 Sören Krecker
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
#include "personbearbeitendialog.h"
#include "ui_personbearbeitendialog.h"

PersonBearbeitenDialog::PersonBearbeitenDialog(DatenQuelle *Daten, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonBearbeitenDialog)
{
    ui->setupUi(this);
    this->Daten=Daten;
    ID=-1;
    connect(ui->buttonBox,SIGNAL(clicked(QAbstractButton*)),this,SLOT(Buttons(QAbstractButton*)));
}

PersonBearbeitenDialog::~PersonBearbeitenDialog()
{
    delete ui;
}

bool PersonBearbeitenDialog::bearbeiten(int ID)
{
    this->ID=ID;
    QString Gruppe;
    if (Daten->getPersonenInfo(ID,&VName,&NName,&Gruppe,&GruppenID))
    {
        ui->EditVName->setText(VName);
        ui->EditNName->setText(NName);
        {
            JugendFeuerwehrTabelle *JfDaten=Daten->getJugendfeuerwehr();
            ui->comboBox->clear();
            for (int i=0;i<JfDaten->Anzahl;++i)
            {
                ui->comboBox->addItem(JfDaten->Name[i],QVariant(JfDaten->ID[i]));
                if (JfDaten->ID[i]==GruppenID)
                {
                    ui->comboBox->setCurrentIndex(i);
                }
            }
            delete JfDaten;
        }
        DatenGeaendert=false;
        exec();
    }
    return false;
}

void PersonBearbeitenDialog::Buttons(QAbstractButton *button)
{
    QFlag standardButton = ui->buttonBox->standardButton(button);
    if (standardButton==QDialogButtonBox::Reset)
        DatenZuruecksetzen();
    else if (standardButton==QDialogButtonBox::Discard)
        this->close();
}

void PersonBearbeitenDialog::DatenZuruecksetzen()
{
    ui->EditVName->setText(VName);
    ui->EditNName->setText(NName);
    for (int i=0;i<ui->comboBox->count();++i)
    {
        if (ui->comboBox->itemData(i)==GruppenID)
            ui->comboBox->setCurrentIndex(i);
    }
    DatenGeaendert=false;
}
