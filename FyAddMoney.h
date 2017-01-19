#if !defined(AFX_FYADDMONEY_H__AA9D2BA2_D444_11D6_968F_AE9E83D8F170__INCLUDED_)
#define AFX_FYADDMONEY_H__AA9D2BA2_D444_11D6_968F_AE9E83D8F170__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FyAddMoney.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FyAddMoney dialog

class FyAddMoney : public CDialog
{
// Construction
public:
	static void ShowTime(CString sID);
	static int g_nUseTimes;
	static void SetUrl(char * chUrl);
	static CString GetUrl();
	static bool ToLogOn();
	static bool ToLogOn2();
	static void ReadSettings(CString &sUser,CString &sID);
	static void SaveSettings(CString sUser,CString sID);
	static CString GetReturnCode(CString sUrl,bool bPrompt,int &nErrorCode);
	FyAddMoney(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(FyAddMoney)
	enum { IDD = IDD_FY_ADDMONEY };
	CString	m_sUser;
	CString	m_sID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FyAddMoney)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FyAddMoney)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FYADDMONEY_H__AA9D2BA2_D444_11D6_968F_AE9E83D8F170__INCLUDED_)
