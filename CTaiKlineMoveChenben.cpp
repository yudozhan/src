// CTaiKlineMoveChenben.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineMoveChenben.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiShanDoc.h"
#include "CTaiScreenParent.h"
#include "CTaiKlineFileHS.h"
#include "CTaiKlineFileKLine.h"//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAXMOVECHENBEN	200000
/////////////////////////////////////////////////////////////////////////////
// CTaiKlineMoveChenben

IMPLEMENT_DYNCREATE(CTaiKlineMoveChenben, CObject)


CTaiKlineMoveChenben::CTaiKlineMoveChenben()
{
}
CTaiKlineMoveChenben::CTaiKlineMoveChenben(CTaiKlineShowKline* pDoKline,CTaiShanKlineShowView* pViewIn,CTaiShanDoc* pDocIn)
{
	pDoc = pDocIn;
	pView = pViewIn;
	pDoK = pDoKline;
	m_pKlineOther=NULL; 
	m_numkline=0;
	m_bDayKline = false;
	m_symbol="";
	m_stkKind = 0;

	ReadInSettings();

	m_nShowKind = 0;
}

CTaiKlineMoveChenben::~CTaiKlineMoveChenben()
{
	if(m_pKlineOther)
		delete []m_pKlineOther;

//	WriteOutSettings();
}



/////////////////////////////////////////////////////////////////////////////
#define RATE_EXTEND 0.98
void CTaiKlineMoveChenben::ShowMoveCB(CDC *pDC,int nShowVirtualLine)
{
	CRect rt;pView->GetCurrClientRect(rt);//pView->m_infoFiguer !=1||
	if(pView->m_nKlineKind2 ==0 || (pView->m_nKlineKind2>5)||nShowVirtualLine>0&&(pView->m_nKlineKind2!=5))
	{
		CString s="";
		CString s2="";
		if(nShowVirtualLine==0)
		{
			s = "只能在日K线";
			s2 = "及5分钟K线周期显示" ;
		}
		else if(pView->m_nKlineKind2!=5)
		{
			s = "只能在日K线周期显示" ;
		}
		CFont ft;
		LOGFONT lgf=pDoc->m_fontstr[0];
		ft.CreateFontIndirect (&lgf );
		CFont* pOld=pDC->SelectObject(&ft);
		pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		pDC->SetTextColor( pDoc->m_colorArray[1]);
		pDC->SetBkColor( pDoc->m_colorArray[0]);
		pDC->SetBkMode(TRANSPARENT   );


		{
			int top = 10;
			int left = rt.right-pView->m_rightLength+5;
			pDC->TextOut(left,top,s);
			int nH = (pDC->GetOutputTextExtent(s)).cy;
			if(s2!="")
				pDC->TextOut(left,top+nH+2,s2);
		}
		pDC->SelectObject(pOld);
		return;
	}
	if(nShowVirtualLine >0 && nShowVirtualLine!=3)
	{
		ShowVol(pDC, nShowVirtualLine);
		return;
	}

	
	CReportData* pdt = NULL;
	if(!pDoc->m_sharesInformation .Lookup (pView->m_sharesSymbol.GetBuffer(0),pdt,pView->m_stkKind ))
		return;
	if(pdt->pBaseInfo==NULL)
		return;
	if(CTaiShanKlineShowView::GetCapital(pdt)<=0)//
		return ;
	if(pView->m_nCountKline <=0)
		return;
	if (nShowVirtualLine==3)
	{
		ShowCommerceCB(pDC,3);
		return;
	}
	int end[2]={-1,-1};
	int begin[2]={0,0};//=m_footChenBenB;
	Kline* pKline[2]={NULL,NULL};
	int totK;
	bool bMin5=pDoc ->m_propertyInitiate.bUseMin5ChenBen;

	int nFootCurrent = pDoK->m_footCurrent;
	if(nFootCurrent>=pView->m_nCountKline )
	{
		nFootCurrent=pView->m_nCountKline-1;
	}
//	if(nShowVirtualLine >0)
//		nFootCurrent=pView->m_nCountKline-1;

	pKline[0]=pView->m_pkline;
	end[0]=nFootCurrent ;

	m_symbol = pView->m_sharesSymbol ;
	m_stkKind = pView->m_stkKind ;

	pDoK->m_nSon=0;
	pDoK->SetRectCurrent(0);

	//compute max and min 
	float maxK =0.01,minK=90000000;
	int i;
	for(int j=0;j<2;j++)
		for( i=0;i<=end[j];i++)
		{
			if(pKline[j]!=NULL)
			{
				if((pKline[j]+i)->high >maxK)
				{
					maxK=(pKline[j]+i)->high;
				}
				if((pKline[j]+i)->low<minK)
				{
					minK=(pKline[j]+i)->low;
				}
			}
		}

//	int nLenData = end[0]-begin[0]+1;//1800;
//	if(end[0]-begin[0]>1800) nLenData = 1800;//begin[0] = end[0]-1800;
	if(maxK>MAXMOVECHENBEN) 
		maxK = MAXMOVECHENBEN;
	if(minK<0.001) minK = 0.001;

	//set memory of data,a value a fen
	float fRuler;
	int n =CTaiKlineMoveChenben::GetRuler( maxK,  minK, fRuler);

	int nBeginY =minK*fRuler;
	int nEndY = maxK*fRuler;

	//--- 计算移动成本
//	int n=nEndY - nBeginY+5;
	if(n<=0)
		return ;
	float* cnp=new  float[n];
	memset(cnp,0,n*4);

	float max=0;
	float volume=0;
	int nVol=0;
	float ltp = CTaiShanKlineShowView::GetCapital(pdt);

	int nFootBegin=0;
	int nFootEnd=0;

	float fAdd = 0.5;
	if(fRuler<100)
		fAdd = 1.5;

	float ltp3 = ltp;
	if(pView->m_sharesSymbol [1] == 'A')
		ltp3 = ltp/100;
	for(int k=0;k<2;k++)
	{
		if(pKline[k]==NULL)
			continue;

		//lmb add

		float fBeginLtp ;
		//end

		for(i=begin[k];i<=end[k];i++)
		{
			int foot;

			if(pKline[k][i].high>maxK) 
				continue;

			if(pKline[k][i].vol>0 )
			{
				int nLow = (pKline[k][i].low*fRuler+0.5);
				int nFen =( pKline[k][i].high- pKline[k][i].low)*fRuler+fAdd;
				if(nFen<=0)
					continue;
				ASSERT(nLow>=nBeginY);
				nLow-=nBeginY;

				float fvol = pKline[k][i].vol/nFen;
				if(i==begin[k]&&k==0)
					fvol = ltp3/nFen;
				//step 1:delete yestoday
				float rat = 1-pKline[k][i].vol/ltp3;
				if(rat<0)
					rat = 0;
				if(rat>=1)
					continue;

				int j;
				for(j=0;j<n;j++)
				{
					cnp[j]*=rat;
				}
				//step 2:add today
				for( j=0;j<nFen;j++)
				{
					cnp[j+nLow]+=fvol;
				}
			}
		}
	}


	//normalizing
	for(i=0;i<n;i++)
		volume+=cnp[i];
	if(volume<=0)
	{
		delete[] cnp;
		return;
	}
	volume=ltp3/volume;//rate now;
	int nFootMax = 0;
	float amount=0;
	for(i=0;i<n;i++)
	{
		cnp[i]*=volume;
//		ASSERT(cnp[i]>=0);
		amount+=(cnp[i]*(i+nBeginY)/(float)fRuler);
		if(cnp[i]>max)
		{
			nFootMax = i;
			max=cnp[i];
		}
	}

	//current page data
	float maxCurr = 0;
	int bottomMin = pDoK->YTransfer((int)0)*fRuler-nBeginY;//pDoK->m_max_sun[0]
	int topMax = pDoK->YTransfer((int)rt.bottom-50)*fRuler-nBeginY;//pDoK->m_min_sun[0]
	if(bottomMin>n-1 )
		 bottomMin=  n-1 ;
	if(topMax<0)
		 topMax= 0;

	for(i = topMax; i<=bottomMin;i++)
	{
		if(cnp[i]>maxCurr)
			maxCurr=cnp[i];
	}

	if(max<=0 ||maxCurr<=0)
	{
		delete[] cnp;
		return;
	}

	//
	float yGap=1/fRuler*(pDoK->m_rectDrawLine.bottom-pDoK->m_rectDrawLine.top)/(pDoK->m_max_sun[0]-pDoK->m_min_sun[0]);//pDoK->YTransfer((float)pDoK->m_min_sun[0])-pDoK->YTransfer((float)(pDoK->m_min_sun[0]+0.01));
	if(yGap<=0 || pView->m_pkline [nFootCurrent].close>maxK)
	{
		delete[] cnp;
		return;
	}

	//mouse point value
	CPoint pp;
	::GetCursorPos (&pp);
	this->pView ->ScreenToClient(&pp);
	int mousePos = pDoK->YTransfer((int)(pp.y ))*fRuler-nBeginY;//pDoK->m_max_sun[0]
	if(mousePos>=n) mousePos = n-1;
	if(mousePos<0) mousePos = 0;
	float fMouseVal = 0;
	for(i=0;i<=mousePos;i++)
	{
		fMouseVal+=cnp[i];
	}

	int nColor = 1;
	//statistics
	{
		volume = 0;
		int nClose = pView->m_pkline [nFootCurrent].close*fRuler-nBeginY;
		if(nClose>=n) nClose = n-1;
		for(i=0;i<=nClose;i++)
		{
			volume+=cnp[i];
		}
		if(volume/ltp3>0.95)
			nColor = 4;
		else if(volume/ltp3<0.05)
			nColor = 3;
	}


	//---	画线

	CPen pen_line(PS_SOLID  ,1,pDoc->m_colorArray[nColor]);//PS_DASHDOTDOT   
	CPen* pOldpen=pDC->SelectObject(&pen_line);
	pDC->SetBkMode(OPAQUE);

	if(nShowVirtualLine == 0)
	{
		int nTop = pDoK->YTransfer((float)(topMax+nBeginY)/(float)fRuler);
		for(i=0;i<bottomMin - topMax ;i ++ )
		{
			pDC->MoveTo(rt.right,nTop-i*yGap);
			pDC->LineTo(rt.right-(int)(RATE_EXTEND * cnp[topMax+i]*(pView->m_rightLength)/maxCurr),nTop-i*yGap);
		}
		pDC->MoveTo(rt.right-pView->m_rightLength+1,pp.y );
		pDC->LineTo(rt.right-pView->m_rightLength+1+20,pp.y);
	}
	else if(pView->m_nKlineKind2 == 5)
	{
		//to add line about fen jia
		//READ HS DATA FROM FILE;
		int nMarket = CSharesCompute::GetMarketKind(m_stkKind );
		CString sPath = FILE_PATH_HSH_SH;
		if(nMarket == SZ_MARKET_EX ) 
		{
			sPath = FILE_PATH_HSH_SZ;
		}
		CBuySellList buySellList;
		CBuySellList* bsList=NULL;
		if(nFootCurrent == pView->m_nCountKline -1) 
			bsList = &pView->m_hs;
		else
		{
			CTaiKlineFileHS fileHs(nMarket);
			CString m_fileName=CTime(pView->m_pkline [nFootCurrent].day).Format("%Y%m%d");
			CFileFind find;
			CString filename =sPath + m_fileName +".hst";
			if(find.FindFile(filename)) 
				if(m_fileName!=_T(""))
				{//open file
					if(fileHs.Open (filename,0))
					{
						fileHs.ReadHS2(m_symbol,buySellList,false);
						bsList = &buySellList;
					}
				}
		}
		
		//统计
		CFJList	fenjiaUp;//分价数据的链表
		CFJList	fenjiaDown;//分价数据的链表
		if(bsList!=NULL)
			CTaiKlineMin1::GetFenJia(0,*bsList,fenjiaUp,fenjiaDown,nShowVirtualLine-1);
		CTaiKlineFileHS::RemoveHs(buySellList);

		//max value
		float maxVol = 0;
		POSITION pos = fenjiaUp.GetHeadPosition ();
		float fAllUp = 0;
		float fAllDown = 0;
		while(pos)
		{
			float f = fenjiaUp.GetNext (pos)->vol ;
			fAllUp+=f;
			if(f>maxVol ) maxVol = f;
		}
		if(nShowVirtualLine == 2)
		{
			pos = fenjiaDown.GetHeadPosition ();
			while(pos)
			{
				float f = fenjiaDown.GetNext (pos)->vol ;
				fAllDown+=f;
				if(f>maxVol ) maxVol = f;
			}
		}

		//draw now
		if(maxVol>0 &&pView->m_pkline [nFootCurrent].high<=maxK && (pView->m_pkline [nFootCurrent].low)>=minK)
		{
			int nTop = pDoK->YTransfer((pView->m_pkline [nFootCurrent].high));
			int nBott = pDoK->YTransfer((pView->m_pkline [nFootCurrent].low));
			int nMid = rt.right-(pView->m_rightLength)/2;
			if(nShowVirtualLine == 1) nMid = rt.right-1;
			pDC->MoveTo(nMid,nTop);
			pDC->LineTo(nMid,nBott);
			pos = fenjiaUp.GetHeadPosition ();
			int nMax = (pView->m_rightLength)-5;
			if(nShowVirtualLine == 2) nMax =  (pView->m_rightLength)/2-2;
			CPen pen2(PS_SOLID  ,1,pDoc->m_colorArray[3]);//PS_DASHDOTDOT   
			pDC->SelectObject (&pen2);
			while(pos)
			{
				FENJIA* phs = fenjiaUp.GetNext (pos);
				int ny = pDoK->YTransfer(phs->price );
				if(ny>nBott || ny<nTop) continue;
				pDC->MoveTo (nMid-nMax*phs->vol /maxVol,ny);
				pDC->LineTo (nMid,ny);
			}
			CPen pen3(PS_SOLID  ,1,pDoc->m_colorArray[4]);//PS_DASHDOTDOT   
			pDC->SelectObject (&pen3);
			if(nShowVirtualLine == 2)
			{
				pos = fenjiaDown.GetHeadPosition ();
				while(pos)
				{
					FENJIA* phs = fenjiaDown.GetNext (pos);
					int ny = pDoK->YTransfer(phs->price );
					if(ny>nBott || ny<nTop) continue;
					pDC->MoveTo (nMid+nMax*phs->vol /maxVol,ny);
					pDC->LineTo (nMid,ny);
				}
			}
			pDC->SelectObject(pOldpen);

			pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
			pDC->SetTextColor( pDoc->m_colorArray[1]);
			pDC->SetBkColor( pDoc->m_colorArray[0]);

			CFont ft;
			LOGFONT lgf=pDoc->m_fontstr[0];
			ft.CreateFontIndirect (&lgf );
			CFont* pOld=pDC->SelectObject(&ft);

			{
				CString s = "成交量分布";
				if(nShowVirtualLine == 2)
					s="外盘";
				int len=(pDC->GetOutputTextExtent(s)).cx+2;
				CString s2;
				s2.Format ("%d ",(int)fAllUp);
				int len2=(pDC->GetOutputTextExtent(s2)).cx+2;
				int hgh=(pDC->GetOutputTextExtent(s)).cy+2;
				pDC->TextOut( nMid - len ,nBott,s);
				pDC->TextOut( nMid - len2 ,nBott+hgh,s2);

				if(nShowVirtualLine == 2)
				{
					s="内盘";
					pDC->TextOut( nMid  ,nBott,s);
					s2.Format (" %d",(int)fAllDown);
					pDC->TextOut( nMid  ,nBott+hgh,s2);
				}

			}
			pDC->SelectObject(pOld);
		}

		//remove
		for(;fenjiaUp.IsEmpty()==0;)
			delete fenjiaUp.RemoveHead();
		fenjiaUp.RemoveAll();
		for(;fenjiaDown.IsEmpty()==0;)
			delete fenjiaDown.RemoveHead();
		fenjiaDown.RemoveAll();



		CPen pen(PS_DOT  ,1,pDoc->m_colorArray[nColor]);//PS_DASHDOTDOT   
		pDC->SelectObject(&pen);
		int nTop = pDoK->YTransfer((float)(topMax+nBeginY)/(float)fRuler);
		pDC->MoveTo(rt.right-(int)(RATE_EXTEND*cnp[topMax]*(pView->m_rightLength)/maxCurr),nTop);
		for(i=0;i<bottomMin - topMax ;i ++ )
			pDC->LineTo(rt.right-(int)(RATE_EXTEND*cnp[topMax+i]*(pView->m_rightLength)/maxCurr),nTop-i*yGap);
		pDC->SelectObject(pOldpen);

	}
	//
	pDC->SelectObject(pOldpen);
	//close line
	if(nShowVirtualLine==0) 
	{
		int color[2]={3,4};
		if(nColor>2)
		{
			color[0] =1;color[1]=1;
		}
		float f[2];
		f[0]=(float)pView->m_pkline [nFootCurrent].close;
		if(f[0]<minK) f[0] = minK;
		if(f[0]>maxK) f[0] = maxK;
		f[1]=amount/ltp3;
		if(f[1]<minK) f[1] = minK;
		if(f[1]>maxK) f[1] = maxK;

		for(int j=0;j<2;j++)
		{
			if(f[j]>=MAXMOVECHENBEN) continue;
			CPen pen_line2(PS_SOLID  ,2,pDoc->m_colorArray[color[j]]);//PS_DASHDOTDOT   
			pOldpen=pDC->SelectObject(&pen_line2);
			{
				int nTop = pDoK->YTransfer(f[j]);
				pDC->MoveTo(rt.right,nTop);
				pDC->LineTo(rt.right-(int)(RATE_EXTEND* cnp[(int)(f[j]*fRuler)-nBeginY]*(pView->m_rightLength)/maxCurr),nTop);
			}
			pDC->SelectObject(pOldpen);
		}
	}


	//statistics
	{
		pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		pDC->SetTextColor( pDoc->m_colorArray[1]);
		pDC->SetBkColor( pDoc->m_colorArray[0]);

		CFont ft;
		LOGFONT lgf=pDoc->m_fontstr[0];
		ft.CreateFontIndirect (&lgf );
		CFont* pOld=pDC->SelectObject(&ft);

		{

			CString s;
			s.Format ("最大量:%d(%d",(int)max,(int)(max/ltp3*100));
			s+="%)(手/分钱)";
			int len=(pDC->GetOutputTextExtent(s)).cy+2;
			int top = rt.bottom-30-len*5;
			int left = rt.right-pView->m_rightLength+1;
//			pDC->TextOut(left,top,s);
			s.Format ("最大量处价:%.2f",(float)(nFootMax+nBeginY)/fRuler);
//			pDC->TextOut(left,top+1*len,s);
			s.Format ("套牢盘:%d(%d",(int)(ltp3-volume),(int)((ltp3-volume)/ltp3*100+0.5));
			s+="%)";
			pDC->TextOut(left,top+2*len,s);
			s.Format ("获利盘:%d(%d",(int)(volume),(int)(volume/ltp3*100+0.5));
			s+="%)";
			pDC->TextOut(left,top+3*len,s);
			s.Format ("流通盘平均成本:%.2f",amount/ltp3);
			pDC->TextOut(left,top+4*len,s);

			s.Format ("%d",(int)((ltp3-fMouseVal)/ltp3*100+0.5));
			s+="%";
			len=(pDC->GetOutputTextExtent(s)).cy+2;
			pDC->TextOut(left,pp.y-len,s);
			s.Format ("%d",(int)((fMouseVal)/ltp3*100+0.5));
			s+="%";
			pDC->TextOut(left,pp.y+2,s);
			

		}
		pDC->SelectObject(pOld);
	}
	delete[] cnp;
	return ;
//add
}
#define NORMALIZING(x) ((int)(x*1000+0.5))
void CTaiKlineMoveChenben::BuySellFiguer(CDC *pDC)
{
	if(pView->m_infoFiguer !=1)
		return;
	
	CReportData* pdt = NULL;
	if(!pDoc->m_sharesInformation .Lookup (pView->m_sharesSymbol.GetBuffer(0),pdt,pView->m_stkKind ))
		return;

	CReportData dt;
	CBuySellList* pBuySell = &pView->m_hs;
	int nTotal = pView->m_hs.GetCount();
	int nCurrent=pDoK->m_footCurrent;//nTotal-1;

	if(pView->m_nKlineKind2==0||pView->m_nKlineKind2==MIN1_KLINE)
	{
		nTotal = 240;
		pBuySell = &pView->pMin1Drawing->m_hsMin1 ;
		if(pView->m_nKlineKind2==0)
			nCurrent = pView->pMin1Drawing->m_footCurrent ;
	}
	else if(pView->m_nKlineKind2!=HS_KLINE)
	{
		nCurrent=nTotal-1;
	}
	if(nTotal<=0)
		return;
	if(nCurrent<0)
		nCurrent=0;
	//get current buy sell info
//	static void Cdat1ToHs(CReportData* pCdat,TRADE_DETAIL_H_PER* pHs,bool bToHs = true, TRADE_DETAIL_H_PER *pHsPre = NULL);
	TRADE_DETAIL_H_PER* pHs=CTaiKlineFileHS::GetAtBS (pBuySell ,nCurrent,nTotal);
	CTime tm(pHs->time );
	CTaiKlineFileHS::Cdat1ToHs(&dt,pHs,false);

	//add history info outside today's price
	CMap<int,int,float,float> fMap;//
	POSITION  pos;
	int high ;
	int low;
	float max=0;

	float fPriceCurr = 0;
	for(int i=0;i<=nCurrent;i++)
	{
		CReportData dt2;
		pHs=CTaiKlineFileHS::GetAtBS (pBuySell ,i,nTotal);

		if(i == nCurrent) fPriceCurr = pHs->price ;

		CTaiKlineFileHS::Cdat1ToHs(&dt2,pHs,false);

		if(i<nCurrent)
		{
			high=NORMALIZING(dt2.psel3 );
			low= NORMALIZING(dt2.pbuy3);
			//remove
			pos=fMap.GetStartPosition();
			while(pos)
			{
				float rValue;
				int rKey;
				fMap.GetNextAssoc( pos, rKey,rValue ) ;
				if(rKey>=(low)&&rKey<=(high))
					fMap.RemoveKey(rKey);
			}

			//add
			if(dt2.pbuy1>0) fMap.SetAt(NORMALIZING(dt2.pbuy1) ,dt2.vbuy1 );
			if(dt2.pbuy2>0) fMap.SetAt(NORMALIZING(dt2.pbuy2)  ,dt2.vbuy2  );
			if(dt2.pbuy3>0) fMap.SetAt(NORMALIZING(dt2.pbuy3)  ,dt2.vbuy3  );
			if(dt2.psel1>0) fMap.SetAt(NORMALIZING(dt2.psel1)  ,dt2.vsel1  );
			if(dt2.psel2>0) fMap.SetAt(NORMALIZING(dt2.psel2)  ,dt2.vsel2  );
			if(dt2.psel3>0) fMap.SetAt(NORMALIZING(dt2.psel3)  ,dt2.vsel3  );
		}

		if(dt2.vbuy1 >max) max = dt2.vbuy1;
		if(dt2.vbuy2 >max) max = dt2.vbuy2;
		if(dt2.vbuy3 >max) max = dt2.vbuy3;
		if(dt2.vsel1 >max) max = dt2.vsel1;
		if(dt2.vsel2 >max) max = dt2.vsel2;
		if(dt2.vsel3 >max) max = dt2.vsel3;
	}

	//current
	high=NORMALIZING(dt.psel3 );
	low= NORMALIZING(dt.pbuy3);

	//remove
	pos=fMap.GetStartPosition();
	while(pos)
	{
		float rValue;
		int rKey;
		fMap.GetNextAssoc( pos, rKey,rValue ) ;
		if(rKey>=low&&rKey<=high)
			fMap.RemoveKey(rKey);
	}

	//draw now
	pDoK->m_nSon=0;
	pDoK->SetRectCurrent(0);

	//word
	CRect rt;pView->GetCurrClientRect(rt);
	CRect r(rt.right -pView->m_rightLength+1,0,rt.right ,rt.bottom -30);
	pView->ClearBack(pDC,r);

	int top;
	int left = rt.right-pView->m_rightLength+1;
	int len2=(pDC->GetOutputTextExtent("时间")).cy+2;
	top = rt.bottom-30-len2*5;
	CPen* pOldpen;
	{
	CPen pen_line(PS_SOLID  ,1,pDoc->m_colorArray[2]);//PS_DASHDOTDOT   
	pOldpen=pDC->SelectObject(&pen_line);
	pDC->MoveTo(rt.right-pView->m_rightLength /2 ,0);
	pDC->LineTo(rt.right-pView->m_rightLength /2 ,top-5 );
	pDC->MoveTo(left-1 ,top-5);
	pDC->LineTo(rt.right ,top-5 );
	pDC->SelectObject(pOldpen);
	}
	{
	CPen pen_line2(PS_DOT   ,1,pDoc->m_colorArray[2]);//PS_DASHDOTDOT   
	pOldpen=pDC->SelectObject(&pen_line2);
	pDC->MoveTo(rt.right-pView->m_rightLength /4 ,0);
	pDC->LineTo(rt.right-pView->m_rightLength /4 ,top-5 );
	pDC->MoveTo(rt.right-pView->m_rightLength*3 /4 ,0);
	pDC->LineTo(rt.right-pView->m_rightLength*3 /4 ,top-5 );
	pDC->SelectObject(pOldpen);
	}

	//line white
	float fTotalVol = 0;
	float fMouse = 0;
	//mouse point value
	CPoint pp;
	::GetCursorPos (&pp);
	this->pView ->ScreenToClient(&pp);
//	pView->pMin1Drawing ->SetRectDraw(0);
	if(pView->m_nKlineKind2==0)
	{
		pView->pMin1Drawing ->m_nSon=0;
		pView->pMin1Drawing ->SetRectDraw(0);
		fMouse = pView->pMin1Drawing ->YTransfer((int)(pp.y ));//pDoK->m_max_sun[0]
	}
	else
	{
		pView->pKlineDrawing ->m_nSon=0;
		pView->pKlineDrawing ->SetRectCurrent(0);
		fMouse = pView->pKlineDrawing ->YTransfer((int)(pp.y ));//pDoK->m_max_sun[0]
	}
	TRACE("fm: = %.2f\n",fMouse);
	int nMouse = NORMALIZING(fMouse );
//	int nMouse = NORMALIZING(fPriceCurr );
	fMouse = 0;

	float fVolCurr = 0;//鼠标处的成交量
	float fPrice2 = 0;//鼠标处的价格
	int nGap = -1;//比较数据
	{
	CPen pen_line(PS_SOLID,2,pDoc->m_colorArray[1]);//PS_DASHDOTDOT   
	pOldpen=pDC->SelectObject(&pen_line);
	pos=fMap.GetStartPosition();
	while(pos && max>0)
	{
		float rValue;
		int rKey;
		fMap.GetNextAssoc( pos, rKey,rValue ) ;
		if(rValue <=0)
			continue;
		int nTop ;
		if(pView->m_nKlineKind2 ==0)
			nTop = pView->pMin1Drawing ->YTransfer(rKey/(float)1000.0);
		else
			nTop = pDoK->YTransfer(rKey/(float)1000.0);
		pDC->MoveTo(left,nTop);
		pDC->LineTo(left + rValue*pView->m_rightLength/max,nTop);

		fTotalVol+=rValue;
		if(abs(nMouse-rKey)<nGap || nGap == -1)
		{
			 fPrice2 = rKey/(float)1000;//
			 fVolCurr = rValue;
			 nGap = abs(nMouse-rKey);
			 TRACE("=%d",nGap);
		}

		if(rKey>=nMouse) fMouse+=rValue;
	}

	//draw line %
	CString s;
	s="1000";
	int len=(pDC->GetOutputTextExtent(s)).cx;
	if(fTotalVol>0)
	{
		pDC->MoveTo(rt.right -len,pp.y);
		pDC->LineTo(rt.right ,pp.y);
	}
	pDC->SelectObject(pOldpen);
	}

	TRACE("\n");
	TRACE("P: = %.2f   v: = %.0f gap: = %d\n",fPrice2,fVolCurr,nGap);
	//line red
	for(int j=0;j<2;j++)
	{
		float rKey[3],rValue[3];
		if(j==0)
		{
			rKey[0]=dt.pbuy1 ;rKey[1]=dt.pbuy2 ;rKey[2]=dt.pbuy3 ;
			rValue[0]=dt.vbuy1 ;rValue[1]=dt.vbuy2 ;rValue[2]=dt.vbuy3 ;
			fTotalVol+=(dt.vbuy1 +dt.vbuy2 +dt.vbuy3 );
			for(int ii=0;ii<3;ii++)
			{
				int nKey = NORMALIZING(rKey[ii]);
				if(abs(nMouse-nKey)<nGap || nGap == -1)
				{
					 fPrice2 = rKey[ii];//
					 fVolCurr = rValue[ii];
					 nGap = abs(nMouse-nKey);
					TRACE("-%d",nGap);
				}
				if(nKey>=nMouse) fMouse+=rValue[ii];
			}
		}
		else
		{
			rKey[0]=dt.psel1  ;rKey[1]=dt.psel2 ;rKey[2]=dt.psel3 ;
			rValue[0]=dt.vsel1  ;rValue[1]=dt.vsel2 ;rValue[2]=dt.vsel3 ;
			fTotalVol+=(rValue[0] +rValue[1] +rValue[2] );
			for(int ii=0;ii<3;ii++)
			{
				int nKey = NORMALIZING(rKey[ii]);
				if(abs(nMouse-nKey)<nGap || nGap == -1)
				{
					 fPrice2 = rKey[ii];//
					 fVolCurr = rValue[ii];
					 nGap = abs(nMouse-nKey);
					TRACE("+%d",nGap);
				}
				if(nKey>=nMouse) fMouse+=rValue[ii];
			}
		}

		CPen pen_line(PS_SOLID   ,2,pDoc->m_colorArray[3+j]);//yong  
		pOldpen=pDC->SelectObject(&pen_line);
		TRACE("\n");
		TRACE("P2: = %.2f   v: = %.0f gap: = %d\n",fPrice2,fVolCurr,nGap);
		for(int i=0;i<3;i++)
		{
			if(rValue[i] <=0)
				continue;
			int nTop ;
			if(pView->m_nKlineKind2 ==0)
				nTop = pView->pMin1Drawing ->YTransfer(rKey[i]);
			else
				nTop = pDoK->YTransfer(rKey[i]);
			pDC->MoveTo(left ,nTop);
			pDC->LineTo(left + rValue[i]*pView->m_rightLength/max,nTop);
		}
		pDC->SelectObject(pOldpen);
	}

	//输出文本
	{
		pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		pDC->SetTextColor( pDoc->m_colorArray[1]);
		pDC->SetBkColor( pDoc->m_colorArray[0]);
		pDC->SetBkMode(TRANSPARENT);

		CFont ft;
		LOGFONT lgf=pDoc->m_fontstr[0];
		ft.CreateFontIndirect (&lgf );
		CFont* pOld=pDC->SelectObject(&ft);

		CString s;
		s=tm.Format ("%Y/%#m/%#d %#H:%#M:%#S");
		if(s =="")
			s = "时间";
		int len=(pDC->GetOutputTextExtent(s)).cy+2;
		top = rt.bottom-30-len*5;
		pDC->TextOut(left,top+len*4,s);

		//
		s.Format ("买卖盘总手：%d",int(fTotalVol));
		pDC->TextOut(left,top+len*1,s);
		s.Format ("鼠标处挂盘信息：");

		pDC->TextOut(left,top+len*2,s);
		//fPrice2 = rKey[ii]/(float)1000;//
		//fVolCurr = rValue;
		s.Format ("价：%.2f量：%d",fPrice2,(int)fVolCurr);
		pDC->TextOut(left,top+len*3,s);

		pDC->SetTextAlign( TA_CENTER );//TA_CENTER||
		for(int i=1;i<4;i++)
		{
			s.Format ("%d",(int)(i*max/4));
			pDC->TextOut(left+i*pView->m_rightLength/4-1,top,s);
		}
		pDC->SetTextAlign( TA_RIGHT );//TA_CENTER||
		s.Format ("%d",(int)(max));
		pDC->TextOut(left+pView->m_rightLength-1,top,s);

		if(fTotalVol>0)
		{
			s.Format ("%d",(int)(fMouse/fTotalVol*100));
			s+="%";
			pDC->TextOut(rt.right-2,pp.y - len,s);
			s.Format ("%d",100 - (int)(fMouse/fTotalVol*100));
			s+="%";
			pDC->TextOut(rt.right-2,pp.y +2,s);
		}


		pDC->SelectObject(pOld);
	}
}

void CTaiKlineMoveChenben::GetMaxMin(Kline *pKline, int nKline, float &fMax, float &fMin)
{
//	float maxK =0.01,minK=90000000;
	int i;
	if(pKline==NULL)
		return;

	for( i=0;i<nKline;i++)
	{
			if((pKline+i)->high >fMax)
			{
				fMax=(pKline+i)->high;
			}
			if((pKline+i)->low<fMin)
			{
				fMin=(pKline+i)->low;
			}
	}
	if(fMax>MAXMOVECHENBEN) fMax = MAXMOVECHENBEN;
	if(fMin<0.001) fMin = 0.001;


}

void CTaiKlineMoveChenben::GetMoveCBPerFoot(Kline *pKline, float *cnp, float ltp, float fMax, float fMin,int nFoot,float fRate)
{
	float fRuler;
	int n =CTaiKlineMoveChenben::GetRuler( fMax,  fMin, fRuler);

	int nBeginY =fMin*fRuler;
	int nEndY = fMax*fRuler;

	//--- 计算移动成本
	if(pKline[0].vol<=0)
		return;
	if(pKline[0].high>fMax)
		return;
	{
		int nLow = (pKline[0].low*fRuler+0.5);
		int nFen =( pKline[0].high- pKline[0].low)*fRuler+0.5;
		if(nFen<=0)
			return;
		//ASSERT(nLow>=nBeginY);
		if(nLow<nBeginY)
			nLow=nBeginY;
		nLow-=nBeginY;

		float fvol = pKline[0].vol*fRate/nFen;
		if(nFoot ==0 )
			fvol = ltp/nFen;
		//step 1:delete yestoday
		float rat = 1-pKline[0].vol*fRate/ltp;
		if(rat<0)
			rat = 0;
		if(rat>=1)
			return;

		int j;
		for(j=0;j<n;j++)
		{
			cnp[j]*=rat;
		}
		//step 2:add today
		for( j=0;j<nFen;j++)
		{
			cnp[j+nLow]+=fvol;
		}
	}

	//normalizing
	float volume=0;
	int i;
	for(i=0;i<n;i++)
		volume+=cnp[i];
	if(volume<=0)
	{
		return;
	}
	volume=ltp/volume;//rate now;
	for(i=0;i<n;i++)
	{
		cnp[i]*=volume;
	}


}

void CTaiKlineMoveChenben::GetMoveCBPerFoot2(Kline* pKline,float* cnp,float ltp,float fMax,float fMin,int nFoot ,float fRate,int n,float fRuler)
{
//	float fRuler;
//	int n =CTaiKlineMoveChenben::GetRuler( fMax,  fMin, fRuler);

	int nBeginY =fMin*fRuler;
	int nEndY = fMax*fRuler;

	//--- 计算移动成本
	if(pKline[0].vol<=0)
		return;
	if(pKline[0].high>fMax)
		return;

	{
		int nLow = (pKline[0].low*fRuler+0.5);
		int nFen =( pKline[0].high- pKline[0].low)*fRuler+0.5;
		if(nFen<=0)
			return;
		//ASSERT(nLow>=nBeginY);
		if(nLow<nBeginY)
			nLow=nBeginY;
		nLow-=nBeginY;

		float fvol = pKline[0].vol*fRate/nFen;
		if(nFoot ==0 )
			fvol = ltp/nFen;
		//step 1:delete yestoday
		float rat = 1-pKline[0].vol*fRate/ltp;
		if(rat<0)
			rat = 0;
		if(rat>=1)
			return;

		int j;
		for(j=0;j<n;j++)
		{
			cnp[j]*=rat;
		}
		//step 2:add today
		for( j=0;j<nFen;j++)
		{
			cnp[j+nLow]+=fvol;
		}
	}
/*
	//normalizing
	float volume=0;
	for(int i=0;i<n;i++)
		volume+=cnp[i];
	if(volume<=0)
	{
		return;
	}
	volume=ltp/volume;//rate now;
	for(i=0;i<n;i++)
	{
		cnp[i]*=volume;
	}

*/
}

int CTaiKlineMoveChenben::GetRuler(float fMax, float fMin, float &fRuler)
{
	fRuler = 100;
	if(fMax-fMin<10)
		fRuler = 100;
	else if(fMax-fMin<20)
		fRuler = 50;
	else if(fMax-fMin<40)
		fRuler = 20;
	else if(fMax-fMin<100)
		fRuler = 10;
	else if(fMax-fMin<1000)
		fRuler = 1;
	else 
		fRuler = 0.01;

	int n =(int)( (fMax - fMin)*fRuler+10);
	return n;
}

void CTaiKlineMoveChenben::ShowCommerceCB(CDC *pDC, int nShowVirtualLine)
{
	CRect rt;pView->GetCurrClientRect(rt);//pView->m_infoFiguer !=1||
	if(pView->m_nKlineKind2 ==0 || (pView->m_nKlineKind2>5)||nShowVirtualLine>0&&(pView->m_nKlineKind2!=5))
	{
		CString s="";
		CString s2="";
		if(nShowVirtualLine==0)
		{
			s = "只能在日K线";
			s2 = "及5分钟K线周期显示" ;
		}
		else if(pView->m_nKlineKind2!=5)
		{
			s = "只能在日K线周期显示" ;
		}
		CFont ft;
		LOGFONT lgf=pDoc->m_fontstr[0];
		ft.CreateFontIndirect (&lgf );
		CFont* pOld=pDC->SelectObject(&ft);
		pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		pDC->SetTextColor( pDoc->m_colorArray[1]);
		pDC->SetBkColor( pDoc->m_colorArray[0]);
		pDC->SetBkMode(TRANSPARENT   );


		{
			int top = 10;
			int left = rt.right-pView->m_rightLength+5;
			pDC->TextOut(left,top,s);
			int nH = (pDC->GetOutputTextExtent(s)).cy;
			if(s2!="")
				pDC->TextOut(left,top+nH+2,s2);
		}
		pDC->SelectObject(pOld);
		return;
	}
	CReportData* pdt = NULL;
	if(!pDoc->m_sharesInformation .Lookup (pView->m_sharesSymbol.GetBuffer(0),pdt,pView->m_stkKind))
		return;
	if(pdt->pBaseInfo==NULL)
		return;
	if(CTaiShanKlineShowView::GetCapital(pdt)<=0)//
		return ;
	if(pView->m_nCountKline <=0)
		return;

	int end=-1;
	int begin=0;//=m_footChenBenB;
	Kline* pKline=NULL;
	int totK;
	//bool bMin5=pDoc ->m_propertyInitiate.bUseMin5ChenBen;

	int nFootCurrent = pDoK->m_footCurrent;
	if(nFootCurrent>=pView->m_nCountKline )
	{
		nFootCurrent=pView->m_nCountKline-1;
	}
//	if(nShowVirtualLine >0)
//		nFootCurrent=pView->m_nCountKline-1;

//	bMin5==false
	{
		pKline=pView->m_pkline;
		end=nFootCurrent ;
		m_symbol = pView->m_sharesSymbol ;
	}

	pDoK->m_nSon=0;
	pDoK->SetRectCurrent(0);

	//compute max and min 
	float maxK =0.01,minK=90000000;
	/*int i;
	for(int j=0;j<2;j++)
		for( i=0;i<=end[j];i++)
		{
			if(pKline[j]!=NULL)
			{
				if((pKline[j]+i)->high >maxK)
				{
					maxK=(pKline[j]+i)->high;
				}
				if((pKline[j]+i)->low<minK)
				{
					minK=(pKline[j]+i)->low;
				}
			}
		}*/

//	int nLenData = end[0]-begin[0]+1;//1800;
//	if(end[0]-begin[0]>1800) nLenData = 1800;//begin[0] = end[0]-1800;
//	if(maxK>MAXMOVECHENBEN) 
//		maxK = MAXMOVECHENBEN;
//	if(minK<0.001) minK = 0.001;
	GetMaxMin(pKline,end+1,maxK,minK);
	//set memory of data,a value a fen
	float fRuler;
	int n =CTaiKlineMoveChenben::GetRuler( maxK,  minK, fRuler);

	int nBeginY =minK*fRuler;
	int nEndY = maxK*fRuler;

	//--- 计算移动成本
//	int n=nEndY - nBeginY+5;
	if(n<=0)
		return ;
	float* cnp=new  float[n];

	int nVol=0;
	float max=0;
	float volume=0;
	
	float ltp = CTaiShanKlineShowView::GetCapital(pdt);

	int nFootBegin=0;
	int nFootEnd=0;

	float fAdd = 0.5;
	if(fRuler<100)
		fAdd = 1.5;

	float ltp3 = ltp;
	if(pView->m_sharesSymbol [1] == 'A')
		ltp3 = ltp/100;
	
	
//取得流通盘比例、成交量比例
//~~~~~~~~~~~~~~~~~~~~~~~~~~~

float fTmpLTP=0.1;//pDoc->m_propertyInitiate.LTP_rate;
float fTmpCJL=0.5;//pDoc->m_propertyInitiate.CJL_rate;
float ltp4=ltp3;
float fTemp;	
for(int nnn=0;nnn<2;nnn++)
{		
	nVol=0;
	max=0;
	volume=0;
	memset(cnp,0,n*4);
	if (nnn==0)
		ltp3=ltp4*fTmpLTP;
	else
		ltp3=ltp4*(1-fTmpLTP);
	fTemp=1-fTmpCJL;
	int i;
	for(i=begin;i<=end;i++)
	{
		if (nnn==0)
			GetMoveCBPerFoot(pKline+i,cnp,ltp3,maxK,minK,i,fTmpCJL);
		else
			GetMoveCBPerFoot(pKline+i,cnp,ltp3,maxK,minK,i,fTemp);			
	}

	//normalizing
	for(i=0;i<n;i++)
		volume+=cnp[i];
	if(volume<=0)
	{
		delete[] cnp;
		return;
	}
	volume=ltp3/volume;//rate now;
	int nFootMax = 0;
	float amount=0;
	for(i=0;i<n;i++)
	{
		cnp[i]*=volume;
//		ASSERT(cnp[i]>=0);
		amount+=(cnp[i]*(i+nBeginY)/(float)fRuler);
		if(cnp[i]>max)
		{
			nFootMax = i;
			max=cnp[i];
		}
	}

	//current page data
	float maxCurr = 0;
	int bottomMin = pDoK->YTransfer((int)0)*fRuler-nBeginY;//pDoK->m_max_sun[0]
	int topMax = pDoK->YTransfer((int)rt.bottom-50)*fRuler-nBeginY;//pDoK->m_min_sun[0]
	if(bottomMin>n-1 )
		 bottomMin=  n-1 ;
	if(topMax<0)
		 topMax= 0;

	for(i = topMax; i<=bottomMin;i++)
	{
		if(cnp[i]>maxCurr)
			maxCurr=cnp[i];
	}

	if(max<=0 ||maxCurr<=0)
	{
		delete[] cnp;
		return;
	}

	//
	float yGap=1/fRuler*(pDoK->m_rectDrawLine.bottom-pDoK->m_rectDrawLine.top)/(pDoK->m_max_sun[0]-pDoK->m_min_sun[0]);//pDoK->YTransfer((float)pDoK->m_min_sun[0])-pDoK->YTransfer((float)(pDoK->m_min_sun[0]+0.01));
	if(yGap<=0 || pView->m_pkline [nFootCurrent].close>maxK)
	{
		delete[] cnp;
		return;
	}

	//mouse point value
	CPoint pp;
	::GetCursorPos (&pp);
	this->pView ->ScreenToClient(&pp);
	int mousePos = pDoK->YTransfer((int)(pp.y ))*fRuler-nBeginY;//pDoK->m_max_sun[0]
	if(mousePos>=n) mousePos = n-1;
	if(mousePos<0) mousePos = 0;
	float fMouseVal = 0;
	for(i=0;i<=mousePos;i++)
	{
		fMouseVal+=cnp[i];
	}

	int nColor = 1;
	//statistics
	{
		volume = 0;
		int nClose = pView->m_pkline [nFootCurrent].close*fRuler-nBeginY;
		if(nClose>=n) nClose = n-1;
		for(i=0;i<=nClose;i++)
		{
			volume+=cnp[i];
		}
		if(volume/ltp3>0.95)
			nColor = 4;
		else if(volume/ltp3<0.05)
			nColor = 3;
	}


	//---	画线

	CPen pen_line(PS_SOLID  ,1,pDoc->m_colorArray[nColor]);//PS_DASHDOTDOT   
	CPen* pOldpen=pDC->SelectObject(&pen_line);
	pDC->SetBkMode(OPAQUE);

	if(nnn == 0)
	{
		int nTop = pDoK->YTransfer((float)(topMax+nBeginY)/(float)fRuler);
		for(i=0;i<bottomMin - topMax ;i ++ )
		{
			pDC->MoveTo(rt.right-pView->m_rightLength/2-1,nTop-i*yGap);
			//                  ~~~~~~~~~~~~~~~~~~~~~
			pDC->LineTo(rt.right-pView->m_rightLength/2-1-(int)(RATE_EXTEND * cnp[topMax+i]*(pView->m_rightLength/2)/maxCurr),nTop-i*yGap);
			//                                                                                                
		}
		pDC->MoveTo(rt.right-pView->m_rightLength+1,pp.y );
		pDC->LineTo(rt.right-pView->m_rightLength+1+20,pp.y);
	}
	else
	{
		int nTop = pDoK->YTransfer((float)(topMax+nBeginY)/(float)fRuler);
		for(i=0;i<bottomMin - topMax ;i ++ )
		{
			pDC->MoveTo(rt.right-pView->m_rightLength/2+1,nTop-i*yGap);
			//                  ~~~~~~~~~~~~~~~~~~~~~
			pDC->LineTo(rt.right-pView->m_rightLength/2+1+(int)(RATE_EXTEND * cnp[topMax+i]*(pView->m_rightLength/2)/maxCurr),nTop-i*yGap);
			//                  ~~~~~~~~~~~~~~~~~~~~~                                                      ~~~          ~~~~~~~~~~~~~~~~~~~~~
		}
		pDC->MoveTo(rt.right-pView->m_rightLength+1,pp.y );
		pDC->LineTo(rt.right-pView->m_rightLength+1+20,pp.y);
	}
	
	//
	pDC->SelectObject(pOldpen);
	//close line

	{
		int color[2]={3,4};
		if(nColor>2)
		{
			color[0] =1;color[1]=1;
		}
		float f[2];
		f[0]=(float)pView->m_pkline [nFootCurrent].close;
		if(f[0]<minK) f[0] = minK;
		if(f[0]>maxK) f[0] = maxK;
		f[1]=amount/ltp3;
		if(f[1]<minK) f[1] = minK;
		if(f[1]>maxK) f[1] = maxK;
	
		for(int j=0;j<2;j++)
		{
			if(f[j]>=MAXMOVECHENBEN) continue;
			CPen pen_line2(PS_SOLID  ,2,pDoc->m_colorArray[color[j]]);//PS_DASHDOTDOT   
			pOldpen=pDC->SelectObject(&pen_line2);
			{
				int nTop = pDoK->YTransfer(f[j]);
				if(nnn==0)
				{
					pDC->MoveTo(rt.right-pView->m_rightLength/2-1,nTop);
					pDC->LineTo(rt.right-pView->m_rightLength/2-1-(int)(RATE_EXTEND* cnp[(int)(f[j]*fRuler)-nBeginY]*(pView->m_rightLength/2)/maxCurr),nTop);
				}
				else
				{
					pDC->MoveTo(rt.right-pView->m_rightLength/2+1,nTop);
					pDC->LineTo(rt.right-pView->m_rightLength/2+1+(int)(RATE_EXTEND* cnp[(int)(f[j]*fRuler)-nBeginY]*(pView->m_rightLength/2)/maxCurr),nTop);
				}
			}
			pDC->SelectObject(pOldpen);
		}
	}


	//statistics
	{
		pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		pDC->SetTextColor( pDoc->m_colorArray[1]);
		pDC->SetBkColor( pDoc->m_colorArray[0]);

		CFont ft;
		LOGFONT lgf=pDoc->m_fontstr[0];
		ft.CreateFontIndirect (&lgf );
		CFont* pOld=pDC->SelectObject(&ft);

		{

			CString s;
			//s.Format ("最大量:%d(%d",(int)max,(int)(max/ltp3*100));
			//s+="%)(手/分钱)";
			//int len=(pDC->GetOutputTextExtent(s)).cy+2;
			//int top = rt.bottom-30-len*5;
			//int left = rt.right-pView->m_rightLength+1;
			//pDC->TextOut(left,top,s);
			//s.Format ("最大量处价:%.2f",(float)(nFootMax+nBeginY)/fRuler);
			//pDC->TextOut(left,top+1*len,s);
			if(nnn==0)
			{
				s.Format ("浮动筹码");
				int len=(pDC->GetOutputTextExtent(s)).cy+2;
				int top = rt.bottom-30-len*7;
				int left = rt.right-pView->m_rightLength+1;
				pDC->TextOut(left,top,s);
				s.Format("套牢盘:");
				pDC->TextOut(left,top+1*len,s);
				s.Format("%d",(int)((ltp3-volume)/ltp3*100+0.5));
				s+="%";
				pDC->TextOut(left,top+2*len,s);
				s.Format ("获利盘:");
				pDC->TextOut(left,top+3*len,s);
				s.Format("%d",(int)(volume/ltp3*100+0.5));
				s+="%";
				pDC->TextOut(left,top+4*len,s);
				s.Format ("流通盘平均成本:");
				pDC->TextOut(left,top+5*len,s);
				s.Format("%.2f",amount/ltp3);
				pDC->TextOut(left,top+6*len,s);
				s.Format ("%d",(int)((ltp3-fMouseVal)/ltp3*100+0.5));
				s+="%";
				len=(pDC->GetOutputTextExtent(s)).cy+2;
				pDC->TextOut(left,pp.y-len,s);
				s.Format ("%d",(int)((fMouseVal)/ltp3*100+0.5));
				s+="%";
				pDC->TextOut(left,pp.y+2,s);
			}
			else
			{
				s.Format("锁定筹码");
				int len=(pDC->GetOutputTextExtent(s)).cy+2;
				int top = rt.bottom-30-len*7;
				int left = rt.right-pView->m_rightLength+1;
				pDC->TextOut(left+pView->m_rightLength/2,top,s);

				s.Format ("%d",(int)((ltp3-volume)/ltp3*100+0.5));
				s+="%";
				
				pDC->TextOut(left+pView->m_rightLength/2,top+2*len,s);
				s.Format ("%d",(int)(volume/ltp3*100+0.5));
				s+="%";
				pDC->TextOut(left+pView->m_rightLength/2,top+4*len,s);
				s.Format ("%.2f",amount/ltp3);
				pDC->TextOut(left+pView->m_rightLength/2,top+6*len,s);
				
				s.Format ("%d",(int)((ltp3-fMouseVal)/ltp3*100+0.5));
				s+="%";
				len=(pDC->GetOutputTextExtent(s)).cy+2;
				pDC->TextOut(rt.right-27,pp.y-len,s);
				s.Format ("%d",(int)((fMouseVal)/ltp3*100+0.5));
				s+="%";
				pDC->TextOut(rt.right-27,pp.y+2,s);
			}

		}
		pDC->SelectObject(pOld);
	}
}
	delete[] cnp;
	return ;

}

void CTaiKlineMoveChenben::ShowVol(CDC *pDC, int nShowVirtualLine)
{

}
//lmb 200303 b

void CTaiKlineMoveChenben::ShowFxjMoveCB(CDC *pDC)
{
	CRect rt;
	pView->GetCurrClientRect(rt);
/*Marenan
	if(pView->m_nKlineKind2 != 5)
	{
		CString s="";
		CString s2="";
		s = "只能在日K线周期显示" ;

		CFont ft;
		LOGFONT lgf=pDoc->m_fontstr[0];
		ft.CreateFontIndirect (&lgf );
		CFont* pOld=pDC->SelectObject(&ft);
		pDC->SetTextAlign( TA_LEFT );
		pDC->SetTextColor( pDoc->m_colorArray[1]);
		pDC->SetBkColor( pDoc->m_colorArray[0]);
		pDC->SetBkMode(TRANSPARENT   );


		{
			int top = 10;
			int left = rt.right-pView->m_rightLength+5;
			pDC->TextOut(left,top,s);
			int nH = (pDC->GetOutputTextExtent(s)).cy;
			if(s2!="")
				pDC->TextOut(left,top+nH+2,s2);
		}
		pDC->SelectObject(pOld);
		return;
	}
*/


//	if(m_nShowKind == 2) 
//	{
//		ShowFxjMoveCB2(pDC);
//		return;
//	}

	CArray<float* ,float*&> floatArr;//保存数据指针

	CReportData* pdt = NULL;
	if(!pDoc->m_sharesInformation .Lookup (pView->m_sharesSymbol.GetBuffer(0),pdt,pView->m_stkKind ))
	{
		goto BOTTOM_LINE;
	}
	if(pdt->pBaseInfo==NULL)
	{
		goto BOTTOM_LINE;
	}
	if(CTaiShanKlineShowView::GetCapital(pdt)<=0)//
	{
		goto BOTTOM_LINE;
	}
	if(pView->m_nCountKline <=0)
	{
		goto BOTTOM_LINE;
	}

	int end[2]={-1,-1};
	int begin[2]={0,0};
	Kline* pKline[2]={NULL,NULL};
	int totK;

	int nFootCurrent = pDoK->m_footCurrent;
	if(nFootCurrent>=pView->m_nCountKline )
	{
		nFootCurrent=pView->m_nCountKline-1;
	}

	pKline[0]=pView->m_pkline;
	end[0]=nFootCurrent ;

	m_symbol = pView->m_sharesSymbol ;
	m_stkKind = pView->m_stkKind ;

	pDoK->m_nSon=0;
	pDoK->SetRectCurrent(0);

	//compute max and min 
	float maxK =0.0001,minK=90000000;
	int i;
	int j=0;
	for( i=0;i<=end[j];i++)
	{
		if(pKline[j]!=NULL)
		{
			if((pKline[j]+i)->high >maxK)
			{
				maxK=(pKline[j]+i)->high;
			}
			if((pKline[j]+i)->low<minK)
			{
				minK=(pKline[j]+i)->low;
			}
		}
	}

	if(maxK > MAXMOVECHENBEN) 
		maxK = MAXMOVECHENBEN;
	if(minK < 0.001) minK = 0.001;

	float fRuler;
	int n =CTaiKlineMoveChenben::GetRuler( maxK,  minK, fRuler);
	if(n<=0)//n 为价格区间，即在最高价和遇低价间分为多少等份
	{
		goto BOTTOM_LINE;
	}

	int nBeginY =minK*fRuler;
	int nEndY = maxK*fRuler;


	float* cnp=new  float[n];//筹码在n个价格上的堆积
	memset(cnp,0,n*4);
	floatArr.Add (cnp);
	if(m_nShowKind == 0)//general
	{
		float* p=new  float[n];
		memset(p,0,n*4);
		floatArr.Add (p);
	}
	if(m_nShowKind == 1)//多少天前成本
	{
		int nArr = m_chengBenBeforArr.GetSize();
		for(int k=0;k<nArr;k++)
		{
			float* p=new  float[n];
			memset(p,0,n*4);
			floatArr.Add (p);
		}
	}
	if(m_nShowKind == 2)//多少天内成本
	{
		int nArr = m_chengBenInsideArr.GetSize();
		for(int k=0;k<nArr;k++)
		{
			float* p=new  float[n];
			memset(p,0,n*4);
			floatArr.Add (p);
		}
	}

	float volume=0;
	int nVol=0;
	float ltp = CTaiShanKlineShowView::GetCapital(pdt);//流通盘

	int nFootBegin=0;
	int nFootEnd=0;

	float fAdd = 0.5;
	if(fRuler<100)
		fAdd = 1.5;

	float ltp3 = ltp;
	if(pView->m_sharesSymbol [1] == 'A')
		ltp3 = ltp/100;
	int k =0;

	float amount=0;
	int nClose;
	CPoint pp;
	float fMouseVal = 0;
	float maxCurr = 0;
	int bottomMin ;//pDoK->m_max_sun[0]
	int topMax ;//pDoK->m_min_sun[0]
	float yGap;//pDoK->YTransfer((float)pDoK->m_min_sun[0])-pDoK->YTransfer((float)(pDoK->m_min_sun[0]+0.01));
	if(m_nShowKind < 2 && pKline[k]!=NULL)//m_nShowKind == 1:火焰山(多少天前成本) 和 m_nShowKind == 0:移动成本分布
	{
		float fBeginLtp ;

		for(i=begin[k];i<=end[k];i++)
		{
			int foot;

			if(pKline[k][i].high>maxK) 
				continue;

			if(pKline[k][i].vol>0 )
			{
				int nLow = (pKline[k][i].low*fRuler+0.5);
				int nFen =( pKline[k][i].high- pKline[k][i].low)*fRuler+fAdd;
				if(nFen<=0)
					continue;
				ASSERT(nLow>=nBeginY);
				nLow-=nBeginY;

				float fvol = pKline[k][i].vol/nFen;//平均成本分布（将当日的换手筹码在当日的最高价和最低价之间平均）
				if(i==begin[k] && k==0)
					fvol = ltp3/nFen;
				//step 1:删除昨日成本分布
				float rat = 1-pKline[k][i].vol/ltp3;//1	- 换手率
				if(rat<0)
					rat = 0;
				if(rat>=1)
					continue;

				for(int j=0;j<n;j++)
				{
					cnp[j] *= rat;//（1-A）*Y
				}
				//step 2:添加当日的成本分布
				for(int j=0;j<nFen;j++)
				{
					cnp[j+nLow]+=fvol;//最低与最高之间进行累积
				}

				if(i==end[k] && m_nShowKind == 0)
				{
					for( j=0;j<nFen;j++)
					{
						floatArr.GetAt (1)[j+nLow]=fvol;
					}
				}
				if(m_nShowKind == 1)//火焰山
				{
					int nn = m_chengBenBeforArr.GetSize();
					for( j=0;j<nn;j++)
					{
						if(m_chengBenBeforArr[j].m_nDays == end[k]-i)
						{
							float *fp = floatArr[j+1];
							memcpy(fp,cnp,sizeof(float)*n);
							break;
						}
					}
				}
			}
		}
		//量化到标准区别，便于画图
		for(i=0;i<n;i++)
			volume+=cnp[i];
		if(volume<=0)
		{
			goto BOTTOM_LINE;
		}
		volume=ltp3/volume;//rate now;
		for(i=0;i<n;i++)
		{
			cnp[i]*=volume;
			amount+=(cnp[i]*(i+nBeginY)/(float)fRuler);
		}
	}
	if(m_nShowKind == 2 && pKline[k]!=NULL)//活跃度：多少天内成本
	{
		float* p=new  float[n];
		memset(p,0,n*4);

		int nMaxDays = 0;
		int nn = m_chengBenInsideArr.GetSize();
		for( j=0;j<nn;j++)
		{
			if(m_chengBenInsideArr[j].m_nDays>nMaxDays)
				nMaxDays = m_chengBenInsideArr[j].m_nDays;
		}
		for(i=end[k];i>=begin[k] && end[k]-i<=nMaxDays;i--)
		{
			int foot;

			if(pKline[k][i].high>maxK) 
				continue;

			if(pKline[k][i].vol>0 )
			{
				int nLow = (pKline[k][i].low*fRuler+0.5);
				int nFen =( pKline[k][i].high- pKline[k][i].low)*fRuler+fAdd;
				if(nFen<=0)
					continue;
				ASSERT(nLow>=nBeginY);
				nLow-=nBeginY;

				float fvol = pKline[k][i].vol/nFen;
				for( j=0;j<nFen;j++)
				{
					p[j+nLow]+=fvol;
				}
			}
			nn = m_chengBenInsideArr.GetSize();
			for( j=0;j<nn;j++)
			{
				if(m_chengBenInsideArr[j].m_nDays == end[k]-i)
				{
					float *fp = floatArr[j+1];
					memcpy(fp,p,sizeof(float)*n);
					break;
				}
			}
		}

		delete [] p ;
	}


	//此后都是如何画成本分布图

	//current page data
	maxCurr = 0;
	bottomMin = pDoK->YTransfer((int)0)*fRuler-nBeginY;//pDoK->m_max_sun[0]
	topMax = pDoK->YTransfer((int)rt.bottom-50)*fRuler-nBeginY;//pDoK->m_min_sun[0]
	if(bottomMin>n-1 )
		 bottomMin=  n-1 ;
	if(topMax<0)
		 topMax= 0;

	for(j=0;j<floatArr.GetSize ();j++)
	{
		float *fp = floatArr[j];
		for(i = topMax; i<=bottomMin;i++)
		{
			if(fp[i]>maxCurr)
				maxCurr=fp[i];
		}
	}

	if(maxCurr<=0)
	{
		goto BOTTOM_LINE;
	}

	//
	yGap=1/fRuler*(pDoK->m_rectDrawLine.bottom-pDoK->m_rectDrawLine.top)/(pDoK->m_max_sun[0]-pDoK->m_min_sun[0]);//pDoK->YTransfer((float)pDoK->m_min_sun[0])-pDoK->YTransfer((float)(pDoK->m_min_sun[0]+0.01));
	if(yGap<=0 || pView->m_pkline [nFootCurrent].close>maxK)
	{
		goto BOTTOM_LINE;
	}

	{
	//mouse point value
	::GetCursorPos (&pp);
	this->pView ->ScreenToClient(&pp);
	int mousePos = pDoK->YTransfer((int)(pp.y ))*fRuler-nBeginY;//pDoK->m_max_sun[0]
	if(mousePos>=n) mousePos = n-1;
	if(mousePos<0) mousePos = 0;
	for(i=0;i<=mousePos;i++)
	{
		fMouseVal+=cnp[i];
	}

	//statistics
	nClose = pView->m_pkline [nFootCurrent].close*fRuler-nBeginY;
	{
		volume = 0;
		if(nClose>=n) nClose = n-1;
		for(i=0;i<=nClose;i++)
		{
			volume+=cnp[i];
		}
	}


	//---	画线
	CPen pen_line(PS_SOLID  ,1,pDoc->m_colorArray[4]);//PS_DASHDOTDOT   
	CPen* pOldpen=pDC->SelectObject(&pen_line);
	pDC->SetBkMode(OPAQUE);

	int nTop = pDoK->YTransfer((float)(topMax+nBeginY)/(float)fRuler);
	CPen pen_line2(PS_SOLID  ,1,pDoc->m_colorArray[3]);//PS_DASHDOTDOT   
	int nClose2 = nClose - topMax;
	if(m_nShowKind < 2 )//before cheng ben
	{
		for(i=0;i<nClose2 ;i ++ )
		{
			pDC->MoveTo(rt.right,nTop-i*yGap);
			pDC->LineTo(rt.right-(int)(RATE_EXTEND * cnp[topMax+i]*(pView->m_rightLength)/maxCurr),nTop-i*yGap);
		}
		if(m_nShowKind == 0)
		{
			float * fp = floatArr[1];
			CPen pen_line3(PS_SOLID  ,1,pDoc->m_colorArray[1]);//PS_DASHDOTDOT   
			CPen* pOldpen3=pDC->SelectObject(&pen_line3);
			for(i=0;i<nClose2 ;i ++ )
			{
				if(fp[topMax+i]>0)
				{
					pDC->MoveTo(rt.right-(int)(RATE_EXTEND * ((cnp[topMax+i]-fp[topMax+i])*(pView->m_rightLength)/maxCurr)),nTop-i*yGap);
					pDC->LineTo(rt.right-(int)(RATE_EXTEND * cnp[topMax+i]*(pView->m_rightLength)/maxCurr),nTop-i*yGap);
				}
			}
			pDC->SelectObject(pOldpen3);
		}
		pDC->SelectObject(&pen_line2);
		for(i=nClose2;i<bottomMin - topMax ;i ++ )
		{
			pDC->MoveTo(rt.right,nTop-i*yGap);
			pDC->LineTo(rt.right-(int)(RATE_EXTEND * cnp[topMax+i]*(pView->m_rightLength)/maxCurr),nTop-i*yGap);
		}
		if(m_nShowKind == 0)
		{
			float * fp = floatArr[1];
			CPen pen_line3(PS_SOLID  ,1,pDoc->m_colorArray[1]);//PS_DASHDOTDOT   
			CPen* pOldpen3=pDC->SelectObject(&pen_line3);
			for(i=nClose2;i<bottomMin - topMax ;i ++ )
			{
				if(fp[topMax+i]>0)
				{
					pDC->MoveTo(rt.right-(int)(RATE_EXTEND * ((cnp[topMax+i]-fp[topMax+i])*(pView->m_rightLength)/maxCurr)),nTop-i*yGap);
					pDC->LineTo(rt.right-(int)(RATE_EXTEND * cnp[topMax+i]*(pView->m_rightLength)/maxCurr),nTop-i*yGap);
				}
			}
			pDC->SelectObject(pOldpen3);
		}
	}
	if(m_nShowKind >= 1 && bottomMin - topMax>0)
	{
		POINT* pt = new POINT[bottomMin - topMax +3];
		int nn = m_chengBenBeforArr.GetSize();
		if( m_nShowKind ==2) nn = m_chengBenInsideArr.GetSize();
		//for(j = 0;j<nn;j++)	// huhe- (15)修改，蓝绿移动成本分布层次显示问题
		for(j = nn-1; j>=0; j--)// huhe+
		{
			float * fp = floatArr[j+1];
			k = 0;
			for(i=0;i<bottomMin - topMax ;i ++ )
			{
				pt[k].y = nTop-i*yGap;
				pt[k].x = rt.right-(int)(RATE_EXTEND * (fp[topMax+i]*(pView->m_rightLength)/maxCurr));
				if(k>0)
				{
					if(pt[k].y == pt[k-1].y && pt[k].x == pt[k-1].x) continue;
				}
				k++;
			}
			int nPre = k-1;
			if(nPre <0) nPre = 0;
			pt[k].y = pt[nPre].y;
			pt[k].x = rt.right-2;
			k++;
			pt[k].y = pt[0].y;
			pt[k].x = rt.right-2;
			k++;
			pt[k].y = pt[0].y;
			pt[k].x = pt[0].x;
			k++;
			CRgn rgn;
			rgn.CreatePolygonRgn (pt,k,ALTERNATE);

			CBrush brush1;
			brush1.CreateSolidBrush (RGB(255,255,255));
			pDC->FrameRgn(&rgn,&brush1,2,2);
			OffsetRgn(rgn,2,0);

			CBrush brush;
			if( m_nShowKind ==2)
				brush.CreateSolidBrush (m_chengBenInsideArr[j].m_colorLine);
			else
				brush.CreateSolidBrush (m_chengBenBeforArr[j].m_colorLine);

			pDC->FillRgn (&rgn,&brush);
		}
		delete [] pt;
	}

	//statistics
	{
		pDC->SetTextAlign( TA_LEFT );//TA_CENTER||
		pDC->SetTextColor( pDoc->m_colorArray[1]);
		pDC->SetBkColor( pDoc->m_colorArray[0]);

		CFont ft;
		LOGFONT lgf=pDoc->m_fontstr[0];
		ft.CreateFontIndirect (&lgf );
		CFont* pOld=pDC->SelectObject(&ft);

		{

			CString s= "获利";
			int len=(pDC->GetOutputTextExtent(s)).cy+2;
			int top = rt.bottom-30-len*5;
			int left = rt.right-pView->m_rightLength+1;
			s.Format ("获利比例: %d",(int)(volume/ltp3*100+0.5));
			s+="%";
			pDC->TextOut(left,top+0*len,s);
			s.Format ("%.2f 元处获利盘: %d",(float)pDoK->YTransfer((int)(pp.y )),(int)((fMouseVal)/ltp3*100+0.5));
			s+="%";
			pDC->TextOut(left,top+1*len,s);
			s.Format ("平均成本: %.2f",amount/ltp3);
			pDC->TextOut(left,top+2*len,s);

			//计算90%、70%集中度
			float f90 = 0;
			float f70 = 0;
			float vol90 = ltp3*0.05;
			float vol70 = ltp3*0.15;
			float * fp = floatArr[0];
			int n90 = -1;
			int n70 = -1;

			for(i = 0;i<n;i++)
			{
				if(f90<vol90)
				{
					f90+=fp[i];
					n90 = i;
				}
				if(f70<vol70)
				{
					f70+=fp[i];
					n70 = i;
				}
				else
					break;
			}
			if(n90!=-1)
				n90++;
			if(n70!=-1)
				n70++;

			f90 = 0;
			f70 = 0;
			int n902 = -1;
			int n702 = -1;
			for(i = n-1;i>=0;i--)
			{
				if(f90<vol90)
				{
					f90+=fp[i];
					n902 = i;
				}
				if(f70<vol70)
				{
					f70+=fp[i];
					n702 = i;
				}
				else
					break;
			}
			if(n902!=-1)
				n902--;
			if(n702!=-1)
				n702--;

			//	int nClose = pView->m_pkline [nFootCurrent].close*fRuler-nBeginY;

			s.Format ("筹码在 %.2f-%.2f 间",(float)(n90+nBeginY)/fRuler,(float)(n902+nBeginY)/fRuler);
			s="%90 " + s;
			pDC->TextOut(left,top+3*len,s);
			s.Format ("筹码在 %.2f-%.2f 间",(float)(n70+nBeginY)/fRuler,(float)(n702+nBeginY)/fRuler);
			s="%70 " + s;
			pDC->TextOut(left,top+4*len,s);

			//draw line
			pDC->SelectObject(&pen_line);
			pDC->MoveTo (rt.right-1,nTop-(n902-topMax)*yGap);
			pDC->LineTo (rt.right-1,nTop-(n90-topMax)*yGap);
			pDC->MoveTo (rt.right-2,nTop-(n902-topMax)*yGap);
			pDC->LineTo (rt.right-2,nTop-(n90-topMax)*yGap);

			pDC->SelectObject(&pen_line2);
			pDC->MoveTo (rt.right-3,nTop-(n702-topMax)*yGap);
			pDC->LineTo (rt.right-3,nTop-(n70-topMax)*yGap);
			pDC->MoveTo (rt.right-4,nTop-(n702-topMax)*yGap);
			pDC->LineTo (rt.right-4,nTop-(n70-topMax)*yGap);


			//draw label of line
			if(m_nShowKind > 0)
			{
				int nCount ;
				if(m_nShowKind == 1)
				{
					nCount = m_chengBenBeforArr.GetSize();
				}
				else
					nCount = m_chengBenInsideArr.GetSize();
				top = rt.bottom-30-(len+1)*(5+nCount);

				s.Format(" 120天前成本分布 ");
				int len2=(pDC->GetOutputTextExtent(s)).cx+20;
				pDC->SetBkMode (TRANSPARENT);
				for(i = 0;i<nCount;i++)
				{
					CBrush brush;
					if(m_nShowKind == 1)
					{
//						brush.CreateSolidBrush (m_chengBenBeforArr[i].m_colorLine);
						s.Format("%d天前成本分布",m_chengBenBeforArr[i].m_nDays);
						pDC->FillSolidRect (rt.right-pView->m_rightLength+1,top+i*(len+1),20,len,m_chengBenBeforArr[i].m_colorLine);
						pDC->TextOut (rt.right-pView->m_rightLength+22,top+i*(len+1),s);
					}
					else
					{
						s.Format("%d天内成本分布",m_chengBenInsideArr[i].m_nDays);
						pDC->FillSolidRect (rt.right-pView->m_rightLength+1,top+i*(len+1),20,len,m_chengBenInsideArr[i].m_colorLine);
						pDC->TextOut (rt.right-pView->m_rightLength+22,top+i*(len+1),s);
					}
				}
			}
		}

		pDC->SelectObject(pOld);

		//draw icon
		CBitmap       m_bitmap;

		CImageList imagelist;
		imagelist.Create(64,16, ILC_MASK, 1, 1);
    
		m_bitmap.LoadBitmap(IDB_BJ_CHENGBEN);
		imagelist.Add(&m_bitmap, (COLORREF)0x00ffFFFF);
		m_bitmap.DeleteObject();
		POINT pnt;
		pnt.x = rt.right-pView->m_rightLength+1;
		pnt.y = 0;
		imagelist.Draw (pDC,0,pnt,ILD_TRANSPARENT);


	}
	//
	pDC->SelectObject(pOldpen);
	}

	i = floatArr.GetSize ();
	for(j = 0;j<i;j++)
		if(floatArr[j]) delete[] floatArr[j];
//add
BOTTOM_LINE:
	//draw icon
	CBitmap       m_bitmap;

	CImageList imagelist;
	imagelist.Create(64,16, ILC_MASK, 1, 1);

	m_bitmap.LoadBitmap(IDB_BJ_CHENGBEN);
	imagelist.Add(&m_bitmap, (COLORREF)0x00ffFFFF);
	m_bitmap.DeleteObject();
	POINT pnt;
	pnt.x = rt.right-pView->m_rightLength+1;
	pnt.y = 0;
	imagelist.Draw (pDC,0,pnt,ILD_TRANSPARENT);

}

void CTaiKlineMoveChenben::ReadInSettings()
{
	CFile fl;
	if(fl.Open("chengben.che",CFile::modeRead))
	{
		int n1 =0;
		fl.Read (&n1,sizeof(int));
		m_chengBenBeforArr.RemoveAll();
		int i;
		for(i = 0;i<n1;i++)
		{
			BjChenBenSetting sett;
			int nRead = fl.Read (&(sett.m_colorLine ),sizeof(sett.m_colorLine));
			if(nRead!=sizeof(sett.m_colorLine))
				return;
			nRead = fl.Read (&(sett.m_nDays  ),sizeof(sett.m_nDays));
			if(nRead!=sizeof(sett.m_nDays))
				return;
			m_chengBenBeforArr.Add(sett);
		}
		fl.Read (&n1,sizeof(int));
		m_chengBenInsideArr.RemoveAll();
		for(i = 0;i<n1;i++)
		{
			BjChenBenSetting sett;
			int nRead = fl.Read (&(sett.m_colorLine ),sizeof(sett.m_colorLine));
			if(nRead!=sizeof(sett.m_colorLine))
				return;
			nRead = fl.Read (&(sett.m_nDays  ),sizeof(sett.m_nDays));
			if(nRead!=sizeof(sett.m_nDays))
				return;
			m_chengBenInsideArr.Add(sett);
		}
	}

#ifdef _DEBUG
	BjChenBenSetting sett;
	if(m_chengBenBeforArr.GetSize()<3)
	{
	sett.m_colorLine = RGB(255,0,0);
	sett.m_nDays = 20;
	m_chengBenBeforArr.Add(sett);
	sett.m_colorLine = RGB(200,50,0);
	sett.m_nDays = 40;
	m_chengBenBeforArr.Add(sett);
	sett.m_colorLine = RGB(100,200,0);
	sett.m_nDays = 80;
	m_chengBenBeforArr.Add(sett);
	}
	if(m_chengBenInsideArr.GetSize()<3)
	{
	sett.m_colorLine = RGB(0,0,255);
	sett.m_nDays = 20;
	m_chengBenInsideArr.Add(sett);
	sett.m_colorLine = RGB(0,200,50);
	sett.m_nDays = 40;
	m_chengBenInsideArr.Add(sett);
	sett.m_colorLine = RGB(0,100,200);
	sett.m_nDays = 80;
	m_chengBenInsideArr.Add(sett);
	}
#endif

}

void CTaiKlineMoveChenben::WriteOutSettings()
{
	CFile fl;
	int i;
	if(fl.Open("chengben.che",CFile::modeCreate|CFile::shareDenyNone |CFile::modeWrite))
	{
		int n1 =m_chengBenBeforArr.GetSize();
		fl.Write (&n1,sizeof(int));
		for(i = 0;i<n1;i++)
		{
			BjChenBenSetting sett = m_chengBenBeforArr[i];
			fl.Write (&(sett.m_colorLine ),sizeof(sett.m_colorLine));
			fl.Write (&(sett.m_nDays  ),sizeof(sett.m_nDays));
		}
		n1 =m_chengBenInsideArr.GetSize();
		fl.Write (&n1,sizeof(int));
		for(i = 0;i<n1;i++)
		{
			BjChenBenSetting sett = m_chengBenInsideArr[i];
			fl.Write (&(sett.m_colorLine ),sizeof(sett.m_colorLine));
			fl.Write (&(sett.m_nDays  ),sizeof(sett.m_nDays));
		}
	}

}
//lmb 200303 e
