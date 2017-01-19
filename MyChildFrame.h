#if !defined(AFX_MYCHILDFRAME_H__F9E97D0A_B5D0_4DCC_8C90_1417B8AA21AF__INCLUDED_)
#define AFX_MYCHILDFRAME_H__F9E97D0A_B5D0_4DCC_8C90_1417B8AA21AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyChildFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiTestChildFrame frame

class CTaiTestChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CTaiTestChildFrame)
protected:
	CTaiTestChildFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiTestChildFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTaiTestChildFrame();

	// Generated message map functions
	//{{AFX_MSG(CTaiTestChildFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCHILDFRAME_H__F9E97D0A_B5D0_4DCC_8C90_1417B8AA21AF__INCLUDED_)
