#if !defined(AFX_BJDLGADDDATA_H__B92BA8A8_F92A_4C28_BD82_F9BBAFA3174D__INCLUDED_)
#define AFX_BJDLGADDDATA_H__B92BA8A8_F92A_4C28_BD82_F9BBAFA3174D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BjDlgAddData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// BjDlgAddData dialog

class BjDlgAddData : public CDialog
{
// Construction
public:
	BjDlgAddData(CWnd* pParent = NULL);   // standard constructor
	COLORREF m_color;
// Dialog Data
	//{{AFX_DATA(BjDlgAddData)
	enum { IDD = IDD_BJ_ADD_DATA };
	UINT	m_nDays;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BjDlgAddData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(BjDlgAddData)
	afx_msg void OnButtonSet();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BJDLGADDDATA_H__B92BA8A8_F92A_4C28_BD82_F9BBAFA3174D__INCLUDED_)
