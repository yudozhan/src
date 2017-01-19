// CTaiKlineTransferKline.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineTransferKline.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "CTaiKlineFileHS.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiKlineDoFenshi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineTransferKline//Split

IMPLEMENT_DYNCREATE(CTaiKlineTransferKline, CCmdTarget)

BEGIN_MESSAGE_MAP(CTaiKlineTransferKline, CCmdTarget)
	//{{AFX_MSG_MAP(CTaiKlineTransferKline)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTaiKlineTransferKline::CTaiKlineTransferKline()
{
	m_stkKindOld = 0;
	m_bCalPowerToday = true;
	m_klineOld.day = -1;
	pBuySellList = NULL;
	pDoc = CMainFrame::m_pDoc ;
	m_bRemoveBuySellList = false;
	m_bUseMinute1Data = false;
	m_pReportData = NULL;	//个股的数据指针
	m_symbolOld = "";
}
CTaiKlineTransferKline::CTaiKlineTransferKline(CBuySellList* pBSList)          //  constructor 
{
	m_bCalPowerToday = true;
	m_klineOld.day = -1;
	pDoc = CMainFrame::m_pDoc ;
	m_bRemoveBuySellList = false;
	pBuySellList = pBSList;
	m_bUseMinute1Data = false;
	m_pReportData = NULL;	//个股的数据指针
	m_symbolOld = "";
	m_stkKindOld=0;
}
CTaiKlineTransferKline::~CTaiKlineTransferKline()
{
	if(	m_bRemoveBuySellList == true )
	{
		CTaiKlineFileHS::RemoveHs(*pBuySellList);
		delete pBuySellList;
	}

}


/////////////////////////////////////////////////////////////////////////////
// CTaiKlineTransferKline message handlers
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：TransferKlineHistory(CString symbol,Kline *&pKlineHistory, int& nKlineInOut, int nTypeTarget, bool bToday)
//      功能说明：used to transfer k line data to another kind of k line
//      参数：pKlineHistory[in][out]:it is the pointer of k line data;
//				  nKlineInOut[in][out]:it is the number of k line data;
//				  nTypeTarget[in]:it is the target type of k line data;
//				  bToday[in]:it is the flag if it need tranfer data of today;
//      返回参数：
/////////////////////////////////////////////////////////////////////////////

bool CTaiKlineTransferKline::TransferKlineHistory(CString symbol,int stkKind,Kline *&pKlineHistory, int& nKlineInOut, int nTypeTarget, bool bToday,bool bSplit)//used to transfer k line data to another kind of k line
{
	m_klineOld.day = -1;
	if(nKlineInOut <=0)
	{
		if(bToday == true)
			TransferKlineToday(symbol,stkKind,pKlineHistory,nKlineInOut,nTypeTarget);
		return true;
	}

	if(bSplit == true)
	{
		if(nTypeTarget>=1 &&nTypeTarget<=8 ||pDoc->m_propertyInitiate.daysOfManyKline>0&&nTypeTarget==MANY_DAY_KLINE)
		{
			powerArr splitArr;
			ReadPower(pDoc,symbol,stkKind, splitArr);
			ComputePower(pKlineHistory, nKlineInOut,  nTypeTarget, splitArr);
		}
	}

	if(nKlineInOut==0)
	{
		if(bToday == true)
			TransferKlineToday(symbol,stkKind,pKlineHistory,nKlineInOut,nTypeTarget);
		return true;
	}

	CTime tmB(1970,1,1,9,30,0);
	CTime tmNow;
	tmNow=tmB+pKlineHistory[0].day;
	CTime tmEnd;
	CTimeSpan tmAll;
	int i;
	int len=0;
	int nDayOfWeek=1;

	int nDay=pKlineHistory[nKlineInOut - 1 ].day/(24*60*60);//m_minGridX
	int nCountDay = 0;
	int j=0,k;
	bool bFirst=true;
	int nKindStock = 2;

	if(symbol==CSharesCompute::GetIndexSymbol(1)||symbol==CSharesCompute::GetIndexSymbol(2))
		nKindStock = 1;
	if(symbol==CSharesCompute::GetIndexSymbol(0))
		nKindStock = 0;
	switch (nTypeTarget)
	{
	case 2:
	case 3:
	case 4:
		if(nTypeTarget==2)
			k=3;
		else if(nTypeTarget==3)
			k=6;
		else if(nTypeTarget==4)
			k=12;

		for(i=0;i<=nKlineInOut - 1;i++)
		{
			int n;
			n=(i)%k;

			InPutValue(pKlineHistory,i,j,n==0,nCountDay,nKindStock);
		}
		break;
	case 6:
		tmEnd=tmNow;//tmEnd为第一天
		nDayOfWeek=tmNow.GetDayOfWeek ()-1;

		for(i=0;i<=nKlineInOut - 1;i++)
		{
			int nWeek1,nWeek2;
			if(i>0)
			{
				tmNow=tmB+pKlineHistory[i-1].day;
				tmAll=tmNow-tmEnd;//tmAll为时间跨度
				nWeek1=(tmAll.GetDays ()+nDayOfWeek)/7;
			}
			else
				nWeek1=-1;

			int nMonth=tmNow.GetMonth ();
			int nYear=tmNow.GetYear ();

			tmNow=tmB+pKlineHistory[i].day;
			tmAll=tmNow-tmEnd;
			nWeek2=(tmAll.GetDays ()+nDayOfWeek)/7;


			InPutValue(pKlineHistory,i,j,nWeek1!=nWeek2,nCountDay,nKindStock);
			if(i==nKlineInOut - 1)
				m_klineOld = pKlineHistory[j-1];
		}
		break;
	case 7:
		for(i=0;i<=nKlineInOut - 1;i++)
		{
			int nMonth,nYear;
			if(i>0)
			{
				tmNow=tmB+pKlineHistory[i-1].day;
				nMonth=tmNow.GetMonth ();
				nYear=tmNow.GetYear ();
			}
			else
			{
				nYear=1970;
				nMonth=2;
			}

			tmNow=tmB+pKlineHistory[i].day;

			InPutValue(pKlineHistory,i,j,nMonth!=tmNow.GetMonth ()||nYear!=tmNow.GetYear (),nCountDay,nKindStock);
			if(i==nKlineInOut - 1)
				m_klineOld = pKlineHistory[j-1];
		}
		break;
	case 8:
		for(i=0;i<=nKlineInOut - 1;i++)
		{
			int nYear;
			if(i>0)
			{
				tmNow=tmB+pKlineHistory[i-1].day;
				nYear=tmNow.GetYear ();
			}
			else
				nYear=1971;

			tmNow=tmB+pKlineHistory[i].day;

			InPutValue(pKlineHistory,i,j,nYear!=tmNow.GetYear (),nCountDay,nKindStock);
			if(i==nKlineInOut - 1)
				m_klineOld = pKlineHistory[j-1];

		}
		break;
	case MANY_DAY_KLINE:
		if(pDoc->m_propertyInitiate.daysOfManyKline>0)
		{
			for(i=0;i<=nKlineInOut - 1;i++)
			{
				int n = (i)%abs(pDoc->m_propertyInitiate.daysOfManyKline);
				InPutValue(pKlineHistory,i,j,n==0,nCountDay,nKindStock);
				if(i==nKlineInOut - 1)
					m_klineOld = pKlineHistory[j-1];
			}
		}
		break;
	case 1:
	case 5:
	case MIN1_KLINE://need change to do history
	case HS_KLINE:
		j=nKlineInOut;
		break;
	}

	nKlineInOut =j;
	if(bToday == true)
		TransferKlineToday(symbol,stkKind,pKlineHistory,nKlineInOut,nTypeTarget);
	//pKlineHistory[0].day = pKlineHistory[6].day;
	return true;

}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：InPutValue(Kline *pKline, int i, int &j, bool bFrst, int &nCountDay,int nKindStock)
//      功能说明：used to transfer data
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////

void CTaiKlineTransferKline::InPutValue(Kline *pKline, int i, int &j, bool bFrst, int &nCountDay,int nKindStock,bool isMinute1)//used to transfer data
{
	int jTemp = j;
	if(bFrst==true)
	{
		/* Marenan 2009-12-23 */

		if (jTemp > 0)
			pKline[jTemp - 1].day=pKline[i].day;
		//Marenan +
		if (jTemp > 0 && !isMinute1)
		{
			pKline[jTemp - 1].day = pKline[i-1].day;
		}
		if (jTemp > 0 && isMinute1)
		{
			pKline[jTemp - 1].day = pKline[i].day;
		}
		/* Marenan 2009-12-23 */


		pKline[jTemp].open=pKline[i].open;
		pKline[jTemp].high=pKline[i].high;
		pKline[jTemp].low=pKline[i].low;
		pKline[jTemp].close=pKline[i].close;
		pKline[jTemp].vol=pKline[i].vol;
		pKline[jTemp].amount=pKline[i].amount;

		if(nKindStock!=2)
		{
			pKline[jTemp].decline =pKline[i].decline;
			pKline[jTemp].advance  =pKline[i].advance;
/*			if(j>0&&nCountDay>0)
			{
				pKline[jTemp-1].decline =pKline[jTemp-1].decline/nCountDay;
				pKline[jTemp-1].advance  =pKline[jTemp-1].advance/nCountDay;
			}*/
		}
		else
		{
			((KlineGeneral*)(pKline+jTemp))->volPositive=((KlineGeneral*)(pKline+i))->volPositive;
		}
		nCountDay = 1;

		j++;
	}
	else
	{
		jTemp--;

		if (isMinute1)
			pKline[jTemp].day=pKline[i].day + 60;
		else
			pKline[jTemp].day=pKline[i].day;

		if(pKline[i].high>pKline[jTemp].high)
			pKline[jTemp].high=pKline[i].high;
		if(pKline[i].low<pKline[jTemp].low)
			pKline[jTemp].low=pKline[i].low;
		pKline[jTemp].close=pKline[i].close;

		pKline[jTemp].vol+=pKline[i].vol;
		pKline[jTemp].amount+=pKline[i].amount;
		if(nKindStock!=2)
		{
			pKline[jTemp].decline =pKline[i].decline;
			pKline[jTemp].advance  =pKline[i].advance;
		}
		else
		{
			((KlineGeneral*)(pKline+jTemp))->volPositive+=((KlineGeneral*)(pKline+i))->volPositive;
		}
		nCountDay ++;
	}


}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：TransferKlineToday(CString symbol,Kline *&pKline, int& nKlineInOut, int nTypeTarget)
//      功能说明：转换当天的K线数据
//      参数：pKline[in][out]:it is the pointer of k line data;
//				  nKlineInOut[in][out]:it is the number of k line data;
//				  nTypeTarget[in]:it is the target type of k line data;
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
bool CTaiKlineTransferKline::TransferKlineToday(CString symbol,int stkKind,Kline *&pKline, int& nKlineInOut, int nTypeTarget)//转换当天的K线数据
{
	if(!(nTypeTarget ==HS_KLINE ||nTypeTarget ==MIN1_KLINE|| nTypeTarget == MANY_DAY_KLINE&& pDoc->m_propertyInitiate.daysOfManyKline <0))
		if(pDoc->m_bInitDone!=TRUE || pDoc->m_bCloseWorkDone==true)
			return false;

	CTime tmB(1970,1,1,9,30,0);
	CTime tmNow;
	CTime tmEnd;
	tmNow=CTime::GetCurrentTime();
	int year=tmNow.GetYear ();
	int mon=tmNow.GetMonth ();
	int day=tmNow.GetDay ();
	CTime tm(year,mon,day,9,30,0,0);
	tmNow=tm;
	CTimeSpan tmAll;
	tmAll=tm-tmB;

	int nMax = nKlineInOut;

	int len=0;
	int nDayOfWeek=1;

	int nDay=tmAll.GetDays() ;//m_minGridX
	int j=0,k;
	bool bFirst=true;
	int nAdd=0;
	Kline *pKline2;

	//如果当日数据已经存在，就不再进行计算
	if(nTypeTarget>0 && nTypeTarget<9 || nTypeTarget == MANY_DAY_KLINE && pDoc->m_propertyInitiate.daysOfManyKline>0)
	{
		if(nKlineInOut>0 && pKline!=NULL)
		{
			CTime tmLast (pKline[nKlineInOut-1].day);
			if(tmLast.GetYear ()==year && tmLast.GetMonth () == mon && tmLast.GetDay () == day)
				return true;
		}
	}

	switch (nTypeTarget)
	{
	case 1:
	case 2:
	case 3:
	case 4:
		if(nTypeTarget==1)
			k=1;
		else if(nTypeTarget==2)
			k=3;
		else if(nTypeTarget==3)
			k=6;
		else if(nTypeTarget==4)
			k=12;

		//---	得到需要更新的个数nAdd
		//Marenan +
		if(nKlineInOut <= 0)
		{
			if (pKline)
			{
				delete [] pKline;
			}
			pKline = new Kline[240];
			memset(pKline,0,240);
		}

		ASSERT(pKline!=NULL);

		pKline2=&pKline[nMax ];

		//afxDump << nMax << "\n";
		nAdd=OneTo5(pKline2,k,symbol,stkKind);
		break;
	case 5:
		nAdd = TransferToday(true,symbol,stkKind,pKline,nKlineInOut);
		break;
	case 6:
		{
			int nWeek1=0,nWeek2;
			nDayOfWeek=tmNow.GetDayOfWeek ()-1;
			tmEnd=tmNow;//tmEnd为今天
			if(nMax>0)
			{
				if(m_klineOld.day !=-1)
				{
					tmNow=tmB+m_klineOld.day;
					CTime tm2(tmNow.GetYear (),tmNow.GetMonth (),tmNow.GetDay (),9,30,0);
					tmAll=tmEnd-tm2;//tmAll为时间跨度
					if(tmAll<=0)
						break;

					if(nDayOfWeek-tmAll.GetDays ()>0)
						nWeek2 = 0;
					else 
						nWeek2 = 1;
				}
				else
					nWeek2 = 1;

			}
			else
				nWeek2 = 1;

			nAdd = TransferToday(nWeek1!=nWeek2,symbol,stkKind,pKline,nKlineInOut);
		}
		break;
	case 7:
		{
			int nMonth1=0,nMonth2=1,nYear1=0,nYear2=0;
			tmEnd=tmNow;//tmEnd为今天
			if(nMax>0)
			{
				tmNow=tmB+pKline[nMax-1].day;
				nYear1=tmNow.GetYear ();
				nMonth1=tmNow.GetMonth ();

				nMonth2=tmEnd.GetMonth ();
				nYear2=tmEnd.GetYear ();
			}

			nAdd = TransferToday(nMonth1!=nMonth2||nYear1!=nYear2,symbol,stkKind,pKline,nKlineInOut);
		}
		break;
	case 8:
		{
			tmEnd=tmNow;//tmEnd为今天
			int nYear1,nYear2;
			if(nMax>0)
			{
				tmNow=tmB+pKline[nMax-1].day;
				nYear1=tmNow.GetYear ();
				nYear2=tmEnd.GetYear ();
			}
			else
			{
				nYear1=0;
				nYear2=1;
			}

			nAdd = TransferToday(nYear1!=nYear2,symbol,stkKind,pKline,nKlineInOut);
		}
		break;
	case MANY_DAY_KLINE:
		if(pDoc->m_propertyInitiate.daysOfManyKline>0)
		{
			int n = (nMax)%pDoc->m_propertyInitiate.daysOfManyKline;
			nAdd = TransferToday(n==0,symbol,stkKind,pKline,nKlineInOut);
		}
		else
		{
			nAdd = CaclKline1Minu(pKline,symbol,stkKind,abs(pDoc->m_propertyInitiate.daysOfManyKline));
			nKlineInOut=nAdd;
			return 1;
		}
		break;
	case MIN1_KLINE:
		nAdd = CaclKline1Minu(pKline,symbol,stkKind,1);
		nKlineInOut=nAdd;
		return 1;
	case HS_KLINE:
		nAdd = CaclKline1Minu(pKline,symbol,stkKind,0);
		nKlineInOut=nAdd;
		return 1;
	}

	nKlineInOut+=nAdd;

	return 1;

}
/////////////////////////////////////////////////////////////////////////////
//      函 数 名：TransferToday(bool bAnother,CString symbol,Kline* pKline,int nFlags)
//      功能说明：转换当天的K线数据类型
//      入口参数：
//      出口参数：无出口数
//      返回参数：
/////////////////////////////////////////////////////////////////////////////
int CTaiKlineTransferKline::TransferToday(bool bAnother, CString symbol,int stkKind, Kline *pKline, int nKline)//转换当天的K线数据类型
{
	if(pKline == NULL )
		return 0;

	int nMax = nKline;

//	CReportData* m_pReportData;	//个股的数据指针
	//--数据有误，返回
	if(m_symbolOld != symbol||m_stkKindOld !=stkKind  || m_pReportData ==NULL)
	{
		if(pDoc->m_sharesInformation.Lookup(symbol.GetBuffer (0),m_pReportData,stkKind)==0)
		{
			m_pReportData = 0;
			return 0;
		}
		m_symbolOld = symbol;
		m_stkKindOld = stkKind ;
	}
	//---		判断股票是否是大盘指标//Aidx1 *Adat[2]

	//up and down
	Rsdn1 *dpdat;
	int isSz=0;
	if(CSharesCompute::GetMarketKind(stkKind) == SZ_MARKET_EX) isSz=1;
	dpdat=pDoc->Nidx[isSz];
	int footEnd = 0;
	if(pDoc->m_bInitDone ==FALSE)
		footEnd=pDoc->m_nOldANT [0];
	else
		footEnd=pDoc->m_nANT [isSz];

	if( m_pReportData->lowp<=0|| m_pReportData->higp <=0|| m_pReportData->opnp<=0|| m_pReportData->nowp<=0)
	{
		if(symbol[0] == 'P' && symbol[1] == 'T' && m_pReportData->nowp>0)
		{
			m_pReportData->lowp = m_pReportData->nowp;
			m_pReportData->higp =m_pReportData->nowp;
			m_pReportData->opnp = m_pReportData->nowp;
			if(m_pReportData->ystc<=0) m_pReportData->ystc = m_pReportData->nowp ;
		}
		else
		{
			if(m_pReportData->nowp>0)
			{
//				static int ij = 0;
//				if(ij<1)
//					AfxMessageBox("行情数据的开盘价或最高价或最低价还没有接收到，\r\n如果依此数据收盘将导致错误数据！，请等数据全部接收好再收盘。");
//				ij++;
				if(m_pReportData->lowp<=0) m_pReportData->lowp = m_pReportData->nowp;
				if(m_pReportData->higp<=0) m_pReportData->higp =m_pReportData->nowp;
				if(m_pReportData->opnp<=0) m_pReportData->opnp = m_pReportData->nowp;
				if(m_pReportData->ystc<=0) m_pReportData->ystc = m_pReportData->nowp ;
			}
			else
				return 0;
		}
	}

	CTime tmB(1970,1,1,9,30,0);
	CTime tmNow;
	tmNow=CTime::GetCurrentTime();
	int year=tmNow.GetYear ();
	int mon=tmNow.GetMonth ();
	int day=tmNow.GetDay ();
	CTime tm(year,mon,day,9,30,0,0);
	tmNow=tm;
	CTimeSpan tmAll;
	tmAll=tm-tmB;
	int nRn =0;

	Kline kln;
	kln.day		= tmAll.GetTotalSeconds();
	kln.open	= m_pReportData->opnp;
	kln.high	= m_pReportData->higp;
	kln.low		= m_pReportData->lowp;
	kln.close	= m_pReportData->nowp;
	kln.vol		= m_pReportData->totv;
	kln.amount	= m_pReportData->totp;

	bool bIndex = false;
	if(!CTaiShanKlineShowView::IsIndexStock(symbol))
	{
		kln.volPositive = m_pReportData->rvol ;
	}
	else
	{
		bIndex = true;
		int isSz=0;
		if(CSharesCompute::GetMarketKind(stkKind) == SZ_MARKET_EX) isSz=1;
		int foot;
		if(pDoc->m_bInitDone ==FALSE)
			foot=pDoc->m_nOldANT [0];
		else
			foot=pDoc->m_nANT [isSz];

		Rsdn1 **Nidx;
		pDoc->m_sharesInformation.GetIndexRsdn((Rsdn1 **)Nidx); 
		if((Nidx [isSz][foot].rsn == 0||Nidx [isSz][foot].dnn == 0 ) && foot>0)
			foot--;
		WORD f1=(WORD)(Nidx [isSz][foot].rsn);

		WORD f2=(WORD)(Nidx [isSz][foot].dnn);

		kln.advance = f1;
		kln.decline = f2;
	}
	//compute today power
	if(m_bCalPowerToday == true)
	{
		powerArr splitArr;
		ReadPower(pDoc,symbol,stkKind, splitArr);
		ComputePowerToday(&kln,1, splitArr);
	}
	//end

	if(bAnother==true)
	{
		pKline[nMax ]=kln;
		nMax++;
		nRn = 1;
	}
	else if(m_klineOld.day!=-1)
	{
		pKline[nMax-1 ] = m_klineOld;
		pKline[nMax-1 ].day=kln.day;
		if(kln.high>pKline[nMax-1 ].high)
			pKline[nMax-1 ].high= kln.high;
		if( kln.low<pKline[nMax-1 ].low)
			pKline[nMax-1 ].low= kln.low;
		pKline[nMax-1 ].close= kln.close;
		if(nMax>1)
		{
			pKline[nMax-1 ].vol= kln.vol+pKline[nMax-1].vol;
			if(bIndex == true) pKline[nMax-1 ].volPositive = kln.volPositive ;
			else pKline[nMax-1 ].volPositive += kln.volPositive ;
			pKline[nMax-1 ].amount+= kln.amount ;
		}
		else
		{
			pKline[nMax-1 ].vol = kln.vol ;
			pKline[nMax-1 ].volPositive = kln.volPositive  ;
			pKline[nMax-1 ].amount = kln.amount ;
		}
	}
	ASSERT(nRn == nMax-nKline);
	return nRn;
}

int CTaiKlineTransferKline::CaclKline1Minu(Kline*& pKline,CString name,int stkKind, int nMinu)
{

	if(CTaiShanKlineShowView::IsIndexStock(name))
	{
		return CaclKline1MinuIndex(pKline,name,stkKind,nMinu);
	}
	//initial data 
	if(pBuySellList==NULL)
	{
		//read from file
		pBuySellList = new CBuySellList;
		CTaiKlineFileHS*	pFileHs;
		pFileHs=CTaiKlineFileHS::GetFilePointer( name,stkKind);//CTaiKlineFileHS::m_pFileHsSh ;
		pFileHs->ReadHS (name,*pBuySellList,true);//CHANGE ,it is a day
		if(m_bRemoveBuySellList==false)
			m_bRemoveBuySellList = true;
		else
			ASSERT(FALSE);
	}

	int nHs=pBuySellList->GetCount();
	ASSERT(nHs>=0);
	if(nHs == 0 )//&& m_bUseMinute1Data == false )
		return 0;
	if(pKline != NULL)
	{
		delete[] pKline;
		pKline = NULL;
	}
	pKline = new Kline[nHs+1];

	if (pKline == NULL)
	{
		return 0;
	}

	float volPre = 0;

	TRADE_DETAIL_H_PER* pHsPre = NULL;
	int j;
	for(j=0;j<nHs;j++)
	{
		POSITION pos=pBuySellList->FindIndex(nHs - j - 1 ) ;
		TRADE_DETAIL_H_PER* pHs=pBuySellList->GetAt( pos );
		pKline[j].day  = pHs->time ;
		pKline[j].open = pHs->price ;
		pKline[j].close = pHs->price ;
		pKline[j].high = pHs->price ;
		pKline[j].low = pHs->price ;
		if(pHs->time <= 5400) 
		{
			pKline[j].day = 10800;
			if(j)
			{
				pKline[j].open  = pKline[j-1].open  ;
				pKline[j].close = pKline[j-1].close ;
				pKline[j].high  = pKline[j-1].high  ;
				pKline[j].low   = pKline[j-1].low   ;
			}
		}
		int nFlag;
		float fVol = CTaiKlineTransferKline::GetVolHs(pHs, pHsPre, nFlag);
		pKline[j].vol  = FABSMY(fVol);
		pKline[j].amount  = pHs->price*pKline[j].vol*100 ;//need test 

		float volPositive = 0;
		if(nFlag >0)
			volPositive = pKline[j].vol ;
		else if(nFlag == 0)
			volPositive = pKline[j].vol/2 ;

		//decline or positive buy
		pKline[j].volPositive  = volPositive ;

		pHsPre = pHs;
	}

	if(nMinu == 0)
		return nHs;

	int tmt = CTime::GetCurrentTime ().GetTime();
	if(nHs>0) tmt = (int)pKline[0].day;
	tmt = tmt/(24*60*60)*24*60*60+((9-8)*60+30)*60;
	int n = 0; //number returned
	int nMax = 240;
	if(	m_bUseMinute1Data == true )
	{
		int isSz=0;
		if(CSharesCompute::GetMarketKind(stkKind) == SZ_MARKET_EX) isSz=1;
		if(pDoc->m_bInitDone ==FALSE)
			nMax=pDoc->m_nOldANT [0]+1;
		else
			nMax=pDoc->m_nANT [isSz]+1;
	}

	Kline* pKline2 = new Kline[240];//

	if (pKline2 == NULL)
	{
		return nHs;
	}

	memset(pKline2,0,240*32);
	int k=0,i;
	Kline kln;
	for(i=0;i<nHs;i++)
	{
		CTime tm1(pKline[i].day);
		n = (tm1.GetHour ()-9)*60-30 + tm1.GetMinute ();
		if(n<0)
			n = 0;
		if(n ==120)
			n=119;
		if(n>120)
			n = n-90;

		if(n>239)
			n=239;

		int timeT  ;
		//repair data to close price
		//if(k<n)
		kln=pKline[i==0?0:i-1];
		kln.vol=0;
		kln.amount = 0 ;
		kln.volPositive  = 0 ;
		for(;k<n;k++)
		{
			pKline2[k]=kln;
			timeT = tmt+k*60 ;
			if(k>=120)
				timeT  = tmt+60*k+90*60 ;
			pKline2[k].day = timeT;
		}
		k=n+1;
		if(i==nHs-1)
		{
			for(int j=n+1;j<nMax;j++)
			{
				pKline2[j]=kln;
				timeT = tmt+j*60 ;
				if(j>=120)
					timeT  = tmt+60*j+90*60 ;
				pKline2[j].day = timeT;
			}
		}
		if(pKline2[n].open<0.001) pKline2[n].open=pKline[i].open;
		if(pKline2[n].high<pKline[i].high) pKline2[n].high=pKline[i].high;
		if(pKline2[n].low>pKline[i].low || pKline2[n].low<0.001) pKline2[n].low=pKline[i].low;
		pKline2[n].close=pKline[i].close;
		pKline2[n].vol+=FABSMY(pKline[i].vol);
		pKline2[n].amount+=(pKline[i].amount);
		pKline2[n].volPositive +=pKline[i].volPositive ;
		timeT = tmt+n*60 ;
		if(n>=120)
			timeT  = tmt+60*n+90*60 ;
		pKline2[n].day = timeT;

	}

	if(pKline != NULL)
	{
		delete [] pKline;
		pKline = NULL;
	}

	pKline = pKline2;

	n = nMax -1;
	if(nMinu == 1)
		return nMax;

	//compute many minutes
	j = 0;
	ASSERT(nMinu>0);
	int nCountDay = 0;
	int nKindStock = 2;
	for(i=0;i<=n;i++)
	{
		int n2 = (i)%nMinu;
		InPutValue(pKline,i,j,n2 == 0,nCountDay,nKindStock,true);
		//if (n2 == 0 && j > 0)
		//{
			//pKline[j - 1].day += 60;
		//}
	}// error 5.12
	//Marenan +
	//修改最后K线项的时间
	pKline[j - 1].day = pKline[i - 1].day + 60;

	return j;
}

int CTaiKlineTransferKline::CaclKline1MinuIndex(Kline*& pKline,CString name,int stkKind, int nMinu)
{
	//initial data 
	if(pBuySellList==NULL)
	{
		//read from file
		pBuySellList = new CBuySellList;
		CTaiKlineFileHS*	pFileHs;
		pFileHs=CTaiKlineFileHS::GetFilePointer (name,stkKind);//m_pFileHsSh ;
		pFileHs->ReadHS (name,*pBuySellList,true);//CHANGE ,it is a day
		if(m_bRemoveBuySellList==false)
			m_bRemoveBuySellList = true;
		else
			ASSERT(FALSE);
	}

	int nHs=pBuySellList->GetCount();
/*	if(nHs>0)
	{
		if(nHs<240)
		{
//			ASSERT(FALSE);
			return 0;
		}
	}*/
	ASSERT(nHs>=0);
	if(!pDoc->m_sharesInformation.Lookup(name.GetBuffer (0),m_pReportData,stkKind))
		return 0 ;
	if(pKline != NULL)
	{
		delete[] pKline;
		pKline = NULL;
	}

	int n;
	try
	{

	int isSz=0;
	if(CSharesCompute::GetMarketKind(stkKind) == SZ_MARKET_EX) isSz=1;
	if(nHs == 0)//compute from data of document ,it is today's
	{
		pKline = new Kline[240];
		if(pDoc->m_bInitDone ==FALSE)
			n=pDoc->m_nOldANT [0]+1;
		else
			n=pDoc->m_nANT [isSz]+1;

		name.ReleaseBuffer ();
		CReportData* pS1=m_pReportData;
		if(pS1==0)
			return 0;
		if(pS1->opnp <=0 || pS1->nowp <=0) return 0;

		Rsdn1 **Nidx;
		pDoc->m_sharesInformation.GetIndexRsdn(Nidx); 
		CTime tm = CTime::GetCurrentTime ();
		int tmt = (int)tm.GetTime ();
		tmt = tmt/(24*60*60)*24*60*60+((9-8)*60+30)*60;
		for(int j=0;j<n;j++)
		{
			pKline[j].day  = tmt+60*j ;
			if(j>=120)
				pKline[j].day  = tmt+60*j+90*60;
			if(pS1->m_Kdata1 [j].Price<=0.01)
			{
				if(j==0)
				{
					pS1->m_Kdata1 [0].Price = pS1->opnp  ;
					pKline[j].open = pS1->opnp ;
				}
				else
				{
					pKline[j].open = pS1->m_Kdata1 [j-1].Price ;
					pS1->m_Kdata1 [j].Price = pS1->m_Kdata1 [j-1].Price ;
				}
			}
			else
				pKline[j].open = pS1->m_Kdata1 [j].Price ;

			pKline[j].close =pKline[j].open ;
			pKline[j].high = pKline[j].open ;
			pKline[j].low =  pKline[j].open;
			if(j==0)
			{
				pKline[j].vol  =    pS1->m_Kdata1 [j].Volume  ;
				pKline[j].amount  = pS1->m_Kdata1 [j].Amount  ;
			}
			else
			{
				if(pS1->m_Kdata1 [j].Volume-pS1->m_Kdata1 [j-1].Volume<=0)
					pKline[j].vol  = 0;
				else
					pKline[j].vol  =    pS1->m_Kdata1 [j].Volume-pS1->m_Kdata1 [j-1].Volume  ;
				if(pS1->m_Kdata1 [j].Amount-pS1->m_Kdata1 [j-1].Amount<=0)
					pKline[j].amount  = 0;
				else
					pKline[j].amount  = pS1->m_Kdata1 [j].Amount-pS1->m_Kdata1 [j-1].Amount  ;
			}

			pKline[j].advance = Nidx[isSz][j].rsn ;
			pKline[j].decline = Nidx[isSz][j].dnn  ;
		}

//		return n;
	}
	else if(nHs)//==240)//history
	{
		int nDays = 1;//CTaiKlineFileHS::ComputeDaysCount (pBuySellList);
//		ASSERT(nDays>0);
		pKline = new Kline[nHs*nDays];
		float volPre = 0;
		float amountPre = 0;
		if(m_pReportData->opnp<=0) return 0;

		float fPrice = m_pReportData->opnp;
		for(int j=0;j<nHs*nDays;j++)
		{
			POSITION pos=pBuySellList->FindIndex(nHs - j - 1 ) ;
			TRADE_DETAIL_H_PER* pHs=pBuySellList->GetAt( pos );
			if(pHs->price>0)
				fPrice = pHs->price;
			pKline[j].day  = pHs->time ;
			pKline[j].open = fPrice ;
			pKline[j].close = fPrice ;
			pKline[j].high = fPrice ;
			pKline[j].low = fPrice ;

			if(pHs->vol - volPre>0)
				pKline[j].vol  = pHs->vol - volPre ;
			else
				pKline[j].vol  = 0 ;
			volPre = FABSMY(pHs->vol) ;

			if(pHs->buySellOrIndex .indexInfo .amount  - amountPre>0)
				pKline[j].amount  = pHs->buySellOrIndex .indexInfo .amount  - amountPre ;
			else
				pKline[j].amount  = 0 ;
			amountPre = (pHs->buySellOrIndex .indexInfo .amount ) ;

			pKline[j].advance = pHs->buySellOrIndex .indexInfo .advance ;
			pKline[j].decline  = pHs->buySellOrIndex .indexInfo .decline ;

		}
		n=nHs*nDays;
	}

//	int nRtn = 0;
	}
	catch(...)
	{
		return 0;
	}


	if(nMinu <= 1)
		return n;

	//compute many minutes
	int j = 0,i;
//	ASSERT(nMinu>0);
	int nCountDay = 0;
	int nKindStock = 0;
	for(i=0;i<n;i++)
	{
		int n2 = (i)%nMinu;
		InPutValue(pKline,i,j,n2 == 0,nCountDay,nKindStock,true);
		//if (n2 == 0 && j > 0)
		//{
			//pKline[j - 1].day += 60;
		//}
	}
	pKline[j - 1].day = pKline[i - 1].day + 60;

	return j;
}


int CTaiKlineTransferKline::OneTo5(Kline *pKline, int nEach, CString symbol,int stkKind)
{
	Kline *pKline2 = NULL;
	int rtn = CaclKline1Minu(pKline2,symbol,stkKind,5*nEach);
	if(pKline2 != NULL)
	{
		memcpy(pKline,pKline2,rtn*sizeof(Kline));
		delete[] pKline2;
	}
	return rtn;
}

int CTaiKlineTransferKline::ReadPower(CTaiShanDoc * pDoc,CString symbol,int stkKind, powerArr &splitArr)
{
	PCdat1 pStockData;
	pDoc->m_sharesInformation .Lookup (symbol.GetBuffer (0),pStockData,stkKind);
	if(pStockData==NULL)
		return 0;

	if(pStockData->pBaseInfo == NULL )
		return 0;
	for(int i=0;i<pStockData->pBaseInfo ->NumSplit ;i++)
	{
		POWER pwr;
		Split* pSplit = pStockData->pBaseInfo ->m_Split+i;
		pwr.fAllocate = pSplit->Allocate ;
		pwr.fAllocatePrice = pSplit->AllocatePrice ;
		pwr.fGive = pSplit->Give ;
		pwr.nTime = pSplit->nTime ;
		pwr.nFlags = pSplit->nFlags ;
		pwr.fDividend = pSplit->Bonus ;
		bool bAdd = false;
		for(int j=0;j<splitArr.GetSize ();j++)
		{
			if(splitArr[j].nTime >= pwr.nTime )
			{
				if(splitArr[j].nTime > pwr.nTime )
					splitArr.InsertAt(j,pwr);
				bAdd = true;
				break;
			}
		}
		if(bAdd == false)
			splitArr.Add(pwr);
	}
	return pStockData->pBaseInfo ->NumSplit ;
}
void CTaiKlineTransferKline::ComputePower(Kline *pKlineHistory, int nKlineInOut, int nTypeTarget, powerArr &splitArr)
{
	CTaiShanDoc * pDoc = CMainFrame::m_pDoc ;
	int n= splitArr.GetSize();
	POWER pwr;
	POWER pwrMax;
	int nFootRecent = 0;
	int i;
	int j;

	if(n<=0 || nKlineInOut<=0) return;
	//to get the amount rate
	float* fAmountRate = new float[nKlineInOut];
	for(i=0;i<nKlineInOut;i++)
	{
		if(pKlineHistory[i].close>0 && pKlineHistory[i].vol>0)
			fAmountRate[i] = pKlineHistory[i].amount /pKlineHistory[i].vol /100/pKlineHistory[i].close;
		else
			fAmountRate[i] = 1;
	}

	bool bDayKline = true;
	if(nTypeTarget>=1 &&nTypeTarget<5 )
		bDayKline = false;

	for(i=0;i<n;i++)
	{
		pwr=splitArr[i];
		//Marenan Modify 
		if(i==0) 
		{
			pwrMax = pwr;
		}

		if(	pwr.fAllocate <=0 && 
			pwr.fAllocatePrice <=0 &&
			pwr.fDividend <=0 &&
			pwr.fGive <=0 ||
			pwr.nTime<=54000)
			continue;

		//--	得到时间对应的K线数据的下标
		bool bAdd;
		CTime tm(pwr.nTime);
		CTime tm2(tm.GetYear (),tm.GetMonth (),tm.GetDay (),1,30,0);
		time_t nTime = tm2.GetTime();
		int rtn=CTaiKlineFileKLine::LookTwoPath(nTime ,pKlineHistory,nKlineInOut, bAdd,bDayKline);
		if(rtn==-1 && bAdd == true)
			rtn = nKlineInOut;
//		if(bAdd == true)
//			rtn--;
		if(rtn<0) rtn =0;

		if(i==0) 
		{
			//Marenan Modify 
			//pwrMax 可能没有初始化，故将这句移到for循环的开始处，保证能够得到初始化
			//pwrMax = pwr;
			nFootRecent = rtn;
		}
		else if(pwr.nTime >pwrMax.nTime )
		{
			pwrMax = pwr;
			nFootRecent = rtn;
		}

		//--	计算
		if((1+pwr.fAllocate )==0)
			break;
		if((1+pwr.fGive  )==0)
			break;
		if(1+pwr.fGive+pwr.fAllocate == 0)
			continue;
		//bDoPowerAfter
		if(pDoc->m_propertyInitiate.bDoPowerAfter == false||pDoc->m_propertyInitiate.bDoPowerAfter == true && i<n-1)
		{
			float f ;//= CalcPowerPer(float(1.0),pwr );
			for(j=0;j<rtn;j++)
			{
				pKlineHistory [j].open =CalcPowerPer(pKlineHistory [j].open,pwr);
				pKlineHistory [j].high =CalcPowerPer(pKlineHistory [j].high ,pwr);
				pKlineHistory [j].low  =CalcPowerPer(pKlineHistory [j].low  ,pwr);
				pKlineHistory [j].close=CalcPowerPer(pKlineHistory [j].close,pwr);

				if(pDoc->m_propertyInitiate.bDoVolPowering == TRUE &&(pwr.fGive>0||pwr.fDividend >0))//成交量除权
				{
					f = pKlineHistory [j].vol*100*(pwr.fDividend-pwr.fAllocatePrice*pwr.fAllocate);
					pKlineHistory [j].vol *=(1+pwr.fGive+pwr.fAllocate);
	//				pKlineHistory [j].amount = pKlineHistory [j].amount
	//					-f;//+pwr.fAllocatePrice*pwr.fAllocate
	//				f = pKlineHistory [j].amount/pKlineHistory [j].vol/100;
	//				if(f>pKlineHistory [j].high || f<pKlineHistory [j].low)
	//					pKlineHistory [j].amount = pKlineHistory [j].vol*100*(pKlineHistory [j].close+pKlineHistory [j].high+pKlineHistory [j].low)/3;
					if((pKlineHistory [j].volPositive<1 && pKlineHistory [j].volPositive!=0 ) || pKlineHistory [j].volPositive>pKlineHistory [j].vol)
						pKlineHistory [j].volPositive = pKlineHistory [j].vol/2;
					else
						pKlineHistory [j].volPositive *=(1+pwr.fGive+pwr.fAllocate);
				}
			}
		}
	}

	//
	if(pDoc->m_propertyInitiate.bDoPowerAfter == true&&n>0)//后除权
	{//nKlineInOut
		pwr =pwrMax;
		float f ;
		for(j=nFootRecent;j<nKlineInOut;j++)
		{
			pKlineHistory [j].open =CalcPowerAfterPer(pKlineHistory [j].open,pwr);
			pKlineHistory [j].high =CalcPowerAfterPer(pKlineHistory [j].high ,pwr);
			pKlineHistory [j].low  =CalcPowerAfterPer(pKlineHistory [j].low  ,pwr);
			pKlineHistory [j].close=CalcPowerAfterPer(pKlineHistory [j].close,pwr);

			if(f>0 && pDoc->m_propertyInitiate.bDoVolPowering == TRUE)//成交量除权
			{
				pKlineHistory [j].vol /=(1+pwr.fGive+pwr.fAllocate);
				if((pKlineHistory [j].volPositive<1 && pKlineHistory [j].volPositive!=0 ) || pKlineHistory [j].volPositive>pKlineHistory [j].vol)
					pKlineHistory [j].volPositive = pKlineHistory [j].vol/2;
				else
					pKlineHistory [j].volPositive /=(1+pwr.fGive+pwr.fAllocate);
			}
		}
	}

	//to compute amount
	for(i=0;i<nKlineInOut;i++)
	{
		float f = fAmountRate[i] *pKlineHistory[i].vol *100*pKlineHistory[i].close;
		pKlineHistory[i].amount = f;
		if( fAmountRate[i] *pKlineHistory[i].close>pKlineHistory [i].high || fAmountRate[i] *pKlineHistory[i].close<pKlineHistory [i].low)
			pKlineHistory [i].amount = pKlineHistory [i].vol*100*(pKlineHistory [i].close+pKlineHistory [i].high+pKlineHistory [i].low)/3;
	}
	;
	//end
	delete [] fAmountRate;

}

float CTaiKlineTransferKline::CalcPowerPer(float fIn, POWER &pwr)
{
	float f = (fIn-pwr.fDividend+pwr.fAllocatePrice*pwr.fAllocate )/(1+pwr.fGive+pwr.fAllocate );
	if(f<=0) f=0.001;
	return f;
}

float CTaiKlineTransferKline::CalcPowerAfterPer(float fIn, POWER &pwr)
{
	float f = (fIn*(1+pwr.fGive+pwr.fAllocate )+pwr.fDividend-pwr.fAllocatePrice*pwr.fAllocate );
	if(f<=0) f=0.001;
	return f;
}

void CTaiKlineTransferKline::ComputePowerToday(Kline *pKlineToday,int nKline, powerArr &splitArr)
{
	if(CMainFrame::m_pDoc ->m_propertyInitiate.bDoPowerAfter == true)//后除权
	{
		//nKlineInOut
		int n= splitArr.GetSize();
		if(n<=0)
			return;
		POWER pwr;
		POWER pwrMax;
		int nFootRecent = 0;
		//int j;
		int i;
		for(i=0;i<n;i++)
		{
			pwr=splitArr[i];

			if(i==0) 
				pwrMax = pwr;
			else if(pwr.nTime >pwrMax.nTime )
				pwrMax = pwr;
		}

		pwr =pwrMax;
		
		float f ;
		for(i=0;i<nKline;i++)
		{
			pKlineToday [i].open =CalcPowerAfterPer(pKlineToday [i].open,pwr);
			pKlineToday [i].high =CalcPowerAfterPer(pKlineToday [i].high ,pwr);
			pKlineToday [i].low  =CalcPowerAfterPer(pKlineToday [i].low  ,pwr);
			pKlineToday [i].close=CalcPowerAfterPer(pKlineToday [i].close,pwr);
//bShowCrossDlg
			if(f>0 && CMainFrame::m_pDoc->m_propertyInitiate.bDoVolPowering == TRUE)//成交量除权
			{
				pKlineToday [i].vol /=(1+pwr.fGive+pwr.fAllocate);
				if((pKlineToday [i].volPositive<1 && pKlineToday [i].volPositive!=0 ) || pKlineToday [i].volPositive>pKlineToday [i].vol)
					pKlineToday [i].volPositive = pKlineToday [i].vol/2;
				else
					pKlineToday [i].volPositive /=(1+pwr.fGive+pwr.fAllocate);
			}
		}
	}

}

float CTaiKlineTransferKline::GetVolHs(TRADE_DETAIL_H_PER *pHs, TRADE_DETAIL_H_PER *pHsPre, int &nFlag)
{
	float f = 0;
	if(pHsPre==NULL)
	{
		f = (pHs->vol);
	}
	else if((pHs->vol)-(pHsPre->vol)>=0)
	{
		f = (pHs->vol)-(pHsPre->vol);
	}

	//compute nflag : nFlag == -1 是内盘 ；== 0 ； == 1 是外盘
	TRADE_DETAIL_H_PER *pHsTemp = pHsPre;
	if(pHsPre==NULL) pHsTemp = pHs;
	float maxFloat= pHsTemp->buySellOrIndex .buySellInfo .unitCountPrc ;

	nFlag = 0;

	//int i;
	float fprc[2]={0,0};
	if(maxFloat>0)
	{
		if(pHsTemp->buySellOrIndex .buySellInfo .buyP [3]!=127)
			fprc[0]=pHsTemp->price - (float)((char)(pHsTemp->buySellOrIndex .buySellInfo .buyP [3] ) * maxFloat)/(float)(125);
		if(pHsTemp->buySellOrIndex .buySellInfo .buyP [0]!=127)
			fprc[1]=pHsTemp->price - (float)((char)(pHsTemp->buySellOrIndex .buySellInfo .buyP [0] ) * maxFloat)/(float)(125);

		if(pHs->price >=fprc[0] && fprc[0]>0 || fprc[0]>0 && fprc[1]<0.01) 
			nFlag = 1;
		else 
			if(pHs->price <=fprc[1]) nFlag = -1;
	}
	else
	{
		if(pHsPre!=NULL)
		{
			if(pHs->price < pHsPre->price) nFlag = -1;
			else if(pHs->price > pHsPre->price) nFlag = 1;
		}
	}
	if(pHsPre==NULL)
		nFlag = 0;


	return f;

}
