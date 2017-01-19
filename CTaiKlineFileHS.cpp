// CTaiKlineFileHS.cpp: implementation of the CTaiKlineFileHS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineFileHS.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "CTaiKlineDoFenshi.h"
#include "ProgressDialog.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineTransferKline.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define HS_HEAD  16
//#define MaxNumStock	4096
#define HsSmallHeadByteEach	sizeof(HSSMALLHEAD)
#define SMALLHEADLENGTH 4096 * HsSmallHeadByteEach + 16
#define FixedHsPerBlock	240
#define HsByteEach	sizeof(TRADE_DETAIL_H_PER)
;

CTaiKlineFileHS*	CTaiKlineFileHS::m_pFileHsSz = NULL;
CTaiKlineFileHS*	CTaiKlineFileHS::m_pFileHsSh = NULL;
CTaiKlineFileHS*	CTaiKlineFileHS::m_fileHsSzBak = NULL;
CTaiKlineFileHS*	CTaiKlineFileHS::m_fileHsShBak = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaiKlineFileHS::CTaiKlineFileHS()
{
	m_nMarket = SZ_MARKET_EX;
	m_bToday = true;
}

CTaiKlineFileHS::~CTaiKlineFileHS()
{
	DeleteMap();
}
CTaiKlineFileHS::CTaiKlineFileHS(int nMarket,bool bToday)
{
	ASSERT(nMarket!=0 && nMarket!=1);
	m_nMarket = nMarket;
	m_nAddReMap = HsByteEach *  FixedHsPerBlock*5;
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	pDoc = pFm->m_pDoc ;
	m_bToday = bToday;

	int n = sizeof(TRADE_DETAIL_H_PER);

}//
//TransferDataEx(CBuySellList* pBuySellList,KlineEx* pKlineEx)
int CTaiKlineFileHS::ReadHS(CString symbol,CBuySellList& buySellList,bool bClearAll)//读取K线数据
{
	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4) return 0;
	if(CTaiShanKlineShowView::IsIndexStock(symbol )&& bClearAll == false) return buySellList.GetCount();

	int n = ReadHS2( symbol,buySellList,  bClearAll);
	if(bClearAll == true)
	{
		if(CTaiShanKlineShowView::IsIndexStock(symbol ))
		{
			ReadIndexHS(symbol,buySellList,bClearAll);
			return	buySellList.GetCount ();//ReadIndexHS(symbol,buySellList,bClearAll);
		}

#ifndef WIDE_NET_VERSION
		HSSMALLHEAD hsSmallHead;
		HSSMALLHEAD* pHsSmallHead = &hsSmallHead;
		int nIndexStock = GetHsSmallHeader(symbol,pHsSmallHead);
	//read the last one
		TRADE_DETAIL_H_PER hsFirst;
		TRADE_DETAIL_H_PER* pHs = NULL;
		if(n>0)
		{
			pHs = GetAtBS(&buySellList, 0,n);
		}

		 char *strLabel = symbol.GetBuffer (0);
		 int nMarket = m_nMarket;
		 int stkKind = CSharesInformation::GetStockKind(nMarket,strLabel);

		CReportData* pCdat = NULL;
		CBuySellList buySellList2;
		if(!pDoc->m_sharesInformation .Lookup(symbol.GetBuffer (0),pCdat,stkKind))
			return n;

	//	int nNow  = pHs->time ;
		bool bNew = false;
		if(pHs ==NULL )
		{
			if(pCdat->nowp <=0 ) return n;
			pHs = new TRADE_DETAIL_H_PER;
			memset(pHs,0,sizeof(TRADE_DETAIL_H_PER));
			CTime tm = CTime::GetCurrentTime ();
			pHs->time = tm.GetTime ();
			if(tm.GetHour ()>=15 || (tm.GetHour ()<9 || tm.GetHour ()==9 && tm.GetMinute ()<26))
			{
				pHs->time = CTime(tm.GetYear (),tm.GetMonth  (),tm.GetDay  (),15,0,0).GetTime();
			}
			pHs->vol = pCdat->totv ;
			pHs->price = pCdat->nowp ;
			buySellList.AddTail (pHs);
			bNew = true;

//			n++;
		}

		CTime tm(pHs->time);
		CTime tmb (tm.GetYear (),tm.GetMonth (),tm.GetDay(),9,30,0);
		int nb = tmb.GetTime ();
		float oldPrice = pCdat->m_Kdata1[0].Price ;

		int footEnd = pDoc->m_nANT [0]+1;
		if(pDoc->m_bInitDone ==FALSE)
			footEnd=pDoc->m_nOldANT [0]+1;
		if(footEnd>240) footEnd = 240;
		for(int i=0;i<footEnd;i++)
		{
			if(pCdat->m_Kdata1[i].Volume<=0.00001 ) 
				continue;
			if(i>0)
				if(pCdat->m_Kdata1[i].Volume == pCdat->m_Kdata1[i-1].Volume) 
					continue;

			if(pCdat->m_Kdata1[i].Price >0 && (pHs->vol >pCdat->m_Kdata1[i].Volume ))
			{
				int i2 = i;
				if(i>=120)
					i2 = i+90;
				if(i2*60+nb>=pHs->time )
					break;

				TRADE_DETAIL_H_PER* pHs2 = new TRADE_DETAIL_H_PER;
				memset(pHs2,0,sizeof(TRADE_DETAIL_H_PER));
				pHs2->time = i2*60+nb;
				pHs2->price  = pCdat->m_Kdata1[i].Price  ;
				pHs2->vol = pCdat->m_Kdata1[i].Volume ;
				if(pHs2->vol <0) 
					pHs2->vol =0;

				oldPrice = pHs2->price;
				buySellList2.AddHead (pHs2);
			}
			else
				break;
		}
		POSITION pos = buySellList2.GetHeadPosition ();
		while(pos)
		{
//			ASSERT(buySellList2.GetNext (pos));
			buySellList.AddTail (buySellList2.GetNext (pos));
		}

		//write back to file
		if(buySellList2.GetCount ()>0 || bNew ==true)
			WriteHS2(symbol,buySellList);
#endif
	}
	return buySellList.GetCount ();
}
int CTaiKlineFileHS::ReadHSPeriod(CString symbol,CBuySellList& buySellList, CTime timeStart, CTime timeEnd,bool bClearAll)//读取一段时间的K线数据
//	bool WriteHS(CString symbol);	//写K线数据 
{
	return 0;
}
//HsToCdat1(CReportData *pCdat, Kline* pKline,int nFoot,CBuySellList* pBuySellList)
void CTaiKlineFileHS::Cdat1ToHs(CReportData *pCdat, TRADE_DETAIL_H_PER *pHs,bool bToHs, TRADE_DETAIL_H_PER *pHsPre)
{
	float* pPriceVol[10][2];
	int i;
	pPriceVol[0][0] = &(pCdat->pbuy1) ;
	pPriceVol[1][0] = &(pCdat->pbuy2) ;
	pPriceVol[2][0] = &(pCdat->pbuy3) ;
	pPriceVol[3][0] = &(pCdat->pbuy4) ;
	pPriceVol[4][0] = &(pCdat->pbuy5) ;
	pPriceVol[5][0] = &(pCdat->psel1) ;
	pPriceVol[6][0] = &(pCdat->psel2) ;
	pPriceVol[7][0] = &(pCdat->psel3) ;
	pPriceVol[8][0] = &(pCdat->psel4) ;
	pPriceVol[9][0] = &(pCdat->psel5) ;

	pPriceVol[0][1] = &(pCdat->vbuy1) ;
	pPriceVol[1][1] = &(pCdat->vbuy2) ;
	pPriceVol[2][1] = &(pCdat->vbuy3) ;
	pPriceVol[3][1] = &(pCdat->vbuy4) ;
	pPriceVol[4][1] = &(pCdat->vbuy5) ;
	pPriceVol[5][1] = &(pCdat->vsel1) ;
	pPriceVol[6][1] = &(pCdat->vsel2) ;
	pPriceVol[7][1] = &(pCdat->vsel3) ;
	pPriceVol[8][1] = &(pCdat->vsel4) ;
	pPriceVol[9][1] = &(pCdat->vsel5) ;

	if(bToHs==true)
	{
		pHs->price = pCdat->nowp ;
		pHs->vol = pCdat->totv  ;

		float maxFloat = 0;//FABSMY(pCdat->pbuy3 - pHs->price);
		for(i=0;i<10;i++)
		{
			if(*pPriceVol[i][0]>=0.001 && FABSMY(*pPriceVol[i][0]- pHs->price)>maxFloat)
				maxFloat = FABSMY(*pPriceVol[i][0]- pHs->price);
		}
		pHs->buySellOrIndex .buySellInfo .unitCountPrc = maxFloat ;

		float temp ;
		maxFloat = pCdat->vsel1 ;
		for(i=0;i<10;i++)
		{
			if(*pPriceVol[i][1]>maxFloat)	
				maxFloat = *pPriceVol[i][1];
		}
		pHs->buySellOrIndex .buySellInfo .unitCountVol = maxFloat ;

		if(pHs->buySellOrIndex .buySellInfo .unitCountPrc<=0)
		{
			//Marenan Modify
			//for(i=0;i<10;i++)
			for(i=0;i<5;i++)
				pHs->buySellOrIndex .buySellInfo .buyP [i] =0;
		}
		else
		{
			//Marenan Modify
			//for(i=0;i<10;i++)
			for(i=0;i<5;i++)
			{
				if(*pPriceVol[i][0]<0.01)
				{
					pHs->buySellOrIndex .buySellInfo .buyP [i] =(BYTE)127;
				}
				else
				{
					pHs->buySellOrIndex .buySellInfo .buyP [i] =(pCdat->nowp - *pPriceVol[i][0]) * 125 / pHs->buySellOrIndex .buySellInfo .unitCountPrc;
				}
			}
		}
		if(pHs->buySellOrIndex .buySellInfo .unitCountVol<=0)
		{
			//Marenan Modify
			//for(i=0;i<10;i++)
			for(i=0;i<5;i++)
				pHs->buySellOrIndex .buySellInfo .buyV [i] =0;
		}
		else
		{
			//Marenan Modify
			//for(i=0;i<10;i++)
			for(i=0;i<5;i++)
				pHs->buySellOrIndex .buySellInfo .buyV [i] =(BYTE)( (*pPriceVol[i][1])*255/ pHs->buySellOrIndex .buySellInfo .unitCountVol+0.5);;
		}

		pHs->time = pCdat->time;

		//CTime tm = CTime::GetCurrentTime ();
		//pHs->time  = (int)tm.GetTime ()  ;

		//int nYear = tm.GetYear();
		//int nMon = tm.GetMonth();
		//int nDay = tm.GetDay();
		//CTime tm9 = CTime(nYear,nMon,nDay,9,25,0);//25+
		//CTime tm11 = CTime(nYear,nMon,nDay,11,31,0);//31
		//if(tm<tm9)
		//{
		//	pHs->time = tm9.GetTime();
		//}
		//else if (tm>=tm11)
		//{
		//	CTime tm13 = CTime(nYear,nMon,nDay,13,0,0);
		//	CTime tm15 = CTime(nYear,nMon,nDay,15,0,0);
		//	if(tm<tm13)
		//	{
		//		CTime tm12 = CTime(nYear,nMon,nDay,12,0,0);
		//		if(tm<tm12)
		//			pHs->time = CTime(nYear,nMon,nDay,11,30,0).GetTime();
		//		else
		//			pHs->time = tm13.GetTime();
		//	}
		//	else if(tm>tm15)
		//	{
		//		pHs->time = tm15.GetTime();
		//	}
		//}

	}
	else
	{
		pCdat->nowp =pHs->price  ;
		pCdat->totv =(pHs->vol);
		int nFlag ;
		pCdat->nowv = CTaiKlineTransferKline::GetVolHs(pHs, pHsPre, nFlag);
		pCdat->InOut = 1;
		if(nFlag == 0) pCdat->InOut = 2;
		if(nFlag > 0) pCdat->InOut = 0;


		float maxFloat;

		maxFloat= pHs->buySellOrIndex .buySellInfo .unitCountPrc ;

		if(maxFloat<=0)
		{
			for(i=0;i<10;i++)
				*pPriceVol[i][0]=0;
		}
		else
		{
			//Marenan Modify
			//for(i=0;i<10;i++)
			for(i=0;i<5;i++)
			{
				if(pHs->buySellOrIndex .buySellInfo .buyP [i] != 127)
					*pPriceVol[i][0]=pCdat->nowp-(float)((char)(pHs->buySellOrIndex .buySellInfo .buyP [i] ) * pHs->buySellOrIndex .buySellInfo .unitCountPrc)/(float)(125);
				else 
					*pPriceVol[i][0] = 0;
			}

		}

		//vol
		float max= (float)pHs->buySellOrIndex .buySellInfo .unitCountVol ;
		if(max<=0)
		{
			for(i=0;i<10;i++)
				*pPriceVol[i][1]=0;
		}
		else
		{
			//Marenan Modify
			//for(i=0;i<10;i++)
			for(i=0;i<5;i++)
				*pPriceVol[i][1]=(float)((int)(pHs->buySellOrIndex .buySellInfo .buyV [i]*max/255 +0.5));;
		}

	}
}
bool CTaiKlineFileHS::WriteHS(CReportData* pCdat,bool bFirstOne)	//写K线数据 CReportData
{
	CString symbol (pCdat->id );
	if(CTaiShanKlineShowView::IsIndexStock(symbol))
		return false;

	if (!bFirstOne)
	{
	//	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4)
	//	{
	//		return false;
	//	}
	//	if(pCdat->opnp <0.001 || pCdat->higp  <0.001
	//		|| pCdat->lowp  <0.001|| pCdat->ystc  <0.001|| pCdat->nowp  <0.001)
	//		return false;

		HSSMALLHEAD hsSmallHead;
		HSSMALLHEAD* pHsSmallHead = &hsSmallHead;

		int nIndexStock = GetHsSmallHeader(symbol,pHsSmallHead);
		if(pCdat->m_serialNumber <0)
		{
			pCdat->m_serialNumber =0;
		}
		this->Seek(16+pCdat->m_serialNumber *sizeof(HSSMALLHEAD),this->begin);
		hsSmallHead = *((HSSMALLHEAD*)this->GetFileCurrentPointer());
		CString symb ( hsSmallHead.StockSign);
		if(symb!=symbol)
		{
			nIndexStock = GetHsSmallHeader(symbol,pHsSmallHead);
			pCdat->m_serialNumber = nIndexStock;
		}



	//	this->SeekToBegin ();
	//	ASSERT(pHsSmallHead->numHS<=0?TRUE:(int)pHsSmallHead->symBlock[(pHsSmallHead->numHS-1) / FixedHsPerBlock ]<0xffff);
	//	{
	//read the last one
	//	int i = pHsSmallHead->numHS-1;
	//	int blkCount= i / FixedHsPerBlock  ;	  //计算股票当前块数
	///	int stockCount=i % FixedHsPerBlock  ;	  //计算股票在当前块中的第几条
	//	int addr ;

	//	TRADE_DETAIL_H_PER hs ;
	//	hs.vol =0;
	/*	if(pHsSmallHead->numHS>0)
		{
			addr= SMALLHEADLENGTH + pHsSmallHead->symBlock[blkCount] 
				* HsByteEach *  FixedHsPerBlock
					 + stockCount * HsByteEach;
			ASSERT(pHsSmallHead->symBlock[blkCount]!=0xffff);
									  //计算股票K线的当前位置
			this->Seek(addr,this->begin);
			Read(&hs,sizeof(TRADE_DETAIL_H_PER));

		}
	*/
	//	if(hs.vol==0||FABSMY(hs.vol )<pCdat->totv )
	//	{
	//	}
		//write now
		TRADE_DETAIL_H_PER hs ;
		Cdat1ToHs(pCdat, &hs);
		WriteHS( pHsSmallHead,&hs);

		//set header
		this->SetHsSmallHeader (nIndexStock,pHsSmallHead);
	//	}
	}
	else
	{
		//char *strLabel = symbol.GetBuffer (0);
		//int nMarket = m_nMarket;
		//int stkKind = CSharesInformation::GetStockKind(nMarket,strLabel);

		TRADE_DETAIL_H_PER* pHs = NULL ;
		CBuySellList buySellList2,buySellList;
		//CReportData* pCdat = NULL;
		//if(!pDoc->m_sharesInformation .Lookup(symbol.GetBuffer (0),pCdat,stkKind))
		//	return false;

		//	int nNow  = pHs->time ;
		//bool bNew = false;
		//if(pHs ==NULL )
		{
			if(pCdat->nowp <=0 ) 
				return false;
			pHs = new TRADE_DETAIL_H_PER;
			memset(pHs,0,sizeof(TRADE_DETAIL_H_PER));
			//CTime tm = CTime::GetCurrentTime ();
			//pHs->time = tm.GetTime ();
			//if(tm.GetHour() >= 15)
			//{
			//	pHs->time = CTime(tm.GetYear (),tm.GetMonth  (),tm.GetDay  (),15,0,0).GetTime();
			//}
			//else if(tm.GetHour () < 9 || (tm.GetHour () == 9 && tm.GetMinute () < 26))
			//{
			//	pHs->time = CTime(tm.GetYear (),tm.GetMonth  (),tm.GetDay  (),9,30,0).GetTime();
			//}
			pHs->time = pCdat->time;

			pHs->vol = pCdat->totv ;
			pHs->price = pCdat->nowp ;
			buySellList.AddTail (pHs);
			//bNew = true;
		}

		CTime tm(pHs->time);
		CTime tmb (tm.GetYear (),tm.GetMonth (),tm.GetDay(),9,30,0);
		int nb = tmb.GetTime ();
		float oldPrice = pCdat->m_Kdata1[0].Price ;

		int footEnd = pDoc->m_nANT [0]+1;
		if(pDoc->m_bInitDone ==FALSE)
			footEnd=pDoc->m_nOldANT [0]+1;
		if(footEnd>240) footEnd = 240;
		for(int i=0;i<footEnd;i++)
		{
			if(pCdat->m_Kdata1[i].Volume<=0.00001 ) 
				continue;
			if(i>0)
				if(pCdat->m_Kdata1[i].Volume == pCdat->m_Kdata1[i-1].Volume) 
					continue;

			if(pCdat->m_Kdata1[i].Price >0 && (pHs->vol >pCdat->m_Kdata1[i].Volume ))
			{
				int i2 = i;
				if(i>=120)//>=
					i2 = i+90;
				if(i2*60+nb>=pHs->time )
					break;

				TRADE_DETAIL_H_PER* pHs2 = new TRADE_DETAIL_H_PER;
				memset(pHs2,0,sizeof(TRADE_DETAIL_H_PER));
				pHs2->time = i2*60+nb;
				pHs2->price  = pCdat->m_Kdata1[i].Price  ;
				pHs2->vol = pCdat->m_Kdata1[i].Volume ;
				if(pHs2->vol <0) 
					pHs2->vol =0;

				oldPrice = pHs2->price;
				buySellList2.AddHead (pHs2);
			}
			else
				break;
		}
		POSITION pos = buySellList2.GetHeadPosition ();
		while(pos)
		{
			//			ASSERT(buySellList2.GetNext (pos));
			buySellList.AddTail (buySellList2.GetNext (pos));
		}

		//write back to file
		if(buySellList2.GetCount ()>0/* || bNew ==true*/)
			WriteHS2(symbol,buySellList);
	}

	return true;

}
void CTaiKlineFileHS::ClearHS()	//set the count of each stock to zero,initialate
{

}
int CTaiKlineFileHS::TransferHsToMin1(CBuySellList& buySellList,TRADE_DETAIL_H_PER* pHs,int nTotal)//transfer hs to 1 minute line
{
//	CTaiKlineFileHS::RemoveHs (buySellListOut);
	if(nTotal==0)
		return 0;
	memset(pHs,0,sizeof(TRADE_DETAIL_H_PER)*nTotal);

	int n = buySellList.GetCount ();
	if(n==0)
		return 0;

	TRADE_DETAIL_H_PER* pHsIn=GetAtBS (&buySellList,0,n);
	//get day
	int tmt = (int)pHsIn->time ;
	CTime tm2(tmt);
//	tmt = tmt/(24*60*60)*24*60*60+((9-8)*60+30)*60;

	int k =0;
	ASSERT(nTotal<=240);

	int j;
	TRADE_DETAIL_H_PER* pHsLast=GetAtBS (&buySellList,n-1,n);
	for(j=0;j<n;j++)
	{
		pHsIn = GetAtBS (&buySellList,j,n);
		int k = GetMinuteSecond(pHsIn->time);
		if(k<930)
			k = 930;
		if(k>=1500)
			k = 1459;

		if(k>=1130&&k<1200)
			k=1129;
		if(k>=1200&&k<1300)
			k=1300;

		if(k<=1129)
			k=(k-900)/100*60+(k-900)%100-30;
		else
			k=(k-1300)/100*60+(k-1300)%100+120;

		if(k==112)
			k=112;
		if(k==113)
			k=113;

		memcpy(pHs+k,pHsIn,sizeof(TRADE_DETAIL_H_PER));
	}
	pHsIn=GetAtBS (&buySellList,0,n);
	if(pHs->price ==0)  
	{
		pHs->price = pHsIn->price ;
	}

	int nYear = tm2.GetYear ();
	int nMon = tm2.GetMonth ();
	int nDay = tm2.GetDay ();
	for(j=0;j<nTotal;j++)
	{
		int day  = 900+(30+j)/60*100 +(30+j)%60;//minute
		if(j>=120)
			day  = 1300+(j-120)/60*100+ (j-120)%60;
		if(pHs[j].price <=0 && j>0)
		{
			memcpy(pHs+j,pHs+j-1,sizeof(TRADE_DETAIL_H_PER));
		}
		pHs[j].time = CTime(nYear,nMon,nDay,day/100,day%100,0).GetTime();
	}

/*	for(int j=0;j<nTotal;j++)
	{
		int day  = 900+(30+j)/60*100 +(30+j)%60;//minute
		if(j>=120)
			day  = 1300+(j-120)/60*100+ (j-120)%60;

		//add
		if(k<n)
		{
			pHsIn = GetAtBS (&buySellList,k,n);
			if(GetMinuteSecond(pHsIn->time) <=day)
			{
				bool bIn = false;
				while(1)//GetMinuteSecond(GetAtBS (&buySellList,k,n)->time)<=day)
				{
//					TRADE_DETAIL_H_PER* pHsPre2 = pHsIn;
					pHsIn = GetAtBS (&buySellList,k,n);
					if(GetMinuteSecond(pHsIn->time)<=day)
					{
						memcpy(pHs+j,pHsIn,sizeof(TRADE_DETAIL_H_PER));
						k++;
						bIn = true;
						if(k>=n)
							break;
					}
					else if(bIn == false)
					{
						pHsIn = GetAtBS (&buySellList,k>0?k-1:0,n);
						memcpy(pHs+j,pHsIn,sizeof(TRADE_DETAIL_H_PER));
						break;
					}
					else
						break;
				}
				if(k>=n) memcpy(pHs+j,pHsLast,sizeof(TRADE_DETAIL_H_PER));
			}
			else 
			{
				pHsIn = GetAtBS (&buySellList,k>0?k-1:0,n);
				memcpy(pHs+j,pHsIn,sizeof(TRADE_DETAIL_H_PER));
			}
		}
		else
		{
			memcpy(pHs+j,pHsLast,sizeof(TRADE_DETAIL_H_PER));
		}
		if(j==nTotal-1)
			memcpy(pHs+j,pHsLast,sizeof(TRADE_DETAIL_H_PER));

		pHs[j].time = CTime(tm2.GetYear (),tm2.GetMonth (),tm2.GetDay (),day/100,day%100,0).GetTime();
	}
*/
	return nTotal;

}

int CTaiKlineFileHS::GetHsSmallHeader(CString symbol,HSSMALLHEAD* pHsSmallHead)//get the small header pointer of a stock
									//use the small header pointer to get
{
	if(m_pSymbolToPos == NULL)
		AddIndexToMap();


	HSSMALLHEAD* pHsSmallHead3 = NULL;
	// Look up from cdate struct
	int i = -1;
	if(symbol.GetLength ()!= 4 && symbol.GetLength () != 6)
	{
		symbol = symbol.Left (6);
	}
	if(!m_pSymbolToPos->Lookup(symbol,(void*&)i))
		i = -1;
	//end

	//so it maybe a new stock
	if(i == -1)
	{
		//add a new stock to file
		AddNewStockToFile(symbol,pHsSmallHead3);
		if(GetStockNumber() > 0)
			i = GetStockNumber() - 1;
		else
			ASSERT(FALSE);
	}
	else
	{
		int addr = 16;
		this->Seek(addr + i * 48,this->begin);
		pHsSmallHead3 = (HSSMALLHEAD*)this->GetFileCurrentPointer();
	}

	memcpy(pHsSmallHead,pHsSmallHead3,sizeof(HSSMALLHEAD));
	ASSERT(pHsSmallHead3!=NULL);

	return i;
}


bool CTaiKlineFileHS::SetHsSmallHeader(int nIndex,HSSMALLHEAD* pHsSmallHead)
{
	int nStock = this->GetStockNumber ();
//	ASSERT(nIndex<nStock);

	if(nIndex>=nStock)
		return false;

	int addr = 16+nIndex*HsSmallHeadByteEach;
	this->Seek(addr,this->begin);
	CString s(pHsSmallHead->StockSign );
	HSSMALLHEAD *pHsSmallHead2 = (HSSMALLHEAD *)this->GetFileCurrentPointer ();
	CString s2(pHsSmallHead2->StockSign );
	if(s!=s2)
	{
		ASSERT(FALSE);
		return false;
	}
	this->Write (pHsSmallHead,sizeof(HSSMALLHEAD));
	return true;
}

//
void CTaiKlineFileHS::WriteHeaderInfo()//according document data ,to write data to small header
{
	this->SeekToBegin();
	int nStock=0;
	Write(&nStock,4);
	Write(&nStock,4);//the count of block
	nStock=65798809;
	Write(&nStock,4);
	WORD wd = FixedHsPerBlock;
	Write(&wd,2);
	wd=MaxNumStock;
	Write(&wd,2);

	char buff[8192];
	memset(buff,255,MaxNumStock);
	for(int i=0;i<HsSmallHeadByteEach;i++)
		Write(buff,MaxNumStock);

	//write stock small header according the document;

}

void CTaiKlineFileHS::AddNewStockToFile(CString symbol,HSSMALLHEAD*& pHsSmallHead)//add new stock to file
{
	int nStock = this->GetStockNumber ();

	if(nStock>=MaxNumStock)//监测股票个数
	{
		AddSmallHeadBlock();
	}
	nStock = this->GetStockNumber ();
	ASSERT(nStock<=MaxNumStock);
	int addr = 16+nStock*HsSmallHeadByteEach;
	this->Seek(addr,this->begin);
	pHsSmallHead = (HSSMALLHEAD*)this->GetFileCurrentPointer();
	memset(pHsSmallHead,0xff,sizeof(HSSMALLHEAD));
	if(symbol.GetLength ()!=4&&symbol.GetLength ()!=6)
	{
		symbol = symbol.Left (6);
	}
	
	Write(symbol.GetBuffer (8),8);

	int nKline = 0;
	Write(&nKline,4);

	this->Seek(addr,this->begin);
	pHsSmallHead = (HSSMALLHEAD*)this->GetFileCurrentPointer();


	//
	m_pSymbolToPos->SetAt(symbol,(CObject*)nStock);
	nStock++;
	this->SetStockNumber (nStock);



}
int CTaiKlineFileHS::CreateOrMoveSmallBlock(HSSMALLHEAD* pHsSmallHead,int& nBlock)
{

	//if need, to move the small header's block
	int rtn = 0;
	int nMax = 16;//max count of block each stock
	if (nBlock >= nMax)
	{
		return 0;
	}
	ASSERT(nBlock<nMax);
	//if need, to create a new block
//	BYTE* pByte=this->GetFileBeginPointer ();
	if(pHsSmallHead->symBlock[nBlock]==0xffff)
	{
		int nCountBlock = this->GetSmallBlockCount ();
		this->SetSmallBlockCount (nCountBlock+1);
		int addr2 = SMALLHEADLENGTH + nCountBlock 
				* HsByteEach *  FixedHsPerBlock;
									  //计算股票K线的当前位置
		char buff[ HsByteEach *  FixedHsPerBlock];
		memset(buff,255,HsByteEach *  FixedHsPerBlock);
		this->Seek(addr2,this->begin);
		this->Write (buff,HsByteEach *  FixedHsPerBlock);

		ASSERT(nBlock>=0);
		pHsSmallHead->symBlock[nBlock]=(WORD)nCountBlock;
		nCountBlock++;
	}
	return 0;

}

BOOL CTaiKlineFileHS::Open(LPCTSTR lpszFileName, UINT nOpenFlags, int nAddToFileEnd,CFileException* pException)
{	
	if(m_bToday == false)
	{
		CFileFind find;
		if(find.FindFile (lpszFileName) == FALSE)
			return FALSE;
	}
	BOOL bOk = TRUE;
	bOk = CTaiKlineMemFile::Open( lpszFileName,  nOpenFlags,  0,
	 pException);
	if(bOk == FALSE) return bOk;

	//check if the file is newly created;
	DWORD dwLen = this->GetLength ();
	if(dwLen<=SMALLHEADLENGTH || bOk == FALSE)
	{
		this->ReMapFromBegin (SMALLHEADLENGTH+250* HsByteEach *  FixedHsPerBlock);

		//write header infomation 
		WriteHeaderInfo();
	}
	else
	{

	//
		int bID = 1;
		if(GetID()!=65798809) bID =0;
		int nBlock = this->GetSmallBlockCount();
		if(this->GetStockNumber ()>4096 
			|| nBlock>4096*16 
			|| this->GetStockNumber ()<0 
			|| nBlock<0 
			|| bID ==0)
			WriteHeaderInfo();
		else
		{
			int nLen = this->GetLength ();
			nLen -= SMALLHEADLENGTH;
			if(nLen>0)
			{
				nLen = nLen/( HsByteEach *  FixedHsPerBlock);
				if(nLen%( HsByteEach *  FixedHsPerBlock)!=0)
					nLen++;
				if(nBlock>nLen)//需要清理多余的大数据块
				{
					ASSERT(FALSE);//警告程序员
					this->SetSmallBlockCount (nLen);

					int nStock = GetStockNumber ();
					for(int i=0;i<nStock;i++)
					{
						int addr = 16+i*HsSmallHeadByteEach;
						Seek(addr,CTaiKlineMemFile::begin);
						HSSMALLHEAD hsSmallHead;
						HSSMALLHEAD* pHsSmallHead = &hsSmallHead;
//						int nIndexStock = GetHsSmallHeader(symbol,pHsSmallHead);
						Read(pHsSmallHead,sizeof(HSSMALLHEAD));//= (KLINE_SMALLHEAD*)pFile->GetFileCurrentPointer();
						pHsSmallHead->StockSign[6]=0;
						CString symbol (pHsSmallHead->StockSign);
						for(int j=0;j<16;j++)
						{
							if(pHsSmallHead->symBlock [j] == 0xffff) break;
							int iData = -1;
							int iIn = pHsSmallHead->symBlock [j] ;
							if(iIn>=nLen)
							{
								pHsSmallHead->numHS  = (j)*FixedHsPerBlock;
								for(;j<16;j++)
									pHsSmallHead->symBlock [j] = 0xffff;
								this->SetHsSmallHeader  (i,pHsSmallHead);
								break;
							}
						}
					}
				}
			}
		}
		this->ReMapFromBegin (SMALLHEADLENGTH+(nAddToFileEnd+this->GetSmallBlockCount())* HsByteEach *  FixedHsPerBlock);
	}

	MaxNumStock = GetMaxNumStock();
	ASSERT(MaxNumStock == 4096);

	return TRUE;
}

void CTaiKlineFileHS::AddIndexToMap()//add the small header's index to document data
{
	int nStock = this->GetStockNumber();
	m_pSymbolToPos = new CMapStringToPtr((int)(nStock*1.25)+1);
	ASSERT(nStock<=MaxNumStock);
	for(int i=0;i<nStock;i++)
	{
		int addr = 16+i*48;
		this->Seek(addr,this->begin);
		HSSMALLHEAD* pHsSmallHead = (HSSMALLHEAD*)this->GetFileCurrentPointer();
		CString symbol (pHsSmallHead->StockSign);
		m_pSymbolToPos->SetAt(symbol,(CObject*)i);

	}
}


//(CString symbol,CBuySellList& buySellList,bool bClearAll)//读取K线数据
int CTaiKlineFileHS::ReadHS2(CString symbol, CBuySellList &buySellList, bool bClearAll)
{
//	try
	{
	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4) return 0;

	HSSMALLHEAD hsSmallHead;
	HSSMALLHEAD* pHsSmallHead = &hsSmallHead;
	int nIndexStock = GetHsSmallHeader(symbol,pHsSmallHead);

	if(bClearAll==true)
	{
		RemoveHs(buySellList);
	}


	int nList = buySellList.GetCount ();
	if(nList>pHsSmallHead->numHS)
	{
		ASSERT(FALSE);
		return nList ;
	}

	this->SeekToBegin ();
	ASSERT(pHsSmallHead->numHS<=0
		?TRUE:(int)pHsSmallHead->symBlock[(pHsSmallHead->numHS-1) / FixedHsPerBlock ]<0xffff);

	//read now
	for(int i=nList;i<pHsSmallHead->numHS ;i++)
	{
		int blkCount = i / FixedHsPerBlock;	  //计算股票当前块数
		int stockCount = i % FixedHsPerBlock; //计算股票在当前块中的第几条
		int addr = SMALLHEADLENGTH + pHsSmallHead->symBlock[blkCount] 
				* HsByteEach * FixedHsPerBlock + stockCount * HsByteEach;  //计算股票K线的当前位置
		this->Seek(addr,this->begin);
		TRADE_DETAIL_H_PER* pHs = new TRADE_DETAIL_H_PER;
		Read(pHs,HsByteEach);
		if(pHs->price == 0 )
			delete pHs;
		else
			buySellList.AddHead(pHs);
	}
	}
/*	catch(...)
	{
		int n = GetLastError();
		CString s;
		s.Format("readhs 2 ; =%d",n);
		AfxMessageBox(s);
	}
*/
	return buySellList.GetCount();
}

bool CTaiKlineFileHS::WriteHS2(CString symbol, CBuySellList &buySellList)
{
	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4) return 0;

	HSSMALLHEAD hsSmallHead;
	HSSMALLHEAD* pHsSmallHead = &hsSmallHead;
	int nIndexStock = GetHsSmallHeader(symbol,pHsSmallHead);

	int nList = buySellList.GetCount ();

	//read now
	for(int i=0;i<nList ;i++)
	{
		int blkCount = i / FixedHsPerBlock  ;	  //计算股票当前块数
		int stockCount = i % FixedHsPerBlock  ;	  //计算股票在当前块中的第几条
		if(stockCount == 0)
		{
			CString s2(pHsSmallHead->StockSign);
			CreateOrMoveSmallBlock(pHsSmallHead, blkCount);
		}

		int addr = SMALLHEADLENGTH + pHsSmallHead->symBlock[blkCount] 
				* HsByteEach *  FixedHsPerBlock + stockCount * HsByteEach;//计算股票K线的当前位置
		this->Seek(addr,this->begin);
		POSITION pos=buySellList.FindIndex(nList - i - 1 ) ;
		TRADE_DETAIL_H_PER* pHs=buySellList.GetAt( pos );
		Write(pHs,HsByteEach);
	}
	//set header
	pHsSmallHead->numHS = nList;
	this->SetHsSmallHeader (nIndexStock,pHsSmallHead);
	return true;
}

void CTaiKlineFileHS::RemoveHs(CBuySellList &buySellList)
{
	TRADE_DETAIL_H_PER* phs;

	for(;buySellList.IsEmpty() == 0;)
	{
		phs=buySellList.RemoveHead();
		delete phs;
	}
	buySellList.RemoveAll();
}

int CTaiKlineFileHS::GetDataCount(CString symbol)
{
	int nIndex = 0;
	HSSMALLHEAD hsSmallHead;
	HSSMALLHEAD* pHsSmallHead = &hsSmallHead;

	int nIndexStock = GetHsSmallHeader(symbol,pHsSmallHead);
	if( pHsSmallHead == NULL)
		return 0;
	return pHsSmallHead->numHS ;
}

void CTaiKlineFileHS::SetDaysCount(int nDays)
{
	int addr = 8;
	this->Seek(addr,this->begin);
	this->Write(&nDays,4);

}

int CTaiKlineFileHS::GetDaysCount()
{
	int addr = 8;
	int nDays = 0;

	this->Seek(addr,this->begin);
	this->Read(&nDays,4);

	return nDays;
}

int CTaiKlineFileHS::ComputeDaysCount(CBuySellList *buySellList)
{
	//if it is today's
	int n = buySellList->GetCount ();
	if(n == 0)
		return 0;

	POSITION pos=buySellList->FindIndex(n - 1 ) ;
	TRADE_DETAIL_H_PER* pHs=buySellList->GetAt( pos );
	CTime tm = CTime::GetCurrentTime ();
	int tmt = (int)tm.GetTime ();

	if(tmt/(24*60*60) == pHs->time /(24*60*60))
		return 1;
	else if(tmt/(24*60*60) < pHs->time /(24*60*60))
	{
		ASSERT(FALSE);
		return 0;
	}

	//it is histroy
	int nRtn = 1;
	int tmPre = tmt/(24*60*60);
	for(int i=240;i<n;i+=240)
	{
		pos=buySellList->FindIndex(n - i - 1 ) ;
		pHs=buySellList->GetAt( pos );
		if(pHs->time / (24*60*60)>tmPre)
		{
			tmPre= pHs->time / (24*60*60);
			nRtn++;
		}
		else if(pHs->time / (24*60*60)<tmPre)
			ASSERT(FALSE);
	}
	pos=buySellList->FindIndex(0 ) ;
	pHs=buySellList->GetAt( pos );
	if(pHs->time /(24*60*60)>tmPre)
	{
		tmPre= pHs->time /(24*60*60);
		nRtn++;
	}
	else if(pHs->time /(24*60*60)<tmPre)
		ASSERT(FALSE);

	return nRtn;

}

void CTaiKlineFileHS::ZeroHsCountEach()
{
	//look up the small header
	int addr = 16;
	int nStock = this->GetStockNumber ();
	for(int i=0;i<nStock;i++)
	{
		this->Seek(addr,this->begin);
		HSSMALLHEAD* pHsSmallHead = (HSSMALLHEAD*)this->GetFileCurrentPointer();
		memset(pHsSmallHead,0xff,sizeof(HSSMALLHEAD));
		pHsSmallHead->numHS = 0;
		addr+=HsSmallHeadByteEach;
	}
	DeleteMap();
	this->SetStockNumber (0);
	this->SetSmallBlockCount (0);

}
void CTaiKlineFileHS::TransferHs(CBuySellList *pBuySellList, ARRAY_BE &pp, int nMax, int nKindIn, int nOther, KlineEx *pKlineEx)
{
	ASSERT(nMax==pBuySellList->GetCount ());
	if(nMax!=pBuySellList->GetCount ())
		return ;

	int n = nOther;
	if(nOther<1)
		n = 1;
	if(nOther>3)
		n=3;

	TRADE_DETAIL_H_PER *pHsPre = NULL;
	int nFlag=0;
	float f = 0;
	for (int i=pp.b;i<=pp.e;i++)
	{
		TRADE_DETAIL_H_PER* pHs = GetAtBS(pBuySellList, i,nMax);
//		CReportData cdat;
//		Cdat1ToHs(&cdat, pHs, false,pHsPre);
		if(nKindIn == BUYVOL||nKindIn == SELLVOL||nKindIn == ISBUYORDER)
			f =  CTaiKlineTransferKline::GetVolHs(pHs, pHsPre, nFlag);
		pHsPre = pHs;
/*	enum{
		BUYVOL=0,SELLVOL=1,ISBUYORDER=2,
		ASKPRICE=3,ASKVOL=4,BIDPRICE=5,BIDVOL=6
	};
*/		
	//compute nflag : nFlag == -1 是内盘 ；== 0 ； == 1 是外盘
		switch(nKindIn)
		{
		case BUYVOL:
			pp.line[i] = nFlag>0?f:(nFlag==0?f/2:0);
			break;
		case SELLVOL:
			pp.line[i] = nFlag<0?f:(nFlag==0?f/2:0);
			break;
		case ISBUYORDER:
			if(nFlag == -1)
				pp.line[i] = 0;//外盘
			else if(nFlag == 0)
				pp.line[i] = 2;//平盘
			else 
				pp.line[i] = 1;//内盘
			break;
		case ASKPRICE:
			if(n==1)
				pp.line[i] = pKlineEx[i].buyPrice [0];//.pbuy1  ;
			else if(n==2)
				pp.line[i] = pKlineEx[i].buyPrice [1];
			else
				pp.line[i] = pKlineEx[i].buyPrice [2];
			break;
		case ASKVOL:
			if(n==1)
				pp.line[i] = pKlineEx[i].buyVol [0];
			else if(n==2)
				pp.line[i] = pKlineEx[i].buyVol [1];
			else
				pp.line[i] = pKlineEx[i].buyVol [2];
			break;
		case BIDPRICE:
			if(n==1)
				pp.line[i] = pKlineEx[i].sellPrice  [0];
			else if(n==2)
				pp.line[i] = pKlineEx[i].sellPrice [1];
			else
				pp.line[i] = pKlineEx[i].sellPrice [2];
			break;
		case BIDVOL:
			if(n==1)
				pp.line[i] = pKlineEx[i].sellVol [0];
			else if(n==2)
				pp.line[i] = pKlineEx[i].sellVol [1];
			else
				pp.line[i] = pKlineEx[i].sellVol [2];
			break;
		}
	}

}
void CTaiKlineFileHS::HsToCdat1(CReportData *pCdat, Kline *pKline, int nFoot, CBuySellList *pBuySellList)
{
/*	//find the foot of pBuySellList;
	time_t tmt = pKline[nFoot].day/(24*60*60)*(24*60*60);
	int i,n = pBuySellList->GetCount ();
	int nFirst = 0;
	float high=pKline[nFoot].high;
	float low=pKline[nFoot].low ;
	float neipan = 0;
	bool bFirst = false;
	float neipanPre=0;
	for( i=0;i<n;i++)
	{
		TRADE_DETAIL_H_PER* pHs = GetAtBS(pBuySellList, i,n);
		if(pHs->time >=tmt)
		{
			bFirst = true;
			if(nFirst==0&&i!=0)
				nFirst =i;
		}

		if(pKline[nFoot].day<=pHs->time)
		{
			if(i>0)
				i--;
			break;
		}

	//get high low and neipan
		if(bFirst == true)
		{
			if(high<pHs->price )
				high = pHs->price ;
			if(low>pHs->price )
				low = pHs->price ;
			neipan+=(pHs->vol >0?FABSMY(FABSMY(pHs->vol)-neipanPre):0);
			neipanPre = FABSMY(pHs->vol);
		}
	}
	//i is the current foot of list hs
	TRADE_DETAIL_H_PER* pHs = GetAtBS(pBuySellList, i,n);
	Cdat1ToHs(pCdat, pHs, false);
	pCdat->higp = high;
	pCdat->lowp = low;
	pCdat->dvol = neipan;
*/
}

TRADE_DETAIL_H_PER* CTaiKlineFileHS::GetAtBS(CBuySellList *pBuySellList, int nFoot,int nCount)
{
	ASSERT(nFoot<nCount);
	POSITION pos=pBuySellList->FindIndex(nCount - nFoot - 1 ) ;
	TRADE_DETAIL_H_PER* pHs=pBuySellList->GetAt( pos );
	return pHs;
}

bool CTaiKlineFileHS::WriteHS(HSSMALLHEAD* pHsSmallHead,TRADE_DETAIL_H_PER *pHs)
{
	CString symbol(pHsSmallHead->StockSign );//CALLBACK
	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4) return 0;

//	this->SeekToBegin ();
	ASSERT(pHsSmallHead->numHS<=0?TRUE:(int)pHsSmallHead->symBlock[(pHsSmallHead->numHS-1) / FixedHsPerBlock ]<0xffff);

	//write now
	int i = pHsSmallHead->numHS;
	int blkCount = i / FixedHsPerBlock;	  //计算股票当前块数
	int stockCount = i % FixedHsPerBlock; //计算股票在当前块中的第几条
	int addr ;
	if(stockCount == 0)
	{
		CString s2(pHsSmallHead->StockSign);
		CreateOrMoveSmallBlock(pHsSmallHead, blkCount);
	}
	addr = SMALLHEADLENGTH + pHsSmallHead->symBlock[blkCount] 
			* HsByteEach * FixedHsPerBlock + stockCount * HsByteEach;//计算股票K线的当前位置
								  
	this->Seek(addr,this->begin);
	Write(pHs,HsByteEach);
	pHsSmallHead->numHS++;

	return true;

}

void CTaiKlineFileHS::DoCloseWorkHs(CProgressDialog* pDlg)
{
	WriteHistoryHsFile(true,pDlg);
	WriteHistoryHsFile(false,pDlg);
}

CTaiKlineFileHS* CTaiKlineFileHS::GetFilePointer(CString symbol,int stkKind)
{
//	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
//	CTaiShanDoc * pDoc = pFm->m_pDoc ;
	CTaiKlineFileHS*	pFileHs;

	pFileHs=CTaiKlineFileHS::m_pFileHsSh;
	if(SZ_MARKET_EX == CSharesCompute::GetMarketKind(stkKind))
		pFileHs=CTaiKlineFileHS::m_pFileHsSz;

	return pFileHs;

}

void CTaiKlineFileHS::WriteHistoryHsFile(bool bSh,CProgressDialog* pDlg)
{
	CTaiShanDoc * pDoc = CMainFrame::m_pDoc ;

	//look up the count of days of history file
	CString sPath = "data\\historysh\\";
	CString strName = CSharesCompute::GetIndexSymbol(0);
	int nMarket = SH_MARKET_EX;
	if(bSh == false)// ) 
		nMarket = SZ_MARKET_EX;

	if(bSh == false)
	{
		sPath = "data\\historysz\\";
		strName = CSharesCompute::GetIndexSymbol(1);
	}

	CFileFind finder;
	int n = 0;
	BOOL bWorking = finder.FindFile(sPath+"*.hst");
	if(bWorking == FALSE)
	{
		CString s = sPath;
		s.TrimRight ("\\");
		::CreateDirectory (s,NULL);
	}

	CStringArray sArray;
	while(bWorking)
	{
	  bWorking = finder.FindNextFile();
	  CString filename = finder.GetFileTitle();
	  int nSize = sArray.GetSize ();
	  if(nSize == 0 )
		  sArray.Add (filename);
	  else
		  for(int j=0;j<nSize;j++)
		  {
			  if(filename <sArray[j])
			  {
				  sArray.InsertAt (j,filename);
				  break;
			  }
			  if(j==nSize-1)
				sArray.Add (filename);
		  };
	  n++;
	}

	CString strMax="";
	CString strMin="";
	if(sArray.GetSize ()>0)
	{
		strMax = sArray[0];
		strMin = sArray[0];
	}
	int i;
	for(i=1;i<sArray.GetSize ();i++)
	{
		if(strMax<sArray[i])
			strMax=sArray[i];
		if(strMin>sArray[i])
			strMin=sArray[i];
	}

	//to delete old file and to create new file
	//delete 
	CTime tm = CTime::GetCurrentTime ();
	CString sToday = tm.Format("%Y%m%d");
	if(sToday == strMax)
	{
		CString s = sPath+strMax+".hst";
		//must first close history file
		::DeleteFile (s);
	}
	if(n>=pDoc->m_propertyInitiate .countOfHistoryFile)
	{
		for(int k=0;k<n-pDoc->m_propertyInitiate .countOfHistoryFile;k++)
		{
			if(sArray.GetSize()<=0) break;
			CString s = sPath+sArray[0]+".hst";
			//must first close history file
			::DeleteFile (s);
			sArray.RemoveAt (0);
		}
	}

	//create
	CTaiKlineFileHS fileHist(nMarket);
	sToday = sPath+sToday+".hst";
	fileHist.Open (sToday,0);

	CTaiKlineFileHS* pFileToday = m_pFileHsSh;//GetFilePointer(strName);
	if(bSh == false) pFileToday = m_pFileHsSz;

	fileHist.ReMapFromBegin (SMALLHEADLENGTH+(pFileToday->GetSmallBlockCount())* HsByteEach *  FixedHsPerBlock/2);

	//copy file
	int addr = 16;
	int nStock = pFileToday->GetStockNumber();

	CBuySellList buySellList;
	for( i=0;i<nStock;i++)
	{

		fileHist.Seek(addr,begin);
		pFileToday->Seek(addr,begin);
		HSSMALLHEAD* pHsSmallHead = (HSSMALLHEAD*)fileHist.GetFileCurrentPointer();
		HSSMALLHEAD* pHsSmallHeadToday = (HSSMALLHEAD*)pFileToday->GetFileCurrentPointer();
		CString symbol(pHsSmallHeadToday->StockSign );
		pHsSmallHead->numHS =0;
		strcpy(pHsSmallHead->StockSign ,symbol);

		//read data from pFileToday
		if(CTaiShanKlineShowView::IsIndexStock3(symbol))
			ASSERT(TRUE);
		pFileToday->ReadHS (symbol,buySellList,true);

		//write to history file
		fileHist.WriteHS2( symbol, buySellList);

		addr+=HsSmallHeadByteEach;

//************显示股票处理信息**********************************
		int spos=100*i /nStock   ;
		pDlg->DisplayZqxx(symbol, spos); 
//**************************************************************
	}

	RemoveHs(buySellList);
	fileHist.Close ();

}

int CTaiKlineFileHS::ReadIndexHS(CString symbol, CBuySellList &buySellList, bool bClearAll)
{
	//read data from document
	if(symbol.GetLength ()!=6&&symbol.GetLength ()!=4) return 0;

	if(bClearAll==true)
	{
		RemoveHs(buySellList);
	}
	else
		return 0;


	int nList = buySellList.GetCount ();
	ASSERT(nList==0);

	//read now
	 char *strLabel = symbol.GetBuffer (0);
	 int nMarket = m_nMarket;
	 int stkKind = CSharesInformation::GetStockKind(nMarket,strLabel);
	CReportData* pdt=NULL;
	if(pDoc->m_sharesInformation.Lookup(symbol.GetBuffer (0),pdt,stkKind)==0)
		return 0;
	int isSz=0;
	if(CSharesCompute::GetMarketKind(stkKind) == SZ_MARKET_EX) isSz=1;

	Rsdn1 **Nidx;
	pDoc->m_sharesInformation.GetIndexRsdn(Nidx); 
	Tidxd **Tidx;
	pDoc->m_sharesInformation.GetIndexTidxd(Tidx); 

	CTime tm = CTime::GetCurrentTime ();
	int tmt = (int)tm.GetTime ();
	tmt = tmt/(24*60*60)*24*60*60+((9-8)*60+30)*60;

	if(pdt->ystc<=0 || pdt->opnp<=0)
		return 0;

	float f = 0;
	if(pdt->m_Kdata1 [0].Price <=0)
		f = pdt->opnp  ;
	int footEnd = pDoc->m_nANT [0]+1;
	if(pDoc->m_bInitDone ==FALSE)
		footEnd=pDoc->m_nOldANT [0]+1;
	if(footEnd>240) footEnd = 240;
	for(int i=0;i<footEnd ;i++)
	{
		TRADE_DETAIL_H_PER* pHs = new TRADE_DETAIL_H_PER;

		if(pdt->m_Kdata1 [i].Price>0 )
		{
			f = pdt->m_Kdata1 [i].Price;
/*			pdt->m_Kdata1 [i].Price = pdt->m_Kdata1 [i-1].Price ;
			pdt->m_Kdata1 [i].Volume=pdt->m_Kdata1 [i-1].Volume;
			pdt->m_Kdata1 [i].Amount  = pdt->m_Kdata1 [i-1].Amount ;*/
		}
		pHs->price = f ;
//		f = pdt->m_Kdata1 [i].Price;
		pHs->vol = pdt->m_Kdata1 [i].Volume  ;
		pHs->time = tmt+60*i ;
		if(i>=120)
				pHs->time  = tmt+60*i+90*60 ;
		pHs->buySellOrIndex .indexInfo .amount   = pdt->m_Kdata1 [i].Amount   ;
		pHs->buySellOrIndex .indexInfo .advance  = Nidx[isSz][i].rsn ;
		pHs->buySellOrIndex .indexInfo .decline  = Nidx[isSz][i].dnn ;
		pHs->buySellOrIndex .indexInfo .tidxd .dp  = Tidx[isSz][i].dp  ;
		pHs->buySellOrIndex .indexInfo .tidxd .res   = Tidx[isSz][i].res  ;
		pHs->buySellOrIndex .indexInfo .tidxd .rp   = Tidx[isSz][i].rp  ;
		pHs->buySellOrIndex .indexInfo .tidxd .sec5   = Tidx[isSz][i].sec5  ;

		if(i==0)
			pHs->buySellOrIndex .indexInfo .faccbaccs    = pdt->accb   ;
		if(i==1)
			pHs->buySellOrIndex .indexInfo .faccbaccs    = pdt->accs   ;

		buySellList.AddHead(pHs);
	}

	return buySellList.GetCount ();
}

bool CTaiKlineFileHS::TransferDataEx(CBuySellList *pBuySellList, KlineEx *&pKlineEx)
{
	if(pKlineEx!=NULL)
		delete [] pKlineEx;
	int nMax = pBuySellList->GetCount ();
	pKlineEx= new KlineEx[nMax];

	int n ;

	TRADE_DETAIL_H_PER *pHsPre = NULL;
	for (int i=0;i<nMax;i++)
	{
		TRADE_DETAIL_H_PER* pHs = GetAtBS(pBuySellList, i,nMax);

		if(pHsPre )
			if(pHs->vol - pHsPre->vol == 430)
			{
				int k = 20;
				k = 90;
			}
		
		CReportData cdat;
		Cdat1ToHs(&cdat, pHs, false,pHsPre);
		float* pf = &(cdat.pbuy1);

		pHsPre = pHs;
		//for(int j=0;j<3;j++)//?marenan 3要改成5或10档行情
		//{
		//	pKlineEx[i].fDataEx [j] = pf[j*2];
		//	pKlineEx[i].fDataEx [j+3] = pf[j*2+1];
		//	pKlineEx[i].fDataEx [j+6] = pf[j*2+6];
		//	pKlineEx[i].fDataEx [j+9] = pf[j*2+6+1];
		//}
		for(int j=0;j<5;j++)//?marenan 3要改成5或10档行情
		{
			pKlineEx[i].fDataEx [j] = pf[j*2];
			pKlineEx[i].fDataEx [j+5] = pf[j*2+1];
			pKlineEx[i].fDataEx [j+10] = pf[j*2+10];
			pKlineEx[i].fDataEx [j+15] = pf[j*2+10+1];
		}	
	}

	return true;

}
//bool CTaiKlineFileHS::BillTransferDataEx(CTaiKlineFileHS* pFileHs,CBuySellList *pBuySellList, KlineEx *&pKlineEx)
//void CTaiKlineDlgHistorySelect::GetFileNameArray(CStringArray &sArry)
//CTaiKlineFileHS* BillOpenFileHs(CString strTime,bool bShangHai,bool bToday)
bool CTaiKlineFileHS::BillTransferDataEx(CTaiKlineFileHS *pFileHs, char *pStockCode, CBuySellList *pBuySellList, KlineEx *&pKlineEx)
//pKlineEx的第一次初值应该是NULL，函数内部用new生成，需要在函数外删除
//pBuySellList用完后需要清空（CTaiKlineFileHS::RemoveHs(*buySellList)）
//GetAtBS(CBuySellList *pBuySellList, int nFoot,int nCount)
{
//int CTaiKlineFileHS::ReadHS2(CString symbol, CBuySellList &buySellList, bool bClearAll)
	if(pFileHs ==NULL)
		return false;

	CString symbol = pStockCode;
	pFileHs->ReadHS2 (symbol,*pBuySellList,true);

	pFileHs->TransferDataEx(pBuySellList, pKlineEx);

	return true;
}

CTaiKlineFileHS* CTaiKlineFileHS::BillOpenFileHs(CString strTime, bool bShangHai, bool bToday)
//返回值在函数内部用new生成，需要在函数外删除
//strTime可用CTaiKlineDlgHistorySelect::GetFileNameArray(CStringArray &sArry)获得
{
	CString sPath;
	int nMarket = SH_MARKET_EX;
	sPath = "data\\historysh\\";
	if(bShangHai==false)
	{
		nMarket = SZ_MARKET_EX;
		sPath = "data\\historysz\\";
	}
	sPath += strTime;
	sPath += ".hst";

	if(bToday == true)
	{
		if(bShangHai == false)
			sPath = g_buysellsz;
		else
			sPath = g_buysellsh;
	}
	CFileFind find;
	if(find.FindFile(sPath) == FALSE) return NULL;

	CTaiKlineFileHS*	pFile = new CTaiKlineFileHS(nMarket,bToday);//sh
	if(!pFile->Open (sPath,0))
	{
		delete pFile;
		return NULL;
	}
	return pFile;
}

int CTaiKlineFileHS::GetMinuteSecond(int timet)
{
	CTime tm(timet);
	return atoi(tm.Format ("%H%M"));
}

void CTaiKlineFileHS::DeleteMap()
{
	if(m_pSymbolToPos!=NULL)
	{
		m_pSymbolToPos->RemoveAll ();
		delete m_pSymbolToPos;
		m_pSymbolToPos = NULL;
	}
}

bool CTaiKlineFileHS::OpenAll()
{
	m_pFileHsSh	= new CTaiKlineFileHS(SH_MARKET_EX);
	m_pFileHsSz	= new CTaiKlineFileHS(SZ_MARKET_EX);

	CString sPath2 ;
	sPath2 = g_buysellsh;
	bool bRtn = true;
	if(!m_pFileHsSh->Open(sPath2, 0,NULL))
	{
		delete m_pFileHsSh;
		m_pFileHsSh=NULL;
		bRtn = false;
		AfxMessageBox("打开文件时出错，请关闭程序后，删除文件 data\\sh\\tick.dat ",MB_OK|MB_ICONERROR);
	}
	sPath2 = g_buysellsz;
	if(!m_pFileHsSz->Open(sPath2, 0,NULL))
	{
		delete m_pFileHsSz;
		m_pFileHsSz=NULL;
		bRtn = false;
		AfxMessageBox("打开文件时出错",MB_OK|MB_ICONERROR);
	}

	return bRtn;

}

void CTaiKlineFileHS::CloseAll()
{
	if(m_pFileHsSh != NULL)
	{
		m_pFileHsSh->Close();
		delete m_pFileHsSh;
		m_pFileHsSh = NULL;
	}
	if(m_pFileHsSz != NULL)
	{
		m_pFileHsSz->Close();
		delete m_pFileHsSz;
		m_pFileHsSz = NULL;
	}
}

int CTaiKlineFileHS::GetCountPre(CString symbol,int stkKind)
{
	int n = 0;
	HSSMALLHEAD hsSmallHead;
	memset(&hsSmallHead,0,sizeof(HSSMALLHEAD));
	HSSMALLHEAD* pHsSmallHead = &hsSmallHead;
	CTaiKlineFileHS* pFile = GetFilePointer( symbol,stkKind);
	int nIndexStock = pFile->GetHsSmallHeader(symbol,pHsSmallHead);
	n = hsSmallHead.numReserved ;
	return n;
}

bool CTaiKlineFileHS::IsNeedHsHistData(CString symbol,int stkKind, CString sDate)
{
	bool brtn = true;
	CTaiShanDoc * pDoc = CMainFrame::m_pDoc ;

	//look up the count of days of history file
	CString sPath = "data\\historysh\\";
//	CString strName = CSharesCompute::GetIndexSymbol(0);
	bool bSh = true;
	int nMarket = SH_MARKET_EX;
	if(CSharesCompute::GetMarketKind(stkKind) == SZ_MARKET_EX)
	{
		nMarket = SZ_MARKET_EX;
		bSh = false;
	}
	if(bSh == false)
	{
		sPath = "data\\historysz\\";
//		strName = CSharesCompute::GetIndexSymbol(1);
	}

	CFileFind finder;
	
//	CString fileName = 
	int n = 0;
	BOOL bWorking = finder.FindFile(sPath + sDate + ".hst");
	if(bWorking == FALSE)
	{
		;//brtn = false;
	}
	else
	{
		HSSMALLHEAD hsSmallHead;
		memset(&hsSmallHead,0,sizeof(HSSMALLHEAD));
		HSSMALLHEAD* pHsSmallHead = &hsSmallHead;
		CTaiKlineFileHS fileHist(nMarket);
		CString sName = sPath+sDate+".hst";
		fileHist.Open (sName,0);
		int nIndexStock = fileHist.GetHsSmallHeader(symbol,pHsSmallHead);
		n = hsSmallHead.numHS  ;
		if(n > 0)
			brtn = false;
		
	}
	return brtn;
}
//RCV_DISPBARGAINING_STRUCTEx//m_nPacketNum
//WriteHsDataWideNet(RCV_DISPBARGAINING_STRUCTEx* pData,int nPacketNum)
/*void CSharesCompute::StockDataDayUpdate(DAY_TOTAL_STRUCTEx * m_GpDay)                  //股票处理程序             
{
    int index=0;
	if(m_GpDay->m_RcvDay->m_head.m_dwHeadTag != EKE_HEAD_TAG)
		return;
	do
	{
        int  endindex=CheckStockDaytime(m_GpDay,index) ; 
		if(endindex <= index&&index<m_GpDay->Day_Count)
		{
			index++;
			continue;
		}
		else if(endindex <= index)
			break;
		int KLineCount=0;
		Kline *pDay=NULL;
        CString StockId;
		HGLOBAL	hMem;
		LPVOID hp;
		hMem = GlobalAlloc( GPTR, (endindex - index  )* sizeof(Kline ) );
		hp=GlobalLock(hMem);
		if( hp )
			pDay= (Kline *)hp;
		else
			AfxMessageBox("分配内存出错",MB_ICONSTOP);
		if(m_GpDay->m_RcvDay[index].m_head.m_dwHeadTag == EKE_HEAD_TAG)
		   StockId=m_GpDay->m_RcvDay[index].m_head.m_szLabel;
		index++;
		do
		{
			pDay[KLineCount].day =m_GpDay->m_RcvDay[index].m_time ;
			pDay[KLineCount].open =m_GpDay->m_RcvDay[index].m_fOpen ;
			pDay[KLineCount].high=m_GpDay->m_RcvDay[index].m_fHigh  ;
			pDay[KLineCount].low =m_GpDay->m_RcvDay[index].m_fLow  ;
			pDay[KLineCount].close =m_GpDay->m_RcvDay[index].m_fClose ;
			pDay[KLineCount].vol =m_GpDay->m_RcvDay[index].m_fVolume ;
			pDay[KLineCount].amount =m_GpDay->m_RcvDay[index].m_fAmount  ;
	        pDay[KLineCount].advance =m_GpDay->m_RcvDay[index].m_wAdvance ;
		    pDay[KLineCount].decline =m_GpDay->m_RcvDay[index].m_wDecline ;
			KLineCount++;
			index++;
		}while(m_GpDay->m_RcvDay[index].m_head.m_dwHeadTag != EKE_HEAD_TAG && index<m_GpDay->Day_Count);

	    GlobalUnlock((HGLOBAL)pDay);        //释放内存
		GlobalFree( (HGLOBAL)pDay);
	}while(index<m_GpDay->Day_Count);
}
	    int ls_pos=index;
		do
		{
			ls_pos++;
		}while(m_GpDay->m_RcvDay[ls_pos].m_head.m_dwHeadTag != EKE_HEAD_TAG && ls_pos<m_GpDay->Day_Count);
		ls_pos--;
*/
//(CString symbol, CString sDate)//WriteHsDataWideNet(CString symbol, CString sDate,RCV_DISPBARGAINING_STRUCTEx *pData,  int nRecord)

bool CTaiKlineFileHS::WriteHsDataWideNet(RCV_DISPBARGAINING_STRUCTEx *pData,int nPacketNum,int nRequest)
{
    int index = 0;

	if(pData->m_head.m_dwHeadTag != EKE_HEAD_TAG)
		return false;

	bool bHistory = false;
	if(nRequest > 19800101)
		bHistory = true;

	while(index < nPacketNum)
	{
		//得到股票代码
        CString StockId;
		int nMarket;
		if(pData[index].m_head.m_dwHeadTag == EKE_HEAD_TAG)
		{
		   StockId = pData[index].m_head.m_szLabel;
		   nMarket = pData[index].m_head.m_wMarket;
		}
		else 
			break;
		index++;

		//得到数据个数
        int  endindex = index; 
		while(pData[endindex].m_head.m_dwHeadTag != EKE_HEAD_TAG && endindex < nPacketNum)
		{
			endindex++;
		}

		//把数据保存到文件
		{
			int nDataCount = endindex - index;
			if(nDataCount <= 0) continue;

			//确定数据的时间
			//int tmt = 
			 CString sDate ="";
			if(bHistory == false)
			{
			}
			else
			{
				CTime tm(pData[index].m_time );
				sDate = tm.Format ("%Y%m%d");
			}
			WriteHsToFileWideNet(StockId,nMarket, sDate, pData+index, nDataCount,nRequest);
		}

		//退出
		index = endindex;
	}
	return true;
}

void CTaiKlineFileHS::WriteHsToFileWideNet(CString symbol,int nMarket, CString sDate, RCV_DISPBARGAINING_STRUCTEx *pData, int nRecord,int nRequest)
{
	int n = 0;
	if(sDate == "")
	{
		CTaiKlineFileHS* pFile = CTaiKlineFileHS::GetFilePointer2(nMarket);
		n = pFile->WriteHsToFile( symbol, pData,  nRecord);

		//set valid data count
		if(nRequest < 2000 )
		{
			HSSMALLHEAD hsSmallHead;
			memset(&hsSmallHead,0,sizeof(HSSMALLHEAD));
			HSSMALLHEAD* pHsSmallHead = &hsSmallHead;
			int nIndexStock = pFile->GetHsSmallHeader(symbol,pHsSmallHead);
			hsSmallHead.numReserved = n;
			pFile->SetHsSmallHeader (nIndexStock,pHsSmallHead);
		}

		if(nRequest == 0 )
		{
			CTime tm(pData->m_time);
			CTime tm2 = CTime::GetCurrentTime();
			if(tm.GetDay()==tm2.GetDay()
				&&tm.GetMonth()==tm2.GetMonth()
				&&tm.GetYear()==tm2.GetYear())
				sDate=tm.Format("%Y%m%d");
		}
	}
	if(sDate!="")
	{
		//历史明细
		CString sPath = FILE_PATH_HSH_SH;
		bool bSh = true;
		char *strLabel = symbol.GetBuffer(0);
		int stkKind2 = CSharesInformation::GetStockKind(nMarket,strLabel);
		int nMarket = SH_MARKET_EX;
		if(CSharesCompute::GetMarketKind(stkKind2) != SH_MARKET_EX)
		{
			nMarket = SZ_MARKET_EX;
			bSh = false;
		}

		if(bSh == false)
		{
			sPath = FILE_PATH_HSH_SZ;
		}
		CTaiKlineFileHS fileHsSh(bSh);// = new CTaiKlineFileHS(true);

		{
			//open file
			CString filename =sPath + sDate +".hst";
			if(fileHsSh.Open (filename,0))
			{
				n = fileHsSh.WriteHsToFile( symbol, pData,  nRecord);
			}
		}
	}

	//to test in param "nRequest"

//	return n;

}

int CTaiKlineFileHS::WriteHsToFile(CString symbol, RCV_DISPBARGAINING_STRUCTEx *pData, int nRecord)
{
//	int n =  GetCountPre(symbol);
	int n = 0;
	HSSMALLHEAD hsSmallHead;

	memset(&hsSmallHead,0,sizeof(HSSMALLHEAD));
	HSSMALLHEAD* pHsSmallHead = &hsSmallHead;
	int nIndexStock = GetHsSmallHeader(symbol,pHsSmallHead);
	n = hsSmallHead.numReserved ;
//end

	CBuySellList buySellList;
	 ReadHS2(symbol, buySellList,false);

	bool bWriteDirect = false;
	int nCountFile = buySellList.GetCount();
	if(nCountFile>0)
	{
		TRADE_DETAIL_H_PER* pHs = CTaiKlineFileHS::GetAtBS(&buySellList, nCountFile-1,nCountFile);
		if(pData[0].m_time>=pHs->time && pData[0].m_fVolume >=pHs->vol )
			bWriteDirect = true;
	}
	else
		bWriteDirect = true;

	int k=0;
	int i;
	for(i = 0;i<nRecord;i++)
	{
		TRADE_DETAIL_H_PER * pHs = new TRADE_DETAIL_H_PER;
		CReportData dat1;
		dat1.nowp = pData[i].m_fNewPrice;
		dat1.totv  = pData[i].m_fVolume;
		dat1.pbuy1  = pData[i].m_fBuyPrice[0];
		dat1.pbuy2  = pData[i].m_fBuyPrice[1];
		dat1.pbuy3  = pData[i].m_fBuyPrice[2];
		dat1.psel1  = pData[i].m_fSellPrice[0];
		dat1.psel2  = pData[i].m_fSellPrice[1];
		dat1.psel3  = pData[i].m_fSellPrice[2];
		dat1.vbuy1   = pData[i].m_fBuyVolume[0];
		dat1.vbuy2   = pData[i].m_fBuyVolume[1];
		dat1.vbuy3   = pData[i].m_fBuyVolume[2];
		dat1.vsel1   = pData[i].m_fSellVolume[0];
		dat1.vsel2   = pData[i].m_fSellVolume[1];
		dat1.vsel3   = pData[i].m_fSellVolume[2];

		Cdat1ToHs(&dat1, pHs,true, NULL);

		pHs->time = pData[i].m_time ;

		if(bWriteDirect)
		{
			buySellList.AddHead (pHs);
		}
		else
		{
			int j;
			for(j=0;j<buySellList.GetCount ();j++)
			{
				TRADE_DETAIL_H_PER* pHs2 = 
					CTaiKlineFileHS::GetAtBS(&buySellList, j,buySellList.GetCount ());
				if(pHs->vol > pHs2->vol )
				{
					break;							
				}
			}
			if(j<buySellList.GetCount ())
			{
				ASSERT(buySellList.GetCount ()>0);
				j++;
				while(j<buySellList.GetCount ())
				{
					delete buySellList.RemoveHead ();
				}

			}
			bWriteDirect = true;
			buySellList.AddHead (pHs);

		}
	}

	//write now
	 WriteHS2(symbol, buySellList);
	 int nRtn = buySellList.GetCount ();
	 RemoveHs (buySellList);
	 return nRtn;

}

void CTaiKlineFileHS::AddSmallHeadBlock()
{
	ASSERT(FALSE);
	int n = (MaxNumStock - 4096)/(240/2);
	if(n < 0) return;

	//1.把有效数据复制到新的块中
	int nStock = GetStockNumber ();
//	HSSMALLHEAD hsSmallHead;
	HSSMALLHEAD klineSmallHead;
	int j = 0;
	int i = 0;
	bool bFound = false;
	for(i = 0;i < nStock;i++)
	{
		int addr = 16;
		Seek(addr + i * HsSmallHeadByteEach,begin);
		Read(&klineSmallHead,sizeof(HSSMALLHEAD));
		for(j = 0;j < 16;j++)
		{
			if(klineSmallHead.symBlock [j] == n)
			{
				bFound = true;
				break;
			}
		}

		if(bFound == true)
			break;
	}

	int nNewBlock = this->GetSmallBlockCount();
	if(bFound == true)
	{
		TRADE_DETAIL_H_PER kline;
		for(int k = 0;k < FixedHsPerBlock;k++)
		{
			int blkCount = j;	  //计算股票当前块数
			int stockCount = k;	  //计算股票在当前块中的第几条
			int addr = SMALLHEADLENGTH + klineSmallHead.symBlock[blkCount] 
					* HsByteEach *  FixedHsPerBlock + stockCount * HsByteEach;
										  //计算股票K线的当前位置
			this->Seek(addr,this->begin);
			Read(&kline,HsByteEach);
			addr = SMALLHEADLENGTH + klineSmallHead.symBlock[nNewBlock] 
					* HsByteEach *  FixedHsPerBlock + stockCount * HsByteEach;
			this->Seek(addr,this->begin);
			Write(&kline,HsByteEach);
		}

		//write small block
		klineSmallHead.symBlock[j] = (WORD)nNewBlock;
		this->SetHsSmallHeader(i, &klineSmallHead);
		//
		nNewBlock++;
		SetSmallBlockCount(nNewBlock);
	}

	//2.
	MaxNumStock += 120;
	WORD nMaxNumStock = MaxNumStock;
	SetMaxNumStock(nMaxNumStock);

}

CTaiKlineFileHS* CTaiKlineFileHS::GetFilePointer2(int nMarket)
{
	CTaiKlineFileHS*	pFileHs = NULL;
	pFileHs = CTaiKlineFileHS::m_pFileHsSh ;

	if(SH_MARKET_EX == nMarket)
		pFileHs = CTaiKlineFileHS::m_pFileHsSh ;
	else if(SZ_MARKET_EX == nMarket)
		pFileHs = CTaiKlineFileHS::m_pFileHsSz ;

	return pFileHs;

}

int CTaiKlineFileHS::GetDataCountAll(CString symbol, int stkKind)
{
	CTaiKlineFileHS* pFile = GetFilePointer( symbol,stkKind);
	return pFile->GetDataCount (symbol);
}

void CTaiKlineFileHS::Symbol4To6()
{
	CFileFind finder;
	CString sPath = "data\\historysz\\";
	BOOL bWorking = finder.FindFile(sPath+"*.hst");

	if(bWorking == FALSE)
	{
		CString s = sPath;
		s.TrimRight ("\\");
		::CreateDirectory (s,NULL);
	}
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		CString filename = finder.GetFilePath();
		CTaiKlineFileHS fl(SZ_MARKET_EX);
		if(fl.Open (filename,CFile::modeRead))
		{

			int nStock = fl.GetStockNumber();

			for(int i = 0;i < nStock;i++)
			{
				int addr = 16 + i * HsSmallHeadByteEach;
				fl.Seek(addr,fl.begin);
				char ch[8];
				ch[7]=0;
				fl.Read (ch,8);
				CString s = ch;
				if(s.GetLength() <= 4) 
				{
					s = CSharesInformation::Symbol4To6(s);//"00" + s;
					strcpy(ch,s);
					fl.Seek(addr,fl.begin);
					fl.Write (ch,8);
				}
			}
		}
	}
}
