#if !defined(AFX_INFOBAR_H__389E67D4_B07D_4E90_9EC8_653E1498F2D7__INCLUDED_)
#define AFX_INFOBAR_H__389E67D4_B07D_4E90_9EC8_653E1498F2D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoBar dialog
#define WM_USERDEFINED_INFOMEMORY		(WM_USER+1000)
#define WM_USERDEFINED_PUBLICINFO		(WM_USER+1001)
#define WM_USERDEFINED_NEWSTOCK			(WM_USER+1002)
#define WM_USERDEFINED_STATISTICS		(WM_USER+1003)
#define WM_USERDEFINED_FINANCENEWS		(WM_USER+1004)
#define WM_USERDEFINED_COMPANYNEWS		(WM_USER+1005)
#define WM_USERDEFINED_TRADEDYNAMIC		(WM_USER+1006)
#define WM_USERDEFINED_MARKETNEWS		(WM_USER+1007)
#define WM_USERDEFINED_YINHESTOCK		(WM_USER+1008)
#define WM_USERDEFINED_GUOTAIJUNAN		(WM_USER+1009)
#define WM_USERDEFINED_SHENGUANG		(WM_USER+1010)
#define WM_USERDEFINED_SHENYINWANGUO	(WM_USER+1011)
#define WM_USERDEFINED_WANGUOCEPING		(WM_USER+1012)
#define WM_USERDEFINED_MINGJIALUNTAN	(WM_USER+1013)
#define WM_USERDEFINED_DAPANFENXI		(WM_USER+1014)
#define WM_USERDEFINED_GEGUFENXI		(WM_USER+1015)
#define WM_USERDEFINED_HOTPOINTCOMMT	(WM_USER+1016)
#define WM_USERDEFINED_BLACKHORSE		(WM_USER+1017)
#define WM_USERDEFINED_POLICY			(WM_USER+1018)
#define WM_USERDEFINED_INVESTTECH		(WM_USER+1019)
#define WM_USERDEFINED_STAFFROOM		(WM_USER+1020)

class CInfoBar : public CDialog
{
// Construction
public:
	CInfoBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInfoBar)
	enum { IDD = IDD_DIALOG_INFO };
	CTreeCtrl	m_treeInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInfoBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreeInfo(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFOBAR_H__389E67D4_B07D_4E90_9EC8_653E1498F2D7__INCLUDED_)
