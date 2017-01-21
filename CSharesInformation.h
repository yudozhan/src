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
    HANDLE			m_hFile,m_hFileMap;              //�ڴ�ӳ���ļ����
	REALDATA *		m_RealFileHead;					 //�ڴ�ӳ���ļ�ͷ���ṹ  
	BYTE  *			m_pbData;                        //�ڴ�ӳ���ļ��ײ�ָ��
	CReportData *	m_pMapData;						 //��Ʊ�������ݽṹ
	STOCKDATASHOW *	m_pData[STOCKTYPENUM];           //��������ָ��
	DWORD			m_dwStockMaxCount[STOCKTYPENUM]; //ÿ���ͱ�������Ʊ��
	DWORD *			m_pdwStockCurrentCount;          //ÿ���ͱ������й�Ʊ�� 
	Rsdn1 *			Nidx[3];                         //���ڴ�����̸����ǵ���
    Tidxd *			Tidx[3];                         //���ڴ������ǿ��ָ��


	int				m_countUp[4][11];
	int				m_countDown[4][11];
	int				m_countEqual[4][11];
	float			m_indexAmount[4][11];

	char			m_sPath[255];
	CSharesBaseInfo m_StockBaseInfo;         //���ڴ����Ʊ��������



public:
    long	GetTotalStockTypeCount() { return STOCKTYPENUM;}   //�õ���Ʊ��������

    long  GetStockTypeCount(DWORD StockType) //�õ���Ʊ��������
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


    BOOL  GetStockTypeMaxCount(DWORD StockType) { return m_dwStockMaxCount[StockType];}   //�õ���Ʊ��������
	long  GetCount();     //�õ���Ʊ����
	BOOL RemoveKey(char *StockId ,DWORD StockType); //�����Ҳ��Ʊ
	BOOL  GetStockItem(DWORD StockType,DWORD nPos,PCdat1 &pStockData);     //�õ���Ʊ��������
//	BOOL  RemoveKey(char *StockId);       //ɾ����Ʊ����
// 	BOOL  Lookup(char *StockId,PCdat1 &pStockData);  //�Ҳ��Ʊ
//    BOOL  InsertItem(char *StockId ,PCdat1 &pStockData);          //����һ����Ʊ
	BOOL InsertItem(char *StockId ,PCdat1 &pStockData,DWORD StockType); //�����Ҳ��Ʊ
	BOOL Lookup(char *StockId,PCdat1 &pStockData,int nKind);  //�����Ҳ��Ʊ
	BOOL  RecvStockDataForType(PSTOCKDATASHOW &p,UINT StockType); //��һ���Ʊת��Ϊ����ʾ��Ʊ
	BOOL  CSharesInformation::RecvAllStockData(PSTOCKDATASHOW &p);
	BOOL  GetStatisticDataForType(PSTOCKDATASHOW &p,UINT StockType); //��һ���Ʊͳ������ת��Ϊ����ʾ��Ʊ

	BOOL  SetMemoryAllocSize(DWORD StockType,unsigned int nSize);  //���ù�Ʊ������ 
 	int   GetStockPos(int StockType,char *StockId);  //�Ҳ��Ʊ

public:
    BOOL InitRealTimeData(CString path);    //��ʼ��ʵʱ�ļ�����
	void CalcIndexBuyAndSell();  //�������ί����ί����
	void GetIndexRsdn(PRsdn1 &pNidx) { pNidx=&Nidx[0]; }                           //���ڴ�����̸����ǵ���
    void GetIndexTidxd(PTidxd &pTidx ){ pTidx=&Tidx[0]; }                         //���ڴ������ǿ��ָ��
	REALDATA *GetRealDataHead() {return m_RealFileHead ;}          //�õ�ʵʱ����ͷ�ṹ
    BOOL SetRealDataHead(PREALDATA &pRealData);                  //����ʵʱ����ͷ�ṹ 
	BOOL ClearRealDataMinute();                                  //���ʵʱ���ݷ���������
	void SaveRealDataToFile(LPCVOID lpBaseAddress,DWORD dwNumberOfBytesToFlush);                                   //����ʵʱ���ݵ��ļ�                                   
public:
	void ReadBaseInfoData(PCdat1 &pStockData);       //�����Ʊ��������
	void InitBaseInfoData();     //��ʼ��������������
	BOOL SavePosToFile();         //����λ�õ��ļ�
	void SavePosToFile(int StockType); //����λ�õ��ļ�

private:
	void ReadAllBaseInfo();             //��ȫ����Ʊ����������Ϣ

private:
    
    BOOL InsertItemPoint(CReportData *pStockData );               //����һ����Ʊ
	BOOL InitRealTimeDataExist();   //��ʼ��ʵʱ�ļ����ݣ�REALTIME ���ڣ�
    BOOL InitRealTimeDataEmpty();   //��ʼ��ʵʱ�ļ����ݣ�REALTIME �����ڣ�

public:
	float GetValueUpDown(int isDown,int whick_stk,int nKind);
	void CalcUpDown(int which_stk);
	static bool IsZhiShu(int kind);
	void AddStocksIfEmpty();
	static CString Symbol4To6(CString sIn);
	static DWORD GetStockKind(int MarketType,char *strLabel);
	BOOL AddOneStockInfo(CString strStockCode,CString strStockName,
		            CString strStockPyjc,int nKind);                  //���ӹ�Ʊ����
	BOOL MOdifyOneStockInfo(CString strStockCode,CString strStockName,
		            CString strStockPyjc,int nKind);                  //�޸Ĺ�Ʊ����
	BOOL DeleteOneStockInfo(CString strStockCode,int nKind);          //ɾ����Ʊ���� 
	BOOL RemoveAllStockCjmxTj();                            //���ȫ����Ʊ�ɽ���ϸ����


	int GetChuQuanInfo(CString strStockCode,int nKind,PSplit &pSplit);   //�õ���Ȩ����
	BOOL AddChuQuanInfo(CString strStockCode,int nKind,Split *pSplit);   //���ӳ�Ȩ���� 
	BOOL ModifyChuQuanInfo(CString strStockCode,int nWhichItem,Split *pSplit,int nKind); //�޸ĳ�Ȩ����
	BOOL DeleteChuQuanInfo(CString strStockCode,int nWhichItem,int nKind);  //ɾ����Ȩ����

	BOOL ExportChuQuanInfo(CString strStockCode,PSplit &pSplit,   //�����Ȩ����
		int& nChuquanTotalTimes,int nKind); 
	BOOL ImportChuQuanInfo(CString strStockCode,Split *pSplit,    //�����Ȩ���� 
		int nChuquanTotalTimes,int nKind);
    BOOL ImportCaiwuInfo(BASEINFO *pBaseinfo);
	BOOL ImportCwData(CString FileName);
	BOOL SaveCaiwuData();

	BOOL RemoveStockInfo();                                       //ɾ����Ȩ����
	BOOL RemoveStockCjmxTj(char *code,int nKind,BOOL IsAdd);                //���ӻ�ɾ���ɽ���ϸͳ�Ʊ�־
	BOOL LookupBase(char *m_szStockId,int nKind,PBASEINFO &m_pStock);      
    BOOL InsertItemCorrect(char *StockId ,PCdat1 pStockData, DWORD StockType); //�����Ҳ��Ʊ
	


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
	return m_StockBaseInfo.Lookup(m_szStockId,nKind,m_pStock);         //���ڴ����Ʊ��������
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

}  //����������λ�õõ�����

#endif // !defined(AFX_STOCKDATAINFO_H__4BE51F0E_A261_11D2_B30C_00C04FCCA334__INCLUDED_)
