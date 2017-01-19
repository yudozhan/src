// STKDRV.cpp: implementation of the CStockDriver class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STKDRV.h"
#include "WH.h"
#include "GetSetReg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStockDriver::CStockDriver()
{
	m_pfnSetExtMsg		= NULL;
	m_pfnGetKData		= NULL;
	m_pfnQueryKData		= NULL;
	m_pfnGetMinData		= NULL;
	m_pfnQueryMinData	= NULL;
	m_pfnQueryTradeData = NULL;

	m_pfnStock_Init		= NULL;
	m_pfnStock_Quit		= NULL;
	m_pfnGetTotalNumber = NULL;
	m_pfnSetupReceiver	= NULL;
	m_pfnGetStockDrvInfo= NULL;
	m_hSTKDrv = NULL;
}

CStockDriver::~CStockDriver()
{
	Stock_Quit(m_hWnd);
}

void CStockDriver::GetAdress()
{
	//if(CWHApp::m_gbUseExe ==true)	return;

	//GetSetReg
	CFileFind flfd;
	CString m_CurrentWorkDirectory;
 	::GetCurrentDirectory(MAX_PATH,m_CurrentWorkDirectory.GetBuffer(MAX_PATH));
	m_CurrentWorkDirectory.ReleaseBuffer();
	m_CurrentWorkDirectory.TrimRight ("\\");
	m_CurrentWorkDirectory.TrimRight ("/");
	m_CurrentWorkDirectory+="\\";
	if(flfd.FindFile (m_CurrentWorkDirectory+"Stock.dll" ))
		m_hSTKDrv = LoadLibrary(m_CurrentWorkDirectory+"Stock.dll");
	else
	{
		GetSetReg reg;
		char ch[512];
		DWORD ncb = sizeof(ch);
		DWORD typ = REG_SZ;
		if(reg.GetValue ("software\\stockdrv","Driver",typ,(unsigned char *)ch,ncb) == ERROR_SUCCESS)
		{
			ch[511] = 0;
			m_hSTKDrv = LoadLibrary(ch);
		}
		else
		{
			m_hSTKDrv = LoadLibrary("Stock.dll");
		}
	}
	if( !m_hSTKDrv ) 
		m_hSTKDrv = LoadLibrary("Stock.dll");
	if( !m_hSTKDrv ) 
		return;

	m_pfnStock_Init = \
		    (int (WINAPI *)(HWND,UINT,int)) GetProcAddress(m_hSTKDrv,"Stock_Init");
	m_pfnStock_Quit = \
			(int (WINAPI *)(HWND)) GetProcAddress(m_hSTKDrv,"Stock_Quit");
	m_pfnGetTotalNumber = \
			(int (WINAPI *)())GetProcAddress(m_hSTKDrv,"GetTotalNumber");
	m_pfnSetupReceiver = \
			(int	(WINAPI *)(BOOL))GetProcAddress(m_hSTKDrv,"SetupReceiver");
	m_pfnGetStockDrvInfo = \
			(DWORD (WINAPI *)(int,void * ))GetProcAddress(m_hSTKDrv,"GetStockDrvInfo");
	m_pfnSetNewsPath = \
		(void (WINAPI*)(LPCTSTR))GetProcAddress(m_hSTKDrv,"SetNewsPath");

	m_pfnSetExtMsg = \
		(int (WINAPI*)(UINT msg))GetProcAddress(m_hSTKDrv,"SetExtMsg");
	m_pfnGetKData = \
		(int (WINAPI*)(LPCSTR scode, time_t mt))GetProcAddress(m_hSTKDrv,"GetKData");
	m_pfnQueryKData = \
		(int (WINAPI*)(LPCSTR scode, int nMarket, time_t mt))GetProcAddress(m_hSTKDrv,"QueryKData");
	m_pfnGetMinData = \
		(int (WINAPI*)(LPCSTR scode, time_t mt))GetProcAddress(m_hSTKDrv,"GetMinData");
	m_pfnQueryMinData = (int (WINAPI*)(LPCSTR scode, int nMarket, time_t mt))GetProcAddress(m_hSTKDrv,"QueryMinData");
	m_pfnQueryTradeData = (int (WINAPI*)(LPCSTR scode, int nMarket, time_t mt))GetProcAddress(m_hSTKDrv,"QueryTradeData");
}

int CStockDriver::Stock_Init(HWND hWnd, UINT uMsg, int nWorkMode)
{
	//if(CWHApp::m_gbUseExe ==true)
	//	return 1;

	if( m_hSTKDrv==NULL || m_pfnStock_Init==NULL )
	{
		Stock_Quit(m_hWnd);

		GetAdress();
		if( m_hSTKDrv==NULL || m_pfnStock_Init==NULL)
		{
			return -1;
		}
	}
		
	m_hWnd = hWnd;
	CString m_CurrentWorkDirectory;
 	::GetCurrentDirectory(MAX_PATH,m_CurrentWorkDirectory.GetBuffer(MAX_PATH));
	m_CurrentWorkDirectory.ReleaseBuffer();
	m_CurrentWorkDirectory.TrimRight ("\\");
	m_CurrentWorkDirectory.TrimRight ("/");
	m_CurrentWorkDirectory+="\\";
	int n = m_pfnStock_Init(hWnd,uMsg,m_pfnQueryTradeData?RCV_WORK_RITE_ACTIVE:nWorkMode); 
	SetNewsPath(m_CurrentWorkDirectory+"news");
	return n;
}

int CStockDriver::Stock_Quit(HWND hWnd)
{
	//if(CWHApp::m_gbUseExe ==true)
	//	return 1;

	int Ret = -1;
	if( m_hSTKDrv )  
	{
		if(m_pfnStock_Quit )
		{
			Ret = m_pfnStock_Quit(hWnd);
			m_pfnStock_Quit = NULL;
		}
		FreeLibrary(m_hSTKDrv);
		m_hSTKDrv = NULL;
	}
	return Ret;
}

int CStockDriver::GetTotalNumber()
{

	if( !m_pfnGetTotalNumber )
		return -1;
	return( (*m_pfnGetTotalNumber)());
}

int CStockDriver::SetupReceiver(BOOL bSetup)
{
	//if(CWHApp::m_gbUseExe ==true)
	//	return 1;
	if( !m_pfnSetupReceiver )
		return -1;
	return( (*m_pfnSetupReceiver)(bSetup));
}

DWORD CStockDriver::GetStockDrvInfo(int nInfo, void *pBuf)
{

	if( !m_pfnGetStockDrvInfo ) 
		return -1;
	return( (*m_pfnGetStockDrvInfo)(nInfo,pBuf));
}
void CStockDriver::SetNewsPath(LPCTSTR lpszNewsPath)
{
	//if(CWHApp::m_gbUseExe ==true)
	//	return ;
	if( !m_pfnSetNewsPath )
		return ;

	try
	{
	(*m_pfnSetNewsPath)(lpszNewsPath);
	}
	catch(...)
	{
	}
}
int CStockDriver::SetExtMsg(UINT msg)
{
	if( !m_pfnSetExtMsg )
		return 0;
	return( (*m_pfnSetExtMsg)(msg));
}
int CStockDriver::GetKData(LPCSTR scode, time_t mt)
{
	if( !m_pfnGetKData )
		return 0;
	return( (*m_pfnGetKData)(scode, mt));
}
int CStockDriver::QueryKData(LPCSTR scode, int nMarket, time_t mt)
{
	if( !m_pfnQueryKData )
		return 0;
	return( (*m_pfnQueryKData)(scode, nMarket, mt));
}
int CStockDriver::GetMinData(LPCSTR scode, time_t mt)
{
	if( !m_pfnGetMinData )
		return 0;
	return( (*m_pfnGetMinData)(scode, mt));
}
int CStockDriver::QueryMinData(LPCSTR scode, int nMarket, time_t mt)
{
	if( !m_pfnQueryMinData )
		return 0;
	m_pfnQueryTradeData(scode, nMarket, mt);
	return m_pfnQueryMinData(scode, nMarket, mt);
}
