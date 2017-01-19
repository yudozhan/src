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
	enum {ZGB=0,LTG=1,OTHER=2	 };     //���ڱ�ʾȨ������  0 ��ʾ�ܹɱ� 1��ʾ��ͨ�ɱ� 2��ʾ���Ȩ�� 

private:
    HANDLE m_hFile,m_hFileMap;          //�ڴ�ӳ���ļ����
	BYTE  *m_pbtData;                   //�ڴ�ӳ���ļ��ײ�ָ��
    BLOCKFILEHEAD *m_pStockTypeHead;     //�������ͷ�ṹָ��
    BLOCKINFO *m_pBlockInfo;			//������ݽṹָ��
    STOCKINFOINBLOCK *m_pStockInfo;       //����й�Ʊ���ݽṹָ�� 
	STOCKPOINTINFO  *m_pStockPoint;    //���ڱ����Ʊ�ṹָ��

private:
	CTaiShanDoc   *m_pDoc;          //�ĵ���ָ��


private:
	BOOL InitBlockDataExist(CString path);       //��ʼ����������ļ���StockTypeInfo.dat ���ڣ�
    BOOL InitBlockDataEmpty(CString path);       //��ʼ����������ļ���StockTypeInfo.dat �����ڣ�
	BOOL AddBlockDataSize(int AddSize=ADDSIZES);                     //���ӹ�Ʊ�������� 

    BOOL SetMemoryAllocSize(unsigned int nSize);     //�����ڴ�
	void SaveDataToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush);                       //�����ڴ����ݵ��ļ�
    BOOL InsertItemPoint(STOCKINFOINBLOCK *m_pStk);    //����һ����Ʊ
	BOOL SavePosToFile();                             //�����Ʊλ��

public:
	void GetBlockHeadPoint(PBLOCKFILEHEAD &pStockTypeHead){pStockTypeHead=m_pStockTypeHead;}//���ذ��ṹָ��
    BOOL InitBlockData(CString path);    //��ʼ���������
	BOOL InitStockPoint();                   //��ʼ����Ʊָ��
	BOOL InitBlockPoint();                //��ʼ����Ʊָ��

	void CalcBlockOpenClosePrice();             //����ȫ�����ָ��
	void CalcRealTimeBlockIndex();       //����ʵʱ���ָ��
	void CalcBlockKlineData();       //����ʵʱ���ָ��
	BOOL AddBlockType(CString strBlockTypeName);//����һ��������
	BOOL DelBlockType(CString strBlockTypeName);//ɾ��һ��������
	void GetAllBlockTypeName(CStringArray &BlockTypeNameArray);//�õ�ȫ������������
	void GetAllBlockCodesFromBlockTypeName(CStringArray &BlockCodeArray,CString strBlockType);//���ݰ��������Ƶõ�ȫ����ذ��
	void GetAllBlockNamesFromBlockTypeName(CStringArray &BlockNameArray,CString strBlockType);//���ݰ��������Ƶõ�ȫ����ذ��
	void UpdateAllBlockStocks();
	UINT GetBlockCountFromBlockTypeName(CString strBlockType);//���ݰ��������Ƶõ�ȫ����ذ����


//��鲿�ִ���
public:
	BOOL InsertBlock(PBLOCKINFO &m_pStktype,int m_iPos);                  //������
	int  GetStockCountFromBlockType(char *m_pszCode);                                  //�õ�ĳһ����Ʊ��
	BOOL DeleteBlock(char *m_pszStockTypeCode);                                  //ɾ�����
	BOOL GetBlockPoint(PBLOCKINFO &m_pStktype,char *m_pszStockTypeCode); //�õ����ָ��
    int  GetInsertBlockPos();                                                //�õ�Ҫ������λ�� 
    UINT GetBlockPos(char *m_szStockTypeCode);                            //�õ����λ�ú� 

	BOOL InsertStockToBlock(PCdat1 &m_pCdat,char *m_szStockTypeCode);        //�����Ʊ��ĳһ���
	BOOL DeleteStockFromBlock(char *m_pszStockCode,int nKind,char *m_szStockTypeCode);      //ɾ����Ʊ��ĳһ���

	BOOL GetStockFromBlockCode(SymbolKindArr &m_StockCodeArray,char *m_pszStockTypeCode);          //�Ӱ���еõ���Ʊ����
	BOOL GetStockFromBlockName(SymbolKindArr &m_StockCodeArray,char *m_pszStockTypeName);          //�Ӱ���еõ���Ʊ����
	int  GetBlockTypeCounts();          //�Ӱ���еõ��������
	void GetAllBlockCodes(CStringArray &m_StockTypeCodeArray);                //�õ�ȫ�����ָ������ 
	void GetAllBlockNames(CStringArray &m_StockTypeNameArray);                //�õ�ȫ�����ָ������
	BOOL GetBlockPointFromName(PBLOCKINFO &m_pStktype,char *m_pszStockTypeName); //�Ӱ�����Ʒ��ذ��ṹָ��

    BOOL ImportBlockData(CString m_szPathStr,CString m_szStockNameStr,PBLOCKINFO &m_pStktype,int nRightType=LTG);      //����ĳһ�������
	BOOL RecvBlockDataForBlockType(PBLOCKINFO &pData,CString strBlockTypeName); //��һ����ת��Ϊ����ʾ���
//��Ʊ������
public:
	static bool CheckKind(int i);
 	BOOL  Lookup(char *m_szStockId,int nKind,PSTOCKINFOINBLOCK &m_pStock);                  //�Ҳ��Ʊ
    BOOL  InsertStockItem(char *m_szStockId ,int nKind,PSTOCKINFOINBLOCK &m_pStock);        //����һ����Ʊ
	BOOL  DeleteStockItem(char *m_szStockId,int nKind);  	//ɾ����Ʊ
	BOOL  RemoveDeletedStock();                 //ɾ����Ʊ�ı�������
	void  CheckStockCount(int AddStockCount);
//��ѡ�ɴ�����
    BOOL  InsertStockToChoose(char *m_szStockId ,int nKind,CString strTypeName = "");        //����һ����Ʊ����ѡ��
	BOOL  DeleteStockFromChoose(char *m_szStockId,int nKind);  	//ɾ����Ʊ����ѡ��
	BOOL  GetChooseStockCode(SymbolKindArr &m_StockCodeArray);	//�õ���ѡ��
	int   GetChooseStockCounts();	//�õ���ѡ��
	BOOL  GetChooseStockTypeNames(CStringArray &chooseType);	//�õ���ѡ��
	BOOL  GetChooseStockTypeCodes(CStringArray &chooseCode);	//�õ���ѡ��
	BOOL  GetChooseStockCodeFromTypeName(SymbolKindArr &m_StockCodeArray,CString strTypeName);	//�õ���ѡ��

//�����ʷ��������ˢ��
    void RefreshHistoryDayLineData(char *pszStockTypeCode, CProgressCtrl *pProgressCtrl = NULL);
	void RefreshHistoryMin5LineData(char * pszStockTypeCode,CProgressCtrl *pProgressCtrl = NULL);

	BOOL GetStockTypeStartDate(char * pszStockTypeCode,SymbolKindArr &StockCodeArray,time_t &t);

    BOOL InsertStockItemCorrect(char *m_szStockId ,PSTOCKINFOINBLOCK m_pStock) ;       //����һ����Ʊ
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
inline UINT CManageBlockData::GetBlockPos(char *m_szStockTypeCode)                            //�õ����λ�ú� 
{
	char mtemp[4];
	memcpy(mtemp,m_szStockTypeCode + 3,3);
	mtemp[3]='\0';
	return ( UINT )atoi(mtemp);
}

#endif // !defined(AFX_MANAGERSTOCKTYPEDATA_H__520245A1_5257_11D4_8829_00400552E583__INCLUDED_)
