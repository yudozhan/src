// CTaiKlineTreeCtl.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineTreeCtl.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "DIAEDITZJXT.h"
#include "CTaiKlineDlgEditIndecator.h"
#include "CTaiKlineDlgEditScreenStock.h"
#include "DIAGSGLMM.h"
#include "CTaiKlineSetNameTreeGroup.h"
#include "CTaiShanKlineShowView.h"
#include "KEYBRODEANGEL.h"
#include "CTaiScreen.h"
#include "CTaiScreenTest.h"
#include "CTaiScreensIndicate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineTreeCtl
static const CString g_chAllIndex = "���м���ָ��";
static CString g_strFileDialogOutPath = "";

CTaiKlineTreeCtl::CTaiKlineTreeCtl()
{
	m_bShowFromMouse = false;
	m_hAllIndex=0;
	m_bNewGroup			=true;
	m_bDeleteGroup		=true;
	m_bModifyGroupName	=true;
	m_bDeleteEquation	=true;
	m_bNewEquation		=true;
	m_bModifyEquation	=true;
	m_bRefreshEquation  =false;

	m_bScreenStock			=false;
	m_bScreenStockTest		=false;
	m_bScreenStockIndicator =false;
	m_bShowIndicator		=false;
	m_bShowKlineGroup		=false;
}

CTaiKlineTreeCtl::~CTaiKlineTreeCtl()
{
}


BEGIN_MESSAGE_MAP(CTaiKlineTreeCtl, CTaiKlineColorTreeCtrl)
	//{{AFX_MSG_MAP(CTaiKlineTreeCtl)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineTreeCtl message handlers
void CTaiKlineTreeCtl::OnDestroy() 
{
	CTaiKlineColorTreeCtrl::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

#define RGBSUB RGB(20,0,100)
#define RGBEND RGB(200,0,0)
#define RGBEND2 RGB(0,0,0)

void CTaiKlineTreeCtl::InsertEqutionTree( int nKind,bool bExtend ,int nInsertKind)
{
	CFormularArray *pList = NULL;

	int i;
	for(i = 0;i<m_nKindArray.GetSize();i++)
	{
		if(m_nKindArray[i] == nKind)
			break;
	}
	if(i == m_nKindArray.GetSize())
		m_nKindArray.Add(nKind);

//	nInsertKind= 0;

	CString s;
	switch(nKind)
	{
	case 0 :
		pList = &CMainFrame::m_pDoc->m_formuar_index;  
		s = "ָ�깫ʽ";
		break;
	case 1 :
		pList = &CMainFrame::m_pDoc->m_formuar_choose;  
		s = "����ѡ�ɹ�ʽ";
		break;
	case 2 :
		pList = &CMainFrame::m_pDoc->m_formuar_kline;  
		s = "��̬��Ϲ�ʽ";
		break;
	case 3 :
		pList = &CMainFrame::m_pDoc->m_formuar_master;  
		s = "����ϵͳ��ʽ";
		break;
	default :
		InsertEqutionTreeOther(  nKind,bExtend);
		return;
	}
//	m_strTileArray.Add (s);


	int nImageKind[5]={4,3,5,0,0};
	SetImageList(CMainFrame::m_pDoc->m_imagelist ,TVSIL_NORMAL);

	HTREEITEM hChapter  = InsertItem(s,17,17,TVI_ROOT,TVI_LAST );//17:
	LOGFONT logfont;
	CFont *pFont = GetFont();
	pFont->GetLogFont( &logfont );
	logfont.lfWeight = FW_BOLD;//FW_REGULAR
	SetItemFont(hChapter, logfont );
	SetItemColor( hChapter, RGB(100,0,20));

	if(! SetItemData( hChapter, (DWORD) (-(nKind+1) )))
		ASSERT(FALSE);

	int nOutV=(int)GetItemData( hChapter);

	int nSubKind = CFormularContent::m_strArrayKind[nKind].GetSize ();

	if(nSubKind == 0)
	{
		CString s= "����";
		CFormularContent::m_strArrayKind[nKind].Add(s);
		nSubKind++;
	}
/*	if(nKind == 2)
		for(int j = 0;j<nSubKind;j++)
			CFormularContent::m_strArrayKind[nKind][j] = "01 K����Ϲ�ʽ";
*/
	//insert often,ALL
	//nInsertKind ==2 OFTEN;nInsertKind == 1 ALL
	logfont.lfWeight = FW_REGULAR;
	if(nInsertKind >0 )
	{
		HTREEITEM hSub = hChapter;//InsertItem("���ù�ʽ",16,16,hChapter);
//		if(! SetItemData( hSub, (DWORD) (0 )))
//			ASSERT(FALSE);
		for(int j=0;j<pList->GetSize ();j++)
		{
			CFormularContent * pIndex= pList->GetAt(j);
			//Marenan 
			if(pIndex->isOfen ==TRUE || nInsertKind==1)
			{
//				HTREEITEM hEnd = InsertItem(pIndex->name + " (" +pIndex->subKindIndex +") " + pIndex->subKindIndexTime+" " + pIndex->explainBrief ,nImageKind[nKind],nImageKind[nKind],hSub);
				HTREEITEM hEnd = InsertItem(pIndex->name + " (" +pIndex->subKindIndex +") " + pIndex->explainBrief ,nImageKind[nKind],nImageKind[nKind],hSub);
				SetItemFont(hEnd, logfont );
				if(pIndex->isOfen ==TRUE) SetItemColor( hEnd, RGBEND);
				else SetItemColor( hEnd, RGBEND2);
				if(! SetItemData( hEnd, (DWORD) pIndex ))
						ASSERT(FALSE);
			}
		}
		return ;
	}
	//end
	//insert all
	//insert
	i=0;
	int j=0;
	if(nKind ==0)
	{
/*		HTREEITEM hSub = InsertItem(g_chAllIndex,16,16,hChapter);
		SetItemColor( hSub, RGBSUB);
		if(! SetItemData( hSub, (DWORD) (0 )))
			ASSERT(FALSE);
		m_hAllIndex = hSub;

		for(j=0;j<pList->GetSize ();j++)
		{
			CFormularContent * pIndex= pList->GetAt(j);
			HTREEITEM hEnd = InsertItem(pIndex->name + " " +pIndex->subKindIndexTime+" " + pIndex->explainBrief ,nImageKind[nKind],nImageKind[nKind],hSub);
			if(pIndex->isOfen )	SetItemColor( hEnd, RGBEND);
			if(! SetItemData( hEnd, (DWORD) pIndex ))
					ASSERT(FALSE);
		}*/
//		SortChildren(  hSub );

	}
	HTREEITEM* hSub = new HTREEITEM[nSubKind+1];
	for(i=0;i<nSubKind;i++)
	{
		hSub[i] = InsertItem(CFormularContent::m_strArrayKind[nKind][i],16,16,hChapter);
		SetItemFont(hSub[i], logfont );
		SetItemColor( hSub[i], RGBSUB);
		if(! SetItemData( hSub[i], (DWORD) (0 )))
			ASSERT(FALSE);
	}
	for(j=0;j<pList->GetSize ();j++)
	{
		CFormularContent * pIndex= pList->GetAt(j);
		if(pIndex->subKindIndex == "")
			pIndex->subKindIndex = "����";
//		if(nKind ==2)
//			pIndex->subKindIndex = "01 K����Ϲ�ʽ";

		//add
		for(i=0;i<nSubKind;i++)
		{
			if(CFormularContent::m_strArrayKind[nKind][i] == pIndex->subKindIndex)
				break;
		}
		if(i>=nSubKind) i=nSubKind-1; 

		CString s ="";
//		if(nKind ==0)
//			s = pIndex->subKindIndexTime+" ";

		HTREEITEM hEnd = InsertItem(pIndex->name + " " +s+ pIndex->explainBrief ,nImageKind[nKind],nImageKind[nKind],hSub[i]);
		SetItemFont(hEnd, logfont );
		if(pIndex->isOfen )	SetItemColor( hEnd, RGBEND);
		else SetItemColor( hEnd, RGBEND2);
		if(! SetItemData( hEnd, (DWORD) pIndex ))
				ASSERT(FALSE);
	}
	delete [] hSub;
	//end
	if(bExtend ==true)
		Expand(hChapter,TVE_EXPAND);
}

//

bool CTaiKlineTreeCtl::GetCurrentItemInfo(int &nKindRoot, int &nKindItem, CFormularContent *&pIndex)//
//nKindRoot is the kind of root,nKindItem is the kind of current item(0 is sub group,-1 is invalid) ,pIndex is the pointer of Jishu(if pIndex=NULL,it is invalid)
{
	nKindRoot = 0;
	nKindItem = -1;
	pIndex = NULL;

	HTREEITEM hItem = GetSelectedItem();

	HTREEITEM hRoot = hItem;//?
	nKindRoot =(int) GetItemData(  hRoot ) ;
	if(nKindRoot>=0 || nKindRoot<-100)
	{
		while(true)
		{
			hRoot=GetParentItem(hRoot);
			if(hRoot == NULL)
				break;
			nKindRoot =(int) GetItemData(  hRoot ) ;
			if(nKindRoot<0&& nKindRoot>-100)
				break;
		}
	}

	int nData = (int) GetItemData(  hItem ) ;
	if(nData ==0)
		nKindItem = 0;
	else if(!ItemHasChildren(hItem) && nData>0 && (nKindRoot>=-4&&nKindRoot<=-1 ||nKindRoot == -6))
		pIndex =( CFormularContent *)nData;
	return true;
}

bool CTaiKlineTreeCtl::CreateNewGroup()
{
	CString sName = "����";
	//display dialog
	CTaiKlineSetNameTreeGroup setName;
	setName.m_bShowFromMouse = m_bShowFromMouse ;
	if(setName.DoModal () == IDCANCEL)	
		return false;

	//end
	sName = setName.m_sName ;

	HTREEITEM hItem = GetSelectedItem();
	//to detect if it has a same group name;
	if(FindGroupQiTa(hItem, sName)!=NULL)
	{
		AfxMessageBox("�������Ѿ�����,����������!");
		return false;
	}
	//end

	//add kind name
	int nKindRoot = GetItemData(hItem);
	if(nKindRoot<=-1 && nKindRoot>=-3)
		CFormularContent::AddKindName(sName  , -nKindRoot-1);

	HTREEITEM hSub = InsertItem(sName,16,16,hItem);
	if(! SetItemData( hSub, (DWORD) (0 )))
		ASSERT(FALSE);
	SortChildren(  hItem );
	return true;
}

bool CTaiKlineTreeCtl::DeleteGroup()
{
	if(AfxMessageBox("�Ƿ����ɾ���˹�ʽ�鼰���ڸ�������й�ʽ��",MB_YESNO )==IDYES)
	{
	HTREEITEM hItem = GetSelectedItem();
	//delete child item
	//LVNI_BELOW   
	int nRtn = 1;
	if(ItemHasChildren( hItem ))
	{
	   HTREEITEM hNextItem;
	   HTREEITEM hChildItem = GetChildItem(hItem);

	   while (hChildItem != NULL && nRtn!=2)
	   {
		  hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
		  if(hChildItem!=NULL)
		  {
				nRtn = DeleteEquation(hChildItem);
		  }
				
		  
		  //delete indicator
		  hChildItem = hNextItem;
	   }
	}

	if(nRtn ==2) return false;
	CString s = GetItemText(hItem);
	if(s == "����") return true;
	HTREEITEM hItemP = this->GetParentItem (hItem);
	//add kind name
	int nKindRoot = GetItemData(hItemP);
	if(nKindRoot<=-1 && nKindRoot>=-3)
	{
		CFormularContent::DeleteKindName(s, -nKindRoot-1);
	}
	DeleteItem( hItem );
	return true;
	}
	return false;
}

bool CTaiKlineTreeCtl::ModifyGroupName()
{
	HTREEITEM hItem = GetSelectedItem();
	int nData = (int) GetItemData(  hItem ) ;
	ASSERT(nData == 0);
	CString sName = GetItemText(  hItem ) ;
	//display dialog
	CTaiKlineSetNameTreeGroup setName;
	setName.m_bShowFromMouse = m_bShowFromMouse ;
	setName.m_sName = sName;
	CString sOld = sName;
	if(setName.DoModal () == IDCANCEL)	
		return false;
//	CEdit* pEdit = EditLabel(  hItem );

	HTREEITEM hItemP = this->GetParentItem (hItem);
	sName = setName.m_sName ;
	if(FindGroupQiTa(hItemP, sName)!=NULL)
	{
		AfxMessageBox("�������Ѿ�����,����������!");
		return false;
	}

	this->SetItemText (hItem,sName);

	//add kind name
	int nKindRoot = GetItemData(hItemP);
	if(nKindRoot<=-1 && nKindRoot>=-3)
	{
		CFormularContent::DeleteKindName(sOld, -nKindRoot-1);
		CFormularContent::AddKindName(sName  , -nKindRoot-1);
	}

	//�ı��й�ָ��ķ�������
	if (ItemHasChildren(hItem))
	{
		HTREEITEM hNextItem;
	    HTREEITEM hChildItem =GetChildItem(hItem);
	    while (hChildItem != NULL)   
		{
		  hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
		  int dwData = (int)GetItemData(hChildItem); 
		  CFormularContent* pJi = GetLJishuFromInt(dwData);
		  if(pJi!=NULL)
		  {
			  pJi->subKindIndex = sName;
		  }
		  
		  hChildItem = hNextItem;   
		}
	}

	SortChildren(  hItemP );
	
	return true;
}

bool CTaiKlineTreeCtl::CreateNewEquation()
{
	//new equation
	bool bSucc = false;
	CString sName = "";
	CFormularContent * pJishu = NULL;
	CFormularContent * pJishuCurrent = NULL;
	//display dialog

	int nKindRoot,  nKindItem;
	GetCurrentItemInfo(nKindRoot, nKindItem, pJishuCurrent);//
	HTREEITEM hItem = GetSelectedItem();

	ASSERT(nKindRoot>=-4&&nKindRoot<=-1);

	CString *strP = NULL;
	CString s = GetItemText(hItem);
	if(nKindItem == 0)
		strP = &s;
	if(pJishuCurrent)
		strP = &(pJishuCurrent->subKindIndex) ;
	pJishu = ShowEditIndicatorDlg( NULL, -1, -nKindRoot-1,strP);
	//end
	if(pJishu == NULL)
		return false;
//	AfxMessageBox("������ʽ�ɹ���");
	if(pJishu->subKindIndex == "")
		pJishu->subKindIndex = "����";

	//add kind name
	pJishu->AddKindName(pJishu->subKindIndex  , -nKindRoot-1);

	hItem = GetSelectedItem();//suspose sub
	if(hItem == NULL)
		return false;

	//INSERT NOW
	if(pJishuCurrent != NULL)//end item
		hItem = GetParentItem(hItem);
	else if(nKindItem !=0)//root
		hItem = FindGroupQiTa(hItem);
	//

	if(hItem == NULL ||-nKindRoot-1<0 ||-nKindRoot-1>4)
		return false;

	int nImageKind[5]={4,3,5,0,0};
	sName=pJishu ->name +" "+pJishu ->explainBrief ;//CMainFrame::m_pDoc->m_formuar_index.GetAt(diaeditzbgs.addpos)->name;
	HTREEITEM hSub = InsertItem(sName,nImageKind[-nKindRoot-1] ,nImageKind[-nKindRoot-1] ,hItem);
	if(hSub == NULL)
		return false;

	if(! SetItemData( hSub, (DWORD) (pJishu )))
		ASSERT(FALSE);
	SortChildren(  hItem );

/*	if(-nKindRoot-1 == 0 )//add to ���м���ָ�� group
	{//m_hAllIndex
//		HTREEITEM hUse = m_hAllIndex;
		if( hItem==m_hAllIndex)
		{
			HTREEITEM hParent = GetParentItem(m_hAllIndex);
			if (hParent != NULL)
			{
				HTREEITEM hNextItem;
				HTREEITEM hChildItem = GetChildItem(hParent);
			   
				while (hChildItem != NULL)   
				{
					hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
					CString s = GetItemText(hChildItem);
					if(s == "����")
					{
						hUse = hChildItem;
						break;
					}
					hChildItem = hNextItem;  
				}
			}
		}
		HTREEITEM hEnd = InsertItem(pJishu->name + " " + pJishu->explainBrief ,nImageKind[-nKindRoot-1],nImageKind[-nKindRoot-1],hUse);
		if(! SetItemData( hEnd, (DWORD) pJishu ))
				ASSERT(FALSE);
//		SortChildren(  hUse );
	}*/
	return true;

}
int CTaiKlineTreeCtl::DeleteEquation(HTREEITEM hItem)
{
	bool bNULL = false;
	if(hItem == NULL)
	{
		bNULL = true;
		hItem = GetSelectedItem();
	}
	int nData = (int) GetItemData(  hItem ) ;
	ASSERT(nData>0);

	//delete equation
	CFormularContent * pJishu = NULL;
	CFormularContent * pJishuCurrent = NULL;
	int nKindRoot,  nKindItem;
	GetCurrentItemInfo(nKindRoot, nKindItem, pJishuCurrent);//
	if(bNULL == false)
	{
		pJishuCurrent = (CFormularContent * )nData;
	}
	ASSERT(nKindRoot>=-4&&nKindRoot<=-1);
	if(pJishuCurrent == NULL)
		return 0;
	if(pJishuCurrent->isSystem )
	{
		AfxMessageBox("����ɾ��ϵͳָ�꣡");
		return 2;
	}

	if(bNULL == true)
		if(AfxMessageBox("�Ƿ����ɾ����ʽ��",MB_YESNO) != IDYES)
			return 0;
	//remove from array in document
	int nFoot = GetFoot(-nKindRoot-1,pJishuCurrent,true);
	ASSERT(nFoot == -1);

	//end

//	if(-nKindRoot-1 != 0)
	DeleteItem( hItem );

/*
	if(-nKindRoot-1 == 0 && m_hAllIndex!=0 )//
	{//m_hAllIndex
		HTREEITEM hParent = GetParentItem(m_hAllIndex);
		if(ItemHasChildren(hParent))
		{
			HTREEITEM hNext;
			HTREEITEM hChild = GetChildItem(hParent);
			while (hChild != NULL)
			{
				hNext = GetNextItem(hChild, TVGN_NEXT);
				if (ItemHasChildren(hChild))
				{   
					HTREEITEM hNextItem;
					HTREEITEM hChildItem = GetChildItem(hChild);
				   
					while (hChildItem != NULL)   
					{
						hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
						CFormularContent * pJ = (CFormularContent * )GetItemData(hChildItem);
						if(pJ == pJishuCurrent)
						{
							DeleteItem( hChildItem );
							break;
						}
						hChildItem = hNextItem;  
					}
				}
				hChild = hNext;  

			}
		}

	}*/

	return 1;

}

bool CTaiKlineTreeCtl::ModifyEquation()
{
	int flag = IDCANCEL; 

	int nKindRoot,  nKindItem;
	CFormularContent * pJishu = NULL;
	CFormularContent * pJishuCurrent = NULL;
	GetCurrentItemInfo(nKindRoot, nKindItem, pJishuCurrent);//
	ASSERT(nKindRoot>=-4&&nKindRoot<=-1);

	int listitem = GetFoot(-nKindRoot-1, pJishuCurrent);
	if(listitem == -1)
		return false;

	if(pJishuCurrent ==NULL)
		return false;

	int nKlineType = -1;
	if(CMainFrame::m_pDoc->m_arKLineViews .GetSize ()>0)
		nKlineType = CMainFrame::m_pDoc->m_arKLineViews[0]->m_nKlineKind2;

	int nTest = 3322;
	if(pJishuCurrent->sellStr != "")
		nTest*=9;
	int kk = 0;
	kk*=3;
	if(nTest == 3322*9)
		return false;

	pJishu = ShowEditIndicatorDlg(pJishuCurrent,  listitem,-nKindRoot-1,NULL,nKlineType);

	if(pJishu == NULL )
		return false;
//	AfxMessageBox("�޸Ĺ�ʽ�ɹ���");

	//add kind name
	pJishu->AddKindName(pJishu->subKindIndex  , -nKindRoot-1);

	HTREEITEM hItem = GetSelectedItem();//suspose sub
	if(hItem == NULL)
		return false;

	CString sName = pJishu->name + " " + pJishu->explainBrief ;
	BOOL b = SetItemText(hItem,sName);
	if(b == FALSE)
		return false;
	if(! SetItemData( hItem, (DWORD) (pJishu )))
		ASSERT(FALSE);
	
	hItem = GetParentItem(hItem);
	if(hItem != NULL )
	SortChildren(  hItem );

	CTaiShanDoc*   pDoc=CMainFrame::m_pDoc ;
	CFormularContent::ReadWriteIndex(pDoc, 0, false);
	CFormularContent::ReadWriteIndex(pDoc, 1, false);
	CFormularContent::ReadWriteIndex(pDoc, 2, false);
	return true;
}


HTREEITEM CTaiKlineTreeCtl::FindGroupQiTa(HTREEITEM hRoot,CString sName)// "����"
{
	if (ItemHasChildren(hRoot))
	{
		HTREEITEM hNextItem;
	    HTREEITEM hChildItem =GetChildItem(hRoot);
	    while (hChildItem != NULL)   
		{
		  hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
		  CString s = GetItemText(hChildItem);  
		  if(s ==sName)
		  {
			  if(GetItemData (hChildItem) == 0)
				  return hChildItem;
		  }
		  hChildItem = hNextItem;   
		}
	}
	return NULL;

}

int CTaiKlineTreeCtl::GetFoot(int nKind, CFormularContent *pJishu,bool bDoRemove)
{
	int i,nRtn = -1;
	CFormularArray *pList[4];

	pList[0] = &CMainFrame::m_pDoc->m_formuar_index;
	pList[1] = &CMainFrame::m_pDoc->m_formuar_choose;
	pList[2] = &CMainFrame::m_pDoc->m_formuar_kline;

	for( i=0;i<pList[nKind]->GetSize();i++)
	{
		if(pJishu==pList[nKind]->GetAt(i))
		{
			if(bDoRemove == true)
			{
				pList[nKind]->RemoveAt(i);
				delete pJishu;
			}
			else
				nRtn = i;
		}
	}

	return nRtn;
}

void CTaiKlineTreeCtl::InsertEqutionTreeOther(int nKind,bool bExtend)
{
	CString s;
	switch(nKind)
	{
	case -6 :
		s = "������ѡ��";
		break;
	case -7 :
		s = "�۸�Ԥ��";
		break;
	case -8 :
		s = "�ǵ���Ԥ��";
		break;
	case -9 :
		s = "������Ԥ��";
		break;
//		nKindUpDownAlert = -9,//������Ԥ��
	}

	//to insert data to ctrl
//    CImageList img;
//	img.Create(IDB_BITMAP5,16,0,RGB(255,255,255));
	//Add( CBitmap* pbmImage, COLORREF crMask );
//	SetImageList(&img,TVSIL_NORMAL);
//	img.Detach();
	int idb[7]={20,12,14,15,0,0,0};
	SetImageList(CMainFrame::m_pDoc->m_imagelist ,TVSIL_NORMAL);

	HTREEITEM hChapter  = InsertItem(s,idb[-nKind-6],idb[-nKind-6]);
/*	for(int i=0;i<33;i++)
	{
		CString ss ;
		ss.LoadString (IDS_ZGB+i);
		HTREEITEM hEnd = InsertItem(ss,idb[6],idb[6],hChapter);
		if(! SetItemData( hEnd, (DWORD) (i+1) ))
			ASSERT(FALSE);
	}*/
	
	LOGFONT logfont;
	CFont *pFont = GetFont();
	pFont->GetLogFont( &logfont );
	logfont.lfWeight = FW_BOLD;
	SetItemFont(hChapter, logfont );
	SetItemColor( hChapter, RGB(100,0,20));
	if(! SetItemData( hChapter, (DWORD) (nKind) ))
		ASSERT(FALSE);
	if(bExtend ==true)
		Expand(hChapter,TVE_EXPAND);

}

void CTaiKlineTreeCtl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UINT uFlags;
	CPoint pt;
	::GetCursorPos (&pt);
	CPoint ptOld(pt);
	ScreenToClient (&pt);
	HTREEITEM hItem = HitTest(pt, &uFlags);
	if ((hItem != NULL) )
	{
	    SelectItem(hItem);
	}

	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	CString sNameItem = GetItemText(hItem);
	if(nKindItem == 0 || pIndex==NULL)
		if(sNameItem == g_chAllIndex )//|| sNameItem == "����" 
			return;
//	if(nKindRoot<-3 ||nKindRoot>-1)
//		return;

	CMenu menu;
	menu.CreatePopupMenu( );
/*
	m_bNewGroup			=true;
	m_bDeleteGroup		=true;
	m_bModifyGroupName	=true;
	m_bDeleteEquation	=true;
	m_bNewEquation		=true;
	m_bModifyEquation	=true;
MF_SEPARATOR
*/
	if(pIndex != NULL)//END ITEM
	{
		if(nKindRoot == -3)
		{
			if(m_bShowKlineGroup == true)
			{
				menu.AppendMenu(MF_STRING | MF_ENABLED, nShowKlineGroup, strShowKlineGroup);
			}
		}
		if(m_bScreenStock	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nScreenStock, strScreenStock);
		if(m_bScreenStockTest	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nScreenStockTest, strScreenStockTest);
		if(m_bScreenStockIndicator	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nScreenStockIndicator, strScreenStockIndicator);

		if(nKindRoot == -1)
		{
			if(m_bShowIndicator	==true)
			menu.AppendMenu(MF_STRING | MF_ENABLED, nShowIndicator, strShowIndicator);
		}

		if(m_bModifyEquation	==true)
		{
			if(pIndex->sellStr == "")
				menu.AppendMenu(MF_STRING | MF_ENABLED, nModifyEquation, strModifyEquation);
		}
		if(m_bDeleteEquation	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nDeleteEquation, strDeleteEquation);

	}
	else if(nKindItem == 0)//SUB
	{
		if(m_bDeleteGroup	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nDeleteEquationGroup, strDeleteEquationGroup);
		if(m_bModifyGroupName	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nChangeNameGroup, strChangeNameGroup);
		if(m_bNewEquation	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nNewEquation, strNewEquation);

	}
	else if(nKindRoot<=-1 && nKindRoot>=-4)//ROOT
	{
		if(m_bNewGroup	==true)
		menu.AppendMenu(MF_STRING | MF_ENABLED, nNewEquationGroup, strNewEquationGroup);
		if(m_bRefreshEquation)
		{
			menu.AppendMenu(MF_SEPARATOR);
			menu.AppendMenu(MF_STRING | MF_ENABLED, nGroupEquation, strGroupEquation);
			menu.AppendMenu(MF_STRING | MF_ENABLED, nAllEquation, strAllEquation);
			menu.AppendMenu(MF_STRING | MF_ENABLED, nOftenEquation, strOftenEquation);
		}
	}
	else
		ASSERT(FALSE);

	(menu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON  ,
			ptOld.x,ptOld.y,this, NULL));

	*pResult = 0;
}

BOOL CTaiKlineTreeCtl::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class

	bool bRefresh = false;
	if(nCode!=0)
		return CTaiKlineColorTreeCtrl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	m_bShowFromMouse = true;
	switch(nID)
	{
	case nNewEquationGroup:
		 CreateNewGroup ();
		 bRefresh = true;
		break;
	case nDeleteEquationGroup:
		 DeleteGroup ();
		 bRefresh = true;
		break;
	case nChangeNameGroup:
		 ModifyGroupName ();
		 bRefresh = true;
		break;
	case nNewEquation:
		 CreateNewEquation ();
		 bRefresh = true;
		break;
	case nModifyEquation:
		 ModifyEquation ();
		break;
	case nDeleteEquation:
		 DeleteEquation ();
		 bRefresh = true;
		break;
	case nGroupEquation:
	case nAllEquation:
	case nOftenEquation:
		ReInsertIterm(nID);
		break;
	case nScreenStock:
		CommandScreenStock();
		break;
	case nScreenStockTest:
		break;
	case nScreenStockIndicator:
		CommandScreenStockIndicator();
		break;
	case nShowIndicator:
		CommandShowIndicator();
		break;
	case nShowKlineGroup:
		CommandShowKlineGroup();
		break;
	default:
		m_bShowFromMouse = false;
		return CTaiKlineColorTreeCtrl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}

	if(	bRefresh == true)
	{
		((CMainFrame*)AfxGetMainWnd())->RefreshFormu();
		CTaiShanDoc*   pDoc=CMainFrame::m_pDoc ;
		CFormularContent::ReadWriteIndex(pDoc, 0, false);
		CFormularContent::ReadWriteIndex(pDoc, 1, false);
		CFormularContent::ReadWriteIndex(pDoc, 2, false);
	}
	m_bShowFromMouse = false;

	return TRUE;
}

CFormularContent * CTaiKlineTreeCtl::ShowEditIndicatorDlg(CFormularContent *pJishuCurrent, int listitem,int nKindRoot,CString * sGroup,int nKlineType)
{
	int flag=0;
	CFormularContent * pJishu = NULL;
	CTaiKlineDlgEditIndecator  diaeditzbgs;
	diaeditzbgs.m_nKlineType = nKlineType;
	CTaiKlineDlgEditScreenStock  diaedittjxg(IDD_6_EDIT_TJXG_KMODE);
//	diaedittjxg.m_bShowFromMouse = m_bShowFromMouse ;
//	diaeditzbgs.m_bShowFromMouse = m_bShowFromMouse ;
	diaedittjxg.m_nKlineType = nKlineType;

//	if(pJishuCurrent ==NULL)
//		return NULL;
	diaeditzbgs.item=listitem;
	diaedittjxg.item=listitem;
	if(pJishuCurrent!=NULL)
	{
	diaeditzbgs.m_sKindFormu = pJishuCurrent->subKindIndex ;
	diaedittjxg.m_sKindFormu = pJishuCurrent->subKindIndex ;
	}
	else if(sGroup!=NULL)
	{
		CString s = g_chAllIndex;
		if(s.CompareNoCase (*sGroup) != 0)
		{
			diaeditzbgs.m_sKindFormu = *sGroup;
			diaedittjxg.m_sKindFormu = *sGroup;
		}
	}
	
	switch(nKindRoot)
	{
	case 0:
		 diaeditzbgs.iskline=2;
		 if(pJishuCurrent!=NULL)
		 {
			if(pJishuCurrent->isSystem)
			{
				if(AfxMessageBox("����һ��ϵͳָ�꣬\r�޸����Ĳ������㷨\r��ı�ϵͳ��ȱʡֵ��\r��Ҫ������",MB_YESNO |MB_ICONQUESTION)==IDNO)
				break;
			}
										  //��ʾ��������Ի���
			 if(pJishuCurrent->isProtected)
			{
				CDialogGSGLMM mdiagsglmm;
				mdiagsglmm.password=pJishuCurrent->password;
				if(mdiagsglmm.DoModal()==IDOK)
				{
					flag=diaeditzbgs.DoModal();
				
				}
			}
			 else
					flag=diaeditzbgs.DoModal();
		 }
		else  
			flag=diaeditzbgs.DoModal();

		if(flag==IDOK)
		{
			pJishu = diaeditzbgs.jishunow ;

			//add to keyboard
			if(pJishu!=NULL)//listitem ==-1 && 
			{
				CString name=pJishu->name ;//CMainFrame::m_pDoc->m_formuar_index.GetAt(diaeditzbgs.addpos)->name;
				CTaiKeyBoardAngelDlg dlg;
				dlg.AddIndicatorName(name);
			}
		}
	break;
	case 1:     
		 diaedittjxg.iskline=false;
		 if(pJishuCurrent!=NULL)
		 {
			 if(pJishuCurrent->isProtected)
			{
				CDialogGSGLMM mdiagsglmm;
				mdiagsglmm.password=pJishuCurrent->password;
				if(mdiagsglmm.DoModal()==IDOK)
				{
					flag=diaedittjxg.DoModal();
				}
			}
			 else
					flag=diaedittjxg.DoModal();
		 }
			
		else  
			flag=diaedittjxg.DoModal();

		if(flag==IDOK)
			pJishu = diaedittjxg.jishunow ;

	break;
	case 2:
		diaedittjxg.iskline=true;
		 if(pJishuCurrent!=NULL)
		 {
			if(pJishuCurrent->isProtected)
			{
				CDialogGSGLMM mdiagsglmm;
				mdiagsglmm.password=pJishuCurrent->password;
				if(mdiagsglmm.DoModal()==IDOK)
				{
					flag=diaedittjxg.DoModal();
				}
			}
			 else
					flag=diaedittjxg.DoModal();
		 }
		else  
			flag=diaedittjxg.DoModal();
		if(flag==IDOK)
			pJishu = diaedittjxg.jishunow ;
		break;
	}

	return pJishu;

}

CFormularContent* CTaiKlineTreeCtl::GetLJishuFromInt(int nData)
{
	CFormularContent* pJi = NULL;
	if(nData>0 || nData<-100)
		pJi = (CFormularContent*)nData;
	return pJi;

}

int CTaiKlineTreeCtl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTaiKlineColorTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	return 0;
}

void CTaiKlineTreeCtl::ReInsertIterm(int nID)
{
	int n = 0;
	if(nID == nAllEquation)
		n = 1;
	else if(nID == nOftenEquation)
		n = 2;
	ReInsertIterm2(n);

}

void CTaiKlineTreeCtl::SelectItemJ(CFormularContent *p)
{
	if(p == NULL) return;
	HTREEITEM root = GetRootItem( );

	while(root)
	{
		int  i= 0;
		i = GetItemData(root);
		if( (i==-1 || i==-2 || i==-3)) 
		{
			HTREEITEM h = RecursiveLook(p,  root);
			if(h)
			{
				SelectItem(h);
				break;
			}
		}
		root = GetNextItem(root, TVGN_NEXT     );
	}
	return ;

}
//CFormularArray
//RecursiveAddChecked(CFormularArray &arr, HTREEITEM root)
HTREEITEM CTaiKlineTreeCtl::RecursiveLook(CFormularContent *p, HTREEITEM root)
{
	if (ItemHasChildren(root))
	{
	   HTREEITEM hNextItem;
	   HTREEITEM hChildItem = GetChildItem(root);

	   while (hChildItem != NULL)
	   {
			DWORD dw = GetItemData(hChildItem);
		   if(dw == (DWORD)p)
		   {
			   return hChildItem;
		   }

		   HTREEITEM hrtn = RecursiveLook(p,  hChildItem);
		   if(hrtn) 
			   return hrtn;

		  hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
		  hChildItem = hNextItem;
	   }
	}
	return NULL;

}

CTaiShanKlineShowView* CTaiKlineTreeCtl::GetTargetView()
{
	CTaiShanKlineShowView* pView = NULL;
   CMDIChildWnd* pchildfram= ((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();

   CView *pView2 = pchildfram->GetActiveView();
   if(pView2 != NULL)
   {
	   if(!pView2->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView)))
	   {
		   pView = NULL;
	   }
	   else
		   pView = (CTaiShanKlineShowView* )pView2;
   }

   return pView;
}

void CTaiKlineTreeCtl::CommandShowKlineGroup()
{
	CTaiShanKlineShowView * pWnd = CTaiKlineTreeCtl::GetTargetView();
	if(!pWnd) return;
	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	if(nKindItem == 0 || pIndex==NULL)
		return;

	if(nKindRoot<-3 ||nKindRoot>-1)
		return;

	pWnd->ShowKlineMode(pIndex);
}
void CTaiKlineTreeCtl::CommandScreenStock()
{
	HTREEITEM hItem = GetSelectedItem ();//this->m_treeFormularCtrl.HitTest(pt, &uFlags);
	if(hItem == NULL )
	{
	  return;
	}
	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	CString sNameItem = GetItemText(hItem);
	if(nKindItem == 0 || pIndex==NULL)
		return;

	if(nKindRoot<-3 ||nKindRoot>-1)
		return;


	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CTaiScreen* pDlg = pMain->ShowTjxg();
	pDlg->m_treeFormularCtrl.SelectItemJ(pIndex);
}

void CTaiKlineTreeCtl::CommandScreenStockIndicator()
{
	CTaiShanKlineShowView * pWnd = CTaiKlineTreeCtl::GetTargetView();
	if(!pWnd) return;
	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	if(nKindItem == 0 || pIndex==NULL)
		return;

	if(nKindRoot<-3 ||nKindRoot>-1)
		return;

	pWnd->ShowMark(pIndex);

}

void CTaiKlineTreeCtl::CommandShowIndicator()
{
	HTREEITEM hItem = GetSelectedItem ();//this->m_treeFormularCtrl.HitTest(pt, &uFlags);
	if(hItem == NULL )
	{
	  return;
	}

	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	CString sNameItem = GetItemText(hItem);
	if(nKindItem == 0 || pIndex==NULL)
		return;


	if(nKindRoot != CTaiKlineTreeCtl::nKindIndex)
		return;

	CTaiShanKlineShowView * pWnd = CTaiKlineTreeCtl::GetTargetView();
	if(pWnd)
	{
		(pWnd)->DrawZhibiao(pIndex->name ,-1);
	}


}

void CTaiKlineTreeCtl::ReInsertIterm2(int n)
{
	DeleteAllItems();
	for(int i = 0;i<this->m_nKindArray.GetSize();i++)
	{
		this->InsertEqutionTree(m_nKindArray[i],false,n);
	}


	HTREEITEM hroot = GetRootItem( );
	if(hroot) SelectItem( hroot );
}
bool CTaiKlineTreeCtl::CanBeginDrag()
{
	UINT uFlags;
	HTREEITEM hItem = GetSelectedItem();
	if ((hItem == NULL) )
	{
	    return false;
	}
	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	CString sNameItem = GetItemText(hItem);
	
	if(pIndex!=NULL && (nKindRoot>=-3&&nKindRoot<=-1))
		return true;
	return false;
}
bool CTaiKlineTreeCtl::CanDroped(HTREEITEM	hitem)
{
	UINT uFlags;
	HTREEITEM hItem = GetSelectedItem();
	if ((hItem == NULL) )
	{
	    return false;
	}
	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	CString sNameItem = GetItemText(hItem);
	
	HTREEITEM hRoot;//?
	hRoot=GetParentItem(hItem);
	if(hRoot == NULL)
		return false;
	if(nKindItem == 0 && pIndex==NULL && (nKindRoot>=-3&&nKindRoot<=-1))
		return true;
	return false;
}
void CTaiKlineTreeCtl::DoDropOut(CWnd* pWnd,HTREEITEM hItem)
{
	UINT uFlags;
	BOOL b = SelectItem(hItem);
	if ((hItem == NULL) || !b )
	{
	    return ;
	}
	int nKindRoot,nKindItem;
	CFormularContent* pIndex;
	bool bSucc =  GetCurrentItemInfo (nKindRoot,nKindItem,pIndex);
	CString sNameItem = GetItemText(hItem);
	
	if(pIndex!=NULL && (nKindRoot>=-3&&nKindRoot<=-1))
	{
		if(nKindRoot == -1)
		{
			((CTaiShanKlineShowView *)pWnd)->DrawZhibiao(pIndex->name ,-2);
		}
		else
		{
			((CTaiShanKlineShowView *)pWnd)->ShowMark(pIndex);
		}
	}
}
void CTaiKlineTreeCtl::DoDropedSelf(HTREEITEM hItem)
{
	int i = 0;
}

void CTaiKlineTreeCtl::ExportFormular(BOOL bCompleteSecret, BOOL bPassword, CString strPassword)
{
	CFormularArray arr[3];
	HTREEITEM root = GetRootItem( );

	while(root)
	{
		int  i= 0;
		i = GetItemData(root);
		if( (i==-1 || i==-2 || i==-3)) 
		{
			RecursiveAddChecked(arr[-i-1], root,true);
		}
		root = GetNextItem(root, TVGN_NEXT     );
	}

	int nTotal = 0;
	for(int i = 0;i<3;i++)
	{
		nTotal+=arr[i].GetSize ();
	}
	if(nTotal==0)
		return;

	CString fileName = "";

	CString defaultname="*.ts";  
	CString     defaultname2 = defaultname+"||";

	CFileDialog  m_filedia(FALSE,0,defaultname,OFN_CREATEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR,defaultname2);
	if(g_strFileDialogOutPath == "")
		m_filedia.m_ofn .lpstrInitialDir = CMainFrame::m_pDoc ->m_CurrentWorkDirectory;
	else
		m_filedia.m_ofn .lpstrInitialDir = g_strFileDialogOutPath;
	                                    //
	if(m_filedia.DoModal()==IDOK)
	{
		CString filename=m_filedia.GetPathName();
		CString ss2 = m_filedia.GetFileName();
		CString sPath = filename;
		sPath.Delete(filename.GetLength()-ss2.GetLength(),ss2.GetLength());
		g_strFileDialogOutPath = sPath;

		if(filename.Find('.') == -1)
		{
	      CString temp = defaultname;
		  temp.TrimLeft ("*");
		  filename+=temp;
		}
		fileName = filename;

		CFormularContent::ExportFormular(&arr[0], 3, fileName, bCompleteSecret,  bPassword,  strPassword);
	}

}

void CTaiKlineTreeCtl::RecursiveAddChecked(CFormularArray &arr, HTREEITEM root,bool bTestCheck)
{
	if (ItemHasChildren(root))
	{
	   HTREEITEM hNextItem;
	   HTREEITEM hChildItem = GetChildItem(root);
		if(GetCheck(root))
			bTestCheck = false;

	   while (hChildItem != NULL)
	   {
//			DWORD	dw = GetItemData(hChildItem);
//		   if(dw > 0 )
		   {
//				if(!bTestCheck || GetCheck(root))
//				   arr.Add((CFormularContent*)dw);
		   }

		   RecursiveAddChecked(arr,  hChildItem,bTestCheck);

		  hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
		  hChildItem = hNextItem;
	   }
	}
	else
	{
		DWORD dw = GetItemData(root);
		if(dw > 0 )
		{
			if(!bTestCheck || GetCheck(root))
				arr.Add((CFormularContent*)dw);
		}
	}

}
