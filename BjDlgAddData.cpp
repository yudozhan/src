// BjDlgAddData.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "BjDlgAddData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BjDlgAddData dialog


BjDlgAddData::BjDlgAddData(CWnd* pParent /*=NULL*/)
	: CDialog(BjDlgAddData::IDD, pParent)
{
	//{{AFX_DATA_INIT(BjDlgAddData)
	m_nDays = 10;
	//}}AFX_DATA_INIT

	m_color = RGB(0,0,0);
}


void BjDlgAddData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BjDlgAddData)
	DDX_Text(pDX, IDC_EDIT1, m_nDays);
	DDV_MinMaxUInt(pDX, m_nDays, 0, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(BjDlgAddData, CDialog)
	//{{AFX_MSG_MAP(BjDlgAddData)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonSet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// BjDlgAddData message handlers

void BjDlgAddData::OnButtonSet() 
{
    CColorDialog m_colordia(0,CC_RGBINIT,this);
    if(m_colordia.DoModal())
	{
		m_color=m_colordia.GetColor();
	}	
	
}

void BjDlgAddData::OnOK() 
{
	
	CDialog::OnOK();
}

void BjDlgAddData::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
