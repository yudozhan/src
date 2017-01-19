// CBGridCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiShanDoc.h"
#include "BGridCtrl.h"
#include "CSharesBigBillDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBGridCtrl
void SwapMx(BIG_LIST &m_pData1,BIG_LIST &m_pData2)
{
     BIG_LIST pData;
	 memcpy(&pData,&m_pData1,sizeof(BIG_LIST));
	 memcpy(&m_pData1,&m_pData2,sizeof(BIG_LIST));
	 memcpy(&m_pData2,&pData,sizeof(BIG_LIST));
}
BOOL QuickSortStringMx(BIG_LIST *m_pStockData,BOOL bAscending,int low,int high)
{
    int lo = low;
    int hi = high;
    BIG_LIST pivot;

    if( hi <= lo ) return FALSE;

	int mid=(low +high)/2;
	pivot=m_pStockData[mid];

    while( lo <= hi )
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        if( bAscending )
		{
            while (lo < high && strcmp(m_pStockData[lo].id  , pivot.id)<0 )
			{
 				++lo;
			}
		}
        else
		{
            while ((lo < high  && strcmp(m_pStockData[lo].id, pivot.id)>0))
			{
 				++lo;
			}
		
		}

        // Find an element that is smaller than or equal to  the partition 
        // element starting from the right Index.
        if( bAscending )
		{
			while (hi > low && strcmp(m_pStockData[hi].id, pivot.id)>0 )
			{
 				--hi;
			}
		}
        else
		{
            while (hi > low && strcmp(m_pStockData[hi].id, pivot.id)<0)
			{
                --hi;
			}
		}
        // If the indexes have not crossed, swap if the items are not equal
        if (lo <= hi)
        {
            // swap only if the items are not equal
            if ( m_pStockData[lo].id != m_pStockData[hi].id  )
            {
				SwapMx(m_pStockData[lo],m_pStockData[hi]);
            }

            ++lo;
            --hi;
        }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if( low < hi )
        QuickSortStringMx(m_pStockData, bAscending, low, hi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( lo < high )
        QuickSortStringMx(m_pStockData, bAscending, lo, high);

    return TRUE;
}

CBGridCtrl::CBGridCtrl()
{
}

CBGridCtrl::~CBGridCtrl()
{
	m_nRealRow=0;
}


BEGIN_MESSAGE_MAP(CBGridCtrl, CGridCtrl)
	//{{AFX_MSG_MAP(CBGridCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
    ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBGridCtrl message handlers
BOOL CBGridCtrl::GetCell(CGridCell* pGridCell,int nRow, int nCol) const
{
    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return FALSE;

    if(nRow==0)
	{
        pGridCell->state=0;       // Cell state (selected/focus etc)
		pGridCell->nFormat =DT_CENTER|DT_VCENTER|DT_SINGLELINE;     // Cell format
		pGridCell->szText=m_pColData[nCol].szText;      // Cell text (or binary data if you wish...)
		pGridCell->iImage=-1;      // Index of the list view item抯 icon 
	    pGridCell->crBkClr=m_pDoc->m_colorArray[18];  // Background colour (or CLR_DEFAULT)
		
		pGridCell->crFgClr=m_pDoc->m_colorArray[14];     // Forground colour (or CLR_DEFAULT)
		pGridCell->lParam=0;      // 32-bit value to associate with item
		pGridCell->lfFont=m_Logfont;     // Cell font
		return TRUE;
	}

	DWORD key= MAKELONG(nRow, nCol);
    CCellID cell;
    if((m_SortColumn==nCol||m_idCurrentCell.row==nRow))
	{
        pGridCell->state=GVIS_SELECTED;       // Cell state (selected/focus etc)
	}
	else
	{
        pGridCell->state=0;       // Cell state (selected/focus etc)
	}

    pGridCell->iImage=-1;      // Index of the list view item抯 icon 
    pGridCell->lParam=0;      // 32-bit value to associate with item
    pGridCell->lfFont=m_Logfont;     // Cell font

    pGridCell->crBkClr=CLR_DEFAULT;  // Background colour (or CLR_DEFAULT)
    pGridCell->nFormat=m_pColData[nCol].nFormat;
    
	if(nRow >= m_nRealRow)
	{
        pGridCell->szText="";
		return TRUE;
	}
	char str[50];
    BIG_LIST *pCjsj=m_pDoc->m_pStockBigBillData->m_pMapData+nRow-1;
	switch(m_pColData[nCol].nID)
	{
	    case 0:
              sprintf(str,"%s",pCjsj->id);
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  break;
		case 1:                                        //代码
              sprintf(str,"%s",pCjsj->name);
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  break;
		case 2:  
			{
			  CString ntime;
			  if(pCjsj->ntime<0)
				  ntime="";
			  else
				  ntime=GetTimeGrid(pCjsj->ntime);
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =ntime;
			}
			break;
		case 3: 				//昨收
              sprintf(str,"%6.2f",pCjsj->ntype  );
			  switch(pCjsj->ntype)
			  {
				  case 0:
					  strcpy(str,"大单买入");
					  break;
				  case 1:
					  strcpy(str,"大单卖出");
					  break;
				  case 2:
					  strcpy(str,"大单挂买");
					  break;
				  case 3:
					  strcpy(str,"大单挂卖");
					  break;
			  }
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =str;
			break;
		case 4:                                //今开
              sprintf(str,"%6.2f",pCjsj->nowv  );
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =str;
			break;
		case 5:                                //最高
              sprintf(str,"%6.2f",pCjsj->nowp  );
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =str;
			break;
		case 6:                                 //最低
              sprintf(str,"%6.2f%s",pCjsj->rdr*100,"%"  );
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =str;
			break;
		}
   return TRUE;
}
BOOL CBGridCtrl::SetRowCount(int nRows)
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
float CBGridCtrl::GetItemTextV(int nRow,int nCol)
{
    BIG_LIST *pCjsj=m_pDoc->m_pStockBigBillData->m_pMapData+nRow-1;

	switch(m_pColData[nCol].nID)
	{
		case 1:                                      
              return 0;
			break;
		case 2:                                      
              return float(pCjsj->ntime);
			break;
		case 3: 				
			return float(pCjsj->ntype);
			break;
		case 4:                                
			return pCjsj->nowv ;
			break;
		case 5:                                
			return pCjsj->nowp  ;
			break;
		case 6:                                 
              return pCjsj->rdr ;
			break;
		}
	return 0;
}
BOOL CBGridCtrl::SortTextItems(int nCol, BOOL bAscending, int low, int high)
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
				SwapMx(m_pDoc->m_pStockBigBillData->m_pMapData[lo -1],m_pDoc->m_pStockBigBillData->m_pMapData[hi -1] );
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
void CBGridCtrl::QuickSortString()
{
    QuickSortStringMx(m_pDoc->m_pStockBigBillData->m_pMapData,TRUE,0,m_nRealRow-2);
}
CString CBGridCtrl::GetTimeGrid(time_t ntime)const
{
  CTime m_Time = ntime;
  return m_Time.Format("%Y%m%d%H%M");
}
void CBGridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    HWND hOldFocusWnd = ::GetFocus();
////////////////////////

//
	nFlags=1;
	BOOL m_IsSort=FALSE;
	if(m_SortColumn!=-1)
       m_IsSort=TRUE;
	if(m_idCurrentCell.row>0)
	   m_CurrSortCell=m_idCurrentCell ;
	m_CurrrSortClick=FALSE;
    m_BeforeRow=m_idCurrentCell.row;

///////////////////////


    m_LeftClickDownPoint = point;
    m_LeftClickDownCell = GetCellFromPt(point);
    if (!IsValid(m_LeftClickDownCell)) return;

    m_SelectionStartCell = (nFlags & MK_SHIFT)? m_idCurrentCell : m_LeftClickDownCell;

    SetFocus();        // Auto-destroy any InPlaceEdit's

    // If the user clicks on the current cell, then prepare to edit it.
    // (If the user moves the mouse, then dragging occurs)
    if (m_LeftClickDownCell == m_idCurrentCell)
    {
        m_MouseMode = MOUSE_PREPARE_EDIT;
		return;
    } else {
        SetFocusCell(-1,-1);
        SetFocusCell(max(m_LeftClickDownCell.row, m_nFixedRows),
                    max(m_LeftClickDownCell.col, m_nFixedCols));
    }

    // If the user clicks on a selected cell, then prepare to drag it.
    // (If the user moves the mouse, then dragging occurs)
    if (m_bAllowDragAndDrop && hOldFocusWnd == GetSafeHwnd() && 
        GetItemState(m_LeftClickDownCell.row, m_LeftClickDownCell.col) & GVNI_SELECTED)
    {
        m_MouseMode = MOUSE_PREPARE_DRAG;
        return;
    }


    SetCapture();

    if (m_MouseMode == MOUSE_OVER_COL_DIVIDE) // sizing column
    {
        m_MouseMode = MOUSE_SIZING_COL;
        CPoint start;
        if (!GetCellOrigin(0, m_LeftClickDownCell.col, &start)) return;

        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(point.x, rect.top, point.x + 2, rect.bottom);

        CDC* pDC = GetDC();
        if (pDC) {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        if (point.x - start.x <= m_nResizeCaptureRange)        // clicked right of border
            if (!GetCellOrigin(0, --m_LeftClickDownCell.col, &start)) return;

        rect.left = start.x;
        ClientToScreen(rect);
        ClipCursor(rect);
    }
    else if (m_MouseMode == MOUSE_OVER_ROW_DIVIDE) // sizing row
    {
        m_MouseMode = MOUSE_SIZING_ROW;
        CPoint start;
        if (!GetCellOrigin(m_LeftClickDownCell, &start)) return;

        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(rect.left, point.y, rect.right, point.y + 2);

        CDC* pDC = GetDC();
        if (pDC) {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        if (point.y - start.y <= m_nResizeCaptureRange)            // clicked below border
            if (!GetCellOrigin(--m_LeftClickDownCell.row, 0, &start)) return;

        rect.top = start.y;
        ClientToScreen(rect);
        ClipCursor(rect);
    }
    else // not sizing or editing -- selecting
    {    
        // If Ctrl pressed, save the current cell selection. This will get added
        // to the new cell selection at the end of the cell selection process
		if (m_LeftClickDownCell.row < GetFixedRowCount())
		{
			//there, will sort 
	        m_CurrrSortClick=TRUE;
            OnFixedRowClick(m_LeftClickDownCell);
		}
        else if (m_LeftClickDownCell.col < GetFixedColumnCount())
		{
	        m_CurrrSortClick=FALSE;
			OnFixedColumnClick(m_LeftClickDownCell);
		}
        else
        {
	        m_CurrrSortClick=FALSE;
            m_MouseMode = m_bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
            OnSelecting(m_LeftClickDownCell);
        }

        m_nTimerID = SetTimer(WM_LBUTTONDOWN, m_nTimerInterval, 0);
    }   
    m_LastMousePoint = point;
    m_CurrSortCell=m_idCurrentCell ;
}
