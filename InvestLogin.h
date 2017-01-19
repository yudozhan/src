#if !defined(AFX_INVESTLOGIN_H__65D5911D_3BA8_4D21_9823_A5FFBB6673C6__INCLUDED_)
#define AFX_INVESTLOGIN_H__65D5911D_3BA8_4D21_9823_A5FFBB6673C6__INCLUDED_

#include "ManageInvest.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InvestLogin.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CInvestLogin dialog

struct Account//帐户的信息
{
	char chName[16];
	char chPassword[16];
	bool bRemember;
	unsigned uTime;
};


class CInvestLogin : public CDialog
{
// Construction
public:
	AccountID m_Account;
	CString m_szFileName;
	bool m_bZoom;


	void Refresh();
	void WriteRegInfo();
	void ReadRegInfo();
	CInvestLogin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInvestLogin)
	enum { IDD = IDD_INVESTLOGIN };
	CButton	m_ok;
	CButton	m_cancel;
	CButton	m_setting;
	CButton	m_editAccount;
	CButton	m_delete;
	CButton	m_add;
	CListCtrl	m_UserCtrlList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInvestLogin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CArray<Account, Account &>m_arrAccount;


	// Generated message map functions
	//{{AFX_MSG(CInvestLogin)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetting();
	afx_msg void OnClose();
	afx_msg void OnSelchangeCombo();
	afx_msg void OnAddAccount();
	afx_msg void OnEditAccount();
	afx_msg void OnDeleteAccount();
	virtual void OnOK();
	afx_msg void OnDblclkUserlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownUserlist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INVESTLOGIN_H__65D5911D_3BA8_4D21_9823_A5FFBB6673C6__INCLUDED_)
