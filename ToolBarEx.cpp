// ToolBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx

CToolBarEx::CToolBarEx()
{
}

CToolBarEx::~CToolBarEx()
{
}


BEGIN_MESSAGE_MAP(CToolBarEx, CTabCtrl)
	//{{AFX_MSG_MAP(CToolBarEx)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarEx message handlers

void CToolBarEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CTabCtrl::OnPaint() for painting messages
}

BOOL CToolBarEx::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
