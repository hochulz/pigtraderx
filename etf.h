#ifndef ETF_H
#define ETF_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QFile>
#include <QEventLoop>
#include <QTextStream>
#include <QDebug>
#include "kiwoom.h"

class ETF : public QObject
{
    Q_OBJECT

public:
    ETF();

    Kiwoom *kiwoom;
    QStringList totalCodeList;
    QStringList totalNameList;

    long long levPDFCash;

    QString levPDFFutureCode;
    QString levPDFFutureName;
    double levPDFFutureQty;
    long long levPDFFutureBalance;

    QVector<QString> levPDFStockCode;
    QVector<QString> levPDFStockName;
    QVector<double> levPDFStockQty;
    QVector<long long> levPDFStockBalance;

    long long levBalance;
    double levNAV;

    long long invPDFCash;

    QString invPDFFutureCode;
    QString invPDFFutureName;
    double invPDFFutureQty;
    long long invPDFFutureBalance;

    long long invBalance;
    double invNAV;



private slots:
    int on_server_connected(int err_code);
    void on_tr_data_received(const QString &sScrNo, const QString &sRQName, const QString &sTrCode, const QString &sRecordName, const QString &sPrevNext, int nDataLengt, const QString &sErrorCode, const QString &sMessage, const QString &sSplMsg);
    void on_real_data_received(const QString &sCode, const QString &sRealType, const QString &sRealData);

signals:
    void marketOpen();
    void marketClose();
    void levNAVReady(double levNAV);
    void invNAVReady(double invNAV);

};

#endif // ETF_H
