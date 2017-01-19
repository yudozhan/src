#if !defined(AFX_CWDSELECTFORMU_H__B9CF3621_FF11_11D6_9690_9B28DE262572__INCLUDED_)
#define AFX_CWDSELECTFORMU_H__B9CF3621_FF11_11D6_9690_9B28DE262572__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CwdSelectFormu.h : header file
//
#include "CTaiScreenParent.h"
#include "CFormularContent.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CFxjSelectFormu dialog

class CFxjSelectFormu : public CTaiScreenParent
{
// Construction
public:
	void RefreshJishuTreeCtrl();
	CFormularContent* GetJishu();
	CFxjSelectFormu(CWnd* pParent = NULL);   // standard constructor
	bool m_bOnlyIndicator;

// Dialog Data
	//{{AFX_DATA(CFxjSelectFormu)
	enum { IDD = IDD_CWD_IMPORT_FORMU };
	CButton	m_buttonExpl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFxjSelectFormu)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFxjSelectFormu)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnSelchangedTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton2();
	afx_msg void OnDblclkTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnModifyFormula();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CWDSELECTFORMU_H__B9CF3621_FF11_11D6_9690_9B28DE262572__INCLUDED_)
