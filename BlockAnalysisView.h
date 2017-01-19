#pragma once

class CBlockPageWnd;
class CSheetScrollBar;
class CBlockAnalysisGridCtrl;

// CBlockAnalysisView 视图
#define BOTTOM_HIGHT	20
#define ID_CGRIDLIST	5001
#define ID_HSCROLLBAR	5002
#define ID_GPHQXS		5003


#define MOUSE_ALL		7001
#define MOUSE_COL		7002
#define MOUSE_ROW		7003
#define MOUSE_CELLS		7004
#define MOUSE_NOTHINGS	7005
#define SYSTEMHEAD		1000

class CBlockAnalysisView : public CView
{
	DECLARE_DYNCREATE(CBlockAnalysisView)

protected:
	CBlockAnalysisView();           // 动态创建所使用的受保护的构造函数
	virtual ~CBlockAnalysisView();

public:
	CBlockAnalysisGridCtrl *m_pGrid;  //define CGrid pointer

	//CPageWnd *m_pPageWnd; //define the object of CPageWnd

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	int		m_nCols;
	int		m_nFixRows;
	int		m_nFixCols;
	int		m_nRows ;
	CFont	m_nFont;

	POINT   m_ptBarPos;       //scroll bar Pos to placed 
	BOOL	m_bListMode;      //display mode
	int     m_nBarLen;      //scroll bar length  

	CRect   m_rcBar;
	UINT	m_VisibleRowCount;                            //可视行数据
	UINT	m_VisibleColumn;

	CTaiShanDoc*		m_pDoc;
	CBlockPageWnd*		m_pPageWnd; //define the object of CPageWnd
	CSheetScrollBar*	m_pScrollBar; //define the object of CPageWnd

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnGridColorChange(WPARAM wParam, LPARAM lParam);

	CTaiShanDoc* GetDocument();

	BOOL InitGridContent( );  //初始化Grid
	void InitGridColumnTitle();     //初始化GridCtrl列名
	BOOL SetShowData(int nBlockCount);
	void ChangeToPage(int nPage);

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


#ifndef _DEBUG  // debug version in NineShowView.cpp
inline CTaiShanDoc* CBlockAnalysisView::GetDocument() { return (CTaiShanDoc*)m_pDocument; }
#endif
