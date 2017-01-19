#if !defined(AFX_TECHDLGEDITINDECATOR_H__10379B44_6173_11D4_970B_0080C8D6450E__INCLUDED_)
#define AFX_TECHDLGEDITINDECATOR_H__10379B44_6173_11D4_970B_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgEditIndecator.h : header file
// 模块编号：005

// 文件名：CTaiKlineDlgEditIndecator.h

// 创建者：

// 创建时间：2000.8.3

// 内容描述：

//		编辑技术指标公式。
//
#include "CTaiKlineDlgEditScreenStock.h"

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgEditIndecator dialog

class CTaiKlineDlgEditIndecator : public CTaiKlineDlgEditScreenStock
{
// Construction
public:
	CTaiKlineDlgEditIndecator(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgEditIndecator)
	enum { IDD = IDD_EDIT_ZBGS };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgEditIndecator)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDlgEditIndecator)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnMyhelp();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHDLGEDITINDECATOR_H__10379B44_6173_11D4_970B_0080C8D6450E__INCLUDED_)
