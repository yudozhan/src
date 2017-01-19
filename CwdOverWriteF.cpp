// CwdOverWriteF.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CwdOverWriteF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFxjOverWriteF dialog


CFxjOverWriteF::CFxjOverWriteF(CWnd* pParent /*=NULL*/)
	: CDialog(CFxjOverWriteF::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFxjOverWriteF)
	m_s1 = _T("");
	//}}AFX_DATA_INIT
	m_nResponse = 0;
}


void CFxjOverWriteF::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFxjOverWriteF)
	DDX_Text(pDX, IDC_STATIC_1, m_s1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFxjOverWriteF, CDialog)
	//{{AFX_MSG_MAP(CFxjOverWriteF)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFxjOverWriteF message handlers

void CFxjOverWriteF::OnButtonAll() 
{
	// TODO: Add your control notification handler code here
	m_nResponse = 2;
	CDialog::OnOK();
}

void CFxjOverWriteF::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	m_nResponse = 0;
	CDialog::OnCancel();
}

void CFxjOverWriteF::OnOK() 
{
	// TODO: Add extra validation here
	
	m_nResponse = 1;
	CDialog::OnOK();
}
