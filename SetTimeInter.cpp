// SetTimeInter.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "SetTimeInter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetTimeInter dialog


CSetTimeInter::CSetTimeInter(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTimeInter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetTimeInter)
	m_TimeInter = 1;
	//}}AFX_DATA_INIT
}


void CSetTimeInter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTimeInter)
	DDX_Control(pDX, IDC_TIMESPIN, m_TimeSpin);
	DDX_Text(pDX, IDC_HOWMANYTIME, m_TimeInter);
	DDV_MinMaxInt(pDX, m_TimeInter, 1, 60);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTimeInter, CDialog)
	//{{AFX_MSG_MAP(CSetTimeInter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTimeInter message handlers

BOOL CSetTimeInter::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_TimeSpin.SetRange(1,60);
	m_TimeSpin.SetPos(m_TimeInter);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
