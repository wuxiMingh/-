#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "wx.h"
#include <QMainWindow>
#include <QTcpSocket>
#include "comment.h"
#include <QMouseEvent>
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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void onConnetcted();
    void  onRead();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    Ui::MainWindow *ui;
    QTcpSocket *clientSocket;
    bool   m_bDrag;
    QPoint  mouseStartPoint;
    QPoint  windowTopLeftPoint;
    WX *weather;//天气窗口
};
#endif // MAINWINDOW_H
