#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 크레온 통신 오브젝트 생성
    mCpCybos = new CpCybos();
    mCpCodeMgr = new CpCodeMgr();
    mCpTdUtil = new CpTdUtil();
    mStockMst2 = new StockMst2();
    mStockCur = new StockCur();
    mStockJpBid = new StockJpBid();
//    mCpSvr7244 = new CpSvr7244();
//    mCpSvr7244_2 = new CpSvr7244();
//    mCpSvrNew7244S = new CpSvrNew7244S();
    mCpTdNew5331A = new CpTdNew5331A();
    mFutureMst = new FutureMst();
    mFutureCurOnly = new FutureCurOnly();
    mCpTd6033 = new CpTd6033();
    mCpConclusion = new CpConclusion();
    mCpTd0311 = new CpTd0311();
    mCpTd0311_2 = new CpTd0311();


    //키움 오브젝트 생성
    kiwoom = new Kiwoom();
    QEventLoop loop;
    connect(kiwoom, SIGNAL(OnEventConnect(int)), this, SLOT(on_server_connected(int)));
    connect(kiwoom, SIGNAL(OnEventConnect(int)), &loop, SLOT(quit()));
    connect(kiwoom, SIGNAL(OnReceiveTrData(QString, QString, QString, QString, QString, int, QString, QString, QString)), this, SLOT(on_tr_data_received(QString, QString, QString, QString, QString, int, QString, QString, QString)));
    connect(kiwoom, SIGNAL(OnReceiveRealData(QString, QString, QString)), this, SLOT(on_real_data_received(QString, QString, QString)));
    kiwoom->CommConnect();
    loop.exec();


    //타이머 설정 (장시작후 XX초 대기)
//    timer = new QTimer(this);
//    timer->setSingleShot(true);
//    timer->setTimerType(Qt::PreciseTimer);
//    timer_intv = 300000;
//    connect(timer, QTimer::timeout, this, &MainWindow::on_timeout);


    //ETF 종목
    mCode = QStringLiteral("A251340"); //인버스
    mCode_2 = QStringLiteral("A233740"); //레버리지
    mStock = new Stock(mCode); //인버스
    mStock_2 = new Stock(mCode_2); //레버리지

    ui->lineEdit->setText(mStock->strCode);
    ui->lineEdit_16->setText(mStock_2->strCode);


    //연결상태 확인
    ui->statusBar->showMessage(QStringLiteral("연결 끊김"));
    if (mCpCybos->IsConnect() == 1) {
        ui->statusBar->showMessage(QStringLiteral("연결 정상"));
    }

    // 주문 초기화 (CpTdUtil)
    int rv = mCpTdUtil->TradeInit(0);
    if (rv == 0) {
        mAccNo = mCpTdUtil->AccountNumber();
        mGoodCode = mCpTdUtil->GoodsList(mAccNo, 1);
        ui->lineEdit_31->setText(mAccNo);
        ui->lineEdit_34->setText(mGoodCode);

        mCpTd0311->SetInputValue(1, mAccNo);
        mCpTd0311->SetInputValue(2, mGoodCode);
        mCpTd0311->SetInputValue(3, mCode);
        mCpTd0311->SetInputValue(5, 0);
        mCpTd0311->SetInputValue(7, QStringLiteral("0"));
        mCpTd0311->SetInputValue(8, QStringLiteral("03"));

        mCpTd0311_2->SetInputValue(1, mAccNo);
        mCpTd0311_2->SetInputValue(2, mGoodCode);
        mCpTd0311_2->SetInputValue(3, mCode_2);
        mCpTd0311_2->SetInputValue(5, 0);
        mCpTd0311_2->SetInputValue(7, QStringLiteral("0"));
        mCpTd0311_2->SetInputValue(8, QStringLiteral("03"));

        mCpTdNew5331A->SetInputValue(0, mAccNo);
        mCpTdNew5331A->SetInputValue(1, mGoodCode);

        mCpTd6033->SetInputValue(0, mAccNo);
        mCpTd6033->SetInputValue(1, mGoodCode);

    }

    // 주문가능금액 (CpTdNew5331A)
    connect(mCpTdNew5331A, SIGNAL(Received()), this, SLOT(received_CpTdNew5331A()));
//    mCpTdNew5331A->SetInputValue(0, mAccNo);
//    mCpTdNew5331A->SetInputValue(1, mGoodCode);
    mCpTdNew5331A->Request();

    // 잔고 (CpTd6033)
    connect(mCpTd6033, SIGNAL(Received()), this, SLOT(received_CpTd6033()));
//    mCpTd6033->SetInputValue(0, mAccNo);
//    mCpTd6033->SetInputValue(1, mGoodCode);
    mCpTd6033->Request();

    // 기본세팅 초기화
//    discFlag = QChar('N');
    marketFlag = QChar('F');
//    holdFlag = QChar('F');
//    ui->lineEdit_38->setText(holdFlag);
    ui->lineEdit_39->setText(marketFlag);

    //타이머 설정
    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, QTimer::timeout, this, &MainWindow::on_checkDisc);


    //ETF 현재가조회 (CpSvr7244)
//    connect(mCpSvr7244, SIGNAL(Received()), this, SLOT(received_CpSvr7244()));
//    mCpSvr7244->SetInputValue(0, mCode);
//    mCpSvr7244->BlockRequest();

//    connect(mCpSvr7244_2, SIGNAL(Received()), this, SLOT(received_CpSvr7244_2()));
//    mCpSvr7244_2->SetInputValue(0, mCode_2);
//    mCpSvr7244_2->BlockRequest();

    //실시간 ETF (CpSvrNew7244S)
//    connect(mCpSvrNew7244S, SIGNAL(Received()), this, SLOT(received_CpSvrNew7244S()));
//    mCpSvrNew7244S->SetInputValue(0, mCode);
//    mCpSvrNew7244S->Subscribe();
//    mCpSvrNew7244S->SetInputValue(0, mCode_2);
//    mCpSvrNew7244S->Subscribe();

//    Thread 세팅
//    ETF *mETF = new ETF();
//    mETF->moveToThread(&ETFThread);
//    connect(&ETFThread, &QThread::finished, mETF, &QObject::deleteLater);
//    connect(mETF, &ETF::marketOpen, this, &MainWindow::on_marketOpen_received);
//    connect(mETF, &ETF::marketClose, this, &MainWindow::on_marketClose_received);
//    connect(mETF, &ETF::levNAVReady, this, &MainWindow::on_levNAVReady_received);
//    connect(mETF, &ETF::invNAVReady, this, &MainWindow::on_invNAVReady_received);
//    ETFThread.start();

    levPDFFutureName = QStringLiteral("코스닥150 F 201803");
    levPDFFutureCode = QStringLiteral("106N3");
    invPDFFutureName = levPDFFutureName;
    invPDFFutureCode = levPDFFutureCode;

    QStringList KospiCodeList = mCpCodeMgr->GetStockListByMarket(1);
    QStringList KosdaqCodeList = mCpCodeMgr->GetStockListByMarket(2);

    totalCodeList = KospiCodeList << KosdaqCodeList;
    totalCodeList.removeDuplicates();

    for(int i = 0; i < totalCodeList.size(); i++) {
        totalNameList.append(mCpCodeMgr->CodeToName(totalCodeList.at(i)));
    }

    /*---------------------------------------------------------------------------*/
    QFile file("레버리지.csv");
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "File not exists";
    } else {
        // Create a thread to retrieve data from a file
        QTextStream in(&file);
        in.setCodec("UTF-8");
        //Reads the data up to the end of file
        while (!in.atEnd())
        {
            QString line = in.readLine();
            // Adding to the model in line with the elements

            line = line.replace(",\"", ";");
            line = line.replace("\"", "");
            line = line.replace(",", "");
            QStringList item = line.split(';', QString::SkipEmptyParts);
            for(int i = 0; i < item.size(); i++) {
            }

            if(item.at(0) == levPDFFutureName) {
                levPDFFutureQty = item.at(1).toDouble();
                qDebug() << levPDFFutureName;
            }
            else if(item.at(0) == QStringLiteral("원화현금")) {
                levPDFCash = item.at(3).toLongLong();
                levBalance = levPDFCash;

            }
            else if(item.at(0).contains(QStringLiteral("F 201"))) {
                qDebug() << item.at(0);

            }
            else if(item.size() == 4 && item.at(0) != QStringLiteral("설정현금액")) {

                levPDFStockName.append(item.at(0));
                levPDFStockQty.append(item.at(1).toDouble());
                levPDFStockBalance.append(0);
                int ix = totalNameList.indexOf(item.at(0));
                if(ix == -1) {
                    levPDFStockCode.append("");
                }
                else{
                    levPDFStockCode.append(totalCodeList.at(ix));
                }
            }
        }
        file.close();
    }

    file.setFileName("인버스.csv");
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "File not exists";
    } else {
        // Create a thread to retrieve data from a file
        QTextStream in(&file);
        in.setCodec("UTF-8");
        //Reads the data up to the end of file
        while (!in.atEnd())
        {
            QString line = in.readLine();
            // Adding to the model in line with the elements

            line = line.replace(",\"", ";");
            line = line.replace("\"", "");
            line = line.replace(",", "");
            QStringList item = line.split(';', QString::SkipEmptyParts);
            for(int i = 0; i < item.size(); i++) {
            }

            if(item.at(0) == invPDFFutureName) {
                invPDFFutureQty = item.at(1).toDouble();
            }
            else if(item.at(0) == QStringLiteral("원화현금")) {
                invPDFCash = item.at(3).toLongLong();
                invBalance = invPDFCash;
            }
        }
        file.close();
    }

    /*---------------------------------------------------------------------------*/

//    for(int j = 0; j < levPDFStockCode.size(); j++) {


//        int price = kiwoom->GetMasterLastPrice(levPDFStockCode.at(j));
//        levPDFStockBalance[j] = levPDFStockQty.at(j) * price;
//        levBalance += levPDFStockBalance.at(j);

//        qDebug() << j << levPDFStockName.at(j) << levPDFStockQty.at(j) << levPDFStockBalance.at(j);
//    }

//    qDebug() << levPDFStockName.size() << levPDFStockCode.size() << levPDFStockQty.size() << levPDFStockBalance.size();
//    qDebug() << "levPDFstock + cash:" << levBalance;
    /*---------------------------------------------------------------------------*/

    //stock 현재가 조회 (최대 200개)
    QStringList temp1;
    QStringList temp2;
    for(int i = 0; i < 100; i++) {
        temp1.append(levPDFStockCode.at(i));
//        qDebug() << levPDFStockName.at(i) << levPDFStockCode.at(i) << levPDFStockQty.at(i);
    }
    for(int i = 100; i < levPDFStockCode.size(); i++) {
        temp2.append(levPDFStockCode.at(i));
//        qDebug() << levPDFStockName.at(i) << levPDFStockCode.at(i) << levPDFStockQty.at(i);
    }
    QString temp11 = temp1.join(",");
    QString temp22 = temp2.join(",");
    connect(mStockMst2, SIGNAL(Received()), this, SLOT(received_StockMst2()));
    mStockMst2->SetInputValue(0, mCode + "," + mCode_2 + "," + temp11);
    mStockMst2->BlockRequest();
    mStockMst2->SetInputValue(0, temp22);
    mStockMst2->BlockRequest();

    //future 현재가 조회
    connect(mFutureMst, SIGNAL(Received()), this, SLOT(received_FutureMst()));
    mFutureMst->SetInputValue(0, levPDFFutureCode);
    mFutureMst->BlockRequest();

    //실시간 시세 (StockCur)
    connect(mStockCur, SIGNAL(Received()), this, SLOT(received_StockCur()));
    mStockCur->SetInputValue(0, mCode);
    mStockCur->Subscribe();
    mStockCur->SetInputValue(0, mCode_2);
    mStockCur->Subscribe();
    for(int i = 0; i < levPDFStockCode.size(); i++) {
        mStockCur->SetInputValue(0, levPDFStockCode.at(i));
        mStockCur->Subscribe();
    }

    //실시간 호가 (StockJpBid)
    connect(mStockJpBid, SIGNAL(Received()), this, SLOT(received_StockJpBid()));
    mStockJpBid->SetInputValue(0, mCode);
    mStockJpBid->Subscribe();
    mStockJpBid->SetInputValue(0, mCode_2);
    mStockJpBid->Subscribe();

    //실시간 시세 (FutureCur)
    connect(mFutureCurOnly, SIGNAL(Received()), this, SLOT(received_FutureCurOnly()));
    mFutureCurOnly->SetInputValue(0, levPDFFutureCode);
    mFutureCurOnly->Subscribe();

    //주식선물 종목
    levPDFSFutureCode << QStringLiteral("1CPN3000");
    levPDFSFutureName << QStringLiteral("셀트리온   F 201803 (  10)");
    levPDFSFutureQty << -2.98;
    levPDFSFuturekBalance << 0;

    kiwoom->SetInputValue(QStringLiteral("근월물구분"), QStringLiteral("1"));
    kiwoom->CommRqData(QStringLiteral("주식선물시세표"), QStringLiteral("opt50044"), 0, QStringLiteral("0003"));

    //주문결과 처리
    connect(mCpConclusion, SIGNAL(Received()), this, SLOT(received_CpConclusion()));
    mCpConclusion->Subscribe();
}

MainWindow::~MainWindow()
{
//    ETFThread.quit();
//    ETFThread.wait();
    delete ui;
}

void MainWindow::on_pushButton_clicked() //Status
{
    if (mCpCybos->IsConnect() == 1) {
        ui->statusBar->showMessage(QStringLiteral("연결 정상"));
    }
    else {
        ui->statusBar->showMessage(QStringLiteral("연결 끊김"));
    }

    long long sum = 0;
    for(int i = 0; i < levPDFStockBalance.size(); i++) {
        sum += levPDFStockBalance.at(i);
        qDebug() << levPDFStockName.at(i) << levPDFStockCode.at(i) << levPDFStockBalance.at(i);
    }
    qDebug() << levPDFCash << sum << levPDFSFuturekBalance.at(0) << levPDFFutureBalance;

    displayNAV(0);
    updateDisc(0);

}

void MainWindow::on_pushButton_2_clicked() //Test
{
//    timer->start(500);
    mCpTd6033->Request();
}

void MainWindow::on_checkDisc()
{
    int spread1 = mStock->askPrice1 - mStock->bidPrice1;
    int spread2 = mStock_2->askPrice1 - mStock_2->bidPrice1;

    if(NAVDiscFlag == QChar('D') && holdFlag == QChar('F')) {
        if(spread1 == 5 && spread2 == 5) {
            QPair<int, int> temp = getBuyQty(mStock->askPrice1, mStock->askVolume1, mStock_2->askPrice1, mStock_2->askVolume1, seed);
            int qty1 = temp.first;
            int qty2 = temp.second;

            mCpTd0311->SetInputValue(0, QStringLiteral("2"));
//            mCpTd0311->SetInputValue(1, mAccNo);
//            mCpTd0311->SetInputValue(2, mGoodCode);
//            mCpTd0311->SetInputValue(3, mCode);
            mCpTd0311->SetInputValue(4, qty1);
//            mCpTd0311->SetInputValue(5, 0);
//            mCpTd0311->SetInputValue(7, QStringLiteral("0"));
//            mCpTd0311->SetInputValue(8, QStringLiteral("03"));
            mCpTd0311->Request();

            mCpTd0311_2->SetInputValue(0, QStringLiteral("2"));
//            mCpTd0311_2->SetInputValue(1, mAccNo);
//            mCpTd0311_2->SetInputValue(2, mGoodCode);
//            mCpTd0311_2->SetInputValue(3, mCode_2);
            mCpTd0311_2->SetInputValue(4, qty2);
//            mCpTd0311_2->SetInputValue(5, 0);
//            mCpTd0311_2->SetInputValue(7, QStringLiteral("0"));
//            mCpTd0311_2->SetInputValue(8, QStringLiteral("03"));
            mCpTd0311_2->Request();

            qDebug() << QStringLiteral("Buy") << mStock->midPrice << mStock->askVolume1 << mStock_2->midPrice << mStock_2->askVolume1;
            qDebug() << QStringLiteral("====================================================");
        }

    }

    else if(NAVDiscFlag == QChar('P')) {
        if(spread1 == 5 && spread2 == 5) {
            QPair<int, int> temp = getSellQty(mStock->qty, mStock->bidPrice1, mStock->bidVolume1, mStock_2->qty, mStock_2->bidPrice1, mStock_2->bidVolume1);
            int qty1 = temp.first;
            int qty2 = temp.second;

            if(qty1 > 0) {
                mCpTd0311->SetInputValue(0, QStringLiteral("1"));
//                mCpTd0311->SetInputValue(1, mAccNo);
//                mCpTd0311->SetInputValue(2, mGoodCode);
//                mCpTd0311->SetInputValue(3, mCode);
                mCpTd0311->SetInputValue(4, qty1);
//                mCpTd0311->SetInputValue(5, 0);
//                mCpTd0311->SetInputValue(7, QStringLiteral("0"));
//                mCpTd0311->SetInputValue(8, QStringLiteral("03"));
                mCpTd0311->Request();
            }
            if(qty2 > 0) {
                mCpTd0311_2->SetInputValue(0, QStringLiteral("1"));
//                mCpTd0311_2->SetInputValue(1, mAccNo);
//                mCpTd0311_2->SetInputValue(2, mGoodCode);
//                mCpTd0311_2->SetInputValue(3, mCode_2);
                mCpTd0311_2->SetInputValue(4, qty2);
//                mCpTd0311_2->SetInputValue(5, 0);
//                mCpTd0311_2->SetInputValue(7, QStringLiteral("0"));
//                mCpTd0311_2->SetInputValue(8, QStringLiteral("03"));
                mCpTd0311_2->Request();
            }
            qDebug() << QStringLiteral("Sell") << mStock->midPrice << mStock->bidVolume1 << mStock_2->midPrice << mStock_2->bidVolume1;
            qDebug() << QStringLiteral("====================================================");
        }
    }

}

void MainWindow::received_StockMst2()
{
    int count = mStockMst2->GetHeaderValue(0).toInt();

    for (int i = 0; i < count; i++) {

        QString strCode = mStockMst2->GetDataValue(0, i).toString();

        if (strCode == mCode) {
            mStock->askPrice1 = mStockMst2->GetDataValue(9, i).toInt();
            mStock->askVolume1 = mStockMst2->GetDataValue(15, i).toInt();
            mStock->bidPrice1 = mStockMst2->GetDataValue(10, i).toInt();
            mStock->bidVolume1 = mStockMst2->GetDataValue(16, i).toInt();
            mStock->midPrice = ((float)mStock->askPrice1 + mStock->bidPrice1) / 2;

            ui->lineEdit_8->setText(QString::number(mStock->askPrice1));
            ui->lineEdit_9->setText(QString::number(mStock->askVolume1));
            ui->lineEdit_13->setText(QString::number(mStock->bidPrice1));
            ui->lineEdit_12->setText(QString::number(mStock->bidVolume1));
            ui->lineEdit_5->setText(QString::number(mStock->midPrice));

        }
        else if (strCode == mCode_2) {
            mStock_2->askPrice1 = mStockMst2->GetDataValue(9, i).toInt();
            mStock_2->askVolume1 = mStockMst2->GetDataValue(15, i).toInt();
            mStock_2->bidPrice1 = mStockMst2->GetDataValue(10, i).toInt();
            mStock_2->bidVolume1 = mStockMst2->GetDataValue(16, i).toInt();
            mStock_2->midPrice = ((float)mStock_2->askPrice1 + mStock_2->bidPrice1) / 2;

            ui->lineEdit_25->setText(QString::number(mStock_2->askPrice1));
            ui->lineEdit_26->setText(QString::number(mStock_2->askVolume1));
            ui->lineEdit_27->setText(QString::number(mStock_2->bidPrice1));
            ui->lineEdit_28->setText(QString::number(mStock_2->bidVolume1));
            ui->lineEdit_20->setText(QString::number(mStock_2->midPrice));
        }
        else {
            int price = mStockMst2->GetDataValue(3, i).toInt();
            int idx = levPDFStockCode.indexOf(strCode);
            levPDFStockBalance[idx] = levPDFStockQty.at(idx) * price;
            levBalance += levPDFStockBalance.at(idx);

        }
    }

    updateDisc(0);
    displayNAV(0);

}

void MainWindow::received_FutureMst()
{
    float increase= mFutureMst->GetHeaderValue(77).toFloat();
    qDebug() << "선물전일대비:" << increase;

    levPDFFutureBalance = increase * levPDFFutureQty * 10000;
    levBalance +=  levPDFFutureBalance;
    levNAV = levBalance / 100000.0;

    invPDFFutureBalance = increase * invPDFFutureQty * 10000;
    invBalance +=  invPDFFutureBalance;
    invNAV = invBalance / 100000.0;

    updateDisc(0);
    displayNAV(0);
}

void MainWindow::received_StockCur()
{
    QString strCode = mStockCur->GetHeaderValue(0).toString();

    if (strCode == mCode) {
        mStock->askPrice1 = mStockCur->GetHeaderValue(7).toInt();
        mStock->bidPrice1 = mStockCur->GetHeaderValue(8).toInt();
        mStock->midPrice = ((float)mStock->askPrice1 + mStock->bidPrice1) / 2;

        ui->lineEdit_8->setText(QString::number(mStock->askPrice1));
        ui->lineEdit_13->setText(QString::number(mStock->bidPrice1));
        ui->lineEdit_5->setText(QString::number(mStock->midPrice));

        updateDisc(1);
    }

    else if (strCode == mCode_2) {
        mStock_2->askPrice1 = mStockCur->GetHeaderValue(7).toInt();
        mStock_2->bidPrice1 = mStockCur->GetHeaderValue(8).toInt();
        mStock_2->midPrice = ((float)mStock_2->askPrice1 + mStock_2->bidPrice1) / 2;

        ui->lineEdit_25->setText(QString::number(mStock_2->askPrice1));
        ui->lineEdit_27->setText(QString::number(mStock_2->bidPrice1));
        ui->lineEdit_20->setText(QString::number(mStock_2->midPrice));

        updateDisc(2);
    }

    else {
        int idx = levPDFStockCode.indexOf(strCode);
        if(idx != -1) {
            int price = mStockCur->GetHeaderValue(13).toInt();
            levBalance = levBalance - levPDFStockBalance.at(idx);
            levPDFStockBalance[idx] = levPDFStockQty.at(idx) * price;
            levBalance += levPDFStockBalance[idx];
            levNAV = levBalance / 100000.0;

        }
        updateDisc(2);
        displayNAV(2);

    }
}

void MainWindow::received_FutureCurOnly()
{
    float increase = mFutureCurOnly->GetHeaderValue(2).toFloat();

    levBalance = levBalance - levPDFFutureBalance;
    levPDFFutureBalance = increase * levPDFFutureQty * 10000;
    levBalance +=  levPDFFutureBalance;
    levNAV = levBalance / 100000.0;

    invBalance = invBalance - invPDFFutureBalance;
    invPDFFutureBalance = increase * invPDFFutureQty * 10000;
    invBalance +=  invPDFFutureBalance;
    invNAV = invBalance / 100000.0;

    updateDisc(0);
    displayNAV(0);
}

void MainWindow::received_StockJpBid()
{
    QString strCode = mStockJpBid->GetHeaderValue(0).toString();

    if (strCode == mCode) {
        mStock->askPrice1 = mStockJpBid->GetHeaderValue(3).toInt();
        mStock->bidPrice1 = mStockJpBid->GetHeaderValue(4).toInt();
        mStock->midPrice = ((float)mStock->askPrice1 + mStock->bidPrice1) / 2;
        mStock->askVolume1 = mStockJpBid->GetHeaderValue(5).toInt();
        mStock->bidVolume1 = mStockJpBid->GetHeaderValue(6).toInt();
        mStock->askPrice2 = mStockJpBid->GetHeaderValue(7).toInt();
        mStock->bidPrice2 = mStockJpBid->GetHeaderValue(8).toInt();
        mStock->askVolume2 = mStockJpBid->GetHeaderValue(9).toInt();
        mStock->bidVolume2 = mStockJpBid->GetHeaderValue(10).toInt();

        ui->lineEdit_8->setText(QString::number(mStock->askPrice1));
        ui->lineEdit_13->setText(QString::number(mStock->bidPrice1));
        ui->lineEdit_5->setText(QString::number(mStock->midPrice));
        ui->lineEdit_9->setText(QString::number(mStock->askVolume1));
        ui->lineEdit_12->setText(QString::number(mStock->bidVolume1));
        ui->lineEdit_6->setText(QString::number(mStock->askPrice2));
        ui->lineEdit_14->setText(QString::number(mStock->bidPrice2));
        ui->lineEdit_7->setText(QString::number(mStock->askVolume2));
        ui->lineEdit_11->setText(QString::number(mStock->bidVolume2));

        updateDisc(1);
    }

    else if (strCode == mCode_2) {
        mStock_2->askPrice1 = mStockJpBid->GetHeaderValue(3).toInt();
        mStock_2->bidPrice1 = mStockJpBid->GetHeaderValue(4).toInt();
        mStock_2->midPrice = ((float)mStock_2->askPrice1 + mStock_2->bidPrice1) / 2;
        mStock_2->askVolume1 = mStockJpBid->GetHeaderValue(5).toInt();
        mStock_2->bidVolume1 = mStockJpBid->GetHeaderValue(6).toInt();
        mStock_2->askPrice2 = mStockJpBid->GetHeaderValue(7).toInt();
        mStock_2->bidPrice2 = mStockJpBid->GetHeaderValue(8).toInt();
        mStock_2->askVolume2 = mStockJpBid->GetHeaderValue(9).toInt();
        mStock_2->bidVolume2 = mStockJpBid->GetHeaderValue(10).toInt();

        ui->lineEdit_25->setText(QString::number(mStock_2->askPrice1));
        ui->lineEdit_27->setText(QString::number(mStock_2->bidPrice1));
        ui->lineEdit_20->setText(QString::number(mStock_2->midPrice));
        ui->lineEdit_26->setText(QString::number(mStock_2->askVolume1));
        ui->lineEdit_28->setText(QString::number(mStock_2->bidVolume1));
        ui->lineEdit_23->setText(QString::number(mStock_2->askPrice2));
        ui->lineEdit_29->setText(QString::number(mStock_2->bidPrice2));
        ui->lineEdit_24->setText(QString::number(mStock_2->askVolume2));
        ui->lineEdit_30->setText(QString::number(mStock_2->bidVolume2));

        updateDisc(2);
    }
}

//void MainWindow::received_CpSvr7244()
//{
//    mStock->NavTime = mCpSvr7244->GetDataValue(0, 0).toString();
//    mStock->NAV =mCpSvr7244->GetDataValue(6, 0).toFloat()/100;
//    mStock->trackingErr = mCpSvr7244->GetDataValue(8, 0).toFloat()/100;
//    mStock->trackingIdx = mCpSvr7244->GetDataValue(10, 0).toFloat()/100;
//    mStock->NAVdisc = (mStock->midPrice - mStock->NAV) / mStock->NAV * 100;
//    NAVdiscSum = mStock->NAVdisc +  mStock_2->NAVdisc;

//    ui->lineEdit_15->setText(mStock->NavTime);
//    ui->lineEdit_4->setText(QString::number(mStock->NAV));
//    ui->lineEdit_3->setText(QString::number(mStock->trackingErr));
//    ui->lineEdit_2->setText(QString::number(mStock->trackingIdx));
//    ui->lineEdit_10->setText(QString::number(mStock->NAVdisc));
//    ui->lineEdit_42->setText(QString::number(NAVdiscSum));
//}

//void MainWindow::received_CpSvr7244_2()
//{
//    mStock_2->NavTime = mCpSvr7244_2->GetDataValue(0, 0).toString();
//    mStock_2->NAV = mCpSvr7244_2->GetDataValue(6, 0).toFloat()/100;
//    mStock_2->trackingErr = mCpSvr7244_2->GetDataValue(8, 0).toFloat()/100;
//    mStock_2->trackingIdx = mCpSvr7244_2->GetDataValue(10, 0).toFloat()/100;
//    mStock_2->NAVdisc = (mStock_2->midPrice - mStock_2->NAV) / mStock_2->NAV * 100;
//    NAVdiscSum = mStock->NAVdisc +  mStock_2->NAVdisc;

//    ui->lineEdit_22->setText(mStock_2->NavTime);
//    ui->lineEdit_19->setText(QString::number(mStock_2->NAV));
//    ui->lineEdit_18->setText(QString::number(mStock_2->trackingErr));
//    ui->lineEdit_17->setText(QString::number(mStock_2->trackingIdx));
//    ui->lineEdit_21->setText(QString::number(mStock_2->NAVdisc));
//    ui->lineEdit_42->setText(QString::number(NAVdiscSum));
//}

//void MainWindow::received_CpSvrNew7244S()
//{
//    QString strCode = mCpSvrNew7244S->GetHeaderValue(0).toString();

//    if (strCode == mCode) {
//        mStock->NavTime = mCpSvrNew7244S->GetHeaderValue(1).toString();
//        mStock->NAV = mCpSvrNew7244S->GetHeaderValue(6).toFloat()/100;
//        mStock->trackingErr = mCpSvrNew7244S->GetHeaderValue(10).toFloat()/100;
//        mStock->trackingIdx = mCpSvrNew7244S->GetHeaderValue(15).toFloat()/100;
//        mStock->NAVdisc = (mStock->midPrice - mStock->NAV) / mStock->NAV * 100;
//        NAVdiscSum = mStock->NAVdisc +  mStock_2->NAVdisc;

//        ui->lineEdit_15->setText(mStock->NavTime);
//        ui->lineEdit_4->setText(QString::number(mStock->NAV));
//        ui->lineEdit_3->setText(QString::number(mStock->trackingErr));
//        ui->lineEdit_2->setText(QString::number(mStock->trackingIdx));
//        ui->lineEdit_10->setText(QString::number(mStock->NAVdisc));
//        ui->lineEdit_42->setText(QString::number(NAVdiscSum));

//        ui->lineEdit_43->setText(mStock->NavTime);
//    }
//    else if (strCode == mCode_2) {
//        mStock_2->NavTime = mCpSvrNew7244S->GetHeaderValue(1).toString();
//        mStock_2->NAV = mCpSvrNew7244S->GetHeaderValue(6).toFloat()/100;
//        mStock_2->trackingErr = mCpSvrNew7244S->GetHeaderValue(10).toFloat()/100;
//        mStock_2->trackingIdx = mCpSvrNew7244S->GetHeaderValue(15).toFloat()/100;
//        mStock_2->NAVdisc = (mStock_2->midPrice - mStock_2->NAV) / mStock_2->NAV * 100;
//        NAVdiscSum = mStock->NAVdisc +  mStock_2->NAVdisc;

//        ui->lineEdit_22->setText(mStock_2->NavTime);
//        ui->lineEdit_19->setText(QString::number(mStock_2->NAV));
//        ui->lineEdit_18->setText(QString::number(mStock_2->trackingErr));
//        ui->lineEdit_17->setText(QString::number(mStock_2->trackingIdx));
//        ui->lineEdit_21->setText(QString::number(mStock_2->NAVdisc));
//        ui->lineEdit_42->setText(QString::number(NAVdiscSum));

//        ui->lineEdit_43->setText(mStock_2->NavTime);
//    }


//    if (MarketFlag == "T" && HoldFlag == "F" && NAVdiscSum < -0.2) {
//        qDebug() << "Buy" << mStock->askPrice1 << mStock->askVolume1 << mStock_2->askPrice1 << mStock_2->askVolume1;
//        HoldFlag = 'T';
//        ui->lineEdit_38->setText(HoldFlag);
//    }
//    else if (HoldFlag == "T" && NAVdiscSum > 0.0) {
//        qDebug() << "Sell" << mStock->bidPrice1 << mStock->bidVolume1 << mStock_2->bidPrice1 << mStock_2->bidVolume1;
//        qDebug() << "==================================================";
//        HoldFlag = 'F';
//        ui->lineEdit_38->setText(HoldFlag);
//    }
//}

void MainWindow::received_CpTdNew5331A()
{
    seed = mCpTdNew5331A->GetHeaderValue(9).toInt();

    if(seed < 100000) {
        holdFlag = QChar('T');
    }
    else {
        holdFlag = QChar('F');
    }
    ui->lineEdit_37->setText(QString::number(seed));
    ui->lineEdit_38->setText(holdFlag);
}

void MainWindow::received_CpTd6033()
{

    int count = mCpTd6033->GetHeaderValue(7).toInt();
    qDebug() << "count" << count;
    if(count == 0) {
        mStock->qty = 0;
        ui->lineEdit_2->setText(QString::number(0));
        mStock_2->qty = 0;
        ui->lineEdit_17->setText(QString::number(0));
    }
    else {
        for(int i = 0; i < count; i++) {
            QString sCode = mCpTd6033->GetDataValue(12, i).toString();
            if(sCode == mCode) {
                int q = mCpTd6033 ->GetDataValue(7, i).toInt();
                mStock->qty = q;
                ui->lineEdit_2->setText(QString::number(q));
            }
            else if(sCode == mCode_2) {
                int q = mCpTd6033 ->GetDataValue(7, i).toInt();
                mStock_2->qty = q;
                ui->lineEdit_17->setText(QString::number(q));
            }
        }
    }

}

void MainWindow::received_CpConclusion()
{
    qDebug() << 6666;

    int sGubun = mCpConclusion ->GetHeaderValue(14).toInt();
    QString sCode = mCpConclusion ->GetHeaderValue(9).toString();
    QString sOrderGubun = mCpConclusion ->GetHeaderValue(12).toString();
    int qty = mCpConclusion ->GetHeaderValue(3).toInt();
    int price = mCpConclusion ->GetHeaderValue(4).toInt();

    switch(sGubun) {
        case 1 : //체결
            qDebug() << QStringLiteral("체결되었습니다") << price << qty;
            if(sOrderGubun == QStringLiteral("1")) { //매도
                if(sCode == mCode) {
                    mStock->qty -= qty;
                    ui->lineEdit_2->setText(QString::number(mStock->qty));
                }
                else if(sCode == mCode_2) {
                    mStock_2->qty -= qty;
                    ui->lineEdit_17->setText(QString::number(mStock_2->qty));
                }
            }
            else if(sOrderGubun == QStringLiteral("2")) { //매수
                if(sCode == mCode) {
                    mStock->qty += qty;
                    ui->lineEdit_2->setText(QString::number(mStock->qty));
                }
                else if(sCode == mCode_2) {
                    mStock_2->qty += qty;
                    ui->lineEdit_17->setText(QString::number(mStock_2->qty));
                }
            }
            break;

        case 2 : //확인
            qDebug() << QStringLiteral("확인되었습니다");
            break;

        case 3 : //거부
            qDebug() << QStringLiteral("거부되었습니다");
            break;

        case 4 : //접수
            qDebug() << QStringLiteral("접수되었습니다");
            qDebug() << qty;
            break;
    }
//    mCpTdNew5331A->SetInputValue(0, mAccNo);
//    mCpTdNew5331A->SetInputValue(1, mGoodCode);
    mCpTdNew5331A->Request();

}

//void MainWindow::on_marketOpen_received()
//{
//    MarketFlag = 'T';
//    ui->lineEdit_39->setText(MarketFlag);
//}

//void MainWindow::on_marketClose_received()
//{
//    MarketFlag = 'F';
//    ui->lineEdit_39->setText(MarketFlag);
//}

//void MainWindow::on_invNAVReady_received(double invNAV)
//{
//    mStock->NAV = invNAV;
//    mStock->NAVdisc = (mStock->midPrice - invNAV) / invNAV * 100;
//    NAVdiscSum = mStock->NAVdisc +  mStock_2->NAVdisc;

//    testBuy();

//    ui->lineEdit_4->setText(QString::number(invNAV));
//    ui->lineEdit_10->setText(QString::number(mStock->NAVdisc));
//    ui->lineEdit_42->setText(QString::number(NAVdiscSum));
//}

//void MainWindow::on_levNAVReady_received(double levNAV)
//{
//    mStock_2->NAV = levNAV;
//    mStock_2->NAVdisc = (mStock_2->midPrice - levNAV) / levNAV * 100;
//    NAVdiscSum = mStock->NAVdisc +  mStock_2->NAVdisc;

//    testBuy();

//    ui->lineEdit_19->setText(QString::number(levNAV));
//    ui->lineEdit_21->setText(QString::number(mStock_2->NAVdisc));
//    ui->lineEdit_42->setText(QString::number(NAVdiscSum));
//}

//void MainWindow::testBuy()
//{
//    if (MarketFlag == "T" && HoldFlag == "F" && NAVdiscSum < -0.2) {
//        qDebug() << QStringLiteral("Buy") << mStock->askPrice1 << mStock->askVolume1 << mStock_2->askPrice1 << mStock_2->askVolume1;
//        HoldFlag = 'T';
//        ui->lineEdit_38->setText(HoldFlag);
//    }
//    else if (HoldFlag == "T" && NAVdiscSum > 0.0) {
//        qDebug() << QStringLiteral("Sell") << mStock->bidPrice1 << mStock->bidVolume1 << mStock_2->bidPrice1 << mStock_2->bidVolume1;
//        qDebug() << QStringLiteral("==================================================");
//        HoldFlag = 'F';
//        ui->lineEdit_38->setText(HoldFlag);
//    }
//}

int MainWindow::on_server_connected(int err_code)
{
    qDebug() << QStringLiteral("키움연결");
    return err_code;
}

void MainWindow::on_tr_data_received(const QString &sScrNo, const QString &sRQName, const QString &sTrCode, const QString &sRecordName, const QString &sPrevNext, int nDataLengt, const QString &sErrorCode, const QString &sMessage, const QString &sSplMsg)
{
    qDebug() << sRQName;
    if(sRQName == QStringLiteral("주식선물시세표")) {
        int cnt = kiwoom->GetRepeatCnt(sTrCode, sRQName);

        for(int i = 0; i < cnt; i++) {
            QString fName = kiwoom->GetCommData(sTrCode, sRQName, i, QStringLiteral("종목명")).toString().trimmed();
            QString fCode = kiwoom->GetCommData(sTrCode, sRQName, i, QStringLiteral("종목코드")).toString().trimmed();
            int idx = levPDFSFutureCode.indexOf(fCode);
            if(idx != -1) {
                qDebug() << fName << fCode;
                int fIncrease = kiwoom->GetCommData(sTrCode, sRQName, i, QStringLiteral("전일대비")).toInt();
                levPDFSFuturekBalance[idx] = levPDFSFutureQty.at(idx) * fIncrease * 10;
                qDebug() << fIncrease << levPDFSFuturekBalance.at(idx);

                levBalance += levPDFSFuturekBalance.at(idx);
                levNAV = levBalance / 100000.0;

                displayNAV(2);
                updateDisc(2);
            }

        }
    }
}

void MainWindow::on_real_data_received(const QString &sCode, const QString &sRealType, const QString &sRealData)
{
    if(sRealType == QStringLiteral("선물시세")) {
        int idx = levPDFSFutureCode.indexOf(sCode);
        if(idx != -1) {
            int increase = kiwoom->GetCommRealData(sRealType, 11).toInt();
            levBalance = levBalance - levPDFSFuturekBalance.at(idx);
            levPDFSFuturekBalance[idx] = levPDFSFutureQty.at(idx) * increase * 10;

            levBalance += levPDFSFuturekBalance.at(idx);
            levNAV = levBalance / 100000.0;

            displayNAV(2);
            updateDisc(2);
        }



    }

    else if(sRealType == QStringLiteral("장시작시간")) {
        QString market_hour_flag = kiwoom->GetCommRealData(sRealType, 215).toString().trimmed();
        QString min_remain = kiwoom->GetCommRealData(sRealType, 214).toString().trimmed();

        if(market_hour_flag == QStringLiteral("0"))
        {
            if(min_remain == QStringLiteral("000100"))
            {
                qDebug() << "장시작 1분전";
            }
        }
        else if(market_hour_flag == QStringLiteral("3"))
        {
            marketFlag = 'T';
            ui->lineEdit_39->setText(marketFlag);
            qDebug() << QStringLiteral("장시작");
            timer->start(500);
        }
        else if(market_hour_flag == QStringLiteral("2"))
        {
            if(min_remain == QStringLiteral("001000")) {
                marketFlag = 'F';
                ui->lineEdit_39->setText(marketFlag);
                qDebug() << "장종료 10분전";
                timer->stop();

            }
        }
        else if(market_hour_flag == QStringLiteral("4"))
        {

        }
    }
}

void MainWindow::displayNAV(int gubun)
{
    if(gubun == 1) {
        ui->lineEdit_4->setText(QString::number(invNAV));
    }
    else if(gubun == 2) {
        ui->lineEdit_19->setText(QString::number(levNAV));
    }
    else {
        ui->lineEdit_4->setText(QString::number(invNAV));
        ui->lineEdit_19->setText(QString::number(levNAV));
    }
}

void MainWindow::updateDisc(int gubun)
{
    if(gubun == 1) {
        invDisc = (mStock->midPrice - invNAV) / invNAV * 100;
        ui->lineEdit_10->setText(QString::number(invDisc));
    }
    else if(gubun == 2){
        levDisc = (mStock_2->midPrice - levNAV) / levNAV * 100;
        ui->lineEdit_21->setText(QString::number(levDisc));
    }
    else {
        invDisc = (mStock->midPrice - invNAV) / invNAV * 100;
        ui->lineEdit_10->setText(QString::number(invDisc));
        levDisc = (mStock_2->midPrice - levNAV) / levNAV * 100;
        ui->lineEdit_21->setText(QString::number(levDisc));
    }

    NAVdiscSum = (2 * invDisc +  levDisc) / 3;
    ui->lineEdit_42->setText(QString::number(NAVdiscSum));

    if(NAVdiscSum < -0.10) {
        NAVDiscFlag = QChar('D');
    }
    else if(NAVdiscSum > 0.05) {
        NAVDiscFlag = QChar('P');
    }
    else {
        NAVDiscFlag = QChar('N');
    }
}

QPair<int, int> MainWindow::getBuyQty(int askp1, int askv1, int askp2, int askv2, unsigned long cash)
{
    cash = cash / 1.3;
    int cash2 = cash / 3;
    int cash1 = cash - cash2;

    int buyQty1 = cash1 / askp1;
    int buyQty2 = cash2 / askp2;

    QPair<int, int> res;

    if(buyQty1 <= askv1 && buyQty2 <= askv2) {
        res.first = buyQty1;
        res.second = buyQty2;

    }
    else if(buyQty1 <= askv1) {
        buyQty2 = askv2;
        long long temp = buyQty2 * askp2 * 2;
        buyQty1 = temp / askp1;
        res.first = buyQty1;
        res.second = buyQty2;
    }
    else if(buyQty2 <= askv2) {
        buyQty1 = askv1;
        long long temp = buyQty1 * askp1 / 2;
        buyQty2 = temp / askp2;
        res.first = buyQty1;
        res.second = buyQty2;

    }
    return res;
}

QPair<int, int> MainWindow::getSellQty(int qty1, int bidp1, int bidv1, int qty2, int bidp2, int bidv2)
{
    QPair<int, int> res;
    int sellQty1;
    int sellQty2;

    if(qty1 <= bidv1 && qty2 <= bidv2) {
        res.first = qty1;
        res.second = qty2;

    }
    else if(qty1 > bidv1) {
        sellQty1 = bidv1;
        float ratio = float(sellQty1) / qty1;
        sellQty2 = qty2 * ratio;

        res.first = sellQty1;
        res.second = sellQty2;
    }
    else if(qty2 > bidv2) {
        sellQty2 = bidv2;
        float ratio = float(sellQty2) / qty2;
        sellQty1 = qty1 * ratio;

        res.first = sellQty1;
        res.second = sellQty2;

    }
    return res;
}
