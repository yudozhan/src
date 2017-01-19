// mainFrmD.cpp : implementation file
//

#include "stdafx.h"
#include "mainFrmD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// mainFrmD dialog


mainFrmD::mainFrmD(CWnd* pParent /*=NULL*/)
	: CDialog(mainFrmD::IDD, pParent)
{
	//{{AFX_DATA_INIT(mainFrmD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void mainFrmD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(mainFrmD)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(mainFrmD, CDialog)
	//{{AFX_MSG_MAP(mainFrmD)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// mainFrmD message handlers

int mainFrmD::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here// SHEET1.cpp : implementation file
//

#include "stdafx.h"



	
	return 0;
}
