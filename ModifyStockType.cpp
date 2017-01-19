// ModifyStockType.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "mainfrm.h"
#include "SelectStock.h"
#include "keybrodeangel.h"
#include "ModifyStockType.h"
#include "CTaiShanReportView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModifyStockType dialog

CModifyStockType::CModifyStockType(PBLOCKINFO &pBlockInfo,BOOL IsAddStockType,CWnd* pParent )   // standard constructor
	: CDialog(CModifyStockType::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModifyStockType)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pBlockInfo=pBlockInfo;
	m_bIsAddNewStockType=IsAddStockType;
    m_bIsChanged=FALSE;
}


CModifyStockType::CModifyStockType(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyStockType::IDD, pParent)
{
}


void CModifyStockType::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifyStockType)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_REFRESH, m_refresh);
	DDX_Control(pDX, IDC_DELETE, m_delete);
	DDX_Control(pDX, IDC_ADD, m_add);
	DDX_Control(pDX, IDC_LIST2, m_lstStockTypeName);
	DDX_Text(pDX, IDC_CODE, m_sStockTypeCode);
	DDX_Text(pDX, IDC_NAME, m_sStockTypeName);
	DDV_MaxChars(pDX, m_sStockTypeName, 8);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Radio(pDX, IDC_RADIO1,m_nRightType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModifyStockType, CDialog)
	//{{AFX_MSG_MAP(CModifyStockType)
	ON_BN_CLICKED(IDC_ADD, OnAddStock)
	ON_BN_CLICKED(IDC_DELETE, OnDeleteStock)
	ON_BN_CLICKED(IDC_RADIO1, OnChangeStockTypeRight)
	ON_BN_CLICKED(IDC_RADIO2, OnChangeStockTypeRight)
	ON_BN_CLICKED(IDC_RADIO3, OnChangeStockTypeRight)
	ON_BN_CLICKED(IDOK, OnUpdateOk)
	ON_BN_CLICKED(IDC_REFRESH, OnRefreshHistoryData)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_ADDSTOCK,OnAddStockCode)
	ON_EN_CHANGE(IDC_NAME, OnChangedName)

	ON_BN_CLICKED(IDCANCEL, &CModifyStockType::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_DELALL, &CModifyStockType::OnBnClickedDelall)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModifyStockType message handlers

BOOL CModifyStockType::OnInitDialog() 
{
	CDialog::OnInitDialog();
	this->CenterWindow();
	
	// TODO: Add extra initialization here
	m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_pDoc ;

	m_pImageList = new CImageList();
	m_pImageListSmall = new CImageList();
	ASSERT(m_pImageList != NULL && m_pImageListSmall != NULL);    // serious allocation failure checking
	m_pImageList->Create(IDB_BITMAP14,32,0,RGB(192,192,192));
	m_pImageListSmall->Create(IDB_BITMAP15,16,0,RGB(192,192,192));
	FillToListCtrlHead();
	if(m_bIsAddNewStockType)
	{
       m_sStockTypeCode=m_pBlockInfo->m_szCode;
	   m_nRightType=1;
	   m_fTotalRight=0;
	   m_nTotalCount=0;
	}
	else
	{
       m_sStockTypeCode=m_pBlockInfo->m_szCode;
       m_sStockTypeName=m_pBlockInfo->m_szName;
	   m_nRightType=m_pBlockInfo->m_iRightType;
	   m_fTotalRight=m_pDoc->m_ManagerBlockData.GetStockBlockRight(m_pBlockInfo->m_szCode);
	   m_nTotalCount=m_pBlockInfo->m_lStockCount;
       SymbolKindArr m_StockCodeArray;
	   if(m_pDoc->m_ManagerBlockData.GetStockFromBlockCode(m_StockCodeArray,m_sStockTypeCode.GetBuffer(0)))          
	   {
			int DisplayCount=m_StockCodeArray.GetSize();

			for(int row=0 ;row <  (int)DisplayCount  ;row++)           //����һ������
			{    
				 CReportData *Cdat;
				 SymbolKind m_SymbolKind; 
				 m_SymbolKind=m_StockCodeArray.GetAt(row) ;
	 			 if (m_pDoc->m_sharesInformation.Lookup(m_SymbolKind.m_chSymbol , Cdat,m_SymbolKind.m_nSymbolKind) == TRUE)     //���ù�Ʊ�Ƿ��Ѽ�¼���ڴ�ģ����
				 {
					 if(Cdat == NULL || Cdat->pBaseInfo==NULL)
						 continue;
                     FillToListCtrlItem(Cdat);
				 }
			}
	   }
	}
	this->UpdateData(FALSE);
    CWnd *pWnd=this->GetDlgItem(IDC_NAME);
	pWnd->SetFocus();

    m_lstStockTypeName.SetFocus();
    m_lstStockTypeName.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;

	m_pImageList->Detach();
	delete m_pImageList;
	m_pImageListSmall->Detach();
	delete m_pImageListSmall;

	return TRUE;  // return TRUE unless you set the focus to a control
              // EXCEPTION: OCX Property Pages should return FALSE
}
void CModifyStockType::FillToListCtrlHead()
{
   	CRect           rect;
	CString         strItem1= _T("��Ʊ����");
	CString         strItem2= _T("��Ʊ����");
	CString         strItem3= _T("�ܹɱ�");
	CString         strItem4= _T("��ͨ�ɱ�");
	CString         strItem5= _T("Ȩ�ر���(%)");

	m_lstStockTypeName.SetExtendedStyle(m_lstStockTypeName.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
    
	this->m_lstStockTypeName.SetImageList(m_pImageList, LVSIL_NORMAL);
	this->m_lstStockTypeName.SetImageList(m_pImageListSmall, LVSIL_SMALL);

	// insert two columns (REPORT mode) and modify the new header items
	this->m_lstStockTypeName.GetWindowRect(&rect);
	this->m_lstStockTypeName.InsertColumn(0, strItem1, LVCFMT_LEFT,
		rect.Width() * 1/5, 0);
	this->m_lstStockTypeName.InsertColumn(1, strItem2, LVCFMT_LEFT,
		rect.Width() * 1/5, 1);
	this->m_lstStockTypeName.InsertColumn(2, strItem3, LVCFMT_LEFT,
		rect.Width() * 1/5, 2);
	this->m_lstStockTypeName.InsertColumn(3, strItem4, LVCFMT_LEFT,
		rect.Width() * 1/5, 3);
	this->m_lstStockTypeName.InsertColumn(4, strItem5, LVCFMT_LEFT,
		rect.Width() * 1/5, 3);

}
void CModifyStockType::FillToListCtrlItem(CReportData *pCdat)
{
	int  iIcon, iItem, iSubItem, iActualItem;
	LV_ITEM  lvitem;
	LVFINDINFO  lvf;
	lvf.flags=LVFI_STRING;
	lvf.psz=pCdat->id;
    if(m_lstStockTypeName.FindItem(&lvf)>=0)
		return;
    iItem=this->m_lstStockTypeName.GetItemCount();
	CString strIconDesc[5];
    strIconDesc[0]=pCdat->id;
    strIconDesc[1]=pCdat->name;
    strIconDesc[2].Format("%6.2f",pCdat->pBaseInfo->zgb);

	if(CTaiShanReportView::IsBg( pCdat->kind))
	{
        strIconDesc[3].Format("%6.2f",pCdat->pBaseInfo->Bg);
	}
	else
    {
        strIconDesc[3].Format("%6.2f",pCdat->pBaseInfo->ltAg);
	}
	if(m_fTotalRight!=0)
	{
	    switch(this->m_nRightType)
		{
			case 0:
					strIconDesc[4].Format("%8.3f",pCdat->pBaseInfo->zgb/m_fTotalRight);
					break;
			case 1:
					if(CTaiShanReportView::IsBg( pCdat->kind))
					{
						strIconDesc[4].Format("%8.3f",pCdat->pBaseInfo->Bg/m_fTotalRight);
					}
					else
					{
						strIconDesc[4].Format("%8.3f",pCdat->pBaseInfo->ltAg/m_fTotalRight);
					}
					break;
			case 2:
					strIconDesc[4].Format("%8.3f",(float)1/m_fTotalRight);
					break;
		}
	}
	for (iSubItem = 0; iSubItem <5; iSubItem++)
	{
		if (iSubItem == 0)
		{
			iIcon = 0;  // choose the icon and legend for the entry
		}
		lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE : 0);
		lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
		lvitem.iSubItem = iSubItem;

		// calculate the main and sub-item strings for the current item
       
		lvitem.pszText=strIconDesc[iSubItem].GetBuffer(strIconDesc[iSubItem].GetLength()); 
		lvitem.iImage = iIcon;
		if (iSubItem == 0)
			iActualItem =this->m_lstStockTypeName.InsertItem(&lvitem); // insert new item
		else
			this->m_lstStockTypeName.SetItem(&lvitem); // modify existing item (the sub-item text)
		m_lstStockTypeName.SetItemData(iItem,pCdat->kind);
	} 
	
}
void CModifyStockType::RefreshShowData()
{
	this->UpdateData();
	m_fTotalRight=0;
	m_nTotalCount=0;

	int i;
	int tmp=m_lstStockTypeName.GetItemCount();
	for(i=0;i<tmp;i++)
	{
	   char temp[20];
       switch(this->m_nRightType)
	   {
	     case ZGB:    m_lstStockTypeName.GetItemText( i, 2, temp, 20);
			          m_fTotalRight+=atof(temp);
				      m_nTotalCount++;
				      break;
	     case LTG:    m_lstStockTypeName.GetItemText( i, 3, temp, 20);
			          m_fTotalRight+=atof(temp);
				      m_nTotalCount++;
				      break; 
	     case OTHER:  m_fTotalRight+=1;
				      m_nTotalCount++;
				      break;
	  }
	}
	for(i=0;i<tmp;i++)
	{
	   char temp[20];
	   float l_fRight;
	   CString l_sRight;
       switch(this->m_nRightType)
	   {
	     case ZGB:    m_lstStockTypeName.GetItemText( i, 2, temp, 20);
			          l_fRight=atof(temp)/m_fTotalRight;
					  l_sRight.Format("%8.3f",l_fRight);
                      m_lstStockTypeName.SetItemText( i, 4 , l_sRight );
				      break;
	     case LTG:    m_lstStockTypeName.GetItemText( i, 3, temp, 20);
			          l_fRight=atof(temp)/m_fTotalRight;
					  l_sRight.Format("%8.3f",l_fRight);
                      m_lstStockTypeName.SetItemText( i, 4 , l_sRight );
				      break; 
	     case OTHER:  l_fRight=(float)1/m_nTotalCount;
					  l_sRight.Format("%8.3f",l_fRight);
                      m_lstStockTypeName.SetItemText( i, 4 , l_sRight );
				      break;
	  }
	}
	m_bIsChanged=TRUE;
}
void CModifyStockType::OnAddStock()
{
	SelectStock pDlg(this,TRUE);
	pDlg.DoModal();
}
void CModifyStockType::OnDeleteStock()
{
    CString Id;
	int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;
	if(temp==-1)
	{
		AfxMessageBox("��ѡ��Ҫɾ����Ʊ!");
		return;
	}
    int rtn = MessageBox("�Ƿ�ȷʵ������һɾ������?","����",MB_YESNO|MB_ICONWARNING);
    if(rtn!=6)
	  return;

	while(temp>=0)
	{
		char l_sStockCode[10];
		CString l_strCode;
        m_lstStockTypeName.GetItemText( temp, 0, l_sStockCode, 10);
        l_strCode=l_sStockCode;
		m_lstStockTypeName.DeleteItem(temp);
        temp=m_lstStockTypeName.GetNextItem( -1,LVNI_SELECTED   ) ;
	}

   RefreshShowData(); 
   m_lstStockTypeName.SetFocus();
   m_lstStockTypeName.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;
}
void CModifyStockType::OnChangeStockTypeRight()
{
   RefreshShowData(); 
}
BOOL CModifyStockType::PreTranslateMessage(MSG* pMsg)
{
    CWnd *pWnd=this->GetFocus();
	CWnd *pChildWnd=this->GetDlgItem(IDC_NAME);
	if(pWnd!=pChildWnd)
	if(pMsg->message==WM_CHAR&&((pMsg->wParam>='0'&&pMsg->wParam<='9')||(pMsg->wParam>='A'&&pMsg->wParam<='z')))
	{
		    CTaiKeyBoardAngelDlg m_diakeybrode;
			if(pMsg->wParam>='a'&&pMsg->wParam<='z')
				pMsg->wParam-=32;
			char input=pMsg->wParam;
			m_diakeybrode.input=input;
			if(pMsg->wParam>='0'&&pMsg->wParam<='9')
				m_diakeybrode.ischar=false;
			else
				m_diakeybrode.ischar=true;
		m_diakeybrode.DoModal();
		if(m_diakeybrode.isresultok)
		{
		   char result[10];                //���صĽ��
		   int nKind=m_diakeybrode.m_stkKind;
           strcpy(result,m_diakeybrode.result);
		   
		   if(strlen(result)==6||strlen(result)==4)
		   {
			  LVFINDINFO  lvf;
			  lvf.flags=LVFI_STRING;
			  lvf.psz=result;
			  if(m_lstStockTypeName.FindItem(&lvf)>=0)
			 	 return  TRUE;
              
			  CReportData *Cdat;
	 		  if (m_pDoc->m_sharesInformation.Lookup(result , Cdat,nKind) == TRUE)      //���ù�Ʊ�Ƿ��Ѽ�¼���ڴ�ģ����
			  {
				   if(Cdat->pBaseInfo!=NULL)
                     FillToListCtrlItem(Cdat);
			  }
		   }
		}
        RefreshShowData(); 
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
LRESULT CModifyStockType::OnAddStockCode(WPARAM wParam, LPARAM lParam) //���ӹ�Ʊ��LISTCTRL
{
	SHARES_DATA_ADD *pAddCode;
	int lsCount=(int)lParam;
    pAddCode=(SHARES_DATA_ADD *)wParam;
	for(int i=0;i<lsCount;i++)
	{
		if(pAddCode[i].StockType<0)
		{
			int nKind=-pAddCode[i].StockType-1;
		    LVFINDINFO  lvf;
		    lvf.flags=LVFI_STRING;
		    lvf.psz=pAddCode[i].name;
			int iItem=m_lstStockTypeName.FindItem(&lvf);
		    if(iItem >=0)
			{
				if(nKind==m_lstStockTypeName.GetItemData(iItem))
			       continue;
			}
		    CReportData *pCdat;
	 	    if (m_pDoc->m_sharesInformation.Lookup(pAddCode[i].name, pCdat,nKind) == TRUE)     //���ù�Ʊ�Ƿ��Ѽ�¼���ڴ�ģ����
			{
				if(CSharesInformation::IsZhiShu(pCdat->kind))
				{
					MessageBox("ָ���ཫ�������ӵ������!","���ڲ�����ʾ",MB_OK);
					continue;
				}
				if(pCdat->pBaseInfo==NULL||pCdat->pBaseInfo->zgb<=0)
				{
					MessageBox("�ڽ���Ʊ������ʱ,�ù�Ʊ�������ݲ���Ϊ��!","���ڲ�����ʾ",MB_OK);
					continue;
				}
               if(pCdat->pBaseInfo!=0) 
                 FillToListCtrlItem(pCdat);
			}
		}
	}
    RefreshShowData(); 
	return 1L;
}

void CModifyStockType::OnUpdateOk()
{
	BLOCKINFO *l_pStockType;
	this->UpdateData(TRUE);
	if(!m_bIsChanged)
	{
      CDialog::OnOK();
	  return;
	}
	if(m_sStockTypeName.GetLength()<=0 || m_sStockTypeName.GetLength() > 11)//||m_lstStockTypeName.GetItemCount()<1)
	{
		AfxMessageBox("��ѡ����ȷ�������!");
		return ;
	}
	CWnd *pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(FALSE);

	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 

    BeginWaitCursor();
	CheckStockCount();
	if(m_bIsAddNewStockType)
	{
		if(m_pDoc->m_ManagerBlockData.CheckBlockName(m_sStockTypeName.GetBuffer(0),NULL))
		{
			AfxMessageBox("��ѡ����ȷ�������! ������Ʋ����ظ�");
			pWnd->EnableWindow(TRUE);
			return ;
		}
		int l_nStockTypePos=m_pDoc->m_ManagerBlockData.GetInsertBlockPos();
		char temp[10];
		if(!m_pDoc->m_ManagerBlockData.InsertBlock(l_pStockType,l_nStockTypePos))
		{
            EndWaitCursor();
			return;
		}
		l_pStockType->m_lIsUse=TRUE;
		l_pStockType->m_bIsDelete =FALSE;

		strcpy(l_pStockType->m_szName,m_sStockTypeName.GetBuffer(0));
		strcpy(l_pStockType->m_szBlockTypeName,m_pBlockInfo->m_szBlockTypeName);
		l_pStockType->m_lStockCount=0;
		l_pStockType->m_fTotalRight=0;
		sprintf(temp,"%3d",l_nStockTypePos);
		int i;
		for(i=0;i<3;i++)
		{
			if(temp[i]==' ')temp[i]='0';
		}
		sprintf(l_pStockType->m_szCode,"%s%s","8K0",temp);
		l_pStockType->m_iRightType=this->m_nRightType;
		int tmp=m_lstStockTypeName.GetItemCount();
		for(i=0;i<tmp;i++)
		{
            char l_szStockCode[10];
			CReportData *l_pCdat;
			int nKind=m_lstStockTypeName.GetItemData(i);
			m_lstStockTypeName.GetItemText( i, 0, l_szStockCode , 20);
            if(m_pDoc->m_sharesInformation.Lookup(l_szStockCode,l_pCdat,nKind))
			{
			   if(l_pCdat->pBaseInfo!=NULL)
			      m_pDoc->m_ManagerBlockData.InsertStockToBlock(l_pCdat,l_pStockType->m_szCode);
			}
		}
		CReportData *pCdat;
		m_pDoc->m_sharesInformation.InsertItem(l_pStockType->m_szCode,pCdat,BLOCKINDEX);
		strcpy(pCdat->name ,l_pStockType->m_szName );
		strcpy(pCdat->id ,l_pStockType->m_szCode );
		pCdat->kind=BLOCKINDEX;
        m_pDoc->m_sharesInformation.SaveRealDataToFile(pCdat,sizeof(CReportData));
		if(strlen(pCdat->name)>0)
		   m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;
		m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
	}
    else
	{
		char BlockCode[MAX_PATH];
		strcpy(BlockCode,m_sStockTypeCode.GetBuffer(0));
		if(m_pDoc->m_ManagerBlockData.CheckBlockName(m_sStockTypeName.GetBuffer(0),BlockCode))
		{
			AfxMessageBox("��ѡ����ȷ�������! ������Ʋ����ظ�");
			pWnd->EnableWindow(TRUE);
			return ;
		}
		m_pDoc->m_ManagerBlockData.DeleteBlock(m_sStockTypeCode.GetBuffer(0));
		int l_nStockTypePos=m_pDoc->m_ManagerBlockData.GetInsertBlockPos();
		char temp[10];
		if(!m_pDoc->m_ManagerBlockData.InsertBlock(l_pStockType,l_nStockTypePos))
		{
            EndWaitCursor();
			return;
		}
		l_pStockType->m_lIsUse=TRUE;
		l_pStockType->m_bIsDelete =FALSE;
		strcpy(l_pStockType->m_szName,m_sStockTypeName.GetBuffer(0));
		l_pStockType->m_lStockCount=0;
		l_pStockType->m_fTotalRight=0;
		sprintf(temp,"%3d",l_nStockTypePos);
		int i;
		for(i=0;i<3;i++)
		{
			if(temp[i]==' ')temp[i]='0';
		}
		sprintf(l_pStockType->m_szCode,"%s%s","8K0",temp);
		l_pStockType->m_iRightType=this->m_nRightType;
		int tmp=m_lstStockTypeName.GetItemCount();
		for(i=0;i<tmp;i++)
		{
            char l_szStockCode[10];
			CReportData *l_pCdat;
			int nKind=m_lstStockTypeName.GetItemData(i);
			m_lstStockTypeName.GetItemText( i, 0, l_szStockCode , 20);
            if(m_pDoc->m_sharesInformation.Lookup(l_szStockCode,l_pCdat,nKind))
			{
			   if(l_pCdat->pBaseInfo!=NULL)
			      m_pDoc->m_ManagerBlockData.InsertStockToBlock(l_pCdat,l_pStockType->m_szCode);
			}
		}
		CReportData *pCdat;
		m_pDoc->m_sharesInformation.InsertItem(l_pStockType->m_szCode,pCdat,BLOCKINDEX);
		strcpy(pCdat->name ,l_pStockType->m_szName );
		strcpy(pCdat->id ,l_pStockType->m_szCode );
		pCdat->kind=BLOCKINDEX;
		if(strlen(pCdat->name)>0)
		   m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;
		m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
	}
    m_pDoc->m_ManagerBlockData.RefreshHistoryDayLineData(m_sStockTypeCode.GetBuffer(0),	&m_Progress);
    EndWaitCursor();
	CDialog::OnOK();
}
void CModifyStockType::OnRefreshHistoryData()
{
	CWnd *pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(FALSE);

	BLOCKINFO *l_pStockType;
	this->UpdateData(TRUE);
    BeginWaitCursor();
	if(!m_bIsChanged)
	{
      m_pDoc->m_ManagerBlockData.RefreshHistoryDayLineData(m_sStockTypeCode.GetBuffer(0),&m_Progress);
	  pWnd->EnableWindow(TRUE);
      EndWaitCursor();
	  return;
	}
	else
	{
		int rtn = MessageBox("�ð�������ѷ����仯,�Ƿ��ڱ��������ݺ��������ˢ�²���!","����",MB_YESNO|MB_ICONWARNING);
		if(rtn!=6)
		{
            m_pDoc->m_ManagerBlockData.RefreshHistoryDayLineData(m_sStockTypeCode.GetBuffer(0),&m_Progress);
	        pWnd->EnableWindow(TRUE);
            EndWaitCursor();
			return;
		}
	}
	if(this->m_sStockTypeName.GetLength()<=0||m_lstStockTypeName.GetItemCount()<1)
	{
		AfxMessageBox("��ѡ����ȷ�������!");
	    pWnd->EnableWindow(TRUE);
        EndWaitCursor();
		return ;
	}
    CheckStockCount();
	if(m_bIsAddNewStockType)
	{
		int l_nStockTypePos=m_pDoc->m_ManagerBlockData.GetInsertBlockPos();
		char temp[10];
		if(!m_pDoc->m_ManagerBlockData.InsertBlock(l_pStockType,l_nStockTypePos))
		{
	        pWnd->EnableWindow(TRUE);
            EndWaitCursor();
			return;
		}
		l_pStockType->m_lIsUse=TRUE;
		l_pStockType->m_bIsDelete =FALSE;
		strcpy(l_pStockType->m_szName,m_sStockTypeName.GetBuffer(0));
		l_pStockType->m_lStockCount=0;
		l_pStockType->m_fTotalRight=0;
		sprintf(temp,"%3d",l_nStockTypePos);
		int i;
		for(i=0;i<3;i++)
		{
			if(temp[i]==' ')temp[i]='0';
		}
		sprintf(l_pStockType->m_szCode,"%s%s","8K0",temp);
		l_pStockType->m_iRightType=this->m_nRightType;
            
		int tmp=m_lstStockTypeName.GetItemCount();
		for(i=0;i<tmp;i++)
		{
            char l_szStockCode[10];
			int nKind=m_lstStockTypeName.GetItemData(i);
			CReportData *l_pCdat;
			m_lstStockTypeName.GetItemText( i, 0, l_szStockCode , 20);
            if(m_pDoc->m_sharesInformation.Lookup(l_szStockCode,l_pCdat,nKind))
			{
			   if(l_pCdat->pBaseInfo!=NULL)
			      m_pDoc->m_ManagerBlockData.InsertStockToBlock(l_pCdat,l_pStockType->m_szCode);
			}
		}
		CReportData *pCdat;
		m_pDoc->m_sharesInformation.InsertItem(l_pStockType->m_szCode,pCdat,BLOCKINDEX);
		strcpy(pCdat->name ,l_pStockType->m_szName );
		strcpy(pCdat->id ,l_pStockType->m_szCode );
		pCdat->kind=BLOCKINDEX;
		if(strlen(pCdat->name)>0)
		   m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;
		m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
	}
    else
	{
		m_pDoc->m_ManagerBlockData.DeleteBlock(m_sStockTypeCode.GetBuffer(0));
		int l_nStockTypePos=m_pDoc->m_ManagerBlockData.GetInsertBlockPos();
		char temp[10];
		if(!m_pDoc->m_ManagerBlockData.InsertBlock(l_pStockType,l_nStockTypePos))
		{
	        pWnd->EnableWindow(TRUE);
            EndWaitCursor();
			return;
		}
		l_pStockType->m_lIsUse=TRUE;
		l_pStockType->m_bIsDelete =FALSE;
		strcpy(l_pStockType->m_szName,m_sStockTypeName.GetBuffer(0));
		l_pStockType->m_lStockCount=0;
		l_pStockType->m_fTotalRight=0;
		sprintf(temp,"%3d",l_nStockTypePos);
		int i;
		for(i=0;i<2;i++)
		{
			if(temp[i]==' ')temp[i]='0';
		}
		sprintf(l_pStockType->m_szCode,"%s%s","8K0",temp);
		l_pStockType->m_iRightType=this->m_nRightType;
            
		int tmp=m_lstStockTypeName.GetItemCount();
		for(i=0;i<tmp;i++)
		{
            char l_szStockCode[10];
			int nKind=m_lstStockTypeName.GetItemData(i);
			CReportData *l_pCdat;
			m_lstStockTypeName.GetItemText( i, 0, l_szStockCode , 20);
            if(m_pDoc->m_sharesInformation.Lookup(l_szStockCode,l_pCdat,nKind))
			{
			   if(l_pCdat->pBaseInfo!=NULL)
			      m_pDoc->m_ManagerBlockData.InsertStockToBlock(l_pCdat,l_pStockType->m_szCode);
			}
		}
		CReportData *pCdat;
		m_pDoc->m_sharesInformation.InsertItem(l_pStockType->m_szCode,pCdat,BLOCKINDEX);
		strcpy(pCdat->name ,l_pStockType->m_szName );
		strcpy(pCdat->id ,l_pStockType->m_szCode );
		pCdat->kind=BLOCKINDEX;
		if(strlen(pCdat->name)>0)
		   m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;
		m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
	}
    m_pDoc->m_ManagerBlockData.RefreshHistoryDayLineData(m_sStockTypeCode.GetBuffer(0),	&m_Progress);
	m_bIsChanged=FALSE;
    m_bIsAddNewStockType=FALSE;
	pWnd->EnableWindow(TRUE);
    EndWaitCursor();
}
void CModifyStockType::CheckStockCount()
{
	int tmp=m_lstStockTypeName.GetItemCount();
	int index=0;
	for(int i=0;i<tmp;i++)
	{
        char l_szStockCode[10];
		PSTOCKINFOINBLOCK pStock;
		m_lstStockTypeName.GetItemText( i, 0, l_szStockCode , 20);
		int nKind=m_lstStockTypeName.GetItemData(i);
        if(!m_pDoc->m_ManagerBlockData.Lookup(l_szStockCode,nKind,pStock))
		{
		   index++;
		}
	}
	m_pDoc->m_ManagerBlockData.CheckStockCount(index);
}
void CModifyStockType::OnChangedName()
{
	m_bIsChanged=TRUE;

}

void CModifyStockType::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CModifyStockType::OnBnClickedDelall()
{
	int rtn = MessageBox("�Ƿ�ȷʵ������һɾ������?","����",MB_YESNO|MB_ICONWARNING);
	if(rtn!=6)
		return;

	while(m_lstStockTypeName.GetItemCount())
	{
		m_lstStockTypeName.DeleteItem(0);
	}

	RefreshShowData(); 
	m_lstStockTypeName.SetFocus();
}
