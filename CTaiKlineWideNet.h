// CTaiKlineWideNet.h: interface for the CTaiKlineWideNet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TECHWIDENET_H__5FFBB872_DAB3_495F_82B2_93EDA6179B84__INCLUDED_)
#define AFX_TECHWIDENET_H__5FFBB872_DAB3_495F_82B2_93EDA6179B84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CTaiShanKlineShowView;
/*#ifndef _SOCKET_ID
typedef struct _SOCKET_ID
{
	HWND	hWnd;	//注册此ID的窗口句柄，用来以后给此窗口发送I/O完成消息
	int iFlag ;	//此ID的标志，用以区分不同的ID；范围从0到49，共50个
}SOCKET_ID;
#endif*/
class CTaiKlineWideNet  
{
public:
	void AddStockKlineDataAll(CString symbol,int stkKind, int nKlineType,bool bAll = false);
	void AddStockHsFileDataAll(CString symbol,int stkKind,bool bAll = false);
	void AddStockHsHistoryFileData(CString symbol,int stkKind,CString sDate);
	void SetHsHistoryRequestFlag(bool bRequested);
	void Request();
	void ReleaseID();
	//注册通道
	//参数：symbol股票代码
	void RegisterID();
	//首次计算某股票的需要的k线数据的个数
	//参数：symbol股票代码
	int GetRequestKlineCount(CString symbol,int stkKind,bool bDay = true);
	//首次进入某股票，申请某股票的数据的传送
	//参数：symbol股票代码
	void AddStockFirst(CString symbol,int stkKind,bool m_bAutoDeletePre = true);
	//申请某股票的所有明细数据
	//参数：symbol股票代码
	void AddStockHsFileData(CString symbol,int stkKind);
	//申请某股票的k线数据
	//参数：symbol股票代码
	void AddStockKlineData(CString symbol,int stkKind,int nKlineType);
	//申请某股票的行情数据的传送
	//参数：symbol股票代码
	void AddStockHs(CString symbol,int stkKind,bool m_bAutoDeletePre = true);
	CTaiKlineWideNet();
	CTaiKlineWideNet(CTaiShanKlineShowView * pView);
	virtual ~CTaiKlineWideNet();

private:
	int GetRequestHsCount(CString symbol,int stkKind);
	int GetRequestMin1Count(CString symbol,int stkKind);
	int GetDaysToToday(Kline* pKline);
	CTaiShanKlineShowView* m_pView;

	CString				m_symbolPre;//前一个股票
	int					m_stkKindPre;

	bool				m_bDayRequested;//日线
	bool				m_bMin5Requested;//5分钟k线
	bool				m_bMin1Requested;//1分钟
	bool				m_bHsRequested;//分笔明细
	bool				m_bHsHistoryRequested;//历史分笔明细
	CString				m_sDateHistory;//历史回忆时间

	static Kline*		m_pKline1A0001;
	static int			m_nKline1A0001;
	static int			m_nKlineToToday;

	//0为实时行情，1为历史k线
	SOCKET_ID	g_socketID[3];
};

#endif // !defined(AFX_TECHWIDENET_H__5FFBB872_DAB3_495F_82B2_93EDA6179B84__INCLUDED_)
