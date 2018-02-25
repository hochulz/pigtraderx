#ifndef KIWOOM_H
#define KIWOOM_H
#include <QAxWidget>

class Kiwoom : public QAxWidget
{
public:
    explicit Kiwoom(QWidget *parent = 0);

    void CommConnect();
    QVariant GetCommRealData(const QString &strRealType, int nFid);
    QString GetCodeListByMarket(const QString &market);
    QString GetMasterCodeName(const QString &strCode);
    int GetMasterLastPrice(const QString &strCode);
    void SetInputValue(const QString &sID, const QString &sValue);
    int GetRepeatCnt(const QString &sTrCode, const QString &sRecordName);
    int CommKwRqData(const QString &sArrCode, int bNext, int nCodeCount, int nTypeFlag, const QString &sRQName, const QString &sScreenNo);
    int CommRqData(const QString &sRQName, const QString &sTRCode, int nPrevNext, const QString &sScreenNo);
    QVariant GetCommData(const QString &strTrCode, const QString &strRecordName, int nIndex, const QString &strItemName);
    int SetRealReg(const QString &strScreenNo, const QString &strCodeList, const QString &strFidList, const QString &strOptType);

};

#endif // KIWOOM_H
