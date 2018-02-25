#ifndef STOCKCUR_H
#define STOCKCUR_H
#include <QAxWidget>

class StockCur : public QAxWidget
{

public:
    explicit StockCur(QWidget *parent = 0) : QAxWidget(parent)
    {
        setControl("{55D0C6E7-0966-11D4-8232-00105A7C4F8C}");
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

#endif // STOCKCUR_H
