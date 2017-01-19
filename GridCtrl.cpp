// GridCtrl.cpp : implementation file
//
// MFC Grid Control
//
// Written by Chris Maunder 
//        mailto:chrismaunder@codeguru.com
//
// Copyright (c) 1998.
//
// The code contained in this file is based on the original
// WorldCom Grid control written by Joe Willcoxson,
//        mailto:chinajoe@aol.com
//        http://users.aol.com/chinajoe
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. If the source code in this file is used in 
// any  commercial application then a statement along the lines of 
// "Portions copyright (c) Chris Maunder, 1998" must be included in 
// the startup banner, "About" box or printed documentation. An email 
// letting me know that you are using it would be nice as well. That's 
// not much to ask considering the amount of work that went into this.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Expect bugs!
// 
// Please use and enjoy, and let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into 
// this file. 
//
//  History:
//  --------
//  This control is constantly evolving, sometimes due to new features that I
//  feel are necessary, and sometimes due to existing bugs. Where possible I 
//  have credited the changes to those who contributed code corrections or
//  enhancements (names in brackets) or code suggestions (suggested by...)
//   
//          1.0     20 Feb 1998   First release version.
//          1.01    24 Feb 1998   Memory leak fix (Jens Bohlmann)
//                                Fixec typo (my fault!) in CMemDC.h - Claus Arend-Schneider)
//                                Bug in GetSelectedCount (Lyn Newton)
//          1.02    4  Mar 1998   Scrolling a little neater (less dead area)
//                                Cell selection via OnTimer correctly updates Focus cell (Suggested by Lyn Newton)
//          1.03    17 Mar 1998   Clipboard functions added, Intellimouse support
//                                Using 32 bit scroll pos functions instead of 16 bit ("cronos")
//                                Added OLE drag and drop.
//          1.04     6 Apr 1998   Added Ctrl-A = Select All, fixed CGridDropTarget 
//                                problem, minor bug in CopyTextFromGrid (assert on
//                                empty string). Cleaned up reponse to m_bEditable
//                                (OnDrop and Ctrl-X disabled)
//          1.05    10 May 1998   Memory leak fixed. (Yuheng Zhao)
//                                Changed OLE initialisation (Carlo Comino)
//                                Added separate fore + background cell colours (Suggested by John Crane)
//                                ExpandToFit etc cleaned up - now decreases and
//                                increases cell sizes to fit client area.
//                                Added notification messages for the grid's parent (Suggested by 
//                                Added GVIS_READONLY state
//          1.06    20 May 1998   Added TAB key handling. (Daniela Rybarova)
//                                Intellimouse code correction for whole page scrolling (Paul Grant) 
//                                Fixed 16 bit thumb track problems (now 32 bit) (Paul Grant) 
//                                Fixed accelerator key problem in CInPlaceEdit (Matt Weagle)
//                                Fixed Stupid ClassWizard code parsing problem (Michael A. Barnhart)
//                                Double buffering now programmatically selectable
//                                Workaround for win95 drag and drop registration problem
//                                Corrected UNICODE implementation of clipboard stuff
//                                Dragging and dropping from a selection onto itself no 
//                                no longer causes the cells to be emptied
//          1.07    28 Jul 1998   Added EnsureVisible. (Roelf Werkman)
//                                Fixed delete key problem on read-only cells. (Serge Weinstock)
//                                OnEndInPlaceEdit sends notification AFTER storing
//                                the modified text in the cell.
//                                Added CreateInPlaceEditControl to make it easier to 
//                                change the way cells are edited. (suggested by Chris Clark)
//                                Added Set/GetGridColor.
//                                CopyTextToClipboard and PasteTextToGrid problem with 
//                                blank cells fixed, and CopyTextToClipboard tweaked.
//                                SetModified called when cutting text or hitting DEL. (Jonathan Watters)
//                                Focus cell made visible when editing begins.
//                                Blank lines now treated correctly when pasting data.
//                                Removed ES_MULTILINE style from the default edit control.
//                                Added virtual CreateCell(row, col) function.
//                                Fonts now specified on a per-cell basis using Get/SetItemFont.
//          1.08     6 Aug 1998   Ctrl+arrows now allows cell navigation. Modified
//                                CreateInPlaceEditControl to accept ID of control.
//                                Added Titletips to grid cells. (Added EnableTitleTips / GetTitleTips)
//          1.09    12 Sep 1998   When printing, parent window title is printed in header - Gert Rijs
//                                GetNextItem search with GVNI_DROPHILITED now returns
//                                cells with GVIS_DROPHILITED set, instead of GVIS_FOCUSED (Franco Bez)
//                                (Also fixed minor bug in GetNextItem) (Franco Bez)
//                                Cell selection using Shift+arrows works - Franco Bez 
//                                SetModified called after edits ONLY if contents changed (Franco Bez)
//                                Cell colours now dithered in 256 colour screens.
//                                Support for MSVC 4.2 (Graham Cheetham)
//          1.10    30 Nov 1998   Titletips now disappear on a scroll event. Compiler errors
//                                fixed. Grid lines drawing fixed (Graham Cheetham). 
//                                Cell focus fix on Isert Row/Col (Jochen Kauffmann) 
//                                Added DeleteNonFixedRows() (John Rackley)
//                                Message #define conflict fixed (Oskar Wieland)
//                                Titletips & cell insert/delete fix (Ramesh Dhar) 
//                                Titletips repeat-creation bug fixed.
//                                GVN_SELCHANGED message now sends current cell ID
//                                Font resource leak in GetTextExtent fixed (Gavin Jerman)
//                                More TAB fixes (Andreas Ruh)
//          1.11    1 Dec 1998    GetNextItem bug fix (suggested by Francis Fu)
//                                InsertColumn (-1) fix (Roy Hopkins)
//                                Was too liberal with the "IsEditable"'s. oops. (Michel Hete)
//          1.11a   4 Jan 1999    Compiler errors in VC6 fixed.
//                                          
// TODO:
//    - OnOutOfMemory function instead of exceptions
//    - Decrease timer interval over time to speed up selection over time
// 
// NOTE: Grid data is stored row-by-row, so all operations on large numbers
//       of cells should be done row-by-row as well.
// 
// KNOWN ISSUES TO BE ADDRESSED (Please don't send bug reports): 
// * Killfocus comes to late when a command is selected by the Menu. 
//   When you are editing a cell and choose a Menuitem that searches for all the
//   modified cells it is not found. When you chose the menu a second time it is
//   found. I assume that the Menu command is executed before the cell receives the
//   KillFocus event. Expect similar Problems with accelerators. (Franco Bez)
// * When you select a cell and move the mouse around (with the Left button down 
//   i.e continuing with your selection) - if the mouse is over the Fixed column 
//   or Row the drawing of the selected region is strange - in particular as you 
//   move up and down say the Left Fixed Column notice the behaviour of the Focus 
//   Cell - it is out of sync. (Vinay Desai)
//
/////////////////////////////////////////////////////////////////////////////
    
#include "stdafx.h"
#include "MemDC.h"
#include "BCMenu.h"
#include "MainFrm.h"

//#include "Grid.h"
#include "GridCtrl.h"
#include "InPlaceEdit.h"
#include "CTaiShanDoc.h"

#include "cellRange.h"
#include "CTaiShanReportView.h"

#include "StatisticSettingDlg.h"

// OLE stuff for clipboard operations
#include <afxadv.h>            // For CSharedFile
#include <afxole.h>            // For COleDataSource
#include <afxconv.h>           // For LPTSTR -> LPSTR macros

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HEADER_HEIGHT       2    // For printing
#define FOOTER_HEIGHT       2
#define LEFT_MARGIN         4
#define RIGHT_MARGIN        4
#define TOP_MARGIN          1
#define BOTTOM_MARGIN       1 
#define GAP                 1


#define MOUSE_ALL  7001
#define MOUSE_COL  7002
#define MOUSE_ROW  7003
#define MOUSE_CELLS 7004
#define MOUSE_NOTHINGS 7005
#define SELECTED_CELL_FONT_WEIGHT 600    // wieght of text for selected items
#define IDC_INPLACE_EDIT            8    // ID of inplace edit controls
#define WM_USER_HSCROLL (WM_USER + 5)
#define FIXCOLS  3

extern void Swap(STOCKDATASHOW &m_pData1,STOCKDATASHOW &m_pData2);


IMPLEMENT_DYNCREATE(CGridCtrl, CWnd)

void AFXAPI DDX_GridControl(CDataExchange* pDX, int nIDC, CGridCtrl& rControl)
{
    if (rControl.GetSafeHwnd() == NULL)    // not subclassed yet
    {
        ASSERT(!pDX->m_bSaveAndValidate);

        HWND hWndCtrl = pDX->PrepareCtrl(nIDC);

        if (!rControl.SubclassWindow(hWndCtrl))
        {
            ASSERT(FALSE);      // possibly trying to subclass twice?
            AfxThrowNotSupportedException();
        }
#ifndef _AFX_NO_OCC_SUPPORT
        else
        {
            // If the control has reparented itself (e.g., invisible control),
            // make sure that the CWnd gets properly wired to its control site.
            if (pDX->m_pDlgWnd->GetSafeHwnd() != ::GetParent(rControl.GetSafeHwnd()))
                rControl.AttachControlSite(pDX->m_pDlgWnd);
        }
#endif //!_AFX_NO_OCC_SUPPORT

    }
}

// Get the number of lines to scroll with each mouse wheel notch
// Why doesn't windows give us this function???
UINT GetMouseScrollLines()
{
    int nScrollLines = 3;            // reasonable default
    HKEY hKey;

    if (RegOpenKeyEx(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
                     0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        TCHAR szData[128];
        DWORD dwKeyDataType;
        DWORD dwDataBufSize = sizeof(szData);

        if (RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType,
                           (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
        {
            nScrollLines = _tcstoul(szData, NULL, 10);
        }
        RegCloseKey(hKey);
    }

    return nScrollLines;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl

CGridCtrl::CGridCtrl(int nRows, int nCols, int nFixedRows, int nFixedCols)
{

    RegisterWindowClass();

    // Initialize OLE libraries
    m_bMustUninitOLE = FALSE;

#ifdef GRIDCONTROL_USE_OLE_DRAGDROP
    _AFX_THREAD_STATE* pState = AfxGetThreadState();
    if (!pState->m_bNeedTerm)
    {
        SCODE sc = ::OleInitialize(NULL);
        if (FAILED(sc))
            AfxMessageBox(_T("OLE initialization failed. Make sure that the OLE libraries are the correct version"));
        else
            m_bMustUninitOLE = TRUE;
    }
#endif

    // Store the system colours in case they change. The gridctrl uses
    // these colours, and in OnSysColorChange we can check to see if 
    // the gridctrl colours have been changed from the system colours.
    // If they have, then leave them, otherwise change them to reflect
    // the new system colours.
    m_crWindowText       = ::GetSysColor(COLOR_WINDOWTEXT);
    m_crWindowColour     = ::GetSysColor(COLOR_WINDOW);
    m_cr3DFace           = ::GetSysColor(COLOR_3DFACE);
    m_crShadow           = ::GetSysColor(COLOR_3DSHADOW);
    m_crGridColour       = RGB(0,0,0);
    m_crCursor           = RGB(127,127,127);
	//m_crCursor           = RGB(200,200,250);

    m_nRows              = 0;
    m_nCols              = 0;
    m_nFixedRows         = 0;
    m_nFixedCols         = 0;
    m_nRealRow           = 0;

    m_nDefCellWidth     = 65;        // These will get changed to something meaningful
    m_nDefCellHeight     = 20;        //    when the window is created or subclassed

    m_nVScrollMax        = 0;         // Scroll position
    m_nHScrollMax        = 0;
    m_nMargin            = 0;         // cell padding
    m_nRowsPerWheelNotch = GetMouseScrollLines(); // Get the number of lines
                                                  // per mouse wheel notch to scroll

    m_MouseMode          = MOUSE_NOTHING;
    m_nGridLines         = GVL_BOTH;
    m_bEditable          = TRUE;
    m_bListMode          = FALSE;
    m_bAllowDraw         = TRUE;      // allow draw updates
    m_bEnableSelection   = TRUE;
    m_bAllowRowResize    = TRUE;
    m_bAllowColumnResize = TRUE;
    m_bSortOnClick       = TRUE;      // Sort on header row click if in list mode
    m_bHandleTabKey      = TRUE;
    m_bDoubleBuffer      = TRUE;      // Use double buffering to avoid flicker?
    //m_bTitleTips         = TRUE;      // show cell title tips
    m_bTitleTips         = FALSE;      // disable show cell title tips

    m_bAscending         = FALSE;      // sorting stuff
    m_SortColumn         = -1;

    m_nTimerID           = 0;         // For drag-selection
    m_nTimerInterval     = 25;        // (in milliseconds)
    m_nResizeCaptureRange = 3;        // When resizing columns/row, the cursor has to be 
                                      // within +/-3 pixels of the dividing line for 
                                      // resizing to be possible
    m_pImageList         = NULL;      
    m_bAllowDragAndDrop  = FALSE;     // for drag and drop
	//////////// Modi by zjh
	m_scrollPos = 0;
	m_GridLineColor = RGB(0,0,255);    

	//Marenan +
	//m_scrollVPos=0;
	//m_bInnerHScroll=TRUE;
	//m_bInnerVScroll=TRUE;
	//m_pHScroll=NULL;
	//m_pVScroll=NULL;
	//Marenan +

    // Initially use the system message font for the GridCtrl font
    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
     VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));    
    memcpy(&m_Logfont, &(ncm.lfSmCaptionFont  ), sizeof(LOGFONT));
    m_Logfont.lfWeight=300;
    // Set up the initial grid size
    SetRowCount(nRows);
    SetColumnCount(nCols);
    SetFixedRowCount(nFixedRows);
    SetFixedColumnCount(nFixedCols);

    // Set the colours
    SetTextColor(m_crWindowText);
    SetTextBkColor(m_crWindowColour);
    SetBkColor(m_crShadow);
    SetFixedTextColor(m_crWindowText);
    SetFixedBkColor(m_cr3DFace);


    // set initial selection range (ie. none)
//    m_SelectedCellMap.RemoveAll();
//    m_PrevSelectedCellMap.RemoveAll();
    m_pColData=NULL; 
	m_pDoc=NULL;
	m_bRefresh=FALSE;
	m_BeforeRow=0;

	m_clrBackGround = RGB(0,0,0);
}

CGridCtrl::~CGridCtrl()
{
    DeleteAllItems();

    DestroyWindow();

    m_Font.DeleteObject();

    // Uninitialize OLE support
    if (m_bMustUninitOLE)
        ::OleUninitialize();
	if(m_pColData!=NULL)
	{
	   GlobalUnlock((HGLOBAL)m_pColData);        //释放内存
	   GlobalFree( (HGLOBAL)m_pColData);
	}
}

// Register the window class if it has not already been registered.
BOOL CGridCtrl::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetResourceHandle();

    if (!(::GetClassInfo(hInst, GRIDCTRL_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = NULL;
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = GRIDCTRL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls)) {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CGridCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwStyle)
{
    ASSERT(pParentWnd->GetSafeHwnd());

    ///////////Add by Zjh///////////
	dwStyle &= ~WS_HSCROLL;

	if (!CWnd::Create(GRIDCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID)) 
        return FALSE;

#ifdef GRIDCONTROL_USE_OLE_DRAGDROP
    m_DropTarget.Register(this);
#endif

    // Create titletips
#ifdef GRIDCONTROL_USE_TITLETIPS
    if (m_bTitleTips)        
        m_TitleTip.Create(this);
#endif

    // The number of rows and columns will only be non-zero if the constructor
    // was called with non-zero initialising parameters. If this window was created
    // using a dialog template then the number of rows and columns will be 0 (which
    // means that the code below will not be needed - which is lucky 'cause it ain't
    // gonna get called in a dialog-template-type-situation.

    try {
        m_arRowHeights.SetSize(m_nRows);    // initialize row heights
        m_arColWidths.SetSize(m_nCols);     // initialize column widths
    }
    catch (CMemoryException *e) {
        e->ReportError();
        e->Delete();
        return FALSE;
    }

	int i;

    for (i = 0; i < m_nRows; i++) m_arRowHeights[i] = m_nDefCellHeight;
    for (i = 0; i < m_nCols; i++)      m_arColWidths[i] = m_nDefCellWidth;

    ResetScrollBars();
    return TRUE;
}

void CGridCtrl::PreSubclassWindow() 
{    
    CWnd::PreSubclassWindow();

    HFONT hFont = ::CreateFontIndirect(&m_Logfont);
    OnSetFont((LPARAM)hFont, 0);
    DeleteObject(hFont);
    
    ResetScrollBars();   
}

BOOL CGridCtrl::SubclassWindow(HWND hWnd) 
{    
    if (!CWnd::SubclassWindow(hWnd)) return FALSE;

#ifdef GRIDCONTROL_USE_OLE_DRAGDROP
    m_DropTarget.Register(this);
#endif

#ifdef GRIDCONTROL_USE_TITLETIPS
    if (m_bTitleTips && !IsWindow(m_TitleTip.m_hWnd))
        m_TitleTip.Create(this);
#endif

    return TRUE;
}

LRESULT CGridCtrl::SendMessageToParent(int nRow, int nCol, int nMessage)
{
    if (!IsWindow(m_hWnd))
        return 0;

    NM_GRIDVIEW nmgv;
    nmgv.iRow         = nRow;
    nmgv.iColumn      = nCol;
    nmgv.hdr.hwndFrom = m_hWnd;
    nmgv.hdr.idFrom   = GetDlgCtrlID();
    nmgv.hdr.code     = nMessage;

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
    else
        return 0;
}

BEGIN_MESSAGE_MAP(CGridCtrl, CWnd)
    //{{AFX_MSG_MAP(CGridCtrl)
    ON_WM_PAINT()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_SIZE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
    ON_WM_GETDLGCODE()
    ON_WM_KEYDOWN()
    ON_WM_CHAR()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
    ON_WM_SETCURSOR()
    ON_WM_SYSCOLORCHANGE()
    ON_WM_CAPTURECHANGED()
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
    ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
    ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_REPORT_AVE, OnReportAve)

	//}}AFX_MSG_MAP
#if _MFC_VER >= 0x0421
    ON_WM_MOUSEWHEEL()
    ON_WM_SETTINGCHANGE()
#endif
    ON_MESSAGE(WM_SETFONT, OnSetFont)
    ON_MESSAGE(WM_GETFONT, OnGetFont)
    ON_NOTIFY(GVN_ENDLABELEDIT, IDC_INPLACE_EDIT, OnEndInPlaceEdit)
	ON_COMMAND(ID_REPORT_MAX, &CGridCtrl::OnReportMax)
	ON_COMMAND(ID_REPORT_MIN, &CGridCtrl::OnReportMin)
	ON_COMMAND(ID_REPORT_SET, &CGridCtrl::OnReportSet)
	ON_COMMAND(ID_REPORT_VAR, &CGridCtrl::OnReportVar)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGridCtrl message handlers

void CGridCtrl::OnPaint() 
{
    CPaintDC dc(this);      // device context for painting
    if (m_bDoubleBuffer)    // Use a memory DC to remove flicker
    {
        CMemDC MemDC(&dc);
        OnDraw(&MemDC);
    }
    else                    // Draw raw - this helps in debugging vis problems.
        OnDraw(&dc);
}

BOOL CGridCtrl::OnEraseBkgnd(CDC* /*pDC*/) 
{
    return TRUE;    // Don't erase the background.
}

// Custom background erasure. This gets called from within the OnDraw function,
// since we will (most likely) be using a memory DC to stop flicker. If we just
// erase the background normally through OnEraseBkgnd, and didn't fill the memDC's
// selected bitmap with colour, then all sorts of vis problems would occur
void CGridCtrl::EraseBkgnd(CDC* pDC) 
{
	CRect  VisRect, ClipRect, rect;
    CBrush FixedBack(GetFixedBkColor()),
           TextBack(GetTextBkColor()),
           Back(GetBkColor());

    if (pDC->GetClipBox(ClipRect) == ERROR) return;
    GetVisibleNonFixedCellRange(VisRect);

    // Draw Fixed columns background
    int nFixedColumnWidth = GetFixedColumnWidth();
    if (ClipRect.left < nFixedColumnWidth && ClipRect.top < VisRect.bottom)
        pDC->FillRect(CRect(ClipRect.left, ClipRect.top, nFixedColumnWidth, VisRect.bottom),
                      &FixedBack);
        
    // Draw Fixed rows background
    int nFixedRowHeight = GetFixedRowHeight();
    if (ClipRect.top < nFixedRowHeight && 
        ClipRect.right > nFixedColumnWidth && ClipRect.left < VisRect.right)
        pDC->FillRect(CRect(nFixedColumnWidth-1, ClipRect.top, VisRect.right, nFixedRowHeight),
                      &FixedBack);

    // Draw non-fixed cell background
    if (rect.IntersectRect(VisRect, ClipRect)) 
    {
        CRect CellRect(max(nFixedColumnWidth, rect.left), max(nFixedRowHeight, rect.top),
                       rect.right, rect.bottom);
        pDC->FillRect(CellRect, &TextBack);
    }

    // Draw right hand side of window outside grid
    if (VisRect.right < ClipRect.right) 
        pDC->FillRect(CRect(VisRect.right, ClipRect.top, ClipRect.right, ClipRect.bottom),
                      &Back);

    // Draw bottom of window below grid
    if (VisRect.bottom < ClipRect.bottom && ClipRect.left < VisRect.right) 
        pDC->FillRect(CRect(ClipRect.left, VisRect.bottom, VisRect.right, ClipRect.bottom),
                      &Back);

  }

void CGridCtrl::OnSize(UINT nType, int cx, int cy) 
{
//    if (::IsWindow(GetSafeHwnd()) && GetFocus()->GetSafeHwnd() != GetSafeHwnd()) 
        SetFocus();        // Auto-destroy any InPlaceEdit's

    CWnd::OnSize(nType, cx, cy);
    ResetScrollBars();    
}

UINT CGridCtrl::OnGetDlgCode() 
{
    UINT nCode = DLGC_WANTARROWS | DLGC_WANTCHARS;

    if (m_bHandleTabKey && !IsCTRLpressed()) 
        nCode |= DLGC_WANTTAB;

    return nCode;
}

// If system colours change, then redo colours
void CGridCtrl::OnSysColorChange() 
{
    CWnd::OnSysColorChange();
    
    if (GetTextColor() == m_crWindowText)                   // Still using system colours
        SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetTextBkColor() == m_crWindowColour)
        SetTextBkColor(::GetSysColor(COLOR_WINDOW));
    if (GetBkColor() == m_crShadow)
        SetBkColor(::GetSysColor(COLOR_3DSHADOW));
    if (GetFixedTextColor() == m_crWindowText)
        SetFixedTextColor(::GetSysColor(COLOR_WINDOWTEXT));
    if (GetFixedBkColor() == m_cr3DFace)
        SetFixedBkColor(::GetSysColor(COLOR_3DFACE));

    m_crWindowText   = ::GetSysColor(COLOR_WINDOWTEXT);
    m_crWindowColour = ::GetSysColor(COLOR_WINDOW);
    m_cr3DFace       = ::GetSysColor(COLOR_3DFACE);
    m_crShadow       = ::GetSysColor(COLOR_3DSHADOW);
}

// If we are drag-selecting cells, or drag and dropping, stop now
void CGridCtrl::OnCaptureChanged(CWnd *pWnd) 
{
    if (pWnd->GetSafeHwnd() == GetSafeHwnd()) return;

    // kill timer if active
    if (m_nTimerID != 0)
    {
        KillTimer(m_nTimerID);
        m_nTimerID = 0;
    }

    // Kill drag and drop if active
    if (m_MouseMode == MOUSE_DRAGGING)
        m_MouseMode = MOUSE_NOTHING;
}

#if _MFC_VER >= 0x0421
// If system settings change, then redo colours
void CGridCtrl::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
    CWnd::OnSettingChange(uFlags, lpszSection);
    
    if (GetTextColor() == m_crWindowText)                   // Still using system colours
        SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetTextBkColor() == m_crWindowColour)
        SetTextBkColor(::GetSysColor(COLOR_WINDOW));
    if (GetBkColor() == m_crShadow)
        SetBkColor(::GetSysColor(COLOR_3DSHADOW));
    if (GetFixedTextColor() == m_crWindowText)
        SetFixedTextColor(::GetSysColor(COLOR_WINDOWTEXT));
    if (GetFixedBkColor() == m_cr3DFace)
        SetFixedBkColor(::GetSysColor(COLOR_3DFACE));

    m_crWindowText   = ::GetSysColor(COLOR_WINDOWTEXT);
    m_crWindowColour = ::GetSysColor(COLOR_WINDOW);
    m_cr3DFace       = ::GetSysColor(COLOR_3DFACE);
    m_crShadow       = ::GetSysColor(COLOR_3DSHADOW);

    m_nRowsPerWheelNotch = GetMouseScrollLines(); // Get the number of lines
}
#endif

// For drag-selection. Scrolls hidden cells into view
// TODO: decrease timer interval over time to speed up selection over time
void CGridCtrl::OnTimer(UINT nIDEvent)
{
    ASSERT(nIDEvent == WM_LBUTTONDOWN);
    if (nIDEvent != WM_LBUTTONDOWN) return;
	if(nIDEvent == WM_LBUTTONDOWN) return;


    CPoint pt, origPt;

    if (!GetCursorPos(&origPt)) return;
    ScreenToClient(&origPt);

    CRect rect;
    GetClientRect(rect);

    int nFixedRowHeight = GetFixedRowHeight();
    int nFixedColWidth = GetFixedColumnWidth();

    pt = origPt;
    if (pt.y > rect.bottom)
    {
        //SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
        SendMessage(WM_KEYDOWN, VK_DOWN, 0);

        if (pt.x < rect.left)  pt.x = rect.left;
        if (pt.x > rect.right) pt.x = rect.right;
        pt.y = rect.bottom;
        OnSelecting(GetCellFromPt(pt));
    }
    else if (pt.y < nFixedRowHeight)
    {
        //SendMessage(WM_VSCROLL, SB_LINEUP, 0);
        SendMessage(WM_KEYDOWN, VK_UP, 0);

        if (pt.x < rect.left)  pt.x = rect.left;
        if (pt.x > rect.right) pt.x = rect.right;
        pt.y = nFixedRowHeight + 1;
        OnSelecting(GetCellFromPt(pt));
    }

    pt = origPt;
    if (pt.x > rect.right)
    {
        // SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
        SendMessage(WM_KEYDOWN, VK_RIGHT, 0);

        if (pt.y < rect.top)    pt.y = rect.top;
        if (pt.y > rect.bottom) pt.y = rect.bottom;
        pt.x = rect.right;
        OnSelecting(GetCellFromPt(pt));
    } 
    else if (pt.x < nFixedColWidth)
    {
        //SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
        SendMessage(WM_KEYDOWN, VK_LEFT, 0);

        if (pt.y < rect.top)    pt.y = rect.top;
        if (pt.y > rect.bottom) pt.y = rect.bottom;
        pt.x = nFixedColWidth + 1;
        OnSelecting(GetCellFromPt(pt));
    }

}

// move about with keyboard
void CGridCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	BOOL Is_Shift;
    Is_Shift=FALSE;
    if (!IsValid(m_idCurrentCell)) 
    {
        CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
        return;
    }

    CCellID next = m_idCurrentCell;
    m_BeforeRow=m_idCurrentCell.row;

    BOOL bChangeLine = FALSE;

	//m_bKeyMessage=TRUE;

    if (IsCTRLpressed())
    {
        switch (nChar)
        {
           case 'A': OnEditSelectAll();  break;
           case 'X': OnEditCut();        break;
           case 'C': OnEditCopy();       break;
           case 'V': OnEditPaste();      break;
        }
    }

    switch (nChar)
    {
        case VK_DELETE: 
            if (IsCellEditable(m_idCurrentCell.row, m_idCurrentCell.col))
            {
                SetItemText(m_idCurrentCell.row, m_idCurrentCell.col, _T(""));
                RedrawCell(m_idCurrentCell);
                SetModified(TRUE);
            }
            break;

        case VK_TAB:    
            if (IsSHIFTpressed())
            {
                if (next.col > m_nFixedCols) 
                    next.col--;
                else if (next.col == m_nFixedCols && next.row > m_nFixedRows) 
                {
                    next.row--; 
                    next.col = GetColumnCount() - 1; 
                    bChangeLine = TRUE;
                }
                else
                    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
            }
            else
            {
                if (next.col < (GetColumnCount() - 1)) 
                    next.col++;
                else if (next.col == (GetColumnCount() - 1) && 
                         next.row < (GetRowCount() - 1) )
                {
                    next.row++; 
                    next.col = m_nFixedCols; 
                    bChangeLine = TRUE;
                }
                else
                    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
            } 
            break;

        case VK_DOWN:   
            if (next.row < (GetRowCount() - 1))
                next.row++; 
            break;            
            
        case VK_UP:     
            if (next.row > m_nFixedRows)           
                next.row--; 
            break;

        case VK_RIGHT:  
            if (next.col < (GetColumnCount() - 1)) 
                next.col++; 
            break;
            
        case VK_LEFT:
            if (next.col > m_nFixedCols)           
                next.col--; 
            break;

        case VK_NEXT:  
			{
            CCellID idOldTopLeft = GetTopleftNonFixedCell();
/////////////////////////

//    
			CCellRange m_Range=GetVisibleNonFixedCellRange();
            if(idOldTopLeft.row +(m_Range.GetMaxRow ()-m_Range.GetMinRow ()) > m_nRealRow) 
            {
				//Marenan -
				SetScrollPos32(SB_VERT, 0);
				next.row =1;
				Invalidate();
				//Marenan -
				//Marenan +
				 //this->m_scrollVPos =0 ; 
				 //if (m_bKeyMessage&&m_pVScroll)
				 //{
					// int nMax,nMin;
					// m_pVScroll->GetScrollRange(&nMin,&nMax); 
					// m_pVScroll->SetScrollPos(nMin+m_scrollVPos*nMax/m_nVScrollMax);
				 //}

				 //Invalidate();
				//Marenan +

				 break;
			}
			else
			{
				SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
                CCellID idNewTopLeft = GetTopleftNonFixedCell();

                int increment = idNewTopLeft.row - idOldTopLeft.row;
				if (increment) {
					next.row += increment;
					if (next.row > (m_nRealRow - 1)) 
						next.row = m_nRealRow - 1;
				}
				else
					next.row = m_nRealRow - 1;
				break;
			}
		
			}
//////////////////////////    
        case VK_PRIOR:  
            {
            CCellID idOldTopLeft = GetTopleftNonFixedCell();
            SendMessage(WM_VSCROLL, SB_PAGEUP, 0);
            CCellID idNewTopLeft = GetTopleftNonFixedCell();

            int increment = idNewTopLeft.row - idOldTopLeft.row;
            if (increment) 
            {
                next.row += increment;
                if (next.row < m_nFixedRows) 
                    next.row = m_nFixedRows;
            } else
                next.row = m_nFixedRows;
                break;
            }
    
        case VK_HOME:   
            SendMessage(WM_VSCROLL, SB_TOP, 0);
            next.row = m_nFixedRows;
            break;
        
        case VK_END:    
			{
                CCellID idOldTopLeft = GetTopleftNonFixedCell();
				CCellRange m_Range=GetVisibleNonFixedCellRange();
				int scrollPos = GetScrollPos32(SB_VERT);
				CRect rect;
				GetClientRect(rect);

				int yScroll = (m_nRealRow - idOldTopLeft.row - 1-(m_Range.GetMaxRow ()-m_Range.GetMinRow ()-2))*GetRowHeight(idOldTopLeft.row);
				SetScrollPos32(SB_VERT, scrollPos + yScroll);
				Invalidate();

/*					rect.top = GetFixedRowHeight() + yScroll;
					ScrollWindow( 0, -yScroll, rect);
					rect.top = rect.bottom - yScroll;
*/
				next.row = m_nRealRow - 1;
                break;
				}

//            SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
//            next.row = GetRowCount() - 1;
            break;
                
        default:
            CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
    }
  
    if (next != m_idCurrentCell) 
    {
        // While moving with the Cursorkeys the current ROW/CELL will get selected
        // OR Selection will get expanded when SHIFT is pressed
        // Cut n paste from OnLButtonDown - Franco Bez 
        // Added check for NULL mouse mode - Chris Maunder.
        if (m_MouseMode == MOUSE_NOTHING)
        {
//            m_PrevSelectedCellMap.RemoveAll();
            m_MouseMode = m_bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
//            if (!IsSHIFTpressed() || nChar == VK_TAB)
/////////////////////////////////

/*			if(m_PrevSelectedCellMap.GetCount()!=GetRowCount() -1)
			{
				m_PrevSelectedCellMap.RemoveAll();
				for (int i=1;i<GetRowCount();i++)
				{
					DWORD key;
					CCellID cell(i,m_SortColumn );
					key=MAKELONG(i, m_SortColumn );
					m_PrevSelectedCellMap.SetAt(key, cell);
				}
			}
*/
/////////////////////////////////

             
            if (!Is_Shift || nChar == VK_TAB)
                m_SelectionStartCell = next;

			
            m_CurrrSortClick=FALSE;
            OnSelecting(next);
            m_MouseMode = MOUSE_NOTHING;
        }

        SetFocusCell(next);

        if (!IsCellVisible(next))
        {   
            EnsureVisible(next); // Make sure cell is visible

            switch (nChar) {
                case VK_RIGHT:  
                    SendMessage(WM_HSCROLL, SB_LINERIGHT, 0); 
                    break;

                case VK_LEFT:   
                    SendMessage(WM_HSCROLL, SB_LINELEFT, 0);  
                    break;

                case VK_DOWN:   
                    SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);  
                    break;
                
                case VK_UP:     
                    SendMessage(WM_VSCROLL, SB_LINEUP, 0);    
                    break;                
                
                case VK_TAB:    
                    if (IsSHIFTpressed())
                    {
                        if (bChangeLine) 
                        {
                            SendMessage(WM_VSCROLL, SB_LINEUP, 0);
                            SetScrollPos32(SB_HORZ, m_nHScrollMax);
                            break;
                        }
                        else 
                            SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
                    }
                    else
                    {
                        if (bChangeLine) 
                        {
                            SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
                            SetScrollPos32(SB_HORZ, 0);
                            break;
                        }
                        else 
                            SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
                    }
                    break;

            }
            Invalidate();
        }
		
    }
    m_CurrSortCell=m_idCurrentCell ;

}

// Instant editing of cells when keys are pressed
void CGridCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if (!IsCTRLpressed() && m_MouseMode == MOUSE_NOTHING)
    {
        if (!m_bHandleTabKey || (m_bHandleTabKey && nChar != VK_TAB))
            OnEditCell(m_idCurrentCell.row, m_idCurrentCell.col, nChar);
    }

    CWnd::OnChar(nChar, nRepCnt, nFlags);
}

// Callback from any CInPlaceEdits that ended. This just calls OnEndEditCell,
// refreshes the edited cell and moves onto next cell if the return character
// from the edit says we should.
void CGridCtrl::OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{    
    GV_DISPINFO *pgvDispInfo = (GV_DISPINFO *)pNMHDR;
    GV_ITEM     *pgvItem = &pgvDispInfo->item;

    // In case OnEndInPlaceEdit called as window is being destroyed
    if (!IsWindow(GetSafeHwnd()))
        return;

    // Only set as modified if (a) it actually was, and (b) ESC was not hit.
    if (pgvItem->lParam != VK_ESCAPE &&
        GetItemText(pgvItem->row, pgvItem->col) != pgvItem->szText)
    {
        SetModified(TRUE);
    }

    OnEndEditCell(pgvItem->row, pgvItem->col, pgvItem->szText);
    InvalidateCellRect(CCellID(pgvItem->row, pgvItem->col));

    SendMessageToParent(pgvItem->row, pgvItem->col, GVN_ENDLABELEDIT);

    switch (pgvItem->lParam) 
    {
        case VK_DOWN: 
        case VK_UP:   
        case VK_RIGHT:
        case VK_LEFT:  
        case VK_NEXT:  
        case VK_PRIOR: 
        case VK_HOME:  
        case VK_END:    OnKeyDown(pgvItem->lParam, 0, 0);
                        OnEditCell(m_idCurrentCell.row, m_idCurrentCell.col, pgvItem->lParam);
    }

    *pResult = 0;
}

// Handle horz scrollbar notifications
void CGridCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* /*pScrollBar*/)
{
    if (GetFocus()->GetSafeHwnd() != GetSafeHwnd()) 
        SetFocus();  // Auto-destroy any InPlaceEdit's

#ifdef GRIDCONTROL_USE_TITLETIPS
    m_TitleTip.Hide();  // hide any titletips
#endif

//    int scrollPos = GetScrollPos32(SB_HORZ);
	//Marenan -
	int scrollPos = m_scrollPos;
    CCellID idTopLeft = GetTopleftNonFixedCell();
	//Marenan -

	//Marenan +
	//int scrollPos;
	//if (m_bInnerHScroll)      // 是否用内部滚动条
	//	scrollPos=GetScrollPos32(SB_HORZ);
	//else
	//	scrollPos = m_scrollHPos;
	//CCellID idTopLeft = GetTopleftNonFixedCell();

	CRect rect;
	GetClientRect(rect);

	//int nOldPos;
	//int nHMin,nHMax;
	//if (!m_bInnerHScroll)
	//{
	//	nOldPos=m_pHScroll->GetScrollPos();  
	//	m_pHScroll->GetScrollRange(&nHMin,&nHMax); 
	//}
	//Marenan +

    switch (nSBCode)
    {
        case SB_LINERIGHT:
            if (scrollPos < m_nHScrollMax)
            {
                int xScroll = GetColumnWidth(idTopLeft.col);
                //SetScrollPos32(SB_HORZ, scrollPos + xScroll);
//                if (GetScrollPos32(SB_HORZ) == scrollPos) break;          // didn't work
				m_scrollPos = m_scrollPos + xScroll;             
				//m_scrollHPos = m_scrollHPos + xScroll;             
                rect.left = GetFixedColumnWidth() + xScroll;
                ScrollWindow(-xScroll, 0, rect);
                rect.left = rect.right - xScroll;
                InvalidateRect(rect);
            }
            break;

        case SB_LINELEFT:
            if (scrollPos > 0 && idTopLeft.col > GetFixedColumnCount())
            {
                int xScroll = GetColumnWidth(idTopLeft.col-1);
                //SetScrollPos32(SB_HORZ, max(0,scrollPos - xScroll));
                m_scrollPos = scrollPos - xScroll;
				//m_scrollHPos = scrollPos - xScroll;
				rect.left = GetFixedColumnWidth();
                ScrollWindow(xScroll, 0, rect);
                rect.right = rect.left + xScroll;
                InvalidateRect(rect);
            }
            break;

        case SB_PAGERIGHT:
            if (scrollPos < m_nHScrollMax)
            {
                rect.left = GetFixedColumnWidth();
                int offset = rect.Width();
                int pos = min(m_nHScrollMax, scrollPos + offset);
                m_scrollPos = pos;
				//m_scrollHPos = min(m_nHScrollMax, scrollPos + offset);
                //SetScrollPos32(SB_HORZ, m_scrollHPos);
				rect.left = GetFixedColumnWidth();
                InvalidateRect(rect);
            }
            break;

        case SB_PAGELEFT:
            if (scrollPos > 0)
            {
                rect.left = GetFixedColumnWidth();
                int offset = -rect.Width();
                int pos = max(0, scrollPos + offset);
                //SetScrollPos32(SB_HORZ, pos);
                m_scrollPos = pos;
				rect.left = GetFixedColumnWidth();
                InvalidateRect(rect);
				//rect.left = GetFixedColumnWidth();
				//int offset = -rect.Width();
				////m_scrollHPos = max(0, scrollPos + offset);
				////SetScrollPos32(SB_HORZ, m_scrollHPos);
				//rect.left = GetFixedColumnWidth();
				//InvalidateRect(rect);

            }
            break;

        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            {
				//if (m_bInnerHScroll)      // 是否用内部垂直滚动条
				//	m_scrollHPos=nPos;
				//SetScrollPos32(SB_HORZ,m_scrollHPos);// GetScrollPos32(SB_HORZ, TRUE));
               //SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
                rect.left = GetFixedColumnWidth();
                InvalidateRect(rect);
            }
            break;

        case SB_LEFT:
            if (scrollPos > 0)
            {
				//m_scrollHPos=0;
                SetScrollPos32(SB_HORZ, 0);
                Invalidate();
            }
            break;

        case SB_RIGHT:
            if (scrollPos < m_nHScrollMax)
            {
				//m_scrollHPos=m_nHScrollMax;
                SetScrollPos32(SB_HORZ, m_nHScrollMax);
                Invalidate();
            }
            break;


        default: break;
    }
	m_CurrSortCell=m_idCurrentCell ;
	//m_bKeyMessage=FALSE;

}

// Handle vert scrollbar notifications
void CGridCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* /*pScrollBar*/)
{
    if (GetFocus()->GetSafeHwnd() != GetSafeHwnd()) 
        SetFocus();        // Auto-destroy any InPlaceEdit's

#ifdef GRIDCONTROL_USE_TITLETIPS
    m_TitleTip.Hide();  // hide any titletips
#endif
    // Get the scroll position ourselves to ensure we get a 32 bit value
    int scrollPos = GetScrollPos32(SB_VERT);
	//int scrollPos;
	//if (m_bInnerVScroll)      // 是否用内部垂直滚动条
	//	scrollPos=GetScrollPos32(SB_VERT);
	//else 
	//	scrollPos= m_scrollVPos;//



    CCellID idTopLeft = GetTopleftNonFixedCell();
	CCellRange VisibleCells = GetVisibleNonFixedCellRange();
    int m_VisibleRowCount=VisibleCells.GetMaxRow()   - VisibleCells.GetMinRow ()  + 1; 

    CRect rect;
    GetClientRect(rect);

    switch (nSBCode)
    {
        case SB_LINEDOWN:
			if(idTopLeft.row + m_VisibleRowCount > m_nRows-1)
				break;
            if (scrollPos < m_nVScrollMax)
            {
                int yScroll = GetRowHeight(idTopLeft.row);
                SetScrollPos32(SB_VERT, scrollPos + yScroll);
                if (GetScrollPos32(SB_VERT) == scrollPos) break;          // didn't work
				//m_scrollVPos = m_scrollVPos + yScroll;             

                rect.top = GetFixedRowHeight() + yScroll;
                ScrollWindow( 0, -yScroll, rect);
                rect.top = rect.bottom - yScroll;
                InvalidateRect(rect);
            }
            break;

        case SB_LINEUP:
            if (scrollPos > 0 && idTopLeft.row > GetFixedRowCount())
            {
                int yScroll = GetRowHeight(idTopLeft.row-1);
                SetScrollPos32(SB_VERT, max(0, scrollPos - yScroll));
				//m_scrollVPos = max(0,m_scrollVPos - yScroll);             
                rect.top = GetFixedRowHeight();
                ScrollWindow(0, yScroll, rect);
                rect.bottom = rect.top + yScroll;
                InvalidateRect(rect);
            }
            break;

        case SB_PAGEDOWN:
            if (scrollPos < m_nVScrollMax)
            {
                rect.top = GetFixedRowHeight();
                scrollPos = min(m_nVScrollMax, scrollPos + rect.Height());
				//m_scrollVPos = scrollPos;             
                SetScrollPos32(SB_VERT, scrollPos);
                rect.top = GetFixedRowHeight();
                InvalidateRect(rect);
            }
            break;

        case SB_PAGEUP:
            if (scrollPos > 0)
            {
				//Marenan -
                rect.top = GetFixedRowHeight();
                int offset = -rect.Height();
                int pos = max(0, scrollPos + offset);
                SetScrollPos32(SB_VERT, pos);
                rect.top = GetFixedRowHeight();
                InvalidateRect(rect);
				//Marenan -

				//Marenan +
				//rect.top = GetFixedRowHeight();
				//scrollPos = max(0, scrollPos - rect.Height());
				//m_scrollVPos = scrollPos;             
				//SetScrollPos32(SB_VERT, scrollPos);
				//rect.top = GetFixedRowHeight();
				//InvalidateRect(rect);
				//Marenan +
           }
            break;

        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            {
				//if (m_bInnerVScroll)      // 是否用内部垂直滚动条
				//	m_scrollVPos=nPos;
				//SetScrollPos32(SB_VERT, m_scrollVPos);
                SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
                rect.top = GetFixedRowHeight();
                InvalidateRect(rect);
            }
            break;

        case SB_TOP:
            if (scrollPos > 0)
            {
				//m_scrollVPos=0;
                SetScrollPos32(SB_VERT, 0);
                Invalidate();
            }
            break;

        case SB_BOTTOM:
            if (scrollPos < m_nVScrollMax)
            {
				//m_scrollVPos=m_nVScrollMax;
                SetScrollPos32(SB_VERT, m_nVScrollMax);
                Invalidate();
            }

        default: break;
    }

	//if (m_pVScroll)//m_bKeyMessage&&
	//{
	//	int nMax,nMin;
	//	m_pVScroll->GetScrollRange(&nMin,&nMax); 
	//	m_pVScroll->SetScrollPos(nMin+m_scrollVPos*nMax/m_nVScrollMax);
	//}
	//m_bKeyMessage=FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl implementation functions

void CGridCtrl::OnDraw(CDC* pDC)
{
    CRect rect;
    int row,col;

    CRect clipRect;
    if (pDC->GetClipBox(&clipRect) == ERROR) return;

    EraseBkgnd(pDC);            // OnEraseBkgnd does nothing, so erase bkgnd here.
                                // This necessary since we may be using a Memory DC.

    int nFixedRowHeight = GetFixedRowHeight();
    int nFixedColWidth  = GetFixedColumnWidth();

    CCellID idTopLeft = GetTopleftNonFixedCell();
    int minVisibleRow = idTopLeft.row,
        minVisibleCol = idTopLeft.col;

    CRect VisRect;
    CCellRange VisCellRange = GetVisibleNonFixedCellRange(VisRect);
    int maxVisibleRow = VisCellRange.GetMaxRow(),
        maxVisibleCol = VisCellRange.GetMaxCol();

    // draw top-left cells 0..m_nFixedRows-1, 0..m_nFixedCols-1
    rect.bottom = -1;
    for (row = 0; row < m_nFixedRows; row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;
        rect.right = -1;
        for (col = 0; col < m_nFixedCols; col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;
            DrawFixedCell(pDC, row, col, rect);
        }
    }
    // draw fixed column cells:  m_nFixedRows..n, 0..m_nFixedCols-1
    rect.bottom = nFixedRowHeight - 1;
    for (row = minVisibleRow; row <= maxVisibleRow; row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom) break;                // Gone past cliprect
        if (rect.bottom < clipRect.top) continue;             // Reached cliprect yet?

        rect.right = -1;
        for (col = 0; col < m_nFixedCols; col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            if (rect.left > clipRect.right) break;            // gone past cliprect
            if (rect.right < clipRect.left) continue;         // Reached cliprect yet?
            DrawFixedCell(pDC, row, col, rect);

        }
    }
 
    // draw fixed row cells  0..m_nFixedRows, m_nFixedCols..n
    rect.bottom = -1;
    for (row = 0; row < m_nFixedRows; row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom) break;                // Gone past cliprect
        if (rect.bottom < clipRect.top) continue;             // Reached cliprect yet?

        rect.right = nFixedColWidth-1;
        for (col = minVisibleCol; col <= maxVisibleCol; col++)
        {                                       
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            if (rect.left > clipRect.right) break;        // gone past cliprect
            if (rect.right < clipRect.left) continue;     // Reached cliprect yet?

            DrawFixedCell(pDC, row, col, rect);
//		if(row==1&&col==0)
//			TRACE( "Xx" );
        }
    }
    // draw rest of non-fixed cells
    rect.bottom = nFixedRowHeight - 1;
    for (row = minVisibleRow; row <= maxVisibleRow; row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom) break;                // Gone past cliprect
        if (rect.bottom < clipRect.top) continue;             // Reached cliprect yet?

        rect.right = nFixedColWidth-1;
        for (col = minVisibleCol; col <= maxVisibleCol; col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            if (rect.left > clipRect.right) break;        // gone past cliprect
            if (rect.right < clipRect.left) continue;     // Reached cliprect yet?

            DrawCell(pDC, row, col, rect);
        }
    }
	DrawGridLine(pDC,m_GridLineColor,nFixedColWidth,minVisibleRow,
							maxVisibleRow,nFixedRowHeight,VisRect,minVisibleCol,maxVisibleCol);

#ifdef USE_MEMDC                        // Use a memDC for flicker free update
}
#else                                   // Use normal DC - this helps in debugging
}
#endif

////////////////////////////////////////////////////////////////////////////////////////
// CGridCtrl Cell selection stuff

BOOL CGridCtrl::IsValid(int nRow, int nCol) const
{
    return (nRow >= 0 && nRow < m_nRows && nCol >= 0 && nCol < m_nCols);
}

BOOL CGridCtrl::IsValid(const CCellID& cell) const
{
    return IsValid(cell.row, cell.col);
}

BOOL CGridCtrl::IsValid(const CCellRange& range) const
{
     return (range.GetMinRow() >= 0 && range.GetMinCol() >= 0 && 
            range.GetMaxRow() >= 0 && range.GetMaxCol() >= 0 &&
            range.GetMaxRow() < m_nRows && range.GetMaxCol() < m_nCols &&
             range.GetMinRow() <= range.GetMaxRow() && range.GetMinCol() <= range.GetMaxCol());
}

// Enables/Disables redraw for certain operations like columns auto-sizing etc,
// but not for user caused things such as selection changes.
void CGridCtrl::SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars /* = FALSE */)
{
    TRACE(_T("%s: Setting redraw to %s\n"), 
             GetRuntimeClass()->m_lpszClassName, bAllowDraw? _T("TRUE") : _T("FALSE"));

    if (bAllowDraw && !m_bAllowDraw) Invalidate();
    m_bAllowDraw = bAllowDraw;
    if (bResetScrollBars) ResetScrollBars();
}

// Forces a redraw of a cell immediately (using a direct DC construction, 
// or the supplied dc)
BOOL CGridCtrl::RedrawCell(const CCellID& cell, CDC* pDC /* = NULL */)
{
    return RedrawCell(cell.row, cell.col, pDC);
}

BOOL CGridCtrl::RedrawCell(int nRow, int nCol, CDC* pDC /* = NULL */)
{
    BOOL bResult = TRUE;
    BOOL bMustReleaseDC = FALSE;

    if (!m_bAllowDraw || !IsCellVisible(nRow, nCol)) return FALSE;

    CRect rect;
    if (!GetCellRect(nRow, nCol, rect)) return FALSE;

    if (!pDC) {
        pDC = GetDC();
        if (pDC) bMustReleaseDC = TRUE;
    }

    if (pDC)
    {
        // Redraw cells directly
        if (nRow < m_nFixedRows || nCol < m_nFixedCols)
            bResult = DrawFixedCell(pDC, nRow, nCol, rect, TRUE);
        else
            bResult = DrawCell(pDC, nRow, nCol, rect, TRUE);

        // Since we have erased the background, we will need to redraw the gridlines
        CPen pen;
        try {
              if (nRow < m_nFixedRows || nCol < m_nFixedCols)
                  pen.CreatePen(PS_SOLID, 0, m_crGridColour);
              else
                  pen.CreatePen(PS_SOLID, 0, m_GridLineColor);
        } catch (...) {}
        
        CPen* pOldPen = (CPen*) pDC->SelectObject(&pen);
        if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ) 
        {
            pDC->MoveTo(rect.left,    rect.bottom);
            pDC->LineTo(rect.right+1, rect.bottom);
        }
        if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT) 
        {
            pDC->MoveTo(rect.right, rect.top);
            pDC->LineTo(rect.right, rect.bottom+1);    
        }
        pDC->SelectObject(pOldPen);

    } else
        InvalidateRect(rect, TRUE);     // Could not get a DC - invalidate it anyway
                                        // and hope that OnPaint manages to get one

    if (bMustReleaseDC) 
        ReleaseDC(pDC);

    return bResult;
}

// redraw a complete row
BOOL CGridCtrl::RedrawRow(int row)
{
    BOOL bResult = TRUE;

    CDC* pDC = GetDC();
    for (int col = 0; col < GetColumnCount(); col++)
        bResult = RedrawCell(row, col, pDC) && bResult;
    if (pDC) ReleaseDC(pDC);

    return bResult;
}

// redraw a complete column
BOOL CGridCtrl::RedrawColumn(int col)
{
    BOOL bResult = TRUE;

    CDC* pDC = GetDC();
    for (int row = 0; row < GetRowCount(); row++)
        bResult = RedrawCell(row, col, pDC) && bResult;
    if (pDC) ReleaseDC(pDC);

    return bResult;
}


// Sets the currently selected cell, returning the previous current cell
CCellID CGridCtrl::SetFocusCell(int nRow, int nCol)
{
    return SetFocusCell(CCellID(nRow, nCol));
}

CCellID CGridCtrl::SetFocusCell(CCellID cell)
{
    if (cell == m_idCurrentCell) 
        return m_idCurrentCell;

    CCellID idPrev = m_idCurrentCell;
    m_idCurrentCell = cell;

    if (IsValid(idPrev)) 
    {
        SendMessageToParent(idPrev.row, idPrev.col, GVN_SELCHANGING); 

        SetItemState(idPrev.row, idPrev.col, 
                     GetItemState(idPrev.row, idPrev.col) & ~GVIS_FOCUSED);
    //    RedrawCell(idPrev);

/*        if (idPrev.col != m_idCurrentCell.col)
            for (int row = 0; row < m_nFixedRows; row++)
                RedrawCell(row, idPrev.col);
        if (idPrev.row != m_idCurrentCell.row)
            for (int col = 0; col < m_nFixedCols; col++) 
		          RedrawCell(idPrev.row, col);
*/    }

    if (IsValid(m_idCurrentCell)) {
        ///////////Protect The Cell Show the other color///
		//////////  modify by zhang//////////////////
		//SetItemState(m_idCurrentCell.row, m_idCurrentCell.col, 
          //           GetItemState(m_idCurrentCell.row, m_idCurrentCell.col) | GVIS_FOCUSED);

       // RedrawCell(m_idCurrentCell);

/*        if (idPrev.col != m_idCurrentCell.col)
            for (int row = 0; row < m_nFixedRows; row++) 
                RedrawCell(row, m_idCurrentCell.col);
        if (idPrev.row != m_idCurrentCell.row)
            for (int col = 0; col < m_nFixedCols; col++) 
                RedrawCell(m_idCurrentCell.row, col);
	//		{} 
*/	   SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_SELCHANGED); 
    }

    return idPrev;
}

// Sets the range of currently selected cells
void CGridCtrl::SetSelectedRange(const CCellRange& Range, 
                                 BOOL bForceRepaint /* = FALSE */)
{
    SetSelectedRange(Range.GetMinRow(), Range.GetMinCol(), Range.GetMaxRow(), Range.GetMaxCol(),
                     bForceRepaint);
}

void CGridCtrl::SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol,
                                 BOOL bForceRepaint /* = FALSE */)
{
    if (!m_bEnableSelection) return;
    CDC* pDC = NULL;
    if (bForceRepaint) pDC = GetDC();
	if(m_BeforeRow!=nMinRow)
	{
		int i;
        for(i=nMinCol;i<=nMaxCol;i++)
		{
            CCellID cell(m_BeforeRow, i);
            if (bForceRepaint && pDC)                    // Redraw NOW
                RedrawCell(cell.row, cell.col, pDC);
            else
                InvalidateCellRect(cell);                // Redraw at leisure
		}
        for(i=nMinCol;i<=nMaxCol;i++)
		{
            CCellID cell(nMinRow, i);
            if (bForceRepaint && pDC)                    // Redraw NOW
                RedrawCell(cell.row, cell.col, pDC);
            else
                InvalidateCellRect(cell);                // Redraw at leisure
		}
	}
    if (pDC != NULL) ReleaseDC(pDC);
} 

// selects all cells
void CGridCtrl::SelectAllCells()
{
    if (!m_bEnableSelection) return;

    SetSelectedRange(m_nFixedRows, m_nFixedCols, GetRowCount()-1, GetColumnCount()-1);
}

// selects columns
void CGridCtrl::SelectColumns(CCellID currentCell)
{
    if (!m_bEnableSelection) return;

    //if (currentCell.col == m_idCurrentCell.col) return;
    if (currentCell.col < m_nFixedCols) return;
    if (!IsValid(currentCell)) return;

    SetSelectedRange(GetFixedRowCount(), min(m_SelectionStartCell.col, currentCell.col), 
                     GetRowCount()-1,    max(m_SelectionStartCell.col, currentCell.col)); 
}

// selects rows
void CGridCtrl::SelectRows(CCellID currentCell)
{  
    if (!m_bEnableSelection) return;

//    if (currentCell.row == m_idCurrentCell.row) return;
    if (currentCell.row < m_nFixedRows) return;
    if (!IsValid(currentCell)) return;

//    SetSelectedRange(min(m_SelectionStartCell.row, currentCell.row), GetFixedColumnCount(), 
//                     max(m_SelectionStartCell.row, currentCell.row), GetColumnCount()-1); 

    SetSelectedRange(min(m_SelectionStartCell.row, currentCell.row), 0, 
                     max(m_SelectionStartCell.row, currentCell.row), GetColumnCount()-1); 
}

// selects cells
void CGridCtrl::SelectCells(CCellID currentCell)
{
    if (!m_bEnableSelection) return;

    int row = currentCell.row;
    int col = currentCell.col;
    if (row < m_nFixedRows || col < m_nFixedCols) return;
    if (!IsValid(currentCell)) return;

    // Prevent unnecessary redraws
    //if (currentCell == m_LeftClickDownCell)  return;
    //else if (currentCell == m_idCurrentCell) return;

    SetSelectedRange(min(m_SelectionStartCell.row, row), min(m_SelectionStartCell.col, col), 
                     max(m_SelectionStartCell.row, row), max(m_SelectionStartCell.col, col)); 
}

void CGridCtrl::OnSelecting(const CCellID& currentCell)
{
    if (!m_bEnableSelection) return;

    switch(m_MouseMode)
    {
        case MOUSE_SELECT_ALL:     SelectAllCells();               break;
        case MOUSE_SELECT_COL:     SelectColumns(currentCell);     break;
        case MOUSE_SELECT_ROW:     SelectRows(currentCell);        break;
        case MOUSE_SELECT_CELLS:   SelectCells(currentCell);       break;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// Clipboard functions

void CGridCtrl::CutSelectedText()
{
    if (!IsEditable())
        return;

    // Clear contents of selected cells.
    Invalidate();
}

COleDataSource* CGridCtrl::CopyTextFromGrid()
{
    USES_CONVERSION;

    CCellRange Selection = GetSelectedCellRange();
    if (!IsValid(Selection)) return NULL;

    // Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE, so we need to convert)
    CSharedFile sf(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

    // Get a tab delimited string to copy to cache
    CString str;
    CGridCell *pCell;
	pCell=new CGridCell;
    for (int row = Selection.GetMinRow(); row <= Selection.GetMaxRow(); row++) 
    {
        str.Empty();
        for (int col = Selection.GetMinCol(); col <= Selection.GetMaxCol(); col++)
        {
            GetCell(pCell,row,col);
            if (pCell && (pCell->state  & GVIS_SELECTED))
            {
                if (pCell->szText.IsEmpty())
                    str += _T(" ");
                else 
                   str += pCell->szText;
            }
            if (col != Selection.GetMaxCol()) 
                str += _T("\t");
        }
        if (row != Selection.GetMaxRow()) 
            str += _T("\n");

        sf.Write(T2A(str.GetBuffer(1)), str.GetLength());
        str.ReleaseBuffer();
    }
	delete pCell;
    char c = '\0';
    sf.Write(&c, 1);

    DWORD dwLen = sf.GetLength();
    HGLOBAL hMem = sf.Detach();
    if (!hMem) 
        return NULL;

    hMem = ::GlobalReAlloc(hMem, dwLen, GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
    if (!hMem) 
        return NULL;

    // Cache data
    COleDataSource* pSource = new COleDataSource();
    pSource->CacheGlobalData(CF_TEXT, hMem);

    return pSource;
}

BOOL CGridCtrl::PasteTextToGrid(CCellID cell, COleDataObject* pDataObject)
{
    if (!IsValid(cell) || !IsCellEditable(cell) || !pDataObject->IsDataAvailable(CF_TEXT))
        return FALSE;

    // Get the text from the COleDataObject
    HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
    CMemFile sf((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));

    // CF_TEXT is ANSI text, so we need to allocate a char* buffer
    // to hold this.
    LPSTR szBuffer = new char[::GlobalSize(hmem)];
    if (!szBuffer)
        return FALSE;

    sf.Read(szBuffer, ::GlobalSize(hmem));
    ::GlobalUnlock(hmem);

    // Now store in generic TCHAR form so we no longer have to deal with
    // ANSI/UNICODE problems
    CString strText = szBuffer;
    delete szBuffer;

    // Parse text data and set in cells...
    strText.LockBuffer();
    CString strLine = strText;
    int nLine = 0;

    // Find the end of the first line
    int nIndex;
    do {
        int nColumn = 0;
        nIndex = strLine.Find(_T("\n"));

        // Store the remaining chars after the newline
        CString strNext = (nIndex < 0)? _T("")  : strLine.Mid(nIndex+1);

        // Remove all chars after the newline
        if (nIndex >= 0) 
            strLine = strLine.Left(nIndex);

        // Make blank entries a "space"
        if (strLine.IsEmpty() && nIndex >= 0)
            strLine = _T(" ");

        LPTSTR szLine = strLine.GetBuffer(1);

        // Break the current line into tokens (tab or comma delimited)
        LPTSTR pszCellText = _tcstok(szLine, _T("\t,\n"));
        while (pszCellText != NULL)
        {
            CCellID TargetCell(cell.row + nLine, cell.col + nColumn);
            if (IsValid(TargetCell))
            {
                CString strCellText = pszCellText;
                strCellText.TrimLeft();  strCellText.TrimRight();

                SetItemText(TargetCell.row, TargetCell.col, strCellText);
                SetModified(TRUE);

                // Make sure cell is not selected to avoid data loss
                SetItemState(TargetCell.row, TargetCell.col,
                             GetItemState(TargetCell.row, TargetCell.col) & ~GVIS_SELECTED);
            }

            pszCellText = _tcstok(NULL, _T("\t,\n"));
            nColumn++;
        }

        strLine.ReleaseBuffer();
        strLine = strNext;
        nLine++;

    } while (nIndex >= 0);

    strText.UnlockBuffer();
    Invalidate();

    return TRUE;
}

void CGridCtrl::OnBeginDrag()
{        
    if (!m_bAllowDragAndDrop) return;

    COleDataSource* pSource = CopyTextFromGrid();
    if (pSource) 
    {    
        SendMessageToParent(GetSelectedCellRange().GetTopLeft().row,
                            GetSelectedCellRange().GetTopLeft().col,
                            GVN_BEGINDRAG);

        m_MouseMode = MOUSE_DRAGGING;
        DROPEFFECT dropEffect = pSource->DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE);

        if (dropEffect & DROPEFFECT_MOVE)
            CutSelectedText();

        if (pSource) 
            delete pSource;    // Did not pass source to clipboard, so must delete
    }    
}

DROPEFFECT CGridCtrl::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, 
                                 CPoint point)
{
    // Any text data available for us?
    if (!m_bAllowDragAndDrop || !IsEditable() || !pDataObject->IsDataAvailable(CF_TEXT))
        return DROPEFFECT_NONE;

    // Find which cell we are over and drop-highlight it
    CCellID cell = GetCellFromPt(point, FALSE);

    // If not valid, set the previously drop-highlighted cell as no longer drop-highlighted
    if (!IsValid(cell))
    {
        OnDragLeave();
        m_LastDragOverCell = CCellID(-1,-1);
        return DROPEFFECT_NONE;
    }

    if (!IsCellEditable(cell))
        return DROPEFFECT_NONE;

    // Have we moved over a different cell than last time?
    if (cell != m_LastDragOverCell) 
    {
        // Set the previously drop-highlighted cell as no longer drop-highlighted
        if (IsValid(m_LastDragOverCell)) {
            UINT nState = GetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col);
            SetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col,
                         nState & ~GVIS_DROPHILITED);
            RedrawCell(m_LastDragOverCell);
        }

        m_LastDragOverCell = cell;

        // Set the new cell as drop-highlighted
        if (IsValid(m_LastDragOverCell)) {
            UINT nState = GetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col);
            SetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col,
                         nState | GVIS_DROPHILITED);
            RedrawCell(m_LastDragOverCell);
        }
    }

    // Return an appropraite value of DROPEFFECT so mouse cursor is set properly
    if (dwKeyState & MK_CONTROL)
        return DROPEFFECT_COPY;
    else
        return DROPEFFECT_MOVE;
}

DROPEFFECT CGridCtrl::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, 
                                  CPoint point)
{
    // Any text data available for us?
    if (!m_bAllowDragAndDrop || !pDataObject->IsDataAvailable(CF_TEXT))
        return DROPEFFECT_NONE;

    // Find which cell we are over and drop-highlight it
    m_LastDragOverCell = GetCellFromPt(point, FALSE);
    if (!IsValid(m_LastDragOverCell))
        return DROPEFFECT_NONE;

    if (!IsCellEditable(m_LastDragOverCell))
        return DROPEFFECT_NONE;

    if (IsValid(m_LastDragOverCell)) 
    {
        UINT nState = GetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col);
        SetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col,
                     nState | GVIS_DROPHILITED);
        RedrawCell(m_LastDragOverCell);
    }

    // Return an appropraite value of DROPEFFECT so mouse cursor is set properly
    if (dwKeyState & MK_CONTROL)
        return DROPEFFECT_COPY;
    else
        return DROPEFFECT_MOVE;
}

void CGridCtrl::OnDragLeave()
{
    // Set the previously drop-highlighted cell as no longer drop-highlighted
    if (IsValid(m_LastDragOverCell)) {
        UINT nState = GetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col);
        SetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col,
                     nState & ~GVIS_DROPHILITED);
        RedrawCell(m_LastDragOverCell);
    }
}

BOOL CGridCtrl::OnDrop(COleDataObject* pDataObject, DROPEFFECT /*dropEffect*/, 
                       CPoint /* point */) 
{
    BOOL bResult = FALSE;

    if (!m_bAllowDragAndDrop || !IsCellEditable(m_LastDragOverCell))
        return bResult;

    m_MouseMode = MOUSE_NOTHING;
    OnDragLeave();

    return PasteTextToGrid(m_LastDragOverCell, pDataObject);
}

void CGridCtrl::OnEditCut()
{
    if (!IsEditable())
        return;

    COleDataSource* pSource = CopyTextFromGrid();
    if (!pSource) return;

    pSource->SetClipboard();
    CutSelectedText();
}

void CGridCtrl::OnEditCopy()
{
    COleDataSource* pSource = CopyTextFromGrid();
    if (!pSource) return;

    pSource->SetClipboard();
}

void CGridCtrl::OnEditPaste()
{
    if (!IsEditable())
        return;

    // Get the Focus cell, or if none, get the topleft (non-fixed) cell
    CCellID cell = GetFocusCell();  
    if (!IsValid(cell)) cell = GetTopleftNonFixedCell();  
    if (!IsValid(cell)) return;

    // Attach a COleDataObject to the clipboard and paste the data to the grid
    COleDataObject obj;
    if (obj.AttachClipboard()) 
        PasteTextToGrid(cell, &obj);
}

void CGridCtrl::OnEditSelectAll() 
{
    SelectAllCells();
}

void CGridCtrl::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
    CCellRange Selection = GetSelectedCellRange();
    pCmdUI->Enable(Selection.Count() && IsValid(Selection));
}

void CGridCtrl::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
    CCellRange Selection = GetSelectedCellRange();
    pCmdUI->Enable(IsEditable() && Selection.Count() && IsValid(Selection));
}

void CGridCtrl::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
    CCellID cell = GetFocusCell();

    BOOL bCanPaste = IsValid(cell) && IsCellEditable(cell) &&
                     ::IsClipboardFormatAvailable(CF_TEXT);

    pCmdUI->Enable(bCanPaste);
}

void CGridCtrl::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(m_bEnableSelection);
}

////////////////////////////////////////////////////////////////////////////////////////
// hittest-like functions

// Get cell from point
CCellID CGridCtrl::GetCellFromPt(CPoint point, BOOL bAllowFixedCellCheck /*=TRUE*/) const
{  
    CCellID idTopLeft = GetTopleftNonFixedCell();
    CCellID cellID; // return value

    // calculate column index
    int fixedColWidth = GetFixedColumnWidth();

    if (point.x < 0 || (!bAllowFixedCellCheck && point.x < fixedColWidth)) // not in window
        cellID.col = -1;
    else if (point.x < fixedColWidth) // in fixed col
    {
        int xpos = 0;
		int col;
        for (col = 0; col < m_nFixedCols; col++)
        {
            xpos += GetColumnWidth(col);
            if (xpos > point.x) break;
        }
        cellID.col = col;
    }
    else    // in non-fixed col
    {
        int xpos = fixedColWidth;
		int col;
        for (col = idTopLeft.col; col < GetColumnCount(); col++)
        {
            xpos += GetColumnWidth(col);
            if (xpos > point.x) break;
        }

        if (col >= GetColumnCount())
            cellID.col = -1;
        else
            cellID.col = col;
    }
    
    // calculate row index
    int fixedRowHeight = GetFixedRowHeight();
    if (point.y < 0 || (!bAllowFixedCellCheck && point.y < fixedRowHeight)) // not in window
        cellID.row = -1;
    else if (point.y < fixedRowHeight) // in fixed col
    {
        int ypos = 0;
		int row;
        for (row = 0; row < m_nFixedRows; row++)
        {
            ypos += GetRowHeight(row);
            if (ypos > point.y) break;
        }
        cellID.row = row;
    }
    else
    {
        int ypos = fixedRowHeight;
		int row;
        for (row = idTopLeft.row; row < GetRowCount(); row++)
        {
            ypos += GetRowHeight(row);
            if (ypos > point.y) break;
        }

        if (row >= GetRowCount())
            cellID.row = -1;
        else
            cellID.row = row;
    }

    return cellID;
}

////////////////////////////////////////////////////////////////////////////////
// CGridCtrl cellrange functions

CCellID CGridCtrl::GetTopleftNonFixedCell() const
{
	/////////////moid by zjh/////////////
	//int nVertScroll = GetScrollPos(SB_VERT), 
	//  nHorzScroll = GetScrollPos(SB_HORZ);
	int nVertScroll = GetScrollPos(SB_VERT), 
		nHorzScroll = m_scrollPos;

	int nColumn = m_nFixedCols, nRight = 0;
	while (nRight < nHorzScroll && nColumn < (GetColumnCount()-1))
		nRight += GetColumnWidth(nColumn++);

	int nRow = m_nFixedRows, nTop = 0;
	while (nTop < nVertScroll && nRow < (GetRowCount()-1))
		nTop += GetRowHeight(nRow++);

	//TRACE("TopLeft cell is row %d, col %d\n",nRow, nColumn);
	return CCellID(nRow, nColumn);
}

// This gets even partially visible cells
CCellRange CGridCtrl::GetVisibleNonFixedCellRange(LPRECT pRect /*=NULL*/) const
{
    CRect rect;
    GetClientRect(rect);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    // calc bottom
    int bottom = GetFixedRowHeight();
	int i;
    for (i = idTopLeft.row; i < GetRowCount(); i++)
    {
        bottom += GetRowHeight(i);
        if (bottom >= rect.bottom) {
            bottom = rect.bottom;
            break;
        }
    }                                
    int maxVisibleRow = min(i, GetRowCount() - 1);
    
    // calc right
    int right = GetFixedColumnWidth();
    for (i = idTopLeft.col; i < GetColumnCount(); i++)
    {
        right += GetColumnWidth(i);
        if (right >= rect.right) {
            right = rect.right;
            break;
        }
    }
    int maxVisibleCol = min(i, GetColumnCount() - 1);
    if (pRect) {
        pRect->left = pRect->top = 0;
        pRect->right = right;
        pRect->bottom = bottom;
    }

    return CCellRange(idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol);
}

// used by ResetScrollBars() - This gets only fully visible cells
CCellRange CGridCtrl::GetUnobstructedNonFixedCellRange() const
{
    CRect rect;
    GetClientRect(rect);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    // calc bottom
    int bottom = GetFixedRowHeight();
	int i;
    for (i = idTopLeft.row; i < GetRowCount(); i++)
    {
        bottom += GetRowHeight(i);
        if (bottom >= rect.bottom) break;
    }
    int maxVisibleRow = min(i, GetRowCount() - 1);
    if (maxVisibleRow > 0 && bottom > rect.bottom) maxVisibleRow--;
    
    // calc right
    int right = GetFixedColumnWidth();
    for (i = idTopLeft.col; i < GetColumnCount(); i++)
    {
        right += GetColumnWidth(i);
        if (right >= rect.right) break;
    }
    int maxVisibleCol = min(i, GetColumnCount() - 1);
    if (maxVisibleCol > 0 && right > rect.right) maxVisibleCol--;


    return CCellRange(idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol);
}


// Returns the minimum bounding range of the current selection
// If no selection, then the returned CCellRange will be invalid
CCellRange CGridCtrl::GetSelectedCellRange() const
{
    CCellRange Selection(GetRowCount(), GetColumnCount(), -1,-1);


    return Selection;
}

// Returns ALL the cells in the grid
CCellRange CGridCtrl::GetCellRange() const
{
    return CCellRange(0, 0, GetRowCount() - 1, GetColumnCount() - 1);
}

void CGridCtrl::ResetSelectedRange()
{
//    SetSelectedRange(-1,-1,-1,-1);
    SetFocusCell(-1,-1);
}

// Get/Set scroll position using 32 bit functions
int CGridCtrl::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);

    if (bGetTrackPos)
    {
        if (GetScrollInfo(nBar, &si, SIF_TRACKPOS))
            return si.nTrackPos;
    }
    else 
    {
        if (GetScrollInfo(nBar, &si, SIF_POS))
            return si.nPos;
    }

    return 0;
}

BOOL CGridCtrl::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_POS;
    si.nPos   = nPos;
    return SetScrollInfo(nBar, &si, bRedraw);
}

void CGridCtrl::ResetScrollBars()
{
    if (!m_bAllowDraw || !::IsWindow(GetSafeHwnd())) 
        return;
	CScrollBar* pBar = GetScrollBarCtrl(SB_VERT);
	CRect rc;
	if (pBar)
	{
		pBar->GetWindowRect(rc);
		rc.left = rc.right - 16;
		pBar->MoveWindow(rc);
	}

    CRect rect;
    GetClientRect(rect);
    rect.left  += GetFixedColumnWidth();
    rect.top   += GetFixedRowHeight();
    if (rect.left >= rect.right || rect.top >= rect.bottom) return;

    CRect VisibleRect(GetFixedColumnWidth(), GetFixedRowHeight(), rect.right, rect.bottom);
    CRect VirtualRect(GetFixedColumnWidth(), GetFixedRowHeight(), GetVirtualWidth(), GetVirtualHeight());

    CCellRange visibleCells = GetUnobstructedNonFixedCellRange();
    if (!IsValid(visibleCells)) return;

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_PAGE;
    si.nPage  = VisibleRect.Width();    SetScrollInfo(SB_HORZ, &si, FALSE); 
    si.nPage  = VisibleRect.Height();   SetScrollInfo(SB_VERT, &si, FALSE); 

    if (VisibleRect.Height() < VirtualRect.Height())
        m_nVScrollMax = VirtualRect.Height()-1; //+ GetRowHeight(visibleCells.GetTopLeft().row);
    else
        m_nVScrollMax = 0;

    if (VisibleRect.Width() < VirtualRect.Width())
        m_nHScrollMax = VirtualRect.Width()-1; //+ GetColumnWidth(visibleCells.GetTopLeft().col);
    else
        m_nHScrollMax = 0;

    ASSERT(m_nVScrollMax < INT_MAX && m_nHScrollMax < INT_MAX); // This should be fine :)
//    SetScrollRange(SB_VERT, 0, 500, TRUE );

    SetScrollRange(SB_VERT, 0, m_nVScrollMax, TRUE );
    //SetScrollRange(SB_HORZ, 0, m_nHScrollMax, TRUE);
	
	//if (m_bInnerVScroll)
	//	SetScrollRange(SB_VERT, 0, m_nVScrollMax,TRUE);
	//if (m_bInnerHScroll)
	//	SetScrollRange(SB_HORZ, 0, m_nHScrollMax,TRUE);


}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column position functions

// returns the top left point of the cell. Returns FALSE if cell not visible.
BOOL CGridCtrl::GetCellOrigin(int nRow, int nCol, LPPOINT p) const
{
    int i;

    if (!IsValid(nRow, nCol)) return FALSE;

    CCellID idTopLeft;
    if (nCol >= m_nFixedCols || nRow >= m_nFixedRows)
        idTopLeft = GetTopleftNonFixedCell();

    if ((nRow >= m_nFixedRows && nRow < idTopLeft.row) ||
        (nCol>= m_nFixedCols && nCol < idTopLeft.col))
        return FALSE;

    p->x = 0;
    if (nCol < m_nFixedCols)                      // is a fixed column
        for (i = 0; i < nCol; i++)
            p->x += GetColumnWidth(i);
    else {                                        // is a scrollable data column
        for (i = 0; i < m_nFixedCols; i++)
            p->x += GetColumnWidth(i);
        for (i = idTopLeft.col; i < nCol; i++)
            p->x += GetColumnWidth(i);
    }

    p->y = 0;
    if (nRow < m_nFixedRows)                      // is a fixed row
        for (i = 0; i < nRow; i++)
            p->y += GetRowHeight(i);
    else {                                        // is a scrollable data row
        for (i = 0; i < m_nFixedRows; i++)
            p->y += GetRowHeight(i);
        for (i = idTopLeft.row; i < nRow; i++)
            p->y += GetRowHeight(i);
    }

    return TRUE;
}

BOOL CGridCtrl::GetCellOrigin(const CCellID& cell, LPPOINT p) const
{
    return GetCellOrigin(cell.row, cell.col, p);
}

// Returns the bounding box of the cell
BOOL CGridCtrl::GetCellRect(const CCellID& cell, LPRECT pRect) const
{
    return GetCellRect(cell.row, cell.col, pRect);
}

BOOL CGridCtrl::GetCellRect(int nRow, int nCol, LPRECT pRect) const
{    
    CPoint CellOrigin;
    if (!GetCellOrigin(nRow, nCol, &CellOrigin)) return FALSE;

    pRect->left   = CellOrigin.x;
    pRect->top    = CellOrigin.y;
    pRect->right  = CellOrigin.x + GetColumnWidth(nCol)-1;
    pRect->bottom = CellOrigin.y + GetRowHeight(nRow)-1;

    //TRACE("Row %d, col %d: L %d, T %d, W %d, H %d:  %d,%d - %d,%d\n",
    //      nRow,nCol, CellOrigin.x, CellOrigin.y, GetColumnWidth(nCol), GetRowHeight(nRow),
    //      pRect->left, pRect->top, pRect->right, pRect->bottom);

    return TRUE;
}

// Returns the bounding box of a range of cells
BOOL CGridCtrl::GetCellRangeRect(const CCellRange& cellRange, LPRECT lpRect) const
{
    CPoint MinOrigin,MaxOrigin;

    if (!GetCellOrigin(cellRange.GetMinRow(), cellRange.GetMinCol(), &MinOrigin)) 
        return FALSE;
    if (!GetCellOrigin(cellRange.GetMaxRow(), cellRange.GetMaxCol(), &MaxOrigin)) 
        return FALSE;

    lpRect->left   = MinOrigin.x;
    lpRect->top    = MinOrigin.y;
    lpRect->right  = MaxOrigin.x + GetColumnWidth(cellRange.GetMaxCol()-1);
    lpRect->bottom = MaxOrigin.y + GetRowHeight(cellRange.GetMaxRow()-1);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// Grid attribute functions

LRESULT CGridCtrl::OnSetFont(WPARAM hFont, LPARAM /*lParam */)
{
    LRESULT result = Default();

    // Get the logical font
    LOGFONT lf;
    if (!GetObject((HFONT) hFont, sizeof(LOGFONT), &lf))
        return result;
    
    // Store font as the global default
    memcpy(&m_Logfont, &lf, sizeof(LOGFONT));
    
    // reset all cells' fonts
    for (int row = 0; row < GetRowCount(); row++)
        for (int col = 0; col < GetColumnCount(); col++)
            SetItemFont(row, col, &lf);

    // Get the font size and hence the default cell size
    CDC* pDC = GetDC();
    if (pDC) 
    {
        m_Font.DeleteObject();
        m_Font.CreateFontIndirect(&m_Logfont);
        CFont* pOldFont = pDC->SelectObject(&m_Font);
        
        TEXTMETRIC tm;
        pDC->GetTextMetrics(&tm);
        
        //m_nMargin = pDC->GetTextExtent(_T(" "),1).cx;
        /////////Add new column width////
		int n_WidMargin = pDC->GetTextExtent(_T(" "),1).cx;
		m_nMargin = 3;
		pDC->SelectObject(pOldFont);
        ReleaseDC(pDC);

        m_nDefCellHeight = tm.tmHeight+tm.tmExternalLeading + 2*m_nMargin;
        //m_nDefCellWidth  = tm.tmAveCharWidth*12 + 2*m_nMargin;
        m_nDefCellWidth  = tm.tmAveCharWidth*8 + 3*m_nMargin;
	}

    if (::IsWindow(GetSafeHwnd())) 
        Invalidate();

    return result;
}

LRESULT CGridCtrl::OnGetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return (LRESULT) (HFONT) m_Font;
}

BOOL CGridCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    if (nHitTest == HTCLIENT)
    {
        switch (m_MouseMode) 
        {
            case MOUSE_OVER_COL_DIVIDE: SetCursor(::LoadCursor(NULL, IDC_SIZEWE)); break;
            case MOUSE_OVER_ROW_DIVIDE: SetCursor(::LoadCursor(NULL, IDC_SIZENS)); break;
            case MOUSE_DRAGGING:        break;
            default:                    SetCursor(::LoadCursor(NULL, IDC_ARROW));
        }
        return TRUE;
    }

    return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column count functions

BOOL CGridCtrl::SetFixedRowCount(int nFixedRows)
{
    ASSERT(nFixedRows >= 0);

    if (nFixedRows > GetRowCount()) 
        if (!SetRowCount(nFixedRows)) return FALSE;

    if (m_idCurrentCell.row < nFixedRows)
        SetFocusCell(-1,-1);

    m_nFixedRows = nFixedRows;

    if (GetSafeHwnd() && m_bAllowDraw)
        Invalidate();

    return TRUE;
}

BOOL CGridCtrl::SetFixedColumnCount(int nFixedCols)
{
    ASSERT(nFixedCols >= 0);
    if (nFixedCols > GetColumnCount())
        if (!SetColumnCount(nFixedCols)) return FALSE;

    if (m_idCurrentCell.col < nFixedCols)
        SetFocusCell(-1,-1);

    m_nFixedCols = nFixedCols;

    if (GetSafeHwnd() && m_bAllowDraw)
        Invalidate();

    return TRUE;
}

BOOL CGridCtrl::SetRowCount(int nRows)
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

BOOL CGridCtrl::SetColumnCount(int nCols)
{
    ASSERT(nCols >= 0);

    if (nCols == GetColumnCount()) return TRUE;

    if (nCols < m_nFixedCols) 
        m_nFixedCols = nCols;

    if (m_idCurrentCell.col >= nCols)
        SetFocusCell(-1,-1);


    int addedCols = nCols - GetColumnCount();
    if(m_pColData==NULL)
	{
		HGLOBAL	hMem = GlobalAlloc( GPTR, (30)* sizeof( GCOL_ITEM) );
		LPVOID hp=GlobalLock(hMem);
		if( hp )
		{
			m_pColData= (GCOL_ITEM *)hp;
		}
		else
			AfxMessageBox("分配内存出错",MB_ICONSTOP);

	}
    // Change the number of columns.
    m_nCols = nCols;
    m_arColWidths.SetSize(nCols);

    // Change the number of columns in each row.
    // If we have just added columns, we need to construct new elements for each cell
    // and set the default column width
    if (addedCols > 0)
    {
        // initialized column widths
        int startCol = nCols - addedCols;
        for (int col = startCol; col < GetColumnCount(); col++)
            m_arColWidths[col] = m_nDefCellWidth;

        // initialise column data
    }
    
    if (GetSafeHwnd() && m_bAllowDraw)
    {
        ResetScrollBars();
        Invalidate();
    }
    return TRUE;
}

// Insert a column at a given position, or add to end of columns (if nColumn = -1)
int CGridCtrl::InsertColumn(LPCTSTR strHeading, 
                            UINT nFormat /* = DT_CENTER|DT_VCENTER|DT_SINGLELINE */,
                            int nColumn  /* = -1 */)
{
    // If the insertion is for a specific column, check it's within range.
    if (nColumn >= 0 && nColumn >= GetColumnCount())
        return -1;

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the column.
    if (m_nRows < 1)
        SetRowCount(1);    
//19991106
    if (nColumn < 0)
    {
        nColumn = m_nCols;
        m_arColWidths.Add(0);
/*      for (int row = 0; row < m_nRows; row++) 
        {
            GRID_ROW* pRow = m_RowData[row];
            if (!pRow) return -1;
            pRow->Add(CreateCell(row, nColumn));
        }
*/    } 
    else
    {
        m_arColWidths.InsertAt(nColumn, (UINT)0);
/*        for (int row = 0; row < m_nRows; row++) 
        {
            GRID_ROW* pRow = m_RowData[row];
            if (!pRow) return -1;
            pRow->InsertAt(nColumn, CreateCell(row, nColumn));
        }
*/  }

    m_nCols++;

    // Initialise column data
    SetItemText(0, nColumn, strHeading);
    for (int row = 0; row < m_nRows; row++) 
    {
        SetItemFormat(row, nColumn, nFormat);
    }

    // initialized column width
    m_arColWidths[nColumn] = GetTextExtent(strHeading).cx;

    if (m_idCurrentCell.col != -1 && nColumn < m_idCurrentCell.col)
        m_idCurrentCell.col++;
    
    ResetScrollBars();

    return nColumn;
}

// Insert a row at a given position, or add to end of rows (if nRow = -1)
int CGridCtrl::InsertRow(LPCTSTR strHeading, int nRow /* = -1 */)
{
    // If the insertion is for a specific row, check it's within range.
    if (nRow >= 0 && nRow >= GetRowCount())
        return -1;

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the row.
    if (m_nCols < 1) 
        SetColumnCount(1);    
//19991106
    // Adding a row to the bottom
    if (nRow < 0) 
    {
        nRow = m_nRows;
        m_arRowHeights.Add(0);
//        m_RowData.Add(new GRID_ROW);
    } 
    else 
    {
        m_arRowHeights.InsertAt(nRow, (UINT)0);
//        m_RowData.InsertAt(nRow, new GRID_ROW);
    }
    
    m_nRows++;
//    m_RowData[nRow]->SetSize(m_nCols);

    // Initialise cell data
/*    for (int col = 0; col < m_nCols; col++)
    {
        GRID_ROW* pRow = m_RowData[nRow];
        if (!pRow) return -1;
        pRow->SetAt(col, CreateCell(nRow, col));
    }
*/
    // Set row title
    SetItemText(nRow, 0, strHeading);

    // initialized row height
    m_arRowHeights[nRow] = GetTextExtent(strHeading).cy;

    if (m_idCurrentCell.row != -1 && nRow < m_idCurrentCell.row)
        m_idCurrentCell.row++;
    
    ResetScrollBars();

    return nRow;
}

// Creates a new grid cell and performs any necessary initialisation
CGridCell* CGridCtrl::CreateCell(int nRow, int nCol)
{
    CGridCell* pCell = new CGridCell;
    if (!pCell)
        return NULL;
    
    // Make format same as cell above
    if (nRow > 0 && nCol > 0 && nCol < m_nCols)
        pCell->nFormat = GetItemFormat(nRow-1, nCol);
 
    // Make font default grid font
    memcpy(&(pCell->lfFont), &m_Logfont, sizeof(LOGFONT));
    
    return pCell;
}

// Performs any cell cleanup necessary to maintain grid integrity
void CGridCtrl::EmptyCell(CGridCell* pCell, int nRow, int nCol)
{
    // Set the cells state to 0. If the cell is selected, this
    // will remove the cell from the selected list.
    SetItemState(nRow, nCol, 0);

    // Empty strings
    pCell->szText.Empty();
}

BOOL CGridCtrl::DeleteColumn(int nColumn)
{
    if (nColumn < 0 || nColumn >= GetColumnCount())
        return FALSE;

//    ResetSelectedRange();
    m_arColWidths.RemoveAt(nColumn);
    m_nCols--;

/*    GCOL_ITEM  *pColNew; 
	HGLOBAL	hMem = GlobalAlloc( GPTR, (m_nCols)* sizeof( GCOL_ITEM) );
	LPVOID hp=GlobalLock(hMem);
    if( hp )
	{
	    pColNew= (GCOL_ITEM *)hp;
	}
	else
		AfxMessageBox("分配内存出错",MB_ICONSTOP);
	if(m_pColData!=NULL)
	  memcpy(pColNew,m_pColData,m_nCols*sizeof(GCOL_ITEM));

	if(m_pColData!=NULL)
	{
	   GlobalUnlock((HGLOBAL)m_pColData);        //释放内存
	   GlobalFree( (HGLOBAL)m_pColData);
	}
	m_pColData=pColNew;
//19991106
/*    for (int row = 0; row < GetRowCount(); row++) 
    {
        GRID_ROW* pRow = m_RowData[row];
        if (!pRow) return FALSE;

        CGridCell* pCell = pRow->GetAt(nColumn);
        if (pCell) {
            EmptyCell(pCell, row, nColumn);
            delete pCell;
        }
        pRow->RemoveAt(nColumn);
    }
*/
    if (nColumn < m_nFixedCols) m_nFixedCols--;

    if (nColumn == m_idCurrentCell.col)
        m_idCurrentCell.col = 0;

//        m_idCurrentCell.row = m_idCurrentCell.col = -1;
    else if (nColumn < m_idCurrentCell.col)
        m_idCurrentCell.col--;

    ResetScrollBars();

    return FALSE;
}

BOOL CGridCtrl::DeleteRow(int nRow)
{
    if ( nRow < 0 || nRow >= GetRowCount())
        return FALSE;
//19991106
//    GRID_ROW* pRow = m_RowData[nRow];
//    if (!pRow) return FALSE;
    ResetSelectedRange();
    m_nRows--;
	m_arRowHeights.RemoveAt(nRow);
    if (nRow < m_nFixedRows) m_nFixedRows--;
    if (nRow == m_idCurrentCell.row)
        m_idCurrentCell.row = m_idCurrentCell.col = -1;
    else if (nRow < m_idCurrentCell.row)
        m_idCurrentCell.row--;
    ResetScrollBars();
    return TRUE;
}

// Handy function that removes all non-fixed rows
BOOL CGridCtrl::DeleteNonFixedRows()
{
    int nFixed = GetFixedRowCount();
    int nCount = GetRowCount();

    // Delete all data rows
    for (int nRow = nCount; nRow >= nFixed; nRow--) 
        DeleteRow(nRow);

    return TRUE;
}

// Removes all rows, columns and data from the grid.
BOOL CGridCtrl::DeleteAllItems()
{
    ResetSelectedRange();

    m_arColWidths.RemoveAll();
    m_arRowHeights.RemoveAll();
//19991106
/*
    // Delete all cells in the grid
    for (int row = 0; row < m_nRows; row++) 
    {
        GRID_ROW* pRow = m_RowData[row];
        if (!pRow) continue;
        for (int col = 0; col < m_nCols; col++)
        {
            CGridCell* pCell = pRow->GetAt(col);
            if (pCell) {
                EmptyCell(pCell, row, col);  // TODO - this is a bit of a performance hit.
                delete pCell;                // better to call m_SelectedCells.RemoveAll()
            }                                // instead. This is safer for changes though.
        }
        delete pRow;
    }

    // Remove all rows
    m_RowData.RemoveAll();
*/
    m_idCurrentCell.row = m_idCurrentCell.col = -1;
    m_nRows = m_nFixedRows = m_nCols = m_nFixedCols = 0;

    ResetScrollBars();

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl data functions

// Set CListCtrl::GetNextItem for details
CCellID CGridCtrl::GetNextItem(CCellID& cell, int nFlags) const
{
    if ((nFlags & GVNI_BELOW) && (nFlags & GVNI_TORIGHT))
    {
        int row = cell.row;
        if (row <= 0) row = GetFixedRowCount();
        for (; row < GetRowCount(); row++)
        {
            int col = cell.col + 1;
            if (col <= 0) col = GetFixedColumnCount();
            for (; col < GetColumnCount(); col++) 
            {
                int nState = GetItemState(row, col);
                if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) 
                    return CCellID(row, col);
                if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)     
                    return CCellID(row, col);
                if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)   
                    return CCellID(row, col);
                if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)   
                    return CCellID(row, col);    
            }
        }
    }
    if (nFlags & GVNI_ABOVE) 
    {
        for (int row = cell.row-1; row >= GetFixedRowCount(); row--) 
        {
            int nState = GetItemState(row, cell.col);
            if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) return CCellID(row, cell.col);
            if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)     return CCellID(row, cell.col);
            if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)   return CCellID(row, cell.col);
            if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)   return CCellID(row, cell.col);
        }
    }
    else if (nFlags & GVNI_BELOW)
    {
        for (int row = cell.row+1; row < GetRowCount(); row++) 
        {
            int nState = GetItemState(row, cell.col);
            if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) return CCellID(row, cell.col);
            if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)     return CCellID(row, cell.col);
            if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)   return CCellID(row, cell.col);
            if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)   return CCellID(row, cell.col);
        }
    } 
    else if (nFlags & GVNI_TOLEFT)
    {
        for (int col = cell.col-1; col >= GetFixedColumnCount(); col--) 
        {
            int nState = GetItemState(cell.row, col);
            if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) return CCellID(cell.row, col);
            if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)     return CCellID(cell.row, col);
            if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)   return CCellID(cell.row, col);
            if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)   return CCellID(cell.row, col);
       }
    }
    else if (nFlags & GVNI_TORIGHT) 
    {
        for (int col = cell.col+1; col < GetColumnCount(); col++) 
        {
            int nState = GetItemState(cell.row, col);
            if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) return CCellID(cell.row, col);
            if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)     return CCellID(cell.row, col);
            if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)   return CCellID(cell.row, col);
            if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)   return CCellID(cell.row, col);
        }
    }

    return CCellID(-1, -1);
}

// Sorts on a given column using the cell text
BOOL CGridCtrl::SortTextItems(int nCol, BOOL bAscending)
{
//add by xia Qiankun
// if clicked the fixed column, do nothing
	if(nCol<GetFixedColumnCount())return TRUE;
    ResetSelectedRange();
    SetFocusCell(-1,-1);
    return SortTextItems(nCol, bAscending, GetFixedRowCount(),-1);
}
float CGridCtrl::GetItemTextV(int nRow,int nCol)
{
    if(m_pDoc==NULL)		
		return TRUE;
	CReportData *pData=m_pDoc->m_pStockDataShow[nRow-m_nFixedRows].pItem;
	if(pData==NULL)
		return 0;
	switch(m_pColData[nCol].nID)
	{
		case ID_YSTC: 				                 //昨收
             return pData->ystc ;
			 break;
		case ID_OPEN:                                //今开
             return pData->opnp ;
			 break;
		case ID_HIGH:                                //最高
             return pData->higp  ;
			 break;
		case ID_LOW:                                 //最低
             return pData->lowp ;
			 break;
		case ID_NEW:                                 //最新
             return pData->nowp ;
			 break;
		case ID_TOTV:                                //总手
             return pData->totv  ;
			 break;
		case ID_NOWV:                                //现手
             return pData->nowv  ;
			 break;
		case ID_TOTP:                                //总额
             return pData->totp  ;
			 break;
		case ID_AVGP:                                //均价
			 if(pData->totv!=0)
				 return pData->totp /pData->totv /100 ;
			 else
                 return 0.0f ;
			 break;
		case ID_ADDE:                                //涨跌
			 if(pData->ystc !=0&&pData->nowp!=0)
			 {
				return pData->nowp - pData->ystc  ;
			 }
			 else
                return 0.0f ;
			 break;
		case ID_HLP:                                 //震幅
             if(pData->ystc !=0)
			 {
                return 100 * (pData->higp - pData->lowp) /pData->ystc ;
			 }
			 else
                return 0.0f; 
			 break;
		case ID_RDR:                                 //涨幅
			 if(pData->ystc !=0&&pData->nowp!=0)
			 {
                return 100 * (pData->nowp - pData->ystc )/ pData->ystc;
			 }
			 else
                return 0.0f; 
			 break;
		case ID_BSSPBAS:                            //委比
			{
				float temp=0;
				if (pData->accb+pData->accs>0)
					if(pData->accs==0&&pData->accb!=0)
					   temp=100;
					else if(pData->accs!=0&&pData->accb==0)
					   temp=-100;
					else
					   temp=(float)(pData->accb-pData->accs)/(float)(pData->accb+pData->accs)*100.0;     //计算委比
				else
					temp=0;
				return temp;
			}
			break;
		case ID_DIFBS:                              //委差
             return pData->accb - pData->accs;
			 break;
		case ID_VOLBI:                              //量比
			 if(m_pDoc->m_nANT[0] !=0 && pData->volume5!=0 )
			 {
                return (pData->totv / m_pDoc->m_nANT[0]) / (pData->volume5 /240 );
			 }
			 else
                return 0.0f; 
			 break;
		case ID_ACCB:                               //委买
               return pData->accb ;
			 break;
		case ID_ACCS:                               //委卖
               return pData->accs ;
			 break;
		case ID_BUY:                                //委买价
			   return pData->pbuy1 ;
			 break;
		case ID_SEL:                                //委卖价
			   return pData->psel1 ;
			 break;
		case ID_RVOL:                               //内盘
               return pData->dvol ;
			 break;
		case ID_DVOL:                              //外盘
               return pData->rvol   ;
			 break;
        case ID_EXCHANGE:
			  if(pData->pBaseInfo==NULL)
			     return 0;
              else
			  {
				  if(pData->kind==SHBG|| pData->kind==SZBG)
                     if(pData->pBaseInfo->Bg!=0) 
                        return pData->totv/pData->pBaseInfo->Bg;
					 else
			            return 0.0f;
				  else
					  if(pData->pBaseInfo->ltAg!=0)
			            return pData->totv/pData->pBaseInfo->ltAg;
					  else
                        return 0;
			  }
			  break;
        case ID_SYL:
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
				 if(pData->pBaseInfo->mgsy!=0)
                   return pData->nowp/pData->pBaseInfo->mgsy; 
				 else
			       return 0.0f;
			  }
			  break;
		case ID_BUYP1:                               //
               return pData->pbuy1  ;
		case ID_BUYP2:                               //
               return pData->pbuy2 ;
		case ID_BUYP3:                               //
               return pData->pbuy3 ;
//		case ID_BUYP4:                               //
  //             return pData->pbuy4 ;
		case ID_BUYV1:                               //
               return pData->vbuy1 ;
		case ID_BUYV2:                               //
               return pData->vbuy2 ;
		case ID_BUYV3:                                //
               return pData->vbuy3 ;
//		case ID_BUYV4:                                //
//               return pData->vbuy4 ;
		case ID_SELP1:                               //
               return pData->psel1 ;
		case ID_SELP2:                               //
               return pData->psel2;
		case ID_SELP3:                               //
               return pData->psel3;
//		case ID_SELP4:                               //
//               return pData->psel4;
		case ID_SELV1:                               //
               return pData->vsel1;
		case ID_SELV2:                               //
               return pData->vsel2;
		case ID_SELV3:                               //
               return pData->vsel3;
//		case ID_SELV4:                               //
  //             return pData->vsel4;
		case ID_ZGB:        //总股本(万股)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->zgb; 
			  }
			    break;
		case ID_GJG:        //国家股(万股)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->gjg; 
			  }
			    break;
		case ID_FQRFRG:     //发起人法人股(万股)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->fqrfrg; 
			  }
			    break;
		case ID_FRG:        //法人股(万股)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->frg; 
			  }
			    break;
		case ID_ZGG:        //职工股(万股)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->zgg; 
			  }
			    break;
		case ID_GZAG:       //公众A股(万股)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->gzAg; 
			  }
			    break;
		case ID_LTAG:       //流通A股(万股)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->ltAg; 
			  }
			    break;
		case ID_HG:         //Ｈ股(万股)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->Hg; 
			  }
			    break;
		case ID_BG:         //B股(万股)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->Bg; 
			  }
			    break;
		case ID_ZPG:        //转配股(万股)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->zpg; 
			  }
			    break;

		case ID_ZZC:        //总资产(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->zzc; 
			  }
			    break;
		case ID_LDZC:       //流动资产(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->ldzc; 
			  }
			    break;
		case ID_CQTZ:       //长期投资(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->cqtz; 
			  }
			    break;
		case ID_GDZC:       //固定资产(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->gdzc; 
			  }
			    break;
		case ID_WXZC:       //无形资产(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->wxzc; 
			  }
			    break;
		case ID_LDFZ:       //流动负债(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->ldfz; 
			  }
			    break;
		case ID_CQFZ:       //长期负债(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->cqfz; 
			  }
			    break;
		case ID_GGQY:       //股东权益(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->ggqy; 
			  }
			    break;
		case ID_ZBGJJ:      //资本公积金(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->zbgjj; 
			  }
			    break;
		case ID_YYGJJ:      //盈余公积金(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->yygjj; 
			  }
			    break;
		case ID_MGJZ:       //每股净值(元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->mgjz; 
			  }
			    break;
		case ID_GDQYBL:     //股东权益比率(%)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->gdqybl; 
			  }
			    break;
		case ID_MGGJJ:      //每股公积金(元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->mggjj; 
			  }
			    break;
		case ID_ZYYWSR:     //主营业务收入(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->zyywsr; 
			  }
			    break;
		case ID_ZYYWLR:     //主营业务利润(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->zyywlr; 
			  }
			    break;
		case ID_QTYWLR:     //其它业务利润(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->qtywlr; 
			  }
			    break;
		case ID_LRZE:       //利润总额(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->lrze; 
			  }
			    break;
		case ID_JLR:        //净利润(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->jlr; 
			  }
			    break;
		case ID_WFPLR:      //未分配利润(万元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->wfplr; 
			  }
			    break;
		case ID_MGSY:       //每股收益(元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->mgsy; 
			  }
			    break;
		case ID_JZCSYL:     //净资产收益率(%)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->jzcsyl; 
			  }
			    break;
		case ID_MGWFPLR:    //每股未分配利润(元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->mgwfplr; 
			  }
			    break;
		case ID_MGJZC:      //每股净资产(元)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
                 return pData->pBaseInfo->mgjzc; 
			  }
			    break;
		case ID_ZJLS:        //资金(%d分)
			  if(m_pDoc->m_nANT[0]>m_pDoc->m_syshqset.capital)
			  {
				int i=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_pDoc->m_syshqset.capital)
				{
					i--;
				}
                return pData->m_Kdata1[i].Amount - pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.capital ].Amount;
			  }
			  else
			  {
				int i=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_pDoc->m_syshqset.capital)
				{
					i--;
				}
                return pData->m_Kdata1[i].Amount;
			  }
			  break;
		case ID_VOLBIFM:    //量比(%d分)
			if(m_pDoc->m_nANT[0] !=0 && pData->volume5!=0 )
			{
				int i=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_pDoc->m_syshqset.quangtity)
				{
				   i--;
				}
                 if(m_pDoc->m_nANT[0] > m_pDoc->m_syshqset.quangtity)
                    return ((pData->m_Kdata1[i].Volume - pData->m_Kdata1[m_pDoc->m_nANT[0] -m_pDoc->m_syshqset.quangtity ].Volume)) / (pData->volume5 );
				 else
                    return ((pData->m_Kdata1[i].Volume ) ) / (pData->volume5  );
			}
			else
			     return 0.0f;
		    break;
		case ID_HLPFM:      //震幅(%d分)
			{
				float high=0,low=0,totp=0;
				int minute=m_pDoc->m_syshqset.swing;
				if(m_pDoc->m_nANT[0] < minute)
                    minute=m_pDoc->m_nANT[0];
				for(int i=0;i<minute;i++)
				{   
				    if(pData->m_Kdata1[m_pDoc->m_nANT[0]-i].Price==0)
						continue;
					if(high==0&&low==0)
					{
						high=low=pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price;
						continue;
					}
					if(pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price > high  )
						high=pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price;
					else if(pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price <low)
						low=pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price;
				}
				if(pData->ystc==0)
					return 0;
				else
				    return (high-low)/pData->ystc;
			}
			break;
		case ID_EXCHANGEFM:  //换手率(%d分)
			  if(pData->pBaseInfo==NULL)
			     return 0.0f;
              else
			  {
				  int i=m_pDoc->m_nANT[0];
				  while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_pDoc->m_syshqset.changehand)
				  {
				 	  i--;
				  }
				  if(pData->kind==SHBG|| pData->kind==SZBG)
                     if(pData->pBaseInfo->Bg!=0) 
					 {
						 if(m_pDoc->m_nANT[0] > m_pDoc->m_syshqset.changehand)
                           return (pData->m_Kdata1[i].Volume - pData->m_Kdata1[m_pDoc->m_nANT[0]-m_pDoc->m_syshqset.changehand].Volume)/pData->pBaseInfo->Bg;
						 else
                           return (pData->m_Kdata1[i].Volume)/pData->pBaseInfo->Bg;
					 }
					 else
			             return 0.0f;
				  else
					  if(pData->pBaseInfo->ltAg!=0)
					  {
						 if(m_pDoc->m_nANT[0] > m_pDoc->m_syshqset.changehand)
			                return (pData->m_Kdata1[i].Volume - pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.changehand].Volume)/pData->pBaseInfo->ltAg;
						 else
			                return (pData->m_Kdata1[i].Volume)/pData->pBaseInfo->ltAg;
					  }
					  else
			             return 0.0f;
			  }
			  break;
		case ID_ADVSP:       //涨速(%d分)
			{
			float m_temp=0;
			if( m_pDoc->m_syshqset.upspeed  <m_pDoc->m_nANT[0])
				if( pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.upspeed ].Price!=0)
				{
					float m_nowp=0;
					int i=m_pDoc->m_nANT[0];
					while( pData->m_Kdata1[i].Price==0&&i!=0)
					{
						  i--;
					}
					m_nowp=pData->m_Kdata1[i].Price;
					m_temp= 100*(m_nowp - 
					pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.upspeed ].Price)/pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.upspeed ].Price;
				}   
				else
                    m_temp=0.0f;
			else
				if(pData->opnp !=0)
				{
					float m_nowp=0;
					int i=m_pDoc->m_nANT[0];
					while( pData->m_Kdata1[i].Price==0&&i!=0)
					{
						  i--;
					}
                    m_nowp=pData->m_Kdata1[i].Price;

					m_temp= 100*(m_nowp - pData->opnp )/pData->opnp ;
				}
				else
                    m_temp=0.0f;
              return m_temp;
			}
			break;
		default:
               if(m_pColData[nCol].nID>=9000&&m_pColData[nCol].nID<9010)
			   {
                   return pData->Index[m_pColData[nCol].nID - 9000];
			   }
			break;
		}
}
// recursive sort implementation
// this function is modified by Xia Qiankun, 1999/4/21
//the sort is based on double data
BOOL CGridCtrl::SortTextItems(int nCol, BOOL bAscending, int low, int high)
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
				Swap(m_pDoc->m_pStockDataShow[lo - m_nFixedRows],m_pDoc->m_pStockDataShow[hi - m_nFixedRows]);
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

// Sorts on a given column using the supplied compare function (see CListCtrl::SortItems)
BOOL CGridCtrl::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, 
                          LPARAM data /* = 0 */)
{
    ResetSelectedRange();
    SetFocusCell(-1,-1);
    return SortItems(pfnCompare, nCol, bAscending, data, GetFixedRowCount(), -1);
}

// recursive sort implementation
BOOL CGridCtrl::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data,
                          int low, int high)
{
    if (nCol >= GetColumnCount()) return FALSE;

    if (high == -1) high = GetRowCount() - 1;

    int lo = low;
    int hi = high;

    if( hi <= lo ) return FALSE;

    LPARAM midItem = GetItemData( (lo+hi)/2, nCol );

    // loop through the list until indices cross
    while( lo <= hi )
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        if( bAscending )
            while (lo < high  && pfnCompare(GetItemData(lo, nCol), midItem, data) < 0)
                ++lo;
        else
            while (lo < high && pfnCompare(GetItemData(lo, nCol), midItem, data) > 0)
                ++lo;

        // Find an element that is smaller than or equal to  the partition 
        // element starting from the right Index.
        if( bAscending )
            while (hi > low && pfnCompare(GetItemData(hi, nCol), midItem, data) > 0)
                --hi;
        else
            while (hi > low && pfnCompare(GetItemData(hi, nCol), midItem, data) < 0)
                --hi;

        // If the indexes have not crossed, swap if the items are not equal
        if (lo <= hi)
        {
            // swap only if the items are not equal
            if (pfnCompare(GetItemData(lo, nCol), GetItemData(hi, nCol), data) != 0)
            {
                CGridCell *ploCell  ;
                CGridCell *phiCell;
			    ploCell=new CGridCell; 
				phiCell=new CGridCell; 
                for (int col = 1; col < GetColumnCount(); col++)
                {
					GetCell(ploCell,lo, col);
					GetCell(phiCell,hi, col);
                    SetCell(lo, col, phiCell);
                    SetCell(hi, col, ploCell);
                }
				delete ploCell;
				delete phiCell;
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
        SortItems(pfnCompare, nCol, bAscending, data, low, hi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( lo < high )
        SortItems(pfnCompare, nCol, bAscending, data, lo, high);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl data functions

BOOL CGridCtrl::SetItem(const GV_ITEM* pItem)
{
    if (!pItem) return FALSE;
    CGridCell* pCell ;
	pCell=new CGridCell; 
    if (!pCell) return FALSE;
	GetCell(pCell,pItem->row, pItem->col);

    if (pItem->mask & GVIF_TEXT)   pCell->szText  = pItem->szText;
    if (pItem->mask & GVIF_PARAM)  pCell->lParam  = pItem->lParam;
    if (pItem->mask & GVIF_IMAGE)  pCell->iImage  = pItem->iImage;
    if (pItem->mask & GVIF_STATE)  pCell->state   = pItem->state;
    if (pItem->mask & GVIF_FORMAT) pCell->nFormat = pItem->nFormat;
    if (pItem->mask & GVIF_BKCLR)  pCell->crBkClr = pItem->crBkClr;
    if (pItem->mask & GVIF_FGCLR)  pCell->crFgClr = pItem->crFgClr;
    if (pItem->mask & GVIF_FONT)   memcpy(&(pCell->lfFont), &(pItem->lfFont), sizeof(LOGFONT));
	SetCell(pItem->row, pItem->col, pCell);
	delete pCell;
    return TRUE;
}

BOOL CGridCtrl::GetItem(GV_ITEM* pItem)
{
    if (!pItem) return FALSE;
    CGridCell* pCell  ;
	pCell=new CGridCell;
    if (!pCell)
	{
		return FALSE;
	}
	GetCell(pCell,pItem->row, pItem->col);

//    if (pItem->mask & GVIF_TEXT)   pItem->szText  = GetItemText(pItem->row, pItem->col);
    if (pItem->mask & GVIF_TEXT)   pItem->szText  = pCell->szText;
    if (pItem->mask & GVIF_PARAM)  pItem->lParam  = pCell->lParam;
    if (pItem->mask & GVIF_IMAGE)  pItem->iImage  = pCell->iImage;
    if (pItem->mask & GVIF_STATE)  pItem->state   = pCell->state;
    if (pItem->mask & GVIF_FORMAT) pItem->nFormat = pCell->nFormat;
    if (pItem->mask & GVIF_BKCLR)  pItem->crBkClr = pCell->crBkClr;
    if (pItem->mask & GVIF_FGCLR)  pItem->crFgClr = pCell->crFgClr;
    if (pItem->mask & GVIF_FONT)   memcpy(&(pItem->lfFont), &(pCell->lfFont), sizeof(LOGFONT));
	delete pCell;
    return TRUE;
}

BOOL CGridCtrl::SetItemText(int nRow, int nCol, LPCTSTR str)
{
    CGridCell* pCell ;
	pCell=new CGridCell; 
    if (!pCell) 
	{
		return FALSE;
	}
	GetCell(pCell,nRow, nCol);
    pCell->szText = str;
	SetCell(nRow, nCol, pCell);
	delete pCell;
    return TRUE;
}

BOOL CGridCtrl::SetItemData(int nRow, int nCol, LPARAM lParam)
{
    CGridCell* pCell ;
	pCell=new CGridCell;
    if (!pCell) 
	{
		return FALSE;
	}
	GetCell(pCell,nRow, nCol);
    pCell->lParam = lParam;
	delete pCell;
    return TRUE;
}

LPARAM CGridCtrl::GetItemData(int nRow, int nCol) const
{    
    CGridCell* pCell ;
	pCell=new CGridCell; 
    if (!pCell) 
	{
		return (LPARAM) 0;
	}
	GetCell(pCell,nRow, nCol);
	int IParam=0;
	IParam=pCell->lParam;
	delete pCell;
    return IParam;
}

BOOL CGridCtrl::SetItemImage(int nRow, int nCol, int iImage)
{
    CGridCell* pCell=NULL ;
	pCell=new CGridCell; 
    if (!pCell) 
	{
		return (LPARAM) 0;
	}
	GetCell(pCell,nRow, nCol);
    pCell->iImage = iImage;
	delete pCell;
    return TRUE;
}

int CGridCtrl::GetItemImage(int nRow, int nCol) const
{
    CGridCell* pCell=NULL;
	pCell=new CGridCell;
    ASSERT(pCell);
    if (!pCell) 
	{
		return -1;
	}
	int iImage=0;
	GetCell(pCell,nRow, nCol);
	iImage=pCell->iImage;
	delete pCell;
    return iImage;
}

BOOL CGridCtrl::SetItemState(int nRow, int nCol, UINT state)
{
    CGridCell* pCell=NULL ;
    pCell=new CGridCell; 
    if (!pCell)
	{
		return FALSE;
	}
	GetCell(pCell,nRow, nCol);
    ASSERT(pCell);
/*
    // If the cell is being unselected, remove it from the selected list
//    if ((pCell->state & GVIS_SELECTED) && !(state & GVIS_SELECTED))
    if (!(state & GVIS_SELECTED))
    {
        CCellID cell;
        DWORD key = MAKELONG(nRow, nCol);

        if (m_SelectedCellMap.Lookup(key, (CCellID&)cell))
            m_SelectedCellMap.RemoveKey(key);
		if(m_PrevSelectedCellMap.GetCount()==GetRowCount() -1)
			if (m_PrevSelectedCellMap.Lookup(key, (CCellID&)cell))
				m_PrevSelectedCellMap.RemoveKey(key);
    }

    // If cell is being selected, add it to the list of selected cells
//    else if (!(pCell->state & GVIS_SELECTED) && (state & GVIS_SELECTED))
    else if ((state & GVIS_SELECTED))
    {
        CCellID cell(nRow, nCol);
        m_SelectedCellMap.SetAt(MAKELONG(nRow, nCol), cell);
    }
*/
    // Set the cell's state
    pCell->state = state;
	delete pCell;
    return TRUE;
}

UINT CGridCtrl::GetItemState(int nRow, int nCol) const
{
    CGridCell* pCell=NULL ;
	pCell=new CGridCell; 
    ASSERT(pCell);
    if (!pCell)
	{
		return 0;
	}
	int state=0;
	GetCell(pCell,nRow, nCol);
	state=pCell->state;
	delete pCell;
    return state;
}

BOOL CGridCtrl::SetItemFormat(int nRow, int nCol, UINT nFormat)
{
    CGridCell* pCell=NULL ;
	pCell=new CGridCell; 
    ASSERT(pCell);
    if (!pCell)
	{
		return FALSE;
	}
	GetCell(pCell,nRow, nCol);
    pCell->nFormat = nFormat;
	delete pCell;
    return TRUE;
}

UINT CGridCtrl::GetItemFormat(int nRow, int nCol) const
{
    CGridCell* pCell=NULL ;
    pCell=new CGridCell;
	int Format=0;
    ASSERT(pCell);
    if (!pCell)
	{
		return 0;
	}
	GetCell(pCell,nRow, nCol);
	Format=pCell->nFormat;
	delete pCell;
    return Format;
}

BOOL CGridCtrl::SetItemBkColour(int nRow, int nCol, COLORREF cr /* = CLR_DEFAULT */)
{
    CGridCell* pCell=NULL		;
	pCell=new CGridCell;
    ASSERT(pCell);
    if (!pCell) 
	{
		return FALSE;
	}
	GetCell(pCell,nRow, nCol);
    pCell->crBkClr = cr;
	delete pCell;
    return TRUE;
}

COLORREF CGridCtrl::GetItemBkColour(int nRow, int nCol) const
{
    CGridCell* pCell=NULL ;
	COLORREF BkClr;
	pCell=new CGridCell; 
    ASSERT(pCell);
    if (!pCell) 
	{
		return 0;
	}
	GetCell(pCell,nRow, nCol);
	BkClr=pCell->crBkClr;
	delete pCell;
    return BkClr;
}

BOOL CGridCtrl::SetItemFgColour(int nRow, int nCol, COLORREF cr /* = CLR_DEFAULT */)
{
    CGridCell* pCell=NULL ;
	pCell=new CGridCell; 
    ASSERT(pCell);
    if (!pCell) 
	{
		return FALSE;
	}
	GetCell(pCell,nRow, nCol);

    pCell->crFgClr = cr;
	delete pCell;
    return TRUE;
}

COLORREF CGridCtrl::GetItemFgColour(int nRow, int nCol) const
{
    CGridCell* pCell=NULL ;
	pCell=new CGridCell; 
    ASSERT(pCell);
    if (!pCell) 
	{
		return 0;
	}
	GetCell(pCell,nRow, nCol);
	COLORREF crFgClr=pCell->crFgClr;
	delete pCell;
    return crFgClr;
}

BOOL CGridCtrl::SetItemFont(int nRow, int nCol, LOGFONT* lf)
{
    CGridCell* pCell=NULL;
	pCell=new CGridCell; 
    ASSERT(pCell);
    if (!pCell) 
	{
		return FALSE;
	}
	GetCell(pCell,nRow, nCol);
    memcpy(&(pCell->lfFont), lf, sizeof(LOGFONT));
	delete pCell;
    return TRUE;
}

LOGFONT* CGridCtrl::GetItemFont(int nRow, int nCol) const
{
    CGridCell* pCell=NULL;
    pCell=new CGridCell; 
    ASSERT(pCell);
    if (!pCell) return NULL;
	GetCell(pCell,nRow, nCol);
	LOGFONT lfFont;
	memcpy(&lfFont,&pCell->lfFont,sizeof(LOGFONT));
	delete pCell;
    return &lfFont;
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column size functions

long CGridCtrl::GetVirtualWidth() const
{
    long lVirtualWidth = 0;
    int iColCount = GetColumnCount();
    for (int i = 0; i < iColCount; i++)
        lVirtualWidth += m_arColWidths[i];

    return lVirtualWidth;
}

long CGridCtrl::GetVirtualHeight() const
{
    long lVirtualHeight = 0;
	int iRowCount;

	iRowCount = GetRowCount() ;
//	iRowCount = GetRealRow() ;

    for (int i = 0; i < iRowCount; i++)
        lVirtualHeight += m_arRowHeights[i];

    return lVirtualHeight;
}

int CGridCtrl::GetRowHeight(int nRow) const
{
    ASSERT(nRow >= 0 && nRow < m_nRows);
    if (nRow < 0 || nRow >= m_nRows) return -1;

    return m_arRowHeights[nRow];
}

int CGridCtrl::GetColumnWidth(int nCol) const
{
    ASSERT(nCol >= 0 && nCol < m_nCols);
    if (nCol < 0 || nCol >= m_nCols) return -1;

    return m_arColWidths[nCol];
}

BOOL CGridCtrl::SetRowHeight(int nRow, int height)
{
    ASSERT(nRow >= 0 && nRow < m_nRows && height >= 0);
    if (nRow < 0 || nRow >= m_nRows || height < 0) return FALSE;

    m_arRowHeights[nRow] = height;    
    return TRUE;
}

BOOL CGridCtrl::SetColumnWidth(int nCol, int width)
{
    ASSERT(nCol >= 0 && nCol < m_nCols && width >= 0);
    if (nCol < 0 || nCol >= m_nCols || width < 0) return FALSE;

    m_arColWidths[nCol] = width;
    return TRUE;
}

int CGridCtrl::GetFixedRowHeight() const
{
    int nHeight = 0;
    for (int i = 0; i < m_nFixedRows; i++)
        nHeight += GetRowHeight(i);

    return nHeight;
}

int CGridCtrl::GetFixedColumnWidth() const
{
    int nWidth = 0;
    for (int i = 0; i < m_nFixedCols; i++)
        nWidth += GetColumnWidth(i);

    return nWidth;
}

BOOL CGridCtrl::AutoSizeColumn(int nCol)
{
    ASSERT(nCol >= 0 && nCol < m_nCols);
    if (nCol < 0 || nCol >= m_nCols) return FALSE;

    CSize size;
    CDC* pDC = GetDC();
    if (!pDC) return FALSE;

    int nWidth = 0;
    int nNumRows = GetRowCount();
    for (int nRow = 0; nRow < nNumRows; nRow++)
    {
        size = GetCellExtent(nRow, nCol, pDC);
        if (size.cx > nWidth) nWidth = size.cx;
    }

    m_arColWidths[nCol] = nWidth;

    ReleaseDC(pDC);
    ResetScrollBars();

    return TRUE;
}

BOOL CGridCtrl::AutoSizeRow(int nRow)
{
    ASSERT(nRow >= 0 && nRow < m_nRows);
    if (nRow < 0 || nRow >= m_nRows) return FALSE;

    CSize size;
    CDC* pDC = GetDC();
    if (!pDC) return FALSE;

    int nHeight = 0;
    int nNumColumns = GetColumnCount();
    for (int nCol = 0; nCol < nNumColumns; nCol++)
    {  
        size = GetCellExtent(nRow, nCol, pDC);
        if (size.cy > nHeight) nHeight = size.cy;
    }  
    m_arRowHeights[nRow] = nHeight;

    ReleaseDC(pDC);
    ResetScrollBars();

    return TRUE;
}

void CGridCtrl::AutoSizeColumns()
{
    int nNumColumns = GetColumnCount();
    for (int nCol = 0; nCol < nNumColumns; nCol++)
        AutoSizeColumn(nCol);
}

void CGridCtrl::AutoSizeRows()
{
    int nNumRows = GetRowCount();
    for (int nRow = 0; nRow < nNumRows; nRow++)
        AutoSizeRow(nRow);
}

// sizes all rows and columns
// faster than calling both AutoSizeColumns() and AutoSizeRows()
void CGridCtrl::AutoSize()
{
    CDC* pDC = GetDC();
    if (!pDC) return;

    int nNumColumns = GetColumnCount();
    int nNumRows = GetRowCount();
    
    // initialize column widths to zero
//    for (int nCol = 0; nCol < nNumColumns; nCol++)
  //      m_arColWidths[nCol] = 0;
    
    // initialize row heights to zero
    //for (int nRow = 0; nRow < nNumRows; nRow++)
      //  m_arRowHeights[nRow] = 0;
    int nCol,nRow;
    CSize size;

/*    for (nCol = 0; nCol < nNumColumns; nCol++)
        for (nRow = 0; nRow < nNumRows; nRow++)
        {
            size = GetCellExtent(nRow, nCol, pDC);
            if (size.cx > (int) m_arColWidths[nCol])  m_arColWidths[nCol] = size.cx;
            if (size.cy > (int) m_arRowHeights[nRow]) m_arRowHeights[nRow] = size.cy;
        } 
  */  
     //////////////////Modi by zjh for speed
	  for (nCol = 0; nCol < nNumColumns; nCol++)
        for (nRow = 0; nRow < 2; nRow++)
        {
            size = GetCellExtent(nRow, nCol, pDC);
            if (size.cx > (int) m_arColWidths[nCol])  m_arColWidths[nCol] = size.cx;
            if (size.cy > (int) m_arRowHeights[nRow]) m_arRowHeights[nRow] = size.cy;
        } 
	for (nRow = 2; nRow < nNumRows; nRow++)
         m_arRowHeights[nRow] = m_arRowHeights[1];
    
	ReleaseDC(pDC);

    if (m_bAllowDraw) {
        ResetScrollBars();
        Invalidate();
    }
}

void CGridCtrl::ExpandColumnsToFit()
{
    if (GetColumnCount() <= 0) return;

    CRect rect;
    GetClientRect(rect);

    long virtualWidth = GetVirtualWidth();
    int nDifference = rect.Width() - (int) virtualWidth;
    int nColumnAdjustment = nDifference / GetColumnCount();

	int i;
    for (i = 0; i < GetColumnCount(); i++)
       m_arColWidths[i] += nColumnAdjustment;

    if (nDifference > 0)
    {
        int leftOver = nDifference % GetColumnCount();
        for (i = 0; i < leftOver; i++)
            m_arColWidths[i] += 1;
    } 
    else 
    {
        int leftOver = (-nDifference) % GetColumnCount();
        for (i = 0; i < leftOver; i++)
            m_arColWidths[i] -= 1;
    }

    if (m_bAllowDraw) 
        Invalidate();
}

void CGridCtrl::ExpandRowsToFit()
{
    if (GetRowCount() <= 0) return;

    CRect rect;
    GetClientRect(rect);

    long virtualHeight = GetVirtualHeight();
    int nDifference = rect.Height() - (int) virtualHeight;
    int nRowAdjustment = nDifference / GetRowCount();

	int i;
    for (i = 0; i < GetRowCount(); i++)
       m_arRowHeights[i] += nRowAdjustment;

    if (nDifference > 0)
    {
        int leftOver = nDifference % GetRowCount();
        for (i = 0; i < leftOver; i++)
            m_arRowHeights[i] += 1;
    } 
    else 
    {
        int leftOver = (-nDifference) % GetRowCount();
        for (i = 0; i < leftOver; i++)
            m_arRowHeights[i] -= 1;
    }

    if (m_bAllowDraw) 
        Invalidate();
}

void CGridCtrl::ExpandToFit()
{
    ExpandColumnsToFit();   // This will remove any existing horz scrollbar
    ExpandRowsToFit();      // This will remove any existing vert scrollbar
    ExpandColumnsToFit();   // Just in case the first adjustment was with a vert
                            // scrollbar in place
}

/////////////////////////////////////////////////////////////////////////////////////
// GridCtrl cell visibility tests and invalidation/redraw functions

// EnsureVisible supplied by Roelf Werkman
void CGridCtrl::EnsureVisible(int nRow, int nCol)
{
    CCellRange VisibleCells = GetVisibleNonFixedCellRange();

    int right = nCol - VisibleCells.GetMaxCol();
    int left  = VisibleCells.GetMinCol() - nCol;
    int down  = nRow - VisibleCells.GetMaxRow();
    int up    = VisibleCells.GetMinRow() - nRow;	

	while (right > 0)
    {
		::SendMessage(GetParent()->m_hWnd ,WM_USER_HSCROLL,1,0);
        SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
        right--;
    }
    while (left > 0)
    {
		::SendMessage(GetParent()->m_hWnd ,WM_USER_HSCROLL,0,0);
        SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
        left--;
    }
    while (down > 0)
    {
        SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
        down--;
   }
   while (up > 0)
   {
       SendMessage(WM_VSCROLL, SB_LINEUP, 0);
       up--;
   }

   // Move one more if we only see a snall bit of the cell
   CRect rectCell, rectWindow;
   GetCellRect(nRow, nCol, rectCell);
   GetClientRect(rectWindow);
   if (rectCell.right > rectWindow.right)
   {
		::SendMessage(GetParent()->m_hWnd ,WM_USER_HSCROLL,1,0);
        SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
   }
   if (rectCell.bottom > rectWindow.bottom)
   {
	   SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
   }
}

BOOL CGridCtrl::IsCellEditable(CCellID &cell) const
{
    return IsCellEditable(cell.row, cell.col); 
}

BOOL CGridCtrl::IsCellEditable(int nRow, int nCol) const
{
    return IsEditable() && ((GetItemState(nRow, nCol) & GVIS_READONLY) != GVIS_READONLY);
}

BOOL CGridCtrl::IsCellVisible(CCellID cell) const
{  
    return IsCellVisible(cell.row, cell.col);
}

BOOL CGridCtrl::IsCellVisible(int nRow, int nCol) const
{  
    if (!IsWindow(m_hWnd))
        return FALSE;

    int x,y;

    CCellID TopLeft;
    if (nCol >= GetFixedColumnCount() || nRow >= GetFixedRowCount())
    {
        TopLeft = GetTopleftNonFixedCell();
        if (nCol >= GetFixedColumnCount() && nCol < TopLeft.col) return FALSE;
        if (nRow >= GetFixedRowCount() && nRow < TopLeft.row) return FALSE;
    }

    CRect rect;
    GetClientRect(rect);
    if (nCol < GetFixedColumnCount())
    {
        x = 0;
        for (int i = 0; i <= nCol; i++) 
        {
            if (x >= rect.right) return FALSE;
            x += GetColumnWidth(i);    
        }
    } 
    else 
    {
        x = GetFixedColumnWidth();
        for (int i = TopLeft.col; i <= nCol; i++) 
        {
            if (x >= rect.right) return FALSE;
            x += GetColumnWidth(i);    
        }
    }

    if (nRow < GetFixedRowCount())
    {
        y = 0;
        for (int i = 0; i <= nRow; i++) 
        {
            if (y >= rect.bottom) return FALSE;
            y += GetRowHeight(i);    
        }
    } 
    else 
    {
        if (nRow < TopLeft.row) return FALSE;
        y = GetFixedRowHeight();
        for (int i = TopLeft.row; i <= nRow; i++) 
        {
            if (y >= rect.bottom) return FALSE;
            y += GetRowHeight(i);    
        }
    }

    return TRUE;
}

BOOL CGridCtrl::InvalidateCellRect(const CCellID& cell)
{
    if (!::IsWindow(GetSafeHwnd()) || !m_bAllowDraw)
        return FALSE;

    ASSERT(IsValid(cell));

    if (!IsCellVisible(cell.row, cell.col)) return FALSE;

    CRect rect;
    if (!GetCellRect(cell, rect)) return FALSE;
    rect.right++; rect.bottom++;
    InvalidateRect(rect, TRUE);

    return TRUE;
}

BOOL CGridCtrl::InvalidateCellRect(const CCellRange& cellRange)
{
    ASSERT(IsValid(cellRange));
    if (!::IsWindow(GetSafeHwnd()) || !m_bAllowDraw) return FALSE;

    CCellRange visibleCellRange = GetVisibleNonFixedCellRange().Intersect(cellRange);

    CRect rect;
    if (!GetCellRangeRect(visibleCellRange, rect)) return FALSE;

    rect.right++; rect.bottom++;
    InvalidateRect(rect, TRUE);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl Mouse stuff

// Handles mouse wheel notifications
// Note - if this doesn't work for win95 then use OnRegisteredMouseWheel instead
#if _MFC_VER >= 0x0421
BOOL CGridCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
    // A m_nRowsPerWheelNotch value less than 0 indicates that the mouse
    // wheel scrolls whole pages, not just lines.
    if (m_nRowsPerWheelNotch == -1)
    {
        int nPagesScrolled = zDelta / 120;

        if (nPagesScrolled > 0)
            for (int i = 0; i < nPagesScrolled; i++)
                PostMessage(WM_VSCROLL, SB_PAGEUP, 0);
        else
            for (int i = 0; i > nPagesScrolled; i--)
                PostMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
    }
    else
    {
        int nRowsScrolled = m_nRowsPerWheelNotch * zDelta / 120;

        if (nRowsScrolled > 0)
            for (int i = 0; i < nRowsScrolled; i++)
                PostMessage(WM_VSCROLL, SB_LINEUP, 0);
        else
            for (int i = 0; i > nRowsScrolled; i--)
                PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
    }

    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
#endif

void CGridCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    CRect rect;
    GetClientRect(rect);

    // If outside client area, return (unless we are drag n dropping)
    if (m_MouseMode != MOUSE_DRAGGING && !rect.PtInRect(point))
        return;

    // If the left mouse button is up, then test to see if row/column sizing is imminent
    if (!(nFlags & MK_LBUTTON))
    {
        if (point.y < GetFixedRowHeight() && m_bAllowColumnResize)
        {
            CCellID idCurrentCell = GetCellFromPt(point);
            CPoint start;
            if (!GetCellOrigin(idCurrentCell, &start)) return;

            int endx = start.x + GetColumnWidth(idCurrentCell.col);

            if ((point.x - start.x <= m_nResizeCaptureRange && idCurrentCell.col != 0) || 
                endx - point.x <= m_nResizeCaptureRange)
            {
                if (m_MouseMode != MOUSE_OVER_COL_DIVIDE)
                    SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
                m_MouseMode = MOUSE_OVER_COL_DIVIDE;
            }
            else 
            {
                if (m_MouseMode != MOUSE_NOTHING)
                    SetCursor(::LoadCursor(NULL, IDC_ARROW));
                m_MouseMode = MOUSE_NOTHING;
            }
        }
        else if (point.x < GetFixedColumnWidth() && m_bAllowRowResize)
        {
            CCellID idCurrentCell = GetCellFromPt(point);
            CPoint start;
            if (!GetCellOrigin(idCurrentCell, &start)) return;

            int endy = start.y + GetRowHeight(idCurrentCell.row);

            if ((point.y - start.y <= m_nResizeCaptureRange && idCurrentCell.row != 0) || 
                endy - point.y <= m_nResizeCaptureRange)
            {
                if (m_MouseMode != MOUSE_OVER_ROW_DIVIDE)
                    SetCursor(::LoadCursor(NULL, IDC_SIZENS));
                m_MouseMode = MOUSE_OVER_ROW_DIVIDE;
            }
            else
            {
                if (m_MouseMode != MOUSE_NOTHING)
                    SetCursor(::LoadCursor(NULL, IDC_ARROW));
                m_MouseMode = MOUSE_NOTHING;
            }
        }
        else
        {
            if (m_MouseMode != MOUSE_NOTHING)
                SetCursor(::LoadCursor(NULL, IDC_ARROW));
            m_MouseMode = MOUSE_NOTHING;
        }

#ifdef GRIDCONTROL_USE_TITLETIPS
        if (m_MouseMode == MOUSE_NOTHING && m_bTitleTips)
        {
            CCellID idCurrentCell = GetCellFromPt(point);
            CRect rect;
            if (GetCellRect(idCurrentCell.row, idCurrentCell.col, rect))
                m_TitleTip.Show( rect, GetItemText(idCurrentCell.row, idCurrentCell.col), 0);
        }
#endif

        m_LastMousePoint = point;
        return;
    }

    if (!IsValid(m_LeftClickDownCell))
    {
        m_LastMousePoint = point;
        return;
    }

    // If the left mouse button is down, the process appropriately
    if (nFlags & MK_LBUTTON) 
    {
        switch(m_MouseMode)
        {
            case MOUSE_SELECT_ALL:        break;
/*
            case MOUSE_SELECT_COL:
            case MOUSE_SELECT_ROW:    
            case MOUSE_SELECT_CELLS:    {
                                            CCellID idCurrentCell = GetCellFromPt(point);
                                            if (!IsValid(idCurrentCell)) return;
                                            OnSelecting(idCurrentCell);
                                            //SetFocusCell(max(idCurrentCell.row, m_nFixedRows),
                                            //             max(idCurrentCell.col, m_nFixedCols));
                                            if (idCurrentCell.row >= m_nFixedRows &&
                                                idCurrentCell.col >= m_nFixedCols)
                                                    SetFocusCell(idCurrentCell);
                                            break;
                                        }  */

            case MOUSE_SIZING_COL:        {
                                            CDC* pDC = GetDC();
                                            if (!pDC) break;

                                            CRect oldInvertedRect(m_LastMousePoint.x, rect.top, 
                                                                  m_LastMousePoint.x + 2, rect.bottom);
                                            pDC->InvertRect(&oldInvertedRect);
                                            CRect newInvertedRect(point.x, rect.top, 
                                                                  point.x + 2, rect.bottom);
                                            pDC->InvertRect(&newInvertedRect);
                                            ReleaseDC(pDC);
                                        }
                                        break;

            case MOUSE_SIZING_ROW:        {
                                            CDC* pDC = GetDC();
                                            if (!pDC) break;

                                            CRect oldInvertedRect(rect.left, m_LastMousePoint.y, 
                                                                  rect.right, m_LastMousePoint.y + 2);
                                            pDC->InvertRect(&oldInvertedRect);
                                            CRect newInvertedRect(rect.left, point.y, 
                                                                  rect.right, point.y + 2);
                                            pDC->InvertRect(&newInvertedRect);
                                            ReleaseDC(pDC);
                                        }
                                        break;

            case MOUSE_PREPARE_DRAG:    OnBeginDrag();    break;
        }    
    }

    m_LastMousePoint = point;
}

void CGridCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    if (m_MouseMode == MOUSE_OVER_COL_DIVIDE) 
    {
        CCellID cell = GetCellFromPt(point);
        ASSERT(IsValid(cell));

        CPoint start;
        if (!GetCellOrigin(0, cell.col, &start)) return;

        if (point.x - start.x <= m_nResizeCaptureRange)     // Clicked right of border
            cell.col--;

        AutoSizeColumn(cell.col);
        Invalidate();
    } 
    else if (m_MouseMode == MOUSE_OVER_ROW_DIVIDE)
    {
        CCellID cell = GetCellFromPt(point);
        ASSERT(IsValid(cell));

        CPoint start;
        if (!GetCellOrigin(0, cell.col, &start)) return;

        if (point.y - start.y <= m_nResizeCaptureRange)     // Clicked below border
            cell.row--;

        AutoSizeRow(cell.row);
        Invalidate();
    }
    else if (m_MouseMode == MOUSE_NOTHING)
    {
        if (m_LeftClickDownCell.row >= m_nFixedRows && 
            IsValid(m_LeftClickDownCell) &&
            m_LeftClickDownCell.col >= m_nFixedCols)
        {
            OnEditCell(m_idCurrentCell.row, m_idCurrentCell.col, VK_LBUTTON);
        }
        else if (m_bListMode)
        {
            CCellID cell = GetCellFromPt(point);
            if (!IsValid(cell)) return;
            if (cell.row >= m_nFixedRows && cell.col < GetFixedColumnCount())
                OnEditCell(cell.row, cell.col, VK_LBUTTON);
        }
    }

    CWnd::OnLButtonDblClk(nFlags, point);
}

void CGridCtrl::OnLButtonDown(UINT nFlags, CPoint point)
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

	CView *pView = (CView* )GetParent();
	if (m_LeftClickDownCell.row == 0 && m_LeftClickDownCell.col == 0 && pView != NULL && pView->IsKindOf(RUNTIME_CLASS(CTaiShanReportView)))//弹出菜单
	{
		CPoint posMouse;
		::GetCursorPos(&posMouse);

		BCMenu menu;
		menu.LoadMenu(IDR_MENU_AVE);
		BCMenu* pMenu = (BCMenu*)menu.GetSubMenu(0);//;
		if (m_pDoc->m_bStatistic[AVE_VALUE])
		{
			pMenu->CheckMenuItem (AVE_VALUE,MF_BYPOSITION|MF_CHECKED );
		}
		if (m_pDoc->m_bStatistic[MAX_VALUE])
		{
			pMenu->CheckMenuItem (MAX_VALUE,MF_BYPOSITION|MF_CHECKED );
		}
		if (m_pDoc->m_bStatistic[MIN_VALUE])
		{
			pMenu->CheckMenuItem (MIN_VALUE,MF_BYPOSITION|MF_CHECKED );
		}
		if (m_pDoc->m_bStatistic[VAR_VALUE])
		{
			pMenu->CheckMenuItem (VAR_VALUE,MF_BYPOSITION|MF_CHECKED );
		}
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN,posMouse.x,posMouse.y,this,NULL);
	}

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
		//if(m_LeftClickDownCell.row < GetFixedRowCount()&&(m_LeftClickDownCell.col==0||m_LeftClickDownCell.col==1||m_LeftClickDownCell.col==2))
		if(m_LeftClickDownCell.row < GetFixedRowCount()&&(m_LeftClickDownCell.col < m_nFixedCols))
		{
           m_idCurrentCell.row=m_BeforeRow;
           return;
		//}else if (m_LeftClickDownCell.row < GetFixedRowCount())
		}else if (m_LeftClickDownCell.row < 1)
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

void CGridCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    CWnd::OnLButtonUp(nFlags, point);
    ClipCursor(NULL);

    if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
    {
        ReleaseCapture();
        KillTimer(m_nTimerID);
        m_nTimerID = 0;
    }

    // m_MouseMode == MOUSE_PREPARE_EDIT only if user clicked down on current cell
    // and then didn't move mouse before clicking up (releasing button)
    if (m_MouseMode == MOUSE_PREPARE_EDIT)    
    {
        OnEditCell(m_idCurrentCell.row, m_idCurrentCell.col, VK_LBUTTON);
    }
    // m_MouseMode == MOUSE_PREPARE_DRAG only if user clicked down on a selected cell
    // and then didn't move mouse before clicking up (releasing button)
    else if (m_MouseMode == MOUSE_PREPARE_DRAG) 
    {
        ResetSelectedRange();
    }
    else if (m_MouseMode == MOUSE_SIZING_COL)
    {
        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(m_LastMousePoint.x, rect.top, m_LastMousePoint.x + 2, rect.bottom);

        CDC* pDC = GetDC();
        if (pDC) {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        if (m_LeftClickDownPoint != point) 
        {   
            CPoint start;
            if (!GetCellOrigin(m_LeftClickDownCell, &start)) return;
            SetColumnWidth(m_LeftClickDownCell.col, point.x - start.x);
            ResetScrollBars();
            Invalidate();
        }
    }
    else if (m_MouseMode == MOUSE_SIZING_ROW)
    {
        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(rect.left, m_LastMousePoint.y, rect.right, m_LastMousePoint.y + 2);
    
        CDC* pDC = GetDC();
        if (pDC) {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }
    
        if (m_LeftClickDownPoint != point) 
        {
            CPoint start;
            if (!GetCellOrigin(m_LeftClickDownCell, &start)) return;

            SetRowHeight(m_LeftClickDownCell.row, point.y - start.y);
            ResetScrollBars();
            Invalidate();
        }
    } 

    m_MouseMode = MOUSE_NOTHING;
    SetCursor(::LoadCursor(NULL, IDC_ARROW));

    if (!IsValid(m_LeftClickDownCell)) return;

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        pOwner->PostMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED), 
                            (LPARAM) GetSafeHwnd());


}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl printing 

void CGridCtrl::Print() 
{
    CDC dc;
    CPrintDialog printDlg(FALSE);

    if (printDlg.DoModal() != IDOK)             // Get printer settings from user
        return;

    dc.Attach(printDlg.GetPrinterDC());         // attach a printer DC
    dc.m_bPrinting = TRUE;

    CString strTitle;
    strTitle.LoadString(AFX_IDS_APP_TITLE);

    DOCINFO di;                                 // Initialise print doc details
    ::ZeroMemory (&di, sizeof (DOCINFO));
    di.cbSize = sizeof (DOCINFO);
    di.lpszDocName = strTitle;

    BOOL bPrintingOK = dc.StartDoc(&di);        // Begin a new print job

     CPrintInfo Info;
    Info.m_rectDraw.SetRect(0,0, dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));

    OnBeginPrinting(&dc, &Info);                // Initialise printing
    for (UINT page = Info.GetMinPage(); page <= Info.GetMaxPage() && bPrintingOK; page++)
    {
        dc.StartPage();                         // begin new page
        Info.m_nCurPage = page;
        OnPrint(&dc, &Info);                    // Print page
        bPrintingOK = (dc.EndPage() > 0);       // end page
    }
    OnEndPrinting(&dc, &Info);                  // Clean up after printing

    if (bPrintingOK)
        dc.EndDoc();                            // end a print job
    else
        dc.AbortDoc();                          // abort job.

    dc.Detach();                                // detach the printer DC
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl printing overridables - for Doc/View print/print preview framework

void CGridCtrl::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo)
{
    // OnBeginPrinting() is called after the user has committed to
    // printing by OK'ing the Print dialog, and after the framework
    // has created a CDC object for the printer or the preview view.

    // This is the right opportunity to set up the page range.
    // Given the CDC object, we can determine how many rows will
    // fit on a page, so we can in turn determine how many printed
    // pages represent the entire document.

    ASSERT(pDC && pInfo);
    if (!pDC || !pInfo) return;

//  int nMaxRowCount = GetRowCount() - GetFixedRowCount();
    int nMaxRowCount =  m_nRealRow - GetFixedRowCount();
    if (!nMaxRowCount) return;

    // Get a DC for the current window (will be a screen DC for print previewing)
    CDC *pCurrentDC = GetDC();        // will have dimensions of the client area
    if (!pCurrentDC) return;

    CSize PaperPixelsPerInch(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
    CSize ScreenPixelsPerInch(pCurrentDC->GetDeviceCaps(LOGPIXELSX), pCurrentDC->GetDeviceCaps(LOGPIXELSY));

    // Create the printer font
    int nFontSize = -9;
    CString strFontName = "Times New Roman";
    m_PrinterFont.CreateFont(nFontSize, 0,0,0, FW_NORMAL, 0,0,0, DEFAULT_CHARSET,
                             OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_DONTCARE, strFontName);
            
    CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);

    // Get the average character width (in GridCtrl units) and hence the margins
    m_CharSize = pDC->GetTextExtent(_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSATUVWXYZ"),52);
    m_CharSize.cx /= 52;
    int nMargins = (LEFT_MARGIN+RIGHT_MARGIN)*m_CharSize.cx;

    // Get the page sizes (physical and logical)
    m_PaperSize = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
    m_LogicalPageSize.cx = GetVirtualWidth()+nMargins;
    m_LogicalPageSize.cy = MulDiv(m_LogicalPageSize.cx, m_PaperSize.cy, m_PaperSize.cx);

    m_nPageHeight = m_LogicalPageSize.cy - GetFixedRowHeight()
                       - (HEADER_HEIGHT+FOOTER_HEIGHT + 2*GAP)*m_CharSize.cy;

    // Get the number of pages. Assumes no row is bigger than the page size.
    int nTotalRowHeight = 0;
    int nNumPages = 1;
//    for (int row = GetFixedRowCount(); row < GetRowCount(); row++)

    for (int row = GetFixedRowCount(); row < m_nRealRow; row++)
    {
        nTotalRowHeight += GetRowHeight(row);
        if (nTotalRowHeight > m_nPageHeight) {
            nNumPages++;
            nTotalRowHeight = GetRowHeight(row);
        }
    }

    // Set up the print info
    pInfo->SetMaxPage(nNumPages);
    pInfo->m_nCurPage = 1;                        // start printing at page# 1

    ReleaseDC(pCurrentDC);
    pDC->SelectObject(pOldFont);
}

void CGridCtrl::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{
    if (!pDC || !pInfo) return;

    //CRect rcPage(pInfo->m_rectDraw);
    CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);

    // Set the page map mode to use GridCtrl units, and setup margin
    pDC->SetMapMode(MM_ANISOTROPIC);
    pDC->SetWindowExt(m_LogicalPageSize);
    pDC->SetViewportExt(m_PaperSize);
    pDC->SetWindowOrg(-LEFT_MARGIN*m_CharSize.cx, 0);

    // Header
    pInfo->m_rectDraw.top    = 0;
    pInfo->m_rectDraw.left   = 0;
    pInfo->m_rectDraw.right  = m_LogicalPageSize.cx - (LEFT_MARGIN+RIGHT_MARGIN)*m_CharSize.cx;
    pInfo->m_rectDraw.bottom = HEADER_HEIGHT*m_CharSize.cy;
    PrintHeader(pDC, pInfo);
    pDC->OffsetWindowOrg(0, -HEADER_HEIGHT*m_CharSize.cy);

    // Gap between header and column headings
    pDC->OffsetWindowOrg(0, -GAP*m_CharSize.cy);

    // Print the column headings
    pInfo->m_rectDraw.bottom = GetFixedRowHeight(); 
    PrintColumnHeadings(pDC, pInfo);
    pDC->OffsetWindowOrg(0, -GetFixedRowHeight()); 

    // We need to find out which row to start printing for this page.
    int nTotalRowHeight = 0;
    UINT nNumPages = 1;
    int nCurrPrintRow = GetFixedRowCount();

    while (nCurrPrintRow < GetRowCount() && nNumPages < pInfo->m_nCurPage)
    {
        nTotalRowHeight += GetRowHeight(nCurrPrintRow);
        if (nTotalRowHeight > m_nPageHeight) {
            nNumPages++;
            if (nNumPages == pInfo->m_nCurPage) break;
            nTotalRowHeight = GetRowHeight(nCurrPrintRow);
        }
        nCurrPrintRow++;
    }
    if (nCurrPrintRow >= GetRowCount()) return;

    // Draw as many rows as will fit on the printed page.
    // Clip the printed page so that there is no partially shown
    // row at the bottom of the page (the same row which will be fully
    // shown at the top of the next page).

    BOOL bFirstPrintedRow = TRUE;
    CRect rect;
    rect.bottom = -1;
    while (nCurrPrintRow < GetRowCount())
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(nCurrPrintRow) - 1;

        if (rect.bottom > m_nPageHeight) break;            // Gone past end of page

        rect.right = -1;
        for (int col = 0; col < GetColumnCount(); col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col) - 1;

            DrawCell(pDC, nCurrPrintRow, col, rect);


			if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ) 
            {
                int Overlap = (col == 0)? 0:1;
                pDC->MoveTo(rect.left-Overlap, rect.bottom);
                pDC->LineTo(rect.right, rect.bottom);
                if (nCurrPrintRow == 0) {
                    pDC->MoveTo(rect.left-Overlap, rect.top);
                    pDC->LineTo(rect.right, rect.top);
                }
            }
            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT) 
            {
                int Overlap = (bFirstPrintedRow)? 0:1;
                pDC->MoveTo(rect.right, rect.top-Overlap);
                pDC->LineTo(rect.right, rect.bottom);    
                if (col == 0) {
                    pDC->MoveTo(rect.left, rect.top-Overlap);
                    pDC->LineTo(rect.left, rect.bottom);    
                }
			}

        }
        nCurrPrintRow++;
        bFirstPrintedRow = FALSE;
    }

    // Footer
    pInfo->m_rectDraw.bottom = FOOTER_HEIGHT*m_CharSize.cy;
    pDC->SetWindowOrg(-LEFT_MARGIN*m_CharSize.cx, -m_LogicalPageSize.cy + FOOTER_HEIGHT*m_CharSize.cy);
    PrintFooter(pDC, pInfo);

    // SetWindowOrg back for next page
    pDC->SetWindowOrg(0,0);

    pDC->SelectObject(pOldFont);
}

void CGridCtrl::PrintColumnHeadings(CDC *pDC, CPrintInfo* /*pInfo*/)
{
    CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);

    CRect rect;
    rect.bottom = -1;
    for (int row = 0; row < GetFixedRowCount(); row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row) - 1;

        rect.right = -1;
        for (int col = 0; col < GetColumnCount(); col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col) - 1;

            DrawFixedCell(pDC, row, col, rect);

            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ) 
            {
                int Overlap = (col == 0)? 0:1;
                pDC->MoveTo(rect.left-Overlap, rect.bottom);
                pDC->LineTo(rect.right, rect.bottom);
                if (row == 0) {
                    pDC->MoveTo(rect.left-Overlap, rect.top);
                    pDC->LineTo(rect.right, rect.top);
                }
            }
            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT) 
            {
                int Overlap = (row == 0)? 0:1;
                pDC->MoveTo(rect.right, rect.top-Overlap);
                pDC->LineTo(rect.right, rect.bottom);    
                if (col == 0) {
                    pDC->MoveTo(rect.left, rect.top-Overlap);
                    pDC->LineTo(rect.left, rect.bottom);    
                }
            }

        }
    }

    pDC->SelectObject(pOldFont);
}

void CGridCtrl::PrintHeader(CDC *pDC, CPrintInfo *pInfo)
{
    CRect   rc(pInfo->m_rectDraw);
    CString strHeaderString;
    CFont   BoldFont;
    LOGFONT lf;

    //create bold font for header and footer
    VERIFY(m_PrinterFont.GetLogFont(&lf));
    lf.lfWeight = FW_BOLD;
    VERIFY(BoldFont.CreateFontIndirect(&lf));
 
    CFont *pNormalFont = pDC->SelectObject(&BoldFont);
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);

    // print App title on top right margin
    strHeaderString.LoadString(AFX_IDS_APP_TITLE);
    pDC->DrawText(strHeaderString, &rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);

    // print parent window title in the centre (Gert Rijs)
    CWnd *pParentWnd = GetParent();
    while (pParentWnd)
    {
        pParentWnd->GetWindowText(strHeaderString);
        if (strHeaderString.GetLength())  // can happen if it is a CView, CChildFrm has the title
            break;
        pParentWnd = pParentWnd->GetParent();
    }
    pDC->DrawText(strHeaderString, &rc, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);


    pDC->SetBkMode(nPrevBkMode);
    pDC->SelectObject(pNormalFont);
    BoldFont.DeleteObject();

    pDC->SelectStockObject(BLACK_PEN);
    pDC->MoveTo(rc.left, rc.bottom);
    pDC->LineTo(rc.right, rc.bottom);
}

//print footer with a line and date, and page number
void CGridCtrl::PrintFooter(CDC *pDC, CPrintInfo *pInfo)
{
    CRect rc(pInfo->m_rectDraw);
    CFont BoldFont;
    LOGFONT lf;

    //draw line
    pDC->MoveTo(rc.left, rc.top);
    pDC->LineTo(rc.right, rc.top);

    //create bold font for header and footer
    m_PrinterFont.GetLogFont(&lf);
    lf.lfWeight = FW_BOLD;
    BoldFont.CreateFontIndirect(&lf);

    CFont *pNormalFont = pDC->SelectObject(&BoldFont);
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);

    // draw page number
    CString   sTemp ;
    rc.OffsetRect(0, m_CharSize.cy/2);
    sTemp.Format(_T("Page %d of %d"), pInfo->m_nCurPage, pInfo->GetMaxPage());
    pDC->DrawText(sTemp,-1,rc, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

    CTime t = CTime::GetCurrentTime();
    sTemp = t.Format(_T("%c"));
    pDC->DrawText(sTemp,-1,rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

    pDC->SetBkMode(nPrevBkMode);
    pDC->SelectObject(pNormalFont);
    BoldFont.DeleteObject();
}

void CGridCtrl::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    m_PrinterFont.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl persistance

BOOL CGridCtrl::Save(LPCTSTR filename)
{
    CStdioFile File;
    CFileException ex;

    if (!File.Open(filename, CFile::modeWrite | CFile::modeCreate| CFile::typeText, &ex)) {
        ex.ReportError();
        return FALSE;
    }

    try {

        int nNumColumns = GetColumnCount();
		int i;
        for (i = 0; i < nNumColumns; i++) {
            File.WriteString(GetItemText(0,i));
            File.WriteString((i==(nNumColumns-1))? _T("\n"):_T(","));
        }

        for (i = 0; i < GetRowCount(); i++) {
            for (int j = 0; j < nNumColumns; j++) {
                File.WriteString(GetItemText(i,j));
                File.WriteString((j==(nNumColumns-1))? _T("\n"):_T(","));
            }
        }

        File.Close();
    }

    catch (CFileException* e) {
        AfxMessageBox(_T("Unable to save grid list"));
        e->Delete();
        return FALSE;
    }

    return TRUE;
}

BOOL CGridCtrl::Load(LPCTSTR filename)
{
    TCHAR *token, *end;
    TCHAR buffer[1024];
    CStdioFile File;
    CFileException ex;

    if (!File.Open(filename, CFile::modeRead | CFile::typeText)) {
        ex.ReportError();
        return FALSE;
    }

    DeleteAllItems();

    try {

        // Read Header off file
        File.ReadString(buffer, 1024);

        // Get first token
        for (token=buffer, end=buffer; 
             *end && (*end != _T(',')) && (*end != _T('\n')); end++);
        if ((*end == _T('\0')) && (token == end)) token = NULL;
        *end = _T('\0');

        while (token) 
        {
            InsertColumn(token);
                
            // Get next token
            for (token=++end; *end && (*end != _T(',')) && (*end != _T('\n'));
                 end++);
            if ((*end == _T('\0')) && (token == end)) token = NULL;
            *end = _T('\0');
        }

        // Read in rest of data
        int nItem = 0;
        while (File.ReadString(buffer, 1024)) {

            // Get first token
            for (token=buffer, end=buffer; 
                 *end && (*end != _T(',')) && (*end != _T('\n')); end++);
            if ((*end == _T('\0')) && (token == end)) token = NULL;
            *end = _T('\0');

            int nSubItem = 0;
            while (token) {

                if (!nSubItem)
                    InsertRow(token);
                else
                    SetItemText(nItem, nSubItem, token);

                // Get next token
                for (token=++end; *end && (*end != _T(',')) && (*end != _T('\n'));
                     end++);
                if ((*end == _T('\0')) && (token == end)) token = NULL;
                *end = _T('\0');

                nSubItem++;
            }
            nItem++;
        }

        AutoSizeColumns();
        File.Close();
    }

    catch (CFileException* e) {
        AfxMessageBox(_T("Unable to load grid data"));
        e->Delete();
        return FALSE;
    }

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl overrideables

// This is no longer needed since I've changed to OLE drag and drop - but it's
// still cool code. :)
CImageList* CGridCtrl::CreateDragImage(CPoint *pHotSpot)
{
    CDC* pDC = GetDC();
    if (!pDC) return NULL;

    CRect rect;
    CCellID cell = GetFocusCell();
    if (!GetCellRect(cell.row, cell.col, rect)) return NULL;

    // Translate coordinate system
    rect.BottomRight() = CPoint(rect.Width(), rect.Height());
    rect.TopLeft()     = CPoint(0,0);
    *pHotSpot = rect.BottomRight(); 

    // Create a new imagelist (the caller of this function has responsibility
    // for deleting this list)
    CImageList* pList = new CImageList;
    if (!pList || !pList->Create(rect.Width(), rect.Height(), ILC_MASK, 1,1))
    {    
        if (pList) delete pList;
        return NULL;
    }

    // Create mem DC and bitmap
    CDC MemDC;
    CBitmap bm;
    MemDC.CreateCompatibleDC(pDC);
    bm.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
    CBitmap* pOldBitmap = MemDC.SelectObject(&bm);
    MemDC.SetWindowOrg(0,0);

    // Draw cell onto bitmap in memDC
    DrawCell(&MemDC, cell.row, cell.col, rect, TRUE);

    // Clean up
    MemDC.SelectObject(pOldBitmap);
    ReleaseDC(pDC);

    // Add the bitmap we just drew to the image list.
    pList->Add(&bm, GetTextBkColor());
    bm.DeleteObject();

    return pList;
}

void CGridCtrl::OnFixedRowClick(CCellID& cell)
{
    if (!IsValid(cell)) 
        return;

    if (m_bListMode)
    {
        if (!m_bSortOnClick) 
            return;

        CWaitCursor waiter;
        if (cell.col == m_SortColumn)
            m_bAscending = !m_bAscending;
        else 
        {
            m_bAscending = FALSE;
            m_SortColumn = cell.col;
        }
        SortTextItems(m_SortColumn, m_bAscending);
/////////////////////////////////

/*	    for (int i=0;i<GetColumnCount();i++)
		{
			DWORD key;
		    CCellID cell(m_CurrSortCell.row ,i );
			key=MAKELONG(m_CurrSortCell.row ,i );
			m_PrevSelectedCellMap.SetAt(key, cell);
		}
 */       m_idCurrentCell=m_CurrSortCell  ;
/////////////////////////////////
        Invalidate();
		////////////Modi by Zhang//////////
		m_MouseMode = MOUSE_SELECT_COL;
        OnSelecting(cell);
		///////////////////////////////////
    } 
    else if (cell.col < GetFixedColumnCount()) 
    {
        m_MouseMode = MOUSE_SELECT_ALL;
        OnSelecting(cell);
    } 
    else 
    {
        m_MouseMode = MOUSE_SELECT_COL;
        OnSelecting(cell);
    }
}

void CGridCtrl::OnFixedColumnClick(CCellID& cell)
{
    if (!IsValid(cell))
        return;

//    if (m_bListMode && (GetItemState(cell.row, m_nFixedCols) & GVNI_SELECTED))
//    {
//        OnEditCell(cell.row, cell.col, VK_LBUTTON);
//        return;
//    }

    if (cell.row < GetFixedRowCount()) 
    {
        m_MouseMode = MOUSE_SELECT_ALL;
        OnSelecting(cell);
    }
    else 
    {
        m_MouseMode = MOUSE_SELECT_ROW;
        OnSelecting(cell);
    }
}

// Gets the extent of the text pointed to by str (no CDC needed)
// By default this uses the selected font (which is a bigger font)
CSize CGridCtrl::GetTextExtent(LPCTSTR str, BOOL bUseSelectedFont /* = TRUE */)
{
    CDC* pDC = GetDC();
    if (!pDC) return CSize(0,0);

    CFont *pOldFont, font;

    if (bUseSelectedFont)
    {
        LOGFONT lf;
        memcpy(&lf, &m_Logfont, sizeof(LOGFONT));
        lf.lfWeight = SELECTED_CELL_FONT_WEIGHT;
            
        font.CreateFontIndirect(&lf);
    
        pOldFont = pDC->SelectObject(&font);
    }
    else
        pOldFont = pDC->SelectObject(&m_Font);

    CSize size = pDC->GetTextExtent(str);
    pDC->SelectObject(pOldFont);
    ReleaseDC(pDC);

    return size + CSize(2*m_nMargin, 2*m_nMargin);
}

CSize CGridCtrl::GetCellExtent(int nRow, int nCol, CDC* pDC)
{
//    LOGFONT *pLF = GetItemFont(nRow, nCol);
    
    // use selected font since it's thicker   
    LOGFONT lf;
//    memcpy(&lf, pLF, sizeof(LOGFONT));
    memcpy(&lf, &m_Logfont, sizeof(LOGFONT));
    
    if (nRow < m_nFixedRows || nCol < m_nFixedCols)
        lf.lfWeight = SELECTED_CELL_FONT_WEIGHT;
            
    CFont font;
    font.CreateFontIndirect(&lf);
    
    CFont* pOldFont = pDC->SelectObject(&font);
    CSize size = pDC->GetTextExtent(GetItemText(nRow, nCol));
    pDC->SelectObject(pOldFont);

    size += CSize(4*m_nMargin, 1.5*m_nMargin);
    //size += CSize(2*m_nMargin, 1.5*m_nMargin);

    CSize ImageSize(0,0);
    if (m_pImageList) {
        int nImage = GetItemImage(nRow, nCol);
        if (nImage >= 0) {
            IMAGEINFO Info;
            if (m_pImageList->GetImageInfo(nImage, &Info))
                ImageSize = CSize(Info.rcImage.right-Info.rcImage.left+1, 
                                  Info.rcImage.bottom-Info.rcImage.top+1);
        }
    }
    
    return CSize(size.cx + ImageSize.cx, max(size.cy, ImageSize.cy));
}

BOOL CGridCtrl::DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
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
        //////////Modify By zhang/////////
		//pDC->FillSolidRect(rect, ::GetSysColor(COLOR_HIGHLIGHT));
        pDC->FillSolidRect(rect, m_crCursor);
		////////////
		rect.right--; rect.bottom--;
//        pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
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
//          darkpen(PS_SOLID,  1, ::GetSysColor(COLOR_3DDKSHADOW)),

         *pOldPen = pDC->GetCurrentPen();

    pDC->SetBkMode(TRANSPARENT);

    pDC->SelectObject(&darkpen);
    pDC->MoveTo(rect.right, rect.top);
    pDC->LineTo(rect.right, rect.bottom);
    pDC->LineTo(rect.left, rect.bottom);

    pDC->SelectObject(pOldPen);
    rect.DeflateRect(1,1);

    rect.DeflateRect(m_nMargin, 0);

	//draw the sort arrow
	if( nRow==0 && nCol>=GetFixedColumnCount() && nCol==m_SortColumn)
	{
		CRect ArrowRect; // add by Xia Qiankun for set sort arrow rect
	    ArrowRect.top =(rect.bottom+rect.top)/2-6;
        ArrowRect.bottom =(rect.bottom+rect.top)/2+4;
        ArrowRect.left =rect.right -11;
	    ArrowRect.right =rect.right-1;
        rect.right-=10;
        if(!m_bAscending)
		{
	        ArrowRect.top-=3;
		    ArrowRect.bottom-=3;
		}
		DrawText(pDC->m_hDC, Item.szText, -1, rect, Item.nFormat);
	    SetSortArrow(pDC,ArrowRect,m_bAscending);
	}
	else if (nRow == 0 && nCol == 0)
	{
		CRect ArrowRect; // add by Xia Qiankun for set sort arrow rect
	    ArrowRect.top =rect.top + 2;
        ArrowRect.bottom =rect.bottom - 2;
        ArrowRect.left =rect.right - 20;
	    ArrowRect.right =rect.right - 1;
        //rect.right-=10;
		DrawText(pDC->m_hDC, Item.szText, -1, rect, Item.nFormat);
		//只对动态显示牌画箭头
		//CMainFrame* pFrameWnd=(CMainFrame*)AfxGetMainWnd();
		//CView *pView = pFrameWnd->GetActiveView();
		CView *pView = (CView* )GetParent();
		if(pView != NULL && pView->IsKindOf(RUNTIME_CLASS(CTaiShanReportView)))
		{
			SetMenuArrow(pDC,ArrowRect,TextClr);
		}

	}
	else
	{
		DrawText(pDC->m_hDC, Item.szText, -1, rect, Item.nFormat);
	}

    pDC->RestoreDC(nSavedDC);
    return TRUE;
}

BOOL CGridCtrl::DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
{
    if (!m_bAllowDraw) return FALSE;
    int cc;
	pDC->SetBkMode(TRANSPARENT);
    GV_ITEM Item;
    Item.mask = GVIF_TEXT | GVIF_FORMAT | GVIF_STATE | GVIF_IMAGE | GVIF_BKCLR | GVIF_FGCLR;
    Item.row = nRow;
    Item.col = nCol;
    if (!GetItem(&Item)) return FALSE;


//    COLORREF TextBkClr = (Item.crBkClr == CLR_DEFAULT)? GetTextBkColor() : Item.crBkClr;
    COLORREF TextBkClr =  Item.crBkClr;
//    COLORREF TextClr   = (Item.crFgClr == CLR_DEFAULT)? GetTextColor()   : Item.crFgClr;
    COLORREF TextClr   = Item.crFgClr;

    // Force redraw of background if custom colour
    if (TextBkClr != CLR_DEFAULT)
        bEraseBk = TRUE;

    int nSavedDC = pDC->SaveDC();

    pDC->SetBkMode(TRANSPARENT);

    if (Item.state & GVIS_FOCUSED && !pDC->IsPrinting()) 
    {
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        if (bEraseBk) 
        {
            CBrush brush(TextBkClr);
            pDC->FillRect(rect, &brush);
        }
        rect.right--; rect.bottom--;    
        pDC->SelectStockObject(BLACK_PEN);
        pDC->SelectStockObject(NULL_BRUSH);
        pDC->Rectangle(rect);
        pDC->SetTextColor(TextClr);

        rect.DeflateRect(1,1);

    }
    else if (Item.state & GVIS_SELECTED && !pDC->IsPrinting()) 
    {
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        //////////Modify By zhang/////////
		//pDC->FillSolidRect(rect, ::GetSysColor(COLOR_HIGHLIGHT));
        pDC->FillSolidRect(rect, m_crCursor);
		////////////
		rect.right--; rect.bottom--;
//        pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->SetTextColor( TextClr );
    } else {
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        if (bEraseBk) 
        {
			COLORREF TextBkClr2 = TextBkClr;
	        pDC->FillSolidRect(rect, TextBkClr2);
//			CBrush brush(TextBkClr2);
  //          pDC->FillRect(rect, &brush);
        }
        rect.right--; rect.bottom--;
        pDC->SetTextColor(TextClr);
    }

    if (Item.state & GVIS_DROPHILITED && !pDC->IsPrinting()) {
        pDC->SelectStockObject(BLACK_PEN);
        pDC->SelectStockObject(NULL_BRUSH);
        pDC->Rectangle(rect);
    }

    // Create the appropriate font and select into DC
    CFont Font;
//    LOGFONT *pLF = GetItemFont(nRow, nCol);
//    if (pLF)
//        Font.CreateFontIndirect(&m_Logfont);
//        Font.CreateFontIndirect(pLF);
//    else
    Font.CreateFontIndirect(&m_Logfont);

    pDC->SelectObject(&Font);
    rect.DeflateRect(m_nMargin, 0);

    if (m_pImageList && Item.iImage >= 0) {
        IMAGEINFO Info;
        if (m_pImageList->GetImageInfo(Item.iImage, &Info)) {
            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
            m_pImageList->Draw(pDC, Item.iImage, rect.TopLeft(), ILD_NORMAL);
            rect.left += nImageWidth+m_nMargin;
        }
    }

    DrawText(pDC->m_hDC, Item.szText, -1, rect, Item.nFormat);

    pDC->RestoreDC(nSavedDC);
    return TRUE;
}

void CGridCtrl::OnEditCell(int nRow, int nCol, UINT nChar)
{
    EnsureVisible(nRow, nCol);

    CCellID cell(nRow, nCol);
    if (!IsValid(cell) || !IsCellEditable(nRow, nCol) || !IsCellVisible(nRow, nCol)) 
        return;

    CRect rect;
    if (!GetCellRect(cell, rect)) return;

    SendMessageToParent(nRow, nCol, GVN_BEGINLABELEDIT);

    GV_ITEM Item;
    Item.mask = GVIF_TEXT | GVIF_FORMAT;
    Item.row = nRow;
    Item.col = nCol;
    if (!GetItem(&Item)) return;

    DWORD dwStyle = ES_LEFT;
    if (Item.nFormat & DT_RIGHT) dwStyle = ES_RIGHT;
    else if (Item.nFormat & DT_CENTER) dwStyle = ES_CENTER;

    CreateInPlaceEditControl(rect, dwStyle, IDC_INPLACE_EDIT, 
                             nRow, nCol, Item.szText, nChar);
}

void CGridCtrl::CreateInPlaceEditControl(CRect& rect, DWORD dwStyle, UINT nID,
                                         int nRow, int nCol,
                                         LPCTSTR szText, int nChar)
{
    // InPlaceEdit auto-deletes itself
    new CInPlaceEdit(this, rect, dwStyle, nID, nRow, nCol, szText, nChar);
}

void CGridCtrl::OnEndEditCell(int nRow, int nCol, CString str)
{
    SetItemText(nRow, nCol, str);
}

CString CGridCtrl::GetItemText(int nRow, int nCol)
{
    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return "";

    CGridCell* pCell=NULL ;
	pCell=new CGridCell; 
    ASSERT(pCell);
    if (!pCell) return "";
	GetCell(pCell,nRow, nCol);
    CString szText=pCell->szText;
	delete pCell;
    return szText;
}

int CGridCtrl::DrawGridLine(CDC *pDC,COLORREF m_color,int nFixedColWidth,int minVisibleRow,
							int maxVisibleRow,int nFixedRowHeight,CRect VisRect,int minVisibleCol,int maxVisibleCol)
{
    //CDC* pDC = GetDC();
	int row,col;
	CPen pen;
	try {
        //pen.CreatePen(PS_SOLID, 0, m_crGridColour);
        pen.CreatePen(PS_SOLID, 0, m_color);
	}
    catch (CResourceException *e)
    {
        e->Delete();
        return 1;
    }
    pDC->SelectObject(&pen);

    
    if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT) 
    {
        int x = nFixedColWidth;
        for (col = minVisibleCol; col <= maxVisibleCol; col++) {
            x += GetColumnWidth(col);
            pDC->MoveTo(x-1, nFixedRowHeight);
            pDC->LineTo(x-1, VisRect.bottom);   
        }
    } 
    
    // draw horizontal lines (drawn at bottom of each cell)
    
	if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ) 
    {
        int y = nFixedRowHeight;
        for (row = minVisibleRow; row <= maxVisibleRow; row++) {
            y += GetRowHeight(row);
            pDC->MoveTo(nFixedColWidth, y-1);    
            pDC->LineTo(VisRect.right,  y-1);
        }
    }  

    pDC->SelectStockObject(NULL_PEN);
	return 0;
}

void CGridCtrl::StockDataDisplay()
{
	CPaintDC dc(this);      // device context for painting
    if (m_bDoubleBuffer)    // Use a memory DC to remove flicker
    {
        CMemDC MemDC(&dc);
        OnDrawData(&MemDC);
    }
    else                    // Draw raw - this helps in debugging vis problems.
        OnDraw(&dc);
}
void CGridCtrl::OnDrawData(CDC *pDC)
{
	    CRect rect;
    int row,col;

    CRect clipRect;
    GetClientRect(&clipRect);


    int nFixedRowHeight = GetFixedRowHeight();
    int nFixedColWidth  = GetFixedColumnWidth();

    CCellID idTopLeft = GetTopleftNonFixedCell();
    int minVisibleRow = idTopLeft.row,
        minVisibleCol = idTopLeft.col;

    CRect VisRect;
    CCellRange VisCellRange = GetVisibleNonFixedCellRange(VisRect);
    int maxVisibleRow = VisCellRange.GetMaxRow(),
        maxVisibleCol = VisCellRange.GetMaxCol();

  
    // draw rest of non-fixed cells
    rect.bottom = nFixedRowHeight-1;
    for (row = minVisibleRow; row <= maxVisibleRow; row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom) break;                // Gone past cliprect
        if (rect.bottom < clipRect.top) continue;             // Reached cliprect yet?

        rect.right = nFixedColWidth-1;
        for (col = minVisibleCol; col <= maxVisibleCol; col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColumnWidth(col)-1;

            if (rect.left > clipRect.right) break;        // gone past cliprect
            if (rect.right < clipRect.left) continue;     // Reached cliprect yet?

            DrawCell(pDC, row, col, rect);
        }
    }
	InvalidateRect(clipRect,TRUE);
#ifdef USE_MEMDC                        // Use a memDC for flicker free update
}
#else                                   // Use normal DC - this helps in debugging
}
#endif

BOOL CGridCtrl::DrawCellData(CDC* pDC, int nRow, int nCol, CRect rect)
{
    if (!m_bAllowDraw) return FALSE;

    GV_ITEM Item;
    Item.mask = GVIF_TEXT | GVIF_FORMAT | GVIF_STATE | GVIF_IMAGE | GVIF_BKCLR | GVIF_FGCLR;
    Item.row = nRow;
    Item.col = nCol;
    if (!GetItem(&Item)) return FALSE;

    COLORREF TextBkClr = (Item.crBkClr == CLR_DEFAULT)? GetTextBkColor() : Item.crBkClr;
    COLORREF TextClr   = (Item.crFgClr == CLR_DEFAULT)? GetTextColor()   : Item.crFgClr;


    int nSavedDC = pDC->SaveDC();

    pDC->SetBkMode(TRANSPARENT);

    if (Item.state & GVIS_SELECTED && !pDC->IsPrinting()) 
    {
        //pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->SetTextColor( TextClr );
    } else
	{
        pDC->SetTextColor(TextClr);
    }

    // Create the appropriate font and select into DC
    CFont Font;
    LOGFONT *pLF = GetItemFont(nRow, nCol);
    if (pLF)
        Font.CreateFontIndirect(pLF);
    else
        Font.CreateFontIndirect(&m_Logfont);

    pDC->SelectObject(&Font);
    rect.DeflateRect(m_nMargin, 0);

    DrawText(pDC->m_hDC, Item.szText, -1, rect, Item.nFormat);
    pDC->RestoreDC(nSavedDC);
    return TRUE;
}
UINT CGridCtrl::GetTopleftNonFixedRow() const
{
/////////////moid by zjh/////////////
    //int nVertScroll = GetScrollPos(SB_VERT), 
      //  nHorzScroll = GetScrollPos(SB_HORZ);
    int nVertScroll = GetScrollPos(SB_VERT);

    int nRow;
	int nTop = 0;
	nRow=m_nFixedRows;
    while (nTop < nVertScroll && nRow < (GetRowCount()-1))
        nTop += GetRowHeight(nRow++);

    //TRACE("TopLeft cell is row %d, col %d\n",nRow, nColumn);
    return nRow;
}
// redraw a complete row
BOOL CGridCtrl::RedrawNoFixedRows(int row)
{
    BOOL bResult = TRUE;

    CDC* pDC = GetDC();

    for (int col = m_nFixedCols  ; col < GetColumnCount(); col++)
        bResult = RedrawCell(row, col, pDC) && bResult;

    if (pDC) ReleaseDC(pDC);

    return bResult;
}

//add by Xia Qiankun 
//this function connect ccgrid with the view's set of really rows
void CGridCtrl::SetRealRow(int nRows)
{
    m_nRealRow=nRows + m_nFixedRows - 1;
}

void CGridCtrl::SetMenuArrow(CDC *pDC, CRect rect,COLORREF forecolor)
{
	CPen *poldpen;
	CPen pen_white(PS_SOLID,1,RGB(255,255,255));  
	CPen pen_black(PS_SOLID,1,RGB(0,0,0));    
	CBrush frBrush(forecolor);
	CRgn rgn;
	int nBackMode;
	nBackMode=pDC->GetBkMode();

	if(nBackMode==TRANSPARENT) pDC->SetBkMode(OPAQUE);

	CPoint points[3];
	points[0] = CPoint(rect.left + 5,rect.top + 4);
	points[1] = CPoint(rect.right - 5,rect.top + 4);
	points[2] = CPoint(rect.left + rect.Width()/2,rect.bottom - 4);
	rgn.CreatePolygonRgn(points,3,WINDING);


	//poldpen = pDC->SelectObject(&pen_white);
	//if(UpDown)
	{
		pDC->FrameRect(rect,&frBrush);
		pDC->FillRgn(&rgn,&frBrush);
	}

	//pDC->SelectObject(poldpen);
	if(nBackMode==TRANSPARENT) pDC->SetBkMode(TRANSPARENT);
} 

//add by Xia Qiankun
//this function set the sort arrow
void CGridCtrl::SetSortArrow(CDC *pDC, CRect rect, BOOL UpDown)
{
     CPen *poldpen;
	 CPen pen_white(PS_SOLID,1,RGB(255,255,255));  
	 CPen pen_black(PS_SOLID,1,RGB(0,0,0));     
	 int nBackMode;
     nBackMode=pDC->GetBkMode();
     if(nBackMode==TRANSPARENT) pDC->SetBkMode(OPAQUE);

	 poldpen = pDC->SelectObject(&pen_white);
	 if(UpDown)
	 {
		 pDC->MoveTo(rect.left,rect.bottom);
		 pDC->LineTo(rect.right,rect.bottom);
		 pDC->MoveTo(rect.right,rect.bottom);
		 pDC->LineTo((rect.left+rect.right)/2,rect.top);
	 }
	 else
	 {
		 pDC->MoveTo(rect.right,rect.top+4);
		 pDC->LineTo((rect.left+rect.right)/2,rect.bottom+4);
	 }
	 pDC->SelectObject(&pen_black);
     if(UpDown)
	 {
		 pDC->MoveTo(rect.left,rect.bottom);
		 pDC->LineTo((rect.left+rect.right)/2,rect.top);
	 }
	 else
	 {
		 pDC->MoveTo(rect.left,rect.top+4);
		 pDC->LineTo(rect.right,rect.top+4);
		 pDC->MoveTo(rect.left,rect.top+4);
		 pDC->LineTo((rect.left+rect.right)/2,rect.bottom+4);
	 }	 
	 pDC->SelectObject(poldpen);
	 if(nBackMode==TRANSPARENT) pDC->SetBkMode(TRANSPARENT);
} 
void CGridCtrl::AddFirstRow()
{
//19991106
/*	GRID_ROW *pRow;
	pRow=m_RowData.GetAt(m_nRows - 1);
    m_RowData.RemoveAt( m_nRows - 1 );
    m_RowData.InsertAt( 1, pRow  );
*/   
	CRect rect;
	GetClientRect(&rect);
    CCellID idTopLeft = GetTopleftNonFixedCell();
    int yScroll = GetRowHeight(idTopLeft.row);
    rect.top = GetFixedRowHeight();
    ScrollWindow(0, yScroll, rect);
    rect.bottom = rect.top + yScroll;
    InvalidateRect(rect);
}
int CGridCtrl::GotoLine(int RowNum,BOOL IsSamePage)
{
    int MaxRowNum = m_nRows;
	CCellID idTopLeft = GetTopleftNonFixedCell();

    CCellRange VisibleCells = GetVisibleNonFixedCellRange();

	if(RowNum>= VisibleCells.GetMinRow() &&RowNum<=  VisibleCells.GetMaxRow()&&IsSamePage )
	{
//	  SetSelectedRange(RowNum,1,RowNum,GetColumnCount()-1,TRUE);
      SetFocusRow( RowNum);
	  return 1;
	}
	int scrollPos = GetScrollPos32(SB_VERT);
	//int scrollPos = this->m_scrollVPos;// GetScrollPos32(SB_VERT);
	CRect rect;
    GetClientRect(rect);

    int yScroll = (RowNum - idTopLeft.row - 1)*GetRowHeight(idTopLeft.row);
    SetScrollPos32(SB_VERT, scrollPos + yScroll);

    rect.top = GetFixedRowHeight() + yScroll;
    ScrollWindow( 0, -yScroll, rect);
    rect.top = rect.bottom - yScroll;
    InvalidateRect(rect);
	
//	SetSelectedRange(RowNum,1,RowNum,GetColumnCount()-1,TRUE);
    SetFocusRow( RowNum);

	return 1;
}
void  CGridCtrl::SetFocusRow(int RowNum)
{
    HWND hOldFocusWnd = ::GetFocus();
    int Flags=1;
    CCellID m_TopLeftCell=GetTopleftNonFixedCell();
    m_LeftClickDownCell.row = RowNum;
    m_LeftClickDownCell.col=m_TopLeftCell.col ;
    if (!IsValid(m_LeftClickDownCell)) return;
    
    m_SelectionStartCell = (Flags & MK_SHIFT)? m_idCurrentCell : m_LeftClickDownCell;

    SetFocus();        // Auto-destroy any InPlaceEdit's

    SetFocusCell(-1,-1);
    SetFocusCell(max(m_LeftClickDownCell.row, m_nFixedRows),
                     max(m_LeftClickDownCell.col, m_nFixedCols));

    // If the user clicks on a selected cell, then prepare to drag it.
    // (If the user moves the mouse, then dragging occurs)



    if (m_MouseMode == MOUSE_OVER_COL_DIVIDE) // sizing column
    {
    }
    else if (m_MouseMode == MOUSE_OVER_ROW_DIVIDE) // sizing row
    {
    }
    else // not sizing or editing -- selecting
    {    
        // If Ctrl pressed, save the current cell selection. This will get added
        // to the new cell selection at the end of the cell selection process
/*		if(m_LeftClickDownCell.row >= GetFixedRowCount())
		{   
            if(m_PrevSelectedCellMap.GetCount()!=GetRowCount() -1)
			{
                m_PrevSelectedCellMap.RemoveAll();
				if(m_SortColumn!=-1)
				for (int i=1;i<GetRowCount();i++)
				{
					DWORD key;
					CCellID cell(i,m_SortColumn );
					key=MAKELONG(i, m_SortColumn );
					m_PrevSelectedCellMap.SetAt(key, cell);
				}
			}
		}
         else
		 {
			 if(m_PrevSelectedCellMap.GetCount()!=GetColumnCount() -1)
			 {

                m_PrevSelectedCellMap.RemoveAll();
				for (int i=1;i<GetColumnCount();i++)
				{
					DWORD key;
					int CurrRow=GetCurrentRow();
					CCellID cell(GetCurrentRow() ,i );
					key=MAKELONG(GetCurrentRow() ,i );
					m_PrevSelectedCellMap.SetAt(key, cell);
				}
			 }
		 }
*/     
        if (m_LeftClickDownCell.row < GetFixedRowCount())
			//there, will sort 
            OnFixedRowClick(m_LeftClickDownCell);
        else if (m_LeftClickDownCell.col < GetFixedColumnCount())
            OnFixedColumnClick(m_LeftClickDownCell);
        else
        {
            m_MouseMode = m_bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
			m_CurrrSortClick=FALSE;
            OnSelecting(m_LeftClickDownCell);
        }
    }   
	m_idCurrentCell.row=RowNum;
	m_CurrSortCell=m_idCurrentCell ;
}
void CGridCtrl::SetMouseState(int nstate)
{
	switch(nstate)
	{
		case MOUSE_ALL:  
			 m_MouseMode=MOUSE_SELECT_ALL;
			 break;
		case MOUSE_COL:  
			 m_MouseMode=MOUSE_SELECT_COL;
			 break;
		case MOUSE_ROW:  
			 m_MouseMode=MOUSE_SELECT_ROW;
			 break;
		case MOUSE_CELLS:
			 m_MouseMode=MOUSE_SELECT_CELLS;
		case MOUSE_NOTHINGS:
             m_MouseMode=MOUSE_NOTHING;
			 break;
	}
}
long CGridCtrl::GetCurrentRow()
{
	return m_idCurrentCell.row;
}
void CGridCtrl::OnSortForOnTimer(int SortColumn,BOOL Ascend)
{
	    CCellID cell(m_CurrSortCell.row,SortColumn);
//		m_PrevSelectedCellMap.RemoveAll();
        SortTextItems(SortColumn, Ascend);
/////////////////////////////////
      m_idCurrentCell=m_CurrSortCell  ;
/////////////////////////////////
        Invalidate();
		////////////Modi by Zhang//////////
		m_MouseMode = MOUSE_SELECT_COL;
        m_SelectionStartCell=cell;
        OnSelecting(cell);
		m_MouseMode = MOUSE_NOTHING;
		///////////////////////////////////
}
void CGridCtrl::SetGridColor(COLORREF BkColor,COLORREF LineColor,COLORREF CursorColor)
{
   m_crCursor=CursorColor;
   m_GridLineColor=LineColor;	   
   m_crTextBkColour=BkColor;
}
void CGridCtrl::SetHotKeySelect(int SortColumn)
{
	CCellID cell(0,SortColumn);
/*	m_PrevSelectedCellMap.RemoveAll();
    for (int i=0;i<GetColumnCount();i++)
	{
		DWORD key;
	    CCellID cell(1 ,i );
		key=MAKELONG(1 ,i );
		m_PrevSelectedCellMap.SetAt(key, cell);
	}
*/    m_idCurrentCell.row=1  ;
    m_idCurrentCell.col=SortColumn;
	m_MouseMode = MOUSE_SELECT_COL;
    m_SelectionStartCell=cell;
     m_CurrSortCell=m_idCurrentCell;
    OnSelecting(cell);
	m_MouseMode = MOUSE_NOTHING;
}
BOOL CGridCtrl::SortTextItemsForId(int nCol, BOOL bAscending, int low, int high)
{
    if (nCol >= GetColumnCount()) return FALSE;

    if (high == -1) high =m_nRealRow-1;// GetRowCount() - 1;

    int lo = low;
    int hi = high;

    if( hi <= lo ) return FALSE;

    CString niMid = GetItemText( (lo+hi)/2, nCol );
   // turn each string into an double value,the compare it
//	double niMid=atof(midItem);
    CString nistr;
    // loop through the list until indices cross
    while( lo <= hi )
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        if( bAscending )
		{
            nistr=GetItemText(lo, nCol);
            while (lo < high && nistr<niMid)
			{
 				++lo;
  				nistr=GetItemText(lo, nCol);
			}
		}
        else
		{
            nistr=GetItemText(lo, nCol);
            while ((lo < high  && nistr>niMid))
			{
 				++lo;
				nistr=GetItemText(lo, nCol);
			}
		
		}

        // Find an element that is smaller than or equal to  the partition 
        // element starting from the right Index.
        if( bAscending )
		{
 			nistr=GetItemText(hi, nCol);
			while (hi > low && nistr>niMid)
			{
 				--hi;
   				nistr=GetItemText(hi, nCol);
			}
		}
        else
		{
			nistr=GetItemText(hi,nCol);
            while (hi > low && nistr<niMid)
			{
                --hi;
				nistr=GetItemText(hi, nCol);
			}
		}
        // If the indexes have not crossed, swap if the items are not equal
        if (lo <= hi)
        {
            // swap only if the items are not equal
            if (GetItemText(lo, nCol) != GetItemText(hi, nCol))
            {
				CReportData nCdat;
				memcpy(&nCdat,m_pDoc->m_pStockDataShow[lo -m_nFixedRows].pItem,sizeof(CReportData));
				memcpy(m_pDoc->m_pStockDataShow[lo - m_nFixedRows].pItem,m_pDoc->m_pStockDataShow[hi - m_nFixedRows].pItem,sizeof(CReportData));
				memcpy(m_pDoc->m_pStockDataShow[hi - m_nFixedRows].pItem,&nCdat,sizeof(CReportData));
            }

            ++lo;
            --hi;
        }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if( low < hi )
        SortTextItemsForId(nCol, bAscending, low, hi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( lo < high )
        SortTextItemsForId(nCol, bAscending, lo, high);

    return TRUE;
}
BOOL CGridCtrl::SortTextItemsForId(int nCol, BOOL bAscending)
{
//add by xia Qiankun
// if clicked the fixed column, do nothing
    ResetSelectedRange();
    SetFocusCell(-1,-1);
    return SortTextItemsForId(nCol, bAscending, GetFixedRowCount(),-1);
}

BOOL CGridCtrl::GetCell(CGridCell* pGridCell,int nRow, int nCol) const
{
//    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return FALSE;
    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return FALSE;

    if(nRow==0)//默认是一行固定
	{
        pGridCell->state=0;       // Cell state (selected/focus etc)
		pGridCell->nFormat =DT_CENTER|DT_VCENTER|DT_SINGLELINE;     // Cell format
		pGridCell->szText=m_pColData[nCol].szText;      // Cell text (or binary data if you wish...)
		pGridCell->iImage=-1;      // Index of the list view item抯 icon 
	    pGridCell->crBkClr=m_clrBackGround;//m_pDoc->m_colorArray[18];  // Background colour (or CLR_DEFAULT)
		
		pGridCell->crFgClr=m_pDoc->m_colorArray[21];     // Forground colour (or CLR_DEFAULT)
		//pGridCell->crFgClr=m_pDoc->m_colorArray[1];     // Forground colour (or CLR_DEFAULT)
		pGridCell->lParam=0;      // 32-bit value to associate with item
		pGridCell->lfFont=m_Logfont;     // Cell font
		return TRUE;
	}
    if(nRow > 0 && nRow < m_nFixedRows)//默认是一行固定
	{
		DWORD key = MAKELONG(nRow, nCol);

		if(nRow==1 && nCol==0)
			nCol=0;

		CCellID cell;
		//    if((m_SortColumn==nCol||m_idCurrentCell.row==nRow)&&nCol!=0)
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

		pGridCell->crBkClr=m_clrBackGround;//m_pDoc->m_colorArray[18];//CLR_DEFAULT;  // Background colour (or CLR_DEFAULT)
		pGridCell->nFormat=m_pColData[nCol].nFormat;

		if(nRow >= m_nRealRow)
		{
			pGridCell->szText="";
			return TRUE;
		}
		char str[50];
		float m_temp;
		if(m_pDoc==NULL)		
		{
			return TRUE;
		}

		CReportData *pData = NULL;

		//if (m_pDoc->m_bStatistic[nRow-1])
		//{
			pData = m_pDoc->m_pStatisticData[m_nFixedRowIndex[nRow-1]].pItem;
		//}

		if(pData==NULL )
			return FALSE;

		switch(m_pColData[nCol].nID)
		{
		case ID_ROWNUM:
			sprintf(str,"%d",nRow);
			pGridCell->szText = "";
			pGridCell->crFgClr = RGB(255 - GetRValue(m_pDoc->m_colorArray[18]),255 - GetGValue(m_pDoc->m_colorArray[18]),255 - GetBValue(m_pDoc->m_colorArray[18])); 
			//pGridCell->crFgClr = m_pDoc->m_colorArray[1]; 
			break;
		case ID_STOCKID:                                        //代码
			sprintf(str,"%s",pData->id);
			pGridCell->szText ="";
			pGridCell->crFgClr =m_pDoc->m_colorArray[21]; 
			//pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_STOCKNAME:                                      //名称
			sprintf(str,"%s",pData->name );
			pGridCell->crFgClr =m_pDoc->m_colorArray[20]; 
			//pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			pGridCell->szText =str;
			break;
		case ID_YSTC: 				//昨收
			if(pData->kind==SHBG)
				sprintf(str,"%6.3f",pData->ystc  );
			else
				sprintf(str,"%6.2f",pData->ystc  );
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_OPEN:                                //今开
			if(pData->kind==SHBG)
				sprintf(str,"%6.3f",pData->opnp  );
			else
				sprintf(str,"%6.2f",pData->opnp );
			pGridCell->szText =str;
			if(pData->opnp > pData->ystc )
				pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			else if(pData->opnp < pData->ystc)
				pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
			else
				pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_HIGH:                                //最高
			if(pData->kind==SHBG)
				sprintf(str,"%6.3f",pData->higp  );
			else
				sprintf(str,"%6.2f",pData->higp );
			pGridCell->szText =str;
			if(pData->higp > pData->ystc )
				pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			else if(pData->higp < pData->ystc)
				pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
			else
				pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_LOW:                                 //最低
			if(pData->kind==SHBG)
				sprintf(str,"%6.3f",pData->lowp  );
			else
				sprintf(str,"%6.2f",pData->lowp  );
			pGridCell->szText =str;
			if(pData->lowp > pData->ystc )
				pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			else if(pData->lowp < pData->ystc)
				pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
			else
				pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_NEW:                                 //最新
			if(pData->kind==SHBG)
				sprintf(str,"%6.3f",pData->nowp  );
			else
				sprintf(str,"%6.2f",pData->nowp   );
			if(pData->nowp > pData->ystc )
				pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			else if(pData->nowp < pData->ystc)
				pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
			else
				pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			pGridCell->szText =str;
			break;
		case ID_TOTV:                                //总手
			sprintf(str,"%6.0f",pData->totv   );
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_NOWV:                                //现手
			sprintf(str,"%4.0f",pData->nowv   );
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_TOTP:                                //总额
			if(pData->kind==SHBG)
				sprintf(str,"%6.3f",pData->totp /10000  );
			else
				sprintf(str,"%6.2f",pData->totp  /10000 );
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_AVGP:                                //均价
			if(pData->totv!=0)
				if(pData->kind==SHBG)
					sprintf(str,"%6.3f",pData->totp /pData->totv /100  );
				else
					sprintf(str,"%6.2f",pData->totp /pData->totv /100 );
			else
				sprintf(str,"%6.2f",0.0f  );
			pGridCell->szText =str;
			if(pData->totv!=0)
			{
				if(pData->totp /pData->totv /100 > pData->ystc )
					pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
				else if(pData->totp /pData->totv /100 < pData->ystc)
					pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
				else
					pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			}
			else
				pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 

			break;
		case ID_ADDE:                                //涨跌
			if(pData->ystc !=0&&pData->nowp!=0)
			{
				if(pData->kind==SHBG)
					sprintf(str,"%6.3f",pData->nowp - pData->ystc  );
				else
					sprintf(str,"%6.2f",pData->nowp - pData->ystc  );
			}
			else
				sprintf(str,"%6.2f",0.0f);

			pGridCell->szText =str;
			if(pData->nowp > pData->ystc )
				pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			else if(pData->nowp < pData->ystc)
				pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
			else
				pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_HLP:                                 //震幅
			if(pData->ystc !=0)
			{
				sprintf(str,"%6.2f%s",100 * (pData->higp - pData->lowp) /pData->ystc ,"%" );
			}
			else
				sprintf(str,"%6.2f%s",0.0f,"%" ); 
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_RDR:                                 //涨幅
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
			break;
		case ID_BSSPBAS:                            //委比
			{
				float temp=0;
				if (pData->accb+pData->accs>0)
					if(pData->accs==0&&pData->accb!=0)
						temp=100;
					else if(pData->accs!=0&&pData->accb==0)
						temp=-100;
					else
						temp=(float)(pData->accb-pData->accs)/(float)(pData->accb+pData->accs)*100.0;     //计算委比
				else
					temp=0;
				sprintf(str,"%6.2f%s",temp ,"%"  );
				pGridCell->szText =str;
				pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			}
			break;
		case ID_DIFBS:                              //委差
			sprintf(str,"%6.0f",pData->accb - pData->accs   );
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_VOLBI:                              //量比
			if(m_pDoc->m_nANT[0] !=0 && pData->volume5!=0 )
			{
				sprintf(str,"%6.2f%s",(pData->totv / m_pDoc->m_nANT[0]) / (pData->volume5 /240 ),"%" );
			}
			else
				sprintf(str,"%6.2f",0.0f   );

			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_ACCB:                               //委买
			sprintf(str,"%6.0f",pData->accb  );
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_ACCS:                               //委卖
			sprintf(str,"%6.0f",pData->accs  );
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_BUY:                                //委买价
			if(pData->kind==SHBG)
				sprintf(str,"%6.3f",pData->pbuy1  );
			else
				sprintf(str,"%6.2f",pData->pbuy1   );
			pGridCell->szText =str;
			if(pData->pbuy1 > pData->ystc )
				pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			else if(pData->pbuy1 < pData->ystc)
				pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
			else
				pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_SEL:                                //委卖价
			if(pData->kind==SHBG)
				sprintf(str,"%6.3f",pData->psel1  );
			else
				sprintf(str,"%6.2f",pData->psel1   );
			pGridCell->szText =str;
			if(pData->psel1 > pData->ystc )
				pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			else if(pData->psel1 < pData->ystc)
				pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
			else
				pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_RVOL:                               //内盘
			sprintf(str,"%6.0f",pData->dvol  );
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
			break;
		case ID_DVOL:                              //外盘
			sprintf(str,"%6.0f",pData->rvol   );
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			break;

		case ID_EXCHANGE:
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				if(pData->kind==SHBG|| pData->kind==SZBG)
					if(pData->pBaseInfo->Bg!=0) 
						sprintf(str,"%6.2f",pData->totv/pData->pBaseInfo->Bg);
					else
						sprintf(str,"%6.2f",0.0f);
				else
					if(pData->pBaseInfo->ltAg!=0)
						sprintf(str,"%6.2f",pData->totv/pData->pBaseInfo->ltAg);
					else
						sprintf(str,"%6.2f",0.0f);
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_SYL:
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				if(pData->pBaseInfo->mgsy!=0)
					sprintf(str,"%6.3f",pData->nowp/pData->pBaseInfo->mgsy); 
				else
					sprintf(str,"%6.2f",0.0f);
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;

		case ID_BUYP1:
			SetPriceString(pGridCell, pData, pData->pbuy1);
			break;
		case ID_BUYP2:
			SetPriceString(pGridCell, pData, pData->pbuy2);
			break;
		case ID_BUYP3:
			SetPriceString(pGridCell, pData, pData->pbuy3);
			break;
			//		case ID_BUYP4:
			//			SetPriceString(pGridCell, pData, pData->pbuy4);
			//			break;
		case ID_SELP1:
			SetPriceString(pGridCell, pData, pData->psel1);
			break;
		case ID_SELP2:
			SetPriceString(pGridCell, pData, pData->psel2);
			break;
		case ID_SELP3:
			SetPriceString(pGridCell, pData, pData->psel3);
			break;
			/*		case ID_SELP4:
			SetPriceString(pGridCell, pData, pData->psel4);
			break;
			*/
		case ID_BUYV1:
			SetVolString(pGridCell, pData, pData->vbuy1, pData->pbuy1);
			break;
		case ID_BUYV2:
			SetVolString(pGridCell, pData, pData->vbuy2, pData->pbuy2);
			break;
		case ID_BUYV3:
			SetVolString(pGridCell, pData, pData->vbuy3, pData->pbuy3);
			break;
			//		case ID_BUYV4:
			//			SetVolString(pGridCell, pData, pData->vbuy4, pData->pbuy4);
			//			break;
		case ID_SELV1:
			SetVolString(pGridCell, pData, pData->vsel1, pData->psel1);
			break;
		case ID_SELV2:
			SetVolString(pGridCell, pData, pData->vsel2, pData->psel2);
			break;
		case ID_SELV3:
			SetVolString(pGridCell, pData, pData->vsel3, pData->psel3);
			break;
			//		case ID_SELV4:
			//			SetVolString(pGridCell, pData, pData->vsel4);
			//			break;

		case ID_ZGB:        //总股本(万股)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->zgb); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_GJG:        //国家股(万股)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->gjg); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_FQRFRG:     //发起人法人股(万股)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->fqrfrg); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_FRG:        //法人股(万股)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->frg); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_ZGG:        //职工股(万股)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->zgg); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_GZAG:       //公众A股(万股)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->gzAg); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_LTAG:       //流通A股(万股)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->ltAg); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_HG:         //Ｈ股(万股)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->Hg); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_BG:         //B股(万股)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->Bg); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_ZPG:        //转配股(万股)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->zpg); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;

		case ID_ZZC:        //总资产(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->zzc); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_LDZC:       //流动资产(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->ldzc); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_CQTZ:       //长期投资(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->cqtz); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_GDZC:       //固定资产(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->gdzc); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_WXZC:       //无形资产(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->wxzc); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_LDFZ:       //流动负债(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->ldfz); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_CQFZ:       //长期负债(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->cqfz); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_GGQY:       //股东权益(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->ggqy); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_ZBGJJ:      //资本公积金(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->zbgjj); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_YYGJJ:      //盈余公积金(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->yygjj); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_MGJZ:       //每股净值(元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->mgjz); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_GDQYBL:     //股东权益比率(%)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.3f",pData->pBaseInfo->gdqybl); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_MGGJJ:      //每股公积金(元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.3f",pData->pBaseInfo->mggjj); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_ZYYWSR:     //主营业务收入(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->zyywsr); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_ZYYWLR:     //主营业务利润(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->zyywlr); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_QTYWLR:     //其它业务利润(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->qtywlr); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_LRZE:       //利润总额(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->lrze); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_JLR:        //净利润(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->jlr); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_WFPLR:      //未分配利润(万元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.2f",pData->pBaseInfo->wfplr); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_MGSY:       //每股收益(元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.3f",pData->pBaseInfo->mgsy); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_JZCSYL:     //净资产收益率(%)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.3f",pData->pBaseInfo->jzcsyl); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_MGWFPLR:    //每股未分配利润(元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.3f",pData->pBaseInfo->mgwfplr); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_MGJZC:      //每股净资产(元)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				sprintf(str,"%6.3f",pData->pBaseInfo->mgjzc); 
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_ZJLS:        //资金(%d分)
			if(m_pDoc->m_nANT[0]>m_pDoc->m_syshqset.capital)
			{
				int i=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_pDoc->m_syshqset.capital)
				{
					i--;
				}
				sprintf(str,"%6.2f",(pData->m_Kdata1[i].Amount - pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.capital ].Amount)/10000);
			}
			else
			{
				int i=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_pDoc->m_syshqset.capital)
				{
					i--;
				}
				sprintf(str,"%6.2f",pData->m_Kdata1[i].Amount/10000 );
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_VOLBIFM:    //量比(%d分)
			if(m_pDoc->m_nANT[0] !=0 && pData->volume5!=0 )
			{
				int i=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_pDoc->m_syshqset.quangtity)
				{
					i--;
				}
				if(m_pDoc->m_nANT[0] > m_pDoc->m_syshqset.quangtity)
				{
					sprintf(str,"%6.2f%s",(100*(pData->m_Kdata1[i].Volume - pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.quangtity ].Volume) / m_pDoc->m_syshqset.quangtity) / (pData->volume5 /240 ),"%" );
				}
				else
				{
					sprintf(str,"%6.2f%s",(100*(pData->m_Kdata1[i].Volume ) / m_pDoc->m_syshqset.quangtity) / (pData->volume5 /240 ),"%" );
				}
			}
			else
				sprintf(str,"%6.2f",0.0f   );
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_HLPFM:      //震幅(%d分)
			{
				float high=0,low=0,totp=0;
				int minute=m_pDoc->m_syshqset.swing;
				if(m_pDoc->m_nANT[0] < minute)
					minute=m_pDoc->m_nANT[0];
				for(int i=0;i<minute;i++)
				{   
					if(pData->m_Kdata1[m_pDoc->m_nANT[0]-i].Price==0)
						continue;
					if(high==0&&low==0)
					{
						high=low=pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price;
						continue;
					}
					if(pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price > high  )
						high=pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price;
					else if(pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price <low)
						low=pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price;
				}
				if(pData->ystc==0)
					sprintf(str,"%6.2f%s",0.0f,"%");
				else
					sprintf(str,"%6.2f%s",100*(high-low)/(pData->ystc),"%");
				pGridCell->szText =str;
				pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			}
			break;
		case ID_EXCHANGEFM:  //换手率(%d分)
			if(pData->pBaseInfo==NULL)
				sprintf(str,"%6.2f",0.0f);
			else
			{
				int i=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_pDoc->m_syshqset.changehand)
				{
					i--;
				}
				if(pData->kind==SHBG|| pData->kind==SZBG)
					if(pData->pBaseInfo->Bg!=0) 
					{
						if(m_pDoc->m_nANT[0] > m_pDoc->m_syshqset.changehand)
							sprintf(str,"%6.2f",(pData->m_Kdata1[i].Volume - pData->m_Kdata1[i-m_pDoc->m_syshqset.changehand].Volume)/pData->pBaseInfo->Bg);
						else
							sprintf(str,"%6.2f",(pData->m_Kdata1[i].Volume)/pData->pBaseInfo->Bg);
					}
					else
						sprintf(str,"%6.2f",0.0f);
				else
					if(pData->pBaseInfo->ltAg!=0)
					{
						if(m_pDoc->m_nANT[0] >m_pDoc->m_syshqset.changehand)
							sprintf(str,"%6.2f",(pData->m_Kdata1[i].Volume - pData->m_Kdata1[i - m_pDoc->m_syshqset.changehand].Volume)/pData->pBaseInfo->ltAg);
						else
							sprintf(str,"%6.2f",(pData->m_Kdata1[i].Volume)/pData->pBaseInfo->ltAg);
					}
					else
						sprintf(str,"%6.2f",0.0f);
			}
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_ADVSP:       //涨速(%d分)
			if( m_pDoc->m_syshqset.upspeed  <m_pDoc->m_nANT[0])
				if( pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.upspeed ].Price!=0)
				{
					float m_nowp=0;
					int i=m_pDoc->m_nANT[0];
					while( pData->m_Kdata1[i].Price==0&&i!=0)
					{
						i--;
					}
					m_nowp=pData->m_Kdata1[i].Price;
					m_temp= 100*(m_nowp - 
						pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.upspeed ].Price)/pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.upspeed ].Price;
				}   
				else
					m_temp=0.0f;
			else
				if(pData->opnp !=0)
				{
					float m_nowp=0;
					int i=m_pDoc->m_nANT[0];
					while( pData->m_Kdata1[i].Price==0&&i!=0)
					{
						i--;
					}
					m_nowp=pData->m_Kdata1[i].Price;

					m_temp= 100*(m_nowp - pData->opnp )/pData->opnp ;
				}
				else
					m_temp=0.0f;
			sprintf(str,"%6.2f%s",m_temp,"%"   );
			pGridCell->szText =str;
			if(m_temp>0 )
				pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			else if(m_temp<0)
				pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
			else
				pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 

			break;
		default:
			if(m_pColData[nCol].nID>=9000&&m_pColData[nCol].nID<9010)
			{
				sprintf(str,"%6.2f",pData->Index[m_pColData[nCol].nID - 9000]);
				pGridCell->szText =str;
				pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			}
			break;
		}
		return TRUE;
	}

	DWORD key= MAKELONG(nRow, nCol);
	if(nRow==1&&nCol==0)
		nCol=0;
    CCellID cell;
//    if((m_SortColumn==nCol||m_idCurrentCell.row==nRow)&&nCol!=0)
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

    pGridCell->crBkClr=m_clrBackGround;//m_pDoc->m_colorArray[18];//CLR_DEFAULT;  // Background colour (or CLR_DEFAULT)
    pGridCell->nFormat=m_pColData[nCol].nFormat;
    
	if(nRow >= m_nRealRow)
	{
        pGridCell->szText="";
		return TRUE;
	}
	char str[50];
	float m_temp;
    if(m_pDoc==NULL)		
	{
		return TRUE;
	}
	CReportData *pData=m_pDoc->m_pStockDataShow[nRow - m_nFixedRows].pItem;
	//CReportData *pData=m_pDoc->m_pStockDataShow[nRow-1].pItem;
	if(pData==NULL )
		return FALSE;
	switch(m_pColData[nCol].nID)
	{
	    case ID_ROWNUM:
              sprintf(str,"%d",nRow - m_nFixedRows + 1);
			  pGridCell->szText =str;
			  pGridCell->crFgClr = RGB(255 - GetRValue(m_pDoc->m_colorArray[18]),255 - GetGValue(m_pDoc->m_colorArray[18]),255 - GetBValue(m_pDoc->m_colorArray[18])); 
			  //pGridCell->crFgClr = m_pDoc->m_colorArray[1]; 
			  break;
		case ID_STOCKID:                                        //代码
              sprintf(str,"%s",pData->id);
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[21]; 
			  //pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			  break;
		case ID_STOCKNAME:                                      //名称
              sprintf(str,"%s",pData->name );
			  pGridCell->crFgClr =m_pDoc->m_colorArray[20]; 
			  //pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			  pGridCell->szText =str;
			break;
		case ID_YSTC: 				//昨收
			if(pData->kind==SHBG)
              sprintf(str,"%6.3f",pData->ystc  );
			else
              sprintf(str,"%6.2f",pData->ystc  );
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_OPEN:                                //今开
			if(pData->kind==SHBG)
              sprintf(str,"%6.3f",pData->opnp  );
			else
              sprintf(str,"%6.2f",pData->opnp );
			  pGridCell->szText =str;
			  if(pData->opnp > pData->ystc )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pData->opnp < pData->ystc)
				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_HIGH:                                //最高
			if(pData->kind==SHBG)
              sprintf(str,"%6.3f",pData->higp  );
			else
              sprintf(str,"%6.2f",pData->higp );
			  pGridCell->szText =str;
			  if(pData->higp > pData->ystc )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pData->higp < pData->ystc)
				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_LOW:                                 //最低
			if(pData->kind==SHBG)
              sprintf(str,"%6.3f",pData->lowp  );
			else
              sprintf(str,"%6.2f",pData->lowp  );
			  pGridCell->szText =str;
			  if(pData->lowp > pData->ystc )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pData->lowp < pData->ystc)
				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_NEW:                                 //最新
			if(pData->kind==SHBG)
              sprintf(str,"%6.3f",pData->nowp  );
			else
              sprintf(str,"%6.2f",pData->nowp   );
			  if(pData->nowp > pData->ystc )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pData->nowp < pData->ystc)
				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  pGridCell->szText =str;
			break;
		case ID_TOTV:                                //总手
              sprintf(str,"%6.0f",pData->totv   );
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_NOWV:                                //现手
              sprintf(str,"%4.0f",pData->nowv   );
			  pGridCell->szText =str;
               pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_TOTP:                                //总额
			if(pData->kind==SHBG)
              sprintf(str,"%6.3f",pData->totp /10000  );
			else
              sprintf(str,"%6.2f",pData->totp  /10000 );
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_AVGP:                                //均价
			  if(pData->totv!=0)
					if(pData->kind==SHBG)
					  sprintf(str,"%6.3f",pData->totp /pData->totv /100  );
					else
                      sprintf(str,"%6.2f",pData->totp /pData->totv /100 );
			  else
                 sprintf(str,"%6.2f",0.0f  );
			  pGridCell->szText =str;
			  if(pData->totv!=0)
			  {
				  if(pData->totp /pData->totv /100 > pData->ystc )
					 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
				  else if(pData->totp /pData->totv /100 < pData->ystc)
					 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
				  else
					 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			  }
			  else
					 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 

			break;
		case ID_ADDE:                                //涨跌
			 if(pData->ystc !=0&&pData->nowp!=0)
			 {
					if(pData->kind==SHBG)
					  sprintf(str,"%6.3f",pData->nowp - pData->ystc  );
					else
                      sprintf(str,"%6.2f",pData->nowp - pData->ystc  );
			 }
			 else
               sprintf(str,"%6.2f",0.0f);

			  pGridCell->szText =str;
			  if(pData->nowp > pData->ystc )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pData->nowp < pData->ystc)
				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_HLP:                                 //震幅
              if(pData->ystc !=0)
			  {
                  sprintf(str,"%6.2f%s",100 * (pData->higp - pData->lowp) /pData->ystc ,"%" );
			  }
			  else
                 sprintf(str,"%6.2f%s",0.0f,"%" ); 
			  pGridCell->szText =str;
			 pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_RDR:                                 //涨幅
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
			break;
		case ID_BSSPBAS:                            //委比
			{
				float temp=0;
				if (pData->accb+pData->accs>0)
					if(pData->accs==0&&pData->accb!=0)
					   temp=100;
					else if(pData->accs!=0&&pData->accb==0)
					   temp=-100;
					else
					   temp=(float)(pData->accb-pData->accs)/(float)(pData->accb+pData->accs)*100.0;     //计算委比
				else
					temp=0;
				  sprintf(str,"%6.2f%s",temp ,"%"  );
				  pGridCell->szText =str;
				 pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			}
			break;
		case ID_DIFBS:                              //委差
              sprintf(str,"%6.0f",pData->accb - pData->accs   );
			  pGridCell->szText =str;
			 pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_VOLBI:                              //量比
			if(m_pDoc->m_nANT[0] !=0 && pData->volume5!=0 )
			{
              sprintf(str,"%6.2f%s",(pData->totv / m_pDoc->m_nANT[0]) / (pData->volume5 /240 ),"%" );
			}
			else
              sprintf(str,"%6.2f",0.0f   );

			  pGridCell->szText =str;
			 pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_ACCB:                               //委买
              sprintf(str,"%6.0f",pData->accb  );
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_ACCS:                               //委卖
              sprintf(str,"%6.0f",pData->accs  );
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			break;
		case ID_BUY:                                //委买价
			if(pData->kind==SHBG)
			  sprintf(str,"%6.3f",pData->pbuy1  );
			else
              sprintf(str,"%6.2f",pData->pbuy1   );
			  pGridCell->szText =str;
			  if(pData->pbuy1 > pData->ystc )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pData->pbuy1 < pData->ystc)
				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_SEL:                                //委卖价
			if(pData->kind==SHBG)
			  sprintf(str,"%6.3f",pData->psel1  );
			else
              sprintf(str,"%6.2f",pData->psel1   );
			  pGridCell->szText =str;
			  if(pData->psel1 > pData->ystc )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(pData->psel1 < pData->ystc)
				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 
			break;
		case ID_RVOL:                               //内盘
              sprintf(str,"%6.0f",pData->dvol  );
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
			break;
		case ID_DVOL:                              //外盘
              sprintf(str,"%6.0f",pData->rvol   );
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			break;

        case ID_EXCHANGE:
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
				  if(pData->kind==SHBG|| pData->kind==SZBG)
                     if(pData->pBaseInfo->Bg!=0) 
                        sprintf(str,"%6.2f",pData->totv/pData->pBaseInfo->Bg);
					 else
			            sprintf(str,"%6.2f",0.0f);
				  else
					  if(pData->pBaseInfo->ltAg!=0)
			            sprintf(str,"%6.2f",pData->totv/pData->pBaseInfo->ltAg);
					  else
			            sprintf(str,"%6.2f",0.0f);
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			  break;
        case ID_SYL:
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
				 if(pData->pBaseInfo->mgsy!=0)
                   sprintf(str,"%6.3f",pData->nowp/pData->pBaseInfo->mgsy); 
				 else
			       sprintf(str,"%6.2f",0.0f);
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			  break;

		case ID_BUYP1:
			SetPriceString(pGridCell, pData, pData->pbuy1);
			break;
		case ID_BUYP2:
			SetPriceString(pGridCell, pData, pData->pbuy2);
			break;
		case ID_BUYP3:
			SetPriceString(pGridCell, pData, pData->pbuy3);
			break;
//		case ID_BUYP4:
//			SetPriceString(pGridCell, pData, pData->pbuy4);
//			break;
		case ID_SELP1:
			SetPriceString(pGridCell, pData, pData->psel1);
			break;
		case ID_SELP2:
			SetPriceString(pGridCell, pData, pData->psel2);
			break;
		case ID_SELP3:
			SetPriceString(pGridCell, pData, pData->psel3);
			break;
/*		case ID_SELP4:
			SetPriceString(pGridCell, pData, pData->psel4);
			break;
*/
		case ID_BUYV1:
			SetVolString(pGridCell, pData, pData->vbuy1, pData->pbuy1);
			break;
		case ID_BUYV2:
			SetVolString(pGridCell, pData, pData->vbuy2, pData->pbuy2);
			break;
		case ID_BUYV3:
			SetVolString(pGridCell, pData, pData->vbuy3, pData->pbuy3);
			break;
//		case ID_BUYV4:
//			SetVolString(pGridCell, pData, pData->vbuy4, pData->pbuy4);
//			break;
		case ID_SELV1:
			SetVolString(pGridCell, pData, pData->vsel1, pData->psel1);
			break;
		case ID_SELV2:
			SetVolString(pGridCell, pData, pData->vsel2, pData->psel2);
			break;
		case ID_SELV3:
			SetVolString(pGridCell, pData, pData->vsel3, pData->psel3);
			break;
//		case ID_SELV4:
//			SetVolString(pGridCell, pData, pData->vsel4);
//			break;

		case ID_ZGB:        //总股本(万股)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->zgb); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_GJG:        //国家股(万股)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->gjg); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_FQRFRG:     //发起人法人股(万股)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->fqrfrg); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_FRG:        //法人股(万股)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->frg); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_ZGG:        //职工股(万股)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->zgg); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_GZAG:       //公众A股(万股)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->gzAg); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_LTAG:       //流通A股(万股)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->ltAg); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_HG:         //Ｈ股(万股)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->Hg); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_BG:         //B股(万股)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->Bg); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_ZPG:        //转配股(万股)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->zpg); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;

		case ID_ZZC:        //总资产(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->zzc); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_LDZC:       //流动资产(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->ldzc); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_CQTZ:       //长期投资(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->cqtz); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_GDZC:       //固定资产(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->gdzc); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_WXZC:       //无形资产(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->wxzc); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_LDFZ:       //流动负债(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->ldfz); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_CQFZ:       //长期负债(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->cqfz); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_GGQY:       //股东权益(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->ggqy); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_ZBGJJ:      //资本公积金(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->zbgjj); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_YYGJJ:      //盈余公积金(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->yygjj); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_MGJZ:       //每股净值(元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->mgjz); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_GDQYBL:     //股东权益比率(%)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.3f",pData->pBaseInfo->gdqybl); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_MGGJJ:      //每股公积金(元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.3f",pData->pBaseInfo->mggjj); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_ZYYWSR:     //主营业务收入(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->zyywsr); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_ZYYWLR:     //主营业务利润(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->zyywlr); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_QTYWLR:     //其它业务利润(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->qtywlr); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_LRZE:       //利润总额(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->lrze); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_JLR:        //净利润(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->jlr); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_WFPLR:      //未分配利润(万元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.2f",pData->pBaseInfo->wfplr); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_MGSY:       //每股收益(元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.3f",pData->pBaseInfo->mgsy); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_JZCSYL:     //净资产收益率(%)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.3f",pData->pBaseInfo->jzcsyl); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_MGWFPLR:    //每股未分配利润(元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.3f",pData->pBaseInfo->mgwfplr); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_MGJZC:      //每股净资产(元)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
                 sprintf(str,"%6.3f",pData->pBaseInfo->mgjzc); 
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			    break;
		case ID_ZJLS:        //资金(%d分)
			  if(m_pDoc->m_nANT[0]>m_pDoc->m_syshqset.capital)
			  {
				int i=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_pDoc->m_syshqset.capital)
				{
					i--;
				}
                sprintf(str,"%6.2f",(pData->m_Kdata1[i].Amount - pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.capital ].Amount)/10000);
			  }
			  else
			  {
				int i=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_pDoc->m_syshqset.capital)
				{
					i--;
				}
                sprintf(str,"%6.2f",pData->m_Kdata1[i].Amount/10000 );
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			  break;
		case ID_VOLBIFM:    //量比(%d分)
			if(m_pDoc->m_nANT[0] !=0 && pData->volume5!=0 )
			{
				int i=m_pDoc->m_nANT[0];
				while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_pDoc->m_syshqset.quangtity)
				{
				   i--;
				}
                 if(m_pDoc->m_nANT[0] > m_pDoc->m_syshqset.quangtity)
				 {
                    sprintf(str,"%6.2f%s",(100*(pData->m_Kdata1[i].Volume - pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.quangtity ].Volume) / m_pDoc->m_syshqset.quangtity) / (pData->volume5 /240 ),"%" );
				 }
				 else
				 {
                    sprintf(str,"%6.2f%s",(100*(pData->m_Kdata1[i].Volume ) / m_pDoc->m_syshqset.quangtity) / (pData->volume5 /240 ),"%" );
				 }
			}
			else
                 sprintf(str,"%6.2f",0.0f   );
			pGridCell->szText =str;
			pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
		    break;
		case ID_HLPFM:      //震幅(%d分)
			{
				float high=0,low=0,totp=0;
				int minute=m_pDoc->m_syshqset.swing;
				if(m_pDoc->m_nANT[0] < minute)
                    minute=m_pDoc->m_nANT[0];
				for(int i=0;i<minute;i++)
				{   
				    if(pData->m_Kdata1[m_pDoc->m_nANT[0]-i].Price==0)
						continue;
					if(high==0&&low==0)
					{
						high=low=pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price;
						continue;
					}
					if(pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price > high  )
						high=pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price;
					else if(pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price <low)
						low=pData->m_Kdata1[m_pDoc->m_nANT[0] -i].Price;
				}
				if(pData->ystc==0)
				  sprintf(str,"%6.2f%s",0.0f,"%");
				else
				  sprintf(str,"%6.2f%s",100*(high-low)/(pData->ystc),"%");
			    pGridCell->szText =str;
			    pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			}
			break;
		case ID_EXCHANGEFM:  //换手率(%d分)
			  if(pData->pBaseInfo==NULL)
			     sprintf(str,"%6.2f",0.0f);
              else
			  {
				  int i=m_pDoc->m_nANT[0];
				  while( pData->m_Kdata1[i].Price==0&&i!=0&&m_pDoc->m_nANT[0]-i<m_pDoc->m_syshqset.changehand)
				  {
				 	  i--;
				  }
				  if(pData->kind==SHBG|| pData->kind==SZBG)
                     if(pData->pBaseInfo->Bg!=0) 
					 {
						 if(m_pDoc->m_nANT[0] > m_pDoc->m_syshqset.changehand)
                           sprintf(str,"%6.2f",(pData->m_Kdata1[i].Volume - pData->m_Kdata1[i-m_pDoc->m_syshqset.changehand].Volume)/pData->pBaseInfo->Bg);
						 else
                           sprintf(str,"%6.2f",(pData->m_Kdata1[i].Volume)/pData->pBaseInfo->Bg);
					 }
					 else
			            sprintf(str,"%6.2f",0.0f);
				  else
					  if(pData->pBaseInfo->ltAg!=0)
					  {
						 if(m_pDoc->m_nANT[0] >m_pDoc->m_syshqset.changehand)
			                sprintf(str,"%6.2f",(pData->m_Kdata1[i].Volume - pData->m_Kdata1[i - m_pDoc->m_syshqset.changehand].Volume)/pData->pBaseInfo->ltAg);
						 else
			                sprintf(str,"%6.2f",(pData->m_Kdata1[i].Volume)/pData->pBaseInfo->ltAg);
					  }
					  else
			            sprintf(str,"%6.2f",0.0f);
			  }
			  pGridCell->szText =str;
			  pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			  break;
		case ID_ADVSP:       //涨速(%d分)
			if( m_pDoc->m_syshqset.upspeed  <m_pDoc->m_nANT[0])
				if( pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.upspeed ].Price!=0)
				{
					float m_nowp=0;
					int i=m_pDoc->m_nANT[0];
					while( pData->m_Kdata1[i].Price==0&&i!=0)
					{
						  i--;
					}
					m_nowp=pData->m_Kdata1[i].Price;
					m_temp= 100*(m_nowp - 
					pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.upspeed ].Price)/pData->m_Kdata1[m_pDoc->m_nANT[0] - m_pDoc->m_syshqset.upspeed ].Price;
				}   
				else
                    m_temp=0.0f;
			else
				if(pData->opnp !=0)
				{
					float m_nowp=0;
					int i=m_pDoc->m_nANT[0];
					while( pData->m_Kdata1[i].Price==0&&i!=0)
					{
						  i--;
					}
                    m_nowp=pData->m_Kdata1[i].Price;

					m_temp= 100*(m_nowp - pData->opnp )/pData->opnp ;
				}
				else
                    m_temp=0.0f;
              sprintf(str,"%6.2f%s",m_temp,"%"   );
			  pGridCell->szText =str;
			  if(m_temp>0 )
				 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
			  else if(m_temp<0)
				 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
              else
				 pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 

			break;
		default:
               if(m_pColData[nCol].nID>=9000&&m_pColData[nCol].nID<9010)
			   {
                   sprintf(str,"%6.2f",pData->Index[m_pColData[nCol].nID - 9000]);
			       pGridCell->szText =str;
				   pGridCell->crFgClr =m_pDoc->m_colorArray[1]; 
			   }
			break;
		}
   return TRUE;
}
BOOL CGridCtrl::SetCell(int nRow, int nCol, CGridCell* pCell)
{
    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return FALSE;
//19991106
	CReportData *pData=m_pDoc->m_pStockDataShow[nRow - m_nFixedRows].pItem;
    if(m_pDoc==NULL)		
		return TRUE;
	switch(m_pColData[nCol].nID)
	{
	    case ID_ROWNUM:
			  break;
		case ID_STOCKID:      			//代码
			  strcpy(pData->id,pCell->szText.GetBuffer(0));
			  break;
		case ID_STOCKNAME:                                      //名称
			  strcpy(pData->name,pCell->szText.GetBuffer(0));
			  break;
		case ID_YSTC: 				//昨收
			pData->ystc=atof(pCell->szText.GetBuffer(0));
			break;
		case ID_OPEN:                                //今开
			pData->opnp=atof(pCell->szText.GetBuffer(0));
			break;
		case ID_HIGH:                                //最高
			pData->higp=atof(pCell->szText.GetBuffer(0));
			break;
		case ID_LOW:                                 //最低
			pData->lowp=atof(pCell->szText.GetBuffer(0));
			break;
		case ID_NEW:                                 //最新
			pData->nowp=atof(pCell->szText.GetBuffer(0));
			break;
		case ID_TOTV:                                //总手
			pData->totv=atof(pCell->szText.GetBuffer(0));
			break;
		case ID_NOWV:                                //现手
			pData->nowv=atof(pCell->szText.GetBuffer(0));
			break;
		case ID_TOTP:                                //总额
			pData->totp=atof(pCell->szText.GetBuffer(0));
			break;
		case ID_AVGP:                                //均价
			break;
		case ID_ADDE:                                //涨跌
			break;
		case ID_HLP:                                 //震幅
			break;
		case ID_RDR:                                 //涨幅
			break;
		case ID_BSSPBAS:                            //委比
			break;
		case ID_DIFBS:                              //委差
			break;
		case ID_VOLBI:                              //量比
			break;
		case ID_ACCB:                               //委买
			pData->accb=atof(pCell->szText.GetBuffer(0));
			break;
		case ID_ACCS:                               //委卖
			pData->accs=atof(pCell->szText.GetBuffer(0));
			break;
		case ID_BUY:                                //委买价
			pData->pbuy1=atof(pCell->szText.GetBuffer(0));
			break;
		case ID_SEL:                                //委卖价
			pData->psel1=atof(pCell->szText.GetBuffer(0));
			break;
		case ID_RVOL:                               //内盘
			pData->dvol=atof(pCell->szText.GetBuffer(0));
			break;
		case ID_DVOL:                              //外盘
			pData->rvol=atof(pCell->szText.GetBuffer(0));
			break;
		case ID_ADVSP:                             //涨速
			break;
		case ID_BUYP1:                               //
                pData->pbuy1  = atof(pCell->szText.GetBuffer(0));break;
		case ID_BUYP2:                               //
                pData->pbuy2 = atof(pCell->szText.GetBuffer(0));break;
		case ID_BUYP3:                               //
                pData->pbuy3 = atof(pCell->szText.GetBuffer(0));break;
//		case ID_BUYP4:                               //
//                pData->pbuy4 = atof(pCell->szText.GetBuffer(0));break;
		case ID_BUYV1:                               //
                pData->vbuy1 = atof(pCell->szText.GetBuffer(0));break;
		case ID_BUYV2:                               //
                pData->vbuy2 = atof(pCell->szText.GetBuffer(0));break;
		case ID_BUYV3:                                //
                pData->vbuy3 = atof(pCell->szText.GetBuffer(0));break;
//		case ID_BUYV4:                                //
//                pData->vbuy4 = atof(pCell->szText.GetBuffer(0));break;
		case ID_SELP1:                               //
                pData->psel1 = atof(pCell->szText.GetBuffer(0));break;
		case ID_SELP2:                               //
                pData->psel2= atof(pCell->szText.GetBuffer(0));break;
		case ID_SELP3:                               //
                pData->psel3= atof(pCell->szText.GetBuffer(0));break;
//		case ID_SELP4:                               //
  //              pData->psel4= atof(pCell->szText.GetBuffer(0));break;
		case ID_SELV1:                               //
                pData->vsel1= atof(pCell->szText.GetBuffer(0));break;
		case ID_SELV2:                               //
                pData->vsel2= atof(pCell->szText.GetBuffer(0));break;
		case ID_SELV3:                               //
                pData->vsel3= atof(pCell->szText.GetBuffer(0));break;
//		case ID_SELV4:                               //
  //              pData->vsel4= atof(pCell->szText.GetBuffer(0));break;
		default:
			break;
		}
    return TRUE;
}
void CGridCtrl::SetMainDocument(CTaiShanDoc *pDocument)
{
   m_pDoc=pDocument;
   SetBackgroundClr(m_pDoc->m_colorArray[18]);
}
void CGridCtrl::NewReDrawRow()
{
	RECT clipRect;
    int nFixedRowHeight = GetFixedRowHeight();
	GetClientRect(&clipRect);
    clipRect.bottom=nFixedRowHeight;
    InvalidateRect(&clipRect,TRUE);
}



void CGridCtrl::OnLeftButton(CPoint point)
{
	UINT nFlags = MK_LBUTTON ;
	OnLButtonDown(nFlags, point);
	OnLButtonUp( nFlags,  point);
}

CPoint CGridCtrl::GetPoint(int nCol)
{
	CPoint p;
	p.y = 1;
	p.x = 0;

	for(int i = 0;i<nCol ;i++)
       p.x = p.x + GetColumnWidth(i);
	p.x+=GetColumnWidth(nCol)/2;
	return p;
}

void CGridCtrl::SetPriceString(CGridCell *pGridCell, CReportData *pData, float fPrice) const
{
	 CString str;
	 if(pData->ystc !=0&&fPrice !=0)
	 {
			if(pData->kind==SHBG)
			  str.Format("%6.3f",fPrice  );
			else
              str.Format("%6.2f",fPrice );
	 }
	 else
       str="0.00";

	  pGridCell->szText =str;
	  if(fPrice > pData->ystc )
		 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
	  else if(fPrice < pData->ystc)
		 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
      else
		 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 

}

void CGridCtrl::SetVolString(CGridCell *pGridCell, CReportData *pData, float fVol,float fPrice) const
{
	 CString str;
     str.Format("%.0f",fVol   );
	 pGridCell->szText =str;

	 pGridCell->szText =str;
	  if(fPrice > pData->ystc )
		 pGridCell->crFgClr =m_pDoc->m_colorArray[13]; 
	  else if(fPrice < pData->ystc)
		 pGridCell->crFgClr =m_pDoc->m_colorArray[15]; 
      else
		 pGridCell->crFgClr =m_pDoc->m_colorArray[14]; 

}

void CGridCtrl::SetBackgroundClr(COLORREF clr)
{
	m_clrBackGround = clr;
	RedrawWindow();
}

void CGridCtrl::OnReportAve()
{
	m_pDoc->m_bStatistic[AVE_VALUE] = !m_pDoc->m_bStatistic[AVE_VALUE];

	if (m_pDoc->m_bStatistic[AVE_VALUE])
	{
		GetParent()->SendMessage(WM_USER_COLNUM,AVE_VALUE,1);
		m_nFixedRowIndex.Add(AVE_VALUE);
		m_nFixedRows++;
		m_nRealRow++;
	}
	else
	{
		GetParent()->SendMessage(WM_USER_COLNUM,AVE_VALUE,-1);
		int index = -1;
		for (int i = 0; i < m_nFixedRowIndex.GetSize();i++)
		{
			if (m_nFixedRowIndex[i] == AVE_VALUE)
			{
				index = i;
				break;
			}
		}
		if (index != -1)
		{
			m_nFixedRowIndex.RemoveAt(index);
		}
		m_nFixedRows--;
		m_nRealRow--;
	}

	if (!m_pDoc->m_bStatisticed)
	{
		m_pDoc->StatisticStockData(m_pDoc->m_nWeightType);
		m_pDoc->m_bStatisticed = true;
	}
	Invalidate();
}
void CGridCtrl::OnReportMax()
{
	m_pDoc->m_bStatistic[MAX_VALUE] = !m_pDoc->m_bStatistic[MAX_VALUE];
	if (m_pDoc->m_bStatistic[MAX_VALUE])
	{
		GetParent()->SendMessage(WM_USER_COLNUM,MAX_VALUE,1);
		m_nFixedRowIndex.Add(MAX_VALUE);
		m_nFixedRows++;
		m_nRealRow++;
	}
	else
	{
		GetParent()->SendMessage(WM_USER_COLNUM,MAX_VALUE,-1);
		int index = -1;
		for (int i = 0; i < m_nFixedRowIndex.GetSize();i++)
		{
			if (m_nFixedRowIndex[i] == MAX_VALUE)
			{
				index = i;
				break;
			}
		}
		if (index != -1)
		{
			m_nFixedRowIndex.RemoveAt(index);
		}

		m_nFixedRows--;
		m_nRealRow--;
	}

	if (!m_pDoc->m_bStatisticed)
	{
		m_pDoc->StatisticStockData(m_pDoc->m_nWeightType);
		m_pDoc->m_bStatisticed = true;
	}
	Invalidate();

}

void CGridCtrl::OnReportMin()
{
	m_pDoc->m_bStatistic[MIN_VALUE] = !m_pDoc->m_bStatistic[MIN_VALUE];
	if (m_pDoc->m_bStatistic[MIN_VALUE])
	{
		GetParent()->SendMessage(WM_USER_COLNUM,MIN_VALUE,1);
		m_nFixedRowIndex.Add(MIN_VALUE);
		m_nFixedRows++;
		m_nRealRow++;
	}
	else
	{
		GetParent()->SendMessage(WM_USER_COLNUM,MIN_VALUE,-1);
		int index = -1;
		for (int i = 0; i < m_nFixedRowIndex.GetSize();i++)
		{
			if (m_nFixedRowIndex[i] == MIN_VALUE)
			{
				index = i;
				break;
			}
		}
		if (index != -1)
		{
			m_nFixedRowIndex.RemoveAt(index);
		}

		m_nFixedRows--;
		m_nRealRow--;
	}

	if (!m_pDoc->m_bStatisticed)
	{
		m_pDoc->StatisticStockData(m_pDoc->m_nWeightType);
		m_pDoc->m_bStatisticed = true;
	}
	Invalidate();

}

void CGridCtrl::OnReportSet()
{
	CStatisticSettingDlg dlg;
	dlg.SetParam(m_pDoc->m_nWeightType);
	ReleaseCapture();
	if (dlg.DoModal() == IDOK)
	{
		dlg.GetParam(m_pDoc->m_nWeightType);
	}
	SetCapture();
	
}

void CGridCtrl::OnReportVar()
{
	m_pDoc->m_bStatistic[VAR_VALUE] = !m_pDoc->m_bStatistic[VAR_VALUE];
	if (m_pDoc->m_bStatistic[VAR_VALUE])
	{
		GetParent()->SendMessage(WM_USER_COLNUM,VAR_VALUE,1);
		m_nFixedRowIndex.Add(VAR_VALUE);
		m_nFixedRows++;
		m_nRealRow++;
	}
	else
	{
		GetParent()->SendMessage(WM_USER_COLNUM,VAR_VALUE,-1);
		int index = -1;
		for (int i = 0; i < m_nFixedRowIndex.GetSize();i++)
		{
			if (m_nFixedRowIndex[i] == VAR_VALUE)
			{
				index = i;
				break;
			}
		}
		if (index != -1)
		{
			m_nFixedRowIndex.RemoveAt(index);
		}

		m_nFixedRows--;
		m_nRealRow--;
	}

	if (!m_pDoc->m_bStatisticed)
	{
		m_pDoc->StatisticStockData(m_pDoc->m_nWeightType);
		m_pDoc->m_bStatisticed = true;

	}
	Invalidate();

}
