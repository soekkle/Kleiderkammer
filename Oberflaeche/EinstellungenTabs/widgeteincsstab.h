#ifndef WIDGETEINCSSTAB_H
#define WIDGETEINCSSTAB_H

#include <QWidget>

namespace Ui {
class WidgetEinCSSTab;
}

class WidgetEinCSSTab : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetEinCSSTab(QWidget *parent = 0);
    ~WidgetEinCSSTab();

private:
    Ui::WidgetEinCSSTab *ui;
};

#endif // WIDGETEINCSSTAB_H
