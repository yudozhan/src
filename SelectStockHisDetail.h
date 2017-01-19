#if !defined(AFX_SELECTSTOCKHISDETAIL_H__40D02050_767F_4D9C_91C5_EE6FFBAB6F58__INCLUDED_)
#define AFX_SELECTSTOCKHISDETAIL_H__40D02050_767F_4D9C_91C5_EE6FFBAB6F58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectStockHisDetail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectStockHisDetail dialog

class CSelectStockHisDetail : public CDialog
{
// Construction
public:
	CSelectStockHisDetail(CWnd* pParent = NULL);   // standard constructor
	CTaiShanDoc *m_pDoc;
	CImageList* m_pImageListSmall;
    CStringArray m_StockTypeNameArray;
	CStringArray m_FileArray;
// Dialog Data
	//{{AFX_DATA(CSelectStockHisDetail)
	enum { IDD = IDD_DIALOG1 };
	CListCtrl	m_SelectedStockList;
	CListCtrl	m_StockList;
	CListCtrl	m_StockTypeList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectStockHisDetail)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectStockHisDetail)
	virtual BOOL OnInitDialog();
	afx_msg void OnStockTypeClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnStockDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectedStockDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDel();
	afx_msg void OnAdd();
	virtual void OnOK();
	afx_msg void OnChangeday();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL InsertStockTypeToListExt(int nKind);
	BOOL InsertStockColumnToList();
	BOOL InsertStockTypeToList();
	BOOL InsertStockToList(char *name,char *code);
	BOOL QuickSort(int *nItem,int low,int high);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTSTOCKHISDETAIL_H__40D02050_767F_4D9C_91C5_EE6FFBAB6F58__INCLUDED_)
