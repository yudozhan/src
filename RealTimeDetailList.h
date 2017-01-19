#if !defined(AFX_REALTIMEDETAILLIST_H__D99F64A3_B550_11D4_882E_00400552E583__INCLUDED_)
#define AFX_REALTIMEDETAILLIST_H__D99F64A3_B550_11D4_882E_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealTimeDetailList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRealTimeDetailList dialog

class CRealTimeDetailList : public CDialog
{
// Construction

public:
    CRealTimeDetailList(CWnd* pParent /*=NULL*/,CTaiShanKlineShowView *m_pVwbaseView,BOOL IsF1);
    CDetailGridCtrl *m_pGrid;
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
    void Show_1Min(BOOL IsFirstDisplay=FALSE);
    void Show_5Min(BOOL IsFirstDisplay=FALSE);
// Dialog Data
	//{{AFX_DATA(CRealTimeDetailList)
	enum { IDD = IDD_DETAIL_HS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRealTimeDetailList)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRealTimeDetailList)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALTIMEDETAILLIST_H__D99F64A3_B550_11D4_882E_00400552E583__INCLUDED_)
