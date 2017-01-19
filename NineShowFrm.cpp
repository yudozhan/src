// ChildFrm.cpp : implementation of the CNineShowFrame class
//

#include "stdafx.h"
#include "WH.h"

#include "NineShowFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNineShowFrame

IMPLEMENT_DYNCREATE(CNineShowFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CNineShowFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CNineShowFrame)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNineShowFrame construction/destruction

CNineShowFrame::CNineShowFrame()
{
	// TODO: add member initialization code here
	
}

CNineShowFrame::~CNineShowFrame()
{
}

BOOL CNineShowFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.x=0;
	cs.y=0;
	cs.cx=::GetSystemMetrics(SM_CXFULLSCREEN);
	cs.cy=::GetSystemMetrics(SM_CYFULLSCREEN)-40;
	cs.style&=~WS_THICKFRAME;
 	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CNineShowFrame diagnostics

#ifdef _DEBUG
void CNineShowFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CNineShowFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNineShowFrame message handlers
void CNineShowFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CMDIChildWnd::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
	//this->MDIMaximize();
}

void CNineShowFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
  CDocument* pDoc = GetActiveDocument();
  if(bAddToTitle && pDoc != NULL)
  {
	  CString strCurCaption,strWindowText,strNewCaption;
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
	GetMDIFrame()->OnUpdateFrameTitle(bAddToTitle);
}
