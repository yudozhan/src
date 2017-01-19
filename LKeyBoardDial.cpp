// LKeyBoardDial.cpp : implementation file
//

#include "stdafx.h"
#include "LKeyBoardDial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LKeyBoardDial dialog


LKeyBoardDial::LKeyBoardDial(CWnd* pParent /*=NULL*/)
	: CDialog(LKeyBoardDial::IDD, pParent)
{
	//{{AFX_DATA_INIT(LKeyBoardDial)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void LKeyBoardDial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LKeyBoardDial)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LKeyBoardDial, CDialog)
	//{{AFX_MSG_MAP(LKeyBoardDial)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LKeyBoardDial message handlers

void LKeyBoardDial::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}
