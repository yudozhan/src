// TechDlgEditIndecator.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineDlgEditIndecator.h"
#include "MainFrm.h"
#include "CFormularCompute.h"
//#include "DIAEDITZBGSHS.h"
#include "DIALOGGSSM.h"
#include "CFormularContent.h"
#include "CTaiKlineIndicatorsManage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgEditIndecator dialog


CTaiKlineDlgEditIndecator::CTaiKlineDlgEditIndecator(CWnd* pParent )
	: CTaiKlineDlgEditScreenStock(CTaiKlineDlgEditIndecator::IDD, pParent)
{

	m_maxChars = 10;
	//{{AFX_DATA_INIT(CTaiKlineDlgEditIndecator)
	//}}AFX_DATA_INIT
}
void CTaiKlineDlgEditIndecator::DoDataExchange(CDataExchange* pDX)
{
	CTaiKlineDlgEditScreenStock::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgEditIndecator)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgEditIndecator, CTaiKlineDlgEditScreenStock)
	//{{AFX_MSG_MAP(CTaiKlineDlgEditIndecator)
	ON_BN_CLICKED(IDC_MYHELP_2, OnMyhelp)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgEditIndecator message handlers


BOOL CTaiKlineDlgEditIndecator::OnInitDialog() 
{
	CTaiKlineDlgEditScreenStock::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaiKlineDlgEditIndecator::OnOK() 
{
	
	CTaiKlineDlgEditScreenStock::OnOK();
}

void CTaiKlineDlgEditIndecator::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

///////////////////////////////////////////////////////////////////
//      功能说明：改变技术指标数据的指标注释内容
//      入口参数：无
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDlgEditIndecator::OnMyhelp() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDialogLOGGSSM mdiagssm;
	mdiagssm.m_bReadOnly = false;
	mdiagssm.m_edit=m_myhelp;
	if(mdiagssm.DoModal()==IDOK)
		m_myhelp=mdiagssm.m_edit;
	UpdateData(FALSE);
	
}

BOOL CTaiKlineDlgEditIndecator::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,CTaiKlineIndicatorsManage::IDD);
//	return TRUE;//CTaiKlineDlgEditScreenStock::OnHelpInfo(pHelpInfo);
	DoHtmlHelp(this);return TRUE;
}
