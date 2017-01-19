#include "CTaiShanDoc.h"
#include "CFormularContent.h"
#if !defined(AFX_DIALOGGSGL_H__DC0261C1_D0B4_11D2_908C_0000E8593F1A__INCLUDED_)
#define AFX_DIALOGGSGL_H__DC0261C1_D0B4_11D2_908C_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DIALOGGSGL.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogLOGGSGL dialog

class CDialogLOGGSGL : public CDialog
{
// Construction
public:
	
	
	CArray<CFormularContent*,CFormularContent*> m_indexbak;
	CArray<CFormularContent*,CFormularContent*> m_klinebak;
	CArray<CFormularContent*,CFormularContent*> m_choosebak;
	void UpdateKlineData(int index);
	void UpdateMasterData(int index);
	void UpdateChooseData(int index);
	void UpdateIndexData(int index);
	

	
//	CString alllist[12];
                      	
	CDialogLOGGSGL(CWnd* pParent = NULL);   // standard constructor
    int  tabitem;                        //用户选择的标签号
	int  listitem;                       //用户选择的列表项的索引
	
// Dialog Data
	//{{AFX_DATA(CDialogLOGGSGL)
	enum { IDD = IDD_DIA_GSCC };
	CButton	m_cancel;
	CButton	m_save;
	CEdit	m_conProduct;
	CEdit	m_editcor;
	CListCtrl	m_tablist;
	CTabCtrl	m_tab1;
	CString	m_tabedit;
	CString	m_editexplain;
	CString	m_sProductCode;
	BOOL	m_check;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogLOGGSGL)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:


	// Generated message map functions
	//{{AFX_MSG(CDialogLOGGSGL)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnModifygs();
	afx_msg void OnAddgs();
	afx_msg void OnSave();
	afx_msg void OnRead();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnItemchangedTablist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeletegs();
	afx_msg void OnDblclkTablist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGGSGL_H__DC0261C1_D0B4_11D2_908C_0000E8593F1A__INCLUDED_)
