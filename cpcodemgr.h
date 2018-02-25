#ifndef CPCODEMGR_H
#define CPCODEMGR_H
#include <QAxWidget>

class CpCodeMgr : public QAxWidget
{

public:
    explicit CpCodeMgr(QWidget *parent = 0) : QAxWidget(parent)
    {
        setControl("{995B5ABE-ED4B-4D04-B46D-6238AB66EA71}");
    }

    QString CodeToName(const QString &code)
    {
        QString ret = dynamicCall("CodeToName(QString)", code).toString();
        return ret;
    }

    QStringList GetStockListByMarket(int CPC_MARKET_KOSPI)
    {
        QStringList ret = dynamicCall("GetStockListByMarket(int)", CPC_MARKET_KOSPI).toStringList();
        return ret;
    }

};

#endif // CPCODEMGR_H
