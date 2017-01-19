#if !defined(AFX_FYREGISTER_H__AA9D2BA1_D444_11D6_968F_AE9E83D8F170__INCLUDED_)
#define AFX_FYREGISTER_H__AA9D2BA1_D444_11D6_968F_AE9E83D8F170__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FyRegister.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FyRegister dialog

class FyRegister : public CDialog
{
// Construction
public:
	static bool IsValidUser();
	FyRegister(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(FyRegister)
	enum { IDD = IDD_FY_REGISTER };
	CString	m_sMachineID;
	CString	m_sEmail;
	CString	m_sID;
	CString	m_sPhone;
	CString	m_sAddress;
	CString	m_sPower;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FyRegister)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FyRegister)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonHand();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FYREGISTER_H__AA9D2BA1_D444_11D6_968F_AE9E83D8F170__INCLUDED_)
