// CwdSelectFormu.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CwdSelectFormu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFxjSelectFormu dialog


CFxjSelectFormu::CFxjSelectFormu(CWnd* pParent /*=NULL*/)
	: CTaiScreenParent(CFxjSelectFormu::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFxjSelectFormu)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bDlgIndicator = true;
	m_bOnlyIndicator = false;
	m_pExplainButton = &m_buttonExpl;
}


void CFxjSelectFormu::DoDataExchange(CDataExchange* pDX)
{
	CTaiScreenParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFxjSelectFormu)
	DDX_Control(pDX, IDC_BUTTON2, m_buttonExpl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFxjSelectFormu, CTaiScreenParent)
	//{{AFX_MSG_MAP(CFxjSelectFormu)
	ON_WM_CLOSE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_FORMULAR_ALL, OnSelchangedTreeFormularAll)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_FORMULAR_ALL, OnDblclkTreeFormularAll)
	ON_BN_CLICKED(IDC_MODIFY__FORMULA, OnModifyFormula)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFxjSelectFormu message handlers

BOOL CFxjSelectFormu::OnInitDialog() 
{
	CTaiScreenParent::OnInitDialog();
	
	//0为指标公式
	this->m_treeFormularCtrl.InsertEqutionTree (0,true);
	if(!m_bOnlyIndicator)
	{
		//条件选股公式
		this->m_treeFormularCtrl.InsertEqutionTree (1,true);
		//K线组合公式
		this->m_treeFormularCtrl.InsertEqutionTree (2);
	}
	HTREEITEM hh = m_treeFormularCtrl.GetRootItem( );
	m_treeFormularCtrl.EnsureVisible(hh);
	if(GetDlgItem(IDC_MODIFY__FORMULA))
	{
		if(this->m_pIndex!=NULL) 
			GetDlgItem(IDC_MODIFY__FORMULA)->ShowWindow(SW_SHOW);//
		else
			GetDlgItem(IDC_MODIFY__FORMULA)->ShowWindow(SW_HIDE);//
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFxjSelectFormu::OnOK() 
{
	if(this->m_pIndex == NULL )
	{
		AfxMessageBox("请选择指标！");
		return;
	}
	if(m_pIndex->isProtected)
	{
		return;
	}
	CTaiScreenParent::OnOK();
}

void CFxjSelectFormu::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CTaiScreenParent::OnCancel();
}

void CFxjSelectFormu::OnClose() 
{
	CTaiScreenParent::OnClose();
}

void CFxjSelectFormu::OnSelchangedTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTaiScreenParent::OnSelchangedTreeFormularAll(pNMHDR, pResult) 	;
	GetDlgItem(IDC_LIST_SUB_FORMULAR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PARAM1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_PARAM2)->ShowWindow(SW_HIDE);//
	
	if(GetDlgItem(IDC_LIST_FORMULAR_LINE_ALL)) GetDlgItem(IDC_LIST_FORMULAR_LINE_ALL)->ShowWindow(SW_HIDE);//
	if(GetDlgItem(IDC_COMBO_PERIOD)) GetDlgItem(IDC_COMBO_PERIOD)->ShowWindow(SW_HIDE);//
	if(GetDlgItem(IDC_STATIC_COND_NEED)) GetDlgItem(IDC_STATIC_COND_NEED)->ShowWindow(SW_HIDE);//
	if(GetDlgItem(IDC_STATIC_P1)) GetDlgItem(IDC_STATIC_P1)->ShowWindow(SW_HIDE);//
	if(GetDlgItem(IDC_STATIC_P2)) GetDlgItem(IDC_STATIC_P2)->ShowWindow(SW_HIDE);//
	if(GetDlgItem(IDC_MODIFY__FORMULA))
	{
		if(this->m_pIndex!=NULL) 
			GetDlgItem(IDC_MODIFY__FORMULA)->ShowWindow(SW_SHOW);//
		else
			GetDlgItem(IDC_MODIFY__FORMULA)->ShowWindow(SW_HIDE);//
	}
	*pResult = 0;
}

void CFxjSelectFormu::OnButton2() 
{
	ShowExplain(this->m_pIndex);
}

CFormularContent* CFxjSelectFormu::GetJishu()
{
	return m_pIndex;
}

void CFxjSelectFormu::OnDblclkTreeFormularAll(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_pIndex)
		OnOK();
	*pResult = 0;
}

void CFxjSelectFormu::RefreshJishuTreeCtrl()
{
}

void CFxjSelectFormu::OnModifyFormula() 
{
	if(this->m_pIndex!=NULL)
		m_treeFormularCtrl.ModifyEquation ();
}
