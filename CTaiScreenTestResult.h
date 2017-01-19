//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_SCREENSTOCKTESTRESULT_H__0D839825_62CE_11D4_970B_0080C8D6450E__INCLUDED_)
#define AFX_SCREENSTOCKTESTRESULT_H__0D839825_62CE_11D4_970B_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiScreenTestResult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiScreenTestResult dialog

class CTaiScreenTestResult : public CDialog
{
// Construction
public:
	CTaiScreenTestResult(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaiScreenTestResult)
	enum { IDD = IDD_DIATJXGRESULT };
	CMSFlexGrid	m_gridResult;	//显示计算结果的表格
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiScreenTestResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiScreenTestResult)
	afx_msg void OnDblClickMsflexgrid1();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENSTOCKTESTRESULT_H__0D839825_62CE_11D4_970B_0080C8D6450E__INCLUDED_)
