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
    HANDLE m_hFile,m_hFileMap;                  //�ڴ�ӳ���ļ����
	BYTE  *m_pbData;                          //�ڴ�ӳ���ļ��ײ�ָ��
	BASEINFOHEAD *m_BaseFileHead;                 //�ڴ�ӳ���ļ�ͷ���ṹ  
	BASEINFO *m_pBaseInfo;                    //�ڴ�ӳ���ļ���������ָ��
	PBASEINFO *m_pBaseInfoPoint;
//	char BaseInfoXx[40][30];
	char m_sFilePath[255];                     //�ļ�·��
public:
	LONG GetStockCount() { return m_BaseFileHead->StockCount ;} ;  //�Ҳ��Ʊ
    BOOL GetStockItem(int ls_pos,PBASEINFO & pBaseItem);         //ȡ�������� 
	BOOL AddStockItem(char *pStockCode,int nKind,PBASEINFO & pBaseItem);      //�ڻ������������ļ�������һ����������
	BOOL ReadBaseInfoData(char *StockCode,int nKind,PBASEINFO & pBaseItem);//��������
    void ReadBaseInfoDataBJW(char *pFileData,PBASEINFO & pBaseItem); //�Ӳ����Ŷ���
    void ReadBaseInfoDataWG(char *pFileData,PBASEINFO & pBaseItem);  //���������
	BOOL Lookup(char *m_szStockId,int nKind,PBASEINFO &m_pStock);
	BOOL InsertItemPoint(BASEINFO *m_pStk);               //����һ����Ʊ
    BOOL SavePosToFile();

public:
    BOOL InitBaseInfoData(CString Path,BOOL& bExist);    //��ʼ����Ʊ������������
	void SaveBaseInfoToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush);  //�����Ʊ��������ͷ���ļ�
	BOOL InitBaseInfoDataEmpty(); //��ʼ����Ʊ�����������ݣ��գ�
    BOOL InitBaseInfoDataExist(); //��ʼ����Ʊ�����������ݣ����ڣ�
	BOOL SetMemoryAllocSize(unsigned int nSize);
    BOOL AddStockItemCorrect(char *pStockCode,PBASEINFO  pBaseItem);
    BOOL AddBlockDataSize();

private:
	void SetBaseInfoData(CString mc,float zbsj,PBASEINFO & pBaseItem,int mode=0);  //���û���������Ϣ

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
