#if !defined(AFX_SORTINDEX_H__110C7021_1503_11D4_8829_00400552E583__INCLUDED_)
#define AFX_SORTINDEX_H__110C7021_1503_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSortIndexDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSortIndexDlg dialog
#include "CTaiScreenParent.h"

class CSortIndexDlg : public CTaiScreenParent
{
// Construction
public:
	CSortIndexDlg(CWnd* pParent = NULL);   // standard constructor
	CSortIndexDlg(IndexDataInfo *pIndexSave,UINT nID,BOOL IsInit,BOOL IsChanged=TRUE,CWnd* pParent = NULL);   // standard constructor
	~CSortIndexDlg();
	void ExpandDialog(int nResourceID,BOOL bExpand); //��չ�Ի���
	void EnableVisibleChildren();                    //ʹչ�Ի��򲿷ֿؼ���Ч
	void InitTechniqueIndex(CFormularContent*	pIndex,BOOL IsInit=FALSE);  //��ʼ������ָ��
	void FillTechniqueToListCtrl();                              //������ָ�������LIST
	void CalcTechniqueIndexData();                               //����ָ��


	IndexDataInfo m_nIndexSave;                              //����ָ��ṹ            
	CString m_sLTypeName;                                  //ָ������
	UINT m_nColumnId;                                     //�к�
	BOOL m_bIsInit;                                       //�Ƿ��ʼ��
	CImageList * m_pImageList;                          //�����ͼ���б�
	CImageList * m_pImageListSmall;                     //����Сͼ���б�
	BOOL m_bIsChangeTechnique;                                //ָ���Ƿ�ı�
	BOOL m_bIsCalc;

// Dialog Data
	//{{AFX_DATA(CSortIndexDlg)
	enum { IDD = IDD_INDEXSORT };
	CProgressCtrl	m_Progress;
	CListCtrl	m_TechniqueList;
	CEdit	m_HelpCtl;
//	CListBox	m_FormularCtl;
	CString	m_periodstr;
	CButton	m_btOK;
	CButton	m_btCancel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortIndexDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSortIndexDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTechnique();
	virtual void OnOK();
	afx_msg void OnClick(NMHDR* wParam, LRESULT *lParam);
	afx_msg void OnClose();
	virtual void OnCancel();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTINDEX_H__110C7021_1503_11D4_8829_00400552E583__INCLUDED_)
