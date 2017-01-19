// StockBigBillData.h: interface for the CSharesBigBillData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOCKCJMXDATA_H__BE205541_27DE_11D4_8829_00400552E583__INCLUDED_)
#define AFX_STOCKCJMXDATA_H__BE205541_27DE_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSharesBigBillData  
{
public:
	CSharesBigBillData();
	virtual ~CSharesBigBillData();
    HANDLE m_hFile,m_hFileMap;                  //�ڴ�ӳ���ļ����
	BIG_LISTHEAD *m_pBigBillHead;                    //�ڴ�ӳ���ļ�ͷ���ṹ  
	BYTE  *m_pbData;                          //�ڴ�ӳ���ļ��ײ�ָ��
	BIG_LIST  *m_pMapData;                        //�ֱʳɽ�����          
	
public:
	BOOL InitRealTimeHsData();              //��ʼ��ʵʱ�ֱ�����
    BOOL InitRealTimeHsDataEmpty();
    BOOL InitRealTimeHsDataExist();

	BOOL AddRealTimeHsData(PBIG_LIST & pCjHs);  //����ʵʱ�ֱ�����
	BOOL ClearRealTimeHsData();              //���ʵʱ�ֱ�����
	BOOL SetBigDataForSetup(float *pBigData,float *pBigMaxData,float *CjmxType);  //�Թ������ô�����
	BOOL SetBigDataForSetupGB(float *pBigData,float *pBigMaxData,float *CjmxType); //�Թ̱��ṹ���ô�����
	BOOL CalcBigBillData(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport);  //ͳ�Ƴɽ�����
	BOOL CalcBigBillDataForType(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport); //�Թ������������ɽ�����
    BOOL CalcBigBillBigData(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport);  //�Թ�������󵥳ɽ�����
	BOOL CalcBigBillDataForTypeGB(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport,float GB); //�Թɱ����������ɽ�����
    BOOL CalcBigBillBigDataGB(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport,float GB); //�Թɱ�����󵥳ɽ�����
	void CalcRealTimeHsBigData(BIG_LIST *pCjHs);    //ͳ��ʵʱ�ֱ���ϸ������
	BOOL GetBigBillData(int index,PBIG_LIST &pCjmx);   //ȡ�ֱ���ϸ������
	void GetParamData(float *pBigData,float *pBigMaxData,float *CjmxType);    //ȡͳ�ƴ����ò���
    void GetParamDataGB(float *pBigData,float *pBigMaxData,float *CjmxType);  // ȡͳ�ƴ����ò������ɱ��ṹ��
	void SetIsCalcForBig(BOOL *IsCalc);	//���ø���ϸ�Ƿ�Ϊ��Ԫ 
	void SetBigBillBigType(BOOL IsGB){m_pBigBillHead->IsGbjg=IsGB;}


	BOOL CalcBigBillHistoryData(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData,int nTotalCount);  //ͳ�Ƴɽ�����
    void CalcBigBillHistoryDataForType(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1);
    void CalcBigBillBigHistoryData(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1);
	void CalcBigBillHistoryDataForTypeGB(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1,float GB);
	void CalcBigBillBigHistoryDataGB(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1,float GB);


};

#endif // !defined(AFX_STOCKCJMXDATA_H__BE205541_27DE_11D4_8829_00400552E583__INCLUDED_)
