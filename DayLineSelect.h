#if !defined(AFX_DAYLINESELECT_H__B46517A2_88B1_11D4_882A_00400552E583__INCLUDED_)
#define AFX_DAYLINESELECT_H__B46517A2_88B1_11D4_882A_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DayLineSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDayLineSelect dialog

class CDayLineSelect : public CDialog
{
// Construction
public:
	CImageList * m_pImageListSmall; //小图象列表

	CDayLineSelect(CWnd* pParent = NULL);   // standard constructor
	CDayLineSelect(UINT uiSelectDayLineType,CWnd* pParent = NULL);   // standard constructor
	void InsertDayLineToList(BOOL IsMonthLine);
	void FillToListCtrlHead();
    BOOL InsertDateToList(char *szData);



public:
	UINT m_uiSelectDayLineType;
// Dialog Data
	//{{AFX_DATA(CDayLineSelect)
	enum { IDD = IDD_SELECT_DAYLINE };
	CButton	m_ok;
	CButton	m_cancel;
	int  m_nDayLineType;
	CListCtrl	m_lstStockTypeName;
	CTime	m_DayLineTime;
	CMonthCalCtrl	m_MonthCalCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDayLineSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDayLineSelect)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDayLineType();
	afx_msg void OnAddDownLoadData();


	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAYLINESELECT_H__B46517A2_88B1_11D4_882A_00400552E583__INCLUDED_)
