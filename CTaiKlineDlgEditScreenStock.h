#if !defined(AFX_TECHDLGEDITSCREENSTOCK_H__10379B43_6173_11D4_970B_0080C8D6450E__INCLUDED_)
#define AFX_TECHDLGEDITSCREENSTOCK_H__10379B43_6173_11D4_970B_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgEditScreenStock.h : header file
// 模块编号：005

// 文件名：CTaiKlineDlgEditScreenStock.h

// 创建者：

// 创建时间：2000.8.3

// 内容描述：

//		编辑选股指标公式、k线组合指标公式。
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgEditScreenStock dialog
#include "FloatEdit.h"
#include "AutoRichEditCtrl.h"
#include "RichEditFormular.h"

class CTaiKlineDlgEditScreenStock : public CDialog
{
// Construction
public:
	void DoMoveWindow();
	bool m_bShowFromMouse;

	bool FindIndicator(int nKind,CString sName,CFormularContent  *jishunow);

	int m_nKlineType;
	int m_nTabPre;

	CString help;

	void ChangePeriodParamData(bool bSave = true);
	void ParamIntoOther(int nFlag);
	int m_maxChars;
	CTaiKlineDlgEditScreenStock(int nID,CWnd* pParent = NULL);   // standard constructor
	virtual ~CTaiKlineDlgEditScreenStock();

	BOOL CheckParmNameInput(CString parname,CString & errmessage);
	BOOL CheckParmInput(CString parminput);
	float StringToFloat(CString str);
	void ShowData(CString &str,float fdata);
	CFormularContent*  jishunow;                    //指向正在操作的列表项的指针
	int  errorposition;                   //错误的位置
	CString errormessage;                 //错误信息
	CString functionstr;                  //插入的函数串
	int item;                             //正在操作的列表项的索引
	int iskline;                         //0:是K线选股,1:是k线组合,2:是技术指标公式
//	RECT datarect;                        //错误信息的输出区域
	int numPara;                          //参数个数
//yhh modify 990625
//	CString inputpre;
	int  addpos;

	//2.0 add
	CString	m_editParam[8][4];//参数信息
	CString m_editParamPeriod[8][11];//不同周期的不同参数
	CString m_strPeriodPre;//上一次的周期的名称
	BYTE	m_nPeriodsUsed[16];	//有效周期
	CString m_strParamExplain;//参数说明

	
// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgEditScreenStock)
	enum { IDD = IDD_EDIT_TJXG_K };
	CButton	m_ok;
	CButton	m_cancel;
	CButton	m_dyhs;
	CButton	m_csgs;
	CButton	m_button2;
	CButton	m_button1;
	CButton	m_canused;
	CFloatEdit	m_floatEdit12;
	CFloatEdit	m_floatEdit13;
	CFloatEdit	m_floatEdit14;
	CFloatEdit	m_floatEdit22;
	CFloatEdit	m_floatEdit23;
	CFloatEdit	m_floatEdit24;
	CFloatEdit	m_floatEdit32;
	CFloatEdit	m_floatEdit33;
	CFloatEdit	m_floatEdit34;
	CFloatEdit	m_floatEdit42;
	CFloatEdit	m_floatEdit43;
	CFloatEdit	m_floatEdit44;
	CTabCtrl	m_tabParam;
	CEdit	m_namecor;
	CEdit	m_passwordcor;
	CButton	m_hfqs;
	CRichEditFormular	m_inputcor;
	BOOL	m_checkmm;
	BOOL	m_checkOften;
	CString	m_explainbrief;
	CString	m_myhelp;
	CString	m_name;
	CString	m_password;
	CEdit	m_xlinecor;
	CString	m_xline;
	CString	m_sTestInfo;
	CString	m_sKindTime;
	CString m_sKindFormu;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgEditScreenStock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetStringParam(int x,int y,CString sIn);
	CString GetStringParam(int x,int y);

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDlgEditScreenStock)
	afx_msg void OnRestoreDefault();	//恢复缺省值
	afx_msg void OnTestFormula();		//测试公式
	afx_msg void OnAddFunc();			//插入函数
	afx_msg void OnImportFormula();		//引入公式
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnCheckmm();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonCanUsed();//可用周期
	afx_msg void OnButton1();//参数说明
	afx_msg void OnSelchangeComboKindTime();//周期种类
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnChangeInput();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void OnChangeInput2();
	bool m_bInit ;
	int m_nID[4][4];
	;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHDLGEDITSCREENSTOCK_H__10379B43_6173_11D4_970B_0080C8D6450E__INCLUDED_)
