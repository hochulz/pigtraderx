#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "cpcybos.h"
#include "cpcodemgr.h"
#include "cptdutil.h"
#include "stockmst2.h"
#include "stockcur.h"
#include "stockjpbid.h"
#include "futuremst.h"
#include "futurecuronly.h"
#include "cptd6033.h"
#include "cptd0311.h"
#include "cpconclusion.h"
//#include "cpsvr7244.h"
//#include "cpsvrnew7244s.h"
#include "cptdnew5331a.h"
#include "stock.h"
#include "kiwoom.h"
#include <QFile>
#include <QTimer>
#include <QTextStream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    CpCybos *mCpCybos;
    CpCodeMgr *mCpCodeMgr;
    CpTdUtil *mCpTdUtil;
    StockMst2 *mStockMst2;
    StockCur *mStockCur;
    StockJpBid *mStockJpBid;
//    CpSvr7244 *mCpSvr7244;
//    CpSvr7244 *mCpSvr7244_2;
//    CpSvrNew7244S *mCpSvrNew7244S;
    CpTdNew5331A *mCpTdNew5331A;
    FutureMst *mFutureMst;
    FutureCurOnly *mFutureCurOnly;
    CpTd6033 *mCpTd6033;
    CpConclusion *mCpConclusion;
    CpTd0311 *mCpTd0311;
    CpTd0311 *mCpTd0311_2;

    Kiwoom *kiwoom;
    QTimer *timer; //NAV 체크용
    QTimer *timer_2; //장초반 거래제한용
//    int timer_intv;

    QString mCode; //인버스
    QString mCode_2; //레버리지
    int mQty; //인버스
    int mQty_2; //레버리지

    Stock *mStock; //인버스
    Stock *mStock_2; //레버리지

    QString mAccNo; //계좌번호
    QString mGoodCode; //상품구분코드
    unsigned long seed; //주문가능금액

    double NAVdiscSum;

    // flag 모음
    QChar NAVDiscFlag; // D: discount P: premium N: neutral
    int marketFlag; // 0: 장마감 1: 9:00 ~ 9:01 2: 9:01~15:20 3: 15:20~15:30
    QChar holdFlag; // T F

//    QThread ETFThread;

//    void testBuy();

    QStringList totalCodeList;
    QStringList totalNameList;

    long long levPDFCash;

    QString levPDFFutureCode;
    QString levPDFFutureName;
    double levPDFFutureQty;
    long long levPDFFutureBalance;

    QString levPDFFutureCode_2;
    QString levPDFFutureName_2;
    double levPDFFutureQty_2;
    long long levPDFFutureBalance_2;

    QVector<QString> levPDFStockCode;
    QVector<QString> levPDFStockName;
    QVector<double> levPDFStockQty;
    QVector<long long> levPDFStockBalance;

    QVector<QString> levPDFSFutureCode;
    QVector<QString> levPDFSFutureName;
    QVector<double> levPDFSFutureQty;
    QVector<long long> levPDFSFuturekBalance;

    long long levBalance;
    double levNAV;
    double levDisc;

    long long invPDFCash;

    QString invPDFFutureCode;
    QString invPDFFutureName;
    double invPDFFutureQty;
    long long invPDFFutureBalance;

    QString invPDFFutureCode_2;
    QString invPDFFutureName_2;
    double invPDFFutureQty_2;
    long long invPDFFutureBalance_2;

    long long invBalance;
    double invNAV;
    double invDisc;

    void displayNAV(int gubun);
    void updateDisc(int gubun);

    QPair<int, int> getBuyQty(int askp1, int askv1, int askp2, int askv2, unsigned long cash);
    QPair<int, int> getSellQty(int qty1, int bidp1, int bidv1, int qty2, int bidp2, int bidv2);


private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();


    void received_StockMst2();
    void received_FutureMst();
    void received_StockCur();
    void received_FutureCurOnly();
    void received_StockJpBid();
//    void received_CpSvr7244();
//    void received_CpSvr7244_2();
//    void received_CpSvrNew7244S();
    void received_CpTdNew5331A();
    void received_CpTd6033();
    void received_CpConclusion();

//    void on_marketOpen_received();
//    void on_marketClose_received();

//    void on_levNAVReady_received(double levNAV);
//    void on_invNAVReady_received(double invNAV);

    int on_server_connected(int err_code);
    void on_tr_data_received(const QString &sScrNo, const QString &sRQName, const QString &sTrCode, const QString &sRecordName, const QString &sPrevNext, int nDataLengt, const QString &sErrorCode, const QString &sMessage, const QString &sSplMsg);
    void on_real_data_received(const QString &sCode, const QString &sRealType, const QString &sRealData);

    void on_timeout();
    void on_timeout_2();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
