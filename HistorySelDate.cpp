// HistorySelDate.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "HistorySelDate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistorySelDate dialog


CHistorySelDate::CHistorySelDate(CWnd* pParent /*=NULL*/)
	: CDialog(CHistorySelDate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistorySelDate)
	m_tm = 0;
	//}}AFX_DATA_INIT
}


void CHistorySelDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistorySelDate)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_tm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistorySelDate, CDialog)
	//{{AFX_MSG_MAP(CHistorySelDate)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistorySelDate message handlers
