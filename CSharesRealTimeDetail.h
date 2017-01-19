#if !defined(AFX_STOCKREALTIMEDETAIL_H__D99F64A4_B550_11D4_882E_00400552E583__INCLUDED_)
#define AFX_STOCKREALTIMEDETAIL_H__D99F64A4_B550_11D4_882E_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSharesRealTimeDetail.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSharesRealTimeDetail dialog

class CSharesRealTimeDetail : public CDialog
{
// Construction
public:
	CSharesRealTimeDetail(CWnd* pParent /*=NULL*/,CTaiShanKlineShowView *m_pVwbaseView,BOOL IsF1);   // standard constructor
    CSharesDetailGridCtrl *m_pGrid;
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
	CCellRange VisibleCells;
    int m_VisibleRowCount;
    CTaiShanDoc *m_pDoc;
	CTaiShanKlineShowView *m_pView;

    BOOL m_IsF1;
	BOOL m_bFirst;
	BOOL m_bFirstDisplay;

	void InitGridColumnTitle();     //��ʼ��GridCtrl����
    int InitGridCtrl();   	//��ʼ��GridCtrl
	BOOL InitGridContent();  //��ʼ��Grid
    void ShowStockDetail(BOOL bFirstDisplay);
    void ShowStockDetailF2(BOOL bFirstDisplay);


// Dialog Data
	//{{AFX_DATA(CSharesRealTimeDetail)
	enum { IDD = IDD_DETAIL_STOCK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSharesRealTimeDetail)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSharesRealTimeDetail)
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOCKREALTIMEDETAIL_H__D99F64A4_B550_11D4_882E_00400552E583__INCLUDED_)
