#if !defined(AFX_DIACOPY_H__E4858281_3E7F_11D3_88F1_0080C8E20736__INCLUDED_)
#define AFX_DIACOPY_H__E4858281_3E7F_11D3_88F1_0080C8E20736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaCopy.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiaCopy dialog

class CDiaCopy : public CDialog
{
// Construction
public:
	CDiaCopy(CWnd* pParent = NULL);   // standard constructor
    BOOL FileCopy(char *filedes,char *filesrc);
// Dialog Data
	//{{AFX_DATA(CDiaCopy)
	enum { IDD = IDD_DIACOPY };
	CProgressCtrl	m_progress;
	CString	m_static;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiaCopy)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
//protected:
	public:
	// Generated message map functions
	//{{AFX_MSG(CDiaCopy)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIACOPY_H__E4858281_3E7F_11D3_88F1_0080C8E20736__INCLUDED_)
