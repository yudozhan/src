// CTaiKlineIndicatorsManage.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineIndicatorsManage.h"
#include "CTaiKlineDlgIndicatorOften.h"
#include "MainFrm.h"
#include "DIALOGGSGL.h"
#include "CTaiShanKlineShowView.h"
#include "CwdExportFormular.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineIndicatorsManage dialog

static CString g_strFileDialogPath = "";
CTaiKlineIndicatorsManage::CTaiKlineIndicatorsManage(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineIndicatorsManage::IDD, pParent)
{
	m_nKindRoot = -1;
	m_pIndex = NULL;
	//{{AFX_DATA_INIT(CTaiKlineIndicatorsManage)
	m_editexplain = _T("");
	m_tabedit = _T("");
	//}}AFX_DATA_INIT
}


void CTaiKlineIndicatorsManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineIndicatorsManage)
	DDX_Control(pDX, IDC_OFTEN, m_often);
	DDX_Control(pDX, IDC_MODIFY__FORMULA, m_conModify);
	DDX_Control(pDX, IDC_ADD_FORMULA, m_conAdd);
	DDX_Control(pDX, ID_DELETE_FORMULA, m_conDelete);
	DDX_Control(pDX, IDC_READ, m_conRead);
	DDX_Control(pDX, IDC_SAVE, m_conSave);
	DDX_Control(pDX, IDC_TREE1, m_treeFormularCtrl);
	DDX_Text(pDX, IDC_BRIEFEXPLAIN, m_editexplain);
	DDX_Text(pDX, IDC_TABEDIT, m_tabedit);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineIndicatorsManage, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineIndicatorsManage)
	ON_BN_CLICKED(IDC_ADD_FORMULA, OnAddFormula)
	ON_BN_CLICKED(IDC_MODIFY__FORMULA, OnModifyFormula)
	ON_BN_CLICKED(ID_DELETE_FORMULA, OnDeleteFormula)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_READ, OnRead)
	ON_BN_CLICKED(IDC_OFTEN, OnOften)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_WM_HELPINFO()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SELECT, OnSelchangeTabSelect)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineIndicatorsManage message handlers


///////////////////////////////////////////////////////////////////
//      ����˵�������ӹ�ʽ
//      ��ڲ�������
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineIndicatorsManage::OnAddFormula() 
{
	// TODO: Add your control notification handler code here
	m_treeFormularCtrl.CreateNewEquation ();
}

///////////////////////////////////////////////////////////////////
//      ����˵�����޸Ĺ�ʽ
//      ��ڲ�������
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineIndicatorsManage::OnModifyFormula() 
{
	// TODO: Add your control notification handler code here
	if(this->m_pIndex!=NULL)
		m_treeFormularCtrl.ModifyEquation ();
}

///////////////////////////////////////////////////////////////////
//      ����˵����ɾ����ʽ
//      ��ڲ�������
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineIndicatorsManage::OnDeleteFormula() 
{
	// TODO: Add your control notification handler code here
	m_treeFormularCtrl.DeleteEquation ();
	
}

///////////////////////////////////////////////////////////////////
//      ����˵������ѡ����ָ�깫ʽ���浽�ļ���
//      ��ڲ�������
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineIndicatorsManage::OnSave() 
{
	// TODO: Add your control notification handler code here
	CFxjExportFormular dlgSave(this);
//	CDialogLOGGSGL dlgSave;
	dlgSave.DoModal ();
	
}

///////////////////////////////////////////////////////////////////
//      ����˵������ѡ�����ļ�����ָ�깫ʽ
//      ��ڲ�������
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineIndicatorsManage::OnRead() 
{
	// TODO: Add your control notification handler code here
	CTaiShanDoc*   pDoc=CMainFrame::m_pDoc ;

	CString     defaultname;

	defaultname="*.ts";  
	CString     defaultname2 = defaultname+"||";

	CFileDialog  m_filedia(TRUE,0,defaultname,OFN_CREATEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR,defaultname2);

	if(g_strFileDialogPath == "")
		m_filedia.m_ofn .lpstrInitialDir = CMainFrame::m_pDoc ->m_CurrentWorkDirectory;
	else
		m_filedia.m_ofn .lpstrInitialDir = g_strFileDialogPath;

	if(m_filedia.DoModal()==IDOK)
	{
		CString filename=m_filedia.GetPathName();
		CString ss2 = m_filedia.GetFileName();
		CString sPath = filename;
		sPath.Delete(filename.GetLength()-ss2.GetLength(),ss2.GetLength());
		g_strFileDialogPath = sPath;

		if(CFormularContent::InstallIndicatorCwd(filename,true,false) == false)
		{
			m_editexplain = _T("");
			m_tabedit = _T("");
			UpdateData(FALSE);

			return;
		}
		this->m_treeFormularCtrl .DeleteAllItems( );
		//0Ϊָ�깫ʽ
		this->m_treeFormularCtrl.InsertEqutionTree (0);
		//����ѡ�ɹ�ʽ
		this->m_treeFormularCtrl.InsertEqutionTree (1);
		//K����Ϲ�ʽ
		this->m_treeFormularCtrl.InsertEqutionTree (2);
		m_conDelete.EnableWindow (FALSE);
		m_conModify.EnableWindow (FALSE);
		
		m_editexplain = _T("");
		m_tabedit = _T("");
		UpdateData(FALSE);
	}
	
	CFormularContent::ReadWriteIndex(pDoc, 0, false);
	CFormularContent::ReadWriteIndex(pDoc, 1, false);
	CFormularContent::ReadWriteIndex(pDoc, 2, false);
	//::SetCurrentDirectory (CMainFrame::m_pDoc ->m_CurrentWorkDirectory);

}

void CTaiKlineIndicatorsManage::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CTaiKlineIndicatorsManage::OnOK() 
{
	// TODO: Add extra validation here
	CTaiShanDoc*   pDoc=CMainFrame::m_pDoc ;
	CFormularContent::ReadWriteIndex(pDoc, 0, false);
	CFormularContent::ReadWriteIndex(pDoc, 1, false);
	CFormularContent::ReadWriteIndex(pDoc, 2, false);
	CDialog::OnOK();
	CTaiShanKlineShowView::OnDataChangAll(200);
}

BOOL CTaiKlineIndicatorsManage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//m_btOK.SetIcon(IDI_ICON_OK);
	//m_btCancel.SetIcon(IDI_EXIT16);
	//m_conDelete.SetIcon(IDI_ICON_DELETE);
	//m_conAdd.SetIcon(IDI_ICON_ADD2);
	
	((CTabCtrl*)GetDlgItem(IDC_TAB_SELECT))->InsertItem( 0, "����");
	((CTabCtrl*)GetDlgItem(IDC_TAB_SELECT))->InsertItem( 1, "ȫ��");
	((CTabCtrl*)GetDlgItem(IDC_TAB_SELECT))->InsertItem( 2, "����");

	// TODO: Add extra initialization here
	//0Ϊָ�깫ʽ
	this->m_treeFormularCtrl.InsertEqutionTree (0);
	//����ѡ�ɹ�ʽ
	this->m_treeFormularCtrl.InsertEqutionTree (1);
	//K����Ϲ�ʽ
	this->m_treeFormularCtrl.InsertEqutionTree (2);
	m_conDelete.EnableWindow (FALSE);
	m_conModify.EnableWindow (FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////
//      ����˵�������ó��ü���ָ��
//      ��ڲ�������
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineIndicatorsManage::OnOften() 
{
	// TODO: Add your control notification handler code here
	CTaiKlineDlgIndicatorOften often;
	often.DoModal ();
}

///////////////////////////////////////////////////////////////////
//      ����˵������ѡ���ָ�깫ʽ�仯����״������Ի��򷢳���֪ͨ��Ϣ
//      ��ڲ�������
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineIndicatorsManage::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	//UINT uFlags;
	CPoint pt;
	::GetCursorPos (&pt);
	CPoint ptOld(pt);
//	m_treeFormularCtrl.ScreenToClient (&pt);

	//�õ���״���Ƶı�ѡ�������
	HTREEITEM hItem = m_treeFormularCtrl.GetSelectedItem ();//this->m_treeFormularCtrl.HitTest(pt, &uFlags);
	if(hItem == NULL)
	{
		m_conDelete.EnableWindow (FALSE);
		m_conModify.EnableWindow (FALSE);
		*pResult = 0;
		m_editexplain = _T("");
		m_tabedit = _T("");
		UpdateData(FALSE);
		return;
	}
	if ((hItem != NULL) )
	{
	   this->m_treeFormularCtrl .SelectItem(hItem);
	}
	int nKindRoot,nKindItem;

	//�õ���״���Ƶı�ѡ���������й���Ϣ
	bool bSucc = m_treeFormularCtrl.GetCurrentItemInfo (nKindRoot,nKindItem,m_pIndex);	
	ASSERT(nKindRoot<0 || nKindRoot>=-4);
	m_nKindRoot = nKindRoot;
	if(m_pIndex == NULL ||nKindItem == 0 )
	{
		m_conDelete.EnableWindow (FALSE);
		m_conModify.EnableWindow (FALSE);
	}
	else
	{
		m_conDelete.EnableWindow (TRUE);
		if(m_pIndex->sellStr != "")
			m_conModify.EnableWindow (FALSE);
		else
			m_conModify.EnableWindow (TRUE);
	}
	
	*pResult = 0;

	if(m_pIndex ==NULL)
	{
		m_editexplain = _T("");
		m_tabedit = _T("");
	}
	else
	{
		m_editexplain=m_pIndex->explainBrief;
		{
//			if(m_pIndex->isProtected)
//				m_tabedit="�˹�ʽ�����뱣��\r\n"+m_pIndex->help;
//			else
				m_tabedit=m_pIndex->help;
		}
	}
	UpdateData(FALSE);

}

BOOL CTaiKlineIndicatorsManage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_treeFormularCtrl.m_hWnd == pMsg->hwnd)
		if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==13)
		{
			OnModifyFormula();
			return TRUE;
		}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CTaiKlineIndicatorsManage::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	OnModifyFormula();
	*pResult = 0;
}

BOOL CTaiKlineIndicatorsManage::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,CTaiKlineIndicatorsManage::IDD);
//	return CDialog::OnHelpInfo(pHelpInfo);
	DoHtmlHelp(this);return TRUE;
}

void CTaiKlineIndicatorsManage::OnSelchangeTabSelect(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int n = ((CTabCtrl*)GetDlgItem(IDC_TAB_SELECT))->GetCurSel();
	if(n>=0)
		this->m_treeFormularCtrl.ReInsertIterm2(n);
	*pResult = 0;
}

void CTaiKlineIndicatorsManage::OnClose() 
{
	OnOK();
}
