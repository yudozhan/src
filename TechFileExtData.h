// TechFileExtData.h: interface for the CTechFileExtData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TECHFILEEXTDATA_H__2FDB8D41_A8AB_11D6_968F_84E6EFA90B76__INCLUDED_)
#define AFX_TECHFILEEXTDATA_H__2FDB8D41_A8AB_11D6_968F_84E6EFA90B76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TechMemFile.h"
#include "IndicatorExtendData.h"
#define MAX_BLOCK_USE 26
typedef struct _KLINE_SMALL_HEAD2
{
	char    StockSign[8]; //前8个字节为股票代码。
	int    numKln	;	  //4个字节整型，代表日K线的个数。
	WORD   symBlock[MAX_BLOCK_USE]; //下面每两个字节代表K线数据的块号
}KLINE_SMALLHEAD;

class CTechFileExtData : public CTechMemFile  
{
public:
	CTechFileExtData();
	virtual ~CTechFileExtData();
	static int WriteIn(RecordIndicator* pData,CString symbol,int nIndicatorExtendID,int nCount) ;
	static int ReadOut(RecordIndicator*& pData,CString symbol,int nIndicatorExtendID,int nDays);//nDays ==-1 is total

	BOOL Open(int nIndicatorExtendID, UINT nOpenFlags,int nAddToFileEnd =0 ,CFileException* pException = NULL);
	static void CloseNow();
	static CTechFileExtData g_fileExtData;

	CString GetSymbol(int iStock);
private:
	int WriteData(RecordIndicator* pData,CString symbol,int nIndicatorExtendID,int nCount) ;
	int ReadData(RecordIndicator*& pData,CString symbol,int nIndicatorExtendID,int nDays);//nDays ==-1 is total
	void LookUpIndexFromMap(CString symbol,int& nIndex);
	void CreateSmallBlock();//when a new file created, create the small block. file length is 16+64*4096
	int GetKlineSmallHeader(CString symbol,KLINE_SMALLHEAD* pKlineSmallHead,bool bWrite = true);//get the small header pointer of a stock
	bool SetKlineSmallHeader(int nIndex,KLINE_SMALLHEAD* pKlineSmallHead);
//	void AddSmallHeadBlock();
	void DeleteMap();

	static bool CheckSymbol(CString symbol);
	void AddIndexToMap();
	int AddNewStockToFile(CString symbol,KLINE_SMALLHEAD*& pKlineSmallHead);
	void WriteHeaderInfo();
	int CreateOrMoveSmallBlock(KLINE_SMALLHEAD* pKlineSmallHead,int& nBlock);
	int CalcMaxCountBlock();

private:
	static int m_nIDPre;
	bool m_bDay;
};

#endif // !defined(AFX_TECHFILEEXTDATA_H__2FDB8D41_A8AB_11D6_968F_84E6EFA90B76__INCLUDED_)
