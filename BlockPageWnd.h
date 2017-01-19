/********************************************************
  This file need defined parameters in the  App header:
 
   #define MAX_PAGE 10  //the page limited 

  Designed by Xia Qiankun
  1999.4.10
*********************************************************/

#if !defined(AFX_MYWND_H__DCABFF9D_9ABF_11D1_8C3A_0000E823CF90__INCLUDED_)
#define AFX_MYWND_H__DCABFF9D_9ABF_11D1_8C3A_0000E823CF90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageWnd.h : header file


//define message ID for connect with the view
#define WM_SHOWTYPE  (WM_USER + 100)
#define MAX_PAGE 64

#include "BCMenu.h"

/////////////////////////////////////////////////////////////////////////////
// CPageWnd window

struct DynaMenuDef {
   CString 	m_nString;					// color name (in string table)
   UINT     m_nID;                      // menu ID
};

typedef CTypedPtrArray<CPtrArray,DynaMenuDef*> m_DynaMenuDef;
class CSheetScrollBar;
class CBlockPageWnd : public CWnd
{
// Construction
public:
	CBlockPageWnd();

///////////Modi by zhangjihai
public:
	//Marenan -
	CSheetScrollBar * m_pScrollBar; //another scrollbar in the view
	//Marenan +
	//CXScrollBar*  m_pScrollBar;

	int		m_nActivePage; //define the active page 
	UINT	m_nBarLen;
	bool	m_bMove;
	CFont	m_Font;

/////////////////////////////

// Attributes
public:
/////////////add by zhang
	int		m_nMovebar; //if the mouse point in the splitter of scrollbar(horz direction) of the view
    CPoint	m_ptBarStartPos; //define the begin position of the view horiz scrollbar
	CPoint	m_ptBarEndPos;   //define the end   position of the view horiz scrollbar
	int		m_nBarlength;  //define the view horiz scrollbar length
   	BOOL	m_bRefresh; 

////////////

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlockPageWnd)
	public:
	virtual BOOL Create(LPCTSTR szTitle, LONG style, const RECT& rect, CWnd* parent);
	//}}AFX_VIRTUAL

// Implementation
public:	
	bool DoLButtonDown(UINT nFlags, CPoint points);
	void DoRButtonDown(UINT nFlags, CPoint point);
	//Replace the exists page(nPage) with the title ReplaceTitle
	void ReplacePage(int nPage,CString ReplaceTitle);
	//show all page content
	void ShowAllPage(CDC *pDC);
	CString GetPageTitle(int nPage); //get the title of the "npage" page
	int GetActivePage(); //return which page is active
    void SetPageTitle(CString m_title,int index);
	int	GetPageCount();
	virtual ~CBlockPageWnd();

	// Generated message map functions
protected:
    //Draw the page frame box
	void DrawPageBox(CDC *pDC,int x,int cx,BOOL bActive);
	CScrollBar  m_PageBtn;//set the bottom scrollbar served for the page move

	//{{AFX_MSG(CBlockPageWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//flag of the refrash of all page

    //Initialize all parameters
	void InitAllPara();
	//show the choosed page's title
	void ShowPageTitle(CDC *pDC,CString &str,LPRECT lpRect,UINT nFormat,int nPage);
	CStringArray m_strPageTitle; //the title of each page
	//the first page to be draw  position
	int m_nBeginX;
    //visible first page 
	int m_nFirstPage;
	//visible last page
	int m_nEndPage;
    //each page's width
	int PageWidth[MAX_PAGE];//

	m_DynaMenuDef m_StockType;
	CMenu *m_TypeMenu;

	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,AFX_CMDHANDLERINFO* pHandlerInfo);
	void ShowRedrawPage(CDC * dc );


}; 

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYWND_H__DCABFF9D_9ABF_11D1_8C3A_0000E823CF90__INCLUDED_)
