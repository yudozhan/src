// CTaiShanKlineShowView.h : interface of the CTaiShanKlineShowView class
//
// 文件名：CTaiShanKlineShowView.h

// 内容描述：

//		处理技术分析画面的用户输入、操作。
//		
//		本类的输入：文档类指针。
//		
//		本类的输出：把技术分析画面画到屏幕上。
//		
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VWBASEVIEW_H__D69209B5_A1E6_11D1_B031_000021002C16__INCLUDED_)
#define AFX_VWBASEVIEW_H__D69209B5_A1E6_11D1_B031_000021002C16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "CFormularCompute.h"
#include "StructKlineView.h"	// Added by ClassView
//#include "vwbaseDoc.h"
#include "CTaiShanDoc.h"
#include "MyTipWin.h"	// Added by ClassView
#include "CTaiKlineDialogCross.h"	// Added by ClassView
#include "CTaiKlineShowText.h"	// Added by ClassView
#include "LBsInfoRS.h"
//#include "LDlgShowTJXG.h"
#include "CTaiKlineDlgChangeIndexParam.h"
#include "StructTaiShares.h"
#include "CTaiKlineDlgDealHistory.h"
#include "MSFlexGrid.h"
#include "CTaiScreensIndicate.h"

#include "XScrollBar.h"    //cjg


#define FIGUER_ALL_NUM 5
#define ZOOMIMG        14

class LBsInfoRS;
class CTaiKlineShowKline;
class CTaiKlineMin1;
class CTaiKlineDialogCross;
class CTaiKlineDrawing;
class CTaiKlineShowText;
class CTaiKlineDialogKLineMode;
//class LDlgShowTJXG;
class CSheetScrollBar;
//class CMSFlexGrid;
class CTaiKlineWideNet;

class CTaiKlineDC : public CDC
{
public:

    // constructor sets up the memory DC
    CTaiKlineDC(CWnd* pWnd,CBitmap* pBitmap,CRect* r = NULL) : CDC()
    {
        ASSERT(pWnd != NULL);

        m_pDC = new CClientDC(pWnd);
        m_pOldBitmap = NULL;
		if(r==NULL)
			pWnd->GetClientRect (m_rect);
		else
			m_rect = *r;
              
        this->CreateCompatibleDC(m_pDC);
        m_pOldBitmap = SelectObject(pBitmap);
//            SetWindowOrg(m_rect.left, m_rect.top);
    }
    
    // Destructor copies the contents of the mem DC to the original DC
    virtual ~CTaiKlineDC()
    {
        // Copy the offscreen bitmap onto the screen.
        ::BitBlt(m_pDC->m_hDC,m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
                          this->m_hDC, m_rect.left, m_rect.top, SRCCOPY);

        //Swap back the original bitmap.
        this->SelectObject(m_pOldBitmap);
		delete m_pDC;
    }

    // Allow usage as a pointer
    CTaiKlineDC* operator->() {return this;}
        
    // Allow usage as a pointer
    operator CTaiKlineDC*() {return this;}

private:
    CBitmap  m_bitmap;      // Offscreen bitmap
    CBitmap* m_pOldBitmap;  // bitmap originally found in CTaiKlineDC
    CDC*     m_pDC;         // Saves CDC passed in constructor
    CRect    m_rect;        // Rectangle of drawing area.
    BOOL     m_bMemDC;      // TRUE if CDC really is a Memory DC.
};

/////////////////////////////////////////////////////////////////////////////
struct PAGE_DATA_INFO
{
	int footBegin;
	int footEnd;
	int klineNumDefault;

	int nStockBegin;//-1 is from report table
	int nStock;
	int nTotalStock;

//	int isShowScroll;			//右边信息框内是否显示滚动条
	int kLineType;			//K线类型，0为1分钟走势线，1为5分钟K线，2为10分钟...
//	int  nFiguer;//子图的个数
	SUB_FIGUER_INIT_INFO	m_infoInit;	//初始化信息
//	int  nOldHitY;//y轴方向的跟踪的坐标

	int tabNum;					//右边信息框内下边的属性页的标签的个数
 	int infoFiguer;		//右边信息框 ,0为没有信息框,1为个股信息框,2为大盘信息框
	int m_bMultiFiguer;//是否是多图显示

	bool bBaseInfo;//是否基本资料
	BYTE isShowShiZi;//是否显示十字光标

	char symbol[STK_SYMBOL_LEN];
	int m_stkKind;
	PAGE_DATA_INFO()
	{
		memset(symbol,0,8);
		m_stkKind = 0;
	};
};
struct KLINE_EXT_PER
{
	//有关模拟k线数据成员
	int m_nCountKlineSelf;//模拟k线总个数
	Kline* m_pKlineSelf;//模拟K线数据
	KLINE_EXT_PER()
	{
		m_nCountKlineSelf = 0;
		m_pKlineSelf = 0;
	};
};
class CTaiTestRichEditCtrl;
class CTaiKlineFileKLine;
class CTaiKlineDlgDealHistory;
class CTaiKlineDialogShiDuanTJ;
class CTaiKlineDlgZhibiaoTongji;

typedef CTypedPtrMap<CMapStringToPtr,CString,KLINE_EXT_PER*> CMapStringToKlineExt;
class CTaiShanKlineShowView : public CView
{
protected: // create from serialization only
	CTaiShanKlineShowView();
	DECLARE_DYNCREATE(CTaiShanKlineShowView)

// Attributes
public:
	void DoHistoryAStep() ;
	float CaclMid(int i);
   	void RemoveHs(int flag);//去掉成交明细链表的数据
	int FindTimeK(int keyVlu);
	int ReadKline5Min(CString fName,int stkKind, Kline *&pKline,int nRead=-1);//5.31
	int ReadKLine(CString fName,int stkKind,Kline*& pKline,int nRead=-1);	//读取K线数据


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTaiShanKlineShowView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL
     

// Implementation
public:
	void ShowKlineMode(CFormularContent *pJishu);
	void DoF4();
	void ShowMark(CFormularContent* pJishu = NULL);
	void WideNetRedraw(WPARAM wp,LPARAM lp);
	int m_stkKind;
	int GetDataKind();
	CString GetMultiSymbol(int iSymbol,int &stkKind);
	void YHParam();
	void HistoryAStep(int nFoot);
	static float GetCapital(CReportData* pdt);
	static bool CheckDiskFreeSpace();
	static bool CheckStockSymbol(CString s);
	void InvertFocusRect2(CDC* pDC);
	void ShowDlgCross(BOOL bShow = TRUE);
	int m_nCurrentKlineKind;
	static int IsIndexStock3(CString s);
	static int IsIndexStock(CString s);
	void HistoryHide();
	static void OnDataChangAll(int nParam);
	void ClearKlineExt();
	void SetKlineExt(Kline* pKlineIn,int nKline=1,int nFootWrite = -1);
	static CTypedPtrMap<CMapStringToPtr,CString,KLINE_EXT_PER*> m_klineExtMap;//CMapStringToKlineExt
	BASEINFO* GetBaseinfoPointer();
	bool GetPriceAve(float &f1,float totv = 0,float totp = 0);
	void RedrawWindowNoCacl();
	static DWORD GetTimeSpan(CString s);
	void OutDataExcel(CMSFlexGrid* grid,int nFiguer);

	CTaiKlineWideNet * m_pWideNet;
	CTaiScreensIndicate m_screenStockShow;
	void GetCurrClientRect(CRect& rt);
	CTaiShanDoc* GetDocument();
	void SetPageData();
	CTaiKlineDialogShiDuanTJ* m_dlgFenshiTj;
	CTaiKlineDialogShiDuanTJ* m_dlgKlineTj;
	CTaiKlineDlgZhibiaoTongji* m_dlgZhibiao;
	CString m_symbol9[9];
	int m_stkKind9[9];
	int m_nKline9[9];
	
	
	//members variables
	CTaiKlineDlgDealHistory* m_pDlgDealHistory;//历史回忆对话框
	CArray<PAGE_DATA_INFO,PAGE_DATA_INFO>	m_pageInfoArray;//保存画面数据信息的数组
	int m_nPageCurrent;//当前是第几页
	CBuySellList m_hs ;//换手数据的链表
	CFJList	m_fenjia;//分价数据的链表
	Kline* m_pkline;	//K线数据
	bool m_bAddKline;
	CString m_sharesSymbol;//股票代码
	int m_nCountKline;//k线总个数
	int m_nCountKlineFile;//文件中读出的k线总个数
	int m_nCountKlineToday;//文件中读出的k线总个数加上当天数据
	SUB_FIGUER_INIT_INFO	m_infoInit;	//初始化信息
	bool bExtend;
	int lineTypeDrawed;
	int m_nOldnFgr;
	int RIGHTBOX_LINECOUNT;

	bool m_bInited;//初始化是否完成
	bool IsBarShow;//是否画线工具条被显示
	bool m_bKD;	//是否前一次操作是键盘按下

	int m_xPrinter;//打印机的绝对坐标宽度
	int m_yPrinter;//打印机的绝对坐标宽度
	int m_cxExt;//打印机的对坐标宽度
	int m_cyExt;//打印机的对坐标宽度

	int m_nBeginHS;//从第几个换手数据开始画
	bool bTrackCurcorBgn;//是否开始跟踪鼠标
	int m_minGridX;
	int m_minGrid;
	int m_heightCaption ;
	int m_midLen        ;	
	int m_rightLength	  ;
	bool m_GetFocus;  ////////////if true the View can get the focus
	///else the view can not always keep the focus
	CTaiKlineDlgChangeIndexParam* m_pChangeIndexParam;//修改指标参数窗口
//	LDlgShowTJXG* m_tjxgShow;//条件选股指示对话框

	int m_nSharesBegin;//-1 is from report table
	int m_nShares;
	int m_nTotalStock;
	int m_AutoChange;

	bool m_bKeyUpDown;//是否键盘一直按下
	bool m_bBaseInfo;//是否基本资料
	bool m_bDoPower;//是否除权
	CTaiKlineDialogKLineMode* pKlineMode;//K线组合
	LBsInfoRS m_bs;//基本资料数据集
	BYTE m_isShowCross;//是否显示十字光标
	CPoint m_toScreen;//转换到屏幕坐标
	int m_bMultiFiguer;//是否是多图显示
	CTaiKlineDialogCross* m_dlgShowCross;//十字光标窗口
	float m_currentValue;//当前光标的浮点数值
	CTaiTestTipWin* m_MyToolTip;//浮动信息窗口
	CRect m_rectHitPos;////跟踪矩形
	int m_nOldHitPos;//原来的鼠标按下的点
	int m_nOldHitY;//y轴方向的跟踪的坐标
	CTaiKlineShowKline* pKlineDrawing;//K线对象的指针
	CTaiKlineMin1* pMin1Drawing;//1分钟实时走势对象的指针
	CTaiKlineDrawing* pDrawLine;//自画线对象指针
	int m_hit;				//鼠标所在处的区域标识ID号
    int m_tjxgshow;                 //if show 条件选股提示 1 show /
 	int m_infoFiguer;		//右边信息框 ,0为没有信息框,1为个股信息框,2为大盘信息框
	int m_nMaxY;				//计算机屏幕的高度
	int m_nMaxX;				//计算机品目的宽度
	CPoint m_pointMove;			//鼠标移动时鼠标所处的位置
//	int m_isShowScroll;			//右边信息框内是否显示滚动条
	int m_nKlineKind2;			//K线类型，0为1分钟走势线，1为5分钟K线，2为10分钟...

	CBitmap	m_bit;				//内存位图
	CDC*    dc;
	int m_tabNum;					//右边信息框内下边的属性页的标签的个数
	//Marenan +
	CXScrollBar* m_scrollBar;		//右边信息框内下边的属性页中的滚动条指针
	//Marenan -
	//CScrollBar* m_scrollBar;		//右边信息框内下边的属性页中的滚动条指针
	
	BOOL  m_bInfoTabFill;           //右边信息框内下边的属性页是否(充满)延伸到顶端 //cjg
	BOOL  m_bZoomEnable;
	CRect  m_rcZoom;				//cjg 放大缩小按钮位置

	RECTFENSHI m_rtMin1;		//分时画面的位置信息
	RECTKLINE m_rtKlineFiguer;	//K线图画面属性

	CTaiTestRichEditCtrl*  m_pRichEdit;		//用于显示黄金眼评测的控件


	void DrawZoom(CDC* pDC,LPRECT lpRect);
	//functions
	void DrawRightSheet(CDC* pDC,int Num,bool bShowScroll=true);//画右边的属性页窗口
	void DrawRightSheet(CDC* pDC,CPoint pt);//画右边的属性页窗口

	void ClearBack(CDC* pDC,CRect& rt,bool bHist=false);//刷新背景

	BOOL DoOnCommand(WPARAM wParam, LPARAM lParam);//执行大多数弹出式菜单命令及常规菜单命令
	//输入参数:wParam 是输入的命令ID号
	//输出参数:无

	bool CreateSplitMy(int bVert=1,int * pInt = NULL);//进行分割窗口
	//输入参数:bVert 是否是垂直分割
	//输出参数:无
	void AddNewStock(CString& symbol,int nStkKind);//主图叠加时新增一只股票
	//输入参数:symbol 是新增股票的代码
	//输出参数:无

	void SetConstant();//设置常数

	void ReadDataMultiFiguer(int nItem);//读数据
	//输入参数:nItem 是第几个股票
	void CaclStockPos();//计算股票在内存中的位置

	void ShowAll(CString sharesymbol,bool bCaclStockPos = true,bool bInitFoot = true);    //显示一只新股票技术分析画面
	//输入参数:sharesymbol 是股票的代码,bCaclStockPos是否重新计算股票的位置,bInitFoot是否重新计算绘画范围
	//输出参数:无

	void ReDrawSheet(BOOL UpOrDown);
	//功能:画右边的属性页窗口
	//输入参数:UpOrDown 是切换的方向
	void OnPageDown(bool bDown,int nPer=1);
	//功能:按照股票顺序切换股票
	//输入参数:bDown 是true时按顺序向下切换,nPer是跳过的个数

	void WriteKLine5min(int nFoot);
	//功能:写5分钟k线数据到文件中
	//输入参数:nFoot 是5分钟k线数据的下标

	void OnPrintKLine(CDC* pDC);
	//功能:打印技术分析图

	void WriteKLine(int nFoot,bool bOne,bool bDelete = false);
	void OnMenu1fs();
//	功能:切换到分时走势
	void OnDayKline();
//	功能:切换到日线
	void ShowKlineBig(bool bBig=true);//是否重新显示K线
//	输入参数: bBig是是否是放大
	void ShowTransferText(CDC* pDC);//显示K线类型转换的文本
	void ShowTextRect(int n,CDC* pDC);//显示选择哪一个子图
	void CopyCaption();//画标题
	void ShowCross(bool bKey=false,int x=-1,int y=-1);//显示十字光标
	void ClearCross();//清除十字光标
	void CopyRect(CRect r);//从内存位图复制到位图
//	输入参数: r要复制的客户区矩形
	void ShowShiZi();//显示十字光标
	void ShowShiZi(int x,int y,int flag=0);//显示十字光标
	void ShowMyMenu(UINT id,int subM=0);//显示快捷菜单
//	输入参数: id 菜单资源ID号,subM是第几个子菜单
	int TransferX(int x);//把x坐标转换为数组下标
	void TrackCursorEnd(CPoint point,BOOL flag);//跟踪鼠标结束
	void TrackCursorGoOn(CPoint p);//继续跟踪鼠标
	void TrackCursorBgn(CPoint point);//跟踪鼠标开始
	void OnSizeMy(int cx, int cy);//重新确定大小
//	输入参数: cx 窗口宽度,cy窗口高度

	void SetPictYPos(int which,int y);//设置子图的位置
//	输入参数: which 是第几个子图,y是纵坐标位置

	void DrawRightBoxDapan(CDC* pDC,bool bClearAll=true);//画大盘的信息窗口
	void DrawRightText(CDC *pDC,float vl,int x, int yNum, int flag,int floatBit=2);//画文本
//	输入参数: vl是输入的浮点数,x是横坐标,yNum是纵坐标相对位置

	void DrawRightbox(CDC* pDC,bool bClearAll=true);	//画右边信息边框
	void CalcCursorShape(CPoint pt);	//计算鼠标形状
	////////////////////////
//-----K线
	void DrawRulorBoxY(int y);	//画Y轴游标矩形
	void DrawRulorBoxX(int x,bool bKey=false);	//画X轴游标矩形
	void DrawFenShi(CDC* pDC);	//画1分钟走势线

	void CaclRulorMovable(CPoint point);		//计算和画出游标小矩形

	void DrawRectPer(CDC* pDC);		//画矩形的边框
	void InitRect(int cx, int cy);	//初始化画面的绝对位置

	void DrawLineCurve(CDC* pDC);	//画右边信息框内下边的属性页的标签
	void BuySell();
	void DrawZhibiao(CString ZhibiaoName,int nFiguer = -1);   //指标显示
//	输入参数: ZhibiaoName 是指标名称
	void RefreshChenben(CTaiKlineDC* pMemdc);
	virtual ~CTaiShanKlineShowView();

  

//	----------------------
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	COLORREF GetColor(CReportData* pp,bool isVol);
	void DrawUpDown(CDC* pDC);
	int PointToFiguer();
	//{{AFX_MSG(CTaiShanKlineShowView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnLineself();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKCacl();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//Marenan +
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pBar);
	//Marenan -
	//afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnXianduan();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKlineMode();
	afx_msg void OnBsInfoShow();
	afx_msg void OnPowerDo();
	afx_msg void OnZhutuDiejia();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnViewGpHuanQuan();
	afx_msg void OnUpdateViewGphq(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnViewBuysell();
	afx_msg void OnRealTime();
	afx_msg void OnDayLine();
	afx_msg void OnUpdateBsInfoShow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewBuysell(CCmdUI* pCmdUI);
	afx_msg void OnViewSimple();
	afx_msg void OnViewPersent();
	afx_msg void OnViewDszb();
	afx_msg void OnUpdateViewDszb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewSimple(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPersent(CCmdUI* pCmdUI);
	afx_msg void OnViewOne();
	afx_msg void OnUpdateViewOne(CCmdUI* pCmdUI);
	afx_msg void OnViewFourstock();
	afx_msg void OnUpdateViewFourstock(CCmdUI* pCmdUI);
	afx_msg void OnViewLine();
	afx_msg void OnUpdateViewLine(CCmdUI* pCmdUI);
	afx_msg void OnViewDayline();
	afx_msg void OnUpdateViewDayline(CCmdUI* pCmdUI);
	afx_msg void OnViewFifteenmin();
	afx_msg void OnUpdateViewFifteenmin(CCmdUI* pCmdUI);
	afx_msg void OnViewThirtymin();
	afx_msg void OnUpdateViewThirtymin(CCmdUI* pCmdUI);
	afx_msg void OnViewSixtymin();
	afx_msg void OnUpdateViewSixtymin(CCmdUI* pCmdUI);
	afx_msg void OnViewMonthline();
	afx_msg void OnUpdateViewMonthline(CCmdUI* pCmdUI);
	afx_msg void OnViewWeekline();
	afx_msg void OnUpdateViewWeekline(CCmdUI* pCmdUI);
	afx_msg void OnView5min();
	afx_msg void OnUpdateView5min(CCmdUI* pCmdUI);
	afx_msg void OnViewOneitem();
	afx_msg void OnUpdateViewOneitem(CCmdUI* pCmdUI);
	afx_msg void OnViewTwoitem();
	afx_msg void OnUpdateViewTwoitem(CCmdUI* pCmdUI);
	afx_msg void OnViewFouritem();
	afx_msg void OnUpdateViewFouritem(CCmdUI* pCmdUI);
	afx_msg void OnViewThreeitem();
	afx_msg void OnUpdateViewThreeitem(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZhutuDiejia(CCmdUI* pCmdUI);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnFileOpen1();
	afx_msg void OnTjxgTjxgts();
	afx_msg void OnUpdateTjxgTjxgts(CCmdUI* pCmdUI);
	afx_msg void OnToolCursor();
	afx_msg void OnUpdateToolCursor(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePowerDo(CCmdUI* pCmdUI);
	afx_msg void OnViewJszb();
	afx_msg void OnViewstockdetail();
	afx_msg void OnF2();
	afx_msg void OnViewYearkline();
	afx_msg void OnUpdateViewYearkline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateKlineMd(CCmdUI* pCmdUI);
	afx_msg void OnToolDrawline();
	afx_msg void OnHldjLmb();
	afx_msg void OnUpdateHldjLmb(CCmdUI* pCmdUI);
	afx_msg void OnHedjLmb();
	afx_msg void OnUpdateHedjLmb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewJszb(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolDrawline(CCmdUI* pCmdUI);
	afx_msg void OnCopyFile();
	afx_msg void OnLoopDisplay();
	afx_msg void OnUpdateLoopDisplay(CCmdUI* pCmdUI);
	afx_msg void OnDelZhutuL();
	afx_msg void OnUpdateDelZhutuL(CCmdUI* pCmdUI);
	afx_msg void OnQldjLmb();
	afx_msg void OnUpdateQldjLmb(CCmdUI* pCmdUI);
	afx_msg void OnQedjLmb();
	afx_msg void OnUpdateQedjLmb(CCmdUI* pCmdUI);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnKlineHs();
	afx_msg void OnUpdateKlineHs(CCmdUI* pCmdUI);
	afx_msg void OnKlineManyDay();
	afx_msg void OnUpdateKlineManyDay(CCmdUI* pCmdUI);
	afx_msg void OnKline1Minu();
	afx_msg void OnUpdateKline1Minu(CCmdUI* pCmdUI);
	afx_msg void OnLSplitV();
	afx_msg void OnUpdateLSplitV(CCmdUI* pCmdUI);
	afx_msg void OnLSplitH();
	afx_msg void OnLSplitDel();
	afx_msg void OnUpdateLSplitDel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLSplitH(CCmdUI* pCmdUI);
	afx_msg void OnLSplit4();
	afx_msg void OnUpdateLSplit4(CCmdUI* pCmdUI);
	afx_msg void OnClassifyJishu();
	afx_msg void OnHistoryOpen();
	afx_msg void OnUpdateHistoryOpen(CCmdUI* pCmdUI);
	afx_msg void OnViewFiveitem();
	afx_msg void OnUpdateViewFiveitem(CCmdUI* pCmdUI);
	afx_msg void OnMenuitemDeleteK();
	afx_msg void OnUpdateMenuitemDeleteK(CCmdUI* pCmdUI);
	afx_msg void OnTjxgMmTestShow();
	afx_msg void OnUpdateTjxgMmTestShow(CCmdUI* pCmdUI);
	afx_msg void OnMoveChengben2();
	afx_msg void OnTimeAstep4();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnNineShow();
	afx_msg void OnMoveFig();
	afx_msg void OnUpdateMoveFig(CCmdUI* pCmdUI);
	afx_msg void OnSaveBitmap();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSwitchHistoryAcc();
	afx_msg void OnAstepHist();
	afx_msg void OnUpdateAstepHist(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSwitchHistoryAcc(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void DoDrawLine(UINT nID);
	afx_msg void DoDrawLineUpdate(CCmdUI* pCmdUI);
	afx_msg LRESULT OnDataChange(WPARAM wParam, LPARAM lParam);//
	afx_msg LRESULT OnDropDrag(WPARAM wParam, LPARAM lParam);//
	DECLARE_MESSAGE_MAP()
private:
	void HistOpen(CString * s = NULL);
	static void ViewSetFocus(CTaiShanKlineShowView* pView);
	BOOL m_bActived ;
	void DoUpdateViewItem(int nItem,CCmdUI* pCmdUI);
	void DoViewItem(int nItem);
	void SavePageFile();
	void OpenPageFile();
//	int m_tabSheetID[10];
	void DrawFinace(CDC* pDC,int nBegin=0);
	void DrawPingCe(CDC* pDC,CRect &rc);	//“评”
	void GetPageData(bool bMovePre = true);
	void CopyScrollBar(CDC* pMemDC);
	void KlineReadTransferData();
};

#ifndef _DEBUG  // debug version in CTaiShanKlineShowView.cpp
inline CTaiShanDoc* CTaiShanKlineShowView::GetDocument()
   { return (CTaiShanDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VWBASEVIEW_H__D69209B5_A1E6_11D1_B031_000021002C16__INCLUDED_)
