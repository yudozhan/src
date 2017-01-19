// CheckData.cpp: implementation of the CCheckData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WH.h"
#include "CheckData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define DLINEHEAD  24
#define DLINESMALLHEAD 4096 * 64
#define DLINECHUQ   32 * 6


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCheckData::CCheckData()
{

}

CCheckData::~CCheckData()
{

}


BOOL CCheckData::OverlapBlock_SH() 
{
	// TODO: Add your control notification handler code here
	return 1;

}

BOOL CCheckData::OverlapBlock_SZ() 
{
	// TODO: Add your control notification handler code here
	return 1;
}

BOOL CCheckData::OverlapSymbol_SH() 				
{
	// TODO: Add your control notification handler code here
	ASSERT(FALSE);
	//LMB 1208 DELETE
	/*
	FILE *fp;
    char *Buff;
	int i;
	FILE *fplog=NULL;
	struct KLINESMALLHEAD
	{
		char    StockSign[8]; //ǰ8���ֽ�Ϊ��Ʊ���롣
		short 	numChuQ	;	  //2���ֽ�Ϊ��Ȩ���ݵ��ܴ���
		long    numKln	;	  //4���ֽ����ͣ�������K�ߵĸ�����
		short   symBlock[25]; //����ÿ�����ֽڴ���K�����ݵĿ��
		long    index;
	};
	struct KLINEHEAD
	{
		int	NumChuQPerBlk;	    	//ÿС��Ĺ�Ʊ��Ȩ���ݵ�����	
		int	numPerBlk	;			//ÿС��Ĺ�ƱK����������,256��
		int	numKline_sh	;			//��֤֤ȯ����
		int	numBlkAll_sh;			//��֤С�������
		int numKline_sz	;	        //��֤֤ȯ����
		int numBlkAll_sz;	        //��֤С������
	};

    KLINESMALLHEAD *m_KLineSmallHead;                      //���������ļ���ͷ�ṹ
    m_KLineSmallHead=new  KLINESMALLHEAD;
    KLINEHEAD m_KLineHead;
    CMapStringToString temp;

	HGLOBAL	hMem = GlobalAlloc( GPTR, (2000)* sizeof( char) ) ;
	LPVOID hp=GlobalLock(hMem);
    if( hp )
	    Buff= (char *)hp;
	else
	    AfxMessageBox("can't alloc Buffers") ; 

	fp=fopen("data\\shanghai\\day.dat","r+b");
	fseek(fp,4,SEEK_SET);
	fread(&m_KLineHead.numPerBlk,1,4,fp);
    fseek(fp,4,SEEK_CUR);
	fread(&m_KLineHead.numKline_sh,1,4,fp);
	fread(&m_KLineHead.numBlkAll_sh,1,4,fp);
	m_KLineHead.NumChuQPerBlk =6*32;
	fseek(fp,24,SEEK_SET);
  	for(i=0;i<m_KLineHead.numKline_sh;i++)                     //��������ģ�� 
	{
		CString m_zqdm;
		int addr1=DLINEHEAD +i*64;
		fseek(fp,addr1,SEEK_SET);
		fread(m_KLineSmallHead->StockSign ,8,1,fp);
		fread(&m_KLineSmallHead->numChuQ ,2,1,fp);
		fread(&m_KLineSmallHead->numKln  ,4,1,fp);
		fread(m_KLineSmallHead->symBlock   ,2,25,fp);
        m_zqdm= m_KLineSmallHead->StockSign;
        m_KLineSmallHead->index=i;
//**********************************************************************
        CString zqdm;
		zqdm=m_KLineSmallHead->StockSign;
	    if(temp.Lookup(zqdm,zqdm)==FALSE )
              temp[zqdm]=zqdm;
		else
			return 0;
	}
	fclose(fp);
	fclose(fplog);
	delete m_KLineSmallHead;
*/
	return 1;
	
}

BOOL CCheckData::OverlapSymbol_SZ() 				
{
	// TODO: Add your control notification handler code here

	return 1;
}


BOOL CCheckData::MaxBlock_SH() 
{
	////////////////////////////////////////////////
	return 0;
	
}

BOOL CCheckData::MaxBlock_SZ() 
{
	////////////////////////////////////////////////
	return 0;
	
}
