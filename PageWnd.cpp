/////////////////////////////////////////////////////////
// PageWnd.cpp : implementation file
// this file is designed to show MAX_PAGE page in a wnd
// each page can move in east & west direction
/////////////////////////////////////////////////////////
//
// this file is designed by Xia Qiankun
// 1999/4/10
// if have any bug, pls Inform me
// Tel:64397490-564/565
// e-mail:x_qk@netease.com
///////////////////////////////////////////////////////////
//
// the method to use this class
//
// 1.create an object of CPageWnd
// 2.call CPageWnd::Create(....)
// 3.deal with the message from CPageWnd
////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WH.h"
#include "Memdc.h"
#include "SheetScrollBar.h"
#include "PageWnd.h"
#include "mainfrm.h"
#include "CTaiShanDoc.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"
#include "managerchoose.h"
#include "BtnST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BOTTOM_HIGHT		20      //the bottom hight
#define DEFAULTFONTWIDTH	10   //Default page title font multiple coeff..

#define IDC_PAGEBUTTON		40021 //the id of the scrollbar
#define IDC_WNDIDNO			40022 //The id of the pagewnd 

#define DEFAULTFORMAT		DT_CENTER|DT_VCENTER|DT_SINGLELINE  //the default text show format
#define SYSTEMHEAD			1000
#define LEFT_LENG			BOTTOM_HIGHT*2

/////////////////////////////////////////////////////////////////////////////
// CPageWnd

//this function is for pop menu used in any where.
//the wnd is a handle where you want to pop a menu, default call MainWnd
//id is the resources  of a menu
//designed by Xia Qiankun
//if have any bug , pls ask me
void CPageWnd::ExecuteMenu(int mode)
{

	CPoint posMouse;
	//::GetCursorPos(&posMouse);
	int pointx = m_nBeginX;
	for (int i = 0;i < m_nActivePage;i++)
	{
		pointx += PageWidth[i];
	}

	CRect rcWnd;
	GetWindowRect(rcWnd);

	posMouse.x = rcWnd.left + pointx;
	posMouse.y = rcWnd.top;
	
	InitTypeMenu(mode) ;
	m_TypeMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,posMouse.x,posMouse.y,this);
}

CPageWnd::CPageWnd()
{
	//Initialize all parameters
	m_pScrollBar = NULL;//new CScrollBar;
    m_nMovebar = 0;
	m_nBarlength = 250;
	m_nBarLen = m_nBarlength;
	m_TypeMenu=NULL;
	m_bMove = FALSE;

}

CPageWnd::~CPageWnd()
{
	m_Font.DeleteObject();
	if(m_TypeMenu) delete m_TypeMenu;
}


BEGIN_MESSAGE_MAP(CPageWnd, CWnd)
	//{{AFX_MSG_MAP(CPageWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPageWnd message handlers

BOOL CPageWnd::Create(LPCTSTR szTitle, LONG style, const RECT& rect, CWnd* parent) 
{
	//register a wnd class
	LPCTSTR lpszBounceClass =
		AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
			LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)(COLOR_WINDOW+1),
			NULL);
	return CWnd::Create(lpszBounceClass, szTitle, style, 
		rect, parent, IDC_WNDIDNO);
}

int CPageWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	//creat a scrollbar , horz direction ,at the left
    CRect rect;
	GetClientRect(rect);
	CSize size(2*BOTTOM_HIGHT + 1,BOTTOM_HIGHT);
	//m_PageBtn.Create(SBS_HORZ|WS_CHILD|WS_VISIBLE,CRect(CPoint(rect.left,rect.top),size),this,IDC_PAGEBUTTON);
    
	InitAllPara();
	m_TypeMenu = new BCMenu;
	HMENU m_hmenu=CreatePopupMenu();
	m_TypeMenu->Attach( m_hmenu );

	LOGFONT m_lf;
	memset(&m_lf,0,sizeof(LOGFONT));
	m_lf.lfHeight = -12;
	m_lf.lfWidth=0;
	m_lf.lfWeight=400;
	m_lf.lfCharSet=134;
	strcpy(m_lf.lfFaceName,"宋体");


	m_Font.CreateFontIndirect(&m_lf);
	return 0;
}


//Initialize all parameters of the class
void CPageWnd::InitAllPara()
{
    CTaiShanReportView* pView = (CTaiShanReportView*)GetParent();
    CTaiShanDoc *pDoc = pView->GetDocument();

	m_nBeginX = BOTTOM_HIGHT * 0;
	//m_nBeginX = BOTTOM_HIGHT * 2;
	m_nActivePage=0;
	m_bRefresh=TRUE;
 	CString str;
	str.Format("分类");
	m_strPageTitle.Add(str);
	str.Format("上指");
	m_strPageTitle.Add(str);
	str.Format("深指");
	m_strPageTitle.Add(str);
	str.Format("A 股");
	m_strPageTitle.Add(str);
	str.Format("中小");
	m_strPageTitle.Add(str);
	str.Format("创业");
	m_strPageTitle.Add(str); 
//	str.Format("B 股");
    str.Format("ALL"); // page-6
	m_strPageTitle.Add(str);
	str.Format("权证");
	m_strPageTitle.Add(str);
	str.Format("基金");
	m_strPageTitle.Add(str);
	str.Format("分类板块");
	m_strPageTitle.Add(str);
	str.Format("自选板块");
	m_strPageTitle.Add(str);
	str.Format("条件选股");
	m_strPageTitle.Add(str);
	//init page
	m_nFirstPage=0;
	m_nEndPage=m_strPageTitle.GetSize();
	int i;
	for(i=0;i<m_nEndPage;i++)
	{
		str=m_strPageTitle.GetAt(i);
		if(str.GetLength( )<5)
	      PageWidth[i]=5*DEFAULTFONTWIDTH;
		else
	      PageWidth[i]=str.GetLength( )*DEFAULTFONTWIDTH;
		PageWidth[i]=8 * DEFAULTFONTWIDTH;
	}
	for(i=m_nEndPage;i<MAX_PAGE;i++)   //Get Page Width
		PageWidth[i]=0;
}

//deal with EraseBkgnd message so the program can refrash the abrupt area
BOOL CPageWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	ShowAllPage((CDC *)&dc);
	ReleaseDC(&dc);
	return 1;
}

void CPageWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
    m_bRefresh=TRUE; //inform the system to refresh all area
 	m_nBeginX = 0;	
	//m_nBeginX = 2*BOTTOM_HIGHT;	
///////////////zjh
	m_ptBarStartPos.x = cx; 
	//m_ptBarStartPos.y = cy;
	//Marenan +
	if (m_pScrollBar)
	{
		//m_pScrollBar->Invalidate();
		m_pScrollBar->SetWindowPos(&wndTop,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
	}
	//Marenan +
}


void CPageWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(m_bRefresh)
	{
		m_bRefresh=FALSE;
		ShowAllPage(&dc);
	}
	ReleaseDC(&dc);

}

void CPageWnd::OnLButtonDown(UINT nFlags, CPoint points) 
{	

	if(!DoLButtonDown( nFlags,  points))
		DoRButtonDown( nFlags,  points);


}


void CPageWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if(pScrollBar->m_hWnd==m_PageBtn.m_hWnd)//if the message is to the m_PageBtn
	{	//recover two arrow show status
		m_PageBtn.EnableScrollBar(ESB_ENABLE_BOTH);
		switch(nSBCode)
		{
			case SB_LINELEFT: //if press left arow
				m_bMove = false;
				m_nFirstPage--;//refresh the visible firstpage 
				if(m_nFirstPage<0)//i
				{
					m_nFirstPage=0;
					//disable the scrollbar to left message
					m_PageBtn.EnableScrollBar(ESB_DISABLE_LTUP);
				}
				else
				{
					//inform system to redraw all message
					m_bRefresh=TRUE;
					Invalidate();
				}
				break;
			case SB_LINERIGHT:// if press right arrow
				m_bMove = false;
				m_nFirstPage++; //refrash the visible firstpage
				if(m_nFirstPage>m_nEndPage-1)
				{
					 m_nFirstPage=m_nEndPage-1;
					 //disable the scrollbar to right message
                     m_PageBtn.EnableScrollBar(ESB_DISABLE_RTDN);
				}
				else
				{
					// inform system to redraw all
					m_bRefresh=TRUE;
					Invalidate();
				}
				break;
		}
	}
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

//return the page title
//return a CString varial
CString CPageWnd::GetPageTitle(int nPage)
{
    CString Result;
    CTaiShanReportView* pView = (CTaiShanReportView*)GetParent();
    CTaiShanDoc *pDoc = pView->GetDocument();

    CString ShMenuStr[]={"上证指数","上证 A 股","上证 B 股","上证基金","上证债券","上证转债","上证回购","上证 ETF ","开放基金","上证权证","上证其它"};
    CString SzMenuStr[]={"深证指数","深证 A 股","深证 B 股","深证基金","深证债券","深证转债","深证回购","深证 ETF ","开放基金","深证权证","三板行情","深证其它"};
	//CString FlMenuStr[]={"上证A股","深证A股","上证B股","深证B股","上证债券","深证债券","深沪A股","深沪B股","深沪债券","深沪基金","深沪权证","深沪指数"};
    switch(nPage)
	{
	case  SHPAGE:  
		if (pDoc->m_nStockTypeShPage >= SHZS && pDoc->m_nStockTypeShPage <= SHOT)
		{
			Result=ShMenuStr[pDoc->m_nStockTypeShPage - SHZS];
			SetPageTitle(Result,SHPAGE);
		}
		break;
	case  SZPAGE:   
		if (pDoc->m_nStockTypeSzPage >= SZZS && pDoc->m_nStockTypeSzPage <= SZOT)
		{
			Result=SzMenuStr[pDoc->m_nStockTypeSzPage - SZZS];
			SetPageTitle(Result,SZPAGE);
		}
         break;
	}
    Result=m_strPageTitle.GetAt(nPage);
	return Result;
}

//show the title in the center 
//the show mode is TRANSPARENT
void CPageWnd::ShowPageTitle(CDC *pDC, CString &str, LPRECT lpRect, UINT nFormat,int nPage)
{
	int nBackMode=pDC->GetBkMode();
    if(nBackMode==OPAQUE)
		pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&m_Font);
	if(nPage==m_nActivePage)//if the page is acitve
		pDC->SetTextColor (RGB(0,0,0));
	//pDC->SetTextColor (CMainFrame::m_pDoc->m_colorArray[13]);
	else
		pDC->SetTextColor (RGB(192,192,192));
	//pDC->SetTextColor (CMainFrame::m_pDoc->m_colorArray[1]);

 	pDC->DrawText(str,lpRect,nFormat);

	if(nBackMode==OPAQUE) 
		pDC->SetBkMode(OPAQUE);
}

//return the active page
int CPageWnd::GetActivePage()
{
    return m_nActivePage;
}

//pDC is a CDC 
//draw the page frame
//x is the position
//cx is the width of a page
//bActive is flag of active or not
void CPageWnd::DrawPageBox(CDC *pDC, int x, int cx, BOOL bActive)
{
	CPoint pPage[5];
	double cxB=0;
	//double cxB=BOTTOM_HIGHT/3.464;
	//set the frame line color
    CPen BlackPen(PS_SOLID,1,CMainFrame::m_pDoc ->m_colorArray[14]);//17
	CPen *OldPen=pDC->SelectObject(&BlackPen);
	//define the points to be draw
    pPage[0]=CPoint(x-cxB,0);
    pPage[1]=CPoint(x+cxB,BOTTOM_HIGHT);
    pPage[2]=CPoint(x+cx-cxB,BOTTOM_HIGHT);
    pPage[3]=CPoint(x+cx+cxB,0); 
	//define brush used to fill / draw 
	CBrush FillBrush(RGB(102,102,102));// a gray brush;RGB(192,192,192)
	CBrush FrameBrush(RGB(152,152,152));//17
	CBrush ActiveBrush(RGB(242,242,242));//RGB(255,255,255));
	//CBrush FillBrush(CMainFrame::m_pDoc ->m_colorArray[18]);// a gray brush;RGB(192,192,192)
	//CBrush FrameBrush(CMainFrame::m_pDoc ->m_colorArray[14]);//17
	//CBrush ActiveBrush(CMainFrame::m_pDoc ->m_colorArray[18]);//RGB(255,255,255));
	CRgn rgn;
	CPen pen(PS_SOLID,1,CMainFrame::m_pDoc ->m_colorArray[18]);
	if(!bActive)//inactive page
	{
 		rgn.CreatePolygonRgn(pPage,4,WINDING);
	    pDC->FillRgn(&rgn,&FillBrush);
		pDC->FrameRgn(&rgn,&FrameBrush,1,1);
		rgn.DeleteObject(); 
	}
	else//active page 
	{
		rgn.CreatePolygonRgn(pPage,4,ALTERNATE);
	    pDC->FillRgn(&rgn,&ActiveBrush);
		pDC->FrameRgn(&rgn,&FrameBrush,1,1);
		//draw the top line to white
		//int pRop2=pDC->SetROP2(R2_WHITE);
		//pDC->MoveTo(pPage[0]);
		//pDC->LineTo(pPage[3]);
		//pDC->SetROP2(R2_COPYPEN);

		//pDC->SelectObject(&pen);
		//pDC->MoveTo(pPage[0].x +1,pPage[0].y);
		//pDC->LineTo(pPage[3].x -1,pPage[3].y);
		rgn.DeleteObject(); 
	}
	pDC->SelectObject(OldPen);

	pDC->SelectObject(OldPen);
}

//show all page information 
//when first run this program
//when redraw
//when resize
//when onpaint
void CPageWnd::ShowAllPage(CDC *pDC)
{
	if(!CMainFrame::m_pDoc)
		return;
	//calculate show area
	CDC dcMem;
	CBitmap bmp;
	CRect ClientRect;
	GetClientRect(&ClientRect);
    CRect m_DrawRect;
	m_DrawRect.SetRect(/*LEFT_LENG+*/1 ,ClientRect.top,ClientRect.right,ClientRect.bottom);
    dcMem.CreateCompatibleDC(pDC);
    bmp.CreateCompatibleBitmap(pDC,m_DrawRect.Width(),m_DrawRect.Height()) ;
    CBitmap *pbmOld=dcMem.SelectObject(&bmp);


 	//fill the background with gray brush;
	CBrush brush(CMainFrame::m_pDoc ->m_colorArray[18]);
//	CBrush brush(CMainFrame::m_pDoc ->m_colorArray[18]);
    CRect m_Draw;
	m_Draw.SetRect(0 ,0,m_DrawRect.Width(),m_DrawRect.Height());

	dcMem.FillRect(m_Draw,&brush);
	CPen pen(PS_SOLID,1,CMainFrame::m_pDoc ->m_colorArray[14]);//17
	CPen* OldPen = dcMem.SelectObject(&pen);
	dcMem.MoveTo(m_DrawRect.left ,m_DrawRect.top );
	dcMem.LineTo(m_DrawRect.right ,m_DrawRect.top);
	dcMem.SelectObject(OldPen);
	
    int nPage=m_nFirstPage,bActivePage=-1;
    CString Title;
	CRect rect,bActiveRect;
	int bActivex=-1;
//	int xBegin=2*BOTTOM_HIGHT;//+BOTTOM_HIGHT/3 ;
	int xBegin=0 ;

	//BJ 20011221
	//m_nBeginX=2*BOTTOM_HIGHT;//+BOTTOM_HIGHT/3;
	m_nBeginX = 0;

	//draw and show visible page
	///////////zjh////////////
	int beginX = PageWidth[nPage] + PageWidth[nPage+1];
	while(nPage < m_nActivePage && m_bMove == true)
	{
	  beginX += PageWidth[nPage+2];
      if(beginX >= m_ptBarStartPos.x)
	  {
		  nPage = m_nFirstPage+1;
          m_nFirstPage = nPage;
		  m_bMove=FALSE;
		  break;
	  }
	  nPage++;
	}
	CString tt;
	tt.Format("%d",m_nFirstPage);
	TRACE(tt);
	if(beginX < m_ptBarStartPos.x)
	{
	   nPage=m_nFirstPage;	
	}
	////////////////////////////////////////
	/*if(nPage >= 8)
	{
	   nPage = 0;
	   m_nFirstPage = 0;
	} */
    if(m_nActivePage == 0 && m_bMove == true)
	{
	  nPage = 0;
	  m_nFirstPage = 0;
	}
	while(nPage < m_nEndPage)
	{
		//get the page title
		Title=GetPageTitle(nPage);
		//draw page frame box
		DrawPageBox(&dcMem,xBegin,PageWidth[nPage],FALSE);
		rect.SetRect(xBegin,0,xBegin+PageWidth[nPage],BOTTOM_HIGHT);
		//show page title
		ShowPageTitle(&dcMem,Title,rect,DEFAULTFORMAT,nPage);
		if(nPage==m_nActivePage)//if the page is active
		{
			bActivex=xBegin;
			bActiveRect=rect;
			bActivePage=nPage;
		}
		//set the next page position
		xBegin+=PageWidth[nPage];
		if(xBegin>ClientRect.right)break;
		nPage++;
	}
	if(bActivex >= 0) //show the active page
	{
		Title=GetPageTitle(bActivePage);
		DrawPageBox(&dcMem,bActivex,PageWidth[bActivePage],TRUE);
		ShowPageTitle(&dcMem,Title,bActiveRect,DEFAULTFORMAT,bActivePage);
	}
   //////////////////Modi by zjh for make page visible

	////////////////////////////////////////////////////
	pDC->BitBlt(0 ,0,ClientRect.right,BOTTOM_HIGHT,&dcMem,0,0,SRCCOPY);
	//pDC->BitBlt(LEFT_LENG+1 ,0,ClientRect.right,BOTTOM_HIGHT,&dcMem,0,0,SRCCOPY);
	
	//Marenna -
	dcMem.SelectObject(pbmOld);

	//Marenna +
	//pDC->BitBlt(LEFT_LENG+1 ,0,m_ptBarStartPos.x,BOTTOM_HIGHT,&dcMem,0,0,SRCCOPY);
	//////////////////////////////////////////////////////
	//pDC->BitBlt(LEFT_LENG+1 ,0,ClientRect.right,BOTTOM_HIGHT,&dcMem,0,0,SRCCOPY);
	//dcMem.SelectObject(pbmOld);
	//m_pScrollBar->SetWindowPos(&wndTop,m_ptBarStartPos.x,m_ptBarStartPos.y,m_ptBarEndPos.x-m_ptBarStartPos.x,m_ptBarEndPos.y-m_ptBarStartPos.y,SWP_SHOWWINDOW);
	//m_pScrollBar->Draw();  
	//Marenna +
	//m_bMove = true;
//	dcMem.DeleteDC();
//    bmp.DeleteObject(); 

}

//replace the page with another page
void CPageWnd::ReplacePage(int nPage,CString ReplaceTitle)
{
 //	int i;
	int xBegin=m_nBeginX;
	CRect rect;
	CClientDC dc(this);    
	m_strPageTitle.SetAt(nPage,ReplaceTitle);
    int ss=ReplaceTitle.GetLength( );
    if(ReplaceTitle.GetLength( )<5)
       PageWidth[nPage]=5*DEFAULTFONTWIDTH;
	else
	   PageWidth[nPage]=ReplaceTitle.GetLength( )*DEFAULTFONTWIDTH;
	
	PageWidth[nPage]=8 * DEFAULTFONTWIDTH;

 	ShowAllPage(&dc);
	ReleaseDC(&dc);
}

//deal with the popupmenu
void CPageWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	DoRButtonDown( nFlags,  point);
	CWnd::OnRButtonDown(nFlags, point);
}

///////////////zjh add the mouse event handle
void CPageWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
   if( m_nMovebar == 1)
   {
	  SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
	  SetCapture();

	  point.x = point.x + 3;
	  m_nBarLen = m_nBarLen + (-point.x + m_ptBarStartPos.x);

	  if(point.x <33)
		  point.x=33;
	  if(point.x +50 > m_ptBarEndPos.x)
         point.x= m_ptBarEndPos.x -50;
	  m_nBarLen = m_nBarLen + (-point.x + m_ptBarStartPos.x);

      MSG msg;

	  SetWindowPos(0,0,m_ptBarStartPos.y,point.x,m_ptBarEndPos.y,SWP_NOREDRAW|SWP_SHOWWINDOW);
	  m_bRefresh=TRUE;
	  Invalidate();
	  if(m_nBarLen > m_nBarlength)
	  if(PeekMessage(&msg,this->GetSafeHwnd() ,WM_PAINT,WM_PAINT,PM_REMOVE))
	  {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	  }

      CRect rc(point.x,m_ptBarStartPos.y,m_ptBarEndPos.x,m_ptBarEndPos.y);
	  m_pScrollBar->SetWindowPos(0,point.x,m_ptBarStartPos.y,rc.Width() ,rc.Height(),SWP_NOREDRAW);
	  //m_pScrollBar->m_bRefresh=TRUE;
	  m_pScrollBar->Invalidate();

	  if(PeekMessage(&msg,m_pScrollBar->GetSafeHwnd() ,WM_PAINT,WM_PAINT,PM_REMOVE))
	  {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	  }

	  m_ptBarStartPos.x = point.x;
      m_nBarLen = m_nBarlength;

   }
   else
   {

     if(point.x > m_ptBarStartPos.x - 5)
	 {
	   SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
       //m_nMovebar = 1;
	 }
     else
	 { 
        m_nMovebar = 0;
	 }
   }
   CWnd::OnMouseMove(nFlags, point);
}

void CPageWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_nMovebar = 0;
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}
void CPageWnd::InitTypeMenu(int mode) 
{
    CTaiShanReportView* pView = (CTaiShanReportView*)GetParent();
    CTaiShanDoc *pDoc = pView->GetDocument();

	CString ShMenuStr[]={"上证指数","上证 A股","上证 B股","上证基金","上证债券","上证转债","上证回购","上证 ETF ","开放基金","上证权证","上证其它"};
	CString SzMenuStr[]={"深证指数","深证 A股","深证 B股","深证基金","深证债券","深证转债","深证回购","深证 ETF ","开放基金","深证权证","三板行情","深证其它"};
	CString FlMenuStr[]={"上证 A股","深证 A股","上证 B股","深证 B股","上证债券","深证债券","深沪 A股","深沪 B股","深沪债券","深沪基金","深沪权证","深沪指数"};
	

//清除菜单项
 //   POSITION pos=m_StockType.GetHeadPosition( );
	//while(pos)
	//{
	//    DynaMenuDef *m_pMenuDef;
	//	m_pMenuDef = m_StockType.GetNext( pos );
 //       m_TypeMenu->DeleteMenu(m_pMenuDef->m_nID,MF_BYCOMMAND);
	//	delete m_pMenuDef;
	//}
	if(m_TypeMenu) 
	{
		delete m_TypeMenu;
		m_TypeMenu = NULL;
	}

	m_TypeMenu = new BCMenu;
	if (!m_TypeMenu)
	{
		return;
	}
	HMENU m_hmenu=CreatePopupMenu();
	m_TypeMenu->Attach( m_hmenu );

	for (int i = 0;i < m_StockType.GetCount();i++)
	{
	    DynaMenuDef *m_pMenuDef;
		m_pMenuDef = m_StockType.GetAt( i );
		//m_TypeMenu->DeleteMenu(m_pMenuDef->m_nID,MF_BYCOMMAND);
		delete m_pMenuDef;
	}
    m_StockType.RemoveAll();

	int index=9000;
	//int index2[STOCKTYPENUM];

	//for (int i = 0;i < STOCKTYPENUM;i++)
	//{
	//	index2[i] = 9000 + i;
	//}
	CStringArray m_BlockTypeNameArray,stockTypeNameArray;
	CArray<int,int> stockTypeCountArray;
	CArray<int,int> RemoveArray;

	switch(mode)
	{
	case CHOOSEPAGE://自选股
		{
			CTaiShanReportView* pView = (CTaiShanReportView*)GetParent();
			CTaiShanDoc *pDoc = pView->GetDocument();
			CStringArray StockArray;
			pDoc->m_ManagerBlockData.GetChooseStockTypeNames(StockArray);
			for(int i=0;i<StockArray.GetCount();i++)
			{
				CString l_sCode=StockArray.GetAt(i);
				DynaMenuDef *m_pMenuDef;
				m_pMenuDef=new DynaMenuDef;
				m_pMenuDef->m_nString=l_sCode;
				m_pMenuDef->m_nID=index++;
				m_StockType.Add(m_pMenuDef);
			}
		}
		break;
	case TJXGPAGE:
		{
			POSITION pos;
			pos=pDoc->m_Tjxgblocklist.GetHeadPosition();
			while(pos)
			{
				BLOCKSTOCK * blocknow=pDoc->m_Tjxgblocklist.GetNext(pos);
				DynaMenuDef *m_pMenuDef;
				m_pMenuDef=new DynaMenuDef;
				m_pMenuDef->m_nString=blocknow->blockname;
				m_pMenuDef->m_nID=index++;
				m_StockType.Add(m_pMenuDef);
			}
		}
		break;
	case BLOCKPAGE:
		{
			pDoc->m_ManagerBlockData.GetAllBlockTypeName(m_BlockTypeNameArray);
			int l_nCount=m_BlockTypeNameArray.GetCount();
			for(int i=0;i<l_nCount;i++)
			{
				CString l_sBlockType=m_BlockTypeNameArray.GetAt(i);
				if (l_sBlockType == "全部板块" || l_sBlockType == "自选板块")
				{
					m_BlockTypeNameArray.RemoveAt(i);
					l_nCount--;
					i--;
				}
			}
			l_nCount=m_BlockTypeNameArray.GetCount();
			for(int i=0;i<l_nCount;i++)
			{
			    CString l_sBlockType=m_BlockTypeNameArray.GetAt(i);
				stockTypeNameArray.RemoveAll();
				pDoc->m_ManagerBlockData.GetAllBlockNamesFromBlockTypeName(stockTypeNameArray,l_sBlockType);
				stockTypeCountArray.Add(stockTypeNameArray.GetCount());
				for (int j = 0;j < stockTypeNameArray.GetCount();j++)
				{
					DynaMenuDef *m_pMenuDef;
					m_pMenuDef=new DynaMenuDef;
					m_pMenuDef->m_nString=stockTypeNameArray[j];
					m_pMenuDef->m_nID=index++;
					m_StockType.Add(m_pMenuDef);
				}
			}
		}
		break;
	case SHPAGE:
		{

			for(int i = SHZS;i <= SHOT;i++)
			{
				DynaMenuDef *m_pMenuDef;
				m_pMenuDef=new DynaMenuDef;
				m_pMenuDef->m_nString=ShMenuStr[i];
				m_pMenuDef->m_nID=index + i;
				m_StockType.Add(m_pMenuDef);
			}
		}
		break;
	case SZPAGE:
		{
			for(int i=SZZS;i<= SZOT;i++)
			{
				DynaMenuDef *m_pMenuDef;
				m_pMenuDef=new DynaMenuDef;
				m_pMenuDef->m_nString=SzMenuStr[i - SZZS];
				m_pMenuDef->m_nID=index + i;
				m_StockType.Add(m_pMenuDef);
			}
		}
		break;
	case FLPAGE:
		{
			for(int i = 0;i < 12;i++)
			{
				DynaMenuDef *m_pMenuDef;
				m_pMenuDef=new DynaMenuDef;
				m_pMenuDef->m_nString = FlMenuStr[i];
				m_pMenuDef->m_nID = index + i;
				m_StockType.Add(m_pMenuDef);
			}
		}
		break;

	}

	if (mode == BLOCKPAGE)
	{
		index=0;

		for (int i = 0;i < m_BlockTypeNameArray.GetCount();i++)
		{
			CMenu tempMenu;
			if(tempMenu.CreatePopupMenu())
			{
				for (int j = 0;j < stockTypeCountArray[i];j++)
				{
					DynaMenuDef *m_pMenuDef;
					m_pMenuDef = m_StockType.GetAt( index );

					if(j%32 == 0 && j != 0)
						tempMenu.AppendMenu(MF_STRING|MF_MENUBARBREAK,m_pMenuDef->m_nID, m_pMenuDef->m_nString);
					else
						tempMenu.AppendMenu(MF_STRING,m_pMenuDef->m_nID, m_pMenuDef->m_nString);
					
					index++;
				}
				m_TypeMenu->AppendMenu(MF_POPUP,(UINT)tempMenu.m_hMenu,m_BlockTypeNameArray.GetAt(i));
			}
		}
		m_TypeMenu->AppendMenu(MF_SEPARATOR);
		m_TypeMenu->AppendMenu(MF_STRING,8999, "板块管理");
		//m_TypeMenu->AppendMenu(MF_STRING,8997, "更新日线");
		//m_TypeMenu->AppendMenu(MF_STRING,8996, "更新5分钟线");
	}
	else if (mode == CHOOSEPAGE)
	{
		m_TypeMenu->AppendMenu(MF_STRING,8998, "自选股管理");
		m_TypeMenu->AppendMenu(MF_SEPARATOR);
		index=0;
		for (int i = 0;i < m_StockType.GetCount();i++)
		{
			DynaMenuDef *m_pMenuDef;
			m_pMenuDef = m_StockType.GetAt( i );
			if(index%14==0 && index!=0)
				m_TypeMenu->AppendMenu(MF_STRING|MF_MENUBARBREAK,m_pMenuDef->m_nID, m_pMenuDef->m_nString);
			else
				m_TypeMenu->AppendMenu(MF_STRING,m_pMenuDef->m_nID, m_pMenuDef->m_nString);
			index++;
		}
	}
	else
	{
		index=0;
		for (int i = 0;i < m_StockType.GetCount();i++)
		{
			DynaMenuDef *m_pMenuDef;
			m_pMenuDef = m_StockType.GetAt( i );
			if(index%14==0 && index!=0)
				m_TypeMenu->AppendMenu(MF_STRING|MF_MENUBARBREAK,m_pMenuDef->m_nID, m_pMenuDef->m_nString);
			else if(pDoc->m_nStockTypeFlPage == m_pMenuDef->m_nID - 9000 && mode == FLPAGE)
				m_TypeMenu->AppendMenu(MF_STRING|MF_CHECKED,m_pMenuDef->m_nID, m_pMenuDef->m_nString);
			else 
				m_TypeMenu->AppendMenu(MF_STRING,m_pMenuDef->m_nID, m_pMenuDef->m_nString);
			index++;
		}
	}
}

BOOL CPageWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    CTaiShanReportView* pView = (CTaiShanReportView*)GetParent();
    CTaiShanDoc *pDoc = pView->GetDocument();
//	POSITION pos;
	// If pHandlerInfo is NULL, then handle the message
	if (pHandlerInfo == NULL)
	{
		if(nCode == CN_COMMAND)
		{
			if (8999 == nID)
			{
				CMainFrame *pWnd = (CMainFrame *)AfxGetMainWnd();
				pWnd->SendMessage(WM_COMMAND,ID_TOOL_BLOCK_MANA,0);
				return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
			}
			else if(8998 == nID)
			{
				CSuperviseChoose l_ManagerChooseDlg;
				l_ManagerChooseDlg.DoModal();    //打开自选股管理窗口
				return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
			}
			else if (8997 == nID)
			{
				CMainFrame *pWnd = (CMainFrame *)AfxGetMainWnd();
				pWnd->SendMessage(WM_COMMAND,ID_UPDATE_DAY_LINE,0);
				return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
			}
			else if (8996 == nID)
			{
				CMainFrame *pWnd = (CMainFrame *)AfxGetMainWnd();
				pWnd->SendMessage(WM_COMMAND,ID_UPDATE_MIN5_LINE,0);
				return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
			}
			for (int i = 0;i < m_StockType.GetCount();i++)
			{
				DynaMenuDef *m_pMenuDef;
				m_pMenuDef = m_StockType.GetAt( i );
				if(m_pMenuDef->m_nID==nID)
				{
					  if(GetActivePage()==BLOCKPAGE)
					  {
						 CString sStockTypeChoose;
						 sStockTypeChoose=m_pMenuDef->m_nString;
						 strcpy(pDoc->m_SystemInitData.StockTypeName,m_pMenuDef->m_nString.GetBuffer(0));
						 pView->ChangeToBlock(pDoc, sStockTypeChoose);
						 ReplacePage(GetActivePage(),m_pMenuDef->m_nString);
					  }
					  else if(GetActivePage()==CHOOSEPAGE)
					  {
						  CString sStockTypeChoose;
						  sStockTypeChoose=m_pMenuDef->m_nString;
						  strcpy(pDoc->m_SystemInitData.ChooseTypeName,m_pMenuDef->m_nString.GetBuffer(0));
						  pView->ChangeToChoose(pDoc, sStockTypeChoose);
						  ReplacePage(GetActivePage(),m_pMenuDef->m_nString);
					  }
					  else if(GetActivePage()==TJXGPAGE)
					  {
						 CString sStockScreenChoose=m_pMenuDef->m_nString;
						 strcpy(pDoc->m_SystemInitData.ScreenStockName,m_pMenuDef->m_nString.GetBuffer(0));
						 pView->ChangeToTjxg(pDoc, sStockScreenChoose);
						 ReplacePage(GetActivePage(),m_pMenuDef->m_nString);
					  }
					  else if(GetActivePage()==SHPAGE)
					  {
						 pDoc->m_nStockTypeShPage=m_pMenuDef->m_nID-9000;
						 pView->ChangeToPage(m_nActivePage);
						 ReplacePage(GetActivePage(),m_pMenuDef->m_nString);
					  }
					  else if(GetActivePage()==SZPAGE)
					  {
						 pDoc->m_nStockTypeSzPage=m_pMenuDef->m_nID-9000;
						 pView->ChangeToPage(m_nActivePage);
						 ReplacePage(GetActivePage(),m_pMenuDef->m_nString);
					  }
					  else if(GetActivePage()==FLPAGE)
					  {
						 pDoc->m_nStockTypeFlPage=m_pMenuDef->m_nID-9000;
						 pView->ChangeToPage(m_nActivePage);
					  }

					  break;			
				}
			}
		}
		// Filter the commands sent to a text color menu option
	}
	// If we didn't process the command, call the base class
	// version of OnCmdMsg so the message-map can handle the message
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CPageWnd::SetPageTitle(CString m_title,int index)
{
    m_strPageTitle.SetAt(index,m_title);
	CString	str=m_strPageTitle.GetAt(index);
	int temp=str.GetLength( )*DEFAULTFONTWIDTH;
	if(temp<5*DEFAULTFONTWIDTH)
		temp=5*DEFAULTFONTWIDTH;
    PageWidth[index]=temp;
	PageWidth[index]=8 * DEFAULTFONTWIDTH;

}

void CPageWnd::DoRButtonDown(UINT nFlags, CPoint point)
{
	int nSplitx=m_nBeginX;
	for(int i=m_nFirstPage;i<m_nEndPage;i++)
	{
		//judge if right mouse clicked is in the area of the special page
		if((point.x >nSplitx) && (point.x <nSplitx+PageWidth[i]))
		{
			if(i != m_nActivePage)
				DoLButtonDown( nFlags,  point);

			if(i==FLPAGE)
			{
	            ExecuteMenu(FLPAGE);
			}
			else if(i==SHPAGE)
			{
				ExecuteMenu(SHPAGE);
			}
			else if(i==SZPAGE)
			{
				ExecuteMenu(SZPAGE);
			}
			else if(i==AGPAGE)
			{
				ExecuteMenu(AGPAGE);
			}
			else if(i==ZXPAGE)
			{
	            ExecuteMenu(ZXPAGE);
			}
			else if(i==CYPAGE)
			{
	            ExecuteMenu(CYPAGE);
			}
			else if(i==BGPAGE)
			{
				ExecuteMenu(BGPAGE);
			}
			else if(i==QZPAGE)
			{
				ExecuteMenu(QZPAGE);
			}
			else if(i==JJPAGE)
			{
				ExecuteMenu(JJPAGE);
			}
			else if(i==BLOCKPAGE)
			{
				ExecuteMenu(BLOCKPAGE);
			}
			else if(i==TJXGPAGE)
			{
				ExecuteMenu(TJXGPAGE);
			}
			else if(i==CHOOSEPAGE)
			{
				ExecuteMenu(CHOOSEPAGE);
			}
			break;
		}
		nSplitx+=PageWidth[i]; //next page
	}
}

bool CPageWnd::DoLButtonDown(UINT nFlags, CPoint points)
{
    CTaiShanReportView* pView = (CTaiShanReportView*)GetParent();
	CClientDC   dc(this);
	//judge if the left mouse clicked is in the area of the page
    if(points.x > m_ptBarStartPos.x - 5)
	{
	   SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
       m_nMovebar = 1;
    }
	else
	{
	  m_nMovebar = 0;
      int nSplitx=m_nBeginX;
	  for(int i=m_nFirstPage;i<m_nEndPage;i++)
	  {
		//judge if left mouse clicked is in the area of a page
		if((points.x >nSplitx) && (points.x <nSplitx+PageWidth[i]))
		{
			if(i!=m_nActivePage)//if clicked in a page but not the activepage
			{
				//jump to the new page whick left mouse choosed
                if((nSplitx+PageWidth[i])>m_ptBarStartPos.x)
					m_bMove=TRUE;
				else
					m_bMove=FALSE;

//                JumpToPage((CDC*)&dc,m_nActivePage,i);
				m_nActivePage=i; //refresh the active page 
                ShowAllPage((CDC*)&dc);				
                //sendmessage to the view
 				//pView->PostMessage(WM_SHOWTYPE,0,0);
				pView->ChangeToPage(m_nActivePage);

				return true; 
			}
		}
		nSplitx+=PageWidth[i]; //next page
	  }
	}
	return false;
}
