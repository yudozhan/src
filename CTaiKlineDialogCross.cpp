// CTaiKlineDialogCross.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiShanDoc.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"
#include "CTaiKlineDialogCross.h"
#include "MemDC.h"
#include "StructKlineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDialogCross dialog


CTaiKlineDialogCross::CTaiKlineDialogCross(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDialogCross::IDD, pParent)
{
	m_nHeight = 240;
	m_bSaveBackground = true;
	pView = (CTaiShanKlineShowView* )pParent;
//	ASSERT(pView!=NULL);
	//{{AFX_DATA_INIT(CTaiKlineDialogCross)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_isFirst=true;
}


void CTaiKlineDialogCross::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDialogCross)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDialogCross, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDialogCross)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	ON_WM_MOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDialogCross message handlers
CTaiShanDoc* pDoc = NULL;		//CTaiShanDoc的指针
CTaiShanKlineShowView* pView = NULL;		//CTaiShanKlineShowView的指针


void CTaiKlineDialogCross::OnPaint() 
{
	if(pView == NULL)
		return;
	if(pView->pMin1Drawing->m_pReportData==NULL)
		return;
	CClientDC dc(this); // device context for painting
	CMemDC memDC(&dc);

	pDoc=CMainFrame::m_pDoc; 
	CRect r2;
	GetClientRect(r2);
	memDC.FillSolidRect(r2,pDoc->m_colorArray[0]);
	int wd = r2.Width();
	int ht = r2.Height ();

	CFont ft2;
	LOGFONT lgf2=pDoc->m_fontstr[1];//m_fontstr[0]
//	lgf2.lfHeight=-14;
//	lgf2.lfWeight =FW_BOLD;
	ft2.CreateFontIndirect (&lgf2 );
	CFont* pOldF=memDC.SelectObject(&ft2);
	m_nHeight = memDC.GetOutputTextExtent("宋体").cy +3;


	static CString sName2="";
	if(pView->pMin1Drawing ->m_pReportData->name!=sName2)
	{
		SetWindowText(pView->pMin1Drawing ->m_pReportData->name);
		sName2 = pView->pMin1Drawing ->m_pReportData->name;
	}

	memDC.SetTextColor( pDoc->m_colorArray[1]);
	memDC.SetBkMode(TRANSPARENT);
	if(m_isFirst==true)
	{
		m_isFirst=false;
	}
	float fVal[16];
	for(int k = 0;k<16;k++)
		fVal[k] = 0;
	if(pView->m_nKlineKind2 ==0)
	{
//		if(m_isFirst==true)
		//11
		static CString strMin1Name[] = {"时间","数值","成交价","成交量","成交额","涨跌值","涨跌幅","成本价","委买量","委卖量","换手率"};

		if(pView->pMin1Drawing ->m_footCurrent<0)
			pView->pMin1Drawing ->m_footCurrent = 0;
		if(pView->pMin1Drawing ->m_footCurrent>239)
			pView->pMin1Drawing ->m_footCurrent = 239;

		memDC.SetTextAlign( TA_LEFT );//TA_CENTER||

		int i;
		for(i=0;i<11;i++)
		{
			memDC.TextOut (1,i*m_nHeight,strMin1Name[i]);
		}
		m_isFirst=false;

		memDC.SetTextAlign( TA_RIGHT );//TA_CENTER||

		CStringArray sArr;
		int right=r2.right -1;
		CString s;
		s=pView->pMin1Drawing ->TimeToString(pView->pMin1Drawing ->m_footCurrent-pView->pMin1Drawing ->m_footBegin);
		sArr.Add(s);

		if(pView->m_hit>=REGION_PICT1&&pView->m_hit<=REGION_PICT5)
		{
			if(pView->pMin1Drawing ->m_currentValue>=1000000)
				s.Format ("%d",(int)pView->pMin1Drawing ->m_currentValue);
			else
				s.Format ("%.3f",pView->pMin1Drawing ->m_currentValue);
		}
		else
			s="";
		sArr.Add(s);

		if(pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]<0)
			pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]=0;
		if(pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]>10000000)
			pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]=0;
		s.Format ("%.3f",pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]);
		sArr.Add(s);
		fVal[2] = pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]
				-pView->pMin1Drawing ->m_close;

		s.Format ("%d",(int)pView->pMin1Drawing ->m_vol[pView->pMin1Drawing ->m_footCurrent]);
		sArr.Add(s);

		s.Format ("%.3f",(float)(pView->pMin1Drawing ->m_amount[pView->pMin1Drawing ->m_footCurrent]/10000));
		sArr.Add(s);

		s.Format ("%.3f",(pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]-pView->pMin1Drawing ->m_close));
		sArr.Add(s);
		fVal[5] = fVal[2];

		if(pView->pMin1Drawing ->m_close>0)
		{
			s.Format ("%.3f",(pView->pMin1Drawing ->m_dataFormular[0].line[0].m_arrBE.line[pView->pMin1Drawing ->m_footCurrent]-pView->pMin1Drawing ->m_close)*100/pView->pMin1Drawing ->m_close);
			s=s+"%";
		}
		else
			s="";
		sArr.Add(s);
		fVal[6] = fVal[2];

		//平均成本
		float f1;
		if(pView->GetPriceAve(f1))
		{
			s.Format ("%.3f",f1 );
		}
		else
			s="";
		sArr.Add(s);
		fVal[7] = fVal[2];

		//委买量
		s.Format ("%d",(int)pView->pMin1Drawing ->m_pReportData->accb );
		sArr.Add(s);
		fVal[8] = fVal[2];
		//委卖量
		s.Format ("%d",(int)pView->pMin1Drawing ->m_pReportData->accs );
		sArr.Add(s);
		fVal[9] = fVal[2];

		//换手率
		float ltp = CTaiShanKlineShowView::GetCapital(pView->pMin1Drawing ->m_pReportData);
		if(ltp>0)
		{
			s.Format ("%.3f",pView->pMin1Drawing->m_vol[pView->pMin1Drawing ->m_footCurrent]/ltp*100 );
		}
		else
			s="";
		sArr.Add(s);
		for(i=0;i<sArr.GetSize ();i++)
		{
			if(i==2 || i>=5 && i<=9 ) SetColor(&memDC,fVal[i]);
			else memDC.SetTextColor( pDoc->m_colorArray[1]);
			memDC.TextOut (right,i*m_nHeight,sArr[i]);
		}

	}
	else if(pView->pKlineDrawing ->ValidDoKline()) 

	{
		//12
		static CString strKlineName[] = {"时间","数值","开盘","最高","最低","收盘","成交量","成交额","涨跌幅","振幅","成本价","换手率"};

		if(pView->pKlineDrawing ->m_footCurrent<0)
			pView->pKlineDrawing ->m_footCurrent = 0;
		if(pView->pKlineDrawing ->m_footCurrent>pView->m_nCountKline -1)
			pView->pKlineDrawing ->m_footCurrent = pView->m_nCountKline -1;

		memDC.SetTextAlign( TA_LEFT );//TA_CENTER||
		int i;
		for(i=0;i<12;i++)
		{
			memDC.TextOut (1,i*m_nHeight,strKlineName[i]);
		}
		m_isFirst=false;

		memDC.SetTextAlign( TA_RIGHT );//TA_CENTER||
		CStringArray sArr;


		int right=r2.right -1;
		CString s;

		if(pView->pKlineDrawing ->m_footCurrent>=0)
		{
			s=pView->pKlineDrawing ->TimeToStr(pView->pKlineDrawing ->m_footCurrent-pView->pKlineDrawing ->m_footBegin);
			sArr.Add(s);

			if(pView->m_hit>=REGION_PICT1&&pView->m_hit<=REGION_PICT5)
			{
				if(pView->pKlineDrawing ->m_currentValue>=1000000)
					s.Format ("%d",(int)pView->pKlineDrawing ->m_currentValue);
				else
					s.Format ("%.3f",pView->pKlineDrawing ->m_currentValue);
			}
			else
				s="";
			sArr.Add(s);

			s.Format ("%.3f",pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].open);
			sArr.Add(s);
			if(pView->pKlineDrawing ->m_footCurrent>0)
				fVal[2] = pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].open
					-pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close;

			s.Format ("%.3f",pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].high);
			sArr.Add(s);
			if(pView->pKlineDrawing ->m_footCurrent>0)
				fVal[3] = pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].high
					-pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close;

			s.Format ("%.3f",pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].low);
			sArr.Add(s);
			if(pView->pKlineDrawing ->m_footCurrent>0)
				fVal[4] = pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].low
					-pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close;

			s.Format ("%.3f",pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].close);
			sArr.Add(s);
			if(pView->pKlineDrawing ->m_footCurrent>0)
				fVal[5] = pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].close
					-pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close;

			s.Format ("%d",(int)pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].vol);
			sArr.Add(s);
			
			s.Format ("%.1f",pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].amount/10000);
			sArr.Add(s);

			s="";
			if(pView->pKlineDrawing ->m_footCurrent > 0)
			{
				if(pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close>0 )
				{
					s.Format ("%.2f",(pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].close
						-pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close)*100/
						pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close);
					s+="%";
				}
			}
			sArr.Add(s);
			fVal[8] = fVal[5];
			//振幅
			s="";
			if(pView->pKlineDrawing ->m_footCurrent > 0)
			{
				if(pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close>0 )
				{
					s.Format ("%.2f",(pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].high
						-pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].low)*100/
						pView->m_pkline[pView->pKlineDrawing ->m_footCurrent-1].close);
					s+="%";
				}
			}
			sArr.Add(s);

			//成本价
			s="";
			float f1;
			if(pView->GetPriceAve( f1,pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].vol,pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].amount))
				s.Format ("%.3f",f1);
			sArr.Add(s);

			//换手率
			float ltp = CTaiShanKlineShowView::GetCapital(pView->pMin1Drawing ->m_pReportData);
			if(ltp >0)
			{
				s.Format ("%.3f",pView->m_pkline[pView->pKlineDrawing ->m_footCurrent].vol/ltp*100  );
			}
			else
				s="";
			sArr.Add(s);

			for(i=0;i<sArr.GetSize ();i++)
			{
				if(i>1 && i<6 || i == 8) SetColor(&memDC,fVal[i]);
				else memDC.SetTextColor( pDoc->m_colorArray[1]);
				memDC.TextOut (right,i*m_nHeight,sArr[i]);
			}

		}

	}
	memDC.SelectObject(pOldF);
	//draw line
	CPen pen(PS_SOLID   ,1, pDoc->m_colorArray[17]);
	CPen* penOld;
	penOld = memDC.SelectObject(&pen);
	for(int i=0;i<ht/m_nHeight; i++)
	{
		memDC.MoveTo(0,i*m_nHeight-3);
		memDC.LineTo(wd,i*m_nHeight-3);
	}
	memDC.SelectObject(penOld);
	//end

	CRect r;
	GetClientRect(r);
	// 将变化的部分从位图拷贝到屏幕上
	ValidateRect(r);
	////////////////
	// Do not call CDialog::OnPaint() for painting messages
}
/*	
	if(f1>0)
		pDC->SetTextColor( GetDocument()->m_colorArray[13]);
	else if(f1<0)
		pDC->SetTextColor( GetDocument()->m_colorArray[XIA_DIE_WORD]);
	else
		pDC->SetTextColor( GetDocument()->m_colorArray[PING_PAN_WORD]);
		SetColor(CDC* pDC,float fVal);
*/
BOOL CTaiKlineDialogCross::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//	LOGFONT lgf=pView->m_fontstr[0];
//	lgf.lfHeight=-12;
//	lgf.lfWidth=8;
	CClientDC dc(this);

	CFont ft2;
	LOGFONT lgf2=CMainFrame::m_pDoc ->m_fontstr[1];//m_fontstr[0]
	ft2.CreateFontIndirect (&lgf2 );
	CFont* pOldF=dc.SelectObject(&ft2);
	m_nHeight = dc.GetOutputTextExtent("宋体").cy +3;
	dc.SelectObject (pOldF);
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaiKlineDialogCross::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

//	KillFocus();
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CTaiKlineDialogCross::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
	if(pView!=NULL)
		pView->SetFocus();
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CTaiKlineDialogCross::OnCancel()
{
	// TODO: Add extra validation here
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	if(pView!=NULL)
		pView->m_isShowCross=0;	
	ShowWindow(SW_HIDE);

	if(pView!=NULL)
		pView->RedrawWindow();
//	CDialog::OnCancel();
}


void CTaiKlineDialogCross::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CTaiKlineDialogCross::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(pView!=NULL)
		pView->m_isShowCross=0;	
	CRect r;
	if(pView!=NULL)
		pView->RedrawWindow();//GetClientRect(r);
//	if(pView->m_infoFiguer>0)
//		r.right-=m_rightLength;
//	pView->CopyRect(r);
	
	CDialog::OnClose();
}

void CTaiKlineDialogCross::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect r;
	if(pView!=NULL)
	{
//		pView->GetClientRect(r);
//		if(pView->m_infoFiguer>0)
//			r.right-=m_rightLength;
//		pView->CopyRect(r);
	}
	
	
	CDialog::OnMouseMove(nFlags, point);
}

void CTaiKlineDialogCross::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	m_bSaveBackground = true;
	RedrawWindow();
	
	// TODO: Add your message handler code here
	
}

BOOL CTaiKlineDialogCross::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;//CDialog::OnEraseBkgnd(pDC);
}

void CTaiKlineDialogCross::OnSetFocus(CWnd* pOldWnd) 
{
	CDialog::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
//	pView->SetFocus();
	
}

BOOL CTaiKlineDialogCross::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,6200);//CTaiKlineDialogCross::IDD);
//	return CDialog::OnHelpInfo(pHelpInfo);
	DoHtmlHelp(this);return TRUE;
}

void CTaiKlineDialogCross::SetColor(CDC *pDC, float fVal)
{
	if(fVal>0.0000001)
		pDC->SetTextColor( CMainFrame::m_pDoc->m_colorArray[13]);
	else if(fVal<-0.0000001)
		pDC->SetTextColor( CMainFrame::m_pDoc->m_colorArray[XIA_DIE_WORD]);
	else
		pDC->SetTextColor( CMainFrame::m_pDoc->m_colorArray[PING_PAN_WORD]);

}
