#include "afxcmn.h"
#if !defined(AFX_MANAGERSTOCKTYPE_H__BD1C43A1_5D50_11D4_8829_00400552E583__INCLUDED_)
#define AFX_MANAGERSTOCKTYPE_H__BD1C43A1_5D50_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManagerStockType.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CSuperviseSharesBlock dialog

class CSuperviseSharesBlock : public CDialog
{
// Construction
public:
	CSuperviseSharesBlock(CWnd* pParent = NULL);   // standard constructor
	enum {ZGB=0,LTG=1,OTHER=2	 };       //用于表示权重类型  0 表示总股本 1表示流通股本 2表示相等权重 

public:
	CImageList * m_pImageList;    //大图象列表
	CImageList * m_pImageListSmall; //小图象列表
    CTaiShanDoc *m_pDoc;        //文档指针
	BOOL m_bIsCalc;


    void FillToListCtrlHead();      //初始化CListCtrl标题列
    void FillToListCtrlItem(BLOCKINFO *pStockType);   //将板块数据插入到CListCtrl控件中
	void RefreshShowData();    //刷新全部板块数据


// Dialog Data
	//{{AFX_DATA(CSuperviseSharesBlock)
	enum { IDD = IDD_MANAGER_STOCKTYPE };
	CButton	m_ok;
	CButton	m_updatestocktype;
	CButton	m_refreshday;
	CButton	m_refresh;
	CButton	m_managerchoose;
	CButton	m_delete;
	CButton	m_add;
	CProgressCtrl	m_Progress;
	CListCtrl	m_lstStockTypeName;                  //控件指针
	// 板块分类名列表
	CListCtrl m_lstBlockTypeName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperviseSharesBlock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSuperviseSharesBlock)
	virtual BOOL OnInitDialog();           //初始化板块数据对话框
	afx_msg void OnImportStockType();      //将全部板块数据导入数据库中
	afx_msg void OnAddStockType();         //增加新板块指数 
	afx_msg void OnUpdateStockType();      //修改板块指数
	afx_msg void OnManagerChoose();        //管理自选股
	afx_msg void OnDeletestocktype();      //删除板块指数 
	afx_msg void OnRefresh();              //刷新板块数据
	afx_msg void OnExport();
	afx_msg void OnRefreshDayLine();
	virtual void OnCancel();
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
public:
	afx_msg void OnBnClickedAddblocktype();
public:
	afx_msg void OnBnClickedDelblocktype();
public:
	afx_msg void OnBnClickedModstocktype();
public:
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
	afx_msg void OnBnClickedRefreshMinute();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGERSTOCKTYPE_H__BD1C43A1_5D50_11D4_8829_00400552E583__INCLUDED_)
