#if !defined(AFX_SCREENSTOCKSINDICATE_H__62BF6203_63BF_11D4_970B_0080C8D6450E__INCLUDED_)
#define AFX_SCREENSTOCKSINDICATE_H__62BF6203_63BF_11D4_970B_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiScreensIndicate.h : header file
//
#include "FloatEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CTaiScreensIndicate dialog
#include "CTaiScreenParent.h"
class CTaiShanKlineShowView;
class CTaiScreensIndicate : public CTaiScreenParent
{
// Construction
public:
	void SetIndicator(CFormularContent *pJishu);
	void Calculate();
	CTaiShanKlineShowView* pView;
	bool m_bUseing;//�Ƿ���ʹ��
	CTaiScreensIndicate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaiScreensIndicate)
	enum { IDD = IDD_6_TJXG_INDICATE };
	CButton	m_button1;
	CButton	m_ok;
	CButton	m_cancel;
	CFloatEdit	m_floatEdit2;
	CFloatEdit	m_floatEdit1;
	CButton	m_buttonExpl;
	float	m_mbly;
	int		m_mbzq;
	BOOL	m_CheckSelect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiScreensIndicate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiScreensIndicate)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetAddCondition();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnButton2();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENSTOCKSINDICATE_H__62BF6203_63BF_11D4_970B_0080C8D6450E__INCLUDED_)
