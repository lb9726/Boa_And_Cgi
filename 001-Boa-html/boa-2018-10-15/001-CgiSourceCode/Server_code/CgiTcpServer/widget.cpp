#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

using std::nothrow;
//#define TESTLINK(a, b, c) #a###b###c
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    mWiFi = nullptr;
    ui->setupUi(this);
    init();
//    qDebug()<<TESTLINK('1', '3', 3);
}

Widget::~Widget()
{
    delete ui;
    if (mWiFi)
    {
        delete mWiFi;
        mWiFi = nullptr;
    }
}

void Widget::init()
{
    mWiFi = new(nothrow) WifiCommunication();
    connect(mWiFi, &WifiCommunication::sigGetAllParametere, this, &Widget::sltGetAllParameter);
}

void Widget::sltGetAllParameter()
{
    QJsonObject map;
    QJsonObject info;

    info.insert("autotest", false);
    info.insert("liftflrvol", 1);
    info.insert("liftarvvol", 2);
    info.insert("admusicvol", 3);
    info.insert("luminance", 4);
    info.insert("darklevel", 5);
    info.insert("standby", 300000);
    info.insert("rotation", 90);
    info.insert("layout", 1);

    map.insert("info", QJsonValue(info));
    map.insert("type", "sendallparameter");
//    qDebug()<<__PRETTY_FUNCTION__<<"map = "<<map;
    QByteArray allPara;
    if (mWiFi)
    {
        allPara = mWiFi->convertJsonToByteArray(map);
        allPara = mWiFi->generateBuffer(allPara);
        mWiFi->writeMsgToClient(allPara, allPara.length());
    }
    qDebug()<<__PRETTY_FUNCTION__<<"allPara = "<<allPara;
}
