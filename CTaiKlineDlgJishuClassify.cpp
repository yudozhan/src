// CTaiKlineDlgJishuClassify.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineDlgJishuClassify.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgJishuClassify dialog


CTaiKlineDlgJishuClassify::CTaiKlineDlgJishuClassify(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDlgJishuClassify::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiKlineDlgJishuClassify)
	//}}AFX_DATA_INIT
	m_bInit = false;
}


void CTaiKlineDlgJishuClassify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgJishuClassify)
	DDX_Control(pDX, IDC_TREE1, m_treeFormularCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgJishuClassify, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgJishuClassify)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, OnRclickTree1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgJishuClassify message handlers

void CTaiKlineDlgJishuClassify::OnOk() 
{
	// TODO: Add your control notification handler code here
//	DestroyWindow();
}

void CTaiKlineDlgJishuClassify::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	DestroyWindow();
//	CDialog::OnCancel();
}

void CTaiKlineDlgJishuClassify::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

BOOL CTaiKlineDlgJishuClassify::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->m_treeFormularCtrl.InsertEqutionTree (0);
	this->m_treeFormularCtrl.InsertEqutionTree (1);
	this->m_treeFormularCtrl.InsertEqutionTree (2);
	this->m_treeFormularCtrl.InsertEqutionTree (3);
	
	// TODO: Add extra initialization here
	m_bInit = true;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaiKlineDlgJishuClassify::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	if(m_bInit == true)
	{
		CRect r(0,0,cx,cy) ;
		this->m_treeFormularCtrl .MoveWindow (r);
	}
	
	// TODO: Add your message handler code here
	
}

BOOL CTaiKlineDlgJishuClassify::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
/*	strNewEquationGroup = "新建公式组";
	strDeleteEquationGroup = "删除公式组";
	strChangeNameGroup = "更改组名";
	strNewEquation = "新建公式";
	strModifyEquation = "修改公式";
	strDeleteEquation = "删除公式";
	strUsageIndex = "指标用法";```
	strShowIndex = "显示指标";
	strSelectStock = "条件选股";
const int nNewEquationGroup		= 201;
const int nDeleteEquationGroup	= 202;
const int nChangeNameGroup		= 203;
const int nNewEquation			= 204;
const int nModifyEquation		= 205;
const int nDeleteEquation		= 206;
const int nUsageIndex			= 207;
const int nShowIndex			= 208;
const int nSelectStock			= 209;
*/
	switch(wParam)
	{
	case nNewEquationGroup:
		m_treeFormularCtrl.CreateNewGroup ();
		break;
	case nDeleteEquationGroup:
		m_treeFormularCtrl.DeleteGroup ();
		break;
	case nChangeNameGroup:
		m_treeFormularCtrl.ModifyGroupName ();
		break;
	case nNewEquation:
		m_treeFormularCtrl.CreateNewEquation ();
		break;
	case nModifyEquation:
		m_treeFormularCtrl.ModifyEquation ();
		break;
	case nDeleteEquation:
		m_treeFormularCtrl.DeleteEquation ();
		break;
	default:
		return CDialog::OnCommand(wParam, lParam);
	}
	return TRUE;
}

void CTaiKlineDlgJishuClassify::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CTaiKlineDlgJishuClassify::OnRclickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UINT uFlags;
	CPoint pt;
	::GetCursorPos (&pt);
	CPoint ptOld(pt);
	m_treeFormularCtrl.ScreenToClient (&pt);
	HTREEITEM hItem = this->m_treeFormularCtrl .HitTest(pt, &uFlags);
	if ((hItem != NULL) )
	{
	   this->m_treeFormularCtrl .SelectItem(hItem);
	}

	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc = m_treeFormularCtrl.GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);

	CMenu menu;
	menu.CreatePopupMenu( );
	if(pIndex != NULL)//END ITEM
	{
		menu.AppendMenu(MF_STRING | MF_ENABLED, nModifyEquation, strModifyEquation);
		menu.AppendMenu(MF_STRING | MF_ENABLED, nDeleteEquation, strDeleteEquation);
	}
	else if(nKindItem == 0)//SUB
	{
		menu.AppendMenu(MF_STRING | MF_ENABLED, nDeleteEquationGroup, strDeleteEquationGroup);
		menu.AppendMenu(MF_STRING | MF_ENABLED, nChangeNameGroup, strChangeNameGroup);
		menu.AppendMenu(MF_STRING | MF_ENABLED, nNewEquation, strNewEquation);
	}
	else if(nKindRoot<=-1 && nKindRoot>=-4)//ROOT
	{
		menu.AppendMenu(MF_STRING | MF_ENABLED, nNewEquationGroup, strNewEquationGroup);
	}
	else
		ASSERT(FALSE);
	(menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON  ,
			ptOld.x,ptOld.y,this, NULL));

	*pResult = 0;
}
