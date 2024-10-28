#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>

TcpClient::TcpClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    loadConfig();

//    connect(&m_tcpSocket,SIGNAL(connect())
//            ,this,SLOT(showConnect()));

    connect(&m_tcpSocket,&QTcpSocket::connected
            ,this,&TcpClient::showConnect);

    m_tcpSocket.connectToHost(QHostAddress(m_strIP),m_usPort);
}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::loadConfig()
{
    QFile file(":/client.config");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray baData = file.readAll();
        QString strData = baData.toStdString().c_str();
        file.close();

        strData.replace("\r\n"," ");

        QStringList strList = strData.split(" ");

        m_strIP = strList.at(0);
        m_usPort = strList.at(1).toUShort();
        qDebug()<<"ip:"<<m_strIP<<" port:"<<m_usPort;
    }
    else
    {
        QMessageBox::critical(this,"openconfig","open config failed");
    }
}

void TcpClient::showConnect()
{
    QMessageBox::information(this,"连接服务器","连接服务器成功");
}


void TcpClient::on_send_pb_clicked()
{
    QString strMsg = ui->lineEdit->text();
    if(!strMsg.isEmpty())
    {
        PDU *pdu =mkPDU(strMsg.size());
        pdu->uiMsgType=8888;
        memcpy(pdu->caMsg,strMsg.toStdString().c_str(),strMsg.size());
        m_tcpSocket.write((char*)pdu,pdu->uiPDULen);
        free(pdu);
        pdu=NULL;
    }
    else
    {
        QMessageBox::warning(this,"信息发送","发送的信息不能为空");
    }
}
