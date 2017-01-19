// CTaiKlineWndChild.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineWndChild.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineWndChild

IMPLEMENT_DYNCREATE(CTaiKlineWndChild, CMDIChildWnd)

CTaiKlineWndChild::CTaiKlineWndChild()
{
	m_pContext = NULL;
}

CTaiKlineWndChild::~CTaiKlineWndChild()
{
	if(m_pContext)  delete m_pContext;
}


BEGIN_MESSAGE_MAP(CTaiKlineWndChild, CMDIChildWnd)
	//{{AFX_MSG_MAP(CTaiKlineWndChild)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineWndChild message handlers

BOOL CTaiKlineWndChild::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
		m_pContext=new CCreateContext;
		memcpy(m_pContext,pContext,sizeof(CCreateContext));
	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}
