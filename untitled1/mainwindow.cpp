#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 创建服务器对象

    server = new QTcpServer(this);

     // 设置端口号

    server->listen(QHostAddress::Any, 9000);

    // 连接信号
    connect(server, &QTcpServer::newConnection, this, &MainWindow::acceptConnect);
    //创建数据库对象
    sqliteOP =new SqliteOperator;
    sqliteOP->openDb();
//创建数据库
//    sqliteOP->normalExec("create table user(name varchar(128),password varchar(128),sex int)");
//    sqliteOP->closeDb();
}

MainWindow::~MainWindow()
{
    sqliteOP->closeDb();//界面结束关闭数据库
    delete ui;
}

void MainWindow::acceptConnect()
{
    //返回连接的套接字对象
   clientSocket = server->nextPendingConnection();
   connect(clientSocket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);//readyRead 信号有消息可读
   connect(clientSocket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);//退出连接
}


 // 槽函数

void MainWindow::onReadyRead() {//接收到消息后的处理
    if(clientSocket)
    {
        QMSG msg;
        int len=clientSocket->read((char*)&msg,sizeof(msg));
        msg.type=(short)msg.type;
        if(msg.type==1)//注册类型
        {
            register_node *rn=(register_node *)msg.data;
            QString sql=QString("insert into user values('%1','%2')").arg(rn->name).arg(rn->password);
            sqliteOP->normalExec(sql);//执行sql语句
        }
        else if(msg.type==2)//登录
        {
            //这个是用注册结构体发来的登录信息
            register_node * rn = (register_node*)msg.data;
            qDebug()<<rn->name<<"  "<<rn->password<<endl;
            QSqlQuery sqlQuery;//这个结构体用来存放查询的结果
            sqliteOP->normalQuery(sqlQuery, QString("select * from user where name='%1' and password='%2'").arg(rn->name).arg(rn->password));
            if(sqlQuery.next())
            {
                msg.type=22;
                clientSocket->write((char *)&msg,sizeof(msg));
            }
        }
    }
}



void MainWindow::onDisconnected() {

     if (clientSocket) {

        qDebug()<<"客户端退出了"<<endl;

     }

}
