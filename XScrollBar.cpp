// XScrollBar.cpp  Version 1.1
//
// Author:  Hans Dietrich
//          hdietrich@gmail.com
//
// History
//     Version 1.1 - 2004 September 21
//     - Update with Steve Mayfield's vertical scrollbar support
//
//     Version 1.0 - 2004 September 9
//     - Initial public release
//
// Acknowledgements:
//     Thanks to Greg Ellis for his CSkinHorizontalScrollbar class,
//     which I used as the starting point for CXScrollBar:
//         http://www.codeproject.com/listctrl/skinlist.asp
//
//     Thanks to Christian Rodemeyer for his CColor class:
//         http://www.codeproject.com/bitmap/ccolor.asp
//
// License:
//     This software is released into the public domain.  You are free to use
//     it in any way you like, except that you may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "memdc1.h"
#include "Color.h"
#include "XScrollBar.h"

#pragma optimize("g", off)	// necessary in VC 6.0 to suppress (release mode)
							//    "fatal error C1001: INTERNAL COMPILER ERROR"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef IDC_HAND
#define IDC_HAND MAKEINTRESOURCE(32649)		// From WINUSER.H
#endif

///////////////////////////////////////////////////////////////////////////////
// timer defines

#define TIMER_MOUSE_OVER_BUTTON		1	// mouse is over an arrow button, and
										// left button is down

#define TIMER_LBUTTON_PRESSED		2	// mouse is over an arrow button, and
										// left button has just been pressed

#define TIMER_MOUSE_OVER_THUMB		3	// mouse is over thumb

///////////////////////////////////////////////////////////////////////////////
// color defines

#define THUMB_MASK_COLOR					RGB(0,0,1)
#define THUMB_GRIPPER_MASK_COLOR			RGB(0,0,2)
#define THUMB_LEFT_TRANSPARENT_MASK_COLOR	RGB(0,0,3)
#define THUMB_RIGHT_TRANSPARENT_MASK_COLOR	RGB(0,0,4)
#define THUMB_UP_TRANSPARENT_MASK_COLOR		THUMB_LEFT_TRANSPARENT_MASK_COLOR
#define THUMB_DOWN_TRANSPARENT_MASK_COLOR	THUMB_RIGHT_TRANSPARENT_MASK_COLOR
#define THUMB_GRIPPER_COLOR					RGB(91,91,91)	// dark gray
#define THUMB_HOVER_COLOR					RGB(255,204,0)	// orange
#define FRAME_COLOR							RGB(76,85,118)	// dark gray

///////////////////////////////////////////////////////////////////////////////
// message map
BEGIN_MESSAGE_MAP(CXScrollBar, CStatic)
	//{{AFX_MSG_MAP(CXScrollBar)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CXScrollBar::CXScrollBar()
{
	m_pParent              = NULL;
	m_bHorizontal          = TRUE;
	m_hCursor              = NULL;
	m_bMouseDown           = FALSE;
	m_bMouseDownArrowLeft  = FALSE;
	m_bMouseDownArrowRight = FALSE;
	m_bMouseDownArrowUp    = FALSE;
	m_bMouseDownArrowDown  = FALSE;
	m_bDragging            = FALSE;
	m_nPos                 = 0;
	m_nMinPos              = 0;
	m_nMaxPos              = 0;
	m_nRange               = 0;
	m_ThumbColor           = RGB(0,0,0);
	m_ThumbHoverColor      = THUMB_HOVER_COLOR;
	m_bChannelColor        = TRUE;
	m_bThumbColor          = FALSE;
	m_bThumbGripper        = TRUE;
	m_bThumbHover          = FALSE;
	m_rectThumb            = CRect(-1,-1,-1,-1);
	m_rectClient           = CRect(-1,-1,-1,-1);
	m_nThumbLeft           = 12;
	m_nThumbTop            = 12;
//	m_nBitmapWidth         = 12;
//	m_nBitmapHeight        = 12;

	m_crArrow      		    =RGB(255,0,0);
	m_crThumb				=RGB(255,0,0);
	m_crChannel				=RGB(0,0,0);
	m_crFrame				=RGB(255,255,255);
	m_crBack                =RGB(0,0,0);
   
	
}

///////////////////////////////////////////////////////////////////////////////
// dtor
CXScrollBar::~CXScrollBar()
{
	if (m_hCursor)
		DestroyCursor(m_hCursor);
	m_hCursor = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//
// CreateFromStatic
//
// Purpose:     Create the CXScrollBar control from STATIC placeholder
//
// Parameters:  dwStyle    - the scroll bar�s style. Typically this will be
//                           SBS_HORZ|WS_CHILD|SS_LEFT|SS_NOTIFY|WS_VISIBLE.
//              pParentWnd - the scroll bar�s parent window, usually a CDialog
//                           object. It must not be NULL.
//              nIdStatic  - the resource id of the placeholder STATIC
//              nId        - the resource id of the CXScrollBar control
//
// Returns:     BOOL       - TRUE = success
//
// Notes:       Hides the STATIC placeholder.  Also loads hand cursor, and
//              sets the thumb bitmap size.

BOOL CXScrollBar::CreateFromStatic(DWORD dwStyle,
								   CWnd* pParentWnd,
								   UINT nIdStatic,
								   UINT nId)
{
	TRACE(_T("in CXScrollBar::CreateFromStatic\n"));

	m_pParent = pParentWnd;
	ASSERT(m_pParent);

	ASSERT(::IsWindow(pParentWnd->GetDlgItem(nIdStatic)->m_hWnd));

	CRect rect;
	pParentWnd->GetDlgItem(nIdStatic)->GetWindowRect(&rect);
	pParentWnd->ScreenToClient(&rect);

	// hide placeholder STATIC
	pParentWnd->GetDlgItem(nIdStatic)->ShowWindow(SW_HIDE);

	// load hand cursor
//	SetDefaultCursor();

	m_bHorizontal = (dwStyle & SBS_VERT) ? FALSE : TRUE;

	BOOL bResult = 	CStatic::Create(_T(""), dwStyle, rect, pParentWnd, nId);

	if (bResult)
	{
		GetClientRect(&m_rectClient);

/*		CBitmap bitmap;

		// we assume that width of thumb is same as width of arrows
		if (bitmap.LoadBitmap(m_bHorizontal ? 
							  IDB_HORIZONTAL_SCROLLBAR_THUMB : 
							  IDB_VERTICAL_SCROLLBAR_THUMB))
		{
			BITMAP bm;
			bitmap.GetBitmap(&bm);

			m_nBitmapWidth = bm.bmWidth;
			m_nBitmapHeight = bm.bmHeight;

			TRACE(_T("m_nBitmapWidth=%d  m_nBitmapHeight=%d\n"),
				m_nBitmapWidth, m_nBitmapHeight);

			GetClientRect(&m_rectClient);
			TRACE(_T("m_rectClient:  %d, %d, %d, %d\n"),
				m_rectClient.left, m_rectClient.top, 
				m_rectClient.right, m_rectClient.bottom);

			if (bitmap.GetSafeHandle())
				bitmap.DeleteObject();
		}
		else
		{
			TRACE(_T("ERROR - failed to load thumb bitmap\n"));
			ASSERT(FALSE);
		}*/
	}
	else
	{
		TRACE(_T("ERROR - failed to create CXScrollBar\n"));
		ASSERT(FALSE);
	}

	return bResult;
}


BOOL CXScrollBar::Create(DWORD dwStyle,
								   CWnd* pParentWnd,
								   LPRECT lpRect,
								   UINT nId)
{
	TRACE(_T("in CXScrollBar::CreateFromStatic\n"));

	m_pParent = pParentWnd;
	ASSERT(m_pParent);
	CRect rect(lpRect);
	pParentWnd->ScreenToClient(&rect);
	// load hand cursor
//	SetDefaultCursor();

	m_bHorizontal = (dwStyle & SBS_VERT) ? FALSE : TRUE;

	BOOL bResult = 	CStatic::Create(_T(""), dwStyle, rect, pParentWnd, nId);
	if (bResult)
	{
			GetClientRect(&m_rectClient);
	}
	else
	{
		TRACE(_T("ERROR - failed to create CXScrollBar\n"));
		ASSERT(FALSE);
	}

	return bResult;
}

///////////////////////////////////////////////////////////////////////////////
// OnPaint
void CXScrollBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	Draw();
}

void CXScrollBar::SetColor(BOOL bReDraw,COLORREF crBack, COLORREF crFrame, COLORREF crArrow,COLORREF crThumb,COLORREF crChannel)
{
	m_crBack                =crBack;
	m_crFrame				=crFrame;
	m_crArrow      		    =(crArrow==NULL?crFrame:crArrow);
	m_crThumb				=(crArrow==NULL?crFrame:crThumb);
	m_crChannel				=(crArrow==NULL?crFrame:crChannel);
	if (bReDraw)
		Draw();
}


///////////////////////////////////////////////////////////////////////////////
// Draw
void CXScrollBar::Draw()
{
	if (m_bHorizontal)
		DrawHorizontal();
	else
		DrawVertical();
}

///////////////////////////////////////////////////////////////////////////////
// DrawHorizontal
void CXScrollBar::DrawHorizontal()
{
	CClientDC dc(this);
	 
	GetClientRect(&m_rectClient);
	CMemDC1 memDC(&dc, &m_rectClient);
    

	CBrush brushBack(m_crBack);
	CBrush brushArrow(m_crArrow);
	CPen   *pPenOld,penFrame,penArrow,penThumb,penChannel;
	CBrush *pBrushOld;
	penFrame.CreatePen(PS_SOLID,1,m_crFrame); 
	penArrow.CreatePen(PS_SOLID,1,m_crArrow); 
	penThumb.CreatePen(PS_SOLID,1,m_crThumb); 
	penChannel.CreatePen(PS_SOLID,1,m_crChannel); 
	
	pBrushOld=(CBrush*)memDC.SelectObject(brushBack);
	pPenOld=(CPen*)memDC.SelectObject(penFrame);
	
	memDC.Rectangle(m_rectClient);
	
	int nHeight=m_rectClient.Height();
	
	m_rcArrow1=m_rectClient;
	m_rcThumb=m_rectClient;
	m_rcChannel=m_rectClient;
	m_rcArrow2=m_rectClient;

	m_rcArrow1.right = m_rectClient.left +nHeight;
	m_rcArrow2.left  = m_rectClient.right-nHeight;
	if (m_rcArrow1.right >m_rectClient.right) m_rcArrow1.right  =m_rectClient.right ;
	if (m_rcArrow2.left <m_rectClient.left ) m_rcArrow2.left =m_rectClient.left  ;
	m_rcChannel.left  = m_rcArrow1.right  +1;
	m_rcChannel.right =m_rcArrow2.left  -1;
	
	int nChannelWidth=m_rcChannel.Width();
	int nRealChannelWidth=nChannelWidth-THUMBLENGTH;
	int nMid;
	if ((m_nMaxPos-m_nMinPos)==0)
		nMid=m_rcChannel.left +THUMBLENGTH/2;
	else
		nMid=m_rcChannel.left +THUMBLENGTH/2+ m_nPos*nRealChannelWidth/(m_nMaxPos-m_nMinPos);
	m_rcThumb.left =nMid-THUMBLENGTH/2;
	m_rcThumb.right  =nMid+THUMBLENGTH/2;
	if (m_rcThumb.left  <m_rcChannel.left  )
		m_rcThumb.left  =m_rcChannel.left  ;
	if (m_rcThumb.right > m_rcChannel.right  )
		m_rcThumb.right  =m_rcChannel.right  ;
    memDC.SelectObject(&penArrow);
	memDC.Rectangle(m_rcArrow1);
	memDC.Rectangle(m_rcArrow2);

    memDC.SelectObject(&brushArrow); 
	CPoint pts[3];
	pts[0].x =m_rcArrow2.left+4;
	pts[0].y =m_rcArrow2.top +4;
	pts[1].x =m_rcArrow2.left +4;
	pts[1].y =m_rcArrow2.bottom -4;
	pts[2].x =m_rcArrow2.right -4;
	pts[2].y =m_rcArrow2.top+m_rcArrow2.Height()/2;
	memDC.Polygon(pts, 3);
	pts[0].x =m_rcArrow1.right -4;
	pts[0].y =m_rcArrow1.top +4;
	pts[1].x =m_rcArrow1.right-4;
	pts[1].y =m_rcArrow1.bottom-4;
	pts[2].x =m_rcArrow1.left+4;
	pts[2].y =m_rcArrow1.top+m_rcArrow2.Height()/2;
	memDC.Polygon(pts, 3);

	memDC.SelectStockObject(NULL_BRUSH);
	memDC.Rectangle(m_rcThumb);
	for (int i=m_rcThumb.left +2;i<m_rcThumb.right  ;i++)
	{
		memDC.MoveTo(i,m_rcThumb.top);
		memDC.LineTo(i,m_rcThumb.bottom);
		i++;
	}
	//memDC.FillSolidRect(m_rcThumb,m_crThumb);
	memDC.Rectangle(m_rectClient);

//	memDC.SelectObject(pPenOld);
//	memDC.SelectObject(pBrushOld);

/*	TRACE(_T("in CXScrollBar::DrawHorizontal\n"));
	GetClientRect(&m_rectClient);
	CClientDC dc(this);
	CMemDC1 memDC(&dc, &m_rectClient);

	CBrush brushFrame(FRAME_COLOR);

	CDC bitmapDC;
	bitmapDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;

	// =====  draw left arrow  =====

	VERIFY(bitmap.LoadBitmap(IDB_HORIZONTAL_SCROLLBAR_LEFTARROW));
	CBitmap* pOldBitmap = bitmapDC.SelectObject(&bitmap);

	// NOTE:  thumb and arrow bitmaps are assumed to be same width and height

	CRect rectLeftArrow(m_rectClient.left, m_rectClient.top,
		m_rectClient.left + m_nBitmapWidth, m_rectClient.bottom);

	memDC.StretchBlt(rectLeftArrow.left, rectLeftArrow.top+1,
		rectLeftArrow.Width(), rectLeftArrow.Height()-1,
		&bitmapDC, 0, 0, m_nBitmapWidth, m_nBitmapHeight, SRCCOPY);

	memDC.FrameRect(&rectLeftArrow, &brushFrame);

	int nChannelStart = m_rectClient.left + m_nBitmapWidth;
	int nChannelWidth = m_rectClient.Width() - 2*m_nBitmapWidth;

	if (pOldBitmap)
		bitmapDC.SelectObject(pOldBitmap);
	if (bitmap.GetSafeHandle())
		bitmap.DeleteObject();
	pOldBitmap = NULL;

	// =====  draw channel  =====

	// save new thumb position
	TRACE(_T("m_nThumbLeft=%d\n"), m_nThumbLeft);
	m_rectThumb.left   = m_rectClient.left + m_nThumbLeft;
	m_rectThumb.right  = m_rectThumb.left + m_nBitmapWidth;
	m_rectThumb.top    = m_rectClient.top;
	m_rectThumb.bottom = m_rectThumb.top + m_rectClient.Height();

	VERIFY(bitmap.LoadBitmap(IDB_HORIZONTAL_SCROLLBAR_CHANNEL));

	pOldBitmap = bitmapDC.SelectObject(&bitmap);

	CRect rectChannelRight(m_rectThumb.left + m_nBitmapWidth/2, m_rectClient.top,
		nChannelStart + nChannelWidth, m_rectClient.bottom);

	memDC.StretchBlt(rectChannelRight.left, rectChannelRight.top+1,
		rectChannelRight.Width(), rectChannelRight.Height()-1,
		&bitmapDC, 0, 0, 1, m_nBitmapHeight, SRCCOPY);

	if (m_bChannelColor && m_bThumbColor)
	{
		// thumb has a color, so use same (lightened) color for channel
		CColor color;
		color.SetRGB(GetRValue(m_ThumbColor),
					 GetGValue(m_ThumbColor),
					 GetBValue(m_ThumbColor));
		color.ToHLS();
		float fLuminance = color.GetLuminance();

		// use 80% L, 150% S for main color
		fLuminance = .80f;
		float fSaturation = color.GetSaturation();
		fSaturation = 0.5f * fSaturation;
		float fHue = color.GetHue();
		color.SetHLS(fHue, fLuminance, fSaturation);
		color.ToRGB();
		COLORREF rgb3 = RGB(color.GetRed(), color.GetGreen(), color.GetBlue());

		// use .87 L for second highlight color
		fLuminance = .87f;
		color.SetHLS(fHue, fLuminance, fSaturation);
		color.ToRGB();
		COLORREF rgb2 = RGB(color.GetRed(), color.GetGreen(), color.GetBlue());

		// use .92 L for first highlight color
		fLuminance = .92f;
		color.SetHLS(fHue, fLuminance, fSaturation);
		color.ToRGB();
		COLORREF rgb1 = RGB(color.GetRed(), color.GetGreen(), color.GetBlue());

		BITMAP bm;
		bitmap.GetBitmap(&bm);

		// set highlight colors
		bitmapDC.SetPixel(0, 0, rgb1);
		bitmapDC.SetPixel(0, 1, rgb2);

		// set main color
		for (int y = 2; y < (bm.bmHeight); y++)
			bitmapDC.SetPixel(0, y, rgb3);
	}

	CRect rectChannelLeft(nChannelStart, m_rectClient.top,
		m_rectThumb.left + m_nBitmapWidth/2, m_rectClient.bottom);

	memDC.StretchBlt(rectChannelLeft.left, rectChannelLeft.top+1,
		rectChannelLeft.Width(), rectChannelLeft.Height()-1,
		&bitmapDC, 0, 0, 1, m_nBitmapHeight, SRCCOPY);

	if (pOldBitmap)
		bitmapDC.SelectObject(pOldBitmap);
	if (bitmap.GetSafeHandle())
		bitmap.DeleteObject();
	pOldBitmap = NULL;

	// =====  draw right arrow  =====

	VERIFY(bitmap.LoadBitmap(IDB_HORIZONTAL_SCROLLBAR_RIGHTARROW));

	pOldBitmap = bitmapDC.SelectObject(&bitmap);

	CRect rectRightArrow(m_rectClient.right - m_nBitmapWidth, m_rectClient.top,
		m_rectClient.right, m_rectClient.bottom);

	memDC.StretchBlt(rectRightArrow.left, rectRightArrow.top+1,
		rectRightArrow.Width(), rectRightArrow.Height()-1,
		&bitmapDC, 0, 0, m_nBitmapWidth, m_nBitmapHeight, SRCCOPY);

	memDC.FrameRect(&rectRightArrow, &brushFrame);

	if (pOldBitmap)
		bitmapDC.SelectObject(pOldBitmap);
	if (bitmap.GetSafeHandle())
		bitmap.DeleteObject();
	pOldBitmap = NULL;

	// If there is nothing to scroll then don't show the thumb
	if (m_nRange)
	{
		// =====  draw thumb  =====

		if (m_bThumbColor)
			VERIFY(bitmap.LoadBitmap(IDB_HORIZONTAL_SCROLLBAR_THUMB));
		else
			VERIFY(bitmap.LoadBitmap(IDB_HORIZONTAL_SCROLLBAR_THUMB_NO_COLOR));

		pOldBitmap = bitmapDC.SelectObject(&bitmap);

		COLORREF rgbThumb = m_ThumbColor;
		if (m_bThumbHover)
			rgbThumb = m_ThumbHoverColor;

		COLORREF rgbPrev = 0;

		// add desired color to thumb
		for (int x = 0; x < m_nBitmapWidth; x++)
		{
			for (int y = 0; y < m_nBitmapHeight; y++)
			{
				COLORREF rgb = bitmapDC.GetPixel(x, y);

				if (m_bThumbColor && (rgb == THUMB_MASK_COLOR))
				{
					bitmapDC.SetPixel(x, y, rgbThumb);
				}
				else if (rgb == THUMB_GRIPPER_MASK_COLOR)
				{
					if (m_bThumbGripper)
						bitmapDC.SetPixel(x, y, THUMB_GRIPPER_COLOR);
					else
						bitmapDC.SetPixel(x, y, rgbPrev);
				}
				else if (rgb == THUMB_LEFT_TRANSPARENT_MASK_COLOR)
				{
					COLORREF rgbLeftChannel = memDC.GetPixel(nChannelStart, y);
					bitmapDC.SetPixel(x, y, rgbLeftChannel);
				}
				else if (rgb == THUMB_RIGHT_TRANSPARENT_MASK_COLOR)
				{
					COLORREF rgbRightChannel =
						memDC.GetPixel(nChannelStart+nChannelWidth-1, y);
					bitmapDC.SetPixel(x, y, rgbRightChannel);
				}

				rgbPrev = rgb;
			}
		}

		memDC.StretchBlt(m_rectThumb.left, m_rectThumb.top,
			m_rectThumb.Width(), m_rectThumb.Height(),
			&bitmapDC, 0, 0, m_nBitmapWidth, m_nBitmapHeight, SRCCOPY);

		if (pOldBitmap)
			bitmapDC.SelectObject(pOldBitmap);
		if (bitmap.GetSafeHandle())
			bitmap.DeleteObject();
		pOldBitmap = NULL;
	}
	else
	{
		m_rectThumb = CRect(-1,-1,-1,-1);
	}

	memDC.FrameRect(&m_rectClient, &brushFrame);
	*/
}

///////////////////////////////////////////////////////////////////////////////
// DrawVertical

/*	m_crBack                =crBack;
	m_crFrame				=crFrame;
	m_crArrow      		    =(crArrow==NULL?crFrame:crArrow);
	m_crThumb				=(crArrow==NULL?crFrame:crThumb);
	m_crChannel				=(crArrow==NULL?crFrame:crChannel);
*/


void CXScrollBar::DrawVertical()
{
	CClientDC dc(this);
	 
	GetClientRect(&m_rectClient);
	CMemDC1 memDC(&dc, &m_rectClient);
    

	CBrush brushBack(m_crBack);
	CBrush brushArrow(m_crArrow);
	CPen   *pPenOld,penFrame,penArrow,penThumb,penChannel;
	CBrush *pBrushOld;
	penFrame.CreatePen(PS_SOLID,1,m_crFrame); 
	penArrow.CreatePen(PS_SOLID,1,m_crArrow); 
	penThumb.CreatePen(PS_SOLID,1,m_crThumb); 
	penChannel.CreatePen(PS_SOLID,1,m_crChannel); 
	
	pBrushOld=(CBrush*)memDC.SelectObject(brushBack);
	pPenOld=(CPen*)memDC.SelectObject(penFrame);
	
	memDC.Rectangle(m_rectClient);
	
	int nWidth=m_rectClient.Width();
	
	m_rcArrow1=m_rectClient;
	m_rcThumb=m_rectClient;
	m_rcChannel=m_rectClient;
	m_rcArrow2=m_rectClient;

	m_rcArrow1.bottom = m_rectClient.top+nWidth;
	m_rcArrow2.top = m_rectClient.bottom-nWidth;
	if (m_rcArrow1.bottom>m_rectClient.bottom) m_rcArrow1.bottom =m_rectClient.bottom ;
	if (m_rcArrow2.top<m_rectClient.top) m_rcArrow2.top =m_rectClient.top ;
	m_rcChannel.top = m_rcArrow1.bottom +1;
	m_rcChannel.bottom =m_rcArrow2.top -1;
	
	int nChannelHeight=m_rcChannel.Height();
	int nRealChannelHeight=nChannelHeight-THUMBLENGTH;
	int nMid;
	if ((m_nMaxPos-m_nMinPos)==0)
		nMid=m_rcChannel.top+THUMBLENGTH/2;
	else
		nMid=m_rcChannel.top +THUMBLENGTH/2+ m_nPos*nRealChannelHeight/(m_nMaxPos-m_nMinPos);
	m_rcThumb.top =nMid-THUMBLENGTH/2;
	m_rcThumb.bottom =nMid+THUMBLENGTH/2;
	if (m_rcThumb.top <m_rcChannel.top )
		m_rcThumb.top =m_rcChannel.top ;
	if (m_rcThumb.bottom > m_rcChannel.bottom )
		m_rcThumb.bottom  =m_rcChannel.bottom ;
    memDC.SelectObject(&penArrow);
	memDC.Rectangle(m_rcArrow1);
	memDC.Rectangle(m_rcArrow2);

    memDC.SelectObject(&brushArrow); 
	CPoint pts[3];
	pts[0].x =m_rcArrow2.left+4;
	pts[0].y =m_rcArrow2.top +4;
	pts[1].x =m_rcArrow2.right-4;
	pts[1].y =m_rcArrow2.top +4;
	pts[2].x =m_rcArrow2.left+m_rcArrow2.Width()/2;
	pts[2].y =m_rcArrow2.bottom -4;
	memDC.Polygon(pts, 3);
	pts[0].x =m_rcArrow1.left+4;
	pts[0].y =m_rcArrow1.bottom-4;
	pts[1].x =m_rcArrow1.right-4;
	pts[1].y =m_rcArrow1.bottom-4;
	pts[2].x =m_rcArrow1.left+m_rcArrow1.Width()/2;
	pts[2].y =m_rcArrow1.top+4;
	memDC.Polygon(pts, 3);

	memDC.SelectStockObject(NULL_BRUSH);
	memDC.Rectangle(m_rcThumb);
	for (int i=m_rcThumb.top+2;i<m_rcThumb.bottom ;i++)
	{
		memDC.MoveTo(m_rcThumb.left,i);
		memDC.LineTo(m_rcThumb.right,i);
		i++;
	}
	//memDC.FillSolidRect(m_rcThumb,m_crThumb);
	memDC.Rectangle(m_rectClient);

//	memDC.SelectObject(pPenOld);
//	memDC.SelectObject(pBrushOld);


//	memDC.FrameRect(&m_rectClient, &brushFrame);
}


/*
void CXScrollBar::DrawVertical()
{
	TRACE(_T("in CXScrollBar::DrawVertical\n"));

	CClientDC dc(this);
	GetClientRect(&m_rectClient);
	CMemDC1 memDC(&dc, &m_rectClient);
	CBrush brushFrame(FRAME_COLOR);

	CDC bitmapDC;
	bitmapDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;

	// =====  draw Up arrow  =====

	VERIFY(bitmap.LoadBitmap(IDB_VERTICAL_SCROLLBAR_UPARROW));
	CBitmap* pOldBitmap = bitmapDC.SelectObject(&bitmap);

	// NOTE:  thumb and arrow bitmaps are assumed to be same width and height
	CRect rectUpArrow(m_rectClient.left, m_rectClient.top, 
		m_rectClient.right, m_rectClient.top + m_nBitmapHeight);

	memDC.StretchBlt(rectUpArrow.left, rectUpArrow.top, 
		rectUpArrow.Width(), rectUpArrow.Height(), 
		&bitmapDC, 0, 0, m_nBitmapWidth, m_nBitmapHeight, SRCCOPY);

	memDC.FrameRect(&rectUpArrow, &brushFrame);
	if (pOldBitmap)
		bitmapDC.SelectObject(pOldBitmap);
	if (bitmap.GetSafeHandle())
		bitmap.DeleteObject();
	pOldBitmap = NULL;

	int nChannelStart = m_rectClient.top + m_nBitmapHeight;
	int nChannelHeight = m_rectClient.Height() - 2*m_nBitmapHeight;

	// =====  draw channel  =====

	// save new thumb position
	TRACE(_T("m_nThumbTop=%d\n"), m_nThumbTop);
	m_rectThumb.left   = m_rectClient.left;
	m_rectThumb.right  = m_rectThumb.left + m_rectClient.Width();
	m_rectThumb.top    = m_rectClient.top + m_nThumbTop;
	m_rectThumb.bottom = m_rectThumb.top + m_nBitmapHeight;

	VERIFY(bitmap.LoadBitmap(IDB_VERTICAL_SCROLLBAR_CHANNEL));
	pOldBitmap = bitmapDC.SelectObject(&bitmap);

	CRect rectChannelDown(m_rectClient.left, m_rectThumb.top + m_nBitmapHeight/2, 
		m_rectClient.right, nChannelStart + nChannelHeight);

	memDC.StretchBlt(rectChannelDown.left+1, rectChannelDown.top, 
		rectChannelDown.Width()-1, rectChannelDown.Height(), 
		&bitmapDC, 0, 0, m_nBitmapWidth, 1, SRCCOPY);

	if (m_bChannelColor && m_bThumbColor)
	{
		// thumb has a color, so use same (lightened) color for channel
		CColor color;
		color.SetRGB(GetRValue(m_ThumbColor),
					 GetGValue(m_ThumbColor), 
					 GetBValue(m_ThumbColor));
		color.ToHLS();
		float fLuminance = color.GetLuminance();

		// use 80% L, 150% S for main color
		fLuminance = .80f;
		float fSaturation = color.GetSaturation();
		fSaturation = 0.5f * fSaturation;
		float fHue = color.GetHue();
		color.SetHLS(fHue, fLuminance, fSaturation);
		color.ToRGB();
		COLORREF rgb3 = RGB(color.GetRed(), color.GetGreen(), color.GetBlue());

		// use .87 L for second highlight color
		fLuminance = .87f;
		color.SetHLS(fHue, fLuminance, fSaturation);
		color.ToRGB();
		COLORREF rgb2 = RGB(color.GetRed(), color.GetGreen(), color.GetBlue());

		// use .92 L for first highlight color
		fLuminance = .92f;
		color.SetHLS(fHue, fLuminance, fSaturation);
		color.ToRGB();
		COLORREF rgb1 = RGB(color.GetRed(), color.GetGreen(), color.GetBlue());

		BITMAP bm;
		bitmap.GetBitmap(&bm);

		// set highlight colors
		bitmapDC.SetPixel(0, 0, rgb1);
		bitmapDC.SetPixel(1, 0, rgb2);

		// set main color
		for (int x = 2; x < (bm.bmWidth); x++)
			bitmapDC.SetPixel(x, 0, rgb3);
	}

	CRect rectChannelUp(m_rectClient.left, nChannelStart, 
		m_rectClient.right, m_rectThumb.top + m_nBitmapHeight/2);

	memDC.StretchBlt(rectChannelUp.left, rectChannelUp.top, 
		rectChannelUp.Width(), rectChannelUp.Height(), 
		&bitmapDC, 0, 0, m_nBitmapWidth, 1, SRCCOPY);

	if (pOldBitmap)
		bitmapDC.SelectObject(pOldBitmap);
	if (bitmap.GetSafeHandle())
		bitmap.DeleteObject();
	pOldBitmap = NULL;

	// =====  draw down arrow  =====

	VERIFY(bitmap.LoadBitmap(IDB_VERTICAL_SCROLLBAR_DOWNARROW));
	pOldBitmap = bitmapDC.SelectObject(&bitmap);

	CRect rectDownArrow(m_rectClient.left, m_rectClient.bottom - m_nBitmapHeight, 
		m_rectClient.right, m_rectClient.bottom);

	memDC.StretchBlt(rectDownArrow.left, rectDownArrow.top, 
		rectDownArrow.Width()-1, rectDownArrow.Height(), 
		&bitmapDC, 0, 0, m_nBitmapWidth, m_nBitmapHeight, SRCCOPY);
	memDC.FrameRect(&rectDownArrow, &brushFrame);

	if (pOldBitmap)
		bitmapDC.SelectObject(pOldBitmap);
	if (bitmap.GetSafeHandle())
		bitmap.DeleteObject();
	pOldBitmap = NULL;

	// If there is nothing to scroll then don't show the thumb
	if (m_nRange)
	{
		// =====  draw thumb  =====

		VERIFY(bitmap.LoadBitmap(m_bThumbColor ? 
			IDB_VERTICAL_SCROLLBAR_THUMB : 
			IDB_VERTICAL_SCROLLBAR_THUMB_NO_COLOR));
		pOldBitmap = bitmapDC.SelectObject(&bitmap);

		COLORREF rgbThumb = m_bThumbHover ? m_ThumbHoverColor : m_ThumbColor;
		COLORREF rgbPrev = 0;

		// add desired color to thumb
		for (int x = 0; x < m_nBitmapWidth; x++)
		{
			for (int y = 0; y < m_nBitmapHeight; y++)
			{
				COLORREF rgb = bitmapDC.GetPixel(x, y);
				switch (rgb)
				{
					case THUMB_MASK_COLOR:
						if (m_bThumbColor)
							bitmapDC.SetPixel(x, y, rgbThumb);
						break;

					case THUMB_GRIPPER_MASK_COLOR:
						bitmapDC.SetPixel(x, y, m_bThumbGripper ? 
							THUMB_GRIPPER_COLOR : rgbPrev);
						break;

					case THUMB_UP_TRANSPARENT_MASK_COLOR:
						bitmapDC.SetPixel(x, y, 
							memDC.GetPixel(x, nChannelStart));
						break;

					case THUMB_DOWN_TRANSPARENT_MASK_COLOR:
						bitmapDC.SetPixel(x, y, 
							memDC.GetPixel(x, nChannelStart+nChannelHeight-1));
						break;
				}
				rgbPrev = rgb;
			}
		}

		memDC.StretchBlt(m_rectThumb.left, m_rectThumb.top, 
			m_rectThumb.Width(), m_rectThumb.Height(), 
			&bitmapDC, 0, 0, m_nBitmapWidth, m_nBitmapHeight, SRCCOPY);

		if (pOldBitmap)
			bitmapDC.SelectObject(pOldBitmap);
		if (bitmap.GetSafeHandle())
			bitmap.DeleteObject();
		pOldBitmap = NULL;
	}
	else
	{
		m_rectThumb = CRect(-1,-1,-1,-1);
	}

	memDC.FrameRect(&m_rectClient, &brushFrame);
}

*/

///////////////////////////////////////////////////////////////////////////////
// OnLButtonDown
void CXScrollBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	if (m_bHorizontal)
	{
/*		int nWidth=m_rectClient.Width();
		rcArrow1=m_rectClient;
		rcThumb=m_rectClient;
		rcChannel=m_rectClient;
		rcArrow2=m_rectClient;

		rcArrow1.bottom = m_rectClient.top+nWidth;
		rcArrow2.top = m_rectClient.bottom-nWidth;
		if (rcArrow1.bottom>m_rectClient.bottom) rcArrow1.bottom =m_rectClient.bottom ;
		if (rcArrow2.top<m_rectClient.top) rcArrow2.top =m_rectClient.top ;
		rcChannel.top = rcArrow1.bottom +1;
		rcChannel.bottom =rcArrow2.top -1;
		
		int nChannelHeight=rcChannel.Height();
		int nRealChannelHeight=nChannelHeight-THUMBLENGTH;
		int nMid;
		if ((m_nMaxPos-m_nMinPos)==0)
			nMid=rcChannel.top+THUMBLENGTH/2;
		else
			nMid=rcChannel.top +THUMBLENGTH/2+ m_nPos*nRealChannelHeight/(m_nMaxPos-m_nMinPos);
		rcThumb.top =nMid-THUMBLENGTH/2;
		rcThumb.bottom =nMid+THUMBLENGTH/2;
		if (rcThumb.top <rcChannel.top )
			rcThumb.top =rcChannel.top ;
		if (rcThumb.bottom > rcChannel.bottom )
			rcThumb.bottom  =rcChannel.bottom ;*/

		if (m_rcThumb.PtInRect(point))
		{
			m_bMouseDown = TRUE;
		}
		else if (m_rcArrow2.PtInRect(point))
		{
			m_bMouseDownArrowRight = TRUE;
			SetTimer(TIMER_LBUTTON_PRESSED, 150, NULL);
		}
		else if (m_rcArrow1.PtInRect(point))
		{
			m_bMouseDownArrowLeft = TRUE;
			SetTimer(TIMER_LBUTTON_PRESSED, 150, NULL);
		}
		else	// button down in channel
		{
			SetPositionFromThumb(point);
			Draw();
			if (m_pParent && ::IsWindow(m_pParent->m_hWnd))
				m_pParent->SendMessage(WM_HSCROLL, MAKELONG(SB_THUMBTRACK, m_nPos), 
				(LPARAM)m_hWnd);
		}

/*		CRect rectLeftArrow(0, 0, m_nBitmapWidth, m_rectClient.Height());
		CRect rectRightArrow(m_rectClient.Width() - m_nBitmapWidth, 0, 
			m_rectClient.Width(), m_rectClient.Height());
		CRect rectThumb(m_nThumbLeft, 0, m_nThumbLeft + m_nBitmapWidth, 
			m_rectClient.Height());

		if (rectThumb.PtInRect(point))
		{
			m_bMouseDown = TRUE;
		}
		else if (rectRightArrow.PtInRect(point))
		{
			m_bMouseDownArrowRight = TRUE;
			SetTimer(TIMER_LBUTTON_PRESSED, 150, NULL);
		}
		else if (rectLeftArrow.PtInRect(point))
		{
			m_bMouseDownArrowLeft = TRUE;
			SetTimer(TIMER_LBUTTON_PRESSED, 150, NULL);
		}
		else	// button down in channel
		{
			m_nThumbLeft = point.x - m_nBitmapWidth / 2;
			SetPositionFromThumb(point);
			Draw();

			if (m_pParent && ::IsWindow(m_pParent->m_hWnd))
				m_pParent->SendMessage(WM_HSCROLL, MAKELONG(SB_THUMBTRACK, m_nPos),
				(LPARAM)m_hWnd);
		}
		*/
	}
	else
	{
/*
		CRect rcArrow1,rcThumb,rcChannel,rcArrow2;
		int nWidth=m_rectClient.Width();
		
		rcArrow1=m_rectClient;
		rcThumb=m_rectClient;
		rcChannel=m_rectClient;
		rcArrow2=m_rectClient;

		rcArrow1.bottom = m_rectClient.top+nWidth;
		rcArrow2.top = m_rectClient.bottom-nWidth;
		if (rcArrow1.bottom>m_rectClient.bottom) rcArrow1.bottom =m_rectClient.bottom ;
		if (rcArrow2.top<m_rectClient.top) rcArrow2.top =m_rectClient.top ;
		rcChannel.top = rcArrow1.bottom +1;
		rcChannel.bottom =rcArrow2.top -1;
		
		int nChannelHeight=rcChannel.Height();
		int nRealChannelHeight=nChannelHeight-THUMBLENGTH;
		int nMid;
		if ((m_nMaxPos-m_nMinPos)==0)
			nMid=rcChannel.top+THUMBLENGTH/2;
		else
			nMid=rcChannel.top +THUMBLENGTH/2+ m_nPos*nRealChannelHeight/(m_nMaxPos-m_nMinPos);
		rcThumb.top =nMid-THUMBLENGTH/2;
		rcThumb.bottom =nMid+THUMBLENGTH/2;
		if (rcThumb.top <rcChannel.top )
			rcThumb.top =rcChannel.top ;
		if (rcThumb.bottom > rcChannel.bottom )
			rcThumb.bottom  =rcChannel.bottom ;
*/
		if (m_rcThumb.PtInRect(point))
		{
			m_bMouseDown = TRUE;
		}
		else if (m_rcArrow2.PtInRect(point))
		{
			m_bMouseDownArrowDown = TRUE;
			SetTimer(TIMER_LBUTTON_PRESSED, 150, NULL);
		}
		else if (m_rcArrow1.PtInRect(point))
		{
			m_bMouseDownArrowUp = TRUE;
			SetTimer(TIMER_LBUTTON_PRESSED, 150, NULL);
		}
		else	// button down in channel
		{
			SetPositionFromThumb(point);
			Draw();
			if (m_pParent && ::IsWindow(m_pParent->m_hWnd))
				m_pParent->SendMessage(WM_VSCROLL, MAKELONG(SB_THUMBTRACK, m_nPos), 
				(LPARAM)m_hWnd);
		}



/*
		CRect rectUpArrow(0, 0, m_rectClient.Width(), m_nBitmapHeight);
		CRect rectDownArrow(0, m_rectClient.Height() - m_nBitmapHeight, m_rectClient.Width(), m_rectClient.Height());
		CRect rectThumb(0, m_nThumbTop, m_rectClient.Width(), m_nThumbTop + m_nBitmapHeight);

		if (rectThumb.PtInRect(point))
		{
			m_bMouseDown = TRUE;
		}
		else if (rectDownArrow.PtInRect(point))
		{
			m_bMouseDownArrowDown = TRUE;
			SetTimer(TIMER_LBUTTON_PRESSED, 150, NULL);
		}
		else if (rectUpArrow.PtInRect(point))
		{
			m_bMouseDownArrowUp = TRUE;
			SetTimer(TIMER_LBUTTON_PRESSED, 150, NULL);
		}
		else	// button down in channel
		{
			m_nThumbTop = point.y - m_nBitmapHeight / 2;
			SetPositionFromThumb();
			Draw();

			if (m_pParent && ::IsWindow(m_pParent->m_hWnd))
				m_pParent->SendMessage(WM_VSCROLL, MAKELONG(SB_THUMBTRACK, m_nPos), 
				(LPARAM)m_hWnd);
		}*/
	}

//	CStatic::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// OnLButtonUp
void CXScrollBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	UpdateThumbPosition();
	KillTimer(1);
	ReleaseCapture();

	if (m_bHorizontal)
	{
/*		CRect rectLeftArrow(0, 0, m_nBitmapWidth, m_rectClient.Height());
		CRect rectRightArrow(m_rectClient.Width() - m_nBitmapWidth, 0, 
		m_rectClient.Width(), m_rectClient.Height());
		CRect rectThumb(m_nThumbLeft, 0, m_nThumbLeft + m_nBitmapWidth, 
		m_rectClient.Height());*/

		if (m_rcArrow1.PtInRect(point))
		{
			ScrollLeft();
		}
		else if (m_rcArrow2.PtInRect(point))
		{
			ScrollRight();
		}
		else if (m_rcThumb.PtInRect(point))
		{
			m_bThumbHover = TRUE;
			Invalidate();
			SetTimer(TIMER_MOUSE_OVER_THUMB, 50, NULL);
		}

		m_bMouseDownArrowLeft = FALSE;
		m_bMouseDownArrowRight = FALSE;
	}
	else
	{
/*		CRect rectUpArrow(0, 0, m_rectClient.Width(), m_nBitmapHeight);
		CRect rectDownArrow(0, m_rectClient.Height() - m_nBitmapHeight, m_rectClient.Width(), m_rectClient.Height());
		CRect rectThumb(0, m_nThumbTop, m_rectClient.Width(), m_nThumbTop + m_nBitmapHeight);
*/
		if (m_rcArrow1.PtInRect(point))
		{
			ScrollUp();
		}
		else if (m_rcArrow2.PtInRect(point))
		{
			ScrollDown();
		}
		else if (m_rcThumb.PtInRect(point))
		{
			m_bThumbHover = TRUE;
			Invalidate();
			SetTimer(TIMER_MOUSE_OVER_THUMB, 50, NULL);
		}

		m_bMouseDownArrowUp = FALSE;
		m_bMouseDownArrowDown = FALSE;
	}

	m_bMouseDown = FALSE;
	m_bDragging = FALSE;

//	CStatic::OnLButtonUp(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// OnMouseMove
void CXScrollBar::OnMouseMove(UINT nFlags, CPoint point)
{
	BOOL bOldThumbHover = m_bThumbHover;
	m_bThumbHover = FALSE;

	if (m_rectThumb.PtInRect(point))
		m_bThumbHover = TRUE;

	if (m_bMouseDown)
		m_bDragging = TRUE;

	if (m_bDragging)
	{
		if (m_bHorizontal)
		{
//			m_nThumbLeft = point.x - m_nBitmapWidth / 2;

			SetPositionFromThumb(point);

			if (m_pParent && ::IsWindow(m_pParent->m_hWnd))
				m_pParent->SendMessage(WM_HSCROLL, MAKELONG(SB_THUMBTRACK, m_nPos), 
					(LPARAM)m_hWnd);
		}
		else
		{
//			m_nThumbTop = point.y - m_nBitmapHeight / 2;
			SetPositionFromThumb(point);
			if (m_pParent && ::IsWindow(m_pParent->m_hWnd))
				m_pParent->SendMessage(WM_VSCROLL, MAKELONG(SB_THUMBTRACK, m_nPos), 
					(LPARAM)m_hWnd);
		}

		Draw();
	}

	if (bOldThumbHover != m_bThumbHover)
	{
		Invalidate();
		SetTimer(TIMER_MOUSE_OVER_THUMB, 50, NULL);
	}

	CStatic::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// OnTimer
void CXScrollBar::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == TIMER_MOUSE_OVER_BUTTON)	// mouse is in an arrow button,
												// and left button is down
	{
		if (m_bMouseDownArrowLeft)
			ScrollLeft();
		if (m_bMouseDownArrowRight)
			ScrollRight();
		if (m_bMouseDownArrowUp)
			ScrollUp();
		if (m_bMouseDownArrowDown)
			ScrollDown();
	}
	else if (nIDEvent == TIMER_LBUTTON_PRESSED)	// mouse is in an arrow button,
												// and left button has just been pressed
	{
		KillTimer(nIDEvent);

		if (m_bMouseDownArrowLeft || 
			m_bMouseDownArrowRight || 
			m_bMouseDownArrowUp || 
			m_bMouseDownArrowDown)
		{
			// debounce left click
			SetTimer(TIMER_MOUSE_OVER_BUTTON, 100, NULL);
		}
	}
	else if (nIDEvent == TIMER_MOUSE_OVER_THUMB)	// mouse is over thumb
	{
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient(&point);

		if (!m_rectThumb.PtInRect(point))
		{
			// no longer over thumb, restore thumb color
			m_bThumbHover = FALSE;
			KillTimer(nIDEvent);
			::SetCursor(::LoadCursor(NULL, IDC_ARROW));
			Invalidate();
		}
	}

	CStatic::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////////////////
// ScrollLeft
void CXScrollBar::ScrollLeft()
{
	if (m_nPos > 0)
		m_nPos--;
	if (m_pParent && ::IsWindow(m_pParent->m_hWnd))
		m_pParent->SendMessage(WM_HSCROLL, MAKELONG(SB_LINELEFT,0), (LPARAM)m_hWnd);
	UpdateThumbPosition();
}

///////////////////////////////////////////////////////////////////////////////
// ScrollRight
void CXScrollBar::ScrollRight()
{
	if (m_nPos < m_nRange)
		m_nPos++;
	if (m_pParent && ::IsWindow(m_pParent->m_hWnd))
		m_pParent->SendMessage(WM_HSCROLL, MAKELONG(SB_LINERIGHT,0), (LPARAM)m_hWnd);
	UpdateThumbPosition();
}

///////////////////////////////////////////////////////////////////////////////
// ScrollUp
void CXScrollBar::ScrollUp()
{
	if (m_nPos > 0)
		m_nPos--;
	if (m_pParent && ::IsWindow(m_pParent->m_hWnd))
		m_pParent->SendMessage(WM_VSCROLL, MAKELONG(SB_LINEUP,0), (LPARAM)m_hWnd);
	UpdateThumbPosition();
}

///////////////////////////////////////////////////////////////////////////////
// ScrollDown
void CXScrollBar::ScrollDown()
{
	if (m_nPos < m_nRange)
		m_nPos++;
	if (m_pParent && ::IsWindow(m_pParent->m_hWnd))
		m_pParent->SendMessage(WM_VSCROLL, MAKELONG(SB_LINEDOWN,0), (LPARAM)m_hWnd);
	UpdateThumbPosition();
}

///////////////////////////////////////////////////////////////////////////////
// SetPositionFromThumb
void CXScrollBar::SetPositionFromThumb(POINT point)
{
	if (m_bHorizontal)
	{
		if (point.x <(m_rcChannel.left + THUMBLENGTH/2))
			m_nPos=m_nMinPos;
		else if (point.x >(m_rcChannel.right -THUMBLENGTH/2))
			m_nPos=m_nMaxPos;
		else if (m_rcChannel.Width()!=THUMBLENGTH)
			m_nPos=m_nMinPos+(point.x - (m_rcChannel.left + THUMBLENGTH/2))*(m_nMaxPos-m_nMinPos)/(m_rcChannel.Width()-THUMBLENGTH);
	}
	else
	{
		if (point.y <(m_rcChannel.top + THUMBLENGTH/2))
			m_nPos=m_nMinPos;
		else if (point.y >(m_rcChannel.bottom -THUMBLENGTH/2))
			m_nPos=m_nMaxPos;
		else if (m_rcChannel.Height()!=THUMBLENGTH)
			m_nPos=m_nMinPos+(point.y - (m_rcChannel.top + THUMBLENGTH/2))*(m_nMaxPos-m_nMinPos)/(m_rcChannel.Height()-THUMBLENGTH);
	}
}

///////////////////////////////////////////////////////////////////////////////
// UpdateThumbPosition
void CXScrollBar::UpdateThumbPosition()
{
/*	double dPixels, dMax, dInterval, dPos;

	dMax = m_nRange;
	dPos = m_nPos;

	if (m_bHorizontal)
	{
		dPixels   = m_rectClient.Width() - 3*m_nBitmapWidth;
		dInterval = dPixels / dMax;
		double dThumbLeft = dPos * dInterval + 0.5;
		m_nThumbLeft = m_nBitmapWidth + (int)dThumbLeft;
	}
	else
	{
		dPixels   = m_rectClient.Height() - 3*m_nBitmapHeight;
		dInterval = dPixels / dMax;
		double dThumbTop = dPos * dInterval + 0.5;
		m_nThumbTop = m_nBitmapHeight + (int)dThumbTop;
	}

	LimitThumbPosition();
*/
	Draw();
}

///////////////////////////////////////////////////////////////////////////////
// LimitThumbPosition
void CXScrollBar::LimitThumbPosition()
{
/*	if (m_bHorizontal)
	{
		if ((m_nThumbLeft + m_nBitmapWidth) > (m_rectClient.Width() - m_nBitmapWidth))
			m_nThumbLeft = m_rectClient.Width() - 2*m_nBitmapWidth;

		if (m_nThumbLeft < (m_rectClient.left + m_nBitmapWidth))
			m_nThumbLeft = m_rectClient.left + m_nBitmapWidth;
	}
	else
	{
		if ((m_nThumbTop + m_nBitmapHeight) > (m_rectClient.Height() - m_nBitmapHeight))
			m_nThumbTop = m_rectClient.Height() - 2*m_nBitmapHeight;

		if (m_nThumbTop < (m_rectClient.top + m_nBitmapHeight))
			m_nThumbTop = m_rectClient.top + m_nBitmapHeight;
	}
	*/
}

///////////////////////////////////////////////////////////////////////////////
// SetScrollRange
void CXScrollBar::SetScrollRange(int nMinPos,
								 int nMaxPos,
								 BOOL bRedraw /*= TRUE*/)
{
	m_nMinPos = nMinPos;
	m_nMaxPos = nMaxPos;
	if (m_nMinPos < m_nMaxPos)
		m_nRange = m_nMaxPos - m_nMinPos;
	else
		m_nRange = m_nMinPos - m_nMaxPos;

	if (bRedraw)
		Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
// SetScrollPos
int CXScrollBar::SetScrollPos(int nPos, BOOL bRedraw /*= TRUE*/)
{
	int nOldPos = m_nPos;

	m_nPos = nPos;

	UpdateThumbPosition();

	if (bRedraw)
		Invalidate();

	return nOldPos;
}

///////////////////////////////////////////////////////////////////////////////
// OnSetCursor
BOOL CXScrollBar::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
	if (m_bThumbHover && m_hCursor)
		::SetCursor(m_hCursor);
	else
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));

	return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// SetDefaultCursor - borrowed from XHyperLink.cpp
void CXScrollBar::SetDefaultCursor()
{
	if (m_hCursor == NULL)				// No cursor handle - try to load one
	{
		// First try to load the Win98 / Windows 2000 hand cursor

		TRACE(_T("loading from IDC_HAND\n"));
		m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_HAND);

		if (m_hCursor == NULL)			// Still no cursor handle -
										// load the WinHelp hand cursor
		{
			// The following appeared in Paul DiLascia's Jan 1998 MSJ articles.
			// It loads a "hand" cursor from the winhlp32.exe module.

			TRACE(_T("loading from winhlp32\n"));

			// Get the windows directory
			CString strWndDir;
			GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
			strWndDir.ReleaseBuffer();

			strWndDir += _T("\\winhlp32.exe");

			// This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
			HMODULE hModule = LoadLibrary(strWndDir);
			if (hModule)
			{
				HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
				if (hHandCursor)
					m_hCursor = CopyCursor(hHandCursor);
				FreeLibrary(hModule);
			}
		}
	}
}


void CXScrollBar::SetSBRect(LPRECT lpRect,BOOL bDraw)
{
	m_rectClient=lpRect;
	this->MoveWindow(lpRect,bDraw);
	if (bDraw)
		Draw();
}
void CXScrollBar::GetScrollRange(LPINT lpMinPos,LPINT lpMaxPos)
{
	if (lpMinPos!=NULL)
		*lpMinPos=m_nMinPos;
	if (lpMaxPos!=NULL)
		*lpMaxPos=m_nMaxPos;
}

void CXScrollBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnLButtonDown(nFlags,point);
	OnLButtonDown(nFlags,point);
}