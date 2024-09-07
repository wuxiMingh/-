#ifndef WX_H
#define WX_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QMouseEvent>
#include <QPoint>
#include <QTime>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>
#include <QSequentialAnimationGroup>
#include <QPushButton>
#include <QStandardItemModel>
namespace Ui {
class WX;
}

class WX : public QWidget
{
    Q_OBJECT

public:
    explicit WX(QWidget *parent = nullptr);
    ~WX();

    QLabel * label_temp;
    QLabel * label_curr_weather;
    QPushButton * btn_fold;
    QNetworkAccessManager *manager;
    void parseJson(QByteArray jsonByte);
    void label_type_image(QString type,QLabel *label_image);
    void Weather_shezhi();
    void up();
    void down();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    private slots:
   void replyFinished(QNetworkReply *reply);
   void on_pushButton_clicked();
   void slot_up();//折叠按钮槽函数

private:
    Ui::WX *ui;
    QPoint m_mousePoint;
    QPoint movePoint;
    bool mousePress;
    QStandardItemModel *pModel;
    QList<QLabel*> labelList;
    QString api_weather;
};

#endif // WX_H
