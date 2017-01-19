#if !defined(AFX_NINEGRIDCTRL_H__DC413962_1E86_11D4_8829_00400552E583__INCLUDED_)
#define AFX_NINEGRIDCTRL_H__DC413962_1E86_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CNineGridCtrl.h : header file
//
#include "GridCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CNineGridCtrl window


class CNineShowView;
class CNineGridCtrl : public CGridCtrl
{
// Construction
	friend class CNineShowView;

public:
	CNineGridCtrl();
	STOCKDATASHOW *m_pShowData;
	BOOL m_bGetFocus;
	int m_ntime;
	BOOL m_bSync;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNineGridCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNineGridCtrl();
	BOOL GetCell(CGridCell* pGridCell,int nRow, int nCol) const;
    BOOL SetRowCount(int nRows = 10);
	BOOL SetShowData(STOCKDATASHOW *pShowData){m_pShowData=pShowData;return TRUE;}
	void SetFocusState(BOOL l_FocesState){m_bGetFocus=l_FocesState;}
    BOOL GetFocusState(){return m_bGetFocus;} 
	void SetTimeInter(int Times){m_ntime=Times;}
    void SetGridSync(BOOL IsSync){m_bSync=IsSync;}
    virtual BOOL  DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk=FALSE);






	// Generated message map functions
protected:
	//{{AFX_MSG(CNineGridCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NINEGRIDCTRL_H__DC413962_1E86_11D4_8829_00400552E583__INCLUDED_)
