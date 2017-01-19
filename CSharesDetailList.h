#if !defined(AFX_STOCKDETAILLIST_H__7E188421_08E3_11D2_88EE_0080C8E20736__INCLUDED_)
#define AFX_STOCKDETAILLIST_H__7E188421_08E3_11D2_88EE_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSharesDetailList.h : header file
//
#include "CTaiShanDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CSharesDetailList window

class CSharesDetailList : public CListCtrl
{
// Construction
public:
	CSharesDetailList();

// Attributes
public:

// Operations
public:
    int   RedOrGreen[2000];         ///draw the number in different color 1 Up 2 Down 3 Equal
		                      //if true draw red else green
	int num;
	bool   m_IsF1;
    CTaiShanDoc* pDoc;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSharesDetailList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSharesDetailList();
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

	// Generated message map functions
protected:
	//{{AFX_MSG(CSharesDetailList)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOCKDETAILLIST_H__7E188421_08E3_11D2_88EE_0080C8E20736__INCLUDED_)
