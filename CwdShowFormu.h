#if !defined(AFX_CWDSHOWFORMU_H__B9CF3622_FF11_11D6_9690_9B28DE262572__INCLUDED_)
#define AFX_CWDSHOWFORMU_H__B9CF3622_FF11_11D6_9690_9B28DE262572__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CwdShowFormu.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFxjShowFormu dialog

class CFxjShowFormu : public CDialog
{
// Construction
public:
	CFxjShowFormu(CWnd* pParent = NULL);   // standard constructor

	CString m_title;
// Dialog Data
	//{{AFX_DATA(CFxjShowFormu)
	enum { IDD = IDD_CWD_SHOW_FORMU };
	CEdit	m_con;
	CString	m_s;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFxjShowFormu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFxjShowFormu)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CWDSHOWFORMU_H__B9CF3622_FF11_11D6_9690_9B28DE262572__INCLUDED_)
