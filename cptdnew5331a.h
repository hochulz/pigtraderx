#ifndef CPTDNEW5331A_H
#define CPTDNEW5331A_H
#include <QAxWidget>

class CpTdNew5331A : public QAxWidget
{

public:
    explicit CpTdNew5331A(QWidget *parent = 0) : QAxWidget(parent)
    {
        setControl("{A01B00EA-B723-4f29-884E-F12F6760F248}");
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

#endif // CPTDNEW5331A_H
