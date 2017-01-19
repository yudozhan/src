#if !defined(AFX_LMBSPLITWND_H__F60EB4E5_A40F_11D3_96E2_0080C8E1242B__INCLUDED_)
#define AFX_LMBSPLITWND_H__F60EB4E5_A40F_11D3_96E2_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineSplitWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineSplitWnd frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CTaiKlineSplitWnd : public CSplitterWnd
{
	DECLARE_DYNCREATE(CTaiKlineSplitWnd)
protected:

// Attributes
protected:
public:
	CTaiKlineSplitWnd();           // protected constructor used by dynamic creation

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineSplitWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	CWnd* GetActivePane(int* row,int *  col);
	virtual ~CTaiKlineSplitWnd();

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineSplitWnd)
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMBSPLITWND_H__F60EB4E5_A40F_11D3_96E2_0080C8E1242B__INCLUDED_)
