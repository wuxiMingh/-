#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTcpSocket>
#include <QWidget>
#include "customdelegete.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setStyleSheet("border-image: url(:/QQ20240826-202147.png);border-radius: 15px; ");
    QPixmap pixmap(":/祁同伟01.bmp");
    ui->label->setPixmap(pixmap);
    ui->label->setScaledContents(true);
    ui->label->setStyleSheet("background-image: url(:/QQ20240826-193102.png);border:1px solid black;border-radius: 60px;");
    QFile file(":/style.qss");//构建一个QFILE对象
    file.open(QIODevice::ReadOnly);//打开构造函数中指定的文件

    QTextStream in(&file);//创建一个文本流，参数为打开的文件
//    in.setCodec("gbk");//设置读取编码为GBK的文件
    //如果是UTF8的文件则需要设置为
    in.setCodec("UTF-8");//设置读取编码为GBK的文件
    QString qs = in.readAll();//读取所有的文件内容
    file.close();

    this->setStyleSheet(qs);
    file.close();
    this->setWindowFlag(Qt::FramelessWindowHint);//把主窗口边框去掉
    this->setAttribute(Qt::WA_TranslucentBackground);//把窗口设置为透明
    //网络链接部分，响应服务器
    clientSocket =new QTcpSocket(this);
    clientSocket->connectToHost("172.24.65.243",9000);
    connect(clientSocket,&QTcpSocket::connected,this,&MainWindow::onConnetcted);
    connect(clientSocket,&QTcpSocket::readyRead,this,&MainWindow::onRead);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_2_clicked()//登录按钮的槽函数
{
    QMSG msg;
    msg.type=2;
    QString str = ui->LineEdit->text();
    std::string str1 = str.toStdString();
    const char * p = str1.c_str();
    QString str2 = ui->LineEdit_2->text();
    std::string str3 = str2.toStdString();
    const char *q = str3.c_str();
    register_node node(p,q);
    memcpy(&msg.data,&node,sizeof(node));
    clientSocket->write((char *)&msg,sizeof(msg));
}

void MainWindow::onConnetcted()
{
qDebug()<<"链接成功"<<endl;
}

void MainWindow::onRead()//对服务器消息的响应
{
     QMSG msg;
     clientSocket->read((char *)&msg,sizeof(msg));
     qDebug()<<msg.type<<endl;
     if(msg.type==22)
     {
       this->close();
       weather =new WX;
       weather->show();
     }
}

void MainWindow::on_pushButton_3_clicked()//注册
{
    QMSG msg;
    msg.type=1;
    QString str = ui->LineEdit->text();
    std::string str1 = str.toStdString();
    const char * p = str1.c_str();
    QString str2 = ui->LineEdit_2->text();
    std::string str3 = str2.toStdString();
    const char *q = str3.c_str();
    register_node node(p,q);
    memcpy(&msg.data,&node,sizeof(node));
    clientSocket->write((char *)&msg,sizeof(msg));
}

//窗口拖动实现
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = true;
        //获得鼠标的初始位置
        mouseStartPoint = event->globalPos();
        //mouseStartPoint = event->pos();
        //获得窗口的初始位置
        windowTopLeftPoint = this->frameGeometry().topLeft();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bDrag)
    {
        //获得鼠标移动的距离
        QPoint distance = event->globalPos() - mouseStartPoint;
        //QPoint distance = event->pos() - mouseStartPoint;
        //改变窗口的位置
        this->move(windowTopLeftPoint + distance);
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}

void MainWindow::on_pushButton_4_clicked()//最小化窗口
{
    showMinimized();
}
