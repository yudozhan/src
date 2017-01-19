// BlockAnalysisGridCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "WH.h"
#include "BlockAnalysisGridCtrl.h"
#include "CTaiShanDoc.h"

////排序字符串
/////////////////////////////////////////////////////////////////////////////
// CBGridCtrl
void SwapMx(BLOCKINFO &pData1,BLOCKINFO &pData2)
{
	BLOCKINFO pData;
	memcpy(&pData,&pData1,sizeof(BLOCKINFO));
	memcpy(&pData1,&pData2,sizeof(BLOCKINFO));
	memcpy(&pData2,&pData,sizeof(BLOCKINFO));
}
BOOL QuickSortStringMx(BLOCKINFO *pBlockData,BOOL bAscending,int low,int high)
{
	int lo = low;
	int hi = high;
	BLOCKINFO pivot;

	if( hi <= lo ) return FALSE;

	int mid=(low +high)/2;
	pivot=pBlockData[mid];

	while( lo <= hi )
	{
		// Find the first element that is greater than or equal to the partition 
		// element starting from the left Index.
		if( bAscending )
		{
			while (lo < high && strcmp(pBlockData[lo].m_szName  , pivot.m_szName) < 0 )
			{
				++lo;
			}
		}
		else
		{
			while ((lo < high  && strcmp(pBlockData[lo].m_szName, pivot.m_szName) > 0))
			{
				++lo;
			}

		}

		// Find an element that is smaller than or equal to  the partition 
		// element starting from the right Index.
		if( bAscending )
		{
			while (hi > low && strcmp(pBlockData[hi].m_szName, pivot.m_szName) > 0 )
			{
				--hi;
			}
		}
		else
		{
			while (hi > low && strcmp(pBlockData[hi].m_szName, pivot.m_szName) < 0)
			{
				--hi;
			}
		}
		// If the indexes have not crossed, swap if the items are not equal
		if (lo <= hi)
		{
			// swap only if the items are not equal
			if ( pBlockData[lo].m_szName != pBlockData[hi].m_szName  )
			{
				SwapMx(pBlockData[lo],pBlockData[hi]);
			}

			++lo;
			--hi;
		}
	}

	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
		QuickSortStringMx(pBlockData, bAscending, low, hi);

	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
		QuickSortStringMx(pBlockData, bAscending, lo, high);

	return TRUE;
}

// CBlockAnalysisGridCtrl

IMPLEMENT_DYNAMIC(CBlockAnalysisGridCtrl, CGridCtrl)

CBlockAnalysisGridCtrl::CBlockAnalysisGridCtrl()
{

}

CBlockAnalysisGridCtrl::~CBlockAnalysisGridCtrl()
{
}


BEGIN_MESSAGE_MAP(CBlockAnalysisGridCtrl, CGridCtrl)
END_MESSAGE_MAP()



// CBlockAnalysisGridCtrl 消息处理程序



/////////////////////////////////////////////////////////////////////////////
// CNineGridCtrl message handlers
BOOL CBlockAnalysisGridCtrl::GetCell(CGridCell* pGridCell,int nRow, int nCol) const
{
	if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return FALSE;
	if(m_pDoc==NULL)		
	{
		return TRUE;
	}
	if(nRow==0)
	{
		pGridCell->state=0;       // Cell state (selected/focus etc)
		pGridCell->nFormat =DT_CENTER|DT_VCENTER|DT_SINGLELINE ;     // Cell format
		pGridCell->szText=m_pColData[nCol].szText;      // Cell text (or binary data if you wish...)
		pGridCell->iImage=-1;      // Index of the list view item抯 icon 
		pGridCell->crBkClr=m_pDoc->m_colorArray[18];  // Background colour (or CLR_DEFAULT)

		pGridCell->crFgClr=m_pDoc->m_colorArray[14];     // Forground colour (or CLR_DEFAULT)
		pGridCell->lParam=0;      // 32-bit value to associate with item
		pGridCell->lfFont=m_Logfont;     // Cell font
		return TRUE;
	}
	DWORD key= MAKELONG(nRow, nCol);
	if(nRow==1&&nCol==0)
		nCol=0;
    CCellID cell;
//    if((m_SortColumn==nCol||m_idCurrentCell.row==nRow)&&nCol!=0)
    if((m_SortColumn == nCol||m_idCurrentCell.row == nRow))
	{
        pGridCell->state = GVIS_SELECTED;       // Cell state (selected/focus etc)
	}
	else
	{
        pGridCell->state = 0;       // Cell state (selected/focus etc)
	}
    pGridCell->iImage = -1;      // Index of the list view item抯 icon 
    pGridCell->lParam = 0;      // 32-bit value to associate with item
    pGridCell->lfFont = m_Logfont;     // Cell font

    pGridCell->crBkClr = CLR_DEFAULT;//m_pDoc->m_colorArray[18];//CLR_DEFAULT;  // Background colour (or CLR_DEFAULT)
    pGridCell->nFormat = m_pColData[nCol].nFormat;
    
	if(nRow >= m_nRealRow)
	{
        pGridCell->szText = "";
		return TRUE;
	}
	char str[50];
	float m_temp;
    if(m_pDoc == NULL)		
	{
		return TRUE;
	}

	BLOCKINFO *pData = m_pDoc->m_pBlockDataShow + nRow - 1;
	if(pData == NULL )
	{
		return FALSE;
	}
	switch(m_pColData[nCol].nID)
	{
	    case ID_ROWNUM:
              sprintf(str,"%d",nRow - m_nFixedRows + 1);
			  pGridCell->szText = str;
			  pGridCell->crFgClr = RGB(255 - GetRValue(m_pDoc->m_colorArray[18]),255 - GetGValue(m_pDoc->m_colorArray[18]),255 - GetBValue(m_pDoc->m_colorArray[18])); 
			  //pGridCell->crFgClr = m_pDoc->m_colorArray[1]; 
			  break;
		case ID_STOCKNAME:                                      //名称
              sprintf(str,"%s",pData->m_szName);
			  pGridCell->crFgClr = m_pDoc->m_colorArray[20]; 
			  //pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			  pGridCell->szText = str;
			break;
		case ID_YSTC: 				//昨收
			  if (pData->m_fTotalRight == 0.0)
			  {
				  sprintf(str,"%6.2f",0);
			  }
			  else
			  {
				sprintf(str,"%6.2f",pData->m_fYstIndex/pData->m_fTotalRight * 1000);
			  }
			  pGridCell->szText = str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_OPEN:                                //今开
			if (pData->m_fTotalRight == 0.0)
			{
				sprintf(str,"%6.2f",0);
			}
			else
			{
				sprintf(str,"%6.2f",pData->m_fOpenIndex / pData->m_fTotalRight * 1000);
			}
			  pGridCell->szText =str;
			  if(pData->m_fOpenIndex > pData->m_fYstIndex )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pData->m_fOpenIndex < pData->m_fYstIndex)
				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_HIGH:                                //最高
			if (pData->m_fTotalRight == 0.0)
			{
				sprintf(str,"%6.2f",0);
			}
			else
			{
				sprintf(str,"%6.2f",pData->m_fHighIndex / pData->m_fTotalRight * 1000);
			}
			  pGridCell->szText = str;
			  if(pData->m_fHighIndex > pData->m_fYstIndex )
				 pGridCell->crFgClr = m_pDoc->m_colorArray[13]; 
			  else if(pData->m_fHighIndex < pData->m_fYstIndex)
				 pGridCell->crFgClr = m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr = m_pDoc->m_colorArray[14]; 
			break;
		case ID_LOW:                                 //最低
			if (pData->m_fTotalRight == 0.0)
			{
				sprintf(str,"%6.2f",0);
			}
			else
			{
              sprintf(str,"%6.2f",pData->m_fLowIndex / pData->m_fTotalRight * 1000);
			}
			  pGridCell->szText =str;
			  if(pData->m_fLowIndex > pData->m_fYstIndex )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pData->m_fLowIndex < pData->m_fYstIndex)
				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_NEW:                                 //最新
			if (pData->m_fTotalRight == 0.0)
			{
				sprintf(str,"%6.2f",0);
			}
			else
			{
              sprintf(str,"%6.2f",pData->m_fNewIndex / pData->m_fTotalRight * 1000);
			}
			  if(pData->m_fNewIndex > pData->m_fYstIndex )
				 pGridCell->crFgClr = m_pDoc->m_colorArray[13]; 
			  else if(pData->m_fNewIndex < pData->m_fYstIndex)
				 pGridCell->crFgClr = m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr = m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =str;
			break;
		case ID_TOTV:                                //总手
              sprintf(str,"%6.0f",pData->m_fTotV);
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_TOTP:                                //总额
              sprintf(str,"%6.2f",pData->m_fTotP / 10000  );
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_HLP:                                 //震幅
              if(pData->m_fYstIndex != 0)
			  {
                  sprintf(str,"%6.2f%s",100 * (pData->m_fHighIndex - pData->m_fLowIndex) /pData->m_fYstIndex ,"%" );
			  }
			  else
                 sprintf(str,"%6.2f%s",0.0f,"%" ); 
			  pGridCell->szText = str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_RDR:                                 //涨幅
			 if(pData->m_fYstIndex != 0 && pData->m_fNewIndex != 0)
			 {
               sprintf(str,"%6.2f%s",100 * (pData->m_fNewIndex - pData->m_fYstIndex ) / pData->m_fYstIndex ,"%"  );
			 }
			 else
               sprintf(str,"%6.2f%s",0.0f,"%" );
			  pGridCell->szText =str;
			  if(pData->m_fNewIndex > pData->m_fYstIndex )
				 pGridCell->crFgClr = m_pDoc->m_colorArray[13]; 
			  else if(pData->m_fNewIndex < pData->m_fYstIndex)
				 pGridCell->crFgClr = m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr = m_pDoc->m_colorArray[14]; 
			break;
		case ID_RVOL:                               //内盘
              sprintf(str,"%6.0f",pData->m_fTotDVol  );
			  pGridCell->szText = str;
			  pGridCell->crFgClr = m_pDoc->m_colorArray[15]; 
			break;
		case ID_DVOL:                              //外盘
              sprintf(str,"%6.0f",pData->m_fTotRVol   );
			  pGridCell->szText = str;
			  pGridCell->crFgClr = m_pDoc->m_colorArray[13]; 
			break;
        case ID_EXCHANGE:
			  if(pData->m_fLTP != 0)
	            sprintf(str,"%6.2f",pData->m_fTotV / pData->m_fLTP);
			  else
	            sprintf(str,"%6.2f",0.0f);
			  pGridCell->szText = str;
			  pGridCell->crFgClr = m_pDoc->m_colorArray[1]; 
			  break;
        case ID_SYL:
			 if(pData->m_fMGSY != 0 && pData->m_lStockCount != 0 && pData->m_fTotalRight != 0)
               sprintf(str,"%6.2f",pData->m_fNewIndex /(pData->m_lStockCount * pData->m_fTotalRight * pData->m_fMGSY) * 1000 ); 
			 else
		       sprintf(str,"%6.2f",0.0f);
			  pGridCell->szText = str;
			  pGridCell->crFgClr = m_pDoc->m_colorArray[1]; 
			  break;
		case ID_FIRSTSTK:
			pGridCell->szText = pData->m_szFirstStock;
			pGridCell->crFgClr = m_pDoc->m_colorArray[20]; 
			break;
		case ID_RSTKRATE:
			sprintf(str,"%d/%d",pData->m_lUpStockCount,pData->m_lStockCount); 
			pGridCell->szText = str;
			pGridCell->crFgClr = m_pDoc->m_colorArray[1]; 
			break;
		default:
      //         if(m_pColData[nCol].nID>=9000&&m_pColData[nCol].nID<9010)
			   //{
      //             sprintf(str,"%6.2f",pData->Index[m_pColData[nCol].nID - 9000]);
			   //    pGridCell->szText =str;
				  // pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			   //}
			break;
		}
	return TRUE;
}


float CBlockAnalysisGridCtrl::GetItemTextV(int nRow,int nCol)
{
	if(m_pDoc == NULL)		
		return TRUE;
	BLOCKINFO *pData = m_pDoc->m_pBlockDataShow + nRow - 1;
	if(pData == NULL)
		return 0;
	switch(m_pColData[nCol].nID)
	{
	case ID_YSTC: 				//昨收
		return pData->m_fYstIndex; 
		break;
	case ID_OPEN:                                //今开
		return pData->m_fOpenIndex; 
		break;
	case ID_HIGH:                                //最高
		return pData->m_fHighIndex; 
		break;
	case ID_LOW:                                 //最低
		return pData->m_fLowIndex; 
		break;
	case ID_NEW:                                 //最新
		return pData->m_fNewIndex;
		break;
	case ID_TOTV:                                //总手
		return pData->m_fTotV; 
		break;
	case ID_TOTP:                                //总额
		return pData->m_fTotP / 10000; 
		break;
	case ID_HLP:                                 //震幅
		if(pData->m_fYstIndex != 0)
		{
			return 100 * (pData->m_fHighIndex - pData->m_fLowIndex) /pData->m_fYstIndex;
		}
		else
			return 0.0; 
		break;
	case ID_RDR:                                 //涨幅
		if(pData->m_fYstIndex != 0 && pData->m_fNewIndex != 0)
		{
			return 100 * (pData->m_fNewIndex - pData->m_fYstIndex ) / pData->m_fYstIndex;
		}
		else
			return 0.0; 
		break;
	case ID_RVOL:                               //内盘
		return pData->m_fTotDVol; 
		break;
	case ID_DVOL:                              //外盘
		return pData->m_fTotRVol; 
		break;
	case ID_EXCHANGE:
		if(pData->m_fLTP != 0)
			return pData->m_fTotV / pData->m_fLTP;
		else
			return 0.0; 
		break;
	case ID_SYL:
		if(pData->m_fMGSY != 0)
			return pData->m_fNewIndex / pData->m_fMGSY; 
		else
			return 0.0f;
		break;
	case ID_RSTKRATE:
		return pData->m_lUpStockCount; 
		break;
	default:
		//         if(m_pColData[nCol].nID>=9000&&m_pColData[nCol].nID<9010)
		//{
		//             sprintf(str,"%6.2f",pData->Index[m_pColData[nCol].nID - 9000]);
		//    pGridCell->szText =str;
		// pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
		//}
		break;
	}
}

BOOL CBlockAnalysisGridCtrl::SetRowCount(int nRows)
{
	ASSERT(nRows >= 0);
	if (nRows == GetRowCount()) return TRUE;

	if (nRows < m_nFixedRows) 
		m_nFixedRows = nRows;

	if (m_idCurrentCell.row >= nRows)
		SetFocusCell(-1,-1);

	int addedRows = nRows - GetRowCount();
	m_nRows = nRows;
	m_arRowHeights.SetSize(nRows);

	// If we have just added rows, we need to construct new elements for each cell
	// and set the default row height
	if (addedRows > 0)
	{
		// initialize row heights and data
		int startRow = nRows - addedRows;
		for (int row = startRow; row < GetRowCount(); row++)
		{
			m_arRowHeights[row] = m_nDefCellHeight;
		}
	}
	if (GetSafeHwnd() && m_bAllowDraw)
	{
		ResetScrollBars();
		Invalidate();
	}
	return TRUE;
}

BOOL CBlockAnalysisGridCtrl::SortTextItems(int nCol, BOOL bAscending, int low, int high)
{
	if (nCol >= GetColumnCount()) return FALSE;

	if (high == -1) high =m_nRealRow-1;// GetRowCount() - 1;

	int lo = low;
	int hi = high;

	if( hi <= lo ) return FALSE;

	double niMid=GetItemTextV((lo+hi)/2, nCol);
	double nistr;
	// loop through the list until indices cross
	while( lo <= hi )
	{
		// Find the first element that is greater than or equal to the partition 
		// element starting from the left Index.
		if( bAscending )
		{
			nistr=GetItemTextV(lo, nCol);
			while (lo < high && nistr<niMid)
			{
				++lo;
				nistr=GetItemTextV(lo, nCol);
			}
		}
		else
		{
			nistr=GetItemTextV(lo, nCol);
			while ((lo < high  && nistr>niMid))
			{
				++lo;
				nistr=GetItemTextV(lo, nCol);
			}

		}

		// Find an element that is smaller than or equal to  the partition 
		// element starting from the right Index.
		if( bAscending )
		{
			nistr=GetItemTextV(hi, nCol);
			while (hi > low && nistr>niMid)
			{
				--hi;
				nistr=GetItemTextV(hi, nCol);
			}
		}
		else
		{
			nistr=GetItemTextV(hi,nCol);
			while (hi > low && nistr<niMid)
			{
				--hi;
				nistr=GetItemTextV(hi, nCol);
			}
		}
		// If the indexes have not crossed, swap if the items are not equal
		if (lo <= hi)
		{
			// swap only if the items are not equal
			if (GetItemTextV(lo, nCol) != GetItemTextV(hi, nCol))
			{
				SwapMx(m_pDoc->m_pBlockDataShow[lo -1],m_pDoc->m_pBlockDataShow[hi -1] );
				UINT nRowHeight = m_arRowHeights[lo];
				m_arRowHeights[lo] = m_arRowHeights[hi];
				m_arRowHeights[hi] = nRowHeight;
			}
			++lo;
			--hi;
		}
	}

	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
		SortTextItems(nCol, bAscending, low, hi);

	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
		SortTextItems(nCol, bAscending, lo, high);

	return TRUE;
}