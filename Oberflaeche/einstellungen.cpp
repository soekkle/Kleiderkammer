#include "einstellungen.h"
#include "ui_einstellungen.h"

Einstellungen::Einstellungen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Einstellungen)
{
    ui->setupUi(this);

    CSSTab=new WidgetEinCSSTab(this);
    ui->tabWidget->addTab(CSSTab,"Bereichte");
}

Einstellungen::~Einstellungen()
{
    delete ui;
}
