// NineShowView.h : interface of the CNineShowView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NINESHOWVIEW_H__BBF6B485_3609_11D4_8829_00400552E583__INCLUDED_)
#define AFX_NINESHOWVIEW_H__BBF6B485_3609_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MOUSE_ALL  7001
#define MOUSE_COL  7002
#define MOUSE_ROW  7003
#define MOUSE_CELLS 7004
#define MOUSE_NOTHINGS 7005

typedef struct
{
	CRect rcBig;
	CRect rcTitle;
	char title[50];
	int minutecount;
	BOOL IsDay;
	BOOL bAscending;
	UINT showtype;
	STOCKDATASHOW *pNineData;
}NineShowData;
class CNineGridCtrl;
class CNineShowView : public CView
{
protected: // create from serialization only
	CNineShowView();
	DECLARE_DYNCREATE(CNineShowView)

// Attributes
public:
	int IsQuit;           ////////Is the program want quit
    int FirstMenu;
	CTaiShanDoc *m_pDoc;
	enum {	IDTOTP = 0, IDHLP = 1, IDRDR = 2,IDBSSPBAS=3,
	        IDVOLBI=4 ,IDDVOL=5 ,IDEXCHANGE=6,IDSYL=7,
            IDZJLS=8 ,IDHLPFM=9 ,IDEXCHANGEFM=10,IDADVSP=11
	};
	CStringArray m_typetitle;
	NineShowData m_NineShowData[9];
	UINT m_unSaveMousePoint;
	CNineGridCtrl *m_pGridCtrl[9];
	int m_nShowStockCount;
	int m_nFocusCount;

	int m_nCols;
    int m_nFixRows;
	int m_nFixCols;
    int m_nRows ;
    BOOL m_bSync;
public:
	CTaiShanDoc* GetDocument();
	void GetNineShowTitle(NineShowData *nNineData);
	void SetSortArrow(CDC *pDC,NineShowData *l_NineShowDat);
    BOOL InitGridContent(CNineGridCtrl *pGrid );  //初始化Grid
	void InitGridColumnTitle(CNineGridCtrl *pGrid,NineShowData *nNineShowData);     //初始化GridCtrl列名
	BOOL SetShowData(int StockCount);
	void LoadShowData(int nStockType);
	void SetNineShowFocus(int nShowPos,BOOL IsSetPos=TRUE);
	void SortDataForTechnique(NineShowData *pNineShowData,CNineGridCtrl *pGrid,BOOL IsRefresh=TRUE);
    BOOL ScrollToSelectStock(char *zqdm,int nKInd);
	void SetNineShowSync(BOOL bIsSync);
	BOOL ScrollToSelectStock(int nShowCounts,char *zqdm,int nKind);
    LRESULT OnGridColorChange(WPARAM wParam, LPARAM lParam);
    CString GetStockCodeForDisplayKLine();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNineShowView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNineShowView();
    void SetStockCodeForDisplayF10();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	//like DrawText()
	void ShowOneData(CDC *pDC, CString str, CRect rect, COLORREF Color,UINT nFormat);
	CString TurnDigitToStr(int nDig);
// Generated message map functions
protected:
	//{{AFX_MSG(CNineShowView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnStockTypeSelect();
	afx_msg void OnSync();
	afx_msg void OnSzeb();
	afx_msg void OnStocktype();
	afx_msg void OnUpdateShstocka(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSync(CCmdUI* pCmdUI) ;
	afx_msg void OnUpdateStocktype(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSzeb(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnRefreshNineShow(WPARAM wPara,LPARAM lPara);
 	afx_msg void OnStockTypeChoose(UINT nID);
    afx_msg void OnTechniqueIndexChoose(UINT nID);
	afx_msg void OnSetTimeInter();
	afx_msg LRESULT OnKeyboardGenius(WPARAM wPara, LPARAM lPara);
    afx_msg LRESULT OnNineShowRowChanged(WPARAM wPara, LPARAM lPara); 




	DECLARE_MESSAGE_MAP()
private:
	//define the client area
	CRect m_Rect;
	//define current active list and item
	int m_nActiveList;
	int m_nActiveItem;
	//define the stock type: Shanghai A,Shanghai B,Shenzhen A,ShenzhenB
	int m_nSharesType;
	//define the variable time inter user hoped
	int m_nTimeInter;
    //define each listbox's title
//	CStringArray m_nTitle;
	//define the text font to be shown
	CFont m_nFont;
	//define the interface color
	COLORREF m_BackColor;  //background color
	COLORREF m_TitleColor; //title color
	COLORREF m_StockNameColor; //stock name color
	COLORREF m_OtherDataColor; //other data color
	COLORREF m_StockUpColor;   //stock up color
	COLORREF m_StockDownColor; //stock down color 
	COLORREF m_StockStopColor; //stock stop up or down color
	COLORREF m_CursorColor;    //The High Selected Color
#ifdef WIDE_NET_VERSION
	TRANS_PACKAGE_STRUCTEx m_TransPackageStruct;
	SOCKET_ID m_SocketID;
	void SendRecvDataInfomation();
#endif

};

#ifndef _DEBUG  // debug version in NineShowView.cpp
inline CTaiShanDoc* CNineShowView::GetDocument()
   { return (CTaiShanDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NINESHOWVIEW_H__BBF6B485_3609_11D4_8829_00400552E583__INCLUDED_)
