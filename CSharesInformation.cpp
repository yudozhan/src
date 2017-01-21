// CSharesInformation.cpp: implementation of the CSharesInformation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <share.h>
#include  <io.h>
#include "StructTaiShares.h"
#include "mainfrm.h"
#include "CTaiShanDoc.h"
#include "CSharesInformation.h"
#include "WH.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSharesInformation::CSharesInformation()
{
	for(int i=0;i<STOCKTYPENUM;i++)
	{
		m_pData[i]=NULL;
		m_dwStockMaxCount[i] = 0;
	}

    m_hFile=NULL;
	m_hFileMap=NULL;
	m_pbData=NULL;
	m_pMapData=NULL;
	m_RealFileHead=NULL;
	m_pdwStockCurrentCount=NULL;

	Nidx[0]=NULL;	//用于储存大盘个股涨跌数
	Nidx[1]=NULL;	//用于储存大盘个股涨跌数
	Nidx[2]=NULL;	//用于储存大盘个股涨跌数
    Tidx[0]=NULL;	//用于储存大盘强弱指标
    Tidx[1]=NULL;	//用于储存大盘强弱指标
    Tidx[2]=NULL;	//用于储存大盘强弱指标

	ClearUpDown(0);
	ClearUpDown(1);

}
CSharesInformation::~CSharesInformation()
{

   BOOL result=SavePosToFile();
   RemoveStockInfo();
   if(result)
     m_RealFileHead->FileExitDone=12345678;  
   else
     m_RealFileHead->FileExitDone=88888888;  
   SaveRealDataToFile(m_RealFileHead,0);
   for(int i=0;i<STOCKTYPENUM;i++)
   {
	   if(m_pData[i])
	   {
		   GlobalUnlock((HGLOBAL)m_pData[i]);        //释放内存
		   GlobalFree( (HGLOBAL)m_pData[i]);
	   }
   }

   if(m_pbData)
	 UnmapViewOfFile(m_pbData);
   if(m_hFileMap) 
	 CloseHandle(m_hFileMap);
   if(m_hFile)
	 CloseHandle(m_hFile);

}
BOOL CSharesInformation::SetMemoryAllocSize(DWORD StockType,unsigned int nSize)
{
	STOCKDATASHOW *tempData;
	int temp=0;
	HGLOBAL	hMem;
	LPVOID hp;
	if(!m_pData[StockType])
	{
		hMem = GlobalAlloc( GPTR, (nSize + ADDCOUNT )* sizeof( STOCKDATASHOW ) );
		hp=GlobalLock(hMem);
		if( hp )
		{
			m_pData[StockType]= (STOCKDATASHOW *)hp;
		}
		else
		{
			AfxMessageBox("分配内存出错",MB_ICONSTOP);
			return FALSE;
		}
	    m_dwStockMaxCount[StockType] = nSize + ADDCOUNT;              //每类型保存最大股票数
	    m_pdwStockCurrentCount[StockType] = 0;  //每类型保存现有股票数 
	}
	else
	{
		if(m_dwStockMaxCount[StockType] <= nSize)
		{
			hMem = GlobalAlloc( GPTR, (nSize + ADDCOUNT ) * sizeof( STOCKDATASHOW ) );
			hp = GlobalLock(hMem);
			if( hp )
			{
				tempData = (STOCKDATASHOW *)hp;
			}
			else
			{
				AfxMessageBox("分配内存出错",MB_ICONSTOP);
				return FALSE;
			}
            memcpy(tempData,m_pData[StockType],sizeof(STOCKDATASHOW)*m_pdwStockCurrentCount[StockType]);
		    GlobalUnlock((HGLOBAL)m_pData[StockType]);        //释放内存
		    GlobalFree( (HGLOBAL)m_pData[StockType]);
		    m_pData[StockType]=tempData;
	        m_dwStockMaxCount[StockType] =nSize + ADDCOUNT;              //每类型保存最大股票数
	   }
	}
	return TRUE;
}

BOOL  CSharesInformation::RecvStockDataForType(PSTOCKDATASHOW &p,UINT StockType) //将一类股票转换为可显示股票
{
	if(m_hFile==NULL||m_hFileMap==NULL || StockType < 0/* || StockType >= STOCKTYPENUM*/)
		return FALSE;

	if(StockType >= SHZS && StockType < STOCKTYPENUM)
	{
		memcpy(p,m_pData[StockType],m_pdwStockCurrentCount[StockType] * sizeof(STOCKDATASHOW));
	}
	else if (StockType > STOCKTYPENUM)
	{
		int nTypeSh = StockType/100 - 1;
		int nTypeSz = StockType%100;

		if(nTypeSh < SHZS || nTypeSh > STOCKTYPENUM || nTypeSz < SHZS || nTypeSz > STOCKTYPENUM)
			return FALSE;

		memcpy(p,m_pData[nTypeSh],m_pdwStockCurrentCount[nTypeSh] * sizeof(STOCKDATASHOW));
		memcpy(p + m_pdwStockCurrentCount[nTypeSh],m_pData[nTypeSz],m_pdwStockCurrentCount[nTypeSz] * sizeof(STOCKDATASHOW));
	}

	return TRUE;
}

BOOL  CSharesInformation::RecvAllStockData(PSTOCKDATASHOW &p) 
{
    LOGS("CSharesInformation::RecvAllStockData()\r\n");
    PSTOCKDATASHOW pShowBuf = p;

	if(m_hFile==NULL||m_hFileMap==NULL /* || StockType >= STOCKTYPENUM*/)
		return FALSE;

    // SHAG
    memcpy(pShowBuf,m_pData[SHAG],m_pdwStockCurrentCount[SHAG] * sizeof(STOCKDATASHOW));
    pShowBuf += m_pdwStockCurrentCount[SHAG];

    // SZAG
    memcpy(pShowBuf,m_pData[SZAG],m_pdwStockCurrentCount[SZAG] * sizeof(STOCKDATASHOW));
    pShowBuf += m_pdwStockCurrentCount[SZAG];

    // SZZXQY
    memcpy(pShowBuf,m_pData[SZZXQY],m_pdwStockCurrentCount[SZZXQY] * sizeof(STOCKDATASHOW));
    pShowBuf += m_pdwStockCurrentCount[SZZXQY];

    // SZCYB
    memcpy(pShowBuf,m_pData[SZCYB],m_pdwStockCurrentCount[SZCYB] * sizeof(STOCKDATASHOW));

	return TRUE;
}

BOOL CSharesInformation::Lookup(char *StockId,PCdat1 &pStockData,int nKind)  //按类找查股票
{
	if(nKind >= STOCKTYPENUM || nKind < 0)
		return FALSE;

	int low=0;
	int high=m_pdwStockCurrentCount[nKind] -1;
	int mid=0;
    while(low <= high)
	{
		 mid=(low+high)/2;
		 if(strcmp(m_pData[nKind][mid].StockId , StockId)>0) 
			 high=mid -1;
         else if(strcmp(m_pData[nKind][mid].StockId , StockId)< 0 )
			 low=mid +1;
		 else 
		 {
			 pStockData=m_pData[nKind][mid].pItem ;
			 return TRUE ;
		 }
	}
    pStockData=NULL;
	
	return FALSE;
}
void CSharesInformation::SavePosToFile(int StockType)
{
	 for(int j = 0;j<m_pdwStockCurrentCount[StockType];j++)
	 {
		  if(m_pData[StockType][j].pItem == NULL)
			  continue;
          m_pData[StockType][j].pItem->sel = j; 
	 }
}
BOOL CSharesInformation::SavePosToFile()
{
     for(int i=0;i<STOCKTYPENUM;i++)
	 {
		 for(int j=0;j<m_pdwStockCurrentCount[i];j++)
		 {
			  if(m_pData[i][j].pItem==NULL)
			  {
				  return FALSE;
			  }
              m_pData[i][j].pItem->sel=j; 
		 }
	 }
	  return TRUE;
}
BOOL  CSharesInformation::InsertItem(char *StockId ,PCdat1 &pStockData, DWORD StockType) //按类找查股票
{
	int low=0;
	int high=m_pdwStockCurrentCount[StockType] -1 ;
	int mid=0;
	int InsertPose=-1;
    while(low <= high)
	{
		 mid=(low+high)/2;
	     if(low==high)
		    if(strcmp(StockId , m_pData[StockType][mid].StockId )>0)
			{
               if(m_pdwStockCurrentCount[StockType]==0||m_pdwStockCurrentCount[StockType]==mid)
                  InsertPose=mid ;
			   else
                  InsertPose=mid +1;   
			   break;
			}
		    else if(strcmp(StockId , m_pData[StockType][mid].StockId)<0)
			{
			   InsertPose=mid ;
			   break;
			}
			else
			{
				 pStockData=m_pData[StockType][mid].pItem ;
				 return TRUE ;
			}
 		 if(strcmp(m_pData[StockType][mid].StockId , StockId)>0) high=mid -1;
         else if(strcmp(m_pData[StockType][mid].StockId , StockId)< 0 ) low=mid +1;
		 else 
		 {
             pStockData=m_pData[StockType][mid].pItem ;
			 return TRUE ;
		 }
	}
	if(high<low)
		InsertPose=low;

	if(m_RealFileHead->StockCount+1>m_RealFileHead->MaxStockCount)
	{
		return FALSE;
	}


	if(m_pdwStockCurrentCount[StockType] + 1 > m_dwStockMaxCount[StockType])  //检验现有股票数是否大于最大保存数
	{
        SetMemoryAllocSize(StockType,m_pdwStockCurrentCount[StockType]);
	}
	if(m_pdwStockCurrentCount[StockType] > InsertPose)
	{
       STOCKDATASHOW *ptemp;
	   HGLOBAL	hMem;
	   LPVOID hp;
	   hMem = GlobalAlloc( GPTR, (m_pdwStockCurrentCount[StockType] - InsertPose )* sizeof( STOCKDATASHOW) );
	   hp=GlobalLock(hMem);
	   if(hp)
	   {
			ptemp= (STOCKDATASHOW *)hp;
	   }
	   else
	   {
			AfxMessageBox("分配内存出错",MB_ICONSTOP);
			return FALSE;
	   }
	   memcpy(ptemp,&m_pData[StockType][InsertPose],(m_pdwStockCurrentCount[StockType] - InsertPose)*sizeof(STOCKDATASHOW));
	   memcpy(&m_pData[StockType][InsertPose+1],ptemp,(m_pdwStockCurrentCount[StockType] - InsertPose)*sizeof(STOCKDATASHOW));
       GlobalUnlock((HGLOBAL)ptemp);        //释放内存
	   GlobalFree( (HGLOBAL)ptemp);
    }
    m_pData[StockType][InsertPose].pItem=m_pMapData+m_RealFileHead->StockCount;
	strcpy(m_pData[StockType][InsertPose].StockId,StockId);
	m_RealFileHead->StockCount++;	
    
	memset(m_pData[StockType][InsertPose].pItem  ,0,sizeof(CReportData));

	m_pData[StockType][InsertPose].pItem->pBaseInfo =NULL;
	m_pData[StockType][InsertPose].pItem->pBlockInfo =NULL;
	
	m_pData[StockType][InsertPose].pItem->IsDelete=FALSE;

    strcpy(m_pData[StockType][InsertPose].pItem->id  ,StockId);
    m_pData[StockType][InsertPose].pItem->kind=StockType;
	
    pStockData=m_pData[StockType][InsertPose].pItem ;
	m_pdwStockCurrentCount[StockType]++;
//    SavePosToFile(StockType);
    SaveRealDataToFile(m_RealFileHead,sizeof(REALDATA)+sizeof(long)*STOCKTYPENUM); 
    SaveRealDataToFile(m_pData[StockType][InsertPose].pItem,sizeof(CReportData)); 

	return TRUE;
}
BOOL  CSharesInformation::RemoveKey(char *StockId ,DWORD StockType) //按类找查股票
{
	int low=0;
	int high=m_pdwStockCurrentCount[StockType] -1 ;
	int mid=0;
	int DeletePos=-1;
    while(low <= high)
	{
		 mid=(low+high)/2;
		 if(strcmp(m_pData[StockType][mid].StockId , StockId)>0) high=mid -1;
         else if(strcmp(m_pData[StockType][mid].StockId  , StockId)< 0 ) low=mid +1;
		 else 
		 {
			 DeletePos=mid;
			 break;
		 }
	}
    if(DeletePos==-1)
	{
		return FALSE;
	}
	m_pData[StockType][DeletePos].pItem->IsDelete=TRUE;
	memmove(&m_pData[StockType][DeletePos],&m_pData[StockType][DeletePos +1],(m_pdwStockCurrentCount[StockType] - DeletePos -1 )*sizeof(STOCKDATASHOW));
    m_pdwStockCurrentCount[StockType]--;
	SavePosToFile(StockType);
    SaveRealDataToFile(m_RealFileHead,0); 
	return TRUE;
}
BOOL CSharesInformation::InitRealTimeDataExist()
{
     BYTE *temp;
	 BOOL IsCorrect=FALSE;

	 m_hFile=CreateFile(g_realtime,GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("打开实时行情数据出错");
		return FALSE; 
	}
	m_hFileMap=CreateFileMapping(m_hFile,
		NULL,
		PAGE_READWRITE,
		0,
		0,
		NULL);
	if(m_hFileMap==NULL)
	{
		AfxMessageBox("创立文件映射内核时出错");
		CloseHandle(m_hFile);
		m_hFile=NULL;
		m_hFileMap=NULL;
		return FALSE;
	}
	m_pbData=(PBYTE)MapViewOfFile(m_hFileMap,
		FILE_MAP_WRITE,
		0,0,0);
	if(m_pbData==NULL)
	{
		AfxMessageBox("将文件数据映射入内存时出错");
		CloseHandle(m_hFile);
		CloseHandle(m_hFileMap);
		m_hFile=NULL;
		m_hFileMap=NULL;
		return FALSE;
	}
 	m_RealFileHead=(REALDATA *)m_pbData;
	if(m_RealFileHead->filetitle!=12345678)
	{
	   UnmapViewOfFile(m_pbData);
	   CloseHandle(m_hFile);
	   CloseHandle(m_hFileMap);
       return InitRealTimeDataEmpty();
	}
	if(m_RealFileHead->StockCount+100 >m_RealFileHead->MaxStockCount)
	{
		 int StockTotalCount;
		 long FileLength=0;
         m_RealFileHead->MaxStockCount+=100;
         StockTotalCount=m_RealFileHead->MaxStockCount;
         SaveRealDataToFile(m_RealFileHead,sizeof(REALDATA)); 
	     UnmapViewOfFile(m_pbData);
	     CloseHandle(m_hFileMap);
         m_hFileMap=NULL;
         m_pbData=NULL;
         FileLength=sizeof(REALDATA)+STOCKTYPENUM*4+240*4*3+240*8*3+sizeof(CReportData)*StockTotalCount;
		m_hFileMap=CreateFileMapping(m_hFile,
			NULL,
			PAGE_READWRITE,
		    0,
			FileLength,
			NULL);
		if(m_hFileMap==NULL)
		{
			AfxMessageBox("创立文件映射内核时出错");
			CloseHandle(m_hFile);
			m_hFile=NULL;
			m_hFileMap=NULL;
			return FALSE;
		}
		m_pbData=(PBYTE)MapViewOfFile(m_hFileMap,
			FILE_MAP_WRITE,
			0,0,0);
		if(m_pbData==NULL)
		{
			AfxMessageBox("将文件数据映射入内存时出错");
			CloseHandle(m_hFile);
			CloseHandle(m_hFileMap);
			m_hFile=NULL;
			m_hFileMap=NULL;
			return FALSE;
		}
 		m_RealFileHead=(REALDATA *)m_pbData;
	}
	if(m_RealFileHead->FileExitDone!=12345678)
	{
		if(m_RealFileHead->FileExitDone!=88888888)
		{
			int rtn = MessageBox(NULL,"程序没有正常退出,是否清除实时行情数据文件?","警告",MB_YESNO|MB_ICONWARNING|MB_DEFBUTTON2);
			if(rtn == IDYES)
			{ 
			   UnmapViewOfFile(m_pbData);
			   CloseHandle(m_hFile);
			   CloseHandle(m_hFileMap);
			   return InitRealTimeDataEmpty();
			}
		}
	    IsCorrect=TRUE;
	}
    m_RealFileHead->FileExitDone=87654321;
    temp=m_pbData+sizeof(REALDATA);
	m_pdwStockCurrentCount=(DWORD *)temp;
	temp += (sizeof(DWORD))*(STOCKTYPENUM);

	Nidx[0]=(Rsdn1 *)temp;		               //用于储存大盘个股涨跌数
	temp +=sizeof(Rsdn1)*240;
	Nidx[1]=(Rsdn1 *)temp;		               //用于储存大盘个股涨跌数
 	temp +=sizeof(Rsdn1)*240;
	Nidx[2]=(Rsdn1 *)temp;		               //用于储存大盘个股涨跌数
 	temp +=sizeof(Rsdn1)*240;

    Tidx[0]=(Tidxd *)temp;                         //用于储存大盘强弱指标
 	temp +=sizeof(Tidxd)*240;
    Tidx[1]=(Tidxd *)temp;                         //用于储存大盘强弱指标
 	temp +=sizeof(Tidxd)*240;
    Tidx[2]=(Tidxd *)temp;                         //用于储存大盘强弱指标
 	temp +=sizeof(Tidxd)*240;

	int j;
	m_pMapData=(CReportData *)temp;		//股票行情数据结构
    for(j=0;j<STOCKTYPENUM;j++)
	{
         if(!SetMemoryAllocSize(j,m_pdwStockCurrentCount[j]))
		{
			AfxMessageBox("初始化数据变量出错");
			return FALSE;
		} 
	}
	if(!IsCorrect)
	{
		for(j=0;j<m_RealFileHead->StockCount;j++)
		{
//			TRACE( (m_pMapData+j)->id );
//			TRACE( "\n" );
			m_pMapData[j].pBaseInfo =NULL; 
			m_pMapData[j].pBlockInfo=NULL;
			this->InsertItemPoint(m_pMapData+j);
		}
	}
	else
	{
		int temp=m_RealFileHead->StockCount;
		m_RealFileHead->StockCount=0;
		for(j=0;j<temp;j++)
		{
			//if(strlen((m_pMapData+j)->id)==6||strlen((m_pMapData+j)->id)==4)
			{
                int stocktype=(m_pMapData+j)->kind;
				if(stocktype >= SZZS && stocktype <= CHOOSESTK && (m_pMapData+j)->IsDelete==FALSE)
					InsertItemCorrect((m_pMapData+j)->id,m_pMapData+j,(m_pMapData+j)->kind);
				else
					(m_pMapData+j)->IsDelete=TRUE;
			}
			//else           
			//	(m_pMapData+j)->IsDelete=TRUE;
		} 
	}
    SaveRealDataToFile(m_RealFileHead,sizeof(REALDATA)); 
	return TRUE;
}
BOOL CSharesInformation::InitRealTimeDataEmpty()
{
	BYTE *temp;
//	#define MAXSTOCKDATABUFFER 40+STOCKTYPENUM*4+240*4*3+240*8*3+2000*3100
	int iii = sizeof(Rsdn1);
	int iiii = sizeof(Tidxd);
    long 
    m_FileLength=sizeof(REALDATA)+STOCKTYPENUM*sizeof(int)+240*3*sizeof(Rsdn1)+240*3*sizeof(Tidxd)+sizeof(CReportData)*8000;
	m_hFile=CreateFile(g_realtime,GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("打开实时行情数据出错");
		return FALSE; 
	}
	m_hFileMap=CreateFileMapping(m_hFile,
		NULL,
		PAGE_READWRITE,
		0,
		m_FileLength,
		NULL);
	if(m_hFileMap==NULL)
	{
		AfxMessageBox("创立文件映射内核时出错");
		CloseHandle(m_hFile);
		m_hFile=NULL;
		m_hFileMap=NULL;
		return FALSE;
	}
	m_pbData=(PBYTE)MapViewOfFile(m_hFileMap,
		FILE_MAP_WRITE,
		0,0,0);
	if(m_pbData==NULL)
	{
		AfxMessageBox("将文件数据映射入内存时出错");
		CloseHandle(m_hFile);
		CloseHandle(m_hFileMap);
		m_hFile=NULL;
		m_hFileMap=NULL;
		return FALSE;
	}
    CTime m_Time = CTime::GetCurrentTime();
    long currDay=((long)m_Time.GetYear())*10000L+(long)(m_Time.GetDay())+(long)(m_Time.GetMonth())*100;

	m_RealFileHead=(REALDATA *)m_pbData;
    m_RealFileHead->filetitle =12345678;
	m_RealFileHead->MaxStockCount=8000; 
	m_RealFileHead->StockCount =0;
	m_RealFileHead->FileExitDone=87654321;
	m_RealFileHead->Day=currDay;
    m_RealFileHead->CloseWorkDone=FALSE;
	m_RealFileHead->OldANT[0]=0;
	m_RealFileHead->OldANT[1]=0;
	m_RealFileHead->OldANT[2]=0;

    temp=m_pbData+sizeof(REALDATA);
	m_pdwStockCurrentCount=(DWORD *)temp;
	temp += (sizeof(DWORD))*(STOCKTYPENUM);
	int j;
	for(j=0;j<STOCKTYPENUM;j++)
        m_pdwStockCurrentCount[j]=0;

	Nidx[0]=(Rsdn1 *)temp;		               //用于储存大盘个股涨跌数
	memset(Nidx[0],0,240*sizeof(Rsdn1));
	temp +=sizeof(Rsdn1)*240;

	Nidx[1]=(Rsdn1 *)temp;		               //用于储存大盘个股涨跌数
	memset(Nidx[1],0,240*sizeof(Rsdn1));
 	temp +=sizeof(Rsdn1)*240;
	
	Nidx[2]=(Rsdn1 *)temp;		               //用于储存大盘个股涨跌数
	memset(Nidx[1],0,240*sizeof(Rsdn1));
 	temp +=sizeof(Rsdn1)*240;

    Tidx[0]=(Tidxd *)temp;                         //用于储存大盘强弱指标
 	temp +=sizeof(Tidxd)*240;
	memset(Tidx[0],0,240*sizeof(Tidxd));

    Tidx[1]=(Tidxd *)temp;                         //用于储存大盘强弱指标
 	temp +=sizeof(Tidxd)*240;
	memset(Tidx[1],0,240*sizeof(Tidxd));

    Tidx[2]=(Tidxd *)temp;                         //用于储存大盘强弱指标
 	temp +=sizeof(Tidxd)*240;
	memset(Tidx[2],0,240*sizeof(Tidxd));

    for (j=0; j<240; j++)
	{
       Tidx[0][j].sec5=Tidx[1][j].sec5=Tidx[2][j].sec5=9911;
	}

	m_pMapData=(CReportData *)temp;		//股票行情数据结构
	//memset(m_pMapData,0,sizeof(CReportData)*4000);

    for(j=0;j<STOCKTYPENUM;j++)
	{
        if(!SetMemoryAllocSize(j,5))
		{
			AfxMessageBox("初始化数据变量出错");
			return FALSE;
		} 
	}

	InitEmptyDatabase();
    SaveRealDataToFile(m_RealFileHead,sizeof(REALDATA)); 

	return TRUE;
}
BOOL CSharesInformation::InitRealTimeData(CString path)
{
	BOOL bExist = FALSE;
    strcpy(m_sPath,path.GetBuffer(0)); 
	this->m_StockBaseInfo.InitBaseInfoData(path,bExist);  
	if(_access(g_realtime,0)==-1)   
	{
       InitRealTimeDataEmpty();
	}
    else
	{
       InitRealTimeDataExist();
	   ReadAllBaseInfo();//
	}

	if (!bExist)
	{
		ImportCwData(g_caiwudata);
	}

	return TRUE;
}
BOOL  CSharesInformation::InsertItemPoint(CReportData *pStockData )               //插入一个股票
{     
	int StockType=pStockData->kind; 
	int InsertPose=pStockData->sel; 
	if(m_pdwStockCurrentCount[StockType] + 1 > m_dwStockMaxCount[StockType])  //检验现有股票数是否大于最大保存数
	{
        SetMemoryAllocSize(StockType,m_pdwStockCurrentCount[StockType]);
	}
    m_pData[StockType][InsertPose].pItem=pStockData;
	strcpy(m_pData[StockType][InsertPose].StockId,pStockData->id);
    m_pdwStockCurrentCount[StockType]++;
    SaveRealDataToFile(m_RealFileHead,sizeof(REALDATA)+sizeof(long)*STOCKTYPENUM); 
    SaveRealDataToFile(m_pData[StockType][InsertPose].pItem,sizeof(CReportData)); 
	return TRUE; 
}
void CSharesInformation::CalcIndexBuyAndSell()
{
	if(m_pdwStockCurrentCount[0]<9)
		return;
	m_pData[0][0].pItem->accb=0;     //上证
	m_pData[0][1].pItem->accb=0;     //A股     
	m_pData[0][2].pItem->accb=0;     //B股
	m_pData[0][0].pItem->accs=0;     //上证
	m_pData[0][1].pItem->accs=0;     //A股
	m_pData[0][2].pItem->accs=0;     //B股
	
	int i;

	for(i=0;i<m_pdwStockCurrentCount[1];i++)
	{
		if(m_pData[1][i].pItem==NULL)
			continue;
        m_pData[0][0].pItem->accb +=m_pData[1][i].pItem->accb;
        m_pData[0][0].pItem->accs +=m_pData[1][i].pItem->accs;

        m_pData[0][1].pItem->accb +=m_pData[1][i].pItem->accb;
        m_pData[0][1].pItem->accs +=m_pData[1][i].pItem->accs;
		
	}
	for(i=0;i<m_pdwStockCurrentCount[2];i++)
	{
		if(m_pData[2][i].pItem==NULL)
			continue;
        m_pData[0][0].pItem->accb +=m_pData[2][i].pItem->accb;
        m_pData[0][0].pItem->accs +=m_pData[2][i].pItem->accs;

        m_pData[0][2].pItem->accb +=m_pData[2][i].pItem->accb;
        m_pData[0][2].pItem->accs +=m_pData[2][i].pItem->accs;
	}
	if(m_pdwStockCurrentCount[11]<9)
		return;
	m_pData[11][0].pItem->accb=0;     //深证
	m_pData[11][1].pItem->accb=0;     //A股     
	m_pData[11][2].pItem->accb=0;     //B股
	m_pData[11][0].pItem->accs=0;     //深证
	m_pData[11][1].pItem->accs=0;     //A股
	m_pData[11][2].pItem->accs=0;     //B股
	for(i=0;i<m_pdwStockCurrentCount[12];i++)
	{
		if(m_pData[12][i].pItem==NULL)
			continue;
        m_pData[11][0].pItem->accb +=m_pData[12][i].pItem->accb;
        m_pData[11][0].pItem->accs +=m_pData[12][i].pItem->accs;

        m_pData[11][1].pItem->accb +=m_pData[12][i].pItem->accb;
        m_pData[11][1].pItem->accs +=m_pData[12][i].pItem->accs;
	}
	for(i=0;i<m_pdwStockCurrentCount[13];i++)
	{
		if(m_pData[13][i].pItem==NULL)
			continue;
        m_pData[11][0].pItem->accb +=m_pData[13][i].pItem->accb;
        m_pData[11][0].pItem->accs +=m_pData[13][i].pItem->accs;

        m_pData[11][2].pItem->accb +=m_pData[13][i].pItem->accb;
        m_pData[11][2].pItem->accs +=m_pData[13][i].pItem->accs;
	}
	if(m_pdwStockCurrentCount[3]>=1)//不知是否是基金
	{
		m_pData[3][0].pItem->accb=0;     //深证
		m_pData[3][0].pItem->accs=0;     //深证
		for(i=0;i<m_pdwStockCurrentCount[14];i++)
		{
			if(m_pData[14][i].pItem==NULL)
				continue;
			m_pData[3][0].pItem->accb +=m_pData[14][i].pItem->accb;
			m_pData[3][0].pItem->accs +=m_pData[14][i].pItem->accs;
		}
	}
}
BOOL CSharesInformation::ClearRealDataMinute()
{
	int i,j;
   for(j=0;j<STOCKTYPENUM;j++)
	{
		for(i=0;i<m_pdwStockCurrentCount[j];i++)
		{
			if(m_pData[j][i].pItem==NULL)
				continue;
			for(int k=0;k<240;k++)
				m_pData[j][i].pItem->rdp[k]='9';
		    m_pData[j][i].pItem->lastclmin =0;
		    m_pData[j][i].pItem->initdown =FALSE;
            m_pData[j][i].pItem->volume5=0;
            memset(&m_pData[j][i].pItem->ystc,0,8*sizeof(float));
            memset(&m_pData[j][i].pItem->rvol,0,2*sizeof(float));
			memset(&m_pData[j][i].pItem->accb,0,2*sizeof(float));
			memset(&m_pData[j][i].pItem->pbuy1,0,12*sizeof(float));
			memset(m_pData[j][i].pItem->m_Kdata1,0,sizeof(Kdata1)*240);
		}
	}
	memset(Nidx[0],0,240*sizeof(Rsdn1));
	memset(Nidx[1],0,240*sizeof(Rsdn1));
	memset(Nidx[2],0,240*sizeof(Rsdn1));
	for(j=0;j<240;j++)
	{
       Tidx[0][j].sec5=Tidx[1][j].sec5=Tidx[2][j].sec5=9911;
	}
	return TRUE;
}
void CSharesInformation::InitBaseInfoData()
{
	int BaseCount=m_StockBaseInfo.GetStockCount(); 
	if(BaseCount<0)
		return;
	for(int i=0;i<BaseCount;i++)
	{
		BASEINFO *pBase;
		CReportData *Cdat;
	    CTaiShanDoc	*m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_pDoc ;
        m_StockBaseInfo.GetStockItem(i,pBase);
		CString m_strZqdmKind=pBase->Symbol;
		CString m_strZqdm=m_strZqdmKind.Right(m_strZqdmKind.GetLength()-2);

        int nKind=m_pDoc->GetStockKind(m_strZqdmKind.Left(2));
		if(nKind<0)
			continue;
		if(this->Lookup(m_strZqdm.GetBuffer(0),Cdat,nKind))
		{
           Cdat->pBaseInfo=pBase;  
		}
	}
}
void CSharesInformation::ReadBaseInfoData(PCdat1 &pStockData)       //读入股票基本资料
{
     if(pStockData == NULL)
	 {
		 return;
	 }

	 if(pStockData->pBaseInfo == NULL)
	 {
	    BASEINFO *pBaseItem;
        if(!m_StockBaseInfo.AddStockItem(pStockData->id,pStockData->kind,pBaseItem))              //在基本资料数据文件中增加一块数据区域
		{
			AfxMessageBox("增加股票基本资料空间区域时出错！");
			return;
		}
        pBaseItem->NumSplit=0;
		pStockData->pBaseInfo=pBaseItem;
	 }
	 m_StockBaseInfo.ReadBaseInfoData(pStockData->id,pStockData->kind,pStockData->pBaseInfo); //读入数据
     m_StockBaseInfo.SaveBaseInfoToFile(pStockData->pBaseInfo,sizeof(BASEINFO));
}
void CSharesInformation::ReadAllBaseInfo()
{
     for(int i=0;i<STOCKTYPENUM;i++)
	 {
		 if(i==SHZS||i==SZZS||i==BLOCKINDEX)
			 continue;
		 for(int j=0;j<m_pdwStockCurrentCount[i];j++)
		 {
			 ReadBaseInfoData(this->m_pData[i][j].pItem);
		 }
	 }
}

BOOL CSharesInformation::AddOneStockInfo(CString strStockCode,CString strStockName,
		            CString strStockPyjc,int nKind)
{
	CReportData *pCdat;
     if(InsertItem(strStockCode.GetBuffer(0) ,pCdat,nKind ))
	 {
         strcpy(pCdat->name,strStockName.GetBuffer (0));
		 strcpy(pCdat->Gppyjc, strStockPyjc);
		 return TRUE;
	 }
	 return FALSE;
}
BOOL CSharesInformation::MOdifyOneStockInfo(CString strStockCode,CString strStockName,
		            CString strStockPyjc,int nKind)
{
	CReportData *pCdat;
	if(this->Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
         strcpy(pCdat->name,strStockName.GetBuffer (0));
		 strcpy(pCdat->Gppyjc, strStockPyjc);
		 return TRUE;
	}

	return FALSE;
}
BOOL CSharesInformation::DeleteOneStockInfo(CString strStockCode,int nKind)
{
    return RemoveKey(strStockCode.GetBuffer(0),nKind );   	
}

int CSharesInformation::GetChuQuanInfo(CString strStockCode,int nKind,PSplit &pSplit)
{
	CReportData *pCdat;
	if(Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
		if(pCdat->pBaseInfo!=NULL)
		{
			pSplit=pCdat->pBaseInfo->m_Split;  
			return pCdat->pBaseInfo->NumSplit; 
		}
	}
	pSplit=NULL;

	return -1;
}
BOOL CSharesInformation::AddChuQuanInfo(CString strStockCode,int nKind,Split *pSplit)
{
	CReportData *pCdat;
	if(Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
		 if(pCdat->pBaseInfo==NULL)
		 {
			BASEINFO *pBaseItem;
			if(!m_StockBaseInfo.AddStockItem(pCdat->id,pCdat->kind,pBaseItem))              //在基本资料数据文件中增加一块数据区域
			{
				AfxMessageBox("增加股票基本资料空间区域时出错！");
				return FALSE;
			}
			pBaseItem->NumSplit=0;
			pCdat->pBaseInfo=pBaseItem;
		 }

         memcpy(pCdat->pBaseInfo->m_Split+pCdat->pBaseInfo->NumSplit,
		  pSplit,sizeof(Split)); 
		 pCdat->pBaseInfo->NumSplit++ ;  
		 return TRUE; 
	}

	return FALSE;
}
BOOL CSharesInformation::ModifyChuQuanInfo(CString strStockCode,int nWhichItem,Split *pSplit,int nKind)
{
	CReportData *pCdat;
	if(Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
		if(pCdat->pBaseInfo!=NULL)
		{
		    if(nWhichItem>=pCdat->pBaseInfo->NumSplit)
			{
			   return FALSE;
			}
            memcpy(pCdat->pBaseInfo->m_Split+nWhichItem,pSplit,sizeof(Split)); 
			return TRUE; 
		}
	}
	return FALSE;
}
BOOL CSharesInformation::DeleteChuQuanInfo(CString strStockCode,int nWhichItem,int nKind)
{
	CReportData *pCdat;

	if(Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
		if(pCdat->pBaseInfo!=NULL)
		{
   			if(nWhichItem>=pCdat->pBaseInfo->NumSplit)
			{
				return FALSE;
			}
			 memmove(pCdat->pBaseInfo->m_Split+nWhichItem,
			  pCdat->pBaseInfo->m_Split+nWhichItem+1 ,
			  (pCdat->pBaseInfo->NumSplit - nWhichItem -1 )*sizeof(Split)); 
			pCdat->pBaseInfo->NumSplit-- ;  
			return TRUE; 
		}
	}
	return FALSE;
}
BOOL CSharesInformation::ImportChuQuanInfo(CString strStockCode,Split *pSplit,
		int nChuquanTotalTimes,int nKind)
{
	CReportData *pCdat;
	if(Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
		 if(pCdat->pBaseInfo==NULL)
		 {
			BASEINFO *pBaseItem;
			if(!m_StockBaseInfo.AddStockItem(pCdat->id,nKind,pBaseItem))              //在基本资料数据文件中增加一块数据区域
			{
				AfxMessageBox("增加股票基本资料空间区域时出错！");
				return FALSE;
			}
			pBaseItem->NumSplit=0;
			pCdat->pBaseInfo=pBaseItem;
		 }

         memcpy(pCdat->pBaseInfo->m_Split,
		  pSplit,sizeof(Split)*nChuquanTotalTimes); 
		 pCdat->pBaseInfo->NumSplit =nChuquanTotalTimes;  
	     m_StockBaseInfo.SaveBaseInfoToFile(pCdat->pBaseInfo,sizeof(BASEINFO)+sizeof(Split)*80);
		 return TRUE; 
	}
	return FALSE;

}
BOOL CSharesInformation::ExportChuQuanInfo(CString strStockCode,PSplit &pSplit,
		int& nChuquanTotalTimes,int nKind)
{
	CReportData *pCdat;
	if(Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
	    if(pCdat->pBaseInfo!=NULL)
		{
            pSplit=pCdat->pBaseInfo->m_Split;
   			nChuquanTotalTimes=pCdat->pBaseInfo->NumSplit;  
			return TRUE; 
		}
	}
	return FALSE;
}
BOOL CSharesInformation::RemoveStockInfo()
{
	int index=m_RealFileHead->StockCount;
	int nDelete=0;
	for(int j=0;j<index;j++)
	{
         if((m_pMapData+j)->IsDelete)
		 {
			 memmove(m_pMapData+j,m_pMapData+index -1 ,sizeof(CReportData));
             nDelete++;
             index--;
			 j--;
		 }
	}
	m_RealFileHead->StockCount -=nDelete;
    return TRUE;    
}
BOOL CSharesInformation::RemoveAllStockCjmxTj()
{
     for(int i=0;i<STOCKTYPENUM;i++)
	 {
		 if(i==SHZS||i==SZZS||i==BLOCKINDEX)
			 continue;
		 for(int j=0;j<m_pdwStockCurrentCount[i];j++)
		 {
			 if(m_pData[i][j].pItem!=NULL)
               m_pData[i][j].pItem->IsMxTj=FALSE;
		 }
	 }
	 return TRUE;
}
BOOL CSharesInformation::RemoveStockCjmxTj(char *code,int nKind,BOOL IsAdd)
{
	CReportData *pCdat;
	if(Lookup(code , pCdat,nKind))
	{
		if(IsAdd)
		   pCdat->IsMxTj=TRUE;
		else
		   pCdat->IsMxTj=FALSE;
		return TRUE;
	}
	return FALSE;
}
int CSharesInformation::GetStockPos(int StockType,char *StockId)  //找查股票
{
	int low=0;
	int high=m_pdwStockCurrentCount[StockType]-1 ;
	int mid=0;
    while(low <= high)
	{
		 mid=(low+high)/2;
		 if(strcmp(m_pData[StockType][mid].StockId , StockId)>0) high=mid -1;
         else if(strcmp(m_pData[StockType][mid].StockId , StockId)< 0 ) low=mid +1;
		 else 
		 {
			 return mid ;
		 }
	}
	return -1;
}
BOOL CSharesInformation::ImportCaiwuInfo(BASEINFO *pBaseinfo)
{
	CReportData *pCdat;
    CTaiShanDoc	*m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_pDoc ;
	CString m_strStockKind=pBaseinfo->Symbol;
    CString m_strZqdm=m_strStockKind.Right(m_strStockKind.GetLength()-2);
	int nKind=m_pDoc->GetStockKind(m_strStockKind.Left(2));
//	if(m_strZqdm == "600601" || m_strZqdm == "600637" )
//		m_strZqdm = m_strZqdm;
	if(Lookup(m_strZqdm.GetBuffer(0),pCdat,nKind))
	{
		 if(pCdat->pBaseInfo==NULL)
		 {
			BASEINFO *pBaseItem;
			if(!m_StockBaseInfo.AddStockItem(pCdat->id,nKind,pBaseItem))              //在基本资料数据文件中增加一块数据区域
			{
				AfxMessageBox("增加股票基本资料空间区域时出错！");
				return FALSE;
			}
			pBaseItem->NumSplit=0;
			pCdat->pBaseInfo=pBaseItem;
		 }
         memcpy(&pCdat->pBaseInfo->zgb, &pBaseinfo->zgb,sizeof(float)*33);
	     m_StockBaseInfo.SaveBaseInfoToFile(pCdat->pBaseInfo,sizeof(BASEINFO));
		 return TRUE; 
	}
	return FALSE;

}
BOOL  CSharesInformation::InsertItemCorrect(char *StockId ,PCdat1 pStockData, DWORD StockType) //按类找查股票
{
	int low=0;
	int high=m_pdwStockCurrentCount[StockType] -1;
	int mid=0;
	int InsertPose=-1;
    while(low <= high)
	{
		 mid=(low+high)/2;
	     if(low==high)
		    if(strcmp(StockId , m_pData[StockType][mid].StockId )>0)
			{
               if(m_pdwStockCurrentCount[StockType]==0||m_pdwStockCurrentCount[StockType]==mid)
                  InsertPose=mid ;
			   else
                  InsertPose=mid +1;   
			   break;
			}
		    else if(strcmp(StockId , m_pData[StockType][mid].StockId)<0)
			{
			   InsertPose=mid ;
			   break;
			}
			else
			{
				 pStockData->IsDelete=TRUE;
				 return TRUE ;
			}
 		 if(strcmp(m_pData[StockType][mid].StockId , StockId)>0) high=mid -1;
         else if(strcmp(m_pData[StockType][mid].StockId , StockId)< 0 ) low=mid +1;
		 else 
		 {
             pStockData->IsDelete=TRUE;
			 return TRUE ;
		 }
	}
	if(high<low)
		InsertPose=low;


	if(m_pdwStockCurrentCount[StockType] + 1 > m_dwStockMaxCount[StockType])  //检验现有股票数是否大于最大保存数
	{
        SetMemoryAllocSize(StockType,m_pdwStockCurrentCount[StockType]);
	}
	if(m_pdwStockCurrentCount[StockType] > InsertPose)
	{
       STOCKDATASHOW *ptemp;
	   HGLOBAL	hMem;
	   LPVOID hp;
	   hMem = GlobalAlloc( GPTR, (m_pdwStockCurrentCount[StockType] - InsertPose )* sizeof( STOCKDATASHOW) );
	   hp=GlobalLock(hMem);
	   if(hp)
	   {
			ptemp= (STOCKDATASHOW *)hp;
	   }
	   else
	   {
			AfxMessageBox("分配内存出错",MB_ICONSTOP);
			return FALSE;
	   }
	   memcpy(ptemp,&m_pData[StockType][InsertPose],(m_pdwStockCurrentCount[StockType] - InsertPose)*sizeof(STOCKDATASHOW));
	   memcpy(&m_pData[StockType][InsertPose+1],ptemp,(m_pdwStockCurrentCount[StockType] - InsertPose)*sizeof(STOCKDATASHOW));
       GlobalUnlock((HGLOBAL)ptemp);        //释放内存
	   GlobalFree( (HGLOBAL)ptemp);
    }
    m_pData[StockType][InsertPose].pItem=pStockData;
	strcpy(m_pData[StockType][InsertPose].StockId,StockId);

	m_pData[StockType][InsertPose].pItem->pBaseInfo =NULL;
	m_pData[StockType][InsertPose].pItem->pBlockInfo =NULL;
	
    strcpy(m_pData[StockType][InsertPose].pItem->id  ,StockId);
    m_pdwStockCurrentCount[StockType]++;
	m_RealFileHead->StockCount++;

    SaveRealDataToFile(m_RealFileHead,sizeof(REALDATA)+sizeof(long)*STOCKTYPENUM); 
    SaveRealDataToFile(m_pData[StockType][InsertPose].pItem,sizeof(CReportData)); 

	return TRUE;
}
BOOL CSharesInformation::DeleteAllStockFromStockType()
{
	int m_counts=GetStockTypeCount(BLOCKINDEX);
	for(int i=m_counts-1;  i>=0 ; i--)
       RemoveKey(m_pData[BLOCKINDEX][i].StockId ,BLOCKINDEX);   
	return TRUE;
}
BOOL CSharesInformation::InitEmptyDatabase()
{
	typedef struct 
	{
		 char kind;              //      股票种类
		 char id[7];             //      证券代码
		 char name[9];           //      证券名称 
		 char Gppyjc[5];         //      证券名称拼音简称
		 char group;             //      股票组别
		 long sel;
	}TempCdat;

	FILE *fp;
    CReportData *pCdat;
	TempCdat Cdat;
	int StockCount[STOCKTYPENUM];
	if(_access("stockname.dat",0)==-1)   
       return FALSE;
	if((fp=_fsopen("stockname.dat","rb",SH_DENYNO))!=NULL)
	{
		fseek(fp,0,SEEK_SET);
		fread(&StockCount[0],4,STOCKTYPENUM, fp);
        while(!feof(fp)&&!ferror(fp))
		{
	        CString Zqdm;
			long Serl=0;
			char group=0;
			memset(&Cdat,0,sizeof(TempCdat));
			fread(&Cdat.kind,1,1,fp);      // 股票种类           1字节
			fread(Cdat.id ,1,6,fp);        // 股票代码           6字节
            Cdat.id[6]='\0';
			fread(Cdat.name,1,8,fp);       // 股票名称           8字节 
            Cdat.name[8]='\0';
			fread(Cdat.Gppyjc ,1,4,fp);    // 股票名称拼音简称   4字节 
            Cdat.Gppyjc[4]='\0'; 
			fread(&Cdat.sel,1,4,fp);      // 股票顺序号4字节  
			char ch123 ;
			fread(&ch123,1,1,fp);

			if (Lookup(Cdat.id, pCdat,Cdat.kind) != TRUE)     //检测该股票是否已记录在内存模板中
			{
				if(strlen(Cdat.id)==6||strlen(Cdat.id)==4)
				{
					int stocktype=Cdat.kind;
					if(stocktype>=0&&stocktype<=STOCKTYPENUM)
					{
						//if(strcmp(Cdat.id,"580023") == 0 || strcmp(Cdat.id,"399001") == 0 )
						//	int iii = 0;
						if(!InsertItem(Cdat.id,pCdat,Cdat.kind))
							continue;
						strcpy(pCdat->name , Cdat.name);
						strcpy(pCdat->id ,Cdat.id );
						pCdat->kind=Cdat.kind;
						strcpy(pCdat->Gppyjc ,Cdat.Gppyjc );
					}
				}
			} 
			else
			{
				if(strcmp(pCdat->name,Cdat.name)!=0)
				{
					strcpy(pCdat->name,Cdat.name);
					strcpy(pCdat->Gppyjc,Cdat.Gppyjc);
				}
			}
//*************************************************
		}
		fclose(fp);
	}
	return TRUE;
}

BOOL CSharesInformation::AddBaseinfoPoint(CString strStockCode,int nKind,PBASEINFO &pBaseinfo)
{
	pBaseinfo=NULL;
	CReportData *pCdat;

	if(Lookup(strStockCode.GetBuffer(0) ,pCdat,nKind))
	{
		 if(pCdat->pBaseInfo==NULL)
		 {
			 BASEINFO *pBaseItem=NULL;
			if(!m_StockBaseInfo.AddStockItem(pCdat->id,nKind,pBaseItem))              //在基本资料数据文件中增加一块数据区域
			{
				AfxMessageBox("增加股票基本资料空间区域时出错！");
				return FALSE;
			}
			pBaseItem->NumSplit=0;
			pCdat->pBaseInfo=pBaseItem;
			pBaseinfo=pBaseItem;
		    return TRUE; 
		 }
		 else
		 {
		    pBaseinfo=pCdat->pBaseInfo;
			return TRUE;
		 }
	}
	return FALSE;
}
#ifdef WIDE_NET_VERSION
BOOL CSharesInformation::ClearAllRealTimeMarketData()
{
	BYTE *temp;
	m_RealFileHead->StockCount =0;
	for(int j=0;j<STOCKTYPENUM;j++)
        m_pdwStockCurrentCount[0]=0;

	memset(Nidx[0],0,240*sizeof(Rsdn1));
	memset(Nidx[1],0,240*sizeof(Rsdn1));
	memset(Nidx[1],0,240*sizeof(Rsdn1));
	memset(Tidx[0],0,240*sizeof(Tidxd));
	memset(Tidx[1],0,240*sizeof(Tidxd));
	memset(Tidx[2],0,240*sizeof(Tidxd));
    for (j=0; j<240; j++)
	{
       Tidx[0][j].sec5=Tidx[1][j].sec5=Tidx[2][j].sec5=9911;
	}
	return TRUE;
}

#endif

DWORD CSharesInformation::GetStockKind(int MarketType,char *strLabel)
{
	char *StockId;
	StockId=strLabel;
	if(MarketType==SH_MARKET_EX)
	{
		if(StockId[0] == '0')
		{
			if(StockId[1] == '0' && StockId[2] == '0')
				return SHZS;//上证指数
			else if (StockId[1] == '0' || StockId[1] == '1')
				return SHZQ;//上证债券
		}
		else if (StockId[0] == '1')
		{
			if((StockId[1] == 'A') || (StockId[1] == 'B') || (StockId[1]=='C'))
				return SHZS;//上证指数
			else if(StockId[1] == '0' || StockId[1] == '1')
				return SHZZ;//上证转债
			else if(StockId[1] == '2')
				return SHZQ;//上证债券
		}
		else if (StockId[0] == '2')
		{
			if(StockId[1] == '0')
				return SHHG;//上证回购
		}
		else if (StockId[0] == '5')
		{
			if(StockId[1] == '0')
				return SHJJ;//上证基金
			else if(StockId[1] == '1')
				return SHETF;//上证ETF基金
			else if(StockId[1] == '8')
				return SHQZ;//上证权证
		}
		else if (StockId[0] == '6')
		{
			return SHAG;//上证A股
		}
		else if (StockId[0] == '9')
		{
			return SHBG;//上证B股
		}
		
		//if ( StockId[0]<'5' || StockId[0]=='7' )	//qi huo zai quan 债券 与指数
		//{
		//	if((StockId[1]=='A') ||(StockId[1]=='B' )||(StockId[1]=='C'))     //指数
		//		return SHZS;
		//	else                                             //配股 
		//		return SHZQ;
		//}
		//else if( StockId[0] == '5' ) 
		//{                                                                    // shanghai stock
  //          return SHJJ; 
		//}			
		//else
		//{                                                                    // shanghai stock
		//	if ( StockId[0] < '9' )             // A
  //              return SHAG; 
		//	else  if(StockId[0] == '9')                      // B
		//		return SHBG;
		//}			
	}
	else if(MarketType==SZ_MARKET_EX)
	{
		if(StockId[0] == '0')
		{
			if(StockId[1] == '0')
			{
				if(StockId[2] == '2')
					return SZZXQY;//深证中小企业
				else
					return SZAG;//深证A股
			}
			else if(StockId[1] == '3')
				return SZQZ;//深证权证
		}
		else if (StockId[0] == '1')
		{
			if(StockId[1] == '0' || StockId[1] == '1')
				return SZZQ;//深证债券
			else if(StockId[1] == '2')
				return SZZZ;//深证转债
			else if(StockId[1] == '3')
				return SZHG;//深证回购
			else if(StockId[1] == '5')
				return SZETF;//深证ETF基金
			else if(StockId[1] == '6')
				return SZKF;//深证开放基金
			else if(StockId[1] == '8')
				return SZJJ;//深证基金
		}
		else if (StockId[0] == '2')
		{
			return SZBG;//深证B股
		}
		else if (StockId[0] == '3')
		{
			if(StockId[1] == '9' && StockId[2] == '9')
				return SZZS;//深证指数
			else if(StockId[1] == '0' && StockId[2] == '0')
				return SZCYB;//深证其它
			else
				return SZOT;//深证其它
		}
		else if (StockId[0] == '4')
		{
			return SZSB;//深证三板
		}

		//if(strlen(StockId)==6)
		//{
		//	if ( StockId[0]=='0' )
		//	{
		//		return SZAG; //A股证券、A股增发、A股权证
		//	}

		//	if (StockId[0]=='1' )
		//	{
		//		if ( StockId[1]=='7'/*原有投资基金*/ || StockId[1]=='8'/*证券投资基金*/ )
		//			return SZJJ;
		//		else
		//			return SZZQ; //债券
		//	}

		//	if (StockId[0]=='2') //B股证券
		//		return SZBG;

		//	if (StockId[0]=='3')
		//	{
		//		if (StockId[1]=='9')//综合指数、成份指数
		//			return SZZS;
		//		else
		//			return EBAG; //创业板
		//	}
		//	
		//}
		//else if(strlen(StockId)==4)
		//{
		//	if(StockId[0]=='9'&&StockId[1]=='9')
		//		return SZZS;
		//	if ( StockId[0]=='0' ||  StockId[0]=='4' )          //A股
		//		return SZAG;
		//	else if (StockId[0]=='6')
		//		return EBAG;
		//	else if(StockId[0]=='2')                             //B股与指数
		//	{
		//		if( StockId[1]=='A'||StockId[1]=='B'||StockId[1]=='C' )   //指数
		//			return SZZS;
		//		else if(StockId[1]=='D')				//B股  
		//		{
		//			return EBZS;
		//		}
		//		else
		//			return SZBG;
		//	}
		//	else                                                              //债券配股
		//		return SZZQ;
		//}
	}
	return BLOCKINDEX;
}

CString CSharesInformation::Symbol4To6(CString sIn)
{
		//
	CString Symbol4[]={
		"2A01","2A02","2A03",
		"0696","0896","1990",
		"9905","9906","9907","9908",
		"9911","9912","9913","9914","9915","9916","9917","9918","9919",
		"9920","9921","9922","9923",
		"9931","9932","9933","9934","9935","9936","9937","9938","9939"
	};

	CString Symbol6[]={
		"399001","399002","399003",
		"001696","001896","131990",
		"399305","399106","399107","399108",
		"399110","399120","399130","399140","399150","399160","399170","399180","399190",
		"399200","399210","399220","399230",
		"399131","399132","399133","399134","399135","399136","399137","399138","399139"
	};
	//
	CString sOut = sIn;
	if(strlen(sIn)==4)
	{
		
		for( int i=0;i<sizeof(Symbol4)/sizeof(CString);i++)
		{
			if(Symbol4[i] == sIn )
				return Symbol6[i];
		}
		
		 //A股证券
		if( sIn[0]=='0' )                          
			return  "00"+sIn;

		//原有投资基金、证券投资基金
		else if( sIn[0]=='4'  )               
		{
			if(sIn[1]=='0' || sIn[1]=='5')//原有投资基金
				return  "17"+sIn;
			if(sIn[1]=='6' || sIn[1]=='7')//证券投资基金
				return  "18"+sIn;
		}

		//B股证券
		else if( sIn[0]=='2' && isdigit( sIn[1] ) ) 
		{
			return "200" + sIn.Right(3);
		}

		 //国债现货
		else if(sIn.Left(2) == "16" ||sIn.Left(2) == "17" || sIn.Left(2) == "19" )
			return  "10" + sIn;

		//国债回购
		else if(sIn.Left(2) == "12" ||sIn.Left(2) == "13" || sIn.Left(2) == "14" ||sIn.Left(2) == "18" ) 
			return  "13" + sIn;

		//可转换债券
		else if(sIn.Left(1) == "5" )  
			return  "12" + sIn;

		//A股权证、A股增发、A股转配
		else if(sIn.Left(1) == "3"||sIn.Left(1) == "7" ||sIn.Left(1) == "8" || sIn.Left(1) == "9") 
		{
			CString ss = "0" + sIn.Left(1) + "0";
			return  ss + sIn.Right (3);
		}

		//债券
		else if(sIn.Left(2) == "10" ) 
			return  "11" + sIn;
	}
	return sOut;
}

void CSharesInformation::AddStocksIfEmpty()
{
	if(this->GetStockTypeCount(SHZS)<=0)
		InitEmptyDatabase();
}

bool CSharesInformation::IsZhiShu(int kind)
{
	bool b = false;
	if( kind==SHZS|| kind==SZZS || kind==BLOCKINDEX)
		b = true;
	return b;

}

void CSharesInformation::CalcUpDown(int which_stk)
{
	//20021218 b
	{
		int nKind1[] = {SHAG,SHBG,SHJJ,SHZQ,SHZZ,SHHG,SHETF,SHKF,SHQZ,SHOT,STOCKTYPENUM,STOCKTYPENUM,STOCKTYPENUM};
		if(which_stk == 1)
		{
			nKind1[0] = SZAG;
			nKind1[1] = SZBG;
			nKind1[2] = SZJJ;
			nKind1[3] = SZZQ;
			nKind1[4] = SZZZ;
			nKind1[5] = SZHG;
			nKind1[6] = SZETF;
			nKind1[7] = SZKF;
			nKind1[8] = SZQZ;
			nKind1[9] = SZSB;
			nKind1[10] = SZOT;
			nKind1[11] = SZZXQY;
			nKind1[12] = SZCYB;
		}

		ClearUpDown(which_stk);
		for(int i=0;i<13;i++)
		{
			int temp=GetStockTypeCount(nKind1[i]);
			for(int j=0;j<temp;j++)
			{
				CString str;
				CReportData * Cdat;
				GetStockItem(nKind1[i], j, Cdat);
				if(Cdat==NULL)
					continue;
			// skip no use stock
				if (Cdat->ystc>0.001 && Cdat->nowp>0.001)
				{
					float tmp1=(float)(Cdat->nowp-Cdat->ystc);
					if(tmp1>0)
					{
						m_countUp[which_stk][i]++;
					}
					else if(tmp1<0)
					{
						m_countDown[which_stk][i]++;
					}
					else
					{
						m_countEqual[which_stk][i]++;
					}
					m_indexAmount[which_stk][i]+=Cdat->totp ;
				}
//				else
//					m_countEqual[which_stk][i]++;
			}
		}
	}
	//20021218 e
}

void CSharesInformation::ClearUpDown(int nMarket)
{
	if(nMarket<0 ||nMarket>2)
		return;
//	for(int i=0;i<3;i++)
		for(int j=0;j<11;j++)
		{
			m_countUp[nMarket][j] =0;
			m_countDown[nMarket][j] = 0;
			m_countEqual[nMarket][j] = 0;
			m_indexAmount[nMarket][j] = 0.0f;
		}

}

float CSharesInformation::GetValueUpDown(int isDown, int whick_stk, int nKind)
{
	if(isDown ==0)
	{
		return (float)m_countUp[whick_stk][nKind];
	}
	else if(isDown ==2)
	{
		return (float)m_countDown[whick_stk][nKind];
	}
	else if(isDown ==1)
	{
		return (float)m_countEqual[whick_stk][nKind];
	}
	else 
	{
		return m_indexAmount[whick_stk][nKind];
	}

}

BOOL CSharesInformation::ImportCwData(CString FileName)
{

	CFile InFile;
	if(!(InFile.Open(FileName,CFile::modeRead,NULL)))
	{
		AfxMessageBox("引入财务文件不可读!");
		return FALSE;
	}
	int nFlag;
	InFile.Read(&nFlag,4);
	char nVer[5];
	int nTotal;
	InFile.Read(&nTotal,4);
	InFile.Seek(8,CFile::begin);

	BASEINFO baseinfo;
	BASEINFO * pBase = &baseinfo;
	char ch[16];
	float fVal[50];
	int nVal[50];
	while(1)
	{
		memset(&baseinfo,0,sizeof(BASEINFO));
		if(InFile.Read (ch,4)!=4) break;
		ch[2] = '\0';
		CString ss = ch;
		if(InFile.Read (ch,8)!=8) break;
		ch[6] = '\0';
		CString ss2 = ch;
		int nKind = 0;
		if(ss == "SH")
			nKind = GetStockKind (SH_MARKET_EX,ch);
		else
			nKind = GetStockKind (SZ_MARKET_EX,ch);

		CString s = CMainFrame::m_pDoc->GetStockKindString(nKind);
		s+=ch;
		CString s640 = ch;
		strcpy(baseinfo.Symbol ,s);

		if(InFile.Read (ch,2)!=2) break;
		float f2 = 0;
		if(InFile.Read (&f2,4)!=4) break;
		if(InFile.Read (fVal,148)!=148) break;
		pBase->zgb = fVal[0];
		pBase->gjg = fVal[1];
		pBase->fqrfrg = fVal[2];
		pBase->frg = fVal[3];
		pBase->Bg = fVal[4];

		pBase->Hg = fVal[5];
		pBase->ltAg = fVal[6];
		pBase->zgg = fVal[7];
		//		   pBase->zpg = fVal[8];//A2转配股
		pBase->zzc = fVal[9]/10;//10

		pBase->ldzc = fVal[10]/10;
		pBase->gdzc = fVal[11]/10;
		pBase->wxzc = fVal[12]/10;
		pBase->cqtz = fVal[13]/10;
		pBase->ldfz = fVal[14]/10;

		pBase->cqfz = fVal[15]/10;
		pBase->zbgjj = fVal[16]/10;
		pBase->mggjj = fVal[17];
		pBase->ggqy = fVal[18]/10;
		pBase->zyywsr = fVal[19]/10;//20

		pBase->zyywlr = fVal[20]/10;
		pBase->qtywlr = fVal[21]/10;
		pBase->zyywlr = fVal[22]/10;
		//		   pBase->tz = fVal[23];
		//		   pBase->zgb = fVal[24];

		//		   pBase->zgb = fVal[25];
		//		   pBase->zgb = fVal[26];
		pBase->lrze = fVal[27]/10;
		//		   pBase->jlr = fVal[28]/10;
		pBase->jlr = fVal[29]/10;//30

		pBase->wfplr = fVal[30]/10;
		pBase->mgwfplr  = fVal[31];
		pBase->mgsy = fVal[32];//mgsy
		pBase->mgjzc = fVal[33];//mgjzc
		//		   pBase-> = fVal[34];

		pBase->gdqybl  = fVal[35];
		pBase->jzcsyl = fVal[36];
		//		   pBase->zgb = fVal[37];

		if(nKind!=SHZS && nKind!=SZZS)//指数没有基本资料
			ImportCaiwuInfo(&baseinfo);
	}
	InFile.Close();

	return TRUE;
}

//保存财务数据
BOOL CSharesInformation::SaveCaiwuData()
{

	CFile OutFile;
	if(!OutFile.Open(g_caiwudata,CFile::modeCreate|CFile::modeWrite,NULL))
	{
		AfxMessageBox("导出财务信息失败!",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	int nHeaderFlag=65792509;
	char nVer[5]="v2.0";
	int nTotalCount=0;
	//
	//first initialize the file header
	OutFile.Write(&nHeaderFlag,sizeof(int));
	OutFile.Write(nVer,4);
	OutFile.Write(&nTotalCount,sizeof(int));
	OutFile.Seek(16,CFile::begin);

	//Get all stock Caiwu info and write to the outfile
	CReportData *pDat1=NULL;
	for(int nStockType=0;nStockType < STOCKTYPENUM;nStockType++)
	{
		for(int i=0;i<GetStockTypeCount(nStockType);i++)
		{
			GetStockItem(nStockType,i,pDat1);
			if(pDat1 && pDat1->pBaseInfo )
			{
				OutFile.Write(pDat1->pBaseInfo,sizeof(BASEINFO)-sizeof(pDat1->pBaseInfo->m_Split)-12);
				nTotalCount++;
			}
		}	
	}		
	//文件的9-12字节为导出股票的总数。
	if(nTotalCount>0)
	{
		OutFile.Seek(8, CFile::begin);
		OutFile.Write(&nTotalCount, sizeof(int));
		OutFile.Close();
	}
	else
	{
		AfxMessageBox("没有财务数据!",MB_ICONASTERISK);
		OutFile.Close();
	}

	return TRUE;
}

void CSharesInformation::UpdateCwData(void)
{
	for(int i=0;i<STOCKTYPENUM;i++)
	{
		if(i==SHZS||i==SZZS||i==BLOCKINDEX)
			continue;
		for(int j=0;j<m_pdwStockCurrentCount[i];j++)
		{
			if(m_pData[i][j].pItem && m_pData[i][j].pItem->pBaseInfo == NULL)
			{
				ImportCwData(g_caiwudata);
				return ;
			}
		}
	}
	return;

}
