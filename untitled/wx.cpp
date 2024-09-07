#include "wx.h"
#include "ui_wx.h"
WX::WX(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WX)
{
    ui->setupUi(this);
    //去掉窗口边框，这个我记不住，需要直接搜手册,其实就是透明，然后在设置背景
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);//此时窗口是透明的
    //设置全局文字颜色
    this->setStyleSheet("QLabel{color:#ffffff;}");
    //颜色画笔
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::white);
    api_weather="101010100";//设置初始天气为北京
    //当前温度文字字样
    label_temp = new QLabel(this);
    QFont ft_temp("Arial",35,70);
    label_temp->setFont(ft_temp);
    label_temp->setStyleSheet("color:#ffffff");
    //天气文字的字体样式
    label_curr_weather = new QLabel(this);
    QFont ft_weather("Arial",20,75);
    label_curr_weather->setFont(ft_weather);
    label_curr_weather->setStyleSheet("color:#ffffff");
    label_curr_weather->setAlignment(Qt::AlignCenter);

    label_temp->move(170,120);//设置初始位置
    label_curr_weather->move(210,220);
    //关闭按钮的样式设计
    QPushButton* btn_close = new QPushButton(this);
    btn_close->setFixedSize(50,50);
    btn_close->setText("关闭");
    btn_close->setStyleSheet("QPushButton{font:Arial;color:#ffffff;font-size:20px;background-color:rgba(0,0,0,0);border:none}"
                             "QPushButton:hover{font-size:22px;}");
    connect(btn_close,SIGNAL(clicked(bool)),this,SLOT(close()));//点击关闭按钮后调用槽函数

    //折叠按钮的样式设计
    btn_fold = new QPushButton(this);
    btn_fold->setFixedSize(50,50);
    btn_fold->setText("折叠");
    btn_fold->setStyleSheet("QPushButton{font:Arial;color:#ffffff;font-size:20px;background-color:rgba(0,0,0,0);border:none}"
                            "QPushButton:hover{font-size:22px;}");
    connect(btn_fold,SIGNAL(clicked(bool)),this,SLOT(slot_up()));//折叠展开槽函数调用
    //确定按钮
    ui->pushButton->setText("确定");
    ui->pushButton->setStyleSheet("QPushButton{font:Arial;color:#ffffff;font-size:20px;background-color:rgba(0,0,0,0);border:none}"
                                  "QPushButton:hover{font-size:22px;}");
   // connect(ui->pushButton, &QPushButton::clicked, this, &WX::on_pushButton_clicked);
    //关闭按钮和折叠按钮的初始位置
    btn_fold->move(350,0);
    btn_close->move(400,0);

    /*获取当前日期时间*/
    QDateTime dt;
    QDate date;
    dt.setDate(date.currentDate());
    QString currentDate = dt.toString("ddd");
    ui->label_curr_date->setStyleSheet("color:#ffffff");
    QFont ft_date("Arial",15,60);
    ui->label_curr_date->setFont(ft_date);
    ui->label_curr_date->setText(currentDate);
    //将label写入到Qlist中，方便后序遍历等操作
    labelList.append(ui->label);
    for (int i = 2; i <= 20; ++i) {
        QString labelName = QString("label_%1").arg(i);
        QLabel *label_push = this->findChild<QLabel*>(labelName);
        if (label_push) {
            labelList.append(label_push);
        }
    }
    Weather_shezhi();
}

WX::~WX()
{
    delete ui;
}
//重写鼠标事件,实现除去标题栏后任然能实现窗口移动
void WX::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mousePress = true;
    }
    //窗口移动距离
    movePoint = event->globalPos() - pos();
}

void WX::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    mousePress = false;
}

void WX::replyFinished(QNetworkReply *reply)
{
    qDebug()<<"网页响应已完成"<<endl;
    QVariant ret = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug()<<"网页返回状态为"<<ret<<endl;
    if(ret == 200)//200说明页面访问成功
    {
        QByteArray data = reply->readAll();//读取所有的网络数据
        parseJson(data);
    }
}
void WX::parseJson(QByteArray jsonByte)//解析json文件内容
{
    int num=0;//用来标识labellist中的label位置
    QString ymd;
    QString high;
    QString low;
    QString type;//天气类型
    QString datatype;
    QString time;//今天时间
    QString wendu;//今天温度
    QString Path;//背景图片路径
    QString shidu;
    QString quality;//空气质量
    QString fx;//风向
    QString fl;//风力
    QString pm25;
    QString notice;
    QString city;
    QString date;//今日天气
    QJsonParseError json_error;
    QJsonDocument parse_doucment=QJsonDocument::fromJson(jsonByte,&json_error);
    if(json_error.error==QJsonParseError::NoError)//判读解析过程中是否有错误
    {
        if(parse_doucment.isObject())//判断文档是否是一个json对象
        {
            QJsonObject rootObj=parse_doucment.object();//获取文档json对象
            if(rootObj.contains("time"))
            {
                QJsonValue valuetime=rootObj.take("time");
                if(valuetime.isString())
                {
                    time=valuetime.toString();
                }
            }
            if(rootObj.contains("cityInfo"))
            {
                QJsonValue valuecityInfo=rootObj.value("cityInfo");
                if(valuecityInfo.isObject())
                {
                    QJsonObject valuecityInfoObj=valuecityInfo.toObject();
                    if(valuecityInfoObj.contains("city"))
                    {
                        QJsonValue city_value=valuecityInfoObj.take("city");
                        if(city_value.isString())
                        {
                            city=city_value.toString();
                        }
                    }
                }
            }
            if(rootObj.contains("data"))
            {
                QJsonValue valuedata=rootObj.value("data");//获取data的值
                if(valuedata.isObject())//判断data对应的值是否是对象
                {
                    QJsonObject valuedataObj=valuedata.toObject();//把OJsonValue转为JsonObject对象
                    if(valuedataObj.contains("wendu"))
                    {
                        QJsonValue valuewendu=valuedataObj.take("wendu");
                        if(valuewendu.isString())
                        {
                            wendu=valuewendu.toString();
                        }
                    }
                    if(valuedataObj.contains("shidu"))
                    {
                        QJsonValue valueshidu=valuedataObj.take("shidu");
                        if(valueshidu.isString())
                        {
                            shidu=valueshidu.toString();
                        }
                    }
                    if(valuedataObj.contains("pm25"))
                    {
                        QJsonValue valuepm25=valuedataObj.take("pm25");
                        if(valuepm25.isDouble())
                        {
                            double pm25_dou;
                            pm25_dou=valuepm25.toDouble();
                            pm25 = QString::number(pm25_dou, 'f', 0);//将double类转为Qstring
                        }
                    }
                    if(valuedataObj.contains("quality"))
                    {
                        QJsonValue valuequality=valuedataObj.take("quality");
                        if(valuequality.isString())
                        {
                            quality=valuequality.toString();
                        }
                    }
                    if(valuedataObj.contains("forecast"))
                    {
                        QJsonValue valueArray=valuedataObj.value("forecast");
                        if(valueArray.isArray())
                        {
                            QJsonArray array=valueArray.toArray();
                            //                            int size =array.size();获取数组大小
                            for(int i=0;i<6;i++)
                            {
                                QJsonValue value=array.at(i);
                                if(value.isObject())
                                {
                                    QJsonObject object=value.toObject();
                                    if(object.contains("ymd"))
                                    {
                                        QJsonValue ymd_value=object.take("ymd");
                                        if(ymd_value.isString())
                                        {
                                            ymd=ymd_value.toString();
                                        }
                                        if(i==0)
                                        {
                                            date=ymd;
                                        }
                                    }
                                    if(object.contains("high"))
                                    {
                                        QJsonValue high_value=object.take("high");
                                        if(high_value.isString())
                                        {
                                            high=high_value.toString();
                                        }
                                    }
                                    if(object.contains("low"))
                                    {
                                        QJsonValue low_value=object.take("low");
                                        if(low_value.isString())
                                        {
                                            low=low_value.toString();
                                        }
                                    }
                                    if(object.contains("type"))
                                    {
                                        QJsonValue type_value=object.take("type");
                                        if(type_value.isString())
                                        {
                                            type=type_value.toString();
                                        }
                                    }
                                    if(object.contains("fx")&&i==0)
                                    {
                                        QJsonValue fx_value=object.take("fx");
                                        if(fx_value.isString())
                                        {
                                            fx=fx_value.toString();
                                        }
                                    }
                                    if(object.contains("fl")&&i==0)
                                    {
                                        QJsonValue fl_value=object.take("fl");
                                        if(fl_value.isString())
                                        {
                                            fl=fl_value.toString();
                                        }
                                    }
                                    if(object.contains("notice")&&i==0)
                                    {
                                        QJsonValue notice_value=object.take("notice");
                                        if(notice_value.isString())
                                        {
                                            notice=notice_value.toString();
                                        }
                                    }
                                }
                                if(i==0)//根据天气换背景
                                {

                                    if(type.contains("云"))
                                    {
                                        Path=":/weather_back/cloudy.png";
                                    }
                                    else if(type.contains("雨"))
                                    {
                                        Path=":/weather_back/day-rain.png";
                                    }
                                    else if(type.contains("雪"))
                                    {
                                        Path=":/weather_back/day-snow.png";
                                    }
                                    else if(type.contains("阴"))
                                    {
                                        Path=":/weather_back/cloudy.png";
                                    }
                                    else
                                    {
                                        Path=":/weather_back/day-sun.png";
                                    }
                                    ui->frame->setAutoFillBackground(true);
                                    // 获取 Qwidget 的调色板
                                    QPalette palette = ui->frame->palette();
                                    // 获取 Qwidget 的尺寸
                                    QSize frameSize = ui->frame->size();
                                    // 加载图片资源并缩放到 Qwidget 的尺寸，使用平滑缩放方式
                                    QPixmap backgroundPixmap(Path);
                                    backgroundPixmap = backgroundPixmap.scaled(frameSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                                    // 清除之前的背景画刷
                                    palette.setBrush(QPalette::Window, QBrush());
                                    // 设置调色板的窗口背景画刷
                                    palette.setBrush(QPalette::Window, QBrush(backgroundPixmap));
                                    // 应用修改后的调色板到 widget_3
                                  ui->frame->setPalette(palette);
                                }
                                if(time.contains(ymd))//防止把今天天气写入到预测里
                                {
                                    datatype=type;
                                    continue;
                                }
                                high.remove("高温");
                                low.remove("低温");
                                low.remove("℃");
                                QString _wendu=low+"~"+high;
                                ymd.remove("2024-");
                                //用来对未来几天天气的label进行赋值
                                for(int j=0;j<4;j++)
                                {
                                    if(num%4==0)
                                    {
                                        label_type_image(type,labelList[num++]);
                                        continue;
                                    }
                                    if(num%4==1)
                                    {
                                        labelList[num++]->setText(type);
                                        continue;
                                    }
                                    if(num%4==2)
                                    {
                                        labelList[num++]->setText(ymd);
                                        continue;
                                    }
                                    if(num%4==3)
                                    {
                                        labelList[num++]->setText(_wendu);
                                        continue;
                                    }
                                }
                            }
                        }

                    }

                }
            }
        }
    }
    //对label赋值
    label_temp->setText(wendu+"℃");
    label_temp->adjustSize();
    label_curr_weather->setText(datatype);
    label_curr_weather->adjustSize();
    shidu="湿度:"+shidu;
    quality="空气"+quality;
    ui->label_21->setText(fx);
    ui->label_25->setText(fl);
    ui->label_23->setText(pm25);
    ui->label_27->setText(quality);
    ui->label_30->setText(notice);
    ui->label_31->setText(city);
    ui->label_32->setText(date);
}
//根据天气类型来判断后面天气预测的图标
void WX::label_type_image(QString type,QLabel *label_image)
{
    QString label_path;
    if(type.contains("小雨"))
    {
        label_path=":/天气图标/天气-小雨.png";
    }
    else if(type.contains("中雨"))
    {
        label_path=":/天气图标/天气-中雨.png";
    }
    else if(type.contains("大雨"))
    {
        label_path=":/天气图标/天气-大雨.png";
    }
    else if(type.contains("多云"))
    {
        label_path=":/天气图标/天气-多云.png";
    }
    else if(type.contains("阴"))
    {
        label_path=":/天气图标/阴.png";
    }
    else if(type.contains("雪"))
    {
        label_path=":/天气图标/雪.png";
    }
    else if(type.contains("霾"))
    {
        label_path=":/天气图标/雾霾.png";
    }
    else
        label_path=":/天气图标/天气图标_晴.png";
    label_image->setStyleSheet(QString("border-image:url(%1)").arg(label_path));
}
//API的改变
void WX::Weather_shezhi()
{
    //通过API获取Json数据
    manager =new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply *)),this,SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(QString("http://t.weather.sojson.com/api/weather/city/%1").arg(api_weather))));
}

void WX::mouseMoveEvent(QMouseEvent *event)
{
    if(mousePress)
    {
        QPoint movePos = event->globalPos();//窗口初始位置
        move(movePos - movePoint);
    }
}


void WX::on_pushButton_clicked()//切换城市确定按钮槽函数
{
    QString city_line=ui->lineEdit->text();
    qDebug()<<city_line<<endl;
    // 打开文件
    QFile file(":/city.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件:" << file.errorString();
        return ;
    }
    // 使用QTextStream读取文件内容
    QTextStream in(&file);
    in.setCodec("UTF-8");
    while (!in.atEnd()) {
        QString line = in.readLine();
        if(line.contains(city_line))
        {
            QStringList parts = line.split("\t");
            api_weather=parts[0];
            break;
            qDebug()<<api_weather<<endl;
        }
    }
    Weather_shezhi();
    // 关闭文件
    file.close();
}
//折叠，展开函数判断
void WX::slot_up()
{
    static bool status = true;
    if(status)
    {
        up();
    }else
    {
        down();
    }
    status = !status;
}
void WX::up()
{
    btn_fold->setText("展开");
    ui->label_curr_date->hide();
    ui->lineEdit->hide();
    ui->widget->hide();
    ui->label_23->hide();
    ui->label_21->hide();
    ui->label_24->hide();
    ui->label_27->hide();
    ui->label_25->hide();
    ui->label_26->hide();
    ui->label_29->hide();
    ui->label_30->hide();
    ui->pushButton->hide();
    //当前温度文字的label移动动画
    QPropertyAnimation * animation_text_temp_left = new QPropertyAnimation(label_temp,"pos");
    animation_text_temp_left->setStartValue(label_temp->pos());//开始移动的位置
    animation_text_temp_left->setEndValue(QPoint(ui->frame->pos().x() + 30,label_temp->pos().y() - 90));//结束位置
    animation_text_temp_left->setDirection(QAbstractAnimation::Direction::Forward);//设置动画的方向
    animation_text_temp_left->setDuration(500);//仿真时间

    /*当前天气状况文字的移动动画*/
    QPropertyAnimation * animation_text_weather_left = new QPropertyAnimation(label_curr_weather,"pos");
    animation_text_weather_left->setStartValue(label_curr_weather->pos());
    animation_text_weather_left->setEndValue(QPoint(ui->frame->pos().x() + 70,label_curr_weather->pos().y() - 120));
    animation_text_weather_left->setDirection(QAbstractAnimation::Direction::Forward);
    animation_text_weather_left->setDuration(500);
    /*未来天气背景框移动动画*/
    QPropertyAnimation * animation_pic_up = new QPropertyAnimation(ui->frame,"pos");
    animation_pic_up->setStartValue(QPoint(ui->frame->pos().x(),ui->frame->pos().y()));
    animation_pic_up->setEndValue(QPoint(ui->frame->pos().x(),ui->frame->pos().y() - 400));
    animation_pic_up->setDirection(QAbstractAnimation::Direction::Forward);
    animation_pic_up->setDuration(300);
    /*设置动画组，按照顺序执行动画*/
    QSequentialAnimationGroup *pPosGroup = new QSequentialAnimationGroup(this);
    pPosGroup->addAnimation(animation_text_temp_left);
    pPosGroup->addAnimation(animation_text_weather_left);
    pPosGroup->addAnimation(animation_pic_up);
    pPosGroup->start(QAbstractAnimation::DeleteWhenStopped);
}
void WX::down()
{
    btn_fold->setText("折叠");

    QPropertyAnimation * animation_pic_up = new QPropertyAnimation(ui->frame,"pos");
    animation_pic_up->setStartValue(QPoint(ui->frame->pos().x(),ui->frame->pos().y()));
    animation_pic_up->setEndValue(QPoint(ui->frame->pos().x(),ui->frame->pos().y() + 400));
    animation_pic_up->setDirection(QAbstractAnimation::Direction::Forward);
    animation_pic_up->setDuration(500);

    //当前温度文字的label移动动画
    QPropertyAnimation * animation_text_temp_left = new QPropertyAnimation(label_temp,"pos");
    animation_text_temp_left->setStartValue(label_temp->pos());//开始移动的位置
    animation_text_temp_left->setEndValue(QPoint(170,120));//结束位置
    animation_text_temp_left->setDirection(QAbstractAnimation::Direction::Forward);//设置动画的方向
    animation_text_temp_left->setDuration(500);//仿真时间

    /*当前天气状况文字的移动动画*/
    QPropertyAnimation * animation_text_weather_left = new QPropertyAnimation(label_curr_weather,"pos");
    animation_text_weather_left->setStartValue(label_curr_weather->pos());
    animation_text_weather_left->setEndValue(QPoint(210,220));
    animation_text_weather_left->setDirection(QAbstractAnimation::Direction::Forward);
    animation_text_weather_left->setDuration(500);


    /*设置动画组，按照顺序执行动画*/
    QSequentialAnimationGroup *pPosGroup = new QSequentialAnimationGroup(this);
    pPosGroup->addAnimation(animation_pic_up);
    pPosGroup->addAnimation(animation_text_weather_left);
    pPosGroup->addAnimation(animation_text_temp_left);
    pPosGroup->start(QAbstractAnimation::DeleteWhenStopped);
    //恢复所有控件的显示
    ui->label_curr_date->show();
    ui->lineEdit->show();
    ui->widget->show();
    ui->label_23->show();
    ui->label_21->show();
    ui->label_24->show();
    ui->label_27->show();
    ui->label_25->show();
    ui->label_26->show();
    ui->label_29->show();
    ui->label_30->show();
    ui->pushButton->show();
}
