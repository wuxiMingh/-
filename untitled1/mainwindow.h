#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include "SqliteOperator.h"
#include "comment.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow(); 
private slots:
    void acceptConnect();
    void  onReadyRead();
     void onDisconnected();
private:
    Ui::MainWindow *ui;
    QTcpServer *server;
    QTcpSocket *clientSocket;
    SqliteOperator *sqliteOP;
};
#endif // MAINWINDOW_H
