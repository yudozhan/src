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

//Ͷ�ʼ�¼���������׺��ʽ�
struct Invest
{
	unsigned uType;//����=0�������ʽ�=1
	union
	{
		struct _Transact
		{
			unsigned uTime;//����ʱ��
			int nWay;//��������Ϊ+1������Ϊ-1
			char chSymbol[8];//����
			int nKind;//��Ʊ����//
			char chName[10];//����
			double dPrice;//�۸�
			double dVolume;//�ɽ���
			double dFee;//������
		}Transact;
		struct _Money
		{
			unsigned uTime;//ʱ��
			int nWay;//���򣬴���Ϊ+1��ȡ��Ϊ-1
			double dAmount;//���
		}Money;
	};
};

//�ʻ�ID
struct AccountID
{
	char chName[16];//01��16�ֽڣ��û�����
	char chPassword[16];//17��32�ֽڣ�PASSWORD��
	unsigned uRecords;//33��36�ֽڣ������ļ��м�¼����
	unsigned uUserType;//37��40�ֽڣ��ʻ����ͣ�0Ϊ��ͨ�ʻ���1Ϊ��B�ʻ���2Ϊ��B
	char chRes[8];//41��48�ֽڣ�������
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
	AccountID m_Account;//�ʻ�
	char m_Password[16];
	Invest* m_pInvest;//Ͷ�ʼ�¼
	double m_ShRate[5][5]; //�Ϻ��ķ���
	double m_SzRate[5][5]; //���ڵķ���

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
