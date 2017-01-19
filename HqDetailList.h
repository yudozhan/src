#if !defined(AFX_HQDETAILLIST_H__19999CE1_33AE_11D3_88EF_0080C8E20736__INCLUDED_)
#define AFX_HQDETAILLIST_H__19999CE1_33AE_11D3_88EF_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HqDetailList.h : header file
//
#include "CTaiShanDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CHqDetailList window

class CHqDetailList : public CListCtrl
{
// Construction
public:
	CHqDetailList();

// Attributes
public:
    CTaiShanDoc *pDoc;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHqDetailList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHqDetailList();
    virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	// Generated message map functions
protected:
	//{{AFX_MSG(CHqDetailList)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HQDETAILLIST_H__19999CE1_33AE_11D3_88EF_0080C8E20736__INCLUDED_)
