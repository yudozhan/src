#if !defined(AFX_HISTORYSELDATE_H__549C22E1_4FB2_11D6_968C_E8A724F73E7B__INCLUDED_)
#define AFX_HISTORYSELDATE_H__549C22E1_4FB2_11D6_968C_E8A724F73E7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistorySelDate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistorySelDate dialog

class CHistorySelDate : public CDialog
{
// Construction
public:
	CHistorySelDate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CHistorySelDate)
	enum { IDD = IDD_DIALOG_SELECT_DATE };
	CButton	m_ok;
	CButton	m_cancel;
	CTime	m_tm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistorySelDate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHistorySelDate)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYSELDATE_H__549C22E1_4FB2_11D6_968C_E8A724F73E7B__INCLUDED_)
