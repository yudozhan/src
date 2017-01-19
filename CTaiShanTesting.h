// CTaiShanTesting.h: interface for the CTaiShanTesting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HJYPINGCE_H__90C60EC1_4989_42EA_B2CF_6DF20D821747__INCLUDED_)
#define AFX_HJYPINGCE_H__90C60EC1_4989_42EA_B2CF_6DF20D821747__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaiShanDoc;
class CTaiShanKlineShowView;
class CTaiShanTesting  
{
private:
	CTaiShanDoc* m_pDoc;
	BOOL m_bRefresh;		//是否需要重新评测，FALSE:不需要，TRUE:需要
	BOOL m_bHaveFormula;
public:
	CString m_strStockID;	//当前股票代码
	CArray<IndexDataInfo,IndexDataInfo> m_strHjyChooseFormula;	//选股公式
	CArray<IndexDataInfo,IndexDataInfo> m_strChooseFormu;	//选股公式
	
	CString m_strResultData2;		//选股结果
	CString m_strResultData;		//选股结果
public:
	CTaiShanTesting();		//构造函数
	CTaiShanTesting(CTaiShanDoc* pDoc);	//构造函数
	void SetStock(CString strStockID=_T(""));	//设置股票代号
	void GetChooseFormula(CTaiShanKlineShowView* pView,BOOL bRefresh=FALSE);	//得到选股公式
	void SelectStock(CTaiShanKlineShowView* pView,BOOL bRefresh=FALSE);		//进行选股，得到选股结果
	void OnDraw(CTaiTestRichEditCtrl* pRich, COLORREF crBack, COLORREF crText, COLORREF crTitle,CRect &rc);	//画出结果
	virtual ~CTaiShanTesting();		//析构函数
};

#endif // !defined(AFX_HJYPINGCE_H__90C60EC1_4989_42EA_B2CF_6DF20D821747__INCLUDED_)
