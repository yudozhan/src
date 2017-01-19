#if !defined(AFX_SELECTSTOCK_H__1FAAAD21_2FCC_11D4_8829_00400552E583__INCLUDED_)
#define AFX_SELECTSTOCK_H__1FAAAD21_2FCC_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectStock.h : header file
//
class CTaiShanDoc;
/////////////////////////////////////////////////////////////////////////////
// SelectStock dialog

class SelectStock : public CDialog
{
public:
	int m_stkKind;
	bool m_bSingleSelect;
	int m_nCurTab;
	CListCtrl * m_pSelectResultList;
	bool m_bScreenStockBlock;
	CTaiShanDoc *m_pDoc;
	CImageList * m_pImageListSmall;
    CStringArray m_StockTypeNameArray;
	BOOL m_bOnlyAddStock;
	BOOL m_bCloseWindows; 
	CString m_strStockCode;


// Construction
public:
	static void DeleteSelectedItem(CListCtrl* pList);
	static void GetStockSymbolArray(CListCtrl *pList,SymbolKindArr& sSymbolArray);
	static void FillToListCtrlItem(CListCtrl* pList,char *Id,int nParam);
	BOOL InsertStockTypeToListExt(int nKind);
//	SelectStock(CWnd* pParent = NULL);  	// standard constructor
	SelectStock(CWnd* pParent = NULL, BOOL bOnlyAddStock=FALSE,bool bScreenStockBlock = false,BOOL bCloseWindows = false);  	// standard constructor
	SelectStock(UINT IDDLG, CWnd* pParent = NULL, BOOL bOnlyAddStock=FALSE,bool bScreenStockBlock = false,BOOL bCloseWindows = false);  	// standard constructor

	BOOL InsertStockTypeToList() ;          //将板块插入至LIST 
	BOOL InsertStockColumnToList();         //将列名插入至LIST
    BOOL InsertStockToList(char *name,char *code,int nKind);//将股票插入至LIST



// Dialog Data
	//{{AFX_DATA(SelectStock)
	enum { IDD = IDD_TYPE_STOCK };
	CButton	m_add;
	CButton	m_ok;
	CButton	m_cancel;
	CListCtrl	m_StockList;
	CListCtrl	m_StockTypeList;
	int		m_nRadio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SelectStock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SelectStock)
	afx_msg void OnClickTab(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnAdd();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTSTOCK_H__1FAAAD21_2FCC_11D4_8829_00400552E583__INCLUDED_)
