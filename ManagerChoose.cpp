// ManagerChoose.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "mainfrm.h"
#include "selectstock.h"
#include "keybrodeangel.h"
#include "ManagerChoose.h"
#include "ChooseTypeNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSuperviseChoose dialog


CSuperviseChoose::CSuperviseChoose(CWnd* pParent /*=NULL*/)
	: CDialog(CSuperviseChoose::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSuperviseChoose)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSuperviseChoose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSuperviseChoose)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_BUTTON2, m_btn2);
	DDX_Control(pDX, IDC_BUTTON1, m_btn1);
	DDX_Control(pDX, IDC_LIST1, m_lstStockName);
	DDX_Control(pDX, IDC_LIST4, m_lstChooseType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSuperviseChoose, CDialog)
	//{{AFX_MSG_MAP(CSuperviseChoose)
	ON_BN_CLICKED(IDC_BUTTON1, OnAddStockToChoose)
	ON_BN_CLICKED(IDC_BUTTON2, OnDeleteStockFromChoose)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_ADDSTOCK,OnAddStockCode)
	ON_BN_CLICKED(IDOK, &CSuperviseChoose::OnBnClickedOk)
	ON_BN_CLICKED(IDC_NEWCHOOSETYPE, &CSuperviseChoose::OnBnClickedNewchoosetype)
	ON_BN_CLICKED(IDC_DELCHOOSETYPE, &CSuperviseChoose::OnBnClickedDelchoosetype)
	ON_BN_CLICKED(IDC_EMPTYCHOOSESTOCK, &CSuperviseChoose::OnBnClickedEmptychoosestock)
	ON_NOTIFY(NM_CLICK, IDC_LIST4, &CSuperviseChoose::OnNMClickList4)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSuperviseChoose message handlers

BOOL CSuperviseChoose::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->CenterWindow();
	m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_pDoc ;

	m_pImageList = new CImageList();
	m_pImageListSmall = new CImageList();
	ASSERT(m_pImageList != NULL && m_pImageListSmall != NULL);    // serious allocation failure checking
	m_pImageList->Create(IDB_BITMAP14,32,0,RGB(192,192,192));
	//m_pImageListSmall->Create(IDB_BITMAP15,16,0,RGB(192,192,192));
	m_pImageListSmall->Create(IDB_BITMAP23,16,0,RGB(255,255,255)); //建立小图象列表
	this->m_lstStockName.SetImageList(m_pImageList, LVSIL_NORMAL);
	this->m_lstStockName.SetImageList(m_pImageListSmall, LVSIL_SMALL);
    FillToListCtrlHead();

	//自选股类型
	CString strFirst = "";
	CStringArray chooseTypeName;
	m_pDoc->m_ManagerBlockData.GetChooseStockTypeCodes(chooseTypeName);
	int nCount=chooseTypeName.GetSize();
	for(int i=0;i<nCount;i++)
	{
		BLOCKINFO *l_pStktype;
		CString l_sStockType=chooseTypeName.GetAt(i);
		m_pDoc->m_ManagerBlockData.GetBlockPoint(l_pStktype,l_sStockType.GetBuffer(0));
		
		m_lstChooseType.InsertItem(i,l_pStktype->m_szName,0);
		m_lstChooseType.SetItemData(i,(DWORD_PTR)l_pStktype);
		if (i == 0)
		{
			strFirst = l_pStktype->m_szName;
		}
	}
	//相应的股票
	SymbolKindArr l_StockArray;
	if (strFirst != "")
	{
		m_pDoc->m_ManagerBlockData.GetStockFromBlockName(l_StockArray,strFirst.GetBuffer(0));
		int l_nCount=l_StockArray.GetSize();
		for(int i=0;i<l_nCount;i++)
		{
			CReportData *pCdat;
			SymbolKind l_sCode=l_StockArray.GetAt(i);
			if(m_pDoc->m_sharesInformation.Lookup(l_sCode.m_chSymbol,pCdat,l_sCode.m_nSymbolKind))
			{
				InsertStockToList(pCdat->name,pCdat->id,l_sCode.m_nSymbolKind);
			}
		}
	}
	m_pImageList->Detach();
	delete m_pImageList;
	m_pImageListSmall->Detach();
	delete m_pImageListSmall;

    m_lstChooseType.SetFocus();
    m_lstChooseType.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSuperviseChoose::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
BOOL CSuperviseChoose::InsertStockToList(char *name,char *code,int nKind)
{
	int  iIcon, iItem, iSubItem, iActualItem;
	LPTSTR   pStrTemp1, pStrTemp2;
	LV_ITEM  lvitem;
	LVFINDINFO  lvf;
	lvf.flags = LVFI_STRING;
	lvf.psz = name;
    //m_lstStockName.SetColumnWidth( 0, 80 );

	if(m_lstStockName.FindItem(&lvf)>=0)
		return FALSE;

	iItem = m_lstStockName.GetItemCount();

	//m_lstStockName.InsertItem(iItem,name,1);
	//m_lstStockName.SetItemData(iItem,(DWORD_PTR)nKind);

	for (iSubItem = 0; iSubItem <2; iSubItem++)
	{
		if (iSubItem == 0)
			iIcon = 0;  // choose the icon and legend for the entry

		lvitem.mask = LVIF_TEXT | (iSubItem == 0? LVIF_IMAGE|LVIF_PARAM  : 0);
		lvitem.iItem = (iSubItem == 0)? iItem : iActualItem;
		lvitem.iSubItem = iSubItem;

		// calculate the main and sub-item strings for the current item
		CString strIconDesc, strIconShortDesc;
        strIconShortDesc=name;
	    strIconDesc=code;
		pStrTemp1= strIconShortDesc.GetBuffer(strIconShortDesc.GetLength()); 
		pStrTemp2= strIconDesc.GetBuffer(strIconDesc.GetLength());
		lvitem.pszText = iSubItem == 0? pStrTemp1 : pStrTemp2;

		lvitem.iImage = iIcon;
		if (iSubItem == 0)
			iActualItem =this->m_lstStockName.InsertItem(&lvitem); // insert new item
		else
			this->m_lstStockName.SetItem(&lvitem); // modify existing item (the sub-item text)
		m_lstStockName.SetItemData(iItem,nKind);
	} 
	return TRUE;
}

BOOL CSuperviseChoose::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
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
		   char result[10];  		   //返回的结果
		   int nKind=m_diakeybrode.m_stkKind;
           strcpy(result,m_diakeybrode.result);
		   
		   if(strlen(result)==6||strlen(result)==4)
		   {
			  CReportData *Cdat;
	 		  if (m_pDoc->m_sharesInformation.Lookup(result , Cdat,nKind) == TRUE)      //检测该股票是否已记录在内存模板中
			  {
				  LV_ITEM  lvitem;
				  LVFINDINFO  lvf;
				  lvf.flags=LVFI_STRING;
				  lvf.psz=Cdat->name;
				  if(m_lstStockName.FindItem(&lvf)>=0)
			 		 return  TRUE;
              
				   m_pDoc->m_ManagerBlockData.InsertStockToChoose(Cdat->id,nKind);
                   InsertStockToList(Cdat->name,Cdat->id,nKind);
			  }
		   }
		}
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSuperviseChoose::OnAddStockToChoose() 
{
	// TODO: Add your control notification handler code here
	SelectStock pDlg(this,TRUE);
	pDlg.DoModal();
}

void CSuperviseChoose::OnDeleteStockFromChoose() 
{
	int nItem = -1;
	CString strItem = "";

	POSITION pos = m_lstChooseType.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			nItem = m_lstChooseType.GetNextSelectedItem(pos);
			break;
		}
	}
	if (nItem != -1)
	{
		CString strItem;

		BLOCKINFO *l_pStktype;
		l_pStktype = (BLOCKINFO *)m_lstChooseType.GetItemData(nItem);

		CString Id;
		int rtn = MessageBox("是否确实进行这一删除操作?","警告",MB_YESNO|MB_ICONWARNING);
		if(rtn!=6)
		  return;

		int temp=m_lstStockName.GetNextItem( -1, LVNI_SELECTED   ) ;
		if(temp==-1)
			return;
		while(temp>=0)
		{
			char l_szCode[10];
			m_lstStockName.GetItemText( temp, 1, l_szCode, 20);
			int nKind=m_lstStockName.GetItemData(temp);
			//m_pDoc->m_ManagerBlockData.DeleteStockFromChoose(l_szCode,nKind); 
			m_pDoc->m_ManagerBlockData.DeleteStockFromBlock(l_szCode,nKind,l_pStktype->m_szCode);

			m_lstStockName.DeleteItem(temp);
			temp=m_lstStockName.GetNextItem( -1,LVNI_SELECTED   ) ;
		}
		m_lstStockName.SetFocus();
		m_lstStockName.SetItemState(0,LVIS_FOCUSED|LVIS_SELECTED , LVIS_SELECTED|LVIS_FOCUSED) ;

	}


}
void CSuperviseChoose::FillToListCtrlHead()
{ 
   	CRect           rect;
	CString         strItem1= _T("股票名称");
	CString         strItem2= _T("股票代码");

	// insert two columns (REPORT mode) and modify the new header items
	m_lstStockName.GetWindowRect(&rect);
	m_lstStockName.InsertColumn(0, strItem1, LVCFMT_LEFT,rect.Width() * 1/4, 0);
	m_lstStockName.InsertColumn(1, strItem2, LVCFMT_LEFT,rect.Width() * 1/4, 1);
	
	m_lstStockName.SetExtendedStyle(m_lstStockName.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	////this->m_lstStockTypeName.SetImageList(m_pImageList, LVSIL_NORMAL);
	m_lstStockName.SetImageList(m_pImageListSmall, LVSIL_SMALL);

	m_lstChooseType.SetExtendedStyle(m_lstChooseType.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	//this->m_lstBlockTypeName.SetImageList(m_pImageList, LVSIL_NORMAL);
	this->m_lstChooseType.SetImageList(m_pImageListSmall, LVSIL_SMALL);
}
LRESULT CSuperviseChoose::OnAddStockCode(WPARAM wParam, LPARAM lParam) //增加股票至LISTCTRL
{
	int nItem = -1;
	CString strItem = "";

	POSITION pos = m_lstChooseType.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			nItem = m_lstChooseType.GetNextSelectedItem(pos);
			break;
		}
	}
	if (nItem != -1)
	{
		BLOCKINFO *l_pStktype;
		l_pStktype = (BLOCKINFO *)m_lstChooseType.GetItemData(nItem);

		SHARES_DATA_ADD *pAddCode;
		int lsCount=(int)lParam;
		pAddCode=(SHARES_DATA_ADD *)wParam;
		for(int i=0;i<lsCount;i++)
		{
			if(pAddCode[i].StockType<0)
			{
				CReportData *Cdat;
				int nKind=-pAddCode[i].StockType-1;
	 			if (m_pDoc->m_sharesInformation.Lookup(pAddCode[i].name, Cdat,nKind) == TRUE)     //检测该股票是否已记录在内存模板中
				{
					LV_ITEM  lvitem;
					int iItem=0;
					LVFINDINFO  lvf;
					lvf.flags=LVFI_STRING;
					lvf.psz=Cdat->name;
					iItem=m_lstStockName.FindItem(&lvf);
					if(iItem>=0)
					{
						if(nKind==m_lstStockName.GetItemData(iItem))  
						   continue;
					}

					if(CSharesInformation::IsZhiShu(Cdat->kind))
					{
						MessageBox("指数类将不能增加到板块中!","窗口操作提示",MB_OK);
						continue;
					}
					if(Cdat->pBaseInfo==NULL||Cdat->pBaseInfo->zgb<=0)
					{
						MessageBox("在将股票加入板块时,该股票财务数据不能为空!","窗口操作提示",MB_OK);
						continue;
					}
					if(Cdat->pBaseInfo!=NULL)
					{
						if(m_pDoc->m_ManagerBlockData.InsertStockToBlock(Cdat,l_pStktype->m_szCode))
							InsertStockToList(Cdat->name,Cdat->id,nKind);
					}
					//m_pDoc->m_ManagerBlockData.InsertStockToChoose(Cdat->id,nKind);
				}
			}
		}	
	}


	return 1L;
}

void CSuperviseChoose::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CSuperviseChoose::OnBnClickedNewchoosetype()
{
	CChooseTypeNameDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString strName;
		BOOL bFind = FALSE;
		dlg.GetString(strName);

		for (int i = 0;i < m_lstChooseType.GetItemCount();i++)
		{
			CString strItem = m_lstChooseType.GetItemText(i,0);
			if (strItem == strName)
			{
				bFind = TRUE;
				break;
			}
		}
		if (!bFind)
		{
			//更改数据库
			BLOCKINFO *l_pStockType = NULL;

			if(m_pDoc->m_ManagerBlockData.CheckBlockName(strName.GetBuffer(0),NULL))
			{
				AfxMessageBox("请选择正确板块名称! 板块名称不能重复");
				return ;
			}
			int l_nPos=m_pDoc->m_ManagerBlockData.GetInsertBlockPos();  //得到新板块位置
			if(!m_pDoc->m_ManagerBlockData.InsertBlock(l_pStockType,l_nPos))
			{
				return;
			}
			l_pStockType->m_lIsUse=TRUE;
			l_pStockType->m_bIsDelete =FALSE;
			char temp[10];
			sprintf(temp,"%3d",l_nPos);         
			for(int i=0;i<3;i++)
			{
				if(temp[i]==' ')temp[i]='0';
			}
			sprintf(l_pStockType->m_szCode,"%s%s","8K0",temp);         //从得到新板块位置生成板块代码
			strcpy(l_pStockType->m_szName,strName.GetBuffer(0));
			strcpy(l_pStockType->m_szBlockTypeName,"自选股");//板块分类名

			l_pStockType->m_iRightType=1;       

			l_pStockType->m_lStockCount=0;
			l_pStockType->m_fTotalRight=0;

			CReportData *pCdat = NULL;
			if (!m_pDoc->m_sharesInformation.InsertItem(l_pStockType->m_szCode,pCdat,CHOOSESTK))
			{
				return;
			}
			strcpy(pCdat->name ,l_pStockType->m_szName );
			strcpy(pCdat->id ,l_pStockType->m_szCode );
			pCdat->kind=CHOOSESTK;
			m_pDoc->m_sharesInformation.SaveRealDataToFile(pCdat,sizeof(CReportData));
			if(strlen(pCdat->name)>0)
				m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;
			m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);

			int iIndex = m_lstChooseType.GetItemCount();
			m_lstChooseType.InsertItem(iIndex,strName,0);
			m_lstChooseType.SetItemData(iIndex,(DWORD_PTR)l_pStockType);
		}
		else
		{
			AfxMessageBox("分类已经存在!");
		}


	}
}

void CSuperviseChoose::OnBnClickedDelchoosetype()
{
	int nItem = -1;
	CString strItem = "";
	CStringArray stockTypeCodeArray;
	BLOCKINFO *l_pStktype;

	POSITION pos = m_lstChooseType.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			nItem = m_lstChooseType.GetNextSelectedItem(pos);
			strItem = m_lstChooseType.GetItemText(nItem,0);
			l_pStktype = (BLOCKINFO *)m_lstChooseType.GetItemData(nItem);
			break;
		}
	}
	if (strcmp(strItem.GetBuffer(0), m_pDoc->m_SystemInitData.ChooseTypeName) == 0)
	{
		AfxMessageBox("当前的自选板块分类，不能删除!");
		return;
	}
	if (nItem != -1)
	{
		m_lstChooseType.DeleteItem(nItem);
		//更改数据库,删除所有相关的板块数据
		m_pDoc->m_ManagerBlockData.DeleteBlock(l_pStktype->m_szCode);
		
		CReportData *pCdat = NULL;
		m_pDoc->m_sharesInformation.Lookup(l_pStktype->m_szCode,pCdat,CHOOSESTK);         //删除板块股票代码
		if (pCdat)
		{
			m_pDoc->DeleteStockFromKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
		}
		m_pDoc->m_sharesInformation.DeleteOneStockInfo(l_pStktype->m_szCode,CHOOSESTK);
		//RefreshShowData();
	}

}

void CSuperviseChoose::OnBnClickedEmptychoosestock()
{
	int nItem = -1;
	CString strItem = "";

	int rtn = MessageBox("是否确实进行这一删除操作?","警告",MB_YESNO|MB_ICONWARNING);
	if(rtn!=6)
		return;

	POSITION pos = m_lstChooseType.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			nItem = m_lstChooseType.GetNextSelectedItem(pos);
			break;
		}
	}
	for (int i = 0;i < m_lstChooseType.GetItemCount();i++)
	{
		CString strItem;

		BLOCKINFO *l_pStktype;
		l_pStktype = (BLOCKINFO *)m_lstChooseType.GetItemData(i);

		CString Id;

		char l_szCode[10];
		m_lstStockName.GetItemText( i, 1, l_szCode, 20);
		int nKind=m_lstStockName.GetItemData(i);
		//m_pDoc->m_ManagerBlockData.DeleteStockFromChoose(l_szCode,nKind); 
		m_pDoc->m_ManagerBlockData.DeleteStockFromBlock(l_szCode,nKind,l_pStktype->m_szCode);

	}
	m_lstStockName.DeleteAllItems();

}
void CSuperviseChoose::RefreshShowData()
{
	BeginWaitCursor();

	//int nItem = -1;
	//CString strItem = "";
	//CStringArray stockTypeCodeArray;

	//POSITION pos = m_lstBlockTypeName.GetFirstSelectedItemPosition();
	//if (pos == NULL)
	//	TRACE0("No items were selected!\n");
	//else
	//{
	//	while (pos)
	//	{
	//		nItem = m_lstBlockTypeName.GetNextSelectedItem(pos);
	//		break;
	//	}
	//}
	//if (nItem != -1)
	//{
	//	strItem = m_lstBlockTypeName.GetItemText(nItem,0);
	//	m_pDoc->m_ManagerBlockData.GetAllBlockCodesFromBlockTypeName(stockTypeCodeArray,strItem);
	//}

	//this->m_lstStockTypeName.DeleteAllItems();
	////m_pDoc->m_ManagerBlockData.GetAllBlockCodes(stockTypeCodeArray); //得到板块数据
	//int l_nCount=stockTypeCodeArray.GetSize();
	//for(int i=0;i<l_nCount;i++)     //将板块数据插入CListCtrl
	//{
	//	BLOCKINFO *l_pStktype;
	//	CString l_sStockType=stockTypeCodeArray.GetAt(i);
	//	m_pDoc->m_ManagerBlockData.GetBlockPoint(l_pStktype,l_sStockType.GetBuffer(0));
	//	this->FillToListCtrlItem(l_pStktype);
	//}

	EndWaitCursor();

}
void CSuperviseChoose::OnNMClickList4(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nItem = -1;
	CString strItem = "";
	SymbolKindArr m_StockCodeArray;

	POSITION pos = m_lstChooseType.GetFirstSelectedItemPosition();
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			nItem = m_lstChooseType.GetNextSelectedItem(pos);
			break;
		}
	}
	if (nItem != -1)
	{
		strItem = m_lstChooseType.GetItemText(nItem,0);
		m_pDoc->m_ManagerBlockData.GetStockFromBlockName(m_StockCodeArray,strItem.GetBuffer(0));
		
		m_lstStockName.DeleteAllItems();
		int nCount = m_StockCodeArray.GetSize();
		for(int i=0;i<nCount;i++)
		{
			CReportData *pCdat;
			SymbolKind l_sCode = m_StockCodeArray.GetAt(i);
			if(m_pDoc->m_sharesInformation.Lookup(l_sCode.m_chSymbol,pCdat,l_sCode.m_nSymbolKind))
			{
				InsertStockToList(pCdat->name,pCdat->id,l_sCode.m_nSymbolKind);
			}
		}
	}


	*pResult = 0;
}
