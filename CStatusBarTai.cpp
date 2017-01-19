// CStatusBar_sjl.cpp : implementation file
//

#include "stdafx.h"
#include "CStatusBarTai.h"
#include "resource.h"
#include "..\\atlmfc\\src\\mfc\\afximpl.h"
//#include "..\\mfc\\src\\afximpl.h"
#include "MainFrm.h"
#include "CAlertSystem.h"
#include "DlgMYXMZ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern AFX_DATA AUX_DATA afxData;

/////////////////////////////////////////////////////////////////////////////
// CStatusBar_sjl

CStatusBar_sjl::CStatusBar_sjl()
{
	int i;
	//bAlarm=false;
	m_pTicker = NULL;
	nCount=-1;
	m_fRedGreen_sh	=	0.5; //�Ϻ����̾�
	m_fRedGreen_sz	=	0.5;
	m_fRedGreen_cy	=	0.5;

	m_fIndex_sh	=	0;  //��ָ֤��
	m_fIndex_sz	=	0;
	m_fIndex_cy	=	0;

	m_fAD_sh	=	0;  //�Ϻ��ǵ�
	m_fAD_sz	=	0;
	m_fAD_cy	=	0;

	m_fVol_sh	=	0; //�Ϻ��ɽ���
	m_fVol_sz	=	0;
	m_fVol_cy	=	0;

	m_fAmount_sh	=	0;  //�Ϻ��ɽ���
	m_fAmount_sz	=	0;
	m_fAmount_cy	=	0;//��Ԫ

	for(i=0;i<4;i++)
		m_aRcvCount[i] =FALSE;//ʵʱ���顢����ʱ�������ߡ�����������

	for(i=0;i<7;i++)//for(i=0;i<6;i++)
		m_aPaneRect[i] = CRect(0,0,0,0);

	memset(m_BigBill.m_szStockName,NULL,10);
	memset(m_BigBill.m_szReason,NULL,10);
	m_BigBill.m_nAmount = 0;
	m_BigBill.m_fPrice = 0;

	m_bHasAlarm = FALSE;
	m_strAlarmReason = "";

	//GDI Object Initalizetion.
	m_ImageList.Create(IDB_STATUSBAR_INDICATOR,16,4,RGB(255,255,255));

}

CStatusBar_sjl::~CStatusBar_sjl()
{
	if(m_pTicker)
	{
		m_pTicker->DestroyWindow();
		m_pTicker=NULL;
	}

}

int CStatusBar_sjl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(100,1000,NULL);
	return 0;
}

BEGIN_MESSAGE_MAP(CStatusBar_sjl, CStatusBar)
	//{{AFX_MSG_MAP(CStatusBar_sjl)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStatusBar_sjl message handlers

HBRUSH CStatusBar_sjl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}


void CStatusBar_sjl::SetRedGreen_sh(float fRedGreen,BOOL bRepaint)
{
	m_fRedGreen_sh = fRedGreen;
	if (bRepaint) 
		InvalidateRect(&m_aPaneRect[2]);
}
void CStatusBar_sjl::SetRedGreen_cy(float fRedGreen,BOOL bRepaint)
{
	m_fRedGreen_cy = fRedGreen;
	if (bRepaint) 
		InvalidateRect(&m_aPaneRect[4]);
}

void CStatusBar_sjl::SetRedGreen_sz(float fRedGreen,BOOL bRepaint)
{
	m_fRedGreen_sz = fRedGreen;
	if (bRepaint) 
		InvalidateRect(&m_aPaneRect[3]);
}

void CStatusBar_sjl::SetIndex_sh(float fIndex,BOOL bRepaint)
{
	m_fIndex_sh = fIndex;
	if (bRepaint) Invalidate();

}
void CStatusBar_sjl::SetIndex_cy(float fIndex,BOOL bRepaint)
{
	m_fIndex_cy = fIndex;
	if (bRepaint) Invalidate();
}
void CStatusBar_sjl::SetIndex_sz(float fIndex,BOOL bRepaint)
{
	m_fIndex_sz = fIndex;
	if (bRepaint) Invalidate();
}

void CStatusBar_sjl::SetAD_sh(float fAD,BOOL bRepaint)
{
	m_fAD_sh = fAD;
	if (bRepaint) Invalidate();
}
void CStatusBar_sjl::SetAD_cy(float fAD,BOOL bRepaint)
{
	m_fAD_cy = fAD;
	if (bRepaint) Invalidate();
}
void CStatusBar_sjl::SetAD_sz(float fAD,BOOL bRepaint)
{
	m_fAD_sz = fAD;
	if (bRepaint) Invalidate();
}

void CStatusBar_sjl::SetVol_sh(float fVol,BOOL bRepaint)
{
	m_fVol_sh = fVol;
	if (bRepaint) Invalidate();
}
void CStatusBar_sjl::SetVol_cy(float fVol,BOOL bRepaint)
{
	m_fVol_cy = fVol;
	if (bRepaint) Invalidate();
}
void CStatusBar_sjl::SetVol_sz(float fVol,BOOL bRepaint)
{
	m_fVol_sz = fVol;
	if (bRepaint) Invalidate();
}

void CStatusBar_sjl::SetAmount_sh(float fAmount,BOOL bRepaint)
{
	m_fAmount_sh = fAmount;
	if (bRepaint) Invalidate();
}
void CStatusBar_sjl::SetAmount_cy(float fAmount,BOOL bRepaint)
{
	m_fAmount_cy = fAmount;
	if (bRepaint) Invalidate();
}
void CStatusBar_sjl::SetAmount_sz(float fAmount,BOOL bRepaint)
{
	m_fAmount_sz = fAmount;
	if (bRepaint) Invalidate();
}


void CStatusBar_sjl::SetAlarm(CString& strReason)
{
}

void CStatusBar_sjl::SetRcvCount(int nCount,int nType,BOOL bRepaint)
{
}

void CStatusBar_sjl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap m_Bitmap;
	m_Bitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	CBitmap* pOld = memDC.SelectObject(&m_Bitmap);
	memDC.FillSolidRect(&rect,CMainFrame::m_pDoc->m_colorArray [0]);

	CFont* oldfont = memDC.SelectObject(GetFont());
	memDC.SelectStockObject(LTGRAY_BRUSH);
	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(  CMainFrame::m_pDoc->m_colorArray [1]);
	//memDC.Draw3dRect(&rect,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));

//	DrawMenuIconPane(memDC,m_aPaneRect[0]);
	DrawShInfoPane  (memDC,m_aPaneRect[0]);
	DrawShRed  (memDC,m_aPaneRect[1]);
	DrawSzInfoPane  (memDC,m_aPaneRect[2]);
	DrawSzRed  (memDC,m_aPaneRect[3]);
	DrawBigBill     (memDC,m_aPaneRect[4]);

//	DrawMyXMZPane(memDC,m_aPaneRect[5]);//added by zhufeng
	{
		DrawAlarmPane   (memDC,m_aPaneRect[5]);//DrawAlarmPane   (memDC,m_aPaneRect[5]);
		DrawSignalPane  (memDC,m_aPaneRect[6]);//DrawSignalPane  (memDC,m_aPaneRect[6]);
	}
	DrawTime(memDC,m_aPaneRect[7]);

	memDC.SelectObject(oldfont);

	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
		&memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOld);
}

/*
	�˵�          20
	���ǹ̶��ģ�150
	�ź��ǹ̶��ģ� 45
	���������
	���Ϊ  255
	���ǹ̶��ģ�   20
	Ԥ���ǹ̶��ģ� 20
*/

void CStatusBar_sjl::OnSize(UINT nType, int cx, int cy) 
{
	int nTotalWidth = cx;
	if(nTotalWidth<300)
		nTotalWidth = 300;

    int nShszInfoWidth = (nTotalWidth - 280)/6;
	int  nBeginPos = 0;
 	m_aPaneRect[0] = CRect(nBeginPos,0,nBeginPos+nShszInfoWidth*2,cy); nBeginPos+=(nShszInfoWidth*2);
	m_aPaneRect[1] = CRect(nBeginPos,0,nBeginPos+nShszInfoWidth,cy); nBeginPos+=nShszInfoWidth;
	m_aPaneRect[2] = CRect(nBeginPos,0,nBeginPos+nShszInfoWidth*2,cy); nBeginPos+=(nShszInfoWidth*2);
	m_aPaneRect[3] = CRect(nBeginPos,0,nBeginPos+nShszInfoWidth,cy); nBeginPos+=nShszInfoWidth;
	m_aPaneRect[4] = CRect(nBeginPos,0,nBeginPos+175,cy); nBeginPos+=175;
	m_aPaneRect[5] = CRect(nBeginPos,0,nBeginPos+20,cy); nBeginPos+=20;
	//
	m_aPaneRect[6]=CRect(nBeginPos,0,nBeginPos+35,cy);nBeginPos+=35;
	m_aPaneRect[7] = CRect(nBeginPos,0,nBeginPos+65,cy); 

	CStatusBar::OnSize(nType, cx, cy);
	Invalidate();
}

void CStatusBar_sjl::DrawBigBill(CDC &dc, CRect rect)
{
	dc.Draw3dRect(&rect,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));
	rect.DeflateRect(1,1);
	if(m_pTicker)
	{
		return;
	}

	CString strInfo;
	strInfo.Format("%s %s %d��%.2fԪ",
		m_BigBill.m_szStockName,
		m_BigBill.m_szReason,
		m_BigBill.m_nAmount,
		m_BigBill.m_fPrice);

	dc.DrawText(strInfo,&rect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
}

void CStatusBar_sjl::DrawShInfoPane(CDC &dc, CRect rect)
{
	dc.Draw3dRect(&rect,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));
	rect.DeflateRect(1,1);

	CString strInfo;
	CString strPrice;
	CString strAmount;
	strPrice.Format ("%.2f  ",m_fIndex_sh);
	strAmount.Format ("%.1f��",m_fAmount_sh);
	COLORREF	clr = CMainFrame::m_pDoc ->m_colorArray [1];
	if( m_fAD_sh>0.0)
	{
		strInfo.Format("+%.2f",m_fAD_sh);
		clr = CMainFrame::m_pDoc ->m_colorArray [3];
	}
	else if( m_fAD_sh == 0.0)
	{
		strInfo = "0";
	}
	else
	{
		strInfo.Format("-%.2f",fabs(m_fAD_sh));
		clr = CMainFrame::m_pDoc ->m_colorArray [4];
	}
	CRect r = rect;
//	dc.DrawText (strAmount,&r,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

	int left=1+rect.left ;
	CString s = "��";

	r.left = left;
	dc.DrawText (s,&r,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	left+=(dc.GetOutputTextExtent(s)).cx;
	left++;
	dc.SetTextColor(clr);
	r.left = left;
	dc.DrawText (strPrice,&r,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	left+=(dc.GetOutputTextExtent(strPrice)).cx;
	left++;
	r.left = left;
	dc.DrawText (strInfo,&r,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	dc.SetTextColor(CMainFrame::m_pDoc ->m_colorArray [1]);
	left+=(dc.GetOutputTextExtent(strAmount)).cx;
	left+=5;
	r.left = left;
	dc.DrawText (strAmount,&r,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
}

void CStatusBar_sjl::DrawSzInfoPane(CDC &dc, CRect rect)
{
	dc.Draw3dRect(&rect,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));
	rect.DeflateRect(1,1);

/*	CString strInfo;
	if( m_fAD_sz>0.0)
		strInfo.Format("��%.2f +%.2f  %.1f��",m_fIndex_sz,m_fAD_sz,//m_fVol_sz,
		m_fAmount_sz);
	else if( m_fAD_sz == 0.0)
	{
		strInfo.Format("��%.2f 0  %.1f��",m_fIndex_sz,
		m_fAmount_sz);
	}
	else
	{
		strInfo.Format("��%.2f -%.2f  %.1f��",m_fIndex_sz,fabs(m_fAD_sz),//m_fVol_sz,
		m_fAmount_sz);
	}
	dc.DrawText(strInfo,&rect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);*/
	CString strInfo;
	CString strPrice;
	CString strAmount;
	strPrice.Format ("%.2f  ",m_fIndex_sz);
	strAmount.Format ("%.1f��",m_fAmount_sz);
	COLORREF	clr = CMainFrame::m_pDoc ->m_colorArray [1];
	if( m_fAD_sz>0.0)
	{
		strInfo.Format("+%.2f",m_fAD_sz);
		clr = CMainFrame::m_pDoc ->m_colorArray [3];
	}
	else if( m_fAD_sz == 0.0)
	{
		strInfo = "0";
	}
	else
	{
		strInfo.Format("-%.2f",fabs(m_fAD_sz));
		clr = CMainFrame::m_pDoc ->m_colorArray [4];
	}
	CRect r = rect;
//	dc.DrawText (strAmount,&r,DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

	int left=1+rect.left ;
	CString s = "��";

	r.left = left;
	dc.DrawText (s,&r,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	left+=(dc.GetOutputTextExtent(s)).cx;
	left++;
	dc.SetTextColor(clr);
	r.left = left;
	dc.DrawText (strPrice,&r,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	left+=(dc.GetOutputTextExtent(strPrice)).cx;
	left++;
	r.left = left;
	dc.DrawText (strInfo,&r,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	dc.SetTextColor(CMainFrame::m_pDoc ->m_colorArray [1]);
	left+=(dc.GetOutputTextExtent(strAmount)).cx;
	left+=5;
	r.left = left;
	dc.DrawText (strAmount,&r,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

}

void CStatusBar_sjl::DrawAlarmPane(CDC &dc, CRect rect)
{
	CMainFrame* pFm = (CMainFrame* )AfxGetMainWnd();
	dc.Draw3dRect(&rect,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));
	rect.DeflateRect(1,1);

	rect.DeflateRect((rect.Width()-16)/2,(rect.Height()-16)/2);
	if(pFm->m_bAlarmExec==false)
	    m_ImageList.Draw(&dc,1,rect.TopLeft(),ILD_TRANSPARENT);
    else 
		m_ImageList.Draw(&dc,0,rect.TopLeft(),ILD_TRANSPARENT);
}

void CStatusBar_sjl::DrawCardPane(CDC &dc, CRect rect)
{
	dc.Draw3dRect(&rect,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));
	rect.DeflateRect(1,1);

	rect.DeflateRect((rect.Width()-16)/2,(rect.Height()-16)/2);
	m_ImageList.Draw(&dc,1,rect.TopLeft(),ILD_TRANSPARENT);
}

void CStatusBar_sjl::DrawSignalPane(CDC &dc, CRect rect)
{
	dc.Draw3dRect(&rect,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));
	rect.DeflateRect(1,1);

	COLORREF ShowColor;
	CRect rectRole = rect;
	int nHeight = rect.Width()/6;
	
	for(int i=0;i<=nCount;i++)
	{
		/*rectRole.left = rect.left+i*2*5+5;
		//rectRole.left = rect.left+5;
		rectRole.right = rectRole.left+5;
		rectRole.top = (5-m_aRcvCount[i])*nHeight;
		//rectRole.top = 5*nHeight;
		//rectRole.top = m_aRcvCount[i]%nHeight;
		dc.FillSolidRect(&rectRole,RGB(255,0,0));*/
		/////////
		/*if(m_aRcvCount[0]==TRUE)
		{
			m_aRcvCount[0]=FALSE;
			ShowColor=RGB(255,0,0);
		}
		if(m_aRcvCount[1]==TRUE)
		
		{
			m_aRcvCount[1]=FALSE;

			ShowColor=RGB(0,0,255);
		}
		if(m_aRcvCount[2]==TRUE)
		{
			m_aRcvCount[2]=FALSE;
			ShowColor=RGB(155,155,0);
		}
		if(m_aRcvCount[3]==TRUE)
		{
			m_aRcvCount[3]=FALSE;
			ShowColor=RGB(55,128,0);
		}*/
		//Sleep(1000);
		ShowColor=bColor[i];
		rectRole.left = rect.left+i*nHeight+2;
		//rectRole.right = rect.right-3;
		//rectRole.left=rectRole.left+m_aRcvCount[0]*nHeight;
		rectRole.top = 3;
		rectRole.bottom=15;
		//rectRole.top = m_aRcvCount[i]%nHeight;
		rectRole.right=rectRole.left+5;
		dc.FillSolidRect(&rectRole,ShowColor);
		
	
	}
	if(nCount==-1)
		nCount++;
}

void CStatusBar_sjl::SetBigBill(char *strStockName, char* strReason, int nAmount, float fPrice,BOOL bRepaint)
{
	strcpy(m_BigBill.m_szStockName,strStockName);
	strcpy(m_BigBill.m_szReason,strReason);
	m_BigBill.m_nAmount = nAmount;
	m_BigBill.m_fPrice = fPrice;
	if(bRepaint)
	   InvalidateRect(&m_aPaneRect[1]);
}

void CStatusBar_sjl::OnRButtonDown(UINT nFlags, CPoint point) 
{
/*	if(m_aPaneRect[5].PtInRect(point))
	{
		int x=m_aPaneRect[5].left+50;
		int y=m_aPaneRect[5].top;
		CPoint ptOrg(x,y);
		ClientToScreen(&ptOrg);
		//m_Menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_BOTTOMALIGN,ptOrg.x,ptOrg.y,this);
		CMenu MyMenu;
		MyMenu.LoadMenu(IDR_STATUS);
		CMenu *pMenu=MyMenu.GetSubMenu(0);
	    //MyMenu.CreatePopupMenu();
		//for(int j=0;j<2;j++)
		//   pMenu->CheckMenuItem (j,MF_BYPOSITION|MF_CHECKED );
		(pMenu->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON  ,
			ptOrg.x,ptOrg.y,this, NULL));
	//	RedrawWindowNoCacl();

	}	*/
	CStatusBar::OnRButtonDown(nFlags, point);
}

void CStatusBar_sjl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CMainFrame* pFm = (CMainFrame* )AfxGetMainWnd();
	if(m_aPaneRect[0].PtInRect(point) ||m_aPaneRect[1].PtInRect(point))
	{
	    pFm->SendMessage (WM_COMMAND,ID_F3);//
	}
	if(m_aPaneRect[2].PtInRect(point) ||m_aPaneRect[3].PtInRect(point))
	{
	    pFm->SendMessage (WM_COMMAND,ID_F4);//
	}
	if(m_aPaneRect[4].PtInRect(point))
	{
	    pFm->SendMessage (WM_COMMAND,ID_STOCK_BIG);//
	}
	if(m_aPaneRect[5].PtInRect(point) )
	{
		CAlertSystem::CreateNew();
/*		bool bAla = pFm->m_bAlarmExec;
		CAlertSystem::SwitchAlarm();
		if(bAla==false)
		{
			if(pFm->m_bAlarmExec==false)
				CAlertSystem::CreateNew();
		}*/
		InvalidateRect(&m_aPaneRect[5]);  
	}
	if(m_aPaneRect[6].PtInRect(point))//if(m_aPaneRect[6].PtInRect(point))
	{
		CDialogMYXMZ dlgxmz=new CDialogMYXMZ;
		dlgxmz.DoModal();
		delete dlgxmz;
	}

	CStatusBar::OnLButtonDblClk(nFlags, point);
}

void CStatusBar_sjl::OnLButtonDown(UINT nFlags, CPoint point) 
{

	CStatusBar::OnLButtonDown(nFlags, point);
}

void CStatusBar_sjl::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
//	AfxMessageBox("Right Button dbl Click");	
	CStatusBar::OnRButtonDblClk(nFlags, point);
}

void CStatusBar_sjl::OnTimer(UINT nIDEvent) 
{
/*	CClientDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);

	CFont* oldfont = dc.SelectObject(GetFont());

	dc.SetBkMode(TRANSPARENT);
	dc.FillSolidRect(&m_aPaneRect[5],RGB(192,192,192));
	DrawAlarmPane   (dc,m_aPaneRect[5]);//DrawAlarmPane   (memDC,m_aPaneRect[5]);
	dc.FillSolidRect(&m_aPaneRect[7],RGB(192,192,192));
	DrawTime(dc,m_aPaneRect[7]);

	dc.SelectObject(oldfont);*/
	RedrawWindow();
	CStatusBar::OnTimer(nIDEvent);
}

void CStatusBar_sjl::DrawMenuIconPane(CDC &dc, CRect rect)
{
	rect.DeflateRect(1,1);
	rect.DeflateRect((rect.Width()-16)/2,(rect.Height()-16)/2);
	m_ImageList.Draw(&dc,2,rect.TopLeft(),ILD_TRANSPARENT);
}

void CStatusBar_sjl::PaintRect(BOOL bRepaint)
{
    if(bRepaint)
		InvalidateRect(&m_aPaneRect[6]);
}

void CStatusBar_sjl::DrawCyInfoPane(CDC &dc, CRect rect)
{
/*	dc.Draw3dRect(&rect,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));
    rect.DeflateRect(1,1);
	
	CRect rectRed,rectGreen;
	rectRed=rect;
	rectRed.DeflateRect(0,1);
	rectGreen=rectRed;
	int nRedWidth=int(rectRed.Width()*m_fRedGreen_cy);
	rectRed.right=rectRed.left+nRedWidth;
	rectGreen.left=rectRed.right;
	dc.FillSolidRect(&rectRed,RedRGB);
	dc.FillSolidRect(&rectGreen,GreenRGB);

	CString strInfo;
	if( m_fAD_cy>0.0)
		strInfo.Format("��ָ%.2f ��%.1f�� %.1f��Ԫ",m_fIndex_cy,m_fAD_cy,//m_fVol_cy,
		m_fAmount_cy);
	else
	{
		strInfo.Format("��ָ%.2f ��%.1f�� %.1f��Ԫ",m_fIndex_cy,fabs(m_fAD_cy),//m_fVol_cy,
		m_fAmount_cy);
	}
	dc.DrawText(strInfo,&rect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
*/
}

void CStatusBar_sjl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
//	int i=LOWORD(wParam);
//	if(i==ID_TJXG)
//		AfxMessageBox("you select ");
	
}


void CStatusBar_sjl::DrawMyXMZPane(CDC &dc, CRect rect)
{
	CMainFrame* pFm = (CMainFrame* )AfxGetMainWnd();
	dc.Draw3dRect(&rect,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));
	rect.DeflateRect(1,1);

	rect.DeflateRect((rect.Width()-16)/2,(rect.Height()-16)/2);
	//if(pFm->m_bAlarmExec==false)
	m_ImageList.Draw(&dc,3,rect.TopLeft(),ILD_TRANSPARENT);
    //else 
}

void CStatusBar_sjl::SetRollText(char *ch)
{
	if(m_pTicker)
	{
		CString s = ch;
		m_pTicker->SetTickerText(s);
	}

}
void CStatusBar_sjl::DrawShRed(CDC &dc, CRect rect)
{
	dc.Draw3dRect(&rect,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));
	rect.DeflateRect(1,1);

	CRect rectRed,rectGreen;
	rectRed=rect;
	rectRed.DeflateRect(0,1);
	rectGreen=rectRed;
	int nRedWidth=int(rectRed.Width()*m_fRedGreen_sh);
	rectRed.right=rectRed.left+nRedWidth;
	rectGreen.left=rectRed.right;
	dc.FillSolidRect(&rectRed,CMainFrame::m_pDoc ->m_colorArray [3]);
	dc.FillSolidRect(&rectGreen,CMainFrame::m_pDoc ->m_colorArray [4]);

}

void CStatusBar_sjl::DrawSzRed(CDC &dc, CRect rect)
{
	dc.Draw3dRect(&rect,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));
	rect.DeflateRect(1,1);

	CRect rectRed,rectGreen;
	rectRed=rect;
	rectRed.DeflateRect(0,1);
	rectGreen=rectRed;
	int nRedWidth=int(rectRed.Width()*m_fRedGreen_sz);
	rectRed.right=rectRed.left+nRedWidth;
	rectGreen.left=rectRed.right;
	dc.FillSolidRect(&rectRed,CMainFrame::m_pDoc ->m_colorArray [3]);
	dc.FillSolidRect(&rectGreen,CMainFrame::m_pDoc ->m_colorArray [4]);
	

}

void CStatusBar_sjl::DrawTime(CDC &dc, CRect rect)
{
	dc.Draw3dRect(&rect,GetSysColor(COLOR_BTNSHADOW),GetSysColor(COLOR_BTNHILIGHT));
    rect.DeflateRect(1,1);
	
	CString strInfo;
	strInfo = CTime::GetCurrentTime ().Format ("%H:%M:%S");
	dc.DrawText(strInfo,&rect,DT_LEFT|DT_VCENTER|DT_SINGLELINE);

}

BOOL CStatusBar_sjl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
}
