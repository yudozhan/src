#if !defined(AFX_DIALOGXZZB_H__584E307B_DB79_11D1_83F9_0000E8593F1A__INCLUDED_)
#define AFX_DIALOGXZZB_H__584E307B_DB79_11D1_83F9_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DIALOGXZZB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogLOGXZZB dialog

class CDialogLOGXZZB : public CDialog
{
// Construction
public:
	CDialogLOGXZZB(CWnd* pParent = NULL);   // standard constructor
	CString result;
// Dialog Data
	//{{AFX_DATA(CDialogLOGXZZB)
	enum { IDD = IDD_DIALOGXZZB };
	CButton	m_ok;
	CButton	m_cancel;
	CListCtrl	m_list;
	CString	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogLOGXZZB)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogLOGXZZB)
	afx_msg void OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGXZZB_H__584E307B_DB79_11D1_83F9_0000E8593F1A__INCLUDED_)
