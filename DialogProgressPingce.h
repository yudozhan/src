#if !defined(AFX_DIALOGPROGRESSPINGCE_H__12610F23_4284_4306_B8E7_615499EDB757__INCLUDED_)
#define AFX_DIALOGPROGRESSPINGCE_H__12610F23_4284_4306_B8E7_615499EDB757__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogProgressPingce.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogProgressPingce dialog

class CDialogProgressPingce : public CDialog
{
// Construction
public:
	CDialogProgressPingce(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogProgressPingce)
	enum { IDD = IDD_DIALOG1 };
	CProgressCtrl	m_Progress;
	//}}AFX_DATA
private:
	int m_nLower;
	int m_nUpper;
	int m_nPos;
public:
	void SetRange(int nLower,int nUpper,BOOL bUpdate=FALSE);
	void SetPos(int nPos);
	void Complete();
	void Close();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogProgressPingce)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogProgressPingce)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPROGRESSPINGCE_H__12610F23_4284_4306_B8E7_615499EDB757__INCLUDED_)
