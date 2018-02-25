#ifndef STOCKJPBID_H
#define STOCKJPBID_H
#include <QAxWidget>

class StockJpBid : public QAxWidget
{

public:
    explicit StockJpBid(QWidget *parent = 0) : QAxWidget(parent)
    {
        setControl("{4809613A-0B62-11D4-8234-00105A7C4F8C}");
    }

    void SetInputValue(int type, const QString &value)
    {
        dynamicCall("SetInputValue(int, QString)", type, value );
    }

    void Subscribe()
    {
        dynamicCall("Subscribe()");
    }

    void Unsubscribe()
    {
        dynamicCall("Unsubscribe()");
    }

    QVariant GetHeaderValue(int type)
    {
        QVariant ret = dynamicCall("GetHeaderValue(int)", type);
        return ret;
    }
};

#endif // STOCKJPBID_H
