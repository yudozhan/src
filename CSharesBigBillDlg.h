#if !defined(AFX_STOCKCJMXTJ_H__7B83B361_24BB_11D4_8829_00400552E583__INCLUDED_)
#define AFX_STOCKCJMXTJ_H__7B83B361_24BB_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSharesBigBillDlg.h : header file
//
#define MOUSE_ALL  7001
#define MOUSE_COL  7002
#define MOUSE_ROW  7003
#define MOUSE_CELLS 7004
#define MOUSE_NOTHINGS 7005

/////////////////////////////////////////////////////////////////////////////
// CSharesBigBillDlg dialog
class BCridCtrl;
class CSharesBigBillDlg : public CDialog
{
// Construction
public:
	CSharesBigBillDlg(CWnd* pParent = NULL);   // standard constructor
	~CSharesBigBillDlg();

public:
	CBGridCtrl *m_pGrid;  //define CGrid pointer
 	int     m_nCols; //the total num of column, if user delete or add a column , it will ++ / --
	int		m_nRows; //the total num of rows.
	int		m_nFixCols;   //固定列
	int		m_nFixRows;   //固定行
	int     m_nBigBillCount; //大单明细数
	int     m_nRealRow;     //大单明细数实际行

	BOOL	m_bEditable;   //是否可以编辑
	BOOL	m_bListMode;   //是否为显示模式
	BOOL	m_bSelectable; //是否为多选 
	BOOL	m_bReadOnly;   //是否为只读
	CRect   m_rcBar;
	int     m_nStartColumn;  //开始列
	int     m_nVisibleRowCount;  //可视部分行数
	CStringArray m_ColumnTitleArray; //define an array to keep the column title string
	CCellID m_CellTitleCommand;        //
	CImageList * m_pImageList;    //大图象列表
	CImageList * m_pImageListSmall; //小图象列表
	CCellRange VisibleCells;
    int m_VisibleRowCount;
	BOOL m_bIsCalc;




    CTaiShanDoc *m_pDoc;
	void InitGridColumnTitle();     //初始化GridCtrl列名
    int InitGridCtrl();   	//初始化GridCtrl
	BOOL InitGridContent();  //初始化Grid
	void FillToListCtrlHead();  //将列名插入LIST
	void FillToListCtrlItem(char *Id,int nParam,BOOL StockType); //将股票插入LISTCTRL
	void DisplayInfomation();  //动态显示统计信息
	void InitBigBillData();       //初始化成交明细数据
	void AddGridRow(int CurrentRow );  //增加Grid行
	void EnableVisibleChildren();      //使扩展对话框中控件有效
	void ExpandDialog(BOOL nDisplayTj,BOOL nDisplayAll); //扩展对话框
	void GetBigBillDataToday();
    void GetBigBillDataHistory();
	void SetGridRealRowData();
	void InitBigBillFileData();
    LRESULT OnGridColorChange(WPARAM wParam, LPARAM lParam);



// Dialog Data
	//{{AFX_DATA(CSharesBigBillDlg)
	enum { IDD = IDD_DIALOG6 };
	CButton	m_ok;
	CButton	m_cancel;
	CButton	m_setup;
	CButton	m_delete;
	CButton	m_calc;
	CButton	m_add;
	CEdit	m_edit;
	CDateTimeCtrl	m_EndCtrl;
	CDateTimeCtrl	m_StartCtrl;
	CListCtrl	m_lstStockTypeName;
	BOOL	m_Check1;
	BOOL	m_Check2;
	BOOL	m_Check3;
	BOOL	m_Check4;
	int		m_Radio1;
	CTime	m_Start;
	CTime	m_End;
	BOOL	m_check5;
	BOOL	m_check6;
	BOOL	m_stop;
	BOOL	m_bSaveData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSharesBigBillDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSharesBigBillDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDelete();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCalc();
	afx_msg void OnCheck5();
	afx_msg void OnCheck6();
	afx_msg void OnSetup();
	afx_msg void OnCheck1();
	virtual void OnCancel();
	virtual void OnAdd();
	afx_msg void OnStop();
	afx_msg void OnSaveBigBillData();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnAddStock(WPARAM wParam, LPARAM lParam); //增加股票至LISTCTRL

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOCKCJMXTJ_H__7B83B361_24BB_11D4_8829_00400552E583__INCLUDED_)
