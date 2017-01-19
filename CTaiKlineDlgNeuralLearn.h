#if !defined(AFX_TECHDLGNEURALLEARN_H__C54E43A3_67B1_11D4_970B_0080C8D6450E__INCLUDED_)
#define AFX_TECHDLGNEURALLEARN_H__C54E43A3_67B1_11D4_970B_0080C8D6450E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CTaiKlineDlgNeuralLearn.h : header file
//
struct STOCK_DATA_SELECTED{
	CString m_symbol;
	CTime m_tmBegin;
	CTime m_tmEnd;
	int		m_stkKind;
};
#include "TechNnetBP.h"
#include "FloatEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgNeuralLearn dialog

class CTaiKlineDlgNeuralLearn : public CDialog
{
// Construction
public:
	CArray<InKlineData,InKlineData> fInArray;
	bool m_bPrepared;
	int m_maxNeededOut;
	int m_maxNeeded;

	void SetEnable(BOOL bEnable = TRUE);
	bool m_bExecuting;
	CTaiKlineNnetBP m_NnetBP;
	CTaiKlineDlgNeuralLearn(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTaiKlineDlgNeuralLearn)
	enum { IDD = IDD_7_NNET_TRAIN };
	CButton	m_ok;
	CButton	m_cancel;
	CButton	m_btn6;
	CButton	m_btn5;
	CButton	m_btn4;
	CButton	m_btn3;
	CButton	m_btn2;
	CButton	m_btn1;
	CButton	m_add;
	CDateTimeCtrl	m_conTmEnd;
	CDateTimeCtrl	m_conTmBegin;
	CFloatEdit	m_floatEdit2;
	CFloatEdit	m_floatEdit1;
	CFloatEdit	m_floatEditDiff;
	CFloatEdit	m_floatEditMaxCount;
	CFloatEdit	m_floatEditReal;
	CProgressCtrl	m_progress;
	CFloatEdit	m_conDays;
	CComboBox	m_conPeriod;
	CListBox	m_conStockData;
	CString	m_sStockData;
	int		m_nDays;
	CTime	m_tmBegin;
	CTime	m_tmEnd;
	CString	m_sOutput;
	CString	m_sInput;
	BOOL	m_bPreDealData;
	int		m_nMaxCount;
	float	m_fDiff;
	int		m_nReal;
	int		m_nTimesTrain;
	CString	m_sFileName;
	CString	m_sTrainInfo;
	int		m_nDaysManyDay;
	int		m_nDaysDeleted;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiKlineDlgNeuralLearn)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
//	afx_msg void OnAddStock();//加入股票

	// Generated message map functions
	//{{AFX_MSG(CTaiKlineDlgNeuralLearn)
	afx_msg void OnTrain();//开始训练
	afx_msg void OnRestoreWeight();//恢复缺省知识库信息
	afx_msg void OnButtonSelectInput();
	afx_msg void OnButtonSelectOutput();
	afx_msg void OnButtonDeleteStock();
	virtual BOOL OnInitDialog();
	afx_msg void OnAddStock();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnButtonEditData();
	afx_msg void OnButtonPrepareData();
	afx_msg void OnSelchangeComboPeriod();
	afx_msg void OnChangeEditPreDays();
	afx_msg void OnChangeEditIn();
	afx_msg void OnChangeEditOut();
	//}}AFX_MSG
	afx_msg LRESULT GetStockSymbolList(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void AddListItem(CString symbol,int stkKind);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TECHDLGNEURALLEARN_H__C54E43A3_67B1_11D4_970B_0080C8D6450E__INCLUDED_)
