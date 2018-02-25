#ifndef CPCONCLUSION_H
#define CPCONCLUSION_H
#include <QAxWidget>

class CpConclusion : public QAxWidget
{

public:
    explicit CpConclusion(QWidget *parent = 0) : QAxWidget(parent)
    {
        setControl("{72FDAF04-F87B-47E1-9396-0A7C98F4E5C5}");
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

#endif // CPCONCLUSION_H
