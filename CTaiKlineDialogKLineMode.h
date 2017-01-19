#if !defined(AFX_LDIALOGKLINEMODE_H__7C4D83A1_A1AB_11D2_91E3_0080C8E1242B__INCLUDED_)
#define AFX_LDIALOGKLINEMODE_H__7C4D83A1_A1AB_11D2_91E3_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDialogKLineMode.h : header file
//
#include "CFormularCompute.h"
/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDialogKLineMode dialog
class CTaiShanKlineShowView;
class CTaiKlineDialogKLineMode : public CDialog
{
// Construction
public:
	void Set(CFormularContent *pJishu);
	CTaiShanKlineShowView* pView ;

	void CaclKLineMode(CTaiShanKlineShowView* pView,CString name,ARRAY_BE& pline);
	//����:����k����Ϲ�ʽ
	//�������:pView�������������洰�ڣ�nameָ������
	//�������:pline������Ľ��
	void DrawKLineMode(CTaiShanKlineShowView* pView,CDC* pDC);
	//����:��k����ϵ�λͼ��
	//�������:pView�������������洰��
	CTaiKlineDialogKLineMode(CWnd* pParent = NULL);   // standard constructor
	CImageList m_imagelist;
	int m_numLine;


// Dialog Data
	//{{AFX_DATA(CTaiKlineDialogKLineMode)
	enum { IDD = IDD_DIALOG_KLINE_M };
	CListCtrl	m_kline_mode;//�г�k����Ϲ�ʽ
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDialogKLineMode)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDialogKLineMode)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnKeydownListKlineModeL(NMHDR* pNMHDR, LRESULT* pResult);//ת��ָ�깫ʽ
	afx_msg void OnClickListKlineModeL(NMHDR* pNMHDR, LRESULT* pResult);//ѡ��ָ�깫ʽ
	afx_msg void OnPaint();
	afx_msg void OnCancel();
	afx_msg void OnKillfocusListKlineModeL(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LDIALOGKLINEMODE_H__7C4D83A1_A1AB_11D2_91E3_0080C8E1242B__INCLUDED_)
