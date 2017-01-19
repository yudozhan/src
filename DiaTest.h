#if !defined(AFX_DIATEST_H__34B3B525_6A8A_11D3_88F5_0080C8E20736__INCLUDED_)
#define AFX_DIATEST_H__34B3B525_6A8A_11D3_88F5_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaTest.h : header file
//
#include "CTaiShanDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CDiaTest dialog

class CDiaTest : public CDialog
{
// Construction
public:
	CDiaTest(CWnd* pParent = NULL);   // standard constructor
    CTaiShanDoc *pDoc;
// Dialog Data
	//{{AFX_DATA(CDiaTest)
	enum { IDD = IDD_DIALOG1};
	CString	m_edit1;
	CString	m_edit2;
	CString	m_edit3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDiaTest)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIATEST_H__34B3B525_6A8A_11D3_88F5_0080C8E20736__INCLUDED_)
