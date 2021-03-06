// CTaiKlineShowKline.cpp: implementation of the CTaiKlineShowKline class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiShanDoc.h"
#include "CTaiKlineDrawLine.h"
#include "CTaiKlineDialogShiDuanTJ.h"
#include "CTaiKlineDialogKLineMode.h"
#include "LBsInfoRS.h"
//#include "LDlgShowTJXG.h"
#include "CTaiKlineMoveChenben.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiKlineTransferKline.h"
#include "StructTaiShares.h"
#include "StructKlineView.h"
#include "CTaiScreensIndicate.h"
#include "CTaiScreenParent.h"
#include "CTaiShanReportView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//#define IDC_SCRBAR                       600666
#define STATICID                       600667

#define WIDTH_KLINEBODY                    6
#define LEN_KLINE                       10

#define CORLOR_ZUOBIAO                    RGB(0,0,0)
#define RIGHTBOX_PERLINE                    18

//#define pView->m_minGrid                  25
///////////////画图常量//////
//////////////////////////////////////////////////////////////////////
// CTaiKlineShowKline construction/destruction
//////////////////////////////////////////////////////////////////////
extern CTime g_timeUseEnd;
IMPLEMENT_DYNCREATE(CTaiKlineShowKline, CObject)

CTaiKlineShowKline::CTaiKlineShowKline()
{
	// TODO: add construction code here
	m_pMoveCB = NULL;
	m_pLmbTransferKline = NULL;
	m_bInvertRect = false;//是否把矩形反色

}
CTaiKlineShowKline::CTaiKlineShowKline(CTaiShanKlineShowView* bsView,CTaiShanDoc* bsDoc)
{
	// TODO: add construction code here
	int i;
	for(i =0;i<5;i++)
	{
		m_nValidIndicator[i] = 1;
		m_sIndicatorAdded[i] = "";
		m_nLineIndicator1[i] =0;
	}

	m_sOldSymbol = "";
	m_sOldStkKind = 0;

	m_nKlineType = -1;
	m_numOldKline = -1;
	m_pLmbTransferKline = new 	CTaiKlineTransferKline(&bsView->m_hs);

	m_nBeginFootTJ = 0;//时段统计的开始点
	m_nEndFootTJ = 0;//时段统计的结束点
	m_bInvertRect = false;//是否把矩形反色

	m_bSetPageData = true;
	m_pMoveCB = NULL;
	m_nTextBox=0;
	m_pMoveCB = new CTaiKlineMoveChenben(this,bsView,bsDoc);
	m_kLine_mode = 0;
	m_bNewStock = true;

	m_highFoot = 0;
	m_lowFoot  = 0 ;
	m_zhiBiao0="MA";

	m_bInitFoot=true;
	m_bClosed=false;
	m_bDrived=true;
	pView = bsView;
	pDoc=bsDoc;

//	m_pklineBak=NULL;//m_pklineBak
	m_nKlineCurrent=0;

	m_klinNumDefault=82;
	m_bToCacl=true;

	m_klinNumDefaultOld=0;
	m_footBeginOld=0;	
	m_footEndOld=0;		

	m_axisType=0;
//		分配技术指标数据的内存
	for(i=0;i<FIGUER_ALL_NUM;i++)
	{
		m_dataFormular[i].numLine =0;
		for(int j=0;j<Total_Count_Line;j++)
		{
			m_dataFormular[i].line[j].m_arrBE.b = -1;
			m_dataFormular[i].line[j].m_arrBE.line =NULL;// 
		}
	}
	m_nTextBox=0;

	for(int j=0;j<6;j++)
	{
		m_pKlineAdd[j]=new Kline[2];
	}

	//--	读除权数据
//	ReadPower(pView->m_sharesSymbol);
}
CTaiKlineShowKline::~CTaiKlineShowKline()
{//
	if(m_pLmbTransferKline!=NULL)
		delete m_pLmbTransferKline;
	if(m_pMoveCB!=NULL)
		delete m_pMoveCB;

	for(int j=0;j<6;j++)
	{
		delete[] m_pKlineAdd[j];
	}

//		释放技术指标数据的内存
	for(int i=0;i<FIGUER_ALL_NUM;i++)
	{
		for(int j=0;j<Total_Count_Line;j++)
		{
			if(m_dataFormular[i].line[j].m_arrBE.line!=NULL)
				delete[] m_dataFormular[i].line[j].m_arrBE.line;
		}
	}

}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CTaiKlineShowKline diagnostics

#ifdef _DEBUG
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：YTransfer(int y)
//      功能说明：屏幕坐标Y转换为实际数据
//      入口参数：y为传入的实际数据
//      出口参数：转换来的实际数据
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
float CTaiKlineShowKline::YTransfer(int y)//屏幕坐标Y转换为实际数据
{
	float rtn;
	if((m_rectDrawLine.top-m_rectDrawLine.bottom)==0)
		return 0;

	if(!(m_nSon==0&&m_axisType==2))
		rtn=((float)(m_max_sun[m_nSon]-m_min_sun[m_nSon]))*(y-m_rectDrawLine.bottom)/(m_rectDrawLine.top-m_rectDrawLine.bottom)+m_min_sun[m_nSon];
	else
	{
		rtn=(float)(pow(10,(log10(m_max_sun[m_nSon])-log10(m_min_sun[m_nSon]))
			*(y-m_rectDrawLine.bottom)/(m_rectDrawLine.top-m_rectDrawLine.bottom)
			+log10(m_min_sun[m_nSon])));
	}

	return rtn;
}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：YTransfer(float y)
//      功能说明：实际数据转换为屏幕坐标Y
//      入口参数：y为传入的实际数据
//      出口参数：转换来的实际数据
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::YTransfer(float y)//实际数据转换为屏幕坐标Y
{
	int rtn;
	if((m_max_sun[m_nSon]-m_min_sun[m_nSon])==0)
	{
		rtn = -10;
		return rtn;
	}

	if(!(m_nSon==0&&m_axisType==2))
		rtn=(int)((float)(m_rectDrawLine.top-m_rectDrawLine.bottom)*(y-m_min_sun[m_nSon])/(m_max_sun[m_nSon]-m_min_sun[m_nSon])+m_rectDrawLine.bottom+0.5);
	else
	{
		if(m_min_sun[m_nSon]<=0 || m_max_sun[m_nSon]<=0 || y<=0)
			return 0;

		rtn=(int)((m_rectDrawLine.top-m_rectDrawLine.bottom)*(log10(y)-log10(m_min_sun[m_nSon]))/(log10(m_max_sun[m_nSon])-log10(m_min_sun[m_nSon]))+m_rectDrawLine.bottom+0.5);
	}
	return rtn;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawLineIndex(CDC *pDC)
//      功能说明：画技术指标曲线
//      入口参数：pDC为传入的设备描述表指针
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DrawLineIndex(CDC *pDC,bool bFenshi)//
{
	if(m_max_sun[m_nSon]==m_min_sun[m_nSon])
	{
		if(pView->m_nKlineKind2==0 && strcmp(m_lineName[m_nSon][0],"成交量")==0)
			return;

		m_max_sun[m_nSon]+=0.001;
		m_min_sun[m_nSon]-=0.001;
//		return;
	}

	if(m_nSon == 0 && pView->m_nKlineKind2 >0)
	{
		if(m_rectDrawLine.left<2 && pView->m_infoInit.initIndex[0].nameIndex[0] =='\0' )
			return;
	}
	if(this==pView->pKlineDrawing )
	{
		if(!ValidDoKline()) return;
	}


	int leftf=m_rectDrawLine.left;
	int m_high=m_rectDrawLine.top;
	int low=m_rectDrawLine.bottom;
	int rightf=m_rectDrawLine.right;

	if(m_rectDrawLine.bottom-m_rectDrawLine.top<2)
		return;

	if(m_footEnd<m_footBegin)
		return ;

//	TRACE("klinejs  %d\n",m_klinNumDefault);
	///////////////////////////////////////////////////////////

	if(m_nSon>=FIGUER_ALL_NUM)
		return;

	//clip region
	CRgn rgn;

	if(!pDC->IsPrinting())
	{
		if(rgn.CreateRectRgn ( leftf, m_high-1, rightf, low+2 )==ERROR)
			return ;
		pDC->SelectClipRgn (&rgn);
//		pDC->BeginPath( );
	}

	int i;
	float cellWidth=(float)(m_rectDrawLine.right-m_rectDrawLine.left)/(m_klinNumDefault);
	float nMax = m_max_sun[m_nSon];
	float nMin = m_min_sun[m_nSon];

	if(m_nValidIndicator[m_nSon]<=0 && pView->m_nKlineKind2 >0 && m_rectDrawLine.left<=0)
	{
		pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		pDC->SetTextColor( pDoc->m_colorArray[2]);
		pDC->SetBkMode(TRANSPARENT);
		CFont ft;
		LOGFONT lgf=pDoc->m_fontstr[0];
		ft.CreateFontIndirect (&lgf );
		CFont* pOld=pDC->SelectObject(&ft);
		pDC->TextOut(leftf+20,m_high+20,"此指标不能在本时间周期显示！");
		pDC->SelectObject(pOld);
		goto END_DRAWLINE;
	}

	for( i=0;i<m_dataFormular[m_nSon].numLine ;i++)
	{
		int bgnFoot=m_dataFormular[m_nSon].line[i].bNow;
		if(bgnFoot<m_footBegin)
			bgnFoot=m_footBegin;
		if( i == m_nLineIndicator1[m_nSon] 
			&& pView->m_nKlineKind2>0 && bFenshi == false)
		{
			m_max_sun[m_nSon]=(float)-1.0E20;
			m_min_sun[m_nSon]=(float)1.0E20;

			bool bVolStick = false;
			for(int j=i;j<m_dataFormular[m_nSon].numLine ;j++)
			{
				if(	m_dataFormular[m_nSon].line[j].m_arrBE.b == -1 
					&& pView->m_nKlineKind2 >0 )
					continue;
				CaclMaxFlt(m_dataFormular[m_nSon].line[j].m_arrBE.line,bgnFoot,j);
/*				if(m_dataFormular[m_nSon].line[j].type==VOLSTICK)
					bVolStick = true;
				if(m_dataFormular[m_nSon].line[j].m_arrBE.kind == ARRAY_BE::DrawStickLine && m_dataFormular[m_nSon].line[j].m_arrBE.lineWidth)
				{
					int n = m_dataFormular[m_nSon].line[j].m_arrBE.looseArr.GetSize();
					for(int k = 0;k<n;k++)
					{
						int nFoot = m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].nFoot;
						if( nFoot<this->m_footBegin
							|| nFoot>this->m_footEnd )
							continue;

						float ftemp = m_dataFormular[m_nSon].line[j].m_arrBE.lineWidth[nFoot] ;
						if(ftemp>m_max_sun[m_nSon])
							m_max_sun[m_nSon]=ftemp;
						if(ftemp<m_min_sun[m_nSon])
							m_min_sun[m_nSon]=ftemp;
					}
				}*/
			}

			if(m_max_sun[m_nSon]==(float)-1.0E20)
				m_max_sun[m_nSon]=1;
			if(m_min_sun[m_nSon]==(float)1.0E20)
				m_min_sun[m_nSon]=1;

	//成交量的最小值为0
//			if(bVolStick && m_min_sun[m_nSon]>0) m_min_sun[m_nSon]=0;
		}
		if(	m_dataFormular[m_nSon].line[i].m_arrBE.b == -1 && pView->m_nKlineKind2 >0)
			continue;


		if(m_dataFormular[m_nSon].line[i].m_arrBE.kind != ARRAY_BE::Normal)
		{
			DrawTextIconLine(pDC,i);
			continue;
		}

//		判断画何种曲线
		int flag= m_dataFormular[m_nSon].line[i].type;
		if(pView->m_nKlineKind2==0)
			flag=0;
		if(strcmp(m_lineName[m_nSon][i],"成交量")==0)
			flag=STICK_VOL_FS;
		else if(strcmp(m_lineName[m_nSon][i],"买卖差")==0)
			flag=STICK;
		else if(strcmp(m_lineName[m_nSon][i],"红绿军")==0)
			flag=STICK_LINGXIAN_FS;

		COLORREF	clr;

		if(m_dataFormular[m_nSon].line[i].clr!= -1)
			clr=m_dataFormular[m_nSon].line[i].clr;
		else
			clr=pDoc->m_colorArray[i%6+7];
		if(flag == STICK_VOL_FS)
		{
			clr = pDoc->m_colorArray[1+7];
		}

		int nStyle=PS_SOLID;// {PS_SOLID,PS_DOT,PS_DASHDOTDOT};
		if(m_dataFormular[m_nSon].line[i].lineThick>=100)
			nStyle=PS_DOT;
		CPen pen(nStyle,m_dataFormular[m_nSon].line[i].lineThick%100,clr);

		CPen* pOldpen=pDC->SelectObject(&pen);


		int k,j=0;
		float j1;
		CBrush brush_yong;
		if (!brush_yong.CreateSolidBrush(pDoc->m_colorArray[3]))
			ASSERT(FALSE);
		CBrush* pOldBrush=pDC->SelectObject(&brush_yong);
		CPen p_yong(nStyle,m_dataFormular[m_nSon].line[i].lineThick%100,pDoc->m_colorArray[3]);
		CPen p_yin2(nStyle,m_dataFormular[m_nSon].line[i].lineThick%100,pDoc->m_colorArray[4]);
		int klineWidthPer,klineSide;
		float widthPer;

		//---		确定画何种线
		int xPre=-1;
		float yPre=0;

//		if(m_kLine_mode == 1)
//			if(!pDC->IsPrinting())
//				pDC->SelectClipRgn (&rgn);

//		CPen* pOldpen2;
		CString sss;
		switch(flag)
		{
		case LINESTICK://
		case STICK_VOL_FS:
			for(j=bgnFoot; j<=m_footEnd; j++ )
			{
				int x= (int)(m_rectDrawLine.left+(cellWidth+1)/2+(j-m_footBegin)*cellWidth);

//				if(flag==STICK_VOL_FS)
//					pDC->MoveTo(x,YTransfer((float)m_min_sun[m_nSon]));
//				else
					pDC->MoveTo(x,YTransfer((float)0));

				float y9 = (float)m_dataFormular[m_nSon].line[i].m_arrBE.line[j];

				pDC->LineTo(x,YTransfer(y9));

			}
			if(flag==STICK_VOL_FS)
				break;
		case 0:
			j1=m_dataFormular[m_nSon].line[i].m_arrBE.line[bgnFoot];
			pDC->MoveTo((int)(m_rectDrawLine.left+cellWidth/2+(bgnFoot-m_footBegin)*cellWidth),YTransfer(j1));
			for(j=bgnFoot; j<=m_footEnd; j++ )
			{
				int x= (int)(m_rectDrawLine.left+(cellWidth+1)/2+(j-m_footBegin)*cellWidth);
				float y9 = (float)m_dataFormular[m_nSon].line[i].m_arrBE.line[j];

				pDC->LineTo(x,	YTransfer(y9));

			}
			break;
		case VOLSTICK:
			widthPer =cellWidth;// klineWidthPer + 2 * klineSide;//每格的宽度
			klineWidthPer = (int)ceil(widthPer*3/5);//k线的宽度
			if(klineWidthPer>2&&klineWidthPer%2==0)
				klineWidthPer++;
			klineSide =  (int)ceil(widthPer/5);

			float close,open,vol;

			pDC->SelectObject (&p_yong);

			low = (int)(YTransfer((float)0) );
			if(low>m_rectDrawLine.bottom) low = m_rectDrawLine.bottom;


			if(cellWidth<5)
				for(k=bgnFoot; k<=m_footEnd; k++ )
				{
					int x= m_rectDrawLine.left +cellWidth/2+(int)( (k-(m_footBegin)) * (float)widthPer);

					open = pView->m_pkline[k].open;
					close = pView->m_pkline[k].close;
					vol = (m_dataFormular[m_nSon].line[i].m_arrBE.line[k]);

					if( vol == 0)
						continue;

					int volk = (int)(YTransfer((float)vol) );
					if(volk<m_rectDrawLine.top)
						volk = m_rectDrawLine.top;

					if( open - close > 0 )
					{
						pDC->SelectObject (&p_yin2);
						pDC->MoveTo (x,volk);
						pDC->LineTo (x,low);
					}
					else if( open - close < 0 )
					{
						pDC->SelectObject (&p_yong);
						pDC->MoveTo (x,volk);
						pDC->LineTo (x,low);
					}
					else 
					{
						if(k>0)
						{
							if(pView->m_pkline[k].close-pView->m_pkline[k-1].close>=0)
							{
								pDC->SelectObject (&p_yong);
								pDC->MoveTo (x,volk);
								pDC->LineTo (x,low);
							}
							else
							{
								pDC->SelectObject (&p_yin2);
								pDC->MoveTo (x,volk);
								pDC->LineTo (x,low);
							}
						}
						else if(k==0)
						{
								pDC->SelectObject (&p_yong);
								pDC->MoveTo (x,volk);
								pDC->LineTo (x,low);
						}
					}
							
				}
			else
				for(k=bgnFoot; k<=m_footEnd; k++ )
				{
					int x= m_rectDrawLine.left +cellWidth/2+(int)( (k-(m_footBegin)) * (float)widthPer);
	//				if(x==xPre)
	//					continue ;

					open = pView->m_pkline[k].open;
					close = pView->m_pkline[k].close;
					vol = (m_dataFormular[m_nSon].line[i].m_arrBE.line[k]);

					if( vol == 0)
						continue;


					int volk = (YTransfer((float)vol) );
					if(volk<m_rectDrawLine.top) volk = m_rectDrawLine.top;

					CRect rt(m_rectDrawLine.left +klineSide +(int)(  (k-(m_footBegin)) * (float)widthPer),volk,
							m_rectDrawLine.left +klineSide + (int)( (k-(m_footBegin)) * (float)widthPer)+klineWidthPer,low);
	//					int x= m_rectDrawLine.left +klineSide + klineWidthPer/2+(int)( (k-(m_footBegin)) * (float)widthPer);
					if( open - close > 0 )
						pDC->FillSolidRect (rt,pDoc->m_colorArray[4]);
					else if( open - close < 0 )
					{
						pDC->FillSolidRect (rt,pDoc->m_colorArray[0]);
	//						pDC->FrameRect (rt,&brush_yong);//bug
						pDC->MoveTo (rt.left,rt.top);//bug
						pDC->LineTo (rt.right,rt.top);//bug
						pDC->LineTo (rt.right,rt.bottom-1);//bug
						pDC->LineTo (rt.left,rt.bottom-1);//bug
						pDC->LineTo (rt.left,rt.top);//bug
					}
					else 
					{
						if(k>0)
						{
							if(pView->m_pkline[k].close-pView->m_pkline[k-1].close>=0)
							{
								pDC->FillSolidRect (rt,pDoc->m_colorArray[0]);
								pDC->MoveTo (rt.left,rt.top);//bug
								pDC->LineTo (rt.right,rt.top);//bug
								pDC->LineTo (rt.right,rt.bottom-1);//bug
								pDC->LineTo (rt.left,rt.bottom-1);//bug
								pDC->LineTo (rt.left,rt.top);//bug
							}
							else
								pDC->FillSolidRect (rt,pDoc->m_colorArray[4]);
						}
						else if(k==0)
						{
							pDC->FillSolidRect (rt,pDoc->m_colorArray[0]);
							pDC->MoveTo (rt.left,rt.top);//bug
							pDC->LineTo (rt.right,rt.top);//bug
							pDC->LineTo (rt.right,rt.bottom-1);//bug
							pDC->LineTo (rt.left,rt.bottom-1);//bug
							pDC->LineTo (rt.left,rt.top);//bug
						}
					}
							
					}
			break;

		case STICK://
			for(j=bgnFoot; j<=m_footEnd; j++ )
			{
				int x= (int)(m_rectDrawLine.left+(cellWidth+1)/2+(j-m_footBegin)*cellWidth);
//				if(x==xPre)
//					continue ;

				if(m_dataFormular[m_nSon].line[i].m_arrBE.line[j]>=0)
					pDC->SelectObject(&p_yong);
				else
					pDC->SelectObject(&p_yin2);

				int yTemp = YTransfer((float)0.0);

				pDC->MoveTo(x,yTemp);

				yTemp = YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[j]);

				pDC->LineTo(x,	yTemp);

			}
			break;

		case STICK_LINGXIAN_FS://
			yPre = YTransfer((float)pView->pMin1Drawing->m_close_Dapan[0]);
			for(j=bgnFoot; j<=m_footEnd; j++ )
			{
				int x= (int)(m_rectDrawLine.left+(cellWidth+1)/2+(j-m_footBegin)*cellWidth);

				if(m_dataFormular[m_nSon].line[i].m_arrBE.line[j]>=pView->pMin1Drawing->m_close_Dapan[0])
					pDC->SelectObject(&p_yong);
				else
					pDC->SelectObject(&p_yin2);


				pDC->MoveTo(x,yPre);
				pDC->LineTo(x,YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[j]));

			}
//			sss.Format("%.3f",pView->pMin1Drawing->m_close_Dapan[0]);
//			pDC->TextOut (50,50,sss);
			break;

		case CROSSDOT:
			for(j=bgnFoot; j<=m_footEnd; j++ )
			{
				int x= (int)(m_rectDrawLine.left+(cellWidth+1)/2+(j-m_footBegin)*cellWidth);
//				if(x==xPre)
//					continue ;

				int y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[j]);
//				int x=(int)(m_rectDrawLine.left+cellWidth/2+(j-m_footBegin)*cellWidth);

				if(y>low || y<m_high)
					continue;

				pDC->MoveTo(x-2,y-2);
				pDC->LineTo(x+2,y+2);
				pDC->MoveTo(x-2,y+2);
				pDC->LineTo(x+2,y-2);
//				xPre=x;
			}
			break;
		case CIRCLE://
			for(j=bgnFoot; j<=m_footEnd; j++ )
			{
				int x= (int)(m_rectDrawLine.left+(cellWidth+1)/2+(j-m_footBegin)*cellWidth);
//				if(x==xPre)
//					continue ;

				if((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[j]
					>pView->m_pkline[j].close)
				{
//					pDC->SelectObject(&p_yin2);
				

					int y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[j]);
					if(y>low || y<m_high)
						continue;
					pDC->Arc(x-2,y-2,x+2,y+2,x+2,y+2,x+2,y+2);
				}
				else
				{
//					pDC->SelectObject(&p_yong);
					int y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[j]);
					if(y>low || y<m_high)
						continue;

					pDC->Arc(x-2,y-2,x+2,y+2,x+2,y+2,x+2,y+2);
				}

			}
			break;
		case BUY_CASE://
			for(j=bgnFoot; j<=m_footEnd; j++ )
			{
				int x= (int)(m_rectDrawLine.left+(cellWidth+1)/2+(j-m_footBegin)*cellWidth);
				if((int)m_dataFormular[m_nSon].line[i].m_arrBE.line[j] != 0)
				{
					int y=YTransfer((float)m_dataFormular[m_nSon].line[0].m_arrBE.line[j]);
					y+=6;
					if(y>low-16) y= low -16;
					if(y<m_high+15) y= m_high+15;

					CPoint p(x-6,y);
					OutBitmap(pDC,IDB_40,p);
				}
			}
			break;//

		case SELL_CASE://
			for(j=bgnFoot; j<=m_footEnd; j++ )
			{
				int x= (int)(m_rectDrawLine.left+(cellWidth+1)/2+(j-m_footBegin)*cellWidth);
				if((int)m_dataFormular[m_nSon].line[i].m_arrBE.line[j] != 0)
				{
					int y=YTransfer((float)m_dataFormular[m_nSon].line[0].m_arrBE.line[j]);
					y-=6;

					if(y>low-16) y= low -16;
					if(y<m_high+15) y= m_high+15;

					CPoint p(x-6,y);
					OutBitmap(pDC,IDB_41,p);
				}
			}
			break;
		}
		pDC->SelectObject (pOldBrush);
		pDC->SelectObject(pOldpen);


	}
	m_max_sun[m_nSon] = nMax;
	m_min_sun[m_nSon] = nMin;

END_DRAWLINE:
	if(!pDC->IsPrinting())
	{
//			pDC->EndPath( );
//			pDC->SelectClipPath (RGN_AND);

		pDC->SelectClipRgn (NULL);//&rgn2
		rgn.DeleteObject ();
	}
}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawKLine(CDC *pDC)
//      功能说明：把K线画到内存位图上
//      入口参数：pDC为传入的设备描述表指针
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DrawKLine(CDC *pDC)//把K线画到内存位图上
{
		if(m_nSon==0)
		{
			if(m_min_sun[m_nSon]<=0 || m_max_sun[m_nSon]<=0)
			{
				if(m_axisType==2)
				{
					m_axisType=0;
				}
			}
			if(m_min_sun[m_nSon] == m_max_sun[m_nSon])
				return;
		}
		if(m_rectDrawLine.bottom-m_rectDrawLine.top<2)
			return;
		if(!ValidDoKline()) return;


		int leftf=m_rectDrawLine.left;
		int topf=m_rectDrawLine.top;
		int bottomf=m_rectDrawLine.bottom;
		int rightf=m_rectDrawLine.right;

		
		//clip region
		CRgn rgn;
		if(!pDC->IsPrinting())
		{
			if(rgn.CreateRectRgn ( leftf, topf, rightf, bottomf+2 )==ERROR)
				return ;
			pDC->SelectClipRgn (&rgn);
		}

		//ext data tell
		int wdth=rightf-leftf;
		int numberKline=m_klinNumDefault;
		///////////////////////////////////////////////////////////
		float widthPer =wdth/(float)(numberKline);// klineWidthPer + 2 * klineSide;//每格的宽度
		if(m_footBegin<pView->m_nCountKlineToday && m_footEnd>=pView->m_nCountKlineToday)
		{//   
			CPen pen(PS_DASHDOT,1,pDoc->m_colorArray[2]);
			CPen* pOldp=pDC->SelectObject(&pen);
			int x =(int)( (pView->m_nCountKlineToday - m_footBegin)*widthPer);
			pDC->MoveTo(x,topf);
			pDC->LineTo (x,bottomf);
			pDC->SelectObject(pOldp);
		}
		//
		if(pView->m_nKlineKind2 ==HS_KLINE)//
		{
			DrawKlineHS(pDC);
			if(!pDC->IsPrinting())
			{
				pDC->SelectClipRgn (NULL);//&rgn2
				rgn.DeleteObject ();
			}
			return;
		}
		if(pDoc->m_systemOption.kline==1)//
		{
			DrawKLineAmeric(pDC);
			if(!pDC->IsPrinting())
			{
				pDC->SelectClipRgn (NULL);//&rgn2
				rgn.DeleteObject ();
			}
			return;
		}
		if(pDoc->m_systemOption.kline==2)//	m_bBTX == true)
		{
			DrawBTX(pDC);
			if(!pDC->IsPrinting())
			{
				pDC->SelectClipRgn (NULL);//&rgn2
				rgn.DeleteObject ();
			}
			return;
		}

//		float cellWidth=(float)(rightf-leftf)/m_klinNumDefault;
		int klineWidthPer = (int)(widthPer*3/10);//k线的宽度

		CPen pen_ping(PS_SOLID,1,pDoc->m_colorArray[2]);
		CPen pen_yong(PS_SOLID,1,pDoc->m_colorArray[3]);
		CPen pen_yin(PS_SOLID,1,pDoc->m_colorArray[4]);
		CPen* pOldpen=pDC->SelectObject(&pen_yong);

		CBrush brush_yong;
		if (!brush_yong.CreateSolidBrush(pDoc->m_colorArray[3]))
			ASSERT(FALSE);
		CBrush* pOldBrush=pDC->SelectObject(&brush_yong);

		for(int i=m_footBegin; i<=m_footEnd; i++ )
		{
			if( pView->m_pkline[i].close<=0||pView->m_pkline[i].high <= 0 )
			{
				if(m_axisType==2)
					break;
				continue;
			}
			int x= leftf +(int)( (i-(m_footBegin)+0.5) * (float)widthPer);

			float close,open,high,low;
			open = pView->m_pkline[i].open;
			close = pView->m_pkline[i].close;
			high = pView->m_pkline[i].high;
			low = pView->m_pkline[i].low;
			int closek = (int)(YTransfer(close) );
			int openk = (int)(YTransfer(open) );
			int highk = (int)YTransfer(high);//( hght * ( m_max_sun[m_nSon] - high ) / fudu_price ) + topf;
			int lowk = (int)YTransfer(low);//( hght * ( m_max_sun[m_nSon]-low ) / fudu_price ) + topf;

//				int flag=0;//0	yin;1	yong;2	ping
			CRect rt;
			rt.top =closek;
			rt.bottom =openk;
//				int x= leftf +(int)( (i-(m_footBegin)+0.5) * (float)widthPer);
			if(klineWidthPer<1)
			{
				rt.left=leftf +(int)(  (i-(m_footBegin)+0.5) * (float)widthPer);
				rt.right =leftf + (int)( (i-(m_footBegin)+0.5) * (float)widthPer);
				if( open - close > 0 )
				{
					pDC->SelectObject (&pen_yin);
					pDC->MoveTo (x,highk);
					pDC->LineTo (x,lowk);
				}
				else if( open - close < 0 )
				{
					pDC->SelectObject (&pen_yong);
					pDC->MoveTo (x,highk);
					pDC->LineTo (x,lowk);
				}
				else 
				{
					if(i>0)
					{
						if(pView->m_pkline[i].close-pView->m_pkline[i-1].close>=0)
							pDC->SelectObject (&pen_yong);
						else
							pDC->SelectObject (&pen_yin);
					}
					else if(i==0)
						pDC->SelectObject (&pen_yong);
					pDC->MoveTo (x,highk);
					pDC->LineTo (x,lowk);
				}
				continue;
			}

			rt.left=leftf -klineWidthPer +(int)(  (i-(m_footBegin)+0.5) * (float)widthPer);
			rt.right =leftf +klineWidthPer + (int)( (i-(m_footBegin)+0.5) * (float)widthPer);
			if( open - close > 0 )
			{
				pDC->SelectObject (&pen_yin);
				pDC->MoveTo (x-(int)(klineWidthPer),openk);
				pDC->LineTo (x-(int)(klineWidthPer),closek);
				pDC->LineTo (x+(int)(klineWidthPer),closek);
				pDC->LineTo (x+(int)(klineWidthPer),openk);
				pDC->LineTo (x-(int)(klineWidthPer),openk);

				pDC->MoveTo (x-(int)(klineWidthPer),openk);
				pDC->LineTo (x+(int)(klineWidthPer),openk);
				pDC->MoveTo (x,highk);
				pDC->LineTo (x,lowk);
				pDC->FillSolidRect (rt,pDoc->m_colorArray[4]);
			}
			else if( open - close < 0 )
			{
//					pDC->FrameRect (rt,&brush_yong);
				pDC->FillSolidRect (rt,pDoc->m_colorArray[0]);
				pDC->SelectObject (&pen_yong);
				pDC->MoveTo (x-(int)(klineWidthPer),openk);
				pDC->LineTo (x-(int)(klineWidthPer),closek);
				pDC->LineTo (x+(int)(klineWidthPer),closek);
				pDC->LineTo (x+(int)(klineWidthPer),openk);
				pDC->LineTo (x-(int)(klineWidthPer),openk);

				pDC->MoveTo (x,highk);
				pDC->LineTo (x,closek);
				pDC->MoveTo (x,lowk);
				pDC->LineTo (x,openk-1);
			}
			else 
			{
				if(i>0)
				{
					if(pView->m_pkline[i].close-pView->m_pkline[i-1].close>=0)
						pDC->SelectObject (&pen_yong);
					else
						pDC->SelectObject (&pen_yin);
				}
				else if(i==0)
					pDC->SelectObject (&pen_yong);

				pDC->MoveTo (x,highk);
				pDC->LineTo (x,lowk);
				pDC->MoveTo (x-(int)(klineWidthPer),openk);
				pDC->LineTo (x+(int)(klineWidthPer),openk);
			}

		}
		pDC->SelectObject (pOldBrush);

		//write high price and low price
		CFont ft;
		LOGFONT lgf=pDoc->m_fontstr[0];
		ft.CreateFontIndirect (&lgf );
		CFont* pftOld = pDC->SelectObject(&ft);
		pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
		pDC->SetTextColor(  pDoc->m_colorArray[1]);
		pDC->SetBkMode(TRANSPARENT);

		int leftHL=(int)( (m_highFoot-(m_footBegin)+0.5) * (float)widthPer)+m_rectDrawLine.left;
		int topHL=YTransfer(pView->m_pkline[m_highFoot].high);
		CString sHL;
		if(pView->m_pkline[m_highFoot].high>=10)
			sHL.Format("%.2f",pView->m_pkline[m_highFoot].high);
		else
			sHL.Format("%.3f",pView->m_pkline[m_highFoot].high);
		int nLenTxt = (pDC->GetOutputTextExtent(sHL)).cx;
		leftHL -= 15;
		if(leftHL - nLenTxt < 10) leftHL = 15+nLenTxt;
/*		if(leftHL>(m_rectDrawLine.right+m_rectDrawLine.left)/2)
			pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
		else
			pDC->SetTextAlign( TA_LEFT );//TA_CENTER||*/
		pDC->ExtTextOut (leftHL,topHL ,ETO_CLIPPED,m_rectDrawLine,sHL,NULL);

		leftHL=(int)( (m_lowFoot-(m_footBegin)+0.5) * (float)widthPer)+m_rectDrawLine.left;
		leftHL -= 15;
		topHL=YTransfer(pView->m_pkline[m_lowFoot].low);
		if(pView->m_pkline[m_lowFoot].low>=10)
			sHL.Format("%.2f",pView->m_pkline[m_lowFoot].low);
		else
			sHL.Format("%.3f",pView->m_pkline[m_lowFoot].low);
		nLenTxt = (pDC->GetOutputTextExtent(sHL)).cx;
		if(leftHL - nLenTxt < 10) leftHL = 15+nLenTxt;
/*		if(leftHL>(m_rectDrawLine.right+m_rectDrawLine.left)/2)
			pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
		else
			pDC->SetTextAlign( TA_LEFT );//TA_CENTER||*/
		topHL-=((pDC->GetOutputTextExtent(sHL)).cy+3);
		pDC->ExtTextOut (leftHL,topHL ,ETO_CLIPPED,m_rectDrawLine,sHL,NULL);
		pDC->SelectObject(pftOld);
		
		if(!pDC->IsPrinting())
		{
			pDC->SelectClipRgn (NULL);//&rgn2
			rgn.DeleteObject ();
		}
		pDC->SelectObject (pOldpen);
}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawKLineAmeric(CDC *pDC)
//      功能说明：把K线画到内存位图上
//      入口参数：pDC为传入的设备描述表指针
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DrawKLineAmeric(CDC *pDC)//把K线画到内存位图上
{
		int leftf=m_rectDrawLine.left;
		int topf=m_rectDrawLine.top;
		int bottomf=m_rectDrawLine.bottom;
		int rightf=m_rectDrawLine.right;

		if(m_rectDrawLine.bottom-m_rectDrawLine.top<2)
			return;
		if(!ValidDoKline()) return;

		int wdth=rightf-leftf;
		int numberKline=m_klinNumDefault;
		///////////////////////////////////////////////////////////
		float widthPer =wdth/(float)(numberKline);// klineWidthPer + 2 * klineSide;//每格的宽度
		int klineWidthPer = (int)(widthPer*3/10);//k线的宽度

		float close,open,high,low;
		CPen pen_yong(PS_SOLID,1,pDoc->m_colorArray[5]);
		CPen* pOldpen=pDC->SelectObject(&pen_yong);

		for(int i=m_footBegin; i<=m_footEnd; i++ )
		{
			int x= leftf +(int)( (i-(m_footBegin)+0.5) * (float)widthPer);

			open = pView->m_pkline[i].open;
			close = pView->m_pkline[i].close;
			high = pView->m_pkline[i].high;
			low = pView->m_pkline[i].low;

			if( pView->m_pkline[i].close>0 && pView->m_pkline[i].high > 0 )
			{
				int closek = (int)(YTransfer(close) );
				int openk = (int)(YTransfer(open) );
				int highk = (int)YTransfer(high);//( hght * ( m_max_sun[m_nSon] - high ) / fudu_price ) + topf;
				int lowk = (int)YTransfer(low);//( hght * ( m_max_sun[m_nSon]-low ) / fudu_price ) + topf;


//				int flag=0;//0	yin;1	yong;2	ping
				CRect rt;
				rt.top =closek;
				rt.bottom =openk;
//				int x= leftf +(int)( (i-(m_footBegin)+0.5) * (float)widthPer);
				if(klineWidthPer<1)
				{
					rt.left=leftf +(int)(  (i-(m_footBegin)+0.5) * (float)widthPer);
					rt.right =leftf + (int)( (i-(m_footBegin)+0.5) * (float)widthPer);
					pDC->MoveTo (x,highk);
					pDC->LineTo (x,lowk);
				}
				else
				{
					rt.left=leftf -klineWidthPer +(int)(  (i-(m_footBegin)+0.5) * (float)widthPer);
					rt.right =leftf +klineWidthPer + (int)( (i-(m_footBegin)+0.5) * (float)widthPer);
				
					pDC->MoveTo (x-(int)(klineWidthPer),openk);
					pDC->LineTo (x,openk);
					pDC->MoveTo (x+(int)(klineWidthPer),closek);
					pDC->LineTo (x,closek);

					pDC->MoveTo (x,highk);
					pDC->LineTo (x,lowk);
				}
			}
			else
			{
				if(m_axisType==2)
					break;
			}
		}

		//write high price and low price
		CFont ft;
		LOGFONT lgf=pDoc->m_fontstr[0];
		ft.CreateFontIndirect (&lgf );
		CFont* pftOld = pDC->SelectObject(&ft);
		pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		pDC->SetTextColor(  pDoc->m_colorArray[1]);
		pDC->SetBkMode(TRANSPARENT);

		int leftHL=(int)( (m_highFoot-(m_footBegin)+0.5) * (float)widthPer);
		int topHL=YTransfer(pView->m_pkline[m_highFoot].high);
		CString sHL;
		sHL.Format("%.2f",pView->m_pkline[m_highFoot].high);
		if(leftHL>(m_rectDrawLine.right+m_rectDrawLine.left)/2)
			pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
		else
			pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		pDC->ExtTextOut (leftHL,topHL ,ETO_CLIPPED,m_rectDrawLine,sHL,NULL);

		leftHL=(int)( (m_lowFoot-(m_footBegin)+0.5) * (float)widthPer);
		topHL=YTransfer(pView->m_pkline[m_lowFoot].low);
		sHL.Format("%.2f",pView->m_pkline[m_lowFoot].low);
		if(leftHL>(m_rectDrawLine.right+m_rectDrawLine.left)/2)
			pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
		else
			pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		topHL-=((pDC->GetOutputTextExtent(sHL)).cy+3);
		pDC->ExtTextOut (leftHL,topHL ,ETO_CLIPPED,m_rectDrawLine,sHL,NULL);
		pDC->SelectObject(pftOld);
		
		pDC->SelectObject (pOldpen);

}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawKLineAdded(CDC *pDC)
//      功能说明：把K线画到内存位图上
//      入口参数：pDC为传入的设备描述表指针
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DrawKLineAdded(CDC *pDC,int nKln)//把K线画到内存位图上
{
		int leftf=0;
		int topf=pView->m_heightCaption;
		int bottomf=pView->m_rtKlineFiguer.rtBlw[0].m_yBottom - 3 ;
		int rightf=pView->m_rtKlineFiguer.rightX;
		if(bottomf-topf<2)
			return;

		//clip region
		CRgn rgn;
		if(!pDC->IsPrinting())
		{
			if(rgn.CreateRectRgn ( leftf, topf, rightf, bottomf )==ERROR)
				return ;
			pDC->SelectClipRgn (&rgn);
		}


		if(pDoc->m_systemOption.kline==1)//
		{
			DrawKLineAmericaAdded(pDC,nKln);
			if(!pDC->IsPrinting())
			{
				pDC->SelectClipRgn (NULL);//&rgn2
				rgn.DeleteObject ();
			}
			return;
		}
		if(pDoc->m_systemOption.kline==2)//
		{
			DrawBTXAdded(pDC, nKln);
			if(!pDC->IsPrinting())
			{
				pDC->SelectClipRgn (NULL);//&rgn2
				rgn.DeleteObject ();
			}
			return;
		}
//		float cellWidth=(float)(rightf-leftf)/m_klinNumDefault;
		int wdth=rightf-leftf;
		int numberKline=m_klinNumDefault;
		///////////////////////////////////////////////////////////
		float widthPer =wdth/(float)(numberKline);// klineWidthPer + 2 * klineSide;//每格的宽度
		int klineWidthPer =(int) (widthPer*3/10);//k线的宽度

		float close,open,high,low;

		CPen pen_yong(PS_SOLID,1,pDoc->m_colorArray[7+nKln]);
		CPen* pOldpen=pDC->SelectObject(&pen_yong);
		CBrush brush_yong;
		if (!brush_yong.CreateSolidBrush(pDoc->m_colorArray[3]))
			ASSERT(FALSE);
		CBrush* pOldBrush=pDC->SelectObject(&brush_yong);
		
		int bAdd=0;
		int eAdd=0;
		int i=0;
		int j=0;
		float moveVla=0;
//		TRACE("CaclBeginFoot\r\n");
		bAdd=CaclBeginFoot(nKln);
		if(bAdd==-1)
			goto END_ADDED;
//		TRACE("CaclEndFoot\r\n");
		eAdd=CaclEndFoot(nKln);
		if(eAdd==-1)
			goto END_ADDED;

		j=m_footBegin;
		if(m_axisType==1)
			moveVla=m_pKlineAdd[nKln][bAdd].open-pView->m_pkline[m_footBegin].open;
		if(m_pKlineAdd[nKln][bAdd].open<=0)
			goto END_ADDED;

//		if(bAdd<200)
//			bAdd = bAdd;
//		TRACE("CaclEndFoot for bAdd=%d eAdd = %d\r\n",bAdd,eAdd);
		for(i=bAdd; i<=eAdd; i++ )
		{
//			TRACE("CaclEndFoot for i = %d\r\n",i);
//			if(i==2090)
//				i=2090;
			if(m_axisType==1)
			{
				open = (m_pKlineAdd[nKln][i].open-m_pKlineAdd[nKln][bAdd].open)*pView->m_pkline[m_footBegin].open/m_pKlineAdd[nKln][bAdd].open+pView->m_pkline[m_footBegin].open;
				close = (m_pKlineAdd[nKln][i].close-m_pKlineAdd[nKln][bAdd].open)*pView->m_pkline[m_footBegin].open/m_pKlineAdd[nKln][bAdd].open+pView->m_pkline[m_footBegin].open;
				high = (m_pKlineAdd[nKln][i].high-m_pKlineAdd[nKln][bAdd].open)*pView->m_pkline[m_footBegin].open/m_pKlineAdd[nKln][bAdd].open+pView->m_pkline[m_footBegin].open;
				low = (m_pKlineAdd[nKln][i].low-m_pKlineAdd[nKln][bAdd].open)*pView->m_pkline[m_footBegin].open/m_pKlineAdd[nKln][bAdd].open+pView->m_pkline[m_footBegin].open;
			}
			else
			{
				open = m_pKlineAdd[nKln][i].open;
				close = m_pKlineAdd[nKln][i].close;
				high = m_pKlineAdd[nKln][i].high;
				low = m_pKlineAdd[nKln][i].low;
			}

			if( m_pKlineAdd[nKln][i].high > 0 )
			{
				int closek = (int)(YTransfer(close) );
				int openk = (int)(YTransfer(open) );
				int highk = (int)YTransfer(high);//( hght * ( m_max_sun[m_nSon] - high ) / fudu_price ) + topf;
				int lowk = (int)YTransfer(low);//( hght * ( m_max_sun[m_nSon]-low ) / fudu_price ) + topf;

//				TRACE("pre iii==%d JJJJJ= %d\r\n",i,j);

//				int flag=0;//0	yin;1	yong;2	ping
				CRect rt;
				rt.top =closek;
				rt.bottom =openk;

				//--确定与第一根K线的关系
				if(pView->m_nKlineKind2 <5)
				{
					if(m_pKlineAdd[nKln][i].day<pView->m_pkline[j].day)
						continue;
					if(m_pKlineAdd[nKln][i].day>pView->m_pkline[j].day)
					{
						while(m_pKlineAdd[nKln][i].day>pView->m_pkline[j].day&&j<m_footEnd)
						{
							j++;
						}
						if(m_pKlineAdd[nKln][i].day<pView->m_pkline[j].day)
							continue;
					}
				}
				else
				{
					int daySec1=(int)(m_pKlineAdd[nKln][i].day+60)/(24*60*60);
					int daySec2=(int)(pView->m_pkline[j].day+60)/(24*60*60);
					if(daySec1<daySec2)
						continue;

					if(daySec1>daySec2)
					{
						while(daySec1>daySec2 
							&&j<m_footEnd)
						{
							j++;
							daySec2=(int)(pView->m_pkline[j].day+60)/(24*60*60);
						}

						if(j>=m_footEnd &&  daySec1>daySec2)
							break;

						if(daySec1<daySec2 || daySec1>daySec2)
							continue;
					}
				}


				if(j>m_footEnd)
					break;
				//-	画出
				int x= leftf +(int)( (j-m_footBegin+0.5) * (float)widthPer);
				if(klineWidthPer<1)
				{
					rt.left=leftf +(int)(  (j-m_footBegin+0.5) * (float)widthPer);
					rt.right =leftf + (int)( (j-m_footBegin+0.5) * (float)widthPer);
					pDC->MoveTo (x,highk);
					pDC->LineTo (x,lowk);
				}
				else
				{
					rt.left=leftf -klineWidthPer +(int)(  (j-m_footBegin+0.5) * (float)widthPer);
					rt.right =leftf +klineWidthPer + (int)( (j-m_footBegin+0.5) * (float)widthPer);
					
					if( open - close > 0 )
					{
						pDC->FillSolidRect (rt,pDoc->m_colorArray[7+nKln]);
						pDC->MoveTo (x-(int)(klineWidthPer),openk);
						pDC->LineTo (x-(int)(klineWidthPer),closek);
						pDC->LineTo (x+(int)(klineWidthPer),closek);
						pDC->LineTo (x+(int)(klineWidthPer),openk);
						pDC->LineTo (x-(int)(klineWidthPer),openk);

						pDC->MoveTo (x-(int)(klineWidthPer),openk);
						pDC->LineTo (x+(int)(klineWidthPer),openk);
						pDC->MoveTo (x,highk);
						pDC->LineTo (x,lowk);
					}
					else if( open - close < 0 )
					{
						pDC->FillSolidRect (rt,pDoc->m_colorArray[0]);
						pDC->MoveTo (x-(int)(klineWidthPer),openk);
						pDC->LineTo (x-(int)(klineWidthPer),closek);
						pDC->LineTo (x+(int)(klineWidthPer),closek);
						pDC->LineTo (x+(int)(klineWidthPer),openk);
						pDC->LineTo (x-(int)(klineWidthPer),openk);

						pDC->MoveTo (x,highk);
						pDC->LineTo (x,closek);
						pDC->MoveTo (x,lowk);
						pDC->LineTo (x,openk-1);
					}
					else 
					{
						pDC->MoveTo (x,highk);
						pDC->LineTo (x,lowk);
						pDC->MoveTo (x-(int)(klineWidthPer),openk);
						pDC->LineTo (x+(int)(klineWidthPer),openk);
					}
				}
			}
//			if(bAdd<500) TRACE("iii==%d JJJJJ= %d\r\n",i,j);
			j++;
			if(j>m_footEnd)
				break;

		}


END_ADDED:
		if(!pDC->IsPrinting())
		{
			pDC->SelectClipRgn (NULL);//&rgn2
			rgn.DeleteObject ();
		}

		pDC->SelectObject (pOldpen);
		pDC->SelectObject (pOldBrush);


}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：CaclMaxFlt(float *pFlt,int footBegin)
//      功能说明：计算线的最大、 最小值
//      入口参数：pFlt为将要计算最大、 最小值的浮点数数组指针，footBegin为数组的开始下标
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::CaclMaxFlt(float *pFlt,int footBegin,int nLine)//
{
	if(nLine>0)
		if((m_dataFormular[m_nSon].line[nLine].type==BUY_CASE || m_dataFormular[m_nSon].line[nLine].type==SELL_CASE))
			return;

	for(int i=footBegin;i<=m_footEnd;i++)
	{
		float ftemp = pFlt[i];
		if(ftemp>m_max_sun[m_nSon])
			m_max_sun[m_nSon]=ftemp;
		if(ftemp<m_min_sun[m_nSon])
			m_min_sun[m_nSon]=ftemp;
	}
	if(nLine>=0)
	{
		int j = nLine;
		bool bVolStick = false;
		if(m_dataFormular[m_nSon].line[j].type==VOLSTICK || m_dataFormular[m_nSon].line[j].type == STICK_VOL_FS 
			|| m_dataFormular[m_nSon].line[j].type == STICK || m_dataFormular[m_nSon].line[j].type == LINESTICK)
			bVolStick = true;
		if(m_dataFormular[m_nSon].line[j].m_arrBE.kind == ARRAY_BE::DrawStickLine && m_dataFormular[m_nSon].line[j].m_arrBE.lineWidth)
		{
			int n = m_dataFormular[m_nSon].line[j].m_arrBE.looseArr.GetSize();
			for(int k = 0;k<n;k++)
			{
				int nFoot = m_dataFormular[m_nSon].line[j].m_arrBE.looseArr[k].nFoot;
				if( nFoot<footBegin
					|| nFoot>this->m_footEnd )
					continue;

				float ftemp = m_dataFormular[m_nSon].line[j].m_arrBE.line[nFoot] ;
				if(ftemp>m_max_sun[m_nSon])
					m_max_sun[m_nSon]=ftemp;
				if(ftemp<m_min_sun[m_nSon])
					m_min_sun[m_nSon]=ftemp;
			}
		}

//成交量的最小值为0
		if(bVolStick && m_min_sun[m_nSon]>0) 
			m_min_sun[m_nSon]=0;
		if(bVolStick && m_max_sun[m_nSon]<0) 
			m_max_sun[m_nSon]=0;
	}

}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：CaclMaxFlt(Kline *pK,int footBegin)
//      功能说明：计算线的最大、 最小值
//      入口参数：pK为将要计算最大、 最小值的K线数组指针，footBegin为数组的开始下标
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::CaclMaxFlt(Kline *pK,int footBegin)//计算线的最大、 最小值
{
	for(int i=m_footBegin;i<=m_footEnd;i++)
	{
		if((pK+i)->high >m_max_sun[m_nSon])
		{
			m_max_sun[m_nSon]=(pK+i)->high;
			m_highFoot=i;
		}
		if((pK+i)->low<m_min_sun[m_nSon])
		{
			m_min_sun[m_nSon]=(pK+i)->low;
			m_lowFoot=i;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
//      函 数 名：InitDrawedLineBE()
//      功能说明：初始化曲线的起始点
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::InitLineParam()//初始化曲线的起始点
{
	if(m_dataFormular[m_nSon].numLine>Total_Count_Line)
		m_dataFormular[m_nSon].numLine = 0;
	for(int i=0;i<m_dataFormular[m_nSon].numLine ;i++)
	{
		int bgn=m_dataFormular[m_nSon].line[i].m_arrBE.b;
		if(bgn == -1)
			continue;

		m_dataFormular[m_nSon].line[i].bNow=bgn;
		if(bgn>pView->m_nCountKline-1 )
			m_dataFormular[m_nSon].line[i].bNow=pView->m_nCountKline-1;
		if(bgn<=m_footBegin)
			m_dataFormular[m_nSon].line[i].bNow=m_footBegin;
	}
	return 1;


}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：InitDrawedLineBE()
//      功能说明：设置画K线的起始点
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
#define BLANK_KLINE	5
void CTaiKlineShowKline::InitDrawedLineBE()//设置画K线的起始点
{
	if(pView->m_nCountKline>0)
	{
		if(pView->m_nCountKline+BLANK_KLINE>m_klinNumDefault)
		{
			m_footBegin=pView->m_nCountKline-m_klinNumDefault+BLANK_KLINE;
			m_footEnd=pView->m_nCountKline-1;
			m_footCurrent=m_footEnd;
		}
		else
		{
			m_footBegin=0;
			m_footEnd=pView->m_nCountKline-1;
			m_footCurrent=m_footEnd;
			if(m_footEnd-m_footBegin+1>10)
				m_klinNumDefault=m_footEnd-m_footBegin+1;
			else
				m_klinNumDefault=10;
		}
	}
	else
	{
			m_klinNumDefault=82;
			m_footBegin=0;
			m_footEnd=0;
			m_footCurrent=0;
	}
	if(m_footCurrent < 0) m_footCurrent = 0;

	ValidDoKline(true);
}

int flagbit=0;

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawKlineFiguer(CDC *pDC)
//      功能说明：把K线总界面画到内存位图上
//      入口参数：pDC为传入的设备描述表指针
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DrawKlineFiguer(CDC *pDC)
{
#ifdef TEST_USER1
	CTime t = CTime::GetCurrentTime();
    CTime t2 = g_timeUseEnd;
	if(t >= t2)
	{
	    return ;
	}
	else
	{
	}
#endif
	//if it is fen shi
	if(pView->m_nKlineKind2 == 0)
	{
		if(m_bSetPageData == true)
			pView->SetPageData();//
		else
			m_bSetPageData = true;

		pView->pMin1Drawing->DrawMin1Figuer (pDC);
//		pView->InvertFocusRect2(pDC);
		return;
	}
	else
		pView->pMin1Drawing->InitMinuteLine();
	if(pView->pMin1Drawing->m_pReportData == NULL)
		return;

	m_highFoot = 0;
	m_lowFoot  = 0 ;

	pView->DrawRectPer (pDC);
	pView->ShowTextRect(m_nTextBox,pDC);
	pView->ShowTransferText(pDC);

	//if(pDoc->m_bInitDone==TRUE && pDoc->m_bCloseWorkDone==false || pView->m_pDlgDealHistory!=NULL)
		TransferKlineTypeDataToday(pView->m_pkline,pView->m_sharesSymbol, pView->m_stkKind,-1);
	
	KLINE_EXT_PER* pExt;
	if(CTaiShanKlineShowView::m_klineExtMap .Lookup(pView->m_sharesSymbol,pExt) )
	{//扩展k线数据
		if(pExt!=NULL)
		{
			if(!(m_nKlineCurrent>0 && pView->m_nKlineKind2 != HS_KLINE))
			{
				if(pView->m_nCountKlineToday + pExt->m_nCountKlineSelf > pView->m_nCountKline )
				{
					pView->m_nCountKline = pView->m_nCountKlineToday + pExt->m_nCountKlineSelf ;
					Kline* pKline = new Kline[pView->m_nCountKline];
					memcpy(pKline,pView->m_pkline,pView->m_nCountKlineToday*sizeof(Kline));
					if(pView->m_pkline !=0) delete[] pView->m_pkline ;
					pView->m_pkline = pKline;
					ASSERT(pExt->m_pKlineSelf !=NULL);
					memcpy(pKline+pView->m_nCountKlineToday,pExt->m_pKlineSelf,pExt->m_nCountKlineSelf*sizeof(Kline));

				}
			}
		}
	}
	int total_son=pView->m_infoInit.nCountIndex;

	//---计算技术指标数据
	bool bToCaclTjxgShow = false;
	if(m_numOldKline!=pView->m_nCountKline 
		|| m_sOldSymbol!=pView->m_sharesSymbol ||m_sOldStkKind!=pView->m_stkKind
		|| m_nKlineType!=pView->m_nKlineKind2 )
	{
		m_sOldSymbol = pView->m_sharesSymbol;
		m_sOldStkKind= pView->m_stkKind;
		m_nKlineType = pView->m_nKlineKind2;
		m_numOldKline = pView->m_nCountKline;
		m_bToCacl = true;
	}

	if(m_bToCacl==true&&pView->m_nCountKline>0)
	{
		bToCaclTjxgShow = true;
		//make the time of kline data correct
		for(int k=0;k<pView->m_nCountKline ;k++)
		{
			if(pView->m_pkline[k].day<0)
				pView->m_pkline[k].day=5400;
		}

		m_nSon=0;

		CString s0(pView->m_infoInit.initIndex[0].nameIndex);
		if(s0!="QLDJ"&&s0!="HLDJ"&&s0!="QEDJ"&&s0!="HEDJ"&&s0!="")
		{
			m_zhiBiao0=s0;
			CalcSonFiguer(s0);
		}
		else
		{
			m_dataFormular[0].numLine=0;
		}

		for(int i=1;i<pView->m_infoInit.nCountIndex;i++)
		{
			m_nSon=i;
			CString s1(pView->m_infoInit.initIndex[m_nSon].nameIndex);
			CalcSonFiguer(s1);
		}
		m_bToCacl=false;
		/////////////////////---初始化
		if(m_bInitFoot==true)
			InitDrawedLineBE();
//
		
	}
	else if(m_bToCacl==true&&pView->m_nCountKline<=0)
	{
		m_bToCacl=false;
		/////////////////////---初始化  
		if(m_bInitFoot==true)
			InitDrawedLineBE();
	}

	if(m_footEnd>=pView->m_nCountKline)
		InitDrawedLineBE();

	if(pView->m_nCountKline<=0)
	{
		m_max_sun[0]=0;
		m_min_sun[0]=0;
		m_max_sun[1]=0;
		m_min_sun[1]=0;
		m_max_sun[2]=0;
		m_min_sun[2]=0;
		m_max_sun[3]=0;
		m_min_sun[3]=0;

		CFont ft;
		LOGFONT lgf=pDoc->m_fontstr[0];
		ft.CreateFontIndirect (&lgf );
		CFont* pOld=pDC->SelectObject(&ft);
		pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		pDC->SetTextColor(  pDoc->m_colorArray[1]);
		pDC->SetBkMode(TRANSPARENT);

		//---		标题
		CString title;
		title=pView->pMin1Drawing->m_pReportData->name;
		title=title+"(无K线数据)";

		CRect rtFill(0,0,pView->m_rtKlineFiguer.rightX-1,0);
		rtFill.top=0;
		rtFill.bottom=rtFill.top+pView->m_heightCaption-2;
		pDC->FillSolidRect(rtFill,pDoc->m_colorArray[0]);

		pDC->TextOut (2,0,title);
		pDC->SelectObject(pOld);

		m_footEnd = -1;

		m_bNewStock = false;
		return;
	}
	if(!ValidDoKline()) return;




	DrawRulorX(pDC);



	m_nSon=0;

	if(	m_bNewStock == true || m_kLine_mode == 0 )
	{
		m_max_sun[m_nSon]=(float)-1.0E20;
		m_min_sun[m_nSon]=(float)1.0E20;
		CaclMaxFlt(pView->m_pkline,0);
	}

	InitLineParam();
	int i;
	if(m_nKlineCurrent>0 && pView->m_nKlineKind2 != HS_KLINE)
		DrawKLineAddedPage(pDC);
	else
	{
		int  bZj=0;
		if(pView->m_isShowCross==0)
		{
			CString s0(pView->m_infoInit.initIndex[0].nameIndex);
			if(s0=="QLDJ"||s0=="HLDJ")
			{
				bZj=1;
			}
			if(s0=="QEDJ"||s0=="HEDJ")
			{
				bZj=2;
			}
		}
		else
		{
			CString s0(pView->m_infoInit.initIndex[0].nameIndex);
			if(s0=="QLDJ"||s0=="HLDJ")
			{
				bZj=3;
			}
			if(s0=="QEDJ"||s0=="HEDJ")
			{
				bZj=4;
			}
		}

		if(bZj==0 && pView->m_infoInit.initIndex[0].nameIndex[0] !='\0')
		for( i=0;i<m_nLineIndicator1[m_nSon] ;i++)
		{
			if(m_dataFormular[m_nSon].line[i].m_arrBE.b == -1)
				continue;
//			if(m_dataFormular[m_nSon].line[i].bNow>m_footEnd)
//			{
//				m_dataFormular[m_nSon].line[i].bNow=m_footEnd;
//			}
//			else
			if(m_bNewStock == true || m_kLine_mode == 0)
				CaclMaxFlt(m_dataFormular[m_nSon].line[i].m_arrBE.line,
					m_dataFormular[m_nSon].line[i].bNow,i);
		}


				//--画K线
		SetRectCurrent(0);
//		AfxMessageBox("DrawRulorText");
		DrawRulorText(pDC);

		//画成本堆积
		if(pView->m_isShowCross==0)
		{
			if(bZj==1)
			{
				ShowCNP (pDC);
			}
			if(bZj==2)
			{
				ShowChengBen (pDC);
			}
		}
				//--主图叠加技术指标曲线
//		AfxMessageBox("DrawLineIndex");
		DrawKLine(pDC);
		if(bZj==0)
			DrawLineIndex(pDC);
//		DrawIndicatorAdded(pDC);
//		AfxMessageBox("DrawKLine");
//		DrawSonFiguer(pDC);

		//---		画自画线
		pView->pDrawLine->DrawTotal(pDC);
		if(!pDC->IsPrinting())
			pDC->SelectClipRgn (NULL);//
		//--画K线模式标志
		if(pView->pKlineMode!=NULL)
			pView->pKlineMode->DrawKLineMode(pView,pDC);
		//--画基本信息
		if(pView->m_bBaseInfo==true)
			DrawBaseInfo(pDC);

		//画除权
		if(pView->m_bDoPower ==true)
			DrawPower(pDC);

		if(pView->m_screenStockShow.m_bUseing == true)
		{
			if(bToCaclTjxgShow==true)
				pView->m_screenStockShow.Calculate() ;
			ShowTjxgShow(pDC);
		}
//		AfxMessageBox("DrawPower");

		DrawCaption(pDC);

	}

	//CLEAR BACKGROUND
	if( pDC->IsPrinting())
	{
		if(m_kLine_mode == 1 )
		{
			for(int k = 0 ; k < pView->m_infoInit.nCountIndex-1;k++)
			{
				CRect r(1,pView->m_rtKlineFiguer.rtBlw[k].m_yBottom+1+pView->m_heightCaption,pView->m_rtKlineFiguer.rightX-1,pView->m_rtKlineFiguer.rtBlw[k+1].m_yBottom-1);
				pView->ClearBack(pDC,r);
			}
			CRect r(1,pView->m_rtKlineFiguer.rtBlw[pView->m_infoInit.nCountIndex-1].m_yBottom+1,
				pView->m_rtKlineFiguer.rightX-1,pView->m_rtKlineFiguer.rtBlw[pView->m_infoInit.nCountIndex-1].m_yBottom-1+pView->m_heightCaption);
			pView->ClearBack(pDC,r);
		}
	}

	//--副图技术指标曲线1
	for(int j=1;j<total_son;j++)
	{
		bool bCalc=false;
		m_nSon=j;
		if(	m_bNewStock == true || m_kLine_mode == 0 )
		{
		m_max_sun[m_nSon]=(float)-1.0E20;
		m_min_sun[m_nSon]=(float)1.0E20;
		}
		InitLineParam();

		for(i=0;i<m_nLineIndicator1[m_nSon] ;i++)
		{
			if(m_dataFormular[m_nSon].line[i].m_arrBE.b == -1)
				continue;

			bCalc=true;
			if(	m_bNewStock == true || m_kLine_mode == 0 )
				CaclMaxFlt(m_dataFormular[m_nSon].line[i].m_arrBE.line,
					m_dataFormular[m_nSon].line[i].bNow,i);
		}
		if(bCalc==false)
		{
			m_max_sun[m_nSon]=0;
			m_min_sun[m_nSon]=0;
		}
		DrawSonFiguer(pDC);
//		DrawIndicatorAdded(pDC);
	}
	m_bNewStock = false;
	if( pDC->IsPrinting())
	{
		DrawRulorX(pDC);
	}

	if(m_bSetPageData == true)
		pView->SetPageData();//
	else
		m_bSetPageData = true;

	if(pDC->IsPrinting ()) return;

	if(m_bInvertRect == true)
	{
		CRect r;
		pView->GetCurrClientRect(r);
		int nb = m_nBeginFootTJ - this->m_footBegin;//时段统计的开始点
		if(nb<0)
			nb = 0;
		int ne =   m_nEndFootTJ - this->m_footBegin+1;//时段统计的开始点
		if(ne<0)
			ne=0;
		if(ne>m_klinNumDefault)
			ne = m_klinNumDefault;

		float f = (float)pView->m_rtKlineFiguer.rightX/m_klinNumDefault;
		r.left = f*nb;
		r.right = f*ne;
		r.top = 2;
//		r.bottom -= 2;

		pDC->InvertRect(r);
	}
//	pView->InvertFocusRect2(pDC);
}




/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawRulorText(CDC *pDC)
//      功能说明：画标尺的标记数字
//      入口参数：pDC为传入的设备描述表指针
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DrawRulorText(CDC *pDC,BYTE wid)//
//wid=0为缺省，wid=1为小图，wid=2为分时画面价格的两边文本，wid=3为分时画面其它的两边文本，wid=4为百分比坐标
{
	if(m_max_sun[m_nSon]==m_min_sun[m_nSon])
		return;
	//---		是否是对数或百分比坐标
	if(DrawRulorTextLog(pDC,wid)==1)
		return;
	if(ShowFenshiRulor(pDC,wid)==true)
		return;

	CRect rt(m_rectDrawLine.right,m_rectDrawLine.top,
		m_rectDrawLine.right+pView->m_midLen,m_rectDrawLine.bottom);

	if(m_rectDrawLine.bottom-m_rectDrawLine.top<2)
		return;
	//------根据最大值及最小值来确定显示的数字的位数
	float max;

	max=(float)(FABSMY(m_max_sun[m_nSon])>FABSMY(m_min_sun[m_nSon])?FABSMY(m_max_sun[m_nSon]):FABSMY(m_min_sun[m_nSon]));
	if(max>(float)10E11)
		return;
	m_step[m_nSon]=0;
	int flag;	//flag=0为浮点数，1为整数，2为大于等于100000的整数
	if(max<100)
		flag=0;
	else if(max<100000)
		flag=1;
	else
	{
		flag=2;
		float temp=max/100000;
		for(int i=0;i<11;i++)
		{
			if(temp<pow((double)10,(double)i))
			{
				m_step[m_nSon]=(int)(pow((double)10,(double)i)+0.5);
				break;
			}
		}
	}
	//------确定画几条线
	int numLine=(m_rectDrawLine.bottom-m_rectDrawLine.top)/pView->m_minGrid+1;
	//------确定每格的数值
	float valuePer;
	valuePer=(m_max_sun[m_nSon]-m_min_sun[m_nSon])/numLine;



	int flagGap=4;//4为画四条分隔线，3为画三条分隔线，

	float k=1;
	int i,sm;
	for(i=0;;i++)
	{
		if(valuePer<0.01*k)
		{
			valuePer=(float)0.01*k;
			break;
		}
		if(valuePer<0.02*k)
		{
			valuePer=(float)0.02*k;
			flagGap=3;
			break;
		}
		if(valuePer<0.05*k)
		{
			valuePer=(float)0.05*k;
			break;
		}
		k*=10;
		if(k>10E20)
		{
//			AfxMessageBox("原始数据有错误");
			return;
		}
	}


	//---起始点   结束点
	int num_b;
	int num_all;
	if(valuePer==0)
		return;
	num_b=(int)floor((m_min_sun[m_nSon]/valuePer))+1;
	num_all=(int)(m_max_sun[m_nSon]/valuePer)-num_b+1;

	CPen pen_solid(PS_SOLID ,1,pDoc->m_colorArray[2]);//PS_DASHDOTDOT   
	CPen* pOldpen=pDC->SelectObject(&pen_solid);

//	int i;
	int m=2;

	//---		画线
	float gap;
	if((m_max_sun[m_nSon]-m_min_sun[m_nSon])==0||(flagGap+1)==0)
	{
		pDC->SelectObject(pOldpen);
		return ;
	}

	//draw line from data
	if(DrawRulorTextSelf(pDC, wid)== true)
	{
		pDC->SelectObject(pOldpen);
		return ;
	}

	gap=((m_rectDrawLine.bottom-m_rectDrawLine.top))*valuePer/(m_max_sun[m_nSon]-m_min_sun[m_nSon])/(flagGap+1);

	for(i=0;i<num_all;i++)
	{
		int y;
		y=YTransfer((float)(valuePer*(num_b+i)));

		if(y<m_rectDrawLine.bottom-3 && y>=m_rectDrawLine.top)
		{
			//---		画虚线
			if(!(pDoc->m_systemOption.showxline==FALSE && (pView->m_nKlineKind2>0 && wid !=1)))
			{
				CPoint p1,p2;
				p1.x=m_rectDrawLine.left;
				p2.x=m_rectDrawLine.right+1;
				p1.y=y;
				p2.y=y;
				ShowVirtualLine(pDC,p1,p2);
			}

			pDC->MoveTo(m_rectDrawLine.right+1,y);
			pDC->LineTo(m_rectDrawLine.right+6,y);
			for(sm=0;sm<flagGap;sm++)
			{
				int yNow=(int)(y-(sm+1)*gap);
				if(yNow>=m_rectDrawLine.top)
				{
					pDC->MoveTo(m_rectDrawLine.right+1,yNow);
					pDC->LineTo(m_rectDrawLine.right+3,yNow);
				}
			}
			if(i==0)
			{
				for( sm=0;sm<flagGap;sm++)
				{
					int yNow2=(int)(y+(sm+1)*gap);
					if(yNow2<m_rectDrawLine.bottom)
					{
						pDC->MoveTo(m_rectDrawLine.right+1,yNow2);
						pDC->LineTo(m_rectDrawLine.right+3,yNow2);
					}
				}
			}//end if
		}//end if
	}

	//---		画文字
	pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
	pDC->SetTextColor( pDoc->m_colorArray[2]);
	pDC->SetBkMode(TRANSPARENT);

	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];
	if(wid==1)
		lgf.lfHeight=-14;
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int width=pView->m_midLen;
 	if( wid == 1)
		width=m_midLen_L;
//wid=0为缺省，wid=1为小图，wid=2为分时画面价格的两边文本，wid=3为分时画面其它的两边文本，wid=4为百分比坐标

	CString bck="";
	if(wid==4)
		bck="%";

	switch(flag)
	{
	case 0:
		for(i=0;i<num_all;i++)
		{
			CString str;
			int y=YTransfer((float)(valuePer*(num_b+i)));

				if(wid!=2&&!(wid==1&&pView->pMin1Drawing->m_nSon==0))
				{
					str.Format("%.2f",(float)(valuePer*(num_b+i)));
					str+=bck;
					if(wid==3)
						pDC->TextOut(m_rectDrawLine.left-m,y-tm.tmHeight/2,str);

					if(y>m_rectDrawLine.bottom-5 || y<m_rectDrawLine.top)
						continue;
					pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);

				}
				//---	如果是小图
				else if(wid==1&&pView->pMin1Drawing->m_nSon==0)
				{
					if(y>m_rectDrawLine.bottom-5 || y<m_rectDrawLine.top)
						continue;
					float cls;
					if(pView->m_tabNum ==3)
						cls=pView->pMin1Drawing->m_close;
					else
						cls=pView->pMin1Drawing->m_close_Dapan[0];

					float nw=(valuePer*(num_b+i));
					str.Format("%.2f",nw);
					if(nw>cls)
						pDC->SetTextColor( pDoc->m_colorArray[13]);
					else if(nw<cls)
						pDC->SetTextColor( pDoc->m_colorArray[15]);
					else
						pDC->SetTextColor( pDoc->m_colorArray[14]);
					pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);

				}
				//---	如果是分时走势
				else if(wid==2)
				{
					float cls=pView->pMin1Drawing->m_close;
					float nw=(valuePer*(num_b+i));
					str.Format("%.2f",nw);
					if(nw>cls)
						pDC->SetTextColor( pDoc->m_colorArray[13]);
					else if(nw<cls)
						pDC->SetTextColor( pDoc->m_colorArray[15]);
					else
						pDC->SetTextColor( pDoc->m_colorArray[PING_PAN_WORD]);
					pDC->TextOut(m_rectDrawLine.left-m,y-tm.tmHeight/2,str);

					if(y>m_rectDrawLine.bottom-5 || y<m_rectDrawLine.top)
						continue;
					if(cls>0)
					{
						str.Format("%.1f",(float)(valuePer*(num_b+i)-cls)*100/cls);
						str+="%";
						pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);
					}
				}
		}//end for
		break;
	case 1:
		for(i=0;i<num_all;i++)
		{
			CString str;
			int y=YTransfer((float)(valuePer*(num_b+i)));

//			if(y<m_rectDrawLine.bottom-5)
			{
				if(wid!=2&&!(wid==1&&pView->pMin1Drawing->m_nSon==0))
				{
					str.Format("%d",(int)(valuePer*(num_b+i)));
					str+=bck;
					if(wid==3)
							pDC->TextOut(m_rectDrawLine.left-m,y-tm.tmHeight/2,str);

					if(y>m_rectDrawLine.bottom-5 || y<m_rectDrawLine.top)
						continue;
					pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);
				}
				//---	如果是小图
				else if(wid==1&&pView->pMin1Drawing->m_nSon==0)
				{
					if(y>m_rectDrawLine.bottom-5 || y<m_rectDrawLine.top)
						continue;
					float cls;
					if(pView->m_tabNum ==3)
						cls=pView->pMin1Drawing->m_close;
					else
						cls=pView->pMin1Drawing->m_close_Dapan[0];

					float nw=(valuePer*(num_b+i));
					str.Format("%d",(int)nw);
					if(nw>cls)
						pDC->SetTextColor( pDoc->m_colorArray[13]);
					else if(nw<cls)
						pDC->SetTextColor( pDoc->m_colorArray[15]);
					else
						pDC->SetTextColor( pDoc->m_colorArray[PING_PAN_WORD]);
					pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);

				}
				//---	如果是分时走势
				else if(wid==2)
				{
					float cls=pView->pMin1Drawing->m_close;
					if(cls>0)
					{
						float nw=(valuePer*(num_b+i));
						str.Format("%d",(int)nw);
						if(nw>cls)
							pDC->SetTextColor( pDoc->m_colorArray[13]);
						else if(nw<cls)
							pDC->SetTextColor( pDoc->m_colorArray[15]);
						else
							pDC->SetTextColor( pDoc->m_colorArray[PING_PAN_WORD]);
						pDC->TextOut(m_rectDrawLine.left-m,y-tm.tmHeight/2,str);

						if(y>m_rectDrawLine.bottom-5 || y<m_rectDrawLine.top)
							continue;
						str.Format("%.1f",(float)((valuePer*(num_b+i)-cls)*100/cls));
						str+="%";
						pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);
					}
				}
			}
		}
		break;
	case 2:
		CString str;
		if(m_step[m_nSon]==0)
		{
			pDC->SelectObject(pOld);
			pDC->SelectObject(pOldpen);
			return;
		}
		for(i=0;i<num_all;i++)
		{
			int y=YTransfer((float)(valuePer*(num_b+i)));

			
			str.Format("%d",(int)((float)(valuePer*(num_b+i))/m_step[m_nSon]));
			str+=bck;
			if(wid==3)
					pDC->TextOut(m_rectDrawLine.left-m,y-tm.tmHeight/2,str);
			if(y>m_rectDrawLine.bottom-5  || y<m_rectDrawLine.top)
				continue;
			pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);
			
		}
		//---		画倍数
		str.Format("%d",m_step[m_nSon]);
		int width=(pDC->GetOutputTextExtent(str)).cx+10; 
		if(width>=pView->m_midLen-2)
			width=pView->m_midLen-3;

		CBrush brush_bk(RGB(255,255,255));
		CBrush* pOldBrush=pDC->SelectObject(&brush_bk);

		int lf=m_rectDrawLine.right+2;
		int rg=m_rectDrawLine.right+2+width;
		int tp;
		if(wid==1)
			tp=m_rectDrawLine.bottom-18;
		else
			tp=m_rectDrawLine.bottom-4-pView->m_heightCaption;
		int bm=m_rectDrawLine.bottom-4;

		CRect rect1(lf,tp,rg,bm);
		pDC->FillSolidRect (rect1,pDoc->m_colorArray[0]);

//		str="×"+str;
		pDC->SetTextAlign( TA_LEFT );//
		pDC->TextOut(lf+8,tp,str);
		pDC->SetTextAlign( TA_RIGHT );//
		
		pDC->SelectObject(&pen_solid);
		pDC->MoveTo(lf,tp);
		pDC->LineTo(lf+width+2,tp);
		pDC->LineTo(lf+width+2,bm);
		pDC->LineTo(lf,bm);
		pDC->LineTo(lf,tp);

		if(wid!=1)
			pDC->MoveTo(m_rectDrawLine.right+4,m_rectDrawLine.bottom-pView->m_heightCaption);
		else
			pDC->MoveTo(m_rectDrawLine.right+4,m_rectDrawLine.bottom-16);
		pDC->LineTo(m_rectDrawLine.right+10,m_rectDrawLine.bottom-8);
		pDC->MoveTo(m_rectDrawLine.right+4,m_rectDrawLine.bottom-8);
		if(wid!=1)
			pDC->LineTo(m_rectDrawLine.right+10,m_rectDrawLine.bottom-pView->m_heightCaption);
		else
			pDC->LineTo(m_rectDrawLine.right+10,m_rectDrawLine.bottom-16);
		break;
	}
	pDC->SelectObject(pOld);
	pDC->SelectObject(pOldpen);
	

}



/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawSonFiguer(CDC* pDC)
//      功能说明：画子图
//      入口参数：pDC为传入的设备描述表指针
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DrawSonFiguer(CDC* pDC)
{
		SetRectCurrent(m_nSon);

		if(pDC->IsPrinting () || m_kLine_mode == 1 )
		{
			CRect r9(1,m_rectDrawLine.top,m_rectDrawLine.right ,m_rectDrawLine.bottom-1 );
//			pView->ClearBack (pDC,r9);
		}


		if(m_nSon>0 )
		{
//			CRect r(0,pView->m_rtKlineFiguer.rtBlw[m_nSon-1].m_yBottom,pView->m_rtKlineFiguer.rightX+pView->m_midLen,pView->m_rtKlineFiguer.rtBlw[m_nSon].m_yBottom);
			if(m_bDaPan[m_nSon-1]==1)
			{
				if(!CTaiShanKlineShowView::IsIndexStock3(pView->m_sharesSymbol ))
				{
					CFont ft;
					LOGFONT lgf=pDoc->m_fontstr[0];
					//lgf.lfHeight=-12;
				//	lgf.lfWidth=8;
					ft.CreateFontIndirect (&lgf );
					CFont* pOld=pDC->SelectObject(&ft);
					pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
					pDC->SetTextColor(  pDoc->m_colorArray[1]);
					pDC->SetBkMode(TRANSPARENT);

					CString s(pView->m_infoInit.initIndex[m_nSon].nameIndex);
					s=s+"指标只能用于大盘指数";
					int len=(pDC->GetOutputTextExtent(s)).cx;

					int left=(m_rectDrawLine.right-m_rectDrawLine.left)/2-len/2;
					if(left<0)
						left=0;
					int top=(m_rectDrawLine.bottom+m_rectDrawLine.top)/2-10;
					if(top<0)
						top=0;

					pDC->ExtTextOut (left,top,ETO_CLIPPED,m_rectDrawLine,s,NULL);
					
					pDC->SelectObject(pOld);

					return;
				}
			}
		}

		
		DrawRulorText(pDC);

		DrawLineIndex(pDC);

		DrawCaption(pDC);


}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：CalcSonFiguer(CString name)
//      功能说明：计算子图的技术指标的数值
//      入口参数：name为将要计算的技术指标的名字
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::CalcSonFiguer(CString name)
{
	m_nLineIndicator1[m_nSon] = 0;
	for(int ii=0;ii<2;ii++)
	{
		if(ii==1)
		{
			name = m_sIndicatorAdded[m_nSon];
			if(name == "") return;
		}

		m_dataFormular[m_nSon].numLine = 0;
		CFormularContent*	pIndex;
		pIndex=(CFormularContent*)pDoc->LookUpArray (pDoc->m_formuar_index,name); 
		if(pIndex!=NULL)
			if(!pIndex->IsValid()) return;

		if(pIndex==NULL)
		{
			for(int i=0;i<Total_Count_Line ;i++)//  document to change
			{
				if(m_dataFormular[m_nSon].line[i].m_arrBE.line!=NULL)
					delete[] m_dataFormular[m_nSon].line[i].m_arrBE.line;
				m_dataFormular[m_nSon].line[i].m_arrBE.line=new float[pView->m_nCountKline];
			}
			return;
		}
		//m_nValidIndicator
		m_nValidIndicator[m_nSon] = CTaiScreenParent::IsPeriodValid(pView->m_nKlineKind2 ,pIndex);

		CString s ;
		int j=0;
		CString str = " ";

	/*	float * vol = NULL;
		if(pView->m_nKlineKind2 == HS_KLINE)
		{
			vol = new float[pView->m_nCountKline];
			for(int k = 0;k<pView->m_nCountKline;k++)
			{
				vol[k] = pView->m_pkline[k].vol;
				pView->m_pkline[k].vol = FABSMY(pView->m_pkline[k].vol);
			}
		}*/
		if(m_nSon>0 && ii==0)
			m_bDaPan[m_nSon-1]=0;

		//--		加入参数
		CFormularCompute* m_formuCompute = NULL;
		INPUT_PARAM input;
		input.pmax			=pView->m_nCountKline-1;
		input.pKLine		= pView->m_pkline;
		input.symbol		= pView->m_sharesSymbol ;
		input.nKindKline	= pView->m_nKlineKind2 ;
		input.pBuySellList	=&( pView->m_hs) ;
		input.pIndex		= pIndex;
		input.bAddBuySell   = false;
		input.m_stkKind		= pView->m_stkKind;
		if(ComputeFormu2(m_formuCompute,input)==0)
		{
			int rtnLine=0;
			m_dataFormular[m_nSon].numLine =m_nLineIndicator1[m_nSon] + m_formuCompute->GetRlineNum ();
			if(m_dataFormular[m_nSon].numLine>Total_Count_Line)
				m_dataFormular[m_nSon].numLine = Total_Count_Line;
			if(m_nSon>0 && ii == 0 )
				m_bDaPan[m_nSon-1]=m_formuCompute->m_isDaPan ;

			for(int i=m_nLineIndicator1[m_nSon];i<m_dataFormular[m_nSon].numLine ;i++)
			{
				if(m_dataFormular[m_nSon].line[i].m_arrBE.line!=NULL)
					delete[] m_dataFormular[m_nSon].line[i].m_arrBE.line;
				m_dataFormular[m_nSon].line[i].m_arrBE.line=new float[pView->m_nCountKline];
				memset(m_dataFormular[m_nSon].line[i].m_arrBE.line,0,4*pView->m_nCountKline);

				CString namePer;
				int nIn = i-m_nLineIndicator1[m_nSon];
				rtnLine = m_formuCompute->GetLine (nIn,m_dataFormular[m_nSon].line[i].m_arrBE,namePer);
				if(rtnLine==1)
					m_dataFormular[m_nSon].line[i].m_arrBE.b = -1;
				//	线型
				m_dataFormular[m_nSon].line[i].type = m_formuCompute->m_RlineType[i] ;
				m_dataFormular[m_nSon].line[i].lineThick = m_formuCompute->m_RlineThick[i] ;
			
				if(namePer.GetLength()>Total_Count_Line)
					m_lineName[m_nSon][i]="未知";
				else
					m_lineName[m_nSon][i]=namePer;

				m_dataFormular[m_nSon].line[i].clr =  m_formuCompute->m_RlineColor[i] ;
			}
			if(ii==0)
				m_nLineIndicator1[m_nSon] = m_dataFormular[m_nSon].numLine;
				
		}
		else
			;//ASSERT(FALSE);


		delete m_formuCompute;
	}
/*	if(vol!=NULL)
	{
		for(int k = 0;k<pView->m_nCountKline;k++)
		{
			pView->m_pkline[k].vol=vol[k] ;
		}
		delete vol;
	}*/

}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawCaption(CDC *pDC)
//      功能说明：计算子图的技术指标的数值
//      入口参数：pDC为传入的设备描述表指针
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DrawCaption(CDC *pDC)
{//?
	if(pView->pMin1Drawing->m_pReportData==NULL)
		return;

	if(m_nSon==0 && m_nKlineCurrent>0)
	{
		DrawCaptionAdded(pDC);
		return;
	}

	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);
	pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
	//pDC->SetTextColor(  pDoc->m_colorArray[20]);
	pDC->SetTextColor(  pDoc->m_colorArray[1]);
	pDC->SetBkMode(TRANSPARENT);

	//---		标题
	CString title = pView->m_sharesSymbol  ;
	title+=pView->pMin1Drawing->m_pReportData->name;

	CRect rtFill(1,0,pView->m_rtKlineFiguer.rightX-1,0);
	if(m_nSon==0)
		rtFill.top=1;
	else
		rtFill.top=pView->m_rtKlineFiguer.rtBlw[m_nSon-1].m_yBottom+2;
	rtFill.bottom=rtFill.top+pView->m_heightCaption-2;
	pView->ClearBack (pDC,rtFill);

	int left=0;
	if(m_nSon==0)
	{
		left=(pDC->GetOutputTextExtent(title)).cx+8;
		pDC->TextOut (2,0,title);
	}

	CString name(pView->m_infoInit.initIndex[m_nSon].nameIndex);
	if(m_nSon==0)
	{
		if(name=="QLDJ"||name=="HLDJ"||name=="QEDJ"||name=="HEDJ")
		{
			name.MakeUpper();
			pDC->TextOut(left,0,name);
			pDC->SelectObject(pOld);
			return;
		}
	}

	CFormularContent*	pIndex = NULL;
	CString paras = GetParamString(name);

	name+=paras;
	name+="  ";
	name.MakeUpper();

	CRect r;
	r.left=0;
	r.right=m_rectDrawLine.right;
	if(m_nSon==0)
		r.top=1;
	else
		r.top=pView->m_rtKlineFiguer.rtBlw[m_nSon-1].m_yBottom+1;
	r.bottom=r.top + pView->m_heightCaption;

	pDC->TextOut(left,r.top,name);

	int len=(pDC->GetOutputTextExtent(name)).cx;
	int num=m_dataFormular[m_nSon].numLine;

	int footCur=m_footCurrent;
	if(pView->m_isShowCross == 0)
		footCur = m_footEnd;

	for(int i=0;i<num;i++)
	{ 
		if(m_nLineIndicator1[m_nSon]>0 && i == m_nLineIndicator1[m_nSon])
		{
			if(m_sIndicatorAdded[m_nSon]=="") break;
			pDC->SetTextColor(  pDoc->m_colorArray[1]);
			CString paras2 = "  "+m_sIndicatorAdded[m_nSon]+GetParamString(m_sIndicatorAdded[m_nSon]);
			paras2+="  ";

			pDC->ExtTextOut (left+len,r.top ,ETO_CLIPPED,r,paras2,NULL);
			len+=(pDC->GetOutputTextExtent(paras2)).cx;
		}
		CString pnm=m_lineName[m_nSon][i];
		if (pnm[0] == 'l' && pnm[1] >= '0' && pnm[1] <= '9')
		{
			continue;
		}
		pnm.MakeUpper();
		pDC->SetTextColor( pDoc->m_colorArray[7+i%6]);
		//---		输出数值
		CString vl;
		float f = m_dataFormular[m_nSon].line[i].m_arrBE.line[footCur];
		vl = " "+CLongString::FloatToString(f);
/*		if(f>=1000)
			vl.Format (" %d",(int)f);
		else if(f>=100)
			vl.Format (" %.1f",f);
		else if(f>=10)
			vl.Format (" %.2f",f);
		else if(f>=0)
			vl.Format (" %.3f",f);
		else
			vl.Format ( " %.4f",f);
*/
		pnm+=vl;
		if(num>1)
		{
			if(i!=num-1 && !(m_nLineIndicator1[m_nSon]>0 && i==m_nLineIndicator1[m_nSon]-1))
				pnm+=" ";
		}
		pDC->ExtTextOut (left+len,r.top ,ETO_CLIPPED,r,pnm,NULL);
		len+=(pDC->GetOutputTextExtent(pnm)).cx;
	}
	pDC->SetTextColor(  pDoc->m_colorArray[1]);
	pDC->ExtTextOut (left+len,r.top ,ETO_CLIPPED,r," ",NULL);

	pDC->SelectObject(pOld);

	//to set buysell infomation
	if(pView->m_isShowCross == 1)
	{
		if(pView->m_nKlineKind2 ==HS_KLINE)
		{
			if(pView->m_hs.GetCount()>footCur )
				CTaiKlineDlgDealHistory::GetBuySellInfoPerFoot(pView->m_sharesSymbol , pView->pMin1Drawing ->m_pReportData, footCur, pView->m_hs , false);
		}
		else if(pView->m_nKlineKind2 ==MIN1_KLINE)
		{
			if(pView->pMin1Drawing ->m_footEnd>=footCur )
				CTaiKlineDlgDealHistory::GetBuySellInfoPerFoot(pView->m_sharesSymbol , pView->pMin1Drawing ->m_pReportData, footCur, pView->pMin1Drawing ->m_hsMin1 , false);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawCaption(CDC *pDC)
//      功能说明：计算子图的技术指标的数值
//      入口参数：pDC为传入的设备描述表指针
//      出口参数：无出口数
//      返回参数：无返回数
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DrawCaptionAdded(CDC *pDC)
{//?
	if(pView->pMin1Drawing->m_pReportData==NULL)
		return;
	if(m_nSon!=0)
		return;
	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];
	//lgf.lfHeight=-12;
//	lgf.lfWidth=8;
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);
	pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
	pDC->SetTextColor(  pDoc->m_colorArray[1]);
	pDC->SetBkMode(TRANSPARENT);

	//---		标题
	CString title;
	title=pView->pMin1Drawing->m_pReportData->name;
	CString name = pView->pMin1Drawing->m_pReportData->id ;
	title = name + title;

	title+= " 主图叠加";
	pDC->TextOut (2,0,title);

	int left=(pDC->GetOutputTextExtent(title)).cx+4;

	CRect r;
	r.left=0;
	r.right=m_rectDrawLine.right;
	r.top=0;
	r.bottom =pView->m_heightCaption;

	int len=0;
	name="  ";
	pDC->TextOut(left+len,0,name);
	len+=(pDC->GetOutputTextExtent(name)).cx;

	for(int i=0;i<m_nKlineCurrent;i++)
	{ 
		CString pnm=m_nameAdd[i];
		pDC->SetTextColor( pDoc->m_colorArray[7+i]);
		//---		输出数值
		if(m_nKlineCurrent>1)
		{
			if(i<m_nKlineCurrent-1)
				pnm+=" ";
		}
		pDC->ExtTextOut (left+len,0,ETO_CLIPPED,r,pnm,NULL);
		len+=(pDC->GetOutputTextExtent(pnm)).cx;
	}
	pDC->SetTextColor(  pDoc->m_colorArray[1]);
	pDC->ExtTextOut (left+len,0,ETO_CLIPPED,r,"  ",NULL);

	pDC->SelectObject(pOld);

}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：TimeToStr(int foot)
//      功能说明：时间转换为字符串
//      入口参数：foot为数组的下标
//      出口参数：无出口数
//      返回参数：返回的字符串
/////////////////////////////////////////////////////////////////////////////
CString CTaiKlineShowKline::TimeToStr(int foot)//
{
	CString s="";
	if(foot+m_footBegin>m_footEnd)
		return s;
	if(!ValidDoKline()) return s;


	CTime tmB(1970,1,1,8,0,0);
	CTime tmNow;

	if(m_footBegin+foot<0) return s;

	tmNow=tmB+pView->m_pkline[m_footBegin+foot].day;

	if(pView->m_nKlineKind2 <5)
	{
//		tmNow = tmNow+5400;
		s=tmNow.Format("%m/%d/%H:%M");
	}
	else if(pView->m_nKlineKind2 < MANY_DAY_KLINE ||pView->m_nKlineKind2 == MANY_DAY_KLINE&&pDoc->m_propertyInitiate.daysOfManyKline >0)
	{
		s=tmNow.Format("%Y/%m/%d");
		//s=tmNow.Format("%Y/%m/%d,%w");
		switch(tmNow.GetDayOfWeek())
		{
		case 1:
			s += "日";
			break;
		case 2:
			s += "一";
			break;
		case 3:
			s += "二";
		    break;
		case 4:
			s += "三";
		    break;
		case 5:
			s += "四";
			break;
		case 6:
			s += "五";
			break;
		case 7:
			s += "六";
		    break;
		default:
		    break;
		}
	}
	else
	{
		s=tmNow.Format ("%H:%M");
	}
	return s;


}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：SetShowTotal(int num)
//      功能说明：设置显示的起始点和结束点
//      入口参数：num为显示的总个数
//      出口参数：无出口数
//      返回参数：返回成功与否
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::SetShowTotal(int num)//设置显示的起始点和结束点
{
	if(num>=m_footEnd+1)
	{
		if(m_footBegin==0)
			return 0;
		else
		{
			m_footBegin=0;
		}
	}
	else
	{
		m_footBegin=m_footEnd-num;
	}
	if(m_footEnd>=m_klinNumDefault-1)
		m_klinNumDefault=m_footEnd+1-m_footBegin;

	ValidDoKline(true);
	return 1;

}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：MoveDisplays(int num)
//      功能说明：移动显示K 线画面
//      入口参数：num为移动的偏移量
//      出口参数：无出口数
//      返回参数：返回成功与否
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::MoveDisplays(int num)//移动显示K 线画面
{
	if(pView->m_nCountKline<=0) return 0;
	if(num>0)
	{
		if(m_footEnd<=pView->m_nCountKline -1)
		{
			m_footEnd=(m_footEnd+num<pView->m_nCountKline -1)?(m_footEnd+num) :(pView->m_nCountKline -1) ;
			m_footBegin = m_footBegin + num;

			if(m_footBegin >=pView->m_nCountKline)
				m_footBegin = pView->m_nCountKline-1;
		}
		else
		{
			return 0;
		}
	}
	else if(num<0)
	{
		if(m_footBegin>0)
		{
			if(m_footBegin+num>0)
			{
				int tot = m_footEnd-m_footBegin+1;
				m_footBegin=m_footBegin+num  ;
				if(tot==m_klinNumDefault)
					m_footEnd+=num;
				else
				{
					if(m_footEnd-m_footBegin+1>m_klinNumDefault)
						m_footEnd=m_footBegin+m_klinNumDefault-1;
				}
			}
			else
			{
				int tot = m_footEnd-m_footBegin+1;
				m_footBegin=0 ;
				if(tot==m_klinNumDefault)
						m_footEnd=m_footBegin+m_klinNumDefault-1;
				else
				{
					if(m_footEnd-m_footBegin+1>m_klinNumDefault)
						m_footEnd=m_footBegin+m_klinNumDefault-1;
				}
			}
		}
		else
		{
			return 0;
		}
	}

	ValidDoKline(true);
	return 1;


}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：OutString(int num, CPoint p)
//      功能说明：输出字符串
//      入口参数：num为第几个子图
//                p为点的坐标
//      出口参数：无出口数
//      返回参数：返回成功与否
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::OutString(int num, CPoint p)//输出字符串
{
	if(pView->pMin1Drawing->m_pReportData==NULL)
		return 0;
	if(!ValidDoKline()) return 0;
	m_nSon=num;

	SetRectCurrent(m_nSon);

	CRect r;
	pView->GetClientRect(r);


	int foot;
	foot=TransferX(p.x);
	foot+=m_footBegin;
	if(foot>m_footEnd)
			return 0;

	//--		输出当前值
//	m_currentValue=YTransfer(p.y);

	float widper=pView->m_rtKlineFiguer .rightX/(float)m_klinNumDefault;
	for(int i=0;i<m_dataFormular[m_nSon].numLine ;i++)
	{
		CRgn rgn;

		if(foot<m_dataFormular[m_nSon].line[i].bNow) break;
		else if(foot>m_dataFormular[m_nSon].line[i].bNow)
		{
			pt[0].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot-1])-2;
			pt[1].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot-1])+2;
			pt[2].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])+2;
			pt[5].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])-2;
			int add=1;
			if(foot==m_footEnd)
				add=0;
			pt[3].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot+add])+2;
			pt[4].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot+add])-2;
//			pt[4].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot-1])-1;
		}
		else
		{
			pt[0].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])-2;
			pt[1].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])+2;
			pt[2].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])+2;
			pt[3].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])+2;
			pt[4].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])-2;
			pt[5].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])-2;
//			pt[4].y=YTransfer((float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot])-1;
		}
		pt[0].x=(int)((TransferX(p.x)-1)*widper+widper/2)-1;
		pt[1].x=(int)((TransferX(p.x)-1)*widper+widper/2)-1;
		pt[2].x=(int)((TransferX(p.x))*widper+widper/2);
		pt[3].x=(int)((TransferX(p.x)+1)*widper+widper/2)+1;
		pt[4].x=(int)((TransferX(p.x)+1)*widper+widper/2)+1;
		pt[5].x=(int)((TransferX(p.x))*widper+widper/2);
//		pt[4].x=foot-1;

		for(int ii=0;ii<6;ii++)
		{
			if(pt[ii].y<m_rectDrawLine.top) pt[ii].y = m_rectDrawLine.top;
			if(pt[ii].y>m_rectDrawLine.bottom) pt[ii].y = m_rectDrawLine.bottom;
		}

		rgn.CreatePolygonRgn (pt,6,ALTERNATE);
		if(rgn.PtInRegion (p)!=0)
		{
			//返回字符串=
			CString rStr;
			rStr.Format (" %.3f\n",(float)m_dataFormular[m_nSon].line[i].m_arrBE.line[foot]);
			m_tipStr=m_lineName[m_nSon][i]+rStr;
			m_tipStr.MakeUpper( );

			return 1;
		}
	}


	if(num>0) return 0;

	r.top =YTransfer((float)pView->m_pkline[foot].high);
	r.bottom =YTransfer((float)pView->m_pkline[foot].low)+1;
	float width=(pView->m_rtKlineFiguer .rightX*3/(float)m_klinNumDefault/10);
//	float widper=pView->m_rtKlineFiguer .rightX/(float)m_klinNumDefault;
//	pt[0].x=(int)((TransferX(p.x)-1)*widper+widper/2);
	r.left =(int)((TransferX(p.x))*widper+widper/2-width);
	r.right =(int)((TransferX(p.x))*widper+widper/2+width)+1;
/*		if(pView->pKlineMode!=NULL)
			pView->pKlineMode->DrawKLineMode(pView,pDC);
		//--画基本信息
		if(pView->m_bBaseInfo==true)
			DrawBaseInfo(pDC);

		//画除权
		if(pView->m_bDoPower ==true)
			DrawPower(pDC);

		if(pView->m_tjxgShow!=NULL)
		{
*/	if(r.PtInRect (p)==0)
	{
		r.left =(int)((TransferX(p.x))*widper+widper/2)-7;
		r.right =(int)((TransferX(p.x))*widper+widper/2)+7;
		//基本资料显示
		CTime tmB(1970,1,1,9,30,0);

		//条件选股指示
		while(pView->m_screenStockShow .m_bUseing == true)
		{
			if(pView->m_screenStockShow .m_testResultArray.GetSize()!=1)
				break;

			SCREEN_RESULT* pResult = pView->m_screenStockShow .m_testResultArray[0];
			int n = pResult->m_detail .GetSize();
			if(n<=0)
				break;

			//显示图标
/*			int tmBegin = pView->m_pkline[foot].day;//tm.GetYear ()*10000 + tm.GetMonth()*100 + tm.GetDay ();
			int tmEnd = pView->m_pkline[foot].day;//	int tmBeginHour = tm.GetHour ()*100+tm.GetMinute ();
			bool bday = false;
			if(CTaiKlineFileKLine::IsDayKline(pView->m_nKlineKind2  ))
			{
				CTime tm(pView->m_pkline[foot].day);
				tmBegin = CTime(tm.GetYear (),tm.GetMonth(),tm.GetDay (),0,0,0).GetTime();
				tmEnd = CTime(tm.GetYear (),tm.GetMonth(),tm.GetDay (),23,59,59).GetTime();
				bday=true;
			}*/
			for(int j=0;j<n;j++)
			{
				SCREEN_DETAIL* pDetail = &(pResult->m_detail[j]);
                int foot2;
				if(pResult->nKindBuySell==1)
					foot2 = pView->FindTimeK (pDetail->selltime);
				else
				    foot2 = pView->FindTimeK (pDetail->buytime);
				int posNow=(int)((foot2-m_footBegin+(float)0.5)*widper);
				if(posNow>r.left && posNow<r.right)
				{
//				}
//				if(pDetail->buytime >=tmBegin && pDetail->buytime <=tmEnd)
//				{
					//显示信息
					CString sP="";
                    /*if(pDoc->m_propertyInitiate.bTjxgZsFs==1)
						if(pResult->nKindBuySell==0)
						{
							m_tipStr="只有买入条件\n买入时间：";
                            sP.Format("%.2",pDetail->buytime);
							m_tipStr+=sP;
							m_tipStr=m_tipStr+"%"+"\n"+"买入价格：";
							sP.Format("%.2",pDetail->buyprice);
							m_tipStr+=sP;
						}
						else  if(pResult->nKindBuySell==1)
							{
								m_tipStr="只有卖出条件\n卖出时间：";
								sP.Format("%.2",pDetail->selltime);
								m_tipStr+=sP;
								m_tipStr=m_tipStr+"%"+"\n"+"卖出价格：";
								sP.Format("%.2",pDetail->sellprice);
								m_tipStr+=sP;
							}
                        else 
						{*/
							m_tipStr="买卖评测指示：\n实际最大收益 ";
							sP.Format("%.2f",pDetail->profit);
							m_tipStr+=sP;
							m_tipStr=m_tipStr+"%"+"\n";
							sP.Format("%d天目标收益%.2f",(int)pView->m_screenStockShow .m_mbzq,(float)pView->m_screenStockShow .m_mbly);
							m_tipStr+=sP;
							m_tipStr=m_tipStr+"%"+"\n";

							if(pDetail->issuccess>=0 && pDetail->issuccess<=2)
							{
								CString s1[] = {"成功","没有成功","未完成收益计算"};
								sP = s1[pDetail->issuccess ];
								m_tipStr+=sP;
								m_tipStr+="\n";
							}
							 
							if(pResult->nKindBuySell>=0 && pResult->nKindBuySell<=2)
							{
								CString sKind[] = {"仅有买入条件\n","仅有卖出条件\n","同时有买入卖出条件\n"};
								sP = sKind[pResult->nKindBuySell ];
								m_tipStr+=sP;
							}
                            if(pResult->nKindBuySell==0)
							{
    							sP ="买入时间："+ CTime(pDetail->buytime  ).Format ("%Y%m%d  ");
								m_tipStr+=sP;
								sP.Format("买入价格：%.2f\n",pDetail->buyprice );
								m_tipStr+=sP;

							}
							else
							{
								sP ="卖出时间："+ CTime(pDetail->selltime  ).Format ("%Y%m%d  ");
								m_tipStr+=sP;
								sP.Format("卖出价格：%.2f\n",pDetail->sellprice );
								m_tipStr+=sP;
							}
							if(pResult->nKindBuySell ==2)
							{
								if(pDetail->nKindSell ==0 || pDetail->nKindSell ==1)
								{
									CString sKindSell[] = {"卖出条件卖出","附加条件卖出"};
									sP = sKindSell[pDetail->nKindSell ];
									m_tipStr+=sP;
									m_tipStr+="\n";
								}
							}
							sP="";
							for(int i=0;i<pView->m_screenStockShow .m_indexSaveArray.GetSize();i++)
							{
								IndexDataInfo index_save = pView->m_screenStockShow .m_indexSaveArray.GetAt(i);
								CFormularContent * pIndex;
								pIndex=(CFormularContent*)CTaiScreenParent::LookUpArray(index_save.iKind, index_save.sIndex_name,pDoc); 
								if(pIndex==NULL) continue;
								sP=sP+pIndex->name+"条件\n";
						}
						m_tipStr+=sP;
						
					m_tipStr.MakeUpper( );

					return 1;
				}
			}
			break;
		}
/*		if(pView->m_tjxgShow!=NULL)
		{
			if(pView->m_tjxgShow->chooseresult.m_detail.GetSize()>0)
			{
				for(int j=0;j<pView->m_tjxgShow->chooseresult.m_detail.GetSize();j++)
				{
					SCREEN_DETAIL_L * detail;
					detail=pView->m_tjxgShow->chooseresult.m_detail.GetAt(j);
					int posNow=(int)((detail->buytime-m_footBegin+(float)0.5)*widper);
					if(posNow>r.left && posNow<r.right)
					{
						r.top =YTransfer((float)pView->m_pkline[foot].low)+2;
						r.bottom =r.top+18;
						if(r.bottom>pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-1)
						{
							r.top =pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-18;
							r.bottom =r.top+16;
						}
						if(r.PtInRect (p)!=0)
						{
							//out string 
							CString sP="";

							m_tipStr="条件选股：\n实际最大收益 ";
							sP.Format("%.2f",detail->profit);
							m_tipStr+=sP;
							m_tipStr=m_tipStr+"%"+"\n";
							sP.Format("%d天目标收益%.2f",(int)pView->m_tjxgShow->m_range,(float)pView->m_tjxgShow->m_profit);
							m_tipStr+=sP;
							m_tipStr=m_tipStr+"%"+"\n";
							sP="";
							for(int i=0;i<pView->m_tjxgShow->m_choosecondition.GetSize();i++)
							{
								int index;
								index=pView->m_tjxgShow->m_choosecondition.GetAt(i);
								CFormularContent * jishu;
								jishu=pDoc->m_formuar_choose.GetAt(index);
								sP=sP+jishu->name+"条件选股\n";
							}
							m_tipStr+=sP;

							m_tipStr.MakeUpper( );

							return 1;
						}
					}

				}
			}
		}
*/		//--	除权显示
		if(pView->m_bDoPower ==true)
		{
			if(p.y<pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-18)
				return 0;

			r.top =pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-18;
			r.bottom =pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-2;
			POWER pwr;
			for(int i=0;i< m_PowerArray.GetSize();i++)
			{
				pwr=m_PowerArray[i];

				int foot2=pView->FindTimeK ((int)pwr.nTime);
				int posNow=(int)((foot2-m_footBegin+(float)0.5)*widper);
				if(posNow>r.left && posNow<r.right)
				{
					if(r.PtInRect (p)!=0)
					{
						CString sP="";
//						sP.Format("送股数：%.1f，分红：%.2f，配股数：%.1f，配股价：%.2f",(float)(pwr.fGive*10),
//							(float)pwr.fDividend,(float)(pwr.fAllocate*10),(float)pwr.fAllocatePrice);
						CString sP2 = "";
						if(pwr.fGive>0)
							sP.Format("送股数：%.1f\n",(float)(pwr.fGive*10));
						sP2+=sP;
						sP="";
						if(pwr.fDividend>0)
							sP.Format("分红：%.2f\n",(float)(pwr.fDividend));
						sP2+=sP;
						sP="";
						if(pwr.fAllocate>0)
							sP.Format("配股数：%.1f\n",(float)(pwr.fAllocate*10));
						sP2+=sP;
						sP="";
						if(pwr.fAllocatePrice>0)
							sP.Format("配股价：%.2f",(float)(pwr.fAllocatePrice));
						sP2+=sP;

						sP2.TrimRight ("\n");


						m_tipStr=sP2;
						m_tipStr+="\n";
						m_tipStr.MakeUpper( );
						return 1;
					}
				}
			}
		}

		if(pView->m_bBaseInfo==true && pView->m_nKlineKind2 >= 5)
		{
			if(p.y < pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-18)
				return 0;

			try
			{
			if(!pView->m_bs.IsOpen())
				pView->m_bs.Open ();
			pView->m_bs.MoveFirst ();

			int footBs=TransferX(p.x)+m_footBegin;
			r.top =pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-18;
			r.bottom =pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-2;
//			r.left =(int)(TransferX(p.x)*widper+widper/2-width);
//			r.right =(int)(TransferX(p.x)*widper+widper/2+width)+1;

			while(!pView->m_bs.IsEOF() )
			{
				//--	计算时间
				int nDay=pView->m_bs.m_time .GetDay ();
				int nYear=pView->m_bs.m_time .GetYear ();
				int nMonth=pView->m_bs.m_time .GetMonth  ();
				CTime tmLib(nYear,nMonth,nDay,9,30,0);
				CTimeSpan tmAll=tmLib-tmB;
				int scds=tmAll.GetTotalSeconds ();
				int foot2=pView->FindTimeK (scds);
				if(foot2==-1)
				{
					pView->m_bs.MoveNext ();
					continue;
				}

				foot2-=m_footBegin;

				int posNow=(int)((foot2+(float)0.5)*widper);
				if(posNow<r.left || posNow>r.right)
				{
					pView->m_bs.MoveNext ();
					continue;
				}

				if(r.PtInRect (p)!=0)
				{
					m_tipStr=pView->m_bs.m_brief;
					m_tipStr+="\n";
					m_tipStr.MakeUpper( );
					return 1;
				}
				pView->m_bs.MoveNext ();
			}
			}
			catch (CDaoException* e)
			{
				e->Delete();
//				return 0;
			}
		}

		return 0;
	}

	CString s;
	s=TimeToStr(foot-m_footBegin);
	s+="\n开: ";
	CString tmps;
	CString fmt = "%.2f";
	if(CSharesCompute::GetMarketKind(pView->m_stkKind ) == SH_MARKET_EX  && pView->pMin1Drawing ->m_pReportData ->kind == 2)
		fmt = "%.3f";
	tmps.Format (fmt,pView->m_pkline[foot].open);
	s+=tmps;

	s+="\n高: ";
	tmps.Format (fmt,pView->m_pkline[foot].high);
	s+=tmps;

	s+="\n低: ";
	tmps.Format (fmt,pView->m_pkline[foot].low);
	s+=tmps;

	s+="\n收: ";
	tmps.Format (fmt,pView->m_pkline[foot].close);
	s+=tmps;
	
	s+="\n量: ";
	tmps.Format ("%d",(int)pView->m_pkline[foot].vol);
	s+=tmps;
	
	s+="\n额: ";
	tmps.Format ("%.1f",pView->m_pkline[foot].amount/10000);
	s+=tmps;
	s+="\n";

	m_tipStr=s;
	m_tipStr.MakeUpper( );

	
	return  1;
}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：TransferX(int x)
//      功能说明：把x坐标转换为对应的数组下标
//      入口参数：x为横轴坐标
//      出口参数：无出口数
//      返回参数：返回数组的下标
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::TransferX(int x)//把x坐标转换为对应的数组下标
{
	int rtn;
	return rtn=(int)((float)(x*m_klinNumDefault) /pView->m_rtKlineFiguer.rightX);
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：OnUp()
//      功能说明：向上箭头处理
//      入口参数：无
//      出口参数：无出口数
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::OnUp()//向上箭头处理
{
	//---		计算总个数
	if(m_klinNumDefault<=10 || pView->m_pkline == 0)
		return 0;

	if(!ValidDoKline()) return 0;
	int tot=m_footEnd-m_footBegin+1;
	int totGap = m_klinNumDefault- tot;

	m_klinNumDefault=(int)(m_klinNumDefault*0.618);
	if(m_klinNumDefault<10)
		m_klinNumDefault=10;

	if(tot>m_klinNumDefault)
	{
		m_footBegin=m_footEnd-(m_klinNumDefault-totGap)+1;
		if(m_footBegin<0)
			m_footBegin=0;
	}

/*	CRect r;
	pView->GetClientRect (r);
	r.right =pView->m_rtKlineFiguer.rightX+pView->m_midLen;
	CTaiKlineDC* pMemdc = new CTaiKlineDC(pView,&pView->m_bit);
	DrawKlineFiguer(pMemdc);*/

	if(pView->m_isShowCross==1)
	{
		if(m_footCurrent>m_footEnd)
			m_footCurrent=m_footEnd;
		else if(m_footCurrent<m_footBegin)
			m_footCurrent=m_footBegin;

		if(m_footCurrent<0) m_footCurrent=0;
		m_currentValue=pView->m_pkline[m_footCurrent].close;
		int foot=m_footCurrent-m_footBegin;
		int xNow=(int)((foot+0.5)*(m_rectDrawLine.right-m_rectDrawLine.left)/m_klinNumDefault);
		int yNow=YTransfer(pView->m_pkline[m_footCurrent].close);
		pView->m_pointMove.x=xNow;
		pView->m_pointMove.y=yNow;
//		pView->ShowShiZi (xNow,yNow);
	}
	pView->RedrawWindow ();
//	pView->RefreshChenben(pMemdc);
//	delete pMemdc;
	ValidDoKline(true);
	return 1;


}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：OnDown()
//      功能说明：向下箭头处理
//      入口参数：无
//      出口参数：无出口数
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::OnDown()
{
	if(m_klinNumDefault<=0 || pView->m_pkline==0)
		return 0;

	if(!ValidDoKline()) return 0;

	if(!(::GetKeyState(VK_CONTROL)&0x8000))
//	if(pView->m_nCountKline>=85)
	{
		//---		计算总个数
		if(m_klinNumDefault>=pView->m_nCountKline +BLANK_KLINE)
			return 0;

		int tot=m_footEnd-m_footBegin+1;
		tot = m_klinNumDefault - tot;
		ASSERT(tot>=0);

		m_klinNumDefault=(int)(m_klinNumDefault/0.618);
		if(m_klinNumDefault>=pView->m_nCountKline+BLANK_KLINE)
			m_klinNumDefault=pView->m_nCountKline+BLANK_KLINE;

		m_footBegin=m_footEnd-(m_klinNumDefault-tot)+1;

		if(m_footBegin<0)
			m_footBegin=0;

		m_footEnd = m_klinNumDefault+m_footBegin-1;
		if(m_footEnd >= pView->m_nCountKline )//m_footBegin
			m_footEnd =pView->m_nCountKline-1; 

	}
	else
	{
		if(m_klinNumDefault>=pView->m_nCountKline )
			return 0;

		m_klinNumDefault=(int)(m_klinNumDefault/0.812);
		if(m_klinNumDefault>=pView->m_nCountKline)
			m_klinNumDefault=pView->m_nCountKline;

		if(m_klinNumDefault>6400)
			m_klinNumDefault=6400;

		if(pView->m_nCountKline - m_footBegin >=m_klinNumDefault)
			m_footEnd=m_footBegin+m_klinNumDefault-1;
		else
		{
			m_footEnd=pView->m_nCountKline - 1;
		}
	}

/*	CRect r;
	pView->GetClientRect (r);
	r.right =pView->m_rtKlineFiguer.rightX+pView->m_midLen;
	CTaiKlineDC* pMemdc = new CTaiKlineDC(pView,&pView->m_bit);
	DrawKlineFiguer(pMemdc);
*/
	if(pView->m_isShowCross==1)
	{
		if(m_footCurrent>m_footEnd)
			m_footCurrent=m_footEnd;
		else if(m_footCurrent<m_footBegin)
			m_footCurrent=m_footBegin;

		if(m_footCurrent<0) m_footCurrent=0;
		m_currentValue=pView->m_pkline[m_footCurrent].close;
		int foot=m_footCurrent-m_footBegin;
		int xNow=(int)((foot+0.5)*(m_rectDrawLine.right-m_rectDrawLine.left)/m_klinNumDefault);
		int yNow=YTransfer(pView->m_pkline[m_footCurrent].close);
		pView->m_pointMove.x=xNow;
		pView->m_pointMove.y=yNow;
//		pView->ShowShiZi (xNow,yNow);
	}
	pView->RedrawWindow ();
//	pView->RefreshChenben(pMemdc);
//	delete pMemdc;
	ValidDoKline(true);
	return 1;
}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：OnLeft()
//      功能说明：向左箭头处理
//      入口参数：无
//      出口参数：无出口数
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::OnLeft()
{
	if(m_klinNumDefault<=0 || pView->m_pkline == 0)
			return 0;
	if(::GetKeyState(VK_CONTROL)&0x8000)
	{
//		int i=::GetKeyState(VK_LEFT);
		if(::GetKeyState(VK_SHIFT)&0x8000)
		{
			MoveDisplays(m_footEnd-m_footBegin+1);
//			m_footCurrent-=m_footEnd-m_footBegin+1;
		}
		else
		{
			MoveDisplays(1);
//			m_footCurrent--;
		}
//		if(m_footCurrent <0) m_footCurrent = 0;
		if(pView->m_isShowCross!=0)
		{
			int foot=m_footCurrent-m_footBegin;
			if(foot<0) foot =0;
			int xNow=(int)((foot+0.5)*(m_rectDrawLine.right-m_rectDrawLine.left)/m_klinNumDefault);
			int yNow=YTransfer(pView->m_pkline[m_footCurrent].close);
			pView->m_pointMove.x=xNow;
			pView->m_pointMove.y=yNow;
			if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE) pView->m_dlgShowCross->ShowWindow(SW_SHOW);
		}
//		pView->RedrawWindow ();
		
		CRect r;
		pView->GetClientRect (r);
		r.right =pView->m_rtKlineFiguer.rightX+pView->m_midLen;
		CTaiKlineDC* pMemdc = new CTaiKlineDC(pView,&pView->m_bit,&r);
		DrawKlineFiguer(pMemdc);
		delete pMemdc;

	}
	else
	{
		if(pView->m_isShowCross==0)
		{
			pView->m_nOldHitPos =-1;
			pView->m_nOldHitY =-1;
			m_footCurrent=m_footEnd;
			if(m_footCurrent<0) m_footCurrent=0;
			//---		确定十字光标的位置 
			m_nSon=0;
			SetRectCurrent(0);

			int foot=m_footEnd-m_footBegin;
			int xNow=(int)((foot+0.5)*(m_rectDrawLine.right-m_rectDrawLine.left)/m_klinNumDefault);
			int yNow=YTransfer(pView->m_pkline[m_footCurrent].close);
			pView->m_pointMove.x=xNow;
			pView->m_pointMove.y=yNow;

			pView->m_isShowCross=1;
			//---	show window
			pView->RedrawWindow ();
			if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE) pView->m_dlgShowCross->ShowWindow(SW_SHOW);
			pView->SetFocus ();
		} 
		else
		{
			m_footCurrent--;
			if(::GetKeyState(VK_SHIFT)&0x8000)
				m_footCurrent=m_footCurrent-(m_footEnd-m_footBegin)/10;
		}

		if(m_footCurrent>m_footEnd||m_footCurrent<m_footBegin )
		{
			m_footCurrent=m_footEnd;
		}

		if(m_footCurrent<0) m_footCurrent=0;
		m_currentValue=pView->m_pkline[m_footCurrent].close;

//		pView->HistoryAStep(m_footCurrent);
		//---		确定十字光标的位置 
		m_nSon=0;
		SetRectCurrent(m_nSon);

		int foot=m_footCurrent-m_footBegin;
		int xNow=(int)((foot+0.5)*(m_rectDrawLine.right-m_rectDrawLine.left)/m_klinNumDefault);
		int yNow=YTransfer(pView->m_pkline[m_footCurrent].close);
		pView->m_pointMove.x=xNow;
		pView->m_pointMove.y=yNow;
		/////////////////////////////////zjh///////////////////
		POINT CurPos;
		CurPos.x = xNow;
		CurPos.y = yNow;
		ClientToScreen(pView->m_hWnd,&CurPos);
		if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE)
		{
		CRect dlgRect;
		GetWindowRect(pView->m_dlgShowCross->m_hWnd,&dlgRect);
		if(PtInRect(dlgRect,CurPos))
		{
		  CRect ClientRect;
		  pView->GetClientRect(&ClientRect);
		  if(CurPos.x > ClientRect.Width()/2)
			  MoveWindow(pView->m_dlgShowCross->m_hWnd,ClientRect.Width()/2 - dlgRect.Width(),CurPos.y,dlgRect.Width(),dlgRect.Height(),TRUE);
		  else
              MoveWindow(pView->m_dlgShowCross->m_hWnd,ClientRect.Width()/2,CurPos.y,dlgRect.Width(),dlgRect.Height(),TRUE);
		}
		}
		SetCursorPos(CurPos.x,CurPos.y);
		//////////////////////////////////////////

	}
	ValidDoKline(true);
	return 1;


}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：OnRight()
//      功能说明：向右箭头处理
//      入口参数：无
//      出口参数：无出口数
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::OnRight()
{
	if(m_klinNumDefault<=0 || pView->m_pkline == 0)
			return 0;

	m_nSon=0;
	SetRectCurrent(m_nSon);

	if(::GetKeyState(VK_CONTROL)&0x8000)
	{
		if(::GetKeyState(VK_SHIFT)&0x8000)
		{
			MoveDisplays(m_footBegin-m_footEnd-1);
//			m_footCurrent+=m_footEnd-m_footBegin+1;
		}
		else
		{
			MoveDisplays(-1);
//			m_footCurrent++;
		}
//		if(m_footCurrent >m_footEnd) m_footCurrent = m_footEnd;
		if(pView->m_isShowCross!=0)
		{
			int foot=m_footCurrent-m_footBegin;
			if(foot<0) foot =0;
			int xNow=(int)((foot+0.5)*(m_rectDrawLine.right-m_rectDrawLine.left)/m_klinNumDefault);
			int yNow=YTransfer(pView->m_pkline[m_footCurrent].close);
			pView->m_pointMove.x=xNow;
			pView->m_pointMove.y=yNow;
			if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE) pView->m_dlgShowCross->ShowWindow(SW_SHOW);
		}
//		pView->RedrawWindow ();
		CRect r;
		pView->GetClientRect (r);
		r.right =pView->m_rtKlineFiguer.rightX+pView->m_midLen;
		CTaiKlineDC* pMemdc = new CTaiKlineDC(pView,&pView->m_bit,&r);
		DrawKlineFiguer(pMemdc);
		delete pMemdc;
	}
	else
	{
		if(pView->m_isShowCross==0)
		{
			pView->m_nOldHitPos =-1;
			pView->m_nOldHitY =-1;
			m_footCurrent=m_footBegin;
			if(m_footCurrent<0) m_footCurrent=0;
			//---		确定十字光标的位置 
			int foot=0;
			int xNow=(int)((foot+0.5)*(m_rectDrawLine.right-m_rectDrawLine.left)/m_klinNumDefault);
			int yNow=YTransfer(pView->m_pkline[m_footCurrent].close);
			pView->m_pointMove.x=xNow;
			pView->m_pointMove.y=yNow;

			pView->m_isShowCross=1;
			pView->RedrawWindow ();
			//---	show window
			if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE) pView->m_dlgShowCross->ShowWindow(SW_SHOW);
			pView->SetFocus ();

		}
		else
		{
			m_footCurrent++;
			TRACE("\n m_footCurrent      %d\n",m_footCurrent);
			if(::GetKeyState(VK_SHIFT)&0x8000)
				m_footCurrent=m_footCurrent+(m_footEnd-m_footBegin)/10;
		}

		if(m_footCurrent>m_footEnd||m_footCurrent<m_footBegin )
		{
			m_footCurrent=m_footBegin;
		}

		if(m_footCurrent<0) m_footCurrent=0;
		m_currentValue=pView->m_pkline[m_footCurrent].close;
//		pView->HistoryAStep(m_footCurrent);
		//---		确定十字光标的位置 
		int foot=m_footCurrent-m_footBegin;
		int xNow=(int)((foot+0.5)*(m_rectDrawLine.right-m_rectDrawLine.left)/m_klinNumDefault+0.5);
		int yNow=YTransfer(pView->m_pkline[m_footCurrent].close);
		pView->m_pointMove.x=xNow;
		pView->m_pointMove.y=yNow;
/////////////////////////////////zjh///////////////////
		CRect dlgRect;
		POINT CurPos;
		CurPos.x = xNow;
		CurPos.y = yNow;
		ClientToScreen(pView->m_hWnd,&CurPos);
		if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE)
		{
		GetWindowRect(pView->m_dlgShowCross->m_hWnd,&dlgRect);
		if(PtInRect(dlgRect,CurPos))
		{
		  CRect ClientRect;
		  pView->GetClientRect(&ClientRect);
		  if(CurPos.x > ClientRect.Width()/2)
			  MoveWindow(pView->m_dlgShowCross->m_hWnd,ClientRect.Width()/2 - dlgRect.Width(),CurPos.y,dlgRect.Width(),dlgRect.Height(),TRUE);
		  else
              MoveWindow(pView->m_dlgShowCross->m_hWnd,ClientRect.Width()/2,CurPos.y,dlgRect.Width(),dlgRect.Height(),TRUE);
		}
		}
		SetCursorPos(CurPos.x,CurPos.y);
		//////////////////////////////////////////
	}
	ValidDoKline(true);
	return 1;


}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：OnEnd()
//      功能说明：响应END键
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::OnEnd()
{
		if(m_klinNumDefault<=0)
			return 0;
		if(pView->m_isShowCross==0)
		{
			//---	show window
			if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE)pView->m_dlgShowCross->ShowWindow(SW_RESTORE);
			pView->m_isShowCross=1;
		}

		m_footCurrent=m_footEnd;

		if(m_footCurrent<0) m_footCurrent=0;
		m_currentValue=pView->m_pkline[m_footCurrent].close;
		//---		确定十字光标的位置 
		m_nSon=0;
		SetRectCurrent(m_nSon);

		int foot=m_footCurrent-m_footBegin;
		int xNow=(int)((foot+0.5)*(m_rectDrawLine.right-m_rectDrawLine.left)/m_klinNumDefault);
		int yNow=YTransfer(pView->m_pkline[m_footCurrent].close);
		pView->m_pointMove.x=xNow;
		pView->m_pointMove.y=yNow;
		ValidDoKline(true);
	    return 1;


}
int CTaiKlineShowKline::OnHome()
{
		if(m_klinNumDefault<=0 || pView->m_pkline == 0)
			return 0;
		if(pView->m_isShowCross==0)
		{
			//---	show window
			if(pDoc->m_propertyInitiate.bShowCrossDlg == TRUE)pView->m_dlgShowCross->ShowWindow(SW_RESTORE);
			pView->m_isShowCross=1;
		}

		m_footCurrent=m_footBegin;

		if(m_footCurrent<0) m_footCurrent=0;
		m_currentValue=pView->m_pkline[m_footCurrent].close;
		//---		确定十字光标的位置 
		m_nSon=0;
		SetRectCurrent(m_nSon);

		int foot=m_footCurrent-m_footBegin;
		int xNow=(int)((foot+0.5)*(m_rectDrawLine.right-m_rectDrawLine.left)/m_klinNumDefault);
		int yNow=YTransfer(pView->m_pkline[m_footCurrent].close);
		pView->m_pointMove.x=xNow;
		pView->m_pointMove.y=yNow;
		ValidDoKline(true);
	    return 1;


}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawRulorTextLog(CDC *pDC,BYTE wid)
//      功能说明：显示对数标尺
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::DrawRulorTextLog(CDC *pDC,BYTE wid)
{
	if(m_nSon>0||m_axisType==0||pView->m_nKlineKind2==0)
		return 0;
	if(wid!=0)
		return 0;

	CRect rt(m_rectDrawLine.right,m_rectDrawLine.top,
		m_rectDrawLine.right+pView->m_midLen,m_rectDrawLine.bottom);

	if(m_rectDrawLine.bottom-m_rectDrawLine.top<2)
		return 0;

	//判断数据是否有效
	if(m_nSon==0)
	{
		if(m_min_sun[m_nSon]<=0 || m_max_sun[m_nSon]<=0)
		{
			if(m_axisType==2)
			{
//				AfxMessageBox("无法以对数坐标画出");
				m_axisType=0;
				return 0;
			}
		}
	}
	//------根据最大值及最小值来确定显示的数字的位数
	float max;
	max=(float)(FABSMY(m_max_sun[m_nSon])>FABSMY(m_min_sun[m_nSon])?FABSMY(m_max_sun[m_nSon]):FABSMY(m_min_sun[m_nSon]));
	m_step[m_nSon]=0;
	int flag=0;	//flag=0为浮点数，1为整数，2为大于等于100000的整数
	if(m_axisType==1)
	{
		max=(float)(FABSMY(m_max_sun[m_nSon]*100/pView->m_pkline[m_footBegin].open-100)>FABSMY(m_min_sun[m_nSon]*100/pView->m_pkline[m_footBegin].open-100)
			?FABSMY(m_max_sun[m_nSon]*100/pView->m_pkline[m_footBegin].open-100):FABSMY(m_min_sun[m_nSon]*100/pView->m_pkline[m_footBegin].open-100));
		if(max<100)
			flag=0;
		else
			flag=1;
	}
	else
	{
		if(max<100)
			flag=0;
		else
			flag=1;
	}

	//------确定画几条线
	int numLine=(m_rectDrawLine.bottom-m_rectDrawLine.top)/pView->m_minGrid+1;
	//------确定每格的数值
	float valuePer;
		//=1 百分比坐标
	if(m_axisType==1)
//		valuePer=(m_max_sun[m_nSon]-m_min_sun[m_nSon])*100/pView->m_pkline[m_footBegin].open/numLine;
		valuePer=(m_max_sun[m_nSon]-m_min_sun[m_nSon])*100/pView->m_pkline[m_footBegin].open/numLine;
	//=2  对数坐标
	else if(m_axisType==2)
	{
		if(m_max_sun[m_nSon]<=0||m_min_sun[m_nSon]<=0)
			return 0;

		valuePer=(float)(pow(10,(log10(m_max_sun[m_nSon])-log10(m_min_sun[m_nSon]))/numLine));
		if(valuePer<=1)
		{
//			AfxMessageBox("计算出非法数值");
			return 0;
		}
	}



	int flagGap=4;//4为画四条分隔线
	if(m_axisType==1)
	{
		float k=1;
		for(int i=0;;i++)
		{
			if(valuePer<(float)0.01*k)
			{
				valuePer=(float)0.01*k;
				break;
			}
			if(valuePer<0.02*k)
			{
				valuePer=(float)0.02*k;
				flagGap=3;
				break;
			}
			if(valuePer<0.05*k)
			{
				valuePer=(float)0.05*k;
				break;
			}
			k*=10;

		}
	}
	//---起始点   结束点
	int num_b;
	int num_all;
	//=1 百分比坐标
	float fBegin;
	if(m_axisType==1)
	{
//		num_b=(int)floor((m_min_sun[m_nSon]*100/pView->m_pkline[m_footBegin].open/valuePer))+1;
		num_all=0;
		int opn=YTransfer((float)(pView->m_pkline[m_footBegin].open));
		float fnow=pView->m_pkline[m_footBegin].open;
//*100/pView->m_pkline[m_footBegin].open
		float nowValuePer=valuePer*pView->m_pkline[m_footBegin].open/100;
		while(true)
		{
			fBegin=fnow;
			fnow-=nowValuePer;
			if(fnow<=m_min_sun[m_nSon])
				break;
			num_all++;	
			if(num_all>5000)
				break;
		}

		fnow=pView->m_pkline[m_footBegin].open;
		while(true)
		{
			num_all++;	
			fnow+=nowValuePer;
			if(fnow>m_max_sun[m_nSon])
				break;
			if(num_all>5000)
				break;
		}
		num_b=0;
	
	}
	//=2  对数坐标
	else if(m_axisType==2)
	{
		float maxDui=m_max_sun[m_nSon];
		num_all=0;
		valuePer=(float)((int)(100*valuePer)/100.0+0.01);
		if(valuePer<=1)
			return 0;
//		(valuePer>1);
		while(true)
		{
			num_all++;	
			maxDui/=valuePer;
			if(maxDui<=m_min_sun[m_nSon])
				break;
			if(num_all>5000)
				break;
		}
		num_b=0;
	}

	CPen pen_solid(PS_SOLID ,1,pDoc->m_colorArray[2]);//PS_DASHDOTDOT   
	CPen* pOldpen=pDC->SelectObject(&pen_solid);

	int m=2;

	//---		画线
	float gap;
	if((m_max_sun[m_nSon]-m_min_sun[m_nSon])!=0&&(flagGap+1)!=0)
	{
		if(m_axisType==1)
			gap=((m_rectDrawLine.bottom-m_rectDrawLine.top))*valuePer*pView->m_pkline[m_footBegin].open/100/(m_max_sun[m_nSon]-m_min_sun[m_nSon])/(flagGap+1);
		//=2  对数坐标
		else
			gap=(YTransfer((float)(m_max_sun[m_nSon]/valuePer))-YTransfer((float)(m_max_sun[m_nSon])))/(float)5.0;
	}
	else
		return 0;


	//draw line from data
	if(DrawRulorTextSelf(pDC, wid)== true)
	{
		return 1;
	}

	int i;
	for(i=0;i<num_all;i++)
	{
		int y=0;
		if(m_axisType==1)
			y=YTransfer((float)(valuePer*pView->m_pkline[m_footBegin].open/100*(i)+fBegin));
		else//2
		{
			if((pow(valuePer,num_all-i-1))!=0)
				y=YTransfer((float)(m_max_sun[m_nSon]/(pow(valuePer,num_all-i-1))));
		}

		if(y<m_rectDrawLine.bottom-3 && y>=m_rectDrawLine.top)
		{
			//---		画虚线
			if(!(pDoc->m_systemOption.showxline==FALSE && (pView->m_nKlineKind2>0 && wid !=1)))
			{
				CPoint p1,p2;
				p1.x=m_rectDrawLine.left;
				p2.x=m_rectDrawLine.right+1;
				p1.y=y;
				p2.y=y;
				ShowVirtualLine(pDC,p1,p2);
			}

			pDC->MoveTo(m_rectDrawLine.right+1,y);
			pDC->LineTo(m_rectDrawLine.right+6,y);
			int sm;
			for(sm=0;sm<flagGap;sm++)
			{
				int yNow=(int)(y-(sm+1)*gap);
				if(yNow>=m_rectDrawLine.top)
				{
					pDC->MoveTo(m_rectDrawLine.right+1,yNow);
					pDC->LineTo(m_rectDrawLine.right+3,yNow);
				}
			}
			if(i==0)
			{
				for( sm=0;sm<flagGap;sm++)
				{
					int yNow2=(int)(y+(sm+1)*gap);
					if(yNow2<m_rectDrawLine.bottom)
					{
						pDC->MoveTo(m_rectDrawLine.right+1,yNow2);
						pDC->LineTo(m_rectDrawLine.right+3,yNow2);
					}
				}
			}//end if
		}//end if
	}

	//---		画文字
	pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
	pDC->SetTextColor( pDoc->m_colorArray[2]);
	pDC->SetBkMode(TRANSPARENT);

	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];
	if(wid==1)
		lgf.lfHeight=-14;
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);

	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int width=pView->m_midLen;
	CString bck="";
	if(m_axisType==1)
		bck="%";
	if(flag==0)
	{
		for(i=0;i<num_all;i++)
		{
			CString str;
			int y;
			if((pow(valuePer,num_all-i-1))!=0&&pView->m_pkline[m_footBegin].open!=0)
			{
				if(m_axisType==1)
					y=YTransfer((float)(valuePer*i*pView->m_pkline[m_footBegin].open/100+fBegin));
				else
					y=YTransfer((float)(m_max_sun[m_nSon]/(pow(valuePer,num_all-i-1))));

				if(y<m_rectDrawLine.bottom-5)
				{
						if(m_axisType==1)
							str.Format("%.0f",(float)(valuePer*i-100+fBegin*100/pView->m_pkline[m_footBegin].open));
						else
							str.Format("%.2f",(float)(m_max_sun[m_nSon]/(pow(valuePer,num_all-i-1))));
						str+=bck;
						pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);

				}
			}
		}//end for
	}
	else if(flag==1)
	{
		for(i=0;i<num_all;i++)
		{
			CString str;
			int y;
			if((pow(valuePer,num_all-i-1))!=0)
			{
				if(m_axisType==1)
					y=YTransfer((float)(valuePer*i*pView->m_pkline[m_footBegin].open/100+fBegin));
				else
					y=YTransfer((float)(m_max_sun[m_nSon]/(pow(valuePer,num_all-i-1))));
			}

			if(y<m_rectDrawLine.bottom-5)
			{
				if(pView->m_pkline[m_footBegin].open!=0&&(pow(valuePer,num_all-i-1))!=0)
				{
					if(m_axisType==1)
						str.Format("%d",(int)(valuePer*i-100+fBegin*100/pView->m_pkline[m_footBegin].open));
					else
						str.Format("%d",(int)(m_max_sun[m_nSon]/(pow(valuePer,num_all-i-1))));
					str+=bck;
					pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);
				}
			}
		}	
	}


	if(m_axisType==1)
	{
		pDC->SelectObject(pOld);
		pDC->SelectObject(pOldpen);
		return 1;
	}

	//---		画倍数
	CString str;
//	if(flag==0)
	str.Format("%.2f",valuePer);
//	else
//		str.Format("%d",(int)valuePer);

	int lf=m_rectDrawLine.right+1;
	int rg=m_rectDrawLine.right+pView->m_midLen-1;
	int tp;
	if(wid==1)
		tp=m_rectDrawLine.bottom-32;
	else
		tp=m_rectDrawLine.bottom- 2*pView->m_heightCaption-4;
	int bm=m_rectDrawLine.bottom-4;

	CRect rect1(lf,tp,rg,bm);
	pDC->FillSolidRect (rect1,pDoc->m_colorArray[0]);

	pDC->SetTextAlign( TA_LEFT );//
	pDC->TextOut(lf+8,tp,"格值");
//	pDC->SetTextAlign( TA_RIGHT );//
	str+="倍";
	pDC->TextOut(lf+2,tp+pView->m_heightCaption,str);
	
	pDC->SelectObject(&pen_solid);
	pDC->MoveTo(lf,tp);
	pDC->LineTo(rg,tp);
	pDC->LineTo(rg,bm);
	pDC->LineTo(lf,bm);
	pDC->LineTo(lf,tp);

	pDC->SelectObject(pOld);
	pDC->SelectObject(pOldpen);
	
	return 1;

}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawRulorX(CDC *pDC)
//      功能说明：显示竖分隔线 
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DrawRulorX(CDC *pDC)
{
	if(pView->m_nKlineKind2 ==8 || pView->m_nCountKline <m_footEnd+1)
		return ;
	if(!ValidDoKline()) return;
	CTime tmB(1970,1,1,9,30,0);
	CTime tmNow;
	tmNow=tmB+pView->m_pkline[m_footBegin].day;
	CTime tmEnd;
	tmEnd=tmB+pView->m_pkline[m_footEnd].day;
	CTimeSpan tmAll;
	tmAll=tmEnd-tmNow;

	int nDay=tmAll.GetDays() ;//m_minGridX
	if(nDay>356*30)
		return;

	int flag=0;//以日为单位，还是以周为单位，还是以月为单位，还是以年为单位

	float nNum=(float)(pView->m_rtKlineFiguer .rightX/(float)pView->m_minGridX);
	if(nNum==0)
		return;

	int nPer=(int)(nDay/(float)(nNum)+1);
	if(nPer<=0)
		return;

	if(nPer<=2)
		flag=0;//以日为单位
	else if(nPer<=7)
		flag=1;//以周为单位
	else if(nPer<=30)
		flag=2;//以月为单位
	else if(nPer<=30*3)
		flag=3;//以季度为单位
	else //if(nPer<=356)
		flag=4;//以年为单位

	if(pView->m_nKlineKind2 == HS_KLINE)
		flag = 5;
	if(pView->m_nKlineKind2 == MIN1_KLINE)//MANY_DAY_KLINE
		flag = 6;
	if(pView->m_nKlineKind2 == MANY_DAY_KLINE && pDoc->m_propertyInitiate.daysOfManyKline <0)
		flag = 5;


	CPen pen_solid(PS_SOLID ,1,pDoc->m_colorArray[2]);//PS_DASHDOTDOT   
	CPen* pOldpen=pDC->SelectObject(&pen_solid);
//	pDC->MoveTo(0,pView->m_heightCaption);
//	pDC->LineTo(pView->m_rtKlineFiguer . rightX,pView->m_heightCaption);
//	for(int k=0;k<pView->m_infoInit . nCountIndex-1;k++)
//	{
//		pDC->MoveTo(0,pView->m_heightCaption + pView->m_rtKlineFiguer . rtBlw[k].m_yBottom);
//		pDC->LineTo(pView->m_rtKlineFiguer . rightX,pView->m_heightCaption + pView->m_rtKlineFiguer . rtBlw[k].m_yBottom);
//	}

	pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
	pDC->SetTextColor( pDoc->m_colorArray[2]);
	pDC->SetBkMode(TRANSPARENT);
	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];
	//lgf.lfHeight=-12;
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);

	int len=0;
	int nDayOfWeek=1;
	CString s;
	float widPer=pView->m_rtKlineFiguer .rightX/(float)m_klinNumDefault;
	int top=pView->m_rtKlineFiguer.rtBlw[pView->m_infoInit.nCountIndex-1].m_yBottom;
	int i;
	int gap=2;
	CRect r(0,top,pView->m_rtKlineFiguer .rightX,top+pView->m_heightCaption);
	CRect rTop(0,0,pView->m_rtKlineFiguer .rightX,top);
	switch(flag)//pView->m_rtKlineFiguer .rightX
	{
	case 0:
		tmNow=tmB+pView->m_pkline[m_footBegin].day;
		s=tmNow.Format("%Y/%m/%d");
		pDC->TextOut ((int)(widPer/2)+gap,top,s);
		len=(pDC->GetOutputTextExtent(s)).cx+gap+gap;
		len+=(int)(widPer/2);
		for(i=m_footBegin+1;i<=m_footEnd;i++)
		{
			tmNow=tmB+pView->m_pkline[i-1].day;
			int nDay=tmNow.GetDay ();
			int nMonth=tmNow.GetMonth ();
			int nYear=tmNow.GetYear ();
			tmNow=tmB+pView->m_pkline[i].day;

			int foot=i-m_footBegin;

			if(nDay!=tmNow.GetDay ()||nMonth!=tmNow.GetMonth ()||nYear!=tmNow.GetYear ())
			{
				//---		画虚线
				if(pDoc->m_systemOption.showyline==TRUE)
				{
					CPoint p1,p2;
					p1.x=(int)(foot*widPer+widPer/2);
					p2.x=p1.x;
					p1.y=pView->m_heightCaption;
					p2.y=top;
					ShowVirtualLine(pDC,p1,p2);
				}
				//---		画文字
				if(nYear!=tmNow.GetYear ())
					s=tmNow.Format("%Y/%m/%d");
				else if(tmNow.GetMonth ()!=nMonth)
					s=tmNow.Format("%m/%d");
				else
					s=tmNow.Format("%d");
				if((int)(foot*widPer+widPer/2)>len)
				{
					pDC->MoveTo((int)(foot*widPer+widPer/2),top+pView->m_heightCaption);
					pDC->LineTo((int)(foot*widPer+widPer/2),top);

//					pDC->IntersectClipRect (r);
					pDC->ExtTextOut ((int)(foot*widPer+widPer/2)+gap,top,ETO_CLIPPED,r,s,NULL);
					len=(pDC->GetOutputTextExtent(s)).cx+gap;
					len+=(int)(foot*widPer+widPer/2);
//					pDC->IntersectClipRect (rTop);
				}

			}
		}
		break;
	case 1:
		tmNow=tmB+pView->m_pkline[m_footBegin].day;
		s=tmNow.Format("%Y/%m/%d");
		tmEnd=tmNow;//tmEnd为第一天
		nDayOfWeek=tmNow.GetDayOfWeek ()-1;

		pDC->TextOut ((int)(widPer/2)+gap,top,s);
		len=(pDC->GetOutputTextExtent(s)).cx+gap;
		len+=(int)(widPer/2);

		for(i=m_footBegin+1;i<=m_footEnd;i++)
		{
			int nWeek1,nWeek2;
			tmNow=tmB+pView->m_pkline[i-1].day;
			tmAll=tmNow-tmEnd;//tmAll为时间跨度
			nWeek1=(tmAll.GetDays ()+nDayOfWeek)/7;

			int nMonth=tmNow.GetMonth ();
			int nYear=tmNow.GetYear ();

			tmNow=tmB+pView->m_pkline[i].day;
			tmAll=tmNow-tmEnd;
			nWeek2=(tmAll.GetDays ()+nDayOfWeek)/7;

			int foot=i-m_footBegin;

			if(nWeek1!=nWeek2)
			{
				//---		画虚线
				if(pDoc->m_systemOption.showyline==TRUE)
				{
					CPoint p1,p2;
					p1.x=(int)(foot*widPer+widPer/2);
					p2.x=p1.x;
					p1.y=pView->m_heightCaption;
					p2.y=top;
					ShowVirtualLine(pDC,p1,p2);
				}
				//---		画文字
				if(nYear!=tmNow.GetYear ())
					s=tmNow.Format("%Y/%m/%d");
				else if(tmNow.GetMonth ()!=nMonth)
					s=tmNow.Format("%m/%d");
				else
					s=tmNow.Format("%d");
				if((int)(foot*widPer+widPer/2)>len)
				{
					pDC->MoveTo((int)(foot*widPer+widPer/2),top+pView->m_heightCaption);
					pDC->LineTo((int)(foot*widPer+widPer/2),top);

//					pDC->IntersectClipRect (r);
					pDC->ExtTextOut ((int)(foot*widPer+widPer/2)+gap,top,ETO_CLIPPED,r,s,NULL);
					len=(pDC->GetOutputTextExtent(s)).cx+gap;
					len+=(int)(foot*widPer+widPer/2);
//					pDC->IntersectClipRect (rTop);
				}

			}
		}
		break;
	case 2:
		tmNow=tmB+pView->m_pkline[m_footBegin].day;
		s=tmNow.Format("%Y/%m");
		pDC->TextOut ((int)(widPer/2)+gap,top,s);
		len=(pDC->GetOutputTextExtent(s)).cx+gap;
		len+=(int)(widPer/2);
		for(i=m_footBegin+1;i<=m_footEnd;i++)
		{
			tmNow=tmB+pView->m_pkline[i-1].day;
			int nMonth=tmNow.GetMonth ();
			int nYear=tmNow.GetYear ();
			tmNow=tmB+pView->m_pkline[i].day;

			int foot=i-m_footBegin;

			if(nMonth!=tmNow.GetMonth ()||nYear!=tmNow.GetYear ())
			{
				//---		画虚线
				if(pDoc->m_systemOption.showyline==TRUE)
				{
					CPoint p1,p2;
					p1.x=(int)(foot*widPer+widPer/2);
					p2.x=p1.x;
					p1.y=pView->m_heightCaption;
					p2.y=top;
					ShowVirtualLine(pDC,p1,p2);
				}
				//---		画文字
				if(nYear!=tmNow.GetYear ())
					s=tmNow.Format("%Y/%m");
				else
					s=tmNow.Format("%m");
				if((int)(foot*widPer+widPer/2)>len)
				{
					pDC->MoveTo((int)(foot*widPer+widPer/2),top+pView->m_heightCaption);
					pDC->LineTo((int)(foot*widPer+widPer/2),top);

//					pDC->IntersectClipRect (r);
					pDC->ExtTextOut ((int)(foot*widPer+widPer/2)+gap,top,ETO_CLIPPED,r,s,NULL);
					len=(pDC->GetOutputTextExtent(s)).cx+gap;
					len+=(int)(foot*widPer+widPer/2);
//					pDC->IntersectClipRect (rTop);
				}

			}
		}
		break;
	case 3:
		tmNow=tmB+pView->m_pkline[m_footBegin].day;
		s=tmNow.Format("%Y/%m");
		pDC->TextOut ((int)(widPer/2)+gap,top,s);
		len=(pDC->GetOutputTextExtent(s)).cx+gap;
		len+=(int)(widPer/2);
		for(i=m_footBegin+1;i<=m_footEnd;i++)
		{
			int nSeason1,nSeason2;
			tmNow=tmB+pView->m_pkline[i-1].day;
			int nMonth=tmNow.GetMonth ();
			nSeason1=(nMonth-1)/3;
			int nYear=tmNow.GetYear ();
			tmNow=tmB+pView->m_pkline[i].day;
			nSeason2=(tmNow.GetMonth ()-1)/3;

			int foot=i-m_footBegin;

			if(nSeason1!=nSeason2)
			{
				//---		画虚线
				if(pDoc->m_systemOption.showyline==TRUE)
				{
					CPoint p1,p2;
					p1.x=(int)(foot*widPer+widPer/2);
					p2.x=p1.x;
					p1.y=pView->m_heightCaption;
					p2.y=top;
					ShowVirtualLine(pDC,p1,p2);
				}
				//---		画文字
				if(nYear!=tmNow.GetYear ())
					s=tmNow.Format("%Y/%m");
				else
					s=tmNow.Format("%m");
				if((int)(foot*widPer+widPer/2)>len)
				{
					pDC->MoveTo((int)(foot*widPer+widPer/2),top+pView->m_heightCaption);
					pDC->LineTo((int)(foot*widPer+widPer/2),top);

//					pDC->IntersectClipRect (r);
					pDC->ExtTextOut ((int)(foot*widPer+widPer/2)+gap,top,ETO_CLIPPED,r,s,NULL);
					len=(pDC->GetOutputTextExtent(s)).cx+gap;
					len+=(int)(foot*widPer+widPer/2);
//					pDC->IntersectClipRect (rTop);
				}

			}
		}
		break;
	case 4:
		tmNow=tmB+pView->m_pkline[m_footBegin].day;
		s=tmNow.Format("%Y");
		pDC->TextOut ((int)(widPer/2)+gap,top,s);
		len=(pDC->GetOutputTextExtent(s)).cx+gap;
		len+=(int)(widPer/2);
		for(i=m_footBegin+1;i<=m_footEnd;i++)
		{
			tmNow=tmB+pView->m_pkline[i-1].day;
			int nDay=tmNow.GetDay ();
			int nMonth=tmNow.GetMonth ();
			int nYear=tmNow.GetYear ();
			tmNow=tmB+pView->m_pkline[i].day;

			int foot=i-m_footBegin;

			if(nYear!=tmNow.GetYear ())//bug
			{
				//---		画虚线
				if(pDoc->m_systemOption.showyline==TRUE)
				{
					CPoint p1,p2;
					p1.x=(int)(foot*widPer+widPer/2);
					p2.x=p1.x;
					p1.y=pView->m_heightCaption;
					p2.y=top;
					ShowVirtualLine(pDC,p1,p2);
				}
				//---		画文字
				s=tmNow.Format("%Y");
				if((int)(foot*widPer+widPer/2)>len)
				{
					pDC->MoveTo((int)(foot*widPer+widPer/2),top+pView->m_heightCaption);
					pDC->LineTo((int)(foot*widPer+widPer/2),top);

//					pDC->IntersectClipRect (r);
					pDC->ExtTextOut ((int)(foot*widPer+widPer/2)+gap,top,ETO_CLIPPED,r,s,NULL);
					len=(pDC->GetOutputTextExtent(s)).cx+gap;
					len+=(int)(foot*widPer+widPer/2);
//					pDC->IntersectClipRect (rTop);
				}

			}
		}
		break;
	case 5:
	case 6:
		
		tmNow=pView->m_pkline[m_footBegin].day;
		s=tmNow.Format ("%m/%d-%H:%M");
		pDC->TextOut ((int)(widPer/2)+gap,top,s);
		len=(pDC->GetOutputTextExtent(s)).cx+gap+gap;
		len+=(int)(widPer/2);
		nDayOfWeek = m_footBegin;
		for(i=m_footBegin+1;i<=m_footEnd;i++)
		{
			tmNow=pView->m_pkline[i-1].day;
			int nHour=tmNow.GetHour  ();
			tmNow=pView->m_pkline[i].day;
			int nMinu=tmNow.GetMinute  ();

			int foot=i-m_footBegin;

			if(nHour!=tmNow.GetHour ()||nMinu==30 && (abs(i-nDayOfWeek)>20))
			{
				nDayOfWeek = i;//nDayOfWeek record "i" of pre
				//---		画虚线
				if(pDoc->m_systemOption.showyline==TRUE)
				{
					CPoint p1,p2;
					p1.x=(int)(foot*widPer+widPer/2);
					p2.x=p1.x;
					p1.y=pView->m_heightCaption;
					p2.y=top;
					ShowVirtualLine(pDC,p1,p2);
				}
				//---		画文字
				s=tmNow.Format ("%H:%M");
				if((int)(foot*widPer+widPer/2)>len)
				{
					pDC->MoveTo((int)(foot*widPer+widPer/2),top+pView->m_heightCaption);
					pDC->LineTo((int)(foot*widPer+widPer/2),top);

//					pDC->IntersectClipRect (r);
					pDC->ExtTextOut ((int)(foot*widPer+widPer/2)+gap,top,ETO_CLIPPED,r,s,NULL);
					len=(pDC->GetOutputTextExtent(s)).cx+gap;
					len+=(int)(foot*widPer+widPer/2);
//					pDC->IntersectClipRect (rTop);
				}

			}
		}
		break;
	}
	pDC->SelectObject(pOld);
	pDC->SelectObject(pOldpen);
}

//void CTaiKlineShowKline::ShowLittleRulor(CDC *pDC)
////////
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：ShowLittleRulor(CDC *pDC,int wid)
//      功能说明：显示小标尺
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::ShowLittleRulor(CDC *pDC,int wid)
{
	CRect rt(m_rectDrawLine.right,m_rectDrawLine.top,
		m_rectDrawLine.right+pView->m_midLen,m_rectDrawLine.bottom);
    /////////////////////////
	if(m_rectDrawLine.bottom-m_rectDrawLine.top<2)
		return;
	//------根据最大值及最小值来确定显示的数字的位数
	float max;
	max=(float)(FABSMY(m_max_sun[m_nSon])>FABSMY(m_min_sun[m_nSon])?FABSMY(m_max_sun[m_nSon]):FABSMY(m_min_sun[m_nSon]));
	m_step[m_nSon]=0;
	int flag;	//flag=0为浮点数，1为整数，2为大于等于100000的整数
	if(max<100)
		flag=0;
	else if(max<100000)
		flag=1;
	else
	{
		flag=2;
		float temp=max/100000;
		for(int i=0;;i++)
		{
			if(temp<pow((double)10,(double)i))
			{
				m_step[m_nSon]=(int)pow((double)10,(double)i);
				break;
			}
			if(i>5000)
				break;
		}
	}
	//------确定画几条线
	int numLine=(m_rectDrawLine.bottom-m_rectDrawLine.top)/pView->m_minGrid+1;
	if(numLine%2==1)
		numLine++;
	//------确定每格的数值
	float valuePer;
	valuePer=(m_max_sun[m_nSon]-m_min_sun[m_nSon])/numLine;

	if(valuePer<0.01)
	{
		numLine=(int)((m_max_sun[m_nSon]-m_min_sun[m_nSon])*100);
		valuePer=(float)0.01;
	}

	CPen pen_solid(PS_SOLID ,1,pDoc->m_colorArray[2]);//PS_DASHDOTDOT   
	CPen* pOldpen=pDC->SelectObject(&pen_solid);

	int m=2;

	//---		画线
	int i;
	for(i=0;i<numLine;i++)
	{
		int y;
		y=YTransfer((float)(valuePer*(i+1)+m_min_sun[m_nSon]));

		if(y<m_rectDrawLine.bottom-3 && y>=m_rectDrawLine.top)
		{
			//---		画虚线
			CPoint p1,p2;
			p1.x=m_rectDrawLine.left;
			p2.x=m_rectDrawLine.right+1;
			p1.y=y;
			p2.y=y;
			ShowVirtualLine(pDC,p1,p2);

			pDC->MoveTo(m_rectDrawLine.right+1,y);
			pDC->LineTo(m_rectDrawLine.right+6,y);
		}//end if
	}

	//---		画文字
	pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
	pDC->SetTextColor( pDoc->m_colorArray[2]);
	pDC->SetBkMode(TRANSPARENT);

	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];
	//lgf.lfHeight=-12;
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int width=pView->m_midLen;
//wid=0为缺省，wid=1为小图，wid=2为分时画面价格的两边文本，wid=3为分时画面其它的两边文本，wid=4为百分比坐标

	switch(flag)
	{
	case 0:
		for(i=0;i<numLine;i++)
		{
			CString str;
			int y=YTransfer((float)(valuePer*(i+1)+m_min_sun[m_nSon]));

			if(y<m_rectDrawLine.bottom-5)
			{
				if(wid!=2)
				{
					str.Format("%.2f",(float)(valuePer*(i+1)+m_min_sun[m_nSon]));
					pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);
				}
				//---	如果是分时走势
				else if(wid==2)
				{
					float cls=pView->pMin1Drawing->m_close;
					float nw=((valuePer*(i+1)+m_min_sun[m_nSon]));
					str.Format("%.2f",nw);
					if(nw>cls)
						pDC->SetTextColor( pDoc->m_colorArray[13]);
					else if(nw<cls)
						pDC->SetTextColor( pDoc->m_colorArray[15]);
					else
						pDC->SetTextColor( pDoc->m_colorArray[PING_PAN_WORD]);
					pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);
				}
			}
		}//end for
		break;
	case 1:
		for(i=0;i<numLine;i++)
		{
			CString str;
			int y=YTransfer((float)(valuePer*(i+1)+m_min_sun[m_nSon]));

			if(y<m_rectDrawLine.bottom-5)
			{
				if(wid!=2&&!(wid==1&&pView->pMin1Drawing->m_nSon==0))
				{
					str.Format("%d",(int)(valuePer*(i+1)+m_min_sun[m_nSon]));
					pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);
					if(wid==3)
							pDC->TextOut(m_rectDrawLine.left-m,y-tm.tmHeight/2,str);
				}
				//---	如果是分时走势
				else if(wid==2)
				{
					float cls=pView->pMin1Drawing->m_close;
					float nw=(valuePer*(i+1)+m_min_sun[m_nSon]);
					str.Format("%d",(int)nw);
					if(nw>cls)
						pDC->SetTextColor( pDoc->m_colorArray[13]);
					else if(nw<cls)
						pDC->SetTextColor( pDoc->m_colorArray[15]);
					else
						pDC->SetTextColor( pDoc->m_colorArray[PING_PAN_WORD]);
					pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);
				}
			}
		}
		break;
	case 2:
		CString str;
		if(m_step[m_nSon]==0)
		{
			pDC->SelectObject(pOld);
			pDC->SelectObject(pOldpen);
			return;
		}
		for(i=0;i<numLine;i++)
		{
			int y=YTransfer((float)((valuePer*(i+1)+m_min_sun[m_nSon])));

			if(y<m_rectDrawLine.bottom-5)
			{
				str.Format("%d",(int)((float)(valuePer*(i+1)+m_min_sun[m_nSon])/m_step[m_nSon]));
				pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);
			}
		}
		//---		画倍数
		str.Format("%d",m_step[m_nSon]);
		int width=tm.tmAveCharWidth*str.GetLength ()+8; 

		CBrush brush_bk(RGB(255,255,255));
		CBrush* pOldBrush=pDC->SelectObject(&brush_bk);

		int lf=m_rectDrawLine.right+2;
		int rg=m_rectDrawLine.right+2+width;
		int tp=m_rectDrawLine.bottom-18;
		int bm=m_rectDrawLine.bottom-4;

		CRect rect1(lf,tp,rg,bm);
		pDC->FillSolidRect (rect1,pDoc->m_colorArray[0]);

//		str="×"+str;
		pDC->SetTextAlign( TA_LEFT );//
		pDC->TextOut(lf+8,tp,str);
		pDC->SetTextAlign( TA_RIGHT );//
		
		pDC->SelectObject(&pen_solid);
		pDC->MoveTo(lf,tp);
		pDC->LineTo(lf+width+2,tp);
		pDC->LineTo(lf+width+2,bm);
		pDC->LineTo(lf,bm);
		pDC->LineTo(lf,tp);

		pDC->MoveTo(m_rectDrawLine.right+4,m_rectDrawLine.bottom-16);
		pDC->LineTo(m_rectDrawLine.right+10,m_rectDrawLine.bottom-8);
		pDC->MoveTo(m_rectDrawLine.right+4,m_rectDrawLine.bottom-8);
		pDC->LineTo(m_rectDrawLine.right+10,m_rectDrawLine.bottom-16);
	}
	pDC->SelectObject(pOld);
	pDC->SelectObject(pOldpen);
	

}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：ShowMultiFiguer(CDC *pDC,int flag)
//      功能说明：显示多图
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::ShowMultiFiguer(CDC *pDC,int flag)
{
	if(pView->pMin1Drawing->m_pReportData==NULL)
		return;
	CRect r(m_rectDrawLine.left,m_rectDrawLine.top-pView->m_heightCaption,
		m_rectDrawLine.right+pView->m_midLen,m_rectDrawLine.top);

	//---		画边框
	CPen pen(PS_SOLID,1,pDoc->m_colorArray[2]);
	CPen* pOldpen=pDC->SelectObject(&pen);

	pDC->MoveTo (m_rectDrawLine.left,m_rectDrawLine.top);
	pDC->LineTo (m_rectDrawLine.right,m_rectDrawLine.top);
	pDC->LineTo (m_rectDrawLine.right,m_rectDrawLine.bottom);//--
	pDC->LineTo (m_rectDrawLine.left,m_rectDrawLine.bottom);//--
	pDC->LineTo (m_rectDrawLine.left,m_rectDrawLine.top);

	pDC->SelectObject(pOldpen);

	//画标题
	if(flag==0)
	{
		pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		pDC->SetTextColor( pDoc->m_colorArray[1]);
		pDC->SetBkMode(TRANSPARENT);
		CFont ft;
		LOGFONT lgf=pDoc->m_fontstr[0];
		//lgf.lfHeight=-12;
		ft.CreateFontIndirect (&lgf );
		CFont* pOld=pDC->SelectObject(&ft);

		CString s=pView->pMin1Drawing ->m_pReportData->name  ;
		pDC->ExtTextOut (r.left ,r.top,ETO_CLIPPED,r,s,NULL);
		pDC->SelectObject(pOld);
	}

	//画线
	pView->m_pkline=pView->m_pkline ;

	m_nSon=0;
	float max_sun=m_max_sun[m_nSon];
	float min_sun=m_min_sun[m_nSon];
	m_max_sun[m_nSon]=(float)-1.0E20;
	m_min_sun[m_nSon]=(float)1.0E20;
	int klinNumDefault=m_klinNumDefault;
	int footB=m_footBegin;
	int footE=m_footEnd;
	int axisType=m_axisType;
	m_axisType=0;
	m_klinNumDefault=60;
	if(pView->m_nCountKline>m_klinNumDefault)
	{
		m_footBegin=pView->m_nCountKline-m_klinNumDefault;
		m_footEnd=pView->m_nCountKline-1;
		m_footCurrent=m_footEnd;
	}
	else
	{
		m_footBegin=0;
		m_footEnd=pView->m_nCountKline-1;
		m_footCurrent=m_footEnd;
	}
	ValidDoKline(true);

	if(flag==0)
	{
		CaclMaxFlt(pView->m_pkline,0);
		ShowLittleRulor(pDC);
		DrawKLine(pDC);
	}
	else
	{
		delete[] m_dataFormular[m_nSon].line[0].m_arrBE.line;
		m_dataFormular[m_nSon].line[0].m_arrBE.kind = ARRAY_BE::Normal;
		m_dataFormular[m_nSon].line[0].m_arrBE.line= new float[pView->m_nCountKline];
		for(int i=0;i<pView->m_nCountKline ;i++)
		{
			m_dataFormular[m_nSon].line[0].m_arrBE.line[i]=pView->m_pkline[i].vol;
		}
		int numln=m_dataFormular[m_nSon].numLine;
		m_dataFormular[m_nSon].numLine=1;
		m_dataFormular[m_nSon].line[0].bNow=m_footBegin;
		CaclMaxFlt(m_dataFormular[m_nSon].line[0].m_arrBE.line,
			m_footBegin,0);
		//add
		m_min_sun[m_nSon]=0;
		ShowLittleRulor(pDC);

		CString sName=m_lineName[m_nSon][0];
		m_lineName[m_nSon][0]="成交量";
		DrawLineIndex(pDC);
		m_lineName[m_nSon][0],sName;

		m_dataFormular[m_nSon].numLine=numln;
	}

	m_max_sun[m_nSon]=max_sun;
	m_min_sun[m_nSon]=min_sun;
	m_klinNumDefault=klinNumDefault;
	m_footBegin=footB;
	m_footEnd=footE;
	m_axisType=axisType;
	ValidDoKline(true);
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：ShowMultiPage(CDC *pDC)
//      功能说明：显示多图
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::ShowMultiPage(CDC *pDC)
{
	if(!pDC->IsPrinting())
		pDC->SelectClipRgn (NULL);//
	CRect r;
	r.top=0;
	r.left=0;
	int nm=pView->m_infoInit.nCountIndex;
	if(pView->m_nKlineKind2 >0)
	{
		if(pView->m_infoFiguer >0)
			r.right=pView->m_rtKlineFiguer.rightX+ pView->m_midLen + pView->m_rightLength ;
		else
			r.right=pView->m_rtKlineFiguer.rightX+ pView->m_midLen ;
		r.bottom=pView->m_rtKlineFiguer.rtBlw[nm-1].m_yBottom+pView->m_heightCaption;
	}
	else
	{
		nm=pView->m_infoInit.nCountMin1;
		if(pView->m_infoFiguer >0)
			r.right=pView->m_rtMin1.rightX+ pView->m_midLen + pView->m_rightLength ;
		else
			r.right=pView->m_rtMin1.rightX+ pView->m_midLen ;
		r.bottom=pView->m_rtMin1.rtBlw[nm-1].m_yBottom+pView->m_heightCaption;
	}
//	SYSTEM_OPTION  m_systemOption;        //系统设置中的选择项
//	SYSTEMDATA    m_systemdata;          //系统设置中的数据项

	int nFiguer;
	if(pDoc->m_systemOption.fourgraph==FALSE)//kline
		nFiguer=4;
	else
		nFiguer=9;

	float n;//=(float)pow(nFiguer,0.5);
	if(nFiguer==4)
		n=2;
	else
		n=3;

	float width=(r.right -r.left )/(float)n;
	float height=(r.bottom -r.top )/(float)n;
	pView->ClearBack (pDC,r);

	CString s5=pView->m_sharesSymbol ;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			//--		从文件读出数据
			pView->ReadDataMultiFiguer((int)(i*n+j));

			m_rectDrawLine.left=(int)(j*width+1);
			m_rectDrawLine.right=(int)((j+1)*width-pView->m_midLen);
			m_rectDrawLine.top=(int)(i*height+pView->m_heightCaption);
			m_rectDrawLine.bottom=(int)(m_rectDrawLine.top+(height-pView->m_heightCaption)*0.72);

			if(pView->m_nKlineKind2 ==0)
			{
				pView->pMin1Drawing ->ShowMultiFiguer(pDC);
			}
			else
				ShowMultiFiguer(pDC);
			m_rectDrawLine.top=m_rectDrawLine.bottom;
			m_rectDrawLine.bottom=(int)((i+1)*height-1);
			if(pView->m_nKlineKind2 ==0)
			{
				pView->pMin1Drawing ->ShowMultiFiguer(pDC,1);
			}
			else 
				ShowMultiFiguer(pDC,1);
		}
	}
	pView->m_sharesSymbol=s5 ;
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：ShowCNP(CDC *pDC,int nFlags)
//      功能说明：显示成交量累积
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::ShowCNP(CDC *pDC,int nFlags)
{
//	if(pView->m_sharesSymbol[1]=='B' || pView->m_sharesSymbol[1]=='C')
//		return;
	int end;
	int begin;//=m_footChenBenB;
	if(nFlags==1)//after
	{
		begin=m_footCurrent;//tmp
		if(m_footCurrent>m_footEnd||m_footCurrent<=m_footBegin)
			begin=m_footBegin;
		end=m_footEnd;
	}
	else if(nFlags==2)//before
	{
		end=m_footCurrent;//tmp
		if(m_footCurrent>=m_footEnd||m_footCurrent<m_footBegin)
			end=m_footEnd;
//		end=m_footEnd;
		begin=m_footBegin;
	}
	else
	{
		end=m_footEnd;
		begin=m_footBegin;
	}


	m_nSon=0;
	SetRectCurrent(m_nSon);

	//--- 计算成交量累积
	int n=m_rectDrawLine.bottom-m_rectDrawLine.top+1;
	if(n<=0)
		return;

	float* cnp=new  float[n];
	memset(cnp,0,n*4);

	float max=0;
	int nVol=0;
	int i;
	for(i=begin;i<=end;i++)
	{
		int foot=YTransfer(pView->CaclMid(i))-pView->m_heightCaption;
		if(foot>=n || foot <0)
			continue;

		cnp[foot]+=FABSMY(pView->m_pkline[i].vol);
//		if(pView->m_pkline[i].vol>0)
			nVol++;
		if(cnp[foot]>max)
			max=cnp[foot];
	}
//	if(nVol<end-begin+1)
//		return;

	//---	画线
	CPen pen_line(PS_SOLID  ,1,pDoc->m_colorArray[2]);//PS_DASHDOTDOT   
	CPen* pOldpen=pDC->SelectObject(&pen_line);

	if(max>0)
	{
		for(i=0;i<n;i++)
		{
			pDC->MoveTo(m_rectDrawLine.right-(int)(cnp[i]*m_rectDrawLine.right/2/max),pView->m_heightCaption+i);
			pDC->LineTo(m_rectDrawLine.right,pView->m_heightCaption+i);
		}
	}

	delete[] cnp;

	pDC->SelectObject(pOldpen);
}

float CTaiKlineShowKline::ShowChengBen(CDC *pDC,int nFlags)
{
	int end;
	int begin;//=m_footChenBenB;
	if(nFlags==1)//after
	{
		begin=m_footCurrent;//tmp
		if(m_footCurrent>m_footEnd||m_footCurrent<=m_footBegin)
			begin=m_footBegin;
		end=m_footEnd;
	}
	else if(nFlags==2)//before
	{
		end=m_footCurrent;//tmp
		if(m_footCurrent>=m_footEnd||m_footCurrent<m_footBegin)
			end=m_footEnd;
//		end=m_footEnd;
		begin=m_footBegin;
	}
	else
	{
		end=m_footEnd;
		begin=m_footBegin;
	}

	m_nSon=0;
	SetRectCurrent(m_nSon);

	//--- 计算成本累积
	int n=m_rectDrawLine.bottom - m_rectDrawLine.top+1;
	if(n<=0)
		return 0;
	float* cnp=new  float[n];
	memset(cnp,0,n*4);

	float max=0;
	float volume=0;
	float amount=0;
	int nVol=0;
	int i;
	for(i=begin;i<=end;i++)
	{
		int foot;
		if(pView->m_pkline[i].vol>0)
		{
			//a bug
			foot=YTransfer((float)pView->CaclMid (i))-m_rectDrawLine.top;
			if(foot<0||foot>=n)
				break;
			cnp[foot]+=pView->m_pkline[i].amount;
			if(cnp[foot]>max)
				max=cnp[foot];
			volume+=pView->m_pkline[i].vol;

			amount+=(float)pView->CaclMid (i)*pView->m_pkline[i].vol;

			if(pView->m_pkline[i].vol>0)
				nVol++;
		}
	}
//	if(nVol<end-begin+1)
//		return 0;

	if(volume>0)
		amount=amount/volume;//平均成本统计
	else
	{
		delete[] cnp;
		return 0;
	}
	int yChengBen=YTransfer(amount);

	//---	画线
	CPen pen_line(PS_SOLID  ,1,pDoc->m_colorArray[2]);//PS_DASHDOTDOT   
	CPen* pOldpen=pDC->SelectObject(&pen_line);
	if(max>0)
	{
		for(i=0;i<n;i++)
		{
			pDC->MoveTo(m_rectDrawLine.right-(int)(cnp[i]*m_rectDrawLine.right/2/max),pView->m_heightCaption+i);
			pDC->LineTo(m_rectDrawLine.right,pView->m_heightCaption+i);
		}
	}

	if((m_footEnd-m_footBegin+1)==0)
	{
		delete[] cnp;
		pDC->SelectObject(pOldpen);
		return 0;
	}
	float widPer=(float)m_rectDrawLine.right/(this->m_klinNumDefault);

	//after dui ji
	CPen pen_line2(PS_SOLID  ,1,pDoc->m_colorArray[1]);//PS_DASHDOTDOT   

	pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
	pDC->SetTextColor( pDoc->m_colorArray[1]);
	pDC->SetBkMode(TRANSPARENT);

	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];
	//lgf.lfHeight=-12;
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);

	pDC->SelectObject(&pen_line2);

	if(nFlags !=2)
	{
		pDC->MoveTo(m_rectDrawLine.right,yChengBen);
		pDC->LineTo((int)(widPer*(begin-m_footBegin+0.5)),yChengBen);

		CString s;
		s.Format ("%.3f",amount);
		int len=(pDC->GetOutputTextExtent(s)).cy;
		CRect r(0,pView->m_heightCaption ,pView->m_rtKlineFiguer .rightX ,pView->m_rtKlineFiguer .rtBlw[0].m_yBottom );
		int nBeginPos = (int)(widPer*(begin-m_footBegin+0.5));
		int nMaxRight = pView->m_rtKlineFiguer .rightX-(pDC->GetOutputTextExtent(s)).cx;
		if(nBeginPos<10)
			nBeginPos = 10;
		if(nBeginPos>nMaxRight)
			nBeginPos = nMaxRight;
		pDC->ExtTextOut (nBeginPos,yChengBen-len-2,ETO_CLIPPED,r,s,NULL);
//		pDC->TextOut((int)(widPer*(begin-m_footBegin+0.5)),yChengBen-len-2,s);
	}
	else
	{
		pDC->MoveTo(0,yChengBen);
		pDC->LineTo((int)(widPer*(end-m_footBegin+0.5)),yChengBen);

		CString s;
		s.Format ("%.3f",amount);
		pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
		int len=(pDC->GetOutputTextExtent(s)).cy;
		CRect r(0,pView->m_heightCaption ,pView->m_rtKlineFiguer .rightX ,pView->m_rtKlineFiguer .rtBlw[0].m_yBottom );

		int nBeginPos = (int)(widPer*(end-m_footBegin+0.5));
		int nMaxRight =(pDC->GetOutputTextExtent(s)).cx;
		if(nBeginPos<2+nMaxRight)
			nBeginPos = 2+nMaxRight;
		pDC->ExtTextOut (nBeginPos,yChengBen-len-2,ETO_CLIPPED,r,s,NULL);
//		pDC->TextOut((int)(widPer*(end-m_footBegin+0.5)),yChengBen-len-2,s);
	}

	pDC->SelectObject(pOldpen);
	pDC->SelectObject(pOld);

	delete[] cnp;
	return amount;
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：TongJi(CTaiKlineDialogShiDuanTJ* pTJ,int nFlags)
//      功能说明：统计数据
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
float CTaiKlineShowKline::TongJi(CTaiKlineDialogShiDuanTJ* pTJ,int nFlags)//统计数据
{
	if (pView->m_nKlineKind2==0)		//分时界面
		return pView->pMin1Drawing->TongJi(pTJ,nFlags);

	float close=0;
	float high=0;
	float low=(float)10E20;
	float open=0;

	float vol=0;
	float amount=0;
	float zhDie=0;
	float zhDieFu=0;
	float junJia_jiaQuan=0;

	int begin,end;

	if(nFlags==0)
	{
		if(pView->m_rectHitPos.left<pView->m_rectHitPos.right)
		{
			begin=pView->m_rectHitPos.left*m_klinNumDefault /pView->m_rtKlineFiguer.rightX;
			end=pView->m_rectHitPos.right*m_klinNumDefault /pView->m_rtKlineFiguer.rightX;
		}
		else
		{
			begin=pView->m_rectHitPos.right*m_klinNumDefault /pView->m_rtKlineFiguer.rightX;
			end=pView->m_rectHitPos.left*m_klinNumDefault /pView->m_rtKlineFiguer.rightX;
		}

		if(begin>=end)
			return -1;
		begin=m_footBegin+begin;
		if(begin>m_footEnd)
			return -1;
		end=m_footBegin+end;
		if(end>m_footEnd)
			end=m_footEnd;
	}
	else
	{
		CTime tmB(1970,1,1,9,30,0);
		CTimeSpan tmAll=pTJ->m_tmB-tmB;
		int rtn;
		if(tmAll.GetTotalSeconds ()>pView->m_pkline[pView->m_nCountKline-1].day)
			rtn=pView->m_nCountKline-1;
		else
			rtn=pView->FindTimeK(tmAll.GetTotalSeconds ());
		if(rtn<0)
			return -1;
		begin=rtn;

		tmAll=pTJ->m_tmE-tmB;
		if(tmAll.GetTotalSeconds ()>pView->m_pkline[pView->m_nCountKline-1].day)
			rtn=pView->m_nCountKline-1;
		else
			rtn=pView->FindTimeK(tmAll.GetTotalSeconds ());
		if(rtn<0)
			return -1;
		end=rtn;
		if(begin>end)
		{
			int tmp;
			tmp=begin;
			begin=end;
			end=tmp;
		}
	}

	float junJia=0;
	for(int i=begin;i<=end;i++)
	{
		vol+=pView->m_pkline[i].vol;
		amount+=pView->m_pkline[i].amount;
		if(pView->m_pkline[i].high>high)
			high=pView->m_pkline[i].high;
		if(pView->m_pkline[i].low<low)
			low=pView->m_pkline[i].low;
		junJia+=pView->CaclMid (i);
	}
	if((end-begin+1)==0)
		junJia=junJia;
	else
		junJia=junJia/(end-begin+1);

	if(vol>0)
	{
			junJia_jiaQuan = 0;
			for(int i=begin;i<=end;i++)
			{
				junJia_jiaQuan+=pView->m_pkline[i].vol *(pView->CaclMid (i));
			}
			junJia_jiaQuan=junJia_jiaQuan/vol;
	}
	close=pView->m_pkline[end].close;
	open=pView->m_pkline[begin].open;
	if(begin>0)
	{
		zhDie=close-pView->m_pkline[begin-1].close;
		if(close>0)
			zhDieFu=zhDie/pView->m_pkline[begin-1].close*100;
	}
	else
	{
		zhDie=close-pView->m_pkline[0].open;
		if(close>0)
			zhDieFu=zhDie/pView->m_pkline[0].open*100;
	}


	CString s;
	s.Format("%.3f",open);
	pTJ->m_open=s;
	s.Format("%.3f",high);
	pTJ->m_high=s;
	s.Format("%.3f",low);
	pTJ->m_low=s;
	s.Format("%.3f",close);
	pTJ->m_close=s;
	s.Format("%.0f",vol);
	pTJ->m_vol=s;
	s.Format("%.2f",amount/10000);
	s=s+"万";
	pTJ->m_amount=s;

	s.Format("%.3f",zhDie);
	pTJ->m_zhDie=s;
	s.Format("%.2f",zhDieFu);
	s=s+"%";
	pTJ->m_zhDieFu=s;

	s.Format("%.3f",junJia);
	pTJ->m_junjia=s;
	s.Format("%.3f",junJia_jiaQuan);
	pTJ->m_chengBenTJ=s;

	s="";
	{
		float ltp = CTaiShanKlineShowView::GetCapital(pView->pMin1Drawing ->m_pReportData);
		if(ltp >0)
		{
			s.Format("%.3f",vol/ltp*100 );
			s+="%";
		}
	}
	pTJ->m_huanShou=s;

	//---	时间
	if(nFlags==0)
	{
		CTime tmB(1970,1,1,9,30,0);
		CTime tmNow=tmB+pView->m_pkline[begin].day;
		pTJ->m_tmB=tmNow;
		tmNow=tmB+pView->m_pkline[end].day;
		pTJ->m_tmE=tmNow;
	}

	s.Format("%d",end-begin+1);
	pTJ->m_num=s;

	m_nBeginFootTJ = begin;//时段统计的开始点
	m_nEndFootTJ = end;//时段统计的结束点

	//to invert rect or redraw
	if(begin>=0 && begin<pView->m_nCountKline 
		&& end>=0 && end<pView->m_nCountKline )
	{
		if(begin<this->m_footBegin || end>this->m_footEnd )
		{
			if(begin<this->m_footBegin )
				m_footBegin = begin;
			if(end>this->m_footEnd)
				m_footEnd=end;
			m_footCurrent=m_footEnd;
			m_klinNumDefault = m_footEnd - m_footBegin + 1;
		}

			CTaiKlineDC* pMemDC = new CTaiKlineDC(pView,&pView->m_bit);
			this->DrawKlineFiguer (pMemDC);
			delete pMemDC;

			pView->RedrawWindowNoCacl ();
			//pView->RedrawWindow ();
	}
	ValidDoKline(true);
	return 1;
}

int CTaiKlineShowKline::TransferKlineTypeData(CString symbol,int stkKind,Kline*& pKline,int nFlags,int nKln)
{
	int nRtn = 0;
	if(pView->m_nKlineKind2 ==HS_KLINE ||pView->m_nKlineKind2 ==MIN1_KLINE|| pView->m_nKlineKind2 == MANY_DAY_KLINE&& pDoc->m_propertyInitiate.daysOfManyKline <0)
	{
		//if(pDoc->m_bInitDone==TRUE && pDoc->m_bCloseWorkDone==false)
			TransferKlineTypeDataToday(pKline,symbol,stkKind,nFlags);
		return 0;
	}

	if(nKln==-1)
		nKln=pView->ReadKLine(symbol,stkKind,pKline);

	if(nFlags==-1)
	{
		pView->m_nCountKline=nKln;
		pView->m_nCountKlineToday=nKln;
		pView->m_nCountKlineFile=pView->m_nCountKline;
	}
	else
	{
		m_nKlineFileAdd[nFlags]=nKln;
		m_nKlineAdd[nFlags]=nKln;
	}

	//	计算除权
	if(pView->m_nKlineKind2>=1 &&pView->m_nKlineKind2<=8 ||pDoc->m_propertyInitiate.daysOfManyKline>0&&pView->m_nKlineKind2==MANY_DAY_KLINE)
		ComputePower(symbol,stkKind,pKline,nKln);

	if(pView->m_nKlineKind2 ==5||pView->m_nKlineKind2 ==1)
	{
		if(pDoc->m_bInitDone==TRUE && pDoc->m_bCloseWorkDone==false)
			TransferKlineTypeDataToday(pKline,symbol,stkKind,nFlags);
		return 0;
	}

	int nKline = nKln;
	m_pLmbTransferKline->m_bUseMinute1Data = true;
	if(pView->m_pDlgDealHistory !=NULL) m_pLmbTransferKline->m_bUseMinute1Data = false;

	m_pLmbTransferKline->TransferKlineHistory( symbol, stkKind,pKline,  nKline, pView->m_nKlineKind2,false);

	int j =nKline;
	if(nFlags==-1)
	{
		pView->m_nCountKline=j;
		pView->m_nCountKlineToday=j;
		pView->m_nCountKlineFile=j;
	}
	else
	{
		m_nKlineFileAdd[nFlags]=j;
		m_nKlineAdd[nFlags]=j;
	}

	return j;
}


/////////////////////////////////////////////////////////////////////////////
//      函 数 名：TransferKlineTypeDataToday(Kline* pKline,CString symbol,int nFlags)
//      功能说明：转换当天的K线数据类型
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::TransferKlineTypeDataToday(Kline*& pKline,CString symbol,int stkKind,int nFlags)
{
	if(	pView->m_bDoPower==false)
	{
		m_pLmbTransferKline->m_bCalPowerToday = false;
	}
	else
		m_pLmbTransferKline->m_bCalPowerToday = true;

	int nKln;
	if(nFlags==-1)
	{
		nKln=pView->m_nCountKlineFile;
	}
	else
	{
		nKln=m_nKlineFileAdd[nFlags];
	}

	int nKline = nKln;

	m_pLmbTransferKline->m_bUseMinute1Data = true;
	if(pView->m_pDlgDealHistory !=NULL) 
	{
		m_pLmbTransferKline->m_bUseMinute1Data = false;
	}

	//update , add by   20040212 ,need test
	if(pView->pMin1Drawing ->m_bHist == false 
		|| pView->m_nKlineKind2 == MIN1_KLINE
		|| pView->m_nKlineKind2 == HS_KLINE)
		m_pLmbTransferKline->TransferKlineToday ( symbol,stkKind,pKline,  nKline, pView->m_nKlineKind2);

	if(nFlags==-1)
	{
		pView->m_nCountKline=nKline;
		pView->m_nCountKlineToday=nKline;
	}
	else
	{
		m_nKlineAdd[nFlags]=nKline;
	}

	return 1;
}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：ComputePower()
//      功能说明：除权计算
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////

//need change 2004.2.25 to many stocks
void CTaiKlineShowKline::ComputePower(CString symbol,int stkKind,Kline* pKline,int nKln)
{
	if(	pView->m_bDoPower==false)
	{
		m_pLmbTransferKline->m_bCalPowerToday = false;
		return;
	}
	m_pLmbTransferKline->m_bCalPowerToday = true;

	static bool bFirst1 = true;
	if(bFirst1 == true)
	{
		try
		{
		powerArr splitArr;
		CTaiKlineTransferKline::ReadPower(pDoc,symbol,stkKind, splitArr);//m_PowerArray
		CTaiKlineTransferKline::ComputePower(pKline , nKln, pView->m_nKlineKind2 ,splitArr);// this-> );
		pView->m_bDoPower=true;
		}
		catch(...)
		{
			AfxMessageBox("计算除权时遇到错误，可能是跟其它程序有冲突。下一次将不在计算除权！");
			bFirst1 = false;
			pView->m_bDoPower=false;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：RestoreKLine()
//      功能说明：恢复K线数据
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::RestoreKLine()
{
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawBaseInfo(CDC *pDC)
//      功能说明：画重大事件
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DrawBaseInfo(CDC *pDC)
{
//	pView->m_bs.Requery();
	if(pView->m_nKlineKind2<5)
		return;
	if(!pView->m_bBaseInfo)
		return;
	try
	{
	if(!pView->m_bs.IsOpen())
		pView->m_bs.Open ();
	pView->m_bs.MoveFirst ();

	int bgn=pView->pKlineDrawing ->m_footBegin;
	int end=pView->pKlineDrawing ->m_footEnd;
	CTime tmB(1970,1,1,9,30,0);
	//---		画出
	CBitmap       m_bitmap;

	CImageList imagelist;
    imagelist.Create(15,15, ILC_MASK, 1, 2);
    
	m_bitmap.LoadBitmap(IDB_BITMAP_136);
	imagelist.Add(&m_bitmap, (COLORREF)0x00ffFFFF);
	m_bitmap.DeleteObject();

	while(!pView->m_bs.IsEOF() )
	{
		//--	计算时间
		int nYear=pView->m_bs.m_time .GetYear ();
		int nMonth=pView->m_bs.m_time .GetMonth  ();
		int nDay=pView->m_bs.m_time .GetDay ();
		CTime tmLib(nYear,nMonth,nDay,9,30,0);
		CTimeSpan tmAll=tmLib-tmB;
		int scds=tmAll.GetTotalSeconds ();
		if(scds>=pView->m_pkline[m_footBegin].day&&scds<=pView->m_pkline[m_footEnd].day)
		{
			int rtn =pView->FindTimeK (scds);
			if(rtn==-1)
				continue;
			POINT p;
			p.y=pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-18;

			if(m_klinNumDefault<=0)
				return ;
			p.x=(int)((rtn-m_footBegin+0.5)*pView->m_rtKlineFiguer.rightX/m_klinNumDefault-8);
			imagelist.Draw (pDC ,0,p,ILD_TRANSPARENT);
		}
		pView->m_bs.MoveNext ();
	}
	}
	catch (CDaoException* e)
	{
		e->Delete();
	}
}

void CTaiKlineShowKline::CaclPercentMaxMin(float &maxPct, float &minPct)
{

}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：AddKlineAdded(CString symbol)
//      功能说明：主图叠加
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::AddKlineAdded(CString symbol,int stkKind)
{
	TransferKlineTypeData(symbol,stkKind,m_pKlineAdd[m_nKlineCurrent],m_nKlineCurrent);//pView->ReadKLine (symbol,m_pKlineAdd[m_nKlineCurrent]);
	if(m_nKlineAdd[m_nKlineCurrent]>0)//m_nKlineAdd[m_nKlineCurrent]=
		m_nKlineCurrent++;
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DeleteKlineAdded()
//      功能说明：删除主图叠加
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DeleteKlineAdded()
{
	int j;
	for(j=0;j<m_nKlineCurrent;j++)
	{
		delete[] m_pKlineAdd[j];
	}
	for( j=0;j<m_nKlineCurrent;j++)
	{
		m_pKlineAdd[j]=new Kline[2];
	}
	m_nKlineCurrent=0;
	pView->RedrawWindow ();
}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：FindTimeK(int keyVlu,Kline* pKline,int max)
//      功能说明：查找某时间的K线
//      入口参数：
//      出口参数：无出口数
//      返回参数：返回数组下标
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::FindTimeK(int keyVlu,Kline* pKline,int max)
{
	bool bDay =false,bAdd = false;
	if( pView->m_nKlineKind2>=5 && pView->m_nKlineKind2<=8 ||pDoc->m_propertyInitiate.daysOfManyKline>0&& pView->m_nKlineKind2==MANY_DAY_KLINE)
		bDay = true;
	int rtn=CTaiKlineFileKLine::LookTwoPath((time_t&)keyVlu ,pKline,max, bAdd,bDay);
//	if(rtn==0&& bAdd == true)
//		rtn = -1;
	if(rtn==-1&& bAdd == true)
		rtn = max;
	return rtn;
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：CaclBeginFoot(int nKline)
//      功能说明：计算开始点
//      入口参数：
//      出口参数：无出口数
//      返回参数：返回起始的数组下标
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::CaclBeginFoot(int nKline)//返回起始的数组下标
{
	int rtn=FindTimeK(pView->m_pkline[m_footBegin].day,m_pKlineAdd[nKline],m_nKlineAdd[nKline]);
	if(rtn>=0&&rtn<1000000)
	{
		if(rtn<m_nKlineAdd[nKline])
			return rtn;
		else
			return -1;
	}
	if(rtn==-1)
	{
		rtn=FindTimeK(m_pKlineAdd[nKline][0].day,pView->m_pkline,pView->m_nCountKline);
		if(rtn<1000000&& rtn>-1)
			return 0;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：CaclMaxAdded()
//      功能说明：计算多图叠加的最大值
//      入口参数：
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::CaclMaxAdded()
{
	m_nSon=0;
	m_max_sun[m_nSon]=(float)-1.0E20;
	m_min_sun[m_nSon]=(float)1.0E20;
	int i;
	float maxPercent=0,minPercent=0;
	switch(m_axisType)
	{
	case 0:
	case 2:
		CaclMaxFlt(pView->m_pkline,0);
		for( i=0;i<m_nKlineCurrent;i++)
		{
			int b=CaclBeginFoot(i);
			if(b==-1)
				continue;
			int e=CaclEndFoot(i);
			if(e==-1)
				continue;
			CaclMaxMin(m_pKlineAdd[i],  b,  e);
		}
		break;
	case 1:
		CaclMaxFlt(pView->m_pkline,0);
		if(pView->m_pkline[m_footBegin].open>0)
		{
			maxPercent=100*(m_max_sun[0]-pView->m_pkline[m_footBegin].open)/pView->m_pkline[m_footBegin].open;
			minPercent=100*(m_min_sun[0]-pView->m_pkline[m_footBegin].open)/pView->m_pkline[m_footBegin].open;
		}
		else 
		{
//			AfxMessageBox("数据有误");
			break;
		}
		for( i=0;i<m_nKlineCurrent;i++)
		{
			int b=CaclBeginFoot(i);
			if(b==-1)
				continue;
			int e=CaclEndFoot(i);
			if(e==-1)
				continue;
			m_max_sun[m_nSon]=(float)-1.0E20;
			m_min_sun[m_nSon]=(float)1.0E20;
			CaclMaxMin(m_pKlineAdd[i],  b,  e);

			if(m_pKlineAdd[i][b].open>0)
			{
				float max=100*(m_max_sun[0]-m_pKlineAdd[i][b].open)/m_pKlineAdd[i][b].open;
				float min=100*(m_min_sun[0]-m_pKlineAdd[i][b].open)/m_pKlineAdd[i][b].open;
				if(max>maxPercent)
					maxPercent=max;
				if(min<minPercent)
					minPercent=min;
			}

			//--	计算最大值
			m_max_sun[0]=maxPercent*pView->m_pkline[m_footBegin].open/100+pView->m_pkline[m_footBegin].open;
			m_min_sun[0]=minPercent*pView->m_pkline[m_footBegin].open/100+pView->m_pkline[m_footBegin].open;
//TRACE("maxPercent = %f,   min  = %f\n",maxPercent,minPercent);
//TRACE("m_max_sun = %f,   min  = %f\n",m_max_sun[0],m_min_sun[0]);
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：CaclEndFoot(int nKline)
//      功能说明：计算结束点的下标
//      入口参数：
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::CaclEndFoot(int nKline)
{
//	int rtn=CaclBeginFoot(nKline);
	int rtn=FindTimeK(pView->m_pkline[m_footEnd].day,m_pKlineAdd[nKline],m_nKlineAdd[nKline]);
	if(rtn>=0&&rtn<1000000)
	{
		if(rtn<m_nKlineAdd[nKline])
			return rtn;
		else if(rtn == m_nKlineAdd[nKline] )
			return m_nKlineAdd[nKline]-1;
	}
	if(rtn==1000000)
	{
		rtn=FindTimeK(m_pKlineAdd[nKline][m_nKlineAdd[nKline]-1].day,pView->m_pkline,pView->m_nCountKline);
		if(rtn<1000000&& rtn>-1)
		{
			if(rtn>=m_footBegin)
				return m_nKlineAdd[nKline]-1;//-rtn+m_footBegin
		}
	}
	return -1;

}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：CaclMaxMin(Kline *pKline, int b, int e)
//      功能说明：计算最大、最小值
//      入口参数：
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::CaclMaxMin(Kline *pKline, int b, int e)
{
	for(int i=b;i<=e;i++)
	{
		if((pKline+i)->high >m_max_sun[0])
			m_max_sun[0]=(pKline+i)->high;
		if((pKline+i)->low<m_min_sun[0])
			m_min_sun[0]=(pKline+i)->low;
	}

}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawKLineAddedPage(CDC *pDC)
//      功能说明：画多图叠加界面
//      入口参数：
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DrawKLineAddedPage(CDC *pDC)
{
	m_nSon=0;
	SetRectCurrent(m_nSon);
	
	if(	m_bNewStock == true || m_kLine_mode == 0 )
		CaclMaxAdded();
	
	DrawRulorText(pDC);

	DrawCaptionAdded(pDC);
	
	DrawKLine(pDC);
	
	for(int i=0;i<m_nKlineCurrent;i++)
	{
		DrawKLineAdded(pDC,i);
	}

}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：ReadPower(CString &symbol)
//      功能说明：读除权数据
//      入口参数：
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineShowKline::ReadPower(CString &symbol,int stkKind)
{
	//读数据
	m_PowerArray.RemoveAll();
	return CTaiKlineTransferKline::ReadPower(pDoc,symbol,stkKind, m_PowerArray);//m_PowerArray
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：WritePower(CString &symbol)
//      功能说明：写除权数据到文件
//      入口参数：nKln为第几个K线
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::WritePower(CString &symbol,int stkKind)
{
	PCdat1 pStockData;
	pDoc->m_sharesInformation .Lookup (symbol.GetBuffer (0),pStockData,stkKind);
	if(pStockData==NULL)
		return ;

	int n = m_PowerArray.GetSize();
	if(n<0|| n>80)
	{
		ASSERT(FALSE);
		n=0;
	}

	if(pStockData->pBaseInfo==0) return;
	pStockData->pBaseInfo->NumSplit = n;
	for(int i=0;i<n ;i++)
	{
		Split* pSplit = pStockData->pBaseInfo->m_Split+i;
		pSplit->Allocate		= m_PowerArray[i].fAllocate		;
		pSplit->AllocatePrice	= m_PowerArray[i].fAllocatePrice;
		pSplit->Give			= m_PowerArray[i].fGive			;
		pSplit->nTime			= m_PowerArray[i].nTime			;
		pSplit->nFlags			= m_PowerArray[i].nFlags		;
		pSplit->Bonus			= m_PowerArray[i].fDividend		;
	}
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawKLineAmericaAdded(CDC *pDC, int nKln)
//      功能说明：显示美国线的多图叠加
//      入口参数：nKln为第几个K线
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DrawKLineAmericaAdded(CDC *pDC, int nKln)
{
		int leftf=0;
		int topf=pView->m_heightCaption;
		int bottomf=pView->m_rtKlineFiguer.rtBlw[0].m_yBottom;
		int rightf=pView->m_rtKlineFiguer.rightX;

		if(bottomf-topf<2)
			return;

//		float cellWidth=(float)(rightf-leftf)/m_klinNumDefault;
		int wdth=rightf-leftf;
		int numberKline=m_klinNumDefault;
		///////////////////////////////////////////////////////////
		float widthPer =wdth/(float)(numberKline);// klineWidthPer + 2 * klineSide;//每格的宽度
		int klineWidthPer =(int) (widthPer*3/10);//k线的宽度

		float close,open,high,low;
		CPen pen_yong(PS_SOLID,1,pDoc->m_colorArray[7+nKln]);
		CPen* pOldpen=pDC->SelectObject(&pen_yong);

		CBrush brush_yong;
		if (!brush_yong.CreateSolidBrush(pDoc->m_colorArray[3]))
			ASSERT(FALSE);
		CBrush* pOldBrush=pDC->SelectObject(&brush_yong);
		
		int bAdd=0;
		int eAdd=0;
		bAdd=CaclBeginFoot(nKln);
		if(bAdd==-1)
		{
			pDC->SelectObject (pOldpen);
			pDC->SelectObject (pOldBrush);
			return;
		}
		eAdd=CaclEndFoot(nKln);
		if(eAdd==-1)
		{
			pDC->SelectObject (pOldpen);
			pDC->SelectObject (pOldBrush);
			return;
		}

		int j=m_footBegin;
		float moveVla=0;
		if(m_axisType==1)
			moveVla=m_pKlineAdd[nKln][bAdd].open-pView->m_pkline[m_footBegin].open;
		if(m_pKlineAdd[nKln][bAdd].open<=0)
		{
			pDC->SelectObject (pOldpen);
			pDC->SelectObject (pOldBrush);
			return;
		}
		for(int i=bAdd; i<=eAdd; i++ )
		{
			if(m_axisType==1)
			{
				open = (m_pKlineAdd[nKln][i].open-m_pKlineAdd[nKln][bAdd].open)*pView->m_pkline[m_footBegin].open/m_pKlineAdd[nKln][bAdd].open+pView->m_pkline[m_footBegin].open;
				close = (m_pKlineAdd[nKln][i].close-m_pKlineAdd[nKln][bAdd].open)*pView->m_pkline[m_footBegin].open/m_pKlineAdd[nKln][bAdd].open+pView->m_pkline[m_footBegin].open;
				high = (m_pKlineAdd[nKln][i].high-m_pKlineAdd[nKln][bAdd].open)*pView->m_pkline[m_footBegin].open/m_pKlineAdd[nKln][bAdd].open+pView->m_pkline[m_footBegin].open;
				low = (m_pKlineAdd[nKln][i].low-m_pKlineAdd[nKln][bAdd].open)*pView->m_pkline[m_footBegin].open/m_pKlineAdd[nKln][bAdd].open+pView->m_pkline[m_footBegin].open;
			}
			else
			{
				open = m_pKlineAdd[nKln][i].open;
				close = m_pKlineAdd[nKln][i].close;
				high = m_pKlineAdd[nKln][i].high;
				low = m_pKlineAdd[nKln][i].low;
			}

			if( m_pKlineAdd[nKln][i].high > 0 )
			{
				int closek = (int)(YTransfer(close) );
				int openk = (int)(YTransfer(open) );
				int highk = (int)YTransfer(high);//( hght * ( m_max_sun[m_nSon] - high ) / fudu_price ) + topf;
				int lowk = (int)YTransfer(low);//( hght * ( m_max_sun[m_nSon]-low ) / fudu_price ) + topf;



//				int flag=0;//0	yin;1	yong;2	ping
				CRect rt;
				rt.top =closek;
				rt.bottom =openk;

				//--确定与第一根K线的关系	old mistake
				int nRate = 1;
				if(pView->m_nKlineKind2 >=5)
					nRate = 60*60*24;
				if(m_pKlineAdd[nKln][i].day/nRate<pView->m_pkline[j].day/nRate)
					continue;
				if(m_pKlineAdd[nKln][i].day/nRate>pView->m_pkline[j].day/nRate)
				{
					while(m_pKlineAdd[nKln][i].day/nRate>pView->m_pkline[j].day/nRate&&j<m_footEnd)
					{
						j++;
					}
					if((m_pKlineAdd[nKln][i].day/nRate<pView->m_pkline[j].day/nRate))
						continue;
				}

				//-	画出
				int x= leftf +(int)( (j-m_footBegin+0.5) * (float)widthPer);
				if(klineWidthPer<1)
				{
					rt.left=leftf +(int)(  (j-m_footBegin+0.5) * (float)widthPer);
					rt.right =leftf + (int)( (j-m_footBegin+0.5) * (float)widthPer);
					pDC->MoveTo (x,highk);
					pDC->LineTo (x,lowk);
				}
				else
				{
					rt.left=leftf -klineWidthPer +(int)(  (j-m_footBegin+0.5) * (float)widthPer);
					rt.right =leftf +klineWidthPer + (int)( (j-m_footBegin+0.5) * (float)widthPer);
					
					pDC->MoveTo (x-(int)(klineWidthPer),openk);
					pDC->LineTo (x,openk);
					pDC->MoveTo (x+(int)(klineWidthPer),closek);
					pDC->LineTo (x,closek);

					pDC->MoveTo (x,highk);
					pDC->LineTo (x,lowk);
				}
			}
			j++;

		}

		pDC->SelectObject (pOldpen);
		pDC->SelectObject (pOldBrush);

}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：DrawPower(CDC *pDC)
//      功能说明：显示除权标志
//      入口参数：
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::DrawPower(CDC *pDC)
{
	POWER pwr;
	for(int i=0;i< m_PowerArray.GetSize();i++)
	{
		pwr=m_PowerArray[i];
		int foot2=pView->FindTimeK ((int)pwr.nTime);

		if(foot2>=m_footBegin&&foot2<=m_footEnd)
		{
			//---		画出
			CBitmap       m_bitmap;

			CImageList imagelist;
			imagelist.Create(15,15, ILC_MASK, 1, 2);
    
			m_bitmap.LoadBitmap(IDB_BITMAP_138);
			imagelist.Add(&m_bitmap, (COLORREF)0x00ffFFFF);
			m_bitmap.DeleteObject();

			POINT p;
			p.y=pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-18;

			if(m_klinNumDefault<=0)
				return ;
			p.x=(int)((foot2-m_footBegin+0.5)*pView->m_rtKlineFiguer.rightX/m_klinNumDefault-8);
			imagelist.Draw (pDC ,0,p,ILD_TRANSPARENT);
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：ShowTjxgShow(CDC *pDC)
//      功能说明：显示条件选股提示
//      入口参数：
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::ShowTjxgShow(CDC *pDC)
{
	//---		画出
	if(m_klinNumDefault<=0)
		return ;

	if(pView->m_screenStockShow .m_testResultArray.GetSize()!=1)
		return;

	SCREEN_RESULT* pResult = pView->m_screenStockShow .m_testResultArray[0];
	int n = pResult->m_detail .GetSize();
	if(n<=0)
		return;

	//显示图标
/*	int tmBegin = pView->m_pkline[m_footBegin].day;//tm.GetYear ()*10000 + tm.GetMonth()*100 + tm.GetDay ();
	int tmEnd = pView->m_pkline[m_footEnd].day;//	int tmBeginHour = tm.GetHour ()*100+tm.GetMinute ();
	bool bday = false;
	if(CTaiKlineFileKLine::IsDayKline(pView->m_nKlineKind2  ))
	{
		CTime tm(pView->m_pkline[m_footBegin].day);
		tmBegin = CTime(tm.GetYear (),tm.GetMonth(),tm.GetDay (),0,0,0).GetTime();

		tm = CTime(tmEnd);
		tmEnd = CTime(tm.GetYear (),tm.GetMonth(),tm.GetDay (),23,59,59).GetTime();
		bday=true;
	}*/
	int k=0;
	for(int j=0;j<n;j++)
	{
		SCREEN_DETAIL* pDetail = &(pResult->m_detail[j]);

//		if(pDetail->buytime >=tmBegin && pDetail->buytime <=tmEnd)
		{
			//得到对应的下标
			for(;k<=m_footEnd;k++)
			{
				if(pView->m_pkline[k].day>=pDetail->buytime)
				{
						//draw now
						//if(detail->issuccess ==TRUE)
						POINT p[2];
						int nFlag[2] ={1,1};
						p[0].y=YTransfer((float)pView->m_pkline[k].low);
						p[0].x=(int)((k-m_footBegin+0.5)*pView->m_rtKlineFiguer.rightX/m_klinNumDefault);
						if((k-m_footBegin+0.5)<0) nFlag[0] = 0;

						if(k+pDetail->timerange <= m_footEnd)
							p[1].y=YTransfer((float)pView->m_pkline[k+pDetail->timerange].high)-18;
						p[1].x=(int)((k-m_footBegin+0.5+pDetail->timerange)*pView->m_rtKlineFiguer.rightX/m_klinNumDefault);
						if((k-m_footBegin+0.5+pDetail->timerange)<0) nFlag[1] = 0;
						for(int ii=0;ii<2;ii++)
						{
							if(pDoc->m_propertyInitiate.bTjxgZsFs==1)
							     if(pResult->nKindBuySell==0&&ii==1||pResult->nKindBuySell==1&&ii==0)
									 continue;
							if(nFlag[ii]==0) continue;
							if(k+pDetail->timerange > m_footEnd && ii == 1)
								break;

							if(pDetail->timerange<=0 && ii ==1)
								break;

							if(p[ii].y>pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-18)
							{
								p[ii].y =pView->m_rtKlineFiguer.rtBlw[0].m_yBottom-18;
							}
							if(p[ii].y<pView->m_heightCaption )
								p[ii].y = pView->m_heightCaption;

							int nMove = 2;
							int nMovey = 8;
							if(ii==0)
							{
								p[0].y += 8;
								nMovey = -8;
							}
							CPoint pt[4]={CPoint(p[ii].x-nMove,p[ii].y),CPoint(p[ii].x+nMove,p[ii].y),
								CPoint(p[ii].x,p[ii].y+nMovey),CPoint(p[ii].x-nMove,p[ii].y)};

							CPen pen;
							;

							//成功填入实心颜色
							if(pDetail->issuccess ==0 && ii == 0)
							{
								CRgn rgn;
								rgn.CreatePolygonRgn (pt,4,ALTERNATE);
								CBrush brush;
								brush.CreateSolidBrush (pView->m_screenStockShow .buySellAdditional.nColorPoint[ii]);
//								CBrush* pOld = pDC->SelectObject (&brush);
								pDC->FillRgn (&rgn,&brush);
//								pDC->SelectObject (pOld);
							}
							if(pen.CreatePen (PS_SOLID,1,pView->m_screenStockShow .buySellAdditional.nColorPoint[ii]))
							{
								CPen* pOld = pDC->SelectObject (&pen);
								pDC->MoveTo (pt[0]);
								for(int i=0;i<2;i++)
								{
									pDC->LineTo (pt[i+1]);
								}
								int nOut = -1;
								if(ii==0)
									nOut = 1;
								pt[0].x = pt[0].x -1;
								pt[0].y = pt[0].y +nOut;
								pt[1].x = pt[1].x +1;
								pt[1].y = pt[1].y +nOut;
								pt[2].x = pt[2].x ;
								pt[2].y = pt[2].y -nOut;
								pDC->LineTo (pt[0]);
								pDC->MoveTo (pt[0]);
								pDC->LineTo (pt[1]);
								pDC->LineTo (pt[2]);
								pDC->LineTo (pt[0]);
								pDC->SelectObject (pOld);
							}
							//imagelist.Draw (pDC ,0,p,ILD_TRANSPARENT);
						}/*
							int nMove = 3;
							int nMovey = 9;
							int ny = 6;
							int ny1 = 1;
							if(ii==0)
							{
								p[0].y += 16;
								nMovey = -9;
								ny = -6;
								ny1 = -1;
							}
							CPoint pt[6]={CPoint(p[ii].x-nMove,p[ii].y),CPoint(p[ii].x+nMove+1,p[ii].y),
								CPoint(p[ii].x+nMove+1,p[ii].y+nMovey-ny1),CPoint(p[ii].x,p[ii].y+nMovey+ny),
								CPoint(p[ii].x-nMove,p[ii].y+nMovey),CPoint(p[ii].x-nMove,p[ii].y-1)};

							CPen pen;
							;
//							COLORREF clr;

							//成功填入实心颜色
							if(pDetail->issuccess ==0 && ii == 0)
							{
								CRgn rgn;
								rgn.CreatePolygonRgn (pt,5,ALTERNATE);
								CBrush brush;
								brush.CreateSolidBrush (pView->m_screenStockShow .buySellAdditional.nColorPoint[ii]);
//								CBrush* pOld = pDC->SelectObject (&brush);
								pDC->FillRgn (&rgn,&brush);
//								pDC->SelectObject (pOld);
							}
							if(pen.CreatePen (PS_SOLID,2,pView->m_screenStockShow .buySellAdditional.nColorPoint[ii]))
							{
								CPen* pOld = pDC->SelectObject (&pen);
								pDC->MoveTo (pt[0]);
								for(int i=0;i<5;i++)
								{
									pDC->LineTo (pt[i+1]);
								}
								pDC->SelectObject (pOld);
							}
							//imagelist.Draw (pDC ,0,p,ILD_TRANSPARENT);
						}*/
						//end
						break;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//      函 数 名：ShowVirtualLine(CDC *pDC, CPoint &p1, CPoint &p2)
//      功能说明：画虚线
//      入口参数：p1,p2为起始点
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
void CTaiKlineShowKline::ShowVirtualLine(CDC *pDC, CPoint &p1, CPoint &p2)
{
//	CPen pen_line(PS_SOLID ,1,pDoc->m_colorArray[2]);//PS_DASHDOTDOT   
//	CPen* pOldpen=pDC->SelectObject(&pen_line);
	int gap=2;
	if(pDC->IsPrinting())
		gap=8;


	if(p1.x==p2.x)
	{
		int y1=p1.y,y2=p2.y;
		if(p1.y>p2.y)
		{
			y2=p1.y;
			y1=p2.y;
		}

		if(y2-y1>100000000)
		{
//			pDC->SelectObject(pOldpen);
			return;
		}

		for(int i=y1;i<=y2;i=i+gap)
		{
			pDC->MoveTo(p1.x,i);
			pDC->LineTo(p1.x,i+1);
		}
	}
	else if(p1.y==p2.y)
	{
		int x1=p1.x,x2=p2.x;
		if(p1.x>p2.x)
		{
			x2=p1.x;
			x1=p2.x;
		}

		if(x2-x1>100000000)
		{
//			pDC->SelectObject(pOldpen);
			return;
		}

		for(int i=x1;i<=x2;i=i+gap)
		{
			pDC->MoveTo(i,p1.y);
			pDC->LineTo(i+1,p1.y);
		}
	}
//	pDC->SelectObject(pOldpen);
}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：ShowFenshiRulor(CDC *pDC,int wid)
//      功能说明：画分时画面的标尺
//      入口参数：wid标识标尺
//      出口参数：无出口数
//      返回参数：无
/////////////////////////////////////////////////////////////////////////////
bool CTaiKlineShowKline::ShowFenshiRulor(CDC *pDC,int wid)
{
	if(pView->m_nKlineKind2>0&&wid!=1)
		return false;
	if((pView->m_nKlineKind2==0||wid==1) && pView->pMin1Drawing ->m_pFlg[m_nSon]!=FS_ZOUSHI && pView->pMin1Drawing ->m_pFlg[m_nSon]!=FS_LINXIAN )
		return false;

	CRect rt(m_rectDrawLine.right,m_rectDrawLine.top,
		m_rectDrawLine.right+pView->m_midLen,m_rectDrawLine.bottom);
    /////////////////////////
	if(m_rectDrawLine.bottom-m_rectDrawLine.top<2)
		return true;
	//------根据最大值及最小值来确定显示的数字的位数
	float max;
	max=(float)(FABSMY(m_max_sun[m_nSon])>FABSMY(m_min_sun[m_nSon])?FABSMY(m_max_sun[m_nSon]):FABSMY(m_min_sun[m_nSon]));
	m_step[m_nSon]=0;
	int flag;	//flag=0为浮点数，1为整数，2为大于等于100000的整数
	if(max<100)
		flag=0;
	else
		flag=1;
	//------确定画几条线
	int numLine=(m_rectDrawLine.bottom-m_rectDrawLine.top)/pView->m_minGrid+1;
	if(numLine%2==1)
		numLine++;
	//------确定每格的数值
	float valuePer;
	valuePer=(m_max_sun[m_nSon]-m_min_sun[m_nSon])/numLine;
	if(valuePer<0.01)
		valuePer=(float)(((int)ceil(valuePer*1000))/(float)1000);
	else
		valuePer=(float)(((int)ceil(valuePer*100))/(float)100);

	//calculate close
	float cls;
	if(wid!=1)
	{
		if(pView->pMin1Drawing ->m_pFlg[m_nSon]==FS_ZOUSHI)
			cls=pView->pMin1Drawing ->m_close;
		else
			cls=pView->pMin1Drawing ->m_close_Dapan[0] ;
	}
	else
	{
		if(!CTaiShanKlineShowView::IsIndexStock(pView->m_sharesSymbol))
		{
			if(pView->m_tabNum ==3)
				cls=pView->pMin1Drawing ->m_close;
			else if(pView->m_tabNum ==2)
				cls=pView->pMin1Drawing ->m_close_Dapan[0] ;
			else
				return true;
		}
		else
		{
			if(pView->m_nKlineKind2 ==0)
				cls=pView->pMin1Drawing ->m_close_Dapan[1] ;
			else
				cls=pView->pMin1Drawing ->m_close_Dapan[0] ;
		}
	}

	m_max_sun[m_nSon]=valuePer*(numLine/2)+cls;

//	if(cls - valuePer*(numLine/2)<=0)
//		return true;

	m_min_sun[m_nSon]=cls - valuePer*(numLine/2);

	CPen pen_solid(PS_SOLID ,1,pDoc->m_colorArray[2]);//PS_DASHDOTDOT   
	CPen* pOldpen=pDC->SelectObject(&pen_solid);

	int m=2;

	//---		画线
	int i;
	for(i=0;i<numLine;i++)
	{
		int y;
		y=YTransfer((float)(valuePer*(i+1)+m_min_sun[m_nSon]));

		if(y<m_rectDrawLine.bottom-3 && y>=m_rectDrawLine.top)
		{
//
//			if(m_nSon==0&&i==numLine-1)
//				y=pView->m_heightCaption;

			pDC->MoveTo(m_rectDrawLine.right+1,y);
			pDC->LineTo(m_rectDrawLine.right+6,y);

			//---		画虚线
			CPoint p1,p2;
			p1.x=m_rectDrawLine.left;
			p2.x=m_rectDrawLine.right+1;
			p1.y=y;
			p2.y=y;
			if(m_nSon==0&&i==numLine-1)
			{
	//---		画实线
//				pDC->MoveTo(p1.x,p1.y);	pDC->MoveTo((int)(pView->m_rtMin1.leftX),pView->m_heightCaption);

//				pDC->LineTo(p2.x,p2.y);
				continue;
			}

			ShowVirtualLine(pDC,p1,p2);
		
		}//end if
	}

//	if(y>m_rectDrawLine.bottom-8) continue;
	//---		画文字
	pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
	pDC->SetTextColor( pDoc->m_colorArray[2]);
	pDC->SetBkMode(TRANSPARENT);

	CFont ft;
	LOGFONT lgf=pDoc->m_fontstr[0];
	if(wid==1)
		lgf.lfHeight=-14;
	ft.CreateFontIndirect (&lgf );
	CFont* pOld=pDC->SelectObject(&ft);
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);

	int width=pView->m_midLen;
	if(wid==1)
		width=m_midLen_L;
//wid=0为缺省，wid=1为小图，wid=2为分时画面价格的两边文本，wid=3为分时画面其它的两边文本，wid=4为百分比坐标

	CString bck="";
	if(wid==4)
		bck="%";

	int num_b=1;
	CString sForm = "%.2f";
	if(CTaiShanReportView::IsBg(pView->pMin1Drawing ->m_pReportData -> kind))
		sForm = "%.3f";
	switch(flag)
	{
	case 0:
		for(i=-1;i<numLine;i++)
		{
			CString str;
			int y=YTransfer((float)(valuePer*(num_b+i)+m_min_sun[m_nSon]));

			if(y<m_rectDrawLine.bottom-5)
			{
				if(wid!=2&&!(wid==1&&pView->pMin1Drawing->m_nSon==0))
				{
					str.Format(sForm,(float)(valuePer*(num_b+i)+m_min_sun[m_nSon]));
					str+=bck;
					if(wid==3)
						pDC->TextOut(m_rectDrawLine.left-m,y-tm.tmHeight/2,str);
					if(y>m_rectDrawLine.bottom-5  || y<m_rectDrawLine.top-1)
						continue;
					pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);

				}
				//---	如果是小图
				else if(wid==1&&pView->pMin1Drawing->m_nSon==0)
				{
					if(y>m_rectDrawLine.bottom-5 || y<m_rectDrawLine.top-1)
						continue;
//					float cls;
//					if(pView->m_tabNum ==3)
//						cls=pView->pMin1Drawing->m_close;
//					else
//						cls=pView->pMin1Drawing->m_close_Dapan[0];

					float nw=(valuePer*(num_b+i)+m_min_sun[m_nSon]);
					str.Format(sForm,nw);
					if(nw>cls)
						pDC->SetTextColor( pDoc->m_colorArray[13]);
					else if(nw<cls)
						pDC->SetTextColor( pDoc->m_colorArray[15]);
					else
						pDC->SetTextColor( pDoc->m_colorArray[14]);
					pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);

				}
				//---	如果是分时走势
				else if(wid==2)
				{
//					float cls=pView->pMin1Drawing->m_close;
					float nw=(valuePer*(num_b+i)+m_min_sun[m_nSon]);
					str.Format(sForm,nw);
					if(nw>cls)
						pDC->SetTextColor( pDoc->m_colorArray[13]);
					else if(nw<cls)
						pDC->SetTextColor( pDoc->m_colorArray[15]);
					else
						pDC->SetTextColor( pDoc->m_colorArray[14]);
					pDC->TextOut(m_rectDrawLine.left-m,y-tm.tmHeight/2,str);

					if(y>m_rectDrawLine.bottom-5 || y<m_rectDrawLine.top-1)
						continue;
					if(cls>0)
					{
						str.Format("%.1f",(float)(valuePer*(num_b+i)+m_min_sun[m_nSon]-cls)*100/cls);
						str+="%";
						pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);
					}
				}
			}
		}//end for
		break;
	case 1:
		for(i=-1;i<numLine;i++)
		{
			CString str;
			int y=YTransfer((float)(valuePer*(num_b+i)+m_min_sun[m_nSon]));

			if(y<m_rectDrawLine.bottom-5)
			{
				if(wid!=2&&!(wid==1&&pView->pMin1Drawing->m_nSon==0))
				{
					str.Format("%d",(int)(valuePer*(num_b+i)+m_min_sun[m_nSon]));
					str+=bck;
					if(wid==3)
							pDC->TextOut(m_rectDrawLine.left-m,y-tm.tmHeight/2,str);
					if(y>m_rectDrawLine.bottom-5 || y<m_rectDrawLine.top-1)
						continue;
					pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);
				}
				//---	如果是小图
				else if(wid==1&&pView->pMin1Drawing->m_nSon==0)
				{
					if(y>m_rectDrawLine.bottom-5 || y<m_rectDrawLine.top-1)
						continue;

//					float cls;
//					if(pView->m_tabNum ==3)
//						cls=pView->pMin1Drawing->m_close;
//					else
//						cls=pView->pMin1Drawing->m_close_Dapan[0];

					float nw=(valuePer*(num_b+i)+m_min_sun[m_nSon]);
					str.Format("%d",(int)nw);
					if(nw>cls)
						pDC->SetTextColor( pDoc->m_colorArray[13]);
					else if(nw<cls)
						pDC->SetTextColor( pDoc->m_colorArray[15]);
					else
						pDC->SetTextColor( pDoc->m_colorArray[PING_PAN_WORD]);
					pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);

				}
				//---	如果是分时走势
				else if(wid==2)
				{
//					float cls=pView->pMin1Drawing->m_close;
					float nw=(valuePer*(num_b+i)+m_min_sun[m_nSon]);
					str.Format("%d",(int)nw);
					if(nw>cls)
						pDC->SetTextColor( pDoc->m_colorArray[13]);
					else if(nw<cls)
						pDC->SetTextColor( pDoc->m_colorArray[15]);
					else
						pDC->SetTextColor( pDoc->m_colorArray[PING_PAN_WORD]);
					pDC->TextOut(m_rectDrawLine.left-m,y-tm.tmHeight/2,str);

					if(y>m_rectDrawLine.bottom-5 || y<m_rectDrawLine.top-1)
						continue;
					if(cls>0)
					{
						str.Format("%.1f",(float)((valuePer*(num_b+i)+m_min_sun[m_nSon]-cls)*100/cls));
						str+="%";
						pDC->TextOut(m_rectDrawLine.right+width-m,y-tm.tmHeight/2,str);
					}
				}
			}
		}
		break;
	}
	pDC->SelectObject(pOld);
	pDC->SelectObject(pOldpen);
	return true;
	

}


bool CTaiKlineShowKline::DrawRulorTextSelf(CDC *pDC, int wid)
{
	if(pView->m_nKlineKind2 == 0 || m_nSon == 0 || wid == 1 )
		return false;

	CString s0(pView->m_infoInit.initIndex[m_nSon].nameIndex);
	CFormularContent*	pIndex;
	pIndex=(CFormularContent*)pDoc->LookUpArray (pDoc->m_formuar_index,s0); 
	if(pIndex==NULL)
		return true;

//	if(pIndex->posFlag == 0)
//		return false;

	CPen pen_solid(PS_SOLID ,1,pDoc->m_colorArray[2]);//PS_DASHDOTDOT   
	CPen* pOldpen=pDC->SelectObject(&pen_solid);
	bool b=false;
	for(int i=0;i<7;i++)
	{
		int y;
		if((float)(pIndex->posX[i] )>m_max_sun[m_nSon] || (float)(pIndex->posX[i] )<m_min_sun[m_nSon])
			continue;

		b= true;

		y=YTransfer((float)(pIndex->posX[i] ));

		

		if(y<m_rectDrawLine.bottom-3 && y >= m_rectDrawLine.top)
		{
			//---		画虚线
			if(!(pDoc->m_systemOption.showxline==FALSE && (pView->m_nKlineKind2>0 && wid !=1)))
			{
				CPoint p1,p2;
				p1.x=m_rectDrawLine.left;
				p2.x=m_rectDrawLine.right+1;
				p1.y=y;
				p2.y=y;
				ShowVirtualLine(pDC,p1,p2);
			}

			pDC->MoveTo(m_rectDrawLine.right+1,y);
			pDC->LineTo(m_rectDrawLine.right+6,y);

			// draw right text 
			//---		画文字
			pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
			pDC->SetTextColor( pDoc->m_colorArray[2]);
			pDC->SetBkMode(TRANSPARENT);

			CFont ft;
			LOGFONT lgf=pDoc->m_fontstr[0];
			ft.CreateFontIndirect (&lgf );
			CFont* pOld=pDC->SelectObject(&ft);

			CString sRight;
			if((float)FABSMY(pIndex->posX[i] )<100)
				sRight.Format("%.2f",(float)pIndex->posX[i] );
			else
				sRight.Format("%d",(int)pIndex->posX[i] );
			int hgt =(pDC->GetOutputTextExtent(sRight)).cy;
			CRect r(m_rectDrawLine.right ,0,m_rectDrawLine.right + pView ->m_midLen,hgt +y);
			pDC->ExtTextOut (m_rectDrawLine.right + pView ->m_midLen -2 ,y - hgt/2 ,ETO_CLIPPED,r,sRight,NULL);
	
			pDC->SelectObject(pOld);
		}//end if
	}

	pDC->SelectObject(pOldpen);
	if(b == false)
		return false;

	return true;
}

void CTaiKlineShowKline::OutBitmap(CDC *pDC,int id,CPoint & p)
{
	CPen pen_red(PS_SOLID,2,RGB(255,0,0));
	CPen pen_green(PS_SOLID,2,RGB(0,128,0));
	CPen* pOldpen;
//	pOldpen=pDC->SelectObject(&pen_yong);

	CBrush* pOldBrush;
	//=pDC->SelectObject(&brush_yong);

	switch (id)
	{
	case IDB_40:
		pOldpen=pDC->SelectObject (&pen_red);
		pDC->MoveTo (p.x+1,p.y+6);
		pDC->LineTo (p.x+7,p.y);
		pDC->LineTo (p.x+13,p.y+6);
		pDC->LineTo (p.x+9,p.y+6);
		pDC->LineTo (p.x+9,p.y+14);
		pDC->LineTo (p.x+5,p.y+14);
		pDC->LineTo (p.x+5,p.y+6);
		pDC->LineTo (p.x+1,p.y+6);
		pDC->SelectObject (pOldpen);
		break;
	case IDB_41://
		pOldpen=pDC->SelectObject (&pen_green);
		pDC->MoveTo (p.x+1,p.y-6);
		pDC->LineTo (p.x+7,p.y);
		pDC->LineTo (p.x+13,p.y-6);
		pDC->LineTo (p.x+9,p.y-6);
		pDC->LineTo (p.x+9,p.y-14);
		pDC->LineTo (p.x+5,p.y-14);
		pDC->LineTo (p.x+5,p.y-6);
		pDC->LineTo (p.x+1,p.y-6);
		pDC->SelectObject (pOldpen);
		break;
	case IDB_BITMAP_137:
		{
			CBrush brush_pink;
			if (!brush_pink.CreateSolidBrush(RGB(255,0,255)))
				ASSERT(FALSE);
			pOldBrush=pDC->SelectObject(&brush_pink);
			CRect r2(p.x+1,p.y+1,p.x+14,p.y+14);
			pDC->Ellipse (r2);

			pDC->SelectObject(pOldBrush);

			CFont ft;
			LOGFONT lgf=pDoc->m_fontstr[0];
			lgf.lfHeight=-13;
			lgf.lfWidth=9;
			ft.CreateFontIndirect (&lgf );
			CFont* pOld=pDC->SelectObject(&ft);
			pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
			pDC->SetTextColor(  RGB(255,255,0));
			pDC->SetBkMode(TRANSPARENT);

			pDC->TextOut (p.x+4,p.y+1,"K");
			pDC->SelectObject(pOld);
		}
		break;
	}
}

void CTaiKlineShowKline::MoveDragMode(CPoint &pPre, CPoint &pNext)
{
	//calculate x position
	int n =( pNext.x-pPre.x )*m_klinNumDefault/pView->m_rtKlineFiguer .rightX ;

	m_footBegin -=n;
	if(m_footBegin<0)
		m_footBegin =0;
	m_footEnd = m_footBegin + m_klinNumDefault - 1;
	if(m_footEnd > pView->m_nCountKline -1)
		m_footEnd = pView->m_nCountKline -1;

	//calculate y position
	float min ;
	float max;

	//set rect
	SetRectCurrent(m_nSon);

	if(m_nSon==0)
	{
		min= YTransfer((int)(pView->m_rtKlineFiguer .rtBlw[m_nSon].m_yBottom  - 3 - pNext.y+pPre.y));
		max = YTransfer((int)(pView->m_heightCaption  - pNext.y+pPre.y));
	}
	else
	{
		min= YTransfer((int)(pView->m_rtKlineFiguer .rtBlw[m_nSon].m_yBottom  - pNext.y+pPre.y));
		max = YTransfer((int)(pView->m_heightCaption + pView->m_rtKlineFiguer .rtBlw[m_nSon-1].m_yBottom  - pNext.y+pPre.y));
	}

	TRACE("pNext = %d     pPre = %d\n",(int)pNext.y,(int)pPre.y);
	m_min_sun[m_nSon] = min;
	m_max_sun[m_nSon] = max;
//	TRACE("after   after MAX  MAX = %.2f     Min  Min = %.2f\n",m_max_sun[m_nSon],m_min_sun[m_nSon]);

	//draw figuer
	CRect r;
	pView->GetClientRect (r);
	r.right =pView->m_rtKlineFiguer.rightX+pView->m_midLen;
	CTaiKlineDC* pMemdc = new CTaiKlineDC(pView,&pView->m_bit,&r);
	ValidDoKline(true);
	DrawKlineFiguer(pMemdc);
	delete pMemdc;
}
void CTaiKlineShowKline::DrawKlineHS(CDC *pDC)
{
		int leftf=m_rectDrawLine.left;
		int topf=m_rectDrawLine.top;
		int bottomf=m_rectDrawLine.bottom;
		int rightf=m_rectDrawLine.right;

		//clip region
		CRgn rgn;
		if(!pDC->IsPrinting())
		{
			if(rgn.CreateRectRgn ( leftf, topf, rightf, bottomf )==ERROR)
				return ;
			pDC->SelectClipRgn (&rgn);
		}
		int wdth=rightf-leftf;
		int numberKline=m_klinNumDefault;
		///////////////////////////////////////////////////////////
		float widthPer =wdth/(float)(numberKline);// klineWidthPer + 2 * klineSide;//每格的宽度

		CPen pen_yong(PS_SOLID,1,pDoc->m_colorArray[3]);
		CPen pen_yin(PS_SOLID,1,pDoc->m_colorArray[4]);
		CPen pen_ping(PS_SOLID,1,pDoc->m_colorArray[2]);
		CPen* pOldpen=pDC->SelectObject(&pen_yong);

		for(int i=m_footBegin; i<=m_footEnd; i++ )
		{

			if( pView->m_pkline[i].close<=0)
			{
				if(m_axisType==2)
					break;
				continue;
			}
			int x= leftf +(int)( (i-(m_footBegin)+0.5) * (float)widthPer);

			float close;
			close = pView->m_pkline[i].open;
			int closek = (int)(YTransfer(close) );

//				int flag=0;//0	yin;1	yong;2	ping
			if( pView->m_pkline[i].vol < 0 )
			{
				pDC->SelectObject (&pen_yin);
				pDC->Ellipse (x-2,closek-2,x+2,closek+2);
			}
			else if( pView->m_pkline[i].vol > 0 )
			{
				pDC->SelectObject (&pen_yong);
				pDC->Ellipse (x-2,closek-2,x+2,closek+2);
			}
			else 
			{
				if(i>0)
				{
					if(pView->m_pkline[i].close-pView->m_pkline[i-1].close>=0)
						pDC->SelectObject (&pen_yong);
					else
						pDC->SelectObject (&pen_yin);
				}
				else if(i==0)
					pDC->SelectObject (&pen_yong);
				pDC->Ellipse (x-2,closek-2,x+2,closek+2);
			}

			if( pView->m_pkline[i].vol < 0 )
				pView->m_pkline[i].vol = - pView->m_pkline[i].vol ;
		}

			
//write high price and low price
		CFont ft;
		LOGFONT lgf=pDoc->m_fontstr[0];
		ft.CreateFontIndirect (&lgf );
		CFont* pftOld = pDC->SelectObject(&ft);
		pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		pDC->SetTextColor(  pDoc->m_colorArray[1]);
		pDC->SetBkMode(TRANSPARENT);

		int leftHL=(int)( (m_highFoot-(m_footBegin)+0.5) * (float)widthPer)+m_rectDrawLine.left;
		int topHL=YTransfer(pView->m_pkline[m_highFoot].high);
		CString sHL;
		sHL.Format("%.2f",pView->m_pkline[m_highFoot].high);
		if(leftHL>(m_rectDrawLine.right+m_rectDrawLine.left)/2)
			pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
		else
			pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		pDC->ExtTextOut (leftHL,topHL ,ETO_CLIPPED,m_rectDrawLine,sHL,NULL);

		leftHL=(int)( (m_lowFoot-(m_footBegin)+0.5) * (float)widthPer)+m_rectDrawLine.left;
		topHL=YTransfer(pView->m_pkline[m_lowFoot].low);
		sHL.Format("%.2f",pView->m_pkline[m_lowFoot].low);
		if(leftHL>(m_rectDrawLine.right+m_rectDrawLine.left)/2)
			pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
		else
			pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		topHL-=((pDC->GetOutputTextExtent(sHL)).cy+3);
		pDC->ExtTextOut (leftHL,topHL ,ETO_CLIPPED,m_rectDrawLine,sHL,NULL);
		pDC->SelectObject(pftOld);
		
		if(!pDC->IsPrinting())
		{
			pDC->SelectClipRgn (NULL);//&rgn2
			rgn.DeleteObject ();
		}
		pDC->SelectObject (pOldpen);

}

bool CTaiKlineShowKline::DrawBTX(CDC *pDC)
{
		int leftf=m_rectDrawLine.left;
		int topf=m_rectDrawLine.top;
		int bottomf=m_rectDrawLine.bottom;
		int rightf=m_rectDrawLine.right;

		int wdth=rightf-leftf;
		int numberKline=m_klinNumDefault;
		///////////////////////////////////////////////////////////
		float widthPer =wdth/(float)(numberKline);// klineWidthPer + 2 * klineSide;//每格的宽度
		int klineWidthPer = (int)(widthPer*3/10);//k线的宽度

		CPen pen_yong(PS_SOLID,1,pDoc->m_colorArray[3]);
		CPen pen_yin(PS_SOLID,1,pDoc->m_colorArray[4]);
		CPen pen_ping(PS_SOLID,1,pDoc->m_colorArray[2]);
		CPen* pOldpen=pDC->SelectObject(&pen_yong);

		CBrush brush_yong;
		if (!brush_yong.CreateSolidBrush(pDoc->m_colorArray[3]))
			ASSERT(FALSE);
		CBrush* pOldBrush=pDC->SelectObject(&brush_yong);

		float closeYest = pView->m_pkline[0].close;//起步价
		float openYest = pView->m_pkline[0].close;
		if(m_footBegin>0)
		{
			closeYest = pView->m_pkline[m_footBegin-1].close;
			openYest = pView->m_pkline[m_footBegin-1].close;
		}

		int begin0 = 0;
		if(m_footBegin == 0)
			begin0 = 1;
		for(int i=m_footBegin+begin0; i<=m_footEnd; i++ )
		{

			if( pView->m_pkline[i].close<=0||pView->m_pkline[i].high <= 0 )
			{
				if(m_axisType==2)
					break;
				continue;
			}
			int x= leftf +(int)( (i-(m_footBegin)+0.5) * (float)widthPer);

			float close;
			close = pView->m_pkline[i].close;
			int closek = (int)(YTransfer(close) );
			int openk = (int)(YTransfer(closeYest) );
			int openkYest = (int)(YTransfer(openYest) );

			CRect rt;
			rt.top =closek;
			rt.bottom =openk;
			if(klineWidthPer<1)
			{
				if( openYest <= closeYest )
				{
					if(close>=closeYest)//draw red
					{
						rt.top =closek;
						rt.bottom =openk;

						pDC->SelectObject (&pen_yong);
						pDC->MoveTo (x ,rt.top);
						pDC->LineTo (x ,rt.bottom);
						openYest = closeYest;
						closeYest = close;
					}
					else
					{
						if(close>=openYest)//draw red
						{
							rt.top =closek;
							rt.bottom =openk;

							pDC->SelectObject (&pen_yong);
							pDC->MoveTo (x ,rt.top);
							pDC->LineTo (x ,rt.bottom);

							openYest = close;
						}
						else//draw red and green
						{
							rt.top =openkYest;
							rt.bottom =openk;

							pDC->SelectObject (&pen_yong);
							pDC->MoveTo (x ,rt.top);
							pDC->LineTo (x ,rt.bottom);

							rt.top =openkYest;
							rt.bottom =closek;
							pDC->SelectObject (&pen_yin);
							pDC->MoveTo (x ,rt.top);
							pDC->LineTo (x ,rt.bottom);

							openYest = closeYest;
							closeYest = close;
						}
					}
				}
				else
				{
					if(close<=closeYest)//draw green
					{
						pDC->SelectObject (&pen_yin);
						pDC->MoveTo (x ,openk);
						pDC->LineTo (x ,closek);

						openYest = closeYest;
						closeYest = close;
					}
					else
					{
						if(close<=openYest)//draw green
						{
							pDC->SelectObject (&pen_yin);
							pDC->MoveTo (x ,openk);
							pDC->LineTo (x ,closek);
						
							openYest = close;
						}
						else//draw red and green
						{
							pDC->SelectObject (&pen_yin);
							pDC->MoveTo (x ,openk);
							pDC->LineTo (x ,openkYest);

							rt.top =openkYest;
							rt.bottom =closek;
							pDC->SelectObject (&pen_yong);
							pDC->MoveTo (x ,openkYest);
							pDC->LineTo (x ,closek);

							openYest = closeYest;
							closeYest = close;
						}
					}
				}

				continue;
			}

			rt.left=leftf -klineWidthPer +(int)(  (i-(m_footBegin)+0.5) * (float)widthPer);
			rt.right =leftf +klineWidthPer + (int)( (i-(m_footBegin)+0.5) * (float)widthPer);
			if( openYest <= closeYest )
			{
				if(close>=closeYest)//draw red
				{
					if(openk<=closek)
					{
						rt.top =openk;
						rt.bottom =closek;
					}
					else
					{
						rt.top =closek;
						rt.bottom =openk;
					}


					pDC->SelectObject (&pen_yong);
					pDC->FillSolidRect (rt,pDoc->m_colorArray[0]);
					pDC->MoveTo (x-(int)(klineWidthPer),rt.top);
					pDC->LineTo (x-(int)(klineWidthPer),rt.bottom);
					pDC->LineTo (x+(int)(klineWidthPer),rt.bottom);
					pDC->LineTo (x+(int)(klineWidthPer),rt.top);
					pDC->LineTo (x-(int)(klineWidthPer),rt.top);

					pDC->MoveTo (x-(int)(klineWidthPer),openk);
					pDC->LineTo (x+(int)(klineWidthPer),openk);

					openYest = closeYest;
					closeYest = close;
				}
				else
				{
					if(close>=openYest)//draw red
					{
						if(openk<=closek)
						{
							rt.top =openk;
							rt.bottom =closek;
						}
						else
						{
							rt.top =closek;
							rt.bottom =openk;
						}

						pDC->SelectObject (&pen_yong);
						pDC->FillSolidRect (rt,pDoc->m_colorArray[0]);
						pDC->MoveTo (x-(int)(klineWidthPer),rt.top);
						pDC->LineTo (x-(int)(klineWidthPer),rt.bottom);
						pDC->LineTo (x+(int)(klineWidthPer),rt.bottom);
						pDC->LineTo (x+(int)(klineWidthPer),rt.top);
						pDC->LineTo (x-(int)(klineWidthPer),rt.top);

						pDC->MoveTo (x-(int)(klineWidthPer),openk);
						pDC->LineTo (x+(int)(klineWidthPer),openk);

						openYest = close;
					}
					else//draw red and green
					{
						rt.top =openkYest;
						rt.bottom =openk;

						pDC->FillSolidRect (rt,pDoc->m_colorArray[0]);
						pDC->SelectObject (&pen_yong);
						pDC->MoveTo (x-(int)(klineWidthPer),rt.top);
						pDC->LineTo (x-(int)(klineWidthPer),rt.bottom);
						pDC->LineTo (x+(int)(klineWidthPer),rt.bottom);
						pDC->LineTo (x+(int)(klineWidthPer),rt.top);
						pDC->LineTo (x-(int)(klineWidthPer),rt.top);

						rt.top =openkYest;
						rt.bottom =closek;
						rt.right +=1;

						pDC->FillSolidRect (rt,pDoc->m_colorArray[4]);

						openYest = closeYest;
						closeYest = close;
						//openYest = openYest;
					}
				}
			}
			else
			{
				if(close<=closeYest)//draw green
				{
					if(openk<=closek)
					{
						rt.top =openk;
						rt.bottom =closek;
					}
					else
					{
						rt.top =closek;
						rt.bottom =openk;
					}

					pDC->SelectObject (&pen_yin);

					pDC->MoveTo (x-(int)(klineWidthPer),openk);
					pDC->LineTo (x+(int)(klineWidthPer),openk);
					
					rt.right +=1;
					pDC->FillSolidRect (rt,pDoc->m_colorArray[4]);


					openYest = closeYest;
					closeYest = close;
				}
				else
				{
					if(close<=openYest)//draw green
					{
						if(openk<=closek)
						{
							rt.top =openk;
							rt.bottom =closek;
						}
						else
						{
							rt.top =closek;
							rt.bottom =openk;
						}

						pDC->SelectObject (&pen_yin);

						pDC->MoveTo (x-(int)(klineWidthPer),openk);
						pDC->LineTo (x+(int)(klineWidthPer),openk);

						rt.right +=1;
						pDC->FillSolidRect (rt,pDoc->m_colorArray[4]);

						openYest = close;
//						openYest = closeYest;
					}
					else//draw red and green
					{
						if(openkYest<=openk)
						{
							rt.top =openkYest;
							rt.bottom =openk;
						}
						else
						{
							rt.top =openk;
							rt.bottom =openkYest;
						}

						rt.right +=1;
						pDC->FillSolidRect (rt,pDoc->m_colorArray[4]);

						if(openkYest<=closek)
						{
							rt.top =openkYest;
							rt.bottom =closek;
						}
						else
						{
							rt.top =closek;
							rt.bottom =openkYest;
						}
						pDC->FillSolidRect (rt,pDoc->m_colorArray[0]);
						pDC->SelectObject (&pen_yong);
						pDC->MoveTo (x-(int)(klineWidthPer),rt.top);
						pDC->LineTo (x-(int)(klineWidthPer),rt.bottom);
						pDC->LineTo (x+(int)(klineWidthPer),rt.bottom);
						pDC->LineTo (x+(int)(klineWidthPer),rt.top);
						pDC->LineTo (x-(int)(klineWidthPer),rt.top);

						openYest = closeYest;
						closeYest = close;
//						openYest = closeYest;

					}
				}
			}
		}
		pDC->SelectObject (pOldBrush);
		pDC->SelectObject (pOldpen);

		return true;
}

void CTaiKlineShowKline::DrawBTXAdded(CDC *pDC, int nKln)
{
		int leftf=0;
		int topf=pView->m_heightCaption;
		int bottomf=pView->m_rtKlineFiguer.rtBlw[0].m_yBottom;
		int rightf=pView->m_rtKlineFiguer.rightX;

		if(bottomf-topf<2)
			return;

//		float cellWidth=(float)(rightf-leftf)/m_klinNumDefault;
		int wdth=rightf-leftf;
		int numberKline=m_klinNumDefault;
		///////////////////////////////////////////////////////////
		float widthPer =wdth/(float)(numberKline);// klineWidthPer + 2 * klineSide;//每格的宽度
		int klineWidthPer =(int) (widthPer*3/10);//k线的宽度

		float close;
		CPen pen_yong(PS_SOLID,1,pDoc->m_colorArray[7+nKln]);
		CPen* pOldpen=pDC->SelectObject(&pen_yong);

		int bAdd=0;
		int eAdd=0;
		bAdd=CaclBeginFoot(nKln);
		if(bAdd==-1)
		{
			pDC->SelectObject (pOldpen);
			return;
		}
		eAdd=CaclEndFoot(nKln);
		if(eAdd==-1)
		{
			pDC->SelectObject (pOldpen);
			return;
		}

		int j=m_footBegin;
		float moveVla=0;
		if(m_axisType==1)
			moveVla=m_pKlineAdd[nKln][bAdd].open-pView->m_pkline[m_footBegin].open;
		if(m_pKlineAdd[nKln][bAdd].open<=0)
		{
			pDC->SelectObject (pOldpen);
			return;
		}

		//BTX
		float closeYest = pView->m_pkline[0].close;//起步价
		float openYest = pView->m_pkline[0].close;
		if(bAdd>0)
		{
			if(m_axisType==1)
				closeYest = (m_pKlineAdd[nKln][bAdd-1].close-m_pKlineAdd[nKln][bAdd].open)*pView->m_pkline[m_footBegin].open/m_pKlineAdd[nKln][bAdd].open+pView->m_pkline[m_footBegin].open;
			else
				closeYest = m_pKlineAdd[nKln][bAdd-1].close;
			openYest = closeYest;
		}
		int begin0 = 0;
		if(bAdd == 0)
			begin0 = 1;

		for(int i=bAdd+begin0; i<=eAdd; i++ )
		{
			if( m_pKlineAdd[nKln][i].close<=0||m_pKlineAdd[nKln][i].high <= 0 )
			{
				if(m_axisType==2)
					break;
				continue;
			}

			if(m_axisType==1)
			{
				close = (m_pKlineAdd[nKln][i].close-m_pKlineAdd[nKln][bAdd].open)*pView->m_pkline[m_footBegin].open/m_pKlineAdd[nKln][bAdd].open+pView->m_pkline[m_footBegin].open;
			}
			else
			{
				close = m_pKlineAdd[nKln][i].close;
			}

//			if( m_pKlineAdd[nKln][i].high > 0 )
			int closek = (int)(YTransfer(close) );

			int openk = (int)(YTransfer(closeYest) );
			int openkYest = (int)(YTransfer(openYest) );


//				int flag=0;//0	yin;1	yong;2	ping
			CRect rt;
			rt.top =closek;
			rt.bottom =openk;

			//--确定与第一根K线的关系	old mistake
			int nRate = 1;
			if(pView->m_nKlineKind2 >=5)
				nRate = 60*60*24;
			if(m_pKlineAdd[nKln][i].day/nRate<pView->m_pkline[j].day/nRate)
				continue;
			if(m_pKlineAdd[nKln][i].day/nRate>pView->m_pkline[j].day/nRate)
			{
				while(m_pKlineAdd[nKln][i].day/nRate>pView->m_pkline[j].day/nRate&&j<m_footEnd)
				{
					j++;
				}
				if((m_pKlineAdd[nKln][i].day/nRate<pView->m_pkline[j].day/nRate))
					continue;
			}

			//-	画出

			int x= leftf +(int)( (j-m_footBegin+0.5) * (float)widthPer);
			rt.top =closek;
			rt.bottom =openk;
			if(klineWidthPer<1)
			{
				if( openYest <= closeYest )
				{
					if(close>=closeYest)//draw red
					{
						rt.top =closek;
						rt.bottom =openk;

//						pDC->SelectObject (&pen_yong);
						pDC->MoveTo (x ,rt.top);
						pDC->LineTo (x ,rt.bottom);
						openYest = closeYest;
						closeYest = close;
					}
					else
					{
						if(close>=openYest)//draw red
						{
							rt.top =closek;
							rt.bottom =openk;

//							pDC->SelectObject (&pen_yong);
							pDC->MoveTo (x ,rt.top);
							pDC->LineTo (x ,rt.bottom);

							openYest = close;
						}
						else//draw red and green
						{
							rt.top =openkYest;
							rt.bottom =openk;

//							pDC->SelectObject (&pen_yong);
							pDC->MoveTo (x ,rt.top);
							pDC->LineTo (x ,rt.bottom);

							rt.top =openkYest;
							rt.bottom =closek;
//							pDC->SelectObject (&pen_yong);
							pDC->MoveTo (x ,rt.top);
							pDC->LineTo (x ,rt.bottom);

							openYest = closeYest;
							closeYest = close;
						}
					}
				}
				else
				{
					if(close<=closeYest)//draw green
					{
//						pDC->SelectObject (&pen_yong);
						pDC->MoveTo (x ,openk);
						pDC->LineTo (x ,closek);

						openYest = closeYest;
						closeYest = close;
					}
					else
					{
						if(close<=openYest)//draw green
						{
//							pDC->SelectObject (&pen_yong);
							pDC->MoveTo (x ,openk);
							pDC->LineTo (x ,closek);
						
							openYest = close;
						}
						else//draw red and green
						{
//							pDC->SelectObject (&pen_yong);
							pDC->MoveTo (x ,openk);
							pDC->LineTo (x ,openkYest);

							rt.top =openkYest;
							rt.bottom =closek;
//							pDC->SelectObject (&pen_yong);
							pDC->MoveTo (x ,openkYest);
							pDC->LineTo (x ,closek);

							openYest = closeYest;
							closeYest = close;
						}
					}
				}

				continue;
			}

			rt.left=leftf -klineWidthPer +(int)(  (j-(m_footBegin)+0.5) * (float)widthPer);
			rt.right =leftf +klineWidthPer + (int)( (j-(m_footBegin)+0.5) * (float)widthPer);
			if( openYest <= closeYest )
			{
				if(close>=closeYest)//draw red
				{
					if(openk<=closek)
					{
						rt.top =openk;
						rt.bottom =closek;
					}
					else
					{
						rt.top =closek;
						rt.bottom =openk;
					}


					pDC->SelectObject (&pen_yong);
					pDC->FillSolidRect (rt,pDoc->m_colorArray[0]);
					pDC->MoveTo (x-(int)(klineWidthPer),rt.top);
					pDC->LineTo (x-(int)(klineWidthPer),rt.bottom);
					pDC->LineTo (x+(int)(klineWidthPer),rt.bottom);
					pDC->LineTo (x+(int)(klineWidthPer),rt.top);
					pDC->LineTo (x-(int)(klineWidthPer),rt.top);

					pDC->MoveTo (x-(int)(klineWidthPer),openk);
					pDC->LineTo (x+(int)(klineWidthPer),openk);

					openYest = closeYest;
					closeYest = close;
				}
				else
				{
					if(close>=openYest)//draw red
					{
						if(openk<=closek)
						{
							rt.top =openk;
							rt.bottom =closek;
						}
						else
						{
							rt.top =closek;
							rt.bottom =openk;
						}

						pDC->SelectObject (&pen_yong);
						pDC->FillSolidRect (rt,pDoc->m_colorArray[0]);
						pDC->MoveTo (x-(int)(klineWidthPer),rt.top);
						pDC->LineTo (x-(int)(klineWidthPer),rt.bottom);
						pDC->LineTo (x+(int)(klineWidthPer),rt.bottom);
						pDC->LineTo (x+(int)(klineWidthPer),rt.top);
						pDC->LineTo (x-(int)(klineWidthPer),rt.top);

						pDC->MoveTo (x-(int)(klineWidthPer),openk);
						pDC->LineTo (x+(int)(klineWidthPer),openk);

						openYest = close;
					}
					else//draw red and green
					{
						rt.top =openkYest;
						rt.bottom =openk;

						pDC->FillSolidRect (rt,pDoc->m_colorArray[0]);
						pDC->SelectObject (&pen_yong);
						pDC->MoveTo (x-(int)(klineWidthPer),rt.top);
						pDC->LineTo (x-(int)(klineWidthPer),rt.bottom);
						pDC->LineTo (x+(int)(klineWidthPer),rt.bottom);
						pDC->LineTo (x+(int)(klineWidthPer),rt.top);
						pDC->LineTo (x-(int)(klineWidthPer),rt.top);

						rt.top =openkYest;
						rt.bottom =closek;
						rt.right +=1;

						pDC->FillSolidRect (rt,pDoc->m_colorArray[7+nKln]);

						openYest = closeYest;
						closeYest = close;
						//openYest = openYest;
					}
				}
			}
			else
			{
				if(close<=closeYest)//draw green
				{
					if(openk<=closek)
					{
						rt.top =openk;
						rt.bottom =closek;
					}
					else
					{
						rt.top =closek;
						rt.bottom =openk;
					}

					pDC->SelectObject (&pen_yong);

					pDC->MoveTo (x-(int)(klineWidthPer),openk);
					pDC->LineTo (x+(int)(klineWidthPer),openk);
					
					rt.right +=1;
					pDC->FillSolidRect (rt,pDoc->m_colorArray[7+nKln]);


					openYest = closeYest;
					closeYest = close;
				}
				else
				{
					if(close<=openYest)//draw green
					{
						if(openk<=closek)
						{
							rt.top =openk;
							rt.bottom =closek;
						}
						else
						{
							rt.top =closek;
							rt.bottom =openk;
						}

						pDC->SelectObject (&pen_yong);

						pDC->MoveTo (x-(int)(klineWidthPer),openk);
						pDC->LineTo (x+(int)(klineWidthPer),openk);

						rt.right +=1;
						pDC->FillSolidRect (rt,pDoc->m_colorArray[7+nKln]);

						openYest = close;
	//						openYest = closeYest;
					}
					else//draw red and green
					{
						if(openkYest<=openk)
						{
							rt.top =openkYest;
							rt.bottom =openk;
						}
						else
						{
							rt.top =openk;
							rt.bottom =openkYest;
						}

						rt.right +=1;
						pDC->FillSolidRect (rt,pDoc->m_colorArray[7+nKln]);

						if(openkYest<=closek)
						{
							rt.top =openkYest;
							rt.bottom =closek;
						}
						else
						{
							rt.top =closek;
							rt.bottom =openkYest;
						}
						pDC->FillSolidRect (rt,pDoc->m_colorArray[0]);
						pDC->SelectObject (&pen_yong);
						pDC->MoveTo (x-(int)(klineWidthPer),rt.top);
						pDC->LineTo (x-(int)(klineWidthPer),rt.bottom);
						pDC->LineTo (x+(int)(klineWidthPer),rt.bottom);
						pDC->LineTo (x+(int)(klineWidthPer),rt.top);
						pDC->LineTo (x-(int)(klineWidthPer),rt.top);

						openYest = closeYest;
						closeYest = close;
					}
				}
			}
			j++;

		}

		pDC->SelectObject (pOldpen);
}

bool CTaiKlineShowKline::ValidDoKline(bool bMakeUp)
{
	if(bMakeUp==false)
	{
		if(pView->m_nCountKline<=0) return false;
		if(this->m_footCurrent<0 || m_footCurrent>=pView->m_nCountKline) m_footCurrent=0;
		if(m_footBegin<0 || m_footBegin>=pView->m_nCountKline ) return false;
		if(m_footEnd<0 || m_footEnd>=pView->m_nCountKline ) return false;
	}
	else
	{
		if(this->m_footCurrent<0 || m_footCurrent>=pView->m_nCountKline) m_footCurrent=0;
		if(m_footBegin<0 || m_footBegin>=pView->m_nCountKline ) m_footBegin=0;
		if(m_footEnd<0 || m_footEnd>=pView->m_nCountKline ) m_footEnd=0;
		if(pView->m_nCountKline<=0) return false;
	}
	return true;
}

void CTaiKlineShowKline::DrawIndicatorAdded(CDC *pDC)
{

}

CString CTaiKlineShowKline::GetParamString(CString sName)
{
	CString paras = "";
	CFormularContent*	pIndex;
	pIndex=(CFormularContent*)pDoc->LookUpArray (pDoc->m_formuar_index,sName); 
	if(pIndex==NULL)
	{
		return paras;
	}

	int num ;
	num=pIndex->numPara  ;

	paras="(";
	for(int i=0;i<num;i++)
	{
		CString tmp = CLongString::FloatToString(CFormularContent::GetParamDataEach(i,pView->m_nKlineKind2 , pIndex));//CFormularContent::GetParamDataEach(i,m_nKindKline, pIndex);//
		tmp+=",";
		paras+=tmp;
	}
	paras.TrimRight(",");
	paras+=")";//pView->m_heightCaption

	return paras;

}

void CTaiKlineShowKline::SetRectCurrent(int nFiguer)
{
	m_rectDrawLine.left=0;
	m_rectDrawLine.right=pView->m_rtKlineFiguer.rightX;
	m_rectDrawLine.bottom=pView->m_rtKlineFiguer.rtBlw[nFiguer].m_yBottom-3;
	if(nFiguer ==0)
	{
		m_rectDrawLine.top=pView->m_heightCaption;
	}
	else
	{
		m_rectDrawLine.top=pView->m_rtKlineFiguer.rtBlw[nFiguer-1].m_yBottom+pView->m_heightCaption;
	}

}
void TransparentBlt(HDC hdc,HBITMAP hBitmap,short xStart,short yStart,COLORREF cTransparentColor)
{   
	int mWidth,mHeight;

	BITMAP           bm;
	GetObject(hBitmap,sizeof(BITMAP),&bm);
	mWidth = bm.bmWidth ;
	mHeight = bm.bmHeight ;


	HDC hMemDC = NULL;
	hMemDC = ::CreateCompatibleDC (hdc);

	HBITMAP pMaskBmp = NULL;
	pMaskBmp = ::CreateBitmap (mWidth, mHeight, 1, 1, NULL);
	HBITMAP pOldBmp = (HBITMAP)::SelectObject (hMemDC, pMaskBmp);

//	COLORREF clrOldBk = ::GetBkColor (hdc);
	HBITMAP pOldDCBmp = (HBITMAP)::SelectObject (hdc, hBitmap);
	COLORREF clrOldBk = ::SetBkColor (hdc, cTransparentColor);
	::BitBlt (hMemDC, 0, 0, mWidth, mHeight, hdc, 0, 0, SRCCOPY);
	::SelectObject (hdc, pOldDCBmp);
	::SetBkColor (hdc, clrOldBk);

	::SelectObject (hMemDC, hBitmap);
	::BitBlt(hdc,xStart, yStart, mWidth, mHeight, hMemDC, 0, 0, SRCINVERT);
	::SelectObject (hMemDC, pMaskBmp);
	::BitBlt(hdc,xStart, yStart, mWidth, mHeight, hMemDC,  0,  0, SRCAND);
	::SelectObject (hMemDC, hBitmap);
	::BitBlt(hdc,xStart, yStart, mWidth, mHeight, hMemDC, 0, 0, SRCINVERT);


	::SelectObject (hMemDC, pOldBmp);
	::DeleteObject (pMaskBmp);
	::DeleteDC (hMemDC);

}

void CTaiKlineShowKline::DrawTextIconLine(CDC *pDC,int i)
{
	COLORREF	clr;

	if(m_dataFormular[m_nSon].line[i].clr!= -1)
		clr=m_dataFormular[m_nSon].line[i].clr;
	else
		clr=pDoc->m_colorArray[i%6+7];

	int nStyle=PS_SOLID;// {PS_SOLID,PS_DOT,PS_DASHDOTDOT};
	if(m_dataFormular[m_nSon].line[i].lineThick>=100)
		nStyle=PS_DOT;
	CPen pen(nStyle,m_dataFormular[m_nSon].line[i].lineThick%100,clr);
	CPen* pOldpen=pDC->SelectObject(&pen);

	float cellWidth=(float)(m_rectDrawLine.right-m_rectDrawLine.left)/(m_klinNumDefault);
	if(m_dataFormular[m_nSon].line[i].m_arrBE.kind == ARRAY_BE::DrawLine)
	{
		bool b = true;
		if(m_dataFormular[m_nSon].line[i].m_arrBE.s == "0")
		{
			b = false;
		}

		int n = m_dataFormular[m_nSon].line[i].m_arrBE.looseArr.GetSize();
		int x = 0;
		for(int j = 0;j<n-1;j+=2)
		{
			if(m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j+1].nFoot>=this->m_footBegin
				&& m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j+1].nFoot<=this->m_footEnd
				||m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].nFoot>=this->m_footBegin
				&& m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].nFoot<=this->m_footEnd )
			{

			x = (int)(m_rectDrawLine.left+(cellWidth+1)/2+(m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].nFoot-m_footBegin)*cellWidth);
			float y9 = (float)m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].fVal;
			float ff = YTransfer(y9);
			pDC->MoveTo(x,ff);
			if(!b)
			{
				pDC->Ellipse (x-1,ff-1,x+1,ff+1);
			}

			if(b == true)
			{
				int x2= (int)(m_rectDrawLine.left+(cellWidth+1)/2
					+(m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j+1].nFoot-m_footBegin)*cellWidth);
				float y2 = (float)m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j+1].fVal;

				if(y2 == y9)
				{
					x = m_rectDrawLine.right;
					y9 = y2;
				}
				else
				{
					float f2 = (x2-x)/(y2-y9);
					if(y2>y9)
					{
						x = f2*(m_max_sun[m_nSon]-y9)+x;
						y9 = m_max_sun[m_nSon];
					}
					else
					{
						x = f2*(m_min_sun[m_nSon]-y9)+x;
						y9 = m_min_sun[m_nSon];
					}
				}

			}
			else
			{
				x= (int)(m_rectDrawLine.left+(cellWidth+1)/2
					+(m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j+1].nFoot-m_footBegin)*cellWidth);
				y9 = (float)m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j+1].fVal;
			}
			ff = YTransfer(y9);
			pDC->LineTo(x,	ff);
			if(!b)
			{
				pDC->Ellipse (x-1,ff-1,x+1,ff+1);
			}

			}
		
		}
	}

	if(m_dataFormular[m_nSon].line[i].m_arrBE.kind == ARRAY_BE::DrawPolyLine)
	{
		int n = m_dataFormular[m_nSon].line[i].m_arrBE.looseArr.GetSize();
		int x = 0;
		bool b = false;
		for(int j = 0;j<n-1;j++)
		{
			if(m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j+1].nFoot< this->m_footBegin
				&& m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].nFoot<this->m_footBegin
				||m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j+1].nFoot>this->m_footEnd
				&& m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].nFoot>this->m_footEnd )
				continue;
			{

				float y9;
				float ff;
				if(b == false)
				{
					x = (int)(m_rectDrawLine.left+(cellWidth+1)/2+(m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].nFoot-m_footBegin)*cellWidth);
					y9 = (float)m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].fVal;
					ff = YTransfer(y9);
					pDC->MoveTo(x,ff);
					b = true;
				}

				{
					x= (int)(m_rectDrawLine.left+(cellWidth+1)/2
						+(m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j+1].nFoot-m_footBegin)*cellWidth);
					y9 = (float)m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j+1].fVal;
				}
				ff = YTransfer(y9);
				pDC->LineTo(x,	ff);

			}
		
		}
	}

	if(m_dataFormular[m_nSon].line[i].m_arrBE.kind == ARRAY_BE::DrawIcon)
	{
		CBitmap       m_bitmap;

		CImageList imagelist;
		imagelist.Create(15,15, ILC_MASK, 1, 1);

		int nIDBit[] = {IDB_DRAWICON1,IDB_DRAWICON2,IDB_DRAWICON3,IDB_DRAWICON4,IDB_DRAWICON5};

		CString sName = m_dataFormular[m_nSon].line[i].m_arrBE.s;
		int nItem = atoi(sName);
		if(nItem<=0) nItem = 1;
		if(nItem>5) nItem = 5;

		m_bitmap.LoadBitmap(nIDBit[nItem-1]);
		imagelist.Add(&m_bitmap, (COLORREF)0x00ffFFFF);
		m_bitmap.DeleteObject();

		{
		int n = m_dataFormular[m_nSon].line[i].m_arrBE.looseArr.GetSize();
		for(int j = 0;j<n;j++)
		{

			int x= (int)(m_rectDrawLine.left+(cellWidth+1)/2
				+(m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].nFoot-m_footBegin)*cellWidth);
			float y9 = (float)m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].fVal;
		
 
			// Paint the image.
			POINT p;
			p.x = x-7;
			p.y = 	YTransfer(y9);

			imagelist.Draw (pDC ,0,p,ILD_TRANSPARENT);

		}
		}
	}

	if(m_dataFormular[m_nSon].line[i].m_arrBE.kind == ARRAY_BE::StringData)
	{
		int n = m_dataFormular[m_nSon].line[i].m_arrBE.looseArr.GetSize();
		for(int j = 0;j<n;j++)
		{
			int x= (int)(m_rectDrawLine.left+(cellWidth+1)/2
				+(m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].nFoot-m_footBegin)*cellWidth);
			float y9 = (float)m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].fVal;

			pDC->SetTextAlign( TA_LEFT );//||
			pDC->SetTextColor( clr);
			pDC->SetBkMode(TRANSPARENT);
			int nh =(pDC->GetOutputTextExtent(m_dataFormular[m_nSon].line[i].m_arrBE.s)).cy;
			CFont ft;
			LOGFONT lgf=pDoc->m_fontstr[0];
			ft.CreateFontIndirect (&lgf );
			CFont* pOld=pDC->SelectObject(&ft);
			pDC->TextOut (x+2,YTransfer(y9)-nh/2,m_dataFormular[m_nSon].line[i].m_arrBE.s);
			pDC->SelectObject(pOld);
		
		}
	}
	
	if(m_dataFormular[m_nSon].line[i].m_arrBE.kind == ARRAY_BE::DrawStickLine && m_dataFormular[m_nSon].line[i].m_arrBE.lineWidth)
	{
		int n = m_dataFormular[m_nSon].line[i].m_arrBE.looseArr.GetSize();
		int x = 0;
		float cellWidth=(float)(m_rectDrawLine.right-m_rectDrawLine.left)/(m_klinNumDefault);
		CString ss = m_dataFormular[m_nSon].line[i].m_arrBE.s;
		int empty = 0;
		float width = 1;
		if(ss.GetLength ()>=3)
		{
			empty = atoi(ss.Right (1));
			ss = ss.Left (ss.GetLength ()-2);
			width = atof(ss);
		}


		//to compute max,min value of width
/*		float fMaxWidth = m_dataFormular[m_nSon].line[i].m_arrBE.lineWidth[m_footBegin] ;
		float fMinWidth = fMaxWidth;
		for(int j = m_footBegin+1;j<m_footEnd;j++)
		{
			if(fMaxWidth<m_dataFormular[m_nSon].line[i].m_arrBE.lineWidth[m_footBegin])
				fMaxWidth = m_dataFormular[m_nSon].line[i].m_arrBE.lineWidth[m_footBegin];
			if(fMinWidth>m_dataFormular[m_nSon].line[i].m_arrBE.lineWidth[m_footBegin])
				fMaxWidth = m_dataFormular[m_nSon].line[i].m_arrBE.lineWidth[m_footBegin];
		}*/

		for(int j = 0;j<n;j++)
		{
			int nFoot = m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].nFoot;
			if( nFoot<this->m_footBegin
				|| nFoot>this->m_footEnd )
				continue;

			width = m_dataFormular[m_nSon].line[i].m_arrBE.lineWidth[nFoot]*10 ;
			if(width>2000)
				width = 2000;
			if(width<10)
				width = 10;
			int klineWidthPer = (int)ceil(cellWidth*width/100);//
			if(klineWidthPer>2&&klineWidthPer%2==0)
				klineWidthPer++;
			float klineSide =  (int)ceil(cellWidth*(100-width)/200);
			if(klineWidthPer<2)
			{
				klineWidthPer = 1;
				klineSide = cellWidth/2;
			}
			{

				x= m_rectDrawLine.left +klineSide+( (nFoot-(m_footBegin)) * (float)cellWidth);
				int x2 = x+klineWidthPer-1;

				float y9;
				int n1;
				y9 = (float)m_dataFormular[m_nSon].line[i].m_arrBE.looseArr[j].fVal;
				n1 = YTransfer(y9);

				y9 = (float)m_dataFormular[m_nSon].line[i].m_arrBE.line[nFoot];
				int n2  = YTransfer(y9);
				if(n1>n2)
				{
					int nn = n1;
					n1 = n2;
					n2 = nn;
				}

				CRect rt(x,n1,x2,n2);
				if(!empty)
					pDC->FillSolidRect (rt,clr);
				pDC->MoveTo (rt.left,rt.top);//bug
				pDC->LineTo (rt.right,rt.top);//bug
				pDC->LineTo (rt.right,rt.bottom-1);//bug
				pDC->LineTo (rt.left,rt.bottom-1);//bug
				pDC->LineTo (rt.left,rt.top);//bug
			}
		
		}
	}


	pDC->SelectObject(pOldpen);

}

void CTaiKlineShowKline::GetRect(CRect &r, int nFiguer)
{
	SetRectCurrent( nFiguer);
	r = this->m_rectDrawLine ;

}
