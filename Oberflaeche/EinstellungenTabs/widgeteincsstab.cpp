#include "widgeteincsstab.h"
#include "ui_widgeteincsstab.h"

WidgetEinCSSTab::WidgetEinCSSTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetEinCSSTab)
{
    ui->setupUi(this);
}

WidgetEinCSSTab::~WidgetEinCSSTab()
{
    delete ui;
}
