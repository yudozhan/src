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
    HANDLE m_hFile,m_hFileMap;                  //内存映射文件句柄
	BIG_LISTHEAD *m_pBigBillHead;                    //内存映射文件头部结构  
	BYTE  *m_pbData;                          //内存映射文件首部指针
	BIG_LIST  *m_pMapData;                        //分笔成交数据          
	
public:
	BOOL InitRealTimeHsData();              //初始化实时分笔数据
    BOOL InitRealTimeHsDataEmpty();
    BOOL InitRealTimeHsDataExist();

	BOOL AddRealTimeHsData(PBIG_LIST & pCjHs);  //增加实时分笔数据
	BOOL ClearRealTimeHsData();              //清空实时分笔数据
	BOOL SetBigDataForSetup(float *pBigData,float *pBigMaxData,float *CjmxType);  //以股数设置大单数据
	BOOL SetBigDataForSetupGB(float *pBigData,float *pBigMaxData,float *CjmxType); //以固本结构设置大单数据
	BOOL CalcBigBillData(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport);  //统计成交数据
	BOOL CalcBigBillDataForType(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport); //以股数计算各分类成交数据
    BOOL CalcBigBillBigData(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport);  //以股数计算大单成交数据
	BOOL CalcBigBillDataForTypeGB(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport,float GB); //以股本计算各分类成交数据
    BOOL CalcBigBillBigDataGB(CReportData *pCdat,RCV_REPORT_STRUCTEx* m_GpHqReport,float GB); //以股本计算大单成交数据
	void CalcRealTimeHsBigData(BIG_LIST *pCjHs);    //统计实时分笔明细大单数据
	BOOL GetBigBillData(int index,PBIG_LIST &pCjmx);   //取分笔明细大单数据
	void GetParamData(float *pBigData,float *pBigMaxData,float *CjmxType);    //取统计大单设置参数
    void GetParamDataGB(float *pBigData,float *pBigMaxData,float *CjmxType);  // 取统计大单设置参数（股本结构）
	void SetIsCalcForBig(BOOL *IsCalc);	//设置该明细是否为大单元 
	void SetBigBillBigType(BOOL IsGB){m_pBigBillHead->IsGbjg=IsGB;}


	BOOL CalcBigBillHistoryData(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData,int nTotalCount);  //统计成交数据
    void CalcBigBillHistoryDataForType(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1);
    void CalcBigBillBigHistoryData(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1);
	void CalcBigBillHistoryDataForTypeGB(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1,float GB);
	void CalcBigBillBigHistoryDataGB(CReportData *pCdat,TRADE_DETAIL_BIG_LIST_PER *pBigBillData2,TRADE_DETAIL_BIG_LIST_PER *pBigBillData1,float GB);


};

#endif // !defined(AFX_STOCKCJMXDATA_H__BE205541_27DE_11D4_8829_00400552E583__INCLUDED_)
