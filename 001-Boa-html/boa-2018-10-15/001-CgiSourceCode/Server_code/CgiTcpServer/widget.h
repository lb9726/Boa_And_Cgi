#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "wificommunication.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void init();
    void sltGetAllParameter();

private:
    Ui::Widget *ui;
    WifiCommunication          *mWiFi;
};

#endif // WIDGET_H
