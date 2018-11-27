#include "tcpmodule.h"

MyTcpServer::MyTcpServer(QObject *parent, int port)
    : QTcpServer(parent)
    , mHasConnectFlag(false)
{
    listen(QHostAddress::Any, port);
    mMyTcpSocket = nullptr;
    mTmpTcpSocket = nullptr;
    qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__<< "will listening";
}

MyTcpServer::~MyTcpServer()
{
    if (mMyTcpSocket)
    {
        qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__;
        mMyTcpSocket->deleteLater();
        mMyTcpSocket = nullptr;
    }
}

int MyTcpServer::writeMsgToClient(QByteArray msg, int length)
{
    if (mMyTcpSocket && mHasConnectFlag)
    {
        int tmplen = mMyTcpSocket->write(msg.data(), length);
        return tmplen;
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<"mMyTcpSocket or mHasConnectFlag write wrong";
        return -1;
    }
}

void MyTcpServer::slotMyTcpServerDealWithMsg(QByteArray msg, int length)
{
    qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__<<msg;
    emit myTcpServerRecvMsg(msg, length);  // TcpServer 发送信号到主界面用于通知显示
}

void MyTcpServer::slotMyTcpServerDisconnected(int descriptor)
{
    qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__;
    if (mMyTcpSocket)
    {
        if (mMyTcpSocket->socketDescriptor() == descriptor)
        {
            qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__<<"descriptor = "<<descriptor;
            mHasConnectFlag = false;
            disconnect(mMyTcpSocket, 0, 0, 0);
            mMyTcpSocket->close();
            emit myTcpServerDeviceDisConnect();
        }
    }
}

void MyTcpServer::slotMyTcpServerCloseSocket()
{
    if (mMyTcpSocket)
    {
        mHasConnectFlag = false;
        emit myTcpServerDeviceDisConnect();
    }
}

void MyTcpServer::slotCloseTcpSocket()
{
    if (mTmpTcpSocket)
    {
        qDebug()<<__PRETTY_FUNCTION__<<"lines = "<<__LINE__<<"will delete TcpSocket";
        mTmpTcpSocket->deleteLater();
        mTmpTcpSocket = nullptr;
    }
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__<< "new connect mHasConnectFlag = "<< mHasConnectFlag;
    if (!mHasConnectFlag)
    {
        mHasConnectFlag = true;
        mTmpTcpSocket = mMyTcpSocket;
        slotCloseTcpSocket();
        mMyTcpSocket = new MyTcpSocket();
        if (mMyTcpSocket)
        {
            connect(mMyTcpSocket, SIGNAL(myTcpSocketRecvMsg(QByteArray, int)),
                    this, SLOT(slotMyTcpServerDealWithMsg(QByteArray, int)));
            connect(mMyTcpSocket, SIGNAL(myTcpSocketDisconnected(int)),
                    this, SLOT(slotMyTcpServerDisconnected(int)));

            mMyTcpSocket->setSocketDescriptor(socketDescriptor);
        }
    }
    else
    {
        qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__<<" already connect do nothing";
    }
}

/******************TcpSocket 模块 satrt **********************/
MyTcpSocket::MyTcpSocket(QObject *parent)
{
    Q_UNUSED(parent);
    qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__;
    connect(this, SIGNAL(readyRead()), this, SLOT(slotMyTcpSocketDataReceived()));
    connect(this, SIGNAL(disconnected()), this, SLOT(slotMyTcpSocketDisconnected()));
}

MyTcpSocket::~MyTcpSocket()
{
    qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__<<"MyTcpSocket deletelater";
}

void MyTcpSocket::slotMyTcpSocketDataReceived()
{
    int length = bytesAvailable();
    if (length > 0)
    {
        QByteArray recvMsg = read(length);
       // qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__<<"len = "<<length<<recvMsg;
        emit myTcpSocketRecvMsg(recvMsg, length);
    }
//    qDebug()<<__PRETTY_FUNCTION__<<" lines = " << __LINE__;
}

void MyTcpSocket::slotMyTcpSocketDisconnected()
{
    int tmpSocketDesp = this->socketDescriptor();
    qDebug()<<__PRETTY_FUNCTION__<<"tmpSocketDesp = "<< tmpSocketDesp;
    emit myTcpSocketDisconnected(tmpSocketDesp);
}
/******************TcpSocket 模块 end **********************/

