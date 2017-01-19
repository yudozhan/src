#if !defined(AFX_CAIWUDATA_H__34161F41_8E06_11D4_9426_0080C8E20736__INCLUDED_)
#define AFX_CAIWUDATA_H__34161F41_8E06_11D4_9426_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CaiwuData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCaiwuData dialog

class CCaiwuData : public CPropertyPage
{
	DECLARE_DYNCREATE(CCaiwuData)

// Construction
public:
	int m_nKind;
    static	BOOL ImportCwData(CString FileName);
	void OutCaiWuData(CString PathFileName);
	CCaiwuData();
	~CCaiwuData();
	CTaiShanDoc* pDoc; 
// Dialog Data
	//{{AFX_DATA(CCaiwuData)
	enum { IDD = IDD_CAIWUINOUT };
	CButton	m_zdtq;
	CButton	m_button6;
	CButton	m_button5;
	CProgressCtrl	m_progress;
	CListCtrl	m_listcw;
	CString	m_ShowStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCaiwuData)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCaiwuData)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeStockSymbol();
	afx_msg void OnExport();
	afx_msg void OnImport();
	afx_msg void OnZdtq();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAIWUDATA_H__34161F41_8E06_11D4_9426_0080C8E20736__INCLUDED_)
