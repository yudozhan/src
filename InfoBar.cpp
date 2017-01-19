// InfoBar.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "InfoBar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfoBar dialog


CInfoBar::CInfoBar(CWnd* pParent /*=NULL*/)
	: CDialog(CInfoBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfoBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInfoBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfoBar)
	DDX_Control(pDX, IDC_TREE_INFO, m_treeInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfoBar, CDialog)
	//{{AFX_MSG_MAP(CInfoBar)
	ON_WM_CREATE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_INFO, OnSelchangedTreeInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfoBar message handlers

int CInfoBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CInfoBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// Create image list
	CImageList imgList; 
	imgList.Create(16, 16, ILC_COLORDDB | ILC_MASK, 0, 1);
	// load images and populate image list -GGH
	CBitmap bmp1, bmp2, bmp3;
	bmp1.LoadBitmap(IDB_TREEBMP1);
	bmp2.LoadBitmap(IDB_TREEBMP2);
	bmp3.LoadBitmap(IDB_TREEBMP3);
	imgList.Add(&bmp1, RGB(192, 192, 192));
	imgList.Add(&bmp2, RGB(192, 192, 192));
	imgList.Add(&bmp3, RGB(192, 192, 192));
	m_treeInfo.SetImageList(&imgList, TVSIL_NORMAL);
	
////////////////////////////////////////////////////////
	TV_INSERTSTRUCT TreeCtrlItem;
	HTREEITEM hTreeItem;
	
	TreeCtrlItem.hInsertAfter=TVI_LAST;
	CString FirstTitle;
////////////////////////////////////////////////////////
//1
	//the first level
	TreeCtrlItem.hParent=TVI_ROOT;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE;
	TreeCtrlItem.item.lParam=1;
	TreeCtrlItem.item.iImage=0;
	FirstTitle="��Ѷ���";
	TreeCtrlItem.item.pszText=FirstTitle.GetBuffer(30);
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	//the second level
	TreeCtrlItem.hParent=hTreeItem;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.lParam=10;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=2;
	strcpy(TreeCtrlItem.item.pszText, "��Ϣ����");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "������Ϣ");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "�¹�����");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "ͳ������");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "�ƾ�Ҫ��");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
////////////////////////////////////////////////////////
//2
	//the first level
	TreeCtrlItem.hParent=TVI_ROOT;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE;
	TreeCtrlItem.item.lParam=1;
	TreeCtrlItem.item.iImage=0;
	FirstTitle="��������";
	TreeCtrlItem.item.pszText=FirstTitle.GetBuffer(30);
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	//the second level
	TreeCtrlItem.hParent=hTreeItem;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.lParam=10;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=2;
	strcpy(TreeCtrlItem.item.pszText, "��˾����");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "��ҵ��̬");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "�г���Ϣ");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
////////////////////////////////////////////////////////
//3
	//the first level
	TreeCtrlItem.hParent=TVI_ROOT;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE;
	TreeCtrlItem.item.lParam=1;
	TreeCtrlItem.item.iImage=0;
	FirstTitle="��������";
	TreeCtrlItem.item.pszText=FirstTitle.GetBuffer(30);
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	//the second level
	TreeCtrlItem.hParent=hTreeItem;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.lParam=10;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=2;
	strcpy(TreeCtrlItem.item.pszText, "����֤ȯ");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "��̩����");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "ɽ�����");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "�������");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "�������");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
////////////////////////////////////////////////////////
//4
	//the first level
	TreeCtrlItem.hParent=TVI_ROOT;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE;
	TreeCtrlItem.item.lParam=1;
	TreeCtrlItem.item.iImage=0;
	FirstTitle="�г�����";
	TreeCtrlItem.item.pszText=FirstTitle.GetBuffer(30);
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	//the second level
	TreeCtrlItem.hParent=hTreeItem;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.lParam=10;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=2;
	strcpy(TreeCtrlItem.item.pszText, "������̳");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "���̷���");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "���ɷ���");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "�ȵ�����");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "����׷��");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
////////////////////////////////////////////////////////
//5
	//the first level
	TreeCtrlItem.hParent=TVI_ROOT;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE;
	TreeCtrlItem.item.lParam=1;
	TreeCtrlItem.item.iImage=0;
	FirstTitle="ͳ�Ʊ���";
	TreeCtrlItem.item.pszText=FirstTitle.GetBuffer(30);
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	//the second level
	TreeCtrlItem.hParent=hTreeItem;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.lParam=10;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=2;
	strcpy(TreeCtrlItem.item.pszText, "��Ϣ����");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
////////////////////////////////////////////////////////
//6
	//the first level
	TreeCtrlItem.hParent=TVI_ROOT;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE;
	TreeCtrlItem.item.lParam=1;
	TreeCtrlItem.item.iImage=0;
	FirstTitle="Ͷ�ʱر�";
	TreeCtrlItem.item.pszText=FirstTitle.GetBuffer(30);
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	//the second level
	TreeCtrlItem.hParent=hTreeItem;
	TreeCtrlItem.item.mask=TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	TreeCtrlItem.item.lParam=10;
	TreeCtrlItem.item.iImage=1;
	TreeCtrlItem.item.iSelectedImage=2;
	strcpy(TreeCtrlItem.item.pszText, "���߷���");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "Ͷ�ʼ���");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	strcpy(TreeCtrlItem.item.pszText, "������");
	hTreeItem=m_treeInfo.InsertItem(&TreeCtrlItem);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInfoBar::OnSelchangedTreeInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString tempString;
	tempString=m_treeInfo.GetItemText(m_treeInfo.GetSelectedItem());

	CMainFrame* pMainFrm = (CMainFrame *)(AfxGetApp()->m_pMainWnd);

	if(tempString.Compare("��Ϣ����")==0){pMainFrm->PostMessage(WM_USERDEFINED_INFOMEMORY,   0, 0);	}
	if(tempString.Compare("������Ϣ")==0){pMainFrm->PostMessage(WM_USERDEFINED_PUBLICINFO,   0, 0);	}
	if(tempString.Compare("�¹�����")==0){pMainFrm->PostMessage(WM_USERDEFINED_NEWSTOCK,     0, 0);	}
	if(tempString.Compare("ͳ������")==0){pMainFrm->PostMessage(WM_USERDEFINED_STATISTICS,   0, 0);	}
	if(tempString.Compare("�ƾ�Ҫ��")==0){pMainFrm->PostMessage(WM_USERDEFINED_FINANCENEWS,  0, 0);	}
	if(tempString.Compare("��˾����")==0){pMainFrm->PostMessage(WM_USERDEFINED_COMPANYNEWS,  0, 0);	}
	if(tempString.Compare("��ҵ��̬")==0){pMainFrm->PostMessage(WM_USERDEFINED_TRADEDYNAMIC, 0, 0);	}
	if(tempString.Compare("�г���Ϣ")==0){pMainFrm->PostMessage(WM_USERDEFINED_MARKETNEWS,   0, 0);	}
	if(tempString.Compare("����֤ȯ")==0){pMainFrm->PostMessage(WM_USERDEFINED_YINHESTOCK,   0, 0);	}
	if(tempString.Compare("��̩����")==0){pMainFrm->PostMessage(WM_USERDEFINED_GUOTAIJUNAN,  0, 0);	}
	if(tempString.Compare("ɽ�����")==0){pMainFrm->PostMessage(WM_USERDEFINED_SHENGUANG,    0, 0);	}
	if(tempString.Compare("�������")==0){pMainFrm->PostMessage(WM_USERDEFINED_SHENYINWANGUO,0, 0);	}
	if(tempString.Compare("�������")==0){pMainFrm->PostMessage(WM_USERDEFINED_WANGUOCEPING, 0, 0);	}
	if(tempString.Compare("������̳")==0){pMainFrm->PostMessage(WM_USERDEFINED_MINGJIALUNTAN,0, 0);	}
	if(tempString.Compare("���̷���")==0){pMainFrm->PostMessage(WM_USERDEFINED_DAPANFENXI,   0, 0);	}
	if(tempString.Compare("���ɷ���")==0){pMainFrm->PostMessage(WM_USERDEFINED_GEGUFENXI,    0, 0);	}
	if(tempString.Compare("�ȵ�����")==0){pMainFrm->PostMessage(WM_USERDEFINED_HOTPOINTCOMMT,0, 0);	}
	if(tempString.Compare("����׷��")==0){pMainFrm->PostMessage(WM_USERDEFINED_BLACKHORSE,   0, 0);	}
	if(tempString.Compare("���߷���")==0){pMainFrm->PostMessage(WM_USERDEFINED_POLICY,       0, 0);	}
	if(tempString.Compare("Ͷ�ʼ���")==0){pMainFrm->PostMessage(WM_USERDEFINED_INVESTTECH,   0, 0);	}
	if(tempString.Compare("������")  ==0){pMainFrm->PostMessage(WM_USERDEFINED_STAFFROOM,    0, 0);	}
	
	*pResult = 0;
}








































