#include "afxcmn.h"
#if !defined(AFX_MANAGERCHOOSE_H__BD1C43A3_5D50_11D4_8829_00400552E583__INCLUDED_)
#define AFX_MANAGERCHOOSE_H__BD1C43A3_5D50_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManagerChoose.h : header file
//
#define WM_USER_ADDSTOCK WM_USER+1200

/////////////////////////////////////////////////////////////////////////////
// CSuperviseChoose dialog

class CSuperviseChoose : public CDialog
{
// Construction
public:
	CSuperviseChoose(CWnd* pParent = NULL);   // standard constructor
public:
	CImageList * m_pImageList;    //大图象列表
	CImageList * m_pImageListSmall; //小图象列表
    CTaiShanDoc *m_pDoc;

// Dialog Data
	//{{AFX_DATA(CSuperviseChoose)
	enum { IDD = IDD_MANAGER_CHOOSE1 };
	CButton	m_ok;
	CButton	m_btn2;
	CButton	m_btn1;
	CListCtrl	m_lstStockName;
	CListCtrl	m_lstChooseType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperviseChoose)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   BOOL InsertStockToList(char *name,char *code,int nKind);
   void FillToListCtrlHead();



	// Generated message map functions
	//{{AFX_MSG(CSuperviseChoose)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAddStockToChoose();
	afx_msg void OnDeleteStockFromChoose();
	//}}AFX_MSG
	afx_msg LRESULT OnAddStockCode(WPARAM wParam, LPARAM lParam); //增加股票至LISTCTRL

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
public:
	afx_msg void OnBnClickedNewchoosetype();
public:
	afx_msg void OnBnClickedDelchoosetype();
public:
	afx_msg void OnBnClickedEmptychoosestock();
	void CSuperviseChoose::RefreshShowData();

public:
	afx_msg void OnNMClickList4(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGERCHOOSE_H__BD1C43A3_5D50_11D4_8829_00400552E583__INCLUDED_)
