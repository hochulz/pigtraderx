#ifndef CPSVRNEW7244S_H
#define CPSVRNEW7244S_H
#include <QAxWidget>

class CpSvrNew7244S : public QAxWidget
{

public:
    explicit CpSvrNew7244S(QWidget *parent = 0) : QAxWidget(parent)
    {
        setControl("{5B6B6338-5843-4d9f-87BB-5C5BB40C579D}");
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

#endif // CPSVRNEW7244S_H
