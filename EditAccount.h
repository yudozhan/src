#if !defined(AFX_EDITACCOUNT_H__0796DD57_5145_419A_A70F_1D9C78D24381__INCLUDED_)
#define AFX_EDITACCOUNT_H__0796DD57_5145_419A_A70F_1D9C78D24381__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditAccount.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditAccount dialog

class CEditAccount : public CDialog
{
// Construction
public:
	CEditAccount(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CEditAccount)
	enum { IDD = IDD_EDITACCOUNT };
	CButton	m_ok;
	CButton	m_cancel;
	CString	m_limitName;
	CString	m_limitOld;
	CString	m_limitPassword;
	CString	m_limitVerify;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditAccount)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	int m_Operate;//Add or Edit

	CString m_szName;//����
	CString m_szPassword;//����
	CString m_szVerify;//ȷ������

	CString m_szOldPassword;//ԭ���룬ֻ��"�༭"ʱ���á�

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditAccount)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITACCOUNT_H__0796DD57_5145_419A_A70F_1D9C78D24381__INCLUDED_)
