#if !defined(AFX_SHEETBKCYBK_H__9B88A303_B596_11D1_83E9_0000E8593F1A__INCLUDED_)
#define AFX_SHEETBKCYBK_H__9B88A303_B596_11D1_83E9_0000E8593F1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SHEETBKCYBK.h : header file
//

/*
*
*  将某一个板块设为常用板块
*  显示－＞属性设置－＞板块股设置－＞设为常用板块
*  将某一个板块设为9X可以直接转到的板块。
*  m_strName 是所有的十个常用板块，
*  m_nIndex 即为要替换的板块序号
*
*/

/////////////////////////////////////////////////////////////////////////////
// CTaiCommonBlockSetDlg dialog

class CTaiCommonBlockSetDlg : public CDialog
{
// Construction
public:
	CTaiCommonBlockSetDlg(CWnd* pParent = NULL);   // standard constructor
	CStringArray m_strName;
// Dialog Data
	//{{AFX_DATA(CTaiCommonBlockSetDlg)
	enum { IDD = IDD_DIALOG1 };
	int		m_nIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiCommonBlockSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiCommonBlockSetDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnChangeChoice(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETBKCYBK_H__9B88A303_B596_11D1_83E9_0000E8593F1A__INCLUDED_)
