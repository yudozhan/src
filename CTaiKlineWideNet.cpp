// CTaiKlineWideNet.cpp: implementation of the CTaiKlineWideNet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineWideNet.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "CTaiKlineMemFile.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiScreenParent.h"
#include "CTaiKlineFileHS.h"
#include "CTaiKlineTransferKline.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDoFenshi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
Kline*		CTaiKlineWideNet::m_pKline1A0001 = NULL;
int			CTaiKlineWideNet::m_nKline1A0001 = 0;
int			CTaiKlineWideNet::m_nKlineToToday = 0;
//m_nKlineToToday

CTaiKlineWideNet::CTaiKlineWideNet()
{
}

CTaiKlineWideNet::CTaiKlineWideNet(CTaiShanKlineShowView * pView)
{
	m_pView = pView;

	m_symbolPre = _T("");
	m_sDateHistory = _T("");
	m_stkKindPre = 0;
	
	m_bDayRequested		= false;
	m_bMin5Requested	= false;
	m_bMin1Requested	= false;
	m_bHsRequested		= false;
	m_bHsHistoryRequested = false;

/*	//read now
	if(m_pKline1A0001 == NULL)
	{
		m_nKline1A0001 = CTaiKlineFileKLine::ReadKLineS("1A0001",m_pKline1A0001,-1,0,true);
		//to request data
		if(m_nKline1A0001<=0)
		{
			AddStockKlineData("1A0001", 5);
			if(m_pKline1A0001 != NULL) 
			{
				delete []m_pKline1A0001;
				m_pKline1A0001 == NULL;
			}
		}
		//
		if(m_pKline1A0001 != NULL)
		{
			m_nKlineToToday = GetDaysToToday(m_pKline1A0001+m_nKline1A0001-1);
		}
	}*/
}

CTaiKlineWideNet::~CTaiKlineWideNet()
{

}

void CTaiKlineWideNet::AddStockHs(CString symbol,int stkKind,bool m_bAutoDeletePre)
{
#ifdef WIDE_NET_VERSION
#endif

}

void CTaiKlineWideNet::AddStockKlineData(CString symbol,int stkKind, int nKlineType)
{
	//2.申请日线//GetDataKind()
	AddStockKlineDataAll( symbol,stkKind,  nKlineType,  false);
}

void CTaiKlineWideNet::AddStockHsFileData(CString symbol,int stkKind)
{
	if(m_bHsRequested	== false && (m_pView->GetDataKind () == HS_KLINE ||m_pView->pMin1Drawing->m_bHist == true ))
	{
		AddStockHsFileDataAll(symbol,stkKind);
	}
}

void CTaiKlineWideNet::AddStockFirst(CString symbol,int stkKind,bool m_bAutoDeletePre)
{
	//first to delete pre
	if(m_symbolPre == symbol
		&&m_stkKindPre == stkKind
		&&m_bDayRequested	== true
		&&m_bMin5Requested	== true
		&&m_bMin1Requested	== true
		&&m_bHsRequested	== true
//		&&m_bHsHistoryRequested	== true
		) 
	{
		return;
	}

	if(m_bAutoDeletePre == true)
	{
		if(m_symbolPre != symbol
			&&m_stkKindPre == stkKind
			)
		{
			m_bDayRequested		= false;
			m_bMin5Requested	= false;
			m_bMin1Requested	= false;
			m_bHsRequested		= false;
		}
		m_symbolPre = symbol;
		m_stkKindPre = stkKind;
	}

	//add now
	{
	//1.首先申请实时行情、1分钟线
		AddStockHs(symbol,stkKind,true);
	//2.申请日线//GetDataKind()
	//3.申请5分钟K线//
		AddStockKlineData( symbol,stkKind, 5);

	//4.申请明细数据//
		AddStockHsFileData(symbol,stkKind);

	}

}

int CTaiKlineWideNet::GetRequestKlineCount(CString symbol,int stkKind, bool bDay)
{
	int n = 0;
	Kline * pKline = NULL;
	int nRead = CTaiKlineFileKLine::ReadKLineS(symbol,stkKind,pKline,1,0,bDay);

	int nRate = 1;
	if(bDay == false) nRate = 48;

	if(nRead!=1 ) n = 6000;
	else
	{
		n = GetDaysToToday(pKline)*nRate;//m_nKlineToToday;
	}

	if(pKline!=NULL) delete[] pKline;
	return n;
}

int CTaiKlineWideNet::GetDaysToToday(Kline *pKline)
{
	int n = 0;
	CTime tm(pKline[0].day );
	CTime tm2(tm.GetYear (),tm.GetMonth (),tm.GetDay (),0,0,0);

	CTime tmCurr = CTime::GetCurrentTime ();
	int nDayOfWeek = tmCurr.GetDayOfWeek ();
	CTimeSpan tmSp = tmCurr - tm2;
	int nDay = tmSp.GetDays();

	nDayOfWeek--;
	if(nDayOfWeek==0) nDayOfWeek = 7;

	if(nDay <= 1)
		n = 0;
	else 
	{
		if(nDay < nDayOfWeek )
			n =  nDayOfWeek - nDay + 1;
		else
		{
			n = nDay-(nDay-nDayOfWeek+5)/7*2;
		}
	}
/*	if(nDayOfWeek == 2)
	{
		if(nDay <= 3)
			n = 0;
		else if(nDay > 3)
			n = nDay-2 - nDay/7*2;
	}
	else
	{
		if(nDay <= 1)
			n = 0;
		else if(nDay > 1)
			n = nDay - nDay/7*2;
	}*/
	if(n == 0) n= -1;
	return n;
}

void CTaiKlineWideNet::RegisterID()
{
#ifdef WIDE_NET_VERSION
#endif
}

void CTaiKlineWideNet::ReleaseID()
{
#ifdef WIDE_NET_VERSION
#endif

}

void CTaiKlineWideNet::Request()
{
	m_bDayRequested		= false;
	m_bMin5Requested	= false;
	m_bMin1Requested	= false;
	m_bHsRequested		= false;
//	m_bHsHistoryRequested		= false;

	AddStockFirst(m_pView->m_sharesSymbol,m_pView->m_stkKind );

}

int CTaiKlineWideNet::GetRequestMin1Count(CString symbol,int stkKind)
{
	int n = 0;
	CReportData * pdt = NULL;
	if(CMainFrame::m_pDoc ->m_sharesInformation.Lookup(symbol.GetBuffer (0),pdt,stkKind)==0)
		return 0;
	if(pdt == NULL) return 0;
/*	for(int i = 0;i<240;i++)
	{
		if(pdt->m_Kdata1[i].Price<0.01)
			break;
	}*/
	n = pdt->lastclmin ;

	return n;

}

int CTaiKlineWideNet::GetRequestHsCount(CString symbol,int stkKind)
{
	int n = 0;
	n = CTaiKlineFileHS::GetCountPre(symbol,stkKind);
	return n;

}

void CTaiKlineWideNet::SetHsHistoryRequestFlag(bool bRequested)
{
	m_bHsRequested = bRequested;
}

void CTaiKlineWideNet::AddStockHsHistoryFileData(CString symbol,int stkKind, CString sDate)
{
#ifdef WIDE_NET_VERSION
#endif

}

void CTaiKlineWideNet::AddStockHsFileDataAll(CString symbol,int stkKind, bool bAll)
{
#ifdef WIDE_NET_VERSION
#endif


}

void CTaiKlineWideNet::AddStockKlineDataAll(CString symbol,int stkKind, int nKlineType, bool bAll)
{
}
