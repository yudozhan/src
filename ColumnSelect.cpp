// ColumnSelect.cpp : implementation file
//


#include "stdafx.h"
#include "WH.h"
#include "CTaiShanDoc.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"
#include "ColumnSelect.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColumnSelect dialog

CColumnSelect::CColumnSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CColumnSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColumnSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CColumnSelect::~CColumnSelect()
{
    POSITION pos=m_DynaTechnique.GetHeadPosition( );
	while(pos)
	{
		DynaTechniqueDef *mDynaIndexDef;
		mDynaIndexDef = m_DynaTechnique.GetNext( pos );
		delete mDynaIndexDef;
	}
    m_DynaTechnique.RemoveAll();
}
CColumnSelect::CColumnSelect(UINT nID,CString nStr,BOOL nDel,BOOL IsJS,int x,int y,CWnd* pParent )   // standard constructor
	: CDialog(CColumnSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColumnSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nPos_x=x;
	m_nPos_y=y;
	m_nSelectID=nID;
	m_sSelectStr=nStr;
	m_bIsTechnique=IsJS;
	m_bIsDelete=nDel;
	m_bIsFirst=TRUE;
	pView=(CTaiShanReportView * )pParent;
}


void CColumnSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColumnSelect)
	DDX_Control(pDX, IDC_TREE1, m_MyTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColumnSelect, CDialog)
	//{{AFX_MSG_MAP(CColumnSelect)
	ON_NOTIFY(NM_DBLCLK,IDC_TREE1, OnDblclk)
	ON_WM_CLOSE()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CColumnSelect message handlers
BOOL CColumnSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int i;

	CImageList img;
	img.Create(IDB_BITMAP12,16,0,RGB(192,192,192));
	m_MyTree.SetImageList(&img,TVSIL_NORMAL);
	img.Detach();

    CTaiShanDoc *pDoc = pView->GetDocument();
	int num=pDoc->m_formuar_index.GetSize();
	for(i=0;i<num;i++)
	{
		//Marenan 
		if(pDoc->m_formuar_index.ElementAt(i)->isOfen )
		{
			DynaTechniqueDef *mDynaIndexDef;
			mDynaIndexDef=new DynaTechniqueDef;
			mDynaIndexDef->m_nID=9000+i;
			strcpy(mDynaIndexDef->m_nString,pDoc->m_formuar_index.ElementAt(i)->name);  
			//strcpy(mDynaIndexDef->m_nExplain,pDoc->m_formuar_index.ElementAt(i)->explainBrief) ;
			m_DynaTechnique.AddTail(mDynaIndexDef);
		}
	}
	HTREEITEM hTreeItem;
	// TODO: Add extra initialization here
	TV_INSERTSTRUCT TreeCtrlItem;
	HTREEITEM TreeCtrlItemCurr;
	BOOL IsFind=FALSE;

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE  ;
	TreeCtrlItem.item.pszText = "��Ʊ��������";
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	TreeCtrlItem.item.lParam =100;
	HTREEITEM hTreeItem1 = this->m_MyTree.InsertItem(&TreeCtrlItem);
	UINT ColumnId=IDS_STOCKNAME;

    for(i=ID_STOCKNAME;i<ID_ZGB;i++ )
	{
		CString str;
		str.LoadString(ColumnId);
		TreeCtrlItem.hParent = hTreeItem1;
		TreeCtrlItem.item.pszText = str.GetBuffer(0) ;
		TreeCtrlItem.item.lParam = i;
        TreeCtrlItem.item.iImage=1;
	    TreeCtrlItem.item.iSelectedImage=2;
		HTREEITEM hItem=this->m_MyTree.InsertItem(&TreeCtrlItem);
		if(m_nSelectID==i)
		{
	       IsFind=TRUE;
           hTreeItem=hTreeItem1; 
		   TreeCtrlItemCurr=hItem;
		}
		ColumnId++;
	}
	
	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.item.pszText = "��Ʊ����ͳ��";
	TreeCtrlItem.item.lParam =200;
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	HTREEITEM hTreeItem2 = this->m_MyTree.InsertItem(&TreeCtrlItem);

	ColumnId=IDS_ZJLS;

    for(i=ID_ZJLS;i<ID_DELITEM;i++ )
	{
		CString str; 
		str.LoadString(ColumnId);
		TreeCtrlItem.hParent = hTreeItem2;
		TreeCtrlItem.item.pszText = str.GetBuffer(0) ;
		TreeCtrlItem.item.lParam = i;
        TreeCtrlItem.item.iImage=1;
	    TreeCtrlItem.item.iSelectedImage=2;
		HTREEITEM hItem=this->m_MyTree.InsertItem(&TreeCtrlItem);
		if(m_nSelectID==i)
		{
	       IsFind=TRUE;
           hTreeItem=hTreeItem2; 
		   TreeCtrlItemCurr=hItem;
		}
		ColumnId++;
	}
	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.item.pszText = "��Ʊ��������";
	TreeCtrlItem.item.lParam =400;
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	HTREEITEM hTreeItem4 = this->m_MyTree.InsertItem(&TreeCtrlItem);

	ColumnId=IDS_ZZC;

    for(i=ID_ZZC;i<ID_MGJZC;i++ )
	{
		CString str;
		str.LoadString(ColumnId);
		TreeCtrlItem.hParent = hTreeItem4;
		TreeCtrlItem.item.pszText = str.GetBuffer(0) ;
		TreeCtrlItem.item.lParam = i;
        TreeCtrlItem.item.iImage=1;
	    TreeCtrlItem.item.iSelectedImage=2;
		HTREEITEM hItem=this->m_MyTree.InsertItem(&TreeCtrlItem);
		if(m_nSelectID==i)
		{
	       IsFind=TRUE;
           hTreeItem=hTreeItem4; 
		   TreeCtrlItemCurr=hItem;
		}
		ColumnId++;
	}

	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.item.pszText = "��Ʊ�ɱ�����";
	TreeCtrlItem.item.lParam =300;
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	HTREEITEM hTreeItem3 = this->m_MyTree.InsertItem(&TreeCtrlItem);

	ColumnId=IDS_ZGB;

    for(i=ID_ZGB;i<ID_ZZC;i++ )
	{
		CString str;
		str.LoadString(ColumnId);
		TreeCtrlItem.hParent = hTreeItem3;
		TreeCtrlItem.item.pszText = str.GetBuffer(0) ;
		TreeCtrlItem.item.lParam = i;
        TreeCtrlItem.item.iImage=1;
	    TreeCtrlItem.item.iSelectedImage=2;
		HTREEITEM hItem=this->m_MyTree.InsertItem(&TreeCtrlItem);
		if(m_nSelectID==i)
		{
	       IsFind=TRUE;
           hTreeItem=hTreeItem3; 
		   TreeCtrlItemCurr=hItem;
		}
		ColumnId++;
	}
	TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.item.pszText = "����ָ��";
	TreeCtrlItem.item.lParam =500;
    TreeCtrlItem.item.iImage=0;
	TreeCtrlItem.item.iSelectedImage=0;
	HTREEITEM hTreeItem5 = this->m_MyTree.InsertItem(&TreeCtrlItem);

    POSITION pos=m_DynaTechnique.GetHeadPosition( );
	while(pos)
	{
		CString str;
		DynaTechniqueDef *mDynaIndexDef;
		mDynaIndexDef = m_DynaTechnique.GetNext( pos );

		str=mDynaIndexDef->m_nString ;
		TreeCtrlItem.hParent = hTreeItem5;
		TreeCtrlItem.item.pszText = str.GetBuffer(0) ;
		TreeCtrlItem.item.lParam = mDynaIndexDef->m_nID ;
        TreeCtrlItem.item.iImage=1;
	    TreeCtrlItem.item.iSelectedImage=2;
		HTREEITEM hItem=this->m_MyTree.InsertItem(&TreeCtrlItem);
		if(strcmp(m_sSelectStr,mDynaIndexDef->m_nString)==0)
		{
	       IsFind=TRUE;
           hTreeItem=hTreeItem5; 
		   TreeCtrlItemCurr=hItem;
		}
	}


	if(m_bIsDelete)
	{
		TreeCtrlItem.hParent = TVI_ROOT;
		TreeCtrlItem.item.pszText = "ɾ��һ��";
		TreeCtrlItem.item.lParam =5064;
        TreeCtrlItem.item.iImage=0;
	    TreeCtrlItem.item.iSelectedImage=0;
		HTREEITEM hTreeItem5 = this->m_MyTree.InsertItem(&TreeCtrlItem);
	}
	else
	{
		TreeCtrlItem.hParent = TVI_ROOT;
		TreeCtrlItem.item.pszText = "�ָ�������";
		TreeCtrlItem.item.lParam =5065;
        TreeCtrlItem.item.iImage=0;
	    TreeCtrlItem.item.iSelectedImage=0;
		HTREEITEM hTreeItem5 = this->m_MyTree.InsertItem(&TreeCtrlItem);
	}

	CRect rc;
	this->GetWindowRect(&rc);
	this->MoveWindow(m_nPos_x,m_nPos_y,rc.Width(),rc.Height());   
	if(IsFind)
	{
		if(m_bIsTechnique)
		{
			TreeCtrlItem.hParent = TreeCtrlItemCurr;
			TreeCtrlItem.item.pszText = "��������" ;
			TreeCtrlItem.item.lParam = 98888 ;
			TreeCtrlItem.item.iImage=3;
			TreeCtrlItem.item.iSelectedImage=3;
			HTREEITEM hItem1=this->m_MyTree.InsertItem(&TreeCtrlItem);

			TreeCtrlItem.hParent = TreeCtrlItemCurr;
			TreeCtrlItem.item.pszText = "���¼���" ;
			TreeCtrlItem.item.lParam = 98889 ;
			TreeCtrlItem.item.iImage=4;
			TreeCtrlItem.item.iSelectedImage=4;
			HTREEITEM hItem2=this->m_MyTree.InsertItem(&TreeCtrlItem);
		    this->m_MyTree.Expand(TreeCtrlItemCurr,TVE_EXPAND);

		}
		this->m_MyTree.Expand(hTreeItem,TVE_EXPAND);
		this->m_MyTree.SelectItem(TreeCtrlItemCurr); 
		this->m_MyTree.EnsureVisible(TreeCtrlItemCurr);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
              // EXCEPTION: OCX Property Pages should return FALSE
}
void CColumnSelect::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class	delete this;
	CDialog::PostNcDestroy();
}

int CColumnSelect::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::DoModal();
}
void CColumnSelect::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
    DestroyWindow();	
}
void CColumnSelect::OnOK() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnOK();
    DestroyWindow();	
}
BOOL CColumnSelect::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	int cc=GetKeyState(VK_CONTROL)&0x7f;
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
	    HTREEITEM hTreeItem;
        hTreeItem=this->m_MyTree.GetSelectedItem();  		 
		UINT nId=this->m_MyTree.GetItemData(hTreeItem);  
		if(nId==100||nId==200||nId==300||nId==400||nId==500)
  	       this->m_MyTree.Expand(hTreeItem,TVE_EXPAND);
		else
		{
			if(nId<9000)
			{
              ::SendMessage(pView->m_hWnd,WM_USER_COLUMNSELECT ,nId,0);       
			}
			else
			{
				if(nId==98888||nId==98889)
				{
				   ::SendMessage(pView->m_hWnd, WM_USER_JISHUCOLUMNSELECT ,nId,0);       
				   return TRUE;
				}
				else
				{
					POSITION pos=m_DynaTechnique.GetHeadPosition( );
					while(pos)
					{
						DynaTechniqueDef *mDynaIndexDef;
						mDynaIndexDef = m_DynaTechnique.GetNext( pos );
						if(mDynaIndexDef->m_nID ==nId)
						{
							::SendMessage(pView->m_hWnd, WM_USER_JISHUCOLUMNSELECT ,98887,(long )&mDynaIndexDef->m_nString);       
							return TRUE;
						}
					}
				}
			}
		}
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CColumnSelect::OnDblclk(NMHDR* wParam, LRESULT *lParam )
{
    HTREEITEM hTreeItem;
    hTreeItem=this->m_MyTree.GetSelectedItem();  		 
	UINT nId=this->m_MyTree.GetItemData(hTreeItem);  
	if(nId==100||nId==200||nId==300||nId==400||nId==500)
 	      return;
	else
	{
		if(nId<9000)
          ::SendMessage(pView->m_hWnd,WM_USER_COLUMNSELECT ,nId,0);       
		else
		{
			if(nId==98888||nId==98889)
			{
               ::SendMessage(pView->m_hWnd, WM_USER_JISHUCOLUMNSELECT ,nId,0);       
			   return;
			}
			else
			{
				POSITION pos=m_DynaTechnique.GetHeadPosition( );
				while(pos)
				{
					DynaTechniqueDef *mDynaIndexDef;
					mDynaIndexDef = m_DynaTechnique.GetNext( pos );
					if(mDynaIndexDef->m_nID ==nId)
					{
						::SendMessage(pView->m_hWnd, WM_USER_JISHUCOLUMNSELECT ,98887,(long )&mDynaIndexDef->m_nString);       
						return;
					}
				}
			}
		}
    }
	return ;
}


void CColumnSelect::ShowMenu()
{
	CTaiShanDoc *pDoc = CMainFrame::m_pDoc ;
	int num=pDoc->m_formuar_index.GetSize();
	int i;
	for(i=0;i<num;i++)
	{
		//Marenan 
		if(pDoc->m_formuar_index.ElementAt(i)->isOfen )
		{
			DynaTechniqueDef *mDynaIndexDef;
			mDynaIndexDef=new DynaTechniqueDef;
			mDynaIndexDef->m_nID=9000+i;
			strcpy(mDynaIndexDef->m_nString,pDoc->m_formuar_index.ElementAt(i)->name);  
			m_DynaTechnique.AddTail(mDynaIndexDef);
		}
	}
	BOOL IsFind=FALSE;

	{
	CMenu  mnu;
	CPoint posMouse;
	::GetCursorPos(&posMouse);

	mnu.CreatePopupMenu( );


	CMenu  menu;
	menu.CreatePopupMenu( );
	mnu.AppendMenu(  MF_POPUP | MF_STRING, (UINT)menu.m_hMenu, "��Ʊ��������");
	UINT ColumnId=IDS_STOCKNAME;
    for(i=ID_STOCKNAME;i<ID_ZGB;i++ )
	{
		CString str;
		if(i<ID_BUYP1)
			str.LoadString(ColumnId);
		else
			str = g_sBuySell[i-ID_BUYP1];
		if(m_nSelectID==i)
			menu.AppendMenu (MF_STRING | MF_ENABLED| MF_CHECKED,i-ID_STOCKNAME+200,str);
		else
			menu.AppendMenu (MF_STRING | MF_ENABLED,i-ID_STOCKNAME+200,str);
		ColumnId++;
	}

//2222222222222
	CMenu  menu2;
	menu2.CreatePopupMenu( );
	mnu.AppendMenu(  MF_POPUP | MF_STRING, (UINT)menu2.m_hMenu, "��Ʊ����ͳ��");

	ColumnId=IDS_ZJLS;

    for(i=ID_ZJLS;i<ID_DELITEM;i++ )
	{
		CString str;
		str.LoadString(ColumnId);
		if(m_nSelectID==i)
			menu2.AppendMenu (MF_STRING | MF_ENABLED| MF_CHECKED,i-ID_ZJLS+300,str);
		else
			menu2.AppendMenu (MF_STRING | MF_ENABLED,i-ID_ZJLS+300,str);
		ColumnId++;
	}

//333333333333333
	CMenu  menu3;
	menu3.CreatePopupMenu( );
	mnu.AppendMenu(  MF_POPUP | MF_STRING, (UINT)menu3.m_hMenu, "��Ʊ��������");

	ColumnId=IDS_ZZC;

    for(i=ID_ZZC;i<ID_MGJZC;i++ )
	{
		CString str;
		str.LoadString(ColumnId);
		if(m_nSelectID==i)
			menu3.AppendMenu (MF_STRING | MF_ENABLED| MF_CHECKED,i-ID_ZZC+400,str);
		else
			menu3.AppendMenu (MF_STRING | MF_ENABLED,i-ID_ZZC+400,str);
		ColumnId++;
	}

///444444444444444
	CMenu  menu4;
	menu4.CreatePopupMenu( );
	mnu.AppendMenu(  MF_POPUP | MF_STRING, (UINT)menu4.m_hMenu, "��Ʊ�ɱ�����");

	ColumnId=IDS_ZGB;

    for(i=ID_ZGB;i<ID_ZZC;i++ )
	{
		CString str;
		str.LoadString(ColumnId);
		if(m_nSelectID==i)
			menu4.AppendMenu (MF_STRING | MF_ENABLED| MF_CHECKED,i-ID_ZGB+500,str);
		else
			menu4.AppendMenu (MF_STRING | MF_ENABLED,i-ID_ZGB+500,str);
		ColumnId++;
	}

//555555555555555
	CMenu  menu5;
	menu5.CreatePopupMenu( );
	mnu.AppendMenu(  MF_POPUP | MF_STRING, (UINT)menu5.m_hMenu, "����ָ��");

	ColumnId=IDS_ZGB;

	CTaiShanDoc *pDoc = CMainFrame::m_pDoc ;
	int num=pDoc->m_formuar_index.GetSize();
	for(i=0;i<num;i++)
	{
		//Marenan 
		if(pDoc->m_formuar_index.ElementAt(i)->isOfen )
		{
			if(strcmp(m_sSelectStr,pDoc->m_formuar_index.ElementAt(i)->name)==0)
				menu5.AppendMenu (MF_STRING | MF_ENABLED| MF_CHECKED,i+600,pDoc->m_formuar_index.ElementAt(i)->name);
			else
				menu5.AppendMenu (MF_STRING | MF_ENABLED,i+600,pDoc->m_formuar_index.ElementAt(i)->name);
		}
	}

//66666666666666666
	if(m_bIsDelete)
	{
		mnu.AppendMenu (MF_STRING | MF_ENABLED,180,"ɾ��һ��");
	}
	else
	{
		mnu.AppendMenu (MF_STRING | MF_ENABLED,181,"�ָ�������");
	}

	mnu.TrackPopupMenu (TPM_RIGHTALIGN | TPM_RIGHTBUTTON  ,
			posMouse.x,posMouse.y,this->pView , NULL);


	}


}

int CColumnSelect::GetId(int nIn)
{
	int n;
	if(nIn == 180)
	{
		n = ID_DELITEM;
	}
	else if(nIn == 181)
	{
		n = ID_DEFAULT;
	}
	else
	{
		int n2 = nIn/100;
		switch(n2)
		{
		case 2:
			n=nIn-200+ID_STOCKNAME;
			break;
		case 3:
			n=nIn-300+ID_ZJLS;
			break;
		case 4:
			n=nIn-400+ID_ZZC;
			break;
		case 5:
			n=nIn-500+ID_ZGB;
			break;
		case 6:
		case 7:
		case 8:
			{
				n=9000;
				CTaiShanDoc *pDoc = CMainFrame::m_pDoc ;
				int num=pDoc->m_formuar_index.GetSize();
				for(int i=0;i<num;i++)
				{
					//Marenan 
					if(pDoc->m_formuar_index.ElementAt(i)->isOfen )
					{
						if(i+600 == nIn)
						{
							n=i+9000;
							break;
						}
					}
				}

			}
			break;
		default:
			n=0;
			break;
		}
	}
	return n;


}
