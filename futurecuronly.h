#ifndef FUTURECURONLY_H
#define FUTURECURONLY_H
#include <QString>
#include <QAxWidget>

class FutureCurOnly : public QAxWidget
{

public:
    explicit FutureCurOnly(QWidget *parent = 0) : QAxWidget(parent)
    {
        setControl("{21123F9D-FFFC-49B2-ACC1-B89B4EE50A18}");
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

#endif // FUTURECURONLY_H
