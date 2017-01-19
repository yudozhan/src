#if !defined(AFX_ALCINDEX_H__C7754381_1C1C_11D4_8829_00400552E583__INCLUDED_)
#define AFX_ALCINDEX_H__C7754381_1C1C_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// alcIndex.h : header file
//

#include "CTaiScreenParent.h"
/////////////////////////////////////////////////////////////////////////////
// CCalcIndex dialog

class CCalcIndex : public CTaiScreenParent
{
// Construction
public:
	CCalcIndex(CWnd* pParent = NULL);   // standard constructor
	CCalcIndex(Index_Technique *pIndex,UINT nID,CWnd* pParent = NULL);   // standard constructor
    
	Index_Technique m_pIndexTechnique;
	UINT m_nColumnId;
	void CalcIndexIndexData();
	BOOL m_bIsCalc;


// Dialog Data
	//{{AFX_DATA(CCalcIndex)
	enum { IDD = IDD_COMPUTEINDEX };
	CButton	m_ok;
	CButton	m_cancel;
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalcIndex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	//}}AFX_VIRTUAL
	afx_msg LRESULT OnCalcIndex(WPARAM wParam, LPARAM lParam);


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalcIndex)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALCINDEX_H__C7754381_1C1C_11D4_8829_00400552E583__INCLUDED_)
