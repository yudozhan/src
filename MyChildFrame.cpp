// MyChildFrame.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "MyChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiTestChildFrame

IMPLEMENT_DYNCREATE(CTaiTestChildFrame, CMDIChildWnd)

CTaiTestChildFrame::CTaiTestChildFrame()
{
}

CTaiTestChildFrame::~CTaiTestChildFrame()
{
}


BEGIN_MESSAGE_MAP(CTaiTestChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CTaiTestChildFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiTestChildFrame message handlers
