// DIALOGTEST.cpp : implementation file
//

#include "stdafx.h"
#include "DIALOGTEST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogLOGTEST dialog

 
CDialogLOGTEST::CDialogLOGTEST(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLOGTEST::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogLOGTEST)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogLOGTEST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogLOGTEST)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogLOGTEST, CDialog)
	//{{AFX_MSG_MAP(CDialogLOGTEST)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogLOGTEST message handlers
