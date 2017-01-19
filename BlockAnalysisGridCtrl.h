#pragma once

#include "GridCtrl.h"

class CBlockAnalysisView;
// CBlockAnalysisGridCtrl

class CBlockAnalysisGridCtrl : public CGridCtrl
{
	DECLARE_DYNAMIC(CBlockAnalysisGridCtrl)

	friend class CBlockAnalysisView;

public:
	CBlockAnalysisGridCtrl();
	virtual ~CBlockAnalysisGridCtrl();

	BOOL GetCell(CGridCell* pGridCell,int nRow, int nCol) const;
	BOOL SetRowCount(int nRows = 10);
	float GetItemTextV(int nRow,int nCol);
	BOOL SortTextItems(int nCol, BOOL bAscending, int low, int high);



protected:
	DECLARE_MESSAGE_MAP()
};


