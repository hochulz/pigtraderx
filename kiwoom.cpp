#include "kiwoom.h"

Kiwoom::Kiwoom(QWidget *parent) : QAxWidget(parent)
{
    setControl("{A1574A0D-6BFA-4BD7-9020-DED88711818D}");
}

void Kiwoom::CommConnect()
{
    dynamicCall("CommConnect()");
}

QVariant Kiwoom::GetCommData(const QString &strTrCode, const QString &strRecordName, int nIndex, const QString &strItemName)
{
    QVariant ret = dynamicCall("GetCommData(QString, QString, int, QString)", strTrCode, strRecordName, nIndex, strItemName);
    return ret;
}

QVariant Kiwoom::GetCommRealData(const QString &strRealType, int nFid)
{
    QVariant ret = dynamicCall("GetCommRealData(QString, int)", strRealType, nFid);
    return ret;
}

QString Kiwoom::GetCodeListByMarket(const QString &market)
{
    QString ret = dynamicCall("GetCodeListByMarket(QString)", market).toString();
    return ret;
}

QString Kiwoom::GetMasterCodeName(const QString &strCode)
{
    QString ret = dynamicCall("GetMasterCodeName(QString)", strCode).toString();
    return ret;
}

int Kiwoom::GetMasterLastPrice(const QString &strCode)
{
    int ret = dynamicCall("GetMasterLastPrice(QString)", strCode).toInt();
    return ret;
}

void Kiwoom::SetInputValue(const QString &sID, const QString &sValue)
{
    dynamicCall("SetInputValue(QString, QString)", sID, sValue);
}

int Kiwoom::CommRqData(const QString &sRQName, const QString &sTRCode, int nPrevNext, const QString &sScreenNo)
{
    int ret = dynamicCall("CommRqData(QString, QString, int, QString)", sRQName, sTRCode, nPrevNext, sScreenNo).toInt();
    return ret;
}

int Kiwoom::GetRepeatCnt(const QString &sTrCode, const QString &sRecordName)
{
    int ret = dynamicCall("GetRepeatCnt(QString, QString)", sTrCode, sRecordName).toInt();
    return ret;
}

int Kiwoom::CommKwRqData(const QString &sArrCode, int bNext, int nCodeCount, int nTypeFlag, const QString &sRQName, const QString &sScreenNo)
{
    int ret = dynamicCall("CommKwRqData(QString, int, int, int, QString, QString)", sArrCode, bNext, nCodeCount, nTypeFlag, sRQName, sScreenNo).toInt();
    return ret;
}

int Kiwoom::SetRealReg(const QString &strScreenNo, const QString &strCodeList, const QString &strFidList, const QString &strOptType)
{
    int ret = dynamicCall("SetRealReg(QString, QString, QString, QString)", strScreenNo, strCodeList, strFidList, strOptType).toInt();
    return ret;
}
