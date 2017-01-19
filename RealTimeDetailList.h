#if !defined(AFX_REALTIMEDETAILLIST_H__D99F64A3_B550_11D4_882E_00400552E583__INCLUDED_)
#define AFX_REALTIMEDETAILLIST_H__D99F64A3_B550_11D4_882E_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealTimeDetailList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRealTimeDetailList dialog

class CRealTimeDetailList : public CDialog
{
// Construction

public:
    CRealTimeDetailList(CWnd* pParent /*=NULL*/,CTaiShanKlineShowView *m_pVwbaseView,BOOL IsF1);
    CDetailGridCtrl *m_pGrid;
 	int     m_nCols; //the total num of column, if user delete or add a column , it will ++ / --
	int		m_nRows; //the total num of rows.
	int		m_nFixCols;   //固定列
	int		m_nFixRows;   //固定行
	int     m_nBigBillCount; //大单明细数
	int     m_nRealRow;     //大单明细数实际行

	BOOL	m_bEditable;   //是否可以编辑
	BOOL	m_bListMode;   //是否为显示模式
	BOOL	m_bSelectable; //是否为多选 
	BOOL	m_bReadOnly;   //是否为只读
	CRect   m_rcBar;
	int     m_nStartColumn;  //开始列
	int     m_nVisibleRowCount;  //可视部分行数
	CStringArray m_ColumnTitleArray; //define an array to keep the column title string
	CCellID m_CellTitleCommand;        //
	CCellRange VisibleCells;
    int m_VisibleRowCount;
    CTaiShanDoc *m_pDoc;
	CTaiShanKlineShowView *m_pView;

    BOOL m_IsF1;
	BOOL m_bFirst;
	BOOL m_bFirstDisplay;

	void InitGridColumnTitle();     //初始化GridCtrl列名
    int InitGridCtrl();   	//初始化GridCtrl
	BOOL InitGridContent();  //初始化Grid
    void Show_1Min(BOOL IsFirstDisplay=FALSE);
    void Show_5Min(BOOL IsFirstDisplay=FALSE);
// Dialog Data
	//{{AFX_DATA(CRealTimeDetailList)
	enum { IDD = IDD_DETAIL_HS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealTimeDetailList)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRealTimeDetailList)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALTIMEDETAILLIST_H__D99F64A3_B550_11D4_882E_00400552E583__INCLUDED_)
