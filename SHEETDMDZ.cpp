// SHEETDMDZ.cpp : implementation file
//

#include "stdafx.h"
#include "SHEETDMDZ.h"

#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "cellRange.h"

#include "CTaiShanReportView.h"
#include "WH.h"

#include "CTaiStockInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTaiCodeContradistinguishPage property page

IMPLEMENT_DYNCREATE(CTaiCodeContradistinguishPage, CPropertyPage)

CTaiCodeContradistinguishPage::CTaiCodeContradistinguishPage() : CPropertyPage(CTaiCodeContradistinguishPage::IDD)
{
	//{{AFX_DATA_INIT(CTaiCodeContradistinguishPage)
	m_RadioSh = -1;
	//}}AFX_DATA_INIT

	//
	//Initialize all flag to false
	//
	m_bAdd=FALSE;
	m_bDelete=FALSE;
	m_bModify=FALSE;
	m_SelfBrush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
}

CTaiCodeContradistinguishPage::~CTaiCodeContradistinguishPage()
{
	
}

void CTaiCodeContradistinguishPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiCodeContradistinguishPage)
	DDX_Control(pDX, IDC_PHONETIC, m_EditPhonetic);
	DDX_Control(pDX, IDC_SYMBOL, m_EditCode);
	DDX_Control(pDX, IDC_NAME, m_EditName);
	DDX_Control(pDX, IDC_SYMBOLCOMPARE, m_ctrlSymbolCompare);
	DDX_Radio(pDX, IDC_RADIOSH, m_RadioSh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiCodeContradistinguishPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTaiCodeContradistinguishPage)
	ON_BN_CLICKED(IDC_RADIOSZ, OnRadiosz)
	ON_BN_CLICKED(IDC_RADIOSH, OnRadiosh)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_NOTIFY(NM_CLICK, IDC_SYMBOLCOMPARE, OnClickSymbolcompare)
	ON_NOTIFY(LVN_KEYDOWN, IDC_SYMBOLCOMPARE, OnKeydownSymbolcompare)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiCodeContradistinguishPage message handlers

BOOL CTaiCodeContradistinguishPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
 
	//
	//Set the default stock type radio
	//
	CheckRadioButton(IDC_RADIOSH,IDC_RADIOSZ,IDC_RADIOSH);

	//
	//Initialzie the listctrl
	//
	InitListCtrl();	

	//
	//Initialize edit box
	//
	m_EditCode.EnableWindow(FALSE);
	m_EditName.EnableWindow(FALSE);
	m_EditPhonetic.EnableWindow(FALSE);

	//
	//disable the button
	//
	GetDlgItem(IDC_MODIFY)->EnableWindow(FALSE);
	GetDlgItem(IDC_DEL)->EnableWindow(FALSE);

	m_nOldSel=-1;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//Initialzie the listctrl
void CTaiCodeContradistinguishPage::InitListCtrl()
{
	CTaiShanDoc* pDoc=GetDocument();

	//
	//Initialize and Set the listctrl's header or column
	//
	LVCOLUMN listColumn;
	char * arColumn[3]={"股票代码","股票名称","拼音简称"};
	listColumn.mask=LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	listColumn.fmt=LVCFMT_CENTER;
	for( int nColumn=0;nColumn<3;nColumn++)
	{
		listColumn.iSubItem=nColumn;
		listColumn.pszText=arColumn[nColumn];
		m_ctrlSymbolCompare.InsertColumn(nColumn, &listColumn);
	}
    //
	//Set the listctrl's property
	//
	ListView_SetExtendedListViewStyle(m_ctrlSymbolCompare.m_hWnd, LVS_EX_FULLROWSELECT);
	COLORREF color=RGB(255,255,220);
    m_ctrlSymbolCompare.SetBkColor( color );
	m_ctrlSymbolCompare.SetTextBkColor( color );
	m_ctrlSymbolCompare.SetColumnWidth(0,80);
	m_ctrlSymbolCompare.SetColumnWidth(1,90);
	m_ctrlSymbolCompare.SetColumnWidth(2,90);

	//
	//Set Listctrl's item
	//
	LVITEM   listItem;
	listItem.mask=LVIF_TEXT|LVIF_IMAGE;
	m_ctrlSymbolCompare.DeleteAllItems();
	int nTimes;
	int i,j;
	int nStockType;
	switch( GetCheckedRadioButton(IDC_RADIOSH,IDC_RADIOSZ) )
	{
	case IDC_RADIOSH: //Fill stock content of Shanghai
		nTimes=0;
		for(i=0;i<4;i++)
		{
			nStockType=i;
			if(i>2)nStockType=6;
			for(j=0;j<pDoc->m_sharesInformation.GetStockTypeCount(nStockType);j++)
			{
				CReportData *pDat1=NULL;
				pDoc->m_sharesInformation.GetStockItem(nStockType,j,pDat1);
				if(pDat1)
				{
					m_ctrlSymbolCompare.InsertItem (nTimes,  pDat1->id);
					m_ctrlSymbolCompare.SetItemText(nTimes,1,pDat1->name);
					m_ctrlSymbolCompare.SetItemText(nTimes,2,pDat1->Gppyjc);
					nTimes++;
				}
				if( nTimes == 20 ) m_ctrlSymbolCompare.Update(20);
			}
		}
		break;

	case IDC_RADIOSZ: //Fill stock content of Shenzhen
		nTimes=0;
		for(i=3;i<7;i++)
		{
			nStockType=i;
			if(i>5)nStockType=7;
			for(j=0;j<pDoc->m_sharesInformation.GetStockTypeCount(nStockType);j++)
			{
				CReportData *pDat1=NULL;
				pDoc->m_sharesInformation.GetStockItem(nStockType,j,pDat1);
				if(pDat1)
				{
					m_ctrlSymbolCompare.InsertItem (nTimes,  pDat1->id);
					m_ctrlSymbolCompare.SetItemText(nTimes,1,pDat1->name);
					m_ctrlSymbolCompare.SetItemText(nTimes,2,pDat1->Gppyjc);
					nTimes++;
				}
				if( nTimes == 20 ) m_ctrlSymbolCompare.Update(20);
			}
		}
		break;
	}
}

void CTaiCodeContradistinguishPage::OnAdd() 
{
	UpdateData(TRUE);
	//
	//Pop up a input dlg for  inputing a new content of a stock
	//
	CTaiStockInputDlg InputDlg;

	//
	//First set the stock type
	//
    if(m_RadioSh==0)
		InputDlg.SetStockType(TRUE);
	else
		InputDlg.SetStockType(FALSE);
	//
	//Run the dlg
	//
	if(InputDlg.DoModal()==IDOK) //if the user choosed OK to add a stock
	{
		if(MessageBox("你真的决定增加新的股票代码吗?","警告",MB_YESNO|MB_ICONQUESTION)==IDNO)
			return;
		//First add the new sotck information to the listctrl(the first pos)
		int nIndex=0;//m_ctrlSymbolCompare.GetItemCount();
		m_ctrlSymbolCompare.InsertItem (nIndex,  InputDlg.m_strStockCode);
		m_ctrlSymbolCompare.SetItemText(nIndex,1,InputDlg.m_strStockName);
		m_ctrlSymbolCompare.SetItemText(nIndex,2,InputDlg.m_strStockPyjc);

        //Change the listctrl's current selected item
		m_ctrlSymbolCompare.SetItemState(nIndex,LVIS_SELECTED,LVIS_SELECTED);

		//Reset other three edit box to the new one
		m_EditCode.SetSel(0,-1);
		m_EditCode.ReplaceSel(InputDlg.m_strStockCode);
		m_EditName.SetSel(0,-1);
		m_EditName.ReplaceSel(InputDlg.m_strStockName);
		m_EditPhonetic.SetSel(0,-1);
		m_EditPhonetic.ReplaceSel(InputDlg.m_strStockPyjc);

		AddOneStockInfo(InputDlg.m_strStockCode,InputDlg.m_strStockName,InputDlg.m_strStockPyjc);
		//Set the add flag
		m_bAdd=TRUE;
		SetModified();
	}
}

void CTaiCodeContradistinguishPage::OnDel() 
{
	POSITION pos = m_ctrlSymbolCompare.GetFirstSelectedItemPosition();
	if (pos == NULL)//judge if the user choose one stock item
	{
		AfxMessageBox("请首先选择须删除的股票项！",MB_ICONSTOP);
		return;
	}
	else
	{
		if( AfxMessageBox("真的要删除这支股票吗？",MB_ICONQUESTION|MB_YESNO)==IDNO )
			return;

        //Get the code information that will be deleted, 
		//of course this is served for deleting multi items
		//
		int nItem = m_ctrlSymbolCompare.GetNextSelectedItem(pos);
		CString strCode,strName,strHypy;
		//Get the stock code info
		strCode=m_ctrlSymbolCompare.GetItemText(nItem,0);
		//Get the stock Name
		strName=m_ctrlSymbolCompare.GetItemText( nItem, 1);
		//Get the stock pyjc
		strHypy=m_ctrlSymbolCompare.GetItemText( nItem, 2);
		if(!DeleteOneStockInfo(strCode,strName,strHypy))
		{
			MessageBox("删除失败","警告",MB_OK|MB_ICONEXCLAMATION);
			return;
		}
		m_ctrlSymbolCompare.DeleteItem(nItem);	
	}

	//Set the delete flag
	m_bDelete=TRUE;
	SetModified();
}

void CTaiCodeContradistinguishPage::OnModify() 
{
	//first judge if the user choosed one stock to be modified
	if( m_ctrlSymbolCompare.GetNextItem(-1,LVIS_SELECTED)<0 )
	{
		AfxMessageBox("请先选择须修改的股票项！",MB_ICONASTERISK);
		return;
	}

	//Enable the edit box to be modified
	m_EditCode.EnableWindow(FALSE);
	m_EditName.EnableWindow(TRUE);
	m_EditPhonetic.EnableWindow(TRUE);

	//set the active cursor to the first pos
	GotoDlgCtrl(GetDlgItem(IDC_NAME));

	//Set the modified flag
	m_bModify=TRUE;
	SetModified();	
}

void CTaiCodeContradistinguishPage::OnRadiosz() 
{
	//Changed the stocktype to Shen zhen

	//Reset all flag
	m_bAdd=FALSE;
	m_bDelete=FALSE;
	m_bModify=FALSE;

	((CEdit*)GetDlgItem(IDC_SYMBOL))->SetLimitText(4);  
	InitListCtrl();	
}

void CTaiCodeContradistinguishPage::OnRadiosh() 
{
	//Changed the stocktype to Shanghai
	
	//Reset all flag
	m_bAdd=FALSE;
	m_bDelete=FALSE;
	m_bModify=FALSE;

	((CEdit*)GetDlgItem(IDC_SYMBOL))->SetLimitText(6);  
	InitListCtrl();		
}

void CTaiCodeContradistinguishPage::OnClickSymbolcompare(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int Index=m_ctrlSymbolCompare.GetNextItem(-1,LVIS_SELECTED);
	if(Index<0)//judge if really choosed the item
	{
		*pResult = 0;
		return;
	}
    
	//judge if the user changed the choice to another item
	if(m_nOldSel!=Index)
	{
		m_nOldSel=Index;
		//
		//disable the edit box
		//
		m_EditCode.EnableWindow(FALSE);
		m_EditName.EnableWindow(FALSE);
		m_EditPhonetic.EnableWindow(FALSE);
		m_bModify=FALSE;
	}	
	
	//
	//Reset three edit box content
	//
	CString szSymbol,szName,szPhonetic;
	szSymbol=m_ctrlSymbolCompare.GetItemText(Index,0);
	m_EditCode.SetSel(0,-1);
	m_EditCode.ReplaceSel(szSymbol);
	szName  =m_ctrlSymbolCompare.GetItemText(Index,1);
	m_EditName.SetSel(0,-1);
	m_EditName.ReplaceSel(szName);
	szPhonetic  =m_ctrlSymbolCompare.GetItemText(Index,2);
	m_EditPhonetic.SetSel(0,-1);
	m_EditPhonetic.ReplaceSel(szPhonetic);

	//
	//Enable the two button
	//
	GetDlgItem(IDC_MODIFY)->EnableWindow(TRUE);
	GetDlgItem(IDC_DEL)->EnableWindow(TRUE);

	*pResult = 0;
}

void CTaiCodeContradistinguishPage::OnChangeName() 
{
	CString strStockHypy;

	//first judge if it's modify for the code
	if(!m_bModify)// if not modify the code
	{
		strStockHypy=m_ctrlSymbolCompare.GetItemText(m_nOldSel,2);
		//
		if(!strStockHypy.IsEmpty())//if the hypy is not empty
			return;
	}

	//
	//fellowing is for Modify button or the hypy is empty
	//
	CString strCode,strName;
	CTaiShanDoc* pDoc=GetDocument();
	//
	//Get the new stock pyjc according to the changed stockname and refresh the edit box
	//
	m_EditCode.GetWindowText(strCode);
	m_EditName.GetWindowText(strName);
	char *pName,strHypy[10];
	pName=strName.GetBuffer(strName.GetLength());
	pDoc->StockNameConvert(pName,strHypy);
	m_EditPhonetic.SetWindowText(strHypy);

	//
	//refresh the listctrl and modify the 
	//
	m_ctrlSymbolCompare.SetItemText(m_nOldSel,1,pName);
	m_ctrlSymbolCompare.SetItemText(m_nOldSel,2,strHypy);
	ModifyOneStockInfo(strCode,strName,strHypy);
}

//
//Change the self-defined ctrl's color
//
HBRUSH CTaiCodeContradistinguishPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if( nCtlColor == CTLCOLOR_EDIT 
		&&(	   pWnd->GetDlgCtrlID() == IDC_SYMBOL 
			|| pWnd->GetDlgCtrlID() == IDC_NAME
			|| pWnd->GetDlgCtrlID() == IDC_PHONETIC )
	   )
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(150,50,30));
		return (HBRUSH)m_SelfBrush.GetSafeHandle();
	}
	else
		return hbr;
}

void CTaiCodeContradistinguishPage::OnOK() 
{
	if( ((CPropertySheet*)GetParent())->GetActiveIndex( ) != 0 )
		return;

	SetModified(FALSE);	

	m_EditCode.EnableWindow(FALSE);
	m_EditName.EnableWindow(FALSE);
	m_EditPhonetic.EnableWindow(FALSE);

	UpdateData(TRUE);

	if(m_bAdd||m_bModify||m_bDelete)
	{
		CTaiShanDoc* pDoc=GetDocument();
		::SendMessage( pDoc->m_WsStock2000View->m_hWnd ,WM_USER_CHANGESTOCK,0,0);
	}
	CPropertyPage::OnOK();
}

CTaiShanDoc * CTaiCodeContradistinguishPage::GetDocument()
{
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	CTaiShanDoc* pDoc=(CTaiShanDoc*)pFrameWnd->GetActiveDocument();
	return pDoc;
}

//
//Add to the keyboard angel list array and the split data center
//
BOOL CTaiCodeContradistinguishPage::AddOneStockInfo(CString strStockCode, CString strStockName, CString strStockPyjc)
{
	CTaiShanDoc* pDoc=GetDocument();
	BOOL bResult=FALSE;
	pDoc->AddStockToKeyboard(strStockCode,strStockName,strStockPyjc);
    bResult=pDoc->m_sharesInformation.AddOneStockInfo(strStockCode,strStockName,strStockPyjc);
	return bResult;
}

//
//modify the keyboard angel list array and sjl data center
//
BOOL CTaiCodeContradistinguishPage::ModifyOneStockInfo(CString strStockCode, CString strStockName, CString strStockPyjc)
{
	CTaiShanDoc* pDoc=GetDocument();
	BOOL bResult=FALSE;
	pDoc->ModifyStockOfKeyboard(strStockCode,strStockName,strStockPyjc);
	bResult=pDoc->m_sharesInformation.MOdifyOneStockInfo(strStockCode,strStockName,strStockPyjc);
	return bResult;
}

//
//delete from the keyboard angel list array and the sjl data center
//
BOOL CTaiCodeContradistinguishPage::DeleteOneStockInfo(CString strStockCode, CString strStockName, CString strStockPyjc)
{
	CTaiShanDoc* pDoc=GetDocument();
	BOOL bResult=FALSE;
	pDoc->DeleteStockFromKeyboard(strStockCode,strStockName,strStockPyjc);
    bResult=pDoc->m_sharesInformation.DeleteOneStockInfo(strStockCode);
	return TRUE;
}

void CTaiCodeContradistinguishPage::StockTypeUpdate(CReportData  *Cdat)
{
	CTaiShanDoc* pDoc=GetDocument();

	//
	//First update the Shanghai stock content 
	//
	if(strlen(Cdat->id)==6|| strlen(Cdat->id)==4)
	{
		if ( Cdat->id[0]<'5' || Cdat->id[0]=='7' )	//Cuo Huo Zai Quan
		{
			Cdat->kind='7';
			if((Cdat->id[1]=='A') ||(Cdat->id[1]=='B' )||(Cdat->id[1]=='C'))//Zhi Shu
            {
				pDoc->m_lStockTypeCount[0]++;
			}
			else if(Cdat->id[0]=='7') //Pei Gu 
			{
				pDoc->m_lStockTypeCount[6]++; 
			}
			else
			{
				pDoc->m_lStockTypeCount[6]++;
			}
		}
		else //other Shanghai Stock
		{ 
			// shanghai stock
			if ( Cdat->id[0] < '9' ) // type A stock
			{
				Cdat->kind='1';
				pDoc->m_lStockTypeCount[1]++;
			}
			else  // type B stock
			{
				Cdat->kind='2';
				pDoc->m_lStockTypeCount[2]++;
			}
		}
	}

	//
	//update all ShenZhen stock
	//
	else 
	{ 
		if(Cdat->id[0]=='9'&&Cdat->id[1]=='9') //type B stock
		{
		   Cdat->kind='6';
		   pDoc->m_lStockTypeCount[3]++;
           return ;
		}
		if ( Cdat->id[0]=='0' ||  Cdat->id[0]=='4' )//type A stock
		{
			Cdat->kind='4';
			pDoc->m_lStockTypeCount[4]++;
		}
		else if(Cdat->id[0]=='2')//Zhi shu and Type B stock
        {
			if( Cdat->id[1]=='A'||Cdat->id[1]=='B'||Cdat->id[1]=='C' )//Zhi Shu 
			{
			   Cdat->kind='6';
			   pDoc->m_lStockTypeCount[3]++;
			}
			else//type B stock
			{
			   Cdat->kind='5';
			   pDoc->m_lStockTypeCount[5]++;
			}
		}
		else //Zai Quan and Pei Gu
		{
                if(Cdat->id[0]=='3'||Cdat->id[0]=='8')
				{
			        pDoc->m_lStockTypeCount[7]++;
				}
				else	
				{
			        pDoc->m_lStockTypeCount[7]++;
				}
				Cdat->kind='6';
		}
	}
	if ( Cdat->kind == '1' )              //Shanghai A
		 Cdat->kind=1;
	else if ( Cdat->kind == '2' )         //Shanghai B
		Cdat->kind=2; 
	else if ( Cdat->kind == '4' )         //Shenzhen A
		Cdat->kind=11;
	else if ( Cdat->kind == '5' )         //ZhenZhen B
		Cdat->kind=12;
	else if ( Cdat->kind == '6' )         //Zhenzhen Zai Quan
		Cdat->kind=13;
	else if ( Cdat->kind == '7' )         //Shanghai Zai Quan
		Cdat->kind=20;
}

void CTaiCodeContradistinguishPage::OnKeydownSymbolcompare(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pLVKeyDow->wVKey==VK_DOWN||pLVKeyDow->wVKey==VK_UP)
	{
		int Index=m_ctrlSymbolCompare.GetNextItem(-1,LVIS_SELECTED);
		if(pLVKeyDow->wVKey==VK_DOWN)
		{
			Index++;
			if(Index>m_ctrlSymbolCompare.GetItemCount())	Index=0;
		}
		else
		{
			Index--;
			if(Index<0)
				Index=m_ctrlSymbolCompare.GetItemCount()-1;
		}
		if(Index<0)//judge if really choosed the item
		{
			*pResult = 0;
			return;
		}
    
		//judge if the user changed the choice to another item
		if(m_nOldSel!=Index)
		{
			m_nOldSel=Index;
			//
			//disable the edit box
			//
			m_EditCode.EnableWindow(FALSE);
			m_EditName.EnableWindow(FALSE);
			m_EditPhonetic.EnableWindow(FALSE);
			m_bModify=FALSE;
		}	
		
		//
		//Reset three edit box content
		//
		CString szSymbol,szName,szPhonetic;
		szSymbol=m_ctrlSymbolCompare.GetItemText(Index,0);
		m_EditCode.SetSel(0,-1);
		m_EditCode.ReplaceSel(szSymbol);
		szName  =m_ctrlSymbolCompare.GetItemText(Index,1);
		m_EditName.SetSel(0,-1);
		m_EditName.ReplaceSel(szName);
		szPhonetic  =m_ctrlSymbolCompare.GetItemText(Index,2);
		m_EditPhonetic.SetSel(0,-1);
		m_EditPhonetic.ReplaceSel(szPhonetic);

		//
		//Enable the two button
		//
		GetDlgItem(IDC_MODIFY)->EnableWindow(TRUE);
		GetDlgItem(IDC_DEL)->EnableWindow(TRUE);
	}
	*pResult = 0;
}
