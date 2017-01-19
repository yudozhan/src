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
// 1.create an object of CBlockPageWnd
// 2.call CBlockPageWnd::Create(....)
// 3.deal with the message from CBlockPageWnd
////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WH.h"
#include "Memdc.h"
#include "SheetScrollBar.h"
#include "BlockPageWnd.h"
#include "mainfrm.h"
#include "CTaiShanDoc.h"
#include "cellRange.h"
#include "BlockAnalysisView.h"
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
// CBlockPageWnd

CBlockPageWnd::CBlockPageWnd()
{
	//Initialize all parameters
	m_pScrollBar = NULL;//new CScrollBar;
    m_nMovebar = 0;
	m_nBarlength = 250;
	m_nBarLen=m_nBarlength;
	m_TypeMenu=NULL;
	m_bMove = FALSE;

}

CBlockPageWnd::~CBlockPageWnd()
{
	m_Font.DeleteObject();
	if(m_TypeMenu) delete m_TypeMenu;
}


BEGIN_MESSAGE_MAP(CBlockPageWnd, CWnd)
	//{{AFX_MSG_MAP(CBlockPageWnd)
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
// CBlockPageWnd message handlers

BOOL CBlockPageWnd::Create(LPCTSTR szTitle, LONG style, const RECT& rect, CWnd* parent) 
{
	//register a wnd class
	LPCTSTR lpszBounceClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,LoadCursor(NULL, IDC_ARROW),(HBRUSH)(COLOR_WINDOW+1),NULL);
	return CWnd::Create(lpszBounceClass, szTitle, style, rect, parent, IDC_WNDIDNO);
}

int CBlockPageWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
void CBlockPageWnd::InitAllPara()
{
    CBlockAnalysisView* pView = (CBlockAnalysisView*)GetParent();
    CTaiShanDoc *pDoc = pView->GetDocument();

	m_nBeginX = BOTTOM_HIGHT * 0;
	//m_nBeginX = BOTTOM_HIGHT * 2;
	m_nActivePage=0;
	m_bRefresh=TRUE;

	pDoc->m_ManagerBlockData.GetAllBlockTypeName(m_strPageTitle);
	//int l_nCount = m_strPageTitle.GetCount();
	//for(int i=0;i<l_nCount;i++)
	//{
	//	CString strBlockType = m_strPageTitle.GetAt(i);
	//	if (strBlockType == "自选板块")
	//	{
	//		m_strPageTitle.SetAt(i,"自选板块");
	//		break;
	//		//m_strPageTitle.RemoveAt(i);
	//		//l_nCount--;
	//		//i--;
	//	}
	//}

	//init page
	m_nFirstPage = 0;
	m_nEndPage = m_strPageTitle.GetSize();
	int i;
	CString str;
	for(i = 0;i < m_nEndPage;i++)
	{
		str = m_strPageTitle.GetAt(i);
		if(str.GetLength() < 5)
	      PageWidth[i] = 5 * DEFAULTFONTWIDTH;
		else
	      PageWidth[i] = str.GetLength( ) * DEFAULTFONTWIDTH;
		PageWidth[i] = 8 * DEFAULTFONTWIDTH;
	}
	for(i = m_nEndPage;i < MAX_PAGE;i++)   //Get Page Width
		PageWidth[i] = 0;
}

//deal with EraseBkgnd message so the program can refrash the abrupt area
BOOL CBlockPageWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	ShowAllPage((CDC *)&dc);
	ReleaseDC(&dc);
	return 1;
}

void CBlockPageWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
    m_bRefresh=TRUE; //inform the system to refresh all area
 	m_nBeginX = 0*BOTTOM_HIGHT;	
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


void CBlockPageWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(m_bRefresh)
	{
		m_bRefresh=FALSE;
		ShowAllPage(&dc);
	}
	ReleaseDC(&dc);

}

void CBlockPageWnd::OnLButtonDown(UINT nFlags, CPoint points) 
{	

	if(!DoLButtonDown( nFlags,  points))
		DoRButtonDown( nFlags,  points);


}


void CBlockPageWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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

//show the title in the center 
//the show mode is TRANSPARENT
void CBlockPageWnd::ShowPageTitle(CDC *pDC, CString &str, LPRECT lpRect, UINT nFormat,int nPage)
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
int CBlockPageWnd::GetActivePage()
{
    return m_nActivePage;
}

//pDC is a CDC 
//draw the page frame
//x is the position
//cx is the width of a page
//bActive is flag of active or not
void CBlockPageWnd::DrawPageBox(CDC *pDC, int x, int cx, BOOL bActive)
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
void CBlockPageWnd::ShowAllPage(CDC *pDC)
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
		Title = GetPageTitle(nPage);
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

	pDC->BitBlt(0 ,0,ClientRect.right,BOTTOM_HIGHT,&dcMem,0,0,SRCCOPY);
	//pDC->BitBlt(LEFT_LENG+1 ,0,ClientRect.right,BOTTOM_HIGHT,&dcMem,0,0,SRCCOPY);
	
	//Marenna -
	dcMem.SelectObject(pbmOld);
}

//replace the page with another page
void CBlockPageWnd::ReplacePage(int nPage,CString ReplaceTitle)
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
void CBlockPageWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	DoRButtonDown( nFlags,  point);

	CWnd::OnRButtonDown(nFlags, point);
}

///////////////zjh add the mouse event handle
void CBlockPageWnd::OnMouseMove(UINT nFlags, CPoint point) 
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
	  {
		  if(PeekMessage(&msg,this->GetSafeHwnd() ,WM_PAINT,WM_PAINT,PM_REMOVE))
		  {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
		  }
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

void CBlockPageWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_nMovebar = 0;
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}

BOOL CBlockPageWnd::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    CBlockAnalysisView* pView = (CBlockAnalysisView*)GetParent();
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

		}
		// Filter the commands sent to a text color menu option
	}
	// If we didn't process the command, call the base class
	// version of OnCmdMsg so the message-map can handle the message
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CBlockPageWnd::SetPageTitle(CString m_title,int index)
{
    m_strPageTitle.SetAt(index,m_title);
	CString	str = m_strPageTitle.GetAt(index);
	int temp = str.GetLength( )*DEFAULTFONTWIDTH;
	if(temp < 5 * DEFAULTFONTWIDTH)
	{
		temp = 5 * DEFAULTFONTWIDTH;
	}
    PageWidth[index] = temp;
	PageWidth[index] = 8 * DEFAULTFONTWIDTH;

}

void CBlockPageWnd::DoRButtonDown(UINT nFlags, CPoint point)
{
	int nSplitx = m_nBeginX;
	for(int i = m_nFirstPage;i < m_nEndPage;i++)
	{
		//judge if right mouse clicked is in the area of the special page
		if((point.x > nSplitx) && (point.x < nSplitx + PageWidth[i]))
		{
			if(i != m_nActivePage)
			{
				DoLButtonDown( nFlags,  point);
			}
			break;
		}
		nSplitx += PageWidth[i]; //next page
	}
}

bool CBlockPageWnd::DoLButtonDown(UINT nFlags, CPoint points)
{
    CBlockAnalysisView* pView = (CBlockAnalysisView*)GetParent();
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

int	CBlockPageWnd::GetPageCount()
{
	return m_strPageTitle.GetCount();
}
CString CBlockPageWnd::GetPageTitle(int nPage)
{
	if (nPage >= 0 && nPage < m_strPageTitle.GetCount())
	{
		return m_strPageTitle.GetAt(nPage);
	}

	return "";
}
