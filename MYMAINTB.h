#if !defined(AFX_MYMAINTB_H__4F771783_BC0C_11D2_9086_0000E8593F1A__INCLUDED_)
#define AFX_MYMAINTB_H__4F771783_BC0C_11D2_9086_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MYMAINTB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMYMAINTB window

class CMYMAINTB : public CToolBarCtrl
{
// Construction
public:
	CMYMAINTB();

// Attributes
public:


	int        buttonnumber;
	TBBUTTON   *m_tbbutton;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMYMAINTB)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMYMAINTB();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMYMAINTB)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYMAINTB_H__4F771783_BC0C_11D2_9086_0000E8593F1A__INCLUDED_)
