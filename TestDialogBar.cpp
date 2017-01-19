// SjlToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "TestDialogBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CTaiToolBar, CDialog)
	//{{AFX_MSG_MAP(CTaiToolBar)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipNotify)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiToolBar dialog


CTaiToolBar::CTaiToolBar(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiToolBar::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiToolBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTaiToolBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiToolBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CTaiToolBar message handlers

BOOL CTaiToolBar::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.cx=500;
	return CDialog::PreCreateWindow(cs);
}

int CTaiToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
			
	if ( !m_wndToolBar.CreateEx(this) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
    
	CImageList  imgList;          
	CString     str;

	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(25, 150);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	//attach hot Imagelist
	imgList.Create(IDB_TOOLBAR_HOT, 25,0,RGB(192, 192, 192));
	m_wndToolBar.GetToolBarCtrl().SetHotImageList(&imgList);
	imgList.Detach();

    //attach normal imagelist
	imgList.Create(IDB_TOOLBAR_DISABLE, 25,0,RGB(192, 192, 192));
	m_wndToolBar.GetToolBarCtrl().SetImageList(&imgList);
	imgList.Detach(); 

    //attach disable imagelist
//	imgList.Create(IDB_DISTOOLBAR, 27,0, RGB(192, 192, 192));
//	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&imgList);
//	imgList.Detach();

	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);
	m_wndToolBar.SetButtons(NULL, 17);

    UINT ID_ARRAY[]=
	{
        ID_FILE_OPEN1,ID_TIMESHARING,ID_DAY_LINE,ID_VIEW_JSZB,
		ID_MOVE_FIGUER,ID_POWER_DO,ID_TJXG_MM_TEST_SHOW,ID_TJXG_TJXG,
		ID_TJXG_ALARM,IDM_SORT_INDEX,ID_TJXG_MM_TEST,ID_FULLSCREEN,
		ID_TOOL_GSGL,ID_STOCK_BIG,ID_VIEW_GGXX,ID_COMPANYINFO,
		ID_HELP_TOPIC
	};
	CString sText[]=
	{
		"��","��ʱ","����","ָ��",
		"ƽ��","��Ȩ","ָʾ","ѡ��",
		"Ԥ��","����","����","ȫ��",
		"��ʽ","��","����","����",
		"����"
	};
	// set up each toolbar button
	for( int k=0;k<17;k++)
	{
		m_wndToolBar.SetButtonInfo(k, ID_ARRAY[k], TBSTYLE_BUTTON, k);
		m_wndToolBar.SetButtonText(k,sText[k]);
	}


	// set up toolbar button sizes
	CRect rectToolBar;
	m_wndToolBar.GetItemRect(0, &rectToolBar);
	rectToolBar.bottom+=12;
	rectToolBar.right +=4;
	m_wndToolBar.SetSizes(rectToolBar.Size(), CSize(25,21));
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle()|CBRS_TOOLTIPS | CBRS_FLYBY| CBRS_SIZE_DYNAMIC);
	
	// Set up tooltip for the toolbar
	if(!this->m_wndToolTip.Create(this,TTS_ALWAYSTIP|WS_CHILD|WS_VISIBLE))
	{
	  return -1;
	}
    m_wndToolTip.AddTool(&m_wndToolBar,LPSTR_TEXTCALLBACK);

	CRect rcClientStart;
	CRect rcClientNow;
	GetClientRect(rcClientStart);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST,0, reposQuery, rcClientNow);
	
	CPoint ptOffset(rcClientNow.left - rcClientStart.left,rcClientNow.top - rcClientStart.top); 

	// Adjust the dialog window dimensions
	CRect rcWindow;
	GetWindowRect(rcWindow);
    rcWindow.right -=rcWindow.left + 2;
	rcWindow.left=2;
	rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
    rcWindow.bottom -= rcWindow.top;
    rcWindow.top=2;
	MoveWindow(rcWindow, FALSE);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
    m_wndToolTip.Activate(TRUE);	

	return 0;
}

void CTaiToolBar::OnFileOpen() 
{
	// TODO: Add your command handler code here
}

BOOL CTaiToolBar::PreTranslateMessage(MSG* pMsg) 
{
    m_wndToolTip.RelayEvent(pMsg);	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CTaiToolBar::OnToolTipNotify(UINT id,NMHDR *pNMHDR,LRESULT *pResult)
{
  TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
  UINT nID = pNMHDR->idFrom;

  if(pTTT->uFlags & TTF_IDISHWND)
  {
	  nID = ::GetDlgCtrlID((HWND)nID);
	  if(nID == 0)
		  return FALSE;
  }
  switch(nID)
  {
     case  ID_FILE_OPEN1:
	      pTTT->lpszText = "ѡ���Ʊ";
	      break;
 	 case  ID_GRID:
          pTTT->lpszText = "��̬��ʾ��";
	      break;
     case ID_TIMESHARING:
		  pTTT->lpszText = "ʵʱ����";
	      break;
	 case ID_DAY_LINE:
		   pTTT->lpszText = "K������";
	      break;
	 case ID_VIEW_JSZB:
		  pTTT->lpszText = "����ָ��";
	      break;
	 case ID_POWER_DO:
		  pTTT->lpszText = "��Ʊ��Ȩ";
	      break;
     case ID_MOVE_FIGUER:
          pTTT->lpszText = "ƽ��ͼ��";
	      break;
     case IDM_TOOL_DRAWLINE:
		  pTTT->lpszText = "ͼ�ع���";
	      break;
     case ID_ENTER_INTERNET:
		  pTTT->lpszText = "����INTERNET";
	      break;
	 case ID_COMPANYINFO:
		  pTTT->lpszText = "���й�˾F10����";
	      break;
	 case ID_TOOL_SJGL:
		  pTTT->lpszText = "���ݹ���";
	      break;
	 case ID_STOCK_BIG:
		  pTTT->lpszText = "��ͳ��";
	      break;
	 case ID_TJXG_ALARM:
          pTTT->lpszText = "Ԥ��ϵͳ";
	      break;
	 case ID_TJXG_TJXG:
		  pTTT->lpszText = "����ѡ��";
	      break;
     case ID_FULLSCREEN:
		  pTTT->lpszText = "ȫ����ʾ�л�";
	      break;
	  case ID_LOGIN_SERVER:
		  pTTT->lpszText = "��¼������";
	      break;
     case ID_HELP_TOPIC:
		  pTTT->lpszText = "���߰���";
	      break;
	 case IDM_SORT_INDEX:
		  pTTT->lpszText = "ָ������";
	      break;
	 case ID_TJXG_MM_TEST_SHOW:
		  pTTT->lpszText = "����ѡ��ָʾ";
	      break;
	 case ID_TJXG_MM_TEST:
		  pTTT->lpszText = "����ѡ������";
	      break;
	 case ID_TOOL_GSGL:
		  pTTT->lpszText = "��ʽ����";
	      break;
	 case ID_VIEW_GGXX:
		  pTTT->lpszText = "������Ϣ";
	      break;
  }
  return TRUE;
} 

void CTaiToolBar::OnMouseMove(UINT nFlags, CPoint point) 
{
    m_wndToolTip.Activate(TRUE);	
	CDialog::OnMouseMove(nFlags, point);
}
