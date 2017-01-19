// TechFileExtData.cpp: implementation of the CTechFileExtData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TechFileExtData.h"
#include "FileExtendData.h"
#include "FileInOutData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define KLINEHEAD  16
#define KLINESMALLHEAD 4096*3 * 64 + KLINEHEAD
#define FixedKlinePerBlock	240
#define KlineByteEach	8

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTechFileExtData CTechFileExtData::g_fileExtData ;
int CTechFileExtData::m_nIDPre =-1;

CTechFileExtData::CTechFileExtData()
{
	MaxNumStock = 4096*3;
	m_bDay = true;

}

CTechFileExtData::~CTechFileExtData()
{

}

CString CTechFileExtData::GetSymbol(int iStock)
{
	CString s="";
	if(m_pSymbolToPos == NULL)
		AddIndexToMap();

	int i=iStock;

	int addr = KLINEHEAD;
	this->Seek(addr+i*64,this->begin);
	KLINE_SMALLHEAD* pKlineSmallHead3 = (KLINE_SMALLHEAD*)this->GetFileCurrentPointer();

	if(pKlineSmallHead3)
		s = pKlineSmallHead3->StockSign;

	return s;
}

int CTechFileExtData::WriteIn(RecordIndicator* pData,CString symbol,int nIndicatorExtendID,int nCount) 
{
	int n = 0;
	n=g_fileExtData.WriteData(pData,symbol,nIndicatorExtendID,nCount);
	return n;
}
int CTechFileExtData::ReadOut(RecordIndicator*& pData,CString symbol,int nIndicatorExtendID,int nDays)//nDays ==-1 is total
{
	int n = 0;
	n=g_fileExtData.ReadData(pData,symbol,nIndicatorExtendID,nDays);
	return n;
}
void CTechFileExtData::DeleteMap()
{
	if(m_pSymbolToPos!=NULL)
	{
		m_pSymbolToPos->RemoveAll ();
		delete m_pSymbolToPos;
		m_pSymbolToPos = NULL;
	}

}

int CTechFileExtData::CreateOrMoveSmallBlock(KLINE_SMALLHEAD *pKlineSmallHead,int& nBlock)//to create a new block,or move
//return the count of kline to be deleted
{
	//if need, to move the small header's block
	int rtn = 0;
	int nMax = MAX_BLOCK_USE;
	if(nBlock>=nMax)
	{
		WORD first = pKlineSmallHead->symBlock[0];
		for(int j = 0;j<nMax-1;j++)
		{
			pKlineSmallHead->symBlock[j] = pKlineSmallHead->symBlock[j+1];
		}
		pKlineSmallHead->symBlock[nMax-1] = first;

		nBlock = nMax-1;
		ASSERT(pKlineSmallHead->symBlock[nBlock]!=0xffff);
		if(nBlock<0)
		{
			ASSERT(FALSE);
			nBlock=0;
		}
		rtn = FixedKlinePerBlock;
		return rtn;
	}
	//if need, to create a new block
	if(pKlineSmallHead->symBlock[nBlock]==0xffff)
	{
		int nCountBlock = this->GetSmallBlockCount ();
		this->SetSmallBlockCount (nCountBlock+1);
		int addr2 = KLINESMALLHEAD + nCountBlock 
				* KlineByteEach *  FixedKlinePerBlock;
									  //计算股票K线的当前位置
		char buff[ KlineByteEach *  FixedKlinePerBlock];
		memset(buff,255,KlineByteEach *  FixedKlinePerBlock);
		this->Seek(addr2,this->begin);
		this->Write (buff,KlineByteEach *  FixedKlinePerBlock);

		ASSERT(nBlock>=0&&nBlock<26);
		pKlineSmallHead->symBlock[nBlock]=(WORD)nCountBlock;
		nCountBlock++;
	}

	//test repeated block
/*	if(m_bFirstWrite == true)
	{
		CMap<int ,int ,int,int> map;
		
		int nStock = GetStockNumber ();
		for(int i=0;i<nStock;i++)
		{
			int addr = KLINEHEAD+i*64;
			Seek(addr,CTechFileKLine::begin);
			KLINE_SMALLHEAD klineSmallHead ;
			KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
			Read(pKlineSmallHead,sizeof(KLINE_SMALLHEAD));//= (KLINE_SMALLHEAD*)pFile->GetFileCurrentPointer();
			CString symbol (pKlineSmallHead->StockSign);
			for(int j=0;j<26;j++)
			{
				if(pKlineSmallHead->symBlock [j] == 0xffff) break;
				int iData = -1;
				int iIn = pKlineSmallHead->symBlock [j] ;
				if(iIn == 3844 || iIn == 3843)
					iIn = iIn;
				if(map.Lookup(iIn,iData))
				{
					int nBolck = j;
					ASSERT(j>=0&&j<26);
					if(pKlineSmallHead->numKln >(j)*FixedKlinePerBlock)
						pKlineSmallHead->numKln = (j)*FixedKlinePerBlock;
					for(int k=j;k<26;k++)
						pKlineSmallHead->symBlock [k] = 0xffff;

					this->SetKlineSmallHeader (i,pKlineSmallHead);
					break;

				}
				else
					map[iIn] = iIn;
			}
		}

		m_bFirstWrite = false;
	}*/
	return 0;

}
int CTechFileExtData::WriteData(RecordIndicator* pData,CString symbol,int nIndicatorExtendID,int nCount)
{
	if(!Open( nIndicatorExtendID,CTechMemFile::modeReadWrite)) return 0;

	int  n = 0 ;
	if(symbol.GetLength ()!=7) return n;
	RecordIndicator* pKline = pData;
	int nWrite = nCount;

	KLINE_SMALLHEAD klineSmallHead;
	KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
	int nIndexStock = GetKlineSmallHeader(symbol,pKlineSmallHead);
	int nMaxBlock = MAX_BLOCK_USE;
	if(pKlineSmallHead->numKln > nMaxBlock*FixedKlinePerBlock)//
	{
		int n1 = pKlineSmallHead->numKln/FixedKlinePerBlock;
		int n2 = pKlineSmallHead->numKln%FixedKlinePerBlock == 0 ? 0:1;
		int nBlockCur = n1 + n2;
		int nMoveBlock = nBlockCur - nMaxBlock ;
		if(nMoveBlock>0)
		{
			for(int i=0;i<nMoveBlock;i++)
			{
				WORD wdVal = pKlineSmallHead->symBlock [0];
				for(int j=0;j<nBlockCur-1;j++)
				{
					pKlineSmallHead->symBlock [j] = pKlineSmallHead->symBlock [j+1];
					ASSERT(pKlineSmallHead->symBlock [j+1]!=0xffff);
				}
				if(nBlockCur-1>=0)
					pKlineSmallHead->symBlock [nBlockCur-1] = wdVal;
				else
					ASSERT(FALSE);
			}
			pKlineSmallHead->numKln = pKlineSmallHead->numKln - nMoveBlock*FixedKlinePerBlock;
		}
		else
		{
			pKlineSmallHead->numKln = 0;
			ASSERT(FALSE);
		}
		this->SetKlineSmallHeader (nIndexStock,pKlineSmallHead);
	}

	ASSERT(pKlineSmallHead!=NULL);

	if(pKline == NULL || nWrite<=0) 
		return 0;

	if(pKlineSmallHead->numKln <=0)
		pKlineSmallHead->numKln =0;

	//find the position of kline

	//read from file to save the kline data ;
	RecordIndicator* pKlineFile = NULL;//
	RecordIndicator* pKlineMid = pKline;//
	CArray<RecordIndicator,RecordIndicator&> klineArray;
	int nWrite2=0;

//	to get end foot
	int nDeleted = 0;
	int blkCount= 0  ;	 
	int nAddOld = 0;
	int b1 = -1;
	int e1 = -1;
	RecordIndicator * pDataRead;
	{
		pDataRead = new RecordIndicator[pKlineSmallHead->numKln];
		int b = pData[0].m_time ;
		int e = pData[nCount-1].m_time;
		b = FileExtendData::RegularTime(b,m_bDay);
		e = FileExtendData::RegularTime(e,m_bDay);

		blkCount= (pKlineSmallHead->numKln-1) / FixedKlinePerBlock  ;
		
		int i;
		RecordIndicator* pDataOld = NULL;
		for(i = pKlineSmallHead->numKln-1;i>=0;i--)
		{
			int stockCount=i % FixedKlinePerBlock  ;
			blkCount= i / FixedKlinePerBlock  ;	  

			int addr = KLINESMALLHEAD + pKlineSmallHead->symBlock[blkCount] 
					* KlineByteEach *  FixedKlinePerBlock
						 + stockCount * KlineByteEach;
			this->Seek(addr,this->begin);
			Read(pDataRead+i,KlineByteEach);

			int time = FileExtendData::RegularTime(pDataRead[i].m_time ,m_bDay);
			if(time < b && b1 == -1)
				b1 = i+1;
			if(time <= e && e1 == -1)
				e1 = i+1;
			if(b1>=0 && e1>=0) break;
		}

		if(b1 == pKlineSmallHead->numKln) 
		{//add write all
/*			fl.Seek (sizeof(ExtHeadData)+sizeof(RecordIndicator)*(head.m_nCount) ,CFile::begin);
			fl.Write (pData,nCount*sizeof(RecordIndicator));
			head.m_nCount = nCount+n;
			SetFileHead( head, fl);
			;*/
			e1 = b1+nCount;
		}
		else
		{
/*			if(b1 == -1) b1 = 0;
			if(e1 == -1) e1 = 0;
			fl.Seek (sizeof(ExtHeadData)+b1*sizeof(RecordIndicator) ,CFile::begin);
			fl.Write (pData,nCount*sizeof(RecordIndicator));
			if(n-e1>0)
				fl.Write (pDataRead+e1,(n-e1)*sizeof(RecordIndicator));
			head.m_nCount = b1+nCount+(n-e1);
			SetFileHead( head, fl);*/
			if(b1 == -1) b1 = 0;
			if(e1 == -1) e1 = 0;
			if(pKlineSmallHead->numKln-e1>0)
			{
				nAddOld = pKlineSmallHead->numKln-e1;
				pDataOld = pDataRead+e1;
			}
			e1 = b1+nAddOld+nCount;
		}
		blkCount= b1 / FixedKlinePerBlock  ;
		for(i=b1;i<e1;i++)
		{
			int stockCount=i % FixedKlinePerBlock  ;	  
			//Create Or Move Small Block
			if(stockCount == 0)
			{
				blkCount= i / FixedKlinePerBlock  ;	  
				nDeleted += CreateOrMoveSmallBlock(pKlineSmallHead, blkCount);
			}

			int addr = KLINESMALLHEAD + pKlineSmallHead->symBlock[blkCount] 
					* KlineByteEach *  FixedKlinePerBlock
						 + stockCount * KlineByteEach;
										  
			this->Seek(addr,this->begin);

			RecordIndicator * pKln = pData+i-b1;
			if(i>=b1+ nCount)//from file old
			{
				ASSERT(pDataOld);
				pKln = pDataOld+i-b1-nCount;
			}
			Write(pKln,KlineByteEach);

		}
		delete [] pDataRead;

	}

	//write the count of kline to header
	pKlineSmallHead->numKln = b1+ nCount + nAddOld;
	ASSERT(pKlineSmallHead->numKln>=0);

	SetKlineSmallHeader(nIndexStock,pKlineSmallHead);
	return nCount;
}
int CTechFileExtData::ReadData(RecordIndicator*& pData,CString symbol,int nIndicatorExtendID,int nDays)//nDays ==-1 is total
{
	if(!Open( nIndicatorExtendID,CTechMemFile::modeReadWrite)) return 0;

	int  n = 0 ;
	KLINE_SMALLHEAD klineSmallHead;
	KLINE_SMALLHEAD* pKlineSmallHead = &klineSmallHead;
	int nIndexStock = GetKlineSmallHeader(symbol,pKlineSmallHead,false);
	if(nIndexStock<0) return 0;

	n = nDays;
	if(n<=-1||n>pKlineSmallHead->numKln)
		n=pKlineSmallHead->numKln ;
	if(pData != NULL)
		delete [] pData;
	pData = new RecordIndicator[n+1];
	if(n==0)
		return 0;

	this->SeekToBegin ();
	CString ss = pKlineSmallHead->StockSign ;
	ASSERT(n<=0?TRUE:KLINESMALLHEAD + pKlineSmallHead->symBlock[(n-1) / FixedKlinePerBlock ] 
				* KlineByteEach *  FixedKlinePerBlock
					 + (FixedKlinePerBlock) * KlineByteEach < this->GetLength ());

	//read now
	for(int i=pKlineSmallHead->numKln-n;i<pKlineSmallHead->numKln ;i++)
	{
		int blkCount= i / FixedKlinePerBlock  ;	  
		int stockCount=i % FixedKlinePerBlock  ;	  
		int addr = KLINESMALLHEAD + pKlineSmallHead->symBlock[blkCount] 
				* KlineByteEach *  FixedKlinePerBlock
					 + stockCount * KlineByteEach;
									  //
		this->Seek(addr,this->begin);
		Read(pData+i-pKlineSmallHead->numKln+n,KlineByteEach);
	}

	return n;
}
int CTechFileExtData::GetKlineSmallHeader(CString symbol,KLINE_SMALLHEAD* pKlineSmallHead,bool bWrite)//get the small header pointer of a stock
{
	if(m_pSymbolToPos == NULL)
		AddIndexToMap();

	KLINE_SMALLHEAD* pKlineSmallHead3 = NULL;
	// Look up from cdate struct
	int i=-1;
	LookUpIndexFromMap( symbol,i);
	//end

	//so it maybe a new stock
	if(i==-1)
	{
		//add a new stock to file
		if(bWrite)
		{
			i = AddNewStockToFile(symbol,pKlineSmallHead3);
			if(i<0)
				ASSERT(FALSE);
		}
	}
	else
	{
		int addr = KLINEHEAD;
		this->Seek(addr+i*64,this->begin);
		pKlineSmallHead3 = (KLINE_SMALLHEAD*)this->GetFileCurrentPointer();
	}

	if(pKlineSmallHead3)
		memcpy(pKlineSmallHead,pKlineSmallHead3,sizeof(KLINE_SMALLHEAD));

	return i;
}
void CTechFileExtData::AddIndexToMap()//add the kline small header's index 
{
	int nStock = this->GetStockNumber();
	m_pSymbolToPos = new CMapStringToPtr((int)(nStock*1.25)+1);
	ASSERT(nStock<=MaxNumStock);
	for(int i=0;i<nStock;i++)
	{
		int addr = KLINEHEAD+i*64;
		this->Seek(addr,this->begin);
		KLINE_SMALLHEAD* pKlineSmallHead = (KLINE_SMALLHEAD*)this->GetFileCurrentPointer();
		CString symbol (pKlineSmallHead->StockSign);
		m_pSymbolToPos->SetAt(symbol,(CObject*)i);

	}
}

int CTechFileExtData::AddNewStockToFile(CString symbol,KLINE_SMALLHEAD *&pKlineSmallHead)//add new stock to kline file
{
	int nStock = this->GetStockNumber ();
	if(nStock>=MaxNumStock)//监测股票个数
	{
		bool bNeedMove = true;
		if(MaxNumStock >= 4096*3)
		{
			nStock = this->GetStockNumber ();
			ASSERT(FALSE);
		}
	}

	int addr = KLINEHEAD+nStock*64;
	this->Seek(addr,this->begin);
	if(!(symbol.GetLength ()==7))
	{
		ASSERT(FALSE);
		symbol = symbol.Left (7);
	}

	int rValue = 0;
	if(m_pSymbolToPos->Lookup (symbol,(void*&)rValue))
	{
		addr = KLINEHEAD+(int)(rValue)*64;
		this->Seek(addr,this->begin);
		pKlineSmallHead = (KLINE_SMALLHEAD*)this->GetFileCurrentPointer();
		ASSERT(FALSE);
		return rValue;
	}
	
	Write(symbol.GetBuffer (8),8);

	int nKline = 0;
	Write(&nKline,4);

	this->Seek(addr,this->begin);
	pKlineSmallHead = (KLINE_SMALLHEAD*)this->GetFileCurrentPointer();

	//
	m_pSymbolToPos->SetAt(symbol,(CObject*)nStock);
	nStock++;
	this->SetStockNumber (nStock);

	return nStock-1;

}
void CTechFileExtData::LookUpIndexFromMap(CString symbol,int& nIndex)//look up the kline small header's index from document data
{
	nIndex = -1;
	if(m_pSymbolToPos==NULL)
		AddIndexToMap();
	if(!m_pSymbolToPos->Lookup(symbol,(void*&)nIndex))
		nIndex = -1;
}



BOOL CTechFileExtData::Open(int nIndicatorExtendID, UINT nOpenFlags,int nAddToFileEnd  ,CFileException* pException )
{
	if(m_nIDPre==nIndicatorExtendID) return TRUE;
	CloseNow();

	IndicatorInfo data;
	bool b = FileExtendData::GetIndicatorInfo( data, nIndicatorExtendID);
	if(!b)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_bDay = true;
	if(data.m_nKlineKind == 1) m_bDay = false;
	CString s;
	s.Format ("%d.ext",nIndicatorExtendID);
	CString s2 = g_chIndicatorPath;
	s = s2+s;


	BOOL bOk = TRUE;
	bOk = CTechMemFile::Open( s.GetBuffer(0),  nOpenFlags,  0,
	 pException);
	if(bOk == FALSE) return bOk;

	//check if the file is newly created;
	DWORD dwLen = this->GetLength ();
	if(dwLen<=KLINESMALLHEAD || bOk == FALSE)
	{
		this->ReMapFromBegin (KLINESMALLHEAD+500* KlineByteEach *  FixedKlinePerBlock);

		//write header infomation 
		WriteHeaderInfo();
//		CreateSmallBlock();
	}
	else
	{
		//
		int bID = 1;
		if(GetID()!=65798809) bID =0;
		int nBlock = this->GetSmallBlockCount();
		if(this->GetStockNumber ()>GetMaxNumStock() || nBlock>GetMaxNumStock()*MAX_BLOCK_USE 
			|| this->GetStockNumber ()<0 
			|| nBlock<0
			|| bID ==0)
			WriteHeaderInfo();
		else
		{
//			ASSERT(FALSE);
		}
	}

//	MaxNumStock = GetMaxNumStock();
	m_nIDPre=nIndicatorExtendID;
	return TRUE;
}
void CTechFileExtData::WriteHeaderInfo()
{
	this->SeekToBegin();
	int nStock=0;
	Write(&nStock,4);
	Write(&nStock,4);//the count of block
	nStock=65798809;
	Write(&nStock,4);
	WORD wd = 240;
	Write(&wd,2);
	wd=MaxNumStock;
	Write(&wd,2);

	char* buff = new char[MaxNumStock];
	memset(buff,255,MaxNumStock);
	for(int i=0;i<64;i++)
		Write(buff,MaxNumStock);

	delete []buff;

	//write stock small header according the document;

}
bool CTechFileExtData::SetKlineSmallHeader(int nIndex, KLINE_SMALLHEAD *pKlineSmallHead)
{
	int nStock = this->GetStockNumber ();
	ASSERT(nIndex<nStock);
	if(nIndex>=nStock)
		return false;

	int addr = KLINEHEAD+nIndex*64;
	this->Seek(addr,this->begin);
	CString s(pKlineSmallHead->StockSign );
	KLINE_SMALLHEAD *pKlineSmallHead2 = (KLINE_SMALLHEAD *)this->GetFileCurrentPointer ();
	CString s2(pKlineSmallHead2->StockSign );
	if(s!=s2)
	{
		ASSERT(FALSE);
	}
	this->Write (pKlineSmallHead,sizeof(KLINE_SMALLHEAD));
	return true;
}
void CTechFileExtData::CloseNow()
{
	g_fileExtData.Close();
	g_fileExtData.m_nIDPre = -1;
	g_fileExtData.DeleteMap();
}