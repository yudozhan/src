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
	char    StockSign[8]; //ǰ8���ֽ�Ϊ��Ʊ���롣
	short 	numChuQ	;	  //2���ֽ�Ϊ��Ȩ���ݵ��ܴ���
	long    numKln	;	  //4���ֽ����ͣ�������K�ߵĸ�����
	short   symBlock[25]; //����ÿ�����ֽڴ���K�����ݵĿ��
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
	//��������, ���߻����������. 
	((CComboBox*)GetDlgItem(IDC_INTYPE))->SetCurSel(0);  
	((CComboBox*)GetDlgItem(IDC_OUTTYPE))->SetCurSel(0);  
	///////////////////////////
	//��װ���ݲ��ֵĳ�ʼ��
	m_progress1.SetPos(0);
	//��װ���ݸ�ʽ
	m_ComboDatatype.InsertString( 0, ""+g_strCompanyName+"���ݸ�ʽ");
	m_ComboDatatype.InsertString( 1, "Ǯ�����ݸ�ʽ");
	m_ComboDatatype.InsertString( 2, "���������ݸ�ʽ");
	m_ComboDatatype.InsertString( 3, "������ݸ�ʽ");
	m_ComboDatatype.InsertString( 4, "ʤ�����ݸ�ʽ");
	
	//��ʼΪ������ʽ
	m_ComboDatatype.SetCurSel(0);
	//��װ�����ļ�
	m_DataSource.ReplaceSel("d:\\dest.day");
	
	//��װ��ʽ
	m_Cover=0;
	GetDlgItem(IDC_INSTALL_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_INSTALL_END)->EnableWindow(FALSE);


	//��װ, ��������ʾ��
	m_install_start=CTime::GetCurrentTime();
	m_install_end  =CTime::GetCurrentTime();
	m_produce_start=CTime::GetCurrentTime();
	m_produce_end  =CTime::GetCurrentTime();


	
 	///////////////////////////
	// �������ݲ��ֵĳ�ʼ��
	m_AssignPeriod=1;    //ʱ�����ɻ���ָ������
	GetDlgItem(IDC_PRODUCE_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_PRODUCE_END)->EnableWindow(FALSE);
	m_destination.ReplaceSel("d:\\dest.day");

	//m_day=0;			 //���߻��Ƿ�ʱ��	
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

	CString DefExt="*.day, *.dad, *.psd, *.min";  //�ļ���չ��.
	CString FileName="D:\\dest.day";  //����������ļ����༭���е��ļ���
	CString Filter="line day data file(*.day)|*.day|(*.dad)|*.dad|(*.psd)|*.psd|(*.min)|*.min||";  //.�ļ���չ��������
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
	
	CString DefExt="*.day,*.min";  //�ļ���չ��.
	CString FileName="D:\\dest.day";  //����������ļ����༭���е��ļ���
	CString Filter="K line data file (*.day)|*.day|(*.min)|*.min||";  //.�ļ���չ��������
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


//�õ�������
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
// ��װ����, �������ǰ�װ�Ͳ��䰲װ
void CGSGLLSSJ::OnButtonBeginInstall() 
{
	//��׽���
	SetCapture();
	BeginWaitCursor();

	::SetCurrentDirectory( pDoc->m_CurrentWorkDirectory );

	//ֹͣ������
	pDoc->m_systemOption.autoday=FALSE;

	//��ʼʱ��,����ʱ��
	UpdateData(TRUE);
	int BeginDate=atoi(m_install_start.Format("%Y%m%d"));
	int EndDate  =atoi(m_install_end.Format("%Y%m%d"));

	if(BeginDate > EndDate)	//�����쳣
	{
		AfxMessageBox("ʱ��ߵ����������룡",MB_ICONSTOP);
		return;
	}

	//��װ���߻����������.
	if( ((CComboBox*)GetDlgItem(IDC_INTYPE))->GetCurSel()==0  )  //��װ����
	{
		//��ʼ��װ
		CString srcfilename;
		switch( m_ComboDatatype.GetCurSel( ) )
		{
		case 0:    //ourself
			m_DataSource.GetWindowText(srcfilename);
			if(  srcfilename.IsEmpty() )
			{
				AfxMessageBox("�ļ�����ӦΪ�գ�");
				return;
			}
			//���������������.
			if( srcfilename[0]=='a'||srcfilename[0]=='b'||srcfilename[0]=='A'||srcfilename[0]=='B')
			{
				if( !InstallFromFloppyDisk(srcfilename) )
				{
					AfxMessageBox("��װû���������У�",MB_ICONASTERISK);
					return;
				}
				srcfilename="c:\\source.day";
			}
			else //��Ӳ���ϵķֽ��ļ����а�װ.
			{
				srcfilename.MakeLower();
				if( srcfilename.Find("tl_disk") != -1 )
				{
					if( ! InstallFromSeveralDiskfile(srcfilename) )
					{
						AfxMessageBox("��װû���������У�",MB_ICONASTERISK);
						return;
					}
					srcfilename="c:\\source.day";
				}
			}

			InstallWanshen(srcfilename, BeginDate, EndDate);
			m_progress1.SetPos(0);
			//AfxMessageBox("��װ�ѽ�����");
			break;
		case 1:    //Ǯ��
			m_DataSource.GetWindowText(srcfilename);
			InstallQianlong(srcfilename, BeginDate, EndDate);
			m_progress1.SetPos(0);
			//AfxMessageBox("��װ�ѽ�����");
			break;
		case 2:    //������
			m_DataSource.GetWindowText(srcfilename);
			InstallFenxijia(srcfilename, BeginDate, EndDate);
			m_progress1.SetPos(0);
			//AfxMessageBox("��װ�ѽ�����");
			break;
		case 3:    //���
			m_DataSource.GetWindowText(srcfilename);
			InstallHuijin(srcfilename, BeginDate, EndDate);
			m_progress1.SetPos(0);
			//AfxMessageBox("��װ�ѽ�����");
			break;
		case 4:    //ʤ��
			m_DataSource.GetWindowText(srcfilename);
			InstallShenglong(srcfilename, BeginDate, EndDate);
			m_progress1.SetPos(0);
			//AfxMessageBox("��װ�ѽ�����");
			break;

		default:
			AfxMessageBox("û��ѡ��װ���ݵ����ͣ�", MB_ICONSTOP);
			break;
		}
	}
	else   //��װ������� ----By Huang Yutu.
	{
		InstallMinteData();
	}


	//SEND MESSAGE
	CMainFrame* pFm=(CMainFrame*)AfxGetMainWnd();
//	if(pFm->pVwbaseView != NULL)
//		::SendMessage(pFm->pVwbaseView->m_hWnd,WM_USER_TELL_CHANGE,0,200) ;

	//�ָ�������
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

	//��ʼʱ��,����ʱ��
	int BeginDate=atoi(m_produce_start.Format("%Y%m%d"));
	int EndDate  =atoi(m_produce_end.Format("%Y%m%d"));

	if(BeginDate > EndDate 	|| BeginDate>atoi( (CTime::GetCurrentTime()).Format("%Y%m%d") )	)	//�����쳣
	{
		AfxMessageBox("û�����ݣ�",MB_ICONASTERISK);
		return;
	}


	//���ý��ȷ�Χ
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
// ��������, ���������ݵ������ļ�
void CGSGLLSSJ::OnButtonBeginproduce() 
{
	UpdateData(TRUE);
	if( ((CButton*)GetDlgItem(IDC_RADIO_RECENTDAYS))->GetCheck() && (m_NumberOfDays>6400 || m_NumberOfDays<1) )
		return;
		
	//��ʼʱ��,����ʱ��
	int BeginDate=atoi(m_produce_start.Format("%Y%m%d"));
	int EndDate  =atoi(m_produce_end.Format("%Y%m%d"));

	if(BeginDate > EndDate 	|| BeginDate>atoi( (CTime::GetCurrentTime()).Format("%Y%m%d") )	)	//�����쳣
	{
		AfxMessageBox("û�����ݣ�",MB_ICONASTERISK);
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
		AfxMessageBox("�ļ�����ӦΪ�գ�");
		return;
	}


	if( IsFileExists( DestFileName ) )
		if( AfxMessageBox("����ͬ���ļ����Ƿ񸲸ǣ�",MB_ICONQUESTION|MB_YESNO )==IDNO )
			return;
	UpdateWindow();

	if( ((CComboBox*)GetDlgItem(IDC_OUTTYPE))->GetCurSel()==0  )  //�������� 
	{
		
		//���������������.
		if( DestFileName[0]=='a'||DestFileName[0]=='b'||DestFileName[0]=='A'||DestFileName[0]=='B')
		{
			FloppyFileName=DestFileName;
			DestFileName="C:\\Source.day";
		}

		m_DestFile.Open(DestFileName, CFile::modeWrite|CFile::modeCreate);
		m_DestFile.Seek(16,CFile::begin);   //Ԥ��16���ֽڴ���ļ���ʶ�͵����Ĺ�Ʊ��
		int NumberOfStock=0;

		//Only Shanghai.
		if( m_sh == 1 && m_sz == 0) 
		{
			if(m_AssignPeriod==0)  //ָ��ʱ��
			{
				YieldDay("data/shanghai/day.dat", NumberOfStock);
				int FileID=FILEID22;		   //�ļ���־
				m_DestFile.SeekToBegin();
				m_DestFile.Write(&FileID,       4  );
				m_DestFile.Write(&NumberOfStock,4  );
				m_DestFile.Close();
			}
			else
			{
				AssignDays("data/shanghai/day.dat", NumberOfStock);  //ָ����������
				int FileID=FILEID22;		   //�ļ���־
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
					if( AfxMessageBox("�ֽ����ɵ������ļ���",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDYES  )
					{
						UpdateWindow();
						SplitOneFileToManyFiles(Path,DestFileName);
					}
				}
				else
					AfxMessageBox("����������ϣ�",MB_ICONASTERISK );
			}

		}

		//Only Shenzhen.
		if( m_sh == 0 && m_sz == 1)
		{
			if(m_AssignPeriod==0)  //ָ��ʱ��
			{
				YieldDay("data/shenzhen/day.dat", NumberOfStock);
				int FileID=FILEID22;		   //�ļ���־
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

				AssignDays("data/shenzhen/day.dat", NumberOfStock);  //ָ����������
				int FileID=FILEID22;		   //�ļ���־
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
					if( AfxMessageBox("�ֽ����ɵ������ļ���",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDYES  )
					{
						UpdateWindow();
						SplitOneFileToManyFiles(Path,DestFileName);
					}
				}
				else
					AfxMessageBox("����������ϣ�",MB_ICONASTERISK );
			}

		}

		//Both Shenzhen and Shanghai.
		if( m_sh == 1 && m_sz == 1)
		{
			if(m_AssignPeriod==0)  //ָ��ʱ��
			{
				YieldDay("data/shenzhen/day.dat", NumberOfStock);
				YieldDay("data/shanghai/day.dat", NumberOfStock);

				int FileID=FILEID22;		   //�ļ���־
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

				AssignDays("data/shanghai/day.dat", NumberOfStock);  //ָ����������
				AssignDays("data/shenzhen/day.dat", NumberOfStock);  //ָ����������
				int FileID=FILEID22;		   //�ļ���־
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
					if( AfxMessageBox("�ֽ����ɵ������ļ���",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDYES  )
					{
						UpdateWindow();
						SplitOneFileToManyFiles(Path,DestFileName);
					}
				}
				else
					AfxMessageBox("����������ϣ�",MB_ICONASTERISK );
			}
		
		}

		//Neither Shenzhen nor Shanghai.
		if( m_sh == 0 && m_sz == 0)
		{
			AfxMessageBox("��ѡ�����ں�(��)�Ϻ�����ʷ���ݡ�",MB_ICONASTERISK);			
			m_DestFile.Close();
		}

	}
	else     //�����������.--By Huang Yutu.
	{
		ProduceMinuteData() ;
	}

	ReleaseCapture();
	EndWaitCursor();

}


//////////////////////////////////////////////////////////////
///////////////////////   ��װ����  //////////////////////////
//�򿪻��ĵ��������ļ�
void CGSGLLSSJ::InstallHuijin(CString srcfilename, int BeginDate, int EndDate) 
{
	if(  BeginDate<19700101 ||BeginDate>20380101||
		 EndDate  <19700101 ||EndDate>20380101 )
	{
		AfxMessageBox("����������ȷ��ʱ�䣡");
		return;
	}
	if( BeginDate>EndDate)
	{
		AfxMessageBox("����ʱ��Ƿ���",MB_ICONASTERISK);
		return;
	}
	
	{// //////////////////////////////////////////////////////

		//�����Ŀ¼��, �����������ļ��ж���
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
			AfxMessageBox("û��Ҫ��װ�����ݡ�",MB_ICONASTERISK);
			return;
		}

		m_progress1.SetRange32(0,NumFile);

		//Fenxijia data file.
		Kline Tianlong;
		CString StockName;  //��Ʊ����	  
		CString szTime;
		CTime tm;

		//������Ŀ¼�µ������ļ�,���а�װ
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

			//Huijin�洢�����ļ�, �����ǵ����������ļ�.
			CString HuijinFileName=finder.GetFilePath();
			CFile HuijinFile(HuijinFileName,CFile::modeRead);
			int FileLength=HuijinFile.GetLength();
			
			StockName=finder.GetFileTitle();
			if( StockName.GetLength()!=8 || StockName.GetLength()!=6) 	continue;  //if not shanghai , then return.
			StockName=StockName.Right(StockName.GetLength()-2);

			m_InstallArray.RemoveAll();    //�������
			
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
//��װǮ����ʽ�ĵ����ļ�
void CGSGLLSSJ::InstallQianlong(CString srcfilename, int BeginDate, int EndDate) 
{
	if(  BeginDate<19700101 ||BeginDate>20380101 ||
		 EndDate  <19700101 ||EndDate  >20380101 || BeginDate > EndDate )
	{
		AfxMessageBox("����������ȷ��ʱ�䣡",MB_ICONASTERISK);
		return;
	}

	//�������� Shanghai//////////////////////////////////////////
	{
		//�����Ŀ¼��, �����������ļ��ж���
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
			AfxMessageBox("û��Ҫ��װ�����ݡ�",MB_ICONASTERISK);
			return;
		}

		m_progress1.SetRange32(0,NumFile);
		m_progress1.SetPos(0);

		Kline Wanshen;
		CString StockName;  //��Ʊ����	  
		CString szTime;
		CTime tm;

		//������Ŀ¼�µ������ļ�,���а�װ
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

			//Ǯ���洢�����ļ�, �����ǵ����������ļ�, ��Ϊ�����������ļ���ѹ����.
			CString QianlongFileName=finder.GetFilePath();
			CFile QianlongFile((LPCTSTR)QianlongFileName,CFile::modeRead);
			int FileLength=QianlongFile.GetLength();
			
			StockName=finder.GetFileTitle();;
			if( StockName.GetLength() !=6&&StockName.GetLength() !=4 )		continue;
			
			m_InstallArray.RemoveAll();    //�������
		
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
			
			//���а�װShanghai.
			StockName.MakeUpper();
			InstallStockKlineEach( StockName, BeginDate, EndDate);
		}

	}

}



//////////////////////////////////////////////////////////////////////
//��װ������ʽ�ĵ����ļ�
void CGSGLLSSJ::InstallWanshen(CString srcfilename, int BeginDate, int EndDate) 
{
	{//begin*************  ****************
	//��������
	//Դ�����ļ������ƺ�·��. 
	CString SourceFilename=srcfilename;
	UpdateData(TRUE);	
	//m_DataSource.GetWindowText(SourceFilename);

	//���������������.
	if( SourceFilename[0]=='a'||SourceFilename[0]=='b'||SourceFilename[0]=='A'||SourceFilename[0]=='B')
		SourceFilename="c:\\source.day";
	
	CFile SourceFile(SourceFilename,CFile::modeRead);
	
	//����װ��֤ȯ����
	int NumStock=0 ;             
	int FileID;  //�ļ���ʶ
	SourceFile.Read(&FileID,4);
	if( FileID!=FILEID22)
	{
		AfxMessageBox("��"+g_strCompanyName+"���ݸ�ʽ��",MB_ICONSTOP);
		return;
	}
	SourceFile.Read(&NumStock,sizeof(int));
	SourceFile.Seek(16,CFile::begin);
	
	//���ý������ķ�Χ
	m_progress1.SetRange(0,NumStock);
	
	//�˹�Ʊ��K����Ŀ
	int NumKline=0;
	char StockName[8];	
	Kline wanshen;
	CTime tm;       //��װʱ��

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

		//����װ������
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
	//�ر�����Դ�ļ��ͱ����ļ�
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
	case 1:    //Ǯ��
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCHFIXDATA))->EnableWindow(FALSE);
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("C:\\ML30\\DATA\\SHASE\\day\\*.day");
		break;
	case 2:    //������
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCHFIXDATA))->EnableWindow(TRUE);
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("d:\\superstk.dad");
		break;
	case 3:    //���
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCHFIXDATA))->EnableWindow(FALSE);
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("C:\\HUIJIN\\EXE\\LINEDAY\\SH\\*.psd");
		break;
	case 4:    //ʤ��
		((CButton*)GetDlgItem(IDC_BUTTON_SEARCHFIXDATA))->EnableWindow(FALSE);
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("D:\\SLON\\DATA\\SH\\DAY\\*.DAY");
		break;

	}
	
}
//����: ѡ��ʱ�ΰ�װ, ����ʱ��༭��.
void CGSGLLSSJ::OnRadioInstalldate() 
{
	GetDlgItem(IDC_INSTALL_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_INSTALL_END)->EnableWindow(TRUE);
}
//����: ����ָ����������������.
//      ָ��������Ϊ 1=< m_AssignDays < 6400 Ϊһ�޷�������.
void CGSGLLSSJ::AssignDays(CString filename, int &NumberOfStock)  //ָ����������
{
	//�ж������ڻ����Ϻ�?
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
	//���ý��ȷ�Χ
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


//�������ݵ�����. ����ʾ���β����Ƭ����.
void CGSGLLSSJ::Produce2FloppyDisk(CString DestFileName,CString FloppyFileName)
{
	CFile SrcFile;   //��Ӳ���ϵ������ļ�.
	
	//�쳣����.
	if( !SrcFile.Open(DestFileName,CFile::modeRead) )
	{
		AfxMessageBox("Open file error",MB_ICONSTOP);
		return;
	}
	int LengthOfSrcFile=SrcFile.GetLength();  //�ļ�����.
	
	//��Լ��Ҫ���Ŵ���.
	int BytesPerDisk=1457000;  
	int NumOfDisks= LengthOfSrcFile%BytesPerDisk==0 ? LengthOfSrcFile/BytesPerDisk:LengthOfSrcFile/BytesPerDisk+1;  
	CString str;
	str.Format("%d",NumOfDisks);
	str="Ҫ������������, ��Լ��Ҫ " + str + " ������(3.5��)��";
	if( AfxMessageBox(str,MB_OKCANCEL)==IDCANCEL )
		return;

	DWORD SectorsPerCluster,BytesPerSector,NumberOfFreeClusters,TotalNumberOfClusters;
	for( int nDisk=1;nDisk<=NumOfDisks+1;nDisk++)
	{
		//�ж��Ƿ��������.
		int nSize=min(BytesPerDisk,LengthOfSrcFile);
		if( nSize<= 0)
		{
			AfxMessageBox("������ϣ�",MB_ICONASTERISK);
			return;
		}

		//�Ƿ����������ȥ.
		str.Format("%d",nDisk);
		str="������ "+str+" �����̡�";
		if( AfxMessageBox(str,MB_OKCANCEL)==IDCANCEL )
			return;

		//ȷ�ϴ����Ƿ�Ϊ��.
		
		::GetDiskFreeSpace(FloppyFileName.Mid(0,2),&SectorsPerCluster,&BytesPerSector,&NumberOfFreeClusters,&TotalNumberOfClusters);
		int FreeSpacePerDisk=NumberOfFreeClusters*SectorsPerCluster*BytesPerSector;
		if( FreeSpacePerDisk<BytesPerDisk+8 )
		{
			AfxMessageBox("��ɾ�������ϵ���������,���߸�ʽ�����̡�",MB_ICONASTERISK);
			return;
		}

		CFile FloppyFile( FloppyFileName,CFile::modeCreate|CFile::modeWrite);
		FloppyFile.Write(&NumOfDisks,sizeof(4));  //��������,ռ�ĸ��ֽ�.
		FloppyFile.Write(&nDisk,sizeof(4));       //���̱��,ռ�ĸ��ֽ�.
		BYTE *Buffer=new BYTE[nSize];   
		SrcFile.Read(Buffer,nSize);   //��Ӳ���ļ�.
		FloppyFile.Write(Buffer,nSize); //д�����ļ�.
		delete []Buffer;              //�ͷ��ڴ�.
		FloppyFile.Close();
		
		LengthOfSrcFile-=nSize;       //ÿ�μ�ȥ nSize ���ֽ�.
	}

	SrcFile.Remove(DestFileName);  //

}

BOOL CGSGLLSSJ::InstallFromFloppyDisk(CString FloppyFileName,bool bDayKline)
{
	int NumOfDisks=0, //��������.
		nDisk=0;      //���̱��.
	CFile SrcFile;    //Դ�����ļ�.
	CFile DestFile;   //Ӳ���ϵ�����.
	CString sExd = ".day";
	if(bDayKline==false)
		sExd = ".min";
	if( !DestFile.Open("c:\\source"+sExd,CFile::modeWrite|CFile::modeCreate) )
	{
		AfxMessageBox("Open file error!",MB_ICONSTOP);
		return 0;
	}

	int IndexOfDisk=1;  //��������.
	int nRepeat=0;      //�ظ�����.
	//do-whileѭ�����а�װ.
	do
	{
		CString str;
		str.Format("%d",IndexOfDisk);
		AfxMessageBox("����� "+str+" �Ŵ��̡�");
		
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
		SrcFile.Read(&NumOfDisks,4);  //����������.
		SrcFile.Read(&nDisk,4);       //�����̱��.
		int LengthOfFile=SrcFile.GetLength();  //�����ļ�����.
		
		//�������ѭ�����.
		if( nDisk != IndexOfDisk )
		{
			nRepeat++;
			AfxMessageBox("���²����"+str+"�Ŵ��̡�");
			SrcFile.Close();
			if( nRepeat>5) return 0;  //����ظ�����5����Ȼ����,���˳�.
			continue;
		}

		LengthOfFile-=8;
		BYTE *Buffer=new BYTE[LengthOfFile];
		SrcFile.Read(Buffer,LengthOfFile);  //�������϶�����.
		SrcFile.Close();
		DestFile.Write(Buffer,LengthOfFile);//��Ӳ����д����.
		delete []Buffer;
		
		IndexOfDisk++;
		nRepeat=0;

	}while(NumOfDisks>nDisk); //��������ѭ������.

	DestFile.Close();

	return 1;
}


//��һ���ļ��ֽ�ɼ����ļ�,���ڿ���.(Ddisk1, Disk2, ....)
void CGSGLLSSJ::SplitOneFileToManyFiles(CString Dir,CString DestFileName,bool bDayKline)
{
	CFile SrcFile;   //��Ӳ���ϵ������ļ�.
	
	//�쳣����.
	CString sExd = ".day";
	if(bDayKline==false)
		sExd = ".min";
	if( !SrcFile.Open(DestFileName,CFile::modeRead) )
	{
		AfxMessageBox("Open file error",MB_ICONSTOP);
		return;
	}
	int LengthOfSrcFile=SrcFile.GetLength();  //�ļ�����.
	
	//��Լ��Ҫ���Ŵ���.
	int BytesPerDisk=1457000;  
	int NumOfDisks= LengthOfSrcFile%BytesPerDisk==0 ? LengthOfSrcFile/BytesPerDisk:LengthOfSrcFile/BytesPerDisk+1;  

	for( int nDisk=1;nDisk<=NumOfDisks+1;nDisk++)
	{
		//�ж��Ƿ��������.
		int nSize=min(BytesPerDisk,LengthOfSrcFile);
		if( nSize<= 0)
		{
			AfxMessageBox("������ϣ�",MB_ICONASTERISK);
			return;
		}

		CString DiskXFileName;
		DiskXFileName.Format("%d",nDisk);
		DiskXFileName=Dir+"\\TL_Disk"+DiskXFileName+sExd;
		CFile DiskXFile( DiskXFileName,CFile::modeCreate|CFile::modeWrite);
		DiskXFile.Write(&NumOfDisks,sizeof(4));  //��������,ռ�ĸ��ֽ�.
		DiskXFile.Write(&nDisk,sizeof(4));       //���̱��,ռ�ĸ��ֽ�.
		BYTE *Buffer=new BYTE[nSize];   
		SrcFile.Read(Buffer,nSize);   //��Ӳ���ļ�.
		DiskXFile.Write(Buffer,nSize); //д�����ļ�.
		delete []Buffer;              //�ͷ��ڴ�.
		DiskXFile.Close();
		
		LengthOfSrcFile-=nSize;       //ÿ�μ�ȥ nSize ���ֽ�.
	}

	SrcFile.Remove(DestFileName);  //

}



/////////////////////////////////////////////////////////////////////
// ��װ�����ҵ����ݸ�ʽ
void CGSGLLSSJ::InstallFenxijia(CString srcfilename ,int BeginDate, int EndDate) 
{
	//�ӷ����ҵ������ļ��ж�����
	CFile SourceFile(srcfilename,CFile::modeRead);
	int NumStock=0;       //����װ��֤ȯ����
	SourceFile.Seek(8,CFile::begin);
	SourceFile.Read(&NumStock,sizeof(int));

	int NumKlinesOfFxj =  (SourceFile.GetLength()-16)/32;

	if( NumKlinesOfFxj<0 )
		return;

	Kline *tianlong=new Kline[NumKlinesOfFxj]; //������Ҫ�ͷ�.
	SourceFile.Seek(16,CFile::begin);
	SourceFile.Read(tianlong,32*NumKlinesOfFxj);
	SourceFile.Close();   //�رշ����ҵ������ļ�.

	m_progress1.SetRange32(0,NumStock);    //���ý������ķ�Χ

	CTime tm;   //��װʱ��
	tm=CTime::GetCurrentTime();
	int CurrentDate=atoi(tm.Format("%Y%m%d"));

	int nFixStyle=GetCheckedRadioButton(IDC_RADIO_COVER,IDC_RADIO_INSTALLDATE); //ѡ��װ��ʽ.

	{//
	CString StockSymbol;
	for( int i=0;i<NumKlinesOfFxj;i++)
	{
		if( tianlong[i].day == -1 )
		{
			m_progress1.OffsetPos(1);  //show the progress

			//����Ϣ���ȴ�����Ϣ.
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
	delete []tianlong;   //�ͷ��ڴ�.
}




//
void CGSGLLSSJ::OnSelchangeIntype() 
{
	if( ((CComboBox*)GetDlgItem(IDC_INTYPE))->GetCurSel()==0  )		//��װ���� 
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
	if( ((CComboBox*)GetDlgItem(IDC_OUTTYPE))->GetCurSel()==0  )		//��װ����
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
	int NumOfDisks=0, //��������.
		nDisk=0;      //���̱��.
	CFile SrcFile;    //Դ�����ļ�.
	CFile DestFile;   //Ӳ���ϵ�����.
	CString sExd = ".day";
	if(bDayKline==false)
		sExd = ".min";
	if( !DestFile.Open("c:\\source"+sExd,CFile::modeWrite|CFile::modeCreate) )
	{
		AfxMessageBox("Open file error!",MB_ICONSTOP);
		return 0;
	}

	int IndexOfDisk=1;  //��������.
	int nRepeat=0;      //�ظ�����.
	//do-whileѭ�����а�װ.
	do
	{
		CString str;
		str.Format("%d",IndexOfDisk);
		//AfxMessageBox("����� "+str+" �Ŵ���.");
		
		FileName.Delete( FileName.GetLength()-5 );
		FileName.Insert( FileName.GetLength()-4, str );
		if( !SrcFile.Open(FileName,CFile::modeRead) )
		{
			AfxMessageBox("Open file error!",MB_ICONSTOP);
			return 0;
		}
		SrcFile.Read(&NumOfDisks,4);  //����������.
		SrcFile.Read(&nDisk,4);       //�����̱��.
		int LengthOfFile=SrcFile.GetLength();  //�����ļ�����.
		
		//�������ѭ�����.
		if( nDisk != IndexOfDisk )
		{
			SrcFile.Close();
			return 0;
		}

		LengthOfFile-=8;
		BYTE *Buffer=new BYTE[LengthOfFile];
		SrcFile.Read(Buffer,LengthOfFile);  //�������϶�����.
		SrcFile.Close();
		DestFile.Write(Buffer,LengthOfFile);//��Ӳ����д����.
		delete []Buffer;
		
		IndexOfDisk++;
		nRepeat=0;

	}while(NumOfDisks>nDisk); //��������ѭ������.

	DestFile.Close();

	return 1;

}
//��װʤ��������
void CGSGLLSSJ::InstallShenglong(CString srcfilename, int BeginDate, int EndDate)
{
	//ʱ�����
	if(  BeginDate<19700101 ||BeginDate>20380101||
		 EndDate  <19700101 ||EndDate  >20380101|| 
		 BeginDate>EndDate   )
	{
		AfxMessageBox("�Ƿ����룡����������ȷ��ʱ�䣡");
		return;
	}

	{ //shanghai shanghai shanghai shanghai shanghai 
		//�����Ŀ¼��, �����������ļ��ж���
		int NumFile=0;
		CString FinderName;
		CFileFind finder;
		//�ӱ༭���ж���Ҫ���ҵ��ļ�����, ���� D:\SLON\DATA\SH\DAY\*.day
		m_DataSource.GetWindowText(FinderName);
		BOOL bwork = finder.FindFile(FinderName);
		while( bwork )
		{
			bwork=finder.FindNextFile();
			NumFile++;
		}

		if( !NumFile )
		{
			AfxMessageBox("û��Ҫ��װ�����ݡ�",MB_ICONASTERISK);
			return;
		}


		//���ý������Ĺ�����Χ.
		m_progress1.SetRange32(0,NumFile);
		m_progress1.SetPos(0);

		CString StockName, /*��Ʊ����*/ 	szTime;
		CTime tm;

		//������Ŀ¼�µ������ļ�,���а�װ
		BOOL bWorking = finder.FindFile(srcfilename);
		while( bWorking )
		{
			//���ȴ���Windows��Ϣ.
			MSG message;
			if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}  
			
			//ÿ��װһֻ��Ʊ,����Ҫ������е�Ԫ��.
			m_InstallArray.RemoveAll();
			m_progress1.OffsetPos(1);

			bWorking=finder.FindNextFile();

			//ʤ���洢�����ļ�, �����ǵ����������ļ�
			CString ShenglongFileName=finder.GetFilePath();
			CFile ShenglongFile(ShenglongFileName,CFile::modeRead);
			int FileLength=ShenglongFile.GetLength();
			
			StockName=finder.GetFileTitle();
			if( StockName.GetLength() != 6||StockName.GetLength() != 4 ) continue;
									
			int NumShenglong=FileLength/sizeof(SHENGLONG);                 //�ļ��й��е�������Ŀ.
			
			if( NumShenglong <= 0 )
			{
				ShenglongFile.Close();
				continue;
			}

			SHENGLONG* SLarray=new SHENGLONG[NumShenglong];
			//int *DateList=new int[NumShenglong];            //����һ��ʱ������
			ShenglongFile.Read( SLarray, sizeof(SHENGLONG)*NumShenglong );
			ShenglongFile.Close();

			Kline wsh;
			for( int i=0; i < NumShenglong; i++)
			{
				if( ShenglongToTianlong(SLarray[i],wsh) )
					m_InstallArray.Add(wsh);
			}
			delete []SLarray;

			if( m_InstallArray.GetSize()==0 )	//����������������߸���Ϊ��,�򷵻�.
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
	//��������
	//Դ�����ļ������ƺ�·��. 
	CString SourceFilename,srcfilename;
	m_DataSource.GetWindowText(SourceFilename);
	srcfilename = SourceFilename;
	//���������������.
	if( srcfilename[0]=='a'||srcfilename[0]=='b'||srcfilename[0]=='A'||srcfilename[0]=='B')
	{
		if( !InstallFromFloppyDisk(srcfilename,false) )
		{
			AfxMessageBox("��װû���������У�",MB_ICONASTERISK);
			return;
		}
		srcfilename="c:\\source.min";
	}
	else //��Ӳ���ϵķֽ��ļ����а�װ.
	{
		srcfilename.MakeLower();
		if( srcfilename.Find("tl_disk") != -1 )
		{
			if( ! InstallFromSeveralDiskfile(srcfilename,false) )
			{
				AfxMessageBox("��װû���������У�",MB_ICONASTERISK);
				return;
			}
			srcfilename="c:\\source.min";
		}
	}

	UpdateData(TRUE);	

	//���������������.
//	if( SourceFilename[0]=='a'||SourceFilename[0]=='b'||SourceFilename[0]=='A'||SourceFilename[0]=='B')
//		SourceFilename="c:\\source.min";
	
	CFile SourceFile(SourceFilename,CFile::modeRead);
	
	//����װ��֤ȯ����
	int NumStock=0 ;             
	int FileID;  //�ļ���ʶ
	SourceFile.Read(&FileID,4);
	if( FileID!=FILEID22)
	{
		AfxMessageBox("��"+g_strCompanyName+"���ݸ�ʽ��",MB_ICONSTOP);
		return;
	}
	SourceFile.Read(&NumStock,sizeof(int));
	SourceFile.Seek(16,CFile::begin);
	
	//���ý������ķ�Χ
	m_progress1.SetRange(0,NumStock);
	
	//�˹�Ʊ��K����Ŀ
	int NumKline=0;
	char StockName[8];	
	Kline wanshen;
	CTime tm;       //��װʱ��

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

		//����װ������
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
	//�ر�����Դ�ļ��ͱ����ļ�
	SourceFile.Close();
}
void CGSGLLSSJ::ProduceMinuteData() 
{
	CString DestFileName,FloppyFileName;
	m_destination.GetWindowText(DestFileName);
	if(  DestFileName.IsEmpty() )
	{
		AfxMessageBox("�ļ�����ӦΪ�գ�");
		return;
	}

	if( IsFileExists( DestFileName ) )
		if( AfxMessageBox("����ͬ���ļ����Ƿ񸲸ǣ�",MB_ICONQUESTION|MB_YESNO )==IDNO )
			return;

	//���������������.
	if( DestFileName[0]=='a'||DestFileName[0]=='b'||DestFileName[0]=='A'||DestFileName[0]=='B')
	{
		FloppyFileName=DestFileName;
		DestFileName="C:\\Source.min";
	}

	m_DestFile.Open(DestFileName, CFile::modeWrite|CFile::modeCreate);
	m_DestFile.Seek(16,CFile::begin);   //Ԥ��16���ֽڴ���ļ���ʶ�͵����Ĺ�Ʊ��
	int NumberOfStock=0;
	int FileID=FILEID22;		   //�ļ���־
	{
		if(m_AssignPeriod==0)  //ָ��ʱ��
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
				if( AfxMessageBox("�ֽ����ɵ������ļ���",MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2)==IDYES  )
				{
					UpdateWindow();
					SplitOneFileToManyFiles(Path,DestFileName,false);
				}
			}
			else
				AfxMessageBox("����������ϣ�",MB_ICONASTERISK );
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
		//ѡ��װ��ʽ.
		int nFixStyle=GetCheckedRadioButton(IDC_RADIO_COVER,IDC_RADIO_INSTALLDATE);
		switch(nFixStyle)
		{
		case IDC_RADIO_REPAIRE:  //���䰲װ
			pFile ->WriteKLine(symbol,m_InstallArray.GetData(),m_InstallArray.GetSize(),CTaiKlineFileKLine::insert);
			break;
		case IDC_RADIO_INSTALLDATE:  //ָ��ʱ�ΰ�װ
			//ɾ��������ʱ�����Ƶ�Ԫ��
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

		case IDC_RADIO_COVER :   //���ǰ�װ
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

	//��ʼʱ��,����ʱ��
	int BeginDate=atoi(m_produce_start.Format("%Y%m%d"));
	int EndDate  =atoi(m_produce_end.Format("%Y%m%d"));

	if(BeginDate > EndDate 	|| BeginDate>atoi( (CTime::GetCurrentTime()).Format("%Y%m%d") )	)	//�����쳣
	{
		AfxMessageBox("û�����ݣ�",MB_ICONASTERISK);
		return 0;
	}


	//���ý��ȷ�Χ
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
