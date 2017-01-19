// CTaiKlineDialogShiDuanTJ.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineDialogShiDuanTJ.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "ChildFrm.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDialogShiDuanTJ dialog


CTaiKlineDialogShiDuanTJ::CTaiKlineDialogShiDuanTJ(CWnd* pParent /*=NULL*/,int nTimeStyle)
	: CDialog(CTaiKlineDialogShiDuanTJ::IDD, pParent)
{
	pView = (CTaiShanKlineShowView* )pParent;
	m_nKlineKind2 = pView->m_nKlineKind2;

	if(m_nKlineKind2 >0)
	pView->pKlineDrawing ->m_bInvertRect=true;
	else
	pView->pMin1Drawing->m_bInvertRect=true;
	if (nTimeStyle<0 || nTimeStyle>1)
		nTimeStyle=0;
	//{{AFX_DATA_INIT(CTaiKlineDialogShiDuanTJ)
	m_chengBenTJ = _T("");
	m_close = _T("");
	m_high = _T("");
	m_junjia = _T("");
	m_low = _T("");
	m_open = _T("");
	m_num = _T("");
	m_vol = _T("");
	m_zhDie = _T("");
	m_zhDieFu = _T("");
	m_amount = _T("");
	m_huanShou = _T("");
	m_nTimeStyle=nTimeStyle;
	m_tmB = 0;
	m_tmE = 0;
	//}}AFX_DATA_INIT

}


void CTaiKlineDialogShiDuanTJ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);//
	//{{AFX_DATA_MAP(CTaiKlineDialogShiDuanTJ)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Text(pDX, IDC_EDIT_CHENBEN, m_chengBenTJ);
	DDX_Text(pDX, IDC_EDIT_CLOSE, m_close);
	DDX_Text(pDX, IDC_EDIT_HIGH, m_high);
	DDX_Text(pDX, IDC_EDIT_JUN_JIA, m_junjia);
	DDX_Text(pDX, IDC_EDIT_LOW, m_low);
	DDX_Text(pDX, IDC_EDIT_OPEN, m_open);
	DDX_Text(pDX, IDC_EDIT_TIME_NUM, m_num);
	DDX_Text(pDX, IDC_EDIT_VOL, m_vol);
	DDX_Text(pDX, IDC_EDIT_ZHD, m_zhDie);
	DDX_Text(pDX, IDC_EDIT_ZHD_FU, m_zhDieFu);
	DDX_Text(pDX, ID_AMOUNT_TJ, m_amount);
	DDX_Text(pDX, IDC_EDIT_HUANSHOU, m_huanShou);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDialogShiDuanTJ, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDialogShiDuanTJ)
	ON_WM_CLOSE()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_NOTIFY(DTN_DATETIMECHANGE,IDC_DATETIME_1, OnDTNotify)
	ON_NOTIFY(DTN_DATETIMECHANGE,IDC_DATETIME_2, OnDTNotify2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDialogShiDuanTJ message handlers
void CTaiKlineDialogShiDuanTJ::OnDTNotify(NMHDR *pHdr, LRESULT * pRes)
{
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_pDoc; 
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	CTime tmB(1970,1,1,9,30,0);
	CTime tmNow;
	int nYear;
	int nMon,nDay;
	switch( pHdr->code)
	{
	case DTN_DATETIMECHANGE:
		if (pView->m_nKlineKind2==0)
			m_DateTime.GetTime(m_tmB);
		else
		{
			m_DateTime.GetTime(tmNow);
			nYear=tmNow.GetYear ();
			nMon=tmNow.GetMonth ();
			nDay=tmNow.GetDay ();
			if(true)
			{
				CTime tm(nYear,nMon,nDay,9,30,0);
				m_tmB=tm;
			}
		}
		if(pView->pKlineDrawing->TongJi(this,1)>0)
			UpdateData(FALSE);
//		else
//			AfxMessageBox("请重新输入时间");

		break;
	}

	*pRes = 0L;

}
void CTaiKlineDialogShiDuanTJ::OnDTNotify2(NMHDR *pHdr, LRESULT * pRes)
{
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_pDoc; 
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();

	CString s1;
	s1=pView->pMin1Drawing->m_pReportData->name;
	s1=s1+" 时段统计";
	SetWindowText(s1);

	CTime tmNow;
	int nYear;
	int nMon,nDay;
	switch( pHdr->code)
	{
	case DTN_DATETIMECHANGE:
		if (pView->m_nKlineKind2==0)
			m_DateTime2.GetTime(m_tmE);
		else
		{
			m_DateTime2.GetTime(tmNow);
			nYear=tmNow.GetYear ();
			nMon=tmNow.GetMonth ();
			nDay=tmNow.GetDay ();
			if(true)
			{
				CTime tm(nYear,nMon,nDay,9,30,0);
				m_tmE=tm;
			}//TongJi(CTaiKlineDialogShiDuanTJ* pTJ,int nFlags)//统计数据
		}
		if(pView->pKlineDrawing->TongJi(this,1)>0)
			UpdateData(FALSE);
//		else
//			AfxMessageBox("请重新输入时间");
		break;
	}
	*pRes = 0L;
}
BOOL CTaiKlineDialogShiDuanTJ::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CTaiShanDoc* pDoc=((CMainFrame*)AfxGetMainWnd())->m_pDoc; 
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();

	
//	CTime tmB(1970,1,1,9,30,0);
//	m_DateTime.SetTime(&tmB);
	CreateTimeCtl();
	//m_tmB = CTime::GetCurrentTime();
	//m_tmE = CTime::GetCurrentTime();
	m_DateTime .SetTime(&m_tmB);
	m_DateTime2 .SetTime(&m_tmE);
	if (pView->m_nKlineKind2==0)
	{
		int nYear=m_tmB.GetYear();
		int nMonth=m_tmB.GetMonth();
		int nDay=m_tmB.GetDay();
		CTime tm1(nYear,nMonth,nDay,9,30,0);
		CTime tm2(nYear,nMonth,nDay,15,0,0);
		m_DateTime.SetRange(&tm1,&tm2);
		m_DateTime2.SetRange(&tm1,&tm2);
	}
	else
	{
		CTime tm1(1980,1,1,8,0,0,0);
		CTime tm2(2036,1,1,8,0,0,0);
		m_DateTime.SetRange( &tm1,&tm2 );
		m_DateTime2.SetRange( &tm1,&tm2 );
	}

	if(pView->m_nKlineKind2 >0 &&pView->m_nKlineKind2 <5)
	{
		m_DateTime2.SetFormat("yyy'/'MM'/'dd','hh':'mm" );
		m_DateTime.SetFormat("yyy'/'MM'/'dd','hh':'mm" );
	}
	else if(pView->m_nKlineKind2 >=5)
	{
		m_DateTime2.SetFormat("yyy'/'MM'/'dd");
		m_DateTime.SetFormat("yyy'/'MM'/'dd");
	}
	else if(pView->m_nKlineKind2==0)
	{
		m_DateTime.SetFormat("H':'mm");
		m_DateTime2.SetFormat("H':'mm");
	}

	SetWindowText("时段统计");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaiKlineDialogShiDuanTJ::CreateTimeCtl()
{
	CWnd *pWnd = GetDlgItem(IDC_STATIC_TIME1);
	CRect rc;
	DWORD dwStyle;
	if (m_nTimeStyle==0)
		dwStyle=WS_CHILD|WS_VISIBLE|DTS_SHORTDATEFORMAT;
	else
		dwStyle=WS_CHILD|WS_VISIBLE|DTS_TIMEFORMAT;

	if(!pWnd) return ;
	pWnd->GetWindowRect(rc);
	pWnd->DestroyWindow(); //pWnd was just a placeholder;
	ScreenToClient(rc);
	m_DateTime.Create(dwStyle,rc,this,IDC_DATETIME_1);
//	m_nPrevStyle=DTS_SHORTDATEFORMAT;

	pWnd = GetDlgItem(IDC_STATIC_TIME2);
	if(!pWnd) return ;
	pWnd->GetWindowRect(rc);
	pWnd->DestroyWindow(); //pWnd was just a placeholder;
	ScreenToClient(rc);
	m_DateTime2.Create(dwStyle,rc,this,IDC_DATETIME_2);
//	m_nPrevStyle=DTS_SHORTDATEFORMAT;

}

void CTaiKlineDialogShiDuanTJ::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_nKlineKind2 >0)
	{
		pView->m_dlgKlineTj = 0;
		pView->pKlineDrawing ->m_bInvertRect = false;
	}
	else
	{
		pView->m_dlgFenshiTj = 0;
		pView->pMin1Drawing ->m_bInvertRect = false;
	}
//	pView->pMin1Drawing->m_bInvertRect = false;
	pView->RedrawWindow ();
	delete this;
	CDialog::PostNcDestroy();
}

void CTaiKlineDialogShiDuanTJ::OnOK() 
{
	// TODO: Add extra validation here
	DestroyWindow();
//	CDialog::OnOK();
}

void CTaiKlineDialogShiDuanTJ::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	DestroyWindow();
//	CDialog::OnClose();
}

void CTaiKlineDialogShiDuanTJ::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	DestroyWindow();
//	CDialog::OnCancel();
}

BOOL CTaiKlineDialogShiDuanTJ::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,CTaiKlineDialogShiDuanTJ::IDD);
//	return CDialog::OnHelpInfo(pHelpInfo);
	DoHtmlHelp(this);return TRUE;
}
