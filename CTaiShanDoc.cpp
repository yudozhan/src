// CTaiShanDoc.cpp : implementation of the CTaiShanDoc class
//
#include "stdafx.h"
#include <afxmt.h>
#include <stdio.h>
#include <share.h>
#include  <io.h>
#include "resource.h"
#include "DownLoadDlg.h"
#include "WideDownLoadDlg.h"
#include "MyTreeView.h"
#include "WH.h"
#include "MainFrm.h"
#include "ProgressDialog.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"
#include "BGridCtrl.h"
#include "CSharesBigBillDlg.h"
#include "CTaiShanKlineShowView.h"
#include "DiaCopy.h"
#include "StructTaiShares.h"
#include "CTaiShanDoc.h"
#include "ReorganizeDayLineData.h"
#include "CheckData.h"
#include "CTaiKlineDoKline.h"
#include "CTaiKlineDrawLine.h"
#include "CTaiKlineTransferKline.h"
#include "CTaiKlineMemFile.h"
#include "CTaiKlineFileKLine.h"
#include "CTaiKlineFileHS.h"
#include "CAlertSystem.h"
#include "CTaiShanTesting.h"
#include "GetSetReg.h"
#define ID_GPHQXS     5003
#define SYSTEMHEAD    1000

extern CTime g_timeUseEnd;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define IDD_DIALOG_PROGRESS             130

#define MLINEHEAD  16
#define MLINESMALLHEAD 20
#define MLINEBLK   48*25*3
#define MLINEBLKBYTECOUNT 48*25*3*32     

#ifdef __cplusplus
extern "C" {
  long ReadDog( void );
  long WriteDog( void );
  short DogAddr,DogBytes;
  void * DogData;
  }
#else
long ReadDog( void );
long WriteDog( void );
short DogAddr,DogBytes;
void * DogData;
#endif

#define MAXRAISE 3.0
/////////////////////////////////////////////////////////////////////////////
BLOCKSTOCK::BLOCKSTOCK(CString string )
{
	this->blockname=string;

}
void BLOCKSTOCK::AddStockToTail(SharesSymbol* sharesymbol)
{
	this->stocklist.AddTail(sharesymbol);
}
void BLOCKSTOCK::AddStockToHead(SharesSymbol* sharesymbol)
{
	this->stocklist.AddHead(sharesymbol);
}
SharesSymbol* BLOCKSTOCK::GetAt(POSITION position)
{   SharesSymbol* result;
	result=this->stocklist.GetAt(position);
	return result;

}

/////////////////////////////////////////////////////////////////////////////
// CTaiShanDoc

IMPLEMENT_DYNCREATE(CTaiShanDoc, CDocument)

BEGIN_MESSAGE_MAP(CTaiShanDoc, CDocument)
	//{{AFX_MSG_MAP(CTaiShanDoc)
	ON_COMMAND(ID_OPEN_DOCUMENT, OnOpenDocument)
	ON_COMMAND(ID_TOOL_CLOSEWORK, OnToolClosework)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiShanDoc construction/destruction
BOOL CTaiShanDoc::m_bAppAuthorized = TRUE;
//robj\objcontent.obj robj\objcalc.obj robj\objsecret.obj 
//dobj\objcontent.obj dobj\objcalc.obj dobj\objsecret.obj 
CTaiShanDoc::CTaiShanDoc()
{
	m_pBlockAnalysisView = NULL;

	HGLOBAL	hMem;
	LPVOID hp;
	STOCKDATASHOW  *ptempStockShow;;
	hMem = GlobalAlloc( GPTR, 4 * sizeof( STOCKDATASHOW) );
	hp = GlobalLock(hMem);
	if( hp )
	{
		m_pStatisticData =(STOCKDATASHOW *)hp;
	}
	else
	{
		AfxMessageBox("�����ڴ����",MB_ICONSTOP);
	}
	char* name[4] = {"ƽ��ֵ","��  ��","���ֵ","��Сֵ"};
	for(int i = 0;i < 4;i++)
	{
		hMem = GlobalAlloc( GPTR, sizeof( CReportData) );
		hp = GlobalLock(hMem);
		if( hp )
		{
			m_pStatisticData[i].pItem = (CReportData *)hp;
		}
		else
		{
			AfxMessageBox("�����ڴ����",MB_ICONSTOP);
			return ;
		}

		strcpy(m_pStatisticData[i].pItem->name,name[i]);

		hMem = GlobalAlloc( GPTR, sizeof( BASEINFO) );
		hp = GlobalLock(hMem);
		if( hp )
		{
			m_pStatisticData[i].pItem->pBaseInfo = (BASEINFO *)hp;
		}
		else
		{
			AfxMessageBox("�����ڴ����",MB_ICONSTOP);
			return ;
		}

		hMem = GlobalAlloc( GPTR, sizeof( STOCKINFOINBLOCK) );
		hp = GlobalLock(hMem);
		if( hp )
		{
			m_pStatisticData[i].pItem->pBlockInfo = (STOCKINFOINBLOCK *)hp;
		}
		else
		{
			AfxMessageBox("�����ڴ����",MB_ICONSTOP);
			return ;
		}	
	}

	for(int i = 0;i < 4;i++)
		m_bStatistic[i] = false;

	m_nWeightType = 0;
	m_bStatisticed = false;


	m_TodayDate = 0;

	m_sShF10 = _T("");
	m_sSzF10 = _T("");
	m_sNews = _T("");
	m_nBlockCalcLastMin = 0;
	m_nBlockCalcCounter = 0;
	CWinApp* pApp = AfxGetApp();
	CString strSection       = "file_set_path";
	CString strStringItem  =  "shF10";
	m_sShF10 = pApp->GetProfileString(strSection, strStringItem);
	strStringItem  =  "szF10";
	m_sSzF10 = pApp->GetProfileString(strSection, strStringItem);
	strStringItem  =  "news_path";
	m_sNews = pApp->GetProfileString(strSection, strStringItem);


	m_infoInit.nCountIndex=1;
	m_infoInit.initIndex[0].yFloatBottom=0.25;
	m_infoInit.initIndex[1].yFloatBottom=0.25;
	m_infoInit.initIndex[2].yFloatBottom=0.25;
	m_infoInit.initIndex[3].yFloatBottom=0.25;

	m_infoInit.nCountMin1=3;
	m_infoInit.initMin1[0].yFloatBottom=0.25;
	m_infoInit.initMin1[1].yFloatBottom=0.25;
	m_infoInit.initMin1[2].yFloatBottom=0.25;
	m_infoInit.initMin1[3].yFloatBottom=0.25;

	//ÿ����ͼ��ָ���ID�š�0Ϊ��ʱ���ƣ�
	//1Ϊ�ɽ�����2Ϊ����������3Ϊ����ָ�꣬
	//4Ϊ����ָ�꣬5Ϊ���ָ��
	m_infoInit.flag[0]=0;
	m_infoInit.flag[1]=1;
	m_infoInit.flag[2]=2;
	m_infoInit.flag[3]=1;
	m_infoInit.flag_dapan[0]=0;
	m_infoInit.flag_dapan[1]=1;
	m_infoInit.flag_dapan[2]=2;
	m_infoInit.flag_dapan[3]=1;

	strcpy(m_infoInit.initIndex[0].nameIndex,"MA");
	strcpy(m_infoInit.initIndex[1].nameIndex,"KDJ");
	strcpy(m_infoInit.initIndex[2].nameIndex,"KDJ");
	strcpy(m_infoInit.initIndex[3].nameIndex,"KDJ");

	m_propertyInitiate.daysOfDayKline = 0;     //the days of day kline
	m_propertyInitiate.daysOfMin5Kline = 15;    //the days of 5 minute kline
	m_propertyInitiate.daysOfManyKline = 50; //the days of 5 many day kline.if it is negtive,it is minute

	m_propertyInitiate.bDoPowerSelectingStock = TRUE;     //
	m_propertyInitiate.bDoVolPowering = TRUE;    //
	
	memset(&m_fontdefault, 0, sizeof m_fontdefault);
	m_fontdefault.lfHeight = -16;
	lstrcpy(m_fontdefault.lfFaceName, _T("����"));
	m_fontdefault.lfOutPrecision = OUT_TT_PRECIS;
	m_fontdefault.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_fontdefault.lfQuality = PROOF_QUALITY;
	m_fontdefault.lfPitchAndFamily = FF_SWISS | VARIABLE_PITCH;
	
	// TODO: add one-time construction code here
	m_nHave_Olddata=0; 

	m_nStockTypeShPage=SHZS;
	m_nStockTypeSzPage=SZZS;
	m_nStockTypeFlPage=SHAG;

	m_pReportView=NULL;
	int i,j;
	for(j=0;j<4;j++)
	for(i=0;i<30;i++)
		m_nColumnIndex[j][i]=-1;
	m_sharesSymbol = CSharesCompute::GetIndexSymbol(0);
	m_stkKind=0;
	m_pNineShowView = NULL;
	m_nCurrentReportPage=AGPAGE;
	m_nCurrentBlockPage = 2;
	m_nPreDisplayRow=0;
	m_nIsClose=FALSE;

	m_pBlockDataShow = NULL;
	m_nBlockShowMaxCount = 0;
	m_nBlockShowCount = 0;

	m_pStockDataShow=NULL;
	m_nShowMaxCount=0;
	m_nShowCount = 0;
	m_sharesCompute.SetMainDocument(this);
	InitSetp=0;
	m_taiViewF9 = NULL;
	m_taiViewF10 = NULL;
	m_F10HeadView = NULL;

	m_pStockCjMxTj=NULL;
	m_pStockBigBillData=NULL;
    m_pDownLoadDlg=NULL;
	m_bOpenStockTypeDlg=FALSE;

	//�����ʼ����Ʒ�û������ڹ�˾��ע��ţ���Ȩ��
	m_strUser=_T("");
	m_strCompany=_T("");
	m_strSerial=_T("");
	m_strCredit=_T("");
    m_CalcBlockCounts=10;

	for(j=0;j<MAXCOLUME;j++)
	{
		m_nColumnWidth[j]=60;
	}
}

CTaiShanDoc::~CTaiShanDoc()
{
	for (int i = 0;i < 4;i++)
	{
		if(m_pStatisticData[i].pItem && m_pStatisticData[i].pItem->pBaseInfo)
		{
			GlobalUnlock((HGLOBAL)m_pStatisticData[i].pItem->pBaseInfo);        //�ͷ��ڴ�
			GlobalFree( (HGLOBAL)m_pStatisticData[i].pItem->pBaseInfo);
		}
		if(m_pStatisticData[i].pItem && m_pStatisticData[i].pItem->pBlockInfo)
		{
			GlobalUnlock((HGLOBAL)m_pStatisticData[i].pItem->pBlockInfo);        //�ͷ��ڴ�
			GlobalFree( (HGLOBAL)m_pStatisticData[i].pItem->pBlockInfo);
		}
		if(m_pStatisticData[i].pItem)
		{
			GlobalUnlock((HGLOBAL)m_pStatisticData[i].pItem);        //�ͷ��ڴ�
			GlobalFree( (HGLOBAL)m_pStatisticData[i].pItem);
		}
	}
	if (m_pStatisticData)
	{
		GlobalUnlock((HGLOBAL)m_pStatisticData);        //�ͷ��ڴ�
		GlobalFree((HGLOBAL)m_pStatisticData);
	}

	int i,length;
	length = m_formuar_index.GetSize();
	for(i=0;i<length;i++)
	{
		CFormularContent * jishu;
		jishu = m_formuar_index.GetAt(i);
		delete jishu;
	}
	
	length = m_formuar_choose.GetSize();
	for(i=0;i<length;i++)
	{
		CFormularContent * jishu;
		jishu = m_formuar_choose.GetAt(i);
		delete jishu;
	}

	length = m_formuar_kline.GetSize();
	for(i=0;i<length;i++)
	{
		CFormularContent * jishu;
		jishu = m_formuar_kline.GetAt(i);
		delete jishu;
	}
	delete m_pTechniqueColumn;
    ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->m_pDoc=NULL; 
    ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->m_MDIChildWnd =NULL; 
#ifndef OEM
//	mydatabase.Close();
#endif
	m_pNineShowView = NULL;
     if(m_pStockDataShow)
	 {
		 GlobalUnlock((HGLOBAL)m_pStockDataShow);        //�ͷ��ڴ�
		 GlobalFree((HGLOBAL)m_pStockDataShow);
	 }
     if(m_pBlockDataShow)
	 {
		 GlobalUnlock((HGLOBAL)m_pBlockDataShow);        //�ͷ��ڴ�
		 GlobalFree((HGLOBAL)m_pBlockDataShow);
	 }
}


/////////////////////////////////////////////////////////////////////////////
// CTaiShanDoc serialization

void CTaiShanDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTaiShanDoc diagnostics

#ifdef _DEBUG
void CTaiShanDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTaiShanDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG




/////////////////////////////////////////////////////////////////////////////
// CTaiShanDoc commands
//****************************************************************************************
//���ĵ���ر��ĵ�����
/////////////////////////////////////////////////////////////////////////////
//      �� �� ����OnNewDocument() 
//      ����˵����MFC ��⺯���������ｫ�Ǵ��ĵ�ʱ������ʼ������ 
//      ��ڲ������������
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
BOOL CTaiShanDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
#ifdef TEST_USER1
	CTime t = CTime::GetCurrentTime();
    CTime t2 = g_timeUseEnd;
	if(t >= t2)
	{
	    return TRUE;
	}
	else
	{
	}
#endif

	try
	{
		CreateDirectory("DATA",NULL);
		CreateDirectory("html",NULL);
		CreateDirectory("bitmap",NULL);
		CreateDirectory("NEWS",NULL);
		CreateDirectory("NEWS\\shanghai",NULL);
		CreateDirectory("NEWS\\shenzhen",NULL);
		CreateDirectory("NEWS\\cj",NULL);
		CreateDirectory("block",NULL);
		CreateDirectory("DATA\\indicator",NULL);
	}
	catch(...)
	{

	}
    m_bCloseReceiver = FALSE;
	m_bAppAuthorized = ((CWHApp*)AfxGetApp())->m_bAppAuthorized;

	CFile SysFile;
	if(SysFile.Open((LPCTSTR)g_xtsx_hz,CFile::modeRead,NULL))
	{
       int FileID;
	   SysFile.Read(&FileID,sizeof(int));

	   try
		{
			SysFile.Seek(16,CFile::begin);
			SysFile.Read(&m_colorArrayDefault[0],sizeof(COLORREF)*6*24);
			SysFile.Read(&m_colorArray,sizeof(COLORREF)*34);
			SysFile.Read(&m_fontdefault,sizeof(LOGFONT));
			SysFile.Seek(sizeof(LOGFONT),CFile::current);//
			SysFile.Read(&m_fontstr,sizeof(LOGFONT)*FONTSTRUCTLEN);
			SysFile.Read(&m_propertyInitiate,sizeof(PROPERTY_INIT));
			SysFile.Seek(40,CFile::current);//
			SysFile.Read(&m_syshqset,sizeof(SYSTEMHQSET));
			SysFile.Seek(40,CFile::current);//
			SysFile.Read(&m_systemdata,sizeof(SYSTEMDATA));
			SysFile.Seek(40,CFile::current);//
			SysFile.Read(&m_systemOption,sizeof(SYSTEM_OPTION));
			SysFile.Seek(40,CFile::current);//
			SysFile.Read(&m_dadanchoice,sizeof(ALERTCHOICE));
			SysFile.Seek(40,CFile::current);//
			SysFile.Read(&m_propertyInitiate2,sizeof(SUB_FIGUER_INIT_INFO));
			SysFile.Seek(40,CFile::current);//
			//SysFile.Read(&temp1,sizeof(SYSTEMINITDATA));
			SysFile.Read(&m_SystemInitData,sizeof(SYSTEMINITDATA));
			SysFile.Seek(40,CFile::current);
			SysFile.Read(&m_Directory,sizeof(SETCURRENTDIRECTORY));
			SysFile.Seek(40,CFile::current);
			SysFile.Read(&m_colorArrayDefault[0],sizeof(COLORREF)*13*34);

		//	SysFile.Seek(40,CFile::current);

			SysFile.Read(m_nColumnWidth,sizeof(int)*MAXCOLUME);
			for(int j=0;j<MAXCOLUME;j++)
			{
				if(m_nColumnWidth[j] < 10 || m_nColumnWidth[j] > 1200)
					m_nColumnWidth[j] = 60;
			}
	   }
	   catch(...)
	   {

	   }
    
	   //SysFile.Close();
	   //SYSTEMINITDATA temp;
	   //memset(&temp,0,sizeof(SYSTEMINITDATA));
	   //temp.BlockTime = temp1.BlockTime;
	   //memcpy(temp.ColumnIndex, temp1.ColumnIndex,30 * sizeof(int));
	   //temp.ColumnType = temp1.ColumnType;
	   //memcpy(temp.ColumnWidth, temp1.ColumnWidth,30 * sizeof(int));
	   //temp.InitComplete = temp1.InitComplete;
	   //memcpy(temp.KswtAddr, temp1.KswtAddr,255);
	   //temp.m_TotalNineViewData = temp1.m_TotalNineViewData;
	   //memcpy(temp.NetAddr, temp1.NetAddr,255);
	   //temp.RealTime = temp1.RealTime;
	   //strcpy(temp.ScreenStockName, "����ѡ��");
	   //strcpy(temp.StockTypeName, "A�ɰ��");
	   //memcpy(temp.TechniqueColumn, temp1.TechniqueColumn,MAXJISHUCOUNT * sizeof(TECHNIQUE_COLUMN));
	   //strcpy(temp.ChooseTypeName, "��ѡ���");

	   //SysFile.Open((LPCTSTR)g_xtsx_hz,CFile::modeCreate|CFile::modeWrite,NULL);
	   //SysFile.Write(&FileID,sizeof(int));
	   //SysFile.Seek(16,CFile::begin);
	   //SysFile.Write(&m_colorArrayDefault[0],sizeof(COLORREF)*6*24);
	   //SysFile.Write(&m_colorArray,sizeof(COLORREF)*34);
	   //SysFile.Write(&m_fontdefault,sizeof(LOGFONT));
	   //SysFile.Seek(sizeof(LOGFONT),CFile::current);//
	   //SysFile.Write(&m_fontstr,sizeof(LOGFONT)*FONTSTRUCTLEN);
	   //SysFile.Write(&m_propertyInitiate,sizeof(PROPERTY_INIT));
	   //SysFile.Seek(40,CFile::current);//
	   //SysFile.Write(&m_syshqset,sizeof(SYSTEMHQSET));
	   //SysFile.Seek(40,CFile::current);//
	   //SysFile.Write(&m_systemdata,sizeof(SYSTEMDATA));
	   //SysFile.Seek(40,CFile::current);//
	   //SysFile.Write(&m_systemOption,sizeof(SYSTEM_OPTION));
	   //SysFile.Seek(40,CFile::current);//
	   //SysFile.Write(&m_dadanchoice,sizeof(ALERTCHOICE));
	   //SysFile.Seek(40,CFile::current);//
	   //SysFile.Write(&m_propertyInitiate2,sizeof(SUB_FIGUER_INIT_INFO));
	   //SysFile.Seek(40,CFile::current);//
	   //SysFile.Write(&temp,sizeof(SYSTEMINITDATA));
	   ////SysFile.Write(&m_SystemInitData,sizeof(SYSTEMINITDATA));
	   //SysFile.Seek(40,CFile::current);
	   //SysFile.Write(&m_Directory,sizeof(SETCURRENTDIRECTORY));
	   //SysFile.Seek(40,CFile::current);

	   //SysFile.Write(m_colorArrayDefault,sizeof(COLORREF)*12*34);//Marenan
	   ////	SysFile.Seek(40,CFile::current);
	   //SysFile.Write(m_nColumnWidth,sizeof(int)*MAXCOLUME);

	   ////end
	   //SysFile.Close();
	}

//Added by ltr 2003-11-30
//Ŀ�ģ����γɹ�Ʊƴ��ʱ��������Щ���ְ���ϵͳ���벻���ҵ�ƴ����
//���¶���һ���࣬����Щ������ȡ�������˹�����ƴ��
	m_spz.Open();	//װ��spz.txt�ļ�
	m_dyz.Open();	//װ��dyz.dat�ļ�
	m_4or6=0;
	m_pPingCe=new CTaiShanTesting(this);
	if (m_pPingCe==NULL)
		AfxMessageBox("initial Ping Ce failed!");
//  begin

//���ļ�
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	pFm->m_pDoc = this;
		
	CTaiKlineFileKLine::OpenAll();
	
	CTime tt;
	CString strTime;
	CString strUpdate;
	strUpdate="20011112";
	tt=CTime::GetCurrentTime();
	strTime=tt.Format("%Y%m%d");
	if(	strcmp(strUpdate,strTime) == 0 ||
		strcmp(strUpdate,strTime) == -1)//����û�����ʱ����ڵ���2001��11��12��
	{
		if(_access("transfered.dat",0)==-1)
		{	
			m_4or6=1;
			CTaiKlineFileKLine::Transfer4To6();
			CFile f;
			int nt=1112;
			f.Open("transfered.dat",CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary);
			f.Write(&nt,sizeof(int));
			f.Close();			
		}
		m_4or6=1;			
		//CTaiKlineFileKLine::ChooseReceiveDataForm();		
	}
	else
	{
		m_4or6=0;
		CTaiKlineFileKLine::Transfer4To6();		
	}
	//end 
	
	//m_4or6=1;			

	CTaiKlineFileHS::OpenAll();

	//this->SetTitle(""+g_strCompanyName+"�������");
	this->SetTitle("�Լ���ţB���");			// zyd modified

	time_t now; 
	time( &now );
    m_week = *localtime( &now );

	//RegisterServiceProcess(GetCurrentProcessID,1);
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
#ifdef WIDE_NET_VERSION
	Init_dat();//�õ���������
    Init_StockData(2);  
#else
	chk_date();//�õ���������
	Init_dat();//�õ���������
	Init_EveryDay();//��ʼ����Ʊ�������ݣ�������ճ�ʼ������
#endif

 	::GetCurrentDirectory(MAX_PATH,m_CurrentWorkDirectory.GetBuffer(MAX_PATH));
	m_CurrentWorkDirectory.ReleaseBuffer();
	
    m_pTechniqueColumn=new TechniqueColumnInfo(this);
	
    int i;
	if(m_SystemInitData.InitComplete!=INITDATAID)
	{
		strcpy(m_SystemInitData.ChooseTypeName,"��ѡ���");        //�������
		strcpy(m_SystemInitData.StockTypeName,"����Ʊ");        //�������
		strcpy(m_SystemInitData.ScreenStockName,"������Ʊ");      //����ѡ������ 
		for(i=0;i<30;i++)
		   m_SystemInitData.ColumnIndex[i]=-1;                    //��̬��ʾ���к�
		m_SystemInitData.ColumnType=0;               //��̬��ʾ��Ŀǰ��ʾ��������
		for( i=0;i<30;i++)
		{
           if(i==0)
		   {
		      m_SystemInitData.ColumnWidth[i]=40;          //��̬��ʾ���п��
			  continue;
		   }
           if(i==1) 
		   {
		      m_SystemInitData.ColumnWidth[i]=70;          //��̬��ʾ���п��
			  continue;
		   }
           if(i==2) 
		   {
		      m_SystemInitData.ColumnWidth[i]=75;          //��̬��ʾ���п��
			  continue;
		   }
	       m_SystemInitData.ColumnWidth[i]=70;          //��̬��ʾ���п��
		}
		for(i=0;i<MAXJISHUCOUNT;i++)
		{
		   memset(&m_SystemInitData.TechniqueColumn[i],0,sizeof(Index_Technique));
		   m_SystemInitData.TechniqueColumn[i].IsUse=FALSE;        ////��̬��ʾ��������
           m_SystemInitData.TechniqueColumn[i].nID =i+9000;
		}
	    m_SystemInitData.InitComplete=INITDATAID;
	}
	for(int i=0;i<MAXJISHUCOUNT;i++)
	{
	   if(m_SystemInitData.TechniqueColumn[i].nID != (UINT)(i+9000))
	   {
		   memset(&m_SystemInitData.TechniqueColumn[i],0,sizeof(Index_Technique));
		   m_SystemInitData.TechniqueColumn[i].IsUse=FALSE;        ////��̬��ʾ��������
		   m_SystemInitData.TechniqueColumn[i].nID =i+9000;
	   }
	}


//////////////////////yhh
	memset(&m_fontdefault, 0, sizeof m_fontdefault);
	m_fontdefault.lfHeight = 16;
	lstrcpy(m_fontdefault.lfFaceName, _T("����"));
	m_fontdefault.lfOutPrecision = OUT_TT_PRECIS;
	m_fontdefault.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_fontdefault.lfQuality = PROOF_QUALITY;
	m_fontdefault.lfPitchAndFamily = FF_SWISS | VARIABLE_PITCH;


	char  blank[2]={' ',' '};              //����֮��ļ��
     
		
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	((CMainFrame*)AfxGetMainWnd())->m_pDoc=this;// 
	

	//����ϵͳ��λͼ
	m_imagelist=new CImageList();
    m_imagelist->Create(16,16, ILC_MASK, 21, 2);

	int idb[21] = {IDB_BITMAP21,IDB_BITMAP2,IDB_BITMAP3,IDB_BITMAP4,IDB_BITMAP5,
		IDB_BITMAP6,IDB_BITMAP7,IDB_BITMAP8,IDB_BITMAP19,IDB_BITMAP20,
		IDB_BITMAP_110,IDB_BITMAP_111,IDB_BITMAP_112,IDB_BITMAP_113,IDB_BITMAP_114,
		IDB_BITMAP_131,IDB_BITMAP_132,IDB_BITMAP_133,IDB_BITMAP_134,IDB_BITMAP_114,
		IDB_BITMAP_135};
	for( i=0;i<21;i++)
	{
		m_bitmap.LoadBitmap(idb[i]);
		m_imagelist->Add(&m_bitmap, (COLORREF)0xffffff);
		m_bitmap.DeleteObject();
	}
	///////////////Open the database////////////////////// 

	long length=0;
	//	---		����ָ��ӳ���
	CFormularContent::ReadWriteIndex(this, 0, true);
	CFormularContent::ReadWriteIndex(this, 1, true);
	CFormularContent::ReadWriteIndex(this, 2, true);

	////////////////////////////////////////**END**////////////////////////////////////
	//��ʼ����̬��ʾ��
	POSITION pos = GetFirstViewPosition(); 
	while (pos != NULL) 
	{
      CView* pView = GetNextView(pos);
	  if(pView->IsKindOf( RUNTIME_CLASS(CTaiShanReportView)))
	  {
		    CTaiShanReportView *m_View;
			m_View=(CTaiShanReportView *)pView;
            m_View->InitGridCtrl();
			m_pReportView=m_View;
	  }
	}   

	/////////////////////////////////////
	//Get Current work directory. --
	//ltr �õ���˾����,�û�������ע��ţ���Ȩ��	2003-11-13
	CString strKey = "SOFTWARE\\WsStock\\Ver2.0";
	HKEY hResult;
	if(ERROR_SUCCESS == RegOpenKey(HKEY_LOCAL_MACHINE,(LPCTSTR)strKey,&hResult))
	{
		char User[40];
		char Company[40];
		char Serial[40];
		char Credit[40];

		User[0] = 0;
		Company[0] = 0;
		Serial[0]=0;
		Credit[0]=0;

		DWORD dwCount = 40;
		RegQueryValueEx(hResult,(LPCTSTR)"User",0,0,(unsigned char *)User,&dwCount);
		dwCount = 40;
		RegQueryValueEx(hResult,(LPCTSTR)"Company",0,0,(unsigned char *)Company,&dwCount);
		dwCount=40;
		RegQueryValueEx(hResult,(LPCTSTR)"Serial",0,0,(unsigned char *)Serial,&dwCount);
		dwCount=40;
		RegQueryValueEx(hResult,(LPCTSTR)"Credit",0,0,(unsigned char *)Credit,&dwCount);
		m_strUser=User;
		m_strCompany=Company;
		m_strSerial=Serial;
		m_strCredit=Credit;
	}
	RegCloseKey(hResult);
	m_strPName=_T(""+g_strCompanyName+"֤ȯ�������ϵͳ");
	//

	//����Ԥ�� //  add 2001 10 24
	CAlertSystem::StartAlarm();

	pFm->m_dlgLeftPop .InsertData();

	return TRUE;
}
void CTaiShanDoc::InitTjxg()
{
	CFile  blocknamefile;
	char test[5];
	int length=0;
	char  blank[2]={' ',' '};
	BOOL istrue;
	if(blocknamefile.Open("block/����ѡ����.DAT",CFile::modeReadWrite))
	{
		char blocknamehead[4]={65,79,88,15};
		blocknamefile.Read(&test,4);
		istrue=true;
		blocknamefile.Read(&length,4);
		if(length>=100)
           return ; 
		if(istrue)
		{
		  for(int i=0;i<length;i++)
		  {
			CString name;
			char namechar[13];
			blocknamefile.Read(&namechar,12);
			blocknamefile.Read(&blank,2);
			name=namechar;
			BLOCKSTOCK *block=new BLOCKSTOCK(name);
			m_Tjxgblocklist.AddTail(block);
		  }
		}
		blocknamefile.Close();
	}
	else
	{
		blocknamefile.Open("block/����ѡ����.DAT",CFile::modeCreate);
        blocknamefile.Close(); 
	    blocknamefile.Open("block/����ѡ����.DAT",CFile::modeReadWrite);
		char blocknamehead[4]={65,79,88,15};
		length=0;
		blocknamefile.Write(&blocknamehead,4);
		blocknamefile.Write(&length,4);
		blocknamefile.Close();
	}

	POSITION position=m_Tjxgblocklist.GetHeadPosition();
	for(int i=0;i<length;i++)
	{
		BLOCKSTOCK * blocknow=m_Tjxgblocklist.GetNext(position);
		int stocklength;
		CFile stockfile;
		CString filepath="�������/"+blocknow->blockname;
    	if(stockfile.Open((LPCTSTR)filepath,CFile::modeReadWrite))
		{
			stockfile.Read(&stocklength,4);
			for(int j=0;j<stocklength;j++)
			{
				SharesSymbol *symbol=new SharesSymbol;
				stockfile.Read(symbol->sharesymbol,6);
                stockfile.Read(&symbol->nKind,4);
				symbol->sharesymbol[6]='\0';
				stockfile.Read(&blank,2);
				blocknow->stocklist.AddTail(symbol);
			}
			stockfile.Close();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
//      �� �� ����OnCloseDocument() 
//      ����˵����MFC ��⺯���������ｫ�ǹر��ĵ�ʱ�������������� 
//      ��ڲ������������
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiShanDoc::OnCloseDocument() 
{
	m_nIsClose=TRUE;
///////////////////////////////////////////////////
	CFile fl;

	if(m_pReportView!=NULL)
	   m_pReportView->KillTimer(ID_GPHQXS);

	if(	m_pDownLoadDlg)
		::SendMessage(	m_pDownLoadDlg->m_hWnd,WM_CLOSE,0,0); 

	if(	m_pStockCjMxTj)
		::SendMessage(	m_pStockCjMxTj->m_hWnd,WM_CLOSE,0,0); 
    if(m_pStockBigBillData)
		delete m_pStockBigBillData;
	int tmp=GetStocktime(0) ;      //����Ŀǰʱ��
	if(m_week.tm_wday!=0&&m_week.tm_wday!=6&&m_nANT[0]==239&&!m_bCloseWorkDone&&tmp>902)
	{
		int rtn = MessageBox(NULL,"��û����������ҵ���Ƿ�������̲�����","����",MB_YESNO|MB_ICONWARNING);
		if(rtn==6)
		{
           StockCloseWork();
		}
	}

	//����ȫ���������ݵ��ļ�
    if(this->m_bInitDone)
		SaveFileData();      
	//�����������
	m_sharesInformation.SaveCaiwuData();

    //�ر��������ϵͳ
    ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->HqStock_Quit();


	//**�ر��ĵ�����**

	//�ͷ� NAMESYMBOL �ڴ�
	delete m_imagelist;
	//�ͷ�����ѡ�ɽṹ�ڴ�
	POSITION pos = m_Tjxgblocklist.GetHeadPosition();
    while(pos!=NULL)
	{
        BLOCKSTOCK * m_pBlockStock;
        m_pBlockStock = m_Tjxgblocklist.GetNext(pos);
        
		POSITION m_pos=m_pBlockStock->stocklist.GetHeadPosition();
        while(m_pos)
		{
           SharesSymbol* m_pStockSymbol;
           m_pStockSymbol=m_pBlockStock->stocklist.GetNext(m_pos);
		   delete m_pStockSymbol;
		}
        m_pBlockStock->stocklist.RemoveAll();
		delete m_pBlockStock;
	}
	m_Tjxgblocklist.RemoveAll();


	//�ͷ�����ѡ�ɹ�ʽ�ṹ�ڴ�
	int length=m_formuar_choose.GetSize();
	FreeKeyBoardAngel();


	m_spz.Close();		//Added by ltr 2003-11-30
	m_dyz.Close();
	if (m_pPingCe != NULL)
	{
		delete m_pPingCe;
		m_pPingCe=NULL;
	}


	//typedef struct _SystemInitData
	//{
	//	int InitComplete;
	//	char StockTypeName[64];        //�������
	//	char ScreenStockName[64];      //����ѡ������ 
	//	int  ColumnIndex[30];          //��̬��ʾ���к�
	//	int  ColumnType;               //��̬��ʾ��Ŀǰ��ʾ��������
	//	int  ColumnWidth[30];          //��̬��ʾ���п��
	//	TECHNIQUE_COLUMN TechniqueColumn[MAXJISHUCOUNT];        ////��̬��ʾ��������
	//	char NetAddr[255];               //Ĭ����ַ
	//	char KswtAddr[255];              //����ί��ȱʡ·��
	//	int  RealTime;                  //ʵʱ����ʱ����
	//	int  BlockTime;                 //���ָ������ʱ����         
	//	TotalNineViewData m_TotalNineViewData;
	//	char ChooseTypeName[64];        //��ѡ������
	//}SYSTEMINITDATA0;	 

	//SYSTEMINITDATA0 temp;
	//memset(&temp,0,sizeof(SYSTEMINITDATA0));
	//temp.BlockTime = m_SystemInitData.BlockTime;
	//memcpy(temp.ColumnIndex, m_SystemInitData.ColumnIndex,30 * sizeof(int));
	//temp.ColumnType = m_SystemInitData.ColumnType;
	//memcpy(temp.ColumnWidth, m_SystemInitData.ColumnWidth,30 * sizeof(int));
	//temp.InitComplete = m_SystemInitData.InitComplete;
	//memcpy(temp.KswtAddr, m_SystemInitData.KswtAddr,255);
	//temp.m_TotalNineViewData = m_SystemInitData.m_TotalNineViewData;
	//memcpy(temp.NetAddr, m_SystemInitData.NetAddr,255);
	//temp.RealTime = m_SystemInitData.RealTime;
	//strcpy(temp.ScreenStockName, m_SystemInitData.ScreenStockName);
	//strcpy(temp.StockTypeName, m_SystemInitData.StockTypeName);
	//memcpy(temp.TechniqueColumn, m_SystemInitData.TechniqueColumn,MAXJISHUCOUNT * sizeof(TECHNIQUE_COLUMN));
	//strcpy(temp.ChooseTypeName, "��ѡ���");

	CFile SysFile;
	SysFile.Open((LPCTSTR)g_xtsx_hz,CFile::modeCreate|CFile::modeWrite,NULL);
	int FileID = time(NULL);
	SysFile.Write(&FileID,sizeof(int));
	SysFile.Seek(16,CFile::begin);
    SysFile.Write(&m_colorArrayDefault[0],sizeof(COLORREF)*6*24);
	SysFile.Write(&m_colorArray,sizeof(COLORREF)*34);
	SysFile.Write(&m_fontdefault,sizeof(LOGFONT));
	SysFile.Seek(sizeof(LOGFONT),CFile::current);//
	SysFile.Write(&m_fontstr,sizeof(LOGFONT)*FONTSTRUCTLEN);
	SysFile.Write(&m_propertyInitiate,sizeof(PROPERTY_INIT));
    SysFile.Seek(40,CFile::current);//
	SysFile.Write(&m_syshqset,sizeof(SYSTEMHQSET));
	SysFile.Seek(40,CFile::current);//
	SysFile.Write(&m_systemdata,sizeof(SYSTEMDATA));
	SysFile.Seek(40,CFile::current);//
	SysFile.Write(&m_systemOption,sizeof(SYSTEM_OPTION));
	SysFile.Seek(40,CFile::current);//
	SysFile.Write(&m_dadanchoice,sizeof(ALERTCHOICE));
	SysFile.Seek(40,CFile::current);//
	SysFile.Write(&m_propertyInitiate2,sizeof(SUB_FIGUER_INIT_INFO));
	SysFile.Seek(40,CFile::current);//
	//SysFile.Write(&temp,sizeof(SYSTEMINITDATA0));
	SysFile.Write(&m_SystemInitData,sizeof(SYSTEMINITDATA));
    SysFile.Seek(40,CFile::current);
	SysFile.Write(&m_Directory,sizeof(SETCURRENTDIRECTORY));
    SysFile.Seek(40,CFile::current);

	/**********Marenan************/
	//if(FileID>1256476020+3600*24*183)//2009/10/25 + 83������
	//{
	//	char Data[1024+1];
	//	SysFile.SeekToBegin();
	//	SysFile.Write(Data,1024); 
	//}
	/**********Marenan************/

/*	for(int l = 0;l<8;l++)
	{
		CString s3;
		s3.Format ("h:\\ϵͳ��ɫ\\%d.dat",l);
		CFile fl;
		if(fl.Open (s3,CFile::modeRead))
		{
			fl.Seek (16+sizeof(COLORREF)*6*24,CFile::begin);
			fl.Read (m_colorArrayDefault+l,sizeof(COLORREF)*1*34);
			fl.Close ();
		}
	}
*/
/*
	//	����
	m_colorArrayDefault[0][0] = RGB(0,0,0);
	m_colorArrayDefault[0][1] = RGB(198,195,198);
	m_colorArrayDefault[0][2] = RGB(198,0,0);
	m_colorArrayDefault[0][3] = RGB(255,81,82);
	m_colorArrayDefault[0][4] = RGB(82,255,255);
	m_colorArrayDefault[0][5] = RGB(82,255,255);
	m_colorArrayDefault[0][6] = RGB(255,255,0);
	m_colorArrayDefault[0][7] = RGB(255,255,255);
	m_colorArrayDefault[0][8] = RGB(255,255,0);
	m_colorArrayDefault[0][9] = RGB(255,0,255);
	m_colorArrayDefault[0][10] = RGB(0,255,0);
	m_colorArrayDefault[0][11] = RGB(198,195,198);
	m_colorArrayDefault[0][12] = RGB(0,0,255);
	m_colorArrayDefault[0][13] = RGB(255,81,82);
	m_colorArrayDefault[0][14] = RGB(255,255,255);
	m_colorArrayDefault[0][15] = RGB(82,255,82);
	m_colorArrayDefault[0][16] = RGB(0,0,132);
	m_colorArrayDefault[0][17] = RGB(0,0,0);
	m_colorArrayDefault[0][18] = RGB(0,0,0);
	m_colorArrayDefault[0][19] = RGB(0,0,0);
	m_colorArrayDefault[0][20] = RGB(255,255,82);
	m_colorArrayDefault[0][21] = RGB(82,255,255);
	//���
	m_colorArrayDefault[1][0] = RGB(0,0,0);
	m_colorArrayDefault[1][1] = RGB(198,195,198);
	m_colorArrayDefault[1][2] = RGB(198,0,0);
	m_colorArrayDefault[1][3] = RGB(255,81,82);
	m_colorArrayDefault[1][4] = RGB(82,255,255);
	m_colorArrayDefault[1][5] = RGB(82,255,255);
	m_colorArrayDefault[1][6] = RGB(255,255,0);
	m_colorArrayDefault[1][7] = RGB(255,255,255);
	m_colorArrayDefault[1][8] = RGB(255,255,0);
	m_colorArrayDefault[1][9] = RGB(255,0,255);
	m_colorArrayDefault[1][10] = RGB(0,255,0);
	m_colorArrayDefault[1][11] = RGB(198,195,198);
	m_colorArrayDefault[1][12] = RGB(0,0,255);
	m_colorArrayDefault[1][13] = RGB(255,81,82);
	m_colorArrayDefault[1][14] = RGB(198,195,198);
	m_colorArrayDefault[1][15] = RGB(82,255,82);
	m_colorArrayDefault[1][16] = RGB(0,0,132);
	m_colorArrayDefault[1][17] = RGB(132,0,0);
	m_colorArrayDefault[1][18] = RGB(0,0,0);
	m_colorArrayDefault[1][19] = RGB(0,0,0);
	m_colorArrayDefault[1][20] = RGB(255,255,82);
	m_colorArrayDefault[1][21] = RGB(82,255,255);
	//�׺�
	m_colorArrayDefault[2][0] = RGB(0,0,0);
	m_colorArrayDefault[2][1] = RGB(198,195,198);
	m_colorArrayDefault[2][2] = RGB(198,0,0);
	m_colorArrayDefault[2][3] = RGB(255,81,82);
	m_colorArrayDefault[2][4] = RGB(82,255,255);
	m_colorArrayDefault[2][5] = RGB(0,255,255);
	m_colorArrayDefault[2][6] = RGB(255,255,0);
	m_colorArrayDefault[2][7] = RGB(255,255,255);
	m_colorArrayDefault[2][8] = RGB(255,255,0);
	m_colorArrayDefault[2][9] = RGB(255,0,255);
	m_colorArrayDefault[2][10] = RGB(0,255,0);
	m_colorArrayDefault[2][11] = RGB(198,195,198);
	m_colorArrayDefault[2][12] = RGB(0,0,255);
	m_colorArrayDefault[2][13] = RGB(255,0,0);
	m_colorArrayDefault[2][14] = RGB(198,195,198);
	m_colorArrayDefault[2][15] = RGB(0,255,255);
	m_colorArrayDefault[2][16] = RGB(0,0,132);
	m_colorArrayDefault[2][17] = RGB(132,130,132);
	m_colorArrayDefault[2][18] = RGB(0,0,0);
	m_colorArrayDefault[2][19] = RGB(0,0,0);
	m_colorArrayDefault[2][20] = RGB(198,195,198);
	m_colorArrayDefault[2][21] = RGB(198,195,198);
	//����
	m_colorArrayDefault[3][0] = RGB(255,255,255);
	m_colorArrayDefault[3][1] = RGB(0,0,0);
	m_colorArrayDefault[3][2] = RGB(132,130,132);
	m_colorArrayDefault[3][3] = RGB(255,0,0);
	m_colorArrayDefault[3][4] = RGB(0,0,255);
	m_colorArrayDefault[3][5] = RGB(0,0,255);
	m_colorArrayDefault[3][6] = RGB(255,0,255);
	m_colorArrayDefault[3][7] = RGB(0,0,132);
	m_colorArrayDefault[3][8] = RGB(255,0,255);
	m_colorArrayDefault[3][9] = RGB(132,130,132);
	m_colorArrayDefault[3][10] = RGB(132,0,0);
	m_colorArrayDefault[3][11] = RGB(0,130,0);
	m_colorArrayDefault[3][12] = RGB(0,255,255);
	m_colorArrayDefault[3][13] = RGB(255,0,0);
	m_colorArrayDefault[3][14] = RGB(198,195,198);
	m_colorArrayDefault[3][15] = RGB(0,255,255);
	m_colorArrayDefault[3][16] = RGB(0,0,132);
	m_colorArrayDefault[3][17] = RGB(132,130,132);
	m_colorArrayDefault[3][18] = RGB(255,255,255);
	m_colorArrayDefault[3][19] = RGB(255,255,255);
	m_colorArrayDefault[3][20] = RGB(0,0,0);
	m_colorArrayDefault[3][21] = RGB(0,0,0);
	//�̰�
	m_colorArrayDefault[4][0] = RGB(255,255,255);
	m_colorArrayDefault[4][1] = RGB(0,0,0);
	m_colorArrayDefault[4][2] = RGB(132,130,132);
	m_colorArrayDefault[4][3] = RGB(255,0,0);
	m_colorArrayDefault[4][4] = RGB(0,130,0);
	m_colorArrayDefault[4][5] = RGB(0,130,132);
	m_colorArrayDefault[4][6] = RGB(0,0,255);
	m_colorArrayDefault[4][7] = RGB(0,0,132);
	m_colorArrayDefault[4][8] = RGB(255,0,255);
	m_colorArrayDefault[4][9] = RGB(132,130,132);
	m_colorArrayDefault[4][10] = RGB(132,0,0);
	m_colorArrayDefault[4][11] = RGB(0,130,0);
	m_colorArrayDefault[4][12] = RGB(0,255,255);
	m_colorArrayDefault[4][13] = RGB(255,0,0);
	m_colorArrayDefault[4][14] = RGB(0,0,0);
	m_colorArrayDefault[4][15] = RGB(0,130,0);
	m_colorArrayDefault[4][16] = RGB(0,255,255);
	m_colorArrayDefault[4][17] = RGB(198,195,198);
	m_colorArrayDefault[4][18] = RGB(255,255,255);
	m_colorArrayDefault[4][19] = RGB(255,255,255);
	m_colorArrayDefault[4][20] = RGB(0,0,0);
	m_colorArrayDefault[4][21] = RGB(0,0,0);
	//�ڰ�
	m_colorArrayDefault[5][0] = RGB(255,255,255);
	m_colorArrayDefault[5][1] = RGB(0,0,0);
	m_colorArrayDefault[5][2] = RGB(132,130,132);
	m_colorArrayDefault[5][3] = RGB(132,0,0);
	m_colorArrayDefault[5][4] = RGB(0,0,0);
	m_colorArrayDefault[5][5] = RGB(0,0,132);
	m_colorArrayDefault[5][6] = RGB(0,130,132);
	m_colorArrayDefault[5][7] = RGB(0,0,132);
	m_colorArrayDefault[5][8] = RGB(255,0,255);
	m_colorArrayDefault[5][9] = RGB(132,130,132);
	m_colorArrayDefault[5][10] = RGB(132,0,0);
	m_colorArrayDefault[5][11] = RGB(0,130,0);
	m_colorArrayDefault[5][12] = RGB(0,255,255);
	m_colorArrayDefault[5][13] = RGB(132,0,0);
	m_colorArrayDefault[5][14] = RGB(0,0,0);
	m_colorArrayDefault[5][15] = RGB(0,130,0);
	m_colorArrayDefault[5][16] = RGB(0,255,255);
	m_colorArrayDefault[5][17] = RGB(198,195,198);
	m_colorArrayDefault[5][18] = RGB(255,255,255);
	m_colorArrayDefault[5][19] = RGB(255,255,255);
	m_colorArrayDefault[5][20] = RGB(0,0,0);
	m_colorArrayDefault[5][21] = RGB(0,0,0);
	//����
	m_colorArrayDefault[6][0] = RGB(0,0,132);
	m_colorArrayDefault[6][1] = RGB(255,255,255);
	m_colorArrayDefault[6][2] = RGB(198,195,198);
	m_colorArrayDefault[6][3] = RGB(255,0,0);
	m_colorArrayDefault[6][4] = RGB(0,255,255);
	m_colorArrayDefault[6][5] = RGB(0,255,255);
	m_colorArrayDefault[6][6] = RGB(255,255,0);
	m_colorArrayDefault[6][7] = RGB(255,0,255);
	m_colorArrayDefault[6][8] = RGB(255,255,255);
	m_colorArrayDefault[6][9] = RGB(0,255,0);
	m_colorArrayDefault[6][10] = RGB(198,195,198);
	m_colorArrayDefault[6][11] = RGB(0,255,255);
	m_colorArrayDefault[6][12] = RGB(255,255,0);
	m_colorArrayDefault[6][13] = RGB(255,0,0);
	m_colorArrayDefault[6][14] = RGB(198,195,198);
	m_colorArrayDefault[6][15] = RGB(0,255,255);
	m_colorArrayDefault[6][16] = RGB(0,130,132);
	m_colorArrayDefault[6][17] = RGB(198,195,198);
	m_colorArrayDefault[6][18] = RGB(0,0,132);
	m_colorArrayDefault[6][19] = RGB(0,0,132);
	m_colorArrayDefault[6][20] = RGB(255,255,255);
	m_colorArrayDefault[6][21] = RGB(255,255,255);
	//����
	m_colorArrayDefault[7][0] = RGB(198,233,198);
	m_colorArrayDefault[7][1] = RGB(0,0,0);
	m_colorArrayDefault[7][2] = RGB(0,130,132);
	m_colorArrayDefault[7][3] = RGB(255,0,255);
	m_colorArrayDefault[7][4] = RGB(0,130,0);
	m_colorArrayDefault[7][5] = RGB(0,0,132);
	m_colorArrayDefault[7][6] = RGB(0,0,255);
	m_colorArrayDefault[7][7] = RGB(0,0,255);
	m_colorArrayDefault[7][8] = RGB(0,130,132);
	m_colorArrayDefault[7][9] = RGB(132,130,0);
	m_colorArrayDefault[7][10] = RGB(255,0,255);
	m_colorArrayDefault[7][11] = RGB(132,0,0);
	m_colorArrayDefault[7][12] = RGB(0,130,0);
	m_colorArrayDefault[7][13] = RGB(255,0,0);
	m_colorArrayDefault[7][14] = RGB(0,0,132);
	m_colorArrayDefault[7][15] = RGB(0,130,0);
	m_colorArrayDefault[7][16] = RGB(0,255,255);
	m_colorArrayDefault[7][17] = RGB(0,130,132);
	m_colorArrayDefault[7][18] = RGB(198,223,198);
	m_colorArrayDefault[7][19] = RGB(198,223,198);
	m_colorArrayDefault[7][20] = RGB(0,0,0);
	m_colorArrayDefault[7][21] = RGB(0,0,0);
	//ǳ��
	m_colorArrayDefault[8][0] = RGB(198,195,198);
	m_colorArrayDefault[8][1] = RGB(0,0,0);
	m_colorArrayDefault[8][2] = RGB(132,130,132);
	m_colorArrayDefault[8][3] = RGB(255,0,255);
	m_colorArrayDefault[8][4] = RGB(0,130,0);
	m_colorArrayDefault[8][5] = RGB(132,0,0);
	m_colorArrayDefault[8][6] = RGB(0,0,255);
	m_colorArrayDefault[8][7] = RGB(0,0,255);
	m_colorArrayDefault[8][8] = RGB(132,130,132);
	m_colorArrayDefault[8][9] = RGB(255,0,255);
	m_colorArrayDefault[8][10] = RGB(132,0,0);
	m_colorArrayDefault[8][11] = RGB(0,0,132);
	m_colorArrayDefault[8][12] = RGB(132,0,132);
	m_colorArrayDefault[8][13] = RGB(255,0,255);
	m_colorArrayDefault[8][14] = RGB(0,0,0);
	m_colorArrayDefault[8][15] = RGB(0,130,0);
	m_colorArrayDefault[8][16] = RGB(165,203,247);
	m_colorArrayDefault[8][17] = RGB(132,130,132);
	m_colorArrayDefault[8][18] = RGB(198,195,198);
	m_colorArrayDefault[8][19] = RGB(198,195,198);
	m_colorArrayDefault[8][20] = RGB(0,0,0);
	m_colorArrayDefault[8][21] = RGB(0,0,0);
	//�����
	m_colorArrayDefault[9][0] = RGB(230,239,238);
	m_colorArrayDefault[9][1] = RGB(13,114,128);
	m_colorArrayDefault[9][2] = RGB(128,128,128);
	m_colorArrayDefault[9][3] = RGB(206,49,33);
	m_colorArrayDefault[9][4] = RGB(37,101,50);
	m_colorArrayDefault[9][5] = RGB(64,128,128);
	m_colorArrayDefault[9][6] = RGB(115,206,214);
	m_colorArrayDefault[9][7] = RGB(193,43,57);
	m_colorArrayDefault[9][8] = RGB(193,156,57);
	m_colorArrayDefault[9][9] = RGB(177,71,143);
	m_colorArrayDefault[9][10] = RGB(0,128,0);
	m_colorArrayDefault[9][11] = RGB(128,128,128);
	m_colorArrayDefault[9][12] = RGB(0,64,128);
	m_colorArrayDefault[9][13] = RGB(206,49,33);
	m_colorArrayDefault[9][14] = RGB(91,91,91);
	m_colorArrayDefault[9][15] = RGB(37,101,50);
	m_colorArrayDefault[9][16] = RGB(13,114,128);
	m_colorArrayDefault[9][17] = RGB(115,206,214);
	m_colorArrayDefault[9][18] = RGB(230,239,238);
	m_colorArrayDefault[9][19] = RGB(230,239,238);
	m_colorArrayDefault[9][20] = RGB(13,114,128);
	m_colorArrayDefault[9][21] = RGB(13,114,128);
	//��ҹ��
	m_colorArrayDefault[10][0] = RGB(212,212,212);
	m_colorArrayDefault[10][1] = RGB(6,6,6);
	m_colorArrayDefault[10][2] = RGB(60,60,60);
	m_colorArrayDefault[10][3] = RGB(174,0,0);
	m_colorArrayDefault[10][4] = RGB(60,60,60);
	m_colorArrayDefault[10][5] = RGB(0,85,0);
	m_colorArrayDefault[10][6] = RGB(131,131,131);
	m_colorArrayDefault[10][7] = RGB(193,43,57);
	m_colorArrayDefault[10][8] = RGB(177,71,143);
	m_colorArrayDefault[10][9] = RGB(181,174,113);
	m_colorArrayDefault[10][10] = RGB(0,128,0);
	m_colorArrayDefault[10][11] = RGB(27,30,73);
	m_colorArrayDefault[10][12] = RGB(0,64,128);
	m_colorArrayDefault[10][13] = RGB(174,0,0);
	m_colorArrayDefault[10][14] = RGB(87,87,87);
	m_colorArrayDefault[10][15] = RGB(0,85,0);
	m_colorArrayDefault[10][16] = RGB(221,221,221);
	m_colorArrayDefault[10][17] = RGB(192,192,192);
	m_colorArrayDefault[10][18] = RGB(231,231,231);
	m_colorArrayDefault[10][19] = RGB(234,234,234);
	m_colorArrayDefault[10][20] = RGB(6,6,6);
	m_colorArrayDefault[10][21] = RGB(6,6,6);
	//Ѥ�ý�
	m_colorArrayDefault[11][0] = RGB(255,252,237);
	m_colorArrayDefault[11][1] = RGB(115,41,8);
	m_colorArrayDefault[11][2] = RGB(156,82,41);
	m_colorArrayDefault[11][3] = RGB(174,0,0);
	m_colorArrayDefault[11][4] = RGB(0,85,0);
	m_colorArrayDefault[11][5] = RGB(214,173,74);
	m_colorArrayDefault[11][6] = RGB(107,25,33);
	m_colorArrayDefault[11][7] = RGB(193,43,57);
	m_colorArrayDefault[11][8] = RGB(181,174,113);
	m_colorArrayDefault[11][9] = RGB(177,71,143);
	m_colorArrayDefault[11][10] = RGB(0,128,0);
	m_colorArrayDefault[11][11] = RGB(34,149,155);
	m_colorArrayDefault[11][12] = RGB(11,11,221);
	m_colorArrayDefault[11][13] = RGB(174,0,0);
	m_colorArrayDefault[11][14] = RGB(115,41,8);
	m_colorArrayDefault[11][15] = RGB(0,85,0);
	m_colorArrayDefault[11][16] = RGB(222,197,123);
	m_colorArrayDefault[11][17] = RGB(223,216,162);
	m_colorArrayDefault[11][18] = RGB(255,252,237);
	m_colorArrayDefault[11][19] = RGB(239,230,189);
	m_colorArrayDefault[11][20] = RGB(115,41,8);
	m_colorArrayDefault[11][21] = RGB(115,41,8);
	//����õ��
	m_colorArrayDefault[12][0] = RGB(250,243,243);
	m_colorArrayDefault[12][1] = RGB(153,37,28);
	m_colorArrayDefault[12][2] = RGB(244,101,70);
	m_colorArrayDefault[12][3] = RGB(239,58,33);
	m_colorArrayDefault[12][4] = RGB(37,99,64);
	m_colorArrayDefault[12][5] = RGB(245,69,69);
	m_colorArrayDefault[12][6] = RGB(192,85,80);
	m_colorArrayDefault[12][7] = RGB(186,33,71);
	m_colorArrayDefault[12][8] = RGB(155,148,81);
	m_colorArrayDefault[12][9] = RGB(177,71,143);
	m_colorArrayDefault[12][10] = RGB(128,64,0);
	m_colorArrayDefault[12][11] = RGB(34,149,155);
	m_colorArrayDefault[12][12] = RGB(11,11,221);
	m_colorArrayDefault[12][13] = RGB(239,58,33);
	m_colorArrayDefault[12][14] = RGB(112,112,112);
	m_colorArrayDefault[12][15] = RGB(37,99,64);
	m_colorArrayDefault[12][16] = RGB(232,216,215);
	m_colorArrayDefault[12][17] = RGB(221,197,196);
	m_colorArrayDefault[12][18] = RGB(250,243,243);
	m_colorArrayDefault[12][19] = RGB(254,254,254);
	m_colorArrayDefault[12][20] = RGB(153,37,28);
	m_colorArrayDefault[12][21] = RGB(153,37,28);
*/
	SysFile.Write(m_colorArrayDefault,sizeof(COLORREF)*13*34);//Marenan
//	SysFile.Seek(40,CFile::current);
    SysFile.Write(m_nColumnWidth,sizeof(int)*MAXCOLUME);

	//end
    SysFile.Close();
	


	CTaiKlineFileKLine::CloseAll();
	CTaiKlineFileHS::CloseAll();

	CDocument::OnCloseDocument();
}


//*******************************��ʼ�����ݴ���*********************************************
/////////////////////////////////////////////////////////////////////////////
//      �� �� ����Init_dat() 
//      ����˵����������ʼ������ ����ʼ�������뿪����ʱ�䣬����ʼ���ڴ���� 
//      ��ڲ������������
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiShanDoc::Init_dat()                                            //��ʼ������ʱ�����������ݱ������   
{
	FILE *inq;
	int shanghai_shenzhen,to_find;
	char str[80];
	inq=_fsopen( "Stocktime.dat", "rt", SH_DENYNO );
	if ( inq!=NULL )
	{
		shanghai_shenzhen = -1;
		while ( fscanf( inq, "%s", str )>0 )
		{
			to_find = -1;
			if ( !strcmpi( str, "[ShangHai]" )  )
				shanghai_shenzhen = 0;
			else if ( !strcmpi( str, "[ShenZhen]" ) )
				shanghai_shenzhen = 1;
			else if ( !strcmpi( str, "[ShenZhenEB]"))
				shanghai_shenzhen = 2;
			else if ( !strcmpi( str, "[ShangHai]" ) ||!strcmpi( str, "[ShenZhen]" ) ||!strcmpi( str, "[ShenZhenEB]" ))
				shanghai_shenzhen = -1;
			else if ( !strcmpi( str, "start_A_hour=" ) )
				to_find = 0;
			else if ( !strcmpi( str, "start_A_min=" ) )
				to_find = 1;
			else if ( !strcmpi( str, "end_A_hour=" ) )
				to_find = 2;
			else if ( !strcmpi( str, "end_A_min=" ) )
				to_find = 3;
			else if ( !strcmpi( str, "start_B_hour=" ) )
				to_find = 4;
			else if ( !strcmpi( str, "start_B_min=" ) )
				to_find = 5;
			else if ( !strcmpi( str, "end_B_hour=" ) )
				to_find = 6;
			else if ( !strcmpi( str, "end_B_min=" ) )
				to_find = 7;
			if ( to_find>=0 && shanghai_shenzhen>=0 )
			{
				fscanf( inq, "%s", str );
				if ( to_find==0 )
					m_nDel_Start_A_hr[shanghai_shenzhen] = atoi( str );
				else if ( to_find==1 )
					m_nDel_Start_A_min[shanghai_shenzhen] = atoi( str );
				else if ( to_find==2 )
					m_nDel_End_A_hr[shanghai_shenzhen] = atoi( str );
				else if ( to_find==3 )
					m_nDel_End_A_min[shanghai_shenzhen] = atoi( str );
				else if ( to_find==4 )
					m_nDel_Start_B_hr[shanghai_shenzhen] = atoi( str );
				else if ( to_find==5 )
					m_nDel_Start_B_min[shanghai_shenzhen] = atoi( str );
				else if ( to_find==6 )
					m_nDel_End_B_hr[shanghai_shenzhen] = atoi( str );
				else if ( to_find==7 )
					m_nDel_End_B_min[shanghai_shenzhen] = atoi( str );
			}
		}
		fclose(inq);
		m_nMaxMaxANT=0;
		for ( int i=0; i<3; i++ )
		{
			m_nANT[i]=0;
			m_nOldANT[i]=0;
			m_nDel_Half_ANT[i] = (m_nDel_End_A_hr[i]-m_nDel_Start_A_hr[i])*60-m_nDel_Start_A_min[i]+m_nDel_End_A_min[i]-1;
			if ( m_nDel_Start_B_hr[i]>=0 )
				m_nMaxANT[i] = m_nDel_Half_ANT[i]+((m_nDel_End_B_hr[i]-m_nDel_Start_B_hr[i])*60-m_nDel_Start_B_min[i]+m_nDel_End_B_min[i]);
			if ( m_nMaxANT[i]>m_nMaxMaxANT )
				m_nMaxMaxANT = m_nMaxANT[i];
		}
	}
	else
	{
		for(int i=0;i<3;i++)
		{
			m_nANT[i]=0;
			m_nOldANT[i]=0;
			m_nMaxANT[i]=239;
			m_nDel_Half_ANT[i]=119;
			m_nDel_Start_A_hr[i]=9;
			m_nDel_Start_A_min[i]=30;
			m_nDel_Start_B_hr[i]=13;
			m_nDel_Start_B_min[i]=0;
			m_nDel_End_A_hr[i]=11;
			m_nDel_End_A_min[i]=30;
			m_nDel_End_B_hr[i]=15;
			m_nDel_End_B_min[i]=0;
		}
			m_nMaxMaxANT=239;
	}
    for(int i=0 ;i<3 ;i++)            //���㿪�з�����
	{
		this->m_lStartMinA[i]=m_nDel_Start_A_hr[i]*60 + m_nDel_Start_A_min[i];
		this->m_lEndMinA[i]=m_nDel_End_A_hr[i]*60 + m_nDel_End_A_min[i];
		this->m_lStartMinB[i]=m_nDel_Start_B_hr[i]*60 + m_nDel_Start_B_min[i];
		this->m_lEndMinB[i]=m_nDel_End_A_hr[i]*60 + m_nDel_End_A_min[i];
	}
	m_nMaxMaxANT++;
    Chk_Ystc();                                            //�������������ݱ������
}
/////////////////////////////////////////////////////////////////////////////
//      �� �� ����Chk_Ystc() 
//      ����˵������⴦�����ڣ���ȷ�������Ƿ�����ʼ������ 
//      ��ڲ������������
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiShanDoc::Chk_Ystc()                                            //�������Ƿ������ݱ���
{
	FILE *in;
	long day;
	
	if( (in=_fsopen(g_realtime,"rb",SH_DENYNO))!=NULL)
	{  
		fseek(in,4,SEEK_SET);
		if ((fread(&day,4,1,in))>0)
		{
			if (m_lDay<=day)
				m_nHave_Olddata=1;
			else
			{
				m_nHave_Olddata=0;
			}
		}
		fclose(in);
	}
	else
		m_nHave_Olddata=0;
}
/////////////////////////////////////////////////////////////////////////////
//      �� �� ����chk_date() 
//      ����˵�������㵱�տ������� 
//      ��ڲ������������
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiShanDoc::chk_date()                      //�õ���������  
{
        CTime m_Time = CTime::GetCurrentTime();
        m_lDay=((long)m_Time.GetYear())*10000L+(long)(m_Time.GetDay())+(long)(m_Time.GetMonth())*100;
}
/////////////////////////////////////////////////////////////////////////////
//      �� �� ����GetStocktime() 
//      ����˵��������ϵͳʱ���뻦��з����� 
//      ��ڲ���������ģʽ MODE=0 ����ϵͳʱ�� MODE=1 ������֤������ MODE=2 ������֤������
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
int CTaiShanDoc::GetStocktime(int mode)           //�õ�����ʱ���뿪�е����ڷ�����
{

  int tmp;
  struct tm* osTime;                            // A pointer to a structure containing time 
  struct tm  tmTime;                           
  
  CTime m_Time = CTime::GetCurrentTime();
  osTime = m_Time.GetLocalTm( &tmTime );
  //osTime = m_Time.GetLocalTm( NULL );//Marenan Modify

  if (mode==0)
     tmp=osTime->tm_hour *60 + osTime->tm_min ;
  else 
  {                                                                      //�õ����翪�з�����
	  if (osTime->tm_hour < m_nDel_Start_B_hr[mode-1] || m_nDel_Start_B_hr[mode-1]<0)
	  {   
           tmp=(osTime->tm_hour -m_nDel_Start_A_hr[mode-1])*60 + osTime->tm_min  - m_nDel_Start_A_min[mode-1] ;
           if (tmp>m_nDel_Half_ANT[mode-1])                                 //����Ƿ�Ϊ�������ʱ��
               tmp=m_nDel_Half_ANT[mode-1];
	  }
	  else 
	  {                                                                   //�õ����翪�з�����
           tmp=(osTime->tm_hour - m_nDel_Start_B_hr[mode-1])*60+osTime->tm_min - m_nDel_Start_B_min[mode-1];
           if (tmp<0)
               tmp=m_nDel_Half_ANT[mode-1];                                  //����Ƿ�Ϊ�������ʱ��
           else
		   {
               tmp+=(m_nDel_Half_ANT[mode-1]+1);
               if (tmp>m_nMaxANT[mode-1] )                                    //����Ƿ�Ϊ�������ʱ��
                   tmp=m_nMaxANT[mode-1] ;
		   }
	  }
  }
  m_nNowHour=osTime->tm_hour ;                                               //���е�ʱСʱ��   
  m_nNowMin=osTime->tm_min ;                                                 //���е�ʱ������
  m_nNowSec=osTime->tm_sec ;                                                 //���е�ʱ���� 

  if (tmp<-88)                                    
    tmp=-88;
  return(tmp);
}
/////////////////////////////////////////////////////////////////////////////
//      �� �� ����Init_EveryDay() 
//      ����˵�����������ݿ��ʼ������ 
//      ��ڲ������������
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiShanDoc::Init_EveryDay()                                     //ÿ�����ݿ��ʼ������ 
{
	int tmp;
	tmp=GetStocktime(0) ;       
	try
	{
	//����Ŀǰ������  
    if(m_week.tm_wday==0||m_week.tm_wday==6)
	{
         Init_StockData(2);                                           //װ���������� 
		//�𶯹�Ʊ�������ϵͳ
		((CMainFrame *)(AfxGetApp()->m_pMainWnd))->HqStock_Init();
	}
	else if(m_nHave_Olddata==1)                                               //��ʾ�����ѳ�ʼ��
	{
         Init_StockData(1);                                           //װ���������� 
	}
	//��ʾ����û�г�ʼ�������ѿ���
	else if (tmp >= this->m_lStartMinA[0]|| tmp >= this->m_lStartMinA [1])   //
	{
		ClearRealData();/*
 	     Init_StockData(0);
		 this->m_ManagerBlockData.GetBlockHeadPoint(pStockTypeHead);
		 pStockTypeHead->m_lLastTime=0;*/
	}
	//��ʾ����û�г�ʼ��������û�п���,�����ʱ��Ϊ5�����ڣ�ֱ�ӽ����ʼ��
    else if ((tmp >= this->m_lStartMinA [0] - 5 ) || (tmp >=this->m_lStartMinA [1]  - 5 ) )
	{
		ClearRealData();/*
 	     Init_StockData(0);
		 this->m_ManagerBlockData.GetBlockHeadPoint(pStockTypeHead);
		 pStockTypeHead->m_lLastTime=0;*/
	}
	//��ʾ����û�г�ʼ��������û�п���,�����ʱ��Ϊ5�������ϵĴ���
	else if(tmp < this->m_lStartMinA[0] - 5 || tmp < this->m_lStartMinA[1] - 5 )   //
	{
		int rtn = MessageBox(NULL,"��������ʱ������","����",MB_YESNO|MB_ICONWARNING);
		if(rtn==6)
		{
			ClearRealData();/*
           Init_StockData(0);                                //�������ݳ�ʼ������
		   this->m_ManagerBlockData.GetBlockHeadPoint(pStockTypeHead);
		   pStockTypeHead->m_lLastTime=0;*/
		}
		else
		{
		   Init_StockData(2);                                //װ����������
		}
	}
	}
	catch(...)
	{
	}

	CheckKind();
	this->m_sharesInformation .AddStocksIfEmpty ();
}
/////////////////////////////////////////////////////////////////////////////
//      �� �� ����Init_StockData()
//      ����˵������Ʊ�������ݳ�ʼ�������ָ��ֲ�ͬģʽ���������ݿ⴦������
//                ���ݽ��������鴦���߳�
//      ��ڲ�������ʼ������  MODE=0  ��3 �����ʼ��������MODE = 1 װ��������������
//                 MODE = 2 װ��������������
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiShanDoc::Init_StockData(int mode)                                          //��ʼ����Ʊ�������� 
{

    LoadStockData(mode) ;                             //���ݲ�ͬ��������ͬ��ʼ������

	if(mode == 2) 		//��MODE=2ʱΪװ���������ݣ����𶯹�Ʊ���鴦���߳�
	{
		m_nANT[0]=m_nOldANT[0];
		m_nANT[1]=m_nOldANT[1];
		m_nANT[2]=m_nOldANT[2];
		this->m_bInitDone =FALSE;
        return ;
	}
	for(int i =0 ;i<3;i++)                            //����Ŀǰ���з����� 
	{
		m_nANT[i]=GetStocktime(i + 1 ) ;
		if(m_nANT[i] <0 )
			m_nANT[i]=0;
	}

	//�𶯹�Ʊ�������ϵͳ
	if(!m_bCloseReceiver)
	{
      ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->HqStock_Init();
	}
	//���õ��������Ѵ����־
	this->m_bInitDone =TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//      �� �� ����GetStockCount() 
//      ����˵����ȡ�õ��컦�������ܹ�Ʊ�� 
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiShanDoc::GetStockCount()
{
//    ((CMainFrame *)(AfxGetApp()->m_pMainWnd))->m_StockDriverDll.GetStockDrvInfo(RI_STKNUM,&m_StockCount);
}

/////////////////////////////////////////////////////////////////////////////
//      �� �� ����LoadStockData() 
//      ����˵�������ݲ�ͬ���ͽ���װ�����ݣ� 
//      ��ڲ�������ʼ������  MODE=0  ��3 �����ʼ��������MODE = 1 װ��������������
//                 MODE = 2 װ��������������
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiShanDoc::LoadStockData(int mode){
	CString path;
	::GetCurrentDirectory(MAX_PATH,path.GetBuffer(MAX_PATH));
	path.ReleaseBuffer();
//*******************�򿪶Ի���*********************************************************************** 
	if(mode != 3) 	//MODE=3 ��ʾ��Ʊ���ձ���װ����
	{
		m_sharesInformation.InitRealTimeData(path);
		m_ManagerBlockData.InitBlockData(path);
        InitChooseAndStockType();
	}
	switch(mode)
	{
	case 0:                                      //������ʼ������
	case 3:
            CreateFileData(mode); 
			m_sharesInformation.ClearRealDataMinute();

			//  add, to initiate the count of hs of each stock
			CTaiKlineFileHS::m_pFileHsSh->ZeroHsCountEach();
			CTaiKlineFileHS::m_pFileHsSz->ZeroHsCountEach();
			CTaiTestTreeView::CheckNewsInfomationFileTime();
		break;
	case 1:                                      //װ�������������� 
		    LoadFileData(mode);                  //װ�����ݿ��ļ����� 
			//m_sharesInformation.ClearRealDataMinute();
			//CTaiKlineFileHS::m_pFileHsSh->ZeroHsCountEach();
			//CTaiKlineFileHS::m_pFileHsSz->ZeroHsCountEach();
		break;
	case 2:                                      //װ��������������
            LoadFileData(mode);                  //װ�����ݿ��ļ����� 
		break;
	}
//******************************************************************************
/*	m_sharesInformation.InitBaseInfoData( );
    FILE *fp;
	char zqdm[2000];
	fp=fopen("StockInfo.txt","wt");
			sprintf(zqdm,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
	                  "��Ʊ����",
	                  "�ܹɱ�(���)",
	                  "���ҹ�(���)",
	                  "�����˷��˹�(���)",
	                  "���˹�(���)",
	                  "ְ����(���)",
	                  "����A��(���)",
	                  "��ͨA��(���)",
	                   "�ȹ�(���)",
	                   "B��(���)",
	                  "ת���(���)",
	                  "���ʲ�(��Ԫ)",
	                "�����ʲ�(��Ԫ)",
	                "����Ͷ��(��Ԫ)",
	                "�̶��ʲ�(��Ԫ)",
	                "�����ʲ�(��Ԫ)",
	                 "������ծ(��Ԫ)",
	                 "���ڸ�ծ(��Ԫ)",
	                 "�ɶ�Ȩ��(��Ԫ)",
	                 "�ʱ�������(��Ԫ)",
	                "ӯ�๫����(��Ԫ)",
	                "ÿ�ɾ�ֵ(Ԫ)",
	                "�ɶ�Ȩ�����(%)",
	                 "ÿ�ɹ�����(Ԫ)",
	                "��Ӫҵ������(��Ԫ)",
	                "��Ӫҵ������(��Ԫ)",
	                "����ҵ������(��Ԫ)",
	                "�����ܶ�(��Ԫ)",
	               "������(��Ԫ)",
	               "δ��������(��Ԫ)",
	               "ÿ������(Ԫ)",
	               "���ʲ�������(%)",
	               "ÿ��δ��������(Ԫ)"
				);
			fwrite(zqdm,1,strlen(zqdm),fp);

	for(int i=0;i<8;i++)
	{
		if(i==0||i==3)
			continue;
		int temp=m_sharesInformation.GetStockTypeCount(i);
		for(int j=0;j<temp;j++)
		{
			CReportData *pCdat;
			char zqdmStr[2000];
            m_sharesInformation.GetStockItem(i, j, pCdat);
	        if(pCdat->pBaseInfo->Symbol==NULL)
				continue;
				
			sprintf(zqdmStr,"%s\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\t%6.2f\n",
	            pCdat->pBaseInfo->Symbol,     //��Ʊ����
	                  pCdat->pBaseInfo->zgb,     //�ܹɱ�(���)
	                  pCdat->pBaseInfo->gjg,     //���ҹ�(���)
	               pCdat->pBaseInfo->fqrfrg,     //�����˷��˹�(���)
	                  pCdat->pBaseInfo->frg,     //���˹�(���)
	                  pCdat->pBaseInfo->zgg,     //ְ����(���)
	                 pCdat->pBaseInfo->gzAg,    //����A��(���)
	                 pCdat->pBaseInfo->ltAg,    //��ͨA��(���)
	                   pCdat->pBaseInfo->Hg,      //�ȹ�(���)
	                   pCdat->pBaseInfo->Bg,      //B��(���)
	                  pCdat->pBaseInfo->zpg,     //ת���(���)
	                  pCdat->pBaseInfo->zzc,     //���ʲ�(��Ԫ)
	                 pCdat->pBaseInfo->ldzc,    //�����ʲ�(��Ԫ)
	                 pCdat->pBaseInfo->cqtz,    //����Ͷ��(��Ԫ)
	                 pCdat->pBaseInfo->gdzc,    //�̶��ʲ�(��Ԫ)
	                 pCdat->pBaseInfo->wxzc,    //�����ʲ�(��Ԫ)
	                 pCdat->pBaseInfo->ldfz,    //������ծ(��Ԫ)
	                 pCdat->pBaseInfo->cqfz,    //���ڸ�ծ(��Ԫ)
	                 pCdat->pBaseInfo->ggqy,    //�ɶ�Ȩ��(��Ԫ)
	                 pCdat->pBaseInfo->zbgjj,   //�ʱ�������(��Ԫ)
	                 pCdat->pBaseInfo->yygjj,   //ӯ�๫����(��Ԫ)
	                 pCdat->pBaseInfo->mgjz,    //ÿ�ɾ�ֵ(Ԫ)
	                pCdat->pBaseInfo->gdqybl,  //�ɶ�Ȩ�����(%)
	                 pCdat->pBaseInfo->mggjj,   //ÿ�ɹ�����(Ԫ)
	                pCdat->pBaseInfo->zyywsr,  //��Ӫҵ������(��Ԫ)
	                pCdat->pBaseInfo->zyywlr,  //��Ӫҵ������(��Ԫ)
	                pCdat->pBaseInfo->qtywlr,  //����ҵ������(��Ԫ)
	                  pCdat->pBaseInfo->lrze,    //�����ܶ�(��Ԫ)
	                   pCdat->pBaseInfo->jlr,     //������(��Ԫ)
	                 pCdat->pBaseInfo->wfplr,   //δ��������(��Ԫ)
	                  pCdat->pBaseInfo->mgsy,    //ÿ������(Ԫ)
	                pCdat->pBaseInfo->jzcsyl,  //���ʲ�������(%)
	               pCdat->pBaseInfo->mgwfplr //ÿ��δ��������(Ԫ)
				);
			fwrite(zqdmStr,1,strlen(zqdmStr),fp);
		}
	}
fclose(fp);
*/
}


//////////////////////////////////////////////////////////////////////////////////////////
//      �� �� ����GetStockDay()
//      ����˵������������ 
//      ��ڲ�����time_t UTCʱ��ֵ
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
long CTaiShanDoc::GetStockDay(time_t time )              //��Ʊ�������             
{
  int tmp;
  if(time<=0)
	  return 0;
  CTime m_Time = time;
  tmp=((long)m_Time.GetYear())*10000L+(long)(m_Time.GetDay())+(long)(m_Time.GetMonth())*100;
  return(tmp);
}

//////////////////////////////////////////////////////////////////////////////////////////
//      �� �� ����GetStockMinute()
//      ����˵������������ 
//      ��ڲ�����time_t UTCʱ��ֵ
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
__int64 CTaiShanDoc::GetStockMinute(time_t time )              //��Ʊ�������             
{
	__int64 tmp;
	if(time<=0)
		return 0;
	CTime m_Time = time;
	tmp=((__int64)m_Time.GetYear())*100000000L +((__int64)(m_Time.GetMonth()))*1000000 + ((__int64)(m_Time.GetDay())) * 10000 + ((__int64)(m_Time.GetHour())) * 100 + (__int64)(m_Time.GetMinute());
	return(tmp);
}
/////////////////////////////////////////////////////////////////////////////
//      �� �� ����CreateFileData() 
//      ����˵�����������ݴ���ռ� 
//      ��ڲ����������ļ�ģʽ MODE =0 ��ʾȫ�����ݿ�
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiShanDoc::CreateFileData(int mode)                                      //�����������ݿ��ļ� 
{
//****************** news file ******************************************************
	FILE * fp;
    fp=_fsopen("news\\news.log","w+b",SH_DENYNO);
    fclose(fp);
	CString FileName;
	FileName.Format ("news\\shanghai\\%d.dat",m_lDay);
	fp=_fsopen(FileName.GetBuffer(0),"w+b",SH_DENYNO);
	fclose(fp);
	FileName.Format ("news\\shenzhen\\%d.dat",m_lDay);
	fp=_fsopen(FileName.GetBuffer(0),"w+b",SH_DENYNO);
	fclose(fp);

	FileName.Format ("news\\cj\\%d.dat",m_lDay);
	fp=_fsopen(FileName.GetBuffer(0),"w+b",SH_DENYNO);
	fclose(fp);
//******************STOCKNOW******************************************************
	REALDATA *RealFileHead;                 //�ڴ�ӳ���ļ�ͷ���ṹ  
    this->m_sharesInformation.SetRealDataHead(RealFileHead);
	RealFileHead->Day =m_lDay;
	RealFileHead->OldANT[0]=m_nANT[0]; 
	RealFileHead->OldANT[1]=m_nANT[1];
	RealFileHead->OldANT[2]=m_nANT[2];
	RealFileHead->CloseWorkDone= m_bCloseWorkDone=FALSE;
}
/////////////////////////////////////////////////////////////////////////////
//      �� �� ����SaveFileData() 
//      ����˵�������浱���������� 
//      ��ڲ����������ļ�ģʽ 
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiShanDoc::SaveFileData()
{
	REALDATA *RealFileHead;                 //�ڴ�ӳ���ļ�ͷ���ṹ  
	chk_date();//�õ���������
    this->m_sharesInformation.SetRealDataHead(RealFileHead);
	RealFileHead->Day =m_lDay;
	RealFileHead->OldANT[0]=m_nOldANT[0]; 
	RealFileHead->OldANT[1]=m_nOldANT[1];
	RealFileHead->CloseWorkDone= m_bCloseWorkDone;
    RealFileHead->FileExitDone=12345678;  
}

/////////////////////////////////////////////////////////////////////////////
//      �� �� ����LoadFileData() 
//      ����˵����װ������������������� 
//      ��ڲ����������ļ�ģʽ MODE
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiShanDoc::LoadFileData(int mode)                                        //װ�����ݿ��ļ����� 
{
	REALDATA *RealFileHead;                 //�ڴ�ӳ���ļ�ͷ���ṹ  
    RealFileHead=this->m_sharesInformation.GetRealDataHead();
	m_lDay=RealFileHead->Day ;
	m_nOldANT[0]=RealFileHead->OldANT[0];
	m_nOldANT[1]=RealFileHead->OldANT[1];
	m_nOldANT[2]=RealFileHead->OldANT[2];
	//m_bCloseWorkDone=FALSE;
	m_bCloseWorkDone=RealFileHead->CloseWorkDone;
	
	if(m_week.tm_wday==0||m_week.tm_wday==6)
	{
		m_nANT[0]=m_nOldANT[0];
		m_nANT[1]=m_nOldANT[1];
		m_nANT[2]=m_nOldANT[2];
	}		
//*******************read file name from news.log ************************************
	CFile fl;
	if(fl.Open ("news\\news.log",CFile::modeCreate|CFile::modeNoTruncate))
		fl.Close();
    FILE * fp;

	fp=_fsopen("news\\news.log","r+b",SH_DENYNO);
	fseek(fp,0,SEEK_SET);
    while(!feof(fp)&&!ferror(fp))
	{
        CString FileName;
		char str[251]="";
		fread(str,250,1,fp);
		FileName=str;
        if(strlen(str)>0)  
		this->m_NewsFileNameMap[FileName]=FileName;
	}
    fclose(fp);
}
//����5���Ӽ��㣬��������ָ����������㵱ʱ���̷�����
/////////////////////////////////////////////////////////////////////////////
//      �� �� ����OnCalcHqDataProgress()
//      ����˵����������̸���ָ��(ÿ5����)
//      ��ڲ������������
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiShanDoc::OnCalcHqDataProgress()
{
#ifndef WIDE_NET_VERSION
	int tmp;
	int i;
//**************************����ָ�����ݼ���************************************************************
    this->m_sharesInformation.CalcIndexBuyAndSell();
    float rd_sh=0,rd_sz=0,rd_eb=0;
	if (m_nANT[0]>=0)
        rd_sh=OnCalcDpTidxdData(0) ;                          //�������ǿ��ָ��
	if (m_nANT[1]>=0)
        rd_sz=OnCalcDpTidxdData(1) ;                          //�������ǿ��ָ��
	//if (m_nANT[2]>=0)
 //       rd_eb=OnCalcDpTidxdData(2) ;                          //�������ǿ��ָ��

    if(!m_bOpenStockTypeDlg && m_nBlockCalcCounter % this->m_SystemInitData.BlockTime == 0)
	{
		if(InitSetp>3) 
		{
           int temp=GetStocktime(1);
		   if(temp<0) temp=0;

		   m_sharesInformation.UpdateCwData();
		   //m_ManagerBlockData.UpdateAllBlockStocks();
          
		   if(this->m_bInitDone && m_CalcBlockCounts != 0 && temp != this->m_nANT[0])
		   {
			   //m_ManagerBlockData.CalcBlockKlineData(); //�����鵱��ÿ���Ӱ��ָ��
			   m_CalcBlockCounts--; 
		   }
		   else
		   {
			   if (m_pBlockAnalysisView)
			   {
				   m_ManagerBlockData.CalcRealTimeBlockIndex();
				   m_pBlockAnalysisView->Invalidate();
			   }
		   }
		}
	}
	m_nBlockCalcCounter++;
	if(m_nBlockCalcCounter >= 100)
		m_nBlockCalcCounter = 0;

	//ͳ�����ݼ���
	CMainFrame* pFrameWnd=(CMainFrame*)AfxGetMainWnd();
	CView *pView = pFrameWnd->GetActiveFrame()->GetActiveView();
	if(pView != NULL && pView->IsKindOf(RUNTIME_CLASS(CTaiShanReportView)))
	{
		CTaiShanReportView* pReportView = (CTaiShanReportView*)pView;
		if(pReportView->GetFixedRowCount() > 1)
			StatisticStockData(m_nWeightType);
	}


	CReportData *p1A0001,*p2A01,*p2D01;
    m_sharesInformation.Lookup((CSharesCompute::GetIndexSymbol(0)).GetBuffer(0),p1A0001,SHZS);
    m_sharesInformation.Lookup((CSharesCompute::GetIndexSymbol(1)).GetBuffer(0),p2A01,SZZS);
	CString seb = CSharesCompute::GetIndexSymbol(2);
    m_sharesInformation.Lookup(seb.GetBuffer (0),p2D01,2);
	if(p1A0001!=NULL&&p2A01!=NULL)
	{
		if(p1A0001->nowp==0)
			((CMainFrame *)(AfxGetApp()->m_pMainWnd))->DisplayBargain(
			0,0,0,0,
			0,0,0,0,
			0,0,0,0,
			0.5 ,0.5 ,0);
		else if(p2D01!=NULL)
		{
			((CMainFrame *)(AfxGetApp()->m_pMainWnd))->DisplayBargain(
			p1A0001->nowp ,p1A0001->nowp - p1A0001->ystc ,p1A0001->totv,p1A0001->totp/10000,
			p2A01->nowp ,p2A01->nowp - p2A01->ystc,p2A01->totv ,p2A01->totp/100000000,
			p2D01->nowp ,p2D01->nowp - p2D01->ystc,p2D01->totv ,p2D01->totp/100000000,
			rd_sh/100,rd_sz/100,rd_eb/100);
		}else
		{
			((CMainFrame *)(AfxGetApp()->m_pMainWnd))->DisplayBargain(
			p1A0001->nowp ,p1A0001->nowp - p1A0001->ystc ,p1A0001->totv,p1A0001->totp/100000000,
			p2A01->nowp ,p2A01->nowp - p2A01->ystc,p2A01->totv ,p2A01->totp/100000000,
			0,0,0,0,
			rd_sh/100,rd_sz/100,rd_eb/100);
		}
	}
	//((CMainFrame *)(AfxGetApp()->m_pMainWnd))->DisplayRedGreen(rd_sh/100,rd_sz/100);

    CTime m_Time = CTime::GetCurrentTime();
	if(m_Time.GetDayOfWeek()==1||m_Time.GetDayOfWeek()==7   )
		return;
    long temp=((long)m_Time.GetYear())*10000L+(long)(m_Time.GetDay())+(long)(m_Time.GetMonth())*100;
	if(!this->m_bInitDone||m_lDay<temp)        //�жϵ����Ƿ�������ʼ��
	{
		tmp=GetStocktime(0) ;      //����Ŀǰʱ��
		if ((tmp > m_lStartMinA[0] - 5 )|| (tmp > m_lStartMinA[1] - 5 ) )  //ϵͳ�����ʼ������
		{
			BLOCKFILEHEAD *pStockTypeHead;
			this->m_bInitDone=TRUE;
			chk_date();
			Init_StockData(3);                                //�������ݳ�ʼ������
			this->m_ManagerBlockData.GetBlockHeadPoint(pStockTypeHead);

            m_nOldANT[0]=0;
			m_nOldANT[1]=0;
			m_nOldANT[2]=0;
			chk_date();
			m_bCloseWorkDone=FALSE;

		}else 
		{
			this->m_bInitDone=FALSE;
			return ;
		}
	}
	if(!m_bCloseWorkDone&&m_systemOption.autoclose)                                       //������ҵʱ��ȷ��  
	{
		tmp=GetStocktime(0) ;                    //����Ŀǰʱ��
		if(tmp == 920 )                          //�����Ƿ�Ϊ������ҵ
		{
			m_bCloseWorkDone=TRUE;
			StockCloseWork();                    //���ڵ���������ҵ
		}
	}
	for(i =0 ;i<3;i++)                           //�������ڷ�����
	{
		m_nANT[i]=GetStocktime(i+1) ;
		if(m_nANT[i] <0 )
			m_nANT[i]=0;
 		if(m_nANT[i]==m_nOldANT[i])
			return;
		else
		{                                                   //����ÿ���Ӵ���
           m_nOldANT[i]=m_nANT[i]; 
		}
	}
#endif
//************************************************************************************
}
/////////////////////////////////////////////////////////////////////////////
//      �� �� ���� OnCalcDpTidxdData()
//      ����˵���� ���ڼ������ǿ��ָ��
//      ��ڲ����� ���ڱ�ʾ֤ȯ�г� which_stk = 0 �Ϻ�, 1 ���� 
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
float CTaiShanDoc::OnCalcDpTidxdData(int which_stk)                           //�������ǿ��ָ��
{
    int i,count[240];
    float tmp[240],tmp1[240];

	for (i = 0;i < 240;i++)
	{
		count[i]=0;
		tmp[i]=0;
		tmp1[i]=0;
	}
    if(which_stk!=0 && which_stk!=1&&which_stk!=2)
       return 0;
	m_sharesInformation.CalcUpDown (which_stk);

	Rsdn1 **Nidx2;
	Tidxd **Tidx2;
	this->m_sharesInformation.GetIndexRsdn(Nidx2); 
    this->m_sharesInformation.GetIndexTidxd(Tidx2); 

	for (i = 0;i < m_nANT[which_stk];i++)
	{
		Nidx2[which_stk][i].rsn=Nidx2[which_stk][i].dnn=0;
		m_nRN[which_stk][i]=m_nRN1[which_stk][i]=m_nRD[which_stk][i]=m_nDN[which_stk][i]=m_nDN1[which_stk][i]=m_nCalStk[which_stk][i]=0;
	}
	int nKind[13] = {SHAG,SHBG,SHJJ,SHZQ,SHZZ,SHHG,SHETF,SHKF,SHQZ,SHOT,STOCKTYPENUM,STOCKTYPENUM,STOCKTYPENUM};
	if(which_stk==1)
	{
		nKind[0]=SZAG;
		nKind[1]=SZBG;
		nKind[2]=SZJJ;
		nKind[3]=SZZQ;
		nKind[4]=SZZZ;
		nKind[5]=SZHG;
		nKind[6]=SZETF;
		nKind[7]=SZKF;
		nKind[8]=SZQZ;
		nKind[9]=SZSB;
		nKind[10]=SZOT;
		nKind[11]=SZZXQY;
		nKind[12]=SZCYB;
//		Start=1;End=2;
	}
/*	else if(which_stk==1)
	{
		Start=4;End=5;
	}
	else
	{
		Start=9;End=9;
	}*/
    for(int k = 0;k<13;k++)
	{
		i = nKind[k];
		int temp=m_sharesInformation.GetStockTypeCount(i);
		for(int j=0;j<temp;j++)
		{
			CString str;
			CReportData * Cdat;
			m_sharesInformation.GetStockItem(i, j, Cdat);
			if(Cdat==NULL)
				continue;
		// skip no use stock
			for(int j = 0;j < m_nANT[which_stk];j++)
			{
				if (Cdat->ystc>0 && Cdat->m_Kdata1[j].Price >0)
				{
					tmp1[j]=(float)(Cdat->m_Kdata1[j].Price - Cdat->ystc)/(float)Cdat->ystc;
					if ( tmp1[j] < MAXRAISE )          //MAXRAISE=3.0
					{
						tmp[j] += tmp1[j];
						count[j]++;
					}

					if (Cdat->m_Kdata1[j].Price > Cdat->ystc)
						Nidx2[which_stk][j].rsn++;
					else if (Cdat->m_Kdata1[j].Price < Cdat->ystc)
						Nidx2[which_stk][j].dnn++;
				}
				if (Cdat->rdp[j]=='3' || Cdat->rdp[j]=='5')
					m_nRN[which_stk][j]++;
				else if (Cdat->rdp[j]=='4' || Cdat->rdp[j]=='6')
					m_nDN[which_stk][j]++;
				else if (Cdat->rdp[j]=='1')
					m_nRN1[which_stk][j]++;
				else if (Cdat->rdp[j]=='2')
					m_nDN1[which_stk][j]++;
				else if (Cdat->rdp[j]=='0')
					m_nRD[which_stk][j]++;

				m_nCalStk[which_stk][j]++;
			}
				//if (Cdat->ystc>0 && Cdat->nowp>0)
				//{
				//	tmp1=(float)(Cdat->nowp-Cdat->ystc)/(float)Cdat->ystc;
				//	if ( tmp1<MAXRAISE )          //MAXRAISE=3.0
				//	{
				//		tmp+=tmp1;
				//		count++;
				//	}

				//	if (Cdat->nowp>Cdat->ystc)
				//		Nidx2[which_stk][m_nANT[which_stk]].rsn++;
				//	else if (Cdat->nowp<Cdat->ystc)
				//		Nidx2[which_stk][m_nANT[which_stk]].dnn++;
				//}
				//if (Cdat->rdp=='3' || Cdat->rdp=='5')
				//	m_nRN[which_stk]++;
				//else if (Cdat->rdp=='4' || Cdat->rdp=='6')
				//	m_nDN[which_stk]++;
				//else if (Cdat->rdp=='1')
				//	m_nRN1[which_stk]++;
				//else if (Cdat->rdp=='2')
				//	m_nDN1[which_stk]++;
				//else if (Cdat->rdp=='0')
				//	m_nRD[which_stk]++;			
		}
	}
	for(int j = 0;j < m_nANT[which_stk];j++)
	{
		if(count[j])
		  tmp[j] = tmp[j]/(float)count[j]*10000;
		else
		  tmp[j] = 0;

		Tidx2[which_stk][j].sec5=(int)tmp[j];
		if(m_nCalStk[which_stk] > 0)
		{
		   Tidx2[which_stk][j].rp = (short)((float)(m_nRN[which_stk][j]*2+m_nRN1[which_stk][j]*3+m_nRD[which_stk][j])*100.0/(float)(m_nCalStk[which_stk][j]*3));
		   Tidx2[which_stk][j].dp = (short)((float)(m_nDN[which_stk][j]*2+m_nDN1[which_stk][j]*3+m_nRD[which_stk][j])*100.0/(float)(m_nCalStk[which_stk][j]*3));
		}
	}
	//if(count)
	//	tmp=tmp/(float)count*10000;
	//else
	//	tmp=0;

	//Tidx2[which_stk][m_nANT[which_stk]].sec5=(int)tmp;
	//if(m_nCalStk[which_stk]>0)
	//{
	//	Tidx2[which_stk][m_nANT[which_stk]].rp=(short)((float)(m_nRN[which_stk]*2+m_nRN1[which_stk]*3+m_nRD[which_stk])*100.0/(float)(m_nCalStk[which_stk]*3));
	//	Tidx2[which_stk][m_nANT[which_stk]].dp=(short)((float)(m_nDN[which_stk]*2+m_nDN1[which_stk]*3+m_nRD[which_stk])*100.0/(float)(m_nCalStk[which_stk]*3));
	//}
    if(Tidx2[which_stk][m_nANT[which_stk]].dp==0)
		return 50;
	return 100.0f*Tidx2[which_stk][m_nANT[which_stk]].rp/(Tidx2[which_stk][m_nANT[which_stk]].dp +Tidx2[which_stk][m_nANT[which_stk]].rp);

//	return 0;
}


/////////////////////////////////////////////////////////////////////////////
//      �� �� ����StockCloseWork() 
//      ����˵�������ڵ���������ҵ
//      ��ڲ������������
//      ���ڲ������޳�����
//      ���ز������޷�����
/////////////////////////////////////////////////////////////////////////////
void CTaiShanDoc::StockCloseWork()
{
	/*
#ifdef PRO
    DogAddr=0;
	DogBytes=9;
	char str[10];
    DogData=str;
    str[9]='\0';
	long RetCode=ReadDog();
    if(RetCode&&strcmp(str,"ZhengTong")!=0)
	{
	   AfxMessageBox("�뽫�������װ����������ҵ!");
	   return ;
    }
#endif
	*/
	CDiaCopy *pdlg;
	CString filedes;
	CString filesrc;
////////////////////////////////////////////////////////////////////
      int lastant=0;
	long m_totalCount;
	CString m_zqdm;
    struct tm when; 
	///////////////zjh//////////
#ifdef TEST
	CTime timenow = CTime::GetCurrentTime();
	if(((CWHApp *)AfxGetApp())->t.GetDay() != timenow.GetDay() )
	{
	  AfxMessageBox("�Ƿ�ʱ��ᵼ�����̴���",MB_OK|MB_ICONSTOP);
	  return;
	}
#endif
	//////////////////////////
	time_t now; 
	Rsdn1 **Nidx2;
	Tidxd **Tidx2;

	this->m_sharesInformation.GetIndexRsdn(Nidx2); 
    this->m_sharesInformation.GetIndexTidxd(Tidx2); 

	time( &now );
//	MSG message;
    when = *localtime( &now );
	if(when.tm_wday==0||when.tm_wday==6)
		return ;

//*******************�򿪶Ի���*********************************************************************** 
	CProgressDialog *dlg;
	dlg=new CProgressDialog;
	dlg->Create(IDD_DIALOG_PROGRESS,NULL);
    dlg->ShowWindow(SW_SHOW); 
	dlg->m_DialogDown =FALSE;

	CButton *m_pButton;
	m_pButton =(CButton *)dlg->GetDlgItem(IDOK); 
    m_pButton->EnableWindow(FALSE) ;

	dlg->SetWindowText(_T("��������������ҵ"));
//*******************�������ݸ���********************************************************************* 
	m_totalCount=m_sharesInformation.GetCount();
	if(m_totalCount<=0)
	{
		m_pButton->EnableWindow(TRUE) ;
		dlg->m_DialogDown =TRUE;
		dlg->ShowWindow(SW_HIDE);
		dlg->OnClose() ; 
		return;
	}
	long m_index=0;

	//  0621
	double ltpAll[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int i,j;
	//end 
    for(i=0;i<STOCKTYPENUM;i++)
	{
		int temp=m_sharesInformation.GetStockTypeCount(i);
		for(j=0;j<temp;j++)
		{
			CReportData *Cdat;
			CString StockId ; 
			Kline pIndexDay;
			KlineGeneral pStockDay;
			m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat==NULL)
				continue;
			StockId =Cdat->id ;

			//  0621
			try
			{
				if(Cdat->pBaseInfo)
				{
					if(i!=SHZS && i!=SZZS)
					{
						if(i == SHBG|| i == SZBG)
							ltpAll[i] += Cdat->pBaseInfo ->Bg*100  ;
						else if (i == SHAG|| i == SZAG|| i == SZZXQY)//|| i== SZQZ
							ltpAll[i] += Cdat->pBaseInfo ->ltAg*100 ;
					}
				}
			}
			catch(...)
			{
			}
			//end

			if(strcmp(Cdat->id,CSharesCompute::GetIndexSymbol(0))==0)                            //������̸����ǵ���
			{
				pIndexDay.day =now;
				pIndexDay.open =Cdat->opnp;
				pIndexDay.high=Cdat->higp ;
				pIndexDay.low =Cdat->lowp ;
				pIndexDay.close =Cdat->nowp;
				pIndexDay.vol =Cdat->totv;
 				pIndexDay.amount =Cdat->totp ;
		        pIndexDay.advance =Nidx2[0][m_nANT[0]].rsn;
			    pIndexDay.decline =Nidx2[0][m_nANT[0]].dnn;
			}
			else if(strcmp(Cdat->id,CSharesCompute::GetIndexSymbol(1))==0)
			{
				pIndexDay.day =now;
				pIndexDay.open =Cdat->opnp;
				pIndexDay.high=Cdat->higp ;
				pIndexDay.low =Cdat->lowp ;
				pIndexDay.close =Cdat->nowp;
				pIndexDay.vol =Cdat->totv;
				pIndexDay.amount =Cdat->totp ;
			    pIndexDay.advance =Nidx2[1][m_nANT[1]].rsn;
			    pIndexDay.decline =Nidx2[1][m_nANT[1]].dnn;
			}
			else
			{
				pStockDay.day =now;
				pStockDay.open =Cdat->opnp;
				pStockDay.high=Cdat->higp ;
				pStockDay.low =Cdat->lowp ;
				pStockDay.close =Cdat->nowp;
				pStockDay.vol =Cdat->totv;
				pStockDay.amount =Cdat->totp ;
				pStockDay.volPositive =Cdat->rvol;
				memcpy(&pIndexDay,&pStockDay,sizeof(Kline));
			}
           
			if(Cdat->kind!=SHZS&&Cdat->kind!=SZZS)
			{
				if( Cdat->nowp !=0&&Cdat->opnp!=0&&Cdat->totp!=0&&Cdat->totv!=0)
				{
					try
					{
					   CTaiKlineFileKLine::WriteKLineS(StockId,Cdat->kind,&pIndexDay,1,0);   //����K������
					}
					catch(...)
					{
					}
				}
			}
			else
			{
				if( Cdat->nowp !=0&&Cdat->opnp!=0)
				{
					try
					{
					   CTaiKlineFileKLine::WriteKLineS(StockId,Cdat->kind,&pIndexDay,1,0);   //����K������
					}
					catch(...)
					{
					}
				}
			}
	//************��ʾ��Ʊ������Ϣ**************************************************
			m_index++;
			m_zqdm.Format ("%s(%s)",Cdat->name ,Cdat->id);
			int spos=100*m_index /(m_totalCount+1)   ;
			dlg->DisplayZqxx(m_zqdm, spos); 
	//**************************************************************
/*			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
*/		}
	}

	//  0621
	try
	{
	for(i = 0;i<12;i=i+11)
	{
		for(int j=0;j<3;j++)
		{
			CReportData *Cdat;
			m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat==NULL)
				continue;
			if(!Cdat->pBaseInfo)
			{
				BASEINFO *pBaseItem;
				if(!m_sharesInformation.AddBaseinfoPoint(Cdat->id,i,pBaseItem))              //�ڻ������������ļ�������һ����������
				{
					AfxMessageBox("���ӹ�Ʊ�������Ͽռ�����ʱ����");
					continue;
//					return ;
				}
				Cdat->pBaseInfo=pBaseItem;
			}

			if(i==0 && j==0)
				Cdat->pBaseInfo ->ltAg = ltpAll[1] + ltpAll[2];
			else if(i==11 && j==0)
				Cdat->pBaseInfo ->ltAg = ltpAll[12] + ltpAll[13];
			else
				Cdat->pBaseInfo ->ltAg = ltpAll[i+j] ;
		}
	}
	}
	catch(...)
	{
	}
	//end 

//******************5���������ݸ���************************************************************************ 
	dlg->SetWindowText(_T("����5������������ҵ"));
	m_index=0;
    
	m_bCloseWorkDone=FALSE;
	Kline *MinKline = NULL;
	//Marenan -
	//HGLOBAL	hMem = GlobalAlloc( GPTR, (48)* sizeof( Kline) ) ;
	//LPVOID hp=GlobalLock(hMem);
	//if( hp )
	//    MinKline= (Kline *)hp;
	//else
	//    AfxMessageBox("can't alloc Buffers",MB_ICONSTOP) ; 
	//Marenan +
	MinKline = new Kline[48];
	if(NULL != MinKline)
	{
		for( i=0;i<STOCKTYPENUM;i++)
		{
			int temp=m_sharesInformation.GetStockTypeCount(i);
			for(int j=0;j<temp;j++)
			{
				CReportData *Cdat;
				CString StockId;
				CTaiKlineTransferKline  nTransferKline;
				nTransferKline.m_bUseMinute1Data = true;
				m_sharesInformation.GetStockItem(i,j,Cdat);
				if(Cdat==NULL)
					continue;
				StockId=Cdat->id ;
				int nClount=0;
				nTransferKline.TransferKlineToday(StockId,Cdat->kind,MinKline,nClount,1);
				if(nClount == 48)
				{
				   CTaiKlineFileKLine::GetFilePointer(StockId,Cdat->kind,false)->WriteKLine(StockId,MinKline,48,0);   //����K������
				}
		//************��ʾ��Ʊ������Ϣ**************************************************
				m_index++;
				m_zqdm.Format ("%s(%s)",Cdat->name ,Cdat->id);
				int spos=100 * m_index /(m_totalCount+1) ;
				dlg->DisplayZqxx(m_zqdm, spos); 
		//**************************************************************
	/*			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
	*/		}
		}
		delete [] MinKline;//�ͷ��ڴ�
	}
	else
		AfxMessageBox("�ڴ治�㣬��ر��޹س���",MB_ICONSTOP) ; 

	//Marenan -
	//GlobalUnlock((HGLOBAL)MinKline);        //�ͷ��ڴ�
	//GlobalFree( (HGLOBAL)MinKline);

	m_bCloseWorkDone=TRUE;

/////////////////////////////////////////////////////////////////////////////////
	dlg->SetWindowText(_T("������ʷ��������������ҵ"));
    CTaiKlineFileHS::DoCloseWorkHs(dlg);
//*******************�رնԻ���*********************************************************************** 
	if(m_sharesInformation.GetCount()>100)
	  WriteStockInfoToFile();
    m_pButton->EnableWindow(TRUE) ;
	dlg->m_DialogDown =TRUE;
	dlg->ShowWindow(SW_HIDE);
	dlg->OnClose() ; 

	//  20040115
	CTaiShanKlineShowView::OnDataChangAll(200);

	REALDATA *RealFileHead;                 //�ڴ�ӳ���ļ�ͷ���ṹ  
    this->m_sharesInformation.SetRealDataHead(RealFileHead);
	RealFileHead->CloseWorkDone= m_bCloseWorkDone;
    this->m_sharesInformation.SaveRealDataToFile(RealFileHead,sizeof(REALDATA));


	//��⵱�ճ�Ȩ��Ʊ
	CFile fl;
	if(fl.Open ("data\\DoPowerToday.txt",CFile::modeReadWrite))
	{
		fl.Close();
		CFile::Remove ("data\\DoPowerToday.txt");
		CStringArray sArr;
		Kline* pKline = NULL;
		for( i=0;i<STOCKTYPENUM;i++)
		{
			int temp=m_sharesInformation.GetStockTypeCount(i);
			for(int j=0;j<temp;j++)
			{
				CReportData *Cdat;
				CString StockId;
				m_sharesInformation.GetStockItem(i,j,Cdat);
				if(Cdat==NULL)
					continue;
				StockId=Cdat->id ;
				int nClount=0;
			   nClount = CTaiKlineFileKLine::ReadKLineS(StockId,Cdat->kind,pKline,2);   //K������

				if(nClount ==2)
				{
					if(int(Cdat->ystc*100)!= int( pKline[0].close*100) 
						&& Cdat->ystc>0 && Cdat->nowp >0)
						sArr.Add (StockId);
				}
			}
		}

		if(fl.Open ("data\\DoPowerToday.txt",CFile::modeReadWrite|CFile::modeCreate))
		{
			if(sArr.GetSize ()>0)//write to file
			{
				{
					for(i = 0;i<sArr.GetSize ();i++)
					{
						CString ss = sArr[i];
						ss+="\r\n";
						fl.Write(ss.GetBuffer (0),ss.GetLength ());
					}
				}
			}
			fl.Close ();
		}
	}

	//�����������
	m_sharesInformation.SaveCaiwuData();
/////////////////////////////////////////////////////////////////////////////////
//filecopy
//	CDiaCopy *pdlg;
    if(!m_systemOption.organizedata)
		return;


}
void CTaiShanDoc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class
	CDocument::DeleteContents();
}


void* CTaiShanDoc::LookUpArray(CFormularArray &js, CString &str)
{
	int num=js.GetSize();
	for(int i=0;i<num;i++)
	{
		if(strcmp(js.ElementAt(i)->name,str)==0)
		{
			return js.ElementAt(i);
		}
	}
	return NULL;
}
void CTaiShanDoc::OnOpenDocument() 
{
	// TODO: Add your command handler code here
}


////////////////////////////////////////////////////
///////Members of FundManagement///////////////////
//IMPLEMENT_SERIAL(CFund,CObject,1);


void CTaiShanDoc::OnToolClosework() 
{
	// TODO: Add your command handler code here
#ifdef TEST
	CTime t3 = CTime::GetCurrentTime();
    CTime t2(2001, 6, 30, 0, 0, 0 );
	if(t3 > t2)
	{
	  AfxMessageBox("ʹ�����ѹ���",MB_ICONSTOP);
	  return;
	}
#endif

	int tmp=GetStocktime(0) ;      //����Ŀǰʱ��
    if(tmp<902)
		return;

    struct tm when; 
	time_t now; 
	time( &now );
    when = *localtime( &now );
	if(when.tm_wday==0||when.tm_wday==6)
		return ;
	if(!m_bCloseWorkDone)
    {
	   m_bCloseWorkDone=TRUE;
       StockCloseWork();
	}
	else
	{
		int rtn = MessageBox(NULL,"���������̣��Ƿ�����������ҵ��","����",MB_YESNO|MB_ICONWARNING);
		if(rtn==6)
		{
			m_bCloseWorkDone=TRUE;
	        StockCloseWork();
		}
	}
}
void  CTaiShanDoc::StockNameConvert(char *StockName,char *pyjc)
{
	char *p;
	char sPy;
	int n;
	unsigned char c1;
	unsigned char c2;
	char str[20];
	int index=0;
	p=StockName;
	n=strlen(StockName);

	if (m_dyz.GetPy(StockName, pyjc))
		return;

	try
	{
		while(*p!='\n'&&n!=0)
		{
			if( (*p&0x80 )==0)
			{
				if(*p!=0x20)
				{
					str[index++]=*p;
				}
				p++;
				n--;
			}
			else
			{
				c1=*p; p++; n--;
				c2=*p; p++; n--;
				CString mzqmc;
				mzqmc.Format("%X%X",c1,c2); 
				//"A"	from B0A1 to B0C5	
				if(mzqmc>=_T("B0A1")&&mzqmc<_T("B0C5"))
				   str[index++]='A';
				//"B"	from B0C5 to B2C1	
				else if(mzqmc>=_T("B0C5")&&mzqmc<_T("B2C1"))
				   str[index++]='B';
				//"C"	from B2C1 to B4EE	
				else if(mzqmc>=_T("B2C1")&&mzqmc<_T("B4EE"))
				   str[index++]='C';
				//"D"	from B4EE to B6EA	
				else if(mzqmc>=_T("B4EE")&&mzqmc<_T("B6EA"))
				   str[index++]='D';
				//"E"	from B0A1 to B0C5	
				else if(mzqmc>=_T("B6EA")&&mzqmc<_T("B7A2"))
				   str[index++]='E';
				//"F"	from B0A1 to B8C1	
				else if(mzqmc>=_T("B7A2")&&mzqmc<_T("B8C1"))
				   str[index++]='F';
				//"G"	from B8C1 to B9FE	
				else if(mzqmc>=_T("B8C1")&&mzqmc<_T("B9FE"))
				   str[index++]='G';
				//"H"	from B9FE to BBF7	
				else if(mzqmc>=_T("B9FE")&&mzqmc<_T("BBF7"))
				   str[index++]='H';
				//"J"	from BBF7 to BFA6	
				else if(mzqmc>=_T("BBF7")&&mzqmc<_T("BFA6"))
				   str[index++]='J';
				//"K"	from BFA6 to C0AC	
				else if(mzqmc>=_T("BFA6")&&mzqmc<_T("C0AC"))
				   str[index++]='K';
				//"L"	from C0AC to C2E8	
				else if(mzqmc>=_T("C0AC")&&mzqmc<_T("C2E8"))
				   str[index++]='L';
				//"M"	from C2E8 to C4C4	
				else if(mzqmc>=_T("C2E8")&&mzqmc<_T("C4C4"))
				   str[index++]='M';
				//"N"	from C4C4 to C5B6	
				else if(mzqmc>=_T("C4C4")&&mzqmc<_T("C5B6"))
				   str[index++]='N';
				//"O"	from C5B6 to C5BE	
				else if(mzqmc>=_T("C5B6")&&mzqmc<_T("C5BE"))
				   str[index++]='O';
				 //"P"	from C5BE to C6DA	
				else if(mzqmc>=_T("C5BE")&&mzqmc<_T("C6DA"))
				   str[index++]='P';
				//"Q"	from C6DA to C8BB	
				else if(mzqmc>=_T("C6DA")&&mzqmc<_T("C8BB"))
				   str[index++]='Q';
				//"R"	from C8BB to C8F6	
				else if(mzqmc>=_T("C8BB")&&mzqmc<_T("C8F6"))
				   str[index++]='R';
				//"S"	from C8F6 to CBFA	
				else if(mzqmc>=_T("C8F6")&&mzqmc<_T("CBFA"))
				   str[index++]='S';
				//"T"	from CBFA to CDDA	
				else if(mzqmc>=_T("CBFA")&&mzqmc<_T("CDDA"))
				   str[index++]='T';
				//"W"	from CDDA to CEF4	
				else if(mzqmc>=_T("CDDA")&&mzqmc<_T("CEF4"))
				   str[index++]='W';
				//"X"	from CEF4 to D1B9	
				else if(mzqmc>=_T("CEF4")&&mzqmc<_T("D1B9"))
				   str[index++]='X'; 
				//"Y"	from D1B9 to D4D1	
				else if(mzqmc>=_T("D1B9")&&mzqmc<_T("D4D1"))
				   str[index++]='Y';
				//"Z"	from D4D1 to D7F9
				else if(mzqmc>=_T("D4D1")&&mzqmc<_T("D7F9"))
				   str[index++]='Z';
				else if(mzqmc==_T("A3C1"))
				   str[index++]='A';
				else if(mzqmc==_T("A3C2"))
				   str[index++]='B';
				else if(mzqmc>=_T("A3B0")&&mzqmc<=_T("A3B9"))
				{
           			str[index++]=c2-128;  
				}
				else if(m_spz.GetPy(p-2,sPy))
				{
					str[index++]=sPy;
				}
			}
		}
	}
	catch(...)
	{
		AfxMessageBox("��ȡ��Ʊƴ��ʱ����");
	}
	str[index]='\0';
	strcpy(pyjc,str);
}
BOOL CTaiShanDoc::IdleProc( LONG lCount )
{
    if(InitSetp>=4) return FALSE;

    if(InitSetp==0)
	{
		InitializeKeyBoardAngle(TRUE);
		InitSetp=1;
	}
	else if(InitSetp==1)
	{
        m_sharesInformation.InitBaseInfoData( );
		InitSetp=2;
	}
	else if(InitSetp==2)
	{
        InitTjxg();         //��ʼ������ѡ��
		InitSetp=3;
	}else if(InitSetp==3)
	{
//        InitChooseAndStockType();
		InitSetp=4;
	}
	return TRUE;
}

//
//This function is used to initialize all data for Key Board angel
//This is set by Xia Qiankun 2000/1/24
//This is based on the old code of Mr Yu Haihong.
//
void CTaiShanDoc::InitializeKeyBoardAngle(BOOL IsAddStock)
{	
	//
    //Add some system hotkey information  based on digital
	//
	//Setting m_hotkey Array;
	//
   CString keystring[]={"61","62","63","64","65","66",
	                   "03","04","06",
           			   "81","82","83","84","85","86","87","89",
					   //"71","72","73","74",
					   "11","12","13","14","15","16",
					   "21","22","23","24","25","26",
					   "31","32","33","34","35","36",
					   "41","42","43","44","45","46",
					   "51","52","53","54","55","56" ,
					   "01","02","05","10","08",
					   "0","1","2","3","4",
					   "5","6","7","8","9",
					   "71","72","73","07","09"
   };

  CString datastring[]={"��A�Ƿ�","��B�Ƿ�","��A�Ƿ�","��B�Ƿ�","��ȯ�Ƿ�","��ȯ�Ƿ�",
	  "��֤����","��֤����","��ѡ���",
	  "��A����","��B����","��A����", "��B����",
	  "��ծ����","��ծ����","�������","��������",
	  //"������Ϣ","������Ϣ","������Ϣ", "������Ϣ",
	  "��A����","��B����","��A����","��B����","��ȯ����","��ȯ����",
      "��A����","��B����","��A����","��B����","��ȯ����","��ȯ����",
      "��A����","��B����","��A����","��B����","��ȯ����","��ȯ����",
      "��A���","��B���","��A���","��B���","��ȯ���","��ȯ���",
      "��Aί��","��Bί��","��Aί��","��Bί��","��ȯί��","��ȯί��",
      "��ϸ","�ּ�","��������","��������","K���л�",
	  "�ֱʳɽ�","1������","5������","15������","30������",
	  "60������","����","����","����","������",
	  "�Ͻ�������","�������","�ƾ�����","��/��ָ���л�","��/���ػ��߹���"
  };

	int i;
    for(i=0;i<67;i++) //total 50 
	{
		DATA_KEYBOARD *keyboarddata=new DATA_KEYBOARD;
		strcpy(keyboarddata->key,keystring[i]);
		strcpy(keyboarddata->data,datastring[i]);
		keyboarddata->id=1;
		m_hotkey.Add(keyboarddata);
	}
    //
	// �������----���ð��.DAT
	//
	for(i=0;i<10;i++)
	{
		DATA_KEYBOARD *keyboarddata=new DATA_KEYBOARD;
		CString key,data;
		key.Format("%d",90+i);
		strcpy(keyboarddata->key,key);
		data=m_cybkname[i];
		strcpy(keyboarddata->data,data);
		keyboarddata->id=1;
		m_hotkey.Add(keyboarddata);
	}

    //
	//Add stock code and name to the key board data center
	//
    //	CSharesInformation m_sharesInformation;
	//
	if(IsAddStock)
	{
		for(int nStockType=0;nStockType<STOCKTYPENUM;nStockType++)
		{
			int ii = m_sharesInformation.GetStockTypeCount(nStockType);
			for(i=0;i<m_sharesInformation.GetStockTypeCount(nStockType);i++)
			{
				CReportData *pDat1=NULL;
				m_sharesInformation.GetStockItem(nStockType,i,pDat1);
				if(pDat1)
				{
					//Add the number data
 					DATA_KEYBOARD *keyboarddata=new DATA_KEYBOARD;
					keyboarddata->id=0;
					strcpy(keyboarddata->key, pDat1->id);
					strcpy(keyboarddata->data,pDat1->name);
					keyboarddata->m_stkKind = pDat1->kind ;
					int nLen=m_keynumberdata.GetSize();
					int j;
					if(nLen>0)
					{
						for(j=0;j<nLen;j++)
						{
							if(strcmp(m_keynumberdata.GetAt(j)->key,keyboarddata->key)>=0)
								break;
						}
						if(j>=nLen)
							m_keynumberdata.Add(keyboarddata);
						else
							m_keynumberdata.InsertAt(j,keyboarddata);
					}
					else
						m_keynumberdata.Add(keyboarddata);
					//
					//Add the char data  
					//
					DATA_KEYBOARD *KeyBoardHypy=new DATA_KEYBOARD;
					KeyBoardHypy->id=2;
					strcpy(KeyBoardHypy->key,pDat1->Gppyjc);
					strcpy(KeyBoardHypy->data,pDat1->id);
					KeyBoardHypy->m_stkKind = pDat1->kind ;
					nLen=m_keychardata.GetSize();
					if(nLen>0)
					{
						for(j=0;j<nLen;j++)
						{
							if(strcmp(m_keychardata.GetAt(j)->key,KeyBoardHypy->key)>=0)
								break;
						}
						if(j>=nLen)
							m_keychardata.Add(KeyBoardHypy);
						else
							m_keychardata.InsertAt(j,KeyBoardHypy);
					}
					else
						m_keychardata.Add(KeyBoardHypy);
				}
			}    
		}
	}
	//
    //��ϵͳ�ȼ����м�������
	//
	int j;
	for(j=0;j<m_hotkey.GetCount();j++)       
	{
		DATA_KEYBOARD * hotkeydata;
		hotkeydata=m_hotkey.GetAt(j);
		int length=m_keynumberdata.GetSize();
		for(i=0;i<length;i++)
		{
			if(strcmp(m_keynumberdata.GetAt(i)->key,hotkeydata->key)>=0)
				break;
		}
		DATA_KEYBOARD *datatoinsert=new DATA_KEYBOARD;
		strcpy(datatoinsert->key,hotkeydata->key);
		strcpy(datatoinsert->data,hotkeydata->data);
		datatoinsert->id=hotkeydata->id;
		if(i>=length)
			m_keynumberdata.Add(datatoinsert);
		else
			m_keynumberdata.InsertAt(i,datatoinsert,1);
	}

	//
	//Add char data to keyboard angel
	//
	int lengthindex=this->m_formuar_index.GetSize();
	for(i=0;i<lengthindex;i++)
	{
		CString name;
		CString sn;
		name=this->m_formuar_index.GetAt(i)->name;
		sn = name;
		DATA_KEYBOARD *keytoadd=new DATA_KEYBOARD;
		if(name.GetLength()<=4)
			name+="ָ��";
		else
			name+="��";
		keytoadd->id=3;
		strcpy(keytoadd->key,sn);
		strcpy(keytoadd->data,name.GetBuffer(name.GetLength()));
		int keylength=m_keychardata.GetSize();
		for(j=0;j<keylength;j++)
		{
			if(strcmp(m_keychardata.GetAt(j)->key,keytoadd->key)>=0)
				break;
		}
		m_keychardata.InsertAt(j,keytoadd);
	}

	const int hotcharlength=7;
	CString  hotcharkey[hotcharlength]={"QLDJ","HLDJ","QEDJ","HEDJ","TWR","BAR","K"};
	CString  hotchardata[hotcharlength]={"ǰ���ѻ�","�����ѻ�","ǰ��ѻ�","���ѻ�","������","������","������"};
	for(i=0;i<hotcharlength;i++)
	{
		char *keychar,*datachar;
		keychar=hotcharkey[i].GetBuffer(hotcharkey[i].GetLength());
		datachar=hotchardata[i].GetBuffer(hotchardata[i].GetLength());
		DATA_KEYBOARD *keytoadd=new DATA_KEYBOARD;
		keytoadd->id=3;
		strcpy(keytoadd->key,keychar);
		strcpy(keytoadd->data,datachar);
		int keylength=m_keychardata.GetSize();
		for(j=0;j<keylength;j++)
		{
			if(strcmp(m_keychardata.GetAt(j)->key,keytoadd->key)>=0)
				break;
		}
		m_keychardata.InsertAt(j,keytoadd);			
	}
}

//
// This function is used to Free key board angel buffer memory
//This set by Xia Qiankun 2000/1/26
//
void CTaiShanDoc::FreeKeyBoardAngel()
{
	int i;
 	int nLength=m_keychardata.GetSize();
	for(i=0;i<nLength;i++)
	{
		if(m_keychardata.GetAt(i))
			delete m_keychardata.GetAt(i);
	}
	m_keychardata.RemoveAll();

	nLength=m_keynumberdata.GetSize();
	DATA_KEYBOARD* ptemp0;
	DATA_KEYBOARD* ptemp1;
	for(i=0;i<nLength;i++)
	{

		if (i > 0)
		{
			ptemp0 = m_keynumberdata.GetAt(i-1);
		}
		ptemp1 = m_keynumberdata.GetAt(i);
		if(m_keynumberdata.GetAt(i))
			delete m_keynumberdata.GetAt(i);
	}
	m_keynumberdata.RemoveAll();

	nLength=m_hotkey.GetSize();
	for(i=0;i<nLength;i++)
	{
		if(m_hotkey.GetAt(i))
			delete m_hotkey.GetAt(i);
	}
	m_hotkey.RemoveAll();

	nLength=m_hotchar.GetSize();
	for(i=0;i<nLength;i++)
	{
		if(m_hotchar.GetAt(i))
			delete m_hotchar.GetAt(i);
	}
	m_hotchar.RemoveAll();
}

//
// This function is used to refresh key board angel data
//This set by Xia Qiankun 2000/1/26
//
void CTaiShanDoc::RefreshKeyBoardAngel(BOOL IsAddStock)
{
	FreeKeyBoardAngel();
	InitializeKeyBoardAngle(IsAddStock);
}

//
//This function is used to add a stock information to the key board angel data buffer
//This set by Xia Qiankun 2000/1/28
//
//Input three parameters: 
//           strStockCode: is the stock code
//           strStockName: is the stock name
//           strStockHypy: is four charater represented the first character of stock name  
//
void CTaiShanDoc::AddStockToKeyboard(CString strStockCode,int stkKind,CString strStockName,CString strStockHypy)
{
	//
	//judge if one of  strStockCode and strStockName is NULL
	//
	strStockCode.TrimLeft();
	strStockCode.TrimRight();
	strStockName.TrimLeft();
	strStockName.TrimRight();
	if(strStockCode.IsEmpty()||strStockName.IsEmpty())
		return;

	//�ж��Ƿ����ظ��Ĵ�����֣�������ظ��ģ����˳�
	int nLength=m_keynumberdata.GetSize();
	int nResult;
	int i;
	for(i=0;i<nLength;i++)
	{
		nResult=strcmp(m_keynumberdata.GetAt(i)->key,strStockCode);
		if(nResult==0 && m_keynumberdata.GetAt(i)->m_stkKind == stkKind)		//������ظ��Ĺ�Ʊ������֣����˳�
			return;
		else 
		{
			if (nResult>0)
				break;
		}
	}

	DATA_KEYBOARD *DataToAdd=new DATA_KEYBOARD;
    //
	//First added stock info to buffer based on number
	//
	DataToAdd->id=0;
	strcpy(DataToAdd->key,strStockCode);
	strcpy(DataToAdd->data,strStockName);
	DataToAdd->m_stkKind = stkKind;

	m_keynumberdata.InsertAt(i,DataToAdd);
	//
    //then added stock info to buffe based on character
	//
	DataToAdd=new DATA_KEYBOARD;
	DataToAdd->id=2;
	strcpy(DataToAdd->key,strStockHypy);
	strcpy(DataToAdd->data,strStockCode);
	DataToAdd->m_stkKind = stkKind;
    nLength=m_keychardata.GetSize();
	for( i=0;i<nLength;i++)
	{
		if(strcmp(m_keychardata.GetAt(i)->key,strStockHypy)>=0 )
			break;
	}
	m_keychardata.InsertAt(i,DataToAdd);
}

//
//This function is used to delete a stock information from the key board angel data buffer
//This set by Xia Qiankun 2000/3/1
//
//Input three parameters: 
//           strStockCode: is the stock code
//           strStockName: is the stock name
//           strStockHypy: is four charater represented the first character of stock name  
//
BOOL CTaiShanDoc::DeleteStockFromKeyboard(CString strStockCode,int stkKind, CString strStockName, CString strStockHypy)
{
	//
	//judge if one of  strStockCode and strStockName is NULL
	//
	if(strStockCode.IsEmpty()||strStockName.IsEmpty())
		return FALSE;
    BOOL bResult=FALSE; 
	int nLength=m_keynumberdata.GetSize();
	int i;
	for(i=0;i<nLength;i++)
	{
		if(strcmp(m_keynumberdata.GetAt(i)->key,strStockCode)==0 && m_keynumberdata.GetAt(i)->m_stkKind == stkKind)
		//	if(strcmp(m_keychardata.GetAt(i)->data,strStockName)==0)
				break;
	}
	if(i<nLength)
	{
		m_keynumberdata.RemoveAt(i);
		bResult=TRUE;
	}

    nLength=m_keychardata.GetSize();
	for( i=0;i<nLength;i++)
	{
		//if(strcmp(m_keychardata.GetAt(i)->key,strStockHypy)==0)
			if(strcmp(m_keychardata.GetAt(i)->data,strStockCode)==0 && m_keychardata.GetAt(i)->m_stkKind == stkKind)
				break;
	}
	if(i<nLength)
	{
		m_keychardata.RemoveAt(i);
		bResult=TRUE;
	}
	return bResult;
}

//
//This function is used to Modify a stock information of the key board angel data buffer
//This set by Xia Qiankun 2000/3/1
//
//Input three parameters: 
//           strStockCode: is the stock code
//           strStockName: is the stock name
//           strStockHypy: is four charater represented the first character of stock name  
//
BOOL CTaiShanDoc::ModifyStockOfKeyboard(CString strStockCode,int stkKind,CString strStockName,CString strStockHypy)
{
	//
	//judge if one of  strStockCode and strStockName is NULL
	//
	if(strStockCode.IsEmpty()||strStockName.IsEmpty())
		return FALSE;
    BOOL bResult=FALSE; 
	DATA_KEYBOARD *DataToAdd=NULL;
	int nLength=m_keynumberdata.GetSize();
	int i;
	for(i=0;i<nLength;i++)
	{
		if(strcmp(m_keynumberdata.GetAt(i)->key,strStockCode)==0 && m_keynumberdata.GetAt(i)->m_stkKind == stkKind)
			break;
	}
	if(i<nLength)
	{
		DataToAdd=new DATA_KEYBOARD;
		m_keynumberdata.RemoveAt(i);
		DataToAdd->id=0;
		strcpy(DataToAdd->key,strStockCode);
		strcpy(DataToAdd->data,strStockName);
		DataToAdd->m_stkKind  = stkKind;
		m_keynumberdata.InsertAt(i,DataToAdd);
		bResult=TRUE;
	}

    nLength=m_keychardata.GetSize();
	for( i=0;i<nLength;i++)
	{
		if(strcmp(m_keychardata.GetAt(i)->data,strStockCode)==0&& m_keychardata.GetAt(i)->m_stkKind == stkKind)
			break;
	}
	if(i<nLength)
	{
		m_keychardata.RemoveAt(i);
		DataToAdd=new DATA_KEYBOARD;
		DataToAdd->id=2;
		strcpy(DataToAdd->key,strStockHypy);
		strcpy(DataToAdd->data,strStockCode);
		DataToAdd->m_stkKind  = stkKind;
		m_keychardata.InsertAt(i,DataToAdd);
		bResult=TRUE;
	}
	return bResult;
}

void CTaiShanDoc::InitChooseAndStockType()
{
	m_ManagerBlockData.InitStockPoint();
	if(m_ManagerBlockData.GetBlockTypeCounts()!=this->m_sharesInformation.GetStockTypeCount(BLOCKINDEX))//ԭ��=10
	{
       m_sharesInformation.DeleteAllStockFromStockType();
	}
	if(this->m_sharesInformation.GetStockTypeCount(BLOCKINDEX)==0)
	  m_ManagerBlockData.InitBlockPoint();                   //��ʼ����Ʊָ��
}
void CTaiShanDoc::InitStockFiveDaysVolumn()
{
    for(int i=0;i<SZCYB;i++)
	{
		if(i==SHZS||i==SZZS||i==BLOCKINDEX)//ָ��
			continue;
		int temp=m_sharesInformation.GetStockTypeCount(i);
		for(int j=0;j<temp;j++)
		{
			CString StockId;
			CReportData *Cdat;
			Kline *pKline=NULL;
			CBuySellList l_BuySellList;
			m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat==NULL)
				continue;
			StockId =Cdat->id ;
			Cdat->volume5=0;
			int count=0;
		    count=CTaiKlineFileKLine::ReadKLineS(StockId,i,pKline,5); 
			if(count==0)
				continue;
			for(int k=0;k<count;k++)
              Cdat->volume5 += pKline[k].vol;
			Cdat->volume5 =Cdat->volume5/count;

			if(pKline)
			delete []pKline;
		}
	}
}
void CTaiShanDoc::InitFiveDaysVolumnForStock(PCdat1 &pCdat)
{
	CString StockId=pCdat->id;
	Kline *pKline=NULL;
	CBuySellList l_BuySellList;

	if(pCdat->kind==0||pCdat->kind==11||pCdat->kind==25)//ָ��
      return;
	StockId =pCdat->id ;
	pCdat->volume5=0;
	int count=0;
   count=CTaiKlineFileKLine::ReadKLineS(StockId,pCdat->kind,pKline,5); 
	if(count==0)
	{
       return;
	}
	for(int k=0;k<count;k++)
      pCdat->volume5 += pKline[k].vol;
	pCdat->volume5 =pCdat->volume5/count;

	if(pKline)
	delete []pKline;
}
void CTaiShanDoc::WriteStockInfoToFile()
{
	int StockCount[STOCKTYPENUM];
	memset(StockCount,0,sizeof(int)*STOCKTYPENUM);
	FILE *fp=_fsopen("StockName.dat","w+b",SH_DENYNO);
	fclose(fp);
	fp=_fsopen("StockName.dat","r+b",SH_DENYNO);
	int i;
	for(i=0;i<STOCKTYPENUM;i++)
	{
        StockCount[i]=this->m_sharesInformation.GetStockTypeCount(i);
	}
	fseek(fp,0,SEEK_SET);                                //��������Ʊд���ļ�
	fwrite(&StockCount[0],4,STOCKTYPENUM, fp);
	for(i=0;i<STOCKTYPENUM;i++)
	{
		int temp=this->m_sharesInformation.GetStockTypeCount(i);
		for(int j=0;j<temp;j++)
		{
			CReportData * Cdat;
			this->m_sharesInformation.GetStockItem(i,j, Cdat);  
			if(Cdat==NULL)
				continue;
			fwrite(&Cdat->kind,1,1,fp);
			fwrite(Cdat->id,1,6,fp);
			fwrite(Cdat->name,1,8,fp);
			fwrite(Cdat->Gppyjc,1,4,fp);
			fwrite(&Cdat->sel,1,4,fp);

			//lmb11
			char ch1 = ' ';
			fwrite(&ch1,1,1,fp);
		}
	}
    fclose(fp);
}

int CTaiShanDoc::GetStockKind(CString strKind)
{
	 if(strKind=="00")
		return SHZS;
	 else if(strKind=="01")
		return SHAG;
	 else if(strKind=="02")
		return SHBG;
	 else if(strKind=="03")
		 return SHJJ;
	 else if(strKind=="04")
		 return SHZQ;
	 else if(strKind=="05")
		 return SHZZ;
	 else if(strKind=="06")
		 return SHHG;
	 else if(strKind=="07")
		 return SHETF;
	 else if(strKind=="08")
		 return SHKF;
	 else if(strKind=="09")
		 return SHQZ;
	 else if(strKind=="10")
		 return SHOT;
	 else if(strKind=="11")
		return SZZS;
	 else if(strKind=="12")
		return SZAG;
	 else if(strKind=="13")
		return SZBG;
	 else if(strKind=="14")
		return SZJJ;
	 else if(strKind=="15")
		return SZZQ;
	 else if(strKind=="16")
	    return SHZZ;	 //   
	 else if(strKind=="17")
	    return SZHG;                             
	 else if(strKind=="18")
		return SZETF;
	 else if(strKind=="19")
		return SZKF;
	 else if(strKind=="20")
		return SZQZ;
	 else if(strKind=="21")
		return SZSB;
	 else if(strKind=="22")
		return SZOT;
	 else if(strKind=="23")
		return SZZXQY;
	 else if(strKind=="24")
	    return SZCYB;	 //��ҵ��   
	 else if(strKind=="25")
	    return BLOCKINDEX;                              //�����Ʊ
	 else if(strKind=="26")
	    return CHOOSESTK;                          //���ָ��  

	 return -1;
}
CString CTaiShanDoc::GetStockKindString(int nKind)
{
	switch(nKind)
	{
	case SHZS:                           //��ָ֤�� 
		return "00";
		break;
    case SHAG:                             //��֤A�� 
		return "01";
		break;
	case SHBG:                             //��֤B�� 
		return "02";
		break;
	case SHJJ:                           //��ָ֤�� 
		return "03";
		break;
    case SHZQ:                             //��֤A�� 
		return "04";
		break;
	case SHZZ:                             //��֤B�� 
		return "05";
		break;
	case SHHG:                           //��ָ֤�� 
		return "06";
		break;
    case SHETF:                             //��֤A�� 
		return "07";
		break;
	case SHKF:                             //��֤B�� 
		return "08";
		break;
	case SHQZ:                              //����ָ��
		return "09";
		break;
	case SHOT:                              //����A��
		return "10";
		break;
	case SZZS:                              //����B��
		return "11";
		break;
	case SZAG:                              //��֤ծȯ
		return "12";
		break;
	case SZBG:                              //����ծȯ
		return "13";
		break;
	case SZJJ:                              //   
		return "14";
		break;
	case SZZQ:                              //����B��
		return "15";
		break;
	case SZZZ:                              //��֤ծȯ
		return "16";
		break;
	case SZHG:                              //����ծȯ
		return "17";
		break;
	case SZETF:                              //   
		return "18";
		break;
	case SZKF:                              //����B��
		return "19";
		break;
	case SZQZ:                              //��֤ծȯ
		return "20";
		break;
	case SZSB:                              //����ծȯ
		return "21";
		break;
	case SZOT:                              //   
		return "22";
		break;
	case SZZXQY:                              //   
		return "23";
		break;
	case SZCYB:                              //����ָ��   
		return "24";
		break;
	case BLOCKINDEX:                              //�����Ʊ
		return "25";
		break;
	case CHOOSESTK:                          //���ָ��  
		return "26";
		break;

	}
	return "";
}
#ifdef WIDE_NET_VERSION
void CTaiShanDoc::WideNetTransactSystemRunPara(RCV_SYSTEMRUN_STRUCTEx *pSystemRunPara)
{
	m_nANT[0]=pSystemRunPara->m_btBargainingTime[0];              
	m_nANT[1]=pSystemRunPara->m_btBargainingTime[1];              
	m_nANT[2]=pSystemRunPara->m_btBargainingTime[2];
	m_bCloseWorkDone = pSystemRunPara->m_btCloseWork;
	this->m_bInitDone= pSystemRunPara->m_btTodayInitialize;
	CReportData *p1A0001,*p2A01,*p2D01;
    m_sharesInformation.Lookup((CSharesCompute::GetIndexSymbol(0)).GetBuffer(0),p1A0001,SHZS);
    m_sharesInformation.Lookup((CSharesCompute::GetIndexSymbol(1)).GetBuffer(0),p2A01,SZZS);
	CString seb = CSharesCompute::GetIndexSymbol(2);
    m_sharesInformation.Lookup(seb.GetBuffer (0),p2D01,EBZS);
	if(p1A0001!=NULL)
	{
		p1A0001->nowp=pSystemRunPara->m_fNewIndex[0];
		p1A0001->totp=pSystemRunPara->m_fAmount[0];
		p1A0001->totv=pSystemRunPara->m_fVolume[0];
		p1A0001->m_Kdata1[m_nANT[0]].Price=pSystemRunPara->m_fNewIndex[0];
		p1A0001->m_Kdata1[m_nANT[0]].Amount=pSystemRunPara->m_fAmount[0];
		p1A0001->m_Kdata1[m_nANT[0]].Volume=pSystemRunPara->m_fVolume[0];
	}	
	if(p2A01!=NULL)
	{
		p2A01->nowp=pSystemRunPara->m_fNewIndex[1];
		p2A01->totp=pSystemRunPara->m_fAmount[1];
		p2A01->totv=pSystemRunPara->m_fVolume[1];
		p2A01->m_Kdata1[m_nANT[1]].Price=pSystemRunPara->m_fNewIndex[1];
		p2A01->m_Kdata1[m_nANT[1]].Amount=pSystemRunPara->m_fAmount[1];
		p2A01->m_Kdata1[m_nANT[1]].Volume=pSystemRunPara->m_fVolume[1];
	}	
	if(p2D01!=NULL)
	{
		p2D01->nowp=pSystemRunPara->m_fNewIndex[2];
		p2D01->totp=pSystemRunPara->m_fAmount[2];
		p2D01->totv=pSystemRunPara->m_fVolume[2];
		p2D01->m_Kdata1[m_nANT[2]].Price=pSystemRunPara->m_fNewIndex[2];
		p2D01->m_Kdata1[m_nANT[2]].Amount=pSystemRunPara->m_fAmount[2];
		p2D01->m_Kdata1[m_nANT[2]].Volume=pSystemRunPara->m_fVolume[2];
	}	

	if(p1A0001!=NULL&&p2A01!=NULL)
	{
		if(p1A0001->nowp==0)
			((CMainFrame *)(AfxGetApp()->m_pMainWnd))->DisplayBargain(
			0,0,0,0,
			0,0,0,0,
			0,0,0,0,
			0.5 ,0.5 ,0);
		else if(p2D01!=NULL)
		{
			((CMainFrame *)(AfxGetApp()->m_pMainWnd))->DisplayBargain(
			p1A0001->nowp ,p1A0001->nowp - p1A0001->ystc ,p1A0001->totv,p1A0001->totp/10000,
			p2A01->nowp ,p2A01->nowp - p2A01->ystc,p2A01->totv ,p2A01->totp/100000000,
			p2D01->nowp ,p2D01->nowp - p2D01->ystc,p2D01->totv ,p2D01->totp/100000000,
			pSystemRunPara->m_fRedGreenArmy[0]/100,pSystemRunPara->m_fRedGreenArmy[1]/100,pSystemRunPara->m_fRedGreenArmy[2]/100);
		}else
		{
			((CMainFrame *)(AfxGetApp()->m_pMainWnd))->DisplayBargain(
			p1A0001->nowp ,p1A0001->nowp - p1A0001->ystc ,p1A0001->totv,p1A0001->totp/100000000,
			p2A01->nowp ,p2A01->nowp - p2A01->ystc,p2A01->totv ,p2A01->totp/100000000,
			0,0,0,0,
			pSystemRunPara->m_fRedGreenArmy[0]/100,pSystemRunPara->m_fRedGreenArmy[1]/100,pSystemRunPara->m_fRedGreenArmy[2]/100);
		}
	}
	if(m_lDay>=pSystemRunPara->m_BargainingDate)
        return ;
	if(m_lDay<pSystemRunPara->m_BargainingDate&&pSystemRunPara->m_btTodayInitialize)
	{
	    SOCKET_ID m_SocketID;
		m_SocketID.iFlag=-1;
		m_SocketID.hWnd=NULL;
		CMainFrame * pMainFrm=((CMainFrame *)(AfxGetApp()->m_pMainWnd));
		if(pMainFrm!=NULL&&pMainFrm->m_pClientTransmitManageWnd)
		{
			if(m_SocketID.iFlag==-1)
			{
				 m_SocketID=pMainFrm->m_pClientTransmitManageWnd->RegisterID(NULL);
			}
			if(m_SocketID.iFlag!=-1)
			{
				 TRANS_PACKAGE_STRUCTEx m_TransPackageStruct;
                 m_TransPackageStruct.m_TransmitType=InitStockCodeList;
			     m_TransPackageStruct.m_dwTransmitStockType=QBGP;
				 pMainFrm->m_pClientTransmitManageWnd->GetMarketData(&m_TransPackageStruct,0,m_SocketID,TRUE);
			}
		}
        m_lDay=pSystemRunPara->m_BargainingDate;  
	}
}
void CTaiShanDoc::WideNetTransactIndexAttribute(RCV_INDEXATTRIBUTE_STRUCTEx *pIndexAttribute ,int nParaCounts, int nIndexType)
{
	Rsdn1 **Nidx2;
	Tidxd **Tidx2;
	this->m_sharesInformation.GetIndexRsdn(Nidx2); 
    this->m_sharesInformation.GetIndexTidxd(Tidx2); 
    for(int i=0;i<nParaCounts;i++)
	{
        int MinuteCount=this->m_sharesCompute.GetStockMinute( pIndexAttribute->m_time,nIndexType);
        Nidx2[nIndexType][MinuteCount].dnn=pIndexAttribute[i].dnn;
        Nidx2[nIndexType][MinuteCount].rsn=pIndexAttribute[i].rsn;
        Tidx2[nIndexType][MinuteCount].sec5=pIndexAttribute[i].sec5;
        Tidx2[nIndexType][MinuteCount].dp=pIndexAttribute[i].dp;
        Tidx2[nIndexType][MinuteCount].rp=pIndexAttribute[i].rp;
	}
}
void CTaiShanDoc::WideNetInitMarketBargainingData(RCV_STKLABEL_STRUCTEx *pStkLabel,int nStkLabelCounts)
{
	this->m_sharesInformation.ClearAllRealTimeMarketData();
	this->RefreshKeyBoardAngel(FALSE);
	for(int i=0;i<nStkLabelCounts;i++)
	{
		CReportData *pCdat;
		int nKind=m_sharesInformation.GetStockKind(pStkLabel[i].m_wMarket,pStkLabel[i].m_szLabel);
		if (this->m_sharesInformation.Lookup(pStkLabel[i].m_szLabel,pCdat,nKind) != TRUE)     //���ù�Ʊ�Ƿ��Ѽ�¼���ڴ�ģ����
		{
			if(strlen(pStkLabel[i].m_szLabel)==6||strlen(pStkLabel[i].m_szLabel)==4)
			{
				if(nKind>=0&&nKind<=10)
				{
					if(!m_sharesInformation.InsertItem(pStkLabel[i].m_szLabel,pCdat,nKind))
						continue;
//					m_pReportView->LoadShowData(nKind);              //��CGRID������һ�� 
					strcpy(pCdat->name ,pStkLabel[i].m_szName);
					strcpy(pCdat->id ,pStkLabel[i].m_szLabel );
					this->StockNameConvert(pCdat->name,pCdat->Gppyjc );
                    pCdat->kind=nKind;
					AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
//					m_pReportView->AddGridRow(nKind);              //��CGRID������һ�� 
					BASEINFO *m_pStockBase;
					if(m_sharesInformation.LookupBase(pCdat->id,pCdat->kind,m_pStockBase))
					{
					   pCdat->pBaseInfo=m_pStockBase;
					}
				}
			}
		} 
	}
	InitChooseAndStockType();
	m_pReportView->ChangeToPage(0);
}
void CTaiShanDoc::WideNetInitMarketBargainingData2(RCV_STKLABEL2_STRUCTEx *pStkLabel,int nStkLabelCounts)
{
	for(int i=0;i<nStkLabelCounts;i++)
	{
		CReportData *pCdat;
		int nKind=m_sharesInformation.GetStockKind(pStkLabel[i].m_wMarket,pStkLabel[i].m_szLabel);
		if (this->m_sharesInformation.Lookup(pStkLabel[i].m_szLabel,pCdat,nKind) == TRUE)     //���ù�Ʊ�Ƿ��Ѽ�¼���ڴ�ģ����
		{
            pCdat->ystc=pStkLabel[i].m_fLastClose;
			pCdat->volume5=pStkLabel[i].m_fAverageVolumn;
		} 
	}
}
#endif

void CTaiShanDoc::ClearRealData()
{
	BLOCKFILEHEAD *pStockTypeHead;
	Init_StockData(0);                                //�������ݳ�ʼ������
	this->m_ManagerBlockData.GetBlockHeadPoint(pStockTypeHead);
	pStockTypeHead->m_lLastTime=0;
}

void CTaiShanDoc::CheckKind()
{
    for(int i=0;i<STOCKTYPENUM;i++)
	{
		int temp=m_sharesInformation.GetStockTypeCount(i);
		for(int j=0;j<temp;j++)
		{
			CReportData *Cdat;
			CString StockId ; 
			m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat==NULL)
				continue;
			StockId =Cdat->id ;
			if(StockId.GetLength ()<4)
				continue;

			if(Cdat->kind == SHAG)
			{
				 if(StockId[0] == '5')
					 Cdat->kind = SHJJ;
			}
			if(Cdat->kind == SZAG)
			{
				 if(StockId[0] == '1')
				 {
					 if(StockId[1] == '7' || StockId[1] == '8')
						 Cdat->kind = SZJJ;
				 }
			}
		}
	}	
}

void CTaiShanDoc::ZeroStatisticStockData()
{

	//ͳ��CReportData�ṹ�б���
	for(int k = 0;k < 33;k++)//CReportData�ṹ����Ҫͳ�Ƶ�����Ϊ 33
	{
		*((float*)&(m_pStatisticData[0].pItem->ystc) + k) = 0;
		*((float*)&(m_pStatisticData[1].pItem->ystc) + k) = 0;
		*((float*)&(m_pStatisticData[2].pItem->ystc) + k) = 0;
		*((float*)&(m_pStatisticData[3].pItem->ystc) + k) = 0;
	}
	//ͳ��BASEINFO�ṹ�б���
	for(int k = 0;k < 33;k++)//BASEINFO�ṹ����Ҫͳ�Ƶ�����Ϊ 33
	{
		*((float*)&(m_pStatisticData[0].pItem->pBaseInfo->zgb) + k) = 0;
		*((float*)&(m_pStatisticData[1].pItem->pBaseInfo->zgb) + k) = 0;
		*((float*)&(m_pStatisticData[2].pItem->pBaseInfo->zgb) + k) = 0;
		*((float*)&(m_pStatisticData[3].pItem->pBaseInfo->zgb) + k) = 0;
	}
	////K������ͳ��
	for (int m = 0;m < 3;m++)
	{
		for (int k = 0;k < 240;k++)// Kdata1   m_Kdata1[240];
		{
			*((float*)&(m_pStatisticData[0].pItem->m_Kdata1[k].Price) + m) = 0;
			*((float*)&(m_pStatisticData[1].pItem->m_Kdata1[k].Price) + m) = 0;
			*((float*)&(m_pStatisticData[2].pItem->m_Kdata1[k].Price) + m) = 0;
			*((float*)&(m_pStatisticData[3].pItem->m_Kdata1[k].Price) + m) = 0;
		}
	}
}


BOOL CTaiShanDoc::StatisticStockData(byte nWeightType,int nStockType)
{
	{
		UINT nCount = 0;
		double dZgb = 0;
		double dLtgb = 0;
		int len = sizeof(STOCKDATASHOW);
		double dMin = 0,dMax = 0,dAve = 0,dVar = 0;
		float* pData = NULL;

		int iNum = m_nShowCount;

		if (m_pStockDataShow == NULL || iNum == 0)//
		{
			ZeroStatisticStockData();
			return true;
		}

		//ͳ��CReportData�ṹ�б���
		for(int k = 0;k < 33;k++)//CReportData�ṹ����Ҫͳ�Ƶ�����Ϊ 33
		{
			if (m_pStockDataShow[0].pItem == NULL)//ͣ��
			{
				continue;
			}
			pData = (float*)&(m_pStockDataShow[0].pItem->ystc);
			//�������ֵ ��Сֵ �� ��ֵ
			dMax = pData[k];
			dMin = pData[k];
			dAve = 0;
			dVar = 0;
			nCount = 0;
			dZgb = 0;
			dLtgb = 0;
			for (int j = 0;j < iNum;j++)
			{
				pData = (float*)&(m_pStockDataShow[j].pItem->ystc);

				if (m_pStockDataShow[j].pItem == NULL || m_pStockDataShow[j].pItem->nowp <= 0.0)//ͣ��
				{
					continue;
				}

				if (m_pStockDataShow[j].pItem->pBaseInfo == NULL )//ͣ��
				{
					nWeightType = 0;
				}
				if (nWeightType == 0)//Ȩϵ�����
				{
					dAve += pData[k];
				}
				else if (nWeightType == 1)//��ͨ�ɱ�
				{
					dLtgb += m_pStockDataShow[j].pItem->pBaseInfo->zgb - m_pStockDataShow[j].pItem->pBaseInfo->fqrfrg;
					dAve += pData[k] * (m_pStockDataShow[j].pItem->pBaseInfo->zgb - m_pStockDataShow[j].pItem->pBaseInfo->fqrfrg);
				}
				else//�ܹɱ�
				{
					dZgb += m_pStockDataShow[j].pItem->pBaseInfo->zgb;
					dAve += pData[k] * m_pStockDataShow[j].pItem->pBaseInfo->zgb;
				}

				if (dMax < pData[k])
				{
					dMax = pData[k];
				}
				if (dMin > pData[k])
				{
					dMin = pData[k];
				}
				nCount++;
			}
			if (nCount == 0)
			{
				nCount = 1;
			}
			if (nWeightType == 0 || (nWeightType == 1 && dLtgb <= 0.0) || (nWeightType == 2 && dZgb <= 0.0))
			{
				dAve /= (double)nCount;//Ȩϵ�����
			}
			else if (nWeightType == 1)
			{
				dAve /= dLtgb;//��ͨ�ɱ�
			}
			else
			{
				dAve /= dZgb;//�ܹɱ�
			}
			//���㷽��
			for (int j = 0;j < iNum;j++)
			{
				if (m_pStockDataShow[j].pItem == NULL || m_pStockDataShow[j].pItem->nowp <= 0)//ͣ��
				{
					continue;
				}
				pData = (float*)&(m_pStockDataShow[j].pItem->ystc);

				dVar += (dAve - pData[k]) * (dAve - pData[k]);
			}
			*((float*)&(m_pStatisticData[0].pItem->ystc) + k) = dAve;
			*((float*)&(m_pStatisticData[1].pItem->ystc) + k) = sqrt(dVar)/nCount;
			*((float*)&(m_pStatisticData[2].pItem->ystc) + k) = dMax;
			*((float*)&(m_pStatisticData[3].pItem->ystc) + k) = dMin;
		}
		//ͳ��BASEINFO�ṹ�б���
		for(int k = 0;k < 33;k++)//BASEINFO�ṹ����Ҫͳ�Ƶ�����Ϊ 33
		{
			if (m_pStockDataShow[0].pItem == NULL || m_pStockDataShow[0].pItem->pBaseInfo == NULL)//ͣ��
			{
				continue;
			}
			pData = (float*)&(m_pStockDataShow[0].pItem->pBaseInfo->zgb);
			//�������ֵ ��Сֵ �� ��ֵ
			dMax = pData[k];
			dMin = pData[k];
			dAve = 0;
			dVar = 0;
			nCount = 0;
			dZgb = 0;
			dLtgb = 0;

			for (int j = 0;j < iNum;j++)
			{
				pData = (float*)&(m_pStockDataShow[j].pItem->pBaseInfo->zgb);

				if (m_pStockDataShow[j].pItem == NULL || m_pStockDataShow[j].pItem->pBaseInfo == NULL|| m_pStockDataShow[j].pItem->nowp <= 0.0)//ͣ��
				{
					continue;
				}

				if (nWeightType == 0)//Ȩϵ�����
				{
					dAve += pData[k];
				}
				else if (nWeightType == 1)//��ͨ�ɱ�
				{
					dLtgb += m_pStockDataShow[j].pItem->pBaseInfo->zgb - m_pStockDataShow[j].pItem->pBaseInfo->fqrfrg;
					dAve += pData[k] * (m_pStockDataShow[j].pItem->pBaseInfo->zgb - m_pStockDataShow[j].pItem->pBaseInfo->fqrfrg);
				}
				else//�ܹɱ�
				{
					dZgb += m_pStockDataShow[j].pItem->pBaseInfo->zgb;
					dAve += pData[k] * m_pStockDataShow[j].pItem->pBaseInfo->zgb;
				}

				if (dMax < pData[k])
				{
					dMax = pData[k];
				}
				if (dMin > pData[k])
				{
					dMin = pData[k];
				}
				nCount++;
			}
			if (nCount == 0)
			{
				nCount = 1;
			}
			if (nWeightType == 0 || (nWeightType == 1 && dLtgb <= 0.0) || (nWeightType == 2 && dZgb <= 0.0))
			{
				dAve /= (double)nCount;//Ȩϵ�����
			}
			else if (nWeightType == 1)
			{
				dAve /= dLtgb;//��ͨ�ɱ�
			}
			else
			{
				dAve /= dZgb;//�ܹɱ�
			}
			//���㷽��
			for (int j = 0;j < iNum;j++)
			{
				if (m_pStockDataShow[j].pItem == NULL || m_pStockDataShow[j].pItem->pBaseInfo == NULL|| m_pStockDataShow[j].pItem->nowp <= 0.0)//ͣ��
				{
					continue;
				}
				pData = (float*)&(m_pStockDataShow[j].pItem->pBaseInfo->zgb);

				dVar += (dAve - pData[k]) * (dAve - pData[k]);
			}
			*((float*)&(m_pStatisticData[0].pItem->pBaseInfo->zgb) + k) = dAve;
			*((float*)&(m_pStatisticData[1].pItem->pBaseInfo->zgb) + k) = sqrt(dVar)/nCount;
			*((float*)&(m_pStatisticData[2].pItem->pBaseInfo->zgb) + k) = dMax;
			*((float*)&(m_pStatisticData[3].pItem->pBaseInfo->zgb) + k) = dMin;
		}
		////K������ͳ��
		for (int m = 0;m < 3;m++)
		{
			for (int k = 0;k < 240;k++)// Kdata1   m_Kdata1[240];
			{
				pData = (float*)&(m_pStockDataShow[0].pItem->m_Kdata1[k].Price);
				//�������ֵ ��Сֵ �� ��ֵ
				dMax = pData[m];
				dMin = pData[m];
				dAve = 0;
				nCount = 0;
				dZgb = 0;
				dLtgb = 0;
				dVar = 0;
				for (int j = 0;j < iNum;j++)
				{
					if (m_pStockDataShow[j].pItem == NULL || m_pStockDataShow[j].pItem->nowp <= 0.0)//ͣ��
					{
						continue;
					}

					pData = (float*)&(m_pStockDataShow[j].pItem->m_Kdata1[k].Price);

					if (m_pStockDataShow[j].pItem->pBaseInfo == NULL)//Ȩϵ�����
					{
						nWeightType = 0;
					}
					if (nWeightType == 0)//Ȩϵ�����
					{
						dAve += pData[m];
					}
					else if (nWeightType == 1)//��ͨ�ɱ�
					{
						dLtgb += m_pStockDataShow[j].pItem->pBaseInfo->zgb - m_pStockDataShow[j].pItem->pBaseInfo->fqrfrg;
						dAve += pData[m] * (m_pStockDataShow[j].pItem->pBaseInfo->zgb - m_pStockDataShow[j].pItem->pBaseInfo->fqrfrg);
					}
					else//�ܹɱ�
					{
						dZgb += m_pStockDataShow[j].pItem->pBaseInfo->zgb;
						dAve += pData[m] * m_pStockDataShow[j].pItem->pBaseInfo->zgb;
					}

					if (dMax < pData[m])
					{
						dMax = pData[m];
					}
					if (dMin > pData[m])
					{
						dMin = pData[m];
					}
					nCount++;
				}
				if (nCount == 0)
				{
					nCount = 1;
				}
				if (nWeightType == 0 || (nWeightType == 1 && dLtgb == 0) || (nWeightType == 2 && dZgb == 0))
				{
					dAve /= (double)nCount;//Ȩϵ�����
				}
				else if (nWeightType == 1)
				{
					dAve /= dLtgb;//��ͨ�ɱ�
				}
				else
				{
					dAve /= dZgb;//�ܹɱ�
				}
				//���㷽��
				for (int j = 0;j < iNum;j++)
				{
					if (m_pStockDataShow[j].pItem == NULL || m_pStockDataShow[j].pItem->nowp <= 0.0)//ͣ��
					{
						continue;
					}

					pData = (float*)&(m_pStockDataShow[j].pItem->m_Kdata1[k].Price);

					dVar += (dAve - pData[m]) * (dAve - pData[m]);
				}
				*((float*)&(m_pStatisticData[0].pItem->m_Kdata1[k].Price) + m) = dAve;
				*((float*)&(m_pStatisticData[1].pItem->m_Kdata1[k].Price) + m) = sqrt(dVar)/nCount;
				*((float*)&(m_pStatisticData[2].pItem->m_Kdata1[k].Price) + m) = dMax;
				*((float*)&(m_pStatisticData[3].pItem->m_Kdata1[k].Price) + m) = dMin;
			}
		}
	}

	return true;
}
