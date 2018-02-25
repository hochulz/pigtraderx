#ifndef CPTD6033_H
#define CPTD6033_H
#include <QAxWidget>

class CpTd6033 : public QAxWidget
{

public:
    explicit CpTd6033(QWidget *parent = 0) : QAxWidget(parent)
    {
        setControl("{1E090CEF-B084-4961-93C3-92360F4D2226}");
    }

    void SetInputValue(int type, const QString &value)
    {
        dynamicCall("SetInputValue(int, QString)", type, value);
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

#endif // CPTD6033_H
