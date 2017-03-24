#include "stdafx.h"
#include "WH.h"
#include "mainfrm.h"


#include "RankingGridCtrl.h"

CRankingGridCtrl::CRankingGridCtrl()
{
    LOGS("CRankingGridCtrl::CRankingGridCtrl(), sizeof(m_pGC_MapData)=%d \n", sizeof(m_pGC_MapData));
    memset(m_pGC_MapData, 0, sizeof(m_pGC_MapData));
}

CRankingGridCtrl::~CRankingGridCtrl()
{
}

BEGIN_MESSAGE_MAP(CRankingGridCtrl, CGridCtrl)
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void CRankingGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    LOGS("CRankingGridCtrl::OnLButtonDblClk.....\n\r");
    CGridCtrl::OnLButtonDblClk(nFlags, point);

    CCellID cell = GetCellFromPt(point);

    CTaiShanDoc* pDoc = ((CMainFrame*)AfxGetMainWnd())->m_pDoc ;
    CString stock_code = m_pGC_MapData[cell.row-1]->id;
    pDoc->m_sharesSymbol = stock_code;
    pDoc->m_stkKind=m_pGC_MapData[cell.row-1]->kind;
    pDoc->m_GetFocus = true;
    CMainFrame * pMainWnd =(CMainFrame*)( AfxGetApp()->m_pMainWnd);
    pMainWnd->SendMessage (WM_USER_DrawKlineWindow,(WPARAM)stock_code.GetBuffer(0),pDoc->m_stkKind*10);
}

BOOL CRankingGridCtrl::GetCell(CGridCell* pGridCell,int nRow, int nCol) const
{
//    LOGS("CRankingGridCtrl::GetCell(),  nRow=%d, nCol=%d\n", nRow, nCol);

    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return FALSE;

    if(nRow==0)
	{
        pGridCell->state=0;       // Cell state (selected/focus etc)
		pGridCell->nFormat =DT_CENTER|DT_VCENTER|DT_SINGLELINE;     // Cell format
		pGridCell->szText=m_pColData[nCol].szText;      // Cell text (or binary data if you wish...)
        pGridCell->crBkClr=m_pDoc->m_colorArray[18];  // Background colour (or CLR_DEFAULT)
		
		pGridCell->crFgClr=m_pDoc->m_colorArray[14];     // Forground colour (or CLR_DEFAULT)
		pGridCell->iImage=-1;      // Index of the list view item抯 icon 
		pGridCell->lParam=0;      // 32-bit value to associate with item
		pGridCell->lfFont=m_Logfont;     // Cell font
		return TRUE;
	}

    if((m_SortColumn==nCol||m_idCurrentCell.row==nRow))
	{
        pGridCell->state=GVIS_SELECTED;       // Cell state (selected/focus etc)
	}
	else
	{
        pGridCell->state=0;       // Cell state (selected/focus etc)
	}

    pGridCell->nFormat =DT_CENTER|DT_VCENTER|DT_SINGLELINE;     // Cell format
    pGridCell->iImage=-1;      // Index of the list view item抯 icon 
    pGridCell->lParam=0;      // 32-bit value to associate with item
    pGridCell->lfFont=m_Logfont;     // Cell font

    pGridCell->crBkClr=CLR_DEFAULT;  // Background colour (or CLR_DEFAULT)
    pGridCell->crBkClr=m_pDoc->m_colorArray[18];  // Background colour (or CLR_DEFAULT)    
    pGridCell->crFgClr=m_pDoc->m_colorArray[14];     // Forground colour (or CLR_DEFAULT)


    CReportData * pData = m_pGC_MapData[nRow-1];

    if( pData == NULL )
        return TRUE;

    char str[50];

    if( nCol==0 )
    {
        sprintf(str,"%s", pData->id);
        pGridCell->crFgClr =m_pDoc->m_colorArray[21]; 
    }
    else if( nCol==1 )
    {
         sprintf(str,"%s", pData->name);
        pGridCell->crFgClr =m_pDoc->m_colorArray[20]; 
    }
    else if( nCol==2 )
    {
        if(pData->ystc !=0&&pData->nowp!=0)
        {
            sprintf(str,"%6.2f%s",100 * (pData->nowp - pData->ystc )/ pData->ystc ,"%"  );
        }
        else
            sprintf(str,"%6.2f%s",0.0f,"%" );
        pGridCell->szText =str;
        if(pData->nowp > pData->ystc )
            pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
        else if(pData->nowp < pData->ystc)
            pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
        else
            pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 

    }
    else if( nCol==3 )
    {
        float temp=0;
        unsigned int mins=0;
        if( pData->lastclmin > 5 )
            mins = pData->lastclmin-5;
        
        if(pData->m_Kdata1[mins].Price !=0&&pData->nowp!=0)
            temp = 100 * (pData->nowp - pData->m_Kdata1[mins].Price )/ pData->m_Kdata1[mins].Price ;
        else
            temp = 0;
        sprintf(str,"%6.2f%s",temp ,"%"  );
        pGridCell->szText =str;
        if( temp > 0 )
            pGridCell->crFgClr =m_pDoc->m_colorArray[13];
        else if( temp < 0 )
            pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
        else
            pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
    }
        
    pGridCell->szText =str;

	
   return TRUE;
}

BOOL CRankingGridCtrl::SetPMdataPtr( CReportData **	m_pMapData )
{
    memcpy(m_pGC_MapData, m_pMapData, sizeof(m_pGC_MapData));

    return TRUE;
}

#define BOTTOM_LINE_STYLE	1

BOOL CRankingGridCtrl::DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
#if 0
{
    CGridCtrl::DrawFixedCell(pDC, nRow, nCol, rect, bEraseBk);
    
    //-TODO: 将父类画好的表格线条擦掉
    int nSavedDC    = pDC->SaveDC();
    CPen *pOldPen   = pDC->GetCurrentPen();

    CPen darkpen(PS_SOLID,  1, GetTextBkColor());
        
    pDC->SelectObject(&darkpen);
    pDC->MoveTo(rect.right, rect.top);
    pDC->LineTo(rect.right, rect.bottom);
    pDC->LineTo(rect.left, rect.bottom);

    pDC->SelectObject(pOldPen);
    pDC->RestoreDC(nSavedDC);
    return TRUE;
}
#else
{
    if (!m_bAllowDraw) return FALSE;

    GV_ITEM Item;
    Item.mask = GVIF_TEXT | GVIF_FORMAT | GVIF_IMAGE | GVIF_BKCLR | GVIF_FGCLR|GVIF_STATE;
    Item.row = nRow;
    Item.col = nCol;
    if (!GetItem(&Item)) return FALSE;


    // Force redraw of background if custom colour
    if (Item.crBkClr != CLR_DEFAULT)
        bEraseBk = TRUE;
    COLORREF TextBkClr = (Item.crBkClr == CLR_DEFAULT)? GetTextBkColor() : Item.crBkClr;
    COLORREF TextClr   = (Item.crFgClr == CLR_DEFAULT)? GetTextColor()   : Item.crFgClr;

    if (nRow==0 && nCol>=GetFixedColumnCount() && nCol==m_SortColumn || Item.state & GVIS_SELECTED) 
    {
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
#ifndef	BOTTOM_LINE_STYLE
        pDC->FillSolidRect(rect, m_crCursor);
#endif
		rect.right--; rect.bottom--;
		pDC->SetTextColor( TextClr );
    }
	else 
	{
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        if (bEraseBk) 
        {
			COLORREF TextBkClr2 = TextBkClr;
	        pDC->FillSolidRect(rect, TextBkClr2);
        }
        rect.right--; rect.bottom--;
        pDC->SetTextColor(TextClr);
    }

    int nSavedDC = pDC->SaveDC();
    
    LOGFONT lf;
	memcpy(&lf, &m_Logfont, sizeof(LOGFONT));
        
    CCellID FocusCell = GetFocusCell();
	CFont Font;
    Font.CreateFontIndirect(&lf);
    pDC->SelectObject(&Font);

    CPen lightpen(PS_SOLID, 1,  ::GetSysColor(COLOR_3DHIGHLIGHT)),
          darkpen(PS_SOLID,  1, m_GridLineColor),

    *pOldPen = pDC->GetCurrentPen();

    pDC->SetBkMode(TRANSPARENT);

    pDC->SelectObject(&darkpen);
    pDC->MoveTo(rect.right, rect.top);
    //    pDC->LineTo(rect.right, rect.bottom);
    //    pDC->LineTo(rect.left, rect.bottom);
#ifdef BOTTOM_LINE_STYLE
    if( Item.state & GVIS_SELECTED )
    {
        pDC->MoveTo(rect.right, rect.bottom);
        pDC->LineTo(rect.left-1, rect.bottom);
        pDC->MoveTo(rect.right, rect.bottom+1);
        pDC->LineTo(rect.left-1, rect.bottom+1);
    }
#endif

    pDC->SelectObject(pOldPen);
    rect.DeflateRect(1,1);

    rect.DeflateRect(m_nMargin, 0);


    DrawText(pDC->m_hDC, Item.szText, -1, rect, Item.nFormat);


    pDC->RestoreDC(nSavedDC);
    return TRUE;
}

#endif
