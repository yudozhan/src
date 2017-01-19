#if !defined(AFX_ALERTSET_H__F218F662_69E0_11D4_970C_0080C8D6450E__INCLUDED_)
#define AFX_ALERTSET_H__F218F662_69E0_11D4_970C_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CAlertSet.h : header file
//
#include "CTaiScreenParent.h"
#include "CAlertSystem.h"

/////////////////////////////////////////////////////////////////////////////
// CAlertSet dialog

class CAlertSet : public CTaiScreenParent
{
// Construction
public:
	void NotifySubclass();
	int m_nItem;
	void GetItemData(ALERT_SET_ITEM &itemData);
	void SetInitData(ALERT_SET_ITEM& itemData);
	BYTE m_btAddtional[4];//附加条件

	CAlertSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlertSet)
	enum { IDD = IDD_TJXG_ALARM_SET };
	CButton	m_settingAdded;
	CButton	m_buttonExpl;
	CListCtrl	m_liststockrange;
	CString	m_sTitle;
	CButton m_btAddS;
	CButton m_btDeleteS;
	CButton m_btOK;
	CButton m_btCancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlertSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlertSet)
	virtual void OnOK();//保存选择的预警条件
	virtual void OnCancel();
	afx_msg void OnDeleteStocks2();
	afx_msg void OnAddStocksParent();
	afx_msg void OnSettingAdded();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton2();
	afx_msg void OnKeydownStockList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSelchangedTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALERTSET_H__F218F662_69E0_11D4_970C_0080C8D6450E__INCLUDED_)
