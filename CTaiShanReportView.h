// Stock2000View.h : interface of the CTaiShanReportView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOCK2000VIEW_H__13C4E5B0_D3B4_11D1_BF39_0000E86564F7__INCLUDED_)
#define AFX_STOCK2000VIEW_H__13C4E5B0_D3B4_11D1_BF39_0000E86564F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XScrollBar.h"    //cjg
#include "ColumnSelect.h"
#include "cellrange.h"


struct TitleMenuDef {
   UINT  m_nString;                  // color name (in string table)
   UINT     m_nID;                      // menu ID
};
class CPageWnd;
class CGridCtrl;
class CChildFrame;
class CSheetScrollBar;
class CColumnSelect;

class CHistoryDlg;
class CTaiShanReportView : public CView
{
protected: // create from serialization only
	CTaiShanReportView();
	DECLARE_DYNCREATE(CTaiShanReportView)

// Attributes
public:
	CTaiShanDoc* GetDocument();
    CChildFrame *m_ChildFrame;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiShanReportView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ChangeToPage(int nPage,BOOL NewTjxg=FALSE);
	void InitGridColumnTitle();
	void OnF6();	//切换到自选股页面
	virtual ~CTaiShanReportView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
public:
//hyt
	CHistoryDlg*	m_pHistoryDlg;
	CColumnSelect *	m_pColumnSelect;
	CGridCtrl *		m_pGrid;  //define CGrid pointer
	//CXScrollBar * m_pVScrollBar;
	//CXScrollBar * m_pHScrollBar;
	CPageWnd *		m_pPageWnd; //define the object of CPageWnd
	CMenu *			m_pPopMenu;
	CSheetScrollBar*	m_pScrollBar; //define bottom horz scroll bar

	TitleMenuDef		m_TitleMenuDef[68];   //用于记录GridCtrl列代号
	CCellID				m_CellTitleCommand;        //
	int					m_nFirst;


	void RefreshData();                //更新Grid数据
	void OnDayLine();

// Generated message map functions
protected:
	BOOL InitGridContent();  //初始化Grid
	//{{AFX_MSG(CTaiShanReportView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnViewDatamanage();
	afx_msg void OnViewInvestmanage();
	//afx_msg void OnAddChoose(UINT nID);
	afx_msg void OnDeleteStocktype();
	afx_msg void OnOpenDocument();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLoopDisplay();
	afx_msg void OnUpdateLoopDisplay(CCmdUI* pCmdUI);
	afx_msg void OnFontChange();
	afx_msg void OnRealTime();
	afx_msg void OnCompanyinfo();
	afx_msg void OnFileOpen1();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSaveStocktype();
	afx_msg void OnChangetype();
	afx_msg void OnOpenStock() ;
	afx_msg void OnStockType1();
	afx_msg void OnStockType2();
	afx_msg void OnStockType3();
	afx_msg void OnStockType4();
	afx_msg void OnOpenNineShow();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpdateStockType2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStockType3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStockType4(CCmdUI* pCmdUI);
	afx_msg void OnHistoryOpen();
	afx_msg void OnUpdateHistoryOpen(CCmdUI* pCmdUI);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void SwitchFinance() ;
	//}}AFX_MSG
	afx_msg LRESULT OnSetScrollBarPos(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT HotKeyProgress(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKlineToThis(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStockChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnChange(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnIndexColumnChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFixedColNumChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDisplayStockTypeChange(UINT nID);
    afx_msg void OnUpdateDisplayStockType(CCmdUI* pCmdUI);

//

	DECLARE_MESSAGE_MAP()
private:
	int     m_nStartColumn; //define the visible first column (can be move in east-west direction)
//xqk
 	int     m_nCols; //the total num of column, if user delete or add a column , it will ++ / --
	int		m_nRows; //the total num of rows.

	CImageList m_ImageList;   //图象列表
	CStatic	m_GridRect;       //Grid of Rect
	int		m_nFixCols;       //column of Fixed
	int		m_nFixRows;       //row of Fixed

	POINT   m_ptBarPos;       //scroll bar Pos to placed 
	BOOL	m_bListMode;      //display mode
	int     m_nBarLen;      //scroll bar length  
	CRect   m_rcBar;
    UINT    m_TimeCount;

	CRect   m_rcVSB;
	CRect   m_rcHSB;


	CStringArray m_nColumnTitleArray; //define an array to keep the column title string
	UINT m_VisibleRowCount;                            //可视行数据
	UINT m_VisibleColumn;
	BOOL m_LoopDisplay;

public:
	void SetColumnWidth();
	void GetColumnWith();
	static bool IsBg(int kind);
	void SetTypeToDocumentLmb(int nStockType,bool bSetPageWnd = true);
	int GetTotalRowsLmb(int nActive);
	int GetStockTypeFromPageWnd(int nActive);
	void SetHistoryColor(bool bHistory);
	void IndexSortLi();
	CString GetCurrentSymbol();
	void ActiveGrid();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
						  AFX_CMDHANDLERINFO* pHandlerInfo);  //处理弹出式菜单命令信息
	void DoUpdateColumn(UINT nID);                            //更新一行数据 
    int InitGridCtrl();                                       //初始化GridCtrl
    void ClearStockRow(long nRow);                             //清除一行数据
	void AddGridRow(UINT nPage);                               //增加一行
	void AddColumn(UINT m_Column,UINT nID);
    void UpdateColumnData(CTaiShanDoc* pDoc,int m_Column,int FirstRow,int EndRow);
    void UpdateColumn(UINT m_Column,UINT nID);
	void ChangeToChoose(CTaiShanDoc* pDoc,CString m_StockType);
	BOOL ChangeToBlock(CTaiShanDoc* pDoc,CString m_StockType) ;
    void PopMenuItemInit(CMenu *pMenuContext,UINT nRow);
	UINT GetActivePage();
	long FindStockInNameSymbor(UINT nType,char *zqdm);
    void ScrollToSelectStock(char *zqdm,int nKind);
    void HotKeyDownProgress(char  *HotKey);
	void SortStockIndex(UINT nPage,UINT nID);
    void DisplaySortDataInPage(SORTSTRUCT *m_pStockData,int DisplayCount);
    UINT GetSortColumn(UINT nID);
    void ScrollToPageOfStock(CTaiShanDoc* pDoc,int nPage,CString m_findzqdm);
	void DisplayDpStockRow(CTaiShanDoc* pDoc,CReportData *Cdat ,long nRow);
    void LoopDisplayOnTimer();
	void ChangeToTjxg(CTaiShanDoc* pDoc);
	BOOL ChangeToTjxg(CTaiShanDoc* pDoc,CString m_StockType );
    LRESULT OnGridColorChange(WPARAM wParam, LPARAM lParam);
	void SortDataForColumn();
    BOOL SetShowData(CTaiShanDoc *pDoc,int StockCount);
    void LoadShowData(int nKind);
	int  GetPageRows();
	BOOL GetStockCodeForRow(int nRows,char *Code,int &nKind);
	BOOL GetRowForStockCode(char * Code,int & nRow);
	void SortDataForPage8();
	void ChangedColumnType(int ColumnType);
    void DefalutColumnSelect();
	void OnAddStocktype(UINT nID);
	void OnAddChoose(UINT nID);
    void OnDeleteTjxgStockType();
	static void OnSaveScreenStocktype(SymbolKindArr& StockCodeArray ,CString *StockName=NULL); 
    void ChangeToTjxg(CTaiShanDoc* pDoc,BOOL IsNewTjxg);
    BOOL GetCdatPointForRow(int nRow,PCdat1 &pStockData);
    void AddChoose(char *StockCode,int nKind);
#ifdef WIDE_NET_VERSION
	TRANS_PACKAGE_STRUCTEx m_TransPackageStruct;
	int m_nMinuteCount;
	SOCKET_ID m_SocketID;
	int n_nSendStockCount;
	TransmitStockAttribute  *m_pTransmitStockAttribute;
    void SendRecvDataInfomation(ClientTransmitAttributes  TransmitType);
    BOOL SetSendStockCodeData(int SendStockCount);
#endif
	
public:
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pBar);
protected:
	virtual void OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

public:

	int GetFixedRowCount(void);
};
#ifndef _DEBUG  // debug version in Stock2000View.cpp
inline CTaiShanDoc* CTaiShanReportView::GetDocument()
   { return (CTaiShanDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOCK2000VIEW_H__13C4E5B0_D3B4_11D1_BF39_0000E86564F7__INCLUDED_)
