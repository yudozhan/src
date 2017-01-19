#if !defined(AFX_MANAGEDATA_H__A891F112_7F6B_455D_842F_60496EEEB9E4__INCLUDED_)
#define AFX_MANAGEDATA_H__A891F112_7F6B_455D_842F_60496EEEB9E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManageData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSuperviseData

class CSuperviseData : public CPropertySheet
{
	DECLARE_DYNAMIC(CSuperviseData)

// Construction
public:
	CSuperviseData(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSuperviseData(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	CButton	m_ok;
	CButton	m_cancel;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperviseData)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSuperviseData();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSuperviseData)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGEDATA_H__A891F112_7F6B_455D_842F_60496EEEB9E4__INCLUDED_)
