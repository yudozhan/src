#if !defined(CRANKING_GRID_CTRL__INCLUDED_)
#define CRANKING_GRID_CTRL__INCLUDED_

#pragma once

#include "GridCtrl.h"

#define MAX_RANKING_NUM     20  

class CRankingGridCtrl : public CGridCtrl
{
private:
	CReportData *	m_pGC_MapData[MAX_RANKING_NUM];	//股票行情数据结构,use for CRankingGridCtrl
	
public:
	CRankingGridCtrl();

public:
	virtual ~CRankingGridCtrl();
    BOOL GetCell(CGridCell* pGridCell,int nRow, int nCol) const;
    BOOL CRankingGridCtrl::SetPMdataPtr( CReportData **	m_pMapData );
    BOOL CRankingGridCtrl::DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk);

	// Generated message map functions
protected:
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#endif // CRANKING_GRID_CTRL__INCLUDED_