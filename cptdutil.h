#ifndef CPTDUTIL_H
#define CPTDUTIL_H
#include <QAxWidget>

class CpTdUtil : public QAxWidget
{

public:
    explicit CpTdUtil(QWidget *parent = 0) : QAxWidget(parent)
    {
        setControl("{1172da0d-1235-4348-880b-10f95ac86e46}");
    }

    QString AccountNumber()
    {
        QString ret = property("AccountNumber").toString();
        return ret;
    }

    int TradeInit(int index)
    {
       int ret =  dynamicCall("TradeInit(int)", index).toInt();
       return ret;
    }

    QString GoodsList(QString accNo, int filter)
    {
       QString ret =  dynamicCall("GoodsList(QString, int)", accNo, filter).toString();
       return ret;
    }
};

#endif // CPTDUTIL_H
