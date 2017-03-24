#pragma once

#include "RankingGridCtrl.h"

// CDialogCustomizeRanking 对话框

class CDialogCustomizeRanking : public CDialog
{
	DECLARE_DYNAMIC(CDialogCustomizeRanking)

public:
	CDialogCustomizeRanking(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogCustomizeRanking();

public:
	//CGridCtrl *m_pGrid;  //define CGrid pointer
	CRankingGridCtrl *m_pGrid;  //define CGrid pointer
 	int     m_nCols; //the total num of column, if user delete or add a column , it will ++ / --
	int		m_nRows; //the total num of rows.
	int		m_nFixCols;   //固定列
	int		m_nFixRows;   //固定行

	BOOL	m_bListMode;   //是否为显示模式
	CStringArray m_ColumnTitleArray; //define an array to keep the column title string

    CTaiShanDoc *m_pDoc;

    void InitGridColumnTitle();     //初始化GridCtrl列名
    int InitGridCtrl();             //初始化GridCtrl
    BOOL InitGridContent();         //初始化Grid

    CReportData *   m_pMapData;     //股票行情数据结构
    long    totalCount;
    CReportData *  m_pRankingData[MAX_RANKING_NUM];

// 对话框数据
	enum { IDD = IDD_DIALOG_CUSTOMIZE_RANKING };

protected:
    virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
    void UpdateRanking() ;
    BOOL needIgnore(CReportData * m_pbData);
    BOOL isExistInRankArray(CReportData * m_pbData); 
    BOOL needSwap(CReportData * m_pbData1, CReportData * m_pbData2);
};
