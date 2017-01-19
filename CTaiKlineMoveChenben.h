#if !defined(AFX_LMBMOVECHENBEN_H__45E004E6_9E5E_11D3_96DF_0080C8E1242B__INCLUDED_)
#define AFX_LMBMOVECHENBEN_H__45E004E6_9E5E_11D3_96DF_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineMoveChenben.h : header file
//
#include "BjChenBenSetting.h"



/////////////////////////////////////////////////////////////////////////////
// CTaiKlineMoveChenben command target
class CTaiShanDoc;//CTaiShanDoc
class CTaiShanKlineShowView;
class CTaiKlineShowKline;

class CTaiKlineMoveChenben : public CObject
{
	DECLARE_DYNCREATE(CTaiKlineMoveChenben)

	CTaiKlineMoveChenben();           // protected constructor used by dynamic creation

// Attributes
public:
	//lmb 200303 b

	CArray<BjChenBenSetting,BjChenBenSetting&> m_chengBenBeforArr;
	CArray<BjChenBenSetting,BjChenBenSetting&> m_chengBenInsideArr;
	void WriteOutSettings();
	void ReadInSettings();

	//lmb 200303 e
	static void GetMoveCBPerFoot2(Kline* pKline,float* cnp,float ltp,float fMax,float fMin,int nFoot ,float fRate,int n,float fRuler);
	
	int			m_nShowKind;
	CString		m_symbol;
	int			m_stkKind;
	bool		m_bDayKline;//是否使用日线数据进行计算
	Kline*		m_pKlineOther;
	int			m_numkline;
	CTaiShanDoc* pDoc ;		//CTaiShanDoc的指针
	CTaiShanKlineShowView* pView ;		//CTaiShanKlineShowView的指针
	CTaiKlineShowKline*		pDoK ;
	CTaiKlineMoveChenben(CTaiKlineShowKline* pDoKline,CTaiShanKlineShowView* pViewIn,CTaiShanDoc* pDocIn);           // protected constructor used by dynamic creation

	virtual ~CTaiKlineMoveChenben();
// Operations
public:
	static int GetRuler(float fMax, float fMin,float& fRuler);
	static void GetMoveCBPerFoot(Kline* pKline,float* cnp,float ltp,float fMax,float fMin,int nFoot = 0,float fRate=1);
	static void GetMaxMin(Kline* pKline,int nKline,float& fMax,float& fMin);
	
	void ShowFxjMoveCB(CDC *pDC);
	void ShowCommerceCB(CDC* pDC,int nShowVirtualLine = 0);
	void BuySellFiguer(CDC* pDC);
	void ShowMoveCB(CDC* pDC,int nShowVirtualLine = 0);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineMoveChenben)
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ShowVol(CDC *pDC,int nShowVirtualLine);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBMOVECHENBEN_H__45E004E6_9E5E_11D3_96DF_0080C8E1242B__INCLUDED_)
