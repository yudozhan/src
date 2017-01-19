#if !defined(AFX_CWDCLOSEWORK_H__07B961B2_AE7F_4EC2_B2BF_00AEFA710681__INCLUDED_)
#define AFX_CWDCLOSEWORK_H__07B961B2_AE7F_4EC2_B2BF_00AEFA710681__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CwdCloseWork.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFxjCloseWork dialog

class CFxjCloseWork : public CPropertyPage
{
	DECLARE_DYNCREATE(CFxjCloseWork)

// Construction
public:
	CFxjCloseWork();
	~CFxjCloseWork();

// Dialog Data
	//{{AFX_DATA(CFxjCloseWork)
	enum { IDD = IDD_CWD_CLOSEWORK };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFxjCloseWork)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFxjCloseWork)
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CWDCLOSEWORK_H__07B961B2_AE7F_4EC2_B2BF_00AEFA710681__INCLUDED_)
