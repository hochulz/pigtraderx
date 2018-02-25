#ifndef STOCKMST2_H
#define STOCKMST2_H
#include <QAxWidget>

class StockMst2 : public QAxWidget
{

public:
    explicit StockMst2(QWidget *parent = 0) : QAxWidget(parent)
    {
        setControl("{EEA6A7D7-1E8C-4D90-8ACB-7BD391694978}");
    }

    void SetInputValue(int type, const QString &value)
    {
        dynamicCall("SetInputValue(int, QString)", type, value );
    }

    void Request()
    {
        dynamicCall("Request()");
    }

    void BlockRequest()
    {
        dynamicCall("BlockRequest()");
    }

    QVariant GetHeaderValue(int type)
    {
        QVariant ret = dynamicCall("GetHeaderValue(int)", type);
        return ret;
    }

    QVariant GetDataValue(int type, int index)
    {
        QVariant ret = dynamicCall("GetDataValue(int, int)", type, index);
        return ret;
    }
};

#endif // STOCKMST2_H
