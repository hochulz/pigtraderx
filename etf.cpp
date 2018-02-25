#include "etf.h"


ETF::ETF()
{
    levPDFFutureName = QStringLiteral("코스닥150 F 201803");
    levPDFFutureCode = QStringLiteral("106N3000");
    invPDFFutureName = levPDFFutureName;
    invPDFFutureCode = levPDFFutureCode;


    // 키움 오브젝트 설정
    kiwoom = new Kiwoom();
    QEventLoop loop;
    connect(kiwoom, SIGNAL(OnEventConnect(int)), this, SLOT(on_server_connected(int)));
    connect(kiwoom, SIGNAL(OnEventConnect(int)), &loop, SLOT(quit()));
    connect(kiwoom, SIGNAL(OnReceiveTrData(QString, QString, QString, QString, QString, int, QString, QString, QString)), this, SLOT(on_tr_data_received(QString, QString, QString, QString, QString, int, QString, QString, QString)));
    connect(kiwoom, SIGNAL(OnReceiveRealData(QString, QString, QString)), this, SLOT(on_real_data_received(QString, QString, QString)));
    kiwoom->CommConnect();
    loop.exec();

    QStringList KospiCodeList = kiwoom->GetCodeListByMarket(QStringLiteral("0")).split(';', QString::SkipEmptyParts);
    QStringList KosdaqCodeList = kiwoom->GetCodeListByMarket(QStringLiteral("10")).split(';', QString::SkipEmptyParts);

    totalCodeList = KospiCodeList << KosdaqCodeList;
    totalCodeList.removeDuplicates();


    for(int i = 0; i < totalCodeList.size(); i++) {
        totalNameList.append(kiwoom->GetMasterCodeName(totalCodeList.at(i)));
    }


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
            }
            else if(item.at(0) == QStringLiteral("원화현금")) {
                levPDFCash = item.at(3).toLongLong();
                levBalance = levPDFCash;

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

    for(int j = 0; j < levPDFStockCode.size(); j++) {


        int price = kiwoom->GetMasterLastPrice(levPDFStockCode.at(j));
        levPDFStockBalance[j] = levPDFStockQty.at(j) * price;
        levBalance += levPDFStockBalance.at(j);

        qDebug() << j << levPDFStockName.at(j) << levPDFStockQty.at(j) << levPDFStockBalance.at(j);
    }

    qDebug() << levPDFStockName.size() << levPDFStockCode.size() << levPDFStockQty.size() << levPDFStockBalance.size();
    qDebug() << "levPDFstock + cash:" << levBalance;


    QStringList temp1;
    QStringList temp2;
    for(int i = 0; i < 100; i++) {
        temp1.append(levPDFStockCode.at(i));
    }
    for(int i = 100; i < levPDFStockCode.size(); i++) {
        temp2.append(levPDFStockCode.at(i));
    }
    QString temp11 = temp1.join(";");
    QString temp22 = temp2.join(";");

    //kiwoom->SetRealReg(QStringLiteral("0001"), temp11, QStringLiteral("10"), QStringLiteral("0"));
   // kiwoom->SetRealReg(QStringLiteral("0002"), temp22, QStringLiteral("10"), QStringLiteral("1"));

    qDebug() << temp11;
    qDebug() << temp22;

    kiwoom->SetInputValue(QStringLiteral("종목코드"), levPDFFutureCode);
    kiwoom->CommRqData(QStringLiteral("opt50001;") + levPDFFutureCode, QStringLiteral("opt50001"), 0, QStringLiteral("0000"));


}

int ETF::on_server_connected(int err_code)
{
    return err_code;
}

void ETF::on_tr_data_received(const QString &sScrNo, const QString &sRQName, const QString &sTrCode, const QString &sRecordName, const QString &sPrevNext, int nDataLengt, const QString &sErrorCode, const QString &sMessage, const QString &sSplMsg)
{

    double increase = kiwoom->GetCommData(sTrCode, sRQName, 0, QStringLiteral("전일대비")).toDouble();
    levPDFFutureBalance = increase * levPDFFutureQty * 10000;
    levBalance +=  levPDFFutureBalance;
    levNAV = levBalance / 100000.0;
     emit levNAVReady(levNAV);

    invPDFFutureBalance = increase * invPDFFutureQty * 10000;
    invBalance +=  invPDFFutureBalance;
    invNAV = invBalance / 100000.0;
    emit invNAVReady(invNAV);

//    QStringList RQlist;
//    RQlist << sRQName.split(";", QString::SkipEmptyParts);
//    QString RQ1 = RQlist.at(0);
//    QString RQ2 = RQlist.at(1);

//    if(RQ1 == QStringLiteral("levETF")) {
//        QString strCode = RQ2;

//        int idx = levPDFStockCode.indexOf(strCode);
//        if(idx != -1) {
//            int price = abs(kiwoom->GetCommData(sTrCode, sRQName, 0, QStringLiteral("현재가")).toInt());
//            levPDFStockBalance[idx] = levPDFStockQty.at(idx) * price;
//            levBalance += levPDFStockBalance[idx];
//        }
//        else if(strCode == levPDFFutureCode) {
//            float increase = kiwoom->GetCommData(sTrCode, sRQName, 0, QStringLiteral("전일대비")).toFloat();
//            levPDFFutureBalance = increase * levPDFFutureQty * 10000;
//            levBalance +=  levPDFFutureBalance;
//        }

//        levNAV = levBalance / 100000.0;
//        qDebug() << "levNAV" << levNAV;
//    }
//    else if(RQ1 == QStringLiteral("invETF")) {
//        QString strCode = RQ2;
//        if(strCode == invPDFFutureCode) {
//            float increase = kiwoom->GetCommData(sTrCode, sRQName, 0, QStringLiteral("전일대비")).toFloat();
//            invPDFFutureBalance = increase * invPDFFutureQty * 10000;
//            invBalance +=  invPDFFutureBalance;

//            invNAV = invBalance / 100000.0;
//            emit invNAVReady(invNAV);
//            emit levNAVReady(levNAV);
//            qDebug() << "invNAV" << invNAV;
//        }

//    }
}

void ETF::on_real_data_received(const QString &sCode, const QString &sRealType, const QString &sRealData)
{
    if(sRealType == QStringLiteral("선물시세")) {

        QVariant increase = kiwoom->GetCommRealData(sRealType, 10);
        qDebug() << increase;
        qDebug() << sCode;

        levBalance = levBalance - levPDFFutureBalance;
        //levPDFFutureBalance = increase * levPDFFutureQty * 10000;
        levBalance +=  levPDFFutureBalance;
        levNAV = levBalance / 100000.0;

        invBalance = invBalance - invPDFFutureBalance;
       // invPDFFutureBalance = increase * invPDFFutureQty * 10000;
        invBalance +=  invPDFFutureBalance;
        invNAV = invBalance / 100000.0;

        emit invNAVReady(invNAV);
        emit levNAVReady(levNAV);
    }

    else if(sRealType == QStringLiteral("주식시세") || sRealType == QStringLiteral("주식체결")) {

        int idx = levPDFStockCode.indexOf(sCode);
        if(idx != -1) {
            int price = abs(kiwoom->GetCommRealData(sRealType, 10).toInt());
            levBalance = levBalance - levPDFStockBalance.at(idx);
            levPDFStockBalance[idx] = levPDFStockQty.at(idx) * price;
            levBalance += levPDFStockBalance[idx];

            levNAV = levBalance / 100000.0;
            emit levNAVReady(levNAV);
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
            emit marketOpen();
            qDebug() << QStringLiteral("장시작");
        }
        else if(market_hour_flag == QStringLiteral("2"))
        {
            if(min_remain == QStringLiteral("001000")) {
                emit marketClose();
                qDebug() << "장종료 10분전";
            }
        }
        else if(market_hour_flag == QStringLiteral("4"))
        {
        }
    }
}
