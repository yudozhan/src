// CTaiKlineDrawing.cpp: implementation of the CTaiKlineDrawing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineDrawLine.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiShanDoc.h"
#include "CTaiKlineDoKline.h"
#include "CAlertSystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define LEN_BITMAP  30
#define HEAD_STOCK_NUM  2048
#define HEAD_BIG  8
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaiKlineDrawing::CTaiKlineDrawing()
{
//pDoc->m_propertyInitiate.bHideLine == TRUE && ((CMainFrame*)AfxGetMainWnd())->m_drawLineToolBar->IsWindowVisible())
}
CTaiKlineDrawing::CTaiKlineDrawing(CTaiShanKlineShowView* bsView,CTaiShanDoc* bsDoc)
{
	// TODO: add construction code here
	m_nCurrentLine = 0;
	m_nCurrentLineClicked = 0;
	m_bSelected=false;
	m_nZhongXianDrawing=0;
	pView = bsView;
	pDoc=bsDoc;
	m_nPointCurrent=0;
	m_bDrawLine=true;
	m_bLineValidate=true;
	m_lineType=-1;
	m_lineTypeOld=-1;
	m_nZhongXian=0;
	//
	m_nPntGroup[0]=2;
	m_nPntGroup[1]=1;
	m_nPntGroup[2]=2;
	m_nPntGroup[3]=2;
	m_nPntGroup[4]=2;

	m_nPntGroup[5]=2;
	m_nPntGroup[6]=2;
	m_nPntGroup[7]=2;
	m_nPntGroup[8]=2;
	m_nPntGroup[9]=2;

	m_nPntGroup[10]=2;
	m_nPntGroup[11]=2;
	m_nPntGroup[12]=2;
	m_nPntGroup[13]=2;
	m_nPntGroup[14]=2;

	m_nPntGroup[15]=3;
	m_nPntGroup[16]=2;
	m_nPntGroup[17]=2;
	m_nPntGroup[18]=1;
	m_nPntGroup[19]=1;

	m_nPntGroup[20]=2;
	m_nPntGroup[21]=1;
	m_nPntGroup[22]=1;//DELETE
	m_nPntGroup[23]=3;//
//SPIRE_LINE
	m_nPntGroup[24]=2;//
	m_nLine=0;

	
	m_fileSh=new CFile();
	CString fileName="data\\sh\\selfLINE.DAT";
	if(!m_fileSh->Open((LPCTSTR)fileName, CFile::modeNoTruncate|
		CFile::shareDenyNone|CFile::modeReadWrite|CFile::modeCreate,NULL))
	{
		DWORD rtn;
		rtn=GetLastError();
		delete m_fileSh;
		m_fileSh=NULL;
	}
	else
	{
		int cfilelength =m_fileSh->GetLength();

		if(cfilelength==0)
		{
			//写头
			m_fileSh->SeekToBegin();
			int hd=6911;
			m_fileSh->Write (&hd,4);
			hd=0;//股票的个数
			m_fileSh->Write (&hd,4);
			char* pch=new char[12* HEAD_STOCK_NUM];
			memset(pch,0,12* HEAD_STOCK_NUM);
			m_fileSh->Write (pch,12* HEAD_STOCK_NUM);
			delete[] pch;
			m_nPosStock=-1;
//			m_fileSh->Close ();

		}
	}

	m_fileSz=new CFile();

	fileName="data\\sz\\selfLINE.DAT";
	if(!m_fileSz->Open((LPCTSTR)fileName, CFile::modeNoTruncate|
		CFile::shareDenyNone|CFile::modeReadWrite|CFile::modeCreate,NULL))
	{
		DWORD rtn;
		rtn=GetLastError();
		delete m_fileSz;
		m_fileSz=NULL;
	}
	else
	{
		int cfilelength = m_fileSz->GetLength();

		if(cfilelength==0)
		{
			//写头
			m_fileSz->SeekToBegin();
			int hd=6911;
			m_fileSz->Write (&hd,4);
			hd=0;//股票的个数
			m_fileSz->Write (&hd,4);
			char* pch=new char[12* HEAD_STOCK_NUM];
			memset(pch,0,12* HEAD_STOCK_NUM);
			m_fileSz->Write (pch,12* HEAD_STOCK_NUM);
			delete[] pch;
			m_nPosStock=-1;
//			m_fileSz->Close ();

		}
	}



	ReadLineself();
		// 将设备和位图产生关联
//	m_memdc.SelectObject( &m_bit );


}
CTaiKlineDrawing::~CTaiKlineDrawing()
{
//	WriteLineself();
	m_info_lineself.RemoveAll ();

	if(m_fileSh!=NULL)
	{
		m_fileSh->Close();
		delete m_fileSh;
	}
	if(m_fileSz!=NULL)
	{
		m_fileSz->Close();
		delete m_fileSz;
	}
}/*
#define XIAN_DUAN                   0
#define SHUI_PING                   1
#define QU_SHI                      2
#define PING_XING					3
#define SU_DU		                4
#define SHAN_XING                   5
#define GAN_SHI                     6
#define HUANG_JIN					7
#define BO_DUAN		                8
#define BAI_FEN_BI                  9
#define HUI_GUI                     10
#define HUI_GUI_DAI_1               11
#define HUI_GUI_DAI_2		        12
#define ZHOU_QI						13
#define NA_QI                       14
#define ZHONG_XIAN					15
#define NA_QI_HU		            16
#define HUANG_JIN_HU                17
#define UP_ARROW                    18
#define DOWN_ARROW					19
#define RECT_KIND		            20
#define TEXT_KIND		            21
#define DELETE_LINE		            22
*/
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawXianDuan(CDC *pDC)
//      功能说明：画线段
//      入口参数：pDC为传入的设备描述表指针
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
/*
void CTaiKlineDrawing::DrawXianDuan(CDC *pDC)
{
	CRgn rgn;
	rgn.CreateRectRgn (m_rtDrawLine.left,m_rtDrawLine.top,m_rtDrawLine.right,m_rtDrawLine.bottom );

	pDC->SelectClipRgn (&rgn);
	pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
	pDC->LineTo(m_pt[1].x ,m_pt[1].y);

	rgn.DeleteObject ();

}*/
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawXianDuan(CDC *pDC,CPoint& p1,CPoint& p2)
//      功能说明：画线段
//      入口参数：pDC为传入的设备描述表指针，p1、p2为传入的两点
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDrawing::DrawXianDuan(CDC *pDC,CPoint& p1,CPoint& p2)
{
	if(p1.x==p2.x&&p1.y==p2.y)
		return;
	pDC->MoveTo(p1.x ,p1.y);
	pDC->LineTo(p2.x ,p2.y);
}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawXianDuanVirtul(CDC *pDC,CPoint& p1,CPoint& p2)
//      功能说明：画线段
//      入口参数：pDC为传入的设备描述表指针，p1、p2为传入的两点
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDrawing::DrawXianDuanVirtul(CDC *pDC,CPoint& p1,CPoint& p2,COLORREF* clr)
{
	if(p1.x==p2.x&&p1.y==p2.y)
		return;
	int width = 1;
	CPen pen_virtul(PS_DOT  ,width,*clr);
	CPen* pOldpen=pDC->SelectObject(&pen_virtul);
	pDC->MoveTo(p1.x ,p1.y);
	pDC->LineTo(p2.x ,p2.y);
	pDC->SelectObject(pOldpen);
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawPointBE(CDC *pDC)
//      功能说明：画线段
//      入口参数：pDC为传入的设备描述表指针
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDrawing::DrawPointBE(CDC *pDC)
{
	SetDrawLineRect();

	int oldROP = pDC->SetROP2(R2_NOT  );//m_nCurrentLine
	if(m_nCurrentLineClicked>=0)
	{
/*#define UP_ARROW                    18
#define 					19
#define RECT_KIND		            20
#define 		            21
*/		int type=m_info_lineself[ m_nCurrentLineClicked ].type;//?
		if(type<0)
			return;

		if(type==UP_ARROW||type==DOWN_ARROW||type==TEXT_KIND)
		{
			if(type==TEXT_KIND)
			{
				CFont ft;
				LOGFONT lgf=pDoc->m_fontstr[0];
				ft.CreateFontIndirect (&lgf );
				CFont* pOld=pDC->SelectObject(&ft);

		//		pDC->TextOut (m_pt[0].x,m_pt[0].y,m_textNow);

				m_lenTextNow=(pDC->GetOutputTextExtent(m_textNow)).cx;
				int y =(pDC->GetOutputTextExtent(m_textNow)).cy;
				CRect r3(m_pt[0].x-1,m_pt[0].y-1,m_pt[0].x+m_lenTextNow+2,m_pt[0].y+y+2);

				pDC->DrawFocusRect(r3);
				pDC->SelectObject(pOld);
			}
			else
			{
				CRect r4(m_pt[0].x-13,m_pt[0].y-1,m_pt[0].x+13,m_pt[0].y+25);
				pDC->DrawFocusRect(r4);
			}
		}
		else
		{
			for(int i=0;i<m_nPntGroup[type];i++)
			{
				SIZE sz;
				sz.cx=2;
				sz.cy=2;
				CRect r(m_ptOld[i].x-3,m_ptOld[i].y-3,m_ptOld[i].x+3,m_ptOld[i].y+3);
				CRect r2(m_pt[i].x-3,m_pt[i].y-3,m_pt[i].x+3,m_pt[i].y+3);
				DrawRect(pDC,r2);
			}
		}
	}
//	m_isFirst=1;
	pDC->SetROP2(oldROP);

}


/////////////////////////////////////////////////////////////////////////////
//      函 数 名：ReadLineself()
//      功能说明：把文件中的画线数据读到内存中
//      入口参数：无
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineDrawing::ReadLineself()
{
//	int dw=GetTickCount();
	CFile* pFile;
	if(CSharesCompute::GetMarketKind(pView->m_stkKind) == SH_MARKET_EX)
	{
		pFile=m_fileSh;
		if(m_fileSh==NULL)
			return 0;
	}
	else
	{
		pFile=m_fileSz;
		if(m_fileSz==NULL)
			return 0;
	}

	m_nShares=0;
	pFile->Seek(4,CFile::begin);
	pFile->Read(&(m_nShares) ,4) ;

	CString sStk;
	int flag=0;
	m_nPosStock=-1;
	m_nLine=0;
	//股票太多是不可能的
	if(m_nShares>20480)
		return 0;
	if(m_nShares<0)
		return 0;
	
	pFile->Seek(8,CFile::begin);
	int i;
	for(i=0;i<m_nShares;i++)
	{
	//	int hj=sizeof(KLINE_LITTLEHEADER);
		int nLine;
		char  name[8];
		memset(name,0,8);
		pFile->Read(name,8) ;
		pFile->Read(&nLine,4) ;
		if(nLine==0)
			m_nPosStock=i;//m_nLine

		CString s(name);
		if(s==pView->m_sharesSymbol )
		{
			flag=1;
			m_nPosStock=i;//m_nLine
			pFile->Seek(8+i*12+8,CFile::begin   );
			pFile->Read(&m_nLine,4) ;
			break;
		}
	}

	//确定放数据的位置
	if(flag==0&&m_nPosStock>=0)
	{
		char  name[8];
		strcpy(name,pView->m_sharesSymbol );
		pFile->Seek(8+m_nPosStock*12,CFile::begin   );
		pFile->Write(name,8) ;
		int i=0;
		pFile->Write(&i,4) ;
		m_nLine=0;
	}
	else if(flag==0&&m_nPosStock<=-1)
	{
		char  name[48*48];
		memset(name,0,48*48);
		strcpy(name,pView->m_sharesSymbol );
		pFile->Seek(8+m_nShares*12,CFile::begin   );
		pFile->Write(name,8) ;
		int i=0;
		pFile->Write(&i,4) ;
		memset(name,0,48*48);
		pFile->Seek(8+m_nPosStock*48*48+12*HEAD_STOCK_NUM,CFile::begin   );
		pFile->Write(name,48*48) ;
		m_nPosStock=m_nShares;
		m_nShares++;
		m_nLine=0;
	}

	m_info_lineself.RemoveAll ();
	INFO_LINESELF lineslf;

	//--		读取数据
	pFile->Seek(8+m_nPosStock*48*48+HEAD_STOCK_NUM*12,CFile::begin   );

	if(m_nLine>48)
		m_nLine=0;

	for(i=0;i<m_nLine;i++)
	{
		pFile->Read(&lineslf,48) ;
		m_info_lineself.Add (lineslf);
	}
//	sStk.Format("m_nLine=%d",(int)m_nLine);
//	AfxMessageBox(sStk);
//	pFile->Close ();
	return m_nLine;
}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：WriteLineself()
//      功能说明：把画线数据写到文件中
//      入口参数：无
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineDrawing::WriteLineself()
{
	m_nLine=m_info_lineself.GetSize ();
	if(m_nLine<0||m_nPosStock<=-1||m_nPosStock>=4096)
		return 0;

	CFile* pFile;
	if(CSharesCompute::GetMarketKind(pView->m_stkKind) == SH_MARKET_EX)
	{
		pFile=m_fileSh;
		if(m_fileSh==NULL)
			return 0;
	}
	else
	{
		pFile=m_fileSz;
		if(m_fileSz==NULL)
			return 0;
	}


	pFile->Seek(4,CFile::begin   );
	pFile->Write(&(m_nShares) ,4) ;

	char clr[32];
	memset(clr,0,32);
	strcpy(clr,pView->m_sharesSymbol);
	pFile->Seek(8+m_nPosStock*12,CFile::begin   );
	pFile->Write(clr ,8) ;

//	pFile->Seek(8+m_nPosStock*12+8,CFile::begin   );
//	m_nLine=m_info_lineself.GetSize ();
	pFile->Write(&(m_nLine) ,4) ;

	//--		写画线数据
	pFile->Seek(8+m_nPosStock*48*48+12*HEAD_STOCK_NUM,CFile::begin   );
	INFO_LINESELF lineslf;

	if(m_nLine>48 || m_nLine!=m_info_lineself.GetSize())
		return 0;

	for(int i=0;i<m_nLine;i++)
	{
		lineslf=m_info_lineself.GetAt (i);
		pFile->Write(&lineslf,48) ;
	}
//	pFile->Close ();
	return m_nLine;
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：SetPointPos(CPoint &point
//      功能说明：确定画线的端点
//      入口参数：输入点
//      出口参数：无出口数
//      返回参数：返回成功与否
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineDrawing::SetPointPos(CPoint &point)
{
	SetDrawLineRect();

	int foot=(int)(point.x /m_widthPer);
	if(foot+pView->pKlineDrawing ->m_footBegin>pView->pKlineDrawing ->m_footEnd)
		return -1;

	if(point.y<pView->m_heightCaption)
		point.y=pView->m_heightCaption;
	if(point.y>pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-3)
		point.y=pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-3;

	pView->pKlineDrawing ->SetRectCurrent(0);
	if(m_lineType<HUI_GUI || m_lineType>HUI_GUI_DAI_2)
	{
		m_pt[m_nPointCurrent].x=(int)((foot+0.5)*m_widthPer);

		//动态吸附
		if(foot+pView->pKlineDrawing ->m_footBegin <= pView->pKlineDrawing ->m_footEnd)
		{
			float out;
			int y = FloatToY(pView->m_pkline [foot+pView->pKlineDrawing ->m_footBegin].close, out,0);
			if(abs(y-point.y)<3)
			{
				m_pt[m_nPointCurrent].y=y;
				return m_nPointCurrent;
			}
			y = FloatToY(pView->m_pkline [foot+pView->pKlineDrawing ->m_footBegin].high, out,0);
			if(abs(y-point.y)<3)
			{
				m_pt[m_nPointCurrent].y=y;
				return m_nPointCurrent;
			}
			y = FloatToY(pView->m_pkline [foot+pView->pKlineDrawing ->m_footBegin].low, out,0);
			if(abs(y-point.y)<3)
			{
				m_pt[m_nPointCurrent].y=y;
				return m_nPointCurrent;
			}
			y = FloatToY(pView->m_pkline [foot+pView->pKlineDrawing ->m_footBegin].open, out,0);
			if(abs(y-point.y)<3)
			{
				m_pt[m_nPointCurrent].y=y;
				return m_nPointCurrent;
			}
			m_pt[m_nPointCurrent].y=point.y;
		}
		else
			m_pt[m_nPointCurrent].y=pView->m_heightCaption;
	}
	else//根据一元回归分析的最小二乘法求N根日K线的线性回归。
	{
		m_pt[m_nPointCurrent].x=(int)((foot+0.5)*m_widthPer);
		m_pt[m_nPointCurrent].y=point.y;
		int footOld=(int)((m_pt[0].x)/m_widthPer);
		int foot2=(int)((m_pt[1].x)/m_widthPer);
//		TRACE("huigui_after     %d\n",(int)(m_pt[1].x/m_widthPer));

		SetPointHuiGui(foot2,footOld);

	}


	return m_nPointCurrent;
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：SetDrawLineRect()
//      功能说明：初始化画线矩形
//      入口参数：无
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDrawing::SetDrawLineRect()
{
	m_rtDrawLine.top=pView->m_heightCaption;
	m_rtDrawLine.left=0;
	m_rtDrawLine.bottom=pView->m_rtKlineFiguer .rtBlw[0].m_yBottom-3;
	m_rtDrawLine.right=pView->m_rtKlineFiguer .rightX;

	if((float)pView->pKlineDrawing ->m_klinNumDefault!=0)
		m_widthPer=m_rtDrawLine.right/(float)pView->pKlineDrawing ->m_klinNumDefault;
	else
		m_widthPer=m_rtDrawLine.right;
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：SetPointMoved(int cx, int cy)
//      功能说明：自画线移动时，设置起始点
//      入口参数：cx、cy为宽度和高度
//      出口参数：无出口数
//      返回参数：返回成功与否
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineDrawing::SetPointMoved(int cx, int cy)
{

	SetDrawLineRect();

	int foot=(int)(cx /m_widthPer);
	int foot2=foot+(int)(m_ptOld[0].x/m_widthPer+0.5);
	if(foot2+pView->pKlineDrawing ->m_footBegin>pView->pKlineDrawing ->m_footEnd)
		return -1;
	if(foot2+pView->pKlineDrawing ->m_footBegin<0)
		return -1;

	if(m_lineType<HUI_GUI || m_lineType>HUI_GUI_DAI_2)
	{
		for(int i=0;i<m_nPntGroup[m_lineType];i++)
		{
			m_pt[i].x=m_ptOld[i].x+(int)((foot)*m_widthPer+0.5);
			m_pt[i].y=m_ptOld[i].y+cy;
		}
	}
	else//根据一元回归分析的最小二乘法求N根日K线的线性回归。
	{
		if((int)(m_ptOld[1].x/m_widthPer+foot)>pView->pKlineDrawing ->m_footEnd-pView->pKlineDrawing ->m_footBegin)
			return -1;

		for(int i=0;i<m_nPntGroup[m_lineType];i++)
		{
			m_pt[i].x=m_ptOld[i].x+(int)((foot)*m_widthPer+0.5);
		}

		int footOld=(int)((m_pt[0].x)/m_widthPer);
		int foot2=(int)((m_pt[1].x)/m_widthPer);
		SetPointHuiGui(foot2,footOld);

	}
	return 1;

}
/*typedef struct{
	int 	type;//线的类型
	int     nPt;//共有几个点
	int     klineType;//K线类型
	int		time[3];//各个点的时间值
	float	valFir[3];//各个点的浮点数值
	int     stored[3];//预留
}INFO_LINESELF;

*/
static CPoint ptLineSide[2];//
static CPoint ptIn[2];//
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：IsOnSelfLine(CPoint &point)
//      功能说明：输入点是否在自画线上
//      入口参数：point为输入点
//      出口参数：无出口数
//      返回参数：返回成功与否
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineDrawing::IsOnSelfLine(CPoint &point)
{
	int i=0;
	int rtn=0,j1,j2;
	CRect r;
	float gap;
	switch(m_lineType)
	{
	case SPIRE_LINE:
	case XIAN_DUAN:
		rtn=IsOnLine(m_pt[0], m_pt[1], point,0);
		break;
	case		SHUI_PING: 
		m_pt[1].x=m_pt[0].x+2;
		m_pt[1].y=m_pt[0].y;
		rtn=IsOnLine(m_pt[0], m_pt[1], point,0);
		break;
	case		QU_SHI   :   
		rtn=IsOnLine(m_pt[0], m_pt[1], point,2);
		break;
	case		PING_XING:
		rtn=IsOnLine(m_pt[0], m_pt[1], point,0);
		break;
	case		SU_DU:
		rtn=IsOnLine(m_pt[0], m_pt[1], point,1);
		if(rtn>0)
			break;
		if(m_pt[0].x>=m_pt[1].x)
			break;

		ptIn[0]=m_pt[0];
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=m_pt[1].y-(m_pt[1].y-m_pt[0].y)/3;
		rtn=IsOnLine(ptIn[0], ptIn[1], point,1);
		if(rtn>0)
			break;

		ptIn[0]=m_pt[0];
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=m_pt[1].y-(m_pt[1].y-m_pt[0].y)*2/3;
		rtn=IsOnLine(ptIn[0], ptIn[1], point,1);
		break;
	case		SHAN_XING:       
		rtn=IsOnLine(m_pt[0], m_pt[1], point,1);
		if(rtn>0)
			break;
		if(m_pt[0].x>=m_pt[1].x)
			break;

		ptIn[0]=m_pt[0];
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=(int)(m_pt[1].y-(m_pt[1].y-m_pt[0].y)*0.318);
		rtn=IsOnLine(ptIn[0], ptIn[1], point,1);
		if(rtn>0)
			break;

		ptIn[0]=m_pt[0];
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=m_pt[1].y-(m_pt[1].y-m_pt[0].y)/2;
		rtn=IsOnLine(ptIn[0], ptIn[1], point,1);
		if(rtn>0)
			break;

		ptIn[0]=m_pt[0];
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=(int)(m_pt[1].y-(m_pt[1].y-m_pt[0].y)*0.618);
		rtn=IsOnLine(ptIn[0], ptIn[1], point,1);
		break;
	case		HUANG_JIN_HU: //(下甘氏)  
//		rtn=IsOnLine(m_pt[0], m_pt[1], point,0);
//		break;
	case		GAN_SHI:         
//		if(m_pt[0].x>=m_pt[1].x)
//			break;
		rtn=IsOnLine(m_pt[0], m_pt[1], point,1);
		if(rtn>0)
			break;
		if(m_pt[0].x>=m_pt[1].x)
			break;

		ptIn[0]=m_pt[0];
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=m_pt[1].y-(m_pt[1].y-m_pt[0].y)/2;
		rtn=IsOnLine(ptIn[0], ptIn[1], point,1);
		if(rtn>0)
			break;

		ptIn[0]=m_pt[0];
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=m_pt[1].y-(m_pt[1].y-m_pt[0].y)*2/3;
		rtn=IsOnLine(ptIn[0], ptIn[1], point,1);
		if(rtn>0)
			break;

		ptIn[0]=m_pt[0];
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=m_pt[1].y-(m_pt[1].y-m_pt[0].y)*3/4;
		rtn=IsOnLine(ptIn[0], ptIn[1], point,1);
		if(rtn>0)
			break;

		ptIn[0]=m_pt[0];
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=m_pt[1].y-(m_pt[1].y-m_pt[0].y)*7/8;
		if(rtn>0)
			break;
		rtn=IsOnLine(ptIn[0], ptIn[1], point,1);

		//---		上部
		ptIn[0]=m_pt[0];
		ptIn[1].y=m_pt[1].y;
		ptIn[1].x=m_pt[0].x+(m_pt[1].x-m_pt[0].x)/8;
		rtn=IsOnLine(ptIn[0], ptIn[1], point,1);
		if(rtn>0)
			break;

		ptIn[0]=m_pt[0];
		ptIn[1].y=m_pt[1].y;
		ptIn[1].x=m_pt[0].x+(m_pt[1].x-m_pt[0].x)/4;
		rtn=IsOnLine(ptIn[0], ptIn[1], point,1);
		if(rtn>0)
			break;

		ptIn[0]=m_pt[0];
		ptIn[1].y=m_pt[1].y;
		ptIn[1].x=m_pt[0].x+(m_pt[1].x-m_pt[0].x)/3;
		rtn=IsOnLine(ptIn[0], ptIn[1], point,1);
		if(rtn>0)
			break;

		ptIn[0]=m_pt[0];
		ptIn[1].y=m_pt[1].y;
		ptIn[1].x=m_pt[0].x+(m_pt[1].x-m_pt[0].x)/2;
		rtn=IsOnLine(ptIn[0], ptIn[1], point,1);

		break;
	case		BO_DUAN:		
		rtn=IsOnPoint(m_pt[0], m_pt[1], point, 0);
		if(rtn>0)
			break;
		ptLineSide[0].x=0;
		ptLineSide[0].y=m_pt[1].y;
		ptLineSide[1].y=m_pt[1].y;
		ptLineSide[1].x=m_rtDrawLine.right;
		rtn=IsOnPoint(ptLineSide[0], ptLineSide[1], point,1);
		if(rtn>0)
			break;

		ptLineSide[0].x=0;
		ptLineSide[1].x=m_rtDrawLine.right;
		ptLineSide[0].y=m_pt[0].y;
		ptLineSide[1].y=m_pt[0].y;
		rtn=IsOnPoint(ptLineSide[0], ptLineSide[1], point,1);
		if(rtn>0)
			break;

		ptLineSide[0].x=0;
		ptLineSide[1].x=m_rtDrawLine.right;

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.236);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.382);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.5);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.618);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		break;
	case		HUANG_JIN	:	    
		ptLineSide[0].x=0;
		ptLineSide[0].y=m_pt[1].y;
		ptLineSide[1].y=m_pt[1].y;
		ptLineSide[1].x=m_rtDrawLine.right;
		rtn=IsOnPoint(m_pt[0], m_pt[1], point, 0);
		if(rtn>0)
			break;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=m_pt[0].y;
		ptLineSide[1].y=m_pt[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;

		ptLineSide[0].x=0;
		ptLineSide[1].x=m_rtDrawLine.right;

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.382);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.5);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.618);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*1.618);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*2.618);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		break;
	case		BAI_FEN_BI:      
		rtn=IsOnPoint(m_pt[0], m_pt[1], point, 0);
		if(rtn>0)
			break;

		ptLineSide[0].x=0;
		ptLineSide[0].y=m_pt[1].y;
		ptLineSide[1].y=m_pt[1].y;
		ptLineSide[1].x=m_rtDrawLine.right;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=m_pt[0].y;
		ptLineSide[1].y=m_pt[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;

		ptLineSide[0].x=0;
		ptLineSide[1].x=m_rtDrawLine.right;

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.333);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.667);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.125);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.25);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.375);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.5);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.625);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.75);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		if(rtn>0)
			break;
		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.875);
		ptLineSide[1].y=ptLineSide[0].y;
		rtn=IsOnLine(ptLineSide[0], ptLineSide[1], point,0);
		break;
	case		HUI_GUI   :      
	case		HUI_GUI_DAI_1:   
//		if(m_pt[0].x>=m_pt[1].x)
//			break;
		rtn=IsOnLine(m_pt[0], m_pt[1], point,0);
		if(rtn>0)
		{
			int ii=89;
		}
		break;
	case		HUI_GUI_DAI_2:	
		if(m_pt[0].x>=m_pt[1].x)
			break;
		rtn=IsOnLine(m_pt[0], m_pt[1], point,1);
		break;
	case		ZHOU_QI		:	
		if(m_pt[0].x>=m_pt[1].x)
		{
			rtn=IsOnLine(m_pt[0], m_pt[1], point, 0);
			break;
		}
		rtn=IsOnPoint(m_pt[0], m_pt[1], point, 0);
		if(rtn>0)
			break;
		ptIn[0].y=pView->m_heightCaption;
		ptIn[1].y=m_rtDrawLine.bottom;
		ptIn[1].x=m_pt[0].x;
		ptIn[0].x=m_pt[0].x;
		rtn=IsOnLine(ptIn[0], ptIn[1], point,0);
		if(rtn>0)
			break;
		ptIn[1].x=m_pt[1].x;
		ptIn[0].x=m_pt[1].x;
		rtn=IsOnLine(ptIn[0], ptIn[1], point,0);
		if(rtn>0)
			break;

		i=m_pt[1].x+(m_pt[1].x-m_pt[0].x);
		if(m_pt[1].x-m_pt[0].x<3)
			break;
		while(i<m_rtDrawLine.right )
		{
			ptIn[1].x=i;
			ptIn[0].x=i;
			rtn=IsOnLine(ptIn[0], ptIn[1], point,0);
			if(rtn>0)
				break;
			i+=(m_pt[1].x-m_pt[0].x);
			
		}
		break;
	case		NA_QI        :   
		if(m_pt[0].x>=m_pt[1].x)
		{
			rtn=IsOnLine(m_pt[0], m_pt[1], point, 0);
			break;
		}
		rtn=IsOnPoint(m_pt[0], m_pt[1], point, 0);
		if(rtn>0)
			break;
		ptIn[0].y=pView->m_heightCaption;
		ptIn[1].y=m_rtDrawLine.bottom;
		ptIn[1].x=m_pt[0].x;
		ptIn[0].x=m_pt[0].x;
		rtn=IsOnLine(ptIn[0], ptIn[1], point,0);
		if(rtn>0)
			break;

		i=1;
		j1=1;
		j2=1;
		while(m_pt[0].x+i*m_widthPer<m_rtDrawLine.right )
		{
			ptIn[1].x=(int)(m_pt[0].x+i*m_widthPer);
			ptIn[0].x=(int)(m_pt[0].x+i*m_widthPer);
			rtn=IsOnLine(ptIn[0], ptIn[1], point,0);
			if(rtn>0)
				break;
			int tmp=j2;
			j2=j1+j2;
			j1=tmp;
			i+=j1;
		}
		break;
	case		ZHONG_XIAN	:	
		ptIn[0].y=(m_pt[0].y+m_pt[1].y)/2;
		ptIn[0].x=(m_pt[0].x+m_pt[1].x)/2;
		rtn=IsOnLine(m_pt[2], ptIn[0], point,1);
		if(rtn>0)
			break;

		ptIn[1].y=ptIn[0].y-m_pt[2].y+m_pt[0].y;
		ptIn[1].x=ptIn[0].x-m_pt[2].x+m_pt[0].x;
		rtn=IsOnLine(m_pt[0], ptIn[1], point,1);
		if(rtn>0)
			break;

		ptIn[1].y=ptIn[0].y-m_pt[2].y+m_pt[1].y;
		ptIn[1].x=ptIn[0].x-m_pt[2].x+m_pt[1].x;
		rtn=IsOnLine(m_pt[1], ptIn[1], point,1);
		break;
	case		NA_QI_HU	:
		rtn=IsOnLine(m_pt[0], m_pt[1], point,0);
		break;
	case		UP_ARROW    :   
	case		DOWN_ARROW	:
		r.left =m_pt[0].x;
		r.top =m_pt[0].y;
		r.right =m_pt[0].x+LEN_BITMAP;
		r.bottom =m_pt[0].y+LEN_BITMAP;//
		if(r.PtInRect (point))
			rtn=4;
		else
			rtn=-1;
		break;
	case		RECT_KIND	:	
		rtn=IsOnPoint(m_pt[0],m_pt[1], point,0);
		if(rtn>0)
				break;
		ptIn[0].y=m_pt[1].y;
		ptIn[0].x=m_pt[0].x;
		rtn=IsOnPoint(m_pt[0], ptIn[0], point,1);
		if(rtn>0)
				break;
		ptIn[0].y=m_pt[0].y;
		ptIn[0].x=m_pt[1].x;
		rtn=IsOnPoint(m_pt[0], ptIn[0], point,1);
		if(rtn>0)
				break;
		ptIn[1].y=m_pt[1].y;
		ptIn[1].x=m_pt[0].x;
		rtn=IsOnPoint(m_pt[1], ptIn[1], point,1);
		if(rtn>0)
				break;
		ptIn[1].y=m_pt[0].y;
		ptIn[1].x=m_pt[1].x;
		rtn=IsOnPoint(m_pt[1], ptIn[1], point,1);
		break;
	case		TEXT_KIND	:	
		r.left =m_pt[0].x;
		r.top =m_pt[0].y;
		r.right =m_pt[0].x+m_lenTextNow+1;
		r.bottom =m_pt[0].y+18;
		if(r.PtInRect (point))
			rtn=4;
		else
			rtn=-1;
		break;
	case		BAG_LINE   :       
		//		ZHONG_XIAN		
			//	计算间距
//		if((m_pt[0].y-m_pt[1].y) == 0)
//			break;

		gap=(float)FABSMY((m_pt[2].y*(m_pt[0].x-m_pt[1].x)-m_pt[1].y*m_pt[0].x+m_pt[0].y*m_pt[1].x)/(float)(m_pt[0].y-m_pt[1].y)-m_pt[2].x);
		gap=(float)((int)(gap/m_widthPer+0.5))*(float)m_widthPer;

		if(gap<=0 || m_pt[0].x-m_pt[1].x == 0)
		{
			rtn=IsOnLine(m_pt[0], m_pt[1], point,0);
			break;
		}
		if(m_pt[0].x<=m_rtDrawLine.right || m_pt[1].x<=m_rtDrawLine.right )
		for(i=0;i<2000;i++ )
		{
			ptIn[0].y=m_pt[0].y;
			ptIn[0].x=(int)(m_pt[0].x+i*gap);
			ptIn[1].y=m_pt[1].y;
			ptIn[1].x=(int)(m_pt[1].x+i*gap);
			CaclTwoPoint(ptIn[0], ptIn[1],0);//???
			if(ptLineSide[0].y>m_rtDrawLine.bottom+2&& ptLineSide[1].y>m_rtDrawLine.bottom+2)
				continue;
			if(ptLineSide[0].y<m_rtDrawLine.top-2&& ptLineSide[1].y<m_rtDrawLine.top-2)
				continue;
			rtn=IsOnLine(ptIn[0], ptIn[1], point,2);
			if(rtn>0)
				break;
		}
		if(rtn>0)
			break;

		if(m_pt[0].x>=m_rtDrawLine.left || m_pt[1].x>=m_rtDrawLine.left )
		for(i=1;i<2000;i++ )
		{
			ptIn[0].y=m_pt[0].y;
			ptIn[0].x=(int)(m_pt[0].x-i*gap);
			ptIn[1].y=m_pt[1].y;
			ptIn[1].x=(int)(m_pt[1].x-i*gap);
//			if(ptIn[0].x<m_rtDrawLine.left-150 && ptIn[1].x<m_rtDrawLine.left-150 )
//				break;
//			if(ptIn[0].x>m_rtDrawLine.right+150 && ptIn[1].x>m_rtDrawLine.right+150 )
//				continue;
			CaclTwoPoint(ptIn[0], ptIn[1],0);//???
			if(ptLineSide[0].y>m_rtDrawLine.bottom+2&& ptLineSide[1].y>m_rtDrawLine.bottom+2)
				continue;
			if(ptLineSide[0].y<m_rtDrawLine.top-2&& ptLineSide[1].y<m_rtDrawLine.top-2)
				continue;
			rtn=IsOnLine(ptIn[0], ptIn[1], point,2);
			if(rtn>0)
				break;
		}
		break;
	}
	return rtn-1;

}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：IsOnLine(CPoint &p1, CPoint &p2, CPoint &pointIn, int flag)
//      功能说明：输入点是否在线上
//      入口参数：p1、p2、pointIn为输入点
//      出口参数：无出口数
//      返回参数：返回输入点在画线的位置
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineDrawing::IsOnLine(CPoint &p1, CPoint &p2, CPoint &pointIn, int flag)
//---		flag=0为线段，flag=1为射线，flag=2为直线
{
	if(m_lineType!=ZHONG_XIAN&& m_lineType!=BAG_LINE)
	{
	//---		是否在两端点
		CRect r1(p1.x-2,p1.y-2,p1.x+2,p1.y+2);
		CRect r2(p2.x-2,p2.y-2,p2.x+2,p2.y+2);
		if(r1.PtInRect (pointIn)!=0)
		{
			return 1;//第一点
		}
		//水平线处理
		if(m_lineType==SHUI_PING)
		{
			if(abs(pointIn.y-p1.y) <3)
				return 4;
			else
				return -1;
		}

		///////////end 
		if(r2.PtInRect (pointIn)!=0)
		{
			return 2;//第二点
		}
	}
	else
	{
	//---		是否在三端点
		if(m_bMoveZhongXian==0)//							pDrawLine->=0;
			return -1;
		CRect r31(m_pt[0].x-2,m_pt[0].y-2,m_pt[0].x+2,m_pt[0].y+2);
		CRect r32(m_pt[1].x-2,m_pt[1].y-2,m_pt[1].x+2,m_pt[1].y+2);
		CRect r33(m_pt[2].x-2,m_pt[2].y-2,m_pt[2].x+2,m_pt[2].y+2);
		if(r31.PtInRect (pointIn)!=0)
			return 1;//第一点
		if(r32.PtInRect (pointIn)!=0)
			return 2;//第二点
		if(r33.PtInRect (pointIn)!=0)
			return 3;//第三点
	}

	int y1,y2;
	if(p1.y<p2.y)
	{
		y1=p1.y;
		y2=p2.y;
	}
	else
	{
		y1=p2.y;
		y2=p1.y;
	}
	int x1,x2;
	if(p1.x<p2.x)
	{
		x1=p1.x;
		x2=p2.x;
	}
	else
	{
		x1=p2.x;
		x2=p1.x;
	}

	//--		是否在画线体上
	int y;
	switch(flag)
	{
	case 0:
		if(pointIn.x<x1-1||pointIn.x>x2+1)
			return -1;
		if(p1.x==p2.x)
		{
			if(pointIn.y>=y1&&pointIn.y<=y2)
				return 4;
			else
				return -1;
		}
		y=(int)(p1.y+(pointIn.x-p1.x)*(p1.y-p2.y)/(float)(p1.x-p2.x));
		if(abs(y-pointIn.y)>2)
			return -1;
		else
			return 4;
		break;
	case 1:
//		if(p1.x>p2.x)
//			return -1;
		if(pointIn.x<x1-1 && pointIn.x<x2)
			return -1;
		if(p1.x==p2.x)
		{
			return -1;
		}
		y=(int)(p1.y+(pointIn.x-p1.x)*(p1.y-p2.y)/(float)(p1.x-p2.x));
		if(abs(y-pointIn.y)>2)
		{
			TRACE("LLLLNNN   ==  %d \n",y-pointIn.y);
			return -1;
		}
		else
			return 4;
		break;
	case 2:
		if(p1.x==p2.x)
		{
			if(abs(pointIn.y-y1)<=1)
				return 4;
			else
				return -1;
		}
		y=(int)(p1.y+(pointIn.x-p1.x)*(p1.y-p2.y)/(float)(p1.x-p2.x));
		if(abs(y-pointIn.y)>2)
			return -1;
		else
			return 4;
		break;
	}
	return -1;

}

int CTaiKlineDrawing::IsOnPoint(CPoint &p1, CPoint &p2, CPoint &pointIn, int flag)
//---		flag=0为点，flag=1为线段
{
	if(flag==0)
	{
	//---		是否在两端点
		CRect r1(p1.x-2,p1.y-2,p1.x+2,p1.y+2);
		CRect r2(p2.x-2,p2.y-2,p2.x+2,p2.y+2);
		if(r1.PtInRect (pointIn)!=0)
		{
			return 1;//第一点
		}
		if(r2.PtInRect (pointIn)!=0)
		{
			return 2;//第二点
		}
	}
	else
	{
		int y1,y2;
		if(p1.y<p2.y)
		{
			y1=p1.y;
			y2=p2.y;
		}
		else
		{
			y1=p2.y;
			y2=p1.y;
		}
		int x1,x2;
		if(p1.x<p2.x)
		{
			x1=p1.x;
			x2=p2.x;
		}
		else
		{
			x1=p2.x;
			x2=p1.x;
		}

	//--		是否在画线体上
		int y;
		if(pointIn.x<x1||pointIn.x>x2)
			return -1;
		if(p1.x==p2.x)
		{
			if(pointIn.y>=y1&&pointIn.y<=y2)
				return 4;
			else
				return -1;
		}
		y=(int)(p1.y+(pointIn.x-p1.x)*(p1.y-p2.y)/(float)(p1.x-p2.x));
		if(abs(y-pointIn.y)>2)
			return -1;
		else
			return 4;
	}
	return -1;
	//--		是否在画线体上

}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawRect(CDC* pDC,CRect &r)
//      功能说明：画矩形
//      入口参数：r为输入矩形
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDrawing::DrawRect(CDC* pDC,CRect &r)
{
//	CRgn rgn;
//	rgn.CreateRectRgn (m_rtDrawLine.left,m_rtDrawLine.top,m_rtDrawLine.right,m_rtDrawLine.bottom );
//	pDC->SelectClipRgn (&rgn);
	CPoint p((r.left+r.right )/2,(r.top+r.bottom)/2);
	if(m_rtDrawLine.PtInRect (p))
		pDC->InvertRect (r);
//	for(int i=r.left;i<=r.right;i++)
//	{
//		pDC->MoveTo(i,r.top);
//		pDC->LineTo(i,r.bottom);
//	}

//	rgn.DeleteObject ();
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawSelfLine(CDC* pDC)
//      功能说明：画一个自画线
//      入口参数：pDC为设备描述表
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineDrawing::DrawSelfLine(CDC *pDC,bool bSetProp)
{
//	if(!pDC->IsPrinting())
		SetDrawLineRect();

	if(m_lineType<0)
		return -1;
	int widthPen = 1;
/*	if(pDC->IsPrinting())
	{
		CRect r(0,0,100,100);
		pDC->LPtoDP(&r);
		if(r.right>1500 || r.right<1500)
			widthPen = 3;
		else if(r.right>15000)
			widthPen = 5;
	}*/
	static int g_lineKind[] = {PS_SOLID,PS_DOT   ,PS_DASHDOT   };
	int nLineKind = 0;
	int nWid2=0;
	COLORREF clr=pDoc->m_colorArray[6];
	if(m_info_lineself.GetSize ()>0 && bSetProp == true)
	{
		if( this->m_nCurrentLine>=m_info_lineself.GetSize ()||this->m_nCurrentLine<0)
		{
			ASSERT(FALSE);
			m_nCurrentLine = 0;
		}
		if(m_info_lineself[ this->m_nCurrentLine  ].byLineKind >=3)
			m_info_lineself[ this->m_nCurrentLine  ].byLineKind =0;
		nLineKind = m_info_lineself[ this->m_nCurrentLine  ].byLineKind ;
		nWid2 = m_info_lineself[ this->m_nCurrentLine  ].byLineWidth;
		clr = m_info_lineself[ this->m_nCurrentLine  ].color ;
	}
	CPen pen1(g_lineKind[nLineKind]  , nWid2+1,clr);
	CPen * pOldPen = pDC->SelectObject(&pen1);

	CRgn rgn;

	if(!pDC->IsPrinting())
	{
	if(rgn.CreateRectRgn (m_rtDrawLine.left,m_rtDrawLine.top-1,m_rtDrawLine.right,m_rtDrawLine.bottom+3 )==ERROR)
		return -1;
	pDC->SelectClipRgn (&rgn);
	}
	int i,j1,j2;
	float gap;
	m_bLineValidate=true;

	switch(m_lineType)
	{
	case		XIAN_DUAN: 
		if(m_pt[0].x==m_pt[1].x&&m_pt[0].y==m_pt[1].y)
		{
			m_bLineValidate=false;
			break;
		}
		if(m_pt[0].x>=pView->m_rtKlineFiguer .rightX &&m_pt[1].x>=pView->m_rtKlineFiguer .rightX)
		{
			m_bLineValidate=false;
			break;
		}
		if(m_pt[0].x<=0 &&m_pt[1].x<=0)
		{
			m_bLineValidate=false;
			break;
		}
		CaclTwoPointXD(m_pt[0],m_pt[1]);
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		break;
	case		SHUI_PING:      
		pDC->MoveTo(0,m_pt[0].y);
		pDC->LineTo(m_rtDrawLine.right ,m_pt[0].y);
		break;
	case		QU_SHI   :       
		if(m_pt[0].x==m_pt[1].x&&m_pt[0].y==m_pt[1].y)
		{
			m_bLineValidate=false;
			break;
		}
		CaclTwoPoint(m_pt[0], m_pt[1],0);
		pDC->MoveTo(ptLineSide[0].x ,ptLineSide[0].y);
		pDC->LineTo(ptLineSide[1].x ,ptLineSide[1].y);

		//alarm
		if( bSetProp == true)
		{
			if(m_info_lineself[ m_nCurrentLine ].byAlarm == 1 )
			{
				DrawArrowUp(IDB_BITMAP_11_ALARMLINE,pDC,1,16);//UINT id
			}
		}
		//
		break;
	case		PING_XING:
		if(m_lineTypeOld!=XIAN_DUAN)//
		{
//			m_lineType=m_lineTypeOld;
			break;
		}
		pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
		pDC->LineTo(m_pt[1].x ,m_pt[1].y);
		break;
	case		SU_DU:
		if(m_pt[0].x>=pView->m_rtKlineFiguer .rightX )
		{
			m_bLineValidate=false;
		}
		if(m_pt[0].x>=m_pt[1].x)
		{
			m_bLineValidate=false;
		}
		CaclTwoPoint(m_pt[0], m_pt[1],1);
		if(ptLineSide[0].x<m_pt[0].x)
		{
			m_bLineValidate=false;
		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}

		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);

		ptIn[0].x=m_pt[0].x;
		ptIn[0].y=m_pt[0].y;
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=m_pt[1].y-(m_pt[1].y-m_pt[0].y)/3;
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);

		ptIn[0].x=m_pt[0].x;
		ptIn[0].y=m_pt[0].y;
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=m_pt[1].y-(m_pt[1].y-m_pt[0].y)*2/3;
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);
		break;
	case		SHAN_XING:       
		if(m_pt[0].x>=m_pt[1].x)
		{
			m_bLineValidate=false;
		}
		if(m_pt[0].x>=pView->m_rtKlineFiguer .rightX )
		{
			m_bLineValidate=false;
		}
		CaclTwoPoint(m_pt[0], m_pt[1],1);
		if(ptLineSide[0].x<m_pt[0].x)
		{
			m_bLineValidate=false;
		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}

		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);

		ptIn[0].x=m_pt[0].x;
		ptIn[0].y=m_pt[0].y;
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=(int)(m_pt[1].y-(m_pt[1].y-m_pt[0].y)*0.318);
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);

		ptIn[0].x=m_pt[0].x;
		ptIn[0].y=m_pt[0].y;
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=m_pt[1].y-(m_pt[1].y-m_pt[0].y)/2;
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);

		ptIn[0].x=m_pt[0].x;
		ptIn[0].y=m_pt[0].y;
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=(int)(m_pt[1].y-(m_pt[1].y-m_pt[0].y)*0.618);
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);
		break;
	case		HUANG_JIN_HU:  //(下甘氏) 
	case		GAN_SHI:         
		if(m_pt[0].x>=m_pt[1].x)
		{
			m_bLineValidate=false;
		}
		if(m_pt[0].x>=pView->m_rtKlineFiguer .rightX )
		{
			m_bLineValidate=false;
		}
		CaclTwoPoint(m_pt[0], m_pt[1],1);
		if(ptLineSide[0].x<m_pt[0].x)
		{
			m_bLineValidate=false;
		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}

		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);

		ptIn[0].x=m_pt[0].x;ptIn[0].y=m_pt[0].y;
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=(2*m_pt[1].y-(m_pt[1].y-m_pt[0].y))/2;
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);

		ptIn[0].x=m_pt[0].x;ptIn[0].y=m_pt[0].y;
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=(3*m_pt[1].y-(m_pt[1].y-m_pt[0].y)*2)/3;
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);

		ptIn[0].x=m_pt[0].x;ptIn[0].y=m_pt[0].y;
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=(4*m_pt[1].y-(m_pt[1].y-m_pt[0].y)*3)/4;
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);

		ptIn[0].x=m_pt[0].x;ptIn[0].y=m_pt[0].y;
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=(8*m_pt[1].y-(m_pt[1].y-m_pt[0].y)*7)/8;
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);

		//---		上部
		ptIn[0].x=m_pt[0].x;ptIn[0].y=m_pt[0].y;
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=m_pt[1].y + (m_pt[1].y-m_pt[0].y);
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);

		ptIn[0].x=m_pt[0].x;ptIn[0].y=m_pt[0].y;
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=m_pt[1].y+(m_pt[1].y-m_pt[0].y)*2;
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);

		ptIn[0].x=m_pt[0].x;ptIn[0].y=m_pt[0].y;
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=m_pt[1].y+(m_pt[1].y-m_pt[0].y)*3;
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);

		ptIn[0].x=m_pt[0].x;ptIn[0].y=m_pt[0].y;
		ptIn[1].x=m_pt[1].x;
		ptIn[1].y=m_pt[1].y+(m_pt[1].y-m_pt[0].y)*7;
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);

///////////////////////////////////////////////////////
/*		ptIn[0].x=m_pt[0].x;ptIn[0].y=m_pt[0].y;
		ptIn[1].y=m_pt[1].y;
		ptIn[1].x=m_pt[0].x+(m_pt[1].x-m_pt[0].x)/8;
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1]);

		ptIn[0].x=m_pt[0].x;ptIn[0].y=m_pt[0].y;
		ptIn[1].y=m_pt[1].y;
		ptIn[1].x=m_pt[0].x+(m_pt[1].x-m_pt[0].x)/4;
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1]);

		ptIn[0].x=m_pt[0].x;ptIn[0].y=m_pt[0].y;
		ptIn[1].y=m_pt[1].y;
		ptIn[1].x=m_pt[0].x+(m_pt[1].x-m_pt[0].x)/3;
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1]);

		ptIn[0].x=m_pt[0].x;ptIn[0].y=m_pt[0].y;
		ptIn[1].y=m_pt[1].y;
		ptIn[1].x=m_pt[0].x+(m_pt[1].x-m_pt[0].x)/2;
		CaclTwoPoint(ptIn[0], ptIn[1],1);
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1]);*/

		break;
	case	BO_DUAN	:		
//		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		if(m_pt[0].y==m_pt[1].y)
		{
			m_bLineValidate=false;
		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}

		ptLineSide[0].x=0;
		ptLineSide[1].x=m_rtDrawLine.right;

		ptLineSide[0].y=(int)(m_pt[0].y);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)0, ptLineSide[0].y,m_nCurrentLine);

		ptLineSide[0].y=(int)(m_pt[1].y);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)1, ptLineSide[0].y,m_nCurrentLine);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.236);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);
		ShowText(pDC,(float)0.236, ptLineSide[0].y,m_nCurrentLine);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.382);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);
		ShowText(pDC,(float)0.382, ptLineSide[0].y,m_nCurrentLine);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.5);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);
		ShowText(pDC,(float)0.5, ptLineSide[0].y,m_nCurrentLine);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.618);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);
		ShowText(pDC,(float)0.618, ptLineSide[0].y,m_nCurrentLine);

		break;
	case	HUANG_JIN		:	    
		if(m_pt[0].y==m_pt[1].y)
		{
			m_bLineValidate=false;
		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}

		ptLineSide[0].x=0;
		ptLineSide[1].x=m_rtDrawLine.right;

		ptLineSide[0].y=(int)(m_pt[0].y);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)0, ptLineSide[0].y,m_nCurrentLine);

		ptLineSide[0].y=(int)(m_pt[1].y);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)1, ptLineSide[0].y,m_nCurrentLine);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.382);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);
		ShowText(pDC,(float)0.382, ptLineSide[0].y,m_nCurrentLine);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.5);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);
		ShowText(pDC,(float)0.5, ptLineSide[0].y,m_nCurrentLine);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.618);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);
		ShowText(pDC,(float)0.618, ptLineSide[0].y,m_nCurrentLine);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*1.618);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);
		ShowText(pDC,(float)1.618, ptLineSide[0].y,m_nCurrentLine);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*2.618);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuanVirtul(pDC,ptLineSide[0],ptLineSide[1],&clr);
		ShowText(pDC,(float)2.618, ptLineSide[0].y,m_nCurrentLine);

		break;
	case		BAI_FEN_BI:      
		if(m_pt[0].y==m_pt[1].y)
		{
			m_bLineValidate=false;
		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}

		ptLineSide[0].x=0;
		ptLineSide[1].x=m_rtDrawLine.right;

		ptLineSide[0].y=(int)(m_pt[0].y);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)0, ptLineSide[0].y,m_nCurrentLine);

		ptLineSide[0].y=(int)(m_pt[1].y);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)1, ptLineSide[0].y,m_nCurrentLine);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.333);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)0.333, ptLineSide[0].y);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.667);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)0.667, ptLineSide[0].y);


		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.125);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)0.125, ptLineSide[0].y);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.25);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)0.25, ptLineSide[0].y);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.375);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)0.375, ptLineSide[0].y);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.5);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)0.5, ptLineSide[0].y);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.625);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)0.625, ptLineSide[0].y);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.75);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)0.75, ptLineSide[0].y);

		ptLineSide[0].y=(int)(m_pt[0].y+(m_pt[1].y-m_pt[0].y)*0.875);
		ptLineSide[1].y=ptLineSide[0].y;
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		ShowText(pDC,(float)0.875, ptLineSide[0].y);

		break;
	case		HUI_GUI   :      
		if(m_pt[0].x>=pView->m_rtKlineFiguer .rightX )
		{
			m_bLineValidate=false;
		}
		if(m_pt[0].x>=m_pt[1].x)
		{
			m_bLineValidate=false;
		}
		if(m_pt[0].x<0 &&m_pt[1].x<0)
		{
			m_bLineValidate=false;
		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}

		CaclTwoPointXD(m_pt[0],m_pt[1]);
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
//		pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
//		pDC->LineTo(m_pt[1].x ,m_pt[1].y);
		break;
	case		HUI_GUI_DAI_1:   
		if(m_pt[0].x>=m_pt[1].x)
		{
			m_bLineValidate=false;
		}
		if(m_pt[0].x>=pView->m_rtKlineFiguer .rightX )
		{
			m_bLineValidate=false;
		}
		if(m_pt[0].x<0 &&m_pt[1].x<0)
		{
			m_bLineValidate=false;
		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}

		CaclTwoPointXD(m_pt[0],m_pt[1]);
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
//		pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
//		pDC->LineTo(m_pt[1].x ,m_pt[1].y);
		//---		画上下两条线
		DrawHuiGuiTop(pDC,0,&clr);
		break;
	case		HUI_GUI_DAI_2:	
		if(m_pt[0].x>=m_pt[1].x)
		{
			m_bLineValidate=false;
		}
		if(m_pt[0].x>=pView->m_rtKlineFiguer .rightX )
		{
			m_bLineValidate=false;
		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}

		CaclTwoPointXD(m_pt[0],m_pt[1]);
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		if(ptLineSide[0]!=ptLineSide[1])
		{
		CPoint p1 = ptLineSide[1];
		CaclTwoPoint(m_pt[0],m_pt[1],1);
		if(ptLineSide[1].x<ptLineSide[0].x)
			ptLineSide[1] = ptLineSide[0];
		DrawXianDuanVirtul(pDC,p1,ptLineSide[1],&clr);
		}
		//---		画上下两条线
		DrawHuiGuiTop(pDC,1,&clr);
		break;
	case		ZHOU_QI		:	
		if(m_pt[0].x>=m_pt[1].x)
		{
			m_bLineValidate=false;
		}
//		if(m_pt[1].x<0)
//			break;
		if(m_pt[1].x>=m_rtDrawLine.right)
			m_bLineValidate=false;
		gap=((int)(m_pt[1].x/(float)m_widthPer)-(int)(m_pt[0].x/(float)m_widthPer))*(float)m_widthPer;

		if(gap<2)
			m_bLineValidate=false;
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}

		if(m_pt[0].x>=0)
		{
		pDC->MoveTo(m_pt[0].x,pView->m_heightCaption);
		pDC->LineTo(m_pt[0].x,m_rtDrawLine.bottom );
		}
		if(m_pt[1].x>=0)
		{
		pDC->MoveTo(m_pt[1].x,pView->m_heightCaption);
		pDC->LineTo(m_pt[1].x,m_rtDrawLine.bottom );
		}

		i=1;
		while((int)(m_pt[1].x+gap*i)<m_rtDrawLine.right )
		{
			int nNow = (int)(m_pt[1].x+gap*i);
			if(nNow >= 0)
			{
			pDC->MoveTo(nNow,pView->m_heightCaption);
			pDC->LineTo(nNow,m_rtDrawLine.bottom );

			if(pView->pKlineDrawing ->m_footEnd -pView->pKlineDrawing ->m_footBegin+1< pView->pKlineDrawing ->m_klinNumDefault  
				&& pView->pKlineDrawing ->m_klinNumDefault<640)
			{
				int n2  = (nNow/m_widthPer);
				n2 -= pView->pKlineDrawing ->m_footEnd-pView->pKlineDrawing ->m_footBegin;
				if(n2>0) 
				{
					CString s;
					s.Format ("%d",(int)(n2+0.5));
					CPoint pp(nNow+2,m_rtDrawLine.bottom - 30 );
					DrawText(pDC,&pp,&s);
				}
			}//DrawText(CDC* pDC,CPoint* pp,CString * s)

			}
			i++;
			if(i>5000)
				break;
		}
		break;
	case		NA_QI        :   
		if(m_pt[0].x>=m_pt[1].x)
		{
			m_bLineValidate=false;
		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}
//		if(m_pt[1].x<0)
//			break;
		if(m_pt[0].x>=0)
		{
		pDC->MoveTo(m_pt[0].x,pView->m_heightCaption);
		pDC->LineTo(m_pt[0].x,m_rtDrawLine.bottom );
		}

		i=1;
		j1=1;
		j2=1;

		while(m_pt[0].x+i*m_widthPer<m_rtDrawLine.right )
		{
			int nNow = (int)(m_pt[0].x+i*m_widthPer);
			if(nNow>=0&&m_widthPer>0)
			{
			pDC->MoveTo(nNow,pView->m_heightCaption);
			pDC->LineTo(nNow,m_rtDrawLine.bottom );

			if(pView->pKlineDrawing ->m_footEnd -pView->pKlineDrawing ->m_footBegin+1< pView->pKlineDrawing ->m_klinNumDefault  
				&&pView->pKlineDrawing ->m_klinNumDefault<640)
			{
				int n2  = (nNow/m_widthPer);
				n2 -= pView->pKlineDrawing ->m_footEnd-pView->pKlineDrawing ->m_footBegin;
				if(n2>0) 
				{
					CString s;
					s.Format ("%d",(int)(n2+0.5));
					CPoint pp(nNow+2,m_rtDrawLine.bottom - 30 );
					DrawText(pDC,&pp,&s);
				}
			}//DrawText(CDC* pDC,CPoint* pp,CString * s)

			}
			int tmp=j2;
			j2=j1+j2;
			j1=tmp;
			i+=j1;
		}
		break;
	case		ZHONG_XIAN	:
		if(m_pt[0].x==m_pt[1].x&&m_pt[0].y==m_pt[1].y)
		{
			m_bLineValidate=false;
		}
		if(m_pt[0].x>=pView->m_rtKlineFiguer .rightX )
		{
			m_bLineValidate=false;
		}
		if(m_pt[1].x>=pView->m_rtKlineFiguer .rightX )
		{
			m_bLineValidate=false;
		}
		if(m_pt[2].x>=pView->m_rtKlineFiguer .rightX )
		{
			m_bLineValidate=false;
		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}
		if(m_nZhongXian==0)
		{
			DrawXianDuan(pDC,m_pt[0],m_pt[1]);
		}
		else
		{
			ptIn[0].y=(m_pt[0].y+m_pt[1].y)/2;
			ptIn[0].x=(m_pt[0].x+m_pt[1].x)/2;
			CaclTwoPoint(m_pt[2], ptIn[0],1);
			if(ptLineSide[0].x>=m_pt[2].x)
			{
				DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
			}

			ptIn[1].y=ptIn[0].y-m_pt[2].y+m_pt[0].y;
			ptIn[1].x=ptIn[0].x-m_pt[2].x+m_pt[0].x;
			CaclTwoPoint(m_pt[0], ptIn[1],1);
			if(ptLineSide[0].x>=m_pt[0].x)
			{
				DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
			}

			ptIn[1].y=ptIn[0].y-m_pt[2].y+m_pt[1].y;
			ptIn[1].x=ptIn[0].x-m_pt[2].x+m_pt[1].x;
			CaclTwoPoint(m_pt[1], ptIn[1],1);
			if(ptLineSide[0].x>=m_pt[1].x)
			{
				DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
			}
		}

		break;
	case		NA_QI_HU	:
		if(m_pt[0].x==m_pt[1].x || m_pt[0].y==m_pt[1].y)
		{
			m_bLineValidate=false;
		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}
		{
			CaclTwoPointXD(m_pt[0],m_pt[1]);
			DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);

			float ax = sqrt(2.0)*FABSMY((m_pt[0].x-m_pt[1].x ));
			float by = sqrt(2.0)*FABSMY((m_pt[0].y-m_pt[1].y ));

			ptIn[0].y=m_pt[0].y;
			ptIn[0].x=m_pt[0].x;
			float fRate[] = {0.382,0.5,0.618,1};
			for(int k=0;k<4;k++)
				ArcInRect (pDC , ptIn[0].x-ax*fRate[k],ptIn[0].y-by*fRate[k],ptIn[0].x+ax*fRate[k],ptIn[0].y+by*fRate[k]);
		}
		break;
	case		UP_ARROW    :  
		DrawArrowUp(IDB_70,pDC);//
		break;
	case		DOWN_ARROW	:	
		DrawArrowUp(IDB_30,pDC);//
		break;
	case		RECT_KIND	:	
		if(m_pt[0].x==m_pt[1].x&&m_pt[0].y==m_pt[1].y)
		{
			m_bLineValidate=false;
		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}
		ptIn[0].x = m_pt[0].x;
		ptIn[0].y = m_pt[1].y;
		CaclTwoPointXD(m_pt[0],ptIn[0]);
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
//		pDC->MoveTo(m_pt[0].x,m_pt[0].y);
//		pDC->LineTo(m_pt[0].x,m_pt[1].y);

		ptIn[0].x = m_pt[0].x;
		ptIn[0].y = m_pt[1].y;
		ptIn[1].x = m_pt[1].x;
		ptIn[1].y = m_pt[1].y;
		CaclTwoPointXD(ptIn[0],ptIn[1]);
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
//		pDC->LineTo(m_pt[1].x,m_pt[1].y);

		ptIn[0].x = m_pt[1].x;
		ptIn[0].y = m_pt[0].y;
		ptIn[1].x = m_pt[1].x;
		ptIn[1].y = m_pt[1].y;
		CaclTwoPointXD(ptIn[1],ptIn[0]);
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
//		pDC->LineTo(m_pt[1].x,m_pt[0].y);
//		pDC->LineTo(m_pt[0].x,m_pt[0].y);
		ptIn[0].x = m_pt[1].x;
		ptIn[0].y = m_pt[0].y;
		ptIn[1].x = m_pt[0].x;
		ptIn[1].y = m_pt[0].y;
		CaclTwoPointXD(ptIn[0],ptIn[1]);
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		break;
	case		TEXT_KIND	:	
		DrawText(pDC);
		break;
	case	BAG_LINE   :       

		//ZZHONG XIAN//
		if( m_pt[0].x==m_pt[1].x)
		{
			m_bLineValidate=false;
//			m_nZhongXian=0;
		}
		if((float)(m_pt[0].y-m_pt[1].y)*(m_pt[0].y-m_pt[1].y)<(float)1)
		{
			m_bLineValidate=false;
		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}

		if(m_nZhongXian==0)//BAG_LINE use m_nZhongXian also
		{
			CaclTwoPoint(m_pt[0], m_pt[1],0);
			if(ptLineSide[0].x == ptLineSide[1].x)
				break;

			pDC->MoveTo(ptLineSide[0].x ,ptLineSide[0].y);
			pDC->LineTo(ptLineSide[1].x ,ptLineSide[1].y);
		}
		else
		{
			//	计算间距
			double gap=10;
			if((m_pt[0].y-m_pt[1].y) ==0)
				break;

			gap=(double)FABSMY((m_pt[2].y*(m_pt[0].x-m_pt[1].x)-m_pt[1].y*m_pt[0].x+m_pt[0].y*m_pt[1].x)/(float)(m_pt[0].y-m_pt[1].y)-m_pt[2].x);
			gap=(double)((int)(gap/m_widthPer+0.5))*(double)m_widthPer;

			if(gap<3)
			{
				pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
				pDC->LineTo(m_pt[1].x ,m_pt[1].y);
				break;
			}
			for(int i=0;i<2000;i++ )
			{

				float y1=(float)m_pt[0].y;
				float x1=(float)(m_pt[0].x+i*gap);
				float y2=(float)m_pt[1].y;
				float x2=(float)(m_pt[1].x+i*gap);

				if(x1 == x2)
					continue;

				CaclTwoPointFloat(x1,y1,x2,y2);
	//			CaclTwoPoint(ptIn[0], ptIn[1],0);
				if(ptLineSide[0].y>m_rtDrawLine.bottom+2&& ptLineSide[1].y>m_rtDrawLine.bottom+2)
					continue;
				if(ptLineSide[0].y<m_rtDrawLine.top-2&& ptLineSide[1].y<m_rtDrawLine.top-2)
					continue;
				DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
			}
			;

			for(i=0;i<2000;i++ )
			{
				float y1=(float)m_pt[0].y;
				float x1=(float)(m_pt[0].x-i*gap);
				float y2=(float)m_pt[1].y;
				float x2=(float)(m_pt[1].x-i*gap);

				if(x1 == x2)
					continue;

				CaclTwoPointFloat(x1,y1,x2,y2);

				if(ptLineSide[0].y>m_rtDrawLine.bottom+2&& ptLineSide[1].y>m_rtDrawLine.bottom+2)
					continue;
				if(ptLineSide[0].y<m_rtDrawLine.top-2&& ptLineSide[1].y<m_rtDrawLine.top-2)
					continue;
				DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
			}
			;

		}

		break;
	case SPIRE_LINE:
		if(m_pt[0].x==m_pt[1].x&&m_pt[0].y==m_pt[1].y)
		{
			m_bLineValidate=false;
		}
//		if(m_pt[0].x>=pView->m_rtKlineFiguer .rightX &&m_pt[1].x>=pView->m_rtKlineFiguer .rightX)
//		{
//			m_bLineValidate=false;
//		}
//		if(m_pt[0].x<=0 &&m_pt[1].x<=0)
//		{
//			m_bLineValidate=false;
//		}
		if(	m_bLineValidate==false)
		{
			pDC->MoveTo(m_pt[0].x ,m_pt[0].y);
			pDC->LineTo(m_pt[1].x ,m_pt[1].y);
			break;
		}
		CaclTwoPointXD(m_pt[0],m_pt[1]);
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);

		//draw spire now
		{
			int flagPos[4][2] = {1,0, 0,-1,
				-1,0, 0,1};
			float rr = sqrt(double((m_pt[0].x-m_pt[1].x)*(m_pt[0].x-m_pt[1].x)+(m_pt[0].y-m_pt[1].y)*(m_pt[0].y-m_pt[1].y)));

			//compute begin position
			int nPos = 0;
			if(m_pt[0].y>= m_pt[1].y)
			{
				if(m_pt[0].x>m_pt[1].x)
					nPos = 1;
			}
			else
			{
				if(m_pt[0].x>m_pt[1].x)
					nPos = 2;
				else nPos =3;
			}

			//draw
			if(bSetProp == true)
			{
				CString sOut;
				sOut.Format ("%.2f",m_info_lineself[ this->m_nCurrentLine  ].valFlt1 );
				pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
				pDC->SetTextColor( pDoc->m_colorArray[1]);
				pDC->SetBkMode(TRANSPARENT);
				pDC->TextOut(m_pt[0].x,m_pt[0].y,sOut);
			}
			CPoint pb(m_pt[0].x+flagPos[nPos][0]*rr,m_pt[0].y+flagPos[nPos][1]*rr);//begin point
			CPoint pCenter = m_pt[0];
			int nComp = 0;
			while(1)
			{
//				int nPosPre = nPos;
				nComp++;
				nPos++;
				nPos = (nPos)%4;
				CPoint pe(pCenter.x+flagPos[nPos][0]*rr,pCenter.y+flagPos[nPos][1]*rr);//end point
				if(!pDC->IsPrinting ())
				{
					if(nComp>20  )
						break;
				}
				else
				{
					if(!this->m_rtDrawLine .PtInRect (pb) || !this->m_rtDrawLine .PtInRect (pe) )
						break;
				}
				CRect r(pCenter.x-rr,pCenter.y-rr,pCenter.x+rr,pCenter.y +rr);
				if(r.Width() >2 && r.Width ()< 32767 &&r.Height() >2 && r.Height ()< 32767 )
					pDC->Arc (r,pb,pe);
				else
					break;
//				break;
				float rrPre = rr;
				rr = 1.618*rr;
				rrPre = rr-rrPre;

				pb = pe;
				pCenter.x=pCenter.x-rrPre*flagPos[nPos][0];
				pCenter.y=pCenter.y-rrPre*flagPos[nPos][1];

			}
		}
		//end
		break;
	}
//	m_lineTypeOld=m_lineType;

//	CRgn rgn2;
//	if(rgn2.CreateRectRgn (0,0,pView->m_nMaxX ,pView->m_nMaxY )==ERROR   )
//		return -1;
	pDC->SelectObject(pOldPen);
	if(!pDC->IsPrinting())
	{
		pDC->SelectClipRgn (NULL);//&rgn2
		rgn.DeleteObject ();
	}
//	rgn2.DeleteObject ();

	return 1;

}
bool CTaiKlineDrawing::IsCrossSide(int nSide,CPoint &p1, CPoint &p2,CPoint &pOut)//nSide = 0 is top,1 is right,2 is bottom ,3 is left
{
	float p1_p2;
	if(p1.x>p2.x)
		p1_p2 = (float)(p1.x-p2.x);///m_widthPer+0.5))*(float)m_widthPer;
	else
		p1_p2 = -(float)(p2.x-p1.x);///m_widthPer+0.5))*(float)m_widthPer;

	switch(nSide)
	{
	case 0:
		if(p1.y-p2.y!=0)
			pOut.x=(int)((m_rtDrawLine.top-p1.y)*p1_p2/(float)(p1.y-p2.y)+p1.x);
		else
			pOut.x=-1;
		pOut.y=m_rtDrawLine.top;
		break;
	case 1:
		pOut.x=m_rtDrawLine.right ;
		if(p1_p2!=0)
			pOut.y=(int)(p1.y+(m_rtDrawLine.right-p1.x)*(p1.y-p2.y)/(float)p1_p2);
		else
			pOut.y=-1;
		break;
	case 2:
		if(p1.y-p2.y!=0)
			pOut.x=(int)((m_rtDrawLine.bottom-p1.y)*p1_p2/(float)(p1.y-p2.y)+p1.x);
		else
			pOut.x=-1;
		pOut.y=m_rtDrawLine.bottom;
		break;
	case 3:
		pOut.x=0;
		if(p1_p2!=0)
			pOut.y=(int)(p1.y+(0-p1.x)*(p1.y-p2.y)/(float)p1_p2);
		else
			pOut.y=-1;
		break;
	}

	CRect r(m_rtDrawLine.left,m_rtDrawLine.top,m_rtDrawLine.right+1,m_rtDrawLine.bottom+1);
	if(r.PtInRect (pOut))
		return true;
	return false;
}
bool CTaiKlineDrawing::IsCrossSideFloat(int nSide,float x1, float y1, float x2, float y2,CPoint &pOut)//nSide = 0 is top,1 is right,2 is bottom ,3 is left
{

	switch(nSide)
	{
	case 0:
		if(y1-y2!=0)
			pOut.x=(int)((m_rtDrawLine.top-y1)*(x1-x2)/(float)(y1-y2)+x1);
		else
			pOut.x=-1;
		pOut.y=m_rtDrawLine.top;
		break;
	case 1:
		pOut.x=m_rtDrawLine.right ;
		if(x1-x2!=0)
			pOut.y=(int)(y1+(m_rtDrawLine.right-x1)*(y1-y2)/(float)(x1-x2));
		else
			pOut.y=-1;
		break;
	case 2:
		if(y1-y2!=0)
			pOut.x=(int)((m_rtDrawLine.bottom-y1)*(x1-x2)/(float)(y1-y2)+x1);
		else
			pOut.x=-1;
		pOut.y=m_rtDrawLine.bottom;
		break;
	case 3:
		pOut.x=0;
		if(x1-x2!=0)
			pOut.y=(int)(y1+(0-x1)*(y1-y2)/(float)(x1-x2));
		else
			pOut.y=-1;
		break;
	}

	CRect r(m_rtDrawLine.left,m_rtDrawLine.top,m_rtDrawLine.right+1,m_rtDrawLine.bottom+1);
	if(r.PtInRect (pOut))
		return true;
	return false;
}
void CTaiKlineDrawing::CaclTwoPointFloat(float x1, float y1, float x2, float y2)
{
		if(x1==x2)
		{
			ptLineSide[0].x=(int)x1;
			ptLineSide[0].y=m_rtDrawLine.top;
			ptLineSide[1].x=(int)x2;//m_rtDrawLine.right ;
			ptLineSide[1].y=m_rtDrawLine.bottom;
		}
		else
		{//IsCrossSide(int nSide,CPoint &p1, CPoint &p2,CPoint &pOut)
			if(y1 == y2)
			{
				ptLineSide[0].x=0;
				ptLineSide[0].y=y1;
				ptLineSide[1].x=m_rtDrawLine.right ;
				ptLineSide[1].y=y2;
				return;
			}
			bool b = false;
			int k;
			for (k=0;k<4;k++)
			{
				if(IsCrossSideFloat(k,x1,y1,x2,y2,ptLineSide[0]) == true)
				{
					b = true;
					break;
				}
			}
			if(b == true)
			{
				b = false;
				int founded = k;
				for (k=0;k<4;k++)
				{
					if( k == founded )
						continue;
					if(IsCrossSideFloat(k,x1,y1,x2,y2,ptLineSide[1]) == true)
					{
						b = true;
						break;
					}
				}

				if(abs(ptLineSide[1].x- ptLineSide[0].x)<3 && abs(ptLineSide[1].y- ptLineSide[0].y)<3)
				{
					b = false;
					int founded2 = k;
					for (k=0;k<4;k++)
					{
						if( k == founded )
							continue;
						if( k == founded2 )
							continue;
						if(IsCrossSideFloat(k,x1,y1,x2,y2,ptLineSide[1]) == true)
						{
							b = true;
							break;
						}
					}
				}
			}
			if(b == false)
			{
				ptLineSide[0].x = -1;
				ptLineSide[0].y = -1;
				ptLineSide[1].x = -1;
				ptLineSide[1].y = -1;
			}
/*			ptLineSide[0].x=0;
			ptLineSide[0].y=(int)(p1.y+(0-p1.x)*(p1.y-p2.y)/(float)(p1.x-p2.x));
			ptLineSide[1].x=m_rtDrawLine.right ;
			ptLineSide[1].y=(int)(p1.y+(m_rtDrawLine.right-p1.x)*(p1.y-p2.y)/(float)(p1.x-p2.x));
*/		}

}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：CaclTwoPoint(CPoint &p1, CPoint &p2,int flag)
//      功能说明：计算直线与画图矩形的交点
//      入口参数：p1、p2为输入点，flag为0时是直线，为1时是射线
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDrawing::CaclTwoPoint(CPoint &p1, CPoint &p2,int flag)
//flag=0为直线，flag=1为射线,flag = 2 is xianduan
{
	int f=flag;
	if(flag==0)
	{
		if(p1.x==p2.x)
		{
			ptLineSide[0].x=p1.x;
			ptLineSide[0].y=m_rtDrawLine.top;
			ptLineSide[1].x=p2.x;//m_rtDrawLine.right ;
			ptLineSide[1].y=m_rtDrawLine.bottom;
			if(p1.x <= m_rtDrawLine.left || p1.x >= m_rtDrawLine.right )
			{
				ptLineSide[0].x = -1;
				ptLineSide[0].y = -1;
				ptLineSide[1].x = -1;
				ptLineSide[1].y = -1;
			}
		}
		else
		{//IsCrossSide(int nSide,CPoint &p1, CPoint &p2,CPoint &pOut)
			if(p1.y ==p2.y)
			{
				ptLineSide[0].x=0;
				ptLineSide[0].y=p1.y;
				ptLineSide[1].x=m_rtDrawLine.right ;
				ptLineSide[1].y=p2.y;
				if(p1.y <= m_rtDrawLine.top || p1.y >= m_rtDrawLine.bottom )
				{
					ptLineSide[0].x = -1;
					ptLineSide[0].y = -1;
					ptLineSide[1].x = -1;
					ptLineSide[1].y = -1;
				}
				return;
			}
			bool b = false;
			int k;
			for (k=0;k<4;k++)
			{
				if(IsCrossSide(k,p1, p2,ptLineSide[0]) == true)
				{
					b = true;
					break;
				}
			}
			if(b == true)
			{
				b = false;
				int founded = k;
				for (k=0;k<4;k++)
				{
					if( k == founded )
						continue;
					if(IsCrossSide(k,p1, p2,ptLineSide[1]) == true)
					{
						b = true;
						break;
					}
				}

				if(abs(ptLineSide[1].y- ptLineSide[0].y)<=4 &&abs(ptLineSide[1].x- ptLineSide[0].x)<=4 && b == true)
				{
					b = false;
					int founded2 = k;
					for (k=0;k<4;k++)
					{
						if( k == founded )
							continue;
						if( k == founded2 )
							continue;
						if(IsCrossSide(k,p1, p2,ptLineSide[1]) == true)
						{
							b = true;
							break;
						}
					}
				}
			}
			if(b == false)
			{
				ptLineSide[0].x = -1;
				ptLineSide[0].y = -1;
				ptLineSide[1].x = -1;
				ptLineSide[1].y = -1;
			}
/*			ptLineSide[0].x=0;
			ptLineSide[0].y=(int)(p1.y+(0-p1.x)*(p1.y-p2.y)/(float)(p1.x-p2.x));
			ptLineSide[1].x=m_rtDrawLine.right ;
			ptLineSide[1].y=(int)(p1.y+(m_rtDrawLine.right-p1.x)*(p1.y-p2.y)/(float)(p1.x-p2.x));
*/		}
	}
	else //if(flag == 1)
	{
		if(p1.x==p2.x)
			return;
		if(!m_rtDrawLine.PtInRect (p1))
		{
			CaclTwoPoint(p1, p2,0);
			return;
		}

		ptLineSide[0].x=p1.x;
		ptLineSide[0].y=p1.y;
//		ptLineSide[1].x=m_rtDrawLine.right ;
//		ptLineSide[1].y=(int)(p1.y+(m_rtDrawLine.right-p1.x)*(p1.y-p2.y)/(float)(p1.x-p2.x));
		bool b = false;
		int k;
		for (k=0;k<4;k++)
		{
			if(IsCrossSide(k,p1, p2,ptLineSide[1]) == true)
			{
				b = true;
				break;
			}
		}
		if(b == true)
		{
			//is the needed point?
			CPoint pTwo(-1,-1);
//			if(ptLineSide[1].x>p1.x)
			{
				pTwo.x = ptLineSide[1].x;
				pTwo.y = ptLineSide[1].y;
			}


			b = false;
			int founded = k;
			for (k=0;k<4;k++)
			{
				if( k == founded )
					continue;
				if(IsCrossSide(k,p1, p2,ptLineSide[1]) == true)
				{
					b = true;
					break;
				}
			}

			if(ptLineSide[1] == pTwo && b == true)//c
			{
				b = false;
				int founded2 = k;
				for (k=0;k<4;k++)
				{
					if( k == founded )
						continue;
					if( k == founded2 )
						continue;
					if(IsCrossSide(k,p1, p2,ptLineSide[1]) == true)
					{
						b = true;
						break;
					}
				}
			}

			//is the data valid?
			if(b == true)
			{
				if(ptLineSide[1].x  < pTwo.x)
				{
					ptLineSide[1].x = pTwo.x;
					ptLineSide[1].y = pTwo.y;
				}
				if(ptLineSide[1].x < ptLineSide[0].x)
					b = false;
			}
		}
		if(b == false)
		{
			ptLineSide[0].x = -1;
			ptLineSide[0].y = -1;
			ptLineSide[1].x = -1;
			ptLineSide[1].y = -1;
		}
	}
/*	else
	{
		if(m_rtDrawLine.PtInRect (p1))
		{
			if(m_rtDrawLine.PtInRect (p2))
			{
				ptLineSide[0].x = p1.x;
				ptLineSide[0].y = p1.y;
				ptLineSide[1].x = p2.x ;
				ptLineSide[1].y = p2.y ;
//				DrawXianDuan(pDC,p1,p2);
			}
			else
			{
//				CaclTwoPoint(p1, p2);
				if(p1.x ==  p2.x)
				{
					int yCrs=0;
					if(	p2.y<m_rtDrawLine.top)	
						yCrs = m_rtDrawLine.top;
					else
						yCrs = m_rtDrawLine.bottom;
					ptLineSide[0].x = p1.x;
					ptLineSide[0].y = p1.y;
					ptLineSide[1].x = p2.x ;
					ptLineSide[1].y = yCrs ;
				}
				else
				{
					int yCrs=0;
					if(	p2.y<m_rtDrawLine.top)	
						yCrs = m_rtDrawLine.top;
					else
						yCrs = m_rtDrawLine.bottom;
					ptLineSide[0].x = p1.x;
					ptLineSide[0].y = p1.y;
					ptLineSide[1].x = p2.x ;
					ptLineSide[1].y = yCrs ;
				}
			}
		}
		else if(m_rtDrawLine.PtInRect (p2))
		{
			CaclTwoPoint(p1, p2);
			if(p1.x ==  p2.x)
			{
				if(ptLineSide[0].y <= p1.y && ptLineSide[0].y >= p2.y
					||ptLineSide[0].y >= p1.y && ptLineSide[0].y <= p2.y)
					DrawXianDuan(pDC,p2,ptLineSide[0]);
				else if(ptLineSide[1].y <= p1.y && ptLineSide[1].y >= p2.y
					||ptLineSide[1].y >= p1.y && ptLineSide[1].y <= p2.y)
					DrawXianDuan(pDC,p2,ptLineSide[1]);
					
			}
			else
			{
				if(ptLineSide[0].x <= p1.x && ptLineSide[0].x >= p2.x
					||ptLineSide[0].x >= p1.x && ptLineSide[0].x <= p2.x)
					DrawXianDuan(pDC,p2,ptLineSide[0]);
				else if(ptLineSide[1].x <= p1.x && ptLineSide[1].x >= p2.x
					||ptLineSide[1].x >= p1.x && ptLineSide[1].x <= p2.x)
					DrawXianDuan(pDC,p2,ptLineSide[1]);
					
			}
		}
	}*/
}

/////////////////////////////////////////////////////////////////////
//      函 数 名：DrawHuiGuiTop(CDC *pDC,int flag)
//      功能说明：画回归带的上下两线
//      入口参数：flag为0时是线段，为1时是射线
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDrawing::DrawHuiGuiTop(CDC *pDC,int flag,COLORREF* clr)
{
	if((float)(m_pt[0].x-m_pt[1].x)==0)
			return;

	pView->pKlineDrawing ->m_nSon=0;
	pView->pKlineDrawing ->SetRectCurrent(0);
/*	pView->pKlineDrawing ->m_rectDrawLine.left=0;
	pView->pKlineDrawing ->m_rectDrawLine.right=m_rtDrawLine.right;
	pView->pKlineDrawing ->m_rectDrawLine.top=m_rtDrawLine.top;
	pView->pKlineDrawing ->m_rectDrawLine.bottom=m_rtDrawLine.bottom;*/

	int foot,footOld;
	int max,min,y;
	foot=(int)(m_pt[1].x/m_widthPer);
	footOld=(int)(m_pt[0].x/m_widthPer);

	if((m_pt[0].x-m_pt[1].x)!=0)
		y=(int)(m_pt[0].y+((footOld+0.5)*m_widthPer-m_pt[0].x)*(m_pt[0].y-m_pt[1].y)/(float)(m_pt[0].x-m_pt[1].x));
	else
		return;
	max=y-pView->pKlineDrawing ->YTransfer(pView->m_pkline[pView->pKlineDrawing ->m_footBegin+footOld].high);
	min=y-pView->pKlineDrawing ->YTransfer(pView->m_pkline[pView->pKlineDrawing ->m_footBegin+footOld].low);

	for(int i=footOld;i<=foot;i++)
	{

		y=(int)(m_pt[0].y+((i+0.5)*m_widthPer-m_pt[0].x)*(m_pt[0].y-m_pt[1].y)/(float)(m_pt[0].x-m_pt[1].x));
		if(max<y-pView->pKlineDrawing ->YTransfer((float)pView->m_pkline[pView->pKlineDrawing ->m_footBegin+i].high))
			max=y-pView->pKlineDrawing ->YTransfer((float)pView->m_pkline[pView->pKlineDrawing ->m_footBegin+i].high);
//		TRACE("max===    %d y===    %d\n",max,(int)(pView->pKlineDrawing ->YTransfer((float)pView->m_pkline[pView->pKlineDrawing ->m_footBegin+i].high)-y));
		if(min>y-pView->pKlineDrawing ->YTransfer(pView->m_pkline[pView->pKlineDrawing ->m_footBegin+i].low))
			min=y-pView->pKlineDrawing ->YTransfer(pView->m_pkline[pView->pKlineDrawing ->m_footBegin+i].low);
	}
	max=abs(max);
	min=abs(min);
//		int y=pView->pKlineDrawing ->YTransfer(yNow);
//	if(flag==0)
	CPoint p11,p22;
	bool bCan[]={true,true};
	{
		CPoint p1(m_pt[0].x,m_pt[0].y-max);
		CPoint p2(m_pt[1].x,m_pt[1].y-max);
		CaclTwoPointXD(p1,p2);
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		if(ptLineSide[0]==ptLineSide[1])
			bCan[0] = false;
		p11 = ptLineSide[1];

		CPoint p3(m_pt[0].x,m_pt[0].y+min);
		CPoint p4(m_pt[1].x,m_pt[1].y+min);
		CaclTwoPointXD(p3,p4);
		DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		if(ptLineSide[0]==ptLineSide[1])
			bCan[1] = false;
		p22 = ptLineSide[1];
//		pDC->MoveTo(m_pt[0].x ,m_pt[0].y-max);
//		pDC->LineTo(m_pt[1].x ,m_pt[1].y-max);
//		pDC->MoveTo(m_pt[0].x ,m_pt[0].y+min);
//		pDC->LineTo(m_pt[1].x ,m_pt[1].y+min);
	}
	if(flag==1 && bCan[0]==true&&bCan[1]==true)
	{
		CPoint p1(m_pt[0].x,m_pt[0].y-max);
		CPoint p2(m_pt[1].x,m_pt[1].y-max);
		CaclTwoPoint(p1, p2,1);
		if(ptLineSide[1].x<ptLineSide[0].x)
			ptLineSide[1] = ptLineSide[0];
		DrawXianDuanVirtul(pDC,p11,ptLineSide[1],clr);
		p1.y=m_pt[0].y+min;
		p2.y=m_pt[1].y+min;
		CaclTwoPoint(p1, p2,1);
		if(ptLineSide[1].x<ptLineSide[0].x)
			ptLineSide[1] = ptLineSide[0];
		DrawXianDuanVirtul(pDC,p22,ptLineSide[1],clr);

	}

}

/////////////////////////////////////////////////////////////////////
//      函 数 名：DrawArrowUp(UINT id,CDC* pDC)//
//      功能说明：画箭头
//      入口参数：id为位图ID号
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDrawing::DrawArrowUp(UINT id,CDC* pDC,int nPnt,int nWidth)//
{
	if(m_pt[0].x<=0)
		return;

	CPoint p;
	int gap = (int)11;
	p.x=m_pt[nPnt].x-(int)gap;
	p.y=m_pt[nPnt].y;

	CImageList list;
	list.Create (id,nWidth,1,RGB(255,255,255));
	list.Draw(pDC,0,p,ILD_TRANSPARENT);

} 

/////////////////////////////////////////////////////////////////////
//      函 数 名：DrawText(CDC* pDC)
//      功能说明：画文本
//      入口参数：pDC为设备描述表
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDrawing::DrawText(CDC* pDC,CPoint* pp,CString * s)
{
	if(m_pt[0].x<=0 && pp==NULL)
		return;

	int mapMode=0;

	pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
	pDC->SetTextColor( pDoc->m_colorArray[1]);
	pDC->SetBkMode(TRANSPARENT   );

	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);

	CPoint p1(m_pt[0].x,m_pt[0].y);
	CString s2 = m_textNow;
	if(pp!=NULL) 
	{
		p1 = *pp;
		s2 = *s;
	}
	pDC->ExtTextOut (p1.x,p1.y,ETO_CLIPPED,m_rtDrawLine ,s2,NULL);

	if(pp==NULL) 
		m_lenTextNow=(pDC->GetOutputTextExtent(s2)).cx;

	pDC->SelectObject(pOld);

}

/////////////////////////////////////////////////////////////////////
//      函 数 名：ScreenToFile(int n, bool isToFile)
//      功能说明：把屏幕坐标转换为相对位置
//      入口参数：n为数组的下标，isToFile为判断转换方向的标志字
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineDrawing::ScreenToFile(int n, bool isToFile,bool bPrinting)
{
	if(bPrinting==false)
		SetDrawLineRect();

	INFO_LINESELF lineslf;
	if(isToFile==false)
	{
		lineslf=m_info_lineself[ n ];
		if(lineslf.klineType !=pView->m_nKlineKind2 )
			return -1;
		if(lineslf.type==TEXT_KIND)
		{
			m_lineType=lineslf.type;
			int rtnFoot=TimeToX(lineslf.time1);
			if(rtnFoot==-1)
			{
				return -1;
			}
			float out;
			m_pt[0].y=FloatToY(lineslf.valFlt1,out);
			CString tmp((char*)&lineslf.time2);
			m_textNow=tmp;
			return 1;
		}
		else
		{
			m_lineType=lineslf.type;
			int i=0;
			int rtnFoot=TimeToX(lineslf.time1);
			if(rtnFoot==-1)
			{
				return -1;
			}
			float out;
			m_pt[0].y=FloatToY(lineslf.valFlt1,out);
			i++;
			if(i>=m_nPntGroup[lineslf.type])
				return 1;

			m_pt[1].x=(int)(m_pt[0].x+(lineslf.time2)*m_widthPer);
			m_pt[1].y=FloatToY(lineslf.valFlt2,out);
			i++;
			if(i>=m_nPntGroup[lineslf.type])
				return 2;
			m_pt[2].x=(int)(m_pt[0].x+(lineslf.time3)*m_widthPer);
			m_pt[2].y=FloatToY(lineslf.valFlt3,out);
			i++;
			return 3;
		}
	}
	else
	{
//		lineslf=m_info_lineself[ n ];
		/*typedef struct{
		int 	type;//线的类型
		int     klineType;//K线类型
		int		time[3];//各个点的时间值
		float	valFlt[3];//各个点的浮点数值
		int     stored[4];//预留
	}INFO_LINESELF;
	*/
		lineslf=m_info_lineself[ n ];
		lineslf.klineType =pView->m_nKlineKind2 ;

		if(m_lineType==TEXT_KIND)
		{
			lineslf.type = m_lineType;
			lineslf.time1=TimeToX(m_pt[0].x,1);
			FloatToY((float)m_pt[0].y,lineslf.valFlt1,1);

			memset((char*)&lineslf.time2,0,32);
			CString tmp;
			if(m_textNow.GetLength ()>=32)
			{
				tmp=m_textNow.Left(30);
				m_textNow=tmp;
			}
			memcpy(&lineslf.time2,m_textNow.GetBuffer (m_textNow.GetLength ()),m_textNow.GetLength ());
			m_info_lineself.SetAt ( n,lineslf);
			return 1;

		}
		else
		{
			lineslf.type = m_lineType;
/*			lineslf.type = m_lineType;
			lineslf.byAlarm = false;
			lineslf.byLineKind =0;
			lineslf.byLineWidth =0;
			lineslf.color = pDoc->m_colorArray[6];*/

			int i=0;
			lineslf.time1=TimeToX(m_pt[0].x,1);
			FloatToY((float)m_pt[0].y,lineslf.valFlt1,1);//FloatToY(float val,float& out,int flag)
			i++;
			if(i>=m_nPntGroup[lineslf.type])
			{
				m_info_lineself.SetAt ( n,lineslf);
				return 1;
			}

			//--四舍五入
			if(m_pt[1].x-m_pt[0].x>=0)
				lineslf.time2=(int)((m_pt[1].x-m_pt[0].x)/m_widthPer+0.5);
			else
				lineslf.time2=(int)((m_pt[1].x-m_pt[0].x)/m_widthPer-0.5);

			FloatToY((float)m_pt[1].y,lineslf.valFlt2,1);//FloatToY(float val,float& out,int flag)
			i++;
			if(i>=m_nPntGroup[lineslf.type])
			{
				m_info_lineself.SetAt ( n,lineslf);
				return 2;
			}

			//--四舍五入
			if(m_pt[2].x-m_pt[0].x>=0)
				lineslf.time3=(int)((m_pt[2].x-m_pt[0].x)/m_widthPer+0.5);
			else
				lineslf.time3=(int)((m_pt[2].x-m_pt[0].x)/m_widthPer-0.5);

			FloatToY((float)m_pt[2].y,lineslf.valFlt3,1);//FloatToY(float val,float& out,int flag)
			m_info_lineself.SetAt ( n,lineslf);
			return 3;
		}
	}
}

/////////////////////////////////////////////////////////////////////
//      函 数 名：DrawTotal(CDC *pDC)
//      功能说明：画所有画线
//      入口参数：pDC为设备描述表
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
static bool bWriteText=false;
void CTaiKlineDrawing::DrawTotal(CDC *pDC)
{
//	if(pDoc->m_propertyInitiate.bHideLine == TRUE && !((CMainFrame*)AfxGetMainWnd())->m_drawLineToolBar->IsWindowVisible())
//		return;

//	ScreenToFile(int n, bool isToFile)
	SetDrawLineRect();
	int mapMode=0;
	bool bPrinting=false;
/*	if(pDC->IsPrinting())
	{
		pDC->LPtoDP(&m_rtDrawLine);
		mapMode=pDC->SetMapMode(MM_TEXT);
		bPrinting=true;
	}*/

	if(pView->pKlineDrawing ->m_klinNumDefault ==0)
		return;

	m_widthPer=m_rtDrawLine.right/(float)pView->pKlineDrawing ->m_klinNumDefault;
	CPen pen_solid(PS_SOLID  ,1,pDoc->m_colorArray[6]);
	CPen* pOldpen=pDC->SelectObject(&pen_solid);

	int count=m_info_lineself.GetSize ();
	bWriteText=true;
	for(int i=0;i<count;i++)
	{
		if(m_info_lineself[ i ].klineType ==pView->m_nKlineKind2 )
		{
			int nR = ScreenToFile(i, false,bPrinting);
			m_nCurrentLine = i;
			if(nR<0)
				continue;

			if(m_info_lineself[ i ].klineType == MANY_DAY_KLINE && m_info_lineself[ i ].nDays != pDoc->m_propertyInitiate.daysOfManyKline)
				continue;

			if(m_lineType==ZHONG_XIAN||m_lineType==BAG_LINE)
				m_nZhongXian=1;
			DrawSelfLine(pDC,true);
			m_nZhongXian=0;
		}
	}
	if(m_bSelected==true&&m_nCurrentLineClicked<count&&m_nCurrentLineClicked>=0)
	{
		ScreenToFile(m_nCurrentLineClicked, false);
//		CTaiKlineDC m_memdc(pView,&pView->m_bit,&this->);
		DrawPointBE(pDC);
	}

	bWriteText=false;

	pDC->SelectObject(pOldpen);
	
/*	if(pDC->IsPrinting())
	{
		pDC->SetMapMode(mapMode);
		pDC->SetViewportExt(pView->m_xPrinter, pView->m_yPrinter);
		pDC->SetWindowExt(pView->m_cxExt, pView->m_cyExt);
	}
*/	
}
/////////////////////////////////////////////////////////////////////
//      函 数 名：IsOnSelfLineTotal(CPoint &point)
//      功能说明：输入点是否在自画线上
//      入口参数：point为输入点
//      出口参数：无出口数
//      返回参数：输入点在画线的位置
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineDrawing::IsOnSelfLineTotal(CPoint &point)
{
	int count=m_info_lineself.GetSize ();
	int rtn=-1;
	for(int i=0;i<count;i++)
	{
		if(m_info_lineself[ i ].klineType ==pView->m_nKlineKind2 )
		{
			ScreenToFile(i, false);
			rtn=IsOnSelfLine(point);
			if(rtn>=0)
			{
				m_nCurrentLine=i;
				m_lineType=m_info_lineself[i].type ;
				m_nLineTypeChanging=m_lineType;
				m_ptOldDrawing[0].x=m_pt[0].x;
				m_ptOldDrawing[0].y=m_pt[0].y;
				m_ptOldDrawing[1].x=m_pt[1].x;
				m_ptOldDrawing[1].y=m_pt[1].y;
				m_ptOldDrawing[2].x=m_pt[2].x;
				m_ptOldDrawing[2].y=m_pt[2].y;
				break;
			}
		}
	}
	return rtn;
}

/////////////////////////////////////////////////////////////////////
//      函 数 名：TimeToX(int time,int flag)
//      功能说明：把时间转换为X轴坐标或反之
//      入口参数：time为时间，flag为判断转换方向的标志字
//      出口参数：无出口数
//      返回参数：转换的数值
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineDrawing::TimeToX(int time,int flag)
{
	int foot=-1;
	if(flag==0)
	{
		foot=pView->pKlineDrawing ->FindTimeK(time,pView->m_pkline ,pView->m_nCountKline );
/*		for(int i=pView->pKlineDrawing ->m_footEnd;i>=0;i--)
		{
			if(pView->m_pkline[i].day<=time)
			{
				foot=i;
				break;
			}
		}*/
		if(foot>=0)
			m_pt[0].x=(int)((foot-pView->pKlineDrawing ->m_footBegin+0.5)*m_widthPer);
	}
	else//坐标转换为时间
	{
		foot=(int)(time/m_widthPer+pView->pKlineDrawing ->m_footBegin);
		if(foot>pView->m_nCountKline||foot<0)
		{
//			AfxMessageBox("画线出错",MB_ICONSTOP);
			foot=0;
		}
		else
			foot=pView->m_pkline[foot].day;
	}

	return foot;

}

/////////////////////////////////////////////////////////////////////
//      函 数 名：FloatToY(float val,float& out,int flag)
//      功能说明：把时间转换为X轴坐标或反之
//      入口参数：val为输入数，flag为判断转换方向的标志字
//      出口参数：out为输出浮点数
//      返回参数：转换的数值
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineDrawing::FloatToY(float val,float& out,int flag)
{
	int y=0;
	pView->pKlineDrawing ->m_nSon=0;
	pView->pKlineDrawing ->SetRectCurrent(0);

/*	pView->pKlineDrawing ->m_rectDrawLine.left=0;
	pView->pKlineDrawing ->m_rectDrawLine.right=m_rtDrawLine.right;
	pView->pKlineDrawing ->m_rectDrawLine.top=m_rtDrawLine.top;
	pView->pKlineDrawing ->m_rectDrawLine.bottom=m_rtDrawLine.bottom;*/


	if(flag==0)
	{
		y=pView->pKlineDrawing ->YTransfer(val);
	}
	else
	{
		int yIn=(int)val;
//		if(yIn<0)
//		{
//			out=0;
//			return y;
//		}
		out=pView->pKlineDrawing ->YTransfer(yIn);
	}
	return y;

}

/////////////////////////////////////////////////////////////////////
//      函 数 名：AddLine()
//      功能说明：加一条画线
//      入口参数：无
//      出口参数：无
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDrawing::AddLine()
{
	SetDrawLineRect();

	int count=m_info_lineself.GetSize ();

	if(m_lineType<0)
		return;

	if(count>=48)
		m_info_lineself.RemoveAt (count-1);//

	INFO_LINESELF lineslf;
	lineslf.type = m_lineType;
	lineslf.byAlarm = false;
	lineslf.byLineKind =0;
	lineslf.byLineWidth =0;
	lineslf.color = pDoc->m_colorArray[6];
	lineslf.nDays = pDoc->m_propertyInitiate.daysOfManyKline;
	m_info_lineself.InsertAt(0,lineslf);

	ScreenToFile(0, true);
	m_nCurrentLineClicked=0;//6.20
	m_nCurrentLine=0;//10.20

	WriteLineself();

}

void CTaiKlineDrawing::DeleteLine()
{
	SetDrawLineRect();
//m_nCurrentLine
	if(	m_bSelected==true)
	{
		int count=m_info_lineself.GetSize ();
		if(m_nCurrentLineClicked>=0&&m_nCurrentLineClicked<count)
		{
			m_info_lineself.RemoveAt (m_nCurrentLineClicked);//
			m_nCurrentLineClicked=-1;
			m_bSelected=false;
			CTaiKlineDC memdc(pView,&pView->m_bit,&m_rtDrawLine);
			pView->pKlineDrawing->DrawKlineFiguer(&memdc);

//			m_nCurrentLine=-1;//6.20
		}
	}

	WriteLineself();

}

/////////////////////////////////////////////////////////////////////
//      函 数 名：DeleteLineAll()
//      功能说明：删除所有画线
//      入口参数：无
//      出口参数：无
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDrawing::DeleteLineAll()
{
	if(	m_bSelected==true)
	{
		int count=m_info_lineself.GetSize ();
		if(m_nCurrentLineClicked<count)
			m_info_lineself.RemoveAt (m_nCurrentLineClicked);//
		CTaiKlineDC memdc(pView,&pView->m_bit,&m_rtDrawLine);
		pView->pKlineDrawing->DrawKlineFiguer(&memdc);
	}
	else if(m_bSelected==false)
	{
		int rtn=MessageBox(pView->m_hWnd ,"是否删除所有画线",g_strCompanyName,MB_YESNO |MB_ICONQUESTION);
		if(rtn==IDYES)//确定
		{
			m_info_lineself.RemoveAll ();//
			CTaiKlineDC memdc(pView,&pView->m_bit,&m_rtDrawLine);
			pView->pKlineDrawing->DrawKlineFiguer(&memdc);
		}
		else
		{
			return;
		}
	}
	m_bSelected=false;

	WriteLineself();
}


/////////////////////////////////////////////////////////////////////////////
//      函 数 名：SetPointHuiGui(int foot, int footOld)
//      功能说明：设置线性回归线的点
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDrawing::SetPointHuiGui(int foot, int footOld)
{
		if(footOld>=foot)
			return ;
		if(foot>pView->pKlineDrawing ->m_footEnd)
			return;

		//--		计算平均值
		float x=0;// X=(1/N) Σxi
		float y=0;// Y=(1/N) ΣYi
		int i;
		for(i=footOld;i<=foot;i++)
		{
			x+=i;
			y+=pView->m_pkline[pView->pKlineDrawing ->m_footBegin+i].close;
		}

		if((foot-footOld+1)==0)
			return;

		x=x/(foot-footOld+1);
		y=y/(foot-footOld+1);
/*根据一元回归分析的最小二乘法求N根日K线的线性回归。
Yi=K0+K1*Xi    I=1，…n,
K0=Y-K1*X
K1=(Σ(Xi-X)Yi/ Σ(Xi-X) (Xi-X)
其中: X=(1/N) Σxi
     Y=(1/N) ΣYi
     N=1,   N
*/
		float sumTop=0;
		float sumBottom=0;
		for(i=footOld;i<=foot;i++)
		{
			sumTop+=(i-x)*pView->m_pkline[pView->pKlineDrawing ->m_footBegin+i].close;
			sumBottom+=(i-x)*(i-x);
		}
		float k1=sumTop/sumBottom;
		float k0=y-k1*x;

		//---		计算开始点
		m_pt[0].x=(int)((footOld+0.5)*m_widthPer);
		float yNow=footOld*k1+k0;
		pView->pKlineDrawing ->m_nSon=0;
		pView->pKlineDrawing ->SetRectCurrent(0);
/*		pView->pKlineDrawing ->m_rectDrawLine.left=0;
		pView->pKlineDrawing ->m_rectDrawLine.right=pView->m_rtKlineFiguer.rightX;
		pView->pKlineDrawing ->m_rectDrawLine.top=pView->m_heightCaption;
		pView->pKlineDrawing ->m_rectDrawLine.bottom=pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-3;*/
		m_pt[0].y=pView->pKlineDrawing ->YTransfer(yNow);

		m_pt[1].x=(int)((foot+0.5)*m_widthPer);
		yNow=foot*k1+k0;
		m_pt[1].y=pView->pKlineDrawing ->YTransfer(yNow);



}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：ShowText(CDC* pDC,float in, int y)
//      功能说明：显示浮点数
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineDrawing::ShowText(CDC* pDC,float in, int y ,int nLine)
{
	if(bWriteText==false)
		return;
	bool bTop;
//	FloatToY((float)y,out,1);
//	if (nLine ==-1) return;

	INFO_LINESELF lineslf;
	lineslf=m_info_lineself[ m_nCurrentLine ];

	if(m_pt[0].y<=m_pt[1].y)
		bTop = false;
	else 
		bTop = true;
	//compute real value
	float out = (lineslf.valFlt2-lineslf.valFlt1 )*in+lineslf.valFlt1;

	int mapMode=0;

	pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
	pDC->SetTextColor( pDoc->m_colorArray[1]);
	pDC->SetBkMode(TRANSPARENT);

	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];
	//lgf.lfHeight=-12;
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);

	CString s;
	s.Format ("%.1f",in*100);
	s+="%(";
	CString s2;
	s2.Format ("%.2f",out);
	s=s+s2+")";

	if(bTop==true)
	{
		int top=pDC->GetOutputTextExtent(s).cy;
		CPoint p1(0,y-top-2);
		pDC->TextOut(p1.x,p1.y,s);
	}
	else
	{
		CPoint p2(0,y+2);
		pDC->TextOut(p2.x,p2.y,s);
	}
	pDC->SelectObject(pOld);

}


void CTaiKlineDrawing::ArcInRect(CDC *pDC, int x1, int y1, int x2, int y2)
{
	if(abs(x2-x1) >2 && abs(x2-x1)< 32767 &&abs(y2-y1)>2 &&abs(y2-y1)< 32767 ) 
	{
	if(!pDC->IsPrinting ())
	{
		pDC->Arc(x1,y1,x2,y2,(x1+x2)/2,y1,(x1+x2)/2,y1);
		return;
	}

	CPoint pt1(x1,y1);
	CPoint pt2(x2,y2);
	if(m_rtDrawLine.PtInRect (pt1) && m_rtDrawLine.PtInRect (pt2) )
		pDC->Arc(x1,y1,x2,y2,(x1+x2)/2,y1,(x1+x2)/2,y1);
	}
}

void CTaiKlineDrawing::CaclTwoPointXD(CPoint &p1, CPoint &p2)
{
 		CRect rDraw(m_rtDrawLine.left,m_rtDrawLine.top,m_rtDrawLine.right+1,m_rtDrawLine.bottom+1);
		if(rDraw.PtInRect (p1))
		{
			if(rDraw.PtInRect (p2))
			{
				ptLineSide[0].x=p1.x;
				ptLineSide[0].y=p1.y;
				ptLineSide[1].x=p2.x;
				ptLineSide[1].y=p2.y;
			}
			else
			{
				CaclTwoPoint(p1, p2);
				if(p1.x ==  p2.x)
				{
					CPoint tmp;
					if(ptLineSide[0].y < p1.y && ptLineSide[0].y >= p2.y
						||ptLineSide[0].y > p1.y && ptLineSide[0].y <= p2.y)
					{
//						tmp.x = ptLineSide[0].x;
//						tmp.y = ptLineSide[0].y;
						ptLineSide[1].x=p1.x;
						ptLineSide[1].y=p1.y;
					}
					else if(ptLineSide[1].y < p1.y && ptLineSide[1].y >= p2.y
						||ptLineSide[1].y > p1.y && ptLineSide[1].y <= p2.y)
					{
//						tmp.x = ptLineSide[1].x;
//						tmp.y = ptLineSide[1].y;
						ptLineSide[0].x=p1.x;
						ptLineSide[0].y=p1.y;
					}
					else
					{
						ptLineSide[0].x=-1;
						ptLineSide[0].y=-1;
						ptLineSide[1].x=-1;
						ptLineSide[1].y=-1;
					}
						
				}
				else
				{
					CPoint tmp;
					if(ptLineSide[0].x < p1.x && ptLineSide[0].x >= p2.x
						||ptLineSide[0].x > p1.x && ptLineSide[0].x <= p2.x)
					{
						ptLineSide[1].x=p1.x;
						ptLineSide[1].y=p1.y;
					}
					else if(ptLineSide[1].x < p1.x && ptLineSide[1].x >= p2.x
						||ptLineSide[1].x > p1.x && ptLineSide[1].x <= p2.x)
					{
						ptLineSide[0].x=p1.x;
						ptLineSide[0].y=p1.y;
					}
					else
					{
						ptLineSide[0].x=-1;
						ptLineSide[0].y=-1;
						ptLineSide[1].x=-1;
						ptLineSide[1].y=-1;
					}
						
				}
//			DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
			}
		}
		else if(rDraw.PtInRect (p2))
		{
			CaclTwoPoint(p1, p2);
			if(p1.x ==  p2.x)
			{
				CPoint tmp;
				if(ptLineSide[0].y <= p1.y && ptLineSide[0].y > p2.y
					||ptLineSide[0].y >= p1.y && ptLineSide[0].y < p2.y)
				{
//					tmp.x = ptLineSide[0].x;
//					tmp.y = ptLineSide[0].y;
					ptLineSide[1].x=p2.x;
					ptLineSide[1].y=p2.y;
//					ptLineSide[1].x=tmp.x;
//					ptLineSide[1].y=tmp.y;
				}
				else if(ptLineSide[1].y <= p1.y && ptLineSide[1].y > p2.y
					||ptLineSide[1].y >= p1.y && ptLineSide[1].y < p2.y)
				{
//					tmp.x = ptLineSide[1].x;
//					tmp.y = ptLineSide[1].y;
					ptLineSide[0].x=p2.x;
					ptLineSide[0].y=p2.y;
//					ptLineSide[0].x=tmp.x;
//					ptLineSide[0].y=tmp.y;
				}
				else
				{
					ptLineSide[0].x=-1;
					ptLineSide[0].y=-1;
					ptLineSide[1].x=-1;
					ptLineSide[1].y=-1;
				}
					
			}
			else
			{
				CPoint tmp;
				if(ptLineSide[0].x <= p1.x && ptLineSide[0].x > p2.x
					||ptLineSide[0].x >= p1.x && ptLineSide[0].x < p2.x)
				{
//					tmp.x = ptLineSide[0].x;
//					tmp.y = ptLineSide[0].y;//0 is valid
					ptLineSide[1].x=p2.x;
					ptLineSide[1].y=p2.y;
				}
				else if(ptLineSide[1].x <= p1.x && ptLineSide[1].x > p2.x
					||ptLineSide[1].x >= p1.x && ptLineSide[1].x < p2.x)
				{
//					tmp.x = ptLineSide[1].x;
//					tmp.y = ptLineSide[1].y;
					ptLineSide[0].x=p2.x;
					ptLineSide[0].y=p2.y;
				}
				else
				{
					ptLineSide[0].x=-1;
					ptLineSide[0].y=-1;
					ptLineSide[1].x=-1;
					ptLineSide[1].y=-1;
				}
					
			}
//			DrawXianDuan(pDC,ptLineSide[0],ptLineSide[1]);
		}
		else
		{
			CaclTwoPoint(p1, p2);

			if(ptLineSide[0].x ==-1 &&ptLineSide[0].y ==-1 &&ptLineSide[1].x ==-1 &&ptLineSide[1].y ==-1 )
				return;

			bool bValid = true;
			int xx1 = p1.x;
			int xx2 = p2.x;
			int tmp;
			if(xx1>xx2)
			{
				tmp = xx1;
				xx1 = xx2;
				xx2 = tmp;
			}
			int yy1 = p1.y;
			int yy2 = p2.y;
			if(yy1>yy2)
			{
				tmp = yy1;
				yy1 = yy2;
				yy2 = tmp;
			}
			CRect rXD(xx1,yy1,xx2+1,yy2+1);
			if(!rXD.PtInRect (ptLineSide[0]) || !rXD.PtInRect (ptLineSide[1]))
			{
				bValid = false;
			}
			if(bValid == false)
			{
				ptLineSide[0].x=-1;
				ptLineSide[0].y=-1;
				ptLineSide[1].x=-1;
				ptLineSide[1].y=-1;
			}
		}


}

bool CTaiKlineDrawing::GetCurrentLineProp(COLORREF &rgb, int &nLineType, int &nLineThick, bool &bAlarm)
{
	if(m_bSelected==true&&m_nCurrentLineClicked<m_info_lineself.GetSize()&&m_nCurrentLineClicked>=0)
	{
		INFO_LINESELF lineslf;
		lineslf=m_info_lineself[ m_nCurrentLineClicked ];
/*	COLORREF	color;//线的颜色
	BYTE	byLineKind;//线的种类:0是实线,1是虚线,2是点横线
	BYTE	byLineWidth;//线的宽度:0是1像素,1是2像素,2是3像素
	BYTE	byAlarm;//是否是条件预警,只对趋势线有效
*/		if(lineslf.type==TEXT_KIND) return false;

		rgb = lineslf.color;
		nLineType = (int)lineslf.byLineKind;
		nLineThick = (int)lineslf.byLineWidth;
		bAlarm = (bool)lineslf.byAlarm;
		
		if(lineslf.byLineKind>=3 || lineslf.byLineWidth>=3)
			return false;
		return true;
	}
	else return false;

}

void CTaiKlineDrawing::SetCurrentLineProp(COLORREF rgb, int nLineType, int nLineThick, bool bAlarm,int nParam)
{
//	GetCurrentLineProp(rgb, nLineType, nLineThick, bAlarm);
	if(m_bSelected==true&&m_nCurrentLineClicked<m_info_lineself.GetSize()&&m_nCurrentLineClicked>=0)
	{
		if(m_info_lineself[ m_nCurrentLineClicked ].type == TEXT_KIND) return ;

		switch(nParam)
		{
		case 0:
			m_info_lineself[ m_nCurrentLineClicked ].color		=	rgb		;
			break;
		case 1:
			m_info_lineself[ m_nCurrentLineClicked ].byLineKind	=		(BYTE)nLineType;
			break;
		case 2:
			m_info_lineself[ m_nCurrentLineClicked ].byLineWidth	=	(BYTE)nLineThick	;
			break;
		case 3:
			{
				if(pView->m_nCountKline<=0)
					break;
				m_info_lineself[ m_nCurrentLineClicked ].byAlarm	=	(BYTE)bAlarm	;
				if(bAlarm == true)//save to file
				{
					bool bUpwards = false;
					//compute the property of upwards
					;
					INFO_LINESELF& ln= m_info_lineself[ m_nCurrentLineClicked ];
					int n = pView->m_nCountKline - TimeToX(ln.time1,0);
					float fKln= pView->m_pkline[pView->m_nCountKline-1].close;
					if(n>0 &&ln.time2>0)
					{
						float fOnline = (ln.valFlt2-ln.valFlt1)/ln.time2*n+ln.valFlt1;
						if(fOnline>fKln)
							bUpwards = true;
						//end

						CAlertSystem::AddLineAlarmItem(m_info_lineself[ m_nCurrentLineClicked ],pView->m_sharesSymbol,bUpwards );//,bool bUpwards
					}
				}
				else//delete from file
					CAlertSystem::DeleteLineAlarmItem(m_info_lineself[ m_nCurrentLineClicked ],pView->m_sharesSymbol );//,bool bUpwards
			}
			break;
		}
	}
	WriteLineself();
}

void CTaiKlineDrawing::SetAlarmFlag(int nLine, BYTE bAlarm)
{
	ASSERT(bAlarm ==0);
	CAlertSystem::DeleteLineAlarmItem(m_info_lineself[ nLine ],pView->m_sharesSymbol );//,bool bUpwards
	m_info_lineself[ nLine ].byAlarm	=	(BYTE)bAlarm	;
}

bool CTaiKlineDrawing::IsTrendLine()
{
	INFO_LINESELF& ln= m_info_lineself[ m_nCurrentLineClicked ];
	if(ln.type == QU_SHI)
		return true;
	else
		return false;

}
