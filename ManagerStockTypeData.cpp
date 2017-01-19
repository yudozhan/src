// ManagerStockTypeData.cpp: implementation of the CManageBlockData class.
//
//////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////

// �ļ�����   ManagerStockTypeData.h 
//            ManagerStockTypeData.cpp
//
// �����ߣ�   

// ����ʱ�䣺 

// �������������ڶ�̬��ʾ�ư�����ݿ��Ʋ���

// ģ���ţ�004

///////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiShanDoc.h"
#include "mainfrm.h"
#include <share.h>
#include  <io.h>
#include "StructTaiShares.h"
#include "ManagerStockTypeData.h"
#include "CTaiShanReportView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define MAX_RIGHTS 3
#define STOCKADDCOUNT 10
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CManageBlockData::CManageBlockData()
{
    m_hFile=NULL;
	m_hFileMap=NULL;
	m_pbtData=NULL;
    m_pStockTypeHead=NULL;     //�������ͷ�ṹָ��
    m_pBlockInfo=NULL;     //������ݽṹָ��
    m_pStockInfo=NULL;       //����й�Ʊ���ݽṹָ�� 
	m_pStockPoint=NULL;    //���ڱ����Ʊ�ṹָ��
}

CManageBlockData::~CManageBlockData()
{
   BOOL result=TRUE;
   if(m_hFile)
   {
	   result=RemoveDeletedStock();
//       SavePosToFile();
   }
   if(result)
     m_pStockTypeHead->m_lFileExit=12345678;           //   �ļ������˳���־
   else
     m_pStockTypeHead->m_lFileExit=87654321;           //   �ļ������˳���־
   if(m_pbtData)
	   UnmapViewOfFile(m_pbtData);
   if(m_hFileMap) 
	   CloseHandle(m_hFileMap);
   if(m_hFile)
	   CloseHandle(m_hFile);
   if(m_pStockPoint)
   {
	   GlobalUnlock((HGLOBAL)m_pStockPoint);        //�ͷ��ڴ�
	   GlobalFree( (HGLOBAL)m_pStockPoint);
   }
}
//���ܣ���ʼ����Ʊָ��
//�����������
//�����������
BOOL CManageBlockData::InitStockPoint()                   
{
//	CTaiKlineFileKLine::ConvertBaseInfoFile();
//	CTaiKlineFileKLine::ConvertStockTypeInfo();
	 for(int j=0;j<m_pStockTypeHead->m_lStockCount;j++)
	 {
		 CReportData *pCdat;
		 CString m_strStockKind=m_pStockInfo[j].m_szSymbol;
		 char m_szStockCode[10];
		 int nKind;		 
		 strcpy(m_szStockCode,m_strStockKind.Right(m_strStockKind.GetLength()-2).GetBuffer(0));
		 nKind=m_pDoc->GetStockKind(m_strStockKind.Left(2));
		 if(nKind<0)
			 continue;
		 if(this->m_pDoc->m_sharesInformation.Lookup(m_szStockCode,pCdat,nKind))
		 {
			 pCdat->pBlockInfo=&m_pStockInfo[j];
		 }
		 
	 }
	 return TRUE;
}
//���ܣ���ʼ����Ʊ����ָ�룬�ڲ����Ʊ�����ʱ����
//�����������
//�����������
BOOL CManageBlockData::InitBlockPoint()                   
{
	for(int i=0;i<BLOCKTYPE_NUM;i++)
	{
		if(m_pBlockInfo[i].m_bIsDelete||m_pBlockInfo[i].m_lIsUse==-1 )
			continue;
		 CReportData *pCdat;
         if(!this->m_pDoc->m_sharesInformation.Lookup(m_pBlockInfo[i].m_szCode,pCdat,BLOCKINDEX))//���ҹ�Ʊ�Ƿ����
		 {
			m_pDoc->m_sharesInformation.InsertItem(m_pBlockInfo[i].m_szCode,pCdat,BLOCKINDEX);    //�����Ʊ
			strcpy(pCdat->name ,m_pBlockInfo[i].m_szName );
			strcpy(pCdat->id ,m_pBlockInfo[i].m_szCode );
            pCdat->kind=BLOCKINDEX; 
			if(strlen(pCdat->name)>0)
			   m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;
			m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
		 }
	}
	 return TRUE;
}
//���ܣ���ʼ������ļ�
//���������pathΪ����ļ�·��
//���������TRUE ��ʾ��ʼ���ɹ�
BOOL CManageBlockData::InitBlockData(CString path)
{
	m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_pDoc;
	if(_access(g_stocktypeinfo,0)==-1)   
	{
       InitBlockDataEmpty(path);          //����ļ������ڳ�ʼ��
	}
    else
	{
       InitBlockDataExist(path);          //����ļ����ڳ�ʼ��
	}

    return TRUE;
}
//���ܣ���ʼ���յİ���ļ�
//���������pathΪ�����ļ�·��
//���������TRUE ��ʾ��ʼ���ɹ�
BOOL CManageBlockData::InitBlockDataEmpty(CString path)
{
	BYTE *temp;
	long m_lBufferLength=0;

    m_lBufferLength=sizeof(BLOCKFILEHEAD)+BLOCKTYPE_NUM*sizeof(BLOCKINFO) + 4096 * sizeof(STOCKINFOINBLOCK)+1024*2;
	m_hFile=CreateFile(g_stocktypeinfo,GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)   //�򿪰�������ļ�
	{
		AfxMessageBox("�򿪰�������ļ�����");
		return FALSE; 
	}

	m_hFileMap=CreateFileMapping(m_hFile,  //��������ļ�ӳ���ں�
		NULL,
		PAGE_READWRITE,
		0,
		 m_lBufferLength,
		NULL);
	if(m_hFileMap==NULL)
	{
		AfxMessageBox("��������ļ�ӳ���ں�ʱ����");
		CloseHandle(m_hFile);
		m_hFile=NULL;
		m_hFileMap=NULL;
		return FALSE;
	}
	m_pbtData=(PBYTE)MapViewOfFile(m_hFileMap,  //����������ļ�ӳ��
		FILE_MAP_WRITE,
		0,0,0);
	if(m_pbtData==NULL)
	{
		AfxMessageBox("����������ļ�ӳ�����ڴ�ʱ����");
		CloseHandle(m_hFile);
		CloseHandle(m_hFileMap);
		m_hFile=NULL;
		m_hFileMap=NULL;
		return FALSE;
	}
    m_pStockTypeHead=(BLOCKFILEHEAD *)m_pbtData;
	m_pStockTypeHead->m_lFileTitle=12345678;          //   �ļ���־
    m_pStockTypeHead->m_lFileExit=87654321;           //   �ļ������˳���־
	m_pStockTypeHead->m_lStockTypeCount=0;            //   �����
    m_pStockTypeHead->m_lStockTypeMaxCount=BLOCKTYPE_NUM; //   �������
    m_pStockTypeHead->m_lStockCount=0;                //   Ŀǰ��Ʊ����
    m_pStockTypeHead->m_lStockMaxCount=4096;          //   ����Ʊ����

	//�����ڶ��ķ���
    m_pStockTypeHead->m_lBlockTypeCount=2;          //   ���������
	strcpy(m_pStockTypeHead->m_szBlockTypeName[0],"ȫ�����");
	strcpy(m_pStockTypeHead->m_szBlockTypeName[1],"��ѡ���");
	SaveDataToFile(m_pStockTypeHead,sizeof(BLOCKFILEHEAD));

    temp=m_pbtData+sizeof(BLOCKFILEHEAD);

    m_pBlockInfo = (BLOCKINFO *)temp;           //������ݽṹָ��
	temp  += sizeof(BLOCKINFO) * BLOCKTYPE_NUM;
	for(int i = 0;i < BLOCKTYPE_NUM;i++)
	{
		m_pBlockInfo[i].m_lIsUse = -1;
		m_pBlockInfo[i].m_bIsDelete = TRUE;
	}
	m_pStockInfo=(STOCKINFOINBLOCK *)temp;             //����й�Ʊ���ݽṹָ�� 
    if(!SetMemoryAllocSize(m_pStockTypeHead->m_lStockMaxCount))
	{
		AfxMessageBox("��ʼ�����ݱ�������");
		return FALSE;
	} 
///*
	//��ȡ������͵Ķ���
	CStringArray blockTypeName,stockName;
	CString strName,strBlockType;
	if(ImportBlockTypeName(blockTypeName))
	{
		for (int i = 0;i < blockTypeName.GetCount();i++)
		{
			stockName.RemoveAll();

			strName = blockTypeName[i];
			int iStart = strName.Find('[');
			int iEnd = strName.Find(']');
			if (iStart != -1 && iEnd != -1)//��������
			{
				strBlockType = strName.Mid(iStart + 1,iEnd - 1);
				AddBlockType(strBlockType);
			}
			else if (ImportBlockName(strName,stockName))
			{
				if(CheckBlockName(strName.GetBuffer(0),NULL))
				{
					continue ;
				}
				int l_nStockTypePos = GetInsertBlockPos();
				char temp[10];
				BLOCKINFO *l_pStockType;
				if(!InsertBlock(l_pStockType,l_nStockTypePos))
				{
					continue;
				}
				l_pStockType->m_lIsUse = TRUE;
				l_pStockType->m_bIsDelete = FALSE;

				strcpy(l_pStockType->m_szName,strName.GetBuffer(0));
				strcpy(l_pStockType->m_szBlockTypeName,strBlockType);
				l_pStockType->m_lStockCount=0;
				l_pStockType->m_fTotalRight=0;
				sprintf(temp,"%3d",l_nStockTypePos);
				int i;
				for(i=0;i<3;i++)
				{
					if(temp[i]==' ')temp[i]='0';
				}
				sprintf(l_pStockType->m_szCode,"%s%s","8K0",temp);
				l_pStockType->m_iRightType = ZGB;
				int tmp=stockName.GetCount();
				for(i=0;i<tmp;i++)
				{
					CString strStockCode;
					CReportData *l_pCdat;
					int nKind;

					strStockCode = stockName[i].Right(6);
					if (stockName[i].Left(2) == "SH")
					{
						nKind = CSharesInformation::GetStockKind(SH_MARKET_EX,strStockCode.GetBuffer(0));
					}
					else
					{
						nKind = CSharesInformation::GetStockKind(SZ_MARKET_EX,strStockCode.GetBuffer(0));
					}
					if(m_pDoc->m_sharesInformation.Lookup(strStockCode.GetBuffer(0),l_pCdat,nKind))
					{
						//if(l_pCdat->pBaseInfo!=NULL)
							InsertStockToBlock(l_pCdat,l_pStockType->m_szCode);
					}
				}
				CReportData *pCdat;
				m_pDoc->m_sharesInformation.InsertItem(l_pStockType->m_szCode,pCdat,BLOCKINDEX);
				strcpy(pCdat->name ,l_pStockType->m_szName );
				strcpy(pCdat->id ,l_pStockType->m_szCode );
				pCdat->kind = BLOCKINDEX;
				m_pDoc->m_sharesInformation.SaveRealDataToFile(pCdat,sizeof(CReportData));
				int n = strlen(pCdat->name);
				BOOL bAddKeyboard = TRUE;

				if(strlen(pCdat->name)>0)
				{
					m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;
					m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
				}
			}
		}

	}
//*/
	//������ѡ��顡���
	strBlockType = "��ѡ���";
	CStringArray blockName;
	blockName.Add("��ѡ��1");
	blockName.Add("��ѡ��2");
	blockName.Add("��ѡ��3");
	blockName.Add("��ѡ��4");
	blockName.Add("��ѡ��5");
	for (int i = 0;i < blockName.GetCount();i++)
	{
		if(CheckBlockName(blockName[i].GetBuffer(0),NULL))
		{
			continue ;
		}
		int l_nStockTypePos = GetInsertBlockPos();
		char temp[10];
		BLOCKINFO *l_pStockType;
		if(!InsertBlock(l_pStockType,l_nStockTypePos))
		{
			continue;
		}
		l_pStockType->m_lIsUse=TRUE;
		l_pStockType->m_bIsDelete =FALSE;

		strcpy(l_pStockType->m_szName,blockName[i].GetBuffer(0));
		strcpy(l_pStockType->m_szBlockTypeName,strBlockType);
		l_pStockType->m_lStockCount=0;
		l_pStockType->m_fTotalRight=0;
		sprintf(temp,"%3d",l_nStockTypePos);
		int j;
		for(j=0;j<3;j++)
		{
			if(temp[j]==' ')temp[j]='0';
		}
		sprintf(l_pStockType->m_szCode,"%s%s","8K0",temp);
		l_pStockType->m_iRightType = ZGB;

		CReportData *pCdat;
		m_pDoc->m_sharesInformation.InsertItem(l_pStockType->m_szCode,pCdat,CHOOSESTK);
		strcpy(pCdat->name ,l_pStockType->m_szName );
		strcpy(pCdat->id ,l_pStockType->m_szCode );
		pCdat->kind = CHOOSESTK;
		m_pDoc->m_sharesInformation.SaveRealDataToFile(pCdat,sizeof(CReportData));
		if(strlen(pCdat->name)>0)
			m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;
		m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
	}
	
	return TRUE;
}
//���ܣ���ʼ���Ѵ��ڰ���ļ�
//�����������
//�����������
BOOL CManageBlockData::InitBlockDataExist(CString path)
{
     BYTE *temp;
	 BOOL IsCorrect=FALSE;
	 m_hFile=CreateFile(g_stocktypeinfo,GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("�򿪰�������ļ�����");
		return FALSE; 
	}
	m_hFileMap=CreateFileMapping(m_hFile,
		NULL,
		PAGE_READWRITE,
		0,
		0,
		NULL);
	if(m_hFileMap==NULL)            //������������ļ�ӳ���ں�
	{
		AfxMessageBox("������������ļ�ӳ���ں�ʱ����");
		CloseHandle(m_hFile);
		m_hFile=NULL;
		m_hFileMap=NULL;
		return FALSE;
	}
	m_pbtData=(PBYTE)MapViewOfFile(m_hFileMap,
		FILE_MAP_WRITE,
		0,0,0);
	if(m_pbtData==NULL)            //����������ļ�ӳ�����ڴ�
	{
		AfxMessageBox("����������ļ�ӳ�����ڴ�ʱ����");
		CloseHandle(m_hFile);
		CloseHandle(m_hFileMap);
		m_hFile=NULL;
		m_hFileMap=NULL;
		return FALSE;
	}

    m_pStockTypeHead=(BLOCKFILEHEAD *)m_pbtData;

	if(!((m_pStockTypeHead->m_lFileTitle==12345678) || (m_pStockTypeHead->m_lFileTitle==55555555)))   //����ļ�����
	{
	   UnmapViewOfFile(m_pbtData);
	   CloseHandle(m_hFile);
	   CloseHandle(m_hFileMap);
       return InitBlockDataEmpty(path);
	}
	if(m_pStockTypeHead->m_lFileExit!=12345678)
	{
	    IsCorrect=TRUE;
	}
    m_pStockTypeHead->m_lFileExit=87654321;

    temp=m_pbtData+sizeof(BLOCKFILEHEAD);             //��ʼ���ļ�

    m_pBlockInfo=(BLOCKINFO *)temp;           //������ݽṹָ��
	temp  +=sizeof(BLOCKINFO)*BLOCKTYPE_NUM;
 
	m_pStockInfo=(STOCKINFOINBLOCK *)temp;             //����й�Ʊ���ݽṹָ�� 

    if(!SetMemoryAllocSize(m_pStockTypeHead->m_lStockMaxCount))
	{
		AfxMessageBox("��ʼ�����ݱ�������");
		return FALSE;
	} 
	if(!IsCorrect)
	{
		for(int j=0;j<m_pStockTypeHead->m_lStockCount;j++)
		{
			this->InsertItemPoint(m_pStockInfo+j);
		}
	}
	else
	{
		int temp=m_pStockTypeHead->m_lStockCount;
		m_pStockTypeHead->m_lStockCount=0;
		for(int j=0;j<temp;j++)
		{
			this->InsertStockItemCorrect((m_pStockInfo+j)->m_szSymbol,m_pStockInfo+j);
		}
	}
	SaveDataToFile(m_pStockTypeHead,sizeof(BLOCKFILEHEAD ));
	return TRUE;
}
//���ܣ�����ȫ�����ָ������Ҫ������ս񿪵Ȱ������
//�����������
//�����������
void CManageBlockData::CalcBlockOpenClosePrice()             //����ȫ�����ָ��
{
	int i;

	for(i=0;i<BLOCKTYPE_NUM;i++)    //��ʼ���������
	 {
		if(m_pBlockInfo[i].m_lIsUse==-1)
		    break;
		if(m_pBlockInfo[i].m_bIsDelete )
			continue;
        m_pBlockInfo[i].m_fOpenIndex=0;
        m_pBlockInfo[i].m_fYstIndex=0;
        m_pBlockInfo[i].m_fTotalRight=0;
	 }
     for(i=0;i<m_pDoc->m_sharesInformation.GetTotalStockTypeCount();i++)   //����ÿһ��Ʊ�����ֵ
	 {
		 ProcMessage();
		 if(!CheckKind( i))
			 continue;
		 float fTotal = 0;
		 float fLtp = 0;
		 for(int j=0;j<m_pDoc->m_sharesInformation.GetStockTypeCount(i);j++)
		 {
            CReportData *pCdat;
			int index=0;
			m_pDoc->m_sharesInformation.GetStockItem(i,j,pCdat);
			if(pCdat==NULL||pCdat->pBlockInfo==NULL)
				continue;

			//20021218 b
/*			if(pCdat->pBaseInfo)
			{
				fTotal+=pCdat->pBaseInfo->zgb ;
				fLtp+=pCdat->pBaseInfo->ltAg  ;
			}*/
			//20021218 e
             
			while(pCdat->pBlockInfo->m_nBlockType[index]!=BLOCKTYPE_NUM&&index<BLOCKTYPE_NUM)//����ÿһ��Ʊȫ�����ָ��
			{
				ProcMessage();
				if (pCdat->pBlockInfo->m_ClosePrice <= 0.0)
				{
					CTaiKlineFileKLine::GetFirstKline(pCdat->id,pCdat->kind,TRUE,1 ,&pCdat->pBlockInfo->m_ClosePrice);
				}
				if (pCdat->pBlockInfo->m_ClosePrice <= 0.0)
				{
					pCdat->pBlockInfo->m_ClosePrice = 1.0f;
				}
                 int l_StockTyep=pCdat->pBlockInfo->m_nBlockType[index];


				 switch(m_pBlockInfo[l_StockTyep].m_iRightType)
				 {
					 case ZGB: if(pCdat->opnp!=0&&pCdat->ystc!=0)
							   {
								  m_pBlockInfo[l_StockTyep].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pCdat->opnp; 
								  m_pBlockInfo[l_StockTyep].m_fYstIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pCdat->ystc; 
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_fRight[ZGB] * pCdat->pBlockInfo->m_ClosePrice;
							   }
							   else if(pCdat->ystc!=0)
							   {
								  m_pBlockInfo[l_StockTyep].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pCdat->ystc; 
								  m_pBlockInfo[l_StockTyep].m_fYstIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pCdat->ystc; 
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_fRight[ZGB]*pCdat->pBlockInfo->m_ClosePrice;
							   }
							   else
							   {
                                  pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
								  m_pBlockInfo[l_StockTyep].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pCdat->ystc; 
								  m_pBlockInfo[l_StockTyep].m_fYstIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pCdat->ystc; 
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_fRight[ZGB]*pCdat->pBlockInfo->m_ClosePrice;
							   }
                               break; 
					 case LTG: if(pCdat->opnp!=0&&pCdat->ystc!=0)
							   {
								  m_pBlockInfo[l_StockTyep].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->opnp; 
								  m_pBlockInfo[l_StockTyep].m_fYstIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->ystc; 
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->pBlockInfo->m_ClosePrice;
							   }
						       else if(pCdat->ystc!=0)
							   {
								  m_pBlockInfo[l_StockTyep].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->ystc; 
								  m_pBlockInfo[l_StockTyep].m_fYstIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->ystc; 
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->pBlockInfo->m_ClosePrice;
							   }
							   else
							   {
                                  pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
								  m_pBlockInfo[l_StockTyep].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->ystc; 
								  m_pBlockInfo[l_StockTyep].m_fYstIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->ystc; 
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->pBlockInfo->m_ClosePrice;
							   }
                               break; 
					 case OTHER: if(pCdat->opnp!=0&&pCdat->ystc!=0)
								 {
								  m_pBlockInfo[l_StockTyep].m_fOpenIndex +=pCdat->opnp; 
								  m_pBlockInfo[l_StockTyep].m_fYstIndex +=pCdat->ystc; 
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;
								 }
						         else if(pCdat->ystc!=0)
								 {
								  m_pBlockInfo[l_StockTyep].m_fOpenIndex +=pCdat->ystc; 
								  m_pBlockInfo[l_StockTyep].m_fYstIndex +=pCdat->ystc; 
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;
								 }
								 else
								 {
                                  pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
								  m_pBlockInfo[l_StockTyep].m_fOpenIndex +=pCdat->ystc; 
								  m_pBlockInfo[l_StockTyep].m_fYstIndex +=pCdat->ystc; 
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;
								 }
                               break; 
				 }
				 index++;
			}

		 }
		//20021218 b
		BASEINFO baseinfo;
		{
/*		   memset(&baseinfo,0,sizeof(BASEINFO));
			 CReportData *pCdat;
			 PBLOCKINFO m_pStktype;
			 m_pDoc->m_sharesInformation.GetStockItem(BLOCKINDEX,i,pCdat);
			 if(pCdat!=NULL)
			 {
			   int nKind = BLOCKINDEX;
			   CString s = CMainFrame::m_pDoc->GetStockKindString(nKind);
			   s+=pCdat->id;
			   strcpy(baseinfo.Symbol ,s);
			   baseinfo.ltAg = fLtp;
			   baseinfo.zgb = fTotal;
			 }*/

		}
//		CMainFrame::m_pDoc->m_sharesInformation.ImportCaiwuInfo(&baseinfo);
		//20021218 e
	 }
	 for(i=0;i<m_pDoc->m_sharesInformation.GetStockTypeCount(BLOCKINDEX);i++)   //����ȫ�������ָ��
	 {
		 ProcMessage();
         CReportData *pCdat;
		 PBLOCKINFO m_pStktype;
		 m_pDoc->m_sharesInformation.GetStockItem(BLOCKINDEX,i,pCdat);
         if(pCdat==NULL)
			 continue;
		 GetBlockPoint(m_pStktype,pCdat->id);
		 if(m_pStktype->m_fTotalRight==0)
			 continue;

		 pCdat->ystc=m_pStktype->m_fYstIndex/(/*m_pStktype->m_lStockCount **/ m_pStktype->m_fTotalRight) * 1000;

		 if(pCdat->lowp>pCdat->ystc)
            pCdat->lowp=pCdat->ystc;
		 if(pCdat->higp<pCdat->ystc)
            pCdat->higp=pCdat->ystc;

         pCdat->opnp=m_pStktype->m_fOpenIndex/(/*m_pStktype->m_lStockCount **/ m_pStktype->m_fTotalRight) * 1000;

		 if(pCdat->lowp>pCdat->opnp)
            pCdat->lowp=pCdat->opnp;
		 if(pCdat->higp<pCdat->opnp)
            pCdat->higp=pCdat->opnp;
	 }
}
//���ܣ�����ʵʱ���ָ����ÿ���Ӽ���ȫ���������
//�����������
//�����������
void CManageBlockData::CalcRealTimeBlockIndex()       //����ʵʱ���ָ��
{
	 if(!m_pDoc-> m_bInitDone)
		 return ;

	 int i;
	 for(i=0;i<BLOCKTYPE_NUM;i++)     //��ʼ��ÿһ�������
	 {
		if(m_pBlockInfo[i].m_lIsUse==-1 )
            break;
		if(m_pBlockInfo[i].m_bIsDelete)
			continue;
        m_pBlockInfo[i].m_fNewIndex=0;
        m_pBlockInfo[i].m_fTotalRight=0;
		m_pBlockInfo[i].m_fHighIndex=INT_MIN;
		m_pBlockInfo[i].m_fLowIndex=INT_MAX;
	    m_pBlockInfo[i].m_fTotP=0;         //   ���³ɽ����  
	    m_pBlockInfo[i].m_fTotV=0;         //   ���³ɽ���
	    m_pBlockInfo[i].m_fTotRVol=0;      //   ����
	    m_pBlockInfo[i].m_fTotDVol=0;      //   ����
		m_pBlockInfo[i].m_fLTP = 0;      //   ��ͨ��
		m_pBlockInfo[i].m_fMGSY = 0;      //   ��ͨ��
		m_pBlockInfo[i].m_lUpStockCount = 0;
		m_pBlockInfo[i].m_fRDR = 0;
	 }
     for(i=0;i<m_pDoc->m_sharesInformation.GetTotalStockTypeCount();i++)     //����ÿһ��Ʊ�������
	 {
		 ProcMessage();
		 if(!CheckKind( i))
			 continue;
		 for(int j=0;j<m_pDoc->m_sharesInformation.GetStockTypeCount(i);j++)
		 {
			ProcMessage();
            CReportData *pCdat;
			int index=0;
			m_pDoc->m_sharesInformation.GetStockItem(i,j,pCdat);
			if(pCdat==NULL || pCdat->pBlockInfo==NULL)
				continue; 
			while(pCdat->pBlockInfo->m_nBlockType[index] != BLOCKTYPE_NUM && index < BLOCKTYPE_NUM)   //����ĳһ��Ʊȫ�����ָ����ֵ
			{
				if (pCdat->pBlockInfo->m_ClosePrice <= 0.0)
				{
					CTaiKlineFileKLine::GetFirstKline(pCdat->id,pCdat->kind,TRUE,1 ,&pCdat->pBlockInfo->m_ClosePrice);
				}

                 int l_StockTyep=pCdat->pBlockInfo->m_nBlockType[index];
				 switch(m_pBlockInfo[l_StockTyep].m_iRightType)
				 {
					 case ZGB: if(pCdat->nowp!=0)
							   {
                                  m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pCdat->nowp; 
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice*pCdat->pBlockInfo->m_fRight[ZGB];
							   }
						       else if(pCdat->ystc!=0)
							   {
                                  m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pCdat->ystc; 
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice*pCdat->pBlockInfo->m_fRight[ZGB];
							   }
							   else
							   {
                                  pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
                                  m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pCdat->ystc; 
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice*pCdat->pBlockInfo->m_fRight[ZGB];
							   }
							   m_pBlockInfo[l_StockTyep].m_fTotP +=pCdat->totp;         //   ���³ɽ����  
							   m_pBlockInfo[l_StockTyep].m_fTotV +=pCdat->totv; 							   //   ���³ɽ���
							   m_pBlockInfo[l_StockTyep].m_fTotRVol+=pCdat->rvol;      //   ����
							   m_pBlockInfo[l_StockTyep].m_fTotDVol+=pCdat->dvol;      //   ����
                               break; 
					 case LTG: if(pCdat->nowp!=0)
							   {
							      m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->nowp;
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice*pCdat->pBlockInfo->m_fRight[LTG];
							   }
						       else if(pCdat->ystc!=0)
							   {
                                  m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->ystc; 
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice*pCdat->pBlockInfo->m_fRight[LTG];
							   }
							   else
							   {
                                  pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
                                  m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->ystc; 
                                  m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice*pCdat->pBlockInfo->m_fRight[ZGB];
							   }
							   m_pBlockInfo[l_StockTyep].m_fTotP +=pCdat->totp;         //   ���³ɽ����  
							   m_pBlockInfo[l_StockTyep].m_fTotV +=pCdat->totv;         //   ���³ɽ���
							   m_pBlockInfo[l_StockTyep].m_fTotRVol +=pCdat->rvol;      //   ����
							   m_pBlockInfo[l_StockTyep].m_fTotDVol +=pCdat->dvol;      //   ����
                               break; 
					 case OTHER: if(pCdat->nowp!=0)
								 {
                                   m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->nowp;
                                   m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;
								 }
						         else if(pCdat->ystc!=0)
								 {
                                   m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->ystc; 
                                   m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;
								 } 
								 else
								 {
                                  pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
                                   m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->ystc; 
                                   m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;
								 }
							   m_pBlockInfo[l_StockTyep].m_fTotP +=pCdat->totp;         //   ���³ɽ����  
							   m_pBlockInfo[l_StockTyep].m_fTotV +=pCdat->totv;         //   ���³ɽ���
							   m_pBlockInfo[l_StockTyep].m_fTotRVol+=pCdat->rvol;      //   ����
							   m_pBlockInfo[l_StockTyep].m_fTotDVol+=pCdat->dvol;      //   ����
                               break; 
				 }
				 m_pBlockInfo[l_StockTyep].m_fLTP += pCdat->pBlockInfo->m_fRight[LTG];      //   ��ͨ��
				 m_pBlockInfo[l_StockTyep].m_fMGSY += pCdat->pBaseInfo->mgsy;      //   ��ͨ��

				 if (pCdat->nowp > pCdat->ystc)
				 {
					m_pBlockInfo[l_StockTyep].m_lUpStockCount++;
				 }
				 float fRDR = (pCdat->nowp - pCdat->ystc )/ pCdat->ystc;
				 if( fRDR> m_pBlockInfo[l_StockTyep].m_fRDR)
				 {
					 m_pBlockInfo[l_StockTyep].m_fRDR = fRDR;
					 strcpy( m_pBlockInfo[l_StockTyep].m_szFirstStock,pCdat->name);
				 }
				 if (m_pBlockInfo[l_StockTyep].m_fNewIndex > m_pBlockInfo[l_StockTyep].m_fHighIndex)
				 {
					 m_pBlockInfo[l_StockTyep].m_fHighIndex = m_pBlockInfo[l_StockTyep].m_fNewIndex;
				 }
				 if (m_pBlockInfo[l_StockTyep].m_fNewIndex < m_pBlockInfo[l_StockTyep].m_fLowIndex)
				 {
					 m_pBlockInfo[l_StockTyep].m_fLowIndex = m_pBlockInfo[l_StockTyep].m_fNewIndex;
				 }
				 index++;
			}
		 }
	 }
	 for(i=0;i<m_pDoc->m_sharesInformation.GetStockTypeCount(BLOCKINDEX);i++)   //����ÿһ���ָ��
	 {
		 ProcMessage();
         CReportData *pCdat;
		 PBLOCKINFO m_pStktype;
		 m_pDoc->m_sharesInformation.GetStockItem(BLOCKINDEX,i,pCdat);
		 if(pCdat==NULL)
			 return;
		 GetBlockPoint(m_pStktype,pCdat->id);
         if(m_pStktype->m_fTotalRight==0)
		    continue;
         if(pCdat->opnp <= 0 && pCdat->ystc <= 0)       
			  CalcBlockOpenClosePrice();

		 pCdat->nowp=m_pStktype->m_fNewIndex/(/*m_pStktype->m_lStockCount * */m_pStktype->m_fTotalRight) * 1000;
		 pCdat->nowv=m_pStktype->m_fTotV - pCdat->totv;              //      ���³ɽ���

		 pCdat->totp=m_pStktype->m_fTotP;
		 pCdat->totv=m_pStktype->m_fTotV;

		 pCdat->rvol=m_pStktype->m_fTotRVol;                        //����
		 pCdat->dvol=m_pStktype->m_fTotRVol;                        //����

		 pCdat->m_Kdata1[m_pDoc->m_nANT[0]].Price=pCdat->nowp;
		 pCdat->m_Kdata1[m_pDoc->m_nANT[0]].Volume=pCdat->totv;     //    �� 
		 pCdat->m_Kdata1[m_pDoc->m_nANT[0]].Amount=pCdat->totp;     //    ��

		 if(pCdat->nowp>pCdat->higp||pCdat->higp==0)
            pCdat->higp=pCdat->nowp;
		 if(pCdat->nowp<pCdat->lowp||pCdat->lowp==0)
            pCdat->lowp=pCdat->nowp;
	 }
	 m_pStockTypeHead->m_lLastTime=m_pDoc->m_nANT[0];
}
//���ܣ�����ʵʱ���ָ��
//�����������
//�����������
void CManageBlockData::CalcBlockKlineData()       //����ʵʱ���ָ��
{
	int i;
	 for(int t = m_pDoc->m_nBlockCalcLastMin;t < m_pDoc->m_nANT[0];t++) //����ÿ����ʵʱ���ָ��
	 {
		 ProcMessage();
		 if (m_pDoc->m_nBlockCalcLastMin == 0)
		 {
			 for(i=0;i<BLOCKTYPE_NUM;i++)       //��ʼ��һ���Ӱ��ָ������
			 {
				if(m_pBlockInfo[i].m_lIsUse==-1 )
					break;
				if(m_pBlockInfo[i].m_bIsDelete)
					continue;


				m_pBlockInfo[i].m_fNewIndex=0;
				m_pBlockInfo[i].m_fTotP=0;         //   ���³ɽ����  
				m_pBlockInfo[i].m_fTotV=0;         //   ���³ɽ���
				m_pBlockInfo[i].m_fTotalRight=0;
			 }
		 }
		 for(i=0;i<m_pDoc->m_sharesInformation.GetTotalStockTypeCount();i++)  //����ÿһ��Ʊ�����ֵ
		 {
			 if(!CheckKind( i))
				 continue;
			 for(int j=0;j<m_pDoc->m_sharesInformation.GetStockTypeCount(i);j++)
			 {
				 ProcMessage();
				CReportData *pCdat;
				int index=0;
				m_pDoc->m_sharesInformation.GetStockItem(i,j,pCdat);
				if(pCdat==NULL||pCdat->pBlockInfo==NULL)
					continue;
				while(pCdat->pBlockInfo->m_nBlockType[index] != BLOCKTYPE_NUM && index < BLOCKTYPE_NUM)  //����
				{
					
					if (pCdat->pBlockInfo->m_ClosePrice <= 0.0)
					{
						CTaiKlineFileKLine::GetFirstKline(pCdat->id,pCdat->kind,TRUE,1 ,&pCdat->pBlockInfo->m_ClosePrice);
					}

					 int l_StockTyep=pCdat->pBlockInfo->m_nBlockType[index];
					 switch(m_pBlockInfo[l_StockTyep].m_iRightType)
					 {
						 case ZGB: if(pCdat->m_Kdata1[t].Price!=0)
								   {
									  m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->pBlockInfo->m_fRight[ZGB] * pCdat->m_Kdata1[t].Price; 
								      m_pBlockInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;         //   ���³ɽ����  
								      m_pBlockInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;         //   ���³ɽ���
								      m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice*pCdat->pBlockInfo->m_fRight[ZGB];
								   }
								   else if(pCdat->ystc!=0)
								   {
									  m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pCdat->ystc; 
								      m_pBlockInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;         //   ���³ɽ����  
								      m_pBlockInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;         //   ���³ɽ���
								      m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice*pCdat->pBlockInfo->m_fRight[ZGB];
								   }
								   else
								   {
                                      pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
									  m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pCdat->ystc; 
								      m_pBlockInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;         //   ���³ɽ����  
								      m_pBlockInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;         //   ���³ɽ���
								      m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice*pCdat->pBlockInfo->m_fRight[ZGB];

								   }
								   break; 
						 case LTG: if(pCdat->m_Kdata1[t].Price!=0)
								   {
									  m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->m_Kdata1[t].Price;
								      m_pBlockInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;         //   ���³ɽ����  
								      m_pBlockInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;         //   ���³ɽ���
								      m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice*pCdat->pBlockInfo->m_fRight[LTG];
								   }
								   else if(pCdat->ystc!=0)
								   {
									  m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->ystc; 
								      m_pBlockInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;         //   ���³ɽ����  
								      m_pBlockInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;         //   ���³ɽ���
								      m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice*pCdat->pBlockInfo->m_fRight[LTG];
								   }
								   else
								   {
                                      pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
									  m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pCdat->ystc; 
								      m_pBlockInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;         //   ���³ɽ����  
								      m_pBlockInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;         //   ���³ɽ���
								      m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice*pCdat->pBlockInfo->m_fRight[LTG];

								   }
								   break; 
						 case OTHER: if(pCdat->m_Kdata1[t].Price!=0)
									 {
									   m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->m_Kdata1[t].Price;
									   m_pBlockInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;         //   ���³ɽ����  
									   m_pBlockInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;         //   ���³ɽ���
									   m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;
									 }
									 else if(pCdat->ystc!=0)
									 {
									   m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->ystc; 
									   m_pBlockInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;         //   ���³ɽ����  
									   m_pBlockInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;         //   ���³ɽ���
									   m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;
									 }
								   else
								   {
                                      pCdat->ystc=CTaiKlineFileKLine::GetLastClose(pCdat->id,pCdat->kind);
									   m_pBlockInfo[l_StockTyep].m_fNewIndex +=pCdat->ystc; 
									   m_pBlockInfo[l_StockTyep].m_fTotP +=pCdat->m_Kdata1[t].Amount;         //   ���³ɽ����  
									   m_pBlockInfo[l_StockTyep].m_fTotV +=pCdat->m_Kdata1[t].Volume;         //   ���³ɽ���
									   m_pBlockInfo[l_StockTyep].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;

								   }
								   break; 
					 }
					 index++;
				}
			 }
		 }
		 for(i=0;i<m_pDoc->m_sharesInformation.GetStockTypeCount(BLOCKINDEX);i++)  //����ÿ���ָ��
		 {
			 ProcMessage();
			 CReportData *pCdat;
			 PBLOCKINFO m_pStktype;
			 m_pDoc->m_sharesInformation.GetStockItem(BLOCKINDEX,i,pCdat);
			 if(pCdat==NULL)
				 continue;
			 GetBlockPoint(m_pStktype,pCdat->id);
			 if(m_pStktype->m_fTotalRight==0)
				continue;
			 if(pCdat->opnp <= 0 && pCdat->ystc <= 0)       
				 CalcBlockOpenClosePrice();

			 pCdat->m_Kdata1[t].Price=m_pStktype->m_fNewIndex/(/*m_pStktype->m_lStockCount **/ m_pStktype->m_fTotalRight) * 1000;
			 //pCdat->m_Kdata1[t].Price=m_pStktype->m_fNewIndex/m_pStktype->m_fTotalRight*1000;
			 pCdat->m_Kdata1[t].Volume=m_pStktype->m_fTotV;     //    �� 
			 pCdat->m_Kdata1[t].Amount=m_pStktype->m_fTotP;     //    ��

			 if(t==0)
                pCdat->higp=pCdat->lowp=pCdat->m_Kdata1[t].Price;
             else
			 {
				 if(pCdat->m_Kdata1[t].Price>pCdat->higp)
					pCdat->higp=pCdat->m_Kdata1[t].Price;
				 if(pCdat->m_Kdata1[t].Price<pCdat->lowp)
					pCdat->lowp=pCdat->m_Kdata1[t].Price;
			 }
		 }
	 }
	 m_pDoc->m_nBlockCalcLastMin = m_pDoc->m_nANT[0];
}
void CManageBlockData::CheckStockCount(int AddStockCount)
{
	if(m_pStockTypeHead->m_lStockCount+AddStockCount>=m_pStockTypeHead->m_lStockMaxCount)//����ʵ�ʹ�Ʊ�������ɱ����Ʊ��С
	{
          AddBlockDataSize(AddStockCount+50);
	}

}
//BLOCKINFO test;

//���ܣ��Զ��������ݿⱣ��ռ䣬���ڴ��������Ʊ����
//�����������
//�����������
BOOL CManageBlockData::AddBlockDataSize(int AddSizes)
{
    BYTE *temp;
	if(m_pStockTypeHead->m_lStockCount+AddSizes>=m_pStockTypeHead->m_lStockMaxCount)//����ʵ�ʹ�Ʊ�������ɱ����Ʊ��С
	{
	   long m_lBufferLength=0;
       m_lBufferLength=sizeof(BLOCKFILEHEAD)+BLOCKTYPE_NUM*sizeof(BLOCKINFO)+(m_pStockTypeHead->m_lStockCount+AddSizes)*sizeof(STOCKINFOINBLOCK)+1024*2;
       m_pStockTypeHead->m_lStockMaxCount =m_pStockTypeHead->m_lStockCount+AddSizes; 
       SavePosToFile();                  //����λ��
	   if(m_pbtData)
		 UnmapViewOfFile(m_pbtData);
	   if(m_hFileMap) 
		 CloseHandle(m_hFileMap);
	   m_hFileMap=CreateFileMapping(m_hFile,     //�ؽ����ڴ�ӳ���ļ�
			NULL,
			PAGE_READWRITE,
			0,
			m_lBufferLength,
			NULL);
	 		if(m_hFileMap==NULL)
		{
			AfxMessageBox("�����ļ�ӳ���ں�ʱ����");
			CloseHandle(m_hFile);
			m_hFile=NULL;
			m_hFileMap=NULL;
			return FALSE;
		}
		m_pbtData=(PBYTE)MapViewOfFile(m_hFileMap,
			FILE_MAP_WRITE,
			0,0,0);
		if(m_pbtData==NULL)
		{
			AfxMessageBox("���ļ�����ӳ�����ڴ�ʱ����");
			CloseHandle(m_hFile);
			CloseHandle(m_hFileMap);
			m_hFile=NULL;
			m_hFileMap=NULL;
			return FALSE;
		}
        m_pStockTypeHead=(BLOCKFILEHEAD *)m_pbtData;   //�س�ʼ������

		temp=m_pbtData+sizeof(BLOCKFILEHEAD);

		m_pBlockInfo=(BLOCKINFO *)temp;           //������ݽṹָ��
		temp  +=sizeof(BLOCKINFO)*BLOCKTYPE_NUM;

		//for test
		//test = m_pBlockInfo[192];
 
		m_pStockInfo=(STOCKINFOINBLOCK *)temp;             //����й�Ʊ���ݽṹָ�� 

		if(!SetMemoryAllocSize(m_pStockTypeHead->m_lStockMaxCount))
		{
			AfxMessageBox("��ʼ�����ݱ�������");
			return FALSE;
		} 
		for(int j=0;j<m_pStockTypeHead->m_lStockCount;j++)
		{
			this->InsertItemPoint(m_pStockInfo+j);
		}

        this->InitStockPoint();
	}
	SaveDataToFile(m_pStockTypeHead,0);
	return TRUE;
}
//���ܣ�����ȫ����Ʊ���ݽṹָ��
//���������nSizeΪ�����ڴ��С
//���������TRUE ��ʾ�����ڴ�ɹ�
BOOL CManageBlockData::SetMemoryAllocSize(unsigned int nSize)
{
	STOCKPOINTINFO *tempData;
	int temp=0;
	HGLOBAL	hMem;
	LPVOID hp;
	if(!m_pStockPoint)//����Ƿ�Ϊ��һ�η����ڴ�
	{
		hMem = GlobalAlloc( GPTR, (nSize )* sizeof( STOCKPOINTINFO) );
		hp=GlobalLock(hMem);
		if( hp )
		{
			m_pStockPoint= (STOCKPOINTINFO *)hp;
		}
		else
		{
			AfxMessageBox("�����ڴ����",MB_ICONSTOP);
			return FALSE;
		}
	}
	else
	{
		hMem = GlobalAlloc( GPTR, (nSize )* sizeof(STOCKPOINTINFO) );
		hp=GlobalLock(hMem);
		if( hp )
		{
			tempData= (STOCKPOINTINFO *)hp;
		}
		else
		{
			AfxMessageBox("�����ڴ����",MB_ICONSTOP);
			return FALSE;
		}
		//����ԭ�����ݽṹָ��
        memcpy(tempData,m_pStockPoint,sizeof(STOCKPOINTINFO)*m_pStockTypeHead->m_lStockCount);
		GlobalUnlock((HGLOBAL)m_pStockPoint);        //�ͷ��ڴ�
		GlobalFree( (HGLOBAL)m_pStockPoint);
		m_pStockPoint=tempData;
	}
	return TRUE;
}
//���ܣ�����Ʊ�������ݿ��У������ع�Ʊ����λ��
//���������m_pStkΪ���ع�Ʊ�ṹָ��
//���������TRUE ��ʾ�ɹ�����
BOOL CManageBlockData::InsertItemPoint(STOCKINFOINBLOCK *m_pStk)               //����һ����Ʊ
{
	int InsertPose=m_pStk->m_iPos; 
    m_pStockPoint[InsertPose].pStockInfo=m_pStk;
	return TRUE;
}
//���ܣ��������й�Ʊ���ݴ���λ��
//�����������
//�����������
BOOL CManageBlockData::SavePosToFile()
{
	 for(int j=0;j<m_pStockTypeHead->m_lStockCount;j++)
	 {
		  if(m_pStockPoint[j].pStockInfo==NULL)
		  {
			  return FALSE;
		  }
          m_pStockPoint[j].pStockInfo->m_iPos=j; //��¼��Ʊλ��
	 }
	 return TRUE;
}


//***************************************************
//��鲿�ִ���
//***************************************************
//���ܣ���ȫ������в����µİ������
//���������m_pStktypeΪ��������İ�����, m_iPosΪ������λ��
//���������TRUE��ʾ����ɹ�
BOOL CManageBlockData::InsertBlock(PBLOCKINFO &m_pStktype,int m_iPos)
{
	if(m_iPos>=BLOCKTYPE_NUM)
	{
	    AfxMessageBox("�����°��λ�ò���ȷ!");
		return FALSE;
	}
	if(m_pBlockInfo[m_iPos].m_bIsDelete||m_pBlockInfo[m_iPos].m_lIsUse==-1 )
	{ 
        m_pStktype=m_pBlockInfo+m_iPos;      //�õ����ṹָ��
		m_pStockTypeHead->m_lStockTypeCount++;
	}
	else
	{
	    AfxMessageBox("�����°��λ��ʱ��ԭ���û��ɾ��������û�гɹ�!");
		return FALSE;
	}
    m_pStktype->m_fNewIndex=0;
    m_pStktype->m_fTotP=0;         //   ���³ɽ����  
    m_pStktype->m_fTotV=0;         //   ���³ɽ���
	m_pStktype->m_fYstIndex=0;
	m_pStktype->m_fOpenIndex=0;

	SaveDataToFile(m_pStockTypeHead,0);
	return TRUE;
}
//���ܣ��õ�ĳһ���ȫ����Ʊ��
//���������m_pszCodeΪ������
//������������ظð���Ʊ����
int CManageBlockData::GetStockCountFromBlockType(char *m_pszCode)
{
	UINT m_btPos=GetBlockPos(m_pszCode) ;
	return m_pBlockInfo[m_btPos].m_lStockCount;
}
//���ܣ���ȫ�������ɾ��ĳһ��� 
//���������m_pszStockTypeCodeΪ��ɾ��������
//���������TRUE ��ʾ�ɹ�ȡ���ṹָ�� 
BOOL CManageBlockData::DeleteBlock(char *m_pszStockTypeCode)                                 
{
	UINT m_btPos=GetBlockPos(m_pszStockTypeCode);
	//for(int i=0;i<m_pStockTypeHead->m_lStockTypeCount;i++)
	for(int i=0;i<m_pStockTypeHead->m_lStockCount;i++)
	{
	   int index=0;
	   BOOL m_bIsFind=FALSE;
       //���ȫ���ð���Ʊ
       while(m_pStockInfo[i].m_nBlockType[index]!=BLOCKTYPE_NUM && index<BLOCKTYPE_NUM && !m_bIsFind)
	   {
          if(m_pStockInfo[i].m_nBlockType[index]==m_btPos)//ɾ����Ʊ����
		  {
             do
			 {
                m_pStockInfo[i].m_nBlockType[index]=m_pStockInfo[i].m_nBlockType[index+1];
                index++; 
			 }while(m_pStockInfo[i].m_nBlockType[index]!=BLOCKTYPE_NUM && index<BLOCKTYPE_NUM);
			 m_bIsFind=TRUE;
             break;
		  }
		  index++;
	   };
	}
    m_pBlockInfo[m_btPos].m_bIsDelete=TRUE;
	CTaiKlineFileKLine::ZeroKlineData(m_pBlockInfo[m_btPos].m_szCode,BLOCKINDEX,TRUE);
	CTaiKlineFileKLine::ZeroKlineData(m_pBlockInfo[m_btPos].m_szCode,BLOCKINDEX,FALSE);
	SaveDataToFile(m_pStockTypeHead,0);
	return TRUE;
}
//���ܣ����ݰ����룬�õ��ð��ָ��ṹ 
//���������m_pStktypeΪ���ṹָ��,m_pszStockTypeCodeΪ������
//���������TRUE ��ʾ�ɹ�ȡ���ṹָ��
BOOL CManageBlockData::GetBlockPoint(PBLOCKINFO &m_pStktype,char *m_pszStockTypeCode)
{
	//BLOCKINFO test1;
	//for (int i = 0;i < BLOCKTYPE_NUM;i++)
	//{
	//	test1 = m_pBlockInfo[192];
	//	int iii = 0;
	//}

	UINT m_btPos = GetBlockPos(m_pszStockTypeCode) ;
	if (m_btPos < BLOCKTYPE_NUM)
	{
		m_pStktype=m_pBlockInfo+m_btPos;
		return TRUE;
	}
	m_pStktype = NULL;
	return FALSE;
}
//���ܣ��õ�Ҫ������λ��
//�����������
//������������ز�����λ��
int  CManageBlockData::GetInsertBlockPos()                                                
{
	for(int i=0;i<BLOCKTYPE_NUM;i++)
	{
		if(m_pBlockInfo[i].m_bIsDelete||m_pBlockInfo[i].m_lIsUse==-1 )
			return i;
	}
	AfxMessageBox("û�п����Ӱ��������ɾ��ĳЩ���ð����������°��!");
	return -1;
}
//���ܣ����ݰ�����ȡ�øð��ȫ����Ʊ���� 
//���������m_StockCodeArrayΪ����ȫ���ð���Ʊ����,m_pszStockTypeCodeΪ������
//���������TRUE Ϊ�ɹ�ȡ��
BOOL CManageBlockData::GetStockFromBlockCode(SymbolKindArr &m_StockCodeArray,char *m_pszStockTypeCode)          
{
	BLOCKINFO *l_pStockType;
	if(!GetBlockPoint(l_pStockType,m_pszStockTypeCode))
	{
		return FALSE;
	}

	//�Ƿ���Ҫ�����������Ʊ
	if (GetStockCountFromBlockType(l_pStockType->m_szCode) < 1)
	{
		CString strName;
		CStringArray stockName;
		strName.Format("%s",l_pStockType->m_szName);
		if (ImportBlockName(strName,stockName))
		{
			//if(m_pDoc->m_ManagerBlockData.CheckBlockName(strName.GetBuffer(0),NULL))
			//{
			//	continue ;
			//}
			//int l_nStockTypePos=m_pDoc->m_ManagerBlockData.GetInsertBlockPos();
			//char temp[10];
			//BLOCKINFO *l_pStockType;
			//if(!m_pDoc->m_ManagerBlockData.InsertBlock(l_pStockType,l_nStockTypePos))
			//{
			//	continue;
			//}
			//l_pStockType->m_lIsUse=TRUE;
			//l_pStockType->m_bIsDelete =FALSE;

			//strcpy(l_pStockType->m_szName,strName.GetBuffer(0));
			//strcpy(l_pStockType->m_szBlockTypeName,strBlockType);
			//l_pStockType->m_lStockCount=0;
			//l_pStockType->m_fTotalRight=0;
			//sprintf(temp,"%3d",l_nStockTypePos);
			//int i;
			//for(i=0;i<3;i++)
			//{
			//	if(temp[i]==' ')temp[i]='0';
			//}
			//sprintf(l_pStockType->m_szCode,"%s%s","8K0",temp);
			//l_pStockType->m_iRightType = 1;
			int tmp=stockName.GetCount();
			for(int i=0;i<tmp;i++)
			{
				CString strStockCode;
				CReportData *l_pCdat;
				int nKind;

				strStockCode = stockName[i].Right(6);
				if (stockName[i].Left(2) == "SH")
				{
					nKind = CSharesInformation::GetStockKind(SH_MARKET_EX,strStockCode.GetBuffer(0));
				}
				else
				{
					nKind = CSharesInformation::GetStockKind(SZ_MARKET_EX,strStockCode.GetBuffer(0));
				}
				if(m_pDoc->m_sharesInformation.Lookup(strStockCode.GetBuffer(0),l_pCdat,nKind))
				{
					if(l_pCdat->pBaseInfo!=NULL)
						m_pDoc->m_ManagerBlockData.InsertStockToBlock(l_pCdat,l_pStockType->m_szCode);
				}
			}
			//CReportData *pCdat;
			//m_pDoc->m_sharesInformation.InsertItem(l_pStockType->m_szCode,pCdat,BLOCKINDEX);
			//strcpy(pCdat->name ,l_pStockType->m_szName );
			//strcpy(pCdat->id ,l_pStockType->m_szCode );
			//pCdat->kind = BLOCKINDEX;
			//m_pDoc->m_sharesInformation.SaveRealDataToFile(pCdat,sizeof(CReportData));
			//if(strlen(pCdat->name)>0)
			//	m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;
			//m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
		}

	}

	UINT m_btPos=GetBlockPos(m_pszStockTypeCode);

    int index=0;
	for(int i=0;i<m_pStockTypeHead->m_lStockCount;i++)
	{
       int j=0;
	   if(m_pStockPoint[i].pStockInfo==NULL)
		   continue;
       while(m_pStockPoint[i].pStockInfo->m_nBlockType[j]!=BLOCKTYPE_NUM&&j<BLOCKTYPE_NUM)
	   {
          if(m_pStockPoint[i].pStockInfo->m_nBlockType[j]==m_btPos)  //����ù�Ʊ�Ƿ�Ϊ�ð���й�Ʊ
		  {
			 SymbolKind CodeStr;
			 CString m_strStockKind=m_pStockPoint[i].pStockInfo->m_szSymbol;
			 char m_szStockCode[10];
			 int nKind;
			 strcpy(m_szStockCode,m_strStockKind.Right(m_strStockKind.GetLength()-2).GetBuffer(0));
			 nKind=m_pDoc->GetStockKind(m_strStockKind.Left(2));
			 strcpy(CodeStr.m_chSymbol,m_szStockCode);
             CodeStr.m_nSymbolKind=nKind;
             m_StockCodeArray.Add(CodeStr);
	         index++;
             break;
		  }
		  j++;
	   };
	}
	return TRUE;
}
//���ܣ����ݰ�����ƣ��õ��ð��ȫ����Ʊ���룬
//���������m_StockCodeArrayΪ����ȫ���ð���Ʊ����,m_pszStockTypeNameΪ�������
//���������TRUE Ϊ�ɹ�ȡ��
BOOL CManageBlockData::GetStockFromBlockName(SymbolKindArr &m_StockCodeArray,char *m_pszStockTypeName)          
{
	BLOCKINFO *l_pStockType;

	if(!GetBlockPointFromName(l_pStockType,m_pszStockTypeName))
	{
		return FALSE;
	}

	char szCode[10];
	strcpy(szCode,l_pStockType->m_szCode );

	//�Ƿ���Ҫ�����������Ʊ
	if (GetStockCountFromBlockType(l_pStockType->m_szCode) < 1)
	{
		CString strName;
		CStringArray stockName;
		strName.Format("%s",m_pszStockTypeName);
		if (ImportBlockName(strName,stockName))
		{
			//if(m_pDoc->m_ManagerBlockData.CheckBlockName(strName.GetBuffer(0),NULL))
			//{
			//	continue ;
			//}
			//int l_nStockTypePos=m_pDoc->m_ManagerBlockData.GetInsertBlockPos();
			//char temp[10];
			//BLOCKINFO *l_pStockType;
			//if(!m_pDoc->m_ManagerBlockData.InsertBlock(l_pStockType,l_nStockTypePos))
			//{
			//	continue;
			//}
			//l_pStockType->m_lIsUse=TRUE;
			//l_pStockType->m_bIsDelete =FALSE;

			//strcpy(l_pStockType->m_szName,strName.GetBuffer(0));
			//strcpy(l_pStockType->m_szBlockTypeName,strBlockType);
			//l_pStockType->m_lStockCount=0;
			//l_pStockType->m_fTotalRight=0;
			//sprintf(temp,"%3d",l_nStockTypePos);
			//int i;
			//for(i=0;i<3;i++)
			//{
			//	if(temp[i]==' ')temp[i]='0';
			//}
			//sprintf(l_pStockType->m_szCode,"%s%s","8K0",temp);
			//l_pStockType->m_iRightType = 1;
			int tmp=stockName.GetCount();
			for(int i=0;i<tmp;i++)
			{
				CString strStockCode;
				CReportData *l_pCdat;
				int nKind;

				strStockCode = stockName[i].Right(6);
				if (stockName[i].Left(2) == "SH")
				{
					nKind = CSharesInformation::GetStockKind(SH_MARKET_EX,strStockCode.GetBuffer(0));
				}
				else
				{
					nKind = CSharesInformation::GetStockKind(SZ_MARKET_EX,strStockCode.GetBuffer(0));
				}
				if(m_pDoc->m_sharesInformation.Lookup(strStockCode.GetBuffer(0),l_pCdat,nKind))
				{
					//if(strcmp(l_pCdat->id , "000652") == 0)
					//	int iiii = 0;
					if(l_pCdat->pBaseInfo!=NULL)
						m_pDoc->m_ManagerBlockData.InsertStockToBlock(l_pCdat,szCode);
					//m_pDoc->m_ManagerBlockData.InsertStockToBlock(l_pCdat,l_pStockType->m_szCode);
				}
			}
			//CReportData *pCdat;
			//m_pDoc->m_sharesInformation.InsertItem(l_pStockType->m_szCode,pCdat,BLOCKINDEX);
			//strcpy(pCdat->name ,l_pStockType->m_szName );
			//strcpy(pCdat->id ,l_pStockType->m_szCode );
			//pCdat->kind = BLOCKINDEX;
			//m_pDoc->m_sharesInformation.SaveRealDataToFile(pCdat,sizeof(CReportData));
			//if(strlen(pCdat->name)>0)
			//	m_pDoc->StockNameConvert(pCdat->name,pCdat->Gppyjc) ;
			//m_pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
		}

	}
	UINT m_btPos = GetBlockPos(szCode);
    int index=0;
	for(int i=0;i<m_pStockTypeHead->m_lStockCount;i++)
	{
       int j=0;

       while((m_pStockPoint+i)!=NULL&&m_pStockPoint[i].pStockInfo->m_nBlockType[j]!=BLOCKTYPE_NUM&&j<BLOCKTYPE_NUM)
	   {
          if(m_pStockPoint[i].pStockInfo->m_nBlockType[j]==m_btPos)   //����ù�Ʊ�Ƿ�Ϊ�ð���й�Ʊ
		  {
			 SymbolKind CodeStr;
			 CString m_strStockKind=m_pStockPoint[i].pStockInfo->m_szSymbol;
			 char m_szStockCode[10];
			 int nKind;
			 strcpy(m_szStockCode,m_strStockKind.Right(m_strStockKind.GetLength()-2).GetBuffer(0));
			 nKind=m_pDoc->GetStockKind(m_strStockKind.Left(2));
			 strcpy(CodeStr.m_chSymbol,m_szStockCode);
			 CodeStr.m_nSymbolKind=nKind;
             m_StockCodeArray.Add(CodeStr);
	         index++;
             break;
		  }
		  j++;
	   };
	}
	return TRUE;
}
//���ܣ������Ʊ��ĳһ���
//���������m_pCdatΪ��������,m_pszStockTypeCodeΪ���ָ������
//���������TRUE ��ʾ����ɹ�
//char code[10];
BOOL CManageBlockData::InsertStockToBlock(PCdat1 &m_pCdat,char *m_pszStockTypeCode)        
{
	//strcpy(code,m_pszStockTypeCode);
    BLOCKINFO *m_pStktype = NULL;
	STOCKINFOINBLOCK *m_pStock = NULL;;
	int index=0;
	if(m_pCdat->pBaseInfo==NULL)
	{
		MessageBox(NULL,"�ڽ���Ʊ������ʱ,�ù�Ʊ�������ݲ���Ϊ��!","���ڲ�����ʾ",MB_OK);
		return FALSE;
	}
	if(!CheckKind(m_pCdat->kind))
	{
		MessageBox(NULL,"ָ���ཫ�������ӵ������!","���ڲ�����ʾ",MB_OK);
		return FALSE;
	}

	UINT m_btPos=GetBlockPos(m_pszStockTypeCode);             
    if(!InsertStockItem(m_pCdat->id ,m_pCdat->kind,m_pStock))    //�����Ʊ
	{
	     AfxMessageBox("�����Ʊʱ����ȷ! �������ӹ�Ʊ");
         return FALSE;
	}
	for(int i=0;m_pStock->m_nBlockType[i]!=BLOCKTYPE_NUM&&i<BLOCKTYPE_NUM;i++)  //��ʼ���������
	{
		if(m_pStock->m_nBlockType[i]==m_btPos)
			return TRUE;
	}
	while(m_pStock->m_nBlockType[index]!=BLOCKTYPE_NUM&&index<BLOCKTYPE_NUM)
	{
		index++;
	};
	//��ʼ����������ļ�
	m_pStock->m_nBlockType[index]=m_btPos;
	//if (m_btPos == 212)
	//{
	//	int ii = 0;
	//}
	m_pStock->m_bDeleted=FALSE;
	m_pStock->m_fRight[ZGB]=m_pCdat->pBaseInfo->zgb;
    if(m_pCdat->kind==SHBG||m_pCdat->kind==SZBG)
	  m_pStock->m_fRight[LTG]=m_pCdat->pBaseInfo->Bg;
    else
	  m_pStock->m_fRight[LTG]=m_pCdat->pBaseInfo->ltAg;

	CTaiKlineFileKLine::GetFirstKline(m_pCdat->id,m_pCdat->kind,TRUE,1 ,&m_pStock->m_ClosePrice);

    m_pCdat->pBlockInfo= m_pStock; 

	
	if(!GetBlockPoint( m_pStktype,m_pszStockTypeCode))    //���������Ƿ����
	{
	     AfxMessageBox("�����벻��ȷ! ���ܴӸð�������ӹ�Ʊ");
         return FALSE;
	}

    switch(m_pStktype->m_iRightType)         //��ʼ����������ļ�
	{
		  case ZGB:    m_pStktype->m_fTotalRight += m_pStock->m_fRight[ZGB] * m_pStock->m_ClosePrice;
					   m_pStktype->m_lStockCount++;
					   break;
		  case LTG:    m_pStktype->m_fTotalRight += m_pStock->m_fRight[LTG] * m_pStock->m_ClosePrice;
					   m_pStktype->m_lStockCount++;
					   break; 
		  case OTHER:  m_pStktype->m_fTotalRight += m_pStock->m_ClosePrice;
					   m_pStktype->m_lStockCount++;
					   break;
	}
	SaveDataToFile(m_pStockTypeHead,0);
    return TRUE;
}
//���ܣ���ĳһ���ɾ����Ʊ
//���������m_pszStockCodeΪ��ɾ����Ʊ����,m_pszStockTypeCodeΪ����ɾ����Ʊ�������
//���������TRUE ��ʾ�ɹ�ɾ��
BOOL CManageBlockData::DeleteStockFromBlock(char *m_pszStockCode,int nKind,char *m_pszStockTypeCode)
{
    BLOCKINFO *m_pStktype;
    UINT m_btPos=GetBlockPos(m_pszStockTypeCode);
	
	if(!GetBlockPoint( m_pStktype,m_pszStockTypeCode))   //���ð���Ʊ�Ƿ����
	{
	     AfxMessageBox("�����벻��ȷ! ���ܴӸð����ɾ����Ʊ");
         return FALSE;
	}
	STOCKINFOINBLOCK *m_pStock;
	if(Lookup(m_pszStockCode,nKind,m_pStock))    //�õ���ɾ����Ʊ����ṹָ��
	{
        int index=0;
		while(m_pStock->m_nBlockType[index]!=BLOCKTYPE_NUM&&index<BLOCKTYPE_NUM)
		{
			if(m_pStock->m_nBlockType[index]==m_btPos)  //�Ӱ����ɾ����Ʊ����
			{
				  switch(m_pStktype->m_iRightType)           //����Ȩ������
				  {
					  case ZGB:    m_pStktype->m_fTotalRight-=m_pStock->m_fRight[ZGB]*m_pStock->m_ClosePrice;
								   m_pStktype->m_lStockCount--;
								   break;
					  case LTG:    m_pStktype->m_fTotalRight-=m_pStock->m_fRight[LTG]*m_pStock->m_ClosePrice;
								   m_pStktype->m_lStockCount--;
								   break; 
					  case OTHER:  m_pStktype->m_fTotalRight-=m_pStock->m_ClosePrice;
								   m_pStktype->m_lStockCount--;
								   break;
				  }
				  do
				  {
					  m_pStock->m_nBlockType[index]=m_pStock->m_nBlockType[index+1];
					  index++; 
				  }while(m_pStock->m_nBlockType[index] != BLOCKTYPE_NUM && index < BLOCKTYPE_NUM);

				  m_pStock->m_nBlockType[index] = BLOCKTYPE_NUM;

				  if(index == 0)
                      m_pStock->m_bDeleted=TRUE;
				  break;
			}
			index++;
		};
	    SaveDataToFile(m_pStockTypeHead,0);
		return TRUE;
	}
    else
	{
	   return FALSE;
	}
}

//���ܣ��õ�ȫ�����ָ������ 
//���������m_StockTypeCodeArrayΪȫ�����ָ����������
//�����������
void CManageBlockData::GetAllBlockCodes(CStringArray &m_StockTypeCodeArray)
{
    for(int i=0;i<BLOCKTYPE_NUM;i++)
	{
		CString m_szTypeCode;
		if(this->m_pBlockInfo[i].m_lIsUse == -1)
			break;
		if(this->m_pBlockInfo[i].m_bIsDelete || strcmp(m_pBlockInfo[i].m_szBlockTypeName, "��ѡ���") == 0)
			continue;
		m_szTypeCode = m_pBlockInfo[i].m_szCode;
        m_StockTypeCodeArray.Add(m_szTypeCode);
	}
}
int CManageBlockData::GetBlockTypeCounts()          //�Ӱ���еõ��������
{
	int index = 0;
    for(int i = 0;i< BLOCKTYPE_NUM;i++)
	{
		if(this->m_pBlockInfo[i].m_lIsUse == -1)
			break;
		if(this->m_pBlockInfo[i].m_bIsDelete || strcmp(m_pBlockInfo[i].m_szBlockTypeName, "��ѡ���") == 0)
			continue;
		index++;
	}
	return index;
}

//���ܣ��õ�ȫ�����ָ������
//���������m_StockTypeNameArrayΪ�õ�ȫ�����ָ������
//�����������
void CManageBlockData::GetAllBlockNames(CStringArray &m_StockTypeNameArray)
{
    for(int i=0;i<BLOCKTYPE_NUM;i++)
	{
		CString m_szTypeName;
		if(this->m_pBlockInfo[i].m_lIsUse==-1)
			break;
		if(this->m_pBlockInfo[i].m_bIsDelete || strcmp(m_pBlockInfo[i].m_szBlockTypeName, "��ѡ���") == 0)
			continue;
		m_szTypeName=m_pBlockInfo[i].m_szName;
        m_StockTypeNameArray.Add(m_szTypeName);
	}
}
//���ܣ����ݰ�����Ʒ��ذ��ṹָ��
//���������m_pStktypeΪ���ṹָ�룬m_pszStockTypeNameΪ�������
//���������TRUE ��ʾ�ɹ�ȡ������
BOOL CManageBlockData::GetBlockPointFromName(PBLOCKINFO &m_pStktype,char *m_pszStockTypeName)
{
    for(int i=0;i<BLOCKTYPE_NUM;i++)
	{
		CString m_szTypeName;
		if(this->m_pBlockInfo[i].m_lIsUse==-1)
			break;
		if(this->m_pBlockInfo[i].m_bIsDelete)
			continue;
		//�����Ƿ�Ϊ��ͬ�������
		if(strcmp(this->m_pBlockInfo[i].m_szName,m_pszStockTypeName)==0)
		{
			m_pStktype=&m_pBlockInfo[i];
			return TRUE;
		}
	}
	return FALSE;
}

//���ܣ�����������ļ�����ĳһ���
//���������m_szPathStrΪ�ļ����·��,m_szStockTyoeNameStrΪ���ɰ������,l_pStockkTypeΪ���ذ��ṹָ��,nRightTypeΪȨ������
//���������
BOOL CManageBlockData::ImportBlockData(CString m_szPathStr,CString m_szStockTyoeNameStr,PBLOCKINFO &l_pStockkType,int nRightType)
{
	//���ð���Ƿ����
    if(GetBlockPointFromName(l_pStockkType,m_szStockTyoeNameStr.GetBuffer(0)))
	{
       DeleteBlock(l_pStockkType->m_szCode);
	   CReportData *pCdat;
       if(m_pDoc->m_sharesInformation.Lookup(l_pStockkType->m_szCode,pCdat,BLOCKINDEX))
	   {
		   m_pDoc->DeleteStockFromKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
		   m_pDoc->m_sharesInformation.DeleteOneStockInfo(l_pStockkType->m_szCode,BLOCKINDEX);
	   }
	}
    int l_nStockTypePos=this->GetInsertBlockPos();
	char temp[10];
	//������
	if(!InsertBlock(l_pStockkType,l_nStockTypePos))
	{
		return FALSE;
	}
	//��ʼ���������
    l_pStockkType->m_lIsUse=TRUE;
	l_pStockkType->m_bIsDelete =FALSE;
	strcpy(l_pStockkType->m_szName,m_szStockTyoeNameStr);
	l_pStockkType->m_lStockCount=0;
	l_pStockkType->m_fTotalRight=0;
    sprintf(temp,"%3d",l_nStockTypePos);
	for(int i=0;i<3;i++)
	{
		if(temp[i]==' ')temp[i]='0';
	}
	sprintf(l_pStockkType->m_szCode,"%s%s","8K0",temp);
    l_pStockkType->m_iRightType=nRightType;

	int stocklength;
	char  blank[2]={' ',' '};  
	CFile stockfile;
	CString filepath;

    filepath.Format("%s%s%s",m_szPathStr.GetBuffer(0),"\\",m_szStockTyoeNameStr.GetBuffer(0));
   	if(stockfile.Open((LPCTSTR)filepath,CFile::modeReadWrite))
	{
		stockfile.Read(&stocklength,4);
		for(int j=0;j<stocklength;j++)
		{
			CReportData *l_pCdat;
			SharesSymbol *symbol=new SharesSymbol;
			stockfile.Read(symbol->sharesymbol,6);
            stockfile.Read(&symbol->nKind,4); 
			symbol->sharesymbol[6]='\0';
			stockfile.Read(&blank,2);
            if(m_pDoc->m_sharesInformation.Lookup(symbol->sharesymbol,l_pCdat,symbol->nKind))
			{
				//�����鵽��ʱ�������ݽṹ��
			   if(l_pCdat->pBaseInfo!=NULL)
				   InsertStockToBlock(l_pCdat,l_pStockkType->m_szCode);
			}
			delete symbol;
		}
		stockfile.Close();
	}
	return TRUE;
}
///////////////////////////////
//��Ʊ����
//////////////////////////////
//���ܣ����ڴ����Ҳ��Ʊ
//���������m_szStockIdΪ�Ҳ��Ʊ����,m_pStock Ϊ���ؽṹ����ָ��
//���������TRUE ��ʾ�ɹ�����
BOOL CManageBlockData::Lookup(char *m_szStockId,int nKind,PSTOCKINFOINBLOCK &m_pStock)                  
{
	int low=0;
	int high=m_pStockTypeHead->m_lStockCount -1 ;
	int mid=0;
    char m_szStockKind[10];
	CString m_strKind=m_pDoc->GetStockKindString(nKind);
	strcpy(m_szStockKind,m_strKind.GetBuffer(0));
	strcat(m_szStockKind,m_szStockId);
	//�Ҳ��Ʊ
    while(low <= high)
	{
		 mid=(low+high)/2;
		 if(strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol, m_szStockKind)>0) high=mid -1;
         else if(strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol , m_szStockKind)< 0 ) low=mid +1;
		 else 
		 {
			 m_pStock=m_pStockPoint[mid].pStockInfo ;
			 return TRUE ;
		 }
	}
    m_pStock=NULL; 
	return FALSE;
}
//���ܣ�����Ʊ���뵽����ṹ��
//���������m_szStockId Ϊ��Ʊ����,m_pStock ������󷵻ؽṹ����ָ��
//���������TRUE �ɹ�����
BOOL CManageBlockData::InsertStockItem(char *m_szStockId ,int nKind,PSTOCKINFOINBLOCK &m_pStock)        //����һ����Ʊ
{
	int low=0;
	int high=m_pStockTypeHead->m_lStockCount -1 ;
	int mid=0;
	int InsertPose=-1;
    char m_szStockKind[10];
	CString m_strKind=m_pDoc->GetStockKindString(nKind);
	strcpy(m_szStockKind,m_strKind.GetBuffer(0));
	strcat(m_szStockKind,m_szStockId);
	//�Ҳ�����Ʊλ��
    while(low <= high)
	{
		 mid=(low+high)/2;
	     if(low==high)
		 {
		    if(strcmp(m_szStockKind , m_pStockPoint[mid].pStockInfo->m_szSymbol )>0)
			{
               if(m_pStockTypeHead->m_lStockCount==0||m_pStockTypeHead->m_lStockCount==mid)
                  InsertPose=mid ;
			   else
                  InsertPose=mid +1;   
			   break;
			}
		    else if(strcmp(m_szStockKind , m_pStockPoint[mid].pStockInfo->m_szSymbol)<0)
			{
			   InsertPose=mid ;
			   break;
			}
			else
			{
				 m_pStock=m_pStockPoint[mid].pStockInfo ;
				 return TRUE ;
			}
		 }
 		 if(strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol , m_szStockKind)>0) high=mid -1;
         else if(strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol ,m_szStockKind)< 0 ) low=mid +1;
		 else 
		 {
             m_pStock=m_pStockPoint[mid].pStockInfo;
			 return TRUE ;
		 }
	}
	if(high<low)
		InsertPose=low;

    //������λ���Ƿ��ں���λ���ϣ��ڴ��Ƿ���Ч
	if(m_pStockTypeHead->m_lStockCount + 1 >m_pStockTypeHead->m_lStockMaxCount)  //�������й�Ʊ���Ƿ������󱣴���
	{
		AddBlockDataSize();    //�����ڴ�
	}
	//����λ���Ƿ�����󣬷����ƶ�λ��
	if(m_pStockTypeHead->m_lStockCount > InsertPose)  
	{
       STOCKPOINTINFO *ptemp;
	   HGLOBAL	hMem;
	   LPVOID hp;
	   hMem = GlobalAlloc( GPTR, (m_pStockTypeHead->m_lStockCount - InsertPose )* sizeof( STOCKPOINTINFO) );
	   hp=GlobalLock(hMem);
	   if(hp)
	   {
			ptemp= (STOCKPOINTINFO *)hp;
	   }
	   else
	   {
			AfxMessageBox("�����ڴ����",MB_ICONSTOP);
			return FALSE;
	   }
	   memcpy(ptemp,&m_pStockPoint[InsertPose],(m_pStockTypeHead->m_lStockCount - InsertPose)*sizeof(STOCKPOINTINFO));
	   memcpy(&m_pStockPoint[InsertPose+1],ptemp,(m_pStockTypeHead->m_lStockCount - InsertPose)*sizeof(STOCKPOINTINFO));
       GlobalUnlock((HGLOBAL)ptemp);        //�ͷ��ڴ�
	   GlobalFree( (HGLOBAL)ptemp);
    }
	//���������
    m_pStockPoint[InsertPose].pStockInfo=m_pStockInfo+m_pStockTypeHead->m_lStockCount;
	strcpy(m_pStockPoint[InsertPose].pStockInfo->m_szSymbol,m_szStockKind);
    //memset(&m_pStockPoint[InsertPose].pStockInfo->m_nBlockType[0],BLOCKTYPE_NUM,sizeof(int)*BLOCKTYPE_NUM);
	for (int i = 0;i < BLOCKTYPE_NUM;i++)
	{
		m_pStockPoint[InsertPose].pStockInfo->m_nBlockType[i] = BLOCKTYPE_NUM;
	}
	m_pStockPoint[InsertPose].pStockInfo->m_bDeleted=FALSE;
	m_pStockTypeHead->m_lStockCount++;	

    m_pStock=m_pStockPoint[InsertPose].pStockInfo ;
	SaveDataToFile(m_pStockTypeHead,0);

	return TRUE;
}
//���ܣ�����Ʊ���ڴ���ɾ��
//���������m_szStockIdΪɾ����Ʊ����
//���������TRUE��ʾ�ɹ�����
BOOL CManageBlockData::DeleteStockItem(char *m_szStockId,int nKind)  	//ɾ����Ʊ
{
	int low=0;
	int high=m_pStockTypeHead->m_lStockCount-1 ;
	int mid=0;
	int DeletePos=-1;
    char m_szStockKind[10];
	CString m_strKind=m_pDoc->GetStockKindString(nKind);
	strcpy(m_szStockKind,m_strKind.GetBuffer(0));
	strcat(m_szStockKind,m_szStockId);
	//�Ҳ齫Ҫɾ����Ʊ
    while(low <= high)
	{
		 mid=(low+high)/2;
		 if(strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol , m_szStockKind)>0) high=mid -1;
         else if(strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol  , m_szStockKind)< 0 ) low=mid +1;
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
	//ɾ����Ʊ
	memmove(&m_pStockPoint[DeletePos],&m_pStockPoint[DeletePos +1],(m_pStockTypeHead->m_lStockCount - DeletePos -1 )*sizeof(STOCKPOINTINFO));
    m_pStockTypeHead->m_lStockCount--;
	SaveDataToFile(m_pStockTypeHead,0);
	return TRUE;
}
//���ܣ������ݿ���ɾ��ȫ�����ù�Ʊ
//�����������
//�����������
BOOL CManageBlockData::RemoveDeletedStock()
{
	BOOL result=TRUE;
	int index=m_pStockTypeHead->m_lStockCount;
	for(int j=0;j<index;j++)
	{
		//������ݿ��й�Ʊ�Ƿ�Ϊ���ù�Ʊ
         if((this->m_pStockInfo+j)->m_nBlockType[0]==BLOCKTYPE_NUM&&(this->m_pStockInfo+j)->m_nBlockType[CHOOSEPOS]==BLOCKTYPE_NUM) 
		 {

			 CString m_strStockKind=(this->m_pStockInfo+j)->m_szSymbol;
			 char m_szStockCode[10];
			 int nKind;
			 strcpy(m_szStockCode,m_strStockKind.Right(m_strStockKind.GetLength()-2).GetBuffer(0));
			 nKind=m_pDoc->GetStockKind(m_strStockKind.Left(2));

             DeleteStockItem(m_szStockCode,nKind);
		 }
	}
	if(!this->SavePosToFile())
        result=FALSE;
	if(index!=m_pStockTypeHead->m_lStockCount)
	for(int j=0;j<index;j++)
	{
         if((this->m_pStockInfo+j)->m_nBlockType[0]==BLOCKTYPE_NUM&&(this->m_pStockInfo+j)->m_nBlockType[CHOOSEPOS]==BLOCKTYPE_NUM)
		 {
			 //�����ݿ�ɾ����Ʊ
			 memmove(this->m_pStockInfo+j,this->m_pStockInfo+index -1 ,sizeof(STOCKINFOINBLOCK));
             index--;
			 j--;
		 }
	}
	SaveDataToFile(m_pStockTypeHead,0);
    return result;    
}
//���ܣ�����һ����Ʊ����ѡ�ɣ������浽��ѡ����
//���������m_szStockIdΪ��ѡ�ɴ���
//���������TRUE��ʾ������ѡ�ɳɹ�
BOOL CManageBlockData::InsertStockToChoose(char *m_szStockId ,int nKind,CString strTypeName)        //����һ����Ʊ����ѡ��
{
	PSTOCKINFOINBLOCK m_pStock;
	if (strTypeName.IsEmpty())
	{
		strTypeName = "��ѡ���";
	}
	InsertStockItem(m_szStockId ,nKind,m_pStock);
	m_pStock->m_bDeleted=FALSE;
	//strcpy(m_pStock->m_szSymbol,strTypeName.GetBuffer(0));
	m_pStock->m_nBlockType[CHOOSEPOS]=CHOOSEPOS;
	SaveDataToFile(m_pStockTypeHead,0);
	return TRUE;
}
//���ܣ������ݿ���ɾ����ѡ��
//���������m_szStockIdΪɾ����Ʊ����
//���������TRUE��ʾɾ����ѡ�ɳɹ�
BOOL CManageBlockData::DeleteStockFromChoose(char *m_szStockId,int nKind)  	//ɾ����Ʊ����ѡ��
{
	PSTOCKINFOINBLOCK m_pStock = NULL;
    Lookup(m_szStockId,nKind,m_pStock);
	if (!m_pStock)
	{
		return FALSE;
	}
	m_pStock->m_nBlockType[CHOOSEPOS]=BLOCKTYPE_NUM;
	SaveDataToFile(m_pStockTypeHead,0);

	return TRUE;
}
int CManageBlockData::GetChooseStockCounts()	//�õ���ѡ��
{
	int index=0;
	for(int i=0;i<BLOCKTYPE_NUM;i++)
	{
		if(this->m_pBlockInfo[i].m_lIsUse==-1)
			break;
		if(this->m_pBlockInfo[i].m_bIsDelete || strcmp(m_pBlockInfo[i].m_szBlockTypeName, "��ѡ���") != 0)
			continue;
		index++;
	}
	return index;
}
BOOL   CManageBlockData::GetChooseStockTypeNames(CStringArray& chooseType)
{
	for(int i=0;i<BLOCKTYPE_NUM;i++)
	{
		CString m_szTypeCode;
		if(this->m_pBlockInfo[i].m_lIsUse==-1)
			break;
		if(this->m_pBlockInfo[i].m_bIsDelete || strcmp(m_pBlockInfo[i].m_szBlockTypeName, "��ѡ���") != 0)
			continue;
		m_szTypeCode=m_pBlockInfo[i].m_szName;
		chooseType.Add(m_szTypeCode);
	}

	return TRUE;

}
BOOL   CManageBlockData::GetChooseStockTypeCodes(CStringArray& chooseCode)
{
	for(int i=0;i<BLOCKTYPE_NUM;i++)
	{
		CString m_szTypeCode;
		if(this->m_pBlockInfo[i].m_lIsUse==-1)
			break;
		if(this->m_pBlockInfo[i].m_bIsDelete || strcmp(m_pBlockInfo[i].m_szBlockTypeName, "��ѡ���") != 0)
			continue;
		m_szTypeCode=m_pBlockInfo[i].m_szCode;
		chooseCode.Add(m_szTypeCode);
	}

	return TRUE;

}
BOOL  CManageBlockData::GetChooseStockCodeFromTypeName(SymbolKindArr &m_StockCodeArray,CString strTypeName)
{
	for(int i=0;i<m_pStockTypeHead->m_lStockCount;i++)
	{
		if(m_pStockPoint[i].pStockInfo==NULL)
			continue;

		if( m_pStockPoint[i].pStockInfo->m_nBlockType[CHOOSEPOS]==CHOOSEPOS)// &&
			//strcmp(m_pStockPoint[i].pStockInfo->m_szChooseName, strTypeName.GetBuffer(0))) //�����Ƿ�Ϊ��ѡ��
		{
			SymbolKind CodeStr;
			CString m_strStockKind=m_pStockPoint[i].pStockInfo->m_szSymbol;
			char m_szStockCode[10];
			int nKind;
			strcpy(m_szStockCode,m_strStockKind.Right(m_strStockKind.GetLength()-2).GetBuffer(0));
			nKind=m_pDoc->GetStockKind(m_strStockKind.Left(2));

			strcpy(CodeStr.m_chSymbol,m_szStockCode);
			CodeStr.m_nSymbolKind= nKind;
			m_StockCodeArray.Add(CodeStr);
			continue;
		}
	}
	return TRUE;
}

//���ܣ������ݿ��еõ���ѡ��
//���������m_StockCodeArrayΪ�õ���ѡ������
//���������TRUE��ʾ�ɹ�ȡ������
BOOL CManageBlockData::GetChooseStockCode(SymbolKindArr &m_StockCodeArray)          
{
	for(int i=0;i<m_pStockTypeHead->m_lStockCount;i++)
	{
		if(m_pStockPoint[i].pStockInfo==NULL)
			continue;

		if( m_pStockPoint[i].pStockInfo->m_nBlockType[CHOOSEPOS]==CHOOSEPOS)// &&
			//strcmp(m_pStockPoint[i].pStockInfo->m_szChooseName, strTypeName.GetBuffer(0))) //�����Ƿ�Ϊ��ѡ��
		{
			 SymbolKind CodeStr;
			 CString m_strStockKind=m_pStockPoint[i].pStockInfo->m_szSymbol;
			 char m_szStockCode[10];
			 int nKind;
			 strcpy(m_szStockCode,m_strStockKind.Right(m_strStockKind.GetLength()-2).GetBuffer(0));
			 nKind=m_pDoc->GetStockKind(m_strStockKind.Left(2));

			 strcpy(CodeStr.m_chSymbol,m_szStockCode);
             CodeStr.m_nSymbolKind= nKind;
             m_StockCodeArray.Add(CodeStr);
             continue;
		}
	}
	return TRUE;
}
//****************************************************
BOOL CManageBlockData::GetStockTypeStartDate(char * pszStockTypeCode,SymbolKindArr& StockCodeArray,time_t &t)
{
	int FirstKlineData=0;
	if(!GetStockFromBlockCode(StockCodeArray,pszStockTypeCode))          
	{
       AfxMessageBox("�ð�鲻����") ;
	   return FALSE;
	}
    int StockCount=StockCodeArray.GetSize();
	for(int i=0;i<StockCount;i++)
	{
		SymbolKind StockSymbol=StockCodeArray.GetAt(i);
        FirstKlineData=CTaiKlineFileKLine::GetFirstKline(StockSymbol.m_chSymbol,StockSymbol.m_nSymbolKind ,TRUE);
		if(i==0)
            t= FirstKlineData;
		if(FirstKlineData!=-1&&t>FirstKlineData)
            t= FirstKlineData;
	}
    return TRUE;    
}
void CManageBlockData::RefreshHistoryDayLineData(char * pszStockTypeCode,CProgressCtrl *pProgressCtrl)
{
	typedef struct
	{
		time_t m_date;
		float m_fOpenIndex;
		float m_fHighIndex;    //   �������
		float m_fLowIndex;     //   �������
		float m_fCloseIndex;     //   ����ָ��
		float m_fTotV;         //   ���³ɽ���
		float m_fTotP;         //   ���³ɽ����
		float m_fTotalRight;
        long ldate;
	}StockTypeInfoTemp;
	MSG message;
    time_t t;
    SymbolKindArr StockCodeArray; 
	BLOCKINFO *m_pStktype;
	CTimeSpan  lDaySpan(1, 0,0, 0 );
	
	if(!GetStockTypeStartDate(pszStockTypeCode,StockCodeArray,t))
		return;
	
	GetBlockPoint(m_pStktype,pszStockTypeCode);
	
	t+=lDaySpan.GetTotalSeconds();
//********************************************
	Kline *pKline=NULL;
	int nTotalCount;
	CBuySellList *pBuySellList=NULL;
	CString SymbolCode;
	int nRead=0;
	CTime *tmEnd=NULL;
	CTime *tmBegin=NULL;
    int nKlineType=5;
	time_t now; 
	time( &now );

    SymbolCode=CSharesCompute::GetIndexSymbol(SHZS);
	tmEnd=new CTime(now);
    tmBegin=new CTime(t);

    nTotalCount=CTaiKlineFileKLine::ReadKLineAny(SymbolCode,SHZS,pKline, nRead,nKlineType, pBuySellList, 48,TRUE,  tmEnd,tmBegin);
	nTotalCount=CorrectKline(pKline,nTotalCount);
	if(nTotalCount<0||nTotalCount<-1)
	{
		if(tmEnd) delete tmEnd;
		if(tmBegin) delete tmBegin;
		return;
	}
//****************************************************************************************
	StockTypeInfoTemp *pStockTypeInfoTemp=NULL;
	HGLOBAL hMem=GlobalAlloc(  GPTR, sizeof(StockTypeInfoTemp)*nTotalCount);
	if(hMem!=NULL)
	   pStockTypeInfoTemp=(StockTypeInfoTemp *)GlobalLock(  hMem );
	else
	{
		if(tmEnd) delete tmEnd;
		if(tmBegin) delete tmBegin;
		AfxMessageBox("�ڴ�������!");
		return;
	}
	int i;
    for(i=0;i<nTotalCount;i++)
	{
        pStockTypeInfoTemp[i].m_date=pKline[i].day;
        pStockTypeInfoTemp[i].ldate=m_pDoc->GetStockDay(pKline[i].day);
	}
	if(pKline)
	{
		delete pKline;        //�ͷ��ڴ�
		pKline=NULL;
	}
	delete tmEnd;
	delete tmBegin;
//**************************************************************
   if(pProgressCtrl!=NULL)
   {
      pProgressCtrl->SetRange(0,StockCodeArray.GetSize());
	  pProgressCtrl->SetPos(0);
   }
   for(int j=0;j<StockCodeArray.GetSize();j++)
   {
      CReportData *pCdat;
	  Kline *pStockKline=NULL;
	  SymbolKind StockSymbol=StockCodeArray.GetAt(j);
	  long StockTotalCount=0;
      if(!m_pDoc->m_sharesInformation.Lookup(StockSymbol.m_chSymbol,pCdat,StockSymbol.m_nSymbolKind))
		  continue;
	  if(pCdat->pBlockInfo==NULL)
		  continue;
      StockTotalCount=CTaiKlineFileKLine::ReadKLineAny(StockSymbol.m_chSymbol,StockSymbol.m_nSymbolKind,pStockKline, -1,5, NULL, 48,TRUE,  NULL,NULL);
	  StockTotalCount=CorrectKline(pStockKline,StockTotalCount);
	   
      if(StockTotalCount<=0)
		  continue; 
	  int index=0;
	  for(i=1;i<StockTotalCount&&index<nTotalCount;i++,index++)
	  {
          long lDate;
          if(i==1)
		  {
		     lDate=m_pDoc->GetStockDay(pStockKline[i].day);
			 if(lDate>pStockTypeInfoTemp[index].ldate)
               while(index<nTotalCount&&lDate>pStockTypeInfoTemp[index].ldate) index++;
			 else if(lDate<pStockTypeInfoTemp[index].ldate)
			 {
               while(i<StockTotalCount&&lDate<pStockTypeInfoTemp[index].ldate)
			   {
		          lDate=m_pDoc->GetStockDay(pStockKline[i].day);
				  i++;
			   }
			 }
			 if(i>=StockTotalCount-1||index>=nTotalCount)
                continue;
		  }
		  else
		  {
		     lDate=m_pDoc->GetStockDay(pStockKline[i].day);
			 while(i<StockTotalCount&&pStockTypeInfoTemp[index].ldate>lDate)  //����������
			 {
		        lDate=m_pDoc->GetStockDay(pStockKline[i].day);
				i++;
			 }
             while(index<nTotalCount&&pStockTypeInfoTemp[index].ldate<lDate)        //��Ʊ������
			 {
				 switch(m_pStktype->m_iRightType)
				 {
					 case ZGB: 
							   pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i-1].open;
							   pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i-1].high;
							   pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i-1].low; 
							   pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i-1].close;
							   pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->zgb*pCdat->pBlockInfo->m_ClosePrice;
							   break; 
					 case LTG: 
							   pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i-1].open;
							   pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i-1].high;
							   pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i-1].low; 
							   pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i-1].close;
							   if(pCdat->kind==SHBG||pCdat->kind==SZBG)
							      pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->Bg*pCdat->pBlockInfo->m_ClosePrice;
							   else
							      pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->ltAg*pCdat->pBlockInfo->m_ClosePrice;
							   break; 
					 case OTHER:
							   pStockTypeInfoTemp[index].m_fOpenIndex +=pStockKline[i-1].open;
							   pStockTypeInfoTemp[index].m_fHighIndex +=pStockKline[i-1].high;
							   pStockTypeInfoTemp[index].m_fLowIndex +=pStockKline[i-1].low; 
							   pStockTypeInfoTemp[index].m_fCloseIndex +=pStockKline[i-1].close;
							   pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;

							   break; 
				 }
				pStockTypeInfoTemp[index].m_fTotP +=pStockKline[i-1].amount; 
				pStockTypeInfoTemp[index].m_fTotV +=pStockKline[i-1].vol;    
                index++; 
			 } 
		  }
		 if(i>=StockTotalCount||index>=nTotalCount)
			 continue;
		 if(pStockTypeInfoTemp[index].ldate!=lDate)
		 {
			 index--;
			 i--;
			 continue;
		 }
		 switch(m_pStktype->m_iRightType)
		 {
			 case ZGB: 
					   pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i].open;
					   pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i].high;
					   pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i].low; 
					   pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i].close;
					   pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->zgb*pCdat->pBlockInfo->m_ClosePrice;
                       break; 
			 case LTG: 
					   pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i].open;
					   pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i].high;
					   pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i].low; 
					   pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i].close;
					   if(CTaiShanReportView::IsBg( pCdat->kind))
						  pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->Bg*pCdat->pBlockInfo->m_ClosePrice;
					   else
						  pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->ltAg*pCdat->pBlockInfo->m_ClosePrice;
                       break; 
			 case OTHER:
					   pStockTypeInfoTemp[index].m_fOpenIndex +=pStockKline[i].open;
					   pStockTypeInfoTemp[index].m_fHighIndex +=pStockKline[i].high;
					   pStockTypeInfoTemp[index].m_fLowIndex +=pStockKline[i].low; 
					   pStockTypeInfoTemp[index].m_fCloseIndex +=pStockKline[i].close;
					   pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;
                       break; 
		 }
		pStockTypeInfoTemp[index].m_fTotP +=pStockKline[i].amount; 
		pStockTypeInfoTemp[index].m_fTotV +=pStockKline[i].vol;    
	  }
//*******************************************************************************
      if(pStockKline == NULL || pStockKline + StockTotalCount - 1 == NULL)
		  continue;
	  long lDate;

	  lDate=m_pDoc->GetStockDay(pStockKline[StockTotalCount-1].day);
      while(index<nTotalCount&&lDate<=pStockTypeInfoTemp[index].ldate)        //��Ʊ������
	  {
		 switch(m_pStktype->m_iRightType)
		 {
			 case ZGB: 
					   pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[StockTotalCount-1].open;
					   pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[StockTotalCount-1].high;
					   pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[StockTotalCount-1].low; 
					   pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[StockTotalCount-1].close;
					   pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->zgb*pCdat->pBlockInfo->m_ClosePrice;
					   break; 
			 case LTG: 
					   pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[StockTotalCount-1].open;
					   pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[StockTotalCount-1].high;
					   pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[StockTotalCount-1].low; 
					   pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[StockTotalCount-1].close;
					   if(CTaiShanReportView::IsBg( pCdat->kind))
						  pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->Bg*pCdat->pBlockInfo->m_ClosePrice;
					   else
						  pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->ltAg*pCdat->pBlockInfo->m_ClosePrice;
					   break; 
			 case OTHER:
					   pStockTypeInfoTemp[index].m_fOpenIndex +=pStockKline[StockTotalCount-1].open;
					   pStockTypeInfoTemp[index].m_fHighIndex +=pStockKline[StockTotalCount-1].high;
					   pStockTypeInfoTemp[index].m_fLowIndex +=pStockKline[StockTotalCount-1].low; 
					   pStockTypeInfoTemp[index].m_fCloseIndex +=pStockKline[StockTotalCount-1].close;
					   pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;

					   break; 
		 }
		 pStockTypeInfoTemp[index].m_fTotP +=pStockKline[StockTotalCount-1].amount; 
		 pStockTypeInfoTemp[index].m_fTotV +=pStockKline[StockTotalCount-1].vol;    
         index++;
	  }
//*******************************************************************************
		
	  if(pStockKline)
		  delete pStockKline;

	  if(pProgressCtrl!=NULL)
	  {
		  pProgressCtrl->SetPos(j);
	  }
	
	  if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
	  {
		  TranslateMessage(&message);
		  DispatchMessage(&message);
	  }
  }
  if(pProgressCtrl!=NULL)
  {
	  pProgressCtrl->SetPos(0);
  }

	hMem=GlobalAlloc(  GPTR, sizeof(Kline)*nTotalCount);
	if(hMem!=NULL)
	   pKline=(Kline *)GlobalLock(  hMem );
	else
	{
		AfxMessageBox("�ڴ�������!");
		return;
	}
    for(i=0;i<nTotalCount;i++)
	{
       pKline[i].day=pStockTypeInfoTemp[i].m_date;
       pKline[i].open=pStockTypeInfoTemp[i].m_fOpenIndex/pStockTypeInfoTemp[i].m_fTotalRight*1000;  
       pKline[i].high=pStockTypeInfoTemp[i].m_fHighIndex/pStockTypeInfoTemp[i].m_fTotalRight*1000;  
	   pKline[i].low=pStockTypeInfoTemp[i].m_fLowIndex/pStockTypeInfoTemp[i].m_fTotalRight*1000;  
	   pKline[i].close=pStockTypeInfoTemp[i].m_fCloseIndex/pStockTypeInfoTemp[i].m_fTotalRight*1000;  
	   pKline[i].amount=pStockTypeInfoTemp[i].m_fTotP;
	   pKline[i].vol=pStockTypeInfoTemp[i].m_fTotV;
	}
	CString strStockTypeCode=pszStockTypeCode;
	CTaiKlineFileKLine::ZeroKlineData(strStockTypeCode,BLOCKINDEX,TRUE);

	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer (strStockTypeCode,BLOCKINDEX,true);
	pFile ->WriteKLine(strStockTypeCode,pKline,nTotalCount,CTaiKlineFileKLine::overWriteAll);
//**************************************************************
	if(pKline)
	{
        GlobalUnlock((HGLOBAL)pKline);
        GlobalFree((HGLOBAL)pKline);
		pKline=NULL;
	}
	if(pStockTypeInfoTemp)
	{
        GlobalUnlock((HGLOBAL)pStockTypeInfoTemp);
        GlobalFree((HGLOBAL)pStockTypeInfoTemp);
	}
}

void CManageBlockData::RefreshHistoryMin5LineData(char * pszStockTypeCode,CProgressCtrl *pProgressCtrl)
{
	typedef struct
	{
		time_t m_date;
		float m_fOpenIndex;
		float m_fHighIndex;    //   �������
		float m_fLowIndex;     //   �������
		float m_fCloseIndex;     //   ����ָ��
		float m_fTotV;         //   ���³ɽ���
		float m_fTotP;         //   ���³ɽ����
		float m_fTotalRight;
		__int64 ldate;
	}StockTypeInfoTemp;

	MSG message;
	time_t t;
	SymbolKindArr StockCodeArray; 
	BLOCKINFO *m_pStktype;
	CTimeSpan  lDaySpan(0, 0,5, 0 );

	if(!GetStockTypeStartDate(pszStockTypeCode,StockCodeArray,t))
		return;

	GetBlockPoint(m_pStktype,pszStockTypeCode);

	t+=lDaySpan.GetTotalSeconds();
	//********************************************
	Kline *pKline=NULL;
	int nTotalCount;
	CBuySellList *pBuySellList=NULL;
	CString SymbolCode;
	int nRead=0;
	CTime *tmEnd=NULL;
	CTime *tmBegin=NULL;
	int nKlineType=1;
	time_t now; 
	time( &now );

	SymbolCode=CSharesCompute::GetIndexSymbol(SHZS);
	tmEnd=new CTime(now);
	tmBegin=new CTime(t);

	nTotalCount=CTaiKlineFileKLine::ReadKLineAny(SymbolCode,SHZS,pKline, nRead,nKlineType, pBuySellList, 48,TRUE,  tmEnd,tmBegin);
	nTotalCount=CorrectKline(pKline,nTotalCount);
	if(nTotalCount<0||nTotalCount<-1)
	{
		if(tmEnd) delete tmEnd;
		if(tmBegin) delete tmBegin;
		return;
	}
	//****************************************************************************************
	StockTypeInfoTemp *pStockTypeInfoTemp=NULL;
	HGLOBAL hMem=GlobalAlloc(  GPTR, sizeof(StockTypeInfoTemp)*nTotalCount);
	if(hMem!=NULL)
		pStockTypeInfoTemp=(StockTypeInfoTemp *)GlobalLock(  hMem );
	else
	{
		if(tmEnd) delete tmEnd;
		if(tmBegin) delete tmBegin;
		AfxMessageBox("�ڴ�������!");
		return;
	}
	int i;
	for(i=0;i<nTotalCount;i++)
	{
		pStockTypeInfoTemp[i].m_date=pKline[i].day;
		pStockTypeInfoTemp[i].ldate=m_pDoc->GetStockMinute(pKline[i].day);
	}
	if(pKline)
	{
		delete pKline;        //�ͷ��ڴ�
		pKline=NULL;
	}
	delete tmEnd;
	delete tmBegin;
	//**************************************************************
	if(pProgressCtrl!=NULL)
	{
		pProgressCtrl->SetRange(0,StockCodeArray.GetSize());
		pProgressCtrl->SetPos(0);
	}
	for(int j=0;j<StockCodeArray.GetSize();j++)
	{
		CReportData *pCdat;
		Kline *pStockKline=NULL;
		SymbolKind StockSymbol=StockCodeArray.GetAt(j);
		long StockTotalCount=0;
		if(!m_pDoc->m_sharesInformation.Lookup(StockSymbol.m_chSymbol,pCdat,StockSymbol.m_nSymbolKind))
			continue;
		if(pCdat->pBlockInfo==NULL)
			continue;
		StockTotalCount=CTaiKlineFileKLine::ReadKLineAny(StockSymbol.m_chSymbol,StockSymbol.m_nSymbolKind,pStockKline, -1,1, NULL, 48,TRUE,  NULL,NULL);
		StockTotalCount=CorrectKline(pStockKline,StockTotalCount);

		if(StockTotalCount<=0)
			continue; 
		int index=0;
		for(i=1;i<StockTotalCount&&index<nTotalCount;i++,index++)
		{
			__int64 lDate;
			if(i==1)
			{
				lDate=m_pDoc->GetStockMinute(pStockKline[i].day);
				if(lDate>pStockTypeInfoTemp[index].ldate)
				{
					while(index<nTotalCount&&lDate>pStockTypeInfoTemp[index].ldate) 
						index++;
				}	
				else if(lDate<pStockTypeInfoTemp[index].ldate)
				{
					 while(i<StockTotalCount&&lDate<pStockTypeInfoTemp[index].ldate)
					 {
						 lDate=m_pDoc->GetStockMinute(pStockKline[i].day);
						 i++;
					 }
			    }
				if(i>=StockTotalCount-1||index>=nTotalCount)
					continue;
			}
			else
			{
				lDate=m_pDoc->GetStockMinute(pStockKline[i].day);
				while(i<StockTotalCount&&pStockTypeInfoTemp[index].ldate>lDate)  //����������
				 {
					 lDate=m_pDoc->GetStockMinute(pStockKline[i].day);
					 i++;
				 }
				while(index<nTotalCount&&pStockTypeInfoTemp[index].ldate<lDate)        //��Ʊ������
				 {
					 switch(m_pStktype->m_iRightType)
					 {
					 case ZGB: 
						 pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i-1].open;
						 pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i-1].high;
						 pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i-1].low; 
						 pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i-1].close;
						 pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->zgb*pCdat->pBlockInfo->m_ClosePrice;
						 break; 
					 case LTG: 
						 pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i-1].open;
						 pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i-1].high;
						 pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i-1].low; 
						 pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i-1].close;
						 if(pCdat->kind==SHBG||pCdat->kind==SZBG)
							 pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->Bg*pCdat->pBlockInfo->m_ClosePrice;
						 else
							 pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->ltAg*pCdat->pBlockInfo->m_ClosePrice;
						 break; 
					 case OTHER:
						 pStockTypeInfoTemp[index].m_fOpenIndex +=pStockKline[i-1].open;
						 pStockTypeInfoTemp[index].m_fHighIndex +=pStockKline[i-1].high;
						 pStockTypeInfoTemp[index].m_fLowIndex +=pStockKline[i-1].low; 
						 pStockTypeInfoTemp[index].m_fCloseIndex +=pStockKline[i-1].close;
						 pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;

						 break; 
					 }
					 pStockTypeInfoTemp[index].m_fTotP +=pStockKline[i-1].amount; 
					 pStockTypeInfoTemp[index].m_fTotV +=pStockKline[i-1].vol;    
					 index++; 
				 } 
			}
			if(i>=StockTotalCount||index>=nTotalCount)
				continue;
			if(pStockTypeInfoTemp[index].ldate!=lDate)
			 {
				 index--;
				 i--;
				 continue;
			 }
			switch(m_pStktype->m_iRightType)
			 {
				case ZGB: 
					pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i].open;
					pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i].high;
					pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i].low; 
					pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[i].close;
					pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->zgb*pCdat->pBlockInfo->m_ClosePrice;
					break; 
				case LTG: 
					pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i].open;
					pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i].high;
					pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i].low; 
					pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[i].close;
					if(CTaiShanReportView::IsBg( pCdat->kind))
						pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->Bg*pCdat->pBlockInfo->m_ClosePrice;
					else
						pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->ltAg*pCdat->pBlockInfo->m_ClosePrice;
					break; 
				case OTHER:
					pStockTypeInfoTemp[index].m_fOpenIndex +=pStockKline[i].open;
					pStockTypeInfoTemp[index].m_fHighIndex +=pStockKline[i].high;
					pStockTypeInfoTemp[index].m_fLowIndex +=pStockKline[i].low; 
					pStockTypeInfoTemp[index].m_fCloseIndex +=pStockKline[i].close;
					pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;
					break; 
			 }
			pStockTypeInfoTemp[index].m_fTotP +=pStockKline[i].amount; 
			pStockTypeInfoTemp[index].m_fTotV +=pStockKline[i].vol;    
		}
		//*******************************************************************************
		if(pStockKline == NULL || pStockKline + StockTotalCount - 1 == NULL)
			continue;
		__int64 lDate;

		lDate=m_pDoc->GetStockMinute(pStockKline[StockTotalCount-1].day);
		while(index<nTotalCount&&lDate<=pStockTypeInfoTemp[index].ldate)        //��Ʊ������
		{
			switch(m_pStktype->m_iRightType)
			 {
				case ZGB: 
					pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[StockTotalCount-1].open;
					pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[StockTotalCount-1].high;
					pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[StockTotalCount-1].low; 
					pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pBlockInfo->m_fRight[ZGB]*pStockKline[StockTotalCount-1].close;
					pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->zgb*pCdat->pBlockInfo->m_ClosePrice;
					break; 
				case LTG: 
					pStockTypeInfoTemp[index].m_fOpenIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[StockTotalCount-1].open;
					pStockTypeInfoTemp[index].m_fHighIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[StockTotalCount-1].high;
					pStockTypeInfoTemp[index].m_fLowIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[StockTotalCount-1].low; 
					pStockTypeInfoTemp[index].m_fCloseIndex +=pCdat->pBlockInfo->m_fRight[LTG]*pStockKline[StockTotalCount-1].close;
					if(CTaiShanReportView::IsBg( pCdat->kind))
						pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->Bg*pCdat->pBlockInfo->m_ClosePrice;
					else
						pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBaseInfo->ltAg*pCdat->pBlockInfo->m_ClosePrice;
					break; 
				case OTHER:
					pStockTypeInfoTemp[index].m_fOpenIndex +=pStockKline[StockTotalCount-1].open;
					pStockTypeInfoTemp[index].m_fHighIndex +=pStockKline[StockTotalCount-1].high;
					pStockTypeInfoTemp[index].m_fLowIndex +=pStockKline[StockTotalCount-1].low; 
					pStockTypeInfoTemp[index].m_fCloseIndex +=pStockKline[StockTotalCount-1].close;
					pStockTypeInfoTemp[index].m_fTotalRight +=pCdat->pBlockInfo->m_ClosePrice;

					break; 
			 }
			pStockTypeInfoTemp[index].m_fTotP +=pStockKline[StockTotalCount-1].amount; 
			pStockTypeInfoTemp[index].m_fTotV +=pStockKline[StockTotalCount-1].vol;    
			index++;
		}
		//*******************************************************************************

		if(pStockKline)
			delete pStockKline;

		if(pProgressCtrl!=NULL)
		{
			pProgressCtrl->SetPos(j);
		}

		if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
	if(pProgressCtrl!=NULL)
	{
		pProgressCtrl->SetPos(0);
	}

	hMem=GlobalAlloc(  GPTR, sizeof(Kline)*nTotalCount);
	if(hMem!=NULL)
		pKline=(Kline *)GlobalLock(  hMem );
	else
	{
		AfxMessageBox("�ڴ�������!");
		return;
	}
	for(i=0;i<nTotalCount;i++)
	{
		pKline[i].day=pStockTypeInfoTemp[i].m_date;
		pKline[i].open=pStockTypeInfoTemp[i].m_fOpenIndex/pStockTypeInfoTemp[i].m_fTotalRight*1000;  
		pKline[i].high=pStockTypeInfoTemp[i].m_fHighIndex/pStockTypeInfoTemp[i].m_fTotalRight*1000;  
		pKline[i].low=pStockTypeInfoTemp[i].m_fLowIndex/pStockTypeInfoTemp[i].m_fTotalRight*1000;  
		pKline[i].close=pStockTypeInfoTemp[i].m_fCloseIndex/pStockTypeInfoTemp[i].m_fTotalRight*1000;  
		pKline[i].amount=pStockTypeInfoTemp[i].m_fTotP;
		pKline[i].vol=pStockTypeInfoTemp[i].m_fTotV;
	}
	CString strStockTypeCode=pszStockTypeCode;
	CTaiKlineFileKLine::ZeroKlineData(strStockTypeCode,BLOCKINDEX,FALSE);

	CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer (strStockTypeCode,BLOCKINDEX,false);
	pFile ->WriteKLine(strStockTypeCode,pKline,nTotalCount,CTaiKlineFileKLine::overWriteAll);
	//**************************************************************
	if(pKline)
	{
		GlobalUnlock((HGLOBAL)pKline);
		GlobalFree((HGLOBAL)pKline);
		pKline=NULL;
	}
	if(pStockTypeInfoTemp)
	{
		GlobalUnlock((HGLOBAL)pStockTypeInfoTemp);
		GlobalFree((HGLOBAL)pStockTypeInfoTemp);
	}
}


BOOL CManageBlockData::InsertStockItemCorrect(char *m_szStockId ,PSTOCKINFOINBLOCK m_pStock)        //����һ����Ʊ
{
	int low=0;
	int high=m_pStockTypeHead->m_lStockCount -1 ;
	int mid=0;
	int InsertPose=-1;
	//�Ҳ�����Ʊλ��
    while(low <= high)
	{
		 mid=(low+high)/2;
	     if(low==high)
		 {
		    if(strcmp(m_szStockId , m_pStockPoint[mid].pStockInfo->m_szSymbol )>0)
			{
               if(m_pStockTypeHead->m_lStockCount==0||m_pStockTypeHead->m_lStockCount==mid)
                  InsertPose=mid ;
			   else
                  InsertPose=mid +1;   
			   break;
			}
		    else if(strcmp(m_szStockId , m_pStockPoint[mid].pStockInfo->m_szSymbol)<0)
			{
			   InsertPose=mid ;
			   break;
			}
			else
			{
			 return TRUE ;
			}
		 }
 		 if(strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol , m_szStockId)>0) high=mid -1;
         else if(strcmp(m_pStockPoint[mid].pStockInfo->m_szSymbol ,m_szStockId)< 0 ) low=mid +1;
		 else 
		 {
			 return TRUE ;
		 }
	}
	if(high<low)
		InsertPose=low;

    //������λ���Ƿ��ں���λ���ϣ��ڴ��Ƿ���Ч
	if(m_pStockTypeHead->m_lStockCount + 1 >m_pStockTypeHead->m_lStockMaxCount)  //�������й�Ʊ���Ƿ������󱣴���
	{
		AddBlockDataSize();    //�����ڴ�
	}
	//����λ���Ƿ�����󣬷����ƶ�λ��
	if(m_pStockTypeHead->m_lStockCount > InsertPose)  
	{
       STOCKPOINTINFO *ptemp;
	   HGLOBAL	hMem;
	   LPVOID hp;
	   hMem = GlobalAlloc( GPTR, (m_pStockTypeHead->m_lStockCount - InsertPose )* sizeof( STOCKPOINTINFO) );
	   hp=GlobalLock(hMem);
	   if(hp)
	   {
			ptemp= (STOCKPOINTINFO *)hp;
	   }
	   else
	   {
			AfxMessageBox("�����ڴ����",MB_ICONSTOP);
			return FALSE;
	   }
	   memcpy(ptemp,&m_pStockPoint[InsertPose],(m_pStockTypeHead->m_lStockCount - InsertPose)*sizeof(STOCKPOINTINFO));
	   memcpy(&m_pStockPoint[InsertPose+1],ptemp,(m_pStockTypeHead->m_lStockCount - InsertPose)*sizeof(STOCKPOINTINFO));
       GlobalUnlock((HGLOBAL)ptemp);        //�ͷ��ڴ�
	   GlobalFree( (HGLOBAL)ptemp);
    }
	//���������
    m_pStockPoint[InsertPose].pStockInfo=m_pStock;
	strcpy(m_pStockPoint[InsertPose].pStockInfo->m_szSymbol,m_szStockId);
	m_pStockTypeHead->m_lStockCount++;	
	return TRUE;
}
long CManageBlockData::CorrectKline(Kline *pKline,long LineCount)
{
	long index=0;
	for(int i=0;i<LineCount-1;i++)
	{
        if(pKline[i].day<pKline[i+1].day)
		{   
			index++;
		}
		else
		{
			memmove(&pKline[i+1],&pKline[i+2],sizeof(Kline)*(LineCount-i-2));
			i--;
			LineCount--;
		}
	}
	return index+1;
}
float CManageBlockData::GetStockBlockRight(char *StockTypeCode)
{
	PBLOCKINFO m_pStktype;
	UINT m_btPos=GetBlockPos(StockTypeCode);
    GetBlockPoint(m_pStktype,StockTypeCode);
    int index=0;
	float TotalBlockRight=0;
	for(int i=0;i<m_pStockTypeHead->m_lStockCount;i++)
	{
       int j=0;
	   if(m_pStockPoint[i].pStockInfo==NULL)
		   continue;
       while(m_pStockPoint[i].pStockInfo->m_nBlockType[j]!=BLOCKTYPE_NUM&&j<BLOCKTYPE_NUM)
	   {
          if(m_pStockPoint[i].pStockInfo->m_nBlockType[j]==m_btPos)  //����ù�Ʊ�Ƿ�Ϊ�ð���й�Ʊ
		  {
			  CReportData *pCdat;
			  CString m_strStockKind=m_pStockPoint[i].pStockInfo->m_szSymbol;
			  char m_szStockCode[10];
			  int nKind;
			  strcpy(m_szStockCode,m_strStockKind.Right(m_strStockKind.GetLength()-2).GetBuffer(0));
			  nKind=m_pDoc->GetStockKind(m_strStockKind.Left(2));

			  if(this->m_pDoc->m_sharesInformation.Lookup(m_szStockCode,pCdat,nKind))
			  {
                  if(pCdat->pBaseInfo==NULL)
				  {
					  j++;
                      break;
				  }
                  switch(m_pStktype->m_iRightType)
				  {
				  case ZGB:  TotalBlockRight+=pCdat->pBaseInfo->zgb;
					         break;
				  case LTG:   if(CTaiShanReportView::IsBg( pCdat->kind))
					            TotalBlockRight+=pCdat->pBaseInfo->Bg;
					          else 
					            TotalBlockRight+=pCdat->pBaseInfo->ltAg;
					          break;
				  case OTHER: TotalBlockRight+=1;
					          break;
				  }
			  }
              break;
		  }
		  j++;
	   };
	}
   return TotalBlockRight;
}
BOOL CManageBlockData::CheckBlockName(char *BlockName,char *BlockCode)
{
    for(int i=0;i<BLOCKTYPE_NUM;i++)
	{
		CString m_szTypeName;
		if(m_pBlockInfo != NULL && m_pBlockInfo[i].m_lIsUse==-1)
			break;
		if(this->m_pBlockInfo[i].m_bIsDelete)
			continue;
		if(BlockCode==NULL)
		{
			if(strcmp(m_pBlockInfo[i].m_szName,BlockName)==0)
				return TRUE;
		}
		else
		{
			if(strcmp(m_pBlockInfo[i].m_szName,BlockName)==0&&strcmp(m_pBlockInfo[i].m_szCode,BlockCode)!=0)
				return TRUE;
		}
	}
	return FALSE;
}

bool CManageBlockData::CheckKind(int i)
{
	bool b = true;
	if(i==SHZS||i==SZZS)
		b = false;
	return b;
}
void CManageBlockData::GetAllBlockTypeName(CStringArray &BlockTypeNameArray)//�õ�ȫ������������
{
	for(int i=0;i<m_pStockTypeHead->m_lBlockTypeCount;i++)
	{
		CString m_szTypeName;
		BOOL bFind = FALSE;
		m_szTypeName = m_pStockTypeHead->m_szBlockTypeName[i];
		for (int j = 0;j < BlockTypeNameArray.GetCount();j++)
		{
			if (m_szTypeName == BlockTypeNameArray[j])
			{
				bFind = TRUE;
				break;
			}
		}
		if (!bFind)
		{
			BlockTypeNameArray.Add(m_szTypeName);
		}
	}

}
void CManageBlockData::GetAllBlockCodesFromBlockTypeName(CStringArray &BlockCodeArray,CString strBlockType)//���ݰ��������Ƶõ�ȫ����ذ��
{
	for(int i = 0;i< BLOCKTYPE_NUM;i++)
	{
		CString szBlockTypeName,szBlockCode;
		BOOL bFind = FALSE;
		szBlockTypeName = m_pBlockInfo[i].m_szBlockTypeName;
		szBlockCode = m_pBlockInfo[i].m_szCode;
		if (strBlockType == "ȫ�����" && !m_pBlockInfo[i].m_bIsDelete)
		{
			BlockCodeArray.Add(szBlockCode);
		}
		else if ((szBlockTypeName == strBlockType) && !m_pBlockInfo[i].m_bIsDelete)
		{
			BlockCodeArray.Add(szBlockCode);
		}

	}
}

void CManageBlockData::GetAllBlockNamesFromBlockTypeName(CStringArray &BlockNameArray,CString strBlockType)//���ݰ��������Ƶõ�ȫ����ذ��
{
	for(int i=0;i<BLOCKTYPE_NUM;i++)
	{
		CString szBlockTypeName,szBlockName;
		BOOL bFind = FALSE;
		szBlockTypeName=m_pBlockInfo[i].m_szBlockTypeName;
		szBlockName = m_pBlockInfo[i].m_szName;
		if (strBlockType == "ȫ�����" && !m_pBlockInfo[i].m_bIsDelete)
		{
			BlockNameArray.Add(szBlockName);
		}
		else if ((szBlockTypeName == strBlockType) && !m_pBlockInfo[i].m_bIsDelete)
		{
			BlockNameArray.Add(szBlockName);
		}

	}
}

UINT CManageBlockData::GetBlockCountFromBlockTypeName(CString strBlockType)//���ݰ��������Ƶõ�ȫ����ذ��
{
	UINT nCounter = 0;
	for(int i = 0;i < BLOCKTYPE_NUM;i++)
	{
		CString szBlockTypeName,szBlockName;
		szBlockTypeName=m_pBlockInfo[i].m_szBlockTypeName;
		szBlockName = m_pBlockInfo[i].m_szName;
		if (m_pBlockInfo[i].m_lIsUse == -1)
		{
			break;
		}
		if (strBlockType == "ȫ�����" && !m_pBlockInfo[i].m_bIsDelete)
		{
			nCounter++;
		}
		else if ((szBlockTypeName == strBlockType) && !m_pBlockInfo[i].m_bIsDelete)
		{
			nCounter++;
		}
	}
	return nCounter;
}

BOOL CManageBlockData::AddBlockType(CString strBlockTypeName)//����һ��������
{
	if(strBlockTypeName.GetLength() > 20)
		return FALSE;
	if (m_pStockTypeHead->m_lBlockTypeCount < BLOCKTYPE_NUM)
	{
		strcpy(m_pStockTypeHead->m_szBlockTypeName[m_pStockTypeHead->m_lBlockTypeCount],strBlockTypeName.GetBuffer(0));
		m_pStockTypeHead->m_lBlockTypeCount++;
		//m_pStockTypeHead->m_szBlockTypeName[m_pStockTypeHead->m_lBlockTypeCount][11] = '\0';
		SaveDataToFile(m_pStockTypeHead,sizeof(BLOCKFILEHEAD ));

		return TRUE;
	}
	return FALSE;
}

BOOL CManageBlockData::DelBlockType(CString strBlockTypeName)//ɾ��һ��������
{
	if(strBlockTypeName.GetLength() > 11)
		return FALSE;

	int iCount = m_pStockTypeHead->m_lBlockTypeCount;
	int iIndex = 0;
	BOOL bFind = FALSE;
	
	for (int i = 0;i < iCount;i++)
	{
		if (strcmp(m_pStockTypeHead->m_szBlockTypeName[i],strBlockTypeName.GetBuffer(0)) == 0)
		{
			bFind = TRUE;
			iIndex = i;
			break;
		}
	}
	if (bFind)
	{	//�ƶ��������
		for (int j = iIndex;j < iCount - 1;j++)
		{
			strcpy(m_pStockTypeHead->m_szBlockTypeName[j], m_pStockTypeHead->m_szBlockTypeName[j + 1]);
		}
		m_pStockTypeHead->m_lBlockTypeCount--;
		SaveDataToFile(m_pStockTypeHead,sizeof(BLOCKFILEHEAD ));
		//ɾ����صİ��
		for(int i = 0;i < BLOCKTYPE_NUM;i++)
		{
			BOOL bFind = FALSE;
			if (strcmp(m_pBlockInfo[i].m_szBlockTypeName,strBlockTypeName.GetBuffer(0)) == 0)
			{
				DeleteBlock(m_pBlockInfo->m_szCode);
				CReportData *pCdat = NULL;
				m_pDoc->m_sharesInformation.Lookup(m_pBlockInfo->m_szCode,pCdat,BLOCKINDEX);         //ɾ������Ʊ����
				if (pCdat)
				{
					m_pDoc->DeleteStockFromKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
					m_pDoc->m_sharesInformation.DeleteOneStockInfo(m_pBlockInfo->m_szCode,BLOCKINDEX);
				}
			}

		}

	}
	return TRUE;

}

BOOL CManageBlockData::ImportBlockTypeName(CStringArray& BlockTypeName)
{
	CString strFile,strBlockType,strBlock,strLine;

	strFile.Format("%s%s",g_blockdir,"Block.def");
	CStdioFile file;

	if (file.Open(strFile,CFile::modeRead|CFile::typeText))
	{
		while (file.ReadString(strLine))
		{
			strLine.Trim();
			BlockTypeName.Add(strLine);
		}
		return TRUE;
	}
	return FALSE;
}
BOOL CManageBlockData::ImportBlockName(CString BlockName,CStringArray& StockArray)
{
	CString strFile,strBlockType,strBlock,strLine;
	char cLine[12];
	strFile.Format("%s%s.blk",g_blockdir,BlockName.GetBuffer(0));
	CFile file;

	//if(BlockName == "��֤��ָ")
	//	int ii = 0;
	if (file.Open(strFile,CFile::modeRead))
	{
		file.Read(cLine,4);
		while (file.Read(cLine,12))
		{
			strLine.Format("%s",cLine);
			strLine.Trim();
			StockArray.Add(strLine);
		}
		return TRUE;
	}
	return FALSE;
}
void CManageBlockData::UpdateAllBlockStocks()
{
	CString strBlockType,strBlock;
	BLOCKINFO *l_pStockType;
	CStringArray BlockTypeNameArray;
	CStringArray BlockCodeArray;

	GetAllBlockTypeName(BlockTypeNameArray);//�õ�ȫ������������
	for (int i = 0;i < BlockTypeNameArray.GetCount();i++)
	{
		BlockCodeArray.RemoveAll();
		strBlockType = BlockTypeNameArray.GetAt(i);
		GetAllBlockNamesFromBlockTypeName(BlockCodeArray,strBlockType);//���ݰ��������Ƶõ�ȫ����ذ��
		for (int j = 0;i < BlockCodeArray.GetCount();j++)
		{
			strBlock = BlockCodeArray.GetAt(j);
			if(!GetBlockPointFromName(l_pStockType,strBlock.GetBuffer(0)))
			{
				return ;
			}

			char szCode[10];
			strcpy(szCode,l_pStockType->m_szCode );

			//�Ƿ���Ҫ�����������Ʊ
			if (GetStockCountFromBlockType(l_pStockType->m_szCode) < 1)
			{
				CString strName;
				CStringArray stockName;
				strName.Format("%s",strBlock);
				if (ImportBlockName(strName,stockName))
				{
					int tmp=stockName.GetCount();
					for(int i=0;i<tmp;i++)
					{
						CString strStockCode;
						CReportData *l_pCdat;
						int nKind;

						strStockCode = stockName[i].Right(6);
						if (stockName[i].Left(2) == "SH")
						{
							nKind = CSharesInformation::GetStockKind(SH_MARKET_EX,strStockCode.GetBuffer(0));
						}
						else
						{
							nKind = CSharesInformation::GetStockKind(SZ_MARKET_EX,strStockCode.GetBuffer(0));
						}
						if(m_pDoc->m_sharesInformation.Lookup(strStockCode.GetBuffer(0),l_pCdat,nKind))
						{
							if(l_pCdat->pBaseInfo!=NULL)
								m_pDoc->m_ManagerBlockData.InsertStockToBlock(l_pCdat,szCode);
							//m_pDoc->m_ManagerBlockData.InsertStockToBlock(l_pCdat,l_pStockType->m_szCode);
						}
					}
				}
			}
		}
	}
}

BOOL  CManageBlockData::RecvBlockDataForBlockType(PBLOCKINFO &pData,CString strBlockTypeName) //��һ����ת��Ϊ����ʾ���
{
	if(m_hFile == NULL || m_hFileMap == NULL)
	{
		return FALSE;
	}
	int nCount = 0;
	for(int i = 0;i < BLOCKTYPE_NUM;i++)
	{
		if (m_pBlockInfo[i].m_lIsUse == -1)
		{
			break;
		}
		if (strBlockTypeName == "ȫ�����" && !m_pBlockInfo[i].m_bIsDelete)
		{
			memcpy(pData + nCount,&m_pBlockInfo[i],sizeof(BLOCKINFO));
			nCount++;
		}
		else if(strcmp(m_pBlockInfo[i].m_szBlockTypeName,strBlockTypeName.GetBuffer()) == 0 && !m_pBlockInfo[i].m_bIsDelete)
		{
			memcpy(pData + nCount,&m_pBlockInfo[i],sizeof(BLOCKINFO));
			nCount++;			
		}
		strBlockTypeName.ReleaseBuffer();
	}

	return TRUE;
}