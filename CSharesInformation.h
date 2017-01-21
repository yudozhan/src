#if !defined(AFX_STOCKDATAINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
#define AFX_STOCKDATAINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define ADDCOUNT   10 
class CSharesInformation  
{
public:
	CSharesInformation();
	virtual ~CSharesInformation();
private:
    HANDLE			m_hFile,m_hFileMap;              //内存映射文件句柄
	REALDATA *		m_RealFileHead;					 //内存映射文件头部结构  
	BYTE  *			m_pbData;                        //内存映射文件首部指针
	CReportData *	m_pMapData;						 //股票行情数据结构
	STOCKDATASHOW *	m_pData[STOCKTYPENUM];           //行情数据指针
	DWORD			m_dwStockMaxCount[STOCKTYPENUM]; //每类型保存最大股票数
	DWORD *			m_pdwStockCurrentCount;          //每类型保存现有股票数 
	Rsdn1 *			Nidx[3];                         //用于储存大盘个股涨跌数
    Tidxd *			Tidx[3];                         //用于储存大盘强弱指标


	int				m_countUp[4][11];
	int				m_countDown[4][11];
	int				m_countEqual[4][11];
	float			m_indexAmount[4][11];

	char			m_sPath[255];
	CSharesBaseInfo m_StockBaseInfo;         //用于处理股票基本资料



public:
    long	GetTotalStockTypeCount() { return STOCKTYPENUM;}   //得到股票类型总数

    long  GetStockTypeCount(DWORD StockType) //得到股票类型总数
	{ 
		int nNum = 0;

		if(StockType >= SHZS && StockType <= SZCYB)
		{
			nNum = m_pdwStockCurrentCount[StockType];
		}
		else if (StockType > STOCKTYPENUM)
		{
			int nTypeSh = StockType/100 - 1;
			int nTypeSz = StockType%100;
			nNum = m_pdwStockCurrentCount[nTypeSh] + m_pdwStockCurrentCount[nTypeSz];

		}
		return nNum;
	}   


    BOOL  GetStockTypeMaxCount(DWORD StockType) { return m_dwStockMaxCount[StockType];}   //得到股票类型总数
	long  GetCount();     //得到股票总数
	BOOL RemoveKey(char *StockId ,DWORD StockType); //按类找查股票
	BOOL  GetStockItem(DWORD StockType,DWORD nPos,PCdat1 &pStockData);     //得到股票行情数据
//	BOOL  RemoveKey(char *StockId);       //删除股票数据
// 	BOOL  Lookup(char *StockId,PCdat1 &pStockData);  //找查股票
//    BOOL  InsertItem(char *StockId ,PCdat1 &pStockData);          //插入一个股票
	BOOL InsertItem(char *StockId ,PCdat1 &pStockData,DWORD StockType); //按类找查股票
	BOOL Lookup(char *StockId,PCdat1 &pStockData,int nKind);  //按类找查股票
	BOOL  RecvStockDataForType(PSTOCKDATASHOW &p,UINT StockType); //将一类股票转换为可显示股票
	BOOL  CSharesInformation::RecvAllStockData(PSTOCKDATASHOW &p);
	BOOL  GetStatisticDataForType(PSTOCKDATASHOW &p,UINT StockType); //将一类股票统计数据转换为可显示股票

	BOOL  SetMemoryAllocSize(DWORD StockType,unsigned int nSize);  //设置股票种类数 
 	int   GetStockPos(int StockType,char *StockId);  //找查股票

public:
    BOOL InitRealTimeData(CString path);    //初始化实时文件数据
	void CalcIndexBuyAndSell();  //计算大盘委买与委卖数
	void GetIndexRsdn(PRsdn1 &pNidx) { pNidx=&Nidx[0]; }                           //用于储存大盘个股涨跌数
    void GetIndexTidxd(PTidxd &pTidx ){ pTidx=&Tidx[0]; }                         //用于储存大盘强弱指标
	REALDATA *GetRealDataHead() {return m_RealFileHead ;}          //得到实时数据头结构
    BOOL SetRealDataHead(PREALDATA &pRealData);                  //设置实时数据头结构 
	BOOL ClearRealDataMinute();                                  //清除实时数据分钟线数据
	void SaveRealDataToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush);                                   //保存实时数据到文件                                   
public:
	void ReadBaseInfoData(PCdat1 &pStockData);       //读入股票基本资料
	void InitBaseInfoData();     //初始化基本资料数据
	BOOL SavePosToFile();         //保存位置到文件
	void SavePosToFile(int StockType); //保存位置到文件

private:
	void ReadAllBaseInfo();             //读全部股票基本资料信息

private:
    
    BOOL InsertItemPoint(CReportData *pStockData );               //插入一个股票
	BOOL InitRealTimeDataExist();   //初始化实时文件数据（REALTIME 存在）
    BOOL InitRealTimeDataEmpty();   //初始化实时文件数据（REALTIME 不存在）

public:
	float GetValueUpDown(int isDown,int whick_stk,int nKind);
	void CalcUpDown(int which_stk);
	static bool IsZhiShu(int kind);
	void AddStocksIfEmpty();
	static CString Symbol4To6(CString sIn);
	static DWORD GetStockKind(int MarketType,char *strLabel);
	BOOL AddOneStockInfo(CString strStockCode,CString strStockName,
		            CString strStockPyjc,int nKind);                  //增加股票数据
	BOOL MOdifyOneStockInfo(CString strStockCode,CString strStockName,
		            CString strStockPyjc,int nKind);                  //修改股票数据
	BOOL DeleteOneStockInfo(CString strStockCode,int nKind);          //删除股票数据 
	BOOL RemoveAllStockCjmxTj();                            //清除全部股票成交明细数据


	int GetChuQuanInfo(CString strStockCode,int nKind,PSplit &pSplit);   //得到除权数据
	BOOL AddChuQuanInfo(CString strStockCode,int nKind,Split *pSplit);   //增加除权数据 
	BOOL ModifyChuQuanInfo(CString strStockCode,int nWhichItem,Split *pSplit,int nKind); //修改除权数据
	BOOL DeleteChuQuanInfo(CString strStockCode,int nWhichItem,int nKind);  //删除除权数据

	BOOL ExportChuQuanInfo(CString strStockCode,PSplit &pSplit,   //输出除权数据
		int& nChuquanTotalTimes,int nKind); 
	BOOL ImportChuQuanInfo(CString strStockCode,Split *pSplit,    //输入除权数据 
		int nChuquanTotalTimes,int nKind);
    BOOL ImportCaiwuInfo(BASEINFO *pBaseinfo);
	BOOL ImportCwData(CString FileName);
	BOOL SaveCaiwuData();

	BOOL RemoveStockInfo();                                       //删除除权数据
	BOOL RemoveStockCjmxTj(char *code,int nKind,BOOL IsAdd);                //增加或删除成交明细统计标志
	BOOL LookupBase(char *m_szStockId,int nKind,PBASEINFO &m_pStock);      
    BOOL InsertItemCorrect(char *StockId ,PCdat1 pStockData, DWORD StockType); //按类找查股票
	


    BOOL DeleteAllStockFromStockType();
    BOOL InitEmptyDatabase();

    BOOL AddBaseinfoPoint(CString strStockCode,int nKind,PBASEINFO & pBaseinfo);

	void UpdateCwData(void);

#ifdef WIDE_NET_VERSION
	BOOL ClearAllRealTimeMarketData();
#endif

protected:
	void ClearUpDown(int nMarket);
};
inline BOOL CSharesInformation::LookupBase(char *m_szStockId,int nKind,PBASEINFO &m_pStock)
{
	return m_StockBaseInfo.Lookup(m_szStockId,nKind,m_pStock);         //用于处理股票基本资料
}

inline void CSharesInformation::SaveRealDataToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush)
{
	FlushViewOfFile(lpBaseAddress,dwNumberOfBytesToFlush);
}
inline BOOL CSharesInformation::SetRealDataHead(PREALDATA &pRealData)
{
	if(m_RealFileHead==NULL)
		return FALSE;
	pRealData=m_RealFileHead;
    SaveRealDataToFile(m_RealFileHead,sizeof(REALDATA)+sizeof(long)*STOCKTYPENUM); 
	return TRUE;
}

inline long  CSharesInformation::GetCount()
{
	long temp=0;
	for(int i=0;i<STOCKTYPENUM;i++)
       temp+=m_pdwStockCurrentCount[i];
	return temp;
}
inline BOOL CSharesInformation::GetStockItem(DWORD StockType,DWORD nPos,PCdat1 &pStockData)
{
	if (StockType < STOCKTYPENUM)
	{
		if(nPos < m_dwStockMaxCount[StockType])
		{
			pStockData=m_pData[StockType][nPos].pItem; 
			return TRUE;
		}
		else
		{
			pStockData=NULL;
			return FALSE;
		}	
	}
	return FALSE;

}  //根据类型与位置得到数据

#endif // !defined(AFX_STOCKDATAINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
