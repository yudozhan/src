#if !defined(AFX_TECHINDICATORSMANAGE_H__10379B42_6173_11D4_970B_0080C8D6450E__INCLUDED_)
#define AFX_TECHINDICATORSMANAGE_H__10379B42_6173_11D4_970B_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineIndicatorsManage.h : header file
// ģ���ţ�005

// �ļ�����CTaiKlineIndicatorsManage.h

// �����ߣ�

// ����ʱ�䣺

// ����������

//		�������ָ�깫ʽ��
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
	CTaiKlineTreeCtl	m_treeFormularCtrl;	//��������ָ�깫ʽ����״����
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
	afx_msg void OnAddFormula();	//���ӹ�ʽ
	afx_msg void OnModifyFormula();	//�޸Ĺ�ʽ
	afx_msg void OnDeleteFormula();	//ɾ����ʽ
	afx_msg void OnSave();			//���湫ʽ
	afx_msg void OnRead();			//���빫ʽ
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnOften();			//����ָ������
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSelchangeTabSelect(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nKindRoot;			//ָ�깫ʽ������
	CFormularContent *	m_pIndex;		//ָ�깫ʽ������ָ��
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHINDICATORSMANAGE_H__10379B42_6173_11D4_970B_0080C8D6450E__INCLUDED_)
