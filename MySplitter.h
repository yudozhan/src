#if !defined(AFX_MYSPLITTER_H__CF140A65_E8A5_11D1_88EA_0080C8E20736__INCLUDED_)
#define AFX_MYSPLITTER_H__CF140A65_E8A5_11D1_88EA_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySplitter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiTestSplitter window

class CTaiTestSplitter : public CSplitterWnd
{
// Construction
public:
	CTaiTestSplitter();
    DECLARE_DYNCREATE(CTaiTestSplitter)
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiTestSplitter)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTaiTestSplitter();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTaiTestSplitter)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSPLITTER_H__CF140A65_E8A5_11D1_88EA_0080C8E20736__INCLUDED_)
