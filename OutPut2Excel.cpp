// OutPut2Excel.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "OutPut2Excel.h"
#include "CTaiShanDoc.h"
#include "gsgllssj.h"
#include "mainfrm.h"
#include "CTaiKlineMemFile.h"
#include "CTaiKlineFileKLine.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutPut2Excel property page

IMPLEMENT_DYNCREATE(COutPut2Excel, CPropertyPage)

COutPut2Excel::COutPut2Excel() : CPropertyPage(COutPut2Excel::IDD)
{
	//{{AFX_DATA_INIT(COutPut2Excel)
	m_StockMarket = _T("�Ϻ�֤ȯ�г�");
	m_BeginDate = CTime::GetCurrentTime();
	m_EndDate = CTime::GetCurrentTime();
	m_OutFile = _T("c:\\dest.xls");
	//}}AFX_DATA_INIT
}

COutPut2Excel::~COutPut2Excel()
{
}

void COutPut2Excel::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutPut2Excel)
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_CBString(pDX, IDC_COMBO_MARKET, m_StockMarket);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_BeginDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_EndDate);
	DDX_Text(pDX, IDC_EDIT1, m_OutFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutPut2Excel, CPropertyPage)
	//{{AFX_MSG_MAP(COutPut2Excel)
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	ON_EN_UPDATE(IDC_EDIT1, OnUpdateEdit1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, OnDatetimechangeDatetimepicker1)
	ON_NOTIFY(DTN_DROPDOWN, IDC_DATETIMEPICKER1, OnDropdownDatetimepicker1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, OnDatetimechangeDatetimepicker2)
	ON_NOTIFY(DTN_DROPDOWN, IDC_DATETIMEPICKER2, OnDropdownDatetimepicker2)
	ON_CBN_SELCHANGE(IDC_COMBO_MARKET, OnSelchangeComboMarket)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutPut2Excel message handlers

void COutPut2Excel::OnSearch() 
{
	CString DefExt="*.xls";  //�ļ���չ��.
	CString FileName="c:\\dest.xls";  //����������ļ����༭���е��ļ���
	CString Filter="Excell file (*.xls)|*.XLS||";  //.�ļ���չ��������
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
		m_OutFile=bSaveFileDialog.GetPathName();
	}
	UpdateData(FALSE);

	CMDIChildWnd * pChildFrame=((CMainFrame*)(AfxGetApp()->m_pMainWnd))->MDIGetActive();
	CTaiShanDoc * pDoc=(CTaiShanDoc *)pChildFrame->GetActiveDocument();
	ASSERT_VALID(pDoc);
	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);
	
	SetModified();
}




void COutPut2Excel::Put2Excel(CString filename, int length)
{



	//�õ��ĵ�ָ��
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	CTaiShanDoc* pDoc=(CTaiShanDoc*)pFrameWnd->GetActiveDocument();

	CString sharesymbol;
	CTime tm;

	UpdateData(TRUE);
	int BeginDate=atoi(m_BeginDate.Format("%Y%m%d"));
	int EndDate  =atoi(m_EndDate.Format("%Y%m%d"));
	if( BeginDate > EndDate )
		return;

	//�������ݿ�
	CDatabase database;
	CreateExcel(database);
	//shenzhen or shanghai
	Kline* pKline = NULL;
	CTaiKlineFileKLine* pFile = NULL;
	int STOCKSYMBOLLENGTH=0, MaxProgress=0,NumberOfStock=0;
	if( filename == "data\\shanghai\\day.dat" )
	{
		STOCKSYMBOLLENGTH=6;
		pFile = CTaiKlineFileKLine::m_fileDaySh ;
		MaxProgress=pFile->GetStockNumber();
	}
	else if( filename == "data\\shenzhen\\day.dat" )
	{
		STOCKSYMBOLLENGTH=4;
		pFile = CTaiKlineFileKLine::m_fileDaySz ;
		MaxProgress=pFile->GetStockNumber();
	}

	//���ý��ȷ�Χ
	m_Progress.SetRange(0,MaxProgress);

	//Open Source file.
	//read now.
	CTime tmb = m_BeginDate;
	CTime tme = m_EndDate;
	for(int k=0;k<MaxProgress;k++)
	{
		//show progress
		m_Progress.OffsetPos(1);

		int n = pFile->ReadKlinePeriod(k, pKline, tmb, tme);
		if(n==0)
			continue;
		CString s = pFile->GetSymbol(k);
		for(int j=0;j<n;j++)
			InsertExcel( database, s,  (TAI_SHAN_DATA&)pKline[j]);
		NumberOfStock++;
	}

	if(pKline!=NULL)	
		delete [] pKline;
	m_Progress.SetPos(0);


	//���蹤��Ŀ¼.
	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);

}


void COutPut2Excel::InsertExcel(CDatabase& database,CString StockSymbol, TAI_SHAN_DATA& tianlong)
{
	CString sSql;
	// Insert data
	CString str0,str1, str2, str3, str4,str5, str6, str7, str8;
	str0="INSERT INTO demo (��Ʊ����,����,���̼�,��߼�,��ͼ�,���̼�,�ɽ���,�ɽ����) VALUES (";
	CTime tm;
	tm=tianlong.nTime;
				
	str1="'" + StockSymbol + "'";             //symbol
	str2="'" + tm.Format("%Y/%m/%d") + "'";   //date
	str3.Format("%.3f",tianlong.fOpen);
	str4.Format("%.3f",tianlong.fHigh);
	str5.Format("%.3f",tianlong.fLow);
	str6.Format("%.3f",tianlong.fClose);
	str7.Format("%.3f",tianlong.fVolume);
	str8.Format("%.3f",tianlong.fAmount);
			
	sSql = str0 
		+ str1 + "," 
		+ str2 + "," 
		+ str3 + ","
		+ str4 + "," 
		+ str5 + ","
		+ str6 + ","
		+ str7 + ","
		+ str8 +        ")";
	database.ExecuteSQL(sSql);

}

BOOL COutPut2Excel::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	((CComboBox*)GetDlgItem(IDC_COMBO_MARKET))->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
	
}



void COutPut2Excel::CreateExcel(CDatabase& database)
{  
	// exactly the same name as in the ODBC-Manager
	CString sDriver = "MICROSOFT EXCEL DRIVER (*.XLS)"; 
	
	//example for CFile::Remove
	char* pFileName = m_OutFile.GetBuffer(0);
	TRY
	{
		CFile::Remove( pFileName );
	}
	CATCH( CFileException, e )
	{
		#ifdef _DEBUG
			afxDump << "File " << pFileName << " cannot be removed\n";
		#endif
	}
	END_CATCH

	// Filename and path for the file to be created
	UpdateData(TRUE);
	CString sExcelFile = m_OutFile;                
	
	CString sSql;      
	TRY  
	{
		// Build the creation string for access without DSN       
	    sSql.Format("DRIVER={%s};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s", sDriver,sExcelFile,sExcelFile);
	
		// Create the database (i.e. Excel sheet)
		if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )    
		{
			// Create table structure
			sSql = "CREATE TABLE demo (	��Ʊ���� TEXT, 	���� TEXT, ���̼� NUMBER,��߼� NUMBER,��ͼ� NUMBER,���̼� NUMBER,�ɽ��� NUMBER,�ɽ���� NUMBER)";
			database.ExecuteSQL(sSql);      
		}          
	}  
	CATCH_ALL(e)  
	{
		TRACE1("Driver not installed: %s",sDriver);
	}  
	END_CATCH_ALL;

}



void COutPut2Excel::OnOK() 
{


	CPropertyPage::OnOK();
}

void COutPut2Excel::OnUpdateEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	//tModified();
	// TODO: Add your control notification handler code here
	
}

void COutPut2Excel::OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//tModified();
	*pResult = 0;
}

void COutPut2Excel::OnDropdownDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//tModified();
	*pResult = 0;
}

void COutPut2Excel::OnDatetimechangeDatetimepicker2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//tModified();
	*pResult = 0;
}

void COutPut2Excel::OnDropdownDatetimepicker2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//tModified();
	*pResult = 0;
}

void COutPut2Excel::OnSelchangeComboMarket() 
{
	// TODO: Add your control notification handler code here
	//tModified();	
}

void COutPut2Excel::OnButton1() 
{
	if( this->m_BeginDate>CTime::GetCurrentTime() )
	{
		AfxMessageBox("ʱ�䳬ǰ��", MB_ICONASTERISK);
		return;
	}

	if( this->m_BeginDate>this->m_EndDate )
	{
		AfxMessageBox("ʱ���Ⱥ�ѭ��ߵ���", MB_ICONASTERISK);
		return;
	}

	CFileFind finder;
	if( finder.FindFile("c:\\program files\\microsoft office\\office\\excel.exe")||
		finder.FindFile("d:\\program files\\microsoft office\\office\\excel.exe")||
		finder.FindFile("e:\\program files\\microsoft office\\office\\excel.exe")  )
		;
	else
	{
		AfxMessageBox("û�а�װ Microsoft Excel !", MB_ICONASTERISK);
		return;
	}

	if( ((CPropertySheet*)GetParent())->GetActiveIndex( ) != 4 )
		return;

	if( ((CComboBox*)GetDlgItem(IDC_COMBO_MARKET))->GetCurSel()==0 )
		Put2Excel("data\\shanghai\\day.dat",6);
	else
		Put2Excel("data\\shenzhen\\day.dat",4);
	
}
