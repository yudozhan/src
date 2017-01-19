// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "resource.h"

#include "CTaiShanDoc.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"
#include "WH.h"
#include "MySplitter.h"
#include "CompanyInfoView.h"
#include "ChildFrm.h"
#include "MainFrm.h"
//#include "LeftView.h"
#include "CTaiShanKlineShowView.h"
#include "MyTreeView.h"
#include "F10HeadView.h"
//#include "BroadcastView.h"
#include "MyRichView.h"
#include "CTaiKlineDialogCross.h"
#include "CTaiKlineDialogKLineMode.h"
#include "CTaiKlineDlgChangeIndexParam.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
class CTaiShanReportView;
/////////////////////////////////////////////////////////////////////////////
// CChildFrame
IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_MDIACTIVATE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_USER_CHANGEPOS,OnSetSplitterPos)

	ON_WM_CREATE()
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_WM_INITMENU()
	ON_WM_INITMENUPOPUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	m_wndSplitter=NULL;
	m_pContext=NULL;
	m_CurrentDoc = NULL;
}

CChildFrame::~CChildFrame()
{

	
	if(m_wndSplitter)  delete m_wndSplitter;

	if(m_pContext)  delete m_pContext;
    
//	if(m_CurrentDoc) delete m_CurrentDoc;

}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	//cs.lpszName = "123";
	cs.style &= ~WS_CAPTION;
	cs.style &= ~WS_SYSMENU ;
	//cs.style &= ~WS_OVERLAPPED;
	//cs.style &= ~WS_THICKFRAME;
	//cs.style &= ~WS_MINIMIZEBOX;
	//cs.style &= ~WS_OVERLAPPEDWINDOW ;
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}
void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pContext=new CCreateContext;
	memcpy(m_pContext,pContext,sizeof(CCreateContext));
	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CChildFrame::AddCuoHeView()
{
    //if(this->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
	//	return false;
/*	BeginWaitCursor();
	m_wndSplitter=new CSplitterWnd;
   	CView* pOldActiveView = GetActiveView();

	// TODO: Add your command handler code here
	if (!m_wndSplitter->CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return;
	}
	CRect rc;
	GetClientRect(&rc);
	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter->CreateView(0, 0,
		m_pContext->m_pNewViewClass, CSize(rc.Width() - 285 - 18 , rc.Height()), m_pContext))
	{
		TRACE0("Failed to create first pane\n");
		return;
	}

	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter->CreateView(0, 1,
		RUNTIME_CLASS(CCuoHeView), CSize(0, 0), m_pContext))
	{
		TRACE0("Failed to create second pane\n");
		return;
	}
	// activate the  view
	SetActiveView((CView*)m_wndSplitter->GetPane(0,0));
	((CTaiShanReportView*)m_wndSplitter->GetPane(0,0))->InitGridCtrl();
    ((CTaiShanReportView*)m_wndSplitter->GetPane(0,0))->m_ChildFrame=this;
    OnSize(SIZE_RESTORED,0,0);

	pOldActiveView->DestroyWindow();
	EndWaitCursor();
*/}
void CChildFrame::DeleteCuoHeView()
{ 
/*	BeginWaitCursor();
    if(m_wndSplitter==NULL)
		return;
	CView *pNewView = STATIC_DOWNCAST(CView, CreateView(m_pContext));
	CTaiShanReportView *pView;
	if (pNewView != NULL)
	{
		pView=(CTaiShanReportView*)pNewView;
        pView->InitGridCtrl();
		pNewView->ShowWindow(SW_SHOW);
		SetActiveView(pView);
		RecalcLayout();

		// finally destroy the old view...

        m_wndSplitter->DeleteView(0,0);
        m_wndSplitter->DeleteView(0,1);
		delete m_wndSplitter;
	    m_wndSplitter=NULL;
	}
    EndWaitCursor();
*/}

void CChildFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CMDIChildWnd::OnShowWindow(bShow, nStatus);
	static int IsTheFirstTime = 0;
    if(IsTheFirstTime == 0)
	{
		//CMenu* mnu;
	 //   mnu=GetSystemMenu(false);
	 //   mnu->EnableMenuItem( SC_CLOSE,MF_GRAYED|MF_DISABLED);
	 //   SendMessage(WM_INITMENU,(UINT)mnu->m_hMenu ,0);
		//this->MDIMaximize( );
	    IsTheFirstTime++;
	} 
	if (!bShow)
	{
		CMainFrame::m_iMenuItemCount = 4;
	}
}

void CChildFrame::OnSize(UINT nType, int cx, int cy) 
{
	
	CMDIChildWnd::OnSize(nType, cx, cy);
	if(cx==0||cy==0)
		return ;
	if(m_wndSplitter==NULL)
		return ;
	int m_width=285+18;
	if(m_width > cx)
       m_width=2 * cx /3;
	m_wndSplitter->SetColumnInfo( 0, cx - m_width , 0  );
	m_wndSplitter->RecalcLayout( );
	// TODO: Add your message handler code here
	
}


void CChildFrame::CreateF9() 
{
  	//m_wndSplitter=new CSplitterWnd//
	m_SplitterWnd = new CTaiTestSplitter;
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	m_CurrentDoc = (CTaiShanDoc*)pActiveWnd->GetActiveDocument();

	CView* pOldActiveView = GetActiveView();
   
	// TODO: Add your command handler code here
	if (!m_SplitterWnd->CreateStatic(this, 1, 2))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return ;
	}
	CRect rc;
	GetClientRect(&rc);
	// add the first splitter pane - the default view in column 0
//	if (!m_SplitterWnd->CreateView(0, 0,
//		RUNTIME_CLASS(CTaiTestTreeView), CSize(rc.Width() - 650 , rc.Height()), m_pContext))
	if (!m_SplitterWnd->CreateView(0, 0,
		RUNTIME_CLASS(CTaiTestTreeView), CSize(rc.Width()/4, rc.Height()), m_pContext))
	{
		TRACE0("Failed to create first pane\n");
		return ;
	}

	// add the second splitter pane - an input view in column 1
	/*
	if (!m_SplitterWnd.CreateView(0, 1,
		RUNTIME_CLASS(CBroadcastView), CSize(0, 0), m_pContext))
	{
		TRACE0("Failed to create second pane\n");
		return ;
	} */

	if (!m_SplitterWnd->CreateView(0, 1,
		RUNTIME_CLASS(CTaiTestRichView), CSize(0, 0), m_pContext))
	{
		TRACE0("Failed to create second pane\n");
		return ;
	}
	// activate the  view
	//SetActiveView((CTaiTestTreeView *)m_SplitterWnd.GetPane(0,0));
	CTaiTestRichView *m_pRichView;
	m_pRichView = (CTaiTestRichView *)m_SplitterWnd->GetPane(0,1);
	m_pRichView->m_bF9ORF10 = TRUE;
	CHARFORMAT m_charformat;
	m_charformat.cbSize = 60;
    m_charformat.dwMask = 3892314127;
	m_charformat.dwEffects = 0;
	m_charformat.yHeight = 240;
	m_charformat.yOffset = 0;
	m_charformat.bCharSet = 134;
    m_charformat.bPitchAndFamily = 2;
	m_charformat.crTextColor = RGB(0 ,0,0);
	lstrcpy(m_charformat.szFaceName,"宋体");
	m_pRichView->GetRichEditCtrl().SetDefaultCharFormat(m_charformat);


	CTaiTestTreeView *m_pTreeView;
	m_pTreeView = (CTaiTestTreeView *)m_SplitterWnd->GetPane(0,0);
	m_pTreeView->m_bF9ORF10 = TRUE; 
	m_pTreeView->ShowAll();
    
	//m_SplitterWnd->GetParent()->UpdateFrameTitle();
	m_CurrentDoc->m_taiViewF9 = m_pTreeView;
	//((CTaiShanReportView*)m_SplitterWnd.GetPane(0,0))->m_ChildFrame=this;
	OnSize(SIZE_RESTORED,0,0);
    pOldActiveView->DestroyWindow();
	OnUpdateFrameTitle(TRUE);
}   

void CChildFrame::CreateF10() 
{
  	//m_wndSplitter=new CSplitterWnd//
	m_SplitterWnd = new CTaiTestSplitter;
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	m_CurrentDoc = (CTaiShanDoc*)pActiveWnd->GetActiveDocument();

	CView* pOldActiveView = GetActiveView();
   
	// TODO: Add your command handler code here
	if (!m_SplitterWnd->CreateStatic(this, 2, 1))
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return ;
	}
	CRect rc;
	GetClientRect(&rc);
	if (!m_SplitterWnd->CreateView(1, 0,
		RUNTIME_CLASS(CTaiTestRichView), CSize(0, 0), m_pContext))
	{
		TRACE0("Failed to create second pane\n");
		return ;
	}
	CTaiTestRichView *m_pRichView;
	m_pRichView = (CTaiTestRichView *)m_SplitterWnd->GetPane(1,0);
	//CTaiTestRichView *m_pRichView;
	//m_pRichView = (CTaiTestRichView *)GetActiveView();



	// add the first splitter pane - the default view in column 0
//	if (!m_SplitterWnd->CreateView(0, 0,
//		RUNTIME_CLASS(CTaiTestTreeView), CSize(rc.Width() - 650 , rc.Height()), m_pContext))
	if (!m_SplitterWnd->CreateView(0, 0,
		RUNTIME_CLASS(CF10HeadView), CSize(rc.Width() , 90), m_pContext))
	{
		TRACE0("Failed to create first pane\n");
		return ;
	}
	//if (!m_SplitterWnd->CreateView(0, 0,
	//	RUNTIME_CLASS(CTaiTestTreeView), CSize(rc.Width()/4 , rc.Height()), m_pContext))
	//{
	//	TRACE0("Failed to create first pane\n");
	//	return ;
	//}


    m_pRichView->m_bF9ORF10 = FALSE;
	CHARFORMAT m_charformat;
	m_charformat.cbSize = 60;
    m_charformat.dwMask = 3892314127;
	m_charformat.dwEffects = 0;
	m_charformat.yHeight = 240;
	m_charformat.yOffset = 0;
	m_charformat.bCharSet = 134;
    m_charformat.bPitchAndFamily = 2;
	m_charformat.crTextColor = RGB(0 ,0,0);
	lstrcpy(m_charformat.szFaceName,"宋体");
	m_pRichView->GetRichEditCtrl().SetDefaultCharFormat(m_charformat);

	
	CF10HeadView *pF10HeadView;
	pF10HeadView = (CF10HeadView *)m_SplitterWnd->GetPane(0,0);
	pF10HeadView->m_pTaiTestRichView = m_pRichView;
	m_pRichView->m_pF10HeadView = pF10HeadView;
	//CTaiTestTreeView *m_pTreeView;
	//m_pTreeView = (CTaiTestTreeView *)m_SplitterWnd->GetPane(0,0);
	//m_pTreeView->m_bF9ORF10 = FALSE;
	//m_pTreeView->ShowAllF10();
    
	pF10HeadView->OnBnClickedRadioItem(IDC_RADIO1);
	CButton * pBtn = (CButton *)pF10HeadView->GetDlgItem(IDC_RADIO1);
	if (pBtn)
	{
		pBtn->SetCheck(1);
	}

	//m_CurrentDoc->m_taiViewF10 = m_pTreeView; 
	m_CurrentDoc->m_F10HeadView = pF10HeadView; 
	OnSize(SIZE_RESTORED,0,0);
    pOldActiveView->DestroyWindow();
	SetWindowText("上市公司基本资料");
    GetMDIFrame()->OnUpdateFrameTitle(TRUE);
}

void CChildFrame::OnClose() 
{
	CView *pView = GetActiveView();
	if(pView == NULL)
	{
	  CMDIChildWnd::OnClose(); 
	}
	else if(pView->IsKindOf(RUNTIME_CLASS(CTaiShanReportView)))
	{
	  return;
	}
	else if(pView->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
	{
	    CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	    CFrameWnd * pActiveWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
		CTaiShanDoc *pDoc = (CTaiShanDoc*)pActiveWnd->GetActiveDocument(); 
		POSITION pos = pDoc->GetFirstViewPosition();
		CView *pView;
	    while(pos != NULL)
		{
			pView = pDoc->GetNextView(pos);
			if(pView->IsKindOf(RUNTIME_CLASS(CTaiShanReportView)))
			{
				((CTaiShanReportView *)pView)->m_nFirst = 1;
			    //CMDIChildWnd::OnClose();
			    break;
			}
		}
	    CMDIChildWnd::OnClose();     
	}
	else
	{
	 CMDIChildWnd::OnClose();
	}
}

void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd) 
{
	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
    //if(GetActiveView() == NULL)
    CView *pView = GetActiveView();
	if(pView == NULL)
		return;
	
	if(bActivate == TRUE && pView->IsKindOf(RUNTIME_CLASS(CTaiShanReportView)))
	{
//	  GetActiveView()->SetFocus(); 
	  ((CTaiShanReportView *)pView)->ActiveGrid();
	  return;
	}
	  
}
LRESULT CChildFrame::OnSetSplitterPos(WPARAM wParam, LPARAM lParam)
{
	long Width=(long)lParam;
	CRect rc;
	GetClientRect(&rc);
	if(rc.right < Width)
       Width=rc.right;  
	m_wndSplitter->SetColumnInfo( 0, rc.right - Width , 0  );
	m_wndSplitter->RecalcLayout( );
	((CTaiShanReportView*)m_wndSplitter->GetPane(0,0))->SetFocus();

	return 1L;
}

void CChildFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
  //CTaiShanDoc* pDoc = (CTaiShanDoc *)GetActiveDocument();
  CTaiShanDoc* pDoc = ((CMainFrame *)AfxGetMainWnd())->m_pDoc;
  CView* pView = GetActiveView();
  CWnd *pWnd = GetWindow(GW_CHILD);
  
  if(bAddToTitle && pDoc != NULL)
  {
	  CString strCurCaption,strWindowText,strNewCaption;
	  if(pView == NULL)
		 return;
	  //if(pWnd->IsKindOf(RUNTIME_CLASS(CTaiTestSplitter)))
	  if(pView->IsKindOf(RUNTIME_CLASS(CTaiTestRichView)))
	  {
	    if(((CTaiTestRichView *)pView)->m_bF9ORF10 == TRUE) 
		   SetWindowText("公告信息");
		if(((CTaiTestRichView *)pView)->m_bF9ORF10 == FALSE) 
           SetWindowText("上市公司基本资料");
		GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
		return;
	  }
	  if(pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
	  {
        GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
		return;
	  }
	  if(!pView->IsKindOf(RUNTIME_CLASS(CInfoView)))
	  {
	   GetWindowText(strCurCaption);
	   GetActiveView()->GetWindowText(strWindowText);
	   const CString&strDocTitle = pDoc->GetTitle();
	   strNewCaption = strDocTitle;
	   if(m_nWindow > 0)
	   {
	    strNewCaption += ":";
		strNewCaption += strWindowText;
	   }
	   if(strNewCaption != strCurCaption)
		SetWindowText(strNewCaption);
	  }
  }
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
}

void CChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
    CView *pView = GetActiveView();
	if(pView == NULL)
	{
		CMDIChildWnd::OnSetFocus(pOldWnd);	
		return;
	}
	if(GetActiveView()->IsKindOf(RUNTIME_CLASS(CTaiShanReportView)))
	{
		((CTaiShanReportView *)GetActiveView())->ActiveGrid();
		return;
	}
	CMDIChildWnd::OnSetFocus(pOldWnd);	
}

void CChildFrame::ActivateFrame(int nCmdShow)
{
	//nCmdShow = SW_SHOWNORMAL;
	nCmdShow = SW_SHOWMAXIMIZED;

	CMDIChildWnd::ActivateFrame(nCmdShow);
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//CWnd* pMainWnd = GetParent();
	//CRect rcMain;

	//pMainWnd->GetClientRect(&rcMain);
	//rcMain.InflateRect(4,4);

	//MoveWindow(rcMain);

	return 0;
}

BOOL CChildFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle , CWnd* pParentWnd , CCreateContext* pContext)
{
	if(!CMDIChildWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}
	//BOOL bRet = m_hMenu.AttachMenu(m_hMenuShared, 0, CSize(16, 15));

	GetMenuCount();

	return TRUE;// CMDIChildWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}

void CChildFrame::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(lpDrawItemStruct->CtlType == ODT_MENU)
	{
		//m_hMenu.DrawItem(lpDrawItemStruct);
		m_iMenuItemWidth = abs(lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left);
		return ;
	}
	CMDIChildWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CChildFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//if(lpMeasureItemStruct->CtlType == ODT_MENU)
	//{
	//	m_hMenu.MeasureItem(lpMeasureItemStruct);
	//	return ;
	//}

	CMDIChildWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CChildFrame::OnInitMenu(CMenu* pMenu)
{
	CMDIChildWnd::OnInitMenu(pMenu);

	// TODO: 在此处添加消息处理程序代码
}

void CChildFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CMDIChildWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	// TODO: 在此处添加消息处理程序代码
}
void CChildFrame::GetMenuCount()
{
	CMenu * pMenu = CMenu::FromHandle(m_hMenuShared);
	int iCount = 0;
	if (pMenu)
	{
		int x = GetSystemMetrics(SM_CXMENUSIZE);
		iCount = pMenu->GetMenuItemCount();
		//for (int i = 0;i < iCount;i++)
		{
			//pMenu->ModifyMenu(i,MF_BYPOSITION|MF_STRING,(UINT)pMenu->GetSubMenu(i)->m_hMenu,"文件(&F)");
			pMenu->ModifyMenu(iCount - 1,MF_BYPOSITION|MF_OWNERDRAW);
		}
		CMainFrame::m_iMenuItemCount = iCount - 1;
	}
}