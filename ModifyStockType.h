#if !defined(AFX_MODIFYSTOCKTYPE_H__BD1C43A2_5D50_11D4_8829_00400552E583__INCLUDED_)
#define AFX_MODIFYSTOCKTYPE_H__BD1C43A2_5D50_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyStockType.h : header file
//
#define WM_USER_ADDSTOCK WM_USER+1200
/////////////////////////////////////////////////////////////////////////////
// CModifyStockType dialog

class CModifyStockType : public CDialog
{
// Construction
public:
	CModifyStockType(CWnd* pParent = NULL);   // standard constructor
	CModifyStockType(PBLOCKINFO &pBlockInfo,BOOL IsAddStockType=TRUE,CWnd* pParent = NULL);   // standard constructor
	enum {ZGB=0,LTG=1,OTHER=2	 };

public:
	CImageList * m_pImageList;    //大图象列表
	CImageList * m_pImageListSmall; //小图象列表
    CTaiShanDoc *m_pDoc;
	BLOCKINFO *m_pBlockInfo;
	BOOL m_bIsAddNewStockType;
	float m_fTotalRight;
	int  m_nTotalCount;
	BOOL m_bIsChanged;
	BOOL m_bIsCalc;

public:
    void FillToListCtrlHead();
    void FillToListCtrlItem(CReportData *pCdat);
	void RefreshShowData();
	void CheckStockCount();

	


// Dialog Data
	//{{AFX_DATA(CModifyStockType)
	enum { IDD = IDD_MODIFY_STOCKTYPE };
	CButton	m_ok;
	CButton	m_cancel;
	CButton	m_refresh;
	CButton	m_delete;
	CButton	m_add;
	CListCtrl	m_lstStockTypeName;
	CString m_sStockTypeCode;
	CString m_sStockTypeName;
	CProgressCtrl	m_Progress;
	int  m_nRightType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModifyStockType)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CModifyStockType)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddStock();
	afx_msg void OnDeleteStock();
	afx_msg void OnChangeStockTypeRight();
	afx_msg void OnUpdateOk();
	afx_msg void OnRefreshHistoryData();
	afx_msg void OnChangedName();




	//}}AFX_MSG
    afx_msg LRESULT OnAddStockCode(WPARAM wParam, LPARAM lParam); //增加股票至LISTCTRL


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnBnClickedDelall();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYSTOCKTYPE_H__BD1C43A2_5D50_11D4_8829_00400552E583__INCLUDED_)
