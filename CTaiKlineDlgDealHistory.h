#if !defined(AFX_LMBDLGDEALHISTORY_H__A3A59948_FF03_11D3_96E9_0080C8E1242B__INCLUDED_)
#define AFX_LMBDLGDEALHISTORY_H__A3A59948_FF03_11D3_96E9_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgDealHistory.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgDealHistory dialog
class CTaiKlineFileHS;
class CTaiKlineMin1;
class CTaiShanKlineShowView;
class CTaiKlineDlgDealHistory : public CDialog
{
// Construction
public:
	void SetCheck1(int n);
	//功能:字符串转换为时间
    //输入说明：tm表示时间，s表示字符串
	static void StringToTime(CTime& tm,CString s);
	//功能:把buySellList中nFoot项的数据转换到pDat中
    //输入说明：symbol为股票代码，pDat为实时行情数据存储结构，nFoot为链表buySellList的下标，buySellList是存放换手数据的链表，bIndex为是否是指数
	//输出说明：返回成功与否
	static bool GetBuySellInfoPerFoot(CString symbol,CReportData* pDat, int nFoot, CBuySellList& buySellList, bool bIndex);
	//功能:把buySellList中nFoot项的数据转换到pDat中
    //输入说明：symbol为股票代码，pDat为实时行情数据存储结构，nFoot为链表buySellList的下标，buySellList是存放换手数据的链表，bIndex为是否是指数
	//输出说明：返回成功与否
	bool GetBuySellInfo(CString symbol,int stkKind,CReportData* pDat,int nFoot, CBuySellList& buySellList,bool bIndex);
//	bool GetBuySellInfoMin1(CString symbol,CReportData* pDat, int nFoot, CBuySellList& buySellList, bool bIndex);
	//功能:历史回忆同步
    //输入说明：time寻找历史回忆数据所依据的时间
	//输出说明：返回成功与否
	bool HistoryAStep(int time);
//	static CTaiKlineDlgDealHistory* pDlgDealHistory;
	static bool HistoryOpen(CTaiShanKlineShowView *pWnd,CString * fileName = NULL);
	//功能:打开历史回忆文件
	void OpenFile();
	//功能:改变时间操作
    //输入说明：bGoHead = true 表示转换到更早的时间，bDoReal表示是否真的更改数据
	//输出说明：返回成功与否
	bool OnDoGo(bool bGoHead = true,bool bDoReal = true);
	bool InitDoFenshi(CTaiKlineMin1* pdoFen,CString symbol,int stkKind);
	//功能:初始化历史回忆数据
    //输入说明：pdoFen分时走势对象指针，symbol为股票代码
	//输出说明：返回成功与否

	CTaiShanKlineShowView * pView;
	CReportData m_ReportDataSh;	//上证指数数据
	CReportData m_ReportDataSz;	//深证指数数据
	Tidxd* m_Tidx[2];
	Rsdn1* m_Nidx[2];

	CString m_fileName;
	CString m_oldSymbol;
	int m_oldStkKind;
	CTaiKlineFileHS*	m_pFileHsSh;
	CTaiKlineFileHS*	m_pFileHsSz;
	bool	m_bShowHistory;
	bool	m_bHistorySynchrostep;

	CTaiKlineDlgDealHistory(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgDealHistory)
	enum { IDD = IDD_DEAL_HISTORY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgDealHistory)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnToolTipNotify(UINT id,NMHDR *pTTTStruct,LRESULT *pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDlgDealHistory)
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnHistoryHide();
	afx_msg void OnHistoryGohead();
	afx_msg void OnHistoryGoback();
	afx_msg void OnHistoryClose();
	afx_msg void OnHistoryAStep();
	afx_msg void OnHistorySwitch();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CToolBar    m_wndToolBar;
	CToolTipCtrl m_wndToolTip; 
	CTaiKlineFileHS* GetFilePointer (CString symbol,int stkKind);
	//功能:转换历史回忆数据
    //输入说明：pdoFen分时走势对象指针，symbol为股票代码，pDat为实时行情数据存储结构，bIndex为是否是指数
	//输出说明：返回成功与否
	void GetCdat1Data(CString symbol,int stkKind, CReportData* pDat,bool bIndex = true,CTaiKlineMin1 *pdoFen = NULL);
	//功能:历史回忆数据改变时作有关处理
	void OnFileChanged();
	//功能:转换历史回忆数据
    //输入说明：pdoFen分时走势对象指针，symbol为股票代码
	//输出说明：返回成功与否
	void PrepareData(CTaiKlineMin1 *pdoFen,CString symbol,int stkKind);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBDLGDEALHISTORY_H__A3A59948_FF03_11D3_96E9_0080C8E1242B__INCLUDED_)
