#if !defined(AFX_DLGMYXMZ_H__FC62D2DB_5ABA_4506_9EF3_4B5E15FAD0D6__INCLUDED_)
#define AFX_DLGMYXMZ_H__FC62D2DB_5ABA_4506_9EF3_4B5E15FAD0D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMYXMZ.h : header file
//
#define WM_MYMSG (WM_USER+1121)
/////////////////////////////////////////////////////////////////////////////
// CDialogMYXMZ dialog

class CDialogMYXMZ : public CDialog
{
// Construction
public:
	CImageList m_imagelist;
	CStringList m_strlistData;
	CDialogMYXMZ(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogMYXMZ)
	enum { IDD = IDD_MYXMZ };
	CListCtrl	m_lstStockTypeName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogMYXMZ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogMYXMZ)
	afx_msg LRESULT DealMsg(WPARAM wParam,LPARAM lParam);	
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMYXMZ_H__FC62D2DB_5ABA_4506_9EF3_4B5E15FAD0D6__INCLUDED_)
