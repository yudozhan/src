#if !defined(AFX_LOGONDLG_H__4DEB2F8A_50EB_4220_820E_B28686630409__INCLUDED_)
#define AFX_LOGONDLG_H__4DEB2F8A_50EB_4220_820E_B28686630409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogonDlg.h : header file
//
#define MSG_LOGON		WM_USER+20	//发送到登录窗口消息
/////////////////////////////////////////////////////////////////////////////
// CLogonDlg dialog
#include <afxmt.h>
//#include "MainFrm.h"

class CLogonDlg : public CDialog 
{
// Construction
public:
	CLogonDlg(CWnd* pParent = NULL);   // standard constructor
	RCV_LOGINAUTH_STRUCTEx m_LoginAuth;
//	CMainFrame* m_pMainFrame;
	BOOL m_bLogonSuccess;

// Dialog Data
	//{{AFX_DATA(CLogonDlg)
	enum { IDD = IDD_DIALOG1 };
	CComboBox	m_comboCtrServer;
	BOOL	m_checkAutoLogon;
	CString	m_editPassword;
	CString	m_editUserName;
	BOOL	m_checkSavePassword;
	CString	m_comboServer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogonDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg LRESULT OnMsgLogon(UINT wParam,LONG lParam);
	afx_msg void OnCheckAutologon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL ValidateUser(CString sServer,CString sUser,CString sPassword);
	void GetLogonInformationFromReg();
	BOOL m_bAutoLogon;
	BOOL m_bSavePassword;
	BOOL m_bTempSavePassword;
	CString m_sAutoLogonUser;
	CString m_sAutoLogonPassword;
	CString m_sAutoLogonServer;
	CEvent m_hEvent;
	CString m_strServer[5];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGONDLG_H__4DEB2F8A_50EB_4220_820E_B28686630409__INCLUDED_)
