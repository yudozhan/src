// CTaiKlineFileHS.h: interface for the CTaiKlineFileHS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LMBFILEHS_H__5655C041_C739_11D3_96E5_0080C8E1242B__INCLUDED_)
#define AFX_LMBFILEHS_H__5655C041_C739_11D3_96E5_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CTaiKlineMemFile.h"
#include "StructKlineView.h"
struct HSSMALLHEAD//size 64
{
	char   StockSign[8];	//ǰ8���ֽ�Ϊ��Ʊ���롣
	int    numHS;			//4���ֽ����ͣ����������
	int    numReserved;		//4���ֽ����͡�
	WORD   symBlock[16];	//����ÿ�����ֽڴ���K�����ݵĿ��
};
class	CTaiShanDoc;
class   CProgressDialog;
class CTaiKlineFileHS : public CTaiKlineMemFile  
{
public:
	static void Symbol4To6();
	static int GetDataCountAll(CString symbol,int stkKind);
	static CTaiKlineFileHS* GetFilePointer2(int nMarket);
	int WriteHsToFile(CString symbol, RCV_DISPBARGAINING_STRUCTEx *pData, int nRecord);
	static void WriteHsToFileWideNet(CString symbol,int nMarket, CString sDate,RCV_DISPBARGAINING_STRUCTEx *pData,  int nRecord,int nRequest);
	static bool WriteHsDataWideNet(RCV_DISPBARGAINING_STRUCTEx* pData,int nPacketNum,int nRequest);//for qyp
	static bool IsNeedHsHistData(CString symbol,int stkKind,CString sDate);
	static CTaiKlineFileHS*	m_pFileHsSz;
	static CTaiKlineFileHS*	m_pFileHsSh;
	static CTaiKlineFileHS*	m_fileHsSzBak;
	static CTaiKlineFileHS*	m_fileHsShBak;

	static int GetCountPre(CString symbol,int stkKind);
	static void CloseAll();
	static bool OpenAll();
	void DeleteMap();
	static int GetMinuteSecond(int timet);
	bool m_bToday ;
	static CTaiKlineFileHS* BillOpenFileHs(CString strTime,bool bShangHai=true,bool bToday=false);
	static bool BillTransferDataEx(CTaiKlineFileHS* pFileHs,char *pStockCode, CBuySellList *pBuySellList, KlineEx *&pKlineEx);
	static bool TransferDataEx(CBuySellList* pBuySellList,KlineEx*& pKlineEx);
	static CTaiKlineFileHS* GetFilePointer(CString symbol,int stkKind);
	static void DoCloseWorkHs(CProgressDialog* pDlg = NULL);
	bool WriteHS(HSSMALLHEAD* pHsSmallHead,TRADE_DETAIL_H_PER* pHs);
	static TRADE_DETAIL_H_PER* GetAtBS(CBuySellList *pBuySellList,int nFoot,int nCount);
	void HsToCdat1(CReportData *pCdat, Kline* pKline,int nFoot,CBuySellList* pBuySellList);
	enum{
		BUYVOL=0,SELLVOL=1,ISBUYORDER=2,
		ASKPRICE=3,ASKVOL=4,BIDPRICE=5,BIDVOL=6
	};
	static void TransferHs(CBuySellList* pBuySellList,ARRAY_BE& pp,int nMax,int nKindIn,int nOther, KlineEx *pKlineEx = NULL);
	void ZeroHsCountEach();
	static int ComputeDaysCount(CBuySellList* buySellList);
	int GetDaysCount();
	void SetDaysCount(int nDays);
	int GetDataCount(CString symbol);
	static void RemoveHs(CBuySellList& buySellList);
	bool WriteHS2(CString symbol,CBuySellList& buySellList);
	int ReadHS2(CString symbol,CBuySellList& buySellList,bool bClearAll);
	CTaiShanDoc * pDoc;
	static void Cdat1ToHs(CReportData* pCdat,TRADE_DETAIL_H_PER* pHs,bool bToHs = true, TRADE_DETAIL_H_PER *pHsPre = NULL);
	int GetHsSmallHeader(CString symbol,HSSMALLHEAD* pHsSmallHead);
	int ReadHS(CString symbol,CBuySellList& buySellList,bool bClearAll = true);	//��ȡ����
	int ReadHSPeriod(CString symbol, CBuySellList& buySellList, CTime timeStart, CTime timeEnd,bool bClearAll = true);//��ȡһ��ʱ���K������
//	bool WriteHS(CString symbol);	//д���� 
	bool WriteHS(CReportData* pCdat,bool bFirstOne = false);	//дK������ CReportData
	void ClearHS();	//set the count of each stock to zero,initialate
	static int TransferHsToMin1(CBuySellList& buySellList,TRADE_DETAIL_H_PER* pHs,int nTotal = 240);//transfer hs to 1 minute line

	bool SetHsSmallHeader(int nIndex,HSSMALLHEAD* pHsSmallHead);
//	int GetHsSmallHeader(CString symbol,HSSMALLHEAD* pHsSmallHead);//get the small header pointer of a stock
										//use the small header pointer to get
	CTaiKlineFileHS();
	CTaiKlineFileHS(int nMarket,bool bToday = true);
	virtual ~CTaiKlineFileHS();
	virtual BOOL Open(LPCTSTR lpszFileName, UINT nOpenFlags,int nAddToFileEnd =0 ,CFileException* pException = NULL);
private:
	void AddSmallHeadBlock();
	int ReadIndexHS(CString symbol,CBuySellList& buySellList,bool bClearAll);
	static void WriteHistoryHsFile(bool bSh,CProgressDialog* pDlg = NULL);
	void AddIndexToMap();
	void AddNewStockToFile(CString symbol,HSSMALLHEAD*& pHsSmallHead);
	void WriteHeaderInfo();
	int CreateOrMoveSmallBlock(HSSMALLHEAD* pHsSmallHead,int& nBlock);
//	int CalcMaxCountBlock();
	int m_nMarket;//the kind of Market

};

#endif // !defined(AFX_LMBFILEHS_H__5655C041_C739_11D3_96E5_0080C8E1242B__INCLUDED_)
