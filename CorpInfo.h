#if !defined(AFX_CORPINFO_H__4679F10A_E8B8_46FB_BE19_725D689C629B__INCLUDED_)
#define AFX_CORPINFO_H__4679F10A_E8B8_46FB_BE19_725D689C629B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CorpInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCorpInfo dialog

class CCorpInfo : public CPropertyPage
{
	DECLARE_DYNCREATE(CCorpInfo)

// Construction
public:
	void OutChuQuanData(CString FilePath);
	int GetChuQuanInfo(CString strStockCode,PSplit &pSplit);
	BOOL IsAlreadyChuQuan(int nSeconds);
	CCorpInfo();
	~CCorpInfo();
	CTaiShanDoc* pDoc;

// Dialog Data
	//{{AFX_DATA(CCorpInfo)
	enum { IDD = IDD_CORPINFO };
	CListCtrl	m_ctrlChuQuan;
	CString	m_strStockCode;
	//}}AFX_DATA
private: 
   int m_nChuQuanKind[80];

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCorpInfo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableButton(BOOL bEnable);
	void DeleteChuQuanInfo(CString strStockCode, int nWhichItem);
	void ModifyChuQuanInfo(CString strStockCode, int nWhichItem,POWER *pPower);
	void AddChuQuanInfo(CString strStockCode, POWER *pPower);
	void InsertItem(int nItem,CString strNo,POWER nPower);
	// Generated message map functions
	//{{AFX_MSG(CCorpInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeStockSymbol();
	afx_msg void OnAddItem();
	afx_msg void OnModifyCq();
	afx_msg void OnDeleteInfo();
	afx_msg void OnButtonLeadout();
	afx_msg void OnExport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CORPINFO_H__4679F10A_E8B8_46FB_BE19_725D689C629B__INCLUDED_)
