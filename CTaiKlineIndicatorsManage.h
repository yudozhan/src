#if !defined(AFX_TECHINDICATORSMANAGE_H__10379B42_6173_11D4_970B_0080C8D6450E__INCLUDED_)
#define AFX_TECHINDICATORSMANAGE_H__10379B42_6173_11D4_970B_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineIndicatorsManage.h : header file
// 模块编号：005

// 文件名：CTaiKlineIndicatorsManage.h

// 创建者：

// 创建时间：

// 内容描述：

//		管理各种指标公式。
//
#include "CTaiKlineTreeCtl.h"

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineIndicatorsManage dialog
class CFormularContent;
class CTaiKlineIndicatorsManage : public CDialog
{
// Construction
public:
	CTaiKlineIndicatorsManage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaiKlineIndicatorsManage)
	enum { IDD = IDD_MANAGE_GONGSHI };
	CButton	m_often;
	CButton	m_conModify;
	CButton	m_conAdd;
	CButton	m_conDelete;
	CButton	m_conRead;
	CButton	m_conSave;
	CTaiKlineTreeCtl	m_treeFormularCtrl;	//用于罗列指标公式的树状控制
	CString	m_editexplain;
	CString	m_tabedit;
	CButton	m_btOK;
	CButton	m_btCancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineIndicatorsManage)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineIndicatorsManage)
	afx_msg void OnAddFormula();	//增加公式
	afx_msg void OnModifyFormula();	//修改公式
	afx_msg void OnDeleteFormula();	//删除公式
	afx_msg void OnSave();			//保存公式
	afx_msg void OnRead();			//读入公式
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnOften();			//常用指标设置
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSelchangeTabSelect(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nKindRoot;			//指标公式的种类
	CFormularContent *	m_pIndex;		//指标公式的数据指针
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHINDICATORSMANAGE_H__10379B42_6173_11D4_970B_0080C8D6450E__INCLUDED_)
