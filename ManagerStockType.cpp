// ManagerStockType.cpp : implementation file
//

#include "stdafx.h"
#include <share.h>
#include  <io.h>
#include <direct.h>
#include "WH.h"
#include "mainfrm.h"
#include "cellRange.h"
#include "PageWnd.h"

#include "CTaiShanReportView.h"
#include "managerchoose.h"
#include "modifystocktype.h"
#include "ManagerStockType.h"
#include "BlockTypeNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSuperviseSharesBlock dialog

///////////////////////////////////////////////////////////////////////////////////////////

// �ļ�����   ManagerStockType.h 
//            ManagerStockType.cpp
//
// �����ߣ�   ������

// ����ʱ�䣺 20000825

// �������������ڶ�̬��ʾ�ư����������ѡ�ɹ�����

// ģ���ţ�004

///////////////////////////////////////////////////////////////////////////////////////////

CSuperviseSharesBlock::CSuperviseSharesBlock(CWnd* pParent /*=NULL*/)
	: CDialog(CSuperviseSharesBlock::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSuperviseSharesBlock)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSuperviseSharesBlock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSuperviseSharesBlock)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_UPDATESTOCKTYPE, m_updatestocktype);
	DDX_Control(pDX, IDC_REFRESH_DAY, m_refreshday);
	DDX_Control(pDX, IDC_REFRESH, m_refresh);
	//DDX_Control(pDX, IDC_MANAGERCHOOSE, m_managerchoose);
	DDX_Control(pDX, IDC_DELETESTOCKTYPE, m_delete);
	DDX_Control(pDX, IDC_ADDSTOCKTYPE, m_add);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_LIST1, m_lstStockTypeName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST3, m_lstBlockTypeName);
}


BEGIN_MESSAGE_MAP(CSuperviseSharesBlock, CDialog)
	//{{AFX_MSG_MAP(CSuperviseSharesBlock)
	ON_BN_CLICKED(IDC_IMPORT, OnImportStockType)
	ON_BN_CLICKED(IDC_ADDSTOCKTYPE, OnAddStockType)
	ON_BN_CLICKED(IDC_UPDATESTOCKTYPE, OnUpdateStockType)
	//ON_BN_CLICKED(IDC_MANAGERCHOOSE, OnManagerChoose)
	ON_BN_CLICKED(IDC_DELETESTOCKTYPE, OnDeletestocktype)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_BN_CLICKED(IDC_EXPORT, OnExport)
	ON_BN_CLICKED(IDC_REFRESH_DAY, OnRefreshDayLine)
	ON_WM_CLOSE()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADDBLOCKTYPE, &CSuperviseSharesBlock::OnBnClickedAddblocktype)
	ON_BN_CLICKED(IDC_DELBLOCKTYPE, &CSuperviseSharesBlock::OnBnClickedDelblocktype)
	ON_BN_CLICKED(IDC_MODSTOCKTYPE, &CSuperviseSharesBlock::OnBnClickedModstocktype)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CSuperviseSharesBlock::OnNMClickList3)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CSuperviseSharesBlock::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_REFRESH_MINUTE, &CSuperviseSharesBlock::OnBnClickedRefreshMinute)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperviseSharesBlock message handlers
//���ܣ���ʼ���Ի���
//�����������
//�����������
BOOL CSuperviseSharesBlock::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_bIsCalc=FALSE;
	this->CenterWindow();
	m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_pDoc ;
   	m_pDoc ->m_bOpenStockTypeDlg=TRUE;
	m_pImageList = new CImageList();
	m_pImageListSmall = new CImageList();
	ASSERT(m_pImageList != NULL && m_pImageListSmall != NULL);    // serious allocation failure checking
	m_pImageList->Create(IDB_BITMAP14,32,0,RGB(255,255,255));    //������ͼ���б�
	m_pImageListSmall->Create(IDB_BITMAP23,16,0,RGB(255,255,255)); //����Сͼ���б�
	//m_pImageListSmall->Create(IDB_BITMAP15,16,0,RGB(192,192,192)); //����Сͼ���б�
	FillToListCtrlHead();                                          //��ʼ��CListCtrl����

	//���������б�
    CStringArray BlockTypeNameArray;
	m_pDoc->m_ManagerBlockData.GetAllBlockTypeName(BlockTypeNameArray);//�õ�ȫ�������
	int nCount=BlockTypeNameArray.GetSize();
    for(int i=0;i<nCount;i++)
	{
		m_lstBlockTypeName.InsertItem(i,BlockTypeNameArray[i],0);
	}
	//�����
    CStringArray m_StockTypeCodeArray;
	m_pDoc->m_ManagerBlockData.GetAllBlockCodes(m_StockTypeCodeArray);//�õ�ȫ�������
	int l_nCount=m_StockTypeCodeArray.GetSize();
    for(int i=0;i<l_nCount;i++)
	{
       BLOCKINFO *l_pStktype;
       CString l_sStockType=m_StockTypeCodeArray.GetAt(i);
       m_pDoc->m_ManagerBlockData.GetBlockPoint(l_pStktype,l_sStockType.GetBuffer(0));
	   this->FillToListCtrlItem(l_pStktype);         //��������CListCtrl�ؼ���
	}

   m_lstBlockTypeName.SetFocus();
   m_lstBlockTypeName.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;

	m_pImageList->Detach();
	delete m_pImageList;
	m_pImageListSmall->Detach();
	delete m_pImageListSmall;

#ifdef WIDE_NET_VERSION
    CWnd *pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(FALSE);
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//���ܣ���ʼ��CListCtrl����
//�����������
//�����������
void CSuperviseSharesBlock::FillToListCtrlHead()
{ 
   	CRect           rect;
	CString         strItem1= _T("ָ������");
	CString         strItem2= _T("ָ������");
	CString         strItem3= _T("Ȩ������");
	CString         strItem4= _T("��Ʊ����");

	m_lstStockTypeName.SetExtendedStyle(m_lstStockTypeName.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
    
	//this->m_lstStockTypeName.SetImageList(m_pImageList, LVSIL_NORMAL);
	this->m_lstStockTypeName.SetImageList(m_pImageListSmall, LVSIL_SMALL);

	m_lstBlockTypeName.SetExtendedStyle(m_lstBlockTypeName.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	//this->m_lstBlockTypeName.SetImageList(m_pImageList, LVSIL_NORMAL);
	this->m_lstBlockTypeName.SetImageList(m_pImageListSmall, LVSIL_SMALL);

	// insert two columns (REPORT mode) and modify the new header items
	//this->m_lstStockTypeName.GetWindowRect(&rect);
	//this->m_lstStockTypeName.InsertColumn(0, strItem1, LVCFMT_LEFT,    //��������
	//	rect.Width() * 1/4, 0); 
	//this->m_lstStockTypeName.InsertColumn(1, strItem2, LVCFMT_LEFT,    //��������
	//	rect.Width() * 1/4, 1);
	//this->m_lstStockTypeName.InsertColumn(2, strItem3, LVCFMT_LEFT,    //��������
	//	rect.Width() * 1/4, 2);
	//this->m_lstStockTypeName.InsertColumn(3, strItem4, LVCFMT_LEFT,    //��������
	//	rect.Width() * 1/4, 3);
}
//���ܣ��������뵽CListCtrl��
//���������pStockType���ṹָ��
//�����������
void CSuperviseSharesBlock::FillToListCtrlItem(BLOCKINFO *pStockType)
{
	int  iIcon, iItem, iSubItem, iActualItem;
	LV_ITEM  lvitem;
	LVFINDINFO  lvf;
	lvf.flags=LVFI_STRING;
	lvf.psz=pStockType->m_szName;
	//lvf.psz=pStockType->m_szCode;
    if(m_lstStockTypeName.FindItem(&lvf)>=0)
		return;
    iItem=this->m_lstStockTypeName.GetItemCount();          //�õ�������λ��

	m_lstStockTypeName.InsertItem(iItem,pStockType->m_szName,1);
	m_lstStockTypeName.SetItemData(iItem,(DWORD_PTR)pStockType);

/*
	for (iSubItem = 0; iSubItem <4; iSubItem++)
	{
		if (iSubItem == 0)
		{
			iIcon = 0;  // choose the icon and legend for the entry
//			lvitem.lParam=nParam;
		}
		lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE : 0);
		lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
		lvitem.iSubItem = iSubItem;

		// calculate the main and sub-item strings for the current item
		CString strIconDesc[4];
        strIconDesc[0]=pStockType->m_szCode;
        strIconDesc[1]=pStockType->m_szName;

		switch(pStockType->m_iRightType)       //Ȩ������
		{
		case ZGB:     strIconDesc[2]="�ܹɱ�";
		              break;  	   
		case LTG:     strIconDesc[2]="��ͨ�ɱ�";
		              break;  	   
		case OTHER:   strIconDesc[2]="���Ȩ��";
		              break;  	   
		}
        strIconDesc[3].Format("%d",pStockType->m_lStockCount);
        
		lvitem.pszText=strIconDesc[iSubItem].GetBuffer(strIconDesc[iSubItem].GetLength()); 
		lvitem.iImage = iIcon;
		if (iSubItem == 0)
			iActualItem =this->m_lstStockTypeName.InsertItem(&lvitem); // insert new item
		else
			this->m_lstStockTypeName.SetItem(&lvitem); // modify existing item (the sub-item text)
	}
*/
}

//���ܣ��Ӱ�������ļ��е���������
//�����������
//�����������
void CSuperviseSharesBlock::OnImportStockType() 
{
	// TODO: Add your control notification handler code here

	int stocklength;
	MSG message;
	char  blank[2]={' ',' '};  
	CString filename;
	CString DefExt="*";  //�ļ���չ��.
	CString FileName="�������\\*.*";  //����������ļ����༭���е��ļ���
	CString Filter="����ļ��� ( *.* )| *.* ||";  //.�ļ���չ��������
	CFileDialog OpenFileDialog(TRUE,(LPCSTR)DefExt,
		(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR|OFN_ALLOWMULTISELECT ,
		(LPCSTR)Filter,this);
	if( OpenFileDialog.DoModal() ==IDOK )         //���ļ�ѡ��Ի���
	{
		CWnd *pWnd=this->GetDlgItem(IDC_IMPORT);
		pWnd->EnableWindow(FALSE);
        BeginWaitCursor();
//**************************************************************************************
		int index=0;
		int AddCounts=0;
        POSITION l_position=OpenFileDialog.GetStartPosition();
		while(l_position)
		{
//			int l_nPos;
			CFile stockfile;
//			CReportData *pCdat;
//			BLOCKINFO *l_pStock;
			filename=OpenFileDialog.GetNextPathName(l_position);
			index++;
   			if(stockfile.Open((LPCTSTR)filename,CFile::modeReadWrite))
			{
				int FileLength=stockfile.GetLength();
				stockfile.Read(&stocklength,4);
				int FileCalcLength=4+stocklength*12;
				if(FileLength!=FileCalcLength)
				{
					MessageBox("�����������ļ�����ȷ,����ѡ����ļ�! ���ε������ʧ��.","��鵼�������ʾ",MB_OK);
				    stockfile.Close();
		            pWnd->EnableWindow(TRUE);
                    EndWaitCursor();
					return;

				}
				for(int j=0;j<stocklength;j++)
				{
					STOCKINFOINBLOCK *pStock;
					SharesSymbol *symbol=new SharesSymbol;
					stockfile.Read(symbol->sharesymbol,6);
					stockfile.Read(&symbol->nKind,4);
					symbol->sharesymbol[6]='\0';
					stockfile.Read(&blank,2);
					if(!m_pDoc->m_ManagerBlockData.Lookup(symbol->sharesymbol,symbol->nKind,pStock))
					{
						AddCounts++;
					}
					delete symbol;
				}
				stockfile.Close();

			}
		}
		m_pDoc->m_ManagerBlockData.CheckStockCount(AddCounts);
//*************************************************************************************
        m_Progress.SetRange(0,index);
        m_Progress.SetPos(0);
		index=0;
        l_position=OpenFileDialog.GetStartPosition();
		while(l_position)
		{
			CString l_sFileName;
			CString l_sFilePath;
			int l_nPos;
			CReportData *pCdat;
			BLOCKINFO *l_pStockkType;
			filename=OpenFileDialog.GetNextPathName(l_position);
			filename.MakeReverse();
			l_nPos=filename.Find("\\",0);
			l_sFileName=filename.Left(l_nPos);
			l_sFileName.MakeReverse();
			if(l_sFileName.GetLength()>8)
			{
				MessageBox("���������ĳ��Ȳ��ܴ���8���ֽ�","��������ʾ",MB_OK);
				continue;
			}
			filename.MakeReverse();
			l_sFilePath=filename.Left(filename.GetLength() - l_nPos-1);   //�õ������ļ���·�����ļ���
			m_pDoc->m_ManagerBlockData.ImportBlockData(l_sFilePath,l_sFileName,l_pStockkType);  //������
			
			m_pDoc->m_sharesInformation.InsertItem(l_pStockkType->m_szCode,pCdat,BLOCKINDEX);              //���ӹ�Ʊ����
			m_pDoc->m_pReportView->LoadShowData(BLOCKINDEX);              //��CGRID������һ�� 
			strcpy(pCdat->name ,l_pStockkType->m_szName );
			strcpy(pCdat->id ,l_pStockkType->m_szCode );
            pCdat->kind=BLOCKINDEX  ;
			if(strlen(pCdat->name)>0)
			   m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;

			m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
			WORD type=BLOCKINDEX;
			m_pDoc->m_pReportView->AddGridRow(type);              //��CGRID������һ�� 

			FillToListCtrlItem(l_pStockkType);           //�������뵽CListCtrl��
//*************************************************************************************
			index++;
			m_Progress.SetPos(index);
			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

//*************************************************************************************
		}	
		pWnd->EnableWindow(TRUE);
        EndWaitCursor();

	}
	m_Progress.SetPos(0);
}

//���ܣ������µİ������
//�����������
//�����������
void CSuperviseSharesBlock::OnAddStockType() 
{
	int nItem = -1;
	CString strItem = "";
	CStringArray stockTypeCodeArray;

	POSITION pos = m_lstBlockTypeName.GetFirstSelectedItemPosition();
	if (pos == NULL)
		return;
	else
	{
		while (pos)
		{
			nItem = m_lstBlockTypeName.GetNextSelectedItem(pos);
			strItem = m_lstBlockTypeName.GetItemText(nItem,0);
			break;
		}
	}


	BLOCKINFO *pBlockInfo=new BLOCKINFO;
	char temp[10];
	int l_nPos=m_pDoc->m_ManagerBlockData.GetInsertBlockPos();  //�õ��°��λ��
    sprintf(temp,"%3d",l_nPos);         
	for(int i=0;i<3;i++)
	{
		if(temp[i]==' ')temp[i]='0';
	}
	sprintf(pBlockInfo->m_szCode,"%s%s","8K0",temp);         //�ӵõ��°��λ�����ɰ�����
	strcpy(pBlockInfo->m_szBlockTypeName,strItem.GetBuffer(0));//��������

	pBlockInfo->m_iRightType=0;       

	CModifyStockType l_ModifyStockType(pBlockInfo,TRUE,this);   //�����Ӱ��Ի���
    if(l_ModifyStockType.DoModal()== IDOK)
	{
		RefreshShowData();                                          //���ӳɹ�ˢ�°������
	}
    delete pBlockInfo;
}

//���ܣ��򿪰�������޸ĶԻ������ڹ������޸�
//�����������
//�����������
void CSuperviseSharesBlock::OnUpdateStockType() 
{
	// TODO: Add your control notification handler code here
	BLOCKINFO *l_pStockType,*l_pStockTypeInfo,*pStockType;
	l_pStockType=new BLOCKINFO;
	int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;  //�õ���ȷ��Ҫ�޸İ������
	char l_StockTypeCode[10];
	if(temp==-1)
	{
		if(l_pStockType) delete l_pStockType;
		AfxMessageBox("��ѡ����ȷ�İ��!");
		return;
    }
	//m_lstStockTypeName.GetItemText( temp, 0, l_StockTypeCode, 10);
	pStockType = (BLOCKINFO *)m_lstStockTypeName.GetItemData(temp);
	m_pDoc->m_ManagerBlockData.GetBlockPoint(l_pStockTypeInfo,pStockType->m_szCode);
	memcpy(l_pStockType,l_pStockTypeInfo,sizeof(BLOCKINFO));
	CModifyStockType l_ModifyStockType(l_pStockType,FALSE);
    if(l_ModifyStockType.DoModal()== IDOK)         //�򿪰���޸ĶԻ���
	{
		RefreshShowData();                         //�ɹ��޸ĺ�ˢ������
	}
	delete l_pStockType;
}

//���ܣ�����ѡ�ɹ�����
//�����������
//�����������
void CSuperviseSharesBlock::OnManagerChoose() 
{
	// TODO: Add your control notification handler code here
	CSuperviseChoose l_ManagerChooseDlg;
    l_ManagerChooseDlg.DoModal();    //����ѡ�ɹ�����
}
//���ܣ�ˢ��CListCtrl������ݣ��ڽ��а�������޸ĺ��������ˢ��
//�����������
//�����������
void CSuperviseSharesBlock::RefreshShowData()
{
    BeginWaitCursor();

	int nItem = -1;
	CString strItem = "";
	CStringArray stockTypeCodeArray;

	POSITION pos = m_lstBlockTypeName.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			nItem = m_lstBlockTypeName.GetNextSelectedItem(pos);
			break;
		}
	}
	if (nItem != -1)
	{
		strItem = m_lstBlockTypeName.GetItemText(nItem,0);
		m_pDoc->m_ManagerBlockData.GetAllBlockCodesFromBlockTypeName(stockTypeCodeArray,strItem);
	}

	this->m_lstStockTypeName.DeleteAllItems();
	//m_pDoc->m_ManagerBlockData.GetAllBlockCodes(stockTypeCodeArray); //�õ��������
	int l_nCount=stockTypeCodeArray.GetSize();
    for(int i=0;i<l_nCount;i++)     //��������ݲ���CListCtrl
	{
       BLOCKINFO *l_pStktype;
       CString l_sStockType=stockTypeCodeArray.GetAt(i);
       m_pDoc->m_ManagerBlockData.GetBlockPoint(l_pStktype,l_sStockType.GetBuffer(0));
	   this->FillToListCtrlItem(l_pStktype);
	}

    EndWaitCursor();

}

//���ܣ���ĳһ����ȫ�������ɾ��
//�����������
//�����������
void CSuperviseSharesBlock::OnDeletestocktype() 
{
	// TODO: Add your control notification handler code here
	int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;  //����ɾ�����
	char l_StockTypeCode[10];
	if(temp==-1)
	{
		AfxMessageBox("��ѡ��ɾ���İ��!");
		return;
    }
    int rtn = MessageBox("�Ƿ�ȷʵ������һɾ������?","����",MB_YESNO|MB_ICONWARNING);
    if(rtn!=6)
	  return;

	//m_lstStockTypeName.GetItemText( temp, 0, l_StockTypeCode, 10);        //�õ�ɾ��������
	//m_pDoc->m_ManagerBlockData.DeleteBlock(l_StockTypeCode);   //ɾ�����
	
	BLOCKINFO *pStockType;
	pStockType = (BLOCKINFO *)m_lstStockTypeName.GetItemData(temp);
	if (!pStockType)
	{
		return;
	}
	strcpy(l_StockTypeCode, pStockType->m_szCode);
	m_pDoc->m_ManagerBlockData.DeleteBlock(l_StockTypeCode);   //ɾ�����


    CReportData *pCdat = NULL;
	m_pDoc->m_sharesInformation.Lookup(l_StockTypeCode,pCdat,BLOCKINDEX);         //ɾ������Ʊ����
	if (pCdat)
	{
		m_pDoc->DeleteStockFromKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
	}
    m_pDoc->m_sharesInformation.DeleteOneStockInfo(l_StockTypeCode,BLOCKINDEX);
	RefreshShowData();
}

//���ܣ��������ˢ�£��ؼ�����ÿ����ָ��
//�����������
//�����������
void CSuperviseSharesBlock::OnRefresh() 
{
	//CWnd *pWnd=this->GetDlgItem(IDC_REFRESH);
	//pWnd->EnableWindow(FALSE);
 //   BeginWaitCursor();

	//m_pDoc->m_ManagerBlockData.CalcBlockKlineData(); //�����鵱��ÿ���Ӱ��ָ��
	//m_pDoc->m_ManagerBlockData.CalcBlockOpenClosePrice();  //�����鵱�쿪�������հ��ָ��
	//pWnd->EnableWindow(TRUE);
 //   EndWaitCursor();
	//pWnd->EnableWindow(TRUE);

	//���°���5����������
	CWnd *pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_ADDSTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_UPDATESTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_DELETESTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_REFRESH_MINUTE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_ADDBLOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_DELBLOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_MODSTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 
	m_bIsCalc=TRUE;
	
	BeginWaitCursor();
	CStringArray BlockCodeArray;
	m_pDoc->m_ManagerBlockData.GetAllBlockCodes(BlockCodeArray);
	for (int i = 0;i < BlockCodeArray.GetCount();i++)
	{
		m_pDoc->m_ManagerBlockData.RefreshHistoryMin5LineData((LPTSTR)(LPCTSTR)BlockCodeArray[i],&m_Progress);
		ProcMessage();
	}

    EndWaitCursor();
	pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_ADDSTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_UPDATESTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_DELETESTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_REFRESH_MINUTE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_ADDBLOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_DELBLOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_MODSTOCKTYPE);
	pWnd->EnableWindow(TRUE);

	m_bIsCalc=FALSE;
	pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_ENABLED); 

}

void CSuperviseSharesBlock::OnExport() 
{
	// TODO: Add your control notification handler code here
	char m_szCurrentDir[MAX_PATH],makedirectory[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, m_szCurrentDir);
	sprintf(makedirectory,"%s\\�������",m_szCurrentDir);
	if(_access(makedirectory,0)==-1)
		_mkdir(makedirectory);

	int nCount=m_lstStockTypeName.GetItemCount();
	char  blank[2]={' ',' '};              //����֮��ļ��
	for(int i=0;i<nCount;i++)
	{
		char strStockCode[10];
		char strStockName[10];
		FILE *fp;
		SymbolKindArr StockCodeArray;
        char FileName[256];
		m_lstStockTypeName.GetItemText( i, 0, strStockCode, sizeof(strStockCode) ) ;
		m_lstStockTypeName.GetItemText( i, 1, strStockName, sizeof(strStockName) ) ;
		sprintf(FileName,"%s\\%s",makedirectory,strStockName);
        m_pDoc->m_ManagerBlockData.GetStockFromBlockCode(StockCodeArray,strStockCode);
        int length=StockCodeArray.GetSize();
		fp=_fsopen(FileName,"w+b",SH_DENYNO);
		if(fp!=NULL)
		{
			fwrite(&length,4,1,fp);
			for(int i=0;i<StockCodeArray.GetSize();i++)
			{
				SymbolKind m_SymbolKind=StockCodeArray.GetAt(i);
				fwrite(m_SymbolKind.m_chSymbol ,1,6,fp);
				fwrite(&m_SymbolKind.m_nSymbolKind ,1,4,fp);

				fwrite(&blank,1,2,fp);
			}
			fclose(fp);
		}       
	}
	if(nCount>0)
		AfxMessageBox("������ݵ����ɹ�!");
}

void CSuperviseSharesBlock::OnRefreshDayLine() 
{
	/*Marenan
	int temp=m_lstStockTypeName.GetNextItem( -1, LVNI_SELECTED   ) ;  //����ɾ�����
	char l_StockTypeCode[20];
	if(temp==-1)
	{
		AfxMessageBox("��ѡ����ȷ��ˢ�°��!");
		return;
    }
	CWnd *pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(FALSE);
	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 

    BeginWaitCursor();
    m_bIsCalc=TRUE;
	m_lstStockTypeName.GetItemText( temp, 0, l_StockTypeCode, 20);        //�õ�ɾ��������
	BLOCKINFO *pStockType = NULL;
	pStockType = (BLOCKINFO *)m_lstStockTypeName.GetItemData(temp);
	if (!pStockType)
	{
		return;
	}
    m_pDoc->m_ManagerBlockData.RefreshHistoryDayLineData( pStockType->m_szCode,&m_Progress);
	pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(TRUE);

	//if(temp+1<m_lstStockTypeName.GetItemCount())
	//{
	//	m_lstStockTypeName.SetFocus();
	//	m_lstStockTypeName.SetItemState(temp+1,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;
	//}
	EndWaitCursor();
	m_bIsCalc=FALSE;
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_ENABLED); 
	
	Marenan*/

	//���°�����������
	CWnd *pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(FALSE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_ADDSTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_UPDATESTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_DELETESTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_REFRESH_MINUTE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_ADDBLOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_DELBLOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_MODSTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 
	m_bIsCalc=TRUE;
	
	CStringArray BlockCodeArray;
	m_pDoc->m_ManagerBlockData.GetAllBlockCodes(BlockCodeArray);
	for (int i = 0;i < BlockCodeArray.GetCount();i++)
	{
		m_pDoc->m_ManagerBlockData.RefreshHistoryDayLineData((LPTSTR)(LPCTSTR)BlockCodeArray[i],&m_Progress);
		ProcMessage();
	}
	pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_ADDSTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_UPDATESTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_DELETESTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_REFRESH_MINUTE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_ADDBLOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_DELBLOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_MODSTOCKTYPE);
	pWnd->EnableWindow(TRUE);

	m_bIsCalc=FALSE;
	pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_ENABLED); 

}
void CSuperviseSharesBlock::OnCancel()
{
	if(m_bIsCalc)
	{
		AfxMessageBox("�����������㲻�ܽ����˳�����!");
		return;
	}

   	m_pDoc ->m_bOpenStockTypeDlg=FALSE;
	CDialog::OnCancel();
    SendMessage(WM_CLOSE,0,0); 
}

void CSuperviseSharesBlock::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bIsCalc)
	{
		AfxMessageBox("�����������㲻�ܽ����˳�����!");
		return;
	}

	::SendMessage(m_pDoc->m_pReportView->m_hWnd,WM_USER_CHANGESTOCK,0,0);
   	m_pDoc ->m_bOpenStockTypeDlg=FALSE;

	CDialog::OnClose();
}

void CSuperviseSharesBlock::OnOK() 
{
	// TODO: Add extra validation here
	if(m_bIsCalc)
	{
		AfxMessageBox("�����������㲻�ܽ����˳�����!");
		return;
	}

	::SendMessage(m_pDoc->m_pReportView->m_hWnd,WM_USER_CHANGESTOCK,0,0);
   	m_pDoc ->m_bOpenStockTypeDlg=FALSE;

	CDialog::OnOK();
}

BOOL CSuperviseSharesBlock::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,248);
  //  return TRUE;	
	DoHtmlHelp(this);return TRUE;
	
//	return CDialog::OnHelpInfo(pHelpInfo);
}

void CSuperviseSharesBlock::OnBnClickedAddblocktype()
{
	CBlockTypeNameDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString strName;
		BOOL bFind = FALSE;
		dlg.GetString(strName);

		for (int i = 0;i < m_lstBlockTypeName.GetItemCount();i++)
		{
			CString strItem = m_lstBlockTypeName.GetItemText(i,0);
			if (strItem == strName)
			{
				bFind = TRUE;
				break;
			}
		}
		if (!bFind)
		{
			m_lstBlockTypeName.InsertItem(m_lstBlockTypeName.GetItemCount(),strName,0);
			//�������ݿ�
			m_pDoc->m_ManagerBlockData.AddBlockType(strName);
		}
		else
		{
			AfxMessageBox("�����Ѿ�����!");
		}

	}
}

void CSuperviseSharesBlock::OnBnClickedDelblocktype()
{
	int nItem = -1;
	CString strItem = "";
	CStringArray stockTypeCodeArray;

	POSITION pos = m_lstBlockTypeName.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			nItem = m_lstBlockTypeName.GetNextSelectedItem(pos);
			strItem = m_lstBlockTypeName.GetItemText(nItem,0);
			break;
		}
	}
	if (strItem == "ȫ�����" || strItem == "�������")
	{
		AfxMessageBox("���ð����࣬����ɾ��!");
	}
	if (nItem != -1)
	{
		m_lstBlockTypeName.DeleteItem(nItem);
		//�������ݿ�,ɾ��������صİ������
		m_pDoc->m_ManagerBlockData.DelBlockType(strItem);
		RefreshShowData();
	}
}

void CSuperviseSharesBlock::OnBnClickedModstocktype()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CSuperviseSharesBlock::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nItem = -1;
	CString strItem = "";
	CStringArray stockTypeCodeArray;

	POSITION pos = m_lstBlockTypeName.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			nItem = m_lstBlockTypeName.GetNextSelectedItem(pos);
			break;
		}
	}
	if (nItem != -1)
	{
		strItem = m_lstBlockTypeName.GetItemText(nItem,0);
		m_pDoc->m_ManagerBlockData.GetAllBlockCodesFromBlockTypeName(stockTypeCodeArray,strItem);
	}

	m_lstStockTypeName.DeleteAllItems();
	int nCount = stockTypeCodeArray.GetSize();
	for(int i=0;i<nCount;i++)
	{
		BLOCKINFO *pStktype;
		CString sStockType = stockTypeCodeArray.GetAt(i);
		m_pDoc->m_ManagerBlockData.GetBlockPoint(pStktype,sStockType.GetBuffer(0));
		this->FillToListCtrlItem(pStktype);         //��������CListCtrl�ؼ���
	}

	*pResult = 0;
}

void CSuperviseSharesBlock::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nItem = -1;
	CString strItem = "";

	POSITION pos = m_lstStockTypeName.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			nItem = m_lstBlockTypeName.GetNextSelectedItem(pos);
			break;
		}
	}
	if (nItem != -1)
	{
		strItem = m_lstStockTypeName.GetItemText(nItem,0);
		strcpy(m_pDoc->m_SystemInitData.StockTypeName,strItem.GetBuffer(0));
		m_pDoc->m_pReportView->ChangeToPage(BLOCKPAGE, 0);
		m_pDoc->m_pReportView->m_pPageWnd->ReplacePage(BLOCKPAGE, strItem);
	}
	*pResult = 0;
}

void CSuperviseSharesBlock::OnBnClickedRefreshMinute()
{
	//���°��ķ���������
	CWnd *pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_ADDSTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_UPDATESTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_DELETESTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_REFRESH_MINUTE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_ADDBLOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_DELBLOCKTYPE);
	pWnd->EnableWindow(FALSE);
	pWnd=this->GetDlgItem(IDC_MODSTOCKTYPE);
	pWnd->EnableWindow(FALSE);
	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 
	m_bIsCalc=TRUE;

	BeginWaitCursor();
	
	m_pDoc->m_ManagerBlockData.CalcBlockKlineData(); //�����鵱��ÿ���Ӱ��ָ��
	m_pDoc->m_ManagerBlockData.CalcBlockOpenClosePrice();  //�����鵱�쿪�������հ��ָ��

	EndWaitCursor();
	pWnd=this->GetDlgItem(IDC_REFRESH);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDOK);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_ADDSTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_UPDATESTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_DELETESTOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_REFRESH_DAY);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_REFRESH_MINUTE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_ADDBLOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_DELBLOCKTYPE);
	pWnd->EnableWindow(TRUE);
	pWnd=this->GetDlgItem(IDC_MODSTOCKTYPE);
	pWnd->EnableWindow(TRUE);

	m_bIsCalc=FALSE;
	pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_ENABLED); 
}
