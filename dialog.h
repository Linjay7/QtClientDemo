#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    
private slots:
    //发送消息响应函数
    void on_btn_sendMessage_clicked();
    //连接服务器响应函数
    void on_btn_connectServer_clicked();
    //连接服务器成功时响应函数
    void OnConnected();
    //访问服务器发生异常响应函数
    void OnErrorOccurred();
    //断开服务器连接时响应函数
    void OnDisconnected();
    //收到服务器消息响应函数
    void OnReadReady();
    
private:
    Ui::Dialog *ui;
    
    QTcpSocket m_socket;
    QHostAddress m_ip;
    quint16 m_port;
    QByteArray m_name;
    bool m_state;
};
#endif // DIALOG_H
