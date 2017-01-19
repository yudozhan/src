// ManagerStockTypeData.h: interface for the CManageBlockData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MANAGERSTOCKTYPEDATA_H__520245A1_5257_11D4_8829_00400552E583__INCLUDED_)
#define AFX_MANAGERSTOCKTYPEDATA_H__520245A1_5257_11D4_8829_00400552E583__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define CHOOSEPOS   511
#define ADDSIZES  50
#include "AFXCMN.H"
class CManageBlockData  
{
public:
	CManageBlockData();
	virtual ~CManageBlockData();
	enum {ZGB=0,LTG=1,OTHER=2	 };     //用于表示权重类型  0 表示总股本 1表示流通股本 2表示相等权重 

private:
    HANDLE m_hFile,m_hFileMap;          //内存映射文件句柄
	BYTE  *m_pbtData;                   //内存映射文件首部指针
    BLOCKFILEHEAD *m_pStockTypeHead;     //板块数据头结构指针
    BLOCKINFO *m_pBlockInfo;			//板块数据结构指针
    STOCKINFOINBLOCK *m_pStockInfo;       //板块中股票数据结构指针 
	STOCKPOINTINFO  *m_pStockPoint;    //用于保存股票结构指针

private:
	CTaiShanDoc   *m_pDoc;          //文档类指针


private:
	BOOL InitBlockDataExist(CString path);       //初始化板块数据文件（StockTypeInfo.dat 存在）
    BOOL InitBlockDataEmpty(CString path);       //初始化板块数据文件（StockTypeInfo.dat 不存在）
	BOOL AddBlockDataSize(int AddSize=ADDSIZES);                     //增加股票保存区域 

    BOOL SetMemoryAllocSize(unsigned int nSize);     //分配内存
	void SaveDataToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush);                       //保存内存数据到文件
    BOOL InsertItemPoint(STOCKINFOINBLOCK *m_pStk);    //插入一个股票
	BOOL SavePosToFile();                             //保存股票位置

public:
	void GetBlockHeadPoint(PBLOCKFILEHEAD &pStockTypeHead){pStockTypeHead=m_pStockTypeHead;}//返回板块结构指针
    BOOL InitBlockData(CString path);    //初始化板块数据
	BOOL InitStockPoint();                   //初始化股票指针
	BOOL InitBlockPoint();                //初始化股票指针

	void CalcBlockOpenClosePrice();             //计算全部板块指数
	void CalcRealTimeBlockIndex();       //计算实时板块指数
	void CalcBlockKlineData();       //计算实时板块指数
	BOOL AddBlockType(CString strBlockTypeName);//增加一个板块分类
	BOOL DelBlockType(CString strBlockTypeName);//删除一个板块分类
	void GetAllBlockTypeName(CStringArray &BlockTypeNameArray);//得到全部板块分类名称
	void GetAllBlockCodesFromBlockTypeName(CStringArray &BlockCodeArray,CString strBlockType);//根据板块分类名称得到全部相关板块
	void GetAllBlockNamesFromBlockTypeName(CStringArray &BlockNameArray,CString strBlockType);//根据板块分类名称得到全部相关板块
	void UpdateAllBlockStocks();
	UINT GetBlockCountFromBlockTypeName(CString strBlockType);//根据板块分类名称得到全部相关板块数


//板块部分处理
public:
	BOOL InsertBlock(PBLOCKINFO &m_pStktype,int m_iPos);                  //插入板块
	int  GetStockCountFromBlockType(char *m_pszCode);                                  //得到某一板块股票数
	BOOL DeleteBlock(char *m_pszStockTypeCode);                                  //删除板块
	BOOL GetBlockPoint(PBLOCKINFO &m_pStktype,char *m_pszStockTypeCode); //得到板块指针
    int  GetInsertBlockPos();                                                //得到要插入板块位置 
    UINT GetBlockPos(char *m_szStockTypeCode);                            //得到板块位置号 

	BOOL InsertStockToBlock(PCdat1 &m_pCdat,char *m_szStockTypeCode);        //插入股票到某一板块
	BOOL DeleteStockFromBlock(char *m_pszStockCode,int nKind,char *m_szStockTypeCode);      //删除股票从某一板块

	BOOL GetStockFromBlockCode(SymbolKindArr &m_StockCodeArray,char *m_pszStockTypeCode);          //从板块中得到股票代码
	BOOL GetStockFromBlockName(SymbolKindArr &m_StockCodeArray,char *m_pszStockTypeName);          //从板块中得到股票代码
	int  GetBlockTypeCounts();          //从板块中得到板块总数
	void GetAllBlockCodes(CStringArray &m_StockTypeCodeArray);                //得到全部板块指数代码 
	void GetAllBlockNames(CStringArray &m_StockTypeNameArray);                //得到全部板块指数名称
	BOOL GetBlockPointFromName(PBLOCKINFO &m_pStktype,char *m_pszStockTypeName); //从板块名称返回板块结构指针

    BOOL ImportBlockData(CString m_szPathStr,CString m_szStockNameStr,PBLOCKINFO &m_pStktype,int nRightType=LTG);      //导入某一板块数据
	BOOL RecvBlockDataForBlockType(PBLOCKINFO &pData,CString strBlockTypeName); //将一类板块转换为可显示板块
//股票处理部分
public:
	static bool CheckKind(int i);
 	BOOL  Lookup(char *m_szStockId,int nKind,PSTOCKINFOINBLOCK &m_pStock);                  //找查股票
    BOOL  InsertStockItem(char *m_szStockId ,int nKind,PSTOCKINFOINBLOCK &m_pStock);        //插入一个股票
	BOOL  DeleteStockItem(char *m_szStockId,int nKind);  	//删除股票
	BOOL  RemoveDeletedStock();                 //删除股票的保存区域
	void  CheckStockCount(int AddStockCount);
//自选股处理部分
    BOOL  InsertStockToChoose(char *m_szStockId ,int nKind,CString strTypeName = "");        //插入一个股票到自选股
	BOOL  DeleteStockFromChoose(char *m_szStockId,int nKind);  	//删除股票从自选股
	BOOL  GetChooseStockCode(SymbolKindArr &m_StockCodeArray);	//得到自选股
	int   GetChooseStockCounts();	//得到自选股
	BOOL  GetChooseStockTypeNames(CStringArray &chooseType);	//得到自选股
	BOOL  GetChooseStockTypeCodes(CStringArray &chooseCode);	//得到自选股
	BOOL  GetChooseStockCodeFromTypeName(SymbolKindArr &m_StockCodeArray,CString strTypeName);	//得到自选股

//板块历史日线数据刷新
    void RefreshHistoryDayLineData(char *pszStockTypeCode, CProgressCtrl *pProgressCtrl = NULL);
	void RefreshHistoryMin5LineData(char * pszStockTypeCode,CProgressCtrl *pProgressCtrl = NULL);

	BOOL GetStockTypeStartDate(char * pszStockTypeCode,SymbolKindArr &StockCodeArray,time_t &t);

    BOOL InsertStockItemCorrect(char *m_szStockId ,PSTOCKINFOINBLOCK m_pStock) ;       //插入一个股票
    long CorrectKline(Kline *pKline,long LineCount);
	float GetStockBlockRight(char *StockTypeCode);
    BOOL CheckBlockName(char *BlockName,char *BlockCode);


public:
	BOOL ImportBlockTypeName(CStringArray& BlockTypeName);
	BOOL ImportBlockName(CString BlockName,CStringArray& StockArray);
};
inline void CManageBlockData::SaveDataToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush)
{
	FlushViewOfFile(lpBaseAddress,dwNumberOfBytesToFlush);
}
inline UINT CManageBlockData::GetBlockPos(char *m_szStockTypeCode)                            //得到板块位置号 
{
	char mtemp[4];
	memcpy(mtemp,m_szStockTypeCode + 3,3);
	mtemp[3]='\0';
	return ( UINT )atoi(mtemp);
}

#endif // !defined(AFX_MANAGERSTOCKTYPEDATA_H__520245A1_5257_11D4_8829_00400552E583__INCLUDED_)
