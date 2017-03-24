#pragma once

#include "RankingGridCtrl.h"

// CDialogCustomizeRanking �Ի���

class CDialogCustomizeRanking : public CDialog
{
	DECLARE_DYNAMIC(CDialogCustomizeRanking)

public:
	CDialogCustomizeRanking(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogCustomizeRanking();

public:
	//CGridCtrl *m_pGrid;  //define CGrid pointer
	CRankingGridCtrl *m_pGrid;  //define CGrid pointer
 	int     m_nCols; //the total num of column, if user delete or add a column , it will ++ / --
	int		m_nRows; //the total num of rows.
	int		m_nFixCols;   //�̶���
	int		m_nFixRows;   //�̶���

	BOOL	m_bListMode;   //�Ƿ�Ϊ��ʾģʽ
	CStringArray m_ColumnTitleArray; //define an array to keep the column title string

    CTaiShanDoc *m_pDoc;

    void InitGridColumnTitle();     //��ʼ��GridCtrl����
    int InitGridCtrl();             //��ʼ��GridCtrl
    BOOL InitGridContent();         //��ʼ��Grid

    CReportData *   m_pMapData;     //��Ʊ�������ݽṹ
    long    totalCount;
    CReportData *  m_pRankingData[MAX_RANKING_NUM];

// �Ի�������
	enum { IDD = IDD_DIALOG_CUSTOMIZE_RANKING };

protected:
    virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
    void UpdateRanking() ;
    BOOL needIgnore(CReportData * m_pbData);
    BOOL isExistInRankArray(CReportData * m_pbData); 
    BOOL needSwap(CReportData * m_pbData1, CReportData * m_pbData2);
};
