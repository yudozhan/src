#if !defined(AFX_DIALOGTEST_H__1BEB0B21_C0E8_11D2_91D7_0080C8E1242B__INCLUDED_)
#define AFX_DIALOGTEST_H__1BEB0B21_C0E8_11D2_91D7_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DIALOGTEST.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDialogLOGTEST dialog

class CDialogLOGTEST : public CDialog
{
// Construction
public:
	CDialogLOGTEST(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogLOGTEST)
	enum { IDD = IDD_CSmy };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogLOGTEST)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogLOGTEST)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTEST_H__1BEB0B21_C0E8_11D2_91D7_0080C8E1242B__INCLUDED_)
