//{{AFX_INCLUDES()
#include "dtpicker.h"
//}}AFX_INCLUDES
#if !defined(AFX_LDIALOGCHANGKLINE_H__467F7902_A7A4_11D2_91E0_0080C8E1242B__INCLUDED_)
#define AFX_LDIALOGCHANGKLINE_H__467F7902_A7A4_11D2_91E0_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDialogChangKLine.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDialogChangKLine dialog
class CTaiShanKlineShowView;
#include "FloatEdit.h"

class CTaiKlineDialogChangKLine : public CDialog
{
// Construction
public:
	CTaiShanKlineShowView* pView ;
	bool m_bInit;
	UINT m_nPrevStyle;
	int m_foot;//K线数据数组的下标
	CString m_time;
	float m_amount;

	int AddData();
	//功能：把新数据写到文件中
	void LoadData(int foot);
	//功能：把foot下标对应的数据装载进来，并显示
	//输入说明：foot是k线数据的下标
//	CDateTimeCtrl m_DateTime;
//	CString m_vol;
/*	CString m_open;
	CString m_high;
	CString m_low;
	CString m_close;
	CString m_vol;
	float m_open;
	float m_high;
	float m_low;
	float m_close;
	int m_vol;
	int m_amount;*/
	CTaiKlineDialogChangKLine(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaiKlineDialogChangKLine)
	enum { IDD = IDD_DIALOG_CHANGE1 };
	CButton	m_ok;
	CButton	m_cancel;
	CButton	m_power;
	CButton	m_big;
	CButton	m_delete;
	CFloatEdit	m_floatEdit7;
	CFloatEdit	m_floatEdit9;
	CFloatEdit	m_floatEdit8;
	CFloatEdit	m_floatEdit5;
	CFloatEdit	m_floatEdit4;
	CFloatEdit	m_floatEdit3;
	CFloatEdit	m_floatEdit2;
	CFloatEdit	m_conAmount;
	CDTPicker	m_dtPicker;
	float	m_close;
	float	m_low;
	float	m_open;
	float	m_high;
	int		m_vol;
	CString	m_amountStr;
	float	m_fInnerPartVol;
	float	m_fOuterPartVol;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDialogChangKLine)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDialogChangKLine)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonDelete();//响应“删除”按钮消息
	afx_msg void OnChangeDtpicker1();////响应“日期”控件时间改变消息
	afx_msg void OnKeyDownDtpicker1(short FAR* KeyCode, short Shift);
	virtual void OnCancel();
	afx_msg void OnButtonPower();//响应“除权”按钮消息
	afx_msg void OnButtonEventBig();//响应“重大事件”按钮消息
	afx_msg void OnChangeEditHighChg();
	afx_msg void OnChangeEditLowChg();
	afx_msg void OnChangeEditAmountChg();
	afx_msg void OnChangeEditOpenChg();
	afx_msg void OnChangeEditCloseChg();
	afx_msg void OnChangeEditVolChg();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LDIALOGCHANGKLINE_H__467F7902_A7A4_11D2_91E0_0080C8E1242B__INCLUDED_)
