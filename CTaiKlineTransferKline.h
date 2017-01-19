#if !defined(AFX_LMBTRANSFERKLINE_H__92CDDF41_D7BE_11D3_96E6_0080C8E1242B__INCLUDED_)
#define AFX_LMBTRANSFERKLINE_H__92CDDF41_D7BE_11D3_96E6_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineTransferKline.h : header file
//

#include "StructKlineView.h"


/////////////////////////////////////////////////////////////////////////////
// CTaiKlineTransferKline command target

class CTaiShanDoc;
class CTaiKlineTransferKline : public CCmdTarget
{
	DECLARE_DYNCREATE(CTaiKlineTransferKline)

// Attributes
public:
	bool m_bCalPowerToday;
	CReportData* m_pReportData;	//个股的数据指针
	CString m_symbolOld;
	int m_stkKindOld;
	bool m_bUseMinute1Data;
	Kline m_klineOld;
	CTaiShanDoc * pDoc;
	CTaiKlineTransferKline();           //  constructor 
	CTaiKlineTransferKline(CBuySellList* pBSList);           //  constructor 

	virtual ~CTaiKlineTransferKline();
// Operations
public:
	static float GetVolHs(TRADE_DETAIL_H_PER* pHs,TRADE_DETAIL_H_PER* pHsPre,int& nFlag);
	static void ComputePowerToday(Kline *pKlineToday,int nKline,powerArr &splitArr);
	static void ComputePower(Kline *pKlineHistory, int nKlineInOut, int nTypeTarget, powerArr& splitArr);
	//功能:计算除权
	//输入参数: pKlineHistory是k线数据指针, nKlineInOut是k线数据个数,  nTypeTarget是时间周期类型, splitArr是除权数据数组
	static int ReadPower(CTaiShanDoc * pDoc,CString symbol,int stkKind,powerArr& splitArr);
	//功能:计算除权
	//输入参数: symbol股票代码,
	//输出参数:splitArr是除权数据数组,返回数据个数
	int OneTo5(Kline *pKline,int nEach,CString symbol,int stkKind);
	//功能:转换周期数据
	//输入参数: pKline是k线数据指针, symbol股票代码,nEach=1是转换为1*5分钟k线
	//输出参数:pKline也是输出的k线数据指针
	int TransferToday(bool bAnother,CString symbol,int stkKind,Kline* pKline,int nKline);
	//功能:计算当天数据
    //输入说明：symbol为股票代码，pKline为K线数据指针，nKline为输入的K线数据个数，bAnother为是否是另加一个数据。
    //输出说明：返k线数据个数。
	void InPutValue(Kline* pKline,int i, int& j, bool bFrst,int& nCountDay,int nKindStock = 2,bool isMinute1 = false);
	//功能:转换k线数据
    //输入说明：pKline为K线数据指针
	bool TransferKlineToday(CString symbol,int stkKind,Kline*& pKline,int& nKlineInOut,int nTypeTarget);
	bool TransferKlineHistory(CString symbol,int stkKind,Kline*& pKlineHistory,int& nKlineInOut,int nTypeTarget,bool bToday = true,bool bSplit = false);
    //输入说明：symbol为股票代码，pKlineHistory为历史K线数据指针，nKlineInOut为输入的K线数据个数，nTypeTarget为目标周期类型，bToday为是否转换今日数据，bSplit是否对数据进行除权。
    //输出说明：nKlineInOut为输出的K线数据个数，pKlineHistory为输出的K线数据指针。

	int CaclKline1Minu(Kline*& pKline,CString name,int stkKind, int nMinu);
	//功能:转换k线数据
    //输入说明：pKline为K线数据指针,name为股票代码，nMinu几分钟(转换的目标周期)
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineTransferKline)
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBuySellList* pBuySellList; 
	bool m_bRemoveBuySellList;

	int CaclKline1MinuIndex(Kline*& pKline,CString name,int stkKind, int nMinu);

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineTransferKline)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	static float CalcPowerAfterPer(float fIn, POWER &pwr);
	static float CalcPowerPer(float fIn,POWER& pwr);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBTRANSFERKLINE_H__92CDDF41_D7BE_11D3_96E6_0080C8E1242B__INCLUDED_)
