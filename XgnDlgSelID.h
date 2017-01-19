#if !defined(AFX_XGNDLGSELID_H__6849FF01_84E7_11D6_968D_F5D7623CFF75__INCLUDED_)
#define AFX_XGNDLGSELID_H__6849FF01_84E7_11D6_968D_F5D7623CFF75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XgnDlgSelID.h : header file
//
#include "indicatorextendData.h"
#include "FileExtendData.h"
#include "FileInOutData.h"
#include "FtpGetSend.h"
#include "FtpParaClass.h"



/////////////////////////////////////////////////////////////////////////////
// XgnDlgSelID dialog

class XgnDlgSelID : public CDialog
{
// Construction
public:
	XgnDlgSelID(CWnd* pParent = NULL);   // standard constructor
	int m_nSel;
	void InsertListInfo();
	indicatorInfoArr m_infoArr;

// Dialog Data
	//{{AFX_DATA(XgnDlgSelID)
	enum { IDD = IDD_XGN_SEL_ID };
	CListCtrl	m_listData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(XgnDlgSelID)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(XgnDlgSelID)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XGNDLGSELID_H__6849FF01_84E7_11D6_968D_F5D7623CFF75__INCLUDED_)
