#include "WH.h"

#if !defined(AFX_GRIDCTRL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_GRIDCTRL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "ColumnSelect.h"

/////////////////////////////////////////////////////////////////////////////
// GridCtrl.h : header file
//
// MFC Grid Control header file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Copyright (c) 1998.
//
// The code contained in this file is based on the original
// WorldCom Grid control written by Joe Willcoxson,
//        E-mail:  chinajoe@aol.com
//        URL:  http://users.aol.com/chinajoe
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
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 
//
/////////////////////////////////////////////////////////////////////////////
//#include "Grid.h"
#define GRIDCONTROL_USE_TITLETIPS       // Use titletips for cells with large data
#define GRIDCONTROL_USE_OLE_DRAGDROP    // Use OLE drag and drop

#include "CellRange.h"
#include <afxtempl.h>

#ifdef GRIDCONTROL_USE_TITLETIPS
#include "TitleTip.h"
#endif

#ifdef GRIDCONTROL_USE_OLE_DRAGDROP
#include "GridDropTarget.h"
#else
#include <afxole.h>                     // This is sloppy I know...
#endif

// Use this as the classname when inserting this control as a custom control
// in the MSVC++ dialog editor
#define GRIDCTRL_CLASSNAME _T("MFCGridCtrl")

#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

// storage typedef for each row in the grid
//typedef CTypedPtrArray<CObArray, CGridCell*> GRID_ROW;

// DDX_GridControl is used where a DDX_Control call is needed. In some strange
// situations the usual DDX_Control does not result in CGridCtrl::SubclassWindow
// or CGridCtrl::PreSubclassWindow being called. Using this version calls 
// CGridCtrl::SubclassWindow directly - ensuring that cell metrics are set properly

class CGridCtrl;
class CTaiShanDoc;
class CXScrollBar;

void AFXAPI DDX_GridControl(CDataExchange* pDX, int nIDC, CGridCtrl& rControl);

typedef struct _GCOL_ITEM { 
    int      col;     // Row and Column of item
    UINT     nFormat;     // Format of cell
    UINT     nID ;        // col for ID number
    char     szText[50];      // Text in cell
	BOOL     IsIndex;
} GCOL_ITEM; 



/////////////////////////////////////////////////////////////////////////////
// CGridCtrl window

class CGridCtrl : public CWnd
{
// Construction

	friend class CTaiShanReportView;

public:
    CGridCtrl(int nRows = 0, int nCols = 0, int nFixedRows = 0, int nFixedCols = 0);
    DECLARE_DYNCREATE(CGridCtrl)

    BOOL Create(const RECT& rect, CWnd* parent, UINT nID,
                DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);
	BOOL SubclassWindow(HWND hWnd);

// Attributes
public:
    int  GetRowCount() const                    { return m_nRows; }
    int  GetColumnCount() const                 { return m_nCols; }
    int  GetFixedRowCount() const               { return m_nFixedRows; }
    int  GetFixedColumnCount() const            { return m_nFixedCols; }
    virtual BOOL SetRowCount(int nRows = 10);
    BOOL SetColumnCount(int nCols = 10);
    BOOL SetFixedRowCount(int nFixedRows = 1);
    BOOL SetFixedColumnCount(int nFixedCols = 1);

    int  GetRowHeight(int nRow) const;
    BOOL SetRowHeight(int row, int height);
    int  GetColumnWidth(int nCol) const;
    BOOL SetColumnWidth(int col, int width);

    BOOL GetCellOrigin(int nRow, int nCol, LPPOINT p) const;
    BOOL GetCellOrigin(const CCellID& cell, LPPOINT p) const;
    BOOL GetCellRect(int nRow, int nCol, LPRECT pRect) const;
    BOOL GetCellRect(const CCellID& cell, LPRECT pRect) const;

    int  GetFixedRowHeight() const;
    int  GetFixedColumnWidth() const;
    long GetVirtualWidth() const;
    long GetVirtualHeight() const;

    void     SetTextColor(COLORREF clr)           { m_crTextColour = clr;             }
    COLORREF GetTextColor() const                 { return m_crTextColour;            }
    void     SetTextBkColor(COLORREF clr)         { m_crTextBkColour = clr;           }
    COLORREF GetTextBkColor() const               { return m_crTextBkColour;          }
    void     SetBkColor(COLORREF clr)             { m_crBkColour = clr;               }
    COLORREF GetBkColor() const                   { return m_crBkColour;              }
    void     SetFixedTextColor(COLORREF clr)      { m_crFixedTextColour = clr;        }
    COLORREF GetFixedTextColor() const            { return m_crFixedTextColour;       }
    void     SetFixedBkColor(COLORREF clr)        { m_crFixedBkColour = clr;          }
    COLORREF GetFixedBkColor() const              { return m_crFixedBkColour;         } 
    void     SetGridColor(COLORREF clr)           { m_crGridColour = clr;             }
    COLORREF GetGridColor() const                 { return m_crGridColour;            }
/////////zjh set the line color////////////////
//	COLORREF GetGridColor()                       { return m_crGridColour;            }
	void     SetGridColor_new(COLORREF m_newcolor)    {m_GridLineColor = m_newcolor;
												  if (::IsWindow(GetSafeHwnd())) Invalidate();}
    void     SetBkColor_new(COLORREF clr)             { m_crBkColour = clr;if (::IsWindow(GetSafeHwnd())) Invalidate();}
	//////////////////
////////set mouse state/////////////////////////////////////
    int  GetMouseState()                          {  return m_MouseMode ;}           
    void SetMouseState(int nstate) ;
////////////////////////////////////////////////////////////////////

//    int GetSelectedCount() const                  { return m_SelectedCellMap.GetCount(); }

    CCellID GetFocusCell() const                  { return m_idCurrentCell;           }

    void SetImageList(CImageList* pList)          { m_pImageList = pList;             }
    CImageList* GetImageList() const              { return m_pImageList;              }
    void SetGridLines(int nWhichLines = GVL_BOTH) { m_nGridLines = nWhichLines; 
                                                    if (::IsWindow(GetSafeHwnd())) Invalidate(); }
                                                    
    int  GetGridLines() const                     { return m_nGridLines;              }

    void SetEditable(BOOL bEditable = TRUE)       { m_bEditable = bEditable;          }
    BOOL IsEditable() const                       { return m_bEditable;               }
    void SetModified(BOOL bModified = TRUE)       { m_bModified = bModified;          }
    BOOL GetModified() const                      { return m_bModified;               }
    void SetListMode(BOOL bEnableListMode = TRUE) { m_bListMode = bEnableListMode;    }
    BOOL GetListMode() const                      { return m_bListMode;               }
    void EnableSelection(BOOL bEnable = TRUE)     { ResetSelectedRange(); m_bEnableSelection = bEnable; ResetSelectedRange(); }
    BOOL IsSelectable() const                     { return m_bEnableSelection;        }
    void EnableDragAndDrop(BOOL bAllow = TRUE)    { m_bAllowDragAndDrop = bAllow;     }
    BOOL GetDragAndDrop() const                   { return m_bAllowDragAndDrop;       }
    void SetRowResize(BOOL bResize = TRUE)        { m_bAllowRowResize = bResize;      }
    BOOL GetRowResize() const                     { return m_bAllowRowResize;         }
    void SetColumnResize(BOOL bResize = TRUE)     { m_bAllowColumnResize = bResize;   }
    BOOL GetColumnResize() const                  { return m_bAllowColumnResize;      }
    void SetHeaderSort(BOOL bSortOnClick = TRUE)  { m_bSortOnClick = bSortOnClick;    }
    BOOL GetHeaderSort() const                    { return m_bSortOnClick;            }
    void SetHandleTabKey(BOOL bHandleTab = TRUE)  { m_bHandleTabKey = bHandleTab;     }
    BOOL GetHandleTabKey() const                  { return m_bHandleTabKey;           }
    void SetDoubleBuffering(BOOL bBuffer = TRUE)  { m_bDoubleBuffer = bBuffer;        }
    BOOL GetDoubleBuffering() const               { return m_bDoubleBuffer;           }
    void EnableTitleTips(BOOL bEnable = TRUE)     { m_bTitleTips = bEnable;           }
    BOOL GetTitleTips()                           { return m_bTitleTips;              }

    BOOL   SetItem(const GV_ITEM* pItem);
    BOOL   GetItem(GV_ITEM* pItem);
    BOOL   SetItemText(int nRow, int nCol, LPCTSTR str);
    virtual CString GetItemText(int nRow, int nCol);
    BOOL   SetItemData(int nRow, int nCol, LPARAM lParam);
    LPARAM GetItemData(int nRow, int nCol) const;
    BOOL   SetItemImage(int nRow, int nCol, int iImage);
    int    GetItemImage(int nRow, int nCol) const;
    BOOL   SetItemState(int nRow, int nCol, UINT state);
    UINT   GetItemState(int nRow, int nCol) const;
    BOOL   SetItemFormat(int nRow, int nCol, UINT nFormat);
    UINT   GetItemFormat(int nRow, int nCol) const;
	BOOL   SetItemBkColour(int nRow, int nCol, COLORREF cr = CLR_DEFAULT);
	COLORREF GetItemBkColour(int nRow, int nCol) const;
	BOOL   SetItemFgColour(int nRow, int nCol, COLORREF cr = CLR_DEFAULT);
	COLORREF GetItemFgColour(int nRow, int nCol) const;
	BOOL SetItemFont(int nRow, int nCol, LOGFONT* lf);
	LOGFONT* GetItemFont(int nRow, int nCol) const;

	int  m_scrollPos;

	CArray<int,int> 	m_nFixedRowIndex;
//Marenan +
	//int  m_scrollHPos;
	//int  m_scrollVPos;
	//BOOL m_bKeyMessage;       //是否是键盘触发的事件
	//BOOL m_bInnerHScroll;      //是否用内部水平滚动条
	//BOOL m_bInnerVScroll;      // 是否用内部垂直滚动条
	//CXScrollBar *m_pHScroll;
	//CXScrollBar *m_pVScroll;
//Marenan +

// Operations
public:
    int  InsertColumn(LPCTSTR strHeading, UINT nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE,
                      int nColumn = -1);
    int  InsertRow(LPCTSTR strHeading, int nRow = -1);
    BOOL DeleteColumn(int nColumn);
    BOOL DeleteRow(int nRow);
    BOOL DeleteNonFixedRows();
    virtual BOOL DeleteAllItems();

    BOOL AutoSizeRow(int nRow);
    BOOL AutoSizeColumn(int nCol);
    void AutoSizeRows();
    void AutoSizeColumns();
    void AutoSize();
    void ExpandColumnsToFit();
    void ExpandRowsToFit();
    void ExpandToFit();

    CSize GetTextExtent(LPCTSTR str, BOOL bUseSelectedFont = TRUE);
    void EnsureVisible(CCellID &cell)       { EnsureVisible(cell.row, cell.col); }
    void EnsureVisible(int nRow, int nCol);
    BOOL IsCellVisible(int nRow, int nCol) const;
    BOOL IsCellVisible(CCellID cell) const;
    BOOL IsCellEditable(int nRow, int nCol) const;
    BOOL IsCellEditable(CCellID &cell) const;

    // SetRedraw stops/starts redraws on things like changing the # rows/columns
    // and autosizing, but not for user-intervention such as resizes
    void SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars = FALSE);
    BOOL RedrawCell(int nRow, int nCol, CDC* pDC = NULL);
    BOOL RedrawCell(const CCellID& cell, CDC* pDC = NULL);
    BOOL RedrawRow(int row);
    BOOL RedrawColumn(int col);

    void Print(); 
    BOOL Save(LPCTSTR filename);
    BOOL Load(LPCTSTR filename);

    CCellRange GetCellRange() const;
    CCellRange GetSelectedCellRange() const;
    void SetSelectedRange(const CCellRange& Range, BOOL bForceRepaint = FALSE);
    void SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol,
                          BOOL bForceRepaint = FALSE);
    BOOL IsValid(int nRow, int nCol) const;
    BOOL IsValid(const CCellID& cell) const;
    BOOL IsValid(const CCellRange& range) const;

    // Clipboard and cut n' paste operations
    virtual void CutSelectedText();
    virtual COleDataSource* CopyTextFromGrid();
    virtual BOOL PasteTextToGrid(CCellID cell, COleDataObject* pDataObject);

    void OnBeginDrag();
    DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
    void OnDragLeave();
    BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);

    virtual void OnEditCut();
    virtual void OnEditCopy();
    virtual void OnEditPaste();
    virtual void OnEditSelectAll();

    CCellID GetNextItem(CCellID& cell, int nFlags) const;

    BOOL SortTextItems(int nCol, BOOL bAscending);
    virtual BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data = 0);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGridCtrl)
    protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL
    public:
    virtual void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
    virtual void OnPrint(CDC *pDC, CPrintInfo *pInfo);
    virtual void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);

// Implementation
public:
//add by Xia Qiankun
//this function connect it with the view for real rows
	void SetRealRow(int nRows);

	//int DrawGridLine(COLORREF m_color);
    int CGridCtrl::DrawGridLine(CDC *pDC,COLORREF m_color,int nFixedColWidth,int minVisibleRow,
							int maxVisibleRow,int nFixedRowHeight,CRect VisRect,int minVisibleCol,int maxVisibleCol);
	virtual ~CGridCtrl();

protected:
    BOOL RegisterWindowClass();
    LRESULT SendMessageToParent(int nRow, int nCol, int nMessage);

    BOOL InvalidateCellRect(const CCellID& cell);
    BOOL InvalidateCellRect(const CCellRange& cellRange);
    void EraseBkgnd(CDC* pDC);

    BOOL GetCellRangeRect(const CCellRange& cellRange, LPRECT lpRect) const;

//    CGridCell* GetCell(int nRow, int nCol) const;
    BOOL SetCell(int nRow, int nCol, CGridCell* pCell);

    int  SetMouseMode(int nMode) { int nOldMode = m_MouseMode; m_MouseMode = nMode; return nOldMode; }
    int  GetMouseMode() const    { return m_MouseMode; }

    CCellID GetCellFromPt(CPoint point, BOOL bAllowFixedCellCheck = TRUE) const;
    CCellID GetTopleftNonFixedCell() const;
    CCellRange GetUnobstructedNonFixedCellRange() const;
    CCellRange GetVisibleNonFixedCellRange(LPRECT pRect = NULL) const;

    CCellID SetFocusCell(CCellID cell);
    CCellID SetFocusCell(int nRow, int nCol);

    void ResetSelectedRange();
    void ResetScrollBars();
    int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
    BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);

    virtual BOOL SortTextItems(int nCol, BOOL bAscending, int low, int high);
    BOOL SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data, 
                   int low, int high);

// Overrrides
protected:
    // Printing    
    virtual void PrintColumnHeadings(CDC *pDC, CPrintInfo *pInfo);
    virtual void PrintHeader(CDC *pDC, CPrintInfo *pInfo);
    virtual void PrintFooter(CDC *pDC, CPrintInfo *pInfo);

    // Drag n' drop
    virtual CImageList* CreateDragImage(CPoint *pHotSpot);    // no longer necessary

    // Mouse Clicks
    virtual void  OnFixedColumnClick(CCellID& cell);
    virtual void  OnFixedRowClick(CCellID& cell);

    // Editing
    virtual CSize GetCellExtent(int nRow, int nCol, CDC* pDC);
    virtual void  OnEndEditCell(int nRow, int nCol, CString str);
    virtual void  OnEditCell(int nRow, int nCol, UINT nChar);
    virtual void  CreateInPlaceEditControl(CRect& rect, DWORD dwStyle, UINT nID,
                                           int nRow, int nCol,
                                           LPCTSTR szText, int nChar);

    // Drawing
    virtual void  OnDraw(CDC* pDC);
    virtual BOOL  DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk=FALSE);
    virtual BOOL  DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk=FALSE);

    // GridCell Creation and Cleanup
    virtual CGridCell* CreateCell(int nRow, int nCol);
    virtual void EmptyCell(CGridCell* pCell, int nRow, int nCol);

// Attributes
protected:
    // General attributes
///zjh     /////////I add a Grilline varible///
	COLORREF    m_crTextColour, m_crTextBkColour, m_crBkColour,   // Grid colours
                m_crFixedTextColour, m_crFixedBkColour, m_crGridColour,m_GridLineColor;
    COLORREF    m_crWindowText, m_crWindowColour, m_cr3DFace,     // System colours
                m_crShadow; 
  	COLORREF    m_crCursor;


    int         m_nGridLines;
    BOOL        m_bEditable;
    BOOL        m_bModified;
    BOOL        m_bAllowDragAndDrop;
    BOOL        m_bListMode;
    BOOL        m_bAllowDraw;
    BOOL        m_bEnableSelection;
    BOOL        m_bSortOnClick;
    BOOL        m_bHandleTabKey;
    BOOL        m_bDoubleBuffer;
    BOOL        m_bTitleTips;

    // Cell size details
    int         m_nRows, m_nFixedRows, m_nCols, m_nFixedCols;
    CUIntArray  m_arRowHeights, m_arColWidths;
    int         m_nMargin;
    int         m_nDefCellWidth, m_nDefCellHeight;
    int         m_nVScrollMax, m_nHScrollMax;

    // Fonts and images
    LOGFONT     m_Logfont;
    CFont       m_PrinterFont,  // for the printer
                m_Font;         // for the grid
    CImageList* m_pImageList;

    // Cell data
//    CTypedPtrArray<CObArray, GRID_ROW*> m_RowData;

    // Mouse operations such as cell selection
    int         m_MouseMode;
    CPoint      m_LeftClickDownPoint, m_LastMousePoint;
    CCellID     m_LeftClickDownCell, m_SelectionStartCell;
    CCellID     m_idCurrentCell;
    int         m_nTimerID;
    int         m_nTimerInterval;
    int         m_nResizeCaptureRange;
    BOOL        m_bAllowRowResize, m_bAllowColumnResize;
    int         m_nRowsPerWheelNotch;
//    CMap<DWORD,DWORD, CCellID, CCellID&> m_SelectedCellMap, m_PrevSelectedCellMap;

#ifdef GRIDCONTROL_USE_TITLETIPS
    CTitleTip   m_TitleTip;             // Title tips for cells
#endif

    // Drag and drop
	BOOL		m_bMustUninitOLE;		// Do we need to uninitialise OLE?
    CCellID     m_LastDragOverCell;
#ifdef GRIDCONTROL_USE_OLE_DRAGDROP
    CGridDropTarget m_DropTarget;       // OLE Drop target for the grid
#endif

    // Printing information
    CSize       m_CharSize;
    int         m_nPageHeight;
    CSize       m_LogicalPageSize,      // Page size in gridctrl units.
                m_PaperSize;            // Page size in device units.

    // sorting
    int         m_bAscending;
    int         m_SortColumn;

//add by Xia Qiankun
//this value is the really rows of the view set
	int         m_nRealRow;

protected:
//add by Xia Qiankun
//this function set the sort arrow
	void SetSortArrow(CDC *pDC, CRect rect, BOOL UpDown);
	void SetMenuArrow(CDC *pDC, CRect rect,COLORREF forecolor);

    void SelectAllCells();
    void SelectColumns(CCellID currentCell);
    void SelectRows(CCellID currentCell);
    void SelectCells(CCellID currentCell);
    void OnSelecting(const CCellID& currentCell);

    // Generated message map functions
    //{{AFX_MSG(CGridCtrl)
    afx_msg void OnPaint();
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg UINT OnGetDlgCode();
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnSysColorChange();
    afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnReportAve();

	//}}AFX_MSG
#if _MFC_VER >= 0x0421
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
#endif
    afx_msg LRESULT OnSetFont(WPARAM hFont, LPARAM lParam);
    afx_msg LRESULT OnGetFont(WPARAM hFont, LPARAM lParam);
    afx_msg void OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()

    enum eMouseModes { MOUSE_NOTHING, MOUSE_SELECT_ALL, MOUSE_SELECT_COL, MOUSE_SELECT_ROW,
                       MOUSE_SELECT_CELLS, MOUSE_SCROLLING_CELLS,
                       MOUSE_OVER_ROW_DIVIDE, MOUSE_SIZING_ROW, 
                       MOUSE_OVER_COL_DIVIDE, MOUSE_SIZING_COL,
                       MOUSE_PREPARE_EDIT, MOUSE_PREPARE_DRAG, MOUSE_DRAGGING};

public:
   void StockDataDisplay();
   void OnDrawData(CDC *pDC);
   BOOL DrawCellData(CDC* pDC, int nRow, int nCol, CRect rect);
   UINT GetTopleftNonFixedRow() const;
   BOOL RedrawNoFixedRows(int row);
   void AddFirstRow();
   void SetSortColumn(int SortColumn,BOOL bAscending);
   int  GetSortColumn() { return m_SortColumn ;}
   BOOL  GetSortAscend() { return m_bAscending ;}
   int  GetRealRow() const    { return m_nRealRow; }
   void OnSortForOnTimer(int SortColumn,BOOL Ascend);
   void SetGridColor(COLORREF BkColor,COLORREF LineColor,COLORREF CursorColor);
   void SetHotKeySelect(int SortColumn);
   BOOL SortTextItemsForId(int nCol, BOOL bAscending) ;
   BOOL SortTextItemsForId(int nCol, BOOL bAscending, int low, int high);
   int GotoLine(int RowNum,BOOL IsSamePage);
   void SetFocusRow(int RowNum);
   long GetCurrentRow();
   BOOL m_CurrrSortClick;
   CCellID  m_CurrSortCell;
//19991106
   virtual BOOL GetCell(CGridCell* pGridCell,int nRow,int nCol ) const;
public:
	void SetBackgroundClr(COLORREF clr);
	CPoint GetPoint(int nCol);
	void OnLeftButton(CPoint point);
//   CReportData *pData;
   GCOL_ITEM  *m_pColData; 
   CTaiShanDoc *m_pDoc;
   BOOL m_bRefresh;
   void SetMainDocument(CTaiShanDoc *pDocument);
   virtual float GetItemTextV(int nRow,int nCol);

   void SetColumnInfo(int nCol,UINT nFormat,char *szText,UINT nID,BOOL nIsIndex=FALSE );
   UINT GetColumnId(int nCol){return m_pColData[nCol].nID; }
   void ClearSortColumn();
   void NewReDrawRow();
   long m_BeforeRow;


//   void SetGridDataForRow(CReportData *Cdat,int nRow);
private:
	COLORREF m_clrBackGround;
	void SetVolString(CGridCell *pGridCell, CReportData *pData, float fVol,float fPrice) const;
	void SetPriceString(CGridCell* pGridCell,CReportData* pData,float fPrice) const;
	afx_msg void OnReportMax();
	afx_msg void OnReportMin();
	afx_msg void OnReportSet();
	afx_msg void OnReportVar();
};
inline void CGridCtrl::ClearSortColumn()
{
 //   m_SelectedCellMap.RemoveAll();
	//m_PrevSelectedCellMap.RemoveAll();

}
inline void  CGridCtrl::SetColumnInfo(int nCol,UINT nFormat,char *szText,UINT nID,BOOL nIsIndex)
{
	if(nCol>=m_nCols)
		return;
	m_pColData[nCol].col=nCol;
    m_pColData[nCol].nFormat = (nFormat&(~DT_END_ELLIPSIS)|DT_RIGHT)&(~DT_CENTER);
	m_pColData[nCol].nID=nID;
	m_pColData[nCol].IsIndex=nIsIndex;

	strcpy(m_pColData[nCol].szText,szText);
}
inline void  CGridCtrl::SetSortColumn(int SortColumn ,BOOL bAscending)
{
   m_SortColumn=SortColumn;
   m_bAscending=bAscending;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDCTRL_H__519FA702_722C_11D1_ABBA_00A0243D1382__INCLUDED_)
