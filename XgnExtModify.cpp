// XgnExtModify.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "XgnExtModify.h"
#include "XgnExtDataDlg.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// XgnExtModify dialog


XgnExtModify::XgnExtModify(CWnd* pParent /*=NULL*/)
	: CTaiScreenParent(XgnExtModify::IDD, pParent)
{
	//{{AFX_DATA_INIT(XgnExtModify)
	m_sExtName = _T("数据1");
	m_nID = 0;
	//}}AFX_DATA_INIT
	m_bXgnExt = true;
	m_pExplainButton = &m_buttonExpl;
	m_nSelInfo = -1;
	m_pParent = (XgnExtDataDlg*)pParent;

}


void XgnExtModify::DoDataExchange(CDataExchange* pDX)
{
	CTaiScreenParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(XgnExtModify)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_COMBO_TARGET_PERIOD, m_combTarget);
	DDX_Control(pDX, IDC_BUTTON1, m_buttonExpl);
	DDX_Text(pDX, IDC_EDIT1, m_sExtName);
	DDV_MaxChars(pDX, m_sExtName, 15);
	DDX_Text(pDX, IDC_EDIT2, m_nID);
	DDV_MinMaxUInt(pDX, m_nID, 0, 999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(XgnExtModify, CTaiScreenParent)
	//{{AFX_MSG_MAP(XgnExtModify)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonExplain)
	ON_CBN_SELCHANGE(IDC_COMBO_PERIOD, OnSelchangeComboPeriod)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// XgnExtModify message handlers

BOOL XgnExtModify::OnInitDialog() 
{
	CTaiScreenParent::OnInitDialog();

	m_conRich.ShowWindow (SW_HIDE);
	
	this->m_treeFormularCtrl.InsertEqutionTree (0,true);

	m_combTarget.AddString ("日线");
	m_combTarget.AddString ("5分钟K线");
	if(	m_nSelInfo != -1)
	{
		if(m_infoData.m_nKlineKind == 1)
			m_combTarget.SetCurSel (1);
		else
			m_combTarget.SetCurSel (0);
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
		CString s = m_infoData.m_chName ;
		CFormularContent* pp = CTaiScreenParent::LookUpArray(-1, s,CMainFrame::m_pDoc );
		this->m_treeFormularCtrl .SelectItemJ( pp);

		int n = this->m_sLineAll.GetCount ();
		CString ssub = m_infoData.m_chSubName ;
		for(int i = 0;i<n;i++)
		{
			CString s2;
			m_sLineAll.GetLBText (i,s2);
			if(s2 == ssub)
			{
				m_sLineAll.SetCurSel (i);
				break;
			}
		}

		m_sExtName = m_infoData.m_chDataName ;
		m_nID = m_infoData.m_nID ;
		int nf = GetPeriodFoot(m_infoData.m_nKlineKindCompute );
		if(nf>=0)
			this->m_period.SetCurSel (nf);
	}
	else
		m_combTarget.SetCurSel (0);
	
//	m_combTarget.EnableWindow (FALSE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void XgnExtModify::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CTaiScreenParent::OnClose();
}

void XgnExtModify::OnDestroy() 
{
	CTaiScreenParent::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL XgnExtModify::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CTaiScreenParent::PreTranslateMessage(pMsg);
}

void XgnExtModify::OnButtonExplain() 
{
	// TODO: Add your control notification handler code here
	ShowExplain(this->m_pIndex);

}

void XgnExtModify::OnOK() 
{
	// TODO: Add extra validation here
	if(!UpdateData())
		return;
	if(this->m_pIndex == NULL)//please select indicator;
		return;

	if(	m_nSelInfo == -1)
	{
		bool b = this->m_pParent ->IsValidID(m_nID);
		if(b == false)
		{
			m_nID = m_pParent->GetAValidID ();
			UpdateData(FALSE);
		}
	}
	m_infoData.m_nID = this->m_nID;//need test

	int nSel = m_combTarget.GetCurSel ();
	m_infoData.m_nKlineKind = 5;
	if(nSel ==LB_ERR )
		nSel = 0;
	{
		if(nSel == 1)
			m_infoData.m_nKlineKind  = 1;
	}

	strcpy(m_infoData.m_chDataName ,this->m_sExtName );
	nSel = this->m_sLineAll .GetCurSel ();
	m_infoData.m_chSubName [0] = '\0';
	if(nSel ==LB_ERR )
		nSel = 0;
	{
		CString ss;
		m_sLineAll.GetLBText (nSel,ss);
		strcpy(m_infoData.m_chSubName ,ss );
	}
	m_infoData.m_reserved = nSel;
	strcpy(m_infoData.m_chName ,this->m_pIndex->name );

	nSel = m_period.GetCurSel ();
	m_infoData.m_nKlineKindCompute   = 5;
	if(nSel ==LB_ERR )
		nSel = 0;
	m_infoData.m_nKlineKindCompute   = nLKind[nSel];

//	char str[32];
	float fVal[8];
	m_conRich.GetParamVal((float *)fVal);
	for(int i=0;i<8;i++)
	{
        m_infoData.fParam[i] =fVal[i];
	}
//	for(int i=0;i<num;i++)
//	{
//		m_param_list.GetItemText(i,1,str,31);
//        m_infoData.fParam[i] = float( atof(str) );
//	}

	CTaiScreenParent::OnOK();
}

void XgnExtModify::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CTaiScreenParent::OnCancel();
}

void XgnExtModify::OnSelchangeComboPeriod() 
{
	// TODO: Add your control notification handler code here
	int	nSel = m_period.GetCurSel ();
	if(nLKind[nSel] == 1)
	{
//		m_combTarget.EnableWindow (TRUE);
	}
	else
	{
//		int nSel2 = 0 ;
//		m_combTarget.SetCurSel (nSel2);
//		m_combTarget.EnableWindow (FALSE);
	}


}
