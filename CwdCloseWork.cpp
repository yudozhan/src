// CwdCloseWork.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CwdCloseWork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFxjCloseWork property page

IMPLEMENT_DYNCREATE(CFxjCloseWork, CPropertyPage)

CFxjCloseWork::CFxjCloseWork() : CPropertyPage(CFxjCloseWork::IDD)
{
	//{{AFX_DATA_INIT(CFxjCloseWork)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CFxjCloseWork::~CFxjCloseWork()
{
}

void CFxjCloseWork::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFxjCloseWork)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFxjCloseWork, CPropertyPage)
	//{{AFX_MSG_MAP(CFxjCloseWork)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFxjCloseWork message handlers

void CFxjCloseWork::OnButton1() 
{
	SendMessage(WM_COMMAND,ID_TOOL_CLOSEWORK);
}
