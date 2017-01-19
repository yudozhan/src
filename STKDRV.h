// STKDRV.h: interface for the CStockDriver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STKDRV_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
#define AFX_STKDRV_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStockDriver  
{
public:
	CStockDriver();
	virtual ~CStockDriver();
public:
	DWORD GetStockDrvInfo(int nInfo,void * pBuf);
	int SetupReceiver(BOOL bSetup);
	int GetTotalNumber();
	int Stock_Quit(HWND hWnd);
	int Stock_Init(HWND hWnd,UINT uMsg,int nWorkMode);
	void SetNewsPath(LPCTSTR lpszNewsPath);
	int SetExtMsg(UINT msg);
	int GetKData(LPCSTR scode, time_t mt);
	int QueryKData(LPCSTR scode, int nMarket, time_t mt);
	int GetMinData(LPCSTR scode, time_t mt);
	int QueryMinData(LPCSTR scode, int nMarket, time_t mt);
	int QueryTradeData(LPCSTR scode, int nMarket, time_t mt);

	int (WINAPI *	m_pfnStock_Init)(HWND hWnd,UINT Msg,int nWorkMode);
	int (WINAPI *	m_pfnStock_Quit)(HWND hWnd);
	int (WINAPI *	m_pfnGetTotalNumber)();													
	int	(WINAPI *	m_pfnSetupReceiver)(BOOL bSetup);
	DWORD (WINAPI * m_pfnGetStockDrvInfo)(int nInfo,void * pBuf);
	void	(WINAPI *	m_pfnSetNewsPath)(LPCTSTR lpszNewsPath);
	//void WINAPI SetNewsPath(LPCTSTR lpszNewsPath)
	/*
	******************************************************/
	int (WINAPI *	m_pfnSetExtMsg)(UINT msg);
	int (WINAPI *	m_pfnGetKData)(LPCSTR scode, time_t mt);
	int (WINAPI *	m_pfnQueryKData)(LPCSTR scode, int nMarket, time_t mt);
	int  (WINAPI *	m_pfnGetMinData)(LPCSTR scode, time_t mt);
	int  (WINAPI *	m_pfnQueryMinData)(LPCSTR scode, int nMarket, time_t mt);
//	int  WINAPI GetTradeData(LPCSTR scode, time_t mt);
	int  (WINAPI *	m_pfnQueryTradeData)(LPCSTR scode, int nMarket, time_t mt);
//	int  WINAPI GetNews(LPCSTR  channel = NULL,  time_t fmt =0 );
//	int  WINAPI GetBaseInfo(LPCSTR  scode,  time_t fmt);
//	int  WINAPI QueryBaseInfo(LPCSTR scode, int nMarket, time_t mt);
//	int  WINAPI GetStockTable();
//	BOOL  WINAPI GetServerTime(time_t *ptime);

private:
	void GetAdress();
	HWND m_hWnd;
	HINSTANCE	m_hSTKDrv;
};

#endif // !defined(AFX_STKDRV_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
