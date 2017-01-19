#if !defined(AFX_MAINFRMD_H__FA099AA6_9AD5_11D1_B032_000021002C16__INCLUDED_)
#define AFX_MAINFRMD_H__FA099AA6_9AD5_11D1_B032_000021002C16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mainFrmD.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// mainFrmD dialog

class mainFrmD : public CDialog
{
// Construction
public:
	mainFrmD(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(mainFrmD)
	enum { IDD = IDD_DIALOGMAINFRAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(mainFrmD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(mainFrmD)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRMD_H__FA099AA6_9AD5_11D1_B032_000021002C16__INCLUDED_)
