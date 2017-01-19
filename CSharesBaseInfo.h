#if !defined(AFX_STOCKBASEINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
#define AFX_STOCKBASEINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSharesBaseInfo  
{
public:
	CSharesBaseInfo();
	virtual ~CSharesBaseInfo();
private:
    HANDLE m_hFile,m_hFileMap;                  //内存映射文件句柄
	BYTE  *m_pbData;                          //内存映射文件首部指针
	BASEINFOHEAD *m_BaseFileHead;                 //内存映射文件头部结构  
	BASEINFO *m_pBaseInfo;                    //内存映射文件基本资料指针
	PBASEINFO *m_pBaseInfoPoint;
//	char BaseInfoXx[40][30];
	char m_sFilePath[255];                     //文件路径
public:
	LONG GetStockCount() { return m_BaseFileHead->StockCount ;} ;  //找查股票
    BOOL GetStockItem(int ls_pos,PBASEINFO & pBaseItem);         //取基本资料 
	BOOL AddStockItem(char *pStockCode,int nKind,PBASEINFO & pBaseItem);      //在基本资料数据文件中增加一块数据区域
	BOOL ReadBaseInfoData(char *StockCode,int nKind,PBASEINFO & pBaseItem);//读入数据
    void ReadBaseInfoDataBJW(char *pFileData,PBASEINFO & pBaseItem); //从博经闻读入
    void ReadBaseInfoDataWG(char *pFileData,PBASEINFO & pBaseItem);  //从万国读入
	BOOL Lookup(char *m_szStockId,int nKind,PBASEINFO &m_pStock);
	BOOL InsertItemPoint(BASEINFO *m_pStk);               //插入一个股票
    BOOL SavePosToFile();

public:
    BOOL InitBaseInfoData(CString Path,BOOL& bExist);    //初始化股票基本资料数据
	void SaveBaseInfoToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush);  //保存股票基本资料头部文件
	BOOL InitBaseInfoDataEmpty(); //初始化股票基本资料数据（空）
    BOOL InitBaseInfoDataExist(); //初始化股票基本资料数据（存在）
	BOOL SetMemoryAllocSize(unsigned int nSize);
    BOOL AddStockItemCorrect(char *pStockCode,PBASEINFO  pBaseItem);
    BOOL AddBlockDataSize();

private:
	void SetBaseInfoData(CString mc,float zbsj,PBASEINFO & pBaseItem,int mode=0);  //设置基本资料信息

};
inline BOOL CSharesBaseInfo::GetStockItem(int ls_pos,PBASEINFO & pBaseItem)
{
    if(ls_pos >= m_BaseFileHead->StockCount )
		return FALSE;
	else
	{
        pBaseItem=&m_pBaseInfo[ls_pos];
		return TRUE;
	}

}
inline void CSharesBaseInfo::SaveBaseInfoToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush)
{
	FlushViewOfFile(lpBaseAddress,dwNumberOfBytesToFlush);
}
#endif // !defined(AFX_STOCKBASEINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
