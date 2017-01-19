// CTaiKlineColorTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineColorTreeCtrl.h"
#include "CTaiShanKlineShowView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineColorTreeCtrl

CTaiKlineColorTreeCtrl::CTaiKlineColorTreeCtrl()
{
	m_pDragImage = NULL;
	m_bLDragging = FALSE;
	m_hitemDrag = NULL;
	m_hitemDrop = NULL;
	m_wndTarget = NULL;
}

CTaiKlineColorTreeCtrl::~CTaiKlineColorTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CTaiKlineColorTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CTaiKlineColorTreeCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineColorTreeCtrl message handlers
void CTaiKlineColorTreeCtrl::SetItemFont(HTREEITEM hItem, LOGFONT& logfont)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		cf.color = (COLORREF)-1;
	cf.logfont = logfont;
	m_mapColorFont[hItem] = cf;
}

void CTaiKlineColorTreeCtrl::SetItemBold(HTREEITEM hItem, BOOL bBold)
{
	SetItemState( hItem, bBold ? TVIS_BOLD: 0, TVIS_BOLD );
}

void CTaiKlineColorTreeCtrl::SetItemColor(HTREEITEM hItem, COLORREF color)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		cf.logfont.lfFaceName[0] = '\0';
	cf.color = color;
	m_mapColorFont[hItem] = cf;
}

BOOL CTaiKlineColorTreeCtrl::GetItemFont(HTREEITEM hItem, LOGFONT * plogfont)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		return FALSE;
	if( cf.logfont.lfFaceName[0] == '\0' ) 
		return FALSE;
	*plogfont = cf.logfont;
	return TRUE;

}

BOOL CTaiKlineColorTreeCtrl::GetItemBold(HTREEITEM hItem)
{
	return GetItemState( hItem, TVIS_BOLD ) & TVIS_BOLD;
}

COLORREF CTaiKlineColorTreeCtrl::GetItemColor(HTREEITEM hItem)
{
	// Returns (COLORREF)-1 if color was not set
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		return (COLORREF)-1;
	return cf.color;

}
void CTaiKlineColorTreeCtrl::OnPaint() 
{
	CPaintDC dc(this);

	// Create a memory DC compatible with the paint DC
	CDC memDC;
	memDC.CreateCompatibleDC( &dc );

	CRect rcClip, rcClient;
	dc.GetClipBox( &rcClip );
	GetClientRect(&rcClient);

	// Select a compatible bitmap into the memory DC
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
	CBitmap * pOld = memDC.SelectObject( &bitmap );
	
	// Set clip region to be same as that in paint DC
	CRgn rgn;
	rgn.CreateRectRgnIndirect( &rcClip );
	memDC.SelectClipRgn(&rgn);
	


	// First let the control do its default drawing.
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );


	HTREEITEM hItem = GetFirstVisibleItem();

	int n = GetVisibleCount()+1;
	while( hItem && n--)
	{
		CRect rect;

		// Do not meddle with selected items or drop highlighted items
		UINT selflag = TVIS_DROPHILITED | TVIS_SELECTED;
		Color_Font cf;
	
		if ( !(GetItemState( hItem, selflag ) & selflag ) 
			&& m_mapColorFont.Lookup( hItem, cf ))
		{
			CFont *pFontDC;
			CFont fontDC;
			LOGFONT logfont;

			if( cf.logfont.lfFaceName[0] != '\0' ) 
			{
				logfont = cf.logfont;
			}
			else
			{
				// No font specified, so use window font
				CFont *pFont = GetFont();
				pFont->GetLogFont( &logfont );
			}

			if( GetItemBold( hItem ) )
				logfont.lfWeight = 700;
			fontDC.CreateFontIndirect( &logfont );
			pFontDC = memDC.SelectObject( &fontDC );

			if( cf.color != (COLORREF)-1 )
				memDC.SetTextColor( cf.color );

			CString sItem = GetItemText( hItem );

			GetItemRect( hItem, &rect, TRUE );
			memDC.SetBkColor( GetSysColor( COLOR_WINDOW ) );
			memDC.TextOut( rect.left+2, rect.top+1, sItem );
			
			memDC.SelectObject( pFontDC );
		}
		hItem = GetNextVisibleItem( hItem );
	}


	dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC, 
				rcClip.left, rcClip.top, SRCCOPY );
	rgn.DeleteObject();
	memDC.SelectObject( pOld );
}


void CTaiKlineColorTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT uFlags ;
	HTREEITEM h = HitTest( point, &uFlags );
	if(h)
		SelectItem(h);

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CTaiKlineColorTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnLButtonUp(nFlags, point);
	if (m_bLDragging)
	{
		m_bLDragging = FALSE;
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();

		delete m_pDragImage;

		if(m_wndTarget && m_wndTarget ->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
		{
			DoDropOut(m_wndTarget,m_hitemDrag);
		}
		else
		{
			// Remove drop target highlighting
			SelectDropTarget(NULL);

			if( m_hitemDrag == m_hitemDrop )
				return;
			if(! CanDroped(m_hitemDrop ))
				return;

			// If Drag item is an ancestor of Drop item then return
			HTREEITEM htiParent = m_hitemDrop;
			while( (htiParent = GetParentItem( htiParent )) != NULL )
			{
				if( htiParent == m_hitemDrag ) return;
			}

			Expand( m_hitemDrop, TVE_EXPAND ) ;

			HTREEITEM htiNew = CopyBranch( m_hitemDrag, m_hitemDrop, TVI_LAST );
			DoDropedSelf(htiNew);

			DeleteItem(m_hitemDrag);
			SelectItem( htiNew );
		}
	}
}

void CTaiKlineColorTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	HTREEITEM	hitem;
	UINT		flags;

	if (m_bLDragging)
	{
		//if(m_wndTarget != NULL)//pFm->ChildWindowFromPoint( p2 ,CWP_ALL);
		CPoint p2;
		POINT pt = point;
		ClientToScreen( &pt );
		m_wndTarget = ((CFrameWnd*)AfxGetMainWnd())->WindowFromPoint  ( pt );//,CWP_ALL);
		CImageList::DragMove(pt);
		if(m_wndTarget && m_wndTarget ->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
		{
		}
		else
		{
			if ((hitem = HitTest(point, &flags)) != NULL)
			{
				if(CanDroped(hitem))
				{
					CImageList::DragShowNolock(FALSE);
					SelectDropTarget(hitem);
					m_hitemDrop = hitem;
					CImageList::DragShowNolock(TRUE);
				}
			}
		}

	}
	
	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CTaiKlineColorTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	if(!CanBeginDrag()) return;

	m_hitemDrag = pNMTreeView->itemNew.hItem;
	m_hitemDrop = NULL;

	m_pDragImage = CreateDragImage(m_hitemDrag);  // get the image list for dragging
	// CreateDragImage() returns NULL if no image list
	// associated with the tree view control
	if( !m_pDragImage )
		return;

	m_bLDragging = TRUE;
	m_pDragImage->BeginDrag(0, CPoint(-15,-15));
	POINT pt = pNMTreeView->ptDrag;
	ClientToScreen( &pt );
	m_pDragImage->DragEnter(NULL, pt);
	SetCapture();
}
HTREEITEM CTaiKlineColorTreeCtrl::CopyBranch(HTREEITEM oldItem, HTREEITEM newParent, HTREEITEM iPosition)
{
	HTREEITEM newItem=InsertItem( GetItemText(oldItem), newParent,iPosition);
	SetNewParent(newItem,oldItem);
	return newItem;
}

void CTaiKlineColorTreeCtrl::SetNewParent(HTREEITEM newPosition, HTREEITEM oldPosition)
{
	HTREEITEM oldChild=GetChildItem(oldPosition);
	HTREEITEM newChild;
	while(oldChild){
		newChild=InsertItem( GetItemText(oldChild), newPosition,TVI_LAST );
		SetNewParent(newChild,oldChild);
		oldChild=GetNextItem(oldChild, TVGN_NEXT);
	}
}
bool CTaiKlineColorTreeCtrl::CanBeginDrag()
{
	return false;
}
bool CTaiKlineColorTreeCtrl::CanDroped(HTREEITEM	hitem)
{
	return false;
}
void CTaiKlineColorTreeCtrl::DoDropOut(CWnd* pWnd,HTREEITEM hItem)
{
}
void CTaiKlineColorTreeCtrl::DoDropedSelf(HTREEITEM hItem)
{
}
