#if !defined(AFX_CWDEXPORTFORMULAR_H__854067A1_051D_11D7_9690_B3036BABFB72__INCLUDED_)
#define AFX_CWDEXPORTFORMULAR_H__854067A1_051D_11D7_9690_B3036BABFB72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CwdExportFormular.h : header file
//
#include "CTaiScreenParent.h"
#include "CFormularContent.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CFxjExportFormular dialog

class CFxjExportFormular : public CTaiScreenParent
{
// Construction
public:
	CFxjExportFormular(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFxjExportFormular)
	enum { IDD = IDD_CWD_EXPORT_FORMU };
	CString	m_sBrief;
	CString	m_sExplain;
	CString	m_sFormular;
	CString	m_sPassword;
	BOOL	m_bSetPassword;
	BOOL	m_bCompleteSecret;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFxjExportFormular)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFxjExportFormular)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnSelchangedTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CWDEXPORTFORMULAR_H__854067A1_051D_11D7_9690_B3036BABFB72__INCLUDED_)
