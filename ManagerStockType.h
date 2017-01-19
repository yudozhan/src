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
	enum {ZGB=0,LTG=1,OTHER=2	 };       //���ڱ�ʾȨ������  0 ��ʾ�ܹɱ� 1��ʾ��ͨ�ɱ� 2��ʾ���Ȩ�� 

public:
	CImageList * m_pImageList;    //��ͼ���б�
	CImageList * m_pImageListSmall; //Сͼ���б�
    CTaiShanDoc *m_pDoc;        //�ĵ�ָ��
	BOOL m_bIsCalc;


    void FillToListCtrlHead();      //��ʼ��CListCtrl������
    void FillToListCtrlItem(BLOCKINFO *pStockType);   //��������ݲ��뵽CListCtrl�ؼ���
	void RefreshShowData();    //ˢ��ȫ���������


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
	CListCtrl	m_lstStockTypeName;                  //�ؼ�ָ��
	// ���������б�
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
	virtual BOOL OnInitDialog();           //��ʼ��������ݶԻ���
	afx_msg void OnImportStockType();      //��ȫ��������ݵ������ݿ���
	afx_msg void OnAddStockType();         //�����°��ָ�� 
	afx_msg void OnUpdateStockType();      //�޸İ��ָ��
	afx_msg void OnManagerChoose();        //������ѡ��
	afx_msg void OnDeletestocktype();      //ɾ�����ָ�� 
	afx_msg void OnRefresh();              //ˢ�°������
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
