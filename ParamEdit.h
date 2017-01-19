#if !defined(AFX_PARAMEDIT_H__0AD5B181_15F3_11D4_941E_0080C8E20736__INCLUDED_)
#define AFX_PARAMEDIT_H__0AD5B181_15F3_11D4_941E_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParamEdit window
#include "FloatEdit.h"

class CParamEdit : public CFloatEdit
{
// Construction
public:
	CString m_textPre;
	CParamEdit();
	CParamEdit(CListCtrl *list,int Item);
    void InsertItem();
	bool m_bTestMax;
protected:
	CListCtrl *m_plist;
	int nItem;
	CString m_str;//////store the initial data////////
	BOOL	bEscapeKey;
	int nMax,nMin;


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParamEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CParamEdit();
	BOOL CheckNumber(CString Num);


	// Generated message map functions
protected:
	//{{AFX_MSG(CParamEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMEDIT_H__0AD5B181_15F3_11D4_941E_0080C8E20736__INCLUDED_)
