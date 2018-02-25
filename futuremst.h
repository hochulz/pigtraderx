#ifndef FUTUREMST_H
#define FUTUREMST_H
#include <QString>
#include <QAxWidget>

class FutureMst : public QAxWidget
{

public:
    explicit FutureMst(QWidget *parent = 0) : QAxWidget(parent)
    {
        setControl("{1583EA45-B4D3-4B3A-8018-A0FDF8334619}");
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
};

#endif // FUTUREMST_H
