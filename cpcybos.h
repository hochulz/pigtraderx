#ifndef CPCYBOS_H
#define CPCYBOS_H
#include <QAxWidget>

class CpCybos : public QAxWidget
{

public:
    explicit CpCybos(QWidget *parent = 0) : QAxWidget(parent)
    {
        setControl("{19A11288-2B28-45C4-8CD4-3A12B60C3BD7}");
    }

    int IsConnect()
    {
        int ret = property("IsConnect").toInt();
        return ret;
    }
};


#endif // CPCYBOS_H
