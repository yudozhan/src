#if !defined(AFX_OPTION_H__2F387001_A115_11D4_9426_0080C8E20736__INCLUDED_)
#define AFX_OPTION_H__2F387001_A115_11D4_9426_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Option.h : header file
//
#include    "Sysface.h"
#include    "Syssetup.h"
#include    "SysSetParaPage.h" 

/////////////////////////////////////////////////////////////////////////////
// COPTION

class COPTION : public CPropertySheet
{
	DECLARE_DYNAMIC(COPTION)

// Construction
public:
	COPTION(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COPTION(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	bool m_bInit;
	CButton	m_ok;
	CButton	m_cancel;

// Operations
public:
   CSYSFACE                  m_syswg;
   CSYSSETUP                 m_syssetup;
   CTaiSysSetParameterPage   m_syspara;
	CButton	m_btOK;
	CButton	m_btCancel;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COPTION)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddNewPage();
	virtual ~COPTION();

	// Generated message map functions
protected:
	//{{AFX_MSG(COPTION)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTION_H__2F387001_A115_11D4_9426_0080C8E20736__INCLUDED_)
