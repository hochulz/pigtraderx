#ifndef STOCK_H
#define STOCK_H
#include <QString>
#include <QObject>

class Stock : public QObject
{
    Q_OBJECT

public:
    Stock(const QString &code);

    QString strCode;
    int qty;

    int askPrice1;
    int askPrice2;
    int bidPrice1;
    int bidPrice2;
    int askVolume1;
    int askVolume2;
    int bidVolume1;
    int bidVolume2;

    float midPrice;
    int YDClosePrice;

//    float trackingIdx;
//    float trackingErr;

//    double NAV;
//    double NAVdisc;
//    QString NavTime;

};

#endif // STOCK_H
