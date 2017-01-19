// CTaiKlineShowText.cpp : implementation file
//

#include "stdafx.h"
#include "CTaiKlineShowText.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineDrawLine.h"
#include "CTaiKlineDoKline.h"
//#include "vwbaseDoc.h"
#define TM_SHOW_TEXT   1
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define MESSAGE_SHOW_MY WM_USER+122

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineShowText dialog


CTaiKlineShowText::CTaiKlineShowText(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineShowText::IDD, pParent)
{
	pView = (CTaiShanKlineShowView*) pParent;
	ASSERT(pView!=NULL);

	//{{AFX_DATA_INIT(CTaiKlineShowText)
	m_editText = _T("");
	//}}AFX_DATA_INIT
}


void CTaiKlineShowText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineShowText)
	DDX_Text(pDX, IDC_EDIT2, m_editText);
	DDV_MaxChars(pDX, m_editText, 180);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineShowText, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineShowText)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CANCELMODE()
	ON_WM_SIZE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_WM_ACTIVATEAPP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(MESSAGE_SHOW_MY,ShowMyWin)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineShowText message handlers

BOOL CTaiKlineShowText::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_editText="";
	m_bMove=true;
	// TODO: Add extra initialization here
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
//	CMDIChildWnd *pChild =(CMDIChildWnd *) pFm->GetActiveFrame();
//	CTaiShanKlineShowView* pView=pFm->pVwbaseView;
	x=pView->m_toScreen.x;
	y=pView->m_toScreen.y;
//	MoveWindow(x,y,60,20);
	SetTimer(TM_SHOW_TEXT,5,NULL);

//	PostMessage(MESSAGE_SHOW_MY);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaiKlineShowText::OnChangeEdit2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	int n = m_editText.GetLength();
	if(n>=31)
		AfxMessageBox("超过最大字符容量",MB_ICONSTOP);
	else
		UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	
}


void CTaiKlineShowText::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::CalcWindowRect(lpClientRect, nAdjustType);
}

BOOL CTaiKlineShowText::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
//	CMDIChildWnd *pChild =(CMDIChildWnd *) pFm->GetActiveFrame();
//	CTaiShanKlineShowView* pView=pFm->pVwbaseView;
	x=pView->pDrawLine ->m_pt[0].x;
	y=pView->pDrawLine ->m_pt[0].y;
	cs.x=x;
	cs.y=y;
	cs.cx=60;
	cs.cy=20;
	return CDialog::PreCreateWindow(cs);
}
void CTaiKlineShowText::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}

void CTaiKlineShowText::ShowWin()
{
}


int CTaiKlineShowText::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}
LRESULT CTaiKlineShowText::ShowMyWin(WPARAM wParam,LPARAM lParam)
{
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
//	CTaiShanKlineShowView* pView=pFm->pVwbaseView;
	x=pView->m_toScreen.x;
	y=pView->m_toScreen.y;
	MoveWindow(x,y,90,20);
	RedrawWindow();
//	SetWindowPos(pView, 0, 0, 0, 0, SWP_HIDEWINDOW|
//					SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
	return 1L;
}

void CTaiKlineShowText::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CDialog::OnPaint() ;
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CTaiKlineShowText::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

int CTaiKlineShowText::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DoModal();
}

void CTaiKlineShowText::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
/*	if(m_bMove==true)
	{
		PostMessage(MESSAGE_SHOW_MY);
		m_bMove=false;
		return;
	}*/
	
	// TODO: Add your message handler code here
	
}

BOOL CTaiKlineShowText::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	int i;
	i=0;
	return CDialog::Create(IDD, pParentWnd);
}

void CTaiKlineShowText::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanging(lpwndpos);
	
	// TODO: Add your message handler code here
	
}

void CTaiKlineShowText::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(TM_SHOW_TEXT);
	ShowMyWin(0,0);
	CDialog::OnTimer(nIDEvent);
}

void CTaiKlineShowText::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	
}

void CTaiKlineShowText::OnActivateApp(BOOL bActive, DWORD hTask) 
{
	CDialog::OnActivateApp(bActive, hTask);
	
	// TODO: Add your message handler code here
	
}
