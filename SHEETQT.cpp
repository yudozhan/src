// SHEETQT.cpp : implementation file
//

#include "stdafx.h"
#include "SHEETQT.h"

#include "CTaiShanDoc.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiSysSetOthersPage property page

IMPLEMENT_DYNCREATE(CTaiSysSetOthersPage, CPropertyPage)

CTaiSysSetOthersPage::CTaiSysSetOthersPage() : CPropertyPage(CTaiSysSetOthersPage::IDD)
{
	//{{AFX_DATA_INIT(CTaiSysSetOthersPage)
	m_autoclose = FALSE;
	m_autominute = FALSE;
	m_maingraph = -1;
	m_autoday = FALSE;
	m_fourgraph = -1;
	m_kline = -1;
	m_AlarmSound=FALSE;
	m_AutoOrganizeData=FALSE;
	m_bAutoSaveParamOfJishu = FALSE;
	m_bHsTodayAddLast = FALSE;
	m_bDoPowerSelectingStock = FALSE;
	m_bShowInfoCrossing = FALSE;
	m_bStartAlertOnBegin = FALSE;
	m_bDoVolPowering = FALSE;
	//}}AFX_DATA_INIT
	
	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();
    
//	m_buyrate=pDoc->m_systemdata.buyrate;                     //ȱʡ������׷���Ϊ7.5
	m_autoclose=pDoc->m_systemOption.autoclose;                  //ȱʡ���̺�20�����Զ�������ҵ
	m_autominute=pDoc->m_systemOption.autominute;                 //ȱʡ�Զ�����ʱ��
	m_autoday=pDoc->m_systemOption.autoday;                    //ȱʡ�Զ���������
//	m_changepage=pDoc->m_systemdata.changepage;                    //ȱʡ�Զ���ҳʱ��Ϊ8��
//	m_AdvSpTimes=pDoc->m_systemdata.AdvSpTimes;                  //ȱʡ�ǵ��Ƚ�ʱ��Ϊ10��
//	m_fiveminutesave=pDoc->m_systemdata.fiveminutesave;               //ȱʡ5�����ߴ洢20��
    m_showxline=pDoc->m_systemOption.showxline;                  //ȱʡ����ʱ������һ������
	m_showyline=pDoc->m_systemOption.showyline;
//	m_noticesave=pDoc->m_systemdata.noticesave;                   //ȱʡ������Ϣ�洢10��
//	m_AlarmPercent=pDoc->m_systemdata.AlarmPercent;                    //ȱʡ�ǵ�ͣ��Ϊ10%
	m_AlarmSound=pDoc->m_systemOption.alarmsound;
	m_AutoOrganizeData=pDoc->m_systemOption.organizedata;
	index=0;                           //ȱʡ�м۷�ʽΪ��+��+��+��)/4
	if(pDoc->m_systemOption.kline)
		m_kline=0;
	else m_kline=1;        //ȱʡ��KLINE
	if(pDoc->m_systemOption.fourgraph)
		m_fourgraph=0;
	else m_fourgraph=1;          //ȱʡ��4ͼ
}

CTaiSysSetOthersPage::~CTaiSysSetOthersPage()
{
}

void CTaiSysSetOthersPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiSysSetOthersPage)
	DDX_Control(pDX, IDC_LISTSELECT, m_listselect);
	DDX_Control(pDX, IDC_LISTALL, m_listall);
	DDX_Check(pDX, IDC_AUTOCLOSE, m_autoclose);
	DDX_Check(pDX, IDC_AUTOMINUTE, m_autominute);
	DDX_Check(pDX, IDC_SHOWXLINE,m_showxline);
	DDX_Check(pDX, IDC_SHOWYLINE,m_showyline);
	DDX_Check(pDX, IDC_AUTODAY, m_autoday);
	DDX_Radio(pDX, IDC_RADIO4T, m_fourgraph);
	DDX_Radio(pDX, IDC_RADIO3, m_kline);
	DDX_Check(pDX,IDC_ALARM_SOUND,m_AlarmSound);
	DDX_Check(pDX,IDC_AUTO_ORGANIZEDATA,m_AutoOrganizeData);
	DDX_Check(pDX, IDC_SYSSET_AUTOSAVEZHIBIAO, m_bAutoSaveParamOfJishu);
	DDX_Check(pDX, IDC_SYSSET_HSTODAYADDLAST, m_bHsTodayAddLast);
	DDX_Check(pDX, IDC_SYSSET_POWERSELECTSTOCK, m_bDoPowerSelectingStock);
	DDX_Check(pDX, IDC_SYSSET_SHOWINFOCROSS, m_bShowInfoCrossing);
	DDX_Check(pDX, IDC_SYSSET_STARTALERT, m_bStartAlertOnBegin);
	DDX_Check(pDX, IDC_SYSSET_VOLPOWER, m_bDoVolPowering);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiSysSetOthersPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTaiSysSetOthersPage)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTALL, OnDblclkListall)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTSELECT, OnDblclkListselect)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO4T, OnRadio4t)
	ON_BN_CLICKED(IDC_RADIO9T, OnRadio9t)
	ON_NOTIFY(NM_SETFOCUS, IDC_LISTALL, OnSetfocusListall)
	ON_NOTIFY(NM_SETFOCUS, IDC_LISTSELECT, OnSetfocusListselect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiSysSetOthersPage message handlers

BOOL CTaiSysSetOthersPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();
    
	CheckRadioButton(IDC_RADIO3,IDC_RADIO4,IDC_RADIO3);//ȱʡK��
	CheckRadioButton(IDC_RADIO4T,IDC_RADIO9T,IDC_RADIO9T);//ȱʡ4ͼ
	UpdateData(FALSE);
	const int combqtlength=5;
    CString  combstring[combqtlength]={"(��+��+��+��)/4","(��+��+��)/3",
		                                   "(��+��+2��)/4","(��+��)/2","���̼�"};
//	for (int i=0;i<combqtlength;i++)
//		m_comcor.AddString((LPCTSTR)combstring[i]);
	int iComItem=pDoc->m_systemdata.middleprice;
//	m_comcor.SetCurSel(iComItem);      //���ÿɲ�����Ͽ���Ϊ��һ��

	m_listall.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	zbtotalnumber=pDoc->m_formuar_index.GetSize();
	m_listselect.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	int j=0;
	for(int i=0;i<zbtotalnumber;i++)
	{
		m_listall.InsertItem(i,(LPCSTR)pDoc->m_formuar_index.GetAt(i)->name,4);
		if(pDoc->m_formuar_index.GetAt(i)->isOfen)
			m_listselect.InsertItem(j++,(LPCSTR)pDoc->m_formuar_index.GetAt(i)->name,4);
		
	}
	GetDlgItem(IDC_ADD)->SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaiSysSetOthersPage::OnAdd() 
{
	// TODO: Add your control notification handler code here
    CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();
	
    POSITION  position=m_listall.GetFirstSelectedItemPosition();
	if(position==NULL)
	{
		AfxMessageBox("��ѡ��һ��ָ��",MB_OK|MB_ICONASTERISK );
			return;
	}
	int selectedcount=m_listall.GetSelectedCount( );
	int item;
	for(int i=0;i<selectedcount;i++)
	{
		item=m_listall.GetNextSelectedItem(position);
		CFormularContent  *toadd;
		
		toadd=pDoc->m_formuar_index.GetAt(item);

		if(toadd->isOfen)
		{
			CString str=toadd->name+"�Ѿ��ǳ���ָ�꣬������ѡ��";
			AfxMessageBox((LPCTSTR)str,MB_OK|MB_ICONASTERISK );
		}
		else
			toadd->isOfen=true;
	}
	m_listselect.DeleteAllItems();
	UpdateImageList();		
}

void CTaiSysSetOthersPage::OnDel() 
{
	// TODO: Add your control notification handler code here
	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();
	
	POSITION  position=m_listselect.GetFirstSelectedItemPosition();
	int total=pDoc->m_formuar_index.GetSize();
	if(position==NULL)
	{
		AfxMessageBox("��ѡ��һ��ָ��",MB_OK|MB_ICONASTERISK );
			return;
	}
	int selectedcount=m_listselect.GetSelectedCount();
	int item;
	int i,j;
	int *array=new int[selectedcount];
	for(j=0;j<selectedcount;j++)
	{
		item=m_listselect.GetNextSelectedItem(position);
									  //item���û��ڳ���ָ���е�ѡ��
		for(i=0;i<total;i++)
		{
			if(pDoc->m_formuar_index.GetAt(i)->isOfen)
			item--;
			if(item==-1)           //i���û�ѡ���ָ��������ָ���е�������
				break;
		}
		array[j]=i;
		
	}
	for(j=0;j<selectedcount;j++)
	{
		pDoc->m_formuar_index.GetAt(array[j])->isOfen=false;
	}
	delete array;
	m_listselect.DeleteAllItems();
	UpdateImageList();
}

void CTaiSysSetOthersPage::UpdateImageList()
{
	CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc*	pDoc=(CTaiShanDoc*)pchildfram->GetActiveDocument();

	int total=pDoc->m_formuar_index.GetSize();
	int j=0;
	for(int i=0;i<total;i++)
	{
		if(pDoc->m_formuar_index.GetAt(i)->isOfen)
		m_listselect.InsertItem(j++,(LPCSTR)pDoc->m_formuar_index.GetAt(i)->name,4);
	}
}

void CTaiSysSetOthersPage::OnDblclkListall(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION  position=m_listall.GetFirstSelectedItemPosition();
    if(position!=NULL)
		OnAdd();
	*pResult = 0;
}

void CTaiSysSetOthersPage::OnDblclkListselect(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POSITION  position=m_listselect.GetFirstSelectedItemPosition();
	if(position!=NULL)
	OnDel();	
	*pResult = 0;
}

void CTaiSysSetOthersPage::OnRadio3() 
{
	// TODO: Add your control notification handler code here
	int itemid;
	itemid=GetCheckedRadioButton(IDC_RADIO4,IDC_RADIO3);
	if(itemid==IDC_RADIO3)
		m_kline=0;
	else
		m_kline=1;
}

void CTaiSysSetOthersPage::OnRadio4() 
{
	// TODO: Add your control notification handler code here
	int itemid;
	itemid=GetCheckedRadioButton(IDC_RADIO4,IDC_RADIO3);
	if(itemid==IDC_RADIO3)
		m_kline=1;
	else
		m_kline=0;
}

void CTaiSysSetOthersPage::OnRadio4t() 
{
	// TODO: Add your control notification handler code here
	int itemid;
	itemid=GetCheckedRadioButton(IDC_RADIO4T,IDC_RADIO9T);
	if(itemid==IDC_RADIO4T)
		m_fourgraph=0;
	else
		m_fourgraph=1;
}

void CTaiSysSetOthersPage::OnRadio9t() 
{
	// TODO: Add your control notification handler code here
	int itemid;
	itemid=GetCheckedRadioButton(IDC_RADIO4T,IDC_RADIO9T);
	if(itemid==IDC_RADIO4T)
		m_fourgraph=1;
	else
		m_fourgraph=0;
}

void CTaiSysSetOthersPage::OnSetfocusListall(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_ADD)->EnableWindow(TRUE);
	GetDlgItem(IDC_DEL)->EnableWindow(FALSE);
	*pResult = 0;
}

void CTaiSysSetOthersPage::OnSetfocusListselect(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_DEL)->EnableWindow(TRUE);
	*pResult = 0;
}
