// CTaiShanKlineShowView.cpp : implementation of the CTaiShanKlineShowView class
//

#include "stdafx.h"
#include "MainFrm.h"
#include "StructKlineView.h"
//#include "SHEETCOR.h"
#include "DIALOGGSGL.h"
#include "DetailGridCtrl.h"
#include "RealTimeDetailList.h"
#include "CSharesDetailGridCtrl.h"
#include "CSharesRealTimeDetail.h"
#include "myricheditctrl.h"
#include "CTaiShanDoc.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"
#include "DIALOGTEST.h"
#include "GuiZhouTvView.h"
#include "CTaiKlineDialogCross.h"
#include "CTaiKlineDrawLine.h"
#include "CTaiKlineShowText.h"
#include "CTaiKlineDialogChangKLine.h"
#include "KeyBrodEangel.h"
#include "CTaiKlineDialogShiDuanTJ.h"
#include "CTaiKlineDialogKLineMode.h"
#include "LBsInfoRS.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"
//#include "LDlgShowTJXG.h"
#include "CTaiKlineDlgChangeIndexParam.h"
#include "DIALOGXZZB.h"
#include "DiaStockdetail.h"
#include "DiaStockdetail.h"
//#include "DiaHqDetail.h"
#include "SheetScrollBar.h"
#include "DiaCopy.h"
#include <math.h>
#include "CTaiKlineSetMaxMin.h"
#include "CTaiKlineMoveChenben.h"
#include "ChildFrm.h"
#include "CTaiKlineSplitWnd.h"
#include "CTaiKlineWndChild.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiKlineMemFile.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiKlineFileHS.h"
#include "CTaiKlineDlgJishuClassify.h"
#include "CTaiKlineDlgHistorySelect.h"
#include "WH.h"
#include "DIALOGGSSM.h"
#include "CTaiKlineExportExecel.h"
#include "CTaiKlineDlgAddSelfKline.h"
#include "CAlertSet.h"
#include "CAlertSystem.h"
#include "CTaiScreenParent.h"
#include "CTaiScreensIndicate.h"
#include "SelectStock.h"
#include "CTaiShanTesting.h"
#include "CTaiKlineTransferKline.h"
#include "CTaiKlineDlgZhibiaoTongji.h"
#include "CTaiKlineDlgYHParam.h"
#include "CTaiKlineWideNet.h"
#include "DIBSectionLite.h"
#include "ByjDrawRightRect.h"
#include "CwdSelectFormu.h"
#include "BjDlgChengBenSetting.h"

//#define OEM
///
///
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define FS_NUM         7
#define IDC_SCRBAR               600666
///////////////画图常量//////
#define  TM_FENSHI		0
#define  TM_START		1
#define  TM_RULER		2
#define  TM_TIP		3
#define  TM_TRACK_CURCOR		4
#define  TM_MOVE_SHIZI		5
#define  TM_MOVE_K		16
#define  TM_UP_K		17
#define  TM_DOWN_K		18
#define  TM_MOVE_DRAG_K		19//REDRAW_WINDOW
#define  TM_REDRAW_WINDOW		29//
#define  TM_SHOW_DLG		39//
#define NUM_PAGE_STORE	16

#define FIGUER_WORD	1

//

#define  MIN_KNUM		10
;
#define IDM_POP_FINANCE		350//
#define IDM_POP_TRADE_DETAIL_PAN		351//
#define IDM_POP_MOVE_CB		352//
static CString gStrTabSheet[] ={"财务","买卖盘图示","筹码分布","扇形分布类指标"};//"双向筹码分布","成交量分布","内外盘成交量分布",
static CString gStrTabBrief[] ={"财","图","成","总","内","双","扇"};//"总","内","双",
static int gScrollBott = 25;
CTypedPtrMap<CMapStringToPtr,CString,KLINE_EXT_PER*> CTaiShanKlineShowView::m_klineExtMap;//
/////////////////////////////////////////////////////////////////////////////
// CTaiShanKlineShowView
static int m_bAppAuthorized = 1;

IMPLEMENT_DYNCREATE(CTaiShanKlineShowView, CView)

BEGIN_MESSAGE_MAP(CTaiShanKlineShowView, CView)
	//{{AFX_MSG_MAP(CTaiShanKlineShowView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_LINESELF, OnLineself)
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_K_CACL, OnKCacl)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_VSCROLL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_XIANDUAN, OnXianduan)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDM_KLINE_MD, OnKlineMode)
	ON_COMMAND(ID_BS_INFO_SHOW, OnBsInfoShow)
	ON_COMMAND(ID_POWER_DO, OnPowerDo)
	ON_COMMAND(ID_ZHUTU_DIEJIA, OnZhutuDiejia)
	ON_WM_KEYUP()
	ON_COMMAND(ID_VIEW_GPHQ, OnViewGpHuanQuan)
	ON_UPDATE_COMMAND_UI(ID_VIEW_GPHQ, OnUpdateViewGphq)
	ON_WM_DESTROY()
	ON_COMMAND(ID_VIEW_BUYSELL, OnViewBuysell)
	ON_COMMAND(ID_REAL_TIME, OnRealTime)
	ON_COMMAND(ID_DAY_LINE, OnDayLine)
	ON_UPDATE_COMMAND_UI(ID_BS_INFO_SHOW, OnUpdateBsInfoShow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BUYSELL, OnUpdateViewBuysell)
	ON_COMMAND(ID_VIEW_SIMPLE, OnViewSimple)
	ON_COMMAND(ID_VIEW_PERSENT, OnViewPersent)
	ON_COMMAND(ID_VIEW_DSZB, OnViewDszb)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DSZB, OnUpdateViewDszb)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIMPLE, OnUpdateViewSimple)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSENT, OnUpdateViewPersent)
	ON_COMMAND(ID_VIEW_ONE, OnViewOne)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ONE, OnUpdateViewOne)
	ON_COMMAND(ID_VIEW_FOURSTOCK, OnViewFourstock)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FOURSTOCK, OnUpdateViewFourstock)
	ON_COMMAND(ID_VIEW_LINE, OnViewLine)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LINE, OnUpdateViewLine)
	ON_COMMAND(ID_VIEW_DAYLINE, OnViewDayline)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DAYLINE, OnUpdateViewDayline)
	ON_COMMAND(ID_VIEW_FIFTEENMIN, OnViewFifteenmin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FIFTEENMIN, OnUpdateViewFifteenmin)
	ON_COMMAND(ID_VIEW_THIRTYMIN, OnViewThirtymin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THIRTYMIN, OnUpdateViewThirtymin)
	ON_COMMAND(ID_VIEW_SIXTYMIN, OnViewSixtymin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIXTYMIN, OnUpdateViewSixtymin)
	ON_COMMAND(ID_VIEW_MONTHLINE, OnViewMonthline)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MONTHLINE, OnUpdateViewMonthline)
	ON_COMMAND(ID_VIEW_WEEKLINE, OnViewWeekline)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WEEKLINE, OnUpdateViewWeekline)
	ON_COMMAND(ID_VIEW_5MIN, OnView5min)
	ON_UPDATE_COMMAND_UI(ID_VIEW_5MIN, OnUpdateView5min)
	ON_COMMAND(ID_VIEW_ONEITEM, OnViewOneitem)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ONEITEM, OnUpdateViewOneitem)
	ON_COMMAND(ID_VIEW_TWOITEM, OnViewTwoitem)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TWOITEM, OnUpdateViewTwoitem)
	ON_COMMAND(ID_VIEW_FOURITEM, OnViewFouritem)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FOURITEM, OnUpdateViewFouritem)
	ON_COMMAND(ID_VIEW_THREEITEM, OnViewThreeitem)
	ON_UPDATE_COMMAND_UI(ID_VIEW_THREEITEM, OnUpdateViewThreeitem)
	ON_UPDATE_COMMAND_UI(ID_ZHUTU_DIEJIA, OnUpdateZhutuDiejia)
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_FILE_OPEN1, OnFileOpen1)
	ON_COMMAND(ID_TJXG_TJXGTS, OnTjxgTjxgts)
	ON_UPDATE_COMMAND_UI(ID_TJXG_TJXGTS, OnUpdateTjxgTjxgts)
	ON_COMMAND(ID_TOOL_CURSOR, OnToolCursor)
	ON_UPDATE_COMMAND_UI(ID_TOOL_CURSOR, OnUpdateToolCursor)
	ON_UPDATE_COMMAND_UI(ID_POWER_DO, OnUpdatePowerDo)
	ON_COMMAND(ID_VIEW_JSZB, OnViewJszb)
	ON_COMMAND(ID_VIEWSTOCKDETAIL, OnViewstockdetail)
	ON_COMMAND(ID_F2, OnF2)
	ON_COMMAND(ID_VIEW_YEARKLINE, OnViewYearkline)
	ON_UPDATE_COMMAND_UI(ID_VIEW_YEARKLINE, OnUpdateViewYearkline)
	ON_UPDATE_COMMAND_UI(IDM_KLINE_MD, OnUpdateKlineMd)
	ON_COMMAND(IDM_TOOL_DRAWLINE, OnToolDrawline)
	ON_COMMAND(ID_HLDJ_LMB, OnHldjLmb)
	ON_UPDATE_COMMAND_UI(ID_HLDJ_LMB, OnUpdateHldjLmb)
	ON_COMMAND(ID_HEDJ_LMB, OnHedjLmb)
	ON_UPDATE_COMMAND_UI(ID_HEDJ_LMB, OnUpdateHedjLmb)
	ON_UPDATE_COMMAND_UI(ID_VIEW_JSZB, OnUpdateViewJszb)
	ON_UPDATE_COMMAND_UI(IDM_TOOL_DRAWLINE, OnUpdateToolDrawline)
	ON_COMMAND(ID_COPY_FILE, OnCopyFile)
	ON_COMMAND(ID_LOOP_DISPLAY, OnLoopDisplay)
	ON_UPDATE_COMMAND_UI(ID_LOOP_DISPLAY, OnUpdateLoopDisplay)
	ON_COMMAND(ID_DEL_ZHUTU_L, OnDelZhutuL)
	ON_UPDATE_COMMAND_UI(ID_DEL_ZHUTU_L, OnUpdateDelZhutuL)
	ON_COMMAND(ID_QLDJ_LMB, OnQldjLmb)
	ON_UPDATE_COMMAND_UI(ID_QLDJ_LMB, OnUpdateQldjLmb)
	ON_COMMAND(ID_QEDJ_LMB, OnQedjLmb)
	ON_UPDATE_COMMAND_UI(ID_QEDJ_LMB, OnUpdateQedjLmb)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_KLINE_HS, OnKlineHs)
	ON_UPDATE_COMMAND_UI(ID_KLINE_HS, OnUpdateKlineHs)
	ON_COMMAND(ID_KLINE_MANY_DAY, OnKlineManyDay)
	ON_UPDATE_COMMAND_UI(ID_KLINE_MANY_DAY, OnUpdateKlineManyDay)
	ON_COMMAND(ID_KLINE_1_MINU, OnKline1Minu)
	ON_UPDATE_COMMAND_UI(ID_KLINE_1_MINU, OnUpdateKline1Minu)
	ON_COMMAND(IDM_L_SPLIT_V, OnLSplitV)
	ON_UPDATE_COMMAND_UI(IDM_L_SPLIT_V, OnUpdateLSplitV)
	ON_COMMAND(IDM_L_SPLIT_H, OnLSplitH)
	ON_COMMAND(IDM_L_SPLIT_DEL, OnLSplitDel)
	ON_UPDATE_COMMAND_UI(IDM_L_SPLIT_DEL, OnUpdateLSplitDel)
	ON_UPDATE_COMMAND_UI(IDM_L_SPLIT_H, OnUpdateLSplitH)
	ON_COMMAND(IDM_L_SPLIT4, OnLSplit4)
	ON_UPDATE_COMMAND_UI(IDM_L_SPLIT4, OnUpdateLSplit4)
	ON_COMMAND(ID_CLASSIFY_JISHU, OnClassifyJishu)
	ON_COMMAND(ID_HISTORY_OPEN, OnHistoryOpen)
	ON_UPDATE_COMMAND_UI(ID_HISTORY_OPEN, OnUpdateHistoryOpen)
	ON_COMMAND(ID_VIEW_FIVEITEM, OnViewFiveitem)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FIVEITEM, OnUpdateViewFiveitem)
	ON_COMMAND(ID_MENUITEM_DELETE_K, OnMenuitemDeleteK)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM_DELETE_K, OnUpdateMenuitemDeleteK)
	ON_COMMAND(ID_TJXG_MM_TEST_SHOW, OnTjxgMmTestShow)
	ON_UPDATE_COMMAND_UI(ID_TJXG_MM_TEST_SHOW, OnUpdateTjxgMmTestShow)
	ON_COMMAND(ID_MOVE_CHENGBEN_2, OnMoveChengben2)
	ON_COMMAND(ID_TIME_ASTEP_4, OnTimeAstep4)
	ON_WM_HELPINFO()
	ON_COMMAND(ID_NINE_SHOW, OnNineShow)
	ON_COMMAND(ID_MOVE_FIGUER, OnMoveFig)
	ON_UPDATE_COMMAND_UI(ID_MOVE_FIGUER, OnUpdateMoveFig)
	ON_COMMAND(ID_SAVE_BITM, OnSaveBitmap)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_SWITCH_HISTORY_ACC, OnSwitchHistoryAcc)
	ON_COMMAND(ID_MENUITEM32975, OnAstepHist)
	ON_UPDATE_COMMAND_UI(ID_MENUITEM32975, OnUpdateAstepHist)
	ON_UPDATE_COMMAND_UI(ID_SWITCH_HISTORY_ACC, OnUpdateSwitchHistoryAcc)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND_RANGE(ID_BEGIN_LINE, ID_OTHER_LINE, DoDrawLine )
	ON_UPDATE_COMMAND_UI_RANGE( ID_BEGIN_LINE, ID_OTHER_LINE, DoDrawLineUpdate )


	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	//
	ON_MESSAGE(WM_USER_TELL_CHANGE,OnDataChange)//
	ON_MESSAGE(WM_USER_DRAGDROP,OnDropDrag)//
	//	afx_msg void OnKlineToThis(WPARAM wParam, LPARAM lParam);

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiShanKlineShowView construction/destruction
//lmb

CTaiShanKlineShowView::CTaiShanKlineShowView()
:m_screenStockShow(this)
{
//		CTaiShanKlineShowView::CheckDiskFreeSpace();
		RIGHTBOX_LINECOUNT = 19;

		m_nOldnFgr = 0;
		m_stkKind = 0;
		m_pWideNet = NULL;
		for(int i = 0;i<9;i++)
		{
			m_symbol9[i] = "";
			m_stkKind9[i]=0;
			m_nKline9[i] = 0;
		}

		m_dlgFenshiTj = 0;
		m_dlgKlineTj = 0;
		m_dlgZhibiao = 0;


		m_bActived = false;
		m_nCurrentKlineKind=0;
		m_pDlgDealHistory = NULL;
		m_nPageCurrent = 0;
		m_dlgShowCross = NULL;
		m_MyToolTip = NULL;

		//from doc
		m_sharesSymbol = CSharesCompute::GetIndexSymbol(0);
		m_nCountKline = 0;
		m_nCountKlineFile = 0;
		m_pkline=new Kline;
		m_infoInit.nCountIndex=2;
		m_infoInit.initIndex[0].yFloatBottom=0.25;
		m_infoInit.initIndex[1].yFloatBottom=0.25;
		m_infoInit.initIndex[2].yFloatBottom=0.25;
		m_infoInit.initIndex[3].yFloatBottom=0.25;
		m_infoInit.initIndex[4].yFloatBottom=0.25;

		m_infoInit.nCountMin1=3;
		m_infoInit.initMin1[0].yFloatBottom=0.25;
		m_infoInit.initMin1[1].yFloatBottom=0.25;
		m_infoInit.initMin1[2].yFloatBottom=0.25;
		m_infoInit.initMin1[3].yFloatBottom=0.25;
		m_infoInit.initMin1[4].yFloatBottom=0.25;

		//每个子图的指标的ID号。0为分时走势，
		//1为成交量，2为买卖力道，3为量比指标，
		//4为领先指标，5为多空指标
		m_infoInit.flag[0]=0;
		m_infoInit.flag[1]=1;
		m_infoInit.flag[2]=2;
		m_infoInit.flag[3]=1;
		m_infoInit.flag[4]=0;
		m_infoInit.flag_dapan[0]=0;
		m_infoInit.flag_dapan[1]=1;
		m_infoInit.flag_dapan[2]=1;
		m_infoInit.flag_dapan[3]=1;
		m_infoInit.flag_dapan[4]=1;

		strcpy(m_infoInit.initIndex[0].nameIndex,"MA");
		strcpy(m_infoInit.initIndex[1].nameIndex,"KDJ");
		strcpy(m_infoInit.initIndex[2].nameIndex,"MACD");
		strcpy(m_infoInit.initIndex[3].nameIndex,"RSI");
		strcpy(m_infoInit.initIndex[4].nameIndex,"KDJ");
		
		m_infoInit.nIDRightSheet[0] = 0;
		m_infoInit.nIDRightSheet[1] = 2;
		//end doc

		m_bInited = false;
		IsBarShow = false;
		m_bKD = false;
//		m_bSetCursor = false;

		m_hit=0;
		m_nBeginHS=0;

		pDrawLine=NULL;
		pMin1Drawing=NULL;
		pKlineDrawing=NULL;
		m_scrollBar=NULL;

		bTrackCurcorBgn=false;
//		bPrintPrecise=false;
		m_minGridX=55;
		m_minGrid=30;
		m_heightCaption=15;
		m_midLen=50;
		m_rightLength=172;
		
		m_pChangeIndexParam=NULL;
//		m_tjxgShow=NULL;
		m_bKeyUpDown=false;

		m_bDoPower=true;
		m_bBaseInfo=false;
		pKlineMode=NULL;

		CString s;
		m_tabNum=0;
		//m_nKlineKind2=5;
		m_nKlineKind2 = ((CWHApp *)AfxGetApp())->Line_mode;

		if(m_nKlineKind2 > 5 && m_nKlineKind2 < 0)
		{
		  MessageBox("Kline mode error");
		  m_nKlineKind2 = 0;
		}
		m_infoFiguer=1;
		m_bMultiFiguer=0;

		m_pointMove.x=0;
		m_pointMove.y=0;

//		m_klinNumDefault=82;//CPrintdlg
//		m_rtMin1.leftX=40;
		m_nOldHitPos=-1;
		m_nOldHitY=-1;

		m_isShowCross=0;
		m_tjxgshow = 1;
		m_AutoChange = false;
		m_GetFocus = TRUE;
		bExtend=false;
		lineTypeDrawed=-1;
}

CTaiShanKlineShowView::~CTaiShanKlineShowView()
{

	CTaiShanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(	m_dlgShowCross != NULL)
		delete m_dlgShowCross;
	if( m_MyToolTip != NULL)
		delete m_MyToolTip;

	
	if(m_bDoPower == false)
	{
		m_infoInit.nIDRightSheet [9] = 65798809	;
	}
	else
		m_infoInit.nIDRightSheet [9] = 0	;
	m_infoInit.nIDRightSheet[8] = m_rightLength;

	memcpy(&pDoc->m_propertyInitiate2,&m_infoInit ,sizeof(SUB_FIGUER_INIT_INFO));
	

	if(m_pkline!=NULL)
	{
		delete[] m_pkline;
		m_pkline = NULL;
	}

	CMainFrame* pMainWnd = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	if(IsBarShow  == TRUE && pMainWnd->m_drawLineToolBar->IsWindowVisible() != 0 )
	{
      pMainWnd->ShowControlBar(pMainWnd->m_drawLineToolBar,FALSE, FALSE);
	  IsBarShow = !IsBarShow;
	}
	if(m_bInited == false)
		return;
	
	((CWHApp *)AfxGetApp())->Line_mode = m_nKlineKind2;

	if(pDrawLine!=NULL)
		delete pDrawLine;
//	AfxMessageBox("pDrawLine");
	if(pMin1Drawing!=NULL)
		delete pMin1Drawing;
//	AfxMessageBox("pMin1Drawing");
	if(pKlineDrawing!=NULL)
		delete pKlineDrawing;
	if(m_scrollBar!=NULL)
		delete m_scrollBar;

	if(m_pRichEdit!=NULL)
		delete m_pRichEdit;
	//
	if(m_pDlgDealHistory!=NULL)
	{
		m_pDlgDealHistory->DestroyWindow();
		delete m_pDlgDealHistory;
		m_pDlgDealHistory = NULL;
	}
  
	try
	{
		if(m_bs.IsOpen ())
			m_bs.Close ();
	}
	catch (CDaoException* e)
	{
		e->Delete();
	}
//    GetDocument()->m_pKlineShowView = NULL;
//	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
//	pFm->pVwbaseView=NULL;
	m_bit.DeleteObject ();
}
////lmb
BOOL CTaiShanKlineShowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style =cs.style|WS_MAXIMIZE;//

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTaiShanKlineShowView drawing
///////////////////////////////////////////////////////////////////
//      函 数 名：DrawLineCurve(CDC *pDC)
//      功能说明：画右边小图的标签
//      入口参数：pDC为设备描述表的指针
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::DrawLineCurve(CDC *pDC)
{
	CRect rt;
	GetCurrClientRect(rt);
	
	//Marenan -
	//CRect rt1(rt.right-m_rightLength+1,19*RIGHTBOX_PERLINE+1,rt.right,rt.bottom+3);
	//Marenan +
	CRect rt1;
	if (m_bInfoTabFill)
		rt1.CopyRect(CRect(rt.right-m_rightLength+1,0,rt.right,rt.bottom));
	else
		rt1.CopyRect(CRect(rt.right-m_rightLength+1,RIGHTBOX_LINECOUNT*RIGHTBOX_PERLINE+1,rt.right,rt.bottom));
	//Marenan +

	if(rt.bottom<=19*RIGHTBOX_PERLINE+gScrollBott+2)
		return;
	//////if
	if(IsIndexStock(m_sharesSymbol))
	{
		rt1.top = rt.bottom -32;
		ClearBack(pDC, rt1,pMin1Drawing->m_bHist);
	}
	else
		ClearBack(pDC, rt1,pMin1Drawing->m_bHist);

	CFont ft;
	LOGFONT lgf=GetDocument()->m_fontstr[0];
	lgf.lfHeight=-14;
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);

	int modify=3;
	int lf=4;
	pDC->SetTextAlign( TA_CENTER );//TA_CENTER||//
	pDC->SetTextColor( GetDocument()->m_colorArray[FIGUER_WORD]);
	int left = rt.right-m_rightLength;
	float gap = (float)(m_rightLength /7);				//Modified by ltr
	CString s[7] = {"细","价","指","势"};		//Modified by ltr
	if(!IsIndexStock(m_sharesSymbol))
	{
		s[4]=gStrTabBrief[m_infoInit.nIDRightSheet [0]];
		s[5]=gStrTabBrief[m_infoInit.nIDRightSheet [1]];
		s[6]="评";		//Modified by ltr
		gScrollBott = 5+pDC->GetOutputTextExtent(s[0]).cy ;
		int high = rt.bottom -gScrollBott;
		for(int j=0;j<7;j++)		//Modified by ltr
		{
			pDC->SetTextColor( GetDocument()->m_colorArray[13]);
			pDC->TextOut(left+(j+0.5)*gap,high,s[j]);
		}
		pDC->SelectObject(pOld);

		CPen pen;
		if (!pen.CreatePen(PS_SOLID, 1,GetDocument()->m_colorArray[2]))
			return ;
		CPen* pOldPen = pDC->SelectObject(&pen);
		if(pDC->IsPrinting ())
		{
			pDC->MoveTo (0,0);
			pDC->LineTo (rt.right-1,0);
			pDC->LineTo (rt.right-1,rt.bottom);
			pDC->LineTo (0,rt.bottom);
			pDC->LineTo (0,0);
		}

		int nSide = 3;
		CPoint pt[6];
		pt[0]=CPoint(left,high-2);
		pt[1]=CPoint(left+m_tabNum*gap-nSide,high-2);
		pt[2]=CPoint(left+m_tabNum*gap+nSide,rt.bottom-2);
		pt[3]=CPoint(left+(m_tabNum+1)*gap-nSide,rt.bottom-2);
		pt[4]=CPoint(left+(m_tabNum+1)*gap+nSide,high-2);
		pt[5]=CPoint(rt.right-1,high-2);
		if(m_tabNum==0)
		{
			pt[1].y =rt.bottom - gScrollBott/2;
			pt[1].x = pt[0].x;
			pt[0].y = pt[1].y;
		}
		if(m_tabNum==6)	//modified by ltr
		{
			pt[4].y =rt.bottom - gScrollBott/2;
			pt[5].y = pt[4].y;
			pt[4].x= pt[5].x;
		}
		pDC->MoveTo (pt[0]);
		for (int i=0;i<6;i++)
		{
			pDC->LineTo(pt[i]);
		}
		pDC->SelectObject(pOldPen);
	}
}
///////////////////////////////////////////////////////////////////
//      函 数 名：DrawRectPer(CDC* pDC)
//      功能说明：画边框
//      入口参数：pDC为设备描述表的指针
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::DrawRectPer(CDC* pDC)
{
	//m_rtMin1.rtFenshiPrc;
	CRect rt;
//	GetClientRect(rt);
	rt.top = 0;
	rt.left = 0;

	CPen pen;
	if (!pen.CreatePen(PS_SOLID,1,GetDocument()->m_colorArray[2]))
		return;
	CPen* pOldpen = pDC->SelectObject(&pen);

	if(m_nKlineKind2 == 0)
	{
		int nm = m_infoInit.nCountMin1;
		rt.right = m_rtMin1.rightX + m_midLen;
		rt.bottom = m_rtMin1.rtBlw[nm - 1].m_yBottom + m_heightCaption + 2;
		//////////////////////////////////////
		// 初始化为白色背景
		ClearBack(pDC, rt);

		int	i;
		int left;
		left = m_rtMin1.leftX;
		for(i = 0;i < nm;i++)
		{
			pDC->MoveTo (left,m_rtMin1.rtBlw[i].m_yBottom);
			pDC->LineTo (m_rtMin1.rightX + m_midLen,m_rtMin1.rtBlw[i].m_yBottom);
		}
//		for(i=0;i<nm-1;i++)
		{
//			pDC->MoveTo (left,m_rtMin1.rtBlw[i].m_yBottom-1);
//			pDC->LineTo (m_rtMin1.rightX+m_midLen,m_rtMin1.rtBlw[i].m_yBottom-1);
		}

		pDC->MoveTo (m_rtMin1.rightX,m_rtMin1.rtBlw[nm-1].m_yBottom+1+m_heightCaption);
		pDC->LineTo (m_rtMin1.rightX,0);
		pDC->MoveTo (m_rtMin1.rightX+m_midLen,m_rtMin1.rtBlw[nm-1].m_yBottom+1+m_heightCaption);
		pDC->LineTo (m_rtMin1.rightX+m_midLen,0);

		if(left>0)
		{
			pDC->MoveTo (left,m_rtMin1.rtBlw[nm-1].m_yBottom+1+m_heightCaption);
			pDC->LineTo (left,0);
		}

	}
	else if(m_nKlineKind2 > 0)
	{
		int nm=m_infoInit.nCountIndex;
		rt.right=m_rtKlineFiguer.rightX+m_midLen;
		rt.bottom=m_rtKlineFiguer.rtBlw[nm-1].m_yBottom + m_heightCaption+2;
		ClearBack(pDC, rt);

		int	i;
		for(i=0;i<nm;i++)
		{
			pDC->MoveTo (0,m_rtKlineFiguer.rtBlw[i].m_yBottom);
			pDC->LineTo (m_rtKlineFiguer.rightX+m_midLen,m_rtKlineFiguer.rtBlw[i].m_yBottom);
		}
//		for(i=0;i<nm-1;i++)
		{
//			pDC->MoveTo (0,m_rtKlineFiguer.rtBlw[i].m_yBottom-1);
//			pDC->LineTo (m_rtKlineFiguer.rightX+m_midLen,m_rtKlineFiguer.rtBlw[i].m_yBottom-1);
		}

		pDC->MoveTo (m_rtKlineFiguer.rightX,m_rtKlineFiguer.rtBlw[nm-1].m_yBottom+1+m_heightCaption);
		pDC->LineTo (m_rtKlineFiguer.rightX,0);
		pDC->MoveTo (m_rtKlineFiguer.rightX+m_midLen,m_rtKlineFiguer.rtBlw[nm-1].m_yBottom+1+m_heightCaption);
		pDC->LineTo (m_rtKlineFiguer.rightX+m_midLen,0);


	}
	if(pDC->IsPrinting ())
	{
		pDC->MoveTo (0,0);
		pDC->LineTo (rt.right-1,0);
		pDC->LineTo (rt.right-1,rt.bottom-1);
		pDC->LineTo (0,rt.bottom-1);
		pDC->LineTo (0,0);
	}

	pDC->SelectObject(pOldpen);

}
void CTaiShanKlineShowView::OnDraw(CDC* pDC)//6.17
{
	if(m_bAppAuthorized > 0)
	{
		m_bAppAuthorized++;
		if(m_bAppAuthorized > 1000) 
			Sleep(1000);
		else if(m_bAppAuthorized > 2000) 
			Sleep(2000);
	}

	if(pKlineMode != NULL)
	{
		if(m_bActived == TRUE)
		{
			if(m_nKlineKind2 == 0)
				pKlineMode->ShowWindow(SW_HIDE);
			else
				pKlineMode->ShowWindow(SW_SHOW);
		}
		else 
			pKlineMode->ShowWindow(SW_HIDE);
	}
	if(m_pChangeIndexParam != NULL)
	{
		if(::IsWindow (m_pChangeIndexParam->m_hWnd))
		{
			if(m_nKlineKind2 == 0)
				m_pChangeIndexParam->ShowWindow(SW_HIDE);
			else
			{
				if(m_bActived==TRUE)
					m_pChangeIndexParam->ShowWindow(SW_SHOW);
				else
					m_pChangeIndexParam->ShowWindow(SW_HIDE);
			}
		}
	}

	if(bTrackCurcorBgn == TRUE && m_nKlineKind2 > 0)
		return;

	CRect r6;
	GetClientRect(r6);
	CRect	rectClip;
	pDC->GetClipBox( &rectClip );
	if(	rectClip.bottom < r6.bottom - 10 || 
		rectClip.top > r6.top + 100 || 
		rectClip.right < r6.right - 10)
	{
		CopyRect(rectClip);
		return;
	}

	CTaiShanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// 得到需重新显示的区域
	if(rectClip.Width() == 0 || rectClip.Height() == 0)
		return;

	{
		//in the place ,must use "{}" ,because dc2 need destruct
		CTaiKlineDC dc2(this,&m_bit);
		CTaiKlineDC* pMemDC = &dc2;//
		//---		多图显示
		if(m_bMultiFiguer == 1)
		{
			//Marenan -
			//m_scrollBar->ShowScrollBar(false );	
			if(m_nKlineKind2 == 0)
				pMin1Drawing->ShowMultiPage(pMemDC);
			else
				pKlineDrawing->ShowMultiPage(pMemDC);
			CRect rClt;
			return;
		}

		DrawRectPer(pMemDC);

		pKlineDrawing->DrawKlineFiguer(pMemDC);
		DrawRightSheet(pMemDC,m_tabNum);
		// 将变化的部分从位图拷贝到屏幕上
	}

	ClearCross();
	ShowCross();
	DrawRulorBoxY(m_pointMove.y);
	DrawRulorBoxX(m_pointMove.x);

//	int * pint = new int[1024*1024];
}
int CTaiShanKlineShowView::OnCreate(LPCREATESTRUCT lpCreateStruct)//6.16
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
//	if(CTaiShanDoc::m_bAppAuthorized )
	m_bAppAuthorized = 0;
//	m_bAppAuthorized = CTaiShanDoc::m_bAppAuthorized;
	CTaiShanDoc* pDoc = GetDocument();
	this->m_sharesSymbol = pDoc->m_sharesSymbol ;
	this->m_stkKind = pDoc->m_stkKind ;
	ASSERT_VALID(pDoc);
	pDoc->m_arKLineViews.Add(this);
	memcpy(&m_infoInit,&pDoc->m_propertyInitiate2 ,sizeof(SUB_FIGUER_INIT_INFO));
	if(m_infoInit.nIDRightSheet [9]!= 65798809)
	{
		m_bDoPower = true;
	}
	else
		m_bDoPower = false;
	m_rightLength = m_infoInit.nIDRightSheet[8] ;
	if(m_rightLength<50) m_rightLength = 172;
	if(m_rightLength>600) m_rightLength = 172;

	if(pDoc->m_arKLineViews.GetSize()>1)
		m_infoFiguer=0;

	m_bInfoTabFill = FALSE;
	m_bZoomEnable = TRUE;


#ifdef WIDE_NET_VERSION
	m_pWideNet = new CTaiKlineWideNet(this);
	m_pWideNet->RegisterID();
#endif

//	设置常数
	SetConstant();
//打开文件m_infoInit.nIDRightSheet m_infoInit.nIDRightSheet

	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
//	pFm->pVwbaseView=this;
	CRect rect(0,0,-1,-1);
	m_MyToolTip = new CTaiTestTipWin();
	m_MyToolTip->Create (this);
	m_dlgShowCross = new CTaiKlineDialogCross(this);
	m_dlgShowCross->Create(IDD_DIALOG1,this);

	CRect r3;
	m_dlgShowCross->GetClientRect(r3);
//	if(r3.Width()<140 || r3.Height()<179)
	{
		CRect r4;
		m_dlgShowCross->GetWindowRect(r4);
		m_dlgShowCross->MoveWindow(100,280,148,r4.Height()-r3.Height()+m_dlgShowCross->m_nHeight*12,FALSE);
	}
	SetFocus ();


	SetTimer(TM_START,10000,NULL);

//////////////////////////////////////////////////
	m_nMaxX=GetSystemMetrics(SM_CXSCREEN);
	m_nMaxY=GetSystemMetrics(SM_CYSCREEN);
	//////////////////////////////////////
	// 建立屏幕显示位图
	CDC* dc=GetDC();

	// 创建显示内存位图
	m_bit.CreateCompatibleBitmap( dc, 
									m_nMaxX,
									m_nMaxY );

	ReleaseDC(dc);

//	btp.DeleteObject();
////////////////////////////////////////////////////////////

	//Marenan -
	//CRect rt(15,15,19,19);
	//m_scrollBar=new CScrollBar();
	//m_scrollBar->Create (WS_CHILD|WS_VISIBLE |SBS_VERT   ,rt,this,IDC_SCRBAR);
	//m_scrollBar->ShowWindow (SW_HIDE);
	//m_scrollBar->SetScrollRange(SB_VERT,0,100);
	//SCROLLINFO pI;
	//m_scrollBar->GetScrollInfo(&pI,SIF_PAGE);
	//pI.nPage=1;
	//m_scrollBar->SetScrollInfo(&pI,SIF_PAGE);
	//Marenan -

	//Marenan +
	CRect rt(15,15,19,19);
	m_scrollBar=new CXScrollBar;
	m_scrollBar->Create(SBS_VERT|WS_CHILD|SS_LEFT|SS_NOTIFY|WS_VISIBLE,this,rt,IDC_SCRBAR); 
	m_scrollBar->ShowWindow (SW_HIDE);
	m_scrollBar->SetScrollRange(0,100);
	m_scrollBar->SetScrollPos(0);
	m_scrollBar->SetColor(FALSE,GetDocument()->m_colorArray[0],GetDocument()->m_colorArray[2]);
	//Marenan +



	m_pRichEdit=new CTaiTestRichEditCtrl;
	CRect rc(0,0,50,50);
	DWORD dwStyle=WS_CHILD | WS_VSCROLL | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN;
	m_pRichEdit->Create(dwStyle,rc,this,IDC_RICHEDIT_PINGCE);
	CHARFORMAT cf;
	cf.cbSize=sizeof(CHARFORMAT);
	cf.dwMask = CFM_FACE | CFM_SIZE | CFM_CHARSET | CFM_BOLD;
	cf.bCharSet=GB2312_CHARSET;
	_tcscpy(cf.szFaceName,_T("宋体"));
	cf.yHeight=180;
	cf.dwEffects=0;
	m_pRichEdit->SetDefaultCharFormat(cf);

	PARAFORMAT2 pf;
	pf.cbSize=sizeof(PARAFORMAT2);
	pf.dwMask=PFM_LINESPACING;
	pf.bLineSpacingRule=0;
	pf.dyLineSpacing=50;
	m_pRichEdit->SetParaFormat(pf);
	
	// TODO: Add your specialized creation code here
	int m_mbzq;
	float m_mbly;
    if(m_screenStockShow.LoadTjxgZsFile("SaveTjxg.dat",false))
	{
		m_screenStockShow.m_bUseing=true;
		CFile file;
		file.Open("SaveTjxg.dat",CFile::modeRead);
		file.Seek(-8,CFile::end);
		file.Read(&m_mbzq,sizeof(int));
		file.Read(&m_mbly,sizeof(float));
		m_screenStockShow.m_mbzq=m_mbzq;
		m_screenStockShow.m_mbly=m_mbly;
	}
	//init

    SetWindowText("图形分析");
	
	pKlineDrawing=new CTaiKlineShowKline(this,pDoc);
	if(pKlineDrawing==NULL)
		AfxMessageBox("initial 3");
	pMin1Drawing=new CTaiKlineMin1(pDoc,this);
	if(pMin1Drawing==NULL)
		AfxMessageBox("initial 4");
	pDrawLine=new CTaiKlineDrawing(this,pDoc);
		

	ShowAll(this->m_sharesSymbol );
	m_bInited = TRUE;

	return 0;


}

/////////////////////////////////////////////////////////////////////////////
// CTaiShanKlineShowView printing

BOOL CTaiShanKlineShowView::OnPreparePrinting(CPrintInfo* pInfo)
{
   pInfo->SetMaxPage(1);
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTaiShanKlineShowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTaiShanKlineShowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTaiShanKlineShowView diagnostics

#ifdef _DEBUG
void CTaiShanKlineShowView::AssertValid() const
{
	CView::AssertValid();
}

void CTaiShanKlineShowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTaiShanDoc* CTaiShanKlineShowView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTaiShanDoc)));
	return (CTaiShanDoc*)m_pDocument;
}
//#ifdef _DEBUG
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CTaiShanKlineShowView message handlers

void CTaiShanKlineShowView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
}
///////////////////////////////////////////////////////////////////
//      函 数 名：InitRect(int cx, int cy) 
//      功能说明：初始化子图的位置
//      入口参数：cx为总宽度，cy为总高度
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::InitRect(int cx, int cy) 
{
	int flag = 0;
	if( m_infoFiguer > 0)
		flag = 1;

	m_rtMin1.rightX = cx - m_rightLength * flag - m_midLen;
	m_rtMin1.leftX = m_midLen;

	float	totalFlt = 0;
	int	i;
	int nm=m_infoInit.nCountMin1;
	for(i=0;i<nm;i++)
		totalFlt+=m_infoInit.initMin1[i].yFloatBottom;

	for(i=0;i<nm;i++)
	{
		if(totalFlt == 0)
		{
			totalFlt = 0.4f;
			for(int k=0;k<nm;k++)
			{
				m_infoInit.initMin1[k].yFloatBottom = 0.4f/nm;
			}
		}
		m_rtMin1.rtBlw[i].m_yBottom=(int)((cy-(nm+1)*m_heightCaption)*
			m_infoInit.initMin1[i].yFloatBottom/totalFlt);
		if(m_rtMin1.rtBlw[i].m_yBottom<0)
			m_rtMin1.rtBlw[i].m_yBottom=0;
		if(i>0)
		{
			m_rtMin1.rtBlw[i].m_yBottom+=m_rtMin1.rtBlw[i-1].m_yBottom + m_heightCaption;
		}
		else 
		{
			m_rtMin1.rtBlw[0].m_yBottom+=m_heightCaption;
		}
	}

//////////////////////

	flag=0;
	if( m_infoFiguer>0)
		flag=1;

	m_rtKlineFiguer.rightX=cx-m_rightLength*flag-m_midLen;

	totalFlt=0;
	nm=m_infoInit.nCountIndex;
	for(i=0;i<nm;i++)
		totalFlt+=m_infoInit.initIndex[i].yFloatBottom;

	for(i=0;i<nm;i++)
	{
		if(totalFlt == 0)
		{
			totalFlt = 0.4f;
			for(int k=0;k<nm;k++)
			{
				m_infoInit.initIndex[k].yFloatBottom = 0.4f/nm;
			}
		}

		{
		m_rtKlineFiguer.rtBlw[i].m_yBottom=(int)((cy-(nm+1)*m_heightCaption)*
			m_infoInit.initIndex[i].yFloatBottom/totalFlt);
		if(m_rtKlineFiguer.rtBlw[i].m_yBottom<0)
			m_rtKlineFiguer.rtBlw[i].m_yBottom=0;
		}
		if(i>0)
		{
			m_rtKlineFiguer.rtBlw[i].m_yBottom+=m_rtKlineFiguer.rtBlw[i-1].m_yBottom+m_heightCaption;
		}
		else 
		{
			m_rtKlineFiguer.rtBlw[0].m_yBottom+=m_heightCaption;
		}
	}
}
void CTaiShanKlineShowView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
    if(cx == 0 && cy == 0)
		return;
	OnSizeMy(cx,cy);
	// TODO: Add your message handler code here
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();

}

BOOL CTaiShanKlineShowView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
//	return CView::OnEraseBkgnd(pDC);
	return TRUE;
}



void CTaiShanKlineShowView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler co de here and/or call default//RDW_INTERNALPAINT   
////
//	Kline  pl;
	/////////////////////////////
	if(m_bMultiFiguer==1)
	{
		CView::OnLButtonDown(nFlags, point);
		return;
	}

    CaclRulorMovable(point);
	CTaiShanDoc* pDoc = GetDocument();
	CRect r;
	int i;
	if(::GetCapture()==NULL)
	{
		switch(m_hit)
		{
		case REGION_PICTBORDER1:
		case REGION_PICTBORDER2:
		case REGION_PICTBORDER3:
		case REGION_PICTBORDER4:
			SetCapture();
			m_nOldHitPos = -1;
			i = m_hit-REGION_PICTBORDER1;
			if(m_nKlineKind2==0)
			{
				r.left = m_rtMin1.leftX;
				r.right = m_rtMin1.rightX;
				if(i==0)
					r.top = m_heightCaption;
				else
					r.top = m_rtMin1.rtBlw[i-1].m_yBottom+m_heightCaption;
				r.bottom = m_rtMin1.rtBlw[i+1].m_yBottom-m_heightCaption;
			}
			else
			{
				r.left = 0;
				r.right = m_rtKlineFiguer.rightX;
				if(i==0)
					r.top = m_heightCaption;
				else
					r.top = m_rtKlineFiguer.rtBlw[i-1].m_yBottom+m_heightCaption;
				r.bottom = m_rtKlineFiguer.rtBlw[i+1].m_yBottom-m_heightCaption;
			}
			m_nOldHitPos = point.y;
			ClientToScreen(r);
			::ClipCursor(r);
			break;
		case REGION_RIGHT_LINE:
			if(m_infoFiguer>0)
			{
				SetCapture();
				m_nOldHitPos = point.x;
			}
			break;
		case REGION_RULER:
 			if(TRUE)
			{
//				m_nOldHitPos = pDoc->m_nDispTotal*
//							   (pDoc->GetAxisRight()-point.x)/
//							   pDoc->m_nAxisTotal;

				m_nOldHitPos = (int)((m_rtKlineFiguer.rightX-point.x)*pKlineDrawing->m_klinNumDefault /m_rtKlineFiguer.rightX);
				SetCapture();
				if(m_nKlineKind2==0)
				{
					r.left = m_rtMin1.leftX;
					r.right = m_rtMin1.rightX;
					r.top = m_rtMin1.rtBlw[m_infoInit.nCountMin1-1].m_yBottom;
					r.bottom = m_rtMin1.rtBlw[m_infoInit.nCountMin1-1].m_yBottom+m_heightCaption;
				}
				else
				{
					r.left = 0;
					r.right = m_rtKlineFiguer.rightX;
					r.top = m_rtKlineFiguer.rtBlw[m_infoInit.nCountIndex-1].m_yBottom;
					r.bottom = m_rtKlineFiguer.rtBlw[m_infoInit.nCountIndex-1].m_yBottom+m_heightCaption;
				}
				ClientToScreen(r);
				::ClipCursor(r);
			}
			break;
		case REGION_TRANSFER:
		case REGION_PICTTEXT1:
			break;
		case REGION_PICTTEXT2:
		case REGION_PICTTEXT3:
		case REGION_PICTTEXT4:
		case REGION_PICTTEXT5:
			{
			i=m_hit-REGION_PICTTEXT2;	//m_nTextBox
			GetClientRect(r);
			if(m_nKlineKind2==0)
			{
				if(i==pMin1Drawing->m_nTextBox)
					break;
				if(IsIndexStock3(m_sharesSymbol))
				{
					if(i == 0)
					{
						break ;
					}
				}
				r.left =m_rtMin1.rightX;
				r.right =m_rtMin1.rightX+m_midLen;
			}
			else
			{
				if(i==pKlineDrawing->m_nTextBox)
					break;
				r.left =m_rtKlineFiguer.rightX;
				r.right =m_rtKlineFiguer.rightX+m_midLen;
			}
			CTaiKlineDC memdc(this,&m_bit,&r);
			if(m_nKlineKind2==0)
				pMin1Drawing->m_nTextBox=i;
			else
				pKlineDrawing->m_nTextBox=i;
			pKlineDrawing->DrawKlineFiguer(&memdc);
			}
			break;
		case REGION_SELFLINE_1://REGION_SELFLINE_1
		case REGION_SELFLINE_2://REGION_SELFLINE_1
		case REGION_SELFLINE_3://REGION_SELFLINE_1
		case REGION_SELFLINE_BODY://REGION_SELFLINE_1
		case REGION_PICT1://
		case REGION_PICT2:
		case REGION_PICT3:
		case REGION_PICT4:
		case REGION_PICT5:
			//HistoryAStep(int time)
			if(m_pDlgDealHistory != NULL )
			{
				if(m_pDlgDealHistory->m_bHistorySynchrostep == true)
				{		
					int first=pKlineDrawing->m_footBegin + point.x*pKlineDrawing->m_klinNumDefault/m_rtKlineFiguer.rightX;
					HistoryAStep(first);
				}
			}
			TrackCursorBgn(point);
			break;
		case REGION_LITTLE_FIGUER:
			{
			CTaiKlineDC memdc(this,&m_bit);
			//Marenan -
			//m_scrollBar->SetScrollPos(0);
			DrawRightSheet(&memdc, point);
			}
			break;
		case REGION_INFO_TRANSFER:
			if(m_infoFiguer>0)
				m_infoFiguer=0;
			else
				m_infoFiguer=1;
			GetClientRect(r);
			OnSizeMy(r.right -r.left , r.bottom -r.top );
			RedrawWindow();
			break;
		case REGION_ADD:
			{
				int nm;
				if(m_nKlineKind2==0)
				{
					nm=m_infoInit.nCountMin1;
				}
				else
				{
					nm=m_infoInit.nCountIndex;
				}
				if(nm>=FIGUER_ALL_NUM)
					break;
				nm++;
				DoViewItem(nm);
			}
			break;
		case REGION_SUB:
			if(m_nKlineKind2==0)
			{
				int nm=m_infoInit.nCountMin1;
				if(nm<=1)
					break;
				nm--;
				m_infoInit.nCountMin1=nm;
			}
			else
			{
				int nm=m_infoInit.nCountIndex;
				if(nm<=1)
					break;
				nm--;
				m_infoInit.nCountIndex=nm;
			}

			m_infoFiguer=(m_infoFiguer+1)/2;
			GetClientRect(r);
			OnSizeMy(r.right -r.left , r.bottom -r.top );
			RedrawWindow();
			break;
		case REGION_MANY_FIGUER:
			if(m_hit==REGION_MANY_FIGUER)
			{
				m_bMultiFiguer=1;
				RedrawWindow();
			}
			break;
//Marenan +
		case REGION_ZOOM:   //cjg
			{
				if (m_bZoomEnable)
				{
					m_bInfoTabFill=!m_bInfoTabFill;
					GetClientRect(&m_rcZoom);
					m_rcZoom.left=m_rcZoom.right-ZOOMIMG;
					m_rcZoom.bottom -=27;

					if (m_bInfoTabFill)
						m_rcZoom.top =ZOOMIMG+1;
					else
						m_rcZoom.top=RIGHTBOX_LINECOUNT*RIGHTBOX_PERLINE+ZOOMIMG+1;
					if(m_scrollBar->m_hWnd!=NULL)
						m_scrollBar->SetSBRect(m_rcZoom,FALSE) ;
					m_rcZoom.top -=(ZOOMIMG+1);
					m_rcZoom.bottom =m_rcZoom.top+ZOOMIMG; 
					RedrawWindow();
				}
			}
			break;
//Marenan +
		default:
			break;
		}
	}

//	CalcCursorShape(point);	

	/////////---temp-----自画线

	/////////////////////////
	CView::OnLButtonDown(nFlags, point);
}
///////////////////////////////////////////////////////////////////
//      函 数 名：CaclRulorMovable(CPoint point)
//      功能说明：确定鼠标移动到何处
//      入口参数：point为鼠标所在的点
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::CaclRulorMovable(CPoint point)//确定鼠标移动到何处
{
	if(m_bMultiFiguer==1)
	{
		return;
	}
	if(m_nKlineKind2==0)
	{
		int nm=m_infoInit.nCountMin1;

		m_hit=REGION_NO;
		if(point.x<m_rtMin1.leftX )
		{
			m_hit=REGION_NO;
//			return;
		}
		else if(point.x<m_rtMin1.rightX )
		{
			//---子图-----
			if(point.y<m_heightCaption )
			{
				m_hit=REGION_PICTCAPTION1;
				if((m_rtMin1.rightX-point.x)/15<=2)
					m_hit=REGION_INFO_TRANSFER-(m_rtMin1.rightX-point.x)/15;
			}
			if(point.y<m_rtMin1.rtBlw[0].m_yBottom-2 &&
				point.y>=m_heightCaption)
				m_hit=REGION_PICT1;//
			for(int i=1;i<nm;i++)
			{
				if(point.y<m_heightCaption+m_rtMin1.rtBlw[i-1].m_yBottom+1 && 
					point.y>=m_rtMin1.rtBlw[i-1].m_yBottom+2)
					m_hit=REGION_PICTCAPTION1+i;
				if(point.y<m_rtMin1.rtBlw[i].m_yBottom-2 
					&& point.y>=m_heightCaption+m_rtMin1.rtBlw[i-1].m_yBottom+1)
				{
					m_hit=REGION_PICT1+i;//
				}

				if(point.y<m_rtMin1.rtBlw[i-1].m_yBottom+2 && point.y>=m_rtMin1.rtBlw[i-1].m_yBottom-2 )
					m_hit=REGION_PICTBORDER1+i-1;//m_nCountIndex
			}
			//----标尺----
//			if(point.y>m_rtMin1.rtBlw[nm-1].m_yBottom-1 )
//				m_hit=REGION_RULER;//


		}
		else if(point.x<m_rtMin1.rightX+m_midLen -2)
		{
			for(int i=1;i<=nm;i++)
			{
				int scnd;
				if(i==1)
					scnd=0;
				else
					scnd=m_rtMin1.rtBlw[i-2].m_yBottom+2;
				if(point.y<m_rtMin1.rtBlw[i-1].m_yBottom-2 && 
					point.y>=scnd)
					m_hit=REGION_PICTTEXT1+i-1;

				if(i<nm)
				{
					if(point.y<m_rtMin1.rtBlw[i-1].m_yBottom+2 && point.y>=m_rtMin1.rtBlw[i-1].m_yBottom-2 )
						m_hit=REGION_PICTBORDER1+i-1;//m_nCountIndex
				}
			}

			//--		K线类型转换
			if(point.y>m_rtMin1.rtBlw[nm-1].m_yBottom-1 )
				m_hit=REGION_TRANSFER;//
		}
		else 
		{
			if(point.x>m_rtMin1.rightX+m_midLen +2)
			{
				CRect rt;
				GetClientRect(rt);
				if(m_infoFiguer>0)
				{
					//Marenan -
					//if(point.y<RIGHTBOX_PERLINE&&point.x>rt.right-20)
					//	m_hit=REGION_MANY_FIGUER;
					//else if(point.y<rt.bottom &&point.y>rt.bottom-25 )
					//	m_hit=REGION_LITTLE_FIGUER;
					//else
					//	m_hit=REGION_NO;
					//Marenan -

					//Marenan +
					if (m_rcZoom.PtInRect(point))
						m_hit=REGION_ZOOM;
					else if(point.y<rt.bottom &&point.y>rt.bottom-25 )
						m_hit=REGION_LITTLE_FIGUER;
					else
						m_hit=REGION_NO;
					//Marenan +

				}
				else
				{
					m_hit=REGION_NO;
				}
			}
			else
				m_hit=REGION_RIGHT_LINE;
		}
	}
	else
	{

		int nm=m_infoInit.nCountIndex;
		if(point.x<m_rtKlineFiguer.rightX )
		{
			//---子图-----
			if(point.y<m_heightCaption )
			{
				m_hit=REGION_PICTCAPTION1;
				if((m_rtKlineFiguer.rightX-point.x)/15<=2)
					m_hit=REGION_INFO_TRANSFER-(m_rtKlineFiguer.rightX-point.x)/15;
			}
			if(point.y<m_rtKlineFiguer.rtBlw[0].m_yBottom-2 && point.y>=m_heightCaption)
			{
				m_hit=REGION_PICT1;//

				if(lineTypeDrawed == -1)
				{
				if(bTrackCurcorBgn==false)
				{
					int rtn= pDrawLine->IsOnSelfLineTotal(point);
					if(rtn>=0)
					{
						m_hit=rtn+REGION_SELFLINE_1;
					}
				}
				else
				{
					if(pDrawLine->m_bMoveZhongXian==1)
					{
						int rtn= pDrawLine->IsOnSelfLineTotal(point);
						if(rtn>=0)
						{
							m_hit=rtn+REGION_SELFLINE_1;
						}
					}
				}
				}
			}
			for(int i=1;i<nm;i++)
			{
				if(point.y<m_heightCaption+m_rtKlineFiguer.rtBlw[i-1].m_yBottom+1 && 
					point.y>=m_rtKlineFiguer.rtBlw[i-1].m_yBottom+2)
					m_hit=REGION_PICTCAPTION1+i;
				if(point.y<m_rtKlineFiguer.rtBlw[i].m_yBottom-2 
					&& point.y>=m_heightCaption+m_rtKlineFiguer.rtBlw[i-1].m_yBottom+1)
				{
					m_hit=REGION_PICT1+i;//
				}

				if(point.y<m_rtKlineFiguer.rtBlw[i-1].m_yBottom+2 && point.y>=m_rtKlineFiguer.rtBlw[i-1].m_yBottom-2 )
					m_hit=REGION_PICTBORDER1+i-1;//m_nCountIndex
			}
			//----标尺----
			if(point.y>m_rtKlineFiguer.rtBlw[nm-1].m_yBottom-1 )
				m_hit=REGION_RULER;//


		}
		else if(point.x<m_rtKlineFiguer.rightX+m_midLen-2 )
		{
			for(int i=1;i<=nm;i++)
			{
				int scnd;
				if(i==1)
					scnd=0;
				else
					scnd=m_rtKlineFiguer.rtBlw[i-2].m_yBottom+2;
				if(point.y<m_rtKlineFiguer.rtBlw[i-1].m_yBottom-2 && 
					point.y>=scnd)
					m_hit=REGION_PICTTEXT1+i-1;

				if(i<nm)
				{
					if(point.y<m_rtKlineFiguer.rtBlw[i-1].m_yBottom+2 && point.y>=m_rtKlineFiguer.rtBlw[i-1].m_yBottom-2 )
						m_hit=REGION_PICTBORDER1+i-1;//m_nCountIndex
				}
			}

			//--		K线类型转换
			if(point.y>m_rtKlineFiguer.rtBlw[nm-1].m_yBottom-1 )
				m_hit=REGION_TRANSFER;//
		}
		else if(!IsIndexStock(m_sharesSymbol))
		{
			if(point.x>m_rtKlineFiguer.rightX+m_midLen+2 )
			{
				CRect rt;
				GetClientRect(rt);
				if(m_infoFiguer>0)
				{
					//Marenan -
					//if(point.y<RIGHTBOX_PERLINE&&point.x>rt.right-20)
					//	m_hit=REGION_MANY_FIGUER;
					//else if(point.y<rt.bottom &&point.y>rt.bottom-25 )
					//	m_hit=REGION_LITTLE_FIGUER;
					//else
					//	m_hit=REGION_NO;
					//Marenan -

					//Marenan +
					if (m_rcZoom.PtInRect(point))
						m_hit=REGION_ZOOM;
					else if(point.y<rt.bottom &&point.y>rt.bottom-25 )
						m_hit=REGION_LITTLE_FIGUER;
					else
						m_hit=REGION_NO;
					//Marenan +
				}
				else
				{
					m_hit=REGION_NO;
				}
			}
			else
				m_hit=REGION_RIGHT_LINE;
		}
	}

	Sleep(0);

	DrawRulorBoxY(point.y);
	DrawRulorBoxX(point.x);

	ClearCross();
	m_pointMove.x=point.x;
	m_pointMove.y=point.y;
	ShowCross(false);

//	CalcCursorShape(point);	
}

void CTaiShanKlineShowView::OnMouseMove(UINT nFlags, CPoint point) //OnMouseWheel
{
	if(m_bAppAuthorized > 1600)
		Sleep(200);

	// TODO: Add your message handler code here and/or call default
//	if (GetCapture() != this)
//		return;
/*	if(::GetCapture()!=m_hWnd)
		CaclRulorMovable(point);

	if(m_bSetCursor ==TRUE)
	{
		m_bSetCursor=false;
		CView::OnMouseMove(nFlags, point);
		return;
	}*/

	if(m_bMultiFiguer==1)
	{
		CView::OnMouseMove(nFlags, point);
		return;
	}

	int i;
	CTaiShanDoc* pDoc = GetDocument();
	CRect r;
	GetClientRect(r);
	if(::GetCapture()==m_hWnd)
	{

		CPoint p(point);
		switch(m_hit)
		{
		case REGION_PICTBORDER1:
		case REGION_PICTBORDER2:
		case REGION_PICTBORDER3:
		case REGION_PICTBORDER4:
			if(nFlags&MK_LBUTTON)
			{
				CDC* pDC=NULL;//GetDC();
				pDC = GetDC();
				int oldROP = pDC->SetROP2(R2_NOTXORPEN);
				if(m_infoFiguer>0)
					r.right-=m_rightLength;
				int left=1;
				if(m_nKlineKind2==0)
				{
					left=m_rtMin1.leftX;
				}
				for(i=0;i<2;i++)
				{
					if(m_nOldHitPos>=0)
					{
						pDC->MoveTo(left,m_nOldHitPos);
						pDC->LineTo(r.right,m_nOldHitPos);
					}
					m_nOldHitPos = p.y;
				}
				pDC->SetROP2(oldROP);
				i=m_hit-REGION_PICTBORDER1;
				if(m_nKlineKind2==0)
				{
					m_rtMin1.rtBlw[i].m_yBottom=p.y;
				}
				else
				{
					m_rtKlineFiguer.rtBlw[i].m_yBottom=p.y;
				}

				ReleaseDC(pDC);
			}

			break;
		case REGION_RIGHT_LINE:
//			m_nOldHitPos = point.x;
			if(m_infoFiguer>0)
			{
				if(nFlags&MK_LBUTTON)
				{
					CDC* pDC=NULL;//GetDC();
					pDC = GetDC();
					int oldROP = pDC->SetROP2(R2_NOTXORPEN);
					for(i=0;i<2;i++)
					{
						if(m_nOldHitPos>=0)
						{
							pDC->MoveTo(m_nOldHitPos,0);
							pDC->LineTo(m_nOldHitPos,r.bottom );
						}
						m_nOldHitPos = p.x;
					}
					pDC->SetROP2(oldROP);

					ReleaseDC(pDC);
				}
			}
			break;
		case REGION_RULER:
			if(m_nKlineKind2==0)
				break;

 			if(nFlags&MK_LBUTTON)
			{
				if((m_rtKlineFiguer.rightX-point.x)<=0)
					break;
				static CPoint pointOld2;
				if(pointOld2 != point)
				{
					pointOld2 = point;
					SetTimer(TM_MOVE_K,2,NULL);
				}
//				else
		/**///		KillTimer(TM_MOVE_K);//6.17
			}
			else
			{
//				m_nOldHitPos = (int)((m_rtKlineFiguer.rightX-point.x)*pKlineDrawing->m_klinNumDefault /m_rtKlineFiguer.rightX);
				//
				if(m_nOldHitPos==-1)
					m_nOldHitPos= point.x;
				i = (m_nOldHitPos-point.x)*pKlineDrawing->m_klinNumDefault/m_rtKlineFiguer.rightX;
				m_pointMove=point;
				TRACE("MOVE = I  = %d \n",i);
				if(i && pKlineDrawing->MoveDisplays(i))
				{
					if(m_infoFiguer>0)
						r.right-=m_rightLength;
					CTaiKlineDC memdc(this,&m_bit,&r);
					pKlineDrawing->DrawKlineFiguer(&memdc);
					m_nOldHitPos=point.x;
				}
			}
			break;
		case REGION_TRANSFER:
		case REGION_PICTTEXT1:
			break;
		case REGION_SELFLINE_1://REGION_SELFLINE_1
		case REGION_SELFLINE_2://REGION_SELFLINE_1
		case REGION_SELFLINE_3://REGION_SELFLINE_1
		case REGION_SELFLINE_BODY://REGION_SELFLINE_1
		case REGION_PICT1:
		case REGION_PICT2:
		case REGION_PICT3:
		case REGION_PICT4:
		case REGION_PICT5:
			TrackCursorGoOn(point);
			break;
		case REGION_LITTLE_FIGUER:
			break;
		default:
			break;
		}
	}
	else
	{
    	CaclRulorMovable(point);
		if(m_MyToolTip->IsWindowVisible())
		{
			CRect rt;
			rt.left =point.x+2;
			rt.top=point.y+2;
			rt.bottom =point.y+m_MyToolTip->m_maxHeight+40;
			rt.right=point.x+m_MyToolTip->max+50;
			CopyRect(rt);
		}
		static CPoint pointOld;
		if(pointOld != point)
		{
			pointOld = point;
			m_MyToolTip->Hide();
			SetTimer(TM_TIP,450,NULL);
		}
		if(m_tabNum>3&&this->m_infoFiguer >0 && m_tabNum<6)
		{
			if(m_infoInit.nIDRightSheet [m_tabNum-4]>=1 )
			{
				r.left = r.right -this->m_rightLength +1;
				int foot;
				int footCurr; 
				if(m_nKlineKind2 ==0)
				{
					foot=pMin1Drawing->TransferX(point.x);
					footCurr = pMin1Drawing->m_footCurrent;
					if(foot<=pMin1Drawing->m_footEnd)
						pMin1Drawing->m_footCurrent = foot;
					else
						pMin1Drawing->m_footCurrent = pMin1Drawing->m_footEnd;
				}
				else
				{
					foot=pKlineDrawing->TransferX(point.x);
					foot+=pKlineDrawing->m_footBegin;
					footCurr = pKlineDrawing->m_footCurrent;
					if(foot<=pKlineDrawing->m_footEnd)
						pKlineDrawing->m_footCurrent = foot;
					else
						pKlineDrawing->m_footCurrent = pKlineDrawing->m_footEnd;
				}
				CTaiKlineDC memdc(this,&m_bit,&r);
				DrawRightSheet(&memdc,m_tabNum,false);
				if(m_nKlineKind2 ==0)
					pMin1Drawing->m_footCurrent = footCurr;
				else
					pKlineDrawing->m_footCurrent = footCurr;
			}
		}
	}
	
	//new function2.32
	CView* pView;
	BOOL bf;
	bf=FALSE;
	POSITION pos = ((CMainFrame*)AfxGetMainWnd())->m_pDoc->GetFirstViewPosition();   
	CRect rect;
	this->GetClientRect(&rect);
	int posi=rect.right  ;	
	if (posi-point.x<=5)
	{
		
		while (pos != NULL)   
		{
			pView = ((CMainFrame*)AfxGetMainWnd())->m_pDoc->GetNextView(pos);
			if(pView->IsKindOf(RUNTIME_CLASS(CGuiZhouTvView)))
			{
				((CMainFrame*)AfxGetMainWnd())->SetActiveView(pView,TRUE);
				bf=TRUE;
				break;
			}
		}   
//		if(!bf)	
//			((CMainFrame*)AfxGetMainWnd())->ShowMyView();
	}
	else
	{
		while (pos != NULL)   
		{
			pView = ((CMainFrame*)AfxGetMainWnd())->m_pDoc->GetNextView(pos);
			if(pView->IsKindOf(RUNTIME_CLASS(CGuiZhouTvView)))
			{
				::SendMessage(pView->m_hWnd,WM_CLOSE,0,0);
				break;
			}
		}   
		
	}


	CView::OnMouseMove(nFlags, point);

}


void CTaiShanKlineShowView::OnFileSave() 
{	
}

void CTaiShanKlineShowView::OnFileSaveAs() 
{
	// TODO: Add your command handler code here

	
}


void CTaiShanKlineShowView::DrawFenShi(CDC *pDC)
{

		DrawRectPer(pDC);
}

///////////////////////////////////////////////////////////////////
//      函 数 名：DrawRulorBoxX(int x)
//      功能说明：画X轴游标矩形
//      入口参数：x为鼠标所在的点的x坐标,bKey为是否键盘按下
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::DrawRulorBoxX(int x,bool bKey)//6.16
{
	int right_x=0;
	int left_x = 0;
	if(m_nKlineKind2==0)
	{
		right_x=m_rtMin1.rightX;
		left_x = m_rtMin1.leftX ;
	}
	else
		right_x=m_rtKlineFiguer.rightX;
	int nHit = m_hit;
//	if(nHit>=REGION_SELFLINE_1 && nHit<=REGION_SELFLINE_BODY)
//		nHit = REGION_PICT1;
	CRect rt;
	GetClientRect(rt);
	CDC* pDC=GetDC();
	if(m_infoFiguer>0)
	{
//		if(m_pointMove.x>rt.right-m_rightLength-m_midLen)
//		{
//			ReleaseDC(pDC);
//			return;
//		}
	}

	pDC->SetTextAlign( TA_LEFT);//TA_CENTER||
	pDC->SetBkMode(TRANSPARENT);
	CFont ft;
	LOGFONT lgf=GetDocument()->m_fontstr[0];
//	lgf.lfHeight=-12;
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);
	pDC->SetTextColor(GetDocument()->m_colorArray [1]);

	CPen pen;
	if (!pen.CreatePen(PS_SOLID, 1,GetDocument()->m_colorArray[2]))
	{
		ReleaseDC(pDC);
		return ;
	}
	CPen* pOldPen = pDC->SelectObject(&pen);

	if(m_nKlineKind2==0)
	{
		int nm=m_infoInit.nCountMin1;
		CRect rIn(m_pointMove.x-10, rt.bottom-m_heightCaption-6,
			m_pointMove.x-10+2*m_midLen, rt.bottom);
		CTaiKlineDC* pMemdc =new CTaiKlineDC(this,&m_bit,&rIn);
		delete pMemdc;
		CRect rtX(x,m_rtMin1.rtBlw[m_infoInit.nCountMin1-1].m_yBottom+1,x+m_midLen*6/5,rt.bottom+2);
//		pDC->FillRect(rtX,&brush);
		if(x<right_x)
			pDC->FillSolidRect(rtX,GetDocument()->m_colorArray[16]);
//			ClearBack(pDC, rtX);

		if(x>right_x || x<left_x)
		{
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOld);
			ReleaseDC(pDC);
			return;
		}

		pDC->MoveTo(rtX.left,rtX.top-1);
		pDC->LineTo(rtX.right+1,rtX.top-1);

		pDC->MoveTo(rtX.left,rtX.top);
		pDC->LineTo(rtX.left,rt.bottom+1);
		pDC->MoveTo(rtX.right,rtX.top);
		pDC->LineTo(rtX.right,rt.bottom+1);


		if((m_rtMin1.rightX-m_rtMin1.leftX)==0)
		{
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOld);
			ReleaseDC(pDC);
			return;
		}

		int tm=pMin1Drawing->m_klinNumDefault*(x-m_rtMin1.leftX)/(m_rtMin1.rightX-m_rtMin1.leftX);
		CString str;
		if(bKey == TRUE)
			tm=pMin1Drawing ->m_footCurrent-pMin1Drawing ->m_footBegin;
		str=pMin1Drawing->TimeToString(tm);
		pDC->TextOut(x+4,rtX.top ,str);

	}
	else
	{
		int nm=m_infoInit.nCountIndex;
		CRect rIn(m_pointMove.x-10, rt.bottom-m_heightCaption-6,
			m_pointMove.x-10+2.4*m_midLen, rt.bottom);
		CTaiKlineDC* pMemdc =new CTaiKlineDC(this,&m_bit,&rIn);
		delete pMemdc;

		CRect rtX(x,m_rtKlineFiguer.rtBlw[m_infoInit.nCountIndex-1].m_yBottom+1,x+2*m_midLen,rt.bottom+2);
		if(x<right_x)
			pDC->FillSolidRect(rtX,GetDocument()->m_colorArray[16]);
//			ClearBack(pDC, rtX);

		if(x>right_x || x<left_x)
		{
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOld);
			ReleaseDC(pDC);
			return;
		}
		pDC->MoveTo(rtX.left,rtX.top-1);
		pDC->LineTo(rtX.right+1,rtX.top-1);

		pDC->MoveTo(rtX.left,rtX.top);
		pDC->LineTo(rtX.left,rt.bottom+1);
		pDC->MoveTo(rtX.right,rtX.top);
		pDC->LineTo(rtX.right,rt.bottom+1);

		if(m_rtKlineFiguer.rightX==0)
		{
			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOld);
			ReleaseDC(pDC);
			return;
		}
		int tm=pKlineDrawing->m_klinNumDefault*(x)/(m_rtKlineFiguer.rightX);
		CString str;
		if(bKey == TRUE)
			tm=pKlineDrawing ->m_footCurrent-pKlineDrawing ->m_footBegin;
		str=pKlineDrawing->TimeToStr(tm);
		pDC->TextOut(x+2,rtX.top ,str);
	}
	pDC->SelectObject(pOldPen);//pOldPen
	pDC->SelectObject(pOld);
	ReleaseDC(pDC);
	

}
//////////////////////////////////////////////////////////////////
//      函 数 名：DrawRulorBoxY(int y)
//      功能说明：画y轴游标矩形
//      入口参数：y为鼠标所在的点的y坐标
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::DrawRulorBoxY(int y)//6.16
{
	if(pMin1Drawing->m_pReportData==NULL)
		return;

	CRect rt;
	GetClientRect(rt);

	int right_x=0;
	if(m_nKlineKind2==0)
		right_x=m_rtMin1.rightX;
	else
		right_x=m_rtKlineFiguer.rightX;

	CRect rIn(right_x, m_pointMove.y-m_heightCaption,
		right_x+m_midLen+2,  m_pointMove.y+5);
	CTaiKlineDC* pMemdc = new CTaiKlineDC(this,&m_bit,&rIn);
	delete pMemdc;
	int nHit = m_hit;
	if(nHit>=REGION_SELFLINE_1 && nHit<=REGION_SELFLINE_BODY)
		nHit = REGION_PICT1;
	if(nHit>=REGION_PICTBORDER1 && nHit<=REGION_PICTBORDER4)
		nHit = REGION_PICT1+nHit-REGION_PICTBORDER1;
	if(nHit>=REGION_PICTCAPTION1 && nHit<=REGION_PICTCAPTION5)
		nHit = REGION_PICT1+nHit-REGION_PICTCAPTION1;

	if((nHit<REGION_PICT1||nHit>REGION_PICT5))
			return;

	CDC* pDC=GetDC();

	pDC->SetTextAlign( TA_LEFT);//TA_CENTER||
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetDocument()->m_colorArray [1]);
	CFont ft;
	LOGFONT lgf=GetDocument()->m_fontstr[0];
//	lgf.lfHeight=-12;
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);

	CRect rtY(right_x,y-m_heightCaption,right_x+m_midLen+1,y+2);
	pDC->FillSolidRect(rtY,GetDocument()->m_colorArray[16]);
//	ClearBack(pDC, rtY);

	CBrush brush2;
	if (!brush2.CreateSolidBrush(GetDocument()->m_colorArray[2]))
		ASSERT(FALSE);
	CBrush* pOldBrush = pDC->SelectObject(&brush2);
	pDC->FrameRect(rtY,&brush2);

	//--	计算字符串
	float max;
	float min;
	int top;
	int bottom;
	int numSon=nHit-REGION_PICT1;
	if(m_nKlineKind2==0)
	{
		max=pMin1Drawing->m_max_sun[numSon];
		min=pMin1Drawing->m_min_sun[numSon];
		if(numSon==0)
			top=m_heightCaption;
		else
			top=m_rtMin1.rtBlw[numSon-1].m_yBottom+m_heightCaption;
		bottom=m_rtMin1.rtBlw[numSon].m_yBottom;
	}
	else
	{
		max=pKlineDrawing->m_max_sun[numSon];
		min=pKlineDrawing->m_min_sun[numSon];
		CRect r3;
		pKlineDrawing->GetRect(r3, numSon);
		top=r3.top;
		bottom=r3.bottom;
	}

	float vlNow=0;
	if(top-bottom!=0)
	{
		vlNow=(y-bottom)*(max-min)/(top-bottom)+min;
//		TRACE("VLNOW  =  %f max = %f  min= %f\n",vlNow,max ,min );
		if(m_nKlineKind2>0 && numSon==0)
		{
			if(pKlineDrawing->m_axisType==2)
			{
				if(max>0&&min>0)
					vlNow=(float)(pow(10,(log10(max)-log10(min))*(y-bottom)/(top-bottom)+log10(min)));
			}
		}
	}

	m_currentValue=vlNow;
	CString str;
	if(vlNow<100)
	{
		CString fmt = "%.2f";
		if(CSharesCompute::GetMarketKind(m_stkKind ) == SH_MARKET_EX  && pMin1Drawing ->m_pReportData ->kind == 2)
			fmt = "%.3f";
		str.Format(fmt,vlNow);
	}
	else
		str.Format("%d",(int)vlNow);

//	pDC->TextOut(right_x+2,y-15 ,str);
	pDC->ExtTextOut (right_x+2,y-m_heightCaption+1 ,ETO_CLIPPED,rtY,str,NULL);

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOld);
	ReleaseDC(pDC);

}

///////////////////////////////////////////////////////////////////
//      函 数 名：CalcCursorShape(CPoint pt)
//      功能说明：计算鼠标处的鼠标形状
//      入口参数：pt为鼠标所在的点
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::CalcCursorShape(CPoint pt)
{
	if(m_bMultiFiguer==1)
	{
		SetCursor(LoadCursor(  NULL, IDC_ARROW )); // name string or cursor resource identifier);
		return;
	}

	switch(m_hit)
	{
	case REGION_PICTBORDER1:
	case REGION_PICTBORDER2:
	case REGION_PICTBORDER3:
	case REGION_PICTBORDER4:
		SetCursor(LoadCursor( AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDC_CURSOR_SPLIT) )); 
		break;
	case REGION_RIGHT_LINE:
		SetCursor(LoadCursor( AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDC_CURSOR_3) )); 
		break;
	case REGION_RULER:
		SetCursor(LoadCursor( AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDC_CURSOR_MOVE) )); 
		break;
	case REGION_TRANSFER:
	case REGION_PICTTEXT1:
		SetCursor(LoadCursor(  AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDC_REDARROW) )); 
		break;
		//REGION_INFO_TRANSFER
	case REGION_INFO_TRANSFER:
		SetCursor(LoadCursor(  AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDC_CURSOR_INFO) )); 
		break;
	case REGION_ADD:
		SetCursor(LoadCursor(  AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDC_CURSOR_ADD) )); 
		break;
	case REGION_SUB:
		SetCursor(LoadCursor(  AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDC_CURSOR_SUB) )); 
		break;
	case REGION_SELFLINE_BODY:
		SetCursor(LoadCursor(  AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDC_MOVE_BODY) )); 
		break;
	case REGION_SELFLINE_1:
	case REGION_SELFLINE_2:
	case REGION_SELFLINE_3:
		SetCursor(LoadCursor(  AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDC_MOVE_POINT) )); 
		break;
	case REGION_MANY_FIGUER:
		SetCursor(LoadCursor(  AfxGetInstanceHandle( ),MAKEINTRESOURCE(ID_CURSOR_MULTI) )); 
		break;
	case REGION_PICT1:
	case REGION_PICT2:
	case REGION_PICT3:
	case REGION_PICT4:
	case REGION_PICT5:
		if(this->m_nKlineKind2 >0 && pKlineDrawing->m_kLine_mode == 1)
		{
		SetCursor(LoadCursor( AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDC_CURSOR_DRAG) )); 
		break;
		}
	default:
		SetCursor(LoadCursor(  NULL, IDC_ARROW )); // name string or cursor resource identifier);
		break;
	}
	Sleep(0);
}


///////////////////////////////////////////////////////////////////
//      函 数 名：DrawRightText(CDC *pDC,float vl,int x, int yNum, int flag)
//      功能说明：画字符串
//      入口参数：vl为输入的浮点数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::DrawRightText(CDC *pDC,float vl,int x, int yNum, int flag,int floatBit)
//flag=0为画浮点数，flag=1为画整数(在外面设颜色)
{

	CFont ft;
	LOGFONT lgf=GetDocument()->m_fontstr[0];
	lgf.lfHeight=-14;
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);

	pDC->SetTextAlign( TA_RIGHT);//TA_CENTER||
	pDC->SetBkMode(TRANSPARENT);
	int modify=3;
	CString s;

	float cls;
	cls=pMin1Drawing->m_close;
//	else
//		cls=pMin1Drawing->m_close_Dapan;

	if(flag==0||flag==2)
	{
		if(flag==0)
		{
			if(floatBit == 2)
				s.Format("%.2f",vl);
			else
				s.Format("%.3f",vl);

			if(vl>cls)
				pDC->SetTextColor( GetDocument()->m_colorArray[13]);
			else if(vl<cls)
				pDC->SetTextColor( GetDocument()->m_colorArray[XIA_DIE_WORD]);
			else
				pDC->SetTextColor( GetDocument()->m_colorArray[PING_PAN_WORD]);
		}
		else
		{
			s.Format("%.1f",vl);
		}

		pDC->TextOut(x,RIGHTBOX_PERLINE*yNum+modify,s);
	}
	else
	{
		s.Format("%d",(int)(vl+0.5));
		pDC->TextOut(x,RIGHTBOX_PERLINE*yNum+modify,s);
	}

	pDC->SelectObject(pOld);

}
///////////////////////////////////////////////////////////////////
//      函 数 名：DrawRightbox(CDC *pDC)
//      功能说明：画右边信息框
//      入口参数：pDC为设备描述表的指针
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::DrawRightbox(CDC *pDC,bool bClearAll)
{
	if(pMin1Drawing->m_pReportData==NULL)
		return;
	CString sN=m_sharesSymbol ;
	if(IsIndexStock(m_sharesSymbol))
	{
		m_infoFiguer=2;
		DrawRightBoxDapan(pDC,bClearAll);
		return;
	}
	m_infoFiguer=1;

	CRect rt;
//	GetClientRect(rt);
	GetCurrClientRect(rt);
	//Marenan -
	//CRect rt1(rt.right-m_rightLength+1,0,rt.right,19*RIGHTBOX_PERLINE);
	//ClearBack(pDC, rt1,pMin1Drawing->m_bHist);
	//Marenan -
	//Marenan +
	if (m_bInfoTabFill)
	{
		CRect rt1(rt.right-m_rightLength+1,0,rt.right,1);
		ClearBack(pDC, rt1,pMin1Drawing->m_bHist);
	}
	else
	{
		CRect rt1(rt.right-m_rightLength+1,0,rt.right,RIGHTBOX_LINECOUNT*RIGHTBOX_PERLINE);

		ClearBack(pDC, rt1,pMin1Drawing->m_bHist);
		//Marenan +

		///////////---画线
		CPen pen_line(PS_SOLID  ,1,GetDocument()->m_colorArray[2]);//PS_DASHDOTDOT   
		CPen* pOldpen=pDC->SelectObject(&pen_line);
		if(pDC->IsPrinting ())
		{
			pDC->MoveTo (0,0);
			pDC->LineTo (rt.right-1,0);
			pDC->LineTo (rt.right-1,rt.bottom);
			pDC->LineTo (0,rt.bottom);
			pDC->LineTo (0,0);
		}

		pDC->MoveTo(rt.right-m_rightLength,RIGHTBOX_PERLINE);
		pDC->LineTo(rt.right,RIGHTBOX_PERLINE);
		pDC->MoveTo(rt.right-m_rightLength,RIGHTBOX_PERLINE*2);
		pDC->LineTo(rt.right,RIGHTBOX_PERLINE*2);
		pDC->MoveTo(rt.right-m_rightLength,RIGHTBOX_PERLINE*7);
		pDC->LineTo(rt.right,RIGHTBOX_PERLINE*7);
		pDC->MoveTo(rt.right-m_rightLength,RIGHTBOX_PERLINE*12);
		pDC->LineTo(rt.right,RIGHTBOX_PERLINE*12);
		pDC->MoveTo(rt.right-m_rightLength,RIGHTBOX_PERLINE*18);
		pDC->LineTo(rt.right,RIGHTBOX_PERLINE*18);
		pDC->MoveTo(rt.right-m_rightLength,RIGHTBOX_PERLINE*19);
		pDC->LineTo(rt.right,RIGHTBOX_PERLINE*19);

		//--	汉字
		pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		pDC->SetTextColor( GetDocument()->m_colorArray[20]);
		//pDC->SetTextColor( GetDocument()->m_colorArray[FIGUER_WORD]);
		pDC->SetBkMode(TRANSPARENT);

		CFont ft2;
		LOGFONT lgf2=GetDocument()->m_fontstr[0];//m_fontstr[0]
		lgf2.lfHeight=-14;
		lgf2.lfWeight =FW_BOLD;
		ft2.CreateFontIndirect (&lgf2 );
		CFont* pOld=pDC->SelectObject(&ft2);

		int modify=3;
		int lf=4;
		CString s=m_sharesSymbol;
		int len=(pDC->GetOutputTextExtent(s)).cx;
	//	pDC->TextOut(rt.right-m_rightLength+lf+4,0,s);
	//	s=GetDocument()->m_stkName;
		s=s+"  ";
		s+=pMin1Drawing->m_pReportData->name;
	//	pDC->TextOut(rt.right-m_rightLength+lf+8+len,modify-2,s);
		CRect rTop;
		rTop.left=rt.right-m_rightLength+4;
		rTop.top=0;
		rTop.right=rt.right;
		rTop.bottom=RIGHTBOX_PERLINE;

		pDC->DrawText(s,rTop,DT_LEFT|DT_VCENTER);

		pDC->SelectObject(pOld);

		CFont ft;
		//Marenan +
		LOGFONT lgf=GetDocument()->m_fontstr[0];
		//Marenan -
		//LOGFONT lgf=GetDocument()->m_fontstr[1];
		lgf.lfHeight=-14;
		ft.CreateFontIndirect (&lgf );
		pOld=pDC->SelectObject(&ft);
		pDC->SetTextColor( GetDocument()->m_colorArray[FIGUER_WORD]);

		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*2+modify,"卖⑤");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*3+modify,"卖④");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*4+modify,"卖③");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*5+modify,"卖②");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*6+modify,"卖①");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*7+modify,"买①");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*8+modify,"买②");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*9+modify,"买③");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*10+modify,"买④");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*11+modify,"买⑤");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*12+modify,"最新");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*13+modify,"涨跌");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*14+modify,"幅度");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*15+modify,"总手");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*16+modify,"现手");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*17+modify,"金额");
		pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*18+modify,"外盘");
		pDC->TextOut(rt.right-m_rightLength/2+lf,RIGHTBOX_PERLINE*12+modify,"均价");
		pDC->TextOut(rt.right-m_rightLength/2+lf,RIGHTBOX_PERLINE*13+modify,"昨收");
		pDC->TextOut(rt.right-m_rightLength/2+lf,RIGHTBOX_PERLINE*14+modify,"今开");
		pDC->TextOut(rt.right-m_rightLength/2+lf,RIGHTBOX_PERLINE*15+modify,"最高");
		pDC->TextOut(rt.right-m_rightLength/2+lf,RIGHTBOX_PERLINE*16+modify,"最低");
		pDC->TextOut(rt.right-m_rightLength/2+lf,RIGHTBOX_PERLINE*17+modify,"量比");
		pDC->TextOut(rt.right-m_rightLength/2+lf,RIGHTBOX_PERLINE*18+modify,"内盘");

		if((pMin1Drawing->m_pReportData->higp)<0 || (pMin1Drawing->m_pReportData->lowp)<0 ||(pMin1Drawing->m_pReportData->nowp )<=0||(pMin1Drawing->m_pReportData->opnp )<0||(pMin1Drawing->m_pReportData->totv  )<0)
		{
			pDC->SelectObject(pOldpen);
			pDC->SelectObject(pOld);
			return;
		}

		int floatBit = 2;
		if(CSharesCompute::GetMarketKind(m_stkKind ) == SH_MARKET_EX  && pMin1Drawing->m_pReportData->kind == 2)
			floatBit =3 ;
		//--	数值
		float f1;
		s="";
		{
			pDC->SetTextColor( GetDocument()->m_colorArray[FIGUER_WORD]);
			float ltp = GetCapital(pMin1Drawing->m_pReportData);
			if(ltp >0)
			{
				f1 = pMin1Drawing->m_pReportData->totv /ltp*100 ;
				s.Format("换手率 %.2f",f1);
				s+="%";
			}
			s+=" 委比";
			pDC->TextOut(rt.right-m_rightLength+1,RIGHTBOX_PERLINE+modify,s);
		}

		pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
		if(pMin1Drawing->m_pReportData->accb+pMin1Drawing->m_pReportData->accs>0)
		{
			f1=(pMin1Drawing->m_pReportData->accb-pMin1Drawing->m_pReportData->accs)*100/
			(pMin1Drawing->m_pReportData->accb+pMin1Drawing->m_pReportData->accs);
			if(f1>0)
				pDC->SetTextColor( GetDocument()->m_colorArray[13]);
			else if(f1<0)
				pDC->SetTextColor( GetDocument()->m_colorArray[XIA_DIE_WORD]);
			else
				pDC->SetTextColor( GetDocument()->m_colorArray[PING_PAN_WORD]);

			s.Empty();
			s.Format("%d",(int)f1);
			s+="%";
			pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
			pDC->TextOut(rt.right-lf,RIGHTBOX_PERLINE+modify,s);
		}

		f1=(pMin1Drawing->m_pReportData->psel5);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-m_rightLength/2,2,0,floatBit);
			f1=(pMin1Drawing->m_pReportData->vsel5);
			DrawRightText(pDC,f1,rt.right-lf,2,1);
		}
		f1=(pMin1Drawing->m_pReportData->psel4);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-m_rightLength/2,3,0,floatBit);
			f1=(pMin1Drawing->m_pReportData->vsel4);
			DrawRightText(pDC,f1,rt.right-lf,3,1);
		}

		f1=(pMin1Drawing->m_pReportData->psel3);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-m_rightLength/2,4,0,floatBit);
			f1=(pMin1Drawing->m_pReportData->vsel3);
			DrawRightText(pDC,f1,rt.right-lf,4,1);
		}

		f1=(pMin1Drawing->m_pReportData->psel2);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-m_rightLength/2,5,0,floatBit);
			f1=(pMin1Drawing->m_pReportData->vsel2);
			DrawRightText(pDC,f1,rt.right-lf,5,1);
		}

		f1=(pMin1Drawing->m_pReportData->psel1);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-m_rightLength/2,6,0,floatBit);
			f1=(pMin1Drawing->m_pReportData->vsel1);
			DrawRightText(pDC,f1,rt.right-lf,6,1);
		}

		f1=(pMin1Drawing->m_pReportData->pbuy1);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-m_rightLength/2,7,0,floatBit);
			f1=(pMin1Drawing->m_pReportData->vbuy1);
			DrawRightText(pDC,f1,rt.right-lf,7,1);
		}
		f1=(pMin1Drawing->m_pReportData->pbuy2);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-m_rightLength/2,8,0,floatBit);
			f1=(pMin1Drawing->m_pReportData->vbuy2);
			DrawRightText(pDC,f1,rt.right-lf,8,1);
		}
		f1=(pMin1Drawing->m_pReportData->pbuy3);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-m_rightLength/2,9,0,floatBit);
			f1=(pMin1Drawing->m_pReportData->vbuy3);
			DrawRightText(pDC,f1,rt.right-lf,9,1);
		}
		f1=(pMin1Drawing->m_pReportData->pbuy4);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-m_rightLength/2,10,0,floatBit);
			f1=(pMin1Drawing->m_pReportData->vbuy4);
			DrawRightText(pDC,f1,rt.right-lf,10,1);
		}
		f1=(pMin1Drawing->m_pReportData->pbuy5);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-m_rightLength/2,11,0,floatBit);
			f1=(pMin1Drawing->m_pReportData->vbuy5);
			DrawRightText(pDC,f1,rt.right-lf,11,1);
		}

		f1=(pMin1Drawing->m_pReportData->nowp);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-m_rightLength/2,12,0,floatBit);
		}
		if(GetPriceAve(f1))
		{
			DrawRightText(pDC,f1,rt.right-lf,12,0,floatBit);
		}
		f1=(pMin1Drawing->m_pReportData->nowp-pMin1Drawing->m_pReportData->ystc);
		s.Empty();
		if(pMin1Drawing->m_pReportData->nowp>5)
			s.Format("%.2f",f1);
		else
			s.Format("%.3f",f1);
		pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
		if(f1>0)
			pDC->SetTextColor( GetDocument()->m_colorArray[13]);
		else if(f1<0)
			pDC->SetTextColor( GetDocument()->m_colorArray[XIA_DIE_WORD]);
		else
			pDC->SetTextColor( GetDocument()->m_colorArray[PING_PAN_WORD]);
		//pDC->TextOut(rt.right-m_rightLength/2,RIGHTBOX_PERLINE*13+modify,s);
		DrawRightText(pDC,atof(s),rt.right-m_rightLength/2,13,0,floatBit);
		f1=(pMin1Drawing->m_pReportData->ystc);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-lf,13,0,floatBit);
		}
		f1=(pMin1Drawing->m_pReportData->nowp-pMin1Drawing->m_pReportData->ystc)*100/pMin1Drawing->m_pReportData->ystc;
		if(pMin1Drawing->m_pReportData->ystc>0)
		{
			s.Empty();
			s.Format("%.2f",f1);
			s+="%";
			pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
			if(f1>0)
				pDC->SetTextColor( GetDocument()->m_colorArray[13]);
			else if(f1<0)
				pDC->SetTextColor( GetDocument()->m_colorArray[XIA_DIE_WORD]);
			else
				pDC->SetTextColor( GetDocument()->m_colorArray[PING_PAN_WORD]);
			//pDC->TextOut(rt.right-m_rightLength/2,RIGHTBOX_PERLINE*14+modify,s);
			DrawRightText(pDC,atof(s),rt.right-m_rightLength/2,14,0,floatBit);
		}
		f1=(pMin1Drawing->m_pReportData->opnp);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-lf,14,0,floatBit);
		}
		//---总手
		f1=(pMin1Drawing->m_pReportData->totv);
		if(f1>0)
		{
			if(pMin1Drawing->m_pReportData->nowp-pMin1Drawing->m_pReportData->ystc>0)
				pDC->SetTextColor( GetDocument()->m_colorArray[13]);
			else if(pMin1Drawing->m_pReportData->nowp-pMin1Drawing->m_pReportData->ystc<0)
				pDC->SetTextColor( GetDocument()->m_colorArray[XIA_DIE_WORD]);
			else
				pDC->SetTextColor( GetDocument()->m_colorArray[PING_PAN_WORD]);
			DrawRightText(pDC,f1,rt.right-m_rightLength/2,15,1);
		}
		f1=(pMin1Drawing->m_pReportData->higp);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-lf,15,0,floatBit);
		}
		{
			f1=(pMin1Drawing->m_pReportData->nowv);
			if(f1>0)
			{
					int nFlag = pMin1Drawing->m_pReportData->InOut ;
					if(nFlag==0)
						pDC->SetTextColor( GetDocument()->m_colorArray[13]);
					else if(nFlag==1)
					{
						pDC->SetTextColor( GetDocument()->m_colorArray[XIA_DIE_WORD]);
					}
					else
					{
						pDC->SetTextColor( GetDocument()->m_colorArray[PING_PAN_WORD]);
					}

				DrawRightText(pDC,f1,rt.right-m_rightLength/2,16,1);
			}
		}
		f1=(pMin1Drawing->m_pReportData->lowp);
		if(f1>0)
		{
			DrawRightText(pDC,f1,rt.right-lf,16,0,floatBit);
		}
		f1=(pMin1Drawing->m_pReportData->totp)/10000;
		if(f1>0)
		{
			float f3=(pMin1Drawing->m_pReportData->nowp-pMin1Drawing->m_pReportData->ystc);

			if(f3>0)
				pDC->SetTextColor( GetDocument()->m_colorArray[13]);
			else if(f3<0)
				pDC->SetTextColor( GetDocument()->m_colorArray[XIA_DIE_WORD]);
			else
				pDC->SetTextColor( GetDocument()->m_colorArray[PING_PAN_WORD]);

			DrawRightText(pDC,f1,rt.right-m_rightLength/2,17,2);
		}

	//(m_pReportData->totv)*240/(pMin1Drawing->m_footEnd)/(m_pReportData->volume5 );
		if((pMin1Drawing->m_pReportData->volume5 )>0&&(pMin1Drawing->m_footEnd)>0)
		{
			int nFoot = pMin1Drawing->m_footEnd;
			{
			if(m_isShowCross!=0 && this->m_nKlineKind2 == 0)//6.18
				nFoot = pMin1Drawing->m_footCurrent;
			if(nFoot > pMin1Drawing->m_footEnd)
				nFoot = pMin1Drawing->m_footEnd;
			}

			if(nFoot>0)
				f1=(pMin1Drawing->m_pReportData->totv*240/(nFoot)/(pMin1Drawing->m_pReportData->volume5 ));
			else 
				f1 = 0;
			if(f1>0)
			{
				DrawRightText(pDC,f1,rt.right-lf,17,0,floatBit);
			}
		}

		if(m_isShowCross==0|| m_nKlineKind2>=1 && m_nKlineKind2<9)//6.18
		{
			int nHs = m_hs.GetCount();
			f1 = 0;
			for(int j=0;j<nHs;j++)
			{
				POSITION pos=m_hs.FindIndex(nHs - j - 1 ) ;
				TRADE_DETAIL_H_PER* pHs=m_hs.GetAt( pos );
				TRADE_DETAIL_H_PER* pHsPre = NULL;
				if(j>0)
				{
				pos=m_hs.FindIndex(nHs - j  ) ;
				pHsPre=m_hs.GetAt( pos );
				}

				CString s;
				float vl=pHs->price;
				int floatBit = 2;
				int nFlag ;
				float fVol = CTaiKlineTransferKline::GetVolHs(pHs, pHsPre, nFlag);
				if(nFlag>0)
				{
					f1+=fVol;
				}
				else if(nFlag==0)
				{
					f1+=(fVol/2);//◆*↓
				}
			}
		}
		else
			f1=(pMin1Drawing->m_pReportData->rvol);

		pDC->SetTextColor( GetDocument()->m_colorArray[13]);
		DrawRightText(pDC,f1,rt.right-m_rightLength/2,18,1);
		f1=float(int(pMin1Drawing->m_pReportData ->totv+0.5) - int(f1+0.5));
		pDC->SetTextColor( GetDocument()->m_colorArray[XIA_DIE_WORD]);
		DrawRightText(pDC,f1,rt.right-lf,18,1);
		pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
	 

		///////////---画属性页

		pDC->SelectObject(pOldpen);
	pDC->SelectObject(pOld);

	}
}



void CTaiShanKlineShowView::OnLineself() 
{
}

void CTaiShanKlineShowView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bMultiFiguer==1)
	{
		CView::OnLButtonUp(nFlags, point);
		return;
	}

	if(::GetCapture()==m_hWnd && !(nFlags&(MK_LBUTTON|MK_RBUTTON)))
	{
		ReleaseCapture();
		::ClipCursor(NULL);

		CTaiShanDoc* pDoc = GetDocument();
		CClientDC dc(this);
		CRect r;
		GetClientRect(r);
		CPoint p(point);	
		
		switch(m_hit)
		{
		case REGION_PICTBORDER1:
		case REGION_PICTBORDER2:
		case REGION_PICTBORDER3:
		case REGION_PICTBORDER4:
			pDrawLine->m_bDrawLine=false;
			lineTypeDrawed = -1;
			pDrawLine->m_lineTypeOld=-1;
			if(m_nOldHitPos)
			{
				SetPictYPos(m_hit-REGION_PICTBORDER1,p.y);
				if(m_infoFiguer>0)
					r.right-=m_rightLength;
//				OnDraw(&dc);
				{
					CTaiKlineDC memdc(this,&m_bit);
					pKlineDrawing->DrawKlineFiguer(&memdc);
				}
				if (m_tabNum==6)
				{
					m_pRichEdit->RedrawWindow();
				}
			}
			break;
		case REGION_RIGHT_LINE:
			if(m_infoFiguer==0) break;

			pDrawLine->m_bDrawLine=false;
			lineTypeDrawed = -1;
			pDrawLine->m_lineTypeOld=-1;
			if(m_nOldHitPos>0&&p.x>0 &&  r.right - p.x>0)
			{
				m_rightLength = r.right - p.x;
				OnSizeMy(r.Width (), r.Height ());
				{
					RedrawWindow();
				}
				if (m_tabNum==6)
				{
					m_pRichEdit->RedrawWindow();
				}
			}
			break;

		case REGION_RULER:
			break;
		case REGION_TRANSFER:
		case REGION_PICTTEXT1:
			break;
		case REGION_SELFLINE_1://REGION_SELFLINE_1
		case REGION_SELFLINE_2://REGION_SELFLINE_1
		case REGION_SELFLINE_3://REGION_SELFLINE_1
		case REGION_SELFLINE_BODY://REGION_SELFLINE_1
		case REGION_PICT1:
		case REGION_PICT2:
		case REGION_PICT3:
		case REGION_PICT4:
		case REGION_PICT5:
			TrackCursorEnd(point,false);
			break;
		case REGION_LITTLE_FIGUER:
			break;
		default:
			break;
		}
	}

	if(m_hit == REGION_NO)
		if(m_infoFiguer>0)
		{
			CRect r;
			GetClientRect(r);
			if(point.x>r.right - this->m_rightLength )
			{
				if(point.y<16)
				{
					if(point.x<r.right - this->m_rightLength +16*4)
					{
						int n = ( point.x-(r.right - this->m_rightLength))/16;
						if(n>=0 && n<3)
						{
							this->pKlineDrawing->m_pMoveCB->m_nShowKind = n;
							RedrawWindow();
						}
						if(n==3)
						{
							BjDlgChengBenSetting dlg;
							if(dlg.DoModal () == IDOK)
							{
								this->pKlineDrawing->m_pMoveCB->ReadInSettings ();
								RedrawWindow();
							}
						}
					}
				}
			}
		}
	CView::OnLButtonUp(nFlags, point);

}

void CTaiShanKlineShowView::OnKCacl() 
{
	// TODO: Add your command handler code here



}


void CTaiShanKlineShowView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(IsWindowVisible()!=TRUE)
		return;


	int i;
	CRect r;
	GetClientRect(r);
	POINT p;
	CPoint point;
	switch(nIDEvent)
	{
	case TM_REDRAW_WINDOW://
		KillTimer(TM_REDRAW_WINDOW);
	case TM_FENSHI://
		{
			CReportData* pdt = NULL;
			if(GetDocument()->m_sharesInformation.Lookup(this->m_sharesSymbol .GetBuffer (0),pdt,m_stkKind)==0)
				break;

			if(pdt->totv > this->pMin1Drawing->m_volPre) 
				RedrawWindow();
		}
		break;
	case TM_START://TM_TRACK_CURCOR
		KillTimer(TM_START);
		SetTimer(TM_FENSHI,2000,NULL);
		break;
	case TM_TRACK_CURCOR://TM_MOVE_SHIZI
		KillTimer(TM_TRACK_CURCOR);
		GetCursorPos(&p);
		ScreenToClient(&p);
		point.x=p.x;
		point.y=p.y;
		TrackCursorEnd(point,TRUE);//GetCursorPos(  LPPOINT lpPoint   // address of structure for cursor position);
		break;
	case TM_MOVE_SHIZI://
		KillTimer(TM_MOVE_SHIZI);
		ShowCross();
		break;
	case TM_RULER:
		if(m_nKlineKind2==0)
			return;
		if(m_infoFiguer>0)
				r.right-=m_rightLength;
		if(m_pointMove.x>m_rtKlineFiguer.rightX-8)
		{
			pKlineDrawing->MoveDisplays(-pKlineDrawing->m_klinNumDefault/5);
		}
		else if(m_pointMove.x<8&&m_nOldHitPos>=0)
		{
			pKlineDrawing->MoveDisplays(pKlineDrawing->m_klinNumDefault/5);
		}
		{
		CTaiKlineDC memdc(this,&m_bit,&r);
		pKlineDrawing->DrawKlineFiguer(&memdc);
		}
		break;
	case 12:
        if(m_bMultiFiguer == 0)	
		  OnPageDown(TRUE);
	    else
          OnPageDown(TRUE,4);
		return;
	case TM_TIP:
		KillTimer(TM_TIP);
		if(m_hit<REGION_PICT1||m_hit>REGION_PICT5)
				return;
		if(m_nKlineKind2>0)
		{
			i = m_hit<=REGION_PICT5 ? m_hit-REGION_PICT1 : 0;
			if(pKlineDrawing->OutString(i, m_pointMove)==0)
			{
				m_MyToolTip->Hide();
			}
			else
			{
				m_MyToolTip->Show(pKlineDrawing->m_tipStr,m_pointMove);
			}
		}
		else
		{
			i = m_hit<=REGION_PICT5 ? m_hit-REGION_PICT1 : 0;
			if(pMin1Drawing->OutString(i, m_pointMove)==0)
			{
				m_MyToolTip->Hide();
			}
			else
			{
				m_MyToolTip->Show(pMin1Drawing->m_tipStr,m_pointMove);
			}
		}
		break;
	case TM_UP_K:
		KillTimer(TM_UP_K);//6.17
		if(m_infoFiguer>0)
			r.right-=m_rightLength;
		pDrawLine->m_bDrawLine=false;
		lineTypeDrawed = -1;
		pDrawLine->m_lineTypeOld=-1;

		pKlineDrawing->m_klinNumDefaultOld=pKlineDrawing->m_klinNumDefault;
		pKlineDrawing->m_footEndOld=pKlineDrawing->m_footEnd;
		pKlineDrawing->m_footBeginOld=pKlineDrawing->m_footBegin;
		(pKlineDrawing->OnUp());
		bExtend=TRUE;
//		CopyRect(r);	
		break;
	case TM_DOWN_K:
		KillTimer(TM_DOWN_K);//6.17
		if(m_infoFiguer>0)
			r.right-=m_rightLength;
		pDrawLine->m_bDrawLine=false;
		lineTypeDrawed = -1;
		pDrawLine->m_lineTypeOld=-1;

		(pKlineDrawing->OnDown());
//		CopyRect(r);	
		break;
	case TM_MOVE_K:
		KillTimer(TM_MOVE_K);//6.17
		if(::GetCapture()!=m_hWnd)
			break;

		GetCursorPos(&p);
		ScreenToClient(&p);
		i = m_nOldHitPos*m_rtKlineFiguer.rightX/(m_rtKlineFiguer.rightX-p.x);
		if(i>=10 && pKlineDrawing->SetShowTotal(i))
		{
			if(m_infoFiguer>0)
				r.right-=m_rightLength;
			CTaiKlineDC memdc(this,&m_bit,&r);
			pKlineDrawing->DrawKlineFiguer(&memdc);
		}	
		break;
	case TM_MOVE_DRAG_K:
		KillTimer(TM_MOVE_DRAG_K);//6.17

		if(TRUE)
		{
			GetCursorPos(&p);
			ScreenToClient(&p);
			i = m_hit<=REGION_PICT5 && m_hit>=REGION_PICT1? m_hit-REGION_PICT1 : -1;
			if(i == -1)
				break;

			pKlineDrawing->m_nSon = i;
			CPoint pIn(m_rectHitPos.left,m_rectHitPos.top);
			CPoint p2(p);
			pKlineDrawing->MoveDragMode(pIn,p2);
			m_rectHitPos.left = p.x;
			m_rectHitPos.top = p.y;
		}
		break;
	case TM_SHOW_DLG:
		KillTimer(TM_SHOW_DLG);//6.17
		ShowDlgCross(m_bActived);

	default:
			break;
	}

	CWnd* pWnd = GetFocus();
	if(pWnd!= this)
	{
		m_MyToolTip->Hide();
	}
	CView::OnTimer(nIDEvent);
}

void CTaiShanKlineShowView::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CView::OnShowWindow(bShow, nStatus);
	// TODO: Add your message handler code here
}

BOOL CTaiShanKlineShowView::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
//	AfxMessageBox("DestroyWindow");
	return CView::DestroyWindow();
}


///////////////////////////////////////////////////////////////////
//      函 数 名：DrawRightBoxDapan(CDC *pDC)
//      功能说明：画右边大盘信息框
//      入口参数：pDC为设备描述表的指针
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::DrawRightBoxDapan(CDC *pDC,bool bClearAll)
{
	CRect rt;
	GetCurrClientRect(rt);
	CRect rt1(rt.right-m_rightLength+1,0,rt.right,rt.bottom );
	if(bClearAll==false)
		rt1=CRect(rt.right-m_rightLength+1,0,rt.right,RIGHTBOX_PERLINE*17);
	ClearBack(pDC, rt1,pMin1Drawing->m_bHist);

	//---画线
	CPen pen_line(PS_SOLID  ,1,GetDocument()->m_colorArray[2]);//PS_DASHDOTDOT   
	CPen* pOldpen=pDC->SelectObject(&pen_line);
	if(pDC->IsPrinting ())
	{
	pDC->MoveTo (0,0);
	pDC->LineTo (rt.right-1,0);
	pDC->LineTo (rt.right-1,rt.bottom);
	pDC->LineTo (0,rt.bottom);
	pDC->LineTo (0,0);
	}

	pDC->MoveTo(rt.right-m_rightLength,RIGHTBOX_PERLINE);
	pDC->LineTo(rt.right,RIGHTBOX_PERLINE);
	pDC->MoveTo(rt.right-m_rightLength,RIGHTBOX_PERLINE*10);
	pDC->LineTo(rt.right,RIGHTBOX_PERLINE*10);
	pDC->MoveTo(rt.right-m_rightLength,RIGHTBOX_PERLINE*17);
	pDC->LineTo(rt.right,RIGHTBOX_PERLINE*17);
	pDC->MoveTo(rt.right-m_rightLength,RIGHTBOX_PERLINE*18);
	pDC->LineTo(rt.right,RIGHTBOX_PERLINE*18);
	pDC->MoveTo(rt.right-m_rightLength,RIGHTBOX_PERLINE*22);
	pDC->LineTo(rt.right,RIGHTBOX_PERLINE*22);
	pDC->MoveTo(rt.right-m_rightLength+m_rightLength/5,RIGHTBOX_PERLINE*17);
	pDC->LineTo(rt.right-m_rightLength+m_rightLength/5,RIGHTBOX_PERLINE*22);

	pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
	pDC->SetTextColor( GetDocument()->m_colorArray[FIGUER_WORD]);
	pDC->SetBkMode(TRANSPARENT);

	CFont ft2;
	LOGFONT lgf2=GetDocument()->m_fontstr[0];//m_fontstr[0]
	lgf2.lfHeight=-14;
	lgf2.lfWeight =FW_BOLD;
	ft2.CreateFontIndirect (&lgf2 );
	CFont* pOld=pDC->SelectObject(&ft2);
	int modify=3;
	int lf=4;
	CString s=m_sharesSymbol;
	int len=(pDC->GetOutputTextExtent(s)).cx;
	pDC->TextOut(rt.right-m_rightLength+lf+4,0,s);
	s=pMin1Drawing->m_pReportData->name;
	pDC->TextOut(rt.right-m_rightLength+lf+8+len,0,s);
	pDC->SelectObject(pOld);

	CFont ft;
	LOGFONT lgf=GetDocument()->m_fontstr[0];
	lgf.lfHeight=-14;
	ft.CreateFontIndirect (&lgf );
	pOld=pDC->SelectObject(&ft);

	pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
	pDC->SetTextColor( GetDocument()->m_colorArray[FIGUER_WORD]);
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE+modify,"最新指数");
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*2+modify,"指数涨跌");
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*3+modify,"指数涨幅");
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*4+modify,"总成交额");
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*5+modify,"总成交量");
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*6+modify,"昨日收盘");
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*7+modify,"今日开盘");
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*8+modify,"最高指数");
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*9+modify,"最低指数");
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*11+modify,"委买手数");
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*12+modify,"委卖手数");
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*13+modify,"委比");
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*14+modify,"上涨家数");
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*15+modify,"平盘家数");
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*16+modify,"下跌家数");
	CString isSh="上海";
	if(CSharesCompute::GetMarketKind(m_stkKind) == SZ_MARKET_EX)
		isSh="深圳";
	isSh+="证券交易所";
	pDC->TextOut(rt.right-m_rightLength+lf,RIGHTBOX_PERLINE*10+modify,isSh);

	s=m_sharesSymbol ;

	if((pMin1Drawing->m_pReportData->nowp ) <=0  )
	{
		pDC->SelectObject(pOldpen);
		pDC->SelectObject(pOld);

		return;
	}
	//-	数值
	float f1;
	int isSz=0;
	if(CSharesCompute::GetMarketKind(m_stkKind) == SZ_MARKET_EX) isSz=1;
	CReportData* pS = pMin1Drawing->m_pS0;
	if(isSz>0) pS = pMin1Drawing->m_pS1;
	if(!pS)
	{
		pDC->SelectObject(pOldpen);
		pDC->SelectObject(pOld);
		return;
	}
	f1=(pS->accb);
	pDC->SetTextColor( GetDocument()->m_colorArray[FIGUER_WORD]);
	if(f1>0)
	{
		DrawRightText(pDC,f1,rt.right-lf,11,1);
	}
	f1=(pS->accs);
	if(f1>0)
	{
		DrawRightText(pDC,f1,rt.right-lf,12,1);
	}
	if(pS->accs+pS->accb>0)
	{
		f1=(pS->accb-pS->accs)
			/(pS->accs+pS->accb);
		if(f1>0)
			pDC->SetTextColor( GetDocument()->m_colorArray[13]);
		else if(f1<0)
			pDC->SetTextColor( GetDocument()->m_colorArray[XIA_DIE_WORD]);
		else
			pDC->SetTextColor( GetDocument()->m_colorArray[PING_PAN_WORD]);
		s.Empty();
		s.Format("%.2f",f1);
		pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
		pDC->TextOut(rt.right-lf,RIGHTBOX_PERLINE*13+modify,s);
	}


/*	else if(isSz == 2)
	{
		nStk=GetDocument()->m_sharesInformation.GetStockTypeCount(9);
	}*/
	int foot;
	if(GetDocument()->m_bInitDone ==FALSE)
		foot=GetDocument()->m_nOldANT [0];
	else
		foot=GetDocument()->m_nANT [isSz];

	Rsdn1 **Nidx;
	Nidx = pMin1Drawing->m_Nidx ;//->m_sharesInformation.GetIndexRsdn(Nidx); 
	if((Nidx [isSz][foot].rsn == 0||Nidx [isSz][foot].dnn == 0 ) && foot>0)
		foot--;

	f1=(float)(Nidx [isSz][foot].rsn);
	pDC->SetTextColor( GetDocument()->m_colorArray[13]);
	if(f1>=0)
	{
		DrawRightText(pDC,f1,rt.right-lf,14,1);
	}
	float f2=(float)(Nidx [isSz][foot].dnn);
	pDC->SetTextColor( GetDocument()->m_colorArray[XIA_DIE_WORD]);
	if(f2>=0)
	{
		DrawRightText(pDC,f2,rt.right-lf,16,1);
	}
	f1=GetDocument()->m_sharesInformation.GetValueUpDown( 1, isSz,  0)
		+GetDocument()->m_sharesInformation.GetValueUpDown( 1, isSz,  1)
		+GetDocument()->m_sharesInformation.GetValueUpDown( 1, isSz,  2);//GetDocument()->m_sharesInformation.GetStockTypeCount(1)+GetDocument()->m_sharesInformation.GetStockTypeCount(2)+GetDocument()->m_sharesInformation.GetStockTypeCount(SHJJ);
	if(isSz==1)
	{
		f1=GetDocument()->m_sharesInformation.GetValueUpDown( 1, isSz,  0)
		+GetDocument()->m_sharesInformation.GetValueUpDown( 1, isSz,  1)
		+GetDocument()->m_sharesInformation.GetValueUpDown( 1, isSz,  2);//GetDocument()->m_sharesInformation.GetStockTypeCount(1)+GetDocument()->m_sharesInformation.GetStockTypeCount(2)+GetDocument()->m_sharesInformation.GetStockTypeCount(SHJJ);
//		nStk=GetDocument()->m_sharesInformation.GetStockTypeCount(4)+GetDocument()->m_sharesInformation.GetStockTypeCount(5)++GetDocument()->m_sharesInformation.GetStockTypeCount(5);
	}
//	f1=(nStk-f1-f2);
	pDC->SetTextColor( GetDocument()->m_colorArray[FIGUER_WORD]);
	if(f1>=0)
	{
		DrawRightText(pDC,f1,rt.right-lf,15,1);
	}

/////================

	if(pMin1Drawing->m_pReportData->nowp >0)
	{
		f1=pMin1Drawing->m_pReportData->nowp;
		DrawRightText(pDC,f1,rt.right-lf,1,0);
	}
	f1=(pMin1Drawing->m_pReportData->nowp-pMin1Drawing->m_pReportData->ystc);
	s.Empty();
	s.Format("%.2f",f1);
	pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
	if(f1>0)
		pDC->SetTextColor( GetDocument()->m_colorArray[13]);
	else if(f1<0)
		pDC->SetTextColor( GetDocument()->m_colorArray[XIA_DIE_WORD]);
	else
		pDC->SetTextColor( GetDocument()->m_colorArray[PING_PAN_WORD]);
	pDC->TextOut(rt.right-lf,RIGHTBOX_PERLINE*2+modify,s);

	if(pMin1Drawing->m_pReportData->ystc>0)
	{
		f1=(f1*100/pMin1Drawing->m_pReportData->ystc);
		if(f1>0)
			pDC->SetTextColor( GetDocument()->m_colorArray[13]);
		else if(f1<0)
			pDC->SetTextColor( GetDocument()->m_colorArray[XIA_DIE_WORD]);
		else
			pDC->SetTextColor( GetDocument()->m_colorArray[PING_PAN_WORD]);
		s.Empty();
		s.Format("%.2f",f1);
		s+="%";
		pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
		pDC->TextOut(rt.right-lf,RIGHTBOX_PERLINE*3+modify,s);
	}
	pDC->SetTextColor( GetDocument()->m_colorArray[FIGUER_WORD]);
	f1=(pMin1Drawing->m_pReportData->totp)/10000;
	if(f1>0)
	{
		DrawRightText(pDC,f1,rt.right-lf,4,2);
	}
	f1=(pMin1Drawing->m_pReportData->totv);
	if(f1>0)
	{
		DrawRightText(pDC,f1,rt.right-lf,5,1);
	}
	f1=(pMin1Drawing->m_pReportData->ystc);
	if(f1>0)
	{
		DrawRightText(pDC,f1,rt.right-lf,6,0);
	}
	f1=(pMin1Drawing->m_pReportData->opnp );
	if(f1>0)
	{
		DrawRightText(pDC,f1,rt.right-lf,7,0);
	}

	f1=(pMin1Drawing->m_pReportData->higp);
	if(f1>0)
	{
		DrawRightText(pDC,f1,rt.right-lf,8,0);
	}
	f1=(pMin1Drawing->m_pReportData->lowp);
	if(f1>0)
	{
		DrawRightText(pDC,f1,rt.right-lf,9,0);
	}
	//add
	pDC->SelectObject(pOldpen);
	pDC->SelectObject(pOld);

	int nMarket=0;
	if(m_nKlineKind2==0)
		nMarket = 1;
//	pMin1Drawing->DrawDapan(pDC,nMarket,1);
	DrawUpDown(pDC);

}

void CTaiShanKlineShowView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pBar) 
{
	// TODO: Add your message handler code here and/or call default
	CXScrollBar* pScrollBar = (CXScrollBar*)pBar;

	if(m_bMultiFiguer==1)
	{
		//Marenan -
		//CView::OnVScroll(nSBCode, nPos, pScrollBar);
		return;
	}
	if(m_infoFiguer!=1)
		return;


	int nMax;
	int nMin;
	int nCurPos;

	nCurPos=pScrollBar->GetScrollPos();
	pScrollBar->GetScrollRange(&nMin,&nMax);

	int step=(nMax-nMin)/8+1;

	switch(nSBCode)
	{
	case SB_LINEDOWN:
		nCurPos+=1;
		break;
	case SB_LINEUP:
		nCurPos-=1;
		break;
	case SB_PAGEDOWN:
		nCurPos+=step;
		break;
	case SB_PAGEUP:
		nCurPos-=step;
		break;
	case SB_THUMBTRACK:
		nCurPos=nPos;
		break;
	default:
		break;
	}

	if(nCurPos>=nMax)
		nCurPos=nMax;
	if(nCurPos<=nMin)
		nCurPos=nMin;
	pScrollBar->SetScrollPos(nCurPos);
	CRect rt;
	GetClientRect(rt);
	rt.left=rt.right-m_rightLength;
	rt.right-=15;
	//Marenan +
	if (m_bInfoTabFill)
		rt.top=rt.top;
	else
		rt.top=RIGHTBOX_LINECOUNT*RIGHTBOX_PERLINE;
	//Marenan +

	//Marenan -
	//rt.top=19*RIGHTBOX_PERLINE+2;
	//Marenan -
//	CopyRect(rt);
	CTaiKlineDC memdc(this,&m_bit,&rt);
	DrawLineCurve(&memdc);

	m_nBeginHS=nCurPos;
	if(m_tabNum<4)
		m_nBeginHS = pMin1Drawing->DrawHs(&memdc,nCurPos, m_tabNum);
	else
		DrawFinace(&memdc,nCurPos);

	//Marenan -

	//CView::OnVScroll(nSBCode, nPos, pScrollBar);
}

///////////////////////////////////////////////////////////////////
//      函 数 名：ClearBack(CDC *pDC, CRect &rt)
//      功能说明：刷新背景
//      入口参数：pDC为设备描述表的指针，rt为矩形
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::ClearBack(CDC *pDC, CRect &rt,bool bHist)
{
	COLORREF	clr = GetDocument()->m_colorArray[19];
	if( m_nKlineKind2 == HS_KLINE || m_nKlineKind2 == MIN1_KLINE || m_nKlineKind2 == 0)
	{
		if( m_isShowCross == 1||pMin1Drawing->m_bHist ==true)
		{
			pDC->FillSolidRect(rt,clr);
			return;		
		}
	}
	if(bHist == true)
	{
		pDC->FillSolidRect(rt,clr);
		return;		
	}
	pDC->FillSolidRect(rt,GetDocument()->m_colorArray[0]);
}

///////////////////////////////////////////////////////////////////
//      函 数 名：DrawRightSheet(CDC *pDC, CPoint pt)
//      功能说明：画右边属性页
//      入口参数：pDC为设备描述表的指针，pt为鼠标的点
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::DrawRightSheet(CDC *pDC, CPoint pt)
{
		int len=m_rightLength;

		CRect rt;
		GetClientRect(rt);

		if(len==0)
			return;
		m_tabNum=(pt.x-rt.right +m_rightLength)*7/(len);	//Modified by ltr
		m_nBeginHS=-1;
		if(IsIndexStock(m_sharesSymbol))
		{
			if(m_tabNum<4)
			{
				m_tabNum = 0;
			}
		}
//		if(m_tabNum=

		DrawRightSheet(pDC,m_tabNum);
}

///////////////////////////////////////////////////////////////////
//      函 数 名：SetPictYPos(int which, int y)
//      功能说明：设置子图的位置
//      入口参数：which为第几个子图，y为鼠标的y点
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::SetPictYPos(int which, int y)
{
	CTaiShanDoc* pDoc = GetDocument();
	if(m_nKlineKind2==0)
	{
//		m_rtMin1.rtBlw[nm-1].m_yBottom;
		float yFlt=m_infoInit.initMin1[which].yFloatBottom+m_infoInit.initMin1[which+1].yFloatBottom;
		int yFrt;
		if(which==0)
			yFrt=m_heightCaption;
		else
			yFrt=m_rtMin1.rtBlw[which-1].m_yBottom + m_heightCaption;

		if((float)(m_rtMin1.rtBlw[which+1].m_yBottom - yFrt- m_heightCaption)==0)
			return ;

		float yB=(y-yFrt)/(float)(m_rtMin1.rtBlw[which+1].m_yBottom - yFrt- m_heightCaption);

		m_infoInit.initMin1[which].yFloatBottom=yFlt*yB;
//		if(yFlt*yB<=0)
//			m_infoInit.initMin1[which].yFloatBottom=0.0001;
		m_infoInit.initMin1[which+1].yFloatBottom=yFlt-yFlt*yB;
//		if(yFlt-yFlt*yB<=0)
//			m_infoInit.initMin1[which+1].yFloatBottom=0.0001;

	}
	else
	{
		float yFlt=m_infoInit.initIndex[which].yFloatBottom+m_infoInit.initIndex[which+1].yFloatBottom;
		int yFrt;
		if(which==0)
			yFrt=m_heightCaption;
		else
			yFrt=m_rtKlineFiguer.rtBlw[which-1].m_yBottom + m_heightCaption;

		if((float)(m_rtKlineFiguer.rtBlw[which+1].m_yBottom - yFrt- m_heightCaption)==0)
			return ;

		float yB=(y-yFrt)/(float)(m_rtKlineFiguer.rtBlw[which+1].m_yBottom - yFrt- m_heightCaption);
		m_infoInit.initIndex[which].yFloatBottom=yFlt*yB;
//		if(yFlt*yB<=0)
//			m_infoInit.initIndex[which].yFloatBottom=0.0001;
		m_infoInit.initIndex[which+1].yFloatBottom=yFlt-yFlt*yB;
//		if(yFlt-yFlt*yB<=0)
//			m_infoInit.initIndex[which+1].yFloatBottom=0.0001;
	}
	CRect rt;
	GetClientRect(rt);
	OnSizeMy(rt.right-rt.left,rt.bottom-rt.top);
	if(m_infoFiguer>0)
		rt.right-=m_rightLength;
	

}

///////////////////////////////////////////////////////////////////
//      函 数 名：OnSizeMy(int cx, int cy)
//      功能说明：初始化子图的位置
//      入口参数：cx为总宽度，cy为总高度
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::OnSizeMy(int cx, int cy)
{
	if(m_bMultiFiguer==1)
	{
		InitRect( cx, cy);
		return;
	}

	InitRect( cx, cy);

	int len=m_rightLength-6;

	RECT rtScroll;
	rtScroll.right=cx-2;
	rtScroll.left=cx-15;
	rtScroll.bottom=cy-24;

	//rtScroll.right=cx;
	//rtScroll.left=cx-15;
	//rtScroll.bottom=cy-27 ;

	//Marenan +
	if (m_bInfoTabFill)
		rtScroll.top =ZOOMIMG+1;
	else
		rtScroll.top=RIGHTBOX_LINECOUNT*RIGHTBOX_PERLINE+ZOOMIMG+1 ;
	//Marenan +

	//Marenan -
	rtScroll.top=RIGHTBOX_PERLINE*RIGHTBOX_LINECOUNT+1 ;

	//Marenan +
	if(m_scrollBar->m_hWnd!=NULL)
		m_scrollBar->SetSBRect(&rtScroll) ;
	m_rcZoom.SetRect(rtScroll.left,rtScroll.top-ZOOMIMG-1,rtScroll.right,rtScroll.top-1) ;
	//Marenan +
	//Marenan -
	//if(m_scrollBar->m_hWnd!=NULL)
	//	m_scrollBar->MoveWindow(rtScroll.left  ,rtScroll.top ,15,rtScroll.bottom-rtScroll.top,TRUE);  
	//Marenan -
	

}

void CTaiShanKlineShowView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bMultiFiguer==1)
	{
		CView::OnRButtonDown(nFlags, point);
		return;
	}

	CRect r;
	GetClientRect(r);
	if(m_infoFiguer>0&&point.x>r.right-m_rightLength)
	{
		CView::OnRButtonDown(nFlags, point);
		return;
	}

   	CaclRulorMovable(point);
	CTaiShanDoc* pDoc = GetDocument();
	if(::GetCapture()==NULL)
	{
		switch(m_hit)
		{
		case REGION_RULER:
			if(m_nKlineKind2>0)
			{
				r.left = 0;
				r.right = m_rtKlineFiguer.rightX;
				r.top = m_rtKlineFiguer.rtBlw[m_infoInit.nCountIndex-1].m_yBottom;
				r.bottom = m_rtKlineFiguer.rtBlw[m_infoInit.nCountIndex-1].m_yBottom+m_heightCaption;
	
				m_nOldHitPos = point.x;
				SetCapture();
				ClientToScreen(r);
				::ClipCursor(r);
				SetTimer(TM_RULER,200,NULL);
			}
			break;

		case REGION_TRANSFER:
//			ShowMyMenu(IDR_POPKLINE);	
			break;
		case REGION_PICTTEXT1:
			break;
		case REGION_PICT1:
		case REGION_PICT2:
		case REGION_PICT3:
		case REGION_PICT4:
		case REGION_PICT5:
			TrackCursorBgn(point);
			break;
		case REGION_LITTLE_FIGUER:
			break;
		default:
			break;
		}
	}
//	CalcCursorShape(point);	
	CView::OnRButtonDown(nFlags, point);

}

void CTaiShanKlineShowView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bMultiFiguer==1)
	{
		CView::OnRButtonUp(nFlags, point);
		return;
	}
	
	if(::GetCapture()==m_hWnd && !(nFlags&(MK_LBUTTON|MK_RBUTTON)))
	{
		::ClipCursor(NULL);
//		if(GetLastError()!=0)
//			AfxMessageBox("ClipCursor error");
		Sleep(0);
		ReleaseCapture();

		RECT r2;
		GetClipCursor(&r2);
		if(r2.right -r2.left >0)
		{
//			AfxMessageBox("GetClipCursor>2");
//			TRACE("%d    top = %d   r=%d   b=%d\n",r2.left,r2.top,r2.right,r2.bottom);
		}

		switch(m_hit)
		{
		case REGION_RULER:
			if(m_nKlineKind2>0)
			{
				KillTimer(TM_RULER);
			}
			break;

		case REGION_TRANSFER:
			ShowMyMenu(IDR_POPKLINE);	
			break;
		case REGION_PICTTEXT1:
			if(m_nKlineKind2==0)
				break;
			ShowMyMenu(IDR_POPKLINE,3);	
			break;//
		case REGION_PICT1:
		case REGION_PICT2:
		case REGION_PICT3:
		case REGION_PICT4:
		case REGION_PICT5:
			if(m_nKlineKind2==0)
			{
				ShowMyMenu(IDR_POPKLINE,4);
				break;
			}

//			SetTimer(TM_TRACK_CURCOR,5,NULL);
			TrackCursorEnd(point,TRUE);
			TRACE("TrackCursorEnd   x==  %d  y==  %d\n",point.x,point.y);
			break;
		default:
			break;
		}
		m_nOldHitPos = -1;
		this->CopyRect(r2);
	}
	else if(::GetCapture()!=m_hWnd)
	{
		switch(m_hit)
		{
		case REGION_TRANSFER:
			ShowMyMenu(IDR_POPKLINE);	
			break;
		case REGION_PICTTEXT1:
			ShowMyMenu(IDR_POPKLINE,3);	
			break;
		case REGION_LITTLE_FIGUER:
			if(m_tabNum>3)
				ShowMyMenu(IDR_POPKLINE,100);	
			break;
		case REGION_SELFLINE_BODY:
		case REGION_SELFLINE_1:
		case REGION_SELFLINE_2:
		case REGION_SELFLINE_3:
			ShowMyMenu(IDR_POPKLINE,5);	
			break;
		case REGION_PICTCAPTION1:
			ShowMyMenu(IDR_POPKLINE,6);
			break;
		}
	}

	CView::OnRButtonUp(nFlags, point);
}

///////////////////////////////////////////////////////////////////
//      函 数 名：TrackCursorBgn(CPoint point)
//      功能说明：跟踪鼠标开始
//      入口参数：鼠标的点
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::TrackCursorBgn(CPoint point)
{
	if(m_hit<REGION_PICT1||m_hit>REGION_PICT5&&m_hit<REGION_SELFLINE_1||m_hit>REGION_SELFLINE_BODY)
		return;

	bTrackCurcorBgn=TRUE;
	CTaiShanDoc* pDoc = GetDocument();
	int i = m_hit<=REGION_PICT5 ? m_hit-REGION_PICT1 : 0;
	if(m_hit>=REGION_SELFLINE_1 && m_hit<=REGION_SELFLINE_BODY)
		i=0;
	CRect r;
	if(m_nKlineKind2==0)
	{
		r.left = m_rtMin1.leftX;
		r.right = m_rtMin1.rightX;
		if(i==0)
			r.top = m_heightCaption;
		else
			r.top = m_rtMin1.rtBlw[i-1].m_yBottom+m_heightCaption;
		r.bottom = m_rtMin1.rtBlw[i].m_yBottom-2;
	}
	else
	{
		r.left = 0;
		r.right = m_rtKlineFiguer.rightX;
		if(i==0)
			r.top = m_heightCaption;
		else
			r.top = m_rtKlineFiguer.rtBlw[i-1].m_yBottom+m_heightCaption;
		r.bottom = m_rtKlineFiguer.rtBlw[i].m_yBottom-2;
	}
	ClientToScreen(r);
	::ClipCursor(r);
	SetCapture();


	pDrawLine->m_bSelected=false;
	if(m_hit>=REGION_SELFLINE_1 && m_hit<=REGION_SELFLINE_BODY)
	{
		//---		自画线操作开始
		KillTimer(TM_TIP);
		if(m_nKlineKind2==0)
			return;
//		if(pDrawLine->m_bDrawLine==false)
//			return;
		CClientDC dc(this);	
		pDrawLine->m_bDrawLine=TRUE;

		bTrackCurcorBgn=TRUE;
		//-		防止数据被改变
		pDrawLine->m_pt[0].x=pDrawLine->m_ptOldDrawing[0].x;
		pDrawLine->m_pt[0].y=pDrawLine->m_ptOldDrawing[0].y;
		pDrawLine->m_pt[1].x=pDrawLine->m_ptOldDrawing[1].x;
		pDrawLine->m_pt[1].y=pDrawLine->m_ptOldDrawing[1].y;
		pDrawLine->m_pt[2].x=pDrawLine->m_ptOldDrawing[2].x;
		pDrawLine->m_pt[2].y=pDrawLine->m_ptOldDrawing[2].y;
		pDrawLine->m_lineType=pDrawLine->m_nLineTypeChanging;
		pDrawLine->m_nCurrentLineClicked=pDrawLine->m_nCurrentLine;

	if(pDrawLine->m_lineType==TEXT_KIND||pDrawLine->m_lineType==UP_ARROW||pDrawLine->m_lineType==DOWN_ARROW)
		{
			int typ=pDrawLine->m_lineType;
			CPoint p1;
			CString sText;
			sText=pDrawLine->m_textNow;
			p1.x=pDrawLine->m_pt[0].x;
			p1.y=pDrawLine->m_pt[0].y;
			pDrawLine->m_bSelected=TRUE;
			pDrawLine->DeleteLine();
//			pDrawLine->AddLine();
			CTaiKlineDC memdc(this,&m_bit,&r);
			pKlineDrawing->DrawKlineFiguer(&memdc);
			pDrawLine->m_pt[0].x=p1.x;
			pDrawLine->m_pt[0].y=p1.y;
			pDrawLine->m_lineType=typ;
			pDrawLine->m_textNow=sText;
//			pDrawLine->DrawPointBE(&dc);
			return;
		}
		pDrawLine->m_bSelected=TRUE;
		pDrawLine->DrawPointBE(&dc);
		int j;
		switch(m_hit)
		{
		case REGION_SELFLINE_BODY:
			m_rectHitPos.left = point.x;
			m_rectHitPos.top = point.y;
			for( j=0;j<pDrawLine->m_nPntGroup[pDrawLine->m_lineType];j++)
			{
				pDrawLine->m_ptOld[j]=pDrawLine->m_pt[j];
			}
			break;
		case REGION_SELFLINE_1:
		case REGION_SELFLINE_2:
		case REGION_SELFLINE_3:
			pDrawLine->m_nPointCurrent=m_hit-REGION_SELFLINE_1;
//			pDrawLine->SetPointPos(point);
			break;
		}
		//-		防止数据被改变
		pDrawLine->m_ptOldDrawing[0].x=pDrawLine->m_pt[0].x;
		pDrawLine->m_ptOldDrawing[0].y=pDrawLine->m_pt[0].y;
		pDrawLine->m_ptOldDrawing[1].x=pDrawLine->m_pt[1].x;
		pDrawLine->m_ptOldDrawing[1].y=pDrawLine->m_pt[1].y;
		pDrawLine->m_ptOldDrawing[2].x=pDrawLine->m_pt[2].x;
		pDrawLine->m_ptOldDrawing[2].y=pDrawLine->m_pt[2].y;


	}
	else
	{
		m_rectHitPos.left = point.x;
		m_rectHitPos.top = point.y;
		m_rectHitPos.right = point.x;
		m_rectHitPos.bottom = point.y;

		//---		自画线操作开始
		if(m_nKlineKind2==0||m_hit!=REGION_PICT1)
			return;
		if(pDrawLine->m_bDrawLine==false)
			return;
		KillTimer(TM_TIP);
		pDrawLine->m_lineType=lineTypeDrawed;//temp my
		if(pDrawLine->m_lineType==-1)
			return ;
		//				pDrawLine->m_nPointCurrent=2;
		bTrackCurcorBgn=TRUE;
		if(pDrawLine->m_lineType!=ZHONG_XIAN && pDrawLine->m_lineType!=BAG_LINE)
		{
			for(int j=0;j<pDrawLine->m_nPntGroup[pDrawLine->m_lineType];j++)
			{
				pDrawLine->m_nPointCurrent=j;
				pDrawLine->SetPointPos(point);
			}
			//-		防止数据被改变
			pDrawLine->m_ptOldDrawing[0].x=pDrawLine->m_pt[0].x;
			pDrawLine->m_ptOldDrawing[0].y=pDrawLine->m_pt[0].y;
			pDrawLine->m_ptOldDrawing[1].x=pDrawLine->m_pt[1].x;
			pDrawLine->m_ptOldDrawing[1].y=pDrawLine->m_pt[1].y;
			pDrawLine->m_ptOldDrawing[2].x=pDrawLine->m_pt[2].x;
			pDrawLine->m_ptOldDrawing[2].y=pDrawLine->m_pt[2].y;

		}
		else
		{
			pDrawLine->m_nZhongXian=pDrawLine->m_nZhongXianDrawing;
			if(pDrawLine->m_nZhongXian==0)
			{
				for(int j=0;j<2;j++)
				{
					pDrawLine->m_nPointCurrent=j;
					pDrawLine->SetPointPos(point);
					pDrawLine->m_ptOldDrawing[j].x=pDrawLine->m_pt[j].x;
					pDrawLine->m_ptOldDrawing[j].y=pDrawLine->m_pt[j].y;
				}
				pDrawLine->m_nPointCurrent=1;
			}
			else
			{
				pDrawLine->m_nPointCurrent=2;
				pDrawLine->SetPointPos(point);
				pDrawLine->m_ptOldDrawing[2].x=pDrawLine->m_pt[2].x;
				pDrawLine->m_ptOldDrawing[2].y=pDrawLine->m_pt[2].y;
//				pDrawLine->m_nZhongXian=0;
			}
		}

		CClientDC dc(this);	
		if(pDrawLine->m_lineType==TEXT_KIND)
		{
			m_toScreen.x=pDrawLine->m_pt[0].x;
			m_toScreen.y=pDrawLine->m_pt[0].y;
			//-		防止数据被改变
			pDrawLine->m_ptOldDrawing[0].x=pDrawLine->m_pt[0].x;
			pDrawLine->m_ptOldDrawing[0].y=pDrawLine->m_pt[0].y;

			int type=pDrawLine->m_lineType;
			ClientToScreen(&m_toScreen);

			CTaiKlineShowText m_showText(this);//显示画线工具中的文本

			if(m_showText.DoModal()==IDOK)
			{
				pDrawLine->m_textNow=m_showText.m_editText;
				pDrawLine->m_lineType=type;
				pDrawLine->m_pt[0].x=pDrawLine->m_ptOldDrawing[0].x;
				pDrawLine->m_pt[0].y=pDrawLine->m_ptOldDrawing[0].y;
				pDrawLine->AddLine();
				CTaiKlineDC memdc(this,&m_bit,&pDrawLine->m_rtDrawLine);
				pKlineDrawing->DrawKlineFiguer(&memdc);
//				CopyRect(pDrawLine->m_rtDrawLine);
			}

//			delete 	showText;
		}


		if(pDrawLine->m_lineType==UP_ARROW||pDrawLine->m_lineType==DOWN_ARROW)
		{
			//-		防止数据被改变
		}



	}


}

///////////////////////////////////////////////////////////////////
//      函 数 名：TrackCursorGoOn(CPoint point)
//      功能说明：继续跟踪鼠标
//      入口参数：鼠标的点
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::TrackCursorGoOn(CPoint p)
{
	if(m_hit<REGION_PICT1||m_hit>REGION_PICT5&&m_hit<REGION_SELFLINE_1||m_hit>REGION_SELFLINE_BODY)
		return;

	CTaiShanDoc* pDoc = GetDocument();
	int i = m_hit<=REGION_PICT5 ? m_hit-REGION_PICT1 : 0;
	if(m_hit>=REGION_SELFLINE_1 && m_hit<=REGION_SELFLINE_BODY)
		i=0;
	CRect r;
	GetClientRect(r);
	CClientDC dc(this);
	CDC* pDC=&dc;
//	CPen pen_line(PS_SOLID  ,1,pDoc->m_colorArray[2]);//PS_DASHDOTDOT   
//	pDC->SelectObject(&pen_line);


	if(m_hit>=REGION_SELFLINE_1 && m_hit<=REGION_SELFLINE_BODY)
	{
		if(m_nKlineKind2==0)
			return;
		//-		防止数据被改变
		pDrawLine->m_pt[0].x=pDrawLine->m_ptOldDrawing[0].x;
		pDrawLine->m_pt[0].y=pDrawLine->m_ptOldDrawing[0].y;
		pDrawLine->m_pt[1].x=pDrawLine->m_ptOldDrawing[1].x;
		pDrawLine->m_pt[1].y=pDrawLine->m_ptOldDrawing[1].y;
		pDrawLine->m_pt[2].x=pDrawLine->m_ptOldDrawing[2].x;
		pDrawLine->m_pt[2].y=pDrawLine->m_ptOldDrawing[2].y;
		pDrawLine->m_lineType=pDrawLine->m_nLineTypeChanging;

		if(pDrawLine->m_lineType==TEXT_KIND||pDrawLine->m_lineType==UP_ARROW||pDrawLine->m_lineType==DOWN_ARROW)
		{
			pDrawLine->DrawPointBE(pDC);

			r.left =pDrawLine->m_pt[0].x-30;
			r.top =pDrawLine->m_pt[0].y-30;
			r.right =pDrawLine->m_rtDrawLine.right;
			r.bottom =r.top+66;
			CopyRect(r);
			pDrawLine->m_nPointCurrent=0;
			pDrawLine->SetPointPos(p);
			pDrawLine->DrawSelfLine(pDC);

			pDrawLine->DrawPointBE(pDC);
			//-		防止数据被改变
			pDrawLine->m_ptOldDrawing[0].x=pDrawLine->m_pt[0].x;
			pDrawLine->m_ptOldDrawing[0].y=pDrawLine->m_pt[0].y;

			return;
		}
		pDrawLine->DrawPointBE(pDC);
		//---		自画线操作继续
		int oldROP = pDC->SetROP2(R2_NOT   );//R2_NOTXORPEN
		switch(m_hit)
		{
		case REGION_SELFLINE_BODY:
//			m_rectHitPos.left = p.x;
//			m_rectHitPos.top = p.y;
			if((pDrawLine->m_lineType==ZHONG_XIAN || pDrawLine->m_lineType==BAG_LINE) )
				pDrawLine->m_nZhongXian=1;
			for(i=0;i<2;i++)
			{
				pDrawLine->DrawSelfLine(pDC);
				if(i==0)
					pDrawLine->SetPointMoved(p.x-m_rectHitPos.left
						, p.y-m_rectHitPos.top);
			}
			break;
		case REGION_SELFLINE_1:
		case REGION_SELFLINE_2:
		case REGION_SELFLINE_3:
			if((pDrawLine->m_lineType==ZHONG_XIAN || pDrawLine->m_lineType==BAG_LINE) )
				pDrawLine->m_nZhongXian=1;
			for(i=0;i<2;i++)
			{
				pDrawLine->DrawSelfLine(pDC);
				pDrawLine->m_nPointCurrent=m_hit-REGION_SELFLINE_1;
				pDrawLine->SetPointPos(p);
			}
			break;
		}
		pDC->SetROP2(oldROP);
		pDrawLine->DrawPointBE(pDC);

		//-		防止数据被改变
		pDrawLine->m_ptOldDrawing[0].x=pDrawLine->m_pt[0].x;
		pDrawLine->m_ptOldDrawing[0].y=pDrawLine->m_pt[0].y;
		pDrawLine->m_ptOldDrawing[1].x=pDrawLine->m_pt[1].x;
		pDrawLine->m_ptOldDrawing[1].y=pDrawLine->m_pt[1].y;
		pDrawLine->m_ptOldDrawing[2].x=pDrawLine->m_pt[2].x;
		pDrawLine->m_ptOldDrawing[2].y=pDrawLine->m_pt[2].y;

	}
	else
	{
		if(lineTypeDrawed==-1)
			pDrawLine->m_bDrawLine=false ;

		if(pDrawLine->m_bDrawLine==false)
		{
			if(m_infoFiguer>0)
				r.right-=m_rightLength;

			if(pKlineDrawing->m_kLine_mode == 1)
			{
				if(this->m_nKlineKind2 >0)
					SetTimer(TM_MOVE_DRAG_K,2,NULL);
//				pKlineDrawing->m_nSon = i;
//				CPoint pIn(m_rectHitPos.left,m_rectHitPos.top);
//				pKlineDrawing->MoveDragMode(pIn,p);
//				m_rectHitPos.left = p.x;
//				m_rectHitPos.top = p.y;
			}
			else
			{
				for(i=0;i<2;i++)
				{
					int oldROP = pDC->SetROP2(R2_NOTXORPEN   );//R2_NOTXORPEN
					pDC->MoveTo(m_rectHitPos.left,m_rectHitPos.top);
					pDC->LineTo(m_rectHitPos.right,m_rectHitPos.top);
					pDC->LineTo(m_rectHitPos.right,m_rectHitPos.bottom);
					pDC->LineTo(m_rectHitPos.left,m_rectHitPos.bottom);
					pDC->LineTo(m_rectHitPos.left,m_rectHitPos.top);
		//			m_nOldHitPos = p.y;
					m_rectHitPos.right = p.x;
					m_rectHitPos.bottom = p.y;
					pDC->SetROP2(oldROP);
				}
			}
		}
		else
		{
			//---		自画线操作继续
			if(m_nKlineKind2==0)
				return;
			if(m_hit<REGION_PICT1||m_hit>REGION_PICT1)
			{
				return;
			}

			//	避免操作期间线型变化，重置线型
			pDrawLine->m_lineType=lineTypeDrawed;//temp my

			if(pDrawLine->m_lineType==TEXT_KIND||pDrawLine->m_lineType==UP_ARROW||pDrawLine->m_lineType==DOWN_ARROW)
			{
				return;
			}
			if(pDrawLine->m_lineType==-1)
				return ;

			if(pDrawLine->m_lineType==PING_XING || pDrawLine->m_lineType==GAN_SHI || pDrawLine->m_lineType==HUANG_JIN_HU)
				return ;
		//				pDrawLine->m_nPointCurrent=2;
			if((pDrawLine->m_lineType==ZHONG_XIAN || pDrawLine->m_lineType==BAG_LINE) && pDrawLine->m_nZhongXianDrawing!=0)
				return;

			//-		防止数据被改变
			pDrawLine->m_pt[0].x=pDrawLine->m_ptOldDrawing[0].x;
			pDrawLine->m_pt[0].y=pDrawLine->m_ptOldDrawing[0].y;
			pDrawLine->m_pt[1].x=pDrawLine->m_ptOldDrawing[1].x;
			pDrawLine->m_pt[1].y=pDrawLine->m_ptOldDrawing[1].y;
//			pDrawLine->m_pt[2].x=pDrawLine->m_ptOldDrawing[2].x;
//			pDrawLine->m_pt[2].y=pDrawLine->m_ptOldDrawing[2].y;

			int oldROP = pDC->SetROP2(R2_NOT   );//R2_NOTXORPEN
			for(i=0;i<2;i++)
			{
				pDrawLine->DrawSelfLine(pDC);
				pDrawLine->m_nPointCurrent=1;
				pDrawLine->SetPointPos(p);
			}
			//-		防止数据被改变
			pDrawLine->m_ptOldDrawing[0].x=pDrawLine->m_pt[0].x;
			pDrawLine->m_ptOldDrawing[0].y=pDrawLine->m_pt[0].y;
			pDrawLine->m_ptOldDrawing[1].x=pDrawLine->m_pt[1].x;
			pDrawLine->m_ptOldDrawing[1].y=pDrawLine->m_pt[1].y;

			pDC->SetROP2(oldROP);
//			pDC->DeleteDC();
		}
	}


}

///////////////////////////////////////////////////////////////////
//      函 数 名：TrackCursorEnd(CPoint point)
//      功能说明：结束跟踪鼠标
//      入口参数：point 鼠标的点
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::TrackCursorEnd(CPoint point, BOOL flag)
{
//		ReleaseCapture();
//		::ClipCursor(NULL);
		WPARAM wParam;
		LPARAM lParam;
		m_rectHitPos.right = point.x;
		m_rectHitPos.bottom = point.y;
		if(m_rectHitPos.right<0)
			m_rectHitPos.right=0;
		if(m_rectHitPos.left<0)
			m_rectHitPos.left=0;

		CRect r;
		GetClientRect(r);
		int rgt = r.right - (this->m_infoFiguer > 0 ?m_rightLength:0)-m_midLen;

		if(m_rectHitPos.right>rgt )
			m_rectHitPos.right=rgt;
		if(m_rectHitPos.left>rgt)
			m_rectHitPos.left=rgt;

		bTrackCurcorBgn=false;

		//---左鼠标按钮弹起
		if(flag==false)
		{
			if(m_hit>=REGION_SELFLINE_1 && m_hit<=REGION_SELFLINE_BODY)
			{
				//---		自画线操作结束
				if(m_nKlineKind2==0)
				{
					RedrawWindowNoCacl();
					return;
				}
				//-		防止数据被改变
				pDrawLine->m_pt[0].x=pDrawLine->m_ptOldDrawing[0].x;
				pDrawLine->m_pt[0].y=pDrawLine->m_ptOldDrawing[0].y;
				pDrawLine->m_pt[1].x=pDrawLine->m_ptOldDrawing[1].x;
				pDrawLine->m_pt[1].y=pDrawLine->m_ptOldDrawing[1].y;
				pDrawLine->m_pt[2].x=pDrawLine->m_ptOldDrawing[2].x;
				pDrawLine->m_pt[2].y=pDrawLine->m_ptOldDrawing[2].y;
				pDrawLine->m_lineType=pDrawLine->m_nLineTypeChanging;

				if(pDrawLine->m_lineType==TEXT_KIND||pDrawLine->m_lineType==UP_ARROW||pDrawLine->m_lineType==DOWN_ARROW)
				{
					pDrawLine->AddLine();
					pDrawLine->m_bSelected=TRUE;
//					memdc.SelectClipRgn (NULL);//&rgn2
					RedrawWindow();
				}
				else
				{
					if(pDrawLine->m_nCurrentLineClicked>=0)
					{
						pDrawLine->ScreenToFile(pDrawLine->m_nCurrentLineClicked, TRUE);
						pDrawLine->SetAlarmFlag(pDrawLine->m_nCurrentLineClicked, (BYTE) 0);
						pDrawLine->WriteLineself();
					}

					int typ=pDrawLine->m_lineType;

//					memdc.SelectClipRgn (NULL);//&rgn2
					RedrawWindow();

				}

			}
			else
			{
				//---如果不是自画线
				if(pDrawLine->m_bDrawLine==false)
				{
					if(m_nKlineKind2==0)
					{
						lParam=0;
						wParam=ID_SHIDUAN_TJ;
						DoOnCommand(wParam, lParam);	//进行时段统计
						return;
					}
					else
					{
						if(pKlineDrawing->m_kLine_mode == 0)
							ShowKlineBig();
					}
				}
				//---如果是自画线
				else
				{
					//---		自画线操作结束
					if(m_nKlineKind2==0||m_hit!=REGION_PICT1)
					{
						RedrawWindowNoCacl();
						return;
					}
					if(pDrawLine->m_lineType==-1)
					{
						RedrawWindowNoCacl();
						return;
					}
					
					pDrawLine->m_bSelected=false;

					//	避免操作期间线型变化，重置线型
					pDrawLine->m_lineType=lineTypeDrawed;//temp my
					//-		防止数据被改变
					pDrawLine->m_pt[0].x=pDrawLine->m_ptOldDrawing[0].x;
					pDrawLine->m_pt[0].y=pDrawLine->m_ptOldDrawing[0].y;
					pDrawLine->m_pt[1].x=pDrawLine->m_ptOldDrawing[1].x;
					pDrawLine->m_pt[1].y=pDrawLine->m_ptOldDrawing[1].y;
					pDrawLine->m_pt[2].x=pDrawLine->m_ptOldDrawing[2].x;
					pDrawLine->m_pt[2].y=pDrawLine->m_ptOldDrawing[2].y;


					if(pDrawLine->m_lineType==TEXT_KIND)
					{
						lineTypeDrawed=-1;
						RedrawWindow();
						return;
					}

					if(pDrawLine->m_lineType!=ZHONG_XIAN && pDrawLine->m_lineType!=BAG_LINE)
					{
						if(pDrawLine->m_lineType==GAN_SHI || pDrawLine->m_lineType==HUANG_JIN_HU)//HUANG_JIN_HU
							pDrawLine->m_bLineValidate=TRUE;
						if(pDrawLine->m_bLineValidate==TRUE)
						{
							//--		设置先前的线形
							if(pDrawLine->m_lineType==XIAN_DUAN ||pDrawLine->m_lineType==QU_SHI)
							{
								pDrawLine->m_ptOldPingXing[0].x=pDrawLine->m_pt[0].x ;
								pDrawLine->m_ptOldPingXing[0].y=pDrawLine->m_pt[0].y ;
								pDrawLine->m_ptOldPingXing[1].x=pDrawLine->m_pt[1].x ;
								pDrawLine->m_ptOldPingXing[1].y=pDrawLine->m_pt[1].y ;
								pDrawLine->m_lineTypeOld=pDrawLine->m_lineType;
							}
							else if(pDrawLine->m_lineType==PING_XING )
							{
								CPoint ptIn;
								ptIn.x=(pDrawLine->m_pt[0].x*2-(pDrawLine->m_ptOldPingXing[1].x-pDrawLine->m_ptOldPingXing[0].x))/2;
								ptIn.y=(pDrawLine->m_pt[0].y*2-(pDrawLine->m_ptOldPingXing[1].y-pDrawLine->m_ptOldPingXing[0].y))/2;
								pDrawLine->m_pt[1].x=(pDrawLine->m_pt[0].x*2+(pDrawLine->m_ptOldPingXing[1].x-pDrawLine->m_ptOldPingXing[0].x))/2;
								pDrawLine->m_pt[1].y=(pDrawLine->m_pt[0].y*2+(pDrawLine->m_ptOldPingXing[1].y-pDrawLine->m_ptOldPingXing[0].y))/2;
								pDrawLine->m_pt[0].x=ptIn.x;
								pDrawLine->m_pt[0].y=ptIn.y;
								if(pDrawLine->m_pt[0].x>pDrawLine->m_pt[1].x)
								{
								ptIn.x=pDrawLine->m_pt[0].x;
								ptIn.y=pDrawLine->m_pt[0].y;
								pDrawLine->m_pt[0].x=pDrawLine->m_pt[1].x;
								pDrawLine->m_pt[0].y=pDrawLine->m_pt[1].y;
								pDrawLine->m_pt[1].x=ptIn.x;
								pDrawLine->m_pt[1].y=ptIn.y;
								}
								if(pDrawLine->m_lineTypeOld!=QU_SHI && pDrawLine->m_lineTypeOld!=XIAN_DUAN)
									pDrawLine->m_lineTypeOld=QU_SHI;
								pDrawLine->m_lineType=pDrawLine->m_lineTypeOld;
							}
					//--		设置先前的线形
							else
								pDrawLine->m_lineTypeOld=lineTypeDrawed;

							if(pDrawLine->m_lineType==GAN_SHI )//HUANG_JIN_HU
							{
								float widthPer =(m_rtKlineFiguer.rightX )/(float)(pKlineDrawing->m_klinNumDefault);// klineWidthPer + 2 * klineSide;//每格的宽度
								if(widthPer<2)
									widthPer=10;
								pDrawLine->m_pt[1].y=pDrawLine->m_pt[0].y-(int)(10*widthPer);
								pDrawLine->m_pt[1].x=pDrawLine->m_pt[0].x+(int)(10*widthPer);
								pDrawLine->m_bLineValidate=false;
							}
							if(pDrawLine->m_lineType==HUANG_JIN_HU )//
							{
								float widthPer =(m_rtKlineFiguer.rightX )/(float)(pKlineDrawing->m_klinNumDefault);// klineWidthPer + 2 * klineSide;//每格的宽度
								if(widthPer<2)
									widthPer=10;
								pDrawLine->m_pt[1].y=pDrawLine->m_pt[0].y+(int)(10*widthPer);
								pDrawLine->m_pt[1].x=pDrawLine->m_pt[0].x+(int)(10*widthPer);
								pDrawLine->m_bLineValidate=false;
							}

							pDrawLine->AddLine();
							CTaiKlineDC memdc(this,&m_bit,&pDrawLine->m_rtDrawLine);
							pKlineDrawing->DrawKlineFiguer(&memdc);
							lineTypeDrawed=-1;
						}
					}
					else
					{//		
						pDrawLine->m_nZhongXian=pDrawLine->m_nZhongXianDrawing;

						pDrawLine->m_lineTypeOld=lineTypeDrawed;
						if(pDrawLine->m_nZhongXianDrawing==0)
						{
							bTrackCurcorBgn = TRUE;
							pDrawLine->m_nZhongXianDrawing=1;
							pDrawLine->m_nZhongXian=1;
							pDrawLine->m_bMoveZhongXian=0;
							
						}
						else
						{
							bTrackCurcorBgn = false;
							if(pDrawLine->m_bLineValidate==TRUE)
							{
								pDrawLine->m_pt[0].x=pDrawLine->m_ptOldDrawing[0].x;
								pDrawLine->m_pt[0].y=pDrawLine->m_ptOldDrawing[0].y;
								pDrawLine->m_pt[1].x=pDrawLine->m_ptOldDrawing[1].x;
								pDrawLine->m_pt[1].y=pDrawLine->m_ptOldDrawing[1].y;
								pDrawLine->m_pt[2].x=pDrawLine->m_ptOldDrawing[2].x;
								pDrawLine->m_pt[2].y=pDrawLine->m_ptOldDrawing[2].y;

								pDrawLine->AddLine();
								CTaiKlineDC memdc(this,&m_bit,&pDrawLine->m_rtDrawLine);
								pKlineDrawing->DrawKlineFiguer(&memdc);
								pDrawLine->m_bMoveZhongXian=1;
//								pDrawLine->m_bDrawLine=false;
								pDrawLine->m_nZhongXianDrawing=0;
								pDrawLine->m_nZhongXian=0;
								lineTypeDrawed=-1;
							}
						}
					}
				}
			}
		}
		//---右鼠标按钮弹起
		else
		{
//			memdc.SelectClipRgn (NULL);//
			if(m_rtKlineFiguer.rightX==0)
			{
				RedrawWindow();
				return;
			}
			if(abs(m_rectHitPos.right-m_rectHitPos.left)*pKlineDrawing->m_klinNumDefault/(m_rtKlineFiguer.rightX )<=1)
			{
				if(m_nKlineKind2==0)
				{
					if(m_hit==REGION_PICT2)
						ShowMyMenu(IDR_POPKLINE,4);
				}
				else if(m_hit>=REGION_PICT1&&m_hit<=REGION_PICT5)
					ShowMyMenu(IDR_POPKLINE,2);
			}
			else
			{
					ShowMyMenu(IDR_POPKLINE,1);
			}
			return;

		}
	
		RedrawWindow();

}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：TransferX(int x)
//      功能说明：把x坐标转换为对应的数组下标
//      入口参数：x为横轴坐标
//      出口参数：无出口数
//      返回参数：返回数组的下标
/////////////////////////////////////////////////////////////////////////////
int CTaiShanKlineShowView::TransferX(int x)
{
	int rtn;
	return rtn=x*pKlineDrawing->m_klinNumDefault /m_rtKlineFiguer.rightX;
}
//IDR_POPKLINE
void CTaiShanKlineShowView::ShowMyMenu(UINT id,int subM)//6.1
{
	CMenu  mnu;
	CPoint posMouse;
	::GetCursorPos(&posMouse);

	if(subM==100)
	{
		mnu.CreatePopupMenu( );
#ifdef OEM
		mnu.AppendMenu(MF_STRING | MF_ENABLED, IDM_POP_FINANCE,gStrTabSheet[0]);
		mnu.AppendMenu(MF_STRING | MF_ENABLED, IDM_POP_FINANCE+1,gStrTabSheet[1]);
#else
		int nID[] = {0,1,2,6};
		for(int j=0;j<4;j++)
			mnu.AppendMenu(MF_STRING | MF_ENABLED, IDM_POP_FINANCE+nID[j],gStrTabSheet[j]);
#endif
		(mnu.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON  ,posMouse.x,posMouse.y,this, NULL));
		RedrawWindowNoCacl();
		return;
	}

	mnu.LoadMenu (id);
	CMenu* pMnu=mnu.GetSubMenu(subM);

	CMenu menu;
	CMenu menu2;
	CMenu menu3;
	int j;
	switch(subM)
	{
	case 0:
		if(m_nKlineKind2== 0)
			pMnu->CheckMenuItem (ID_MENU_1FS,MF_BYCOMMAND|MF_CHECKED );

		for(j=0;j<9;j++)
		{
			if(j==m_nKlineKind2 )
				pMnu->CheckMenuItem (j+4,MF_BYPOSITION|MF_CHECKED );
			else
				pMnu->CheckMenuItem (j+4,MF_BYPOSITION|MF_UNCHECKED );
			//
		}
		if(m_nKlineKind2 == MANY_DAY_KLINE)
			pMnu->CheckMenuItem (ID_KLINE_MANY_DAY,MF_BYCOMMAND|MF_CHECKED );
		else
			pMnu->CheckMenuItem (ID_KLINE_MANY_DAY,MF_BYCOMMAND|MF_UNCHECKED );
		if(m_nKlineKind2 == HS_KLINE)
			pMnu->CheckMenuItem (ID_KLINE_HS,MF_BYCOMMAND|MF_CHECKED );
		else
			pMnu->CheckMenuItem (ID_KLINE_HS,MF_BYCOMMAND|MF_UNCHECKED );
		if(m_nKlineKind2 == MIN1_KLINE)
			pMnu->CheckMenuItem (ID_KLINE_1_MINU,MF_BYCOMMAND|MF_CHECKED );
		else
			pMnu->CheckMenuItem (ID_KLINE_1_MINU,MF_BYCOMMAND|MF_UNCHECKED );
		break;
	case 1:
		break;
	case 2:
		if(bExtend==TRUE)
		{
			pMnu->EnableMenuItem (ID_RESTORE_KLINE, MF_ENABLED);
		}
		else
			pMnu->EnableMenuItem (ID_RESTORE_KLINE, MF_GRAYED);
		if(m_nKlineKind2==5||m_nKlineKind2==1)
		{
			pMnu->EnableMenuItem (ID_CHANGE_DATA, MF_ENABLED);
		}
		else
			pMnu->EnableMenuItem (ID_CHANGE_DATA, MF_GRAYED);
			//ID_EXTEND_KLINE

		//change formular
#ifdef OEM
		pMnu->EnableMenuItem (ID_MODIFY_JISHU_POP, MF_GRAYED);
		pMnu->EnableMenuItem (ID_PARAM_YH, MF_GRAYED);
#endif
		//-	调整指标参数菜单
		pMnu->AppendMenu(MF_STRING | MF_ENABLED, IDM_POP_CHANGE_PARAM, "调整指标参数");
		
		//-	加上“取消选股指示”菜单
//		if(m_hit==REGION_PICT1)
		{
			pMnu->AppendMenu(  MF_SEPARATOR);

			if(pKlineMode==NULL)  
				pMnu->AppendMenu(MF_STRING | MF_ENABLED, IDM_KLINE_MD, "K线组合指示");
			else 
				pMnu->AppendMenu(MF_STRING | MF_ENABLED, IDM_KLINE_MD, "取消K线组合指示");

			if(m_screenStockShow.m_bUseing == true)
				pMnu->AppendMenu(MF_STRING | MF_ENABLED, IDM_POP_TJXG_SHOW, "取消条件选股指示");
			else
				pMnu->AppendMenu(MF_STRING | MF_ENABLED, IDM_POP_TJXG_SHOW, "条件选股指示");
			if(pKlineDrawing->m_nKlineCurrent >0)
				pMnu->AppendMenu(MF_STRING | MF_ENABLED, IDM_POP_KLINE_ADDED, "取消主图叠加");

#ifndef OEM
			if(m_pDlgDealHistory==NULL)
				pMnu->AppendMenu(MF_STRING | MF_ENABLED, ID_HIST_OPEN_POP, "打开历史回忆");
#endif
		}
		m_nOldnFgr= PointToFiguer();
		if(pKlineDrawing->m_sIndicatorAdded[m_nOldnFgr] != "")
		{
			pMnu->AppendMenu(MF_STRING | MF_ENABLED, ID_DELETE_ADDED_INDICATOR, "取消指标叠加");
		}

		//-		常用指标菜单
		if(m_hit>=REGION_PICT1&&m_hit<=REGION_PICT5)
		{
			//得到常用指标数组
			CStringArray m_zhiBiaoOften;
			for(int i=0;i<GetDocument()->m_formuar_index .GetSize ();i++)
			{
				CFormularContent * jishu;
				jishu=GetDocument()->m_formuar_index.GetAt(i );
				if(jishu->isOfen ==TRUE)
				{
					CString sName(jishu->name);
					sName.MakeUpper();
					m_zhiBiaoOften.Add( sName );
				}
			}

			if(menu.CreatePopupMenu ()==FALSE)
				return;
			pMnu->AppendMenu(  MF_SEPARATOR);
			pMnu->AppendMenu(MF_STRING | MF_ENABLED, IDM_POP_DRAG_MODE, "图形平移方式");
			if(pKlineDrawing->m_kLine_mode == 1 )
				pMnu->CheckMenuItem (IDM_POP_DRAG_MODE,MF_BYCOMMAND|MF_CHECKED );

			pMnu->AppendMenu(  MF_SEPARATOR);
			pMnu->AppendMenu(  MF_POPUP | MF_STRING, (UINT)menu.m_hMenu, "常用指标");
			for(int nPos = 0 ; nPos < m_zhiBiaoOften.GetSize() ; ++nPos)
			{
				int n = (nPos)%10;
				if(n == 0 && nPos!=0)
					menu.AppendMenu(MF_STRING  | MF_MENUBARBREAK, IDM_Index__OFTEN_1 + nPos, m_zhiBiaoOften[nPos]);
				else
					menu.AppendMenu(MF_STRING , IDM_Index__OFTEN_1 + nPos, m_zhiBiaoOften[nPos]);
			}

/*			//均线选择
			if(menu2.CreatePopupMenu ()==FALSE)
				return;
			pMnu->AppendMenu(  MF_POPUP | MF_STRING, (UINT)menu2.m_hMenu, "均线选择");
			nPos = 0;
			for(i=0;i<GetDocument()->m_formuar_index .GetSize ();i++)
			{
				CFormularContent * jishu;
				jishu=GetDocument()->m_formuar_index.GetAt(i );
				if(jishu->isMainFiguer  ==TRUE)
				{
					CString sName(jishu->explainBrief );
					int n = (nPos)%10;
					if(n == 0 && nPos!=0)
						menu2.AppendMenu(MF_STRING  | MF_MENUBARBREAK, IDM_Index__OFTEN_1 + nPos+200, sName);
					else
						menu2.AppendMenu(MF_STRING , IDM_Index__OFTEN_1 + nPos+200, sName);
					nPos++;
				}
			}

			//特色指标
			if(menu3.CreatePopupMenu ()==FALSE)
				return;
			pMnu->AppendMenu(  MF_POPUP | MF_STRING, (UINT)menu3.m_hMenu, "特色指标");
			nPos = 0;
			for(i=0;i<GetDocument()->m_formuar_index .GetSize ();i++)
			{
				CFormularContent * jishu;
				jishu=GetDocument()->m_formuar_index.GetAt(i );
				if(jishu->subKindIndex .Find ("特色指标" )>=0)
				{
					CString sName(jishu->explainBrief );
					int n = (nPos)%10;
					if(n == 0 && nPos!=0)
						menu3.AppendMenu(MF_STRING  | MF_MENUBARBREAK, IDM_Index__OFTEN_1 + nPos+400, sName);
					else
						menu3.AppendMenu(MF_STRING , IDM_Index__OFTEN_1 + nPos+400, sName);
					nPos++;
				}
			}
//			DRAG MODE		//*/
		}

		break;
	case 3:
		if(m_nKlineKind2== 0)
			return;
		for(j=0;j<3;j++)
		{
			if(j==pKlineDrawing->m_axisType )
				pMnu->CheckMenuItem (j,MF_BYPOSITION|MF_CHECKED );
			else
				pMnu->CheckMenuItem (j,MF_BYPOSITION|MF_UNCHECKED );
		}
		break;
	case 4:
		if(m_nKlineKind2!= 0 )
			return;
#ifndef OEM
		if(m_pDlgDealHistory==NULL)
			pMnu->AppendMenu(MF_STRING | MF_ENABLED, ID_HIST_OPEN_POP, "打开历史回忆");
#endif
		if(m_hit == REGION_PICT1)
		{
			if(IsIndexStock3(m_sharesSymbol))
			{
			  pMnu->EnableMenuItem (ID_L_FENSHI, MF_GRAYED); 
			}
			if(IsIndexStock(m_sharesSymbol))//
			{
			pMnu->EnableMenuItem (ID_L_CHENG_JIAO_L, MF_GRAYED);
			pMnu->EnableMenuItem (ID_L_MMLD, MF_GRAYED);
			pMnu->EnableMenuItem (ID_L_LB, MF_GRAYED);
			pMnu->EnableMenuItem (ID_L_DUOKONG, MF_GRAYED);
			pMnu->EnableMenuItem (ID_L_TENGLUO, MF_GRAYED);
			}
			else
			{
				return;
			}
		}
		if(m_hit == REGION_PICT2)
		{
			if(IsIndexStock3(m_sharesSymbol))
			{
				return;
			}
		}
		for(j=0;j<FS_NUM;j++)
		{
			if(pMin1Drawing->m_pFlg[m_hit-REGION_PICT1]== j)
				pMnu->CheckMenuItem (j,MF_BYPOSITION|MF_CHECKED );
			else
				pMnu->CheckMenuItem (j,MF_BYPOSITION|MF_UNCHECKED );
		}
		if(pMin1Drawing->m_nKlineCurrent>0)
		{
			pMnu->AppendMenu(  MF_SEPARATOR);
			pMnu->AppendMenu(MF_STRING | MF_ENABLED, IDM_POP_KLINE_ADDED, "取消主图叠加");
		}
//			menu.AppendMenu(MF_STRING | MF_ENABLED, 100, "取消主图叠加");

		break;
	case 5://画线菜单
		if(m_nKlineKind2== 0)
			return;
		{
			COLORREF  rgb;
			int  nLineType,   nLineThick;
			bool  bAlarm;
			if(!this->pDrawLine ->GetCurrentLineProp(rgb, nLineType, nLineThick, bAlarm))
				return;
			int nID[] = {ID_DRAWLINE_SOLIDLINE,ID_DRAWLINE_VIRTUALLINE,ID_DRAWLINE_DOTLINE};
			int nID2[] = {ID_DRAWLINE_LINEWID1,ID_DRAAWLINE_LINEWID2,ID_DRAAWLINE_LINEWID3};
			pMnu->CheckMenuItem (nID[nLineType],MF_BYCOMMAND|MF_CHECKED );
			pMnu->CheckMenuItem (nID2[nLineThick],MF_BYCOMMAND|MF_CHECKED );
			if(!(m_nKlineKind2 >0 && m_nKlineKind2 <9) || !pDrawLine->IsTrendLine())
				pMnu->EnableMenuItem (ID_TREND_LINE_ALARM,MF_GRAYED);
			if(bAlarm==true) pMnu->CheckMenuItem (ID_TREND_LINE_ALARM,MF_BYCOMMAND|MF_CHECKED );
		}
		break;
	case 6://加入到自选股菜单
		//加入自选股板块菜单
		CStringArray StockTypeNameArray;
		int index1=8900;
		int index=9900;
		CMenu menu1;
		menu1.CreatePopupMenu();
		pMnu->AppendMenu(  MF_POPUP | MF_STRING, (UINT)menu1.m_hMenu, "加入到自选股");

		pMnu->DeleteMenu(0,MF_BYPOSITION);

		CTaiShanDoc* pDoc = GetDocument();
		pDoc->m_ManagerBlockData.GetChooseStockTypeNames(StockTypeNameArray);
		int l_nCount=StockTypeNameArray.GetSize();
		int CurrentPos=-1;
		for(int i=0;i<l_nCount;i++,index1++)
		{
			CString l_sStockType=StockTypeNameArray.GetAt(i);
			if(i%20==0 && i!=0)
				menu1.AppendMenu(MF_STRING|MF_MENUBARBREAK ,index, l_sStockType);
			else
				menu1.AppendMenu(MF_STRING  ,index1 ,l_sStockType );
			if(	pDoc->m_nCurrentReportPage==4&&pDoc->m_SystemInitData.ChooseTypeName==l_sStockType)
				CurrentPos=i;
		}
		menu1.EnableMenuItem( CurrentPos,MF_BYPOSITION|MF_DISABLED|MF_GRAYED);

		break;
	}
	CRect r;
	r.left =0;
	r.right =1000;//m_rtKlineFiguer.rightX;
	r.top =0;//m_heightCaption;
	r.bottom  =800;//m_rtKlineFiguer.rtBlw[0].m_yBottom;
	if(pMnu)
	{
//		bTrackCurcorBgn = false;
		(pMnu->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON  ,
			posMouse.x,posMouse.y,this, NULL));
//		RedrawWindowNoCacl();
	}

}
///////////////////////////////////////////////////////////////////
//      函 数 名：OnMenu1fs()
//      功能说明：转换K线类型的菜单命令处理
//      入口参数：无
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::OnMenu1fs() 
{
	// TODO: Add your command handler code here
	m_nKlineKind2=0;
	CRect r;
	GetClientRect(r);
	OnSizeMy(r.right-r.left,r.bottom-r.top);
	pMin1Drawing->InitMinuteLine();
	InvalidateRect(r);
	if(m_isShowCross==1)
		m_dlgShowCross->RedrawWindow();
	
}

///////////////////////////////////////////////////////////////////
//      函 数 名：OnDayKline()
//      功能说明：转换K线类型的菜单命令处理
//      入口参数：无
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::OnDayKline() 
{
	m_nKlineKind2=5;
	
	bTrackCurcorBgn=false;
	
	CRect r;
	GetClientRect(r);
	OnSizeMy(r.right-r.left,r.bottom-r.top);

	ShowAll(m_sharesSymbol);
}

void CTaiShanKlineShowView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bAppAuthorized > 1000)
		Sleep(1000);

	bTrackCurcorBgn=false;

	CRect r;
	GetClientRect(r);
//	TRACE("NCHAR  == %d\n",nChar);
	if(m_infoFiguer>0)
		r.right-=m_rightLength;
	CTaiShanDoc* pDoc = GetDocument();

	ClearCross();//6.17
	int x2=m_pointMove.x;
	int y2=m_pointMove.y;

	if(m_bMultiFiguer==1)
	{
		int nFiguer;
		if(pDoc->m_systemOption.fourgraph==FALSE)//kline
			nFiguer=4;
		else
			nFiguer=9;
		switch(nChar)
		{
		case VK_ESCAPE:
			pKlineDrawing -> m_bNewStock = TRUE ;
			m_bMultiFiguer=0;
			ShowAll(m_sharesSymbol);
			break;
		case VK_PRIOR ://UPPAGE
			OnPageDown(false,nFiguer);
	//		CopyRect(r);
			return;
		case VK_NEXT ://UPPAGE
			OnPageDown(TRUE,nFiguer);
	//		CopyRect(r);
			return;
		case VK_F5:
			if(m_nKlineKind2 == 0)
			{
				pKlineDrawing -> m_bNewStock = TRUE ;
				OnDayKline();
			}
			else 
			{
			  OnMenu1fs();
			}
			break; 
		}//
		return;
	}

	m_MyToolTip->Hide();

	//escape
	if(nChar == VK_ESCAPE)
	{
		if(m_isShowCross==1)
		{
			int rtn=0;
			m_dlgShowCross->EndDialog(rtn);
			m_isShowCross=0;
			RedrawWindow();
		}
		else
		{
			//////////To End The Draw Line Bar/////////
			CMainFrame* pMainWnd = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
			if(IsBarShow  == TRUE && pMainWnd->m_drawLineToolBar->IsWindowVisible() != 0 )
			{
              pMainWnd->ShowControlBar(pMainWnd->m_drawLineToolBar,FALSE, FALSE);
			  IsBarShow = !IsBarShow;
			  return;
			}
			/////////////////////////////////////
			this->m_GetFocus = false;
			this->m_AutoChange = false;
			KillTimer(12);
			if(GetParentFrame() == GetParent())
			{
				GetParentFrame()->SendMessage(WM_CLOSE);
			}
			pDoc->m_pReportView->GetParentFrame()->ActivateFrame();
//			pDoc->m_pReportView->GetParentFrame()->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
//			pDoc->m_pReportView->SetFocus();
		}
		return ;
	}
	//end

	//VK_INSERT
	if(nChar == VK_INSERT)
	{
		//宽带版数据申请
		if(::GetKeyState(VK_CONTROL)&0x8000)
		{
			if(m_pWideNet) m_pWideNet->AddStockKlineDataAll( m_sharesSymbol,  this->m_nKlineKind2 ,  true);
		}
		else
		{
			if(m_pWideNet) m_pWideNet->AddStockHsFileDataAll(this->m_sharesSymbol, true);
		}
	}

	if(m_nKlineKind2>0)
	{
		switch(nChar)
		{
			//20021013
		case 190:
			{
				static int nIndicator = 0;
				CString ZhibiaoName;
				switch(nIndicator)
				{
				case 0:
					ZhibiaoName = "MA";
					break;
				case 1:
					ZhibiaoName = "MA1";
					break;
				case 2:
					ZhibiaoName = "MA2";
					break;
				case 3:
					ZhibiaoName = "MA3";
					break;
				case 4:
					ZhibiaoName = "MA4";
					break;
				}
				DrawZhibiao(ZhibiaoName,0);
				nIndicator++;
				nIndicator%=5;
			}
			goto END_ROW;
		case VK_UP:
			SetTimer(TM_UP_K,2,NULL);
			goto END_ROW;
		case VK_DOWN:
			SetTimer(TM_DOWN_K,2,NULL);
			goto END_ROW;
		case VK_LEFT:
			if(m_AutoChange == TRUE)
			{
			 m_AutoChange = false;
			 KillTimer(12);
			}
			(pKlineDrawing->OnLeft());
			DoHistoryAStep() ;
			m_bKeyUpDown=TRUE;
			break;
		case VK_RIGHT:
			if(m_AutoChange == TRUE)
			{
			 m_AutoChange = false;
			 KillTimer(12);
			}
			(pKlineDrawing->OnRight());
			DoHistoryAStep() ;
//			RefreshChenben();

//			if(::GetKeyState(VK_CONTROL)&0x8000)
//				CopyRect(r);
			m_bKeyUpDown=TRUE;
			break;
		case 111:               ////////Key '/'
//		case VK_HOME:
			if(m_isShowCross == 1)
			{
				(pKlineDrawing->OnHome());
				CTaiKlineDC memdc(this,&m_bit);
				RefreshChenben(&memdc);
			}
			else
			{//FS_NUM
				CString s(m_infoInit.initIndex[pKlineDrawing->m_nTextBox+1].nameIndex);
				int i;
				int n=GetDocument()->m_formuar_index .GetSize ();
				CFormularContent * jishu=NULL;

				for(i=0;i<n;i++)
				{
					jishu=GetDocument()->m_formuar_index.GetAt(i);
					if(jishu->name ==s)
					{
						break;
					}
					jishu=NULL;
				}
				for(int j=0;j<n;j++)
				{
					if(i==0)
						i=n-1;
					else
						i--;
					jishu=GetDocument()->m_formuar_index.GetAt(i);
					if(jishu->isOfen == 1)
					{
						strcpy(m_infoInit.initIndex[pKlineDrawing->m_nTextBox+1].nameIndex,jishu->name);
						break;
					}
				}
				pKlineDrawing->m_bToCacl =TRUE;
				pKlineDrawing ->m_bInitFoot =false;//m_bInitFoot

//				CBitmap bit;
				CTaiKlineDC memdc(this,&m_bit,&r);
				pKlineDrawing->DrawKlineFiguer (&memdc);
				goto END_ROW;
			}
			break;
		case 106:              ////////Key '*'
//		case VK_END:
			if(m_isShowCross == 1)
			{
				(pKlineDrawing->OnEnd());
				CTaiKlineDC memdc(this,&m_bit);
				RefreshChenben(&memdc);
			}
			else
			{
				CString s(m_infoInit.initIndex[pKlineDrawing->m_nTextBox+1].nameIndex);
				int i;
				int n=GetDocument()->m_formuar_index .GetSize ();
				CFormularContent * jishu=NULL;

				for(i=0;i<n;i++)
				{
					jishu=GetDocument()->m_formuar_index.GetAt(i);
					if(jishu->name ==s)
					{
						break;
					}
					jishu=NULL;
				}

				for(int j=0;j<n;j++)
				{
					i++;
					jishu=GetDocument()->m_formuar_index.GetAt(i%n);
					if(jishu->isOfen == 1)
					{
						strcpy(m_infoInit.initIndex[pKlineDrawing->m_nTextBox+1].nameIndex,jishu->name);
						break;
					}
				}

				pKlineDrawing->m_bToCacl =TRUE;
				pKlineDrawing ->m_bInitFoot =false;//m_bInitFoot
				CTaiKlineDC memdc(this,&m_bit,&r);
				pKlineDrawing->DrawKlineFiguer (&memdc);
				goto END_ROW;
			}
			break;
		case VK_TAB:
			if(m_infoInit .nCountIndex==1)
				return;
			pKlineDrawing->m_nTextBox=(pKlineDrawing->m_nTextBox+1)%(m_infoInit .nCountIndex-1);
			RedrawWindow();
			goto END_ROW;
		case VK_PRIOR ://UPPAGE
			if(m_bKeyUpDown==false)
				OnPageDown(false);
			else
				OnPageDown(false,4);

			m_bKeyUpDown=TRUE;
			goto END_ROW;
		case VK_NEXT ://UPPAGE
			if(::GetKeyState(VK_CONTROL)&0x8000)
				goto END_ROW;
			if(m_bKeyUpDown==false)
				OnPageDown(TRUE);
			else
				OnPageDown(TRUE,4);

			m_bKeyUpDown=TRUE;
			goto END_ROW;
		case VK_DELETE:
			pDrawLine ->DeleteLine ();
			RedrawWindow();
			goto END_ROW;
		case VK_F5:
			if(m_nKlineKind2 == 0)
			{
			  OnDayKline();
			}
			else //if(m_nKlineKind2 == 5)
			{
			  OnMenu1fs();
			}
			goto END_ROW;
		case 107:              ////////Key '+'
            ReDrawSheet(TRUE);
			break;
		}//
	}
	else
	{
		switch(nChar)
		{
		case VK_LEFT:
			if(m_AutoChange == TRUE)
			{
			 m_AutoChange = false;
			 KillTimer(12);
			}
			(pMin1Drawing->OnLeft());
//			if(::GetKeyState(VK_CONTROL)&0x8000)
//				CopyRect(r);
			m_bKeyUpDown=TRUE;
			break;
		case VK_RIGHT:
			if(m_AutoChange == TRUE)
			{
			 m_AutoChange = false;
			 KillTimer(12);
			}			
			(pMin1Drawing->OnRight());
//			if(::GetKeyState(VK_CONTROL)&0x8000)
//				CopyRect(r);
			m_bKeyUpDown=TRUE;
			break;
		case VK_HOME:
			if(m_isShowCross == 1)
				(pMin1Drawing->OnHome());
			else
			{//FS_NUM
				if(pMin1Drawing->m_pFlg[pMin1Drawing->m_nTextBox+1]==0)
					pMin1Drawing->m_pFlg[pMin1Drawing->m_nTextBox+1]=FS_NUM-1;
				else
					pMin1Drawing->m_pFlg[pMin1Drawing->m_nTextBox+1]--;
				RedrawWindow();
				goto END_ROW;
			}
			break;
		case VK_END:
			if(m_isShowCross == 1)
				(pMin1Drawing->OnEnd());
			else
			{
				pMin1Drawing->m_pFlg[pMin1Drawing->m_nTextBox+1]++;
				pMin1Drawing->m_pFlg[pMin1Drawing->m_nTextBox+1]%=FS_NUM;
				RedrawWindow();
				goto END_ROW;
			}
			break;
		case VK_TAB:
			if(m_infoInit .nCountMin1==1)
				goto END_ROW;
			pMin1Drawing->m_nTextBox=(pMin1Drawing->m_nTextBox+1)%(m_infoInit .nCountMin1-1);
			RedrawWindow();
			goto END_ROW;
		case VK_PRIOR ://UPPAGE
			if(m_bKeyUpDown==false)
				OnPageDown(false);
			else
				OnPageDown(false,4);

			m_bKeyUpDown=TRUE;
	//		CopyRect(r);
			goto END_ROW;
		case VK_NEXT ://UPPAGE
			if(m_bKeyUpDown==false)
				OnPageDown(TRUE);
			else
				OnPageDown(TRUE,4);

			m_bKeyUpDown=TRUE;
			goto END_ROW;
		case VK_F5:
			if(m_nKlineKind2 == 0)
			{
				pKlineDrawing -> m_bNewStock = TRUE ;
				OnDayKline();
			}
			else //if(m_nKlineKind2 == 5)
			{
			  OnMenu1fs();
			}
			goto END_ROW;
		case 107:              ////////Key '+'
            ReDrawSheet(TRUE);
			break;
		}//
	}

	ShowCross(TRUE,x2,y2);

END_ROW:
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CTaiShanKlineShowView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if(nHitTest==HTCLIENT &&  pWnd==this)
	{
//		m_bSetCursor = TRUE;
		CPoint point;
		::GetCursorPos(&point);
		ScreenToClient (&point);
		CalcCursorShape(point);	
		return TRUE;
	}
	return CView::OnSetCursor(pWnd, nHitTest, message);
}

///////////////////////////////////////////////////////////////////
//      函 数 名：ShowShiZi(int x, int y,int flag)
//      功能说明：转换K线类型的菜单命令处理
//      入口参数：x为横轴坐标，y为纵轴坐标
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::ShowShiZi(int x, int y,int flag)
{
	CClientDC dc(this);
	CPen pen;
	if (!pen.CreatePen(PS_SOLID,1,GetDocument()->m_colorArray[2]))
			return;
	CPen* pOldpen=dc.SelectObject(&pen);
	int oldROP = dc.SetROP2(R2_NOT);//XORPEN
	if( dc.GetDeviceCaps (RASTERCAPS ) == 0 )//RC_NONE
		dc.SetROP2(oldROP);
//		int oldROP = dc.SetROP2(R2_NOT);//XORPEN
	int lft=1;
	CRect r;
	GetClientRect(r);
	if(m_nKlineKind2==0)
	lft=m_rtMin1.leftX+1;
	m_nOldHitPos = x;
	m_nOldHitY = y;
	if(m_nOldHitPos>=0)
	{
		if(m_nOldHitY>m_heightCaption)
		{
			dc.MoveTo(1,m_nOldHitY);
			dc.LineTo(m_rtKlineFiguer.rightX,m_nOldHitY);
		}
		dc.MoveTo(m_nOldHitPos,m_heightCaption);
		dc.LineTo(m_nOldHitPos,m_rtKlineFiguer.rtBlw[m_infoInit.nCountIndex-1].m_yBottom);
	}
	dc.SetROP2(oldROP);
	dc.SelectObject(pOldpen);
}
void CTaiShanKlineShowView::ShowShiZi()
{
}

///////////////////////////////////////////////////////////////////
//      函 数 名：CopyRect(CRect r)
//      功能说明：从内存位图复制到位图
//      入口参数：r为对应的矩形
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::CopyRect(CRect r)
{
	CTaiKlineDC memdc(this,&m_bit,&r);
}

///////////////////////////////////////////////////////////////////
//      函 数 名：ClearCross()
//      功能说明：清除十字光标
//      入口参数：无
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::ClearCross()
{
	if(m_isShowCross==0)
		return;
	CRect r;
	GetClientRect(r);
	if(m_infoFiguer>0)
		r.right-=m_rightLength;
	r.bottom -=m_heightCaption;

	if(m_pointMove.x>r.right )
		return;

	CRect rIn(0, m_pointMove.y,
		r.right-m_midLen,m_pointMove.y+1);
	CTaiKlineDC* pMemdc = new CTaiKlineDC(this,&m_bit,&rIn);
	delete pMemdc;
	CRect rIn2(m_pointMove.x, 0,
		m_pointMove.x+1, r.bottom);
	pMemdc =new  CTaiKlineDC(this,&m_bit,&rIn2);
	delete pMemdc;

}

///////////////////////////////////////////////////////////////////
//      函 数 名：ShowCross()
//      功能说明：显示十字光标
//      入口参数：无
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::ShowCross(bool bKey,int x,int y)//6.17
{
	if(bKey == TRUE)
		m_bKD = TRUE;
	//--		显示游标
	int x2=m_pointMove.x;
	int y2=m_pointMove.y;
	m_pointMove.y=y;
	m_pointMove.x=x;

	if(x>-1)
		DrawRulorBoxY(y2);
	if(bKey == false)
	{
		if(y>-1)
			DrawRulorBoxX(x2);
	}
	else
	{
		DrawRulorBoxX(x2,TRUE);
	}

	m_pointMove.y=y2;
	m_pointMove.x=x2;

	if(m_isShowCross==0)
		return;

	//--		输出当前值
	if(m_nKlineKind2>0)
	{
		pKlineDrawing->m_nSon=0;
		pKlineDrawing->SetRectCurrent(0);

		int foot;
		foot=pKlineDrawing->TransferX(m_pointMove.x);
		foot+=pKlineDrawing->m_footBegin;
		if(foot>pKlineDrawing->m_footEnd)
		{
			pKlineDrawing->m_footCurrent = pKlineDrawing->m_footEnd;
			if(pKlineDrawing->m_footCurrent<0) pKlineDrawing->m_footCurrent = 0;
			return ;
		}

		if(bKey==false && m_bKD == false)
		{
			pKlineDrawing->m_currentValue=m_currentValue;
			TRACE("pKlineDrawing m_footCurrent b = %d\n",pKlineDrawing->m_footCurrent);

			if(!(abs(m_pointMove.x-x)<1 && pKlineDrawing->m_klinNumDefault >m_rtKlineFiguer.rightX ))
				pKlineDrawing->m_footCurrent=foot;
			if(pKlineDrawing->m_footCurrent<pKlineDrawing->m_footBegin) pKlineDrawing->m_footCurrent = pKlineDrawing->m_footBegin;
			if(pKlineDrawing->m_footCurrent>pKlineDrawing->m_footEnd) pKlineDrawing->m_footCurrent = pKlineDrawing->m_footEnd;
			TRACE("pKlineDrawing m_footCurrent a = %d\n",pKlineDrawing->m_footCurrent);
		}
	}
	else
	{
		pMin1Drawing->m_nSon=0;
		pMin1Drawing->SetRectDraw(0);

		int foot;
		if((m_rtMin1.rightX-m_rtMin1.leftX)==0)
			return ;
		foot=(m_pointMove.x-m_rtMin1.leftX)*pMin1Drawing->m_klinNumDefault /(m_rtMin1.rightX-m_rtMin1.leftX);
		foot+=pMin1Drawing->m_footBegin;
		if(foot>pMin1Drawing->m_footEnd||foot<0)
		{
			pMin1Drawing->m_footCurrent = pMin1Drawing->m_footEnd;
			if(foot<0) pMin1Drawing->m_footCurrent = 0;
			return ;
		}


		if(bKey==false)
		{
			pMin1Drawing->m_currentValue=m_currentValue;
			pMin1Drawing->m_footCurrent=foot;
			if(pMin1Drawing->m_footCurrent<pMin1Drawing->m_footBegin) pMin1Drawing->m_footCurrent = pMin1Drawing->m_footBegin;
			if(pMin1Drawing->m_footCurrent>pMin1Drawing->m_footEnd) pMin1Drawing->m_footCurrent = pMin1Drawing->m_footEnd;
		}
	}

	CopyCaption();

	//--		显示十字光标
	CRect rt;
	GetClientRect(rt);
	if(m_infoFiguer>0)
	{
		rt.right =rt.right-m_rightLength-m_midLen;
	}
	rt.bottom -=m_heightCaption;
	if(m_isShowCross==1)//6.18
	{
//		if(!m_dlgShowCross->IsWindowVisible())
//			if(GetDocument()->m_propertyInitiate.bShowCrossDlg == TRUE) m_dlgShowCross->ShowWindow(SW_SHOW);

		if(m_nKlineKind2>0)
		{
			CString s0(m_infoInit.initIndex[0].nameIndex);
			CClientDC dc(this);
			CDC* pDC=&dc;
			rt.top=m_heightCaption;
			int bttm=rt.bottom;
			rt.bottom=m_rtKlineFiguer.rtBlw[0].m_yBottom;
			bool bMove = true;
			if(s0=="QLDJ")
			{
				CopyRect(rt);
				pKlineDrawing->ShowCNP (pDC,2);
				bMove = false;
			}
			if(s0=="HLDJ")
			{
				CopyRect(rt);
				pKlineDrawing->ShowCNP (pDC,1);
				bMove = false;
			}
			if(s0=="QEDJ")
			{
				CopyRect(rt);
				pKlineDrawing->ShowChengBen (pDC,2);
				bMove = false;
			}
			if(s0=="HEDJ")
			{
				CopyRect(rt);
				pKlineDrawing->ShowChengBen (pDC,1);
				bMove = false;
			}
			rt.bottom=bttm;
		}
		else
		{
			rt.top=m_heightCaption;
			int bttm=rt.bottom;
			rt.right = m_rtMin1.rightX ;
			rt.bottom=m_rtMin1.rtBlw[0].m_yBottom;

			CopyRect(rt);
			CClientDC dc(this);
			CDC* pDC=&dc;
			pMin1Drawing->ShowCNP (pDC,pMin1Drawing->m_nCnp);
			rt.bottom = bttm;
		}
		int leftNow=1;
		if(m_nKlineKind2==0)
			leftNow=m_rtMin1.leftX+1;
		if(m_pointMove.x<rt.right&&m_pointMove.x>=leftNow&&m_pointMove.y<rt.bottom)
			ShowShiZi(m_pointMove.x, m_pointMove.y,1);

		m_dlgShowCross->RedrawWindow();
	}

	if(bKey == false)
		m_bKD = false;
}

///////////////////////////////////////////////////////////////////
//      函 数 名：CopyCaption()
//      功能说明：画标题
//      入口参数：无
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::CopyCaption()//6.17
{
	CRect r;
	GetClientRect(r);
	if(m_infoFiguer>0)
		r.right-=m_rightLength;
	r.bottom -=m_heightCaption;
//	CDC* pDC=GetDC();

	if(m_nKlineKind2==0)
	{
		int nm=m_infoInit.nCountMin1;
		CRect rIn(0,0,r.right-m_midLen,m_heightCaption);
		
		{
		CTaiKlineDC memdc3(this,&m_bit,&rIn);
		CTaiKlineDC* pMemdc = &memdc3;

		for(int j=0;j<nm;j++)
		{
			pMin1Drawing->m_nSon =j;
			pMin1Drawing->SetRectDraw(j);
			pMin1Drawing->DrawCapt(pMemdc);
		}
		}

		for(int i=1;i<nm;i++)
		{
			CRect r2(0, m_rtMin1.rtBlw[i-1].m_yBottom,
				r.right-m_midLen, m_rtMin1.rtBlw[i-1].m_yBottom+m_heightCaption);
			CopyRect(r2);
		}

		//right box
		if(m_tabNum>3&&this->m_infoFiguer >0 && m_infoInit.nIDRightSheet [m_tabNum-4]>=1||m_tabNum==6)
		{
		}
		else
		{
			rIn=CRect(r.right ,0,r.right+m_rightLength,r.bottom );
			CTaiKlineDC memdc3(this,&m_bit,&rIn);
			CTaiKlineDC* pMemdc = &memdc3;
			CopyScrollBar(pMemdc);
	//		DrawRightbox(pMemdc,false);
			DrawRightSheet( pMemdc,m_tabNum,true);
		}
	}
	else
	{
		int nm=m_infoInit.nCountIndex;
		CRect rIn(0,0,r.right-m_midLen,m_heightCaption);

		{
			CTaiKlineDC memdc3(this,&m_bit,&rIn);
			CTaiKlineDC* pMemdc = &memdc3;

			for(int j=0;j<nm;j++)
			{
				pKlineDrawing->m_nSon =j;
				pKlineDrawing->SetRectCurrent(j);
				
				pKlineDrawing->DrawCaption(pMemdc);
			}
		}


		for(int i=1;i<nm;i++)
		{
			CRect r2(0, m_rtKlineFiguer.rtBlw[i-1].m_yBottom,
				r.right-m_midLen, m_rtKlineFiguer.rtBlw[i-1].m_yBottom+m_heightCaption);
			CopyRect(r2);
		}

		//right box history//if(pView->m_nKlineKind2 >=HS_KLINE || pView->m_nKlineKind2 == MANY_DAY_KLINE&& pDoc->m_propertyInitiate.daysOfManyKline <0)
		if(m_nKlineKind2 ==HS_KLINE ||m_nKlineKind2 ==MIN1_KLINE)//|| m_nKlineKind2 == MANY_DAY_KLINE&& pDoc->m_propertyInitiate.daysOfManyKline <0)
		{
			rIn=CRect(r.right ,0,r.right+m_rightLength,r.bottom );
			CTaiKlineDC memdc3(this,&m_bit,&rIn);
			CTaiKlineDC* pMemdc = &memdc3;
			CopyScrollBar(pMemdc);
			DrawRightSheet( pMemdc,m_tabNum,true);
		}
	}
//	ReleaseDC(pDC);


}

///////////////////////////////////////////////////////////////////
//      函 数 名：ShowTextRect(int n)
//      功能说明：显示选择哪一个子图
//      入口参数：n为第几个子图
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::ShowTextRect(int n,CDC * pDC)
{
	CPen pen_line(PS_SOLID ,1,GetDocument()->m_colorArray[1]);//PS_DASHDOTDOT   
//	CRect r;
//	GetClientRect(r);
	if(m_nKlineKind2==0)
	{
		if(m_infoInit.nCountMin1==1)
			return;
		if(n>m_infoInit.nCountMin1-2)
			n=m_infoInit.nCountMin1-2;
		pMin1Drawing->m_nTextBox=n;

		CPen* pOldpen=pDC->SelectObject(&pen_line);

		pDC->MoveTo(m_rtMin1.rightX+2,m_rtMin1.rtBlw[n].m_yBottom+2);
		pDC->LineTo(m_rtMin1.rightX+m_midLen-2,m_rtMin1.rtBlw[n].m_yBottom+2);
		pDC->LineTo(m_rtMin1.rightX+m_midLen-2,m_rtMin1.rtBlw[n+1].m_yBottom-2);
		pDC->LineTo(m_rtMin1.rightX+2,m_rtMin1.rtBlw[n+1].m_yBottom-2);
		pDC->LineTo(m_rtMin1.rightX+2,m_rtMin1.rtBlw[n].m_yBottom+2);

		pDC->SelectObject(pOldpen);
//		r.left =m_rtMin1.rightX;
//		r.right =m_rtMin1.rightX+m_midLen;

	}
	else
	{
		if(m_infoInit.nCountIndex==1)
			return;
		if(n>m_infoInit.nCountIndex-2)
			n=m_infoInit.nCountIndex-2;
		pKlineDrawing->m_nTextBox=n;
//		pKlineDrawing->DrawKlineFiguer(&memdc);

		CPen* pOldpen=pDC->SelectObject(&pen_line);

		pDC->MoveTo(m_rtKlineFiguer.rightX+2,m_rtKlineFiguer.rtBlw[n].m_yBottom+2);
		pDC->LineTo(m_rtKlineFiguer.rightX+m_midLen-2,m_rtKlineFiguer.rtBlw[n].m_yBottom+2);
		pDC->LineTo(m_rtKlineFiguer.rightX+m_midLen-2,m_rtKlineFiguer.rtBlw[n+1].m_yBottom-2);
		pDC->LineTo(m_rtKlineFiguer.rightX+2,m_rtKlineFiguer.rtBlw[n+1].m_yBottom-2);
		pDC->LineTo(m_rtKlineFiguer.rightX+2,m_rtKlineFiguer.rtBlw[n].m_yBottom+2);

		pDC->SelectObject(pOldpen);

//		r.left =m_rtKlineFiguer.rightX;
//		r.right =m_rtKlineFiguer.rightX+m_midLen;
	}
//	CopyRect(r);


}

void CTaiShanKlineShowView::ShowTransferText(CDC *pDC)
{
	CRect r;
	CString s;
	CString s1="";
	switch(m_nKlineKind2)
	{
	case 0:
		s="分时";
		break;
	case 1:
		s="分钟";
		s1="5";
		break;
	case 2:
		s="分钟";
		s1="15";
		break;
	case 3:
		s="分钟";
		s1="30";
		break;
	case 4:
		s="分钟";
		s1="60";
		break;
	case 5:
		s="日线";
		break;
	case 6:
		s="周线";
		break;
	case 7:
		s="月线";
		break;
	case 8://
		s="年线";
		break;
	case MANY_DAY_KLINE:
		{
			CString s1;
			s1.Format ("%d",abs(GetDocument()->m_propertyInitiate.daysOfManyKline));
			if(this->GetDocument()->m_propertyInitiate.daysOfManyKline >0)
				s = s1+"日线";
			else
				s = s1 + "分钟线";
		break;
		}
	case HS_KLINE:
		s="分笔线";
		break;
	case MIN1_KLINE:
		s="1分钟线";
		break;
	}
	if(m_nKlineKind2==0)
	{
		int nm=m_infoInit.nCountMin1;
		r.left=m_rtMin1.rightX+1;
		r.top=m_rtMin1.rtBlw[nm-1].m_yBottom;
		r.bottom=m_rtMin1.rtBlw[nm-1].m_yBottom+m_heightCaption;
	}
	else
	{
		int nm=m_infoInit.nCountIndex;
		r.left=m_rtKlineFiguer.rightX+1;
		r.top=m_rtKlineFiguer.rtBlw[nm-1].m_yBottom;
		r.bottom=m_rtKlineFiguer.rtBlw[nm-1].m_yBottom+m_heightCaption;
	}

	CFont ft;
	LOGFONT lgf=GetDocument()->m_fontstr[0];
//	lgf.lfHeight=-12;
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);
	pDC->SetTextAlign( TA_LEFT|TA_TOP    );//TA_CENTER||
	pDC->SetTextColor( GetDocument()->m_colorArray[FIGUER_WORD]);
	pDC->SetBkMode(TRANSPARENT);

	s1+=s;
	pDC->TextOut(r.left +1,r.top,s1);
	pDC->SelectObject(pOld);

}

void CTaiShanKlineShowView::OnXianduan() 
{
	// TODO: Add your command handler code here
	
}
void CTaiShanKlineShowView::DoDrawLine(UINT nID) 
{
	// TODO: Add your command handler code here
	pDrawLine->m_bDrawLine=TRUE;

	switch(nID)
	{
	case ID_BEGIN_LINE     :
		pDrawLine->m_bDrawLine=false;
		lineTypeDrawed=-1;
		break;
	case ID_XIAN_DUAN      :
		lineTypeDrawed=XIAN_DUAN ;
		break;
	case ID_SHUI_PING      :
		lineTypeDrawed=SHUI_PING  ;
		break;
	case ID_QU_SHI         :
		lineTypeDrawed=QU_SHI ;
		break;
	case ID_PING_XING      :
		lineTypeDrawed=PING_XING ;
		break;
	case ID_SU_DU          :
		lineTypeDrawed= SU_DU;
		break;
	case ID_SHAN_XING      :
		lineTypeDrawed=SHAN_XING ;
		break;
	case ID_GAN_SHI        :
		lineTypeDrawed=GAN_SHI ;
		break;
	case ID_HUANG_JIN      :
		lineTypeDrawed=HUANG_JIN ;
		break;
	case ID_BO_DUAN        :
		lineTypeDrawed=BO_DUAN ;
		break;
	case ID_BAI_FEN_BI     :
		lineTypeDrawed=BAI_FEN_BI ;
		break;
	case ID_HUI_GUI        :
		lineTypeDrawed=HUI_GUI ;
		break;
	case ID_HUI_GUI_DAI_1  :
		lineTypeDrawed=HUI_GUI_DAI_1  ;
		break;
	case ID_HUI_GUI_DAI_2  :
		lineTypeDrawed=HUI_GUI_DAI_2 ;
		break;
	case ID_ZHOU_QI        :
		lineTypeDrawed=ZHOU_QI ;
		break;
	case ID_NA_QI          :
		lineTypeDrawed=NA_QI ;
		break;
	case ID_ZHONG_XIAN     :
//		if(pDrawLine->m_nZhongXian==1)
		pDrawLine->m_nZhongXianDrawing=0;
		lineTypeDrawed=ZHONG_XIAN ;
		break;
	case ID_NA_QI_HU       :
		lineTypeDrawed=NA_QI_HU ;
		break;
	case ID_HUANG_JIN_HU   :
		lineTypeDrawed=HUANG_JIN_HU ;
		break;
	case ID_UP_ARROW       :
		lineTypeDrawed=UP_ARROW ;
		break;
	case ID_DOWN_ARROW     :
		lineTypeDrawed=DOWN_ARROW ;
		break;
	case ID_RECT_KIND      :
		lineTypeDrawed=RECT_KIND ;
		break;
	case ID_TEXT_KIND      :
		lineTypeDrawed=TEXT_KIND ;
		break;
	case ID_DELETE_LINE    :
		pDrawLine->DeleteLineAll();
		pDrawLine->m_lineTypeOld=-1;
		break;
	case ID_BAG_LINE     :
		pDrawLine->m_nZhongXian=0;
		lineTypeDrawed=BAG_LINE ;
		break;
	case ID_SPIRE_LINE2  :
		lineTypeDrawed=SPIRE_LINE ;
		break;
	}
	//---		设置先前的线型
//	TRACE("pDrawLine->m_lineTypeOld    =   %d\n",pDrawLine->m_lineTypeOld);
	
}
void CTaiShanKlineShowView::DoDrawLineUpdate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here//Enable
	//---		平行线
	if(m_nKlineKind2>0)
	{
		if(pCmdUI->m_nID==ID_PING_XING&&(pDrawLine->m_lineTypeOld==XIAN_DUAN||pDrawLine->m_lineTypeOld==QU_SHI||pDrawLine->m_lineTypeOld==PING_XING))
		{
			pCmdUI->Enable(TRUE);
		}
		else if(pCmdUI->m_nID==ID_PING_XING)
		{
			pCmdUI->Enable(false);
		}
	}
	else
	{
		pCmdUI->Enable(false);
	}

	if(pCmdUI->m_nID==ID_DELETE_LINE && m_nKlineKind2>0)
	{
		if(pDrawLine->m_info_lineself.GetSize()>0)
			pCmdUI->Enable(TRUE);
		else
			pCmdUI->Enable(false);
	}
	pCmdUI->SetCheck((UINT)(ID_BEGIN_LINE+lineTypeDrawed+1) == pCmdUI->m_nID);
}

void CTaiShanKlineShowView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bMultiFiguer!=1)
	{
		CView::OnLButtonDblClk(nFlags, point);
		return;
	}

	m_bMultiFiguer=0;
	CClientDC dc(this);

	CRect r;GetClientRect (r);
	int nFiguer;
	if(GetDocument()->m_systemOption.fourgraph==FALSE)//kline
		nFiguer=4;
	else
		nFiguer=9;

	float n=(float)(pow(nFiguer,0.5));

	if(n==0)
		return;
	float width=(r.right -r.left )/(float)n;
	float height=(r.bottom -r.top )/(float)n;
	if(width<=0||height<=0)
		return;

	m_bMultiFiguer=0;

	int nStk=(int)((point.y/(int)height)*n+point.x/(int)width);
	if(GetDocument()->m_pReportView !=NULL&&m_nSharesBegin == -1)//from report table
	{
		char  Code [10];
		memset(Code,0,10);
		int nRow = -1;

		m_nShares=(m_nShares+nStk)%m_nTotalStock;

		//lmb 2.1
		int nStkKind;
		GetDocument()->m_pReportView->GetStockCodeForRow(m_nShares+1,Code, nStkKind);
		m_sharesSymbol=Code ;
		m_stkKind=nStkKind ;
		ASSERT(m_sharesSymbol.GetLength()>3);

	}
	else
	{
//lmb error
		n=(float)((m_nShares+nStk)%m_nTotalStock);
		PCdat1 pStockData;
		GetDocument()->m_sharesInformation .GetStockItem(m_nSharesBegin,n,pStockData);
		if(pStockData!=NULL)
		{
		m_sharesSymbol=pStockData->id ;
		m_stkKind = pStockData->kind ;
		ASSERT(m_sharesSymbol.GetLength()>3);
		}

	}

	ShowAll(m_sharesSymbol);
	CView::OnLButtonDblClk(nFlags, point);
}


BOOL CTaiShanKlineShowView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL bRet=CView::OnCommand(wParam, lParam);	

	if(DoOnCommand(wParam, lParam) == TRUE)
		return TRUE;
	else
		return bRet;
}

///////////////////////////////////////////////////////////////////
//      函 数 名：ShowKlineBig(bool bBig)
//      功能说明：是否重新显示K线
//      入口参数：bBig确定是放大，还是取消放大
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::ShowKlineBig(bool bBig)
{
	if(bBig==TRUE)
	{
		int first=m_rectHitPos.left*pKlineDrawing->m_klinNumDefault/m_rtKlineFiguer.rightX;
		int scd=m_rectHitPos.right*pKlineDrawing->m_klinNumDefault/m_rtKlineFiguer.rightX;

		int fg=0;
		if(scd<first)
		{
			int tmp=first;
			first=scd;
			scd=tmp;
		}
		//---		判断画K线的起始点和结束点
		pKlineDrawing->m_klinNumDefaultOld=pKlineDrawing->m_klinNumDefault;
		pKlineDrawing->m_footEndOld=pKlineDrawing->m_footEnd;
		pKlineDrawing->m_footBeginOld=pKlineDrawing->m_footBegin;
		if(scd-first>=MIN_KNUM&&pKlineDrawing->m_footEnd-pKlineDrawing->m_footBegin>MIN_KNUM)
		{
			if(scd<=pKlineDrawing->m_footEnd-pKlineDrawing->m_footBegin)
			{
				pKlineDrawing->m_footEnd=pKlineDrawing->m_footBegin+scd;
				pKlineDrawing->m_footBegin=pKlineDrawing->m_footBegin+first;

				if(pKlineDrawing->m_footEnd>=m_nCountKline )
					pKlineDrawing->m_footEnd = m_nCountKline ;
				if(pKlineDrawing->m_footBegin < 0 )
					pKlineDrawing->m_footBegin = 0 ;

				pKlineDrawing->m_klinNumDefault=pKlineDrawing->m_footEnd-pKlineDrawing->m_footBegin+1;
				fg=1;
				bExtend=TRUE;
			}
			else
			{
				if(pKlineDrawing->m_footEnd-pKlineDrawing->m_footBegin-first>=MIN_KNUM)
				{
					pKlineDrawing->m_footBegin=pKlineDrawing->m_footEnd-(scd-first);

					if(pKlineDrawing->m_footEnd>=m_nCountKline )
						pKlineDrawing->m_footEnd = m_nCountKline ;
					if(pKlineDrawing->m_footBegin < 0 )
						pKlineDrawing->m_footBegin = 0 ;

					pKlineDrawing->m_klinNumDefault=pKlineDrawing->m_footEnd-pKlineDrawing->m_footBegin+1;
					fg=1;//m_footCurrent
					bExtend=TRUE;
				}
			}
		}
	}
	else
	{
		if(pKlineDrawing->m_klinNumDefaultOld<10)
			return;
		if(pKlineDrawing->m_footEndOld<0)
			return;
		if(pKlineDrawing->m_footBeginOld<0)
			return;

		pKlineDrawing->m_klinNumDefault=pKlineDrawing->m_klinNumDefaultOld;
		pKlineDrawing->m_footEnd=pKlineDrawing->m_footEndOld;
		pKlineDrawing->m_footBegin=pKlineDrawing->m_footBeginOld;
	}
	CRect r;
	GetClientRect(r);
	if(m_infoFiguer>0)
			r.right-=m_rightLength;

	CTaiKlineDC memdc(this,&m_bit,&r);
	pKlineDrawing->ValidDoKline(true);
	pKlineDrawing->DrawKlineFiguer(&memdc);

//		mnu.EnableMenuItem( ID_RESTORE_KLINE, MF_GRAYED );
}
#define MLINEBLK   48*25*3
void CTaiShanKlineShowView::WriteKLine5min(int nFoot)
{
	if(nFoot>=m_nCountKlineFile)
		return;

	CTaiKlineFileKLine::WriteKLineS (m_sharesSymbol,m_stkKind,&m_pkline [nFoot],1,0,false);
}
void CTaiShanKlineShowView::WriteKLine(int nFoot,bool bOne,bool bDelete )
{
	if(nFoot==this->m_nCountKlineToday )
		return;
	else if (nFoot>this->m_nCountKlineToday)
	{
		SetKlineExt(m_pkline, 1, nFoot);
		return;
	}

	if(m_nKlineKind2==1)
	{
		WriteKLine5min(nFoot);
		return;
	}
	if(m_nKlineKind2!=5)
		return;

	CTaiShanDoc* pDoc = GetDocument();

	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(m_sharesSymbol,m_stkKind,true);

	if(bDelete == false)
		pFile->WriteKLine (m_sharesSymbol,&m_pkline [nFoot],1,0);
	else
		pFile->DeleteKlineData(m_sharesSymbol, nFoot);

}

void CTaiShanKlineShowView::OnPrintKLine(CDC *pDC)
{
	CTaiShanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
//////////////////////////
	///////////////
	CRect	rectClip;
	pDC->GetClipBox( &rectClip );
//	pDC->LPtoDP(&rectClip);

	//---		多图显示
	if(m_bMultiFiguer==1)
	{
		//Marenan -
		//m_scrollBar->ShowScrollBar(false );	
		if(m_nKlineKind2==0)
			pMin1Drawing->ShowMultiPage(pDC);
		else
			pKlineDrawing->ShowMultiPage(pDC);
		return;
	}

	DrawFenShi(	pDC);

	pKlineDrawing->DrawKlineFiguer(pDC);

	DrawRightSheet(pDC,m_tabNum,false);
}

void CTaiShanKlineShowView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	if(pDC->IsPrinting() && pInfo != NULL)
	   pInfo->SetMaxPage(1);
	// TODO: Add your specialized code here and/or call the base class
	CView::OnPrepareDC(pDC, pInfo);
}

void CTaiShanKlineShowView::OnPrint(CDC* pDC, CPrintInfo* pInfo) //6.18
{
	// TODO: Add your specialized code here and/or call the base class
	CRect r=pInfo->m_rectDraw;
	CRect r2;
	GetClientRect(r2);

    pDC->SetMapMode(MM_TEXT);
	CRgn rgn;

//	if(!pDC->IsPrinting())
	if(rgn.CreateRectRgn (r.left,r.top,r.right,r.bottom )==ERROR)
		return ;
	pDC->SelectClipRgn (&rgn);
//	pDC->StretchBlt(r.left, r.top,
//		r.Width(), r.Height(), &memdc, 0,0,r2.right,r2.bottom , SRCCOPY  );
	
		pDC->SetMapMode(MM_ANISOTROPIC);
		int cx=pDC->GetDeviceCaps(LOGPIXELSX);
		int cy=pDC->GetDeviceCaps(LOGPIXELSY);
		//--	计算出打印区域对应的屏幕坐标
		int x=pInfo->m_rectDraw.Width();
		int y=pInfo->m_rectDraw.Height();
		m_xPrinter=x;
		m_yPrinter=y;


		pDC->SetViewportExt(x, y);

		if(x==0||y==0)
		{
			pDC->SetWindowExt(800, 600);
			m_cxExt=800;
			m_cyExt=600;
			//set contant
			CFont ft2;
			LOGFONT lgf2=GetDocument()->m_fontstr[0];//m_fontstr[0]
			ft2.CreateFontIndirect (&lgf2 );
			CFont* pOld=pDC->SelectObject(&ft2);

			CString name="上证指数jjj55";
			int y2=(pDC->GetOutputTextExtent(name)).cy+2;
			m_heightCaption=y2;

			name="111222";
			int x2=(pDC->GetOutputTextExtent(name)).cx;
			m_midLen=x2;
			pDC->SelectObject(pOld);

			OnSizeMy(800,600);
		}
		else
		{
			int y1=800;
			int x1=x*y1/y;
			m_cxExt=x1;
			m_cyExt=y1;
			pDC->SetWindowExt(x1,y1);
			//set contant
			CFont ft2;
			LOGFONT lgf2=GetDocument()->m_fontstr[0];//m_fontstr[0]
			ft2.CreateFontIndirect (&lgf2 );
			CFont* pOld=pDC->SelectObject(&ft2);

			CString name="上证指数jjj55";
			int y2=(pDC->GetOutputTextExtent(name)).cy+2;
			m_heightCaption=y2;

			name="111222";
			int x2=(pDC->GetOutputTextExtent(name)).cx;
			m_midLen=x2;
			pDC->SelectObject(pOld);

			OnSizeMy(x1,y1);
		}


		OnPrintKLine(pDC);
    	pDC->SetMapMode(MM_TEXT);

		pDC->SelectClipRgn (NULL);//&rgn2
		rgn.DeleteObject ();

		SetConstant();
		OnSizeMy(r2.Width(),r2.Height());

		RedrawWindow();

//	CView::OnPrint(pDC, pInfo);
}

void CTaiShanKlineShowView::OnKlineMode() 
{
	// TODO: Add your command handler code here
	if(m_AutoChange == TRUE)
	{
	 m_AutoChange = false;
     KillTimer(12);
	}
	if(pKlineMode==NULL)  
	{
		pKlineMode=new CTaiKlineDialogKLineMode(this);
		pKlineMode->Create(IDD_DIALOG_KLINE_MODE,this);
		pKlineMode->ShowWindow(SW_SHOW);
		RedrawWindow ();

	}
	else
	{
		pKlineMode->DestroyWindow();
		delete pKlineMode;
		pKlineMode=NULL;
	}

}

void CTaiShanKlineShowView::OnBsInfoShow() 
{
	// TODO: Add your command handler code here
	if(m_bBaseInfo==false)
		m_bBaseInfo=TRUE;
	else
		m_bBaseInfo=false;
	ShowAll(m_sharesSymbol);
//	RedrawWindow();

}

void CTaiShanKlineShowView::OnPowerDo() 
{
	// TODO: Add your command handler code here
	CTaiShanDoc* pDoc=GetDocument();
	if(m_bDoPower==false)
	{
		m_bDoPower=TRUE;
		KlineReadTransferData();
		pKlineDrawing->m_bToCacl=TRUE;
		pKlineDrawing ->m_bInitFoot =false;//m_bInitFoot
		if(m_isShowCross==1)
			m_dlgShowCross->RedrawWindow();
	}
	else
	{
		m_bDoPower=false;

		//need change 2000.2.25 to KlineReadTransferData()
		KlineReadTransferData();
		pKlineDrawing->m_bToCacl=TRUE;
		pKlineDrawing ->m_bInitFoot =false;//

		if(m_isShowCross==1)
			m_dlgShowCross->RedrawWindow();
	}
	RedrawWindow();
}

void CTaiShanKlineShowView::OnZhutuDiejia() 
{
	if(pKlineDrawing->m_nKlineCurrent>=6)
		return;
	////////////Modi By zhangjiahi///////////
	//////////////Get the stock symbol want to add in the main picture///////
	SelectStock pDlg(this,TRUE,FALSE,TRUE);
	CString s;
	if(pDlg.DoModal()==IDOK)
	{
		s = pDlg.m_strStockCode;
		
		int nStkKind = pDlg.m_stkKind;
		CReportData* pdt;	//个股的数据指针
  	    CTaiShanDoc* pDoc = GetDocument();
		if(pDoc->m_sharesInformation.Lookup(s.GetBuffer (0),pdt,nStkKind)!=0)
		{
			AddNewStock(s,nStkKind);
		}
	}
	
}
void CTaiShanKlineShowView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	m_bKeyUpDown=false;

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}
static int g_stcID[12] = {ID_MENU_1FS,ID_KLINE_HS,ID_KLINE_1_MINU,
				  ID_KLINE_5,ID_KLINE_15,ID_KLINE_30,ID_KLINE_60,
				  ID_DAY_KLINE,ID_KLINE_ZHOU2,ID_KLINE_YUE2,ID_KLINE_NIAN,
				  ID_KLINE_MANY_DAY};

BOOL CTaiShanKlineShowView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	
	CTaiShanDoc* pDoc=(CTaiShanDoc*)GetDocument();
    if(pMsg->message == WM_KEYDOWN || pMsg->message == WM_LBUTTONDOWN  
		 || pMsg->message == WM_LBUTTONDBLCLK || pMsg->message == WM_RBUTTONDOWN ||
		 pMsg->message == WM_COMMAND|| pMsg->message == WM_MOUSEMOVE
		 || pMsg->message == WM_SYSKEYDOWN)
	{
	  pDoc->m_GetFocus = false;
	}

	if(pMsg->message == WM_KEYDOWN )
		bTrackCurcorBgn=false;

    if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F8)
	{

//			  if(m_bMultiFiguer == 1)
//				  return false;
			  if(this->m_nKlineKind2 == 0)
				  m_nCurrentKlineKind = 1;
			  if(this->m_nKlineKind2 == HS_KLINE)
				  m_nCurrentKlineKind = 2;
			  if(this->m_nKlineKind2 == MIN1_KLINE)
				  m_nCurrentKlineKind = 3;
			  if(this->m_nKlineKind2 >0 &&  m_nKlineKind2<8)
				  m_nCurrentKlineKind = 3+m_nKlineKind2;
			  if(this->m_nKlineKind2==8)
				  m_nCurrentKlineKind = 11;
			  if(this->m_nKlineKind2==MANY_DAY_KLINE)
				  m_nCurrentKlineKind = 0;
			  m_nCurrentKlineKind%=12;
			  DoOnCommand((WPARAM )g_stcID[m_nCurrentKlineKind], 0);

			  return TRUE;
	}
//	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F1)
	{
	}
   	/*
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F2)
	{
			static int nCurr = 0;
			if(nCurr == 0)
			{
				nCurr=1;
				if(IsIndexStock3(m_sharesSymbol))
				{
				  CRealTimeDetailList m_hqdetail(NULL,this,TRUE);
				  m_hqdetail.DoModal();
				  return TRUE;
				}
				CSharesRealTimeDetail m_stockdetail(NULL,this,TRUE);
				m_stockdetail.DoModal();
			}
			else
			{
//			return TRUE;
				nCurr=0;
				if(IsIndexStock3(m_sharesSymbol))
				{
				  CRealTimeDetailList m_hqdetail(NULL,this,false);
				  m_hqdetail.DoModal();
				  return TRUE;
				}
				CSharesRealTimeDetail m_stockdetail(NULL,this,false);
				m_stockdetail.DoModal();
			}
			return TRUE;
	}
	*/
	
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F4)
	{
		DoF4();
		return TRUE;
	} 

	if(pMsg->message==WM_CHAR&&((pMsg->wParam>='0'&&pMsg->wParam<='9')||(pMsg->wParam>='A'&&pMsg->wParam<='z')))
	{
			CTaiKeyBoardAngelDlg m_diakeybrode(this);
			if(pMsg->wParam>='a'&&pMsg->wParam<='z')
				pMsg->wParam-=32;
			char input=pMsg->wParam;
			m_diakeybrode.input=input;
			if(pMsg->wParam>='0'&&pMsg->wParam<='9')
				m_diakeybrode.ischar=false;
			else
				m_diakeybrode.ischar=TRUE;

		if(m_diakeybrode.DoModal() != IDOK)
			return TRUE;
	    
		//pDoc->m_stkName= stock_name;
//		CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
//		CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
		if(m_diakeybrode.isresultok ==false)
		{
			CString sRtn(m_diakeybrode.inputpre );
			int k = 0;
			for(k = 0; k<sRtn.GetLength ();k++)
			{
				if(sRtn[k]<'0' || sRtn[k]>'9')
					break;
			}
			if(k>0)
			{
				CString sN = sRtn.Left (k);

				CString s2 = sRtn.Right (1);
				if(k+1 == sRtn.GetLength ())
				{
					int nn = atoi(sN);
					if(nn ==0)
						return TRUE;

					if(s2 == "R")
					{
						m_nKlineKind2= MANY_DAY_KLINE;
						GetDocument()->m_propertyInitiate.daysOfManyKline = nn;
					}
					if(s2 == "F")
					{
						m_nKlineKind2= MANY_DAY_KLINE;
						if(nn>240)
							nn =240;
						GetDocument()->m_propertyInitiate.daysOfManyKline = -nn;
					}
					ShowAll(m_sharesSymbol,false);
				}
			}
			return TRUE;
		}
		//pDoc->m_stkName = m_diakeybrode.
        CString sKeys(m_diakeybrode.result);
		if(sKeys.GetLength()==2&&!m_diakeybrode.ischar)
		{
		  if(sKeys == "01")
		  {
			  {
			  CTaiKlineDC memdc(this,&m_bit);
			  DrawRightSheet(&memdc,0);
			  }
              return TRUE;
		  }
		  if(sKeys == "03")
		  {
			  if(m_bMultiFiguer == 1) m_bMultiFiguer = 0;
			  m_nKlineKind2 = 0;
			  m_stkKind = SHZS;
			  this->ShowAll(CSharesCompute::GetIndexSymbol(0));
			  
              return TRUE;
		  }
		  if(sKeys == "04")
		  {
			  if(m_bMultiFiguer == 1) m_bMultiFiguer = 0;
			  m_nKlineKind2 = 0;
			  m_stkKind = SZZS;
			  this->ShowAll(CSharesCompute::GetIndexSymbol(1));
              return TRUE;
		  }
		  if(sKeys == "09")
		  {
			  OnToolDrawline();
              return TRUE;
		  }
		  if(sKeys == "02")
		  {
			  {
			  CTaiKlineDC memdc(this,&m_bit);
			  DrawRightSheet(&memdc,1);
 			  }
              return TRUE;
		  }
		  if(sKeys == "08")
		  {
			  if(this->m_nKlineKind2 == 0)
				  m_nCurrentKlineKind = 1;
			  if(this->m_nKlineKind2 == HS_KLINE)
				  m_nCurrentKlineKind = 2;
			  if(this->m_nKlineKind2 == MIN1_KLINE)
				  m_nCurrentKlineKind = 3;
			  if(this->m_nKlineKind2 >0 &&  m_nKlineKind2<8)
				  m_nCurrentKlineKind = 3+m_nKlineKind2;
			  if(this->m_nKlineKind2==8)
				  m_nCurrentKlineKind = 11;
			  if(this->m_nKlineKind2==MANY_DAY_KLINE)
				  m_nCurrentKlineKind = 0;
			  m_nCurrentKlineKind%=12;
			  DoOnCommand((WPARAM )g_stcID[m_nCurrentKlineKind], 0);
		      this->SetFocus();
			  return TRUE;
		  }
		  if(sKeys == "10")
		  {
		    CTaiShanDoc *pDoc = GetDocument();
			pDoc->m_GetFocus = false;
			CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
	        ((CMainFrame *)pMainWnd)->OnCompanyinfo();
			return TRUE;
		  }
		  if(sKeys == "05")
		  {
		   	 if(m_nKlineKind2 == 0)
				 OnDayKline();
			 else
				 OnMenu1fs();
			 this->SetFocus();
			 return TRUE;
		  }
		}

        if(m_diakeybrode.ischar && m_diakeybrode.isresultstock == FALSE)
		{
		  DrawZhibiao(m_diakeybrode.result);
		  return TRUE;
		}
//		if(m_nKlineKind2==0)
		{
			if(m_diakeybrode.isresultstock ==TRUE)
			{
				bExtend=false;

				if(::GetKeyState(VK_CONTROL)&0x8000)
				{
					CString s3 = m_diakeybrode.result;
					AddNewStock(s3,m_diakeybrode.m_stkKind);
				}
				else
				{
				m_sharesSymbol = m_diakeybrode.result;
				m_stkKind = m_diakeybrode.m_stkKind;
				ShowAll(m_sharesSymbol);

//				pMin1Drawing->	InitMinuteLine();
//				RedrawWindow();
//				CaclStockPos();
				}
				return TRUE;
			}
			CString sKey(m_diakeybrode.result);
			if(sKey.GetLength()==2&&!m_diakeybrode.ischar)
			{
				//hyt
				pDoc->m_pReportView->GetParentFrame()->ActivateFrame();
				int m_key=atoi(sKey);
				if(m_key == 3) this->m_stkKind = SHZS;
				if(m_key == 4) this->m_stkKind = SZZS;
				
				::SendMessage(pDoc->m_pReportView->m_hWnd,WM_USER_FROM_KLINE,0,m_key); 
			}
			else if(sKey.GetLength()==1)
			{
				if(sKey[0]>='0' && sKey[0]<='9' )
				{
					int nn = sKey[0] - '0';
					int nID[] = {ID_KLINE_HS,ID_KLINE_1_MINU,
									  ID_KLINE_5,ID_KLINE_15,ID_KLINE_30,ID_KLINE_60,
									  ID_DAY_KLINE,ID_KLINE_ZHOU2,ID_KLINE_YUE2,
									  ID_KLINE_MANY_DAY};
					DoOnCommand((WPARAM )nID[nn], 0);
				}
			}
			return TRUE;
				
		}
		//--删除多余内存
/*		if(m_diakeybrode.isresultstock ==TRUE)
		{
			if(::GetKeyState(VK_CONTROL)&0x8000)
			{
				CString s3 = m_diakeybrode.result;
				AddNewStock(s3);
				return TRUE;
			}

			pKlineDrawing -> m_bNewStock = TRUE ;

			m_sharesSymbol = m_diakeybrode.result;

			ShowAll(m_sharesSymbol);
//			CaclStockPos();

			return TRUE;
		}

		CString sKey(m_diakeybrode.result);
		if(sKey.GetLength()!=2)
			return TRUE;
		//hyt
		if(sKey.GetLength()==2&&!m_diakeybrode.ischar)
		{
			//hyt
			pDoc->m_pReportView->GetParentFrame()->ActivateFrame();
			int m_key=atoi(sKey);
			::SendMessage(pDoc->m_pReportView->m_hWnd,WM_USER_FROM_KLINE,0,m_key); 
			return TRUE; 
		}
*/
	}
	
	return CView::PreTranslateMessage(pMsg);
}
void CTaiShanKlineShowView::OnViewGpHuanQuan() 
{
}

void CTaiShanKlineShowView::OnUpdateViewGphq(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}


void CTaiShanKlineShowView::OnPageDown(bool bDown,int nPer)
{
	pKlineDrawing -> m_bNewStock = TRUE ;

	bExtend=false;
	CTaiShanDoc* pDoc = GetDocument();
	//int n;

	if(pDoc->m_pReportView !=NULL&&m_nSharesBegin == -1)//from report table
	{
		char  Code [10];
		memset(Code,0,10);
		int nRow = -1;

		if(bDown==TRUE)
		{
			m_nShares=(m_nShares+nPer)%m_nTotalStock;
		}
		else
		{
			if(m_nShares-nPer<0)
			{
				m_nShares=m_nShares-nPer+m_nTotalStock;
			}
			else
			{
				m_nShares=m_nShares-nPer;
			}
		}

		int nStkKind;
		pDoc->m_pReportView->GetStockCodeForRow(m_nShares+1,Code,nStkKind);
		m_sharesSymbol=Code ;
		m_stkKind=nStkKind ;
		ASSERT(m_sharesSymbol.GetLength()>3);

	}
	else
	{

		if(bDown==TRUE && m_nTotalStock != 0)
		{
			m_nShares=(m_nShares+nPer)%m_nTotalStock;
		}
		else
		{
			if(m_nShares-nPer<0)
			{
				m_nShares=m_nShares-nPer+m_nTotalStock;
			}
			else
			{
				m_nShares=m_nShares-nPer;
			}
		}

		PCdat1 pStockData;
		pDoc->m_sharesInformation .GetStockItem(m_nSharesBegin,m_nShares,pStockData);
		m_sharesSymbol=pStockData->id ;
		m_stkKind=pStockData->kind ;
		ASSERT(m_sharesSymbol.GetLength()>3);

	}
	if(	m_nShares>=m_nTotalStock) return;


//	pKlineDrawing->m_klinNumDefault = 82;
	if(this->m_nCountKline > 82 && pKlineDrawing->m_klinNumDefault<82)
		pKlineDrawing->m_klinNumDefault = 82;

	ShowAll(m_sharesSymbol,false);
}
//功能:计算股票的位置
void CTaiShanKlineShowView::CaclStockPos()
{
	CTaiShanDoc* pDoc = GetDocument();
	if(pDoc->m_pReportView !=NULL)
	{
		int nRow = -1;
		if(pDoc->m_pReportView->GetRowForStockCode(m_sharesSymbol .GetBuffer(0), nRow) == TRUE)
		{
			m_nSharesBegin = -1;
			m_nTotalStock = pDoc->m_pReportView->GetPageRows();
			m_nShares = nRow;
			if(m_nShares <0)
				m_nShares =0;
			m_sharesSymbol.ReleaseBuffer();
			return;
		}
	}

	m_nSharesBegin = m_stkKind;//pDoc->m_sharesInformation .GetStockType(m_sharesSymbol.GetBuffer(0));
	m_sharesSymbol.ReleaseBuffer();
	m_nTotalStock = pDoc->m_sharesInformation .GetStockTypeCount(m_nSharesBegin);
	m_nShares =pDoc->m_sharesInformation.GetStockPos(m_nSharesBegin,m_sharesSymbol.GetBuffer(0));//need change
	m_sharesSymbol.ReleaseBuffer();
	if(m_nShares<0)
		m_nShares =0;
	m_sharesSymbol.ReleaseBuffer();
}

void CTaiShanKlineShowView::ReadDataMultiFiguer(int nItem)
{
	//need opt
	CTaiShanDoc* pDoc = GetDocument();
	int n;
	m_sharesSymbol = GetMultiSymbol(nItem,m_stkKind);

	pMin1Drawing ->InitMinuteLine();
	if(this->m_nKlineKind2 == 0)
		return;

	n=0;
	int per=82;
	switch(m_nKlineKind2)
	{
	case 1:
	case 5:
		n=per;
		break;
	case 2:
		n=per*3;
		break;
	case 3:
		n=per*6;
		break;
	case 4:
		n=per*12;
		break;
	case 6:
		n=per*5;
		break;
	case 7:
		n=per*23;
		break;
	case 8:
		n=-1;
		break;
	case MANY_DAY_KLINE:
		if(GetDocument()->m_propertyInitiate.daysOfManyKline>0)
			n=per*abs(GetDocument()->m_propertyInitiate.daysOfManyKline);
		break;
	}
	if(m_nKlineKind2!=MIN1_KLINE&&m_nKlineKind2!=HS_KLINE&&(!(m_nKlineKind2==MANY_DAY_KLINE&&pDoc->m_propertyInitiate.daysOfManyKline<0)))
	{
		m_nCountKline=ReadKLine(m_sharesSymbol,m_stkKind,m_pkline,n);
		n=m_nCountKline;
		m_nCountKlineFile=m_nCountKline;
		m_nCountKlineToday = m_nCountKline;
	}

	//m_nCountKlineFile
	m_nKline9[nItem] = m_nCountKlineFile;

	pKlineDrawing->TransferKlineTypeData(m_sharesSymbol,m_stkKind ,m_pkline,-1,n);

}

void CTaiShanKlineShowView::OnDestroy() 
{

	CTaiShanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_pWideNet) m_pWideNet->ReleaseID();
	if(m_pWideNet) delete m_pWideNet;
	m_pWideNet = NULL;

	int k;
	for(k= 0;k<pDoc->m_arKLineViews.GetSize();k++)
	{
		if(pDoc->m_arKLineViews[k] == this)
		{
			pDoc->m_arKLineViews.RemoveAt(k);
		}
	}
	CView::OnDestroy();
	if(m_nKlineKind2==0)
		KillTimer(TM_FENSHI);

	if(	m_dlgShowCross != NULL)
		m_dlgShowCross->DestroyWindow();
	if( m_MyToolTip != NULL)
		m_MyToolTip->DestroyWindow();
	m_scrollBar->DestroyWindow();

	if(pKlineMode!=NULL)
	{
		pKlineMode->DestroyWindow();
		delete pKlineMode;
		pKlineMode=NULL;
	}
	if(m_pChangeIndexParam!=NULL)
	{
		m_pChangeIndexParam->DestroyWindow();
		delete m_pChangeIndexParam;
		m_pChangeIndexParam=NULL;
	}

	if(m_dlgKlineTj) m_dlgKlineTj->DestroyWindow();
	if(m_dlgFenshiTj) m_dlgFenshiTj->DestroyWindow();
	if(m_dlgZhibiao != 0)
		m_dlgZhibiao->DestroyWindow();

	// TODO: Add your message handler code here
	
}

void CTaiShanKlineShowView::BuySell()
{
	if(m_infoFiguer>0)
		m_infoFiguer=0;
	else
		m_infoFiguer=1;
	  CRect r;
	  GetClientRect(r);
	  OnSizeMy(r.right -r.left , r.bottom -r.top );
	  RedrawWindow();
}

void CTaiShanKlineShowView::OnViewBuysell() 
{
    BuySell();	
}

void CTaiShanKlineShowView::OnRealTime() 
{
	// TODO: Add your command handler code here
	OnMenu1fs();
}

void CTaiShanKlineShowView::OnDayLine() 
{
  OnDayKline();
}



void CTaiShanKlineShowView::OnUpdateBsInfoShow(CCmdUI* pCmdUI) 
{
	if(m_nKlineKind2 == 0)
		pCmdUI->Enable(false);
	else
	pCmdUI->SetCheck(m_bBaseInfo == TRUE?1:0);	
}

void CTaiShanKlineShowView::OnUpdateViewBuysell(CCmdUI* pCmdUI) 
{
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
		pCmdUI->SetCheck(m_infoFiguer > 0?1:0);	
		pCmdUI->Enable (TRUE);
	}
}

void CTaiShanKlineShowView::OnUpdateViewDszb(CCmdUI* pCmdUI) 
{
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
	pCmdUI->Enable(m_nKlineKind2 == 0?false:TRUE);
	pCmdUI->SetCheck(pKlineDrawing->m_axisType == 2?1:0);	
//		pCmdUI->Enable (TRUE);
	}
}

void CTaiShanKlineShowView::OnUpdateViewSimple(CCmdUI* pCmdUI) 
{
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
    pCmdUI->Enable(m_nKlineKind2 == 0?false:TRUE);
	pCmdUI->SetCheck(pKlineDrawing->m_axisType == 0?1:0);
//		pCmdUI->Enable (TRUE);
	}
	
}

void CTaiShanKlineShowView::OnUpdateViewPersent(CCmdUI* pCmdUI) 
{
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
	pCmdUI->Enable(m_nKlineKind2 == 0?false:TRUE);
	pCmdUI->SetCheck(pKlineDrawing->m_axisType == 1?1:0);	
//		pCmdUI->Enable (TRUE);
	}
}

void CTaiShanKlineShowView::OnViewOne() 
{
     if(m_bMultiFiguer == 0)
		 return;
	 CClientDC dc(this);
	 m_bMultiFiguer=0;
	 ShowAll(m_sharesSymbol);//lmb error
}


void CTaiShanKlineShowView::OnUpdateViewOne(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bMultiFiguer == 0?1:0);	
}

void CTaiShanKlineShowView::OnViewFourstock() 
{
	m_bMultiFiguer=1;
	RedrawWindow();
}

void CTaiShanKlineShowView::OnUpdateViewFourstock(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bMultiFiguer == 1?1:0);	
	
}

void CTaiShanKlineShowView::OnViewLine() 
{
   OnMenu1fs();	
}

void CTaiShanKlineShowView::OnUpdateViewLine(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(m_nKlineKind2 == 0?1:0);	
}

void CTaiShanKlineShowView::OnViewDayline() 
{
  OnDayKline();	
}


void CTaiShanKlineShowView::OnUpdateViewDayline(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nKlineKind2 == 5?1:0);
	
}

void CTaiShanKlineShowView::OnViewFifteenmin() 
{		    
	DoOnCommand((WPARAM) ID_KLINE_15, (LPARAM) 0);	
}

void CTaiShanKlineShowView::OnUpdateViewFifteenmin(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(m_nKlineKind2 == 2?1:0);	
}

void CTaiShanKlineShowView::OnViewThirtymin() 
{
	DoOnCommand((WPARAM) ID_KLINE_30, (LPARAM) 0);	
}


void CTaiShanKlineShowView::OnUpdateViewThirtymin(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_nKlineKind2 == 3?1:0);		
}

void CTaiShanKlineShowView::OnViewSixtymin() 
{
	DoOnCommand((WPARAM) ID_KLINE_60, (LPARAM) 0);	
}


void CTaiShanKlineShowView::OnUpdateViewSixtymin(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_nKlineKind2 == 4?1:0);
}

void CTaiShanKlineShowView::OnViewMonthline() 
{
	DoOnCommand((WPARAM) ID_KLINE_YUE2, (LPARAM) 0);	
}

void CTaiShanKlineShowView::OnUpdateViewMonthline(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_nKlineKind2 == 7?1:0);	
}

void CTaiShanKlineShowView::OnViewWeekline() 
{
	//ID_KLINE_ZHOU2
	DoOnCommand((WPARAM) ID_KLINE_ZHOU2, (LPARAM) 0);	
}

void CTaiShanKlineShowView::OnUpdateViewWeekline(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_nKlineKind2 == 6?1:0);	
}

void CTaiShanKlineShowView::OnView5min() 
{
	DoOnCommand((WPARAM) ID_KLINE_5, (LPARAM) 0);	

}

void CTaiShanKlineShowView::OnUpdateView5min(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_nKlineKind2 == 1?1:0);
}

void CTaiShanKlineShowView::OnViewOneitem() 
{
	DoViewItem(1);
}

void CTaiShanKlineShowView::OnUpdateViewOneitem(CCmdUI* pCmdUI) 
{
	DoUpdateViewItem(1,pCmdUI);
}

void CTaiShanKlineShowView::OnViewTwoitem() 
{
	DoViewItem(2);
}

void CTaiShanKlineShowView::OnUpdateViewTwoitem(CCmdUI* pCmdUI) 
{
	DoUpdateViewItem(2,pCmdUI);
}

void CTaiShanKlineShowView::OnViewFouritem() 
{
	DoViewItem(4);
}

void CTaiShanKlineShowView::OnUpdateViewFouritem(CCmdUI* pCmdUI) 
{
	DoUpdateViewItem(4,pCmdUI);
}

void CTaiShanKlineShowView::OnViewThreeitem() 
{
	DoViewItem(3);
}

void CTaiShanKlineShowView::OnUpdateViewThreeitem(CCmdUI* pCmdUI) 
{
	DoUpdateViewItem(3,pCmdUI);
}

void CTaiShanKlineShowView::OnUpdateZhutuDiejia(CCmdUI* pCmdUI) 
{
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
		if(m_nKlineKind2 != HS_KLINE)
		{
			if(m_nKlineKind2 == 0)
				pCmdUI->Enable(pMin1Drawing->m_nKlineCurrent >=5?0:1);
			else
				pCmdUI->Enable(pKlineDrawing->m_nKlineCurrent >=6?0:1);
		}
		else
			pCmdUI->Enable (FALSE);
	}
}

void CTaiShanKlineShowView::ShowAll(CString sharesymbol,bool bCaclStockPos,	bool bInitFoot)
{
		//test sharesymbol
		CReportData* pdt;	//个股的数据指针
  	    CTaiShanDoc* pDoc = GetDocument();
		if(pDoc->m_sharesInformation.Lookup(sharesymbol.GetBuffer (0),pdt,m_stkKind)==0)
		{
			sharesymbol.ReleaseBuffer ();
			sharesymbol = CSharesCompute::GetIndexSymbol(0);
		}
		else
			sharesymbol.ReleaseBuffer ();
		//end

	    m_sharesSymbol = sharesymbol;
		m_tabNum = IsIndexStock(m_sharesSymbol)?0:m_tabNum;
		if(m_bMultiFiguer==1)
		{
			int nFiguer;
			if(pDoc->m_systemOption.fourgraph==FALSE)//kline
				nFiguer=4;
			else
				nFiguer=9;
			for(int j= 0;j<nFiguer;j++)
			{
				this->m_symbol9 [j] = GetMultiSymbol(j,m_stkKind9[j]);
			}
		}
		//宽带版数据申请
		if(m_pWideNet) m_pWideNet->AddStockFirst(m_sharesSymbol,m_stkKind);

		ASSERT(m_sharesSymbol.GetLength()>3);
		pMin1Drawing->InitMinuteLine();

		if(m_nKlineKind2==0)
		{
			RedrawWindow();
			if(bCaclStockPos==true)
				CaclStockPos();
			return;
		}
		if(m_bBaseInfo==TRUE)
		{
			CString s,s1,s2;
			if(CSharesCompute::GetMarketKind(m_stkKind) == SH_MARKET_EX)
				s2="'1A0001'";
			else
				s2="'2A01'";
			m_bs.m_strFilter="Sheet1.symbol = "+s2+" OR Sheet1.symbol = '"+m_sharesSymbol+"' ";
			try
			{
				if(m_bs.IsOpen())
					m_bs.Requery ();
				else
					m_bs.Open ();
			}
			catch (CDaoException* e)
			{
				e->Delete();
			}
		}


		this->KlineReadTransferData ();
		pKlineDrawing->m_bToCacl = TRUE;

		pKlineDrawing->m_bInitFoot = bInitFoot;//
		pDrawLine->m_lineTypeOld = -1;
		if(bCaclStockPos==true)
		{
			pKlineDrawing->m_klinNumDefault=82;
			pKlineDrawing->m_bNewStock = true;
		}
		RedrawWindow();
		if(bCaclStockPos==true)
			CaclStockPos();

		if(m_isShowCross==1)
			m_dlgShowCross->RedrawWindow();

}

void CTaiShanKlineShowView::OnKillFocus(CWnd* pNewWnd) 
{
   CTaiShanDoc *pDoc = GetDocument();
	if(pDoc->m_GetFocus == TRUE)
	{
		this->SetFocus();
		return;
		
    }
	CView::OnKillFocus(pNewWnd);

}
void CTaiShanKlineShowView::OnFileOpen1() 
{
	SelectStock pDlg(IDD_STOCKTYPE1,this,TRUE,FALSE,TRUE);
	CString s;
	if(pDlg.DoModal()==IDOK)
	{
		s = pDlg.m_strStockCode;
		m_stkKind = pDlg.m_stkKind;
		this->ShowAll(s);
	}
}
void CTaiShanKlineShowView::OnTjxgTjxgts() 
{
	// TODO: Add your command handler code here
	if(m_AutoChange == TRUE)
	{
	 m_AutoChange = false;
     KillTimer(12);
	}
}


void CTaiShanKlineShowView::OnUpdateTjxgTjxgts(CCmdUI* pCmdUI) 
{
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
    if(m_nKlineKind2 == 0)
	{
	  pCmdUI->Enable(FALSE);
	  return;
	}
	pCmdUI->SetCheck(m_tjxgshow == 1?0:1);	
//		pCmdUI->Enable (TRUE);
	}
}
void CTaiShanKlineShowView::OnToolCursor() 
{
    if(m_AutoChange == TRUE)
	{
	 m_AutoChange = false;
     KillTimer(12);
	}
	if(m_isShowCross==0)
	{
		CRect r(0,0,0,0);
		if(m_nKlineKind2>0)
			(pKlineDrawing->OnLeft());
		else
			pMin1Drawing->OnLeft();

		ShowCross(false);
		m_isShowCross=1;
		RedrawWindow();
	}
	else
	{
		int rtn=0;
		m_dlgShowCross->EndDialog(rtn);
		m_isShowCross=0;
		RedrawWindow();
		return;
	}
}

void CTaiShanKlineShowView::OnUpdateToolCursor(CCmdUI* pCmdUI) 
{
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
	   pCmdUI->SetCheck(m_isShowCross == 0?0:1);	
		pCmdUI->Enable (TRUE);
	}
}

void CTaiShanKlineShowView::OnUpdatePowerDo(CCmdUI* pCmdUI) 
{
	if(m_nKlineKind2 == 0)
      pCmdUI->Enable(false);	
	else
		pCmdUI->SetCheck(m_bDoPower == TRUE?1:0);	
}

void CTaiShanKlineShowView::OnViewJszb() 
{
	CFxjSelectFormu dlg;
	dlg.m_bOnlyIndicator = true;
	if(dlg.DoModal () == IDOK)
	{
		DrawZhibiao(dlg.GetJishu() ->name);
	}
}

void CTaiShanKlineShowView::DrawRightSheet(CDC* pDC,int Num,bool bShowScroll)
{      
		if(m_infoFiguer==0)
		{
			//Marenan -
			//m_scrollBar->ShowScrollBar(FALSE );	
			m_scrollBar->ShowWindow(SW_HIDE);	
			m_pRichEdit->ShowWindow(SW_HIDE);
			return;
		}

//		if(IsIndexStock(m_sharesSymbol))
		//Marenan +
		if ((Num<2)||(Num==4))
		{
			m_scrollBar->ShowWindow(SW_SHOW);	
			m_bZoomEnable=TRUE; 
		}
		else
		{
			m_scrollBar->ShowWindow(SW_HIDE);	
			m_bZoomEnable=FALSE;
		}
		//Marenan +
		if(m_tabNum<4)
			DrawRightbox(pDC);
		else if(m_infoInit.nIDRightSheet [m_tabNum-4]==0 || IsIndexStock(m_sharesSymbol))
			DrawRightbox(pDC);

	    int len=m_rightLength;
		CRect rt;
		GetCurrClientRect(rt);
		if(len==0)
			return;
		m_tabNum = Num;
		if (m_tabNum!=6)
		{
			if (m_pRichEdit->IsWindowVisible())
				m_pRichEdit->ShowWindow(SW_HIDE);
		}

		if(m_infoFiguer!=1)
		{
			if(IsIndexStock(m_sharesSymbol))
			{
				//Marenan -
				//m_scrollBar->ShowScrollBar(FALSE );	
				//Marenan +
				m_scrollBar->ShowWindow(SW_HIDE);	

				DrawLineCurve(pDC);
				switch(m_tabNum)
				{
					case 4:
					case 5:
						switch(m_infoInit.nIDRightSheet [m_tabNum-4])
						{
							case 0:
								DrawFinace(pDC,m_nBeginHS);
								break;
							case 1:
								pKlineDrawing->m_pMoveCB->BuySellFiguer(pDC);
								break;//BuySellFiguer(CDC *pDC)
							case 2:
							{
								CRect r(rt.right -m_rightLength+1,0,rt.right ,rt.bottom -gScrollBott-5);
								ClearBack(pDC,r);
									pKlineDrawing->m_pMoveCB->ShowFxjMoveCB(pDC);
									break;
							}
							case 3:
							case 4:
							case 5:
								{
									CRect r(rt.right -m_rightLength+1,0,rt.right ,rt.bottom -gScrollBott-5);
									ClearBack(pDC,r);
									pKlineDrawing->m_pMoveCB->ShowMoveCB(pDC,m_infoInit.nIDRightSheet [m_tabNum-4]-2);
								}
								break;
						}
						break;
					case 6:
						CRect r(rt.right-m_rightLength+1,0,rt.right,rt.bottom-gScrollBott-5);
						ClearBack(pDC,r);
						DrawPingCe(pDC,r);
						break;
				}
				return;
			}
			else m_infoFiguer=1;
		}

		DrawLineCurve(pDC);
		switch(m_tabNum)
		{
			case 0:
			case 1:
				pMin1Drawing->DrawHs(pDC,m_nBeginHS, m_tabNum);
				break;
			case 2:
				pMin1Drawing->DrawDapan(pDC);
				break;
			case 3:
				pMin1Drawing->DrawLittleGegu(pDC);
				break;
			case 4:
			case 5:
				switch(m_infoInit.nIDRightSheet [m_tabNum-4])
				{
					case 0:
						DrawFinace(pDC,m_nBeginHS);
						break;
					case 1:
						{
							CRect r(rt.right -m_rightLength+1,0,rt.right ,rt.bottom -gScrollBott-5);
							ClearBack(pDC,r);
							pKlineDrawing->m_pMoveCB->BuySellFiguer(pDC);
						}
						break;//BuySellFiguer(CDC *pDC)
					case 2:
						{
							CRect r(rt.right -m_rightLength+1,0,rt.right ,rt.bottom -gScrollBott-5);
							ClearBack(pDC,r);
							//pKlineDrawing->m_pMoveCB->ShowMoveCB(pDC);
							pKlineDrawing->m_pMoveCB->ShowFxjMoveCB(pDC);
							break;
						}
					case 3:
					case 4:
					case 5:
						{
							CRect r(rt.right -m_rightLength+1,0,rt.right ,rt.bottom -gScrollBott-5);
							ClearBack(pDC,r);
							pKlineDrawing->m_pMoveCB->ShowMoveCB(pDC,m_infoInit.nIDRightSheet [m_tabNum-4]-2);
						}
						break;
					case 6:
						{
							CRect r(rt.right -m_rightLength+1,0,rt.right ,rt.bottom -gScrollBott-5);
							ClearBack(pDC,r);
							ByjDrawRightRect byj(this,CMainFrame::m_pDoc );
							byj.CalcSonFiguer(pDC,r);
						}
						break;
				}
				break;
			case 6:
				CRect r(rt.right-m_rightLength+1,0,rt.right,rt.bottom-gScrollBott-5);
				DrawPingCe(pDC,r);
				break;
		}
		//Marenan -
		//if(bShowScroll==false)
		//	return;

		//CRect rt1(rt.right-m_rightLength+1,19*RIGHTBOX_PERLINE+3,rt.right,rt.bottom);

		//if(m_scrollBar->m_hWnd !=NULL)
		//{
		//	if(m_tabNum==0||m_tabNum==1)
		//	{
		//		m_scrollBar->ShowScrollBar(TRUE );
		//	}
		//	else if(m_tabNum>3)
		//	{
		//		if(m_infoInit.nIDRightSheet [m_tabNum-4]==0)
		//			m_scrollBar->ShowScrollBar(TRUE );
		//		else
		//			m_scrollBar->ShowScrollBar(FALSE );	
		//	}
		//	else m_scrollBar->ShowScrollBar(FALSE );	
		//}

		//CopyScrollBar(pDC);
		//Marenan -

		//Marenan +
		if(bShowScroll==false)
		{
			m_scrollBar->ShowWindow(SW_HIDE);	
			return;
		}

		CRect rt1(rt.right-m_rightLength+1,RIGHTBOX_LINECOUNT*RIGHTBOX_PERLINE+1,rt.right,rt.bottom);

		/*		if(m_scrollBar->m_hWnd !=NULL)
		{
		if(m_tabNum==0||m_tabNum==1)
		{
		m_scrollBar->ShowWindow(SW_SHOW);
		}
		else if(m_tabNum>3)
		{
		if(m_infoInit.nIDRightSheet [m_tabNum-4]==0)
		m_scrollBar->ShowWindow(SW_SHOW);
		else
		m_scrollBar->ShowWindow(SW_HIDE);	
		}
		else m_scrollBar->ShowWindow(SW_HIDE);;	
		}*/
		if ((Num<2)||(Num==4))
			CopyScrollBar(pDC);
		//Marenan +

}

void CTaiShanKlineShowView::ReDrawSheet(BOOL UpOrDown)
{
//	CClientDC dc(this);
  if(UpOrDown == TRUE)
  {
	  m_tabNum++;
	  if(m_tabNum > 6)
		  m_tabNum = 0;
  }
  else
  {
    m_tabNum--;
    if(m_tabNum < 0)
	   m_tabNum = 6;
  }
  {
	CTaiKlineDC memdc(this,&m_bit);
	DrawRightSheet(&memdc,m_tabNum);
  }
}
///////////////////lmb lmb

void CTaiShanKlineShowView::DrawZhibiao(CString ZhibiaoName,int nFiguer)
{
	if(m_nKlineKind2 == 0)
		return;

	int nFgr;
	if(nFiguer == -1)
		nFgr = pKlineDrawing->m_nTextBox+1;
	else if(nFiguer == -2)
	{
		nFgr = PointToFiguer();
	}
	else
		nFgr = nFiguer;
		
	
	if(ZhibiaoName=="QLDJ"||ZhibiaoName=="HLDJ"||ZhibiaoName=="QEDJ"||ZhibiaoName=="HEDJ")
	{
		strcpy(m_infoInit.initIndex[0].nameIndex,ZhibiaoName);
		RedrawWindow();
		return;
	}
	
	if(ZhibiaoName=="TWR")
	{
		GetDocument()->m_systemOption.kline=2;
		RedrawWindow();
		return;
	}
	if(ZhibiaoName=="K")
	{
		GetDocument()->m_systemOption.kline=0;
		RedrawWindow();
		return;
	}
	if(ZhibiaoName=="BAR")
	{
		GetDocument()->m_systemOption.kline=1;
		RedrawWindow();
		return;
	}

   for(int i=0;i<GetDocument()->m_formuar_index.GetSize ();i++)
   {
			CFormularContent * jishu;
			jishu=GetDocument()->m_formuar_index.GetAt(i);
			if(jishu->name == ZhibiaoName)
			{
				if(jishu->bRightBox )
				{
					ByjDrawRightRect byj(this,GetDocument());
					byj.SaveName (jishu->name);
					if(this->m_tabNum !=4 && this->m_tabNum !=5)
						m_tabNum = 4;
					m_infoInit.nIDRightSheet [m_tabNum-4] = IDM_POP_FINANCE+6;
					DoOnCommand(IDM_POP_FINANCE+6, 0);
					return;
				}
				CString sName(jishu->name);
				sName.MakeUpper();
				//主图叠加//	if(::GetKeyState(VK_CONTROL)&0x8000)

				if(::GetKeyState(VK_CONTROL)&0x8000)
				{
					if(m_hit == REGION_PICTTEXT1 || m_hit == REGION_PICT1) nFgr = 0 ;//
					if(pKlineDrawing->m_sIndicatorAdded[nFgr] == sName)
						pKlineDrawing->m_sIndicatorAdded[nFgr] = "";
					else
						pKlineDrawing->m_sIndicatorAdded[nFgr] = sName;
				}
				else
				{
					if(jishu->isMainFiguer ==0)
					{
						int nm = m_infoInit.nCountIndex;
						if(nm == 1)
							return;
						//strcpy(m_infoInit.initIndex[nm+3-REGION_PICT1].nameIndex,mdiaxzzb.result);
						if(nFgr>0)
							strcpy(m_infoInit.initIndex[nFgr].nameIndex,ZhibiaoName);
					}
					else if(jishu->isMainFiguer == 1)
					{
						if(strcmp(m_infoInit.initIndex[0].nameIndex,ZhibiaoName) == 0)
							m_infoInit.initIndex[0].nameIndex[0] = '\0';
						else
							strcpy(m_infoInit.initIndex[0].nameIndex,ZhibiaoName);
					}
					else
						return;
				}
				pKlineDrawing->m_bToCacl =TRUE;
				pKlineDrawing ->m_bInitFoot =false;//
				RedrawWindow();
				return;
			}
   }

}
void CTaiShanKlineShowView::SetConstant()
{
	CFont ft2;
	CClientDC dc(this);
	CDC* pDC=&dc;

	LOGFONT lgf2=GetDocument()->m_fontstr[0];//m_fontstr[0]
//	lgf2.lfHeight=-10;
//	lgf2.lfWeight =FW_BOLD;
	ft2.CreateFontIndirect (&lgf2 );
	CFont* pOld=pDC->SelectObject(&ft2);

	CString name="上证指数jjj55";
	int y=(pDC->GetOutputTextExtent(name)).cy+2;
	m_heightCaption=y;

	name="888888";
	int x=(pDC->GetOutputTextExtent(name)).cx;
	m_midLen=x;


	pDC->SelectObject(pOld);

}


void CTaiShanKlineShowView::OnViewstockdetail() 
{	
    CDiaStockdetail m_stockdetail(NULL,this,TRUE);
    m_stockdetail.DoModal();
}

LRESULT CTaiShanKlineShowView::OnDataChange(WPARAM wParam, LPARAM lParam)
{
	if(m_bInited == false)
		return 0L;
	if(m_bMultiFiguer == 1)
		return 0L;
	if(lParam==999)
	{
		SetConstant();
		CRect r;
		GetClientRect(r);
		OnSizeMy(r.Width(),r.Height());
	}
	if(lParam==200 ||lParam==999)
	{
		ShowAll(m_sharesSymbol);
		//Marenan +
		m_scrollBar->SetColor(TRUE,GetDocument()->m_colorArray[0],GetDocument()->m_colorArray[2]);
		return 0L;
	}
	CTaiShanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CReportData* pdt;	//个股的数据指针
	if(pDoc->m_sharesInformation.Lookup(m_sharesSymbol.GetBuffer (0),pdt,m_stkKind)==0)
		return 0L;

	if(pdt->totv<=pMin1Drawing->m_volPre)
		return 0L;

	pKlineDrawing->m_bToCacl=TRUE;
	//lmb7.6
	if(m_nKlineKind2>0 )
	{
		if( m_nKlineKind2<5)
			pKlineDrawing ->m_bInitFoot =true;//
		else if( m_nKlineKind2>8)
			pKlineDrawing ->m_bInitFoot =true;//
		else
			pKlineDrawing ->m_bInitFoot =false;//
		pMin1Drawing->InitMinuteLine();
	}

//	TRACE("data change = %s",m_sharesSymbol);
	m_scrollBar->SetColor(FALSE,GetDocument()->m_colorArray[0],GetDocument()->m_colorArray[2]);

	RedrawWindow();

	return 1L;
}

void CTaiShanKlineShowView::OnF2() 
{
	CDiaStockdetail m_stockdetail(NULL,this,false);
    m_stockdetail.DoModal();
}
void CTaiShanKlineShowView::OnViewYearkline() 
{
	DoOnCommand((WPARAM) ID_KLINE_NIAN, (LPARAM) 0);	
}

void CTaiShanKlineShowView::OnUpdateViewYearkline(CCmdUI* pCmdUI) 
{
   pCmdUI->SetCheck(m_nKlineKind2 == 8?1:0);	
}

void CTaiShanKlineShowView::OnUpdateKlineMd(CCmdUI* pCmdUI) 
{
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
#ifdef OEM
	pCmdUI->Enable(FALSE);
	return;
//#else
  //  pCmdUI->Enable(TRUE);
#endif

	if(m_nKlineKind2 == 0)
	{
		pCmdUI->Enable(FALSE);	
	}
	else
	{
		pCmdUI->SetCheck(pKlineMode == NULL?0:1);	
	}
//		pCmdUI->Enable (TRUE);
	}
}

void CTaiShanKlineShowView::OnToolDrawline() 
{
  if(m_AutoChange == TRUE)
  {
	m_AutoChange = false;
    KillTimer(12);
  }
	CMainFrame* pMainWnd = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	if(IsBarShow  == TRUE && pMainWnd->m_drawLineToolBar->IsWindowVisible() != 0 )
	{
	  pMainWnd->ShowControlBar(pMainWnd->m_drawLineToolBar,FALSE, FALSE);
	  IsBarShow = !IsBarShow;
	  return;
	}
	else
	{
	  if(IsBarShow == false)
	   IsBarShow = !IsBarShow;
	  ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->Drawline();
	}
}

void CTaiShanKlineShowView::OnHldjLmb() 
{
	// TODO: Add your command handler code here
	if(m_nKlineKind2>0)
	{
	CString s0(m_infoInit.initIndex[0].nameIndex);
	if(s0!="HLDJ")
	{
		DrawZhibiao("HLDJ");
	}
	else
	{
		strcpy(m_infoInit.initIndex[0].nameIndex,pKlineDrawing->m_zhiBiao0);
		pKlineDrawing->m_bToCacl=TRUE;
		pKlineDrawing ->m_bInitFoot =false;//
		RedrawWindow();
	}
	}
	else
	{
		if(pMin1Drawing->m_nCnp == 1)
			pMin1Drawing->m_nCnp = -1;
		else
			pMin1Drawing->m_nCnp = 1;
		RedrawWindow();

	}
}

void CTaiShanKlineShowView::OnUpdateHldjLmb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
	if(m_nKlineKind2 == 0)
	{
		if(pMin1Drawing->m_nCnp == 1)
			pCmdUI->SetCheck(1);
		else
			pCmdUI->SetCheck(0);
	  return;
	}
	CString s0(m_infoInit.initIndex[0].nameIndex);
	if(s0!="HLDJ")
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
//		pCmdUI->Enable (TRUE);
	}
}

void CTaiShanKlineShowView::OnHedjLmb() 
{
	// TODO: Add your command handler code here
	CString s0(m_infoInit.initIndex[0].nameIndex);
	if(s0!="HEDJ")
	{
		DrawZhibiao("HEDJ");
	}
	else
	{
		strcpy(m_infoInit.initIndex[0].nameIndex,pKlineDrawing->m_zhiBiao0);
		pKlineDrawing->m_bToCacl=TRUE;
		pKlineDrawing ->m_bInitFoot =false;//
		RedrawWindow();
	}
	
}

void CTaiShanKlineShowView::OnUpdateHedjLmb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
	if(m_nKlineKind2 == 0)
	{
	  pCmdUI->Enable(FALSE);
	  return;
	}
	CString s0(m_infoInit.initIndex[0].nameIndex);
	if(s0!="HEDJ")
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
//		pCmdUI->Enable (TRUE);
	}
	
}
void CTaiShanKlineShowView::OnUpdateViewJszb(CCmdUI* pCmdUI) 
{
  if(m_nKlineKind2 == 0)
	  pCmdUI->Enable(false);
  else
  {
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
		pCmdUI->Enable (TRUE);
	}
  }
}

void CTaiShanKlineShowView::OnUpdateToolDrawline(CCmdUI* pCmdUI) 
{
   if(m_nKlineKind2 == 0)	
	   pCmdUI->Enable(false);
   else
   {
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
		pCmdUI->Enable (TRUE);
	}

	if(IsBarShow)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
   }
}

void CTaiShanKlineShowView::OnViewSimple() 
{
  	pKlineDrawing->m_axisType=0;
	RedrawWindow();	
}

void CTaiShanKlineShowView::OnViewPersent() 
{
  	pKlineDrawing->m_axisType=1;
	RedrawWindow();	
}

void CTaiShanKlineShowView::OnViewDszb() 
{
	pKlineDrawing->m_axisType=2;
	RedrawWindow();	
}


void CTaiShanKlineShowView::OnCopyFile() 
{
	CDiaCopy *m_dia;
	m_dia = new CDiaCopy;
	m_dia->Create(IDD_DIACOPY,NULL);
    m_dia->ShowWindow(SW_SHOW);
	m_dia->FileCopy("e:/day.dat","e:/wsstock2000/data/shanghai/day.dat");
    m_dia->ShowWindow(SW_HIDE);
	m_dia->OnClose() ;
}

void CTaiShanKlineShowView::OnLoopDisplay() 
{
	CTaiShanDoc *pDoc = GetDocument();
	if(m_AutoChange == false)
	{
		//SetTimer(12,60000,NULL);
		SetTimer(12,pDoc->m_systemdata.changepage*1000,NULL);
		m_AutoChange = TRUE;
	}
    else
    {
	  KillTimer(12);
      m_AutoChange = false;
	}
}

void CTaiShanKlineShowView::OnUpdateLoopDisplay(CCmdUI* pCmdUI) 
{
  	pCmdUI->SetCheck(m_AutoChange == 1?1:0);	
}

void CTaiShanKlineShowView::OnDelZhutuL() 
{
	// TODO: Add your command handler code here
	if(m_nKlineKind2==0)
	{
		pMin1Drawing->m_nKlineCurrent=0;
		RedrawWindow();
	}
	else
		pKlineDrawing->DeleteKlineAdded();

	
}

void CTaiShanKlineShowView::OnUpdateDelZhutuL(CCmdUI* pCmdUI) 
{
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
	if(m_nKlineKind2 == 0)
	{
	//	pCmdUI->Enable(false);
		pCmdUI->Enable(pMin1Drawing->m_nKlineCurrent == 0 ?false:TRUE);
	    return;
	}
    pCmdUI->Enable(pKlineDrawing->m_nKlineCurrent == 0 ?false:TRUE);
	}
}

void CTaiShanKlineShowView::OnQldjLmb() 
{
	// TODO: Add your command handler code here
	if(m_nKlineKind2>0)
	{
	CString s0(m_infoInit.initIndex[0].nameIndex);
	if(s0!="QLDJ")
	{
		DrawZhibiao("QLDJ");
	}
	else
	{
		strcpy(m_infoInit.initIndex[0].nameIndex,pKlineDrawing->m_zhiBiao0);
		pKlineDrawing->m_bToCacl=TRUE;
		pKlineDrawing ->m_bInitFoot =false;//
		RedrawWindow();
	}
	
	}
	else
	{
		if(pMin1Drawing->m_nCnp == 2)
			pMin1Drawing->m_nCnp = -1;
		else
			pMin1Drawing->m_nCnp = 2;
		RedrawWindow();

	}
}

void CTaiShanKlineShowView::OnUpdateQldjLmb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
	if(m_nKlineKind2 == 0)
	{
		if(pMin1Drawing->m_nCnp == 2)
			pCmdUI->SetCheck(1);
		else
			pCmdUI->SetCheck(0);
	  return;
	}
	CString s0(m_infoInit.initIndex[0].nameIndex);
	if(s0!="QLDJ")
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
//		pCmdUI->Enable (TRUE);
	}
}

void CTaiShanKlineShowView::OnQedjLmb() 
{
	// TODO: Add your command handler code here
	CString s0(m_infoInit.initIndex[0].nameIndex);
	if(s0!="QEDJ")
	{
		DrawZhibiao("QEDJ");
	}
	else
	{
		strcpy(m_infoInit.initIndex[0].nameIndex,pKlineDrawing->m_zhiBiao0);
		pKlineDrawing->m_bToCacl=TRUE;
		pKlineDrawing ->m_bInitFoot =false;//
		RedrawWindow();
	}
	
}

void CTaiShanKlineShowView::OnUpdateQedjLmb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command handler code here
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
	if(m_nKlineKind2 == 0)
	{
	  pCmdUI->Enable(FALSE);
	  return;
	}
	CString s0(m_infoInit.initIndex[0].nameIndex);
	if(s0!="QEDJ")
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
//		pCmdUI->Enable (TRUE);
	}
	
}

void CTaiShanKlineShowView::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnDropFiles(hDropInfo);
}
LRESULT CTaiShanKlineShowView::OnDropDrag(WPARAM wParam, LPARAM lParam)
{
	if(m_bInited == false)
		return 0L;
	CString s = (LPSTR)lParam;
	if(wParam==16)
	{
//		CString s = (LPSTR)lParam;
		CString s2 = s.Left(2);
		int stkKind = atoi(s2);

		s2=s.Right (s.GetLength()-2);
		s2.TrimRight (" ");

		if(s2.GetLength () == 4 || s2.GetLength () == 6)
		{
			if(::GetKeyState(VK_CONTROL)&0x8000)
			{
				AddNewStock(s2,stkKind);
			}
			else
			{
				this->m_stkKind = stkKind;
				ShowAll(s2);
			}
		}
		return 0L;
	}
	if(wParam==17)
	{
		DrawZhibiao(s,-2);
		return 0L;
	}
	return 1L;
}


BOOL CTaiShanKlineShowView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	return CView::OnDrop(pDataObject, dropEffect, point);
}

DROPEFFECT CTaiShanKlineShowView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	return CView::OnDragEnter(pDataObject, dwKeyState, point);
}

DROPEFFECT CTaiShanKlineShowView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	return CView::OnDragOver(pDataObject, dwKeyState, point);
}

void CTaiShanKlineShowView::OnDragLeave() 
{
	CView::OnDragLeave();
}

void CTaiShanKlineShowView::AddNewStock(CString &symbol,int nStkKind)
{
	if(m_nKlineKind2 == HS_KLINE)
		return;

	if(m_nKlineKind2 == 0)
	{
		if(pMin1Drawing->m_nKlineCurrent>=5)
			return;
	}
	else
	{
		if(pKlineDrawing->m_nKlineCurrent>=6)
			return;
	}
	//////////////Get the stock symbol end ///////
    CTaiShanDoc* pDoc = (CTaiShanDoc*)GetDocument();
	CString m_CurStockSymbol;
	m_CurStockSymbol = m_sharesSymbol;
	int curStkKind = m_stkKind;

	//DRAG MODE
	pKlineDrawing->m_bNewStock = TRUE;
	CReportData* pdt = NULL;
	pDoc->m_sharesInformation.Lookup(symbol.GetBuffer (0),pdt,nStkKind);
	if(pdt==NULL)
		return;
	CString sNameAdd = pdt->name;

	if(m_nKlineKind2 == 0)
	{

		if(!IsIndexStock(symbol))
		{
		if(pdt->nowp <=0 ||pdt->ystc<=0
			||pdt->totv<=0||pdt->higp<=0)
			return;
		}
		pMin1Drawing->m_nameAdd[pMin1Drawing->m_nKlineCurrent] = sNameAdd ;
		pMin1Drawing->m_symbolAdd[pMin1Drawing->m_nKlineCurrent] = symbol;
		pMin1Drawing->m_stkKindAdd[pMin1Drawing->m_nKlineCurrent] = nStkKind;
		if(pMin1Drawing->m_nKlineCurrent<5)
			pMin1Drawing->m_nKlineCurrent++;
		pDoc->m_sharesInformation.Lookup(m_CurStockSymbol.GetBuffer (0),pMin1Drawing->m_pReportData,curStkKind);
		RedrawWindow();
		return;
	}
	//lmb 7.6
	if(pKlineDrawing->m_nKlineCurrent==0 && pKlineDrawing->m_axisType!=1)
	{
		int rtn=AfxMessageBox("使用百分比坐标吗？",MB_YESNO|MB_ICONQUESTION );
		if(rtn==IDYES)//确定
			pKlineDrawing->m_axisType=1;
	}

	pKlineDrawing->m_nameAdd[pKlineDrawing->m_nKlineCurrent] = sNameAdd ;
	pKlineDrawing->m_symbolAdd[pKlineDrawing->m_nKlineCurrent] = symbol;
	pKlineDrawing->m_stkKindAdd[pKlineDrawing->m_nKlineCurrent] = nStkKind;
	pKlineDrawing->AddKlineAdded(pKlineDrawing->m_symbolAdd[pKlineDrawing->m_nKlineCurrent],pKlineDrawing->m_stkKindAdd[pKlineDrawing->m_nKlineCurrent]);
	
	pDoc->m_sharesInformation.Lookup(m_CurStockSymbol.GetBuffer (0),pMin1Drawing->m_pReportData,curStkKind);
	RedrawWindow();
}

void CTaiShanKlineShowView::OnKlineHs() 
{
	// TODO: Add your command handler code here
	int i=ID_KLINE_HS;
	DoOnCommand((WPARAM) i, (LPARAM) 0);	
	
}

void CTaiShanKlineShowView::OnUpdateKlineHs(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_nKlineKind2 == HS_KLINE ?1:0);
	
}

void CTaiShanKlineShowView::OnKlineManyDay() 
{
	// TODO: Add your command handler code here
	int i=ID_KLINE_MANY_DAY;
	DoOnCommand((WPARAM) i, (LPARAM) 0);	
	
}

void CTaiShanKlineShowView::OnUpdateKlineManyDay(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//
  pCmdUI->SetCheck(m_nKlineKind2 == MANY_DAY_KLINE ?1:0);
}

void CTaiShanKlineShowView::OnKline1Minu() 
{
	// TODO: Add your command handler code here
	int i=ID_KLINE_1_MINU;
	DoOnCommand((WPARAM) i, (LPARAM) 0);	
}

void CTaiShanKlineShowView::OnUpdateKline1Minu(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
  pCmdUI->SetCheck(m_nKlineKind2 == MIN1_KLINE ?1:0);
	
}

BOOL CTaiShanKlineShowView::DoOnCommand(WPARAM wParam, LPARAM lParam)
{
	bool bRet = false;
	bTrackCurcorBgn=false;

	int i=LOWORD(wParam);
	int nCode=HIWORD(wParam);
//	if(nCode!=0||bRet!=FALSE)	
	CTaiShanDoc* pDoc = GetDocument();
	if(nCode!=0)	//Modified by ltr
		return bRet;
	
	//if(i==ID_ADD_TO_SELF)
	//{
	//	CString m_zqdm = this->m_sharesSymbol;
	//	pDoc->m_pReportView ->AddChoose (m_zqdm.GetBuffer (0),m_stkKind);
	//	return TRUE;
	//}
	CStringArray StockTypeNameArray;
	pDoc->m_ManagerBlockData.GetChooseStockTypeNames(StockTypeNameArray);
	int l_nCount=StockTypeNameArray.GetSize();
	if(i >= 8900 && i <= 8900 + l_nCount)
	{
		//CString m_zqdm = this->m_sharesSymbol;
		//pDoc->m_pReportView ->AddChoose (m_zqdm.GetBuffer (0),m_stkKind);
		pDoc->m_pReportView->OnAddChoose(i - 8900);
		return TRUE;
	}
	//-	是否是常用指标菜单	
	if(i>=IDM_Index__OFTEN_1&&i<IDM_Index__OFTEN_1+600)
	{
		//得到常用指标数组
		CString zhiBiaoOften;
		CFormularContent * jishu;
		int j=0;
		for(int k=0;k<GetDocument()->m_formuar_index .GetSize ();k++)
		{
			jishu=GetDocument()->m_formuar_index.GetAt(k );
			bool bl = false;
			int nBegin = 0;

			if(i<IDM_Index__OFTEN_1+200)
			{
				if(jishu->isOfen ==TRUE) bl = true;
			}
			else if(i<IDM_Index__OFTEN_1+400)
			{
				if(jishu->isMainFiguer ==TRUE) bl = true;
				nBegin = 200;
			}
			else
			{
				if(jishu->subKindIndex .Find ("特色指标" )>=0) bl = true;
				nBegin = 400;
			}
			if(bl  ==TRUE)
			{
				if(j==i-IDM_Index__OFTEN_1-nBegin)
				{
					//主图叠加
					if(jishu->isMainFiguer ==0)
					{
						if(m_hit-REGION_PICT1>0)
							strcpy(m_infoInit.initIndex[m_hit-REGION_PICT1].nameIndex,jishu->name );
						else
							return TRUE;
					}
					else if(jishu->isMainFiguer ==1)
					{
						if(strcmp(m_infoInit.initIndex[0].nameIndex,jishu->name) == 0)
							m_infoInit.initIndex[0].nameIndex[0] = '\0';
						else
							strcpy(m_infoInit.initIndex[0].nameIndex,jishu->name);
					}
					else
						return TRUE;
					pKlineDrawing->m_bToCacl =TRUE;
					pKlineDrawing ->m_bInitFoot =false;//m_bInitFoot
					RedrawWindow();
					break;
				}
				j++;
			}
		}

	}
	//--		1分钟走势快捷菜单
	if(i == ID_HIST_OPEN_POP )
	{
		CString* sp = NULL;
		CString s;
		//CTime tm = 
		
		if(m_nKlineKind2>0)
		{
			int foot=pKlineDrawing->TransferX(m_pointMove.x);
			foot+=pKlineDrawing->m_footBegin;
			if(foot>pKlineDrawing->m_footEnd) foot = pKlineDrawing->m_footEnd;
			CTime tm = CTime(m_pkline[foot].day);
			s = tm.Format ("%Y%m%d");
			sp= &s;
		}

		HistOpen(sp);
	}
	if(m_nKlineKind2==0)
	{
		if(i == IDM_POP_KLINE_ADDED)
		{
			pMin1Drawing->m_nKlineCurrent=0;
			RedrawWindow();
			return TRUE;
		}
		if(i==ID_SHIDUAN_TJ)
		{
			CTaiKlineDialogShiDuanTJ* tj = 0;
			if(m_dlgFenshiTj == 0) 
			{
				m_dlgFenshiTj = new CTaiKlineDialogShiDuanTJ(this,1);
				tj = m_dlgFenshiTj;
				//第二个参数为0:表示在对话框中的日期时间控件显示格式为日期，1为时间

				if(pKlineDrawing->TongJi( tj)==-1)
				{
					delete m_dlgFenshiTj;
					m_dlgFenshiTj = 0;
					pMin1Drawing->m_bInvertRect = false;
					return TRUE;//统计数据
				}
				tj->Create (IDD_DIALOG_SHIDUAN_TJ,this);
			}
			else
			{
				tj = m_dlgFenshiTj;
				if(pKlineDrawing->TongJi( tj)==-1)
					return TRUE;//统计数据
			}
			tj->ShowWindow (SW_SHOW);
			RedrawWindow();
			return TRUE;
		}


		if(i>=ID_L_FENSHI&&i<=ID_L_TENGLUO)
		{
			if(m_hit>=REGION_PICT1)
			{
				CString s=m_sharesSymbol;
				if(IsIndexStock(s))
				{
					m_infoInit.flag_dapan[m_hit-REGION_PICT1]=i-ID_L_FENSHI;
				}
				else
				{
					m_infoInit.flag[m_hit-REGION_PICT1]=i-ID_L_FENSHI;
				}

				RedrawWindow();
			}
			return TRUE;
		}
	}


	CRect r;
	//int k;
	switch(i)
	{//
	case ID_EXTEND_KLINE:
		ShowKlineBig();
		bExtend=TRUE;
		return TRUE;
	case ID_DELETE_LINE:
		pDrawLine ->DeleteLine ();
		RedrawWindow();
		return TRUE;
	case ID_SHIDUAN_TJ:
		{
			CTaiKlineDialogShiDuanTJ* tj = 0;
			if(m_dlgKlineTj == 0) 
			{
				m_dlgKlineTj = new CTaiKlineDialogShiDuanTJ(this);
				tj = m_dlgKlineTj;
				//第二个参数为0:表示在对话框中的日期时间控件显示格式为日期，1为时间

				if(pKlineDrawing->TongJi( tj)==-1)
				{
					delete m_dlgKlineTj;
					m_dlgKlineTj = 0;
					pKlineDrawing->m_bInvertRect = false;
					break;//统计数据
				}
				tj->Create (IDD_DIALOG_SHIDUAN_TJ,this);
			}
			else
			{
				tj = m_dlgKlineTj;
				if(pKlineDrawing->TongJi( tj)==-1)
					break;//统计数据
			}
			tj->ShowWindow (SW_SHOW);
		}
		RedrawWindow();
		return TRUE;
	case IDM_TONGJI_Index_:
		{
			if(m_dlgZhibiao != 0)
			{
				m_dlgZhibiao->DestroyWindow();
				m_dlgZhibiao = 0;
			}
			CTaiKlineDlgZhibiaoTongji* tj = new CTaiKlineDlgZhibiaoTongji(this);
			m_dlgZhibiao = tj;
			//第二个参数为0:表示在对话框中的日期时间控件显示格式为日期，1为时间
			tj->Create (IDD_DIALOG_tongji_zb,this);
			tj->ShowWindow (SW_SHOW);
			RedrawWindow();
			return TRUE;
		}
	case ID_RESTORE_KLINE:
		ShowKlineBig(false);
		bExtend=false;
		return TRUE;
	case ID_CHANGE_DATA://
		if(TRUE)
		{
			CTaiKlineDialogChangKLine changeK(this);
			int foot=m_rectHitPos.left*pKlineDrawing->m_klinNumDefault /m_rtKlineFiguer.rightX;
			foot=pKlineDrawing->m_footBegin+foot;
			if(foot>pKlineDrawing->m_footEnd)
				break;
			changeK.m_foot=foot;
			changeK.DoModal();
			RedrawWindow();
		}
		return TRUE;
	case ID_GENERAL_XY://
	case ID_BFB_XY://
	case ID_DUISHU_XY://
		if(i==ID_GENERAL_XY)
			pKlineDrawing->m_axisType=0;
		else if(i==ID_BFB_XY)
			pKlineDrawing->m_axisType=1;
		else if(i==ID_DUISHU_XY)
			pKlineDrawing->m_axisType=2;
		RedrawWindow();
		return TRUE;

	case ID_MENU_1FS://
		OnMenu1fs();
		return TRUE;
	case ID_DAY_KLINE://
	case ID_KLINE_5://
	case ID_KLINE_15://
	case ID_KLINE_30://
	case ID_KLINE_60://
	case ID_KLINE_ZHOU2://
	case ID_KLINE_YUE2://
	case ID_KLINE_NIAN://
	case ID_KLINE_MANY_DAY://
	case ID_KLINE_HS://
	case ID_KLINE_1_MINU://
		m_nKlineKind2=5;
		if(i==ID_KLINE_5)
			m_nKlineKind2=1;
		if(i==ID_KLINE_15)
			m_nKlineKind2=2;
		if(i==ID_KLINE_30)
			m_nKlineKind2=3;
		if(i==ID_KLINE_60)
			m_nKlineKind2=4;
		if(i==ID_KLINE_ZHOU2)
			m_nKlineKind2=6;
		if(i==ID_KLINE_YUE2)
			m_nKlineKind2=7;
		if(i==ID_KLINE_NIAN)
			m_nKlineKind2=8;//
		if(i==ID_KLINE_MANY_DAY)
			m_nKlineKind2=MANY_DAY_KLINE;
		if(i == ID_KLINE_HS)
			m_nKlineKind2=HS_KLINE;
		if(i == ID_KLINE_1_MINU)
			m_nKlineKind2=MIN1_KLINE;

		GetClientRect(r);
		OnSizeMy(r.right-r.left,r.bottom-r.top);

		if(this->m_nCountKline > 82 && pKlineDrawing->m_klinNumDefault<82)
			pKlineDrawing->m_klinNumDefault = 82;
		ShowAll(this->m_sharesSymbol,false,true);
		if(m_isShowCross==1)
			m_dlgShowCross->RedrawWindow();
		return TRUE;
	case IDM_POP_TJXG_SHOW:
		OnTjxgMmTestShow() ;
//		if(m_tjxgShow!=NULL)
		{
/*			if(m_tjxgShow->chooseresult.m_detail.GetSize()>0)
			{
				for(int j=0;j<m_tjxgShow->chooseresult.m_detail.GetSize();j++)
				{
					SCREEN_DETAIL_L * detail;
					detail=m_tjxgShow->chooseresult.m_detail.GetAt(j);
					delete detail;
				}
				m_tjxgShow->chooseresult.m_detail.RemoveAll();
			}
			m_tjxgShow->DestroyWindow();
			delete m_tjxgShow;
			m_tjxgShow=NULL;
			m_tjxgshow=1;*/
		}
		RedrawWindow();
		return TRUE;
	case IDM_POP_KLINE_ADDED:
		pKlineDrawing->DeleteKlineAdded();
		return TRUE;
	case IDM_POP_CHANGE_PARAM:
		if(m_pChangeIndexParam==NULL)
		{
			m_pChangeIndexParam=new CTaiKlineDlgChangeIndexParam(this);
			CPoint pt;
			pt.x=m_pointMove.x;
			pt.y=m_pointMove.y;
//			ClientToScreen(&pt);
			m_pChangeIndexParam->m_pt.x=pt.x;
			m_pChangeIndexParam->m_pt.y=pt.y;
			m_pChangeIndexParam->m_nHit=m_hit-REGION_PICT1;
			m_pChangeIndexParam->Create(IDD_DIALOG_INDEX_CHANGE_PARA,this);
//			m_pChangeIndexParam->SetWindowPos(this,pt.x,pt.y,160 *2-60,100*2-20,SWP_NOREDRAW);
			m_pChangeIndexParam->ShowWindow(SW_SHOW);
		}
		return TRUE;
	case IDM_POP_DRAG_MODE:
		pKlineDrawing->m_kLine_mode ++;
		pKlineDrawing->m_kLine_mode %= 2;
		if(pKlineDrawing->m_kLine_mode == 1)
		{
			CTaiKlineSetMaxMin setMM(this);
			if(setMM.DoModal ()!=IDOK)
			{
				pKlineDrawing->m_kLine_mode = 0;
				return TRUE;
			}
		}
		RedrawWindow();
		return TRUE;
	case IDM_POP_FINANCE://
	case IDM_POP_FINANCE+1://
	case IDM_POP_FINANCE+2://
	case IDM_POP_FINANCE+3://
	case IDM_POP_FINANCE+4://
	case IDM_POP_FINANCE+5://
	case IDM_POP_FINANCE+6://
		if(m_tabNum>3)
		{
			m_infoInit.nIDRightSheet[m_tabNum-4]=i-IDM_POP_FINANCE;
			RedrawWindow();
		}
		return TRUE;
		//ID_MODIFY_JISHU_POP//
	case ID_MODIFY_JISHU_POP://
		{
			CFormularContent * pJishuCurrent = NULL;
			int nInd= m_hit-REGION_PICT1;
			if(nInd<0 || nInd>4)
				nInd = 0;
			CString name(m_infoInit.initIndex[nInd].nameIndex);
			pJishuCurrent=(CFormularContent*)GetDocument()->LookUpArray (GetDocument()->m_formuar_index,name); 
			for(int i=0;i<GetDocument()->m_formuar_index.GetSize();i++)
			{
				if(pJishuCurrent==GetDocument()->m_formuar_index.GetAt(i))
				{
					if(pJishuCurrent!=NULL)
						CTaiKlineTreeCtl::ShowEditIndicatorDlg(pJishuCurrent,i,0,&(pJishuCurrent->subKindIndex),this->m_nKlineKind2);

					CTaiShanDoc*   pDoc=CMainFrame::m_pDoc ;
					CFormularContent::ReadWriteIndex(pDoc, 0, false);
					ShowAll(this->m_sharesSymbol,false);
					break;
				}
			}
		}
		return TRUE;
	case ID_JISHU_EXPLAIN://ID_COPY_DATA_EXECEL
		{
			int nInd= m_hit-REGION_PICT1;
			if(nInd<0 || nInd>4)
				nInd = 0;
			CString name(m_infoInit.initIndex[nInd].nameIndex);
			CFormularContent * pJishuCurrent = NULL;
			pJishuCurrent=(CFormularContent*)GetDocument()->LookUpArray (GetDocument()->m_formuar_index,name); 
			if(pJishuCurrent!=NULL)
			{
				CDialogLOGGSSM mdiagssm;
				mdiagssm.m_edit=pJishuCurrent->help ;
				mdiagssm.DoModal();
			}
		}
		return TRUE;
	case ID_COPY_DATA_EXECEL://ID_PAGE_GO_BACK
		{
			int nInd= m_hit-REGION_PICT1;
			if(nInd<0 || nInd>4)
				nInd = 0;
			CTaiKlineExportExecel outData(this);
			outData.m_nFiguer = nInd;
			outData.DoModal ();
		}
		return TRUE;
	case ID_PAGE_GO_BACK://
		GetPageData(true);
		return TRUE;
	case ID_PAGE_GO_AHEAD://
		GetPageData(false);
		return TRUE;
	case ID_DRAWLINE_COLOR://
		{
			COLORREF  rgb;
			int  nLineType,   nLineThick;
			bool  bAlarm;
			if(this->pDrawLine ->GetCurrentLineProp(rgb, nLineType, nLineThick, bAlarm))
			{
			CColorDialog colordia(rgb,CC_RGBINIT,this);
			if(colordia.DoModal())
			{
				rgb=colordia.GetColor();
				this->pDrawLine ->SetCurrentLineProp(rgb);
				RedrawWindow();
			}
			}
		}
		return TRUE;
	case ID_DRAWLINE_SOLIDLINE://
	case ID_DRAWLINE_VIRTUALLINE://
	case ID_DRAWLINE_DOTLINE://
		this->pDrawLine ->SetCurrentLineProp(0, i-ID_DRAWLINE_SOLIDLINE, 0,0, 1);
		RedrawWindow();
		return TRUE;
	case ID_DRAWLINE_LINEWID1://
	case ID_DRAAWLINE_LINEWID2://
	case ID_DRAAWLINE_LINEWID3://
		this->pDrawLine ->SetCurrentLineProp(0,0 , i-ID_DRAWLINE_LINEWID1,0, 2);
		RedrawWindow();
		return TRUE;
	case ID_TREND_LINE_ALARM://
		{
			COLORREF  rgb;
			int  nLineType,   nLineThick;
			bool  bAlarm;
			if(this->pDrawLine ->GetCurrentLineProp(rgb, nLineType, nLineThick, bAlarm))
			{
				if(bAlarm == true)// return TRUE;
					this->pDrawLine ->SetCurrentLineProp(0, 0, 0,0, 3);
				else
					this->pDrawLine ->SetCurrentLineProp(0, 0, 0,1, 3);
				RedrawWindow();//NOW DRAW ALERT ICON
			}
		}
		return TRUE;
	case ID_PARAM_YH://
		{
			//lmb11
#ifndef OEM
			YHParam();
			pKlineDrawing->m_bToCacl = true;
			RedrawWindow();
#endif
		}
		return TRUE;
	case ID_DELETE_ADDED_INDICATOR://
		{
			pKlineDrawing->m_sIndicatorAdded[m_nOldnFgr] = "";
			pKlineDrawing->m_bToCacl =TRUE;
			pKlineDrawing ->m_bInitFoot =false;//
			RedrawWindow();
		}
		return TRUE;

	}
	return bRet;


}

int CTaiShanKlineShowView::ReadKLine(CString fName,int stkKind,Kline*& pKline,int nRead)
{
//	return 0;

	if( m_nKlineKind2 >= HS_KLINE)
		return 0 ;
//	CString sOut;
//	int stkKind = SeparateSymbol(fName,sOut);

	CTaiShanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_sharesSymbol = fName;
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();

	pDrawLine->ReadLineself();
	if( m_nKlineKind2>=1&& m_nKlineKind2<5)
		return ReadKline5Min( fName,stkKind, pKline,nRead);
	pKlineDrawing->ReadPower(fName,stkKind);

	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(fName,stkKind,true);

	int rtnK = pFile->ReadKLine (fName,pKline,nRead,1);

	//is the last kline useful?
	if(GetDocument()->m_bInitDone==TRUE && GetDocument()->m_bCloseWorkDone==false)
	{
		CReportData* pData ;
		GetDocument()->m_sharesInformation .Lookup(fName.GetBuffer (0),pData,stkKind);
		if(pData!=NULL)
		if(pData->nowp>0 && rtnK>0)
		{
			CTime tm = CTime::GetCurrentTime();
			CTime tm2(pKline[rtnK-1].day);
			CString s = tm.Format ("%Y%m%d");
			CString s2 = tm2.Format ("%Y%m%d");
			if(s == s2)
			{
				rtnK --;
			}
		}
	}

	 time_t mt = 0;
	 if(rtnK>0) mt= pKline[rtnK-1].day;
	((CMainFrame*)AfxGetMainWnd())->m_StockDriverDll.QueryKData(fName, CSharesCompute::GetMarketKind(stkKind),mt);
	return rtnK;
}
///////////////////////////////////////////////////////////////////
//      函 数 名：RemoveHs(int flag)
//      功能说明：去掉成交明细链表的数据
//      入口参数：flag为标识字
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiShanKlineShowView::RemoveHs(int flag)
{
	if(flag==0)
	{
		for(;m_hs.IsEmpty()==0;)
		{
			delete m_hs.RemoveHead();
		}
		m_hs.RemoveAll();
	}
	else
	{
		FENJIA* phs;
		for(;m_fenjia.IsEmpty()==0;)
		{
			phs=m_fenjia.RemoveHead();
			delete phs;
		}
		m_fenjia.RemoveAll();
	}
}

int CTaiShanKlineShowView::FindTimeK(int keyVlu)
{
	bool bDay =false;
	if( m_nKlineKind2>=5 && m_nKlineKind2<=8 ||GetDocument()->m_propertyInitiate.daysOfManyKline>0&& m_nKlineKind2==MANY_DAY_KLINE)
		bDay = true;
	int rtn=CTaiKlineFileKLine::LookTwoPath((time_t&)keyVlu ,m_pkline,m_nCountKline, m_bAddKline,bDay);
//	if(rtn==0&& m_bAddKline == true)
//		rtn = -1;
	if(rtn==-1&& m_bAddKline == true)
		rtn = m_nCountKline;
	return rtn;
}

float CTaiShanKlineShowView::CaclMid(int i)
{
	int j=GetDocument()->m_systemdata.middleprice;
	if(j<0||j>4)
		j=0;
	float rtn;
	switch(j)
	{
	case(0):
		rtn=(m_pkline[i].high+m_pkline[i].low+2*m_pkline[i].close)/4;
		break;
	case(1):
		rtn=(m_pkline[i].high+m_pkline[i].low+m_pkline[i].close)/3;
		break;
	case(2):
		rtn=(m_pkline[i].high+m_pkline[i].low)/2;
		break;
	case(3):
		rtn=(m_pkline[i].high+m_pkline[i].low+m_pkline[i].open+m_pkline[i].close)/4;
		break;
	case(4):
		rtn=m_pkline[i].close;
		break;
	}
	return rtn;
}
int CTaiShanKlineShowView::ReadKline5Min(CString fName,int stkKind, Kline *&pKline,int nRead)//5.31
{
	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer(fName,stkKind,false);
	int rtnK = pFile->ReadKLine (fName,pKline,nRead,48);

	return rtnK;
}
bool CTaiShanKlineShowView::CreateSplitMy(int bVert,int * pInt)
{
//	try
	{
		if(bVert<2)
		{
			CWnd* pWnd = this->GetParent();
			int nKind = 0;
			if(pWnd->IsKindOf (RUNTIME_CLASS(CChildFrame)))
				nKind = 1;
			if(pWnd->IsKindOf (RUNTIME_CLASS(CTaiKlineSplitWnd)))
				nKind = 2;
			if(nKind == 0)
				return false;

			CTaiKlineSplitWnd * wndSplitter=new CTaiKlineSplitWnd;
   			CView* pOldActiveView = this;
			CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
			CChildFrame* pFrameWnd=(CChildFrame*)((CMainFrame*)pMainWnd)->MDIGetActive();


			// TODO: Add your command handler code here
			CRect rc;
			GetClientRect(&rc);
			//CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);
			if(bVert == 1)
			{
				if(nKind == 1)
				{
					if (!wndSplitter->CreateStatic(pWnd, 1, 2))
					{
						TRACE0("Failed to CreateStaticSplitter\n");
						delete wndSplitter;
						return false;
					}

				// add the first splitter pane - the default view in column 0
					if (!wndSplitter->CreateView(0, 0,
						RUNTIME_CLASS(CTaiShanKlineShowView), CSize(rc.Width() /2 , rc.Height()), pFrameWnd->m_pContext))
					{
						TRACE0("Failed to create first pane\n");
						delete wndSplitter;
						return false;
					}
					if (!wndSplitter->CreateView(0, 1,
						RUNTIME_CLASS(CTaiShanKlineShowView), CSize(rc.Width() /2 , rc.Height()), pFrameWnd->m_pContext))
					{
						TRACE0("Failed to create first pane\n");
						delete wndSplitter;
						return false;
					}
				}
				else
				{
					int row,col;
					CWnd* pW = ((CTaiKlineSplitWnd*)pWnd)-> GetActivePane(&row, &col);
					if(pW==NULL ||row<0||row>100||col<0||col>100)
					{
						delete wndSplitter;
						return false;
					}

					if (!wndSplitter->CreateStatic((CTaiKlineSplitWnd*)pWnd, 1, 2,
						WS_CHILD | WS_VISIBLE | WS_BORDER,  // style, WS_BORDER is needed
					((CTaiKlineSplitWnd*)pWnd)->IdFromRowCol(row, col)))//OR 0,0
					{
						TRACE0("Failed to CreateStaticSplitter\n");
						delete wndSplitter;
						return false;
					}

					if (!wndSplitter->CreateView(0, 0,
						RUNTIME_CLASS(CTaiShanKlineShowView), CSize(rc.Width() /2 , rc.Height()),
						pFrameWnd->m_pContext ))
					{
						TRACE0("Failed to create first pane\n");
						delete wndSplitter;
						return false;
					}
					if (!wndSplitter->CreateView(0, 1,
						RUNTIME_CLASS(CTaiShanKlineShowView), CSize(rc.Width() /2 , rc.Height()),
						pFrameWnd->m_pContext ))
					{
						TRACE0("Failed to create first pane\n");
						delete wndSplitter;
						return false;
					}
				}
			}
			else
			{
				if(nKind == 1)
				{
					if (!wndSplitter->CreateStatic(pWnd, 2, 1))
					{
						TRACE0("Failed to CreateStaticSplitter\n");
						delete wndSplitter;
						return false;
					}

				// add the first splitter pane - the default view in column 0
					if (!wndSplitter->CreateView(0, 0,
						RUNTIME_CLASS(CTaiShanKlineShowView), CSize(rc.Width() /2 , rc.Height()), pFrameWnd->m_pContext))
					{
						TRACE0("Failed to create first pane\n");
						delete wndSplitter;
						return false;
					}
					if (!wndSplitter->CreateView(1, 0,
						RUNTIME_CLASS(CTaiShanKlineShowView), CSize(rc.Width() /2 , rc.Height()), pFrameWnd->m_pContext))
					{
						TRACE0("Failed to create first pane\n");
						delete wndSplitter;
						return false;
					}
				}
				else
				{
					int row,col;
					CWnd* pW = ((CTaiKlineSplitWnd*)pWnd)-> GetActivePane(&row, &col);
					if(pW==NULL ||row<0||row>100||col<0||col>100)
					{
						delete wndSplitter;
						return false;
					}

					if (!wndSplitter->CreateStatic((CTaiKlineSplitWnd*)pWnd, 2, 1,
						WS_CHILD | WS_VISIBLE | WS_BORDER,  // style, WS_BORDER is needed
					((CTaiKlineSplitWnd*)pWnd)->IdFromRowCol(row, col)))//OR 0,0
					{
						TRACE0("Failed to CreateStaticSplitter\n");
						delete wndSplitter;
						return false;
					}

					if (!wndSplitter->CreateView(0, 0,
						RUNTIME_CLASS(CTaiShanKlineShowView), CSize(rc.Width()  , rc.Height()/2),
						pFrameWnd->m_pContext ))
					{
						TRACE0("Failed to create first pane\n");
						delete wndSplitter;
						return false;
					}
					if (!wndSplitter->CreateView(1, 0,
						RUNTIME_CLASS(CTaiShanKlineShowView), CSize(rc.Width()  , rc.Height()/2),
						pFrameWnd->m_pContext ))
					{
						TRACE0("Failed to create first pane\n");
						delete wndSplitter;
						return false;
					}
				}
			}
			wndSplitter->RecalcLayout();
			CRect r;
			this->GetClientRect (r);
			ClientToScreen(r);
			pWnd->ScreenToClient (r);

			r.left-=2;
			r.top-=2;
			r.right+=2;
			r.bottom+=2;
			wndSplitter->MoveWindow (r);
		//	wndSplitter->ShowWindow(SW_SHOW);
			ASSERT(!wndSplitter->IsTracking());

			bTrackCurcorBgn=true;

			wndSplitter->DoKeyboardSplit();
			wndSplitter->GetPane (0,0)->SetFocus ();

			bTrackCurcorBgn=false;
			pOldActiveView->DestroyWindow ();

		//	OnTimeAstep4() ;
			return true;

		}
		else
		{
			CWnd* pWnd = this->GetParent();

			CTaiKlineSplitWnd * wndSplitter=new CTaiKlineSplitWnd;
			CView* pOldActiveView = this;
			CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
			CChildFrame* pFrameWnd=(CChildFrame*)((CMainFrame*)pMainWnd)->MDIGetActive();


			// TODO: Add your command handler code here
			CRect rc;
			GetClientRect(&rc);
			//CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);
			if (!wndSplitter->CreateStatic(pWnd, 2, 2))
			{
				TRACE0("Failed to CreateStaticSplitter\n");
				delete wndSplitter;
				return false;
			}

		// add the first splitter pane - the default view in column 0
			if (!wndSplitter->CreateView(0, 0,
				RUNTIME_CLASS(CTaiShanKlineShowView), CSize(rc.Width() /2 , rc.Height()/2), pFrameWnd->m_pContext))
			{
				TRACE0("Failed to create first pane\n");
				delete wndSplitter;
				return false;
			}
			if (!wndSplitter->CreateView(0, 1,
				RUNTIME_CLASS(CTaiShanKlineShowView), CSize(rc.Width() /2 , rc.Height()/2), pFrameWnd->m_pContext))
			{
				TRACE0("Failed to create first pane\n");
				delete wndSplitter;
				return false;
			}

			if (!wndSplitter->CreateView(1, 0,
				RUNTIME_CLASS(CTaiShanKlineShowView), CSize(rc.Width() /2 , rc.Height()/2), pFrameWnd->m_pContext))
			{
				TRACE0("Failed to create first pane\n");
				delete wndSplitter;
				return false;
			}

			if (!wndSplitter->CreateView(1, 1,
				RUNTIME_CLASS(CTaiShanKlineShowView), CSize(rc.Width() /2 , rc.Height()/2), pFrameWnd->m_pContext))
			{
				TRACE0("Failed to create first pane\n");
				delete wndSplitter;
				return false;
			}
			wndSplitter->RecalcLayout();
			rc.bottom +=4;
			rc.right +=4;
			wndSplitter->MoveWindow (rc);
			pFrameWnd->RecalcLayout();
			pOldActiveView->DestroyWindow ();
			if(pInt!=NULL)
			{
				CTaiShanKlineShowView * pView = (CTaiShanKlineShowView *)wndSplitter->GetPane (0,0);
				pView->m_nKlineKind2 = pInt[0];
				pView->ShowAll(pView->m_sharesSymbol );
				pView = (CTaiShanKlineShowView *)wndSplitter->GetPane (0,1);
				pView->m_nKlineKind2 = pInt[1];
				pView->ShowAll(pView->m_sharesSymbol );
				pView = (CTaiShanKlineShowView *)wndSplitter->GetPane (1,0);
				pView->m_nKlineKind2 = pInt[2];
				pView->ShowAll(pView->m_sharesSymbol );
				pView = (CTaiShanKlineShowView *)wndSplitter->GetPane (1,1);
				pView->m_nKlineKind2 = pInt[3];
				pView->ShowAll(pView->m_sharesSymbol );
			}
			wndSplitter->DoKeyboardSplit();
			wndSplitter->GetPane (0,0)->SetFocus ();
			return true;
		}
	}
//	catch(...)
	{
	}

}

void CTaiShanKlineShowView::OnLSplitV() 
{
	// TODO: Add your command handler code here
	__try
	{
	CreateSplitMy(1);	
	}
	__except(1)
	{
	}
}

void CTaiShanKlineShowView::OnUpdateLSplitV(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CTaiShanDoc* pDoc=GetDocument();
	if(pDoc->m_arKLineViews .GetSize()>=6)
		pCmdUI->Enable(FALSE);
	
}

void CTaiShanKlineShowView::OnLSplitH() 
{
	// TODO: Add your command handler code here
	__try
	{
	CreateSplitMy(0);	
	}
	__except(1)
	{
	}
	
}

void CTaiShanKlineShowView::OnLSplitDel() 
{
	CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
	CChildFrame* pFrameWnd=(CChildFrame*)((CMainFrame*)pMainWnd)->MDIGetActive();
	if(!pFrameWnd->GetActiveView ()->IsKindOf (RUNTIME_CLASS(CTaiShanKlineShowView)))
		return;
	if(pFrameWnd->IsKindOf (RUNTIME_CLASS(CChildFrame)))
		pFrameWnd = pFrameWnd;

	CTaiShanKlineShowView* pView = (CTaiShanKlineShowView*)pFrameWnd->GetActiveView ();
	CWnd* pWnd = pView->GetParent();
	int nKind = 0;
	if(pWnd->IsKindOf (RUNTIME_CLASS(CChildFrame)))
		nKind = 1;
	if(pWnd->IsKindOf (RUNTIME_CLASS(CTaiKlineSplitWnd)))
		nKind = 2;
	if(nKind == 0)
		return ;

	if(nKind == 1)
		pWnd->SendMessage (WM_CLOSE);
	if(nKind == 2)
	{
		CTaiKlineSplitWnd * pSplit = (CTaiKlineSplitWnd*)pWnd;

		//int row,col;
		CTaiKlineSplitWnd * pSplParent = (CTaiKlineSplitWnd*)pWnd->GetParent ();

		if(pWnd->GetParent ()->IsKindOf (RUNTIME_CLASS(CChildFrame)))//if CChildFrame
		{
			CString s = this->m_sharesSymbol;
			CMainFrame * pMainWnd =(CMainFrame*)( AfxGetApp()->m_pMainWnd);
			pMainWnd->SendMessage (WM_USER_DrawKlineWindow,(WPARAM)s.GetBuffer (0),this->m_stkKind*10+0);
			pFrameWnd->SendMessage (WM_CLOSE);
		}
		else//if CTaiKlineSplitWnd
		{
			int nRow = pSplParent->GetRowCount ();
			int nCol = pSplParent->GetColumnCount ();

			if(! pSplParent->IsChildPane( pWnd, &nRow, &nCol))
				return;

			int k = 0,j=0;

			CRect rc;
			pWnd->GetClientRect(&rc);
			CRect r;
			pWnd->GetClientRect (r);
			pWnd->ClientToScreen(r);
			pSplParent->ScreenToClient (r);
			CWnd* pPane = pSplParent->GetPane(nRow,nCol);
			pPane->DestroyWindow ();

			if (!pSplParent->CreateView(nRow, nCol,
				RUNTIME_CLASS(CTaiShanKlineShowView), CSize(rc.Width()  , rc.Height()/2),
				pFrameWnd->m_pContext ))
			{
				TRACE0("Failed to create first pane\n");
				return ;
			}


			k = nRow;
			j = nCol;

			r.left+=2;
			r.top+=2;
			r.right-=2;
			r.bottom-=2;
			pSplParent->GetPane(k,j)->MoveWindow (r);
			if(!(pSplParent->GetPane(k,j)->IsKindOf(RUNTIME_CLASS(CTaiShanKlineShowView))))
				return;
			CTaiShanKlineShowView* pView2 = (CTaiShanKlineShowView*)(pSplParent->GetPane(k,j));
			pView2->SetFocus ();
			pSplParent->SetActivePane( k,j, NULL );

			ViewSetFocus(pView2);
		}
	}
	
}

void CTaiShanKlineShowView::OnUpdateLSplitDel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CTaiShanKlineShowView::OnUpdateLSplitH(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CTaiShanDoc* pDoc=GetDocument();
	if(pDoc->m_arKLineViews .GetSize()>=6)
		pCmdUI->Enable(FALSE);
	
}

void CTaiShanKlineShowView::OnLSplit4() 
{
	// TODO: Add your command handler code here
	CTaiKlineFileKLine* pFile;
	CString s="";

	for(int k = 0;k<2;k++)
	{
		pFile = CTaiKlineFileKLine::m_fileDaySh;
		if(k==1) pFile = CTaiKlineFileKLine::m_fileDaySz;
		CMap<int ,int ,int,int> map;
		
		int nStock = pFile->GetStockNumber ();
		for(int i=0;i<nStock;i++)
		{
			int addr = 16+i*64;
			pFile->Seek(addr,CTaiKlineFileKLine::begin);
			KLINE_SMALLHEAD* pKlineSmallHead = (KLINE_SMALLHEAD*)pFile->GetFileCurrentPointer();
			CString symbol (pKlineSmallHead->StockSign);
			for(int j=0;j<26;j++)
			{
				if(pKlineSmallHead->symBlock [j] == 0xffff) break;
				int iData = -1;
				int iIn = pKlineSmallHead->symBlock [j] ;
				if(iIn == 2790)
					iIn = iIn;
				if(map.Lookup(iIn,iData))
				{
					CString s2;
					s2.Format ("=%d  ",iIn);
					s2=symbol+s2;
					s+=s2;
				}
				else
					map[iIn] = iIn;
			}
		}
	//	if(s.GetLength ()>0)
	//		AfxMessageBox(s);
	}
	if(s.GetLength ()>0)
	{
		CFile fl;
		if(fl.Open("blockTestReport.txt",CFile::modeCreate|CFile::modeWrite))
		{
			fl.Write(s.GetBuffer (s.GetLength ()),s.GetLength ());
			fl.Close();
		}
	}

	//
	int nKind[4] = {5,4,1,6};
	try
	{
		CreateSplitMy(2,nKind);
	}
//	__except(1)
	catch(...)
	{
	}
	
}

void CTaiShanKlineShowView::OnUpdateLSplit4(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CTaiShanDoc* pDoc=GetDocument();
	if(pDoc->m_arKLineViews .GetSize()>=3)
		pCmdUI->Enable(FALSE);
	
}

void CTaiShanKlineShowView::KlineReadTransferData()
{
	pKlineDrawing->TransferKlineTypeData(m_sharesSymbol,m_stkKind ,m_pkline,-1);
	if(pKlineDrawing->m_nKlineCurrent>0)
	{
		for(int j=0;j<pKlineDrawing->m_nKlineCurrent;j++)
		{
			pKlineDrawing->TransferKlineTypeData(pKlineDrawing->m_symbolAdd[j],pKlineDrawing->m_stkKindAdd[j] ,pKlineDrawing->m_pKlineAdd[j],j);
		}
	}

}

void CTaiShanKlineShowView::CopyScrollBar(CDC *pMemDC)
{
	if(m_infoFiguer==1)
	{
		//marenan +
		m_scrollBar->SetWindowPos(&wndTop,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
		bool bShow = false;
		if(m_tabNum==0||m_tabNum==1)
			bShow=true;
		if(m_tabNum>3)
			if(m_infoInit.nIDRightSheet [m_tabNum-4]==0)		bShow=true;;
		if(bShow==true)
		{
			CRect r,r2;
			GetClientRect(r);
			CClientDC dc(this);
			this->m_scrollBar->GetWindowRect(r2);
			//Marenan +
			this->m_scrollBar->Draw();  

			int wid = r2.right-r2.left;
			//::BitBlt( pMemDC->m_hDC,
			//	r.right-wid, RIGHTBOX_PERLINE*19+1,
			//	wid, r.bottom-(RIGHTBOX_PERLINE*19+1+gScrollBott),
			//	dc.m_hDC,
			//	r.right-wid, RIGHTBOX_PERLINE*19+1,
			//	SRCCOPY );//?
			int nTop;
			if (m_bInfoTabFill)
				nTop=1+ZOOMIMG;
			else
				nTop=RIGHTBOX_LINECOUNT*RIGHTBOX_PERLINE+1+ZOOMIMG;
			//			m_scrollBar->MoveWindow(r2.left,nTop,r2.right,nTop+r.Height(),TRUE);   
			::BitBlt( pMemDC->m_hDC,
				r.right-wid, nTop,
				wid, r.bottom-(nTop+gScrollBott)-2,
				dc.m_hDC,
				r.right-wid, nTop,
				SRCCOPY );//?
			//DrawZoom(pMemDC,m_rcZoom);

		}
	}

}

//cjg
void CTaiShanKlineShowView::DrawZoom(CDC* pDC,LPRECT lpRect)
{
	CBrush *pOldBrush;
	CBrush brush;
	brush.CreateSolidBrush(GetDocument()->m_colorArray[0]);
	pOldBrush=(CBrush*)pDC->SelectObject(&brush);
	CPen pen;
	if (!pen.CreatePen(PS_SOLID, 1,GetDocument()->m_colorArray[2]))
		return ;
	CPen* pOldPen = pDC->SelectObject(&pen);
	int nWidth=lpRect->right-lpRect->left;
	int nHeight=lpRect->bottom -lpRect->top;
	CRect rcCircle(lpRect);
	int nCircleD=(nWidth>nHeight?nHeight:nWidth);
	rcCircle.bottom =rcCircle.top +nCircleD;
	rcCircle.right =rcCircle.left +nCircleD;

	if (m_bInfoTabFill)
	{
		pDC->Ellipse(rcCircle);
		pDC->MoveTo(rcCircle.left,rcCircle.bottom-nCircleD/2);
		pDC->LineTo(rcCircle.right,rcCircle.bottom-nCircleD/2);    
	}
	else
	{
		pDC->Ellipse(rcCircle);
		pDC->MoveTo(rcCircle.left,rcCircle.bottom-nCircleD/2);
		pDC->LineTo(rcCircle.right,rcCircle.bottom-nCircleD/2);    
		pDC->MoveTo(rcCircle.left+nCircleD/2 ,rcCircle.top);
		pDC->LineTo(rcCircle.left+nCircleD/2 ,rcCircle.bottom);
	}
	pDC->SelectObject(pOldPen) ;
	pDC->SelectObject(pOldBrush); 
}

void CTaiShanKlineShowView::OnClassifyJishu() 
{
	// TODO: Add your command handler code here
/*	CTaiKlineDlgJishuClassify* dlg;
	dlg = new CTaiKlineDlgJishuClassify(this);
	dlg->Create (IDD_LMB_CLASSIFY_JISHU,this);
	dlg->ShowWindow (SW_SHOW);*/
	CTaiKlineDlgAddSelfKline dlg(this);
	dlg.DoModal();

}

void CTaiShanKlineShowView::SetPageData()
{
	PAGE_DATA_INFO info;

	info.bBaseInfo		    = m_bBaseInfo		;
	info.infoFiguer			= m_infoFiguer			;
	info.isShowShiZi		= m_isShowCross		;
	info.kLineType			= m_nKlineKind2		;	
	info.nStock				= m_nShares			;	
	info.nStockBegin		= m_nSharesBegin		;
	info.nTotalStock		= m_nTotalStock		;
	strcpy(info.symbol, m_sharesSymbol)			;	
	info.tabNum				= m_tabNum			;	
	info.m_bMultiFiguer		= m_bMultiFiguer ;
	info.m_stkKind			= m_stkKind ;

	if(m_nKlineKind2==0)
	{
		info.klineNumDefault	= pMin1Drawing->m_klinNumDefault	;
		info.footBegin			= pMin1Drawing->m_footBegin			;
		info.footEnd			= pMin1Drawing->m_footEnd				;
	}
	else
	{
		info.klineNumDefault	= pKlineDrawing->m_klinNumDefault	;
		info.footBegin			= pKlineDrawing->m_footBegin			;
		info.footEnd			= pKlineDrawing->m_footEnd				;
	}
	info.m_infoInit = m_infoInit;

	int nCount = m_pageInfoArray.GetSize();
	if(nCount>0 && m_nPageCurrent<nCount)
	{
		if(memcmp(&m_pageInfoArray[m_nPageCurrent],&info,sizeof(PAGE_DATA_INFO) ) == 0)
			return;
	}
	if(m_nPageCurrent >= nCount)
		m_nPageCurrent = nCount - 1;
	for(int k=m_nPageCurrent+1;k<m_pageInfoArray.GetSize();k++)
		m_pageInfoArray.RemoveAt(k);
	m_pageInfoArray.Add(info);
	while(m_pageInfoArray.GetSize() > NUM_PAGE_STORE )
		m_pageInfoArray.RemoveAt(0);
	m_nPageCurrent = m_pageInfoArray.GetSize()-1;
	//TRACE("set m_nPageCurrent = %d\r\n",m_nPageCurrent);
}

void CTaiShanKlineShowView::GetPageData(bool bMovePre)
{
	PAGE_DATA_INFO info;//m_nPageCurrent
	int n = m_pageInfoArray.GetSize();
	if(n<=0)
		return;
	if(bMovePre == true) 
	{
		if(m_nPageCurrent==0)
			return;
		m_nPageCurrent --;

	}
	else
	{
		if(m_nPageCurrent==0)
			return;
		if(m_nPageCurrent>=n-1 )
		{
			m_nPageCurrent = n-1;
			return;
		}
		if(m_nPageCurrent>=NUM_PAGE_STORE-1)
		{
			m_nPageCurrent = NUM_PAGE_STORE-1;
			return;
		}
		m_nPageCurrent ++;
	}

	TRACE("m_nPageCurrent = %d\r\n",m_nPageCurrent);

	info = m_pageInfoArray[m_nPageCurrent];

	//set data to view
	bool bSize  = false;
	CString symbol = m_sharesSymbol;
	int nKind = m_nKlineKind2;
	m_bBaseInfo		=info.bBaseInfo		     	;
	if(m_infoFiguer	!=info.infoFiguer)
		bSize = true;

	m_infoFiguer	=info.infoFiguer			 		;
	m_isShowCross	=info.isShowShiZi		 	;
	m_nKlineKind2		=info.kLineType			 	;	
	m_nShares		=info.nStock				 	;	
	m_nSharesBegin	=info.nStockBegin		 	;
	m_nTotalStock	=info.nTotalStock		 	;
	m_sharesSymbol		=info.symbol				 	;	
	ASSERT(m_sharesSymbol.GetLength()>3);
	m_tabNum		=info.tabNum				 	;	
	m_bMultiFiguer	=info.m_bMultiFiguer ;
	m_stkKind		=info.m_stkKind ;

	if(m_nKlineKind2==0)
	{
		pMin1Drawing->m_klinNumDefault		=	info.klineNumDefault ;
		pMin1Drawing->m_footBegin				=	info.footBegin		;			
		pMin1Drawing->m_footEnd				=	info.footEnd		 ;
	}
	else
	{
		pKlineDrawing->m_klinNumDefault		=	info.klineNumDefault ;
		pKlineDrawing->m_footBegin			=	info.footBegin		;			
		pKlineDrawing->m_footEnd				=	info.footEnd		 ;
	}

	if(memcmp(&m_infoInit,&(info.m_infoInit),sizeof(SUB_FIGUER_INIT_INFO))!=0)
	{
		pKlineDrawing->m_bToCacl = true;
		if(info.m_infoInit.nCountIndex !=m_infoInit.nCountIndex
			|| info.m_infoInit.nCountMin1 !=m_infoInit.nCountMin1)
			bSize = true;

		m_infoInit = info.m_infoInit ;
	}
	if(	bSize == true)
	{
		CRect r;
		GetClientRect(r);
		OnSizeMy(r.right -r.left , r.bottom -r.top );
	}


	//to redraw
	if(symbol != m_sharesSymbol)
	{
		pKlineDrawing->m_bNewStock = true;
	}
	pKlineDrawing->m_bSetPageData = false;
	if(nKind == m_nKlineKind2 && symbol == m_sharesSymbol)
		RedrawWindow();
	else
		ShowAll(m_sharesSymbol,false,false);
	pKlineDrawing->ValidDoKline(true);
	TRACE("SYMBOL = %s\r\n",symbol);
}

void CTaiShanKlineShowView::OnHistoryOpen() 
{
	// TODO: Add your command handler code here
#ifdef OEM
	return;
#endif
	HistOpen();
}

void CTaiShanKlineShowView::OnUpdateHistoryOpen(CCmdUI* pCmdUI) 
{
#ifdef OEM
	pCmdUI->Enable (FALSE);
#else
	m_pDlgDealHistory == NULL?pCmdUI->Enable (TRUE):pCmdUI->Enable (FALSE);
#endif
}
void CTaiShanKlineShowView::DrawFinace(CDC *pDC,int nBegin)
{
	CRect rt;
	GetCurrClientRect(rt);
	if(IsIndexStock(m_sharesSymbol))
	{
		CRect rt1;
		rt1.top=0;
		rt1.bottom = rt.bottom - 30;
		rt1.left = rt.right - m_rightLength+1;
		rt1.right = rt.right;
		ClearBack(pDC, rt1,pMin1Drawing->m_bHist);
		return;
	}
	//Marenan +
	int nShow,nTop;         //cjg...↓//↑
	if (m_bInfoTabFill)
	{
		nTop=0;
	}
	else
	{
		nTop=RIGHTBOX_LINECOUNT*RIGHTBOX_PERLINE;
	}
	//Marenan +

	if(nBegin<0) nBegin = 0;
	m_scrollBar->SetScrollPos(nBegin);
	if(rt.bottom<=19*RIGHTBOX_PERLINE+gScrollBott+2)
		return;

	BASEINFO* pBase = GetBaseinfoPointer();
	if(pBase==NULL)
		return;

	CFont ft;
	//Marenan +
	LOGFONT lgf=GetDocument()->m_fontstr[0];
	//Marenan -
	//LOGFONT lgf=GetDocument()->m_fontstr[1];
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);

	int modify=3;
	int lf=4;
	pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
	pDC->SetBkMode(TRANSPARENT);
	int left = rt.right-m_rightLength + 4;
	float gap = m_rightLength /6.0f;
	CString s ;

	COLORREF	clr[2];
	{
		int np = 20;
		int g = GetGValue(GetDocument()->m_colorArray [0]);
		if(g+np>255) g=g-np;
		else	g+=np;
		int r = GetRValue(GetDocument()->m_colorArray [0]);
		if(r+np>255) r-=np;
		else	r+=np;
		int b = GetBValue(GetDocument()->m_colorArray [0]);
		if(b+np>255) b-=np;
		else	b+=np;
		clr[0]=RGB(r,g,b);
	}
	clr[1]=GetDocument()->m_colorArray [0];
	
	float* pf = &(pBase->zgb);
	s.LoadString (IDS_ZGB);
	int high = pDC->GetOutputTextExtent(s).cy ;
	int high2 = pDC->GetOutputTextExtent("8").cy ;
	int highTot = (high+high2+2);
	
	//Marena -
	//int nShow = (rt.bottom - 19*RIGHTBOX_PERLINE-gScrollBott-1)/highTot;
	int NewBegin[]={0,6,20,29,22,5,7,8,9,10,11,12,13,14,15,16,17,18,19,21,23
	                 ,24,25,26,27,28,30,31,32,33};
	//Marena +
	if (m_bInfoTabFill)
	{
		nShow = (rt.bottom - rt.top -gScrollBott-3)/highTot;
		pDC->FillSolidRect (left+1,nTop,rt.right -left-1,1,clr[0]);
	}
	else
		nShow = (rt.bottom - RIGHTBOX_LINECOUNT*RIGHTBOX_PERLINE-gScrollBott-3)/highTot;
	//Marena +

	for(int i=0;i<nShow;i++)
	{
		if(i+nBegin>29)
			break;
		if(nBegin<3)
		{
			if(i+nBegin<2)
			{
				//Marenan +
				pDC->FillSolidRect (left+1,nTop+1+i*highTot,rt.right -left-1,highTot,clr[(i+1)%2]); //cjg
				//Marenan -
				//pDC->FillSolidRect (left+1,19*RIGHTBOX_PERLINE+1+i*highTot,rt.right -left-1,highTot,clr[(i+1)%2]);
			    s.LoadString (IDS_ZGB+NewBegin[i+nBegin]);
			    s+=":";
			    pDC->SetTextColor( GetDocument()->m_colorArray[FIGUER_WORD]);
			    pDC->TextOut(left+1,19*RIGHTBOX_PERLINE+1+i*highTot,s);
			    s.Format ("%.2f",(float)pf[NewBegin[i+nBegin]]);
			    pDC->SetTextColor( GetDocument()->m_colorArray[13]);
			    pDC->TextOut(left+1,19*RIGHTBOX_PERLINE+1+(i)*highTot+high+1,s);
			}
			else
			{
				if(i+nBegin==2)
				{
					pDC->FillSolidRect (left+1,19*RIGHTBOX_PERLINE+1+i*highTot,rt.right -left-1,highTot,clr[(i+1)%2]);
					//s.LoadString (IDS_ZGB+NewBegin[i+nBegin]);
					s="市赢率:";
					pDC->SetTextColor( GetDocument()->m_colorArray[FIGUER_WORD]);
					pDC->TextOut(left+1,19*RIGHTBOX_PERLINE+1+i*highTot,s);
					float Syll;
					if(pf[29]==0)
						Syll=0.0;
					else
					    Syll=pMin1Drawing->m_pReportData->ystc/(float)pf[29];
					if(Syll==0)
						s="";
					else
					    s.Format ("%.2f",Syll);
					pDC->SetTextColor( GetDocument()->m_colorArray[13]);
					pDC->TextOut(left+1,19*RIGHTBOX_PERLINE+1+(i)*highTot+high+1,s);
				    nShow--;
				}
				pDC->FillSolidRect (left+1,19*RIGHTBOX_PERLINE+1+(i+1)*highTot,rt.right -left-1,highTot,clr[(i+2)%2]);
			    s.LoadString (IDS_ZGB+NewBegin[i+nBegin]);
			    s+=":";
			    pDC->SetTextColor( GetDocument()->m_colorArray[FIGUER_WORD]);
			    pDC->TextOut(left+1,19*RIGHTBOX_PERLINE+1+(i+1)*highTot,s);
			    s.Format ("%.2f",(float)pf[NewBegin[i+nBegin]]);
			    pDC->SetTextColor( GetDocument()->m_colorArray[13]);
			    pDC->TextOut(left+1,19*RIGHTBOX_PERLINE+1+(i+1)*highTot+high+1,s);
			}
		}
		else
		{
			pDC->FillSolidRect (left+1,19*RIGHTBOX_PERLINE+1+i*highTot,rt.right -left-1,highTot,clr[(i+1)%2]);
			s.LoadString (IDS_ZGB+NewBegin[i+nBegin]);
			s+=":";
			pDC->SetTextColor( GetDocument()->m_colorArray[FIGUER_WORD]);
			pDC->TextOut(left+1,19*RIGHTBOX_PERLINE+1+i*highTot,s);
			s.Format ("%.2f",(float)pf[NewBegin[i+nBegin]]);
			pDC->SetTextColor( GetDocument()->m_colorArray[13]);
			pDC->TextOut(left+1,19*RIGHTBOX_PERLINE+1+(i)*highTot+high+1,s);
		}
	}
	if(nBegin<3)
		nShow++;
	high2 =rt.bottom -gScrollBott - (19*RIGHTBOX_PERLINE+1+nShow*highTot)-15;
	if(high2>0)
		pDC->FillSolidRect (left+1,19*RIGHTBOX_PERLINE+1+nShow*highTot,rt.right -left-1,high2,clr[(nShow+1)%2]);
	pDC->SelectObject(pOld);
	m_scrollBar->SetScrollRange(0,34);
}

void CTaiShanKlineShowView::GetCurrClientRect(CRect &rt)
{
	rt.top=0;
	rt.left=0;
	if(m_nKlineKind2==0)
	{
		int nm=m_infoInit.nCountMin1;
		if(this->m_infoFiguer >0)
			rt.right=m_rtMin1.rightX+ m_midLen + m_rightLength ;
		else
			rt.right=m_rtMin1.rightX+ m_midLen ;
		rt.bottom=m_rtMin1.rtBlw[nm-1].m_yBottom+m_heightCaption;
	}
	else
	{
		int nm=m_infoInit.nCountIndex;
		if(this->m_infoFiguer >0)
			rt.right=m_rtKlineFiguer.rightX+ m_midLen + m_rightLength ;
		else
			rt.right=m_rtKlineFiguer.rightX+ m_midLen ;
		rt.bottom=m_rtKlineFiguer.rtBlw[nm-1].m_yBottom+m_heightCaption;
	}

}



void CTaiShanKlineShowView::OpenPageFile()
{

}

void CTaiShanKlineShowView::SavePageFile()
{

}

void CTaiShanKlineShowView::OnViewFiveitem() 
{
	// TODO: Add your command handler code here
	DoViewItem(5);
}

void CTaiShanKlineShowView::OnUpdateViewFiveitem(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	DoUpdateViewItem(5,pCmdUI);
}

void CTaiShanKlineShowView::DoViewItem(int nItem)
{
	CRect r;
	if(m_nKlineKind2==0)
	{
		m_infoInit.nCountMin1=nItem;
	}
	else
	{
		m_infoInit.nCountIndex=nItem;
	}
	GetClientRect(r);
	OnSizeMy(r.right -r.left , r.bottom -r.top );
	pKlineDrawing->m_bToCacl = true;
	RedrawWindow();
}

void CTaiShanKlineShowView::DoUpdateViewItem(int nItem,CCmdUI* pCmdUI)
{
	if(m_nKlineKind2==0)
	{
		int nm=m_infoInit.nCountMin1;
		pCmdUI->SetCheck(nm == nItem?1:0);
	}
	else
	{
		int nm=m_infoInit.nCountIndex;
		pCmdUI->SetCheck(nm == nItem?1:0);
	}
	if(m_bMultiFiguer==1 )
	{
		pCmdUI->Enable (FALSE);
	}
	else
	{
		pCmdUI->Enable (TRUE);
	}

}

void CTaiShanKlineShowView::OutDataExcel(CMSFlexGrid *grid, int nFiguer)
{
	int nInd= nFiguer;
	if(nInd<0 || nInd>4)
		nInd = 0;

	CLongString longStr;

	//add title and kline data
	int nPerLine = pKlineDrawing->m_footEnd - pKlineDrawing->m_footBegin +1;
	if(nPerLine<=0)
		return ;

	int nOutLine = 0;
	int nBeginValid[30];
	float* pFloatArray[30];
	CString sTitleArray[30] = {"时间","开盘","最高","最低","收盘","成交量","成交金额"};
	int j=0,k=0;
	Kline* pKline = this->m_pkline+pKlineDrawing->m_footBegin;
	CString timeFormat = "%Y%m%d";
	if(this->m_nKlineKind2 <5 || m_nKlineKind2 == MANY_DAY_KLINE && GetDocument()->m_propertyInitiate.daysOfManyKline < 0	)
	{
		timeFormat = "%m%d%H%M";
	}
	else if(m_nKlineKind2 == HS_KLINE||m_nKlineKind2 == MIN1_KLINE)
	{
		timeFormat = "%H%M";
	}

	int *pInt = new int[nPerLine];
	int nFixed = 7;
	if( m_nKlineKind2 !=0)
	{
		if(nInd == 0)
		{
			nOutLine = 7+pKlineDrawing->m_dataFormular[nInd].numLine;
			for(j=0;j<7;j++)
			{
				nBeginValid[j]=0;
				if(j>0)
					pFloatArray[j] = new float[nPerLine];
				if(j==0)//time
				{
					for(k=0;k<nPerLine;k++)
						pInt[k] = atoi(CTime(pKline[k].day ).Format (timeFormat));
				}
				else//other data
				{
					int nRate = 1;
					if(j==6)
						nRate = 10000;
					for(k=0;k<nPerLine;k++)
						pFloatArray[j][k] = *(&(pKline[k].open)+j-1)/nRate ;
				}

			}
			for(j=7;j<nOutLine;j++)
			{
				pFloatArray[j] = pKlineDrawing->m_dataFormular[nInd].line[j-7].m_arrBE.line+pKlineDrawing->m_footBegin;//line[i].m_arrBE.line

				if(pKlineDrawing->m_dataFormular[nInd].line[j-7].m_arrBE.b<=-1)
					nBeginValid[j]=-1;
				else if(pKlineDrawing->m_dataFormular[nInd].line[j-7].m_arrBE.b<pKlineDrawing->m_footBegin)
					nBeginValid[j]=0;
				else
					nBeginValid[j]=pKlineDrawing->m_dataFormular[nInd].line[j-7].m_arrBE.b-pKlineDrawing->m_footBegin;
				sTitleArray[j]=pKlineDrawing->m_lineName[nInd][j-7];
			}
		}
		else
		{
			nFixed=1;

			nOutLine = 1+pKlineDrawing->m_dataFormular[nInd].numLine;
			//time
			nBeginValid[j]=0;
			for(k=0;k<nPerLine;k++)
					pInt[k] = atoi(CTime(pKline[k].day ).Format (timeFormat));
			for(j=1;j<nOutLine;j++)
			{
				pFloatArray[j] = pKlineDrawing->m_dataFormular[nInd].line[j-1].m_arrBE.line+pKlineDrawing->m_footBegin;
				//pKlineDrawing->m_footBegin
				if(pKlineDrawing->m_dataFormular[nInd].line[j-1].m_arrBE.b<=-1)
					nBeginValid[j]=-1;
				else if(pKlineDrawing->m_dataFormular[nInd].line[j-1].m_arrBE.b<pKlineDrawing->m_footBegin)
					nBeginValid[j]=0;
				else
					nBeginValid[j]=pKlineDrawing->m_dataFormular[nInd].line[j-1].m_arrBE.b - pKlineDrawing->m_footBegin;
				sTitleArray[j]=pKlineDrawing->m_lineName[nInd][j-1];
			}
		}
	}
	else
	{
		nFixed=1;
		nPerLine = pMin1Drawing->m_footEnd+1;

		nOutLine = 1+pMin1Drawing->m_dataFormular[nInd].numLine;
		//time
		nBeginValid[j]=0;
		CTime tm = CTime::GetCurrentTime ();
		CTime tm2(tm.GetYear(),tm.GetMonth(),tm.GetDay(),9,30,0);
		int nTot = (int)tm2.GetTime ();

		for(k=0;k<nPerLine;k++)
		{
			int nAdd = 0;
			if(k>119)
				nAdd = 30*3;
			timeFormat = "%m%d%H%M";
			pInt[k] = atoi(CTime(nTot + 60 * k +nAdd ).Format (timeFormat));
		}
		for(j=1;j<nOutLine;j++)
		{
			pFloatArray[j] = pMin1Drawing->m_dataFormular[nInd].line[j-1].m_arrBE.line;
			//pKlineDrawing->m_footBegin
			if(pMin1Drawing->m_dataFormular[nInd].line[j-1].m_arrBE.b<=-1)
				nBeginValid[j]=-1;
			else if(pMin1Drawing->m_dataFormular[nInd].line[j-1].m_arrBE.b<pMin1Drawing->m_footBegin)
				nBeginValid[j]=0;
			else
				nBeginValid[j]=pMin1Drawing->m_dataFormular[nInd].line[j-1].m_arrBE.b;
			sTitleArray[j]=pMin1Drawing->m_lineName[nInd][j-1];
		}
	}
	grid->SetRows(nPerLine+1);
	grid->SetCols(nOutLine);
	for(j=0;j<nOutLine;j++)
	{
		grid->SetTextArray(j,sTitleArray[j]);
	}

	//add other
	for(int i =0;i<nOutLine;i++)
	{
		for(j=0;j<nPerLine;j++)
		{
			CString s;
			if(i>0)
				s = CLongString::FloatToString (pFloatArray[i][j]);
			else
				s.Format("%d",pInt[j]);
			if(nBeginValid[i]<0 || nBeginValid[i]>j)
				s=" ";
			grid->SetTextMatrix(j+1,i,s);
		}
	}

	//delete float
	delete[] pInt;
	for(j=1;j<nFixed;j++)
		delete []pFloatArray[j];

}

DWORD CTaiShanKlineShowView::GetTimeSpan(CString s)
{
	static DWORD g_timeTick = 0;
	int n = GetTickCount();
	TRACE("%s %d\r\n",s,n-g_timeTick);
	g_timeTick=n;
	return 0;
}

void CTaiShanKlineShowView::RedrawWindowNoCacl()
{
	CRect r;
	GetCurrClientRect(r);
	this->CopyRect(r);

	//当m_tabNum=6时，右边的是一个RichEditCtrl，它不能通过CopyRect(r)得到，
	//所以必须重画
	if (m_tabNum==6)
	{
		CRect rc(r.right-m_rightLength+1,0,r.right,r.bottom-gScrollBott-5);
		DrawPingCe(GetDC(),rc);
	}
}

//得到平均成本
bool CTaiShanKlineShowView::GetPriceAve(float& f1,float totv,float totp)
{
	if(pMin1Drawing->m_pReportData==NULL)
		return false;
	if(totv==0 && totp ==0)
	{
		totv= pMin1Drawing->m_pReportData->totv ;
		totp= pMin1Drawing->m_pReportData->totp ;
	}
	if(totv <=0)
		return false;
	if(IsIndexStock(m_sharesSymbol))
	{
		return false;
	}

	float rat = 100;
	if(pMin1Drawing->m_pReportData->kind == 13 ||pMin1Drawing->m_pReportData->kind == 20)
	{
		if(pMin1Drawing->m_pReportData->id [0]<='2')
			rat = 1000;
	}
	f1=(totp/rat/totv);
	return true;

}

BASEINFO* CTaiShanKlineShowView::GetBaseinfoPointer()
{
	CReportData* pdt;
	BASEINFO* pBase = NULL;
	if(GetDocument()->m_sharesInformation.Lookup(this->m_sharesSymbol.GetBuffer(0),pdt,m_stkKind)==1)
	{
		if(pdt !=NULL)
			pBase = pdt->pBaseInfo;
	}
	m_sharesSymbol.ReleaseBuffer();
	return  pBase;

}
//增加模拟k线数据
void CTaiShanKlineShowView::SetKlineExt(Kline *pKlineIn, int nKline, int nFootWrite)
{
	KLINE_EXT_PER* pExt;
	Kline * pKline = NULL;

	if(m_klineExtMap .Lookup(m_sharesSymbol,pExt) )
	{
		ASSERT(pExt!=NULL);
	}
	else
	{
		pExt = new KLINE_EXT_PER;
		m_klineExtMap[m_sharesSymbol]=pExt;
	}
	if(nFootWrite>=0)
	{
		if (nFootWrite-this->m_nCountKlineToday < pExt->m_nCountKlineSelf )
		{
			pExt->m_pKlineSelf[nFootWrite-this->m_nCountKlineToday] = pKlineIn [nFootWrite];
		}
	}
	else
	{
		pKline = new Kline[pExt->m_nCountKlineSelf + nKline];
		if(pExt->m_nCountKlineSelf>0)
			memcpy(pKline,pExt->m_pKlineSelf ,pExt->m_nCountKlineSelf*sizeof(Kline));
		for(int i=0;i<nKline;i++)
		{
			pKline[pExt->m_nCountKlineSelf+i] = pKlineIn[i] ;
		}
		pExt->m_nCountKlineSelf += nKline;
		if(pExt->m_pKlineSelf != NULL) delete pExt->m_pKlineSelf ;
		
		pExt->m_pKlineSelf = pKline;

	}

	//add
}

void CTaiShanKlineShowView::ClearKlineExt()
{
	KLINE_EXT_PER* pExt;
	Kline * pKline = NULL;

	if(m_klineExtMap .Lookup(m_sharesSymbol,pExt) )
	{
		ASSERT(pExt!=NULL);
		pExt->m_nCountKlineSelf = 0;
		if(pExt->m_pKlineSelf!=NULL) 
		{
			delete[] pExt->m_pKlineSelf ;
			pExt->m_pKlineSelf = NULL;
			RedrawWindow();
		}
	}
}
/*
void CTaiShanKlineShowView::OnAlarmSetTemp() 
{
	// TODO: Add your command handler code here
	CAlertSystem dlg;
	dlg.DoModal ();
}
*/
void CTaiShanKlineShowView::OnDataChangAll(int nParam)
{
	CTaiShanDoc* pDoc = CMainFrame::m_pDoc ;
	int n = pDoc->m_arKLineViews.GetSize();
	for(int i =0;i<n;i++)
	{
		pDoc->m_arKLineViews [i]->SendMessage(WM_USER_TELL_CHANGE,0,(LPARAM )nParam);
	}

}

void CTaiShanKlineShowView::OnMenuitemDeleteK() 
{
	// TODO: Add your command handler code here
	ClearKlineExt();	
}

void CTaiShanKlineShowView::OnUpdateMenuitemDeleteK(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	KLINE_EXT_PER* pExt;
	Kline * pKline = NULL;


	if(m_klineExtMap .Lookup(m_sharesSymbol,pExt) )
	{
		ASSERT(pExt!=NULL);
		if(pExt->m_nCountKlineSelf > 0)
		{
			pCmdUI->Enable ();
			return;
		}
	}
	pCmdUI->Enable (FALSE);
}

void CTaiShanKlineShowView::HistoryHide()
{
	if(m_pDlgDealHistory != NULL)
	{
		m_pDlgDealHistory ->DestroyWindow ();
		delete m_pDlgDealHistory ;
		m_pDlgDealHistory = NULL;
		pMin1Drawing->m_sOldSymbol ="";
		this->pMin1Drawing->InitMinuteLine();
		if(m_nKlineKind2!=0)
		{
			pKlineDrawing->m_bToCacl =TRUE;
			pKlineDrawing ->m_bInitFoot =true;//
		}
	}
	RedrawWindow();

}

void CTaiShanKlineShowView::OnTjxgMmTestShow() 
{
	// TODO: Add your command handler code here
	if(m_screenStockShow.m_bUseing == false)
		m_screenStockShow.DoModal();
	else 
	{
		m_screenStockShow.m_bUseing = false;
	    CFile file;
		file.Remove("SaveTjxg.dat");
	}
	RedrawWindow();
}

void CTaiShanKlineShowView::OnUpdateTjxgMmTestShow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	CString s = "条件选股指示...";
	if(m_screenStockShow.m_bUseing == true)
		s = "取消选股指示"; 
	pCmdUI->SetText (s);
}

int CTaiShanKlineShowView::IsIndexStock(CString s)
{
	if(s.GetLength() < 4) return 0;

	if((s[1]=='A'||s[1]=='B'||s[1]=='C'||s[1]=='K')||(s[0] == '3'&&s[1] == '9'&&s[2] == '9'))
		return 1;
	else 
		return 0 ;
}

int CTaiShanKlineShowView::IsIndexStock3(CString s)
{
	if(s==CSharesCompute::GetIndexSymbol(0)||s==CSharesCompute::GetIndexSymbol(1)||s==CSharesCompute::GetIndexSymbol(2))
		return 1;
	else 
		return 0 ;      

}
//bShowCrossDlg
//			if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE)//成交量除权

void CTaiShanKlineShowView::ShowDlgCross(BOOL bShow)
{
	int nFlag = SW_HIDE;
	if(bShow == TRUE)
		nFlag = SW_SHOW;
	if(::IsWindow(m_dlgShowCross->m_hWnd))
	{
		if(::IsWindow (m_dlgShowCross->m_hWnd))
		{
			if(GetDocument()->m_propertyInitiate.bShowCrossDlg == TRUE && bShow == TRUE && m_isShowCross==1) m_dlgShowCross->ShowWindow(SW_SHOW);
			else m_dlgShowCross->ShowWindow(SW_HIDE);
		}
	}
	if(pKlineMode!=NULL)
	{
		if(::IsWindow (pKlineMode->m_hWnd))
		{
			if(m_nKlineKind2 == 0)
				pKlineMode->ShowWindow(SW_HIDE);
			else
				pKlineMode->ShowWindow(nFlag);
		}
	}
	if(m_pChangeIndexParam!=NULL)
	{
		if(::IsWindow (m_pChangeIndexParam->m_hWnd))
		{
			if(m_nKlineKind2 == 0)
				m_pChangeIndexParam->ShowWindow(SW_HIDE);
			else
				m_pChangeIndexParam->ShowWindow(nFlag);
		}
	}
	if(m_pDlgDealHistory!=NULL)
	{
		if(::IsWindow (m_pDlgDealHistory->m_hWnd))
			m_pDlgDealHistory->ShowWindow(nFlag);
	}
	if(bShow == TRUE)
	{
		this->SetFocus();
	}
	RedrawWindow();

}

void CTaiShanKlineShowView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
//	ShowDlgCross(bActivate);
	m_bActived =bActivate;

	CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
	CChildFrame* pFrameWnd=(CChildFrame*)((CMainFrame*)pMainWnd)->MDIGetActive();
	pFrameWnd->GetMenuCount();

	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	SetTimer(TM_SHOW_DLG,50,NULL);

	//宽带版数据申请
	if(bActivate == TRUE)
		if(m_pWideNet) m_pWideNet->AddStockFirst(m_sharesSymbol,m_stkKind);
//	RedrawWindow();
}

void CTaiShanKlineShowView::InvertFocusRect2(CDC* pDC)
{
	if(m_bActived==TRUE && !pDC->IsPrinting ())
	{
		CRect rt;
		GetCurrClientRect(rt);
		if(m_nKlineKind2==0)
		{
			rt.right=m_rtMin1.rightX;
			rt.left=m_rtMin1.leftX  ;
			rt.bottom++;
		}
		else
		{
			rt.right=m_rtKlineFiguer.rightX;
			rt.left=1  ;
		}
		rt.top=rt.bottom - m_heightCaption+1;
		pDC->InvertRect (rt);
	}

}

void CTaiShanKlineShowView::OnMoveChengben2() 
{
	// TODO: Add your command handler code here
	if(m_tabNum == 5 && m_infoInit.nIDRightSheet [5-4] == 2 && m_infoFiguer==1)
		return;
//	if(IsIndexStock(this->m_sharesSymbol))
//		return;

	m_tabNum = 5;
	m_infoInit.nIDRightSheet [5-4] = 2;
	m_infoFiguer=1;
	CRect r;
	GetClientRect(r);
	OnSizeMy(r.Width (),r.Height ());

	ShowAll(m_sharesSymbol);
}

void CTaiShanKlineShowView::OnTimeAstep4() 
{
	// TODO: Add your command handler code here
//	int nKind[4] = {0,1,5,6};
//	CreateSplitMy(2,nKind);

}

bool CTaiShanKlineShowView::CheckStockSymbol(CString s)
{
	CTaiShanDoc* pDoc = CMainFrame::m_pDoc ;
	int n = pDoc->m_arKLineViews.GetSize();
	for(int i =0;i<n;i++)
	{
		if(pDoc->m_arKLineViews [i]->m_sharesSymbol == s)
			return false;
	}
	return true;

}

void CTaiShanKlineShowView::RefreshChenben(CTaiKlineDC* pMemdc)
{
	if(m_tabNum>3 && m_infoInit.nIDRightSheet [m_tabNum-4]>0)
	{
		DrawRightSheet(pMemdc,m_tabNum);
	}

}

BOOL CTaiShanKlineShowView::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
/*	
	CMainFrame* pMainWnd = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	int nID = 6200;
	if(pMainWnd->m_drawLineToolBar->IsWindowVisible() != 0 )
		nID = 131;
	else if(this->m_tabNum>3)
	{
		if(m_infoInit.nIDRightSheet [m_tabNum-4] == 3 
			|| m_infoInit.nIDRightSheet [m_tabNum-4] == 4)
			;//nID = ;
	}
	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,nID);*/
	DoHtmlHelp(this);
	return TRUE;//CView::OnHelpInfo(pHelpInfo);
}

bool CTaiShanKlineShowView::CheckDiskFreeSpace()
{
	try
	{
	void* pGetDiskFreeSpaceEx = (void*)GetProcAddress( GetModuleHandle("kernel32.dll"),
							 "GetDiskFreeSpaceExA");

	if (pGetDiskFreeSpaceEx)
	{
		char ch[256];
		ULARGE_INTEGER i64FreeBytesToCaller,i64TotalBytes,i64FreeBytes;		
		::GetCurrentDirectory(256,ch);
		BOOL fResult = GetDiskFreeSpaceEx (ch,
					(PULARGE_INTEGER)&i64FreeBytesToCaller,
					(PULARGE_INTEGER)&i64TotalBytes,
					(PULARGE_INTEGER)&i64FreeBytes);

	   if(fResult)
	   {		   
		   long nZhao = i64FreeBytesToCaller.QuadPart /(1024*1024);
		   if(nZhao<50)
		   {
			   CString s;
			   s.Format ("您的硬盘空间已经低于%d,\r\n请删除无用文件！", nZhao);
			   AfxMessageBox(s);
			   return false;
		   }
	   }
	// Process GetDiskFreeSpaceEx results.
	}
	}
	catch(...)
	{
	}
	return true;
}

void CTaiShanKlineShowView::ViewSetFocus(CTaiShanKlineShowView *pView)
{
	pView->m_bActived = TRUE;
	pView->RedrawWindow();
	int n = CMainFrame::m_pDoc->m_arKLineViews.GetSize();
	for(int i=0;i<n;i++)
	{
		if(CMainFrame::m_pDoc->m_arKLineViews[i]==pView) continue;
		if(CMainFrame::m_pDoc->m_arKLineViews[i]->m_bActived== TRUE)
		{
			CMainFrame::m_pDoc->m_arKLineViews[i]->m_bActived = FALSE;
			CMainFrame::m_pDoc->m_arKLineViews[i]->RedrawWindow();
		}
	}
}

void CTaiShanKlineShowView::HistOpen(CString *s)
{
	if(CTaiKlineDlgDealHistory::HistoryOpen(this,s) == false)
		return;
	if(m_pDlgDealHistory!=NULL)
		pMin1Drawing->m_bHist =true;
	this->pMin1Drawing->InitMinuteLine();
	this->m_nKlineKind2 =0;
	RedrawWindow();
	SetFocus();

}

void CTaiShanKlineShowView::OnNineShow() 
{
	// TODO: Add your command handler code here
	GetDocument()->m_pReportView->GetParentFrame()->ActivateFrame();
	int m_key=81;
	::SendMessage(GetDocument()->m_pReportView->m_hWnd,WM_USER_FROM_KLINE,0,m_key); 
	
}

float CTaiShanKlineShowView::GetCapital(CReportData *pdt)
{
	float f =0;
	BASEINFO* pBase = NULL;
	{
		if(pdt !=NULL)
		{
			pBase = pdt->pBaseInfo;
			if(pBase!=NULL)
			{
				if(pdt->kind == SZBG || pdt->kind == SHBG)
					f = pBase->Bg *100;
				else
					f = pBase->ltAg *100;
			}
		}
	}
	return f;


}

void CTaiShanKlineShowView::HistoryAStep(int nFoot)
{
	if(m_pDlgDealHistory != NULL )
	{
		if(m_pDlgDealHistory->m_bHistorySynchrostep == true)
		{		
			int first=nFoot;//pKlineDrawing->m_footBegin + point.x*pKlineDrawing->m_klinNumDefault/m_rtKlineFiguer.rightX;
			//---		判断画K线的起始点和结束点
			if(first>pKlineDrawing->m_footEnd)
				return;
			if(m_pDlgDealHistory->HistoryAStep(this->m_pkline [first].day))
			{					
				m_pDlgDealHistory->m_bShowHistory = true;
				m_pDlgDealHistory->m_oldSymbol = "";
				pMin1Drawing->m_bHist =true;
				//m_fileName
				m_pDlgDealHistory->SetWindowText("历史回忆("+m_pDlgDealHistory->m_fileName+")");
			}
			else
			{					
				m_pDlgDealHistory->m_bShowHistory = false;
				pMin1Drawing->m_bHist =false;
				pMin1Drawing->m_sOldSymbol = "";
				m_pDlgDealHistory->SetWindowText("历史回忆");
			}
			RedrawWindow();
		}
	}

}

//评测 Added by ltr
void CTaiShanKlineShowView::DrawPingCe(CDC* pDC,CRect &rc)
{
	CTaiShanDoc* pDoc=(CTaiShanDoc*)GetDocument();
	COLORREF crBack=pDoc->m_colorArray[0];
	COLORREF crText=pDoc->m_colorArray[1];
	COLORREF crTitle=pDoc->m_colorArray[7];
	pDoc->m_pPingCe->GetChooseFormula(this);	//得到选股公式
	pDoc->m_pPingCe->SetStock(m_sharesSymbol);		//设置股票
	pDoc->m_pPingCe->SelectStock(this);			//进行评测
	ClearBack(pDC,rc);
	pDoc->m_pPingCe->OnDraw(m_pRichEdit, crBack, crText, crTitle, rc);			//打印结果
//	m_pRichEdit->ValidateRect( NULL);
/*	ClearBack(pDC,rc);

	ByjDrawRightRect byj(this,pDoc);
	byj.CalcSonFiguer(pDC,rc);*/

}

void CTaiShanKlineShowView::YHParam()
{
	CFormularContent * pJishuCurrent = NULL;
	int nInd= m_hit-REGION_PICT1;
	if(nInd<0 || nInd>4)
		nInd = 0;
	CString name(m_infoInit.initIndex[nInd].nameIndex);
	pJishuCurrent=(CFormularContent*)GetDocument()->LookUpArray (GetDocument()->m_formuar_index,name);
	if(pJishuCurrent == 0) return ;

	//
	CTaiKlineDlgYHParam dlg;
	SymbolKindArr sArr;
	SymbolKind sm;
	strcpy(sm.m_chSymbol ,m_sharesSymbol);
	sm.m_nSymbolKind = m_stkKind;
	sArr.Add (sm);
	if(dlg.Init (NULL,this->m_pkline,this->m_nCountKline,pJishuCurrent,this->m_nKlineKind2,&m_hs,&sArr))
	{
		dlg.DoModal ();
//		dlg.DestroyWindow ();
	}
}

CString CTaiShanKlineShowView::GetMultiSymbol(int iSymbol,int& stkKind)
{
	CString s;
	CTaiShanDoc* pDoc = GetDocument();
	int n;
	if(pDoc->m_pReportView !=NULL&&m_nSharesBegin == -1)//from report table
	{
		char  Code [10];
		memset(Code,0,10);
		int nRow = -1;

		n=(m_nShares+iSymbol)%m_nTotalStock;

		pDoc->m_pReportView->GetStockCodeForRow(n+1,Code,stkKind);
		s=Code ;

	}
	else
	{
		PCdat1 pStockData;
		pDoc->m_sharesInformation .GetStockItem(m_nSharesBegin,(m_nShares+iSymbol)%m_nTotalStock,pStockData);
		if(pStockData!=NULL)
		{
			s=pStockData->id ;
			stkKind = pStockData->kind;
			ASSERT(m_sharesSymbol.GetLength()>3);
		}
	}
	return s;

}
//返回值：0为分时走势，1为5分钟k线，5为日线，HS_KLINE(25)为分笔明细
int CTaiShanKlineShowView::GetDataKind()
{
	int n = HS_KLINE;
	if(this->m_nKlineKind2==0)
		n= 0 ;
	else if(this->m_nKlineKind2>0 && m_nKlineKind2<5)
		n = 1;
	//		if(pDoc->m_propertyInitiate.daysOfManyKline>0)
	else if(this->m_nKlineKind2>4 && m_nKlineKind2<9 || m_nKlineKind2 == MANY_DAY_KLINE && GetDocument()->m_propertyInitiate.daysOfManyKline>0)
		n = 5;
	return n;
}

void CTaiShanKlineShowView::WideNetRedraw(WPARAM wp, LPARAM lp)
{
	if(CTaiKlineFileHS::GetDataCountAll(m_sharesSymbol, m_stkKind) > this->m_hs.GetCount())
		CTaiKlineFileHS::RemoveHs (m_hs);
	if(this->m_nKlineKind2==0)
	{
		RedrawWindow();
		return;
	}
	else
	{
		if(m_bMultiFiguer==1)
		{
			//处理主图叠加的另外的股票
			for(int k = 0;k<4;k++)//only 4
			{
				int nk = CTaiKlineFileKLine::GetKlineCount(m_symbol9 [k], m_stkKind9[k],CTaiKlineFileKLine::IsDayKline(m_nKlineKind2));
				//m_symbol9 [k],m_stkKind9[k]
				if(m_nKline9[k]!=nk) //to reread
				{
					RedrawWindow();
				}
			}
		}
		else
		{
			int nk = CTaiKlineFileKLine::GetKlineCount(m_sharesSymbol, m_stkKind,CTaiKlineFileKLine::IsDayKline(m_nKlineKind2));
			if(nk != m_nCountKlineFile)
			{
				ShowAll(m_sharesSymbol,false,true);
			}
			bool bChange = false;
			for(int j=0;j<pKlineDrawing->m_nKlineCurrent;j++)
			{
				//
				int nk = CTaiKlineFileKLine::GetKlineCount(pKlineDrawing->m_symbolAdd[j], pKlineDrawing->m_stkKindAdd[j],CTaiKlineFileKLine::IsDayKline(m_nKlineKind2));
				if(nk != pKlineDrawing->m_nKlineFileAdd[j])
				{
					bChange = true;
					pKlineDrawing->TransferKlineTypeData(pKlineDrawing->m_symbolAdd[j], pKlineDrawing->m_stkKindAdd[j],pKlineDrawing->m_pKlineAdd[j],j);
				}
			}
			if(bChange == true)
				RedrawWindow();
			//m_nKlineFileAdd[nFlags]
		}
	}
	
}
void CTaiShanKlineShowView::OnMoveFig()
{
	DoOnCommand(IDM_POP_DRAG_MODE, (LPARAM)0);
}
void CTaiShanKlineShowView::OnUpdateMoveFig(CCmdUI* pCmdUI)
{
	if(pKlineDrawing->m_kLine_mode == 1)
		pCmdUI->SetCheck ();
	else
		pCmdUI->SetCheck (0);

}

void CTaiShanKlineShowView::OnSaveBitmap() 
{
	CDC* dc=GetDC();
	CRect r;
	GetCurrClientRect(r);
	// 创建显示内存位图
	CBitmap bit;
	bit.CreateCompatibleBitmap( dc, r.Width(),	r.Height() );
	CDC dcmem;
	dcmem.CreateCompatibleDC(dc);
	CBitmap* pOld = dcmem.SelectObject (&m_bit);

	CDC dcmem2;
	dcmem2.CreateCompatibleDC(dc);
	CBitmap* pOld2 = dcmem2.SelectObject (&bit);

    ::BitBlt(dcmem2.m_hDC,0, 0, r.Width(), r.Height(),
                          dcmem.m_hDC,0,0, SRCCOPY);
	dcmem.SelectObject (pOld);
	dcmem.DeleteDC ();
	dcmem2.SelectObject (pOld2);
	dcmem2.DeleteDC ();
	ReleaseDC(dc);


	CDIBSectionLite lite;
	lite.SetBitmap((HBITMAP)(bit));
	
	CString DefExt="*.bmp";
	CString FileName = m_sharesSymbol+".bmp";
	CString Filter="位图文件(*.bmp)|*.bmp||";  //.文件扩展名过滤器
    CFileDialog bSaveFileDialog(FALSE,(LPCSTR)DefExt,
		(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
		(LPCSTR)Filter,this);
    
	if( bSaveFileDialog.DoModal() ==IDOK )
	{
		CString NewFileName;  
		NewFileName=bSaveFileDialog.GetPathName();
		if(NewFileName.GetLength ()>4)
		{
			CString ext = NewFileName.Right (4);
			if(ext.CompareNoCase (".bmp") != 0)
				NewFileName += ".bmp";
		}
		else
			NewFileName += ".bmp";
		lite.Save (NewFileName);
	}	
}

void CTaiShanKlineShowView::ShowMark(CFormularContent *pJishu)
{
	m_screenStockShow.SetIndicator(pJishu);
	m_screenStockShow.DoModal();
	m_screenStockShow.m_bUseing = false;
	RedrawWindow();

}

void CTaiShanKlineShowView::DoF4()
{
	if(m_bMultiFiguer == 1) m_bMultiFiguer = 0;
	static bool bSh = true;
	if(bSh )
	{
		m_nKlineKind2 = 0;
		m_stkKind = SHZS;
		this->ShowAll(CSharesCompute::GetIndexSymbol(0));
	}
	else
	{
        m_nKlineKind2 = 0;
		m_stkKind = SZZS;
		this->ShowAll(CSharesCompute::GetIndexSymbol(1));
	}
	bSh = !bSh;

}

int CTaiShanKlineShowView::PointToFiguer()
{
	CPoint p;
	GetCursorPos(&p);
	ScreenToClient(&p);
	int nm=m_infoInit.nCountIndex;
	int n = 0 ;
	for(int i = 0 ; i< nm-1; i++)
	{
		if(p.y<m_rtKlineFiguer.rtBlw[i].m_yBottom)
		{
			n = i;
			break;
		}
		else
			n = i+1;
	}
	if(n>nm-1)
		n = nm - 1 ;

	return n;

}

void CTaiShanKlineShowView::ShowKlineMode(CFormularContent *pJishu)
{
	if(m_AutoChange == TRUE)
	{
	 m_AutoChange = false;
	 KillTimer(12);
	}

	if(pJishu)
	{
		if(pKlineMode==NULL)  
		{
			pKlineMode=new CTaiKlineDialogKLineMode(this);
			pKlineMode->Create(IDD_DIALOG_KLINE_MODE,this);
		}
		pKlineMode->Set(pJishu);
		pKlineMode->ShowWindow(SW_SHOW);
		RedrawWindow ();

	}
	else
	{
		if(pKlineMode==NULL)  
		{
			pKlineMode=new CTaiKlineDialogKLineMode(this);
			pKlineMode->Create(IDD_DIALOG_KLINE_MODE,this);
			pKlineMode->ShowWindow(SW_SHOW);
			RedrawWindow ();

		}
		else
		{
			pKlineMode->DestroyWindow();
			delete pKlineMode;
			pKlineMode=NULL;
		}
	}

}

BOOL CTaiShanKlineShowView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	bool bDown = true;
	int nPer = 1;
	int n = abs(zDelta)/120;
	if(n>nPer)
		nPer = n;
	if(zDelta>0)//page up
	{
		bDown = false;
	}
	OnPageDown( bDown, nPer);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CTaiShanKlineShowView::OnSwitchHistoryAcc() 
{

	if(m_pDlgDealHistory!=NULL)
	{
		m_pDlgDealHistory->SendMessage(WM_COMMAND,ID_HISTORY_SWITCH);
	}
	
}

void CTaiShanKlineShowView::OnAstepHist() 
{
	if(m_pDlgDealHistory!=NULL)
	{
		m_pDlgDealHistory->SendMessage(WM_COMMAND,ID_HISTORY_A_STEP);
	}
}

void CTaiShanKlineShowView::OnUpdateAstepHist(CCmdUI* pCmdUI) 
{
	if(m_pDlgDealHistory!=NULL)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

void CTaiShanKlineShowView::OnUpdateSwitchHistoryAcc(CCmdUI* pCmdUI) 
{
	if(m_pDlgDealHistory!=NULL)
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
	
}
void CTaiShanKlineShowView::DoHistoryAStep() 
{
	if(m_pDlgDealHistory != NULL )
	{
		if(m_pDlgDealHistory->m_bHistorySynchrostep == true)
		{		
			int first=pKlineDrawing->m_footCurrent;
			HistoryAStep(first);
		}
	}
}

void CTaiShanKlineShowView::DrawUpDown(CDC *pDC)
{
	CTaiShanDoc* pDoc = GetDocument();
	CFont ft;
	LOGFONT lgf=GetDocument()->m_fontstr[0];
	lgf.lfHeight=-14;
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);

	pDC->SetTextAlign( TA_LEFT);//TA_CENTER||
	pDC->SetBkMode(TRANSPARENT);
	int modify=3;
	CString s;

	float cls;
	cls=pMin1Drawing->m_close;
	pDC->SetTextColor( GetDocument()->m_colorArray[13]);

	int isSz=0;
	if(CSharesCompute::GetMarketKind(m_stkKind) == SZ_MARKET_EX) isSz=1;
	int isDown=0;
	int nKind=0;
	float f ;
	int nColorFoot[] = {1,13,PING_PAN_WORD,XIA_DIE_WORD,1};

	CRect rt;
	GetCurrClientRect(rt);
	int nEach = m_rightLength/5;
	int left = rt.right - m_rightLength+2;
	int top = RIGHTBOX_PERLINE*17+1;
	CString ss1[] = {"分类","涨","平","跌","额(亿)"};
	CString ss[] = {"A股","B股","基金","其它"};

	pDC->SetTextColor( GetDocument()->m_colorArray[1]);
	for(isDown = 0;isDown<5;isDown++)
	{
		pDC->TextOut (left+isDown*nEach ,top, ss1[isDown]);
	}
	for(;nKind<4;nKind++)
	{
		//text out name
		for(isDown = 0;isDown<5;isDown++)
		{
			pDC->SetTextColor( GetDocument()->m_colorArray[nColorFoot[isDown]]);
			CString s ;
			if(isDown)
			{
				f = GetDocument()->m_sharesInformation.GetValueUpDown( isDown-1, isSz,  nKind);
				if(isDown != 4)
					s.Format ("%d",(int)f);
				else
					s.Format ("%.2f",f/100000000);
			}
			else
			{
				s= ss[nKind];
			}
			pDC->TextOut (left+isDown*nEach ,top+(nKind+1)*RIGHTBOX_PERLINE, s);
		}

	}

	top = RIGHTBOX_PERLINE*22+1;;
	int nShow = (rt.bottom - top - gScrollBott-5)/RIGHTBOX_PERLINE;
	if(nShow>0)
	{
		int nData = GetDocument()->m_sharesCompute.m_dataArr[isSz].GetSize();
		int b = 0;
		if(nData>nShow)
			b = nData-nShow;
		int iLine = 0;
		pDC->SetTextAlign( TA_LEFT);//TA_CENTER||
		nEach = m_rightLength/3;
		for(int i = b;i<nData;i++)
		{
			CReportData* pp;
			if(pDoc->m_sharesInformation.Lookup(pDoc->m_sharesCompute.m_dataArr[isSz][i].m_chSymbol,pp,pDoc->m_sharesCompute.m_dataArr[isSz][i].m_nSymbolKind))
			{
				//draw
				pDC->SetTextAlign( TA_LEFT);//
				pDC->SetTextColor( pDoc->m_colorArray[1]);
				pDC->TextOut (left+0*nEach ,top+(iLine)*RIGHTBOX_PERLINE, pp->name );

				pDC->SetTextAlign( TA_RIGHT);//
				pDC->SetTextColor( GetColor(pp, false));
				CString s;
				s.Format ("%.2f",pp->nowp );
				pDC->TextOut (left+2*nEach-1 ,top+(iLine)*RIGHTBOX_PERLINE, s );
				pDC->SetTextColor( GetColor(pp, true));
				s.Format ("%.0f",pp->nowv );
				pDC->TextOut (left+3*nEach-1 ,top+(iLine)*RIGHTBOX_PERLINE, s);

				iLine++;
			}
		}
	}


	pDC->SelectObject(pOld);

}

COLORREF CTaiShanKlineShowView::GetColor(CReportData *pp, bool isVol)
{
	CTaiShanDoc* pDoc = GetDocument();

	if(!isVol)
	{
		if(pp->nowp>pp->ystc )
			return pDoc->m_colorArray[13];
		if(pp->nowp<pp->ystc )
			return pDoc->m_colorArray[XIA_DIE_WORD];
		if(pp->nowp==pp->ystc )
			return pDoc->m_colorArray[PING_PAN_WORD];
	}
	else
	{
		if(pp->nowp>=pp->psel1  )
			return pDoc->m_colorArray[13];
		if(pp->nowp<=pp->pbuy1  )
			return pDoc->m_colorArray[XIA_DIE_WORD];
		else
			return pDoc->m_colorArray[PING_PAN_WORD];
	}
}
/*		DWORD dw = GetTickCount();
		for(int i = 0;i<200;i++)
		{*/
/*		}
		dw = GetTickCount()-dw;
		TRACE("in LoadImageMap:   %d\n",dw);*/
