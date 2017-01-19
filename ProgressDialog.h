#if !defined(AFX_PROGRESSDIALOG_H__F9A7EF81_F24A_11D2_91E5_0000E8654960__INCLUDED_)
#define AFX_PROGRESSDIALOG_H__F9A7EF81_F24A_11D2_91E5_0000E8654960__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressDialog dialog

class CProgressDialog : public CDialog
{
// Construction
public:
	CProgressDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgressDialog)
	enum { IDD = IDD_DIALOG_PROGRESS };
	CProgressCtrl	m_Progress;
	CString	m_zqdm;
	CString	m_comp;
	//}}AFX_DATA
    BOOL m_DialogDown;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CProgressDialog)
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	void DisplayZqxx(CString zqdm, int m_pos) ;
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSDIALOG_H__F9A7EF81_F24A_11D2_91E5_0000E8654960__INCLUDED_)
