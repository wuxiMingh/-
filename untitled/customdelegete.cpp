#include "customdelegete.h"

customDelegete::customDelegete()
{

}

void customDelegete::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    QStyledItemDelegate::paint(painter, option, index);
//    qDebug()<<"tttttttttttttt"<<endl;

    QVariant ret = index.model()->data(index);//index.model()获取表格的数据模型，在通过该模型获取索引为index的数据
    qDebug()<<ret<<endl;
    QImage image;
    image.load(":/天气图标/天气-中度霾.png");
    qDebug()<<option.rect<<endl;//格子的矩形
    QRect rect = option.rect;//option.rect是表格的矩形，包含了格子在整个表格的位置和矩形的宽高
    rect.setX(5);//这个75是以400为基数，也就是矩形的右边是475
    rect.setY(5);//矩形的左边是425
    painter->drawImage(rect, image);
}
