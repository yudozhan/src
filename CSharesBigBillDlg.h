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
	int		m_nFixCols;   //�̶���
	int		m_nFixRows;   //�̶���
	int     m_nBigBillCount; //����ϸ��
	int     m_nRealRow;     //����ϸ��ʵ����

	BOOL	m_bEditable;   //�Ƿ���Ա༭
	BOOL	m_bListMode;   //�Ƿ�Ϊ��ʾģʽ
	BOOL	m_bSelectable; //�Ƿ�Ϊ��ѡ 
	BOOL	m_bReadOnly;   //�Ƿ�Ϊֻ��
	CRect   m_rcBar;
	int     m_nStartColumn;  //��ʼ��
	int     m_nVisibleRowCount;  //���Ӳ�������
	CStringArray m_ColumnTitleArray; //define an array to keep the column title string
	CCellID m_CellTitleCommand;        //
	CImageList * m_pImageList;    //��ͼ���б�
	CImageList * m_pImageListSmall; //Сͼ���б�
	CCellRange VisibleCells;
    int m_VisibleRowCount;
	BOOL m_bIsCalc;




    CTaiShanDoc *m_pDoc;
	void InitGridColumnTitle();     //��ʼ��GridCtrl����
    int InitGridCtrl();   	//��ʼ��GridCtrl
	BOOL InitGridContent();  //��ʼ��Grid
	void FillToListCtrlHead();  //����������LIST
	void FillToListCtrlItem(char *Id,int nParam,BOOL StockType); //����Ʊ����LISTCTRL
	void DisplayInfomation();  //��̬��ʾͳ����Ϣ
	void InitBigBillData();       //��ʼ���ɽ���ϸ����
	void AddGridRow(int CurrentRow );  //����Grid��
	void EnableVisibleChildren();      //ʹ��չ�Ի����пؼ���Ч
	void ExpandDialog(BOOL nDisplayTj,BOOL nDisplayAll); //��չ�Ի���
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
	afx_msg LRESULT OnAddStock(WPARAM wParam, LPARAM lParam); //���ӹ�Ʊ��LISTCTRL

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOCKCJMXTJ_H__7B83B361_24BB_11D4_8829_00400552E583__INCLUDED_)
