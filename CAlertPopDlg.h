#if !defined(AFX_ALERTPOPDLG_H__FD452F62_6C45_11D4_970C_0080C8D6450E__INCLUDED_)
#define AFX_ALERTPOPDLG_H__FD452F62_6C45_11D4_970C_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CAlertPopDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlertPopDlg dialog

class CAlertPopDlg : public CDialog
{
// Construction
public:
	CAlertPopDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlertPopDlg)
	enum { IDD = IDD_1UPALERT };
	CButton	m_ok;
	CButton	m_cancel;
	CButton	m_bt1;
	CListBox	m_conInfo;
	CString	m_sInfo;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlertPopDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlertPopDlg)
	afx_msg void OnToAlarmSystem();//切换到预警系统
	afx_msg void OnToKlineFiguer();//切换到图形分析界面
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDblclkList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALERTPOPDLG_H__FD452F62_6C45_11D4_970C_0080C8D6450E__INCLUDED_)
