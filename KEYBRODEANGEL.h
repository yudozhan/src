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
  
	char    input;                 //用于传递变量 
	bool    ischar;                //是否是字符数据由VIEV传入
	char*   result;                //返回的结果
	int		m_stkKind;				//返回的股票种类结果
	BOOL    isresultstock;         //结果是否是股票代码 
	BOOL    isresultok;            //用户选择了确定还是取消
//	CString m_strPreInput;              //上一次的输入      
    CString inputpre;				  //上一次的输入

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
	int     m_nListPosition;          //滚动的位置
	int     m_nCurrentItemIndex;      //当前选定的索引值
	BOOL    m_bLastFocusFlag;         //用于确定上一次焦点的标志位
	int     m_nPreStarIndex;           //上一次的开始位置
	int     m_nPreEndIndex;             //上一次的结束位置
	   
	CArray<int,int> m_nStartPosArray; //存放开始位置的动态数组
	CArray<int,int> m_nEndPosArray;   //存放结束位置的动态数组
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_KEYBOARDANGELDLG_H_)
