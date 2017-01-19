#if !defined(AFX_CWDOVERWRITEF_H__9501D63A_56E8_4C44_ACA3_FE5D89EB0770__INCLUDED_)
#define AFX_CWDOVERWRITEF_H__9501D63A_56E8_4C44_ACA3_FE5D89EB0770__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CwdOverWriteF.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFxjOverWriteF dialog

class CFxjOverWriteF : public CDialog
{
// Construction
public:
	CFxjOverWriteF(CWnd* pParent = NULL);   // standard constructor
	int m_nResponse;


// Dialog Data
	//{{AFX_DATA(CFxjOverWriteF)
	enum { IDD = IDD_CWD_OVERWRITE_FORMULAR };
	CString	m_s1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFxjOverWriteF)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFxjOverWriteF)
	afx_msg void OnButtonAll();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CWDOVERWRITEF_H__9501D63A_56E8_4C44_ACA3_FE5D89EB0770__INCLUDED_)
