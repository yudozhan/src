
// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__61A31CED_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
#define AFX_MAINFRM_H__61A31CED_9707_11D1_ACAB_0000E823DBFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CTaiShanKlineShowView.h"
#include "TestDialogbar.h"
//#include  "DrawToolBar.h"
#include "CTaiScreen.h"
#include "CTaiScreenTest.h"
#include "CStatusBarTai.h"
#include "CTaiKlineMemFile.h"
#include "DialogCwdLeftPop.h"
//add by qyp 2004.10.22
#include "ClientTransmitManageWnd.h"
#include "CustomMenu.h"
#include "BCMenu.h"

class CTaiShanDoc ;
class CMDIChildWnd ;

#define	 Gp_Msg_StkData			WM_APP + 1
class CAlertSystem;
class CAlertPopDlg;
class CTaiKlineDlgNeuralLearn;

class CMainFrame : public CXTPMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

    CTaiToolBar m_TestDialogbar; //This bar is for all view
	CDialogCwdLeftPop m_dlgLeftPop;

	//added by qyp 2004.10.22
#ifdef WIDE_NET_VERSION
	static CClientTransmitManageWnd* m_pClientTransmitManageWnd;
	//discribe logon status:if value=TRUE,logon success;value=FALSE,logon failed...
	static BOOL m_bLogonSuccess;
#else
    CStockDriver	m_StockDriverDll;
#endif
private:
#ifdef WIDE_NET_VERSION
	BOOL m_bLogonInterface;
#endif
	//added end

// Attributes
public:
	CTaiKlineMemFile m_memFile;

	int InformationLoop;

// Operations

public:

	CAlertPopDlg* m_pDlgAlarmPop;
	CAlertSystem* m_pDlgAlarm;
	bool m_bAlarmExec;
	static CTaiShanDoc *m_pDoc;      //Active Document
    CMDIChildWnd *m_MDIChildWnd;            //Active Child Wondow
    CTaiShanKlineShowView* pVwbaseView;
	int nLKind[11];

	////modified by Xia Qiankun 2004/2/18 ////////////
	//
	//CXTPToolBar* m_drawLineToolBar; //This bar is only for Lmb: Drawing line Tool
	CToolBar* m_drawLineToolBar; //This bar is only for Lmb: Drawing line Tool
	//
	//other toolbar is no used and deleted by Xia Qiankun 2/18
	////////////end 2004/2/18/////////////////////////////////////////

	/////////////zjh///////////
    void        Drawline();

	int         m_EnableF10;               ///if 0 means no view
                                           ///if 1 means do nothing
	                                       ///if 2 means replace
    BOOL		m_EnableF9;                 ///if true Enable F9
		                                   ///else disable F9
	BOOL        m_F9ORF10;                   //indentify if click on the F9 or F10;
	                                       //True is F9 and then is F10
	bool        m_BaseInfo;                ///Enable Base info///
	BOOL        m_StopCuoHe;               //Open or Stop Cuohe       
    ////////Add for test the performance////////////////
	IndexDataInfo m_index_save;
	//////////////////////////////////////////////

	// Overrides          
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	public:
	afx_msg void OnCompanyinfo();

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	bool CanUseIndicatorProtected();
	BOOL IsSuccRegistered();
	void ViewGgxx(int  nID = -1);
	void RefreshFormu();
	CString GetCurrentSelectStockCode( WORD & wMarket );
	CTaiScreen* ShowTjxg();
	BOOL m_fDLL;
	void start_stockdll();
	int nHeight;
	int nY;
	int nX;
	
	
	void TestHQ();
	static void ActiveVwbase(CTaiShanKlineShowView* pView);
	static int StaticDrawKlineWindow(WPARAM wParam, LPARAM lParam);
	CStatusBar_sjl  m_wndStatusBar;
	bool bAlarm;
    void DisplayBargain(/*CString szStockName,CString szReason,int Amount,float Price,*/
	      float Index_sh,float UpDown_sh,float Vol_sh,float Amount_sh,
	      float Index_sz,float UpDown_sz,float Vol_sz,float Amount_sz,
          float Index_cy,float UpDown_cy,float Vol_cy,float Amount_cy,
	      float percent_sh, float percent_sz,float percent_cy);
	void SetStatusBar();	
	void OnUpdateViewFullScreen(CCmdUI* pCmdUI);
	void OnMenuFullscreen();
	CRect m_FullScreenWindowRect;
	BOOL m_bFullScreen;		//全屏幕显示标志
	void ReViewInfo(void);	////全屏幕显示窗口Rect
	WINDOWPLACEMENT m_wpPrev;        //用于保存正常视图时的窗口位置信息
	int CreateToolDrawLine();
	void HqStock_Init();     //起动股票接收操作
	void HqStock_Quit();     //退出股票接收操作
   //BOOL CalculateAlarm();
    void MFmessageBox(char * str);
    void MFLogs(char * str);

	static CTaiScreen *m_pWndTJXG;
	static CTaiScreenTest *m_pWndTJXGTest;
	static CTaiKlineDlgNeuralLearn* m_pWndNL;
/////////Add for Test//////////////
protected:
   	 CStringArray m_symbol_array;
	 IndexDataInfo* pIndex_save;
	 int *nDays;
	 int nIndexNum;
	 int nStockNum;

	 //////////////
protected:  // control bar embedded members
	
	//CDialogTJXG    *m_pWndTJXG;
	BOOL	m_bRunFlag;				// 
	CRect m_RiseFallRect_sz;
	CRect m_RiseFallRect_sh;

// Generated message map functions
protected:

	CRect m_rcButtExit;	//关闭按钮位置
	CRect m_rcButtMax;	//最大化按钮位置
	CRect m_rcButtMin;	//最小化按钮位置
	CRect m_rcButtHelp;	//帮助按钮位置
	CRect m_rcIcon;		//图标位置
	CRect m_rcTitleBar;	//标题条位置

public:
	static int		m_iMenuItemCount;//菜单的宽度
	 int			m_iMenuWidth;//菜单的项数

protected:
	
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg long OnStkDataOK( UINT wFileType, LONG lPara)	;
    afx_msg long OnExtService(UINT wFileType, LONG lPara)	;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnViewCompanyinfo();
	afx_msg void OnViewGgxx();
	afx_msg void OnFullscreen();
	afx_msg void OnNomalscreen();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	//afx_msg void OnNcPaint();
	afx_msg void OnToolSjgl();
	afx_msg void OnToolGsgl();
	afx_msg void OnTjxgTjxg();
	afx_msg void OnViewXx();
	afx_msg void OnStopCuohe();
	afx_msg void OnToolTzgl();
	afx_msg void OnToolRiskIndex();
	afx_msg void OnUpdateFullscreen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNomalscreen(CCmdUI* pCmdUI);
	afx_msg void OnOpenBigBill();
	afx_msg void OnF3();
	afx_msg void OnF4();
	afx_msg void OnClose();
	afx_msg void OnHelpTopic();
	afx_msg void OnUpdateToolGsgl(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBaseinfoGrid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCuoheOpen(CCmdUI* pCmdUI);
	afx_msg void OnGrid();
	afx_msg void OnUpdateGrid(CCmdUI* pCmdUI);
	afx_msg void OnUpdateF3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateF4(CCmdUI* pCmdUI);
	afx_msg void OnWindowNew();
	afx_msg void OnUpdateWindowNew(CCmdUI* pCmdUI);
	afx_msg void OnTjxgAlarm();
	afx_msg void OnToolBlockMana();
	afx_msg void OnUpdateBlockDayLine();
	afx_msg void OnUpdateBlockMin5Line();
	afx_msg void OnDownload20();
	afx_msg void OnCloseReceiver();
	afx_msg void OnUpdateCloseReceiver(CCmdUI* pCmdUI);
	afx_msg void OnEnterInternet();
	afx_msg void OnTjxgMmTest();
	afx_msg void OnToolNnetTrain();
	afx_msg void OnToolAutoSubmit();
	afx_msg void OnDayLine();
	afx_msg void OnMin1Line();
	afx_msg void OnUpdateStockBig(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTjxgAlarm(CCmdUI* pCmdUI);
	afx_msg void OnF6();		//切换到动态显示牌的自选股页面
	afx_msg void OnPaint();
	afx_msg void OnSortIndex();
	afx_msg void OnXgnExtData();
	afx_msg void OnClearRealdata();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnCalc();
	afx_msg void OnGdfxRegister();
	//}}AFX_MSG
    afx_msg void DisplayAlarmForMsg(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnTjxg(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT DrawKlineWindow(WPARAM wParam, LPARAM lParam);


#ifdef WIDE_NET_VERSION
    afx_msg void OnLoginServer();
	afx_msg void OnRecvStockcode();
#endif

	BCMenu m_hMenuReport;
	BCMenu m_hMenuDraw;


	DECLARE_MESSAGE_MAP()
public:
	HMENU AttachReportViewMenu();
	HMENU AttachDrawLineMenu();
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void DrawTitleBar(CDC *pDC);
protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
public:
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
public:
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
public:
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
public:
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnViewBlockanalysis();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__61A31CED_9707_11D1_ACAB_0000E823DBFD__INCLUDED_)
