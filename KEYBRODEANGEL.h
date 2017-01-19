//
// KEYBRODEANGEL.h : header file
//
////////////////////////////////////////////////////////////////////////////////////
//
//This file support a key board angel for inputting stock code/name or other code data
//
//This file is based on the old Shi Ji Long Single version 1.0
//Modified by Xia Qiankun, 2004/1/18
//
////////////////////////////////////////////////////////////////////////////////////

#if !defined(_SJLKEYBOARDANGELDLG_H_)
#define _SJLKEYBOARDANGELDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "Resource.h"

//including CTaiKlineListCtrl for supporting mouse drag/drop
#include "CTaiKlineListCtrl.h"

//
//This define compatible with the old code of Sjl Vesion 1.0
//Defined by Xia Qiankun 2004/1/28
//

class CTaiKeyBoardAngelDlg : public CDialog
{
// Construction
public:
	static void DeleteIndicatorName(CString name);
	void AddIndicatorName(CString name);
	void SetStartPos(BOOL bFellowingMouse,int px,int py);
	CTaiKeyBoardAngelDlg(CWnd* pParent = NULL);   // standard constructor
  
	char    input;                 //���ڴ��ݱ��� 
	bool    ischar;                //�Ƿ����ַ�������VIEV����
	char*   result;                //���صĽ��
	int		m_stkKind;				//���صĹ�Ʊ������
	BOOL    isresultstock;         //����Ƿ��ǹ�Ʊ���� 
	BOOL    isresultok;            //�û�ѡ����ȷ������ȡ��
//	CString m_strPreInput;              //��һ�ε�����      
    CString inputpre;				  //��һ�ε�����

// Dialog Data
	//{{AFX_DATA(CTaiKeyBoardAngelDlg)
	enum { IDD = IDD_ANGELKEYBRODE};
	CTaiKlineListCtrl	m_ListCtrl;
	CEdit	m_InputEditCtrl;
	CString	m_strInputResult;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKeyBoardAngelDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	char* SymbolToName(char * pSymbol);
	void InsertItemToList(int nWhichItem,int nStartItem,int nDefItem);
	void CalculateListRange(CString strInput);

	// Generated message map functions
	//{{AFX_MSG(CTaiKeyBoardAngelDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeInput();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindragList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSetfocusInput();
	afx_msg void OnKillfocusInput();
	//}}AFX_MSG
	afx_msg LRESULT OnDragBegin(WPARAM wParam, LPARAM lParam);//
	DECLARE_MESSAGE_MAP()

private:
	void OnReturn();
	BOOL m_bFellowingMouse;
	POINT m_nFellowingPt;
	//the wnd Handle which Mouse Drag/Drop to
	CWnd* m_wndParent;
	int     m_nStrLength;
	int     m_nNumberLength;
	int     m_nListPosition;          //������λ��
	int     m_nCurrentItemIndex;      //��ǰѡ��������ֵ
	BOOL    m_bLastFocusFlag;         //����ȷ����һ�ν���ı�־λ
	int     m_nPreStarIndex;           //��һ�εĿ�ʼλ��
	int     m_nPreEndIndex;             //��һ�εĽ���λ��
	   
	CArray<int,int> m_nStartPosArray; //��ſ�ʼλ�õĶ�̬����
	CArray<int,int> m_nEndPosArray;   //��Ž���λ�õĶ�̬����
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_KEYBOARDANGELDLG_H_)
