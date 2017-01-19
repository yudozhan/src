// BlockAnalysisView.cpp : 实现文件
//

#include "stdafx.h"
#include "WH.h"
#include "BlockAnalysisView.h"
#include "StructTaiShares.h"
#include "CTaiShanDoc.h"
#include "BlockAnalysisGridCtrl.h"
#include "SheetScrollBar.h"
#include "BlockPageWnd.h"
#include "CTaiShanReportView.h"

// CBlockAnalysisView

#define BOTTOM_HIGHT	20


IMPLEMENT_DYNCREATE(CBlockAnalysisView, CView)

CBlockAnalysisView::CBlockAnalysisView()
{
	m_nCols = 17;
	m_nFixRows = 1;
	m_nFixCols = 2;
	m_nRows = 1000;

}

CBlockAnalysisView::~CBlockAnalysisView()
{
	if (m_pGrid)      
		delete m_pGrid;

	if(m_pScrollBar)
		delete m_pScrollBar;

	if(m_pPageWnd)
		delete m_pPageWnd;
	if (m_pDoc->m_pBlockAnalysisView)
	{
		m_pDoc->m_pBlockAnalysisView = NULL;
	}

}

BEGIN_MESSAGE_MAP(CBlockAnalysisView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_USER_CHANGECOLOR,OnGridColorChange)

	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CBlockAnalysisView 绘图

void CBlockAnalysisView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	m_pGrid->Invalidate();                            //更新显示

}


// CBlockAnalysisView 诊断

#ifdef _DEBUG
void CBlockAnalysisView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBlockAnalysisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif

CTaiShanDoc* CBlockAnalysisView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTaiShanDoc)));
	return (CTaiShanDoc*)m_pDocument;
}
#endif //_DEBUG


// CBlockAnalysisView 消息处理程序

int CBlockAnalysisView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pDoc = GetDocument();

	//Create and inititalize a CGrid
	DWORD dwStyle;
	dwStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE;
	dwStyle &= ~WS_HSCROLL;

	m_pGrid = new CBlockAnalysisGridCtrl;
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->Create(CRect(0,0,10,10),this,ID_BLOCKANALYSES,dwStyle);

	InitGridContent();

	m_pGrid->SetItemState(1,1, m_pGrid->GetItemState(1,1) | GVIS_READONLY);
	m_pGrid->AutoSize();

	m_pGrid->SetColumnWidth(0,65);
	m_pGrid->SetColumnWidth(1,180);
	m_pGrid->SetColumnWidth(2,80);
	m_pGrid->SetColumnWidth(3,80);
	m_pGrid->SetColumnWidth(4,80);
	m_pGrid->SetColumnWidth(5,80);
	m_pGrid->SetColumnWidth(6,80);
	m_pGrid->SetColumnWidth(7,80);
	m_pGrid->SetColumnWidth(8,80);
	m_pGrid->SetColumnWidth(9,80);
	m_pGrid->SetColumnWidth(10,80);
	m_pGrid->SetColumnWidth(11,80);
	m_pGrid->SetColumnWidth(12,80);

	m_pGrid->SetRowHeight(0, (int)1.2 * m_pGrid->GetRowHeight(0));
	m_pGrid->SetEditable(FALSE);
	m_pGrid->SetRowResize(FALSE);
	m_pGrid->SetMouseState(MOUSE_NOTHINGS);
	m_pGrid->SetListMode(TRUE);

	InitGridColumnTitle();

	m_pGrid->SetFocus();
	m_pGrid->SetFocusRow(2);
	m_pGrid->Invalidate();

	m_pDoc->m_pBlockAnalysisView = this;

	//创建控制页面	
	m_pPageWnd = new CBlockPageWnd;
	//Create and init hScorllBar
	//////////////create page wnd/////////////
	m_pPageWnd->Create(_T(" "),WS_CHILD | WS_VISIBLE, CRect(100,100,200,200), this);
	//创建水平条	
	m_pScrollBar = new CSheetScrollBar;
	m_pScrollBar->Create( SBS_HORZ | WS_CHILD | WS_VISIBLE,CRect(20,20,100,100),this,ID_HSCROLLBAR);
	m_pScrollBar->SetScrollRange(0,500,TRUE);

	m_pPageWnd->m_pScrollBar = (CSheetScrollBar *)GetDlgItem(ID_HSCROLLBAR);

	m_pDoc->m_ManagerBlockData.CalcBlockOpenClosePrice();
	m_pDoc->m_ManagerBlockData.CalcRealTimeBlockIndex();

	ChangeToPage(m_pDoc->m_nCurrentBlockPage);
	m_pPageWnd->m_nActivePage = m_pDoc->m_nCurrentBlockPage;
	m_pPageWnd->m_bRefresh = TRUE;
	m_pPageWnd->Invalidate();
	SetCursor(LoadCursor(  NULL, IDC_ARROW )); 

	return 0;
}

void CBlockAnalysisView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(cx == 0 || cy == 0)
		return;

	//CRect clientrect;
	GetClientRect(m_rcBar);
	//SetWindowPos(NULL,0,0,cx,cy,SWP_NOMOVE|SWP_NOZORDER);
	m_pGrid->MoveWindow(0,0,cx,cy - BOTTOM_HIGHT,TRUE);
	int bar_lengh = 250;   
	ShowWindow(SW_SHOW);
	m_pScrollBar->m_bRefresh=TRUE;
	m_pScrollBar->MoveWindow(cx-bar_lengh,cy-BOTTOM_HIGHT,bar_lengh,BOTTOM_HIGHT,TRUE);
	ShowWindow(SW_SHOW);

	int x,y;
	x  = cx - bar_lengh;
	y  = cy - BOTTOM_HIGHT;
	m_pPageWnd->m_bRefresh = TRUE;
	m_pPageWnd->MoveWindow(0,y,x,BOTTOM_HIGHT,true);
	ShowWindow(SW_SHOW);

	m_ptBarPos.x = cx - bar_lengh;
	m_ptBarPos.y = cy - BOTTOM_HIGHT;
	///////   Modi by zjh
	m_pPageWnd->m_ptBarStartPos.y = cy-16;
	m_pPageWnd->m_ptBarEndPos.x = cx;
	m_pPageWnd->m_ptBarEndPos.y = cy;

	CCellRange VisibleCells = m_pGrid->GetVisibleNonFixedCellRange();

	if(VisibleCells.GetMaxRow()<0 || VisibleCells.GetMinRow ()<0)
		return ;

	m_VisibleRowCount = VisibleCells.GetMaxRow() - VisibleCells.GetMinRow ()  + 1; 


	CCellRange m_VisibleCellRange = m_pGrid->GetVisibleNonFixedCellRange(); 
	m_VisibleColumn = m_VisibleCellRange.GetMaxCol()-m_VisibleCellRange.GetMinCol();


	int pos = m_pScrollBar->GetScrollPos();
	for(int i = 0;i < pos;i++)
		m_pGrid->SendMessage(WM_HSCROLL,SB_LINELEFT,0);
	int nMax = m_nCols -  m_nFixCols + 1;//m_VisibleColumn +8;
	m_pScrollBar->SetScrollRange( 0, nMax, TRUE);
	m_pScrollBar->SetScrollPos(0,true);

	m_pGrid->SetFocus();
}

//Init CGrid ctr 's title and frame
/////////////////////////////////////////////////////////////////////////////
//      函 数 名： 
//      功能说明：
//      入口参数：无入口数
//      出口参数：无出口数
//      返回参数：无返回数
////////////////////////////////////////////////////////////////////////////
BOOL CBlockAnalysisView::InitGridContent()
{
	CTaiShanDoc* pDoc = GetDocument();

	ASSERT_VALID(pDoc);

	m_pGrid->SetMainDocument(pDoc);

	if(m_nRows < 25)
		m_nRows = 1000;
	try 
	{
		m_pGrid->SetRowCount(m_nRows);
		m_pGrid->SetColumnCount(m_nCols);
		m_pGrid->SetFixedRowCount(m_nFixRows);
		m_pGrid->SetFixedColumnCount(m_nFixCols);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
	m_pGrid->SetGridColor(pDoc->m_colorArray[18],pDoc->m_colorArray[17],pDoc->m_colorArray[16]);
	m_pGrid->SetFixedBkColor(pDoc->m_colorArray[18]);

	CFont Font;
	Font.CreateFontIndirect(&pDoc->m_fontstr[1]);
	m_pGrid->SetFont(&Font);
	Font.DeleteObject();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名： 
//      功能说明：
//      入口参数：无入口数
//      出口参数：无出口数
//      返回参数：无返回数
////////////////////////////////////////////////////////////////////////////
void CBlockAnalysisView::InitGridColumnTitle()
{
	CString str = "排名";
	m_pGrid->SetColumnInfo(0,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,	str.GetBuffer(0),ID_ROWNUM);
	str = "板块名称";
	m_pGrid->SetColumnInfo(1,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,str.GetBuffer(0),ID_STOCKNAME);
	str = "昨收指数";
	m_pGrid->SetColumnInfo(2,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,	str.GetBuffer(0),ID_YSTC);
	str = "今开指数";
	m_pGrid->SetColumnInfo(3,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,str.GetBuffer(0),ID_OPEN);
	str = "当日最新";
	m_pGrid->SetColumnInfo(4,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,str.GetBuffer(0),ID_NEW);
	str = "当日最高";
	m_pGrid->SetColumnInfo(5,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,str.GetBuffer(0),ID_HIGH);
	str = "当日最底";
	m_pGrid->SetColumnInfo(6,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,	str.GetBuffer(0),ID_LOW);
	str = "涨幅";
	m_pGrid->SetColumnInfo(7,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,	str.GetBuffer(0),ID_RDR);
	str = "振幅";
	m_pGrid->SetColumnInfo(8,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,	str.GetBuffer(0),ID_RDR);
	str = "外盘";
	m_pGrid->SetColumnInfo(9,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,str.GetBuffer(0),ID_RVOL);
	str = "内盘";
	m_pGrid->SetColumnInfo(10,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,str.GetBuffer(0),ID_DVOL);
	str = "总成交量";
	m_pGrid->SetColumnInfo(11,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,str.GetBuffer(0),ID_TOTV);
	str = "总成交额";
	m_pGrid->SetColumnInfo(12,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,str.GetBuffer(0),ID_TOTP);
	str = "换手率";
	m_pGrid->SetColumnInfo(13,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,str.GetBuffer(0),ID_EXCHANGE);
	str = "市盈利";
	m_pGrid->SetColumnInfo(14,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,str.GetBuffer(0),ID_SYL);
	str = "领涨股票";
	m_pGrid->SetColumnInfo(15,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,str.GetBuffer(0),ID_FIRSTSTK);
	str = "涨股比";
	m_pGrid->SetColumnInfo(16,DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,str.GetBuffer(0),ID_RSTKRATE);


	m_nCols = 17; //total column
    
	m_nRows = BLOCKTYPE_NUM + 5 ;

}


LRESULT CBlockAnalysisView::OnGridColorChange(WPARAM wParam, LPARAM lParam)
{
	m_pGrid->SetGridColor(m_pDoc->m_colorArray[18],m_pDoc->m_colorArray[17],m_pDoc->m_colorArray[16]);
	m_pGrid->SetFixedBkColor(m_pDoc->m_colorArray[18]);

	//InitGridColumnTitle();
	//更新显示
	m_pGrid->Invalidate();   
	
	return 1L;
}


BOOL CBlockAnalysisView::SetShowData(int nBlockCount)
{
	HGLOBAL	hMem;
	LPVOID hp;
	BLOCKINFO  *ptempStockShow;
	if(!m_pDoc->m_pBlockDataShow)
	{
		hMem=GlobalAlloc( GPTR, (nBlockCount)* sizeof( BLOCKINFO) );
		hp=GlobalLock(hMem);
		if( hp )
		{
			m_pDoc->m_pBlockDataShow=(BLOCKINFO *)hp;
			m_pDoc->m_nBlockShowMaxCount = nBlockCount;
			return TRUE;
		}
		else
		{
			AfxMessageBox("分配内存出错",MB_ICONSTOP);
			return FALSE;
		}
	}
	else
	{
		if(m_pDoc->m_nBlockShowMaxCount >= nBlockCount)
		{
			return TRUE;
		}
		else
		{
			hMem=GlobalAlloc( GPTR, (nBlockCount)* sizeof( BLOCKINFO) );
			hp=GlobalLock(hMem);
			if( hp )
			{
				ptempStockShow=(BLOCKINFO *)hp;
				GlobalUnlock((HGLOBAL)m_pDoc->m_pBlockDataShow);        //释放内存
				GlobalFree( (HGLOBAL)m_pDoc->m_pBlockDataShow);

				m_pDoc->m_pBlockDataShow = ptempStockShow;
				m_pDoc->m_nBlockShowMaxCount = nBlockCount;

				return TRUE;
			}
			else
			{
				AfxMessageBox("分配内存出错",MB_ICONSTOP);
				return FALSE;
			}
		}
	}
}

void CBlockAnalysisView::ChangeToPage(int nPage)
{
	if(nPage >= m_pPageWnd->GetPageCount()) 
	{
		ASSERT(FALSE);
		nPage = m_pPageWnd->GetPageCount() - 1;
	}

	m_pGrid->SetSortColumn(-1 ,TRUE);
	m_pGrid->ClearSortColumn();
	m_pGrid->SetFocus();

	m_pGrid->SetFocusRow(m_nFixRows); 
	m_pGrid->SetMouseState(MOUSE_NOTHINGS);
	m_pDoc->m_nCurrentReportPage = nPage;
	m_pPageWnd->m_nActivePage = nPage;

	CString	strBlockTypeName = "全部板块";
	strBlockTypeName = m_pPageWnd->GetPageTitle(nPage);
	int DisplayCount;
	m_pDoc->m_nBlockShowCount = m_pDoc->m_ManagerBlockData.GetBlockCountFromBlockTypeName(strBlockTypeName);
	DisplayCount = m_pDoc->m_nBlockShowCount;

	SetShowData(DisplayCount);

	m_pDoc->m_ManagerBlockData.RecvBlockDataForBlockType(m_pDoc->m_pBlockDataShow,strBlockTypeName);
	//change the real row numbers
	m_pGrid->SetRealRow(DisplayCount + 1); // add by xqk 1999/4/21
	m_pGrid->SetScrollPos32(SB_VERT, 0, TRUE);
	UINT TotRow = 0;
	if(DisplayCount > m_nRows)
	{
		TotRow = DisplayCount + 5;
		try { m_pGrid->SetRowCount(TotRow); }
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return ;
		}
		m_nRows = TotRow;
	}

	m_pGrid->Invalidate();                            //更新显示

}
void CBlockAnalysisView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	CView::OnChar(nChar, nRepCnt, nFlags);
}

void CBlockAnalysisView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CBlockAnalysisView::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_CHAR && pMsg->wParam == VK_ESCAPE)
	{
		//GetParent()->SendMessage(WM_SHOWWINDOW,FALSE,0);
		m_pDoc->m_pReportView->GetParentFrame()->ActivateFrame();
		GetParent()->SendMessage(WM_CLOSE);

		return true;
	}

	return CView::PreTranslateMessage(pMsg);
}

void CBlockAnalysisView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CView::OnShowWindow(bShow, nStatus);

	//m_pDoc->m_ManagerBlockData.CalcBlockOpenClosePrice();
	//m_pDoc->m_ManagerBlockData.CalcRealTimeBlockIndex();
}

BOOL CBlockAnalysisView::OnEraseBkgnd(CDC* pDC)
{
	return  TRUE;
	//return CView::OnEraseBkgnd(pDC);
}
