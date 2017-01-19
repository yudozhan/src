#if !defined(AFX_SHEETBHADD_H__9B88A302_B596_11D1_83E9_0000E8593F1A__INCLUDED_)
#define AFX_SHEETBHADD_H__9B88A302_B596_11D1_83E9_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SHEETBHADD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSHEETBHADD dialog

class CSHEETBHADD : public CDialog
{
// Construction
public:
	CSHEETBHADD(CWnd* pParent = NULL);   // standard constructor
//////////////////////

	BOOL m_OpenKind;

// Dialog Data
	//{{AFX_DATA(CSHEETBHADD)
	enum { IDD = IDD_DIALOG1 };
	CEdit	m_cBKNAME;
	CStatic	m_Static;
	CString	m_bkname;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSHEETBHADD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSHEETBHADD)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETBHADD_H__9B88A302_B596_11D1_83E9_0000E8593F1A__INCLUDED_)
