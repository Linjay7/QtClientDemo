#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    
    //连接服务器状态：初始化为false
    m_state = false;
    //连接服务器成功时，发送
    connect(&m_socket, &QTcpSocket::readyRead, this, &Dialog::OnReadReady);
    connect(&m_socket, &QTcpSocket::connected, this, &Dialog::OnConnected);
    connect(&m_socket, &QTcpSocket::errorOccurred, this, &Dialog::OnErrorOccurred);
    connect(&m_socket, &QTcpSocket::disconnected, this, &Dialog::OnDisconnected);
}

Dialog::~Dialog()
{
    delete ui;
}

//发送消息响应函数
void Dialog::on_btn_sendMessage_clicked()
{
    //获取消息内容
    QByteArray content = ui->edit_message->text().toUtf8();
    QByteArray message = m_name + "：" + content;
    //发送至后清空消息框内容
    ui->edit_message->clear();
    //发送消息
    m_socket.write(message);
}

//连接服务器响应函数
void Dialog::on_btn_connectServer_clicked()
{
    //断开服务器连接
    if (m_state) {
        //发送我已离线消息给服务器
        QByteArray message = m_name + "：已退出聊天室";
        m_socket.write(message);
        //断开服务器连接
        m_socket.disconnectFromHost();
    }
    //连接服务器
    else {
        //获取服务器ip、端口
        m_ip.setAddress(ui->edit_ip->text());
        m_port = ui->edit_port->text().toShort();
        //连接服务器
        m_socket.connectToHost(m_ip, m_port);
    }
}
//连接服务器成功时响应函数
void Dialog::OnConnected()
{
    //连接成功就发送一条消息给服务器
    //获取昵称
    m_name = ui->edit_name->text().toUtf8();
    QByteArray message = m_name + "：已进入聊天室";
    m_socket.write(message);
    
    //状态修改成已连接
    m_state = true;
    //连接成功之后就不能修改ip、端口、昵称；“连接服务器”按钮改成“断开服务器连接”；发送按钮改成可编辑
    ui->edit_ip->setEnabled(false);
    ui->edit_port->setEnabled(false);
    ui->edit_name->setEnabled(false);
    ui->btn_sendMessage->setEnabled(true);
    ui->btn_connectServer->setText("断开服务器连接");
}
//访问服务器发生异常响应函数
void Dialog::OnErrorOccurred()
{
    //连接失败则弹窗提示
    QMessageBox::critical(this, "错误", m_socket.errorString());
}
//断开服务器连接时响应函数
void Dialog::OnDisconnected()
{
    if (this->isActiveWindow()) {
        //状态修改成已离线
        m_state = false;
        //离线之后可修改ip、端口、昵称；“断开服务器连接”按钮改成“连接服务器”；发送按钮改成不可编辑
        ui->edit_ip->setEnabled(true);
        ui->edit_port->setEnabled(true);
        ui->edit_name->setEnabled(true);
        ui->btn_sendMessage->setEnabled(false);
        ui->btn_connectServer->setText("连接服务器");
    }
}
//收到服务器消息响应函数
void Dialog::OnReadReady()
{
    if (m_socket.bytesAvailable()) {
        auto content = m_socket.readAll();
        ui->listWidget->addItem(content);
    }
}

