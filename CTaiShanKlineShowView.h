// CTaiShanKlineShowView.h : interface of the CTaiShanKlineShowView class
//
// �ļ�����CTaiShanKlineShowView.h

// ����������

//		����������������û����롢������
//		
//		��������룺�ĵ���ָ�롣
//		
//		�����������Ѽ����������滭����Ļ�ϡ�
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

//	int isShowScroll;			//�ұ���Ϣ�����Ƿ���ʾ������
	int kLineType;			//K�����ͣ�0Ϊ1���������ߣ�1Ϊ5����K�ߣ�2Ϊ10����...
//	int  nFiguer;//��ͼ�ĸ���
	SUB_FIGUER_INIT_INFO	m_infoInit;	//��ʼ����Ϣ
//	int  nOldHitY;//y�᷽��ĸ��ٵ�����

	int tabNum;					//�ұ���Ϣ�����±ߵ�����ҳ�ı�ǩ�ĸ���
 	int infoFiguer;		//�ұ���Ϣ�� ,0Ϊû����Ϣ��,1Ϊ������Ϣ��,2Ϊ������Ϣ��
	int m_bMultiFiguer;//�Ƿ��Ƕ�ͼ��ʾ

	bool bBaseInfo;//�Ƿ��������
	BYTE isShowShiZi;//�Ƿ���ʾʮ�ֹ��

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
	//�й�ģ��k�����ݳ�Ա
	int m_nCountKlineSelf;//ģ��k���ܸ���
	Kline* m_pKlineSelf;//ģ��K������
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
   	void RemoveHs(int flag);//ȥ���ɽ���ϸ���������
	int FindTimeK(int keyVlu);
	int ReadKline5Min(CString fName,int stkKind, Kline *&pKline,int nRead=-1);//5.31
	int ReadKLine(CString fName,int stkKind,Kline*& pKline,int nRead=-1);	//��ȡK������


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
	CTaiKlineDlgDealHistory* m_pDlgDealHistory;//��ʷ����Ի���
	CArray<PAGE_DATA_INFO,PAGE_DATA_INFO>	m_pageInfoArray;//���滭��������Ϣ������
	int m_nPageCurrent;//��ǰ�ǵڼ�ҳ
	CBuySellList m_hs ;//�������ݵ�����
	CFJList	m_fenjia;//�ּ����ݵ�����
	Kline* m_pkline;	//K������
	bool m_bAddKline;
	CString m_sharesSymbol;//��Ʊ����
	int m_nCountKline;//k���ܸ���
	int m_nCountKlineFile;//�ļ��ж�����k���ܸ���
	int m_nCountKlineToday;//�ļ��ж�����k���ܸ������ϵ�������
	SUB_FIGUER_INIT_INFO	m_infoInit;	//��ʼ����Ϣ
	bool bExtend;
	int lineTypeDrawed;
	int m_nOldnFgr;
	int RIGHTBOX_LINECOUNT;

	bool m_bInited;//��ʼ���Ƿ����
	bool IsBarShow;//�Ƿ��߹���������ʾ
	bool m_bKD;	//�Ƿ�ǰһ�β����Ǽ��̰���

	int m_xPrinter;//��ӡ���ľ���������
	int m_yPrinter;//��ӡ���ľ���������
	int m_cxExt;//��ӡ���Ķ�������
	int m_cyExt;//��ӡ���Ķ�������

	int m_nBeginHS;//�ӵڼ����������ݿ�ʼ��
	bool bTrackCurcorBgn;//�Ƿ�ʼ�������
	int m_minGridX;
	int m_minGrid;
	int m_heightCaption ;
	int m_midLen        ;	
	int m_rightLength	  ;
	bool m_GetFocus;  ////////////if true the View can get the focus
	///else the view can not always keep the focus
	CTaiKlineDlgChangeIndexParam* m_pChangeIndexParam;//�޸�ָ���������
//	LDlgShowTJXG* m_tjxgShow;//����ѡ��ָʾ�Ի���

	int m_nSharesBegin;//-1 is from report table
	int m_nShares;
	int m_nTotalStock;
	int m_AutoChange;

	bool m_bKeyUpDown;//�Ƿ����һֱ����
	bool m_bBaseInfo;//�Ƿ��������
	bool m_bDoPower;//�Ƿ��Ȩ
	CTaiKlineDialogKLineMode* pKlineMode;//K�����
	LBsInfoRS m_bs;//�����������ݼ�
	BYTE m_isShowCross;//�Ƿ���ʾʮ�ֹ��
	CPoint m_toScreen;//ת������Ļ����
	int m_bMultiFiguer;//�Ƿ��Ƕ�ͼ��ʾ
	CTaiKlineDialogCross* m_dlgShowCross;//ʮ�ֹ�괰��
	float m_currentValue;//��ǰ���ĸ�����ֵ
	CTaiTestTipWin* m_MyToolTip;//������Ϣ����
	CRect m_rectHitPos;////���پ���
	int m_nOldHitPos;//ԭ������갴�µĵ�
	int m_nOldHitY;//y�᷽��ĸ��ٵ�����
	CTaiKlineShowKline* pKlineDrawing;//K�߶����ָ��
	CTaiKlineMin1* pMin1Drawing;//1����ʵʱ���ƶ����ָ��
	CTaiKlineDrawing* pDrawLine;//�Ի��߶���ָ��
	int m_hit;				//������ڴ��������ʶID��
    int m_tjxgshow;                 //if show ����ѡ����ʾ 1 show /
 	int m_infoFiguer;		//�ұ���Ϣ�� ,0Ϊû����Ϣ��,1Ϊ������Ϣ��,2Ϊ������Ϣ��
	int m_nMaxY;				//�������Ļ�ĸ߶�
	int m_nMaxX;				//�����ƷĿ�Ŀ��
	CPoint m_pointMove;			//����ƶ�ʱ���������λ��
//	int m_isShowScroll;			//�ұ���Ϣ�����Ƿ���ʾ������
	int m_nKlineKind2;			//K�����ͣ�0Ϊ1���������ߣ�1Ϊ5����K�ߣ�2Ϊ10����...

	CBitmap	m_bit;				//�ڴ�λͼ
	CDC*    dc;
	int m_tabNum;					//�ұ���Ϣ�����±ߵ�����ҳ�ı�ǩ�ĸ���
	//Marenan +
	CXScrollBar* m_scrollBar;		//�ұ���Ϣ�����±ߵ�����ҳ�еĹ�����ָ��
	//Marenan -
	//CScrollBar* m_scrollBar;		//�ұ���Ϣ�����±ߵ�����ҳ�еĹ�����ָ��
	
	BOOL  m_bInfoTabFill;           //�ұ���Ϣ�����±ߵ�����ҳ�Ƿ�(����)���쵽���� //cjg
	BOOL  m_bZoomEnable;
	CRect  m_rcZoom;				//cjg �Ŵ���С��ťλ��

	RECTFENSHI m_rtMin1;		//��ʱ�����λ����Ϣ
	RECTKLINE m_rtKlineFiguer;	//K��ͼ��������

	CTaiTestRichEditCtrl*  m_pRichEdit;		//������ʾ�ƽ�������Ŀؼ�


	void DrawZoom(CDC* pDC,LPRECT lpRect);
	//functions
	void DrawRightSheet(CDC* pDC,int Num,bool bShowScroll=true);//���ұߵ�����ҳ����
	void DrawRightSheet(CDC* pDC,CPoint pt);//���ұߵ�����ҳ����

	void ClearBack(CDC* pDC,CRect& rt,bool bHist=false);//ˢ�±���

	BOOL DoOnCommand(WPARAM wParam, LPARAM lParam);//ִ�д��������ʽ�˵��������˵�����
	//�������:wParam �����������ID��
	//�������:��

	bool CreateSplitMy(int bVert=1,int * pInt = NULL);//���зָ��
	//�������:bVert �Ƿ��Ǵ�ֱ�ָ�
	//�������:��
	void AddNewStock(CString& symbol,int nStkKind);//��ͼ����ʱ����һֻ��Ʊ
	//�������:symbol ��������Ʊ�Ĵ���
	//�������:��

	void SetConstant();//���ó���

	void ReadDataMultiFiguer(int nItem);//������
	//�������:nItem �ǵڼ�����Ʊ
	void CaclStockPos();//�����Ʊ���ڴ��е�λ��

	void ShowAll(CString sharesymbol,bool bCaclStockPos = true,bool bInitFoot = true);    //��ʾһֻ�¹�Ʊ������������
	//�������:sharesymbol �ǹ�Ʊ�Ĵ���,bCaclStockPos�Ƿ����¼����Ʊ��λ��,bInitFoot�Ƿ����¼���滭��Χ
	//�������:��

	void ReDrawSheet(BOOL UpOrDown);
	//����:���ұߵ�����ҳ����
	//�������:UpOrDown ���л��ķ���
	void OnPageDown(bool bDown,int nPer=1);
	//����:���չ�Ʊ˳���л���Ʊ
	//�������:bDown ��trueʱ��˳�������л�,nPer�������ĸ���

	void WriteKLine5min(int nFoot);
	//����:д5����k�����ݵ��ļ���
	//�������:nFoot ��5����k�����ݵ��±�

	void OnPrintKLine(CDC* pDC);
	//����:��ӡ��������ͼ

	void WriteKLine(int nFoot,bool bOne,bool bDelete = false);
	void OnMenu1fs();
//	����:�л�����ʱ����
	void OnDayKline();
//	����:�л�������
	void ShowKlineBig(bool bBig=true);//�Ƿ�������ʾK��
//	�������: bBig���Ƿ��ǷŴ�
	void ShowTransferText(CDC* pDC);//��ʾK������ת�����ı�
	void ShowTextRect(int n,CDC* pDC);//��ʾѡ����һ����ͼ
	void CopyCaption();//������
	void ShowCross(bool bKey=false,int x=-1,int y=-1);//��ʾʮ�ֹ��
	void ClearCross();//���ʮ�ֹ��
	void CopyRect(CRect r);//���ڴ�λͼ���Ƶ�λͼ
//	�������: rҪ���ƵĿͻ�������
	void ShowShiZi();//��ʾʮ�ֹ��
	void ShowShiZi(int x,int y,int flag=0);//��ʾʮ�ֹ��
	void ShowMyMenu(UINT id,int subM=0);//��ʾ��ݲ˵�
//	�������: id �˵���ԴID��,subM�ǵڼ����Ӳ˵�
	int TransferX(int x);//��x����ת��Ϊ�����±�
	void TrackCursorEnd(CPoint point,BOOL flag);//����������
	void TrackCursorGoOn(CPoint p);//�����������
	void TrackCursorBgn(CPoint point);//������꿪ʼ
	void OnSizeMy(int cx, int cy);//����ȷ����С
//	�������: cx ���ڿ��,cy���ڸ߶�

	void SetPictYPos(int which,int y);//������ͼ��λ��
//	�������: which �ǵڼ�����ͼ,y��������λ��

	void DrawRightBoxDapan(CDC* pDC,bool bClearAll=true);//�����̵���Ϣ����
	void DrawRightText(CDC *pDC,float vl,int x, int yNum, int flag,int floatBit=2);//���ı�
//	�������: vl������ĸ�����,x�Ǻ�����,yNum�����������λ��

	void DrawRightbox(CDC* pDC,bool bClearAll=true);	//���ұ���Ϣ�߿�
	void CalcCursorShape(CPoint pt);	//���������״
	////////////////////////
//-----K��
	void DrawRulorBoxY(int y);	//��Y���α����
	void DrawRulorBoxX(int x,bool bKey=false);	//��X���α����
	void DrawFenShi(CDC* pDC);	//��1����������

	void CaclRulorMovable(CPoint point);		//����ͻ����α�С����

	void DrawRectPer(CDC* pDC);		//�����εı߿�
	void InitRect(int cx, int cy);	//��ʼ������ľ���λ��

	void DrawLineCurve(CDC* pDC);	//���ұ���Ϣ�����±ߵ�����ҳ�ı�ǩ
	void BuySell();
	void DrawZhibiao(CString ZhibiaoName,int nFiguer = -1);   //ָ����ʾ
//	�������: ZhibiaoName ��ָ������
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
	void DrawPingCe(CDC* pDC,CRect &rc);	//������
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
