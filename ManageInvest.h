//{{AFX_INCLUDES()
#include "msflexgrid.h"
//}}AFX_INCLUDES
#if !defined(AFX_MANAGEINVEST_H__E646B0F1_B52D_490A_BE68_4E2041A4BDC1__INCLUDED_)
#define AFX_MANAGEINVEST_H__E646B0F1_B52D_490A_BE68_4E2041A4BDC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManageInvest.h : header file
//
 
/////////////////////////////////////////////////////////////////////////////
// CSuperviseInvest dialog

//投资记录，包括交易和资金
struct Invest
{
	unsigned uType;//交易=0，还是资金=1
	union
	{
		struct _Transact
		{
			unsigned uTime;//交易时间
			int nWay;//方向，买入为+1，卖出为-1
			char chSymbol[8];//代码
			int nKind;//股票种类//
			char chName[10];//名称
			double dPrice;//价格
			double dVolume;//成交量
			double dFee;//手续费
		}Transact;
		struct _Money
		{
			unsigned uTime;//时间
			int nWay;//方向，存入为+1，取出为-1
			double dAmount;//金额
		}Money;
	};
};

//帐户ID
struct AccountID
{
	char chName[16];//01－16字节，用户名，
	char chPassword[16];//17－32字节，PASSWORD，
	unsigned uRecords;//33－36字节，数据文件中记录数，
	unsigned uUserType;//37－40字节，帐户类型，0为普通帐户、1为沪B帐户、2为深B
	char chRes[8];//41－48字节，保留。
};


class CSuperviseInvest : public CDialog
{
// Construction
public:
	int m_nKind;
	static void AlarmCallInvest(SymbolKindArr & SymbolArray,CString AccountName,CString Password);
	double GetFeeRate(unsigned uKind,char chSymbol[]);
	double m_dLeftMoney;
	CString m_szFileName;
	AccountID m_Account;//帐户
	char m_Password[16];
	Invest* m_pInvest;//投资记录
	double m_ShRate[5][5]; //上海的费率
	double m_SzRate[5][5]; //深圳的费率

	void DisplayList( unsigned nTab);
	CSuperviseInvest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSuperviseInvest)
	enum { IDD = IDD_MANAGEINVEST };
	CButton	m_cancel;
	CButton	m_fee;
	CButton	m_edit;
	CButton	m_delete;
	CButton	m_add;
	CTabCtrl	m_TabCtrl;
	CMSFlexGrid	m_MsFlexGrid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSuperviseInvest)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSuperviseInvest)
	virtual BOOL OnInitDialog();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnAdd();
	afx_msg void OnFee();
	virtual void OnCancel();
	afx_msg void OnDblClickMsflexgrid1();
	afx_msg void OnKeyDownMsflexgrid1(short FAR* KeyCode, short Shift);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGEINVEST_H__E646B0F1_B52D_490A_BE68_4E2041A4BDC1__INCLUDED_)
