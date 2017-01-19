// CTaiScreenTestResult.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiScreenTestResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiScreenTestResult dialog


CTaiScreenTestResult::CTaiScreenTestResult(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiScreenTestResult::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiScreenTestResult)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTaiScreenTestResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiScreenTestResult)
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_gridResult);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiScreenTestResult, CDialog)
	//{{AFX_MSG_MAP(CTaiScreenTestResult)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiScreenTestResult message handlers

BEGIN_EVENTSINK_MAP(CTaiScreenTestResult, CDialog)
    //{{AFX_EVENTSINK_MAP(CTaiScreenTestResult)
	ON_EVENT(CTaiScreenTestResult, IDC_MSFLEXGRID1, -601 /* DblClick */, OnDblClickMsflexgrid1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTaiScreenTestResult::OnDblClickMsflexgrid1() 
{
	// TODO: Add your control notification handler code here
	
}
