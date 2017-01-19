#if !defined(AFX_BGRIDCTRL_H__DC413962_1E86_11D4_8829_00400552E583__INCLUDED_)
#define AFX_BGRIDCTRL_H__DC413962_1E86_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CBGridCtrl.h : header file
//
#include "GridCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CBGridCtrl window


class CSharesCjMxTj;
class CBGridCtrl : public CGridCtrl
{
// Construction
	friend class CSharesBigBillDlg;

public:
	CBGridCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBGridCtrl)
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBGridCtrl();
	BOOL GetCell(CGridCell* pGridCell,int nRow, int nCol) const;
    BOOL SetRowCount(int nRows = 10);
	float GetItemTextV(int nRow,int nCol);
	BOOL SortTextItems(int nCol, BOOL bAscending, int low, int high);
	void QuickSortString();
	CString GetTimeGrid(time_t ntime)const;







	// Generated message map functions
protected:
	//{{AFX_MSG(CBGridCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BGRIDCTRL_H__DC413962_1E86_11D4_8829_00400552E583__INCLUDED_)
