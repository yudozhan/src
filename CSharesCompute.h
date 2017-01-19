#if !defined(AFX_STOCKDATACALC_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
#define AFX_STOCKDATACALC_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "StructKlineView.h"

#define MAX_COUNT_CDAT1    30

class CTaiShanDoc;
class CSharesCompute  
{
public:
	CSharesCompute();
	virtual ~CSharesCompute();
private:
	CTaiShanDoc   *m_MainDocument;
public:
	void AddDataCdat1(CReportData* p);
	SymbolKindArr m_dataArr[2];//SymbolKind
	static CString GetIndexSymbol(int nKind);
	static int GetMarketKind(int nKind);
	void SetMainDocument(CTaiShanDoc *m_MainDoc ) { m_MainDocument = m_MainDoc;}   //设置文档指针
	void chk_rdp(CReportData  *Cdat,float NewPrice,int iTime);                                        //计算该股票RDP值
	void UpdateHs(CReportData  *Cdat, short Curr_Min);                                    //更新该股票每次成交数据
	void SendCuoHeDataToDialog(CReportData *Cdat);                                    //送撮合数据到对话框
	short GetStockMinute(time_t time,int mode);                                 //取得时间
    void  StockDataDayUpdate(DAY_TOTAL_STRUCTEx * m_GpDay);                     //股票处理程序             
    int CheckStockDaytime(DAY_TOTAL_STRUCTEx * m_GpDay,int index);              //股票处理程序     
    void  StockDataMin5Update(MIN5_TOTAL_STRUCTEx * m_GpMin5);                     //股票处理程序             
	int CheckStockMin5time(MIN5_TOTAL_STRUCTEx * m_GpMin5,int index);              //股票处理程序             

    void StockDataPowerUpdate(POWER_TOTAL_STRUCTEx * m_GpPower);                  //股票处理程序             

#ifdef WIDE_NET_VERSION
	void WideStockDataUpdate(RCV_WIDOFEREPORT_STRUCTEx* m_GpHqReport);                           //更新即时行情数据             
	void WideStockItemUpdate(CReportData  *Cdat,RCV_WIDOFEREPORT_STRUCTEx* m_GpHqReport);             //用于计算每一股票数据
    void WideStockDataMinUpdate(MIN_TOTAL_STRUCTEx * m_GpMinute);                  //股票处理程序             
#else
	void StockDataUpdate(RCV_REPORT_STRUCTEx* m_GpHqReport);                           //更新即时行情数据             
	void StockItemUpdate(CReportData  *Cdat,RCV_REPORT_STRUCTEx* m_GpHqReport);             //用于计算每一股票数据
    void StockDataMinUpdate(MIN_TOTAL_STRUCTEx * m_GpMinute);                  //股票处理程序             

#endif	

private:

};
#endif // !defined(AFX_STOCKDATACalc_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
