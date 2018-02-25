#ifndef CPSVR7244_H
#define CPSVR7244_H
#include <QAxWidget>

class CpSvr7244 : public QAxWidget
{

public:
    explicit CpSvr7244(QWidget *parent = 0) : QAxWidget(parent)
    {
        setControl("{F5075466-0FB5-45CC-85AA-D902F1D173C3}");
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

#endif // CPSVR7244_H
