#ifndef CPTD0311_H
#define CPTD0311_H
#include <QAxWidget>

class CpTd0311 : public QAxWidget
{

public:
    explicit CpTd0311(QWidget *parent = 0) : QAxWidget(parent)
    {
        setControl("{ACAD853E-9FA9-4165-9CE9-166BF4414AA3}");
    }

    void SetInputValue(int type, const QString &value)
    {
        dynamicCall("SetInputValue(int, QString)", type, value);
    }

    void SetInputValue(int type, int value)
    {
        dynamicCall("SetInputValue(int, int)", type, value);
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

#endif // CPTD0311_H
