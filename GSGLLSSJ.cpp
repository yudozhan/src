// GSGLLSSJ.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"


#include <stdio.h>
#include <share.h>
#include  <io.h>
#include "GSGLLSSJ.h"
#include "afxdlgs.h"
#include "math.h"
#include "afxtempl.h"
#include "CTaiShanDoc.h"
#include "mainfrm.h"
#include "CTaiKlineMemFile.h"
#include "CTaiKlineFileKLine.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MLINEHEAD  16
#define MLINESMALLHEAD 20
#define MLINEBLK   48*25*3
#define MLINEBLKBYTECOUNT 48*25*3*32     

struct KLINESMALLHEAD_CUI
{
	char    StockSign[8]; //前8个字节为股票代码。
	short 	numChuQ	;	  //2个字节为除权数据的总次数
	long    numKln	;	  //4个字节整型，代表日K线的个数。
	short   symBlock[25]; //下面每两个字节代表K线数据的块号
};


/////////////////////////////////////////////////////////////////////////////
// CGSGLLSSJ property page

IMPLEMENT_DYNCREATE(CGSGLLSSJ, CPropertyPage)

CGSGLLSSJ::CGSGLLSSJ() : CPropertyPage(CGSGLLSSJ::IDD)
{
	//{{AFX_DATA_INIT(CGSGLLSSJ)
	m_sh = FALSE;
	m_sz = FALSE;
	m_Cover = -1;
	m_AssignPeriod = -1;
	m_NumberOfDays = 0;
	m_install_start = 0;
	m_install_end = 0;
	m_produce_end = 0;
	m_produce_start = 0;
	//}}AFX_DATA_INIT
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
	pDoc = pFm->m_taiShanDoc ;
	
}

CGSGLLSSJ::~CGSGLLSSJ()
{
}

void CGSGLLSSJ::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGSGLLSSJ)
	DDX_Control(pDX, IDC_PROGRESSYIELD, m_ProgressYield);
	DDX_Control(pDX, IDC_EDIT_DESTINATION, m_destination);
	//DDX_Control(pDX, IDC_EDIT_END, m_end);
	//DDX_Control(pDX, IDC_EDIT_START, m_start);
	//DDX_Control(pDX, IDC_EDIT_FIXSTARTDATE, m_FixStartDate);
	//DDX_Control(pDX, IDC_EDIT_FIXENDDATE, m_FixEndDate);
	DDX_Control(pDX, IDC_BUTTON_SEARCHFIXDATA, m_SearchFixData);
	DDX_Control(pDX, IDC_EDIT_DATASOURCE, m_DataSource);
	DDX_Control(pDX, IDC_COMBO_DATATYPE, m_ComboDatatype);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress1);
	DDX_Check(pDX, IDC_CHECK_SH, m_sh);
	DDX_Check(pDX, IDC_CHECK_SZ, m_sz);
	DDX_Radio(pDX, IDC_RADIO_COVER, m_Cover);
	DDX_Radio(pDX, IDC_RADIO_ASSIGNPERIOD, m_AssignPeriod);
	DDX_Text(pDX, IDC_EDIT_ASSIGNDAYS, m_NumberOfDays);
	DDV_MinMaxInt(pDX, m_NumberOfDays, 1, 6400);
	DDX_DateTimeCtrl(pDX, IDC_INSTALL_START, m_install_start);
	DDX_DateTimeCtrl(pDX, IDC_INSTALL_END, m_install_end);
	DDX_DateTimeCtrl(pDX, IDC_PRODUCE_END, m_produce_end);
	DDX_DateTimeCtrl(pDX, IDC_PRODUCE_START, m_produce_start);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGSGLLSSJ, CPropertyPage)
	//{{AFX_MSG_MAP(CGSGLLSSJ)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHFIXDATA, OnButtonSearchfixdata)
	ON_BN_CLICKED(IDD_BUTTON_BEGINFIX, OnButtonBeginInstall)
	ON_BN_CLICKED(IDC_RADIO_REPAIRE, OnRadioRepaire)
	ON_BN_CLICKED(IDC_BUTTON_DESTINATION, OnButtonDestination)
	ON_BN_CLICKED(ID_BUTTON_BEGINPRODUCE, OnButtonBeginproduce)
	ON_BN_CLICKED(IDC_RADIO_DAY, OnRadioDay)
	ON_CBN_SELCHANGE(IDC_COMBO_DATATYPE, OnSelchangeComboDatatype)
	ON_BN_CLICKED(IDC_RADIO_INSTALLDATE, OnRadioInstalldate)
	ON_BN_CLICKED(IDC_RADIO_COVER, OnRadioCover)
	ON_BN_CLICKED(IDC_RADIO_ASSIGNPERIOD, OnRadioAssignperiod)
	ON_BN_CLICKED(IDC_RADIO_RECENTDAYS, OnRadioRecentdays)
	ON_CBN_SELCHANGE(IDC_INTYPE, OnSelchangeIntype)
	ON_CBN_CLOSEUP(IDC_OUTTYPE, OnCloseupOuttype)
	ON_EN_CHANGE(IDC_EDIT_ASSIGNDAYS, OnChangeEditAssigndays)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGSGLLSSJ message handlers

BOOL CGSGLLSSJ::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	//数据类型, 日线还是五分钟线. 
	((CComboBox*)GetDlgItem(IDC_INTYPE))->SetCurSel(0);  
	((CComboBox*)GetDlgItem(IDC_OUTTYPE))->SetCurSel(0);  
	///////////////////////////
	//安装数据部分的初始化
	m_progress1.SetPos(0);
	//安装数据格式
	m_ComboDatatype.InsertString( 0, ""+g_strCompanyName+"数据格式");
	m_ComboDatatype.InsertString( 1, "钱龙数据格式");
	m_ComboDatatype.InsertString( 2, "分析家数据格式");
	m_ComboDatatype.InsertString( 3, "汇金数据格式");
	m_ComboDatatype.InsertString( 4, "胜龙数据格式");
	
	//初始为天龙格式
	m_ComboDatatype.SetCurSel(0);
	//安装数据文件
	m_DataSource.ReplaceSel("d:\\dest.day");
	
	//安装方式
	m_Cover=0;
	GetDlgItem(IDC_INSTALL_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_INSTALL_END)->EnableWindow(FALSE);


	//安装, 生成日期示例
	m_install_start=CTime::GetCurrentTime();
	m_install_end  =CTime::GetCurrentTime();
	m_produce_start=CTime::GetCurrentTime();
	m_produce_end  =CTime::GetCurrentTime();


	
 	///////////////////////////
	// 生成数据部分的初始化
	m_AssignPeriod=1;    //时段生成还是指定天数
	GetDlgItem(IDC_PRODUCE_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_PRODUCE_END)->EnableWindow(FALSE);
	m_destination.ReplaceSel("d:\\dest.day");

	//m_day=0;			 //日线还是分时线	
	m_ProgressYield.SetPos(0);
	m_sh=m_sz=1;
	m_NumberOfDays=1;

	//CheckRadioButton(IDC_RADIO_FIVEMINUTES,IDC_RADIO_DAY,IDC_RADIO_TIMESPAN);
	
	UpdateData(FALSE);	
	return TRUE;  

}

void CGSGLLSSJ::OnButtonSearchfixdata() 
{
	CString filename;

	CString DefExt="*.day, *.dad, *.psd, *.min";  //文件扩展名.
	CString FileName="D:\\dest.day";  //最初出现在文件名编辑框中的文件名
	CString Filter="line day data file(*.day)|*.day|(*.dad)|*.dad|(*.psd)|*.psd|(*.min)|*.min||";  //.文件扩展名过滤器
	CFileDialog bOpenFileDialog(TRUE,(LPCSTR)DefExt,
		(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		(LPCSTR)Filter,this);
#ifdef NET_CLIENT
	bOpenFileDialog.m_ofn .lpstrInitialDir = CMainFrame::m_taiShanDoc ->m_sPrivatePath ;
#else
	bOpenFileDialog.m_ofn .lpstrInitialDir = CMainFrame::m_taiShanDoc ->m_CurrentWorkDirectory;
#endif
	if( bOpenFileDialog.DoModal() ==IDOK )
	{
		filename=bOpenFileDialog.GetPathName();
	}

	m_DataSource.SetSel(0,-1);
	m_DataSource.ReplaceSel(filename);
	m_DataSource.GetWindowText(SourceFileName);

	CMDIChildWnd * pChildFrame=((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc * pDoc=(CTaiShanDoc *)pChildFrame->GetActiveDocument();
	ASSERT_VALID(pDoc);
	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);

}


void CGSGLLSSJ::OnRadioRepaire() 
{
	GetDlgItem(IDC_INSTALL_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_INSTALL_END)->EnableWindow(FALSE);

}


void CGSGLLSSJ::OnButtonDestination() 
{
	CString filename;
	
	CString DefExt="*.day,*.min";  //文件扩展名.
	CString FileName="D:\\dest.day";  //最初出现在文件名编辑框中的文件名
	CString Filter="K line data file (*.day)|*.day|(*.min)|*.min||";  //.文件扩展名过滤器
	CFileDialog bSaveFileDialog(FALSE,(LPCSTR)DefExt,
		(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		(LPCSTR)Filter,this);
#ifdef NET_CLIENT
	bSaveFileDialog.m_ofn .lpstrInitialDir = CMainFrame::m_taiShanDoc ->m_sPrivatePath ;
#else
	bSaveFileDialog.m_ofn .lpstrInitialDir = CMainFrame::m_taiShanDoc ->m_CurrentWorkDirectory;
#endif
	if( bSaveFileDialog.DoModal() ==IDOK )
	{
		filename=bSaveFileDialog.GetPathName();
		m_destination.SetSel(0,-1);
		m_destination.ReplaceSel(filename);
		m_DataSource.GetWindowText(DestinationFile);
	}


	CMDIChildWnd * pChildFrame=((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc * pDoc=(CTaiShanDoc *)pChildFrame->GetActiveDocument();
	ASSERT_VALID(pDoc);
	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);

}


void CGSGLLSSJ::OnRadioDay() 
{
	// TODO: Add your control notification handler code here
	m_destination.SetSel(0,-1);
	m_destination.ReplaceSel("d:\\dest_sh.day");
}

void CGSGLLSSJ::OnCancel() 
{
	// TODO: Add extra cleanup here
	CPropertyPage::OnCancel();
}


//得到总秒数
BOOL CGSGLLSSJ::GetStockTime(int ymd, LONG& tm)
{
	CString szTime;
	szTime.Format("%d",ymd);
	if( szTime.GetLength()!=8 )
		return 0;
	int year=atoi(szTime.Mid(0,4));
	int month=atoi(szTime.Mid(4,2));
	int day=atoi(szTime.Mid(6,2));
	if( year>1970 && year <2038 && month>=1 && month<=12 && day>=1 && day<=31)
	{
		CTime time( year,month,day, 9, 30, 0);
		tm=time.GetTime();
		return  1;
	}

	return 0;

}


//FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
// 安装数据, 包括覆盖安装和补充安装
void CGSGLLSSJ::OnButtonBeginInstall() 
{
	//朴捉光标
	SetCapture();
	BeginWaitCursor();

	::SetCurrentDirectory( pDoc->m_CurrentWorkDirectory );

	//停止补日线
	pDoc->m_systemOption.autoday=FALSE;

	//开始时间,结束时间
	UpdateData(TRUE);
	int BeginDate=atoi(m_install_start.Format("%Y%m%d"));
	int EndDate  =atoi(m_install_end.Format("%Y%m%d"));

	if(BeginDate > EndDate)	//处理异常
	{
		AfxMessageBox("时间颠倒，重新输入！",MB_ICONSTOP);
		return;
	}

	//安装日线还是五分钟线.
	if( ((CComboBox*)GetDlgItem(IDC_INTYPE))->GetCurSel()==0  )  //安装日线
	{
		//开始安装
		CString srcfilename;
		switch( m_ComboDatatype.GetCurSel( ) )
		{
		case 0:    //ourself
			m_DataSource.GetWindowText(srcfilename);
			if(  srcfilename.IsEmpty() )
			{
				AfxMessageBox("文件名不应为空！");
				return;
			}
			//如果生成在软盘上.
			if( srcfilename[0]=='a'||srcfilename[0]=='b'||srcfilename[0]=='A'||srcfilename[0]=='B')
			{
				if( !InstallFromFloppyDisk(srcfilename) )
				{
					AfxMessageBox("安装没能正常进行！",MB_ICONASTERISK);
					return;
				}
				srcfilename="c:\\source.day";
			}
			else //从硬盘上的分解文件进行安装.
			{
				srcfilename.MakeLower();
				if( srcfilename.Find("tl_disk") != -1 )
				{
					if( ! InstallFromSeveralDiskfile(srcfilename) )
					{
						AfxMessageBox("安装没能正常进行！",MB_ICONASTERISK);
						return;
					}
					srcfilename="c:\\source.day";
				}
			}

			InstallWanshen(srcfilename, BeginDate, EndDate);
			m_progress1.SetPos(0);
			//AfxMessageBox("安装已结束！");
			break;
		case 1:    //钱龙
			m_DataSource.GetWindowText(srcfilename);
			InstallQianlong(srcfilename, BeginDate, EndDate);
			m_progress1.SetPos(0);
			//AfxMessageBox("安装已结束！");
			break;
		case 2:    //分析家
			m_DataSource.GetWindowText(srcfilename);
			InstallFenxijia(srcfilename, BeginDate, EndDate);
			m_progress1.SetPos(0);
			//AfxMessageBox("安装已结束！");
			break;
		case 3:    //汇金
			m_DataSource.GetWindowText(srcfilename);
			InstallHuijin(srcfilename, BeginDate, EndDate);
			m_progress1.SetPos(0);
			//AfxMessageBox("安装已结束！");
			break;
		case 4:    //胜龙
			m_DataSource.GetWindowText(srcfilename);
			InstallShenglong(srcfilename, BeginDate, EndDate);
			m_progress1.SetPos(0);
			//AfxMessageBox("安装已结束！");
			break;

		default:
			AfxMessageBox("没有选择安装数据的类型！", MB_ICONSTOP);
			break;
		}
	}
	else   //安装五分钟线 ----By Huang Yutu.
	{
		InstallMinteData();
	}


	//SEND MESSAGE
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
//	if(pFm->pVwbaseView != NULL)
//		::SendMessage(pFm->pVwbaseView->m_hWnd,WM_USER_TELL_CHANGE,0,200) ;

	//恢复补日线
	pDoc->m_systemOption.autoday=1;	

	ReleaseCapture();
	EndWaitCursor();
}

//////////////////////////////////////////////
LONG CGSGLLSSJ::GetStockDay(time_t NumSeconds)
{
    if(NumSeconds<0)
		return 0;
	CTime time(NumSeconds);
	CString szTime=time.Format("%Y%m%d");
	return atoi(szTime);
}


////////////////////////////////////////////////////////////
void CGSGLLSSJ::YieldDay(CString filename, int &NumberOfStock)
{
	//shenzhen or shanghai
	CTaiKlineFileKLine* pFile = NULL;
	int STOCKSYMBOLLENGTH=0, MaxProgress=0;
	if( filename == "data/shanghai/day.dat" )
	{
		STOCKSYMBOLLENGTH=6;
		pFile = CTaiKlineFileKLine::m_fileDaySh ;
		MaxProgress=pFile->GetStockNumber();
	}
	else if( filename == "data/shenzhen/day.dat" )
	{
		STOCKSYMBOLLENGTH=4;
		pFile = CTaiKlineFileKLine::m_fileDaySz ;
		MaxProgress=pFile->GetStockNumber();
	}

	//开始时间,结束时间
	int BeginDate=atoi(m_produce_start.Format("%Y%m%d"));
	int EndDate  =atoi(m_produce_end.Format("%Y%m%d"));

	if(BeginDate > EndDate 	|| BeginDate>atoi( (CTime::GetCurrentTime()).Format("%Y%m%d") )	)	//处理异常
	{
		AfxMessageBox("没有数据！",MB_ICONASTERISK);
		return;
	}


	//设置进度范围
	m_ProgressYield.SetRange(0,MaxProgress);

	//Open Source file.
	//read now.
	Kline *pKline = NULL;
	CTime tmb = m_produce_start;
	CTime tme = m_produce_end;
	for(int k=0;k<MaxProgress;k++)
	{
		//show progress
		m_ProgressYield.OffsetPos(1);

		int n = pFile->ReadKlinePeriod(k, pKline, tmb, tme);
		if(n==0)
			continue;
		CString s = pFile->GetSymbol(k);
		m_DestFile.Write(s.GetBuffer (8),8);
		m_DestFile.Write(&n , sizeof(int) );
		m_DestFile.Write(pKline,sizeof(Kline)*n);
			
		NumberOfStock++;
	}

	if(pKline!=NULL)	
		delete [] pKline;
	m_ProgressYield.SetPos(0);
}


////////////////////////////////////////////////////////
// 生成数据, 并导出数据到磁盘文件
void CGSGLLSSJ::OnButtonBeginproduce() 
{
	UpdateData(TRUE);
	if( ((CButton*)GetDlgItem(IDC_RADIO_RECENTDAYS))->GetCheck() && (m_NumberOfDays>6400 || m_NumberOfDays<1) )
		return;
		
	//开始时间,结束时间
	int BeginDate=atoi(m_produce_start.Format("%Y%m%d"));
	int EndDate  =atoi(m_produce_end.Format("%Y%m%d"));

	if(BeginDate > EndDate 	|| BeginDate>atoi( (CTime::GetCurrentTime()).Format("%Y%m%d") )	)	//处理异常
	{
		AfxMessageBox("没有数据！",MB_ICONASTERISK);
		return;
	}

	SetCapture();
	BeginWaitCursor();

	::SetCurrentDirectory( pDoc->m_CurrentWorkDirectory );

	//file to produce
	CString DestFileName,FloppyFileName;
	m_destination.GetWindowText(DestFileName);

	if(  DestFileName.IsEmpty() )
	{
		AfxMessageBox("文件名不应为空！");
		return;
	}


	if( IsFileExists( DestFileName ) )
		if( AfxMessageBox("存在同名文件，是否覆盖？",MB_ICONQUESTION|MB_YESNO )==IDNO )
			return;
	UpdateWindow();

	if( ((CComboBox*)GetDlgItem(IDC_OUTTYPE))->GetCurSel()==0  )  //生成日线 
	{
		
		//如果生成在软盘上.
		if( DestFileName[0]=='a'||DestFileName[0]=='b'||DestFileName[0]=='A'||DestFileName[0]=='B')
		{
			FloppyFileName=DestFileName;
			DestFileName="C:\\Source.day";
		}

		m_DestFile.Open(DestFileName, CFile::modeWrite|CFile::modeCreate);
		m_DestFile.Seek(16,CFile::begin);   //预留16个字节存放文件标识和导出的股票数
		int NumberOfStock=0;

		//Only Shanghai.
		if( m_sh == 1 && m_sz == 0) 
		{
			if(m_AssignPeriod==0)  //指定时段
			{
				YieldDay("data/shanghai/day.dat", NumberOfStock);
				int FileID=FILEID22;		   //文件标志
				m_DestFile.SeekToBegin();
				m_DestFile.Write(&FileID,       4  );
				m_DestFile.Write(&NumberOfStock,4  );
				m_DestFile.Close();
			}
			else
			{
				AssignDays("data/shanghai/day.dat", NumberOfStock);  //指定天数生成
				int FileID=FILEID22;		   //文件标志
				m_DestFile.SeekToBegin();
				m_DestFile.Write(&FileID,       4  );
				m_DestFile.Write(&NumberOfStock,4  );
				m_DestFile.Close();
			}

			if( FloppyFileName.Mid(0,2)=="a:"||FloppyFileName.Mid(0,2)=="b:"||FloppyFileName.Mid(0,2)=="A:"||FloppyFileName.Mid(0,2)=="B:")
				Produce2FloppyDisk(DestFileName,FloppyFileName);
			else
			{
				CFile file(DestFileName,CFile::modeRead);
				int FileLength=file.GetLength();
				CString Path, Name;
				Path=file.GetFilePath();
				Name=file.GetFileName();
				file.Close();
				Path=Path.Left(Path.GetLength()-Name.GetLength()-1);
				if( FileLength > 1457000 )
				{
					if( AfxMessageBox("分解生成的数据文件吗？",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDYES  )
					{
						UpdateWindow();
						SplitOneFileToManyFiles(Path,DestFileName);
					}
				}
				else
					AfxMessageBox("生成数据完毕！",MB_ICONASTERISK );
			}

		}

		//Only Shenzhen.
		if( m_sh == 0 && m_sz == 1)
		{
			if(m_AssignPeriod==0)  //指定时段
			{
				YieldDay("data/shenzhen/day.dat", NumberOfStock);
				int FileID=FILEID22;		   //文件标志
				m_DestFile.SeekToBegin();
				m_DestFile.Write(&FileID,       4  );
				m_DestFile.Write(&NumberOfStock,4  );
				m_DestFile.Close();
			}
			else
			{
				if( m_NumberOfDays >6400 )	
				{
					m_DestFile.Close();
					return;
				}

				AssignDays("data/shenzhen/day.dat", NumberOfStock);  //指定天数生成
				int FileID=FILEID22;		   //文件标志
				m_DestFile.SeekToBegin();
				m_DestFile.Write(&FileID,       4  );
				m_DestFile.Write(&NumberOfStock,4  );
				m_DestFile.Close();
			}
			if( FloppyFileName.Mid(0,2)=="a:"||FloppyFileName.Mid(0,2)=="b:"||FloppyFileName.Mid(0,2)=="A:"||FloppyFileName.Mid(0,2)=="B:")
				Produce2FloppyDisk(DestFileName,FloppyFileName);
			else
			{
				CFile file(DestFileName,CFile::modeRead);
				int FileLength=file.GetLength();
				CString Path, Name;
				Path=file.GetFilePath();
				Name=file.GetFileName();
				file.Close();
				Path=Path.Left(Path.GetLength()-Name.GetLength()-1);
				if( FileLength > 1457000 )
				{
					if( AfxMessageBox("分解生成的数据文件吗？",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDYES  )
					{
						UpdateWindow();
						SplitOneFileToManyFiles(Path,DestFileName);
					}
				}
				else
					AfxMessageBox("生成数据完毕！",MB_ICONASTERISK );
			}

		}

		//Both Shenzhen and Shanghai.
		if( m_sh == 1 && m_sz == 1)
		{
			if(m_AssignPeriod==0)  //指定时段
			{
				YieldDay("data/shenzhen/day.dat", NumberOfStock);
				YieldDay("data/shanghai/day.dat", NumberOfStock);

				int FileID=FILEID22;		   //文件标志
				m_DestFile.SeekToBegin();
				m_DestFile.Write(&FileID,       4  );
				m_DestFile.Write(&NumberOfStock,4  );

				m_DestFile.Close();
			}
			else
			{
				if( m_NumberOfDays >6400 )	
				{
					m_DestFile.Close();
					return;
				}

				AssignDays("data/shanghai/day.dat", NumberOfStock);  //指定天数生成
				AssignDays("data/shenzhen/day.dat", NumberOfStock);  //指定天数生成
				int FileID=FILEID22;		   //文件标志
				m_DestFile.SeekToBegin();
				m_DestFile.Write(&FileID,       4  );
				m_DestFile.Write(&NumberOfStock,4  );
				m_DestFile.Close();
			}
			if( FloppyFileName.Mid(0,2)=="a:"||FloppyFileName.Mid(0,2)=="b:"||FloppyFileName.Mid(0,2)=="A:"||FloppyFileName.Mid(0,2)=="B:")
				Produce2FloppyDisk(DestFileName,FloppyFileName);
			else
			{
				CFile file(DestFileName,CFile::modeRead);
				int FileLength=file.GetLength();
				CString Path, Name;
				Path=file.GetFilePath();
				Name=file.GetFileName();
				file.Close();
				Path=Path.Left(Path.GetLength()-Name.GetLength()-1);
				if( FileLength > 1457000 )
				{
					if( AfxMessageBox("分解生成的数据文件吗？",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDYES  )
					{
						UpdateWindow();
						SplitOneFileToManyFiles(Path,DestFileName);
					}
				}
				else
					AfxMessageBox("生成数据完毕！",MB_ICONASTERISK );
			}
		
		}

		//Neither Shenzhen nor Shanghai.
		if( m_sh == 0 && m_sz == 0)
		{
			AfxMessageBox("请选择深圳和(或)上海的历史数据。",MB_ICONASTERISK);			
			m_DestFile.Close();
		}

	}
	else     //生成五分钟线.--By Huang Yutu.
	{
		ProduceMinuteData() ;
	}

	ReleaseCapture();
	EndWaitCursor();

}


//////////////////////////////////////////////////////////////
///////////////////////   安装数据  //////////////////////////
//打开汇金的导出数据文件
void CGSGLLSSJ::InstallHuijin(CString srcfilename, int BeginDate, int EndDate) 
{
	if(  BeginDate<19700101 ||BeginDate>20380101||
		 EndDate  <19700101 ||EndDate>20380101 )
	{
		AfxMessageBox("必须输入正确的时间！");
		return;
	}
	if( BeginDate>EndDate)
	{
		AfxMessageBox("输入时间非法！",MB_ICONASTERISK);
		return;
	}
	
	{// //////////////////////////////////////////////////////

		//计算此目录下, 满足条件的文件有多少
		int NumFile=0;
		CString FinderName;
		CFileFind finder;
		m_DataSource.GetWindowText(FinderName);
		BOOL bwork = finder.FindFile(FinderName);
		while( bwork )
		{
			bwork=finder.FindNextFile();
			NumFile++;
		}

		if( !NumFile )
		{
			AfxMessageBox("没有要安装的数据。",MB_ICONASTERISK);
			return;
		}

		m_progress1.SetRange32(0,NumFile);

		//Fenxijia data file.
		Kline Tianlong;
		CString StockName;  //股票代码	  
		CString szTime;
		CTime tm;

		//遍历此目录下的所有文件,进行安装
		BOOL bWorking = finder.FindFile(srcfilename);
		while( bWorking )
		{
			MSG message;
			if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}  
			
			//
			m_InstallArray.RemoveAll();
			m_progress1.OffsetPos(1);

			bWorking=finder.FindNextFile();

			//Huijin存储数据文件, 而不是导出的数据文件.
			CString HuijinFileName=finder.GetFilePath();
			CFile HuijinFile(HuijinFileName,CFile::modeRead);
			int FileLength=HuijinFile.GetLength();
			
			StockName=finder.GetFileTitle();
			if( StockName.GetLength()!=8 || StockName.GetLength()!=6) 	continue;  //if not shanghai , then return.
			StockName=StockName.Right(StockName.GetLength()-2);

			m_InstallArray.RemoveAll();    //必须清空
			
			int numKline=FileLength/29;

			if( numKline<=0 )
			{
				HuijinFile.Close();
				continue;
			}

			HUIJIN hj;
			HUIJIN* huijin=new HUIJIN[numKline];
			for( int j=0; j < numKline; j++)
			{

				HuijinFile.Read( &hj, 29 );
				huijin[j]=hj;
			}
			HuijinFile.Close();

			LONG time;
			for( int i=0; i < numKline; i++)
			{
				if( GetStockTime(huijin[i].nDate,time) )
				{
					Tianlong.day   = time;
					Tianlong.open   = huijin[i].fOpen;
					Tianlong.high   = huijin[i].fHigh;
					Tianlong.low    = huijin[i].fLow;
					Tianlong.close  = huijin[i].fClose;
					Tianlong.vol = (float)huijin[i].nVolume;
					Tianlong.amount = (float)(huijin[i].nVolume*huijin[i].fMean*100.0);
					Tianlong.advance= 0;
					Tianlong.decline= 0;
					m_InstallArray.Add(Tianlong);
				}
			}
			delete []huijin;
			
			//
			if(m_InstallArray.GetSize()==0)	
				continue;
			InstallStockKlineEach( StockName, BeginDate, EndDate);
		}
			
	}  

}


////////////////////////////////////////////////////////////////
//安装钱龙格式的导出文件
void CGSGLLSSJ::InstallQianlong(CString srcfilename, int BeginDate, int EndDate) 
{
	if(  BeginDate<19700101 ||BeginDate>20380101 ||
		 EndDate  <19700101 ||EndDate  >20380101 || BeginDate > EndDate )
	{
		AfxMessageBox("必须输入正确的时间！",MB_ICONASTERISK);
		return;
	}

	//本地数据 Shanghai//////////////////////////////////////////
	{
		//计算此目录下, 满足条件的文件有多少
		int NumFile=0;
		CString FinderName;
		CFileFind finder;
		m_DataSource.GetWindowText(FinderName);
		BOOL bwork = finder.FindFile(FinderName);
		while( bwork )
		{
			bwork=finder.FindNextFile();
			NumFile++;
		}
		if( !NumFile )
		{
			AfxMessageBox("没有要安装的数据。",MB_ICONASTERISK);
			return;
		}

		m_progress1.SetRange32(0,NumFile);
		m_progress1.SetPos(0);

		Kline Wanshen;
		CString StockName;  //股票代码	  
		CString szTime;
		CTime tm;

		//遍历此目录下的所有文件,进行安装
		BOOL bWorking = finder.FindFile(srcfilename);
		while( bWorking )
		{
			MSG message;
			if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}  
			
			//
			m_InstallArray.RemoveAll();
			m_progress1.OffsetPos(1);

			bWorking=finder.FindNextFile();

			//钱龙存储数据文件, 而不是导出的数据文件, 因为导出的数据文件是压缩的.
			CString QianlongFileName=finder.GetFilePath();
			CFile QianlongFile((LPCTSTR)QianlongFileName,CFile::modeRead);
			int FileLength=QianlongFile.GetLength();
			
			StockName=finder.GetFileTitle();;
			if( StockName.GetLength() !=6&&StockName.GetLength() !=4 )		continue;
			
			m_InstallArray.RemoveAll();    //必须清空
		
			int numKline=FileLength/40;
			
			if( numKline <= 0 ) 
			{
				QianlongFile.Close();
				continue;
			}

			QIANLONG* Qianlong=new QIANLONG[numKline];
			QianlongFile.Read( Qianlong, sizeof(QIANLONG)*numKline );
			QianlongFile.Close();

			LONG time;
			for( int i=0; i < numKline; i++)
			{
				if( GetStockTime(Qianlong[i].dwDate,time) && Qianlong[i].dwLow  >0 && Qianlong[i].dwVolume  >0 ) 
				{
					Wanshen.day   = time; 
					Wanshen.open   = (float)(Qianlong[i].dwOpen/1000.0);
					Wanshen.high   = (float)(Qianlong[i].dwHigh/1000.0);
					Wanshen.low    = (float)(Qianlong[i].dwLow/1000.0);
					Wanshen.close  = (float)(Qianlong[i].dwClose/1000.0);
					Wanshen.vol = (float)(Qianlong[i].dwVolume);
					Wanshen.amount = (float)(Qianlong[i].dwAmount*1000.0);
					Wanshen.advance=		  Qianlong[i].wUp;
					Wanshen.decline=		  Qianlong[i].wDown;
					m_InstallArray.Add(Wanshen);
				}
			}
			delete []Qianlong;
			
			//进行安装Shanghai.
			StockName.MakeUpper();
			InstallStockKlineEach( StockName, BeginDate, EndDate);
		}

	}

}



//////////////////////////////////////////////////////////////////////
//安装天龙格式的导出文件
void CGSGLLSSJ::InstallWanshen(CString srcfilename, int BeginDate, int EndDate) 
{
	{//begin*************  ****************
	//本地数据
	//源数据文件的名称和路径. 
	CString SourceFilename=srcfilename;
	UpdateData(TRUE);	
	//m_DataSource.GetWindowText(SourceFilename);

	//如果生成在软盘上.
	if( SourceFilename[0]=='a'||SourceFilename[0]=='b'||SourceFilename[0]=='A'||SourceFilename[0]=='B')
		SourceFilename="c:\\source.day";
	
	CFile SourceFile(SourceFilename,CFile::modeRead);
	
	//待安装的证券总数
	int NumStock=0 ;             
	int FileID;  //文件标识
	SourceFile.Read(&FileID,4);
	if( FileID!=FILEID22)
	{
		AfxMessageBox("非"+g_strCompanyName+"数据格式。",MB_ICONSTOP);
		return;
	}
	SourceFile.Read(&NumStock,sizeof(int));
	SourceFile.Seek(16,CFile::begin);
	
	//设置进度条的范围
	m_progress1.SetRange(0,NumStock);
	
	//此股票的K线数目
	int NumKline=0;
	char StockName[8];	
	Kline wanshen;
	CTime tm;       //安装时间

	for( int nStock=0; nStock<NumStock; nStock++)
	{

		MSG message;
		if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}  

		//show the progress
		m_progress1.SetPos(nStock);
		m_InstallArray.RemoveAll();

		//待安装的数据
		SourceFile.Read(StockName,8);
		SourceFile.Read(&NumKline,4);
		for( int nKline=0; nKline<NumKline; nKline++)
		{
			SourceFile.Read( &wanshen,sizeof(Kline) );
			m_InstallArray.Add(wanshen);
		}
		CString symbol(StockName);
		InstallStockKlineEach( symbol, BeginDate, EndDate);
	}

	////////////////////////////////////////////////////////////////
	//关闭数据源文件和本地文件
	SourceFile.Close();

	}//end************* ****************
}


void CGSGLLSSJ::OnSelchangeComboDatatype() 
{
	// TODO: Add your control notification handler code here
	switch( m_ComboDatatype.GetCurSel( ) )
	{
	case 0:    //ourself
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCHFIXDATA))->EnableWindow(TRUE);
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("D:\\dest.day");
		break;
	case 1:    //钱龙
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCHFIXDATA))->EnableWindow(FALSE);
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("C:\\ML30\\DATA\\SHASE\\day\\*.day");
		break;
	case 2:    //分析家
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCHFIXDATA))->EnableWindow(TRUE);
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("d:\\superstk.dad");
		break;
	case 3:    //汇金
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCHFIXDATA))->EnableWindow(FALSE);
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("C:\\HUIJIN\\EXE\\LINEDAY\\SH\\*.psd");
		break;
	case 4:    //胜龙
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCHFIXDATA))->EnableWindow(FALSE);
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("D:\\SLON\\DATA\\SH\\DAY\\*.DAY");
		break;

	}
	
}
//功能: 选择时段安装, 激活时间编辑框.
void CGSGLLSSJ::OnRadioInstalldate() 
{
	GetDlgItem(IDC_INSTALL_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_INSTALL_END)->EnableWindow(TRUE);
}
//功能: 按照指定的天数生成数据.
//      指定的天数为 1=< m_AssignDays < 6400 为一无符号整数.
void CGSGLLSSJ::AssignDays(CString filename, int &NumberOfStock)  //指定天数生成
{
	//判断是深圳还是上海?
	//shenzhen or shanghai
	CTaiKlineFileKLine* pFile = NULL;
	int MaxProgress=0;
	if( filename == "data/shanghai/day.dat" )
	{
		pFile = CTaiKlineFileKLine::m_fileDaySh ;
		MaxProgress=pFile->GetStockNumber();
	}
	else if( filename == "data/shenzhen/day.dat" )
	{
		pFile = CTaiKlineFileKLine::m_fileDaySz ;
		MaxProgress=pFile->GetStockNumber();
	}
	//设置进度范围
	m_ProgressYield.SetRange32(0,MaxProgress);

	//read now.
	Kline *pKline = NULL;
	for(int k=0;k<MaxProgress;k++)
	{
		//show progress
		m_ProgressYield.OffsetPos(1);

		int n = pFile->ReadKLine(k, pKline, m_NumberOfDays);
		if(n==0)
			continue;
		CString s = pFile->GetSymbol(k);
		m_DestFile.Write(s.GetBuffer (8),8);
		m_DestFile.Write(&n , sizeof(int) );
		m_DestFile.Write(pKline,sizeof(Kline)*n);
			
		NumberOfStock++;
	}
	m_ProgressYield.SetPos(0);
	if(pKline!=NULL)	
		delete [] pKline;
}



void CGSGLLSSJ::OnRadioCover() 
{
	GetDlgItem(IDC_INSTALL_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_INSTALL_END)->EnableWindow(FALSE);
}

// for data produce
void CGSGLLSSJ::OnRadioAssignperiod() 
{
	m_NumberOfDays=1;
	UpdateData(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_ASSIGNPERIOD))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RADIO_RECENTDAYS))->SetCheck(0);

	GetDlgItem(IDC_EDIT_ASSIGNDAYS)->EnableWindow(FALSE);
	GetDlgItem(IDC_PRODUCE_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_PRODUCE_END)->EnableWindow(TRUE);

	((CEdit*)GetDlgItem( IDC_EDIT_ASSIGNDAYS))->EnableWindow(FALSE);
	
}

void CGSGLLSSJ::OnRadioRecentdays() 
{
	GetDlgItem(IDC_PRODUCE_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_PRODUCE_END)->EnableWindow(FALSE);
	((CEdit*)GetDlgItem( IDC_EDIT_ASSIGNDAYS))->EnableWindow(TRUE);
}


//导出数据到软盘. 按提示依次插入各片软盘.
void CGSGLLSSJ::Produce2FloppyDisk(CString DestFileName,CString FloppyFileName)
{
	CFile SrcFile;   //在硬盘上的数据文件.
	
	//异常处理.
	if( !SrcFile.Open(DestFileName,CFile::modeRead) )
	{
		AfxMessageBox("Open file error",MB_ICONSTOP);
		return;
	}
	int LengthOfSrcFile=SrcFile.GetLength();  //文件长度.
	
	//大约需要几张磁盘.
	int BytesPerDisk=1457000;  
	int NumOfDisks= LengthOfSrcFile%BytesPerDisk==0 ? LengthOfSrcFile/BytesPerDisk:LengthOfSrcFile/BytesPerDisk+1;  
	CString str;
	str.Format("%d",NumOfDisks);
	str="要导出到软盘上, 大约需要 " + str + " 张软盘(3.5寸)。";
	if( AfxMessageBox(str,MB_OKCANCEL)==IDCANCEL )
		return;

	DWORD SectorsPerCluster,BytesPerSector,NumberOfFreeClusters,TotalNumberOfClusters;
	for( int nDisk=1;nDisk<=NumOfDisks+1;nDisk++)
	{
		//判断是否生成完毕.
		int nSize=min(BytesPerDisk,LengthOfSrcFile);
		if( nSize<= 0)
		{
			AfxMessageBox("生成完毕！",MB_ICONASTERISK);
			return;
		}

		//是否继续生成下去.
		str.Format("%d",nDisk);
		str="请插入第 "+str+" 张软盘。";
		if( AfxMessageBox(str,MB_OKCANCEL)==IDCANCEL )
			return;

		//确认磁盘是否为空.
		
		::GetDiskFreeSpace(FloppyFileName.Mid(0,2),&SectorsPerCluster,&BytesPerSector,&NumberOfFreeClusters,&TotalNumberOfClusters);
		int FreeSpacePerDisk=NumberOfFreeClusters*SectorsPerCluster*BytesPerSector;
		if( FreeSpacePerDisk<BytesPerDisk+8 )
		{
			AfxMessageBox("请删除磁盘上的所有数据,或者格式化磁盘。",MB_ICONASTERISK);
			return;
		}

		CFile FloppyFile( FloppyFileName,CFile::modeCreate|CFile::modeWrite);
		FloppyFile.Write(&NumOfDisks,sizeof(4));  //磁盘总数,占四个字节.
		FloppyFile.Write(&nDisk,sizeof(4));       //磁盘编号,占四个字节.
		BYTE *Buffer=new BYTE[nSize];   
		SrcFile.Read(Buffer,nSize);   //读硬盘文件.
		FloppyFile.Write(Buffer,nSize); //写软盘文件.
		delete []Buffer;              //释放内存.
		FloppyFile.Close();
		
		LengthOfSrcFile-=nSize;       //每次减去 nSize 个字节.
	}

	SrcFile.Remove(DestFileName);  //

}

BOOL CGSGLLSSJ::InstallFromFloppyDisk(CString FloppyFileName,bool bDayKline)
{
	int NumOfDisks=0, //磁盘总数.
		nDisk=0;      //磁盘编号.
	CFile SrcFile;    //源数据文件.
	CFile DestFile;   //硬盘上的数据.
	CString sExd = ".day";
	if(bDayKline==false)
		sExd = ".min";
	if( !DestFile.Open("c:\\source"+sExd,CFile::modeWrite|CFile::modeCreate) )
	{
		AfxMessageBox("Open file error!",MB_ICONSTOP);
		return 0;
	}

	int IndexOfDisk=1;  //磁盘索引.
	int nRepeat=0;      //重复次数.
	//do-while循环进行安装.
	do
	{
		CString str;
		str.Format("%d",IndexOfDisk);
		AfxMessageBox("插入第 "+str+" 号磁盘。");
		
		CFileFind finder;
		BOOL bWorking=finder.FindFile(FloppyFileName.Left(3)+"*.*");
		while(bWorking)
		{
			bWorking=finder.FindNextFile();
			FloppyFileName=finder.GetFilePath();
		}

		if( !SrcFile.Open(FloppyFileName,CFile::modeRead) )
		{
			AfxMessageBox("Open file error!",MB_ICONSTOP);
			return 0;
		}
		SrcFile.Read(&NumOfDisks,4);  //读磁盘总数.
		SrcFile.Read(&nDisk,4);       //读磁盘编号.
		int LengthOfFile=SrcFile.GetLength();  //数据文件长度.
		
		//如果磁盘循序错误.
		if( nDisk != IndexOfDisk )
		{
			nRepeat++;
			AfxMessageBox("重新插入第"+str+"张磁盘。");
			SrcFile.Close();
			if( nRepeat>5) return 0;  //如果重复插入5次仍然不对,则退出.
			continue;
		}

		LengthOfFile-=8;
		BYTE *Buffer=new BYTE[LengthOfFile];
		SrcFile.Read(Buffer,LengthOfFile);  //从软盘上读数据.
		SrcFile.Close();
		DestFile.Write(Buffer,LengthOfFile);//往硬盘上写数据.
		delete []Buffer;
		
		IndexOfDisk++;
		nRepeat=0;

	}while(NumOfDisks>nDisk); //满足条件循环进行.

	DestFile.Close();

	return 1;
}


//把一个文件分解成几个文件,便于拷贝.(Ddisk1, Disk2, ....)
void CGSGLLSSJ::SplitOneFileToManyFiles(CString Dir,CString DestFileName,bool bDayKline)
{
	CFile SrcFile;   //在硬盘上的数据文件.
	
	//异常处理.
	CString sExd = ".day";
	if(bDayKline==false)
		sExd = ".min";
	if( !SrcFile.Open(DestFileName,CFile::modeRead) )
	{
		AfxMessageBox("Open file error",MB_ICONSTOP);
		return;
	}
	int LengthOfSrcFile=SrcFile.GetLength();  //文件长度.
	
	//大约需要几张磁盘.
	int BytesPerDisk=1457000;  
	int NumOfDisks= LengthOfSrcFile%BytesPerDisk==0 ? LengthOfSrcFile/BytesPerDisk:LengthOfSrcFile/BytesPerDisk+1;  

	for( int nDisk=1;nDisk<=NumOfDisks+1;nDisk++)
	{
		//判断是否生成完毕.
		int nSize=min(BytesPerDisk,LengthOfSrcFile);
		if( nSize<= 0)
		{
			AfxMessageBox("生成完毕！",MB_ICONASTERISK);
			return;
		}

		CString DiskXFileName;
		DiskXFileName.Format("%d",nDisk);
		DiskXFileName=Dir+"\\TL_Disk"+DiskXFileName+sExd;
		CFile DiskXFile( DiskXFileName,CFile::modeCreate|CFile::modeWrite);
		DiskXFile.Write(&NumOfDisks,sizeof(4));  //磁盘总数,占四个字节.
		DiskXFile.Write(&nDisk,sizeof(4));       //磁盘编号,占四个字节.
		BYTE *Buffer=new BYTE[nSize];   
		SrcFile.Read(Buffer,nSize);   //读硬盘文件.
		DiskXFile.Write(Buffer,nSize); //写软盘文件.
		delete []Buffer;              //释放内存.
		DiskXFile.Close();
		
		LengthOfSrcFile-=nSize;       //每次减去 nSize 个字节.
	}

	SrcFile.Remove(DestFileName);  //

}



/////////////////////////////////////////////////////////////////////
// 安装分析家的数据格式
void CGSGLLSSJ::InstallFenxijia(CString srcfilename ,int BeginDate, int EndDate) 
{
	//从分析家导出的文件中读数据
	CFile SourceFile(srcfilename,CFile::modeRead);
	int NumStock=0;       //待安装的证券总数
	SourceFile.Seek(8,CFile::begin);
	SourceFile.Read(&NumStock,sizeof(int));

	int NumKlinesOfFxj =  (SourceFile.GetLength()-16)/32;

	if( NumKlinesOfFxj<0 )
		return;

	Kline *tianlong=new Kline[NumKlinesOfFxj]; //最后必须要释放.
	SourceFile.Seek(16,CFile::begin);
	SourceFile.Read(tianlong,32*NumKlinesOfFxj);
	SourceFile.Close();   //关闭分析家的数据文件.

	m_progress1.SetRange32(0,NumStock);    //设置进度条的范围

	CTime tm;   //安装时间
	tm=CTime::GetCurrentTime();
	int CurrentDate=atoi(tm.Format("%Y%m%d"));

	int nFixStyle=GetCheckedRadioButton(IDC_RADIO_COVER,IDC_RADIO_INSTALLDATE); //选择安装方式.

	{//
	CString StockSymbol;
	for( int i=0;i<NumKlinesOfFxj;i++)
	{
		if( tianlong[i].day == -1 )
		{
			m_progress1.OffsetPos(1);  //show the progress

			//有消息就先处理消息.
			MSG message;
			if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}  

			if(StockSymbol.GetLength () == 6||StockSymbol.GetLength () == 4) 
			{
				InstallStockKlineEach( StockSymbol, BeginDate, EndDate);
				m_InstallArray.RemoveAll();
				StockSymbol=(char*)&tianlong[i]+6;
			}
		}
		else
		{
			tm=tianlong[i].day;
			if( tm>0 && atoi(tm.Format("%Y%m%d")) <=CurrentDate )
				m_InstallArray.Add(tianlong[i]);
		}
	}
	}//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	delete []tianlong;   //释放内存.
}




//
void CGSGLLSSJ::OnSelchangeIntype() 
{
	if( ((CComboBox*)GetDlgItem(IDC_INTYPE))->GetCurSel()==0  )		//安装日线 
	{
		m_DataSource.SetWindowText("D:\\dest.day");
		GetDlgItem(IDC_COMBO_DATATYPE)->EnableWindow(TRUE);
	}
	else
	{
		m_DataSource.SetWindowText("D:\\dest.min");
		GetDlgItem(IDC_COMBO_DATATYPE)->EnableWindow(FALSE);
	}
	
}

void CGSGLLSSJ::OnCloseupOuttype() 
{
	if( ((CComboBox*)GetDlgItem(IDC_OUTTYPE))->GetCurSel()==0  )		//安装日线
	{
		m_destination.SetWindowText("D:\\dest.day");
		
	}
	else
	{
		m_destination.SetWindowText("D:\\dest.min");

	}

	
}
int CGSGLLSSJ::InstallFromSeveralDiskfile(CString FileName,bool bDayKline)
{
	int NumOfDisks=0, //磁盘总数.
		nDisk=0;      //磁盘编号.
	CFile SrcFile;    //源数据文件.
	CFile DestFile;   //硬盘上的数据.
	CString sExd = ".day";
	if(bDayKline==false)
		sExd = ".min";
	if( !DestFile.Open("c:\\source"+sExd,CFile::modeWrite|CFile::modeCreate) )
	{
		AfxMessageBox("Open file error!",MB_ICONSTOP);
		return 0;
	}

	int IndexOfDisk=1;  //磁盘索引.
	int nRepeat=0;      //重复次数.
	//do-while循环进行安装.
	do
	{
		CString str;
		str.Format("%d",IndexOfDisk);
		//AfxMessageBox("插入第 "+str+" 号磁盘.");
		
		FileName.Delete( FileName.GetLength()-5 );
		FileName.Insert( FileName.GetLength()-4, str );
		if( !SrcFile.Open(FileName,CFile::modeRead) )
		{
			AfxMessageBox("Open file error!",MB_ICONSTOP);
			return 0;
		}
		SrcFile.Read(&NumOfDisks,4);  //读磁盘总数.
		SrcFile.Read(&nDisk,4);       //读磁盘编号.
		int LengthOfFile=SrcFile.GetLength();  //数据文件长度.
		
		//如果磁盘循序错误.
		if( nDisk != IndexOfDisk )
		{
			SrcFile.Close();
			return 0;
		}

		LengthOfFile-=8;
		BYTE *Buffer=new BYTE[LengthOfFile];
		SrcFile.Read(Buffer,LengthOfFile);  //从软盘上读数据.
		SrcFile.Close();
		DestFile.Write(Buffer,LengthOfFile);//往硬盘上写数据.
		delete []Buffer;
		
		IndexOfDisk++;
		nRepeat=0;

	}while(NumOfDisks>nDisk); //满足条件循环进行.

	DestFile.Close();

	return 1;

}
//安装胜龙的数据
void CGSGLLSSJ::InstallShenglong(CString srcfilename, int BeginDate, int EndDate)
{
	//时间检验
	if(  BeginDate<19700101 ||BeginDate>20380101||
		 EndDate  <19700101 ||EndDate  >20380101|| 
		 BeginDate>EndDate   )
	{
		AfxMessageBox("非法输入！重新输入正确的时间！");
		return;
	}

	{ //shanghai shanghai shanghai shanghai shanghai 
		//计算此目录下, 满足条件的文件有多少
		int NumFile=0;
		CString FinderName;
		CFileFind finder;
		//从编辑框中读入要查找的文件类型, 例如 D:\SLON\DATA\SH\DAY\*.day
		m_DataSource.GetWindowText(FinderName);
		BOOL bwork = finder.FindFile(FinderName);
		while( bwork )
		{
			bwork=finder.FindNextFile();
			NumFile++;
		}

		if( !NumFile )
		{
			AfxMessageBox("没有要安装的数据。",MB_ICONASTERISK);
			return;
		}


		//设置进度条的滚动范围.
		m_progress1.SetRange32(0,NumFile);
		m_progress1.SetPos(0);

		CString StockName, /*股票代码*/ 	szTime;
		CTime tm;

		//遍历此目录下的所有文件,进行安装
		BOOL bWorking = finder.FindFile(srcfilename);
		while( bWorking )
		{
			//优先处理Windows消息.
			MSG message;
			if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}  
			
			//每安装一只股票,首先要清除所有的元素.
			m_InstallArray.RemoveAll();
			m_progress1.OffsetPos(1);

			bWorking=finder.FindNextFile();

			//胜龙存储数据文件, 而不是导出的数据文件
			CString ShenglongFileName=finder.GetFilePath();
			CFile ShenglongFile(ShenglongFileName,CFile::modeRead);
			int FileLength=ShenglongFile.GetLength();
			
			StockName=finder.GetFileTitle();
			if( StockName.GetLength() != 6||StockName.GetLength() != 4 ) continue;
									
			int NumShenglong=FileLength/sizeof(SHENGLONG);                 //文件中共有的日线数目.
			
			if( NumShenglong <= 0 )
			{
				ShenglongFile.Close();
				continue;
			}

			SHENGLONG* SLarray=new SHENGLONG[NumShenglong];
			//int *DateList=new int[NumShenglong];            //定义一个时间数组
			ShenglongFile.Read( SLarray, sizeof(SHENGLONG)*NumShenglong );
			ShenglongFile.Close();

			Kline wsh;
			for( int i=0; i < NumShenglong; i++)
			{
				if( ShenglongToTianlong(SLarray[i],wsh) )
					m_InstallArray.Add(wsh);
			}
			delete []SLarray;

			if( m_InstallArray.GetSize()==0 )	//如果满足条件的日线个数为零,则返回.
				continue;
			
			InstallStockKlineEach( StockName, BeginDate, EndDate);
		}
	} //shanghai shanghai shanghai shanghai shanghai 
}


BOOL CGSGLLSSJ::ShenglongToTianlong(SHENGLONG shenglong,Kline& Wanshen)
{
	shenglong.date=~shenglong.date;
	shenglong.oprice=~shenglong.oprice;
	shenglong.cprice=~shenglong.cprice;
	shenglong.hprice=~shenglong.hprice;
	shenglong.lprice=~shenglong.lprice;
	shenglong.tovalue=~shenglong.tovalue;
	shenglong.tohands=~shenglong.tohands;
	shenglong.topiece=~shenglong.topiece;
	
	LONG tm;
	if(  GetStockTime(shenglong.date,tm) )
	{
		Wanshen.day   = tm; 
		Wanshen.open   = (float)(shenglong.oprice/1000.0);
		Wanshen.high   = (float)(shenglong.hprice/1000.0);
		Wanshen.low    = (float)(shenglong.lprice/1000.0);
		Wanshen.close  = (float)(shenglong.cprice/1000.0);
		Wanshen.vol = (float)(shenglong.tohands);
		Wanshen.amount = (float)(shenglong.tovalue*1000.0);
		Wanshen.advance=		 0;
		Wanshen.decline=		 0;
		return 1;
	}

	return 0;

	
}
void CGSGLLSSJ::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::OnOK();
}

//***************************************************************************

///////////By Huang Yutu....
void CGSGLLSSJ::InstallMinteData()
{
	//本地数据
	//源数据文件的名称和路径. 
	CString SourceFilename,srcfilename;
	m_DataSource.GetWindowText(SourceFilename);
	srcfilename = SourceFilename;
	//如果生成在软盘上.
	if( srcfilename[0]=='a'||srcfilename[0]=='b'||srcfilename[0]=='A'||srcfilename[0]=='B')
	{
		if( !InstallFromFloppyDisk(srcfilename,false) )
		{
			AfxMessageBox("安装没能正常进行！",MB_ICONASTERISK);
			return;
		}
		srcfilename="c:\\source.min";
	}
	else //从硬盘上的分解文件进行安装.
	{
		srcfilename.MakeLower();
		if( srcfilename.Find("tl_disk") != -1 )
		{
			if( ! InstallFromSeveralDiskfile(srcfilename,false) )
			{
				AfxMessageBox("安装没能正常进行！",MB_ICONASTERISK);
				return;
			}
			srcfilename="c:\\source.min";
		}
	}

	UpdateData(TRUE);	

	//如果生成在软盘上.
//	if( SourceFilename[0]=='a'||SourceFilename[0]=='b'||SourceFilename[0]=='A'||SourceFilename[0]=='B')
//		SourceFilename="c:\\source.min";
	
	CFile SourceFile(SourceFilename,CFile::modeRead);
	
	//待安装的证券总数
	int NumStock=0 ;             
	int FileID;  //文件标识
	SourceFile.Read(&FileID,4);
	if( FileID!=FILEID22)
	{
		AfxMessageBox("非"+g_strCompanyName+"数据格式。",MB_ICONSTOP);
		return;
	}
	SourceFile.Read(&NumStock,sizeof(int));
	SourceFile.Seek(16,CFile::begin);
	
	//设置进度条的范围
	m_progress1.SetRange(0,NumStock);
	
	//此股票的K线数目
	int NumKline=0;
	char StockName[8];	
	Kline wanshen;
	CTime tm;       //安装时间

	for( int nStock=0; nStock<NumStock; nStock++)
	{

		MSG message;
		if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}  

		//show the progress
		m_progress1.SetPos(nStock);
		m_InstallArray.RemoveAll();

		//待安装的数据
		SourceFile.Read(StockName,8);
		SourceFile.Read(&NumKline,4);
		for( int nKline=0; nKline<NumKline; nKline++)
		{
			SourceFile.Read( &wanshen,sizeof(Kline) );
			m_InstallArray.Add(wanshen);
		}
		ASSERT(NumKline>0);

		CString symbol(StockName);
		CTime tm(m_InstallArray[0].day);
		int BeginDate =  atoi(tm.Format ("%Y%m%d"));
		tm = (m_InstallArray[m_InstallArray.GetSize()-1].day);
		int EndDate =  atoi(tm.Format ("%Y%m%d"));
		InstallStockKlineEach( symbol, BeginDate, EndDate,false);
	}

	////////////////////////////////////////////////////////////////
	//关闭数据源文件和本地文件
	SourceFile.Close();
}
void CGSGLLSSJ::ProduceMinuteData() 
{
	CString DestFileName,FloppyFileName;
	m_destination.GetWindowText(DestFileName);
	if(  DestFileName.IsEmpty() )
	{
		AfxMessageBox("文件名不应为空！");
		return;
	}

	if( IsFileExists( DestFileName ) )
		if( AfxMessageBox("存在同名文件，是否覆盖？",MB_ICONQUESTION|MB_YESNO )==IDNO )
			return;

	//如果生成在软盘上.
	if( DestFileName[0]=='a'||DestFileName[0]=='b'||DestFileName[0]=='A'||DestFileName[0]=='B')
	{
		FloppyFileName=DestFileName;
		DestFileName="C:\\Source.min";
	}

	m_DestFile.Open(DestFileName, CFile::modeWrite|CFile::modeCreate);
	m_DestFile.Seek(16,CFile::begin);   //预留16个字节存放文件标识和导出的股票数
	int NumberOfStock=0;
	int FileID=FILEID22;		   //文件标志
	{
		if(m_AssignPeriod==0)  //指定时段
		{
			//	//YieldMinuteFile(bool bShangHai,bool bAssignDays)
			if(m_sh==1)
				NumberOfStock = YieldMinuteFile(true,false);
			if(m_sz==1)
				NumberOfStock += YieldMinuteFile(false,false);
			m_DestFile.SeekToBegin();
			m_DestFile.Write(&FileID,       4  );
			m_DestFile.Write(&NumberOfStock,4  );
			m_DestFile.Close();
		}
		else
		{
			if(m_sh==1)
				NumberOfStock = YieldMinuteFile(true,true);
			if(m_sz==1)
				NumberOfStock += YieldMinuteFile(false,true);
			m_DestFile.SeekToBegin();
			m_DestFile.Write(&FileID,       4  );
			m_DestFile.Write(&NumberOfStock,4  );
			m_DestFile.Close();
		}

		if( FloppyFileName.Mid(0,2)=="a:"||FloppyFileName.Mid(0,2)=="b:"||FloppyFileName.Mid(0,2)=="A:"||FloppyFileName.Mid(0,2)=="B:")
			Produce2FloppyDisk(DestFileName,FloppyFileName);
		else
		{
			CFile file(DestFileName,CFile::modeRead);
			int FileLength=file.GetLength();
			CString Path, Name;
			Path=file.GetFilePath();
			Name=file.GetFileName();
			file.Close();
			Path=Path.Left(Path.GetLength()-Name.GetLength()-1);
			if( FileLength >1457000 )
			{
				if( AfxMessageBox("分解生成的数据文件吗？",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDYES  )
				{
					UpdateWindow();
					SplitOneFileToManyFiles(Path,DestFileName,false);
				}
			}
			else
				AfxMessageBox("生成数据完毕！",MB_ICONASTERISK );
		}

	}
}


BOOL CGSGLLSSJ::IsFileExists( CString filename)
{
	CFile file;
	if( file.Open(_T(filename),CFile::modeRead,NULL) )
	{
		file.Close();
		return TRUE;
	}
	return FALSE;

}

void CGSGLLSSJ::OnChangeEditAssigndays() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO: Add your control notification handler code here
	
}

void CGSGLLSSJ::InstallStockKlineEach(CString symbol,int BeginDate,int EndDate,bool bDayKline)
{
		//Get the pointer of kline file .
		CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer (symbol,bDayKline);
		
		CTime tm;
		//选择安装方式.
		int nFixStyle=GetCheckedRadioButton(IDC_RADIO_COVER,IDC_RADIO_INSTALLDATE);
		switch(nFixStyle)
		{
		case IDC_RADIO_REPAIRE:  //补充安装
			pFile ->WriteKLine(symbol,m_InstallArray.GetData(),m_InstallArray.GetSize(),CTaiKlineFileKLine::insert);
			break;
		case IDC_RADIO_INSTALLDATE:  //指定时段安装
			//删除不满足时间限制的元素
			{
			for( int i=0;i<m_InstallArray.GetSize();i++)
			{
				tm=m_InstallArray[i].day;
				if(  atoi( tm.Format("%Y%m%d") ) < BeginDate  )
				{
					m_InstallArray.RemoveAt(i);
					i--;
				}
				else
					break;
			}
			int nSize=m_InstallArray.GetSize();
			while( nSize-- )
			{
				tm=m_InstallArray[nSize].day;
				if(  atoi( tm.Format("%Y%m%d") ) > EndDate  )
					m_InstallArray.RemoveAt(nSize);
				else
					break;
			}
			
			//exception  
			if( m_InstallArray.GetSize() ==0 ) 
				break;
			}

		case IDC_RADIO_COVER :   //覆盖安装
			pFile ->WriteKLine(symbol,m_InstallArray.GetData(),m_InstallArray.GetSize(),CTaiKlineFileKLine::overWriteAll);
			break;
		}

}

int CGSGLLSSJ::YieldMinuteFile(bool bShangHai,bool bAssignDays)
{
	//shenzhen or shanghai
	CTaiKlineFileKLine* pFile = NULL;
	int MaxProgress=0;
	if( bShangHai == true )
		pFile = CTaiKlineFileKLine::m_fileMin5Sh ;
	else 
		pFile = CTaiKlineFileKLine::m_fileMin5Sz ;
	MaxProgress=pFile->GetStockNumber();

	//开始时间,结束时间
	int BeginDate=atoi(m_produce_start.Format("%Y%m%d"));
	int EndDate  =atoi(m_produce_end.Format("%Y%m%d"));

	if(BeginDate > EndDate 	|| BeginDate>atoi( (CTime::GetCurrentTime()).Format("%Y%m%d") )	)	//处理异常
	{
		AfxMessageBox("没有数据！",MB_ICONASTERISK);
		return 0;
	}


	//设置进度范围
	m_ProgressYield.SetRange(0,MaxProgress);

	//Open Source file.
	//read now.
	if(bAssignDays == true && m_NumberOfDays<=0)
		return 0;

	Kline *pKline = NULL;
	CTime tmb = CTime((int)(m_produce_start.GetTime())/(24*60*60)*(24*60*60));
	CTime tme = CTime((int)(m_produce_end.GetTime())/(24*60*60)*(24*60*60)+(15*60*60)); 
	int NumberOfStock = 0;
	for(int k=0;k<MaxProgress;k++)
	{
		//show progress
		m_ProgressYield.OffsetPos(1);

		int n;
		if(bAssignDays == false)
			n= pFile->ReadKlinePeriod(k, pKline, tmb, tme);
		else
			n = pFile->ReadKLine(k, pKline, m_NumberOfDays*48);
		if(n==0)
			continue;
		n= n/48*48;

		CString s = pFile->GetSymbol(k);
		m_DestFile.Write(s.GetBuffer (8),8);
		m_DestFile.Write(&n , sizeof(int) );
		m_DestFile.Write(pKline,sizeof(Kline)*n);
			
		NumberOfStock++;
	}

	if(pKline!=NULL)	
		delete [] pKline;
	m_ProgressYield.SetPos(0);

	return NumberOfStock;

}
