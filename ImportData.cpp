// ImportData.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "ImportData.h"
#include "mainfrm.h"
#include "CTaiKlineFileKLine.h"
#include "SBDestination.h"
#include "BrowseForFolder.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int shijianflag=0;


/////////////////////////////////////////////////////////////////////////////
// CImportData property page

IMPLEMENT_DYNCREATE(CImportData, CPropertyPage)

CImportData::CImportData() : CPropertyPage(CImportData::IDD)
{
	//{{AFX_DATA_INIT(CImportData)
	m_importAll = 0;
	m_install_end = 0;
	m_install_start = 0;
	//}}AFX_DATA_INIT
	pDoc=CMainFrame::m_pDoc ;

	m_Shanghai=FALSE;
	m_Shenzhen=FALSE;
    m_nImportorcancel=0;
    m_bImport=TRUE;
}

CImportData::~CImportData()
{
}

void CImportData::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportData)
	DDX_Control(pDX, 1012, m_button1);
	DDX_Control(pDX, IDC_INSTALLEND, m_ctrlTimeEnd);
	DDX_Control(pDX, IDC_INSTALLSTART, m_ctrlTimeStart);
	DDX_Control(pDX, IDC_LIST, m_listfw);
	DDX_Control(pDX, IDOK, m_okbutton);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress1);
	DDX_Control(pDX, 1042, m_ComboDatatype);
	DDX_Control(pDX, 1011, m_ComboFiletype);
	DDX_Control(pDX, 1008, m_DataSource);
	DDX_Radio(pDX, 1013, m_importAll);
	DDX_DateTimeCtrl(pDX, IDC_INSTALLEND, m_install_end);
	DDX_DateTimeCtrl(pDX, IDC_INSTALLSTART, m_install_start);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportData, CPropertyPage)
	//{{AFX_MSG_MAP(CImportData)
	ON_CBN_SELCHANGE(1011, OnSelchangedatatype)
	ON_CBN_SELCHANGE(1042, OnSelchangedataformat)
	ON_LBN_SETFOCUS(IDC_LIST, OnSetfocusfwxz)
	ON_EN_SETFOCUS(1008, OnSetfocus)
	ON_BN_CLICKED(1013, Onqbyr)
	ON_BN_CLICKED(1014, Onsdyr)
	ON_EN_SETFOCUS(1019, OnSetfocusup)
	ON_EN_SETFOCUS(1020, OnSetfocusdown)
	ON_BN_CLICKED(IDOK, Onimport)
	ON_BN_CLICKED(1012, OnButtonSearchfixdata)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
	//ON_MESSAGE(WM_DRAWLIST,drawzqsc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportData message handlers

void CImportData::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CPropertyPage::OnOK();
}



BOOL CImportData::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here
    ((CComboBox *)GetDlgItem(1042))->SetCurSel(0);
    ((CComboBox *)GetDlgItem(1011))->SetCurSel(0);

    m_ComboDatatype.InsertString( 0, "ϵͳ���ݸ�ʽ");
	m_ComboDatatype.InsertString( 1, "Ǯ�����ݸ�ʽ");
	m_ComboDatatype.InsertString( 2, "���������ݸ�ʽ");
	m_ComboDatatype.InsertString( 3, "������ݸ�ʽ");
	m_ComboDatatype.InsertString( 4, "ʤ�����ݸ�ʽ");
	
	m_ComboDatatype.SetCurSel(0);
    m_DataSource.ReplaceSel(pDoc->m_CurrentWorkDirectory+"\\StockData.day");

	m_importAll = 0;
	
	GetDlgItem(IDC_INSTALLSTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_INSTALLEND)->EnableWindow(FALSE);

    
    m_install_start=CTime(1999,1,1,1,1,1);
	m_install_end  =CTime::GetCurrentTime();

	LVITEM myitem;
	myitem.mask=LVIF_TEXT|LVIF_IMAGE;
	myitem.iSubItem=0;
	myitem.iImage=6;
	m_listfw.SetImageList(pDoc->m_imagelist,LVSIL_SMALL);
	CString stockgroup[]={"�Ϻ�","����","��ҵ"};
	for(int i=0;i<3;i++)
	{
		myitem.iItem=i;
		if(i==2)
			myitem.iImage=9;
	    myitem.pszText=(LPTSTR)stockgroup[i].GetBuffer(stockgroup[i].GetLength());
	    m_listfw.InsertItem(&myitem);
		if(i==2)
		   m_stockrange.Add(FALSE);
		else
		   m_stockrange.Add(TRUE);
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CImportData::OnSelchangedatatype() 
{
	// TODO: Add your control notification handler code here
	//����ѡ�����ò�ͬ�ĳ�ʼ·��
	if( ((CComboBox*)GetDlgItem(1011))->GetCurSel()==0  )		//��װ���� --By CuiWei.	
	{
		m_DataSource.SetWindowText(pDoc->m_CurrentWorkDirectory+"\\StockData.day");
	    GetDlgItem(1042)->EnableWindow(TRUE);
		GetDlgItem(1013)->EnableWindow(TRUE);
	    GetDlgItem(1014)->EnableWindow(TRUE);
		if(shijianflag==1)
		{
		   GetDlgItem(IDC_INSTALLSTART)->EnableWindow(TRUE);
	       GetDlgItem(IDC_INSTALLEND)->EnableWindow(TRUE);
		}
	}
	else if( ((CComboBox*)GetDlgItem(1011))->GetCurSel()==1  )	
	{
		m_DataSource.SetWindowText(pDoc->m_CurrentWorkDirectory+"\\StockData.min");
		GetDlgItem(1042)->EnableWindow(FALSE);
	   	GetDlgItem(1013)->EnableWindow(TRUE);
	    GetDlgItem(1014)->EnableWindow(TRUE);
		if(shijianflag==1)
		{
		   GetDlgItem(IDC_INSTALLSTART)->EnableWindow(TRUE);
	       GetDlgItem(IDC_INSTALLEND)->EnableWindow(TRUE);
		}
	}
	else if( ((CComboBox*)GetDlgItem(1011))->GetCurSel()==2  )	
	{
		m_DataSource.SetWindowText("C:\\");
		GetDlgItem(1042)->EnableWindow(FALSE);
		GetDlgItem(1013)->EnableWindow(FALSE);
		GetDlgItem(1014)->EnableWindow(FALSE);
		if(shijianflag==1)
		{
		   GetDlgItem(IDC_INSTALLSTART)->EnableWindow(FALSE);
	       GetDlgItem(IDC_INSTALLEND)->EnableWindow(FALSE);
		}
	}

}

void CImportData::OnSelchangedataformat() 
{
	// TODO: Add your control notification handler code here
	switch( m_ComboDatatype.GetCurSel( ) )
	{
	case 0:    //our
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel(pDoc->m_CurrentWorkDirectory+"\\StockData.day");
		if(m_ComboFiletype.GetCount()==1)
		{
			m_ComboFiletype.InsertString(1,"���������");
            m_ComboFiletype.InsertString(2,"һ��������");
		}
		break;
	case 1:    //Ǯ��
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("C:\\");
		if(m_ComboFiletype.GetCount()==3)
		{
			m_ComboFiletype.DeleteString(1);
			m_ComboFiletype.DeleteString(1);
		}
		break;
	case 2:    //������
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("C:\\superstk.dad");
		if(m_ComboFiletype.GetCount()==3)
		{
			m_ComboFiletype.DeleteString(1);
			m_ComboFiletype.DeleteString(1);
		}
		break;
	case 3:    //���
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("C:\\HUIJIN");
		if(m_ComboFiletype.GetCount()==3)
		{
			m_ComboFiletype.DeleteString(1);
			m_ComboFiletype.DeleteString(1);
		}
		break;
	case 4:    //ʤ��
		m_DataSource.SetSel(0,-1);
		m_DataSource.ReplaceSel("C:\\SLON");
		if(m_ComboFiletype.GetCount()==3)
		{
			m_ComboFiletype.DeleteString(1);
			m_ComboFiletype.DeleteString(1);
		}
		break;

	}


}

void CImportData::OnSetfocusfwxz() 
{
	// TODO: Add your control notification handler code here
	
}

void CImportData::OnSetfocus() 
{
	// TODO: Add your control notification handler code here
	
}


void CImportData::Onqbyr() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_INSTALLSTART)->EnableWindow(FALSE);
	GetDlgItem(IDC_INSTALLEND)->EnableWindow(FALSE);
    shijianflag=0;
}

void CImportData::Onsdyr() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_INSTALLSTART)->EnableWindow(TRUE);
	GetDlgItem(IDC_INSTALLEND)->EnableWindow(TRUE);
    shijianflag=1;
}

void CImportData::OnSetfocusup() 
{
	// TODO: Add your control notification handler code here
	
}

void CImportData::OnSetfocusdown() 
{
	// TODO: Add your control notification handler code here
	
}

void CImportData::Onimport() 
{
	//ֹͣ������
	BOOL IsChange=FALSE;//
	if( pDoc->m_systemOption.autoday==TRUE )
	{
		pDoc->m_systemOption.autoday=FALSE;
		IsChange=TRUE;
	}
	/*CTime tm;
	char strErrMessage[255];
	try
	{
		m_ctrlTimeStart.GetTime(tm);
		if (tm<0)
		{
			AfxMessageBox("as");
			return;
		}

	}
	catch(CException *e)
	{
		e->GetErrorMessage(strErrMessage,255);
		AfxMessageBox(strErrMessage);
		e->Delete();
		return;
	}*/

	UpdateData(TRUE);
	m_Shanghai=FALSE;
	m_Shenzhen=FALSE;

	m_nImportorcancel++;
	if(m_nImportorcancel>1)
	{
		//m_nImportorcancel;
		GetDlgItem(IDOK)->SetWindowText("����");
		return;
	}	
  
	int length=m_stockrange.GetSize();
	for (int i=0;i<length;i++)
		if(m_stockrange.GetAt(i)==TRUE)
			if(i==0)
				m_Shanghai=TRUE;
			else if (i==1)
				m_Shenzhen=TRUE;

	if (m_Shanghai==FALSE&&m_Shenzhen==FALSE)
	{
		AfxMessageBox("��ѡ��Χ! ");
	    m_nImportorcancel=0;
		return ;
	}

	if(m_install_start<=0||m_install_end<=0)
	{
		AfxMessageBox("ʱ�����ô���!");
		//GetDlgItem(IDC_INSTALLSTART)->SetFocus();
		m_nImportorcancel=0;
		return;
	}
	int BeginDate=atoi(m_install_start.Format("%Y%m%d"));
	int EndDate  =atoi(m_install_end.Format("%Y%m%d"));
	CTime CurrentTime=CTime::GetCurrentTime();
   
		
	if (shijianflag==1)
		if(BeginDate > EndDate)	//�����쳣
		{
			AfxMessageBox("ʱ��ߵ�,����������!");
			m_nImportorcancel=0;
			return;
		}
		else if(m_install_start>CurrentTime) 
		{
			AfxMessageBox("��ʼʱ�䲻��ȷ,����������!");
			m_nImportorcancel=0;
			return;
		}

		//��װ���߻����������.
	if( ((CComboBox*)GetDlgItem(1011))->GetCurSel()==0  )  //��װ���� 
	{
		//��ʼ��װ
		CString srcfilename;
        switch (m_ComboDatatype.GetCurSel())
		{
		case 0:    //our
              m_DataSource.GetWindowText(srcfilename);
			if(  srcfilename.IsEmpty() )
			{
				AfxMessageBox("�ļ�����ӦΪ��!");
				m_nImportorcancel=0;
				return;
			}
			//�Ƿ�����
			if(IsFileType(srcfilename)!=0)
			{
				AfxMessageBox("�ǺϷ���������!");
				m_nImportorcancel=0;
				return;
			}
            //���������������.
			if( srcfilename[0]=='a'||srcfilename[0]=='b'||srcfilename[0]=='A'||srcfilename[0]=='B')
			{
				if( !InstallFromFloppyDisk(srcfilename) )
				{
					AfxMessageBox("��װû����������!",MB_ICONASTERISK);
					m_nImportorcancel=0;
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
						m_nImportorcancel=0;
						return;
					}
					srcfilename="c:\\source.day";
				}
			}
            GetDlgItem(IDOK)->SetWindowText("ȡ��");
			InstallWanshen(srcfilename, BeginDate, EndDate);
			m_nImportorcancel=0;
			GetDlgItem(IDOK)->SetWindowText("����");
			break;
	   case 1:    //Ǯ��
			m_DataSource.GetWindowText(srcfilename);
			GetDlgItem(IDOK)->SetWindowText("ȡ��");
			InstallQianlong(srcfilename, BeginDate, EndDate);
			m_progress1.SetPos(0);
			if(m_nImportorcancel==1)
			    //AfxMessageBox("���ݵ������");
		    m_nImportorcancel=0;
			GetDlgItem(IDOK)->SetWindowText("����");
			break;
		case 2:    //������ 
			m_DataSource.GetWindowText(srcfilename);
			GetDlgItem(IDOK)->SetWindowText("ȡ��");
			InstallFenxijia(srcfilename, BeginDate, EndDate);
			m_nImportorcancel=0;
			GetDlgItem(IDOK)->SetWindowText("����");
			break;
	    case 3:    //���
			m_DataSource.GetWindowText(srcfilename);
			GetDlgItem(IDOK)->SetWindowText("ȡ��");
			InstallHuijin(srcfilename, BeginDate, EndDate);
			m_nImportorcancel=0;
			GetDlgItem(IDOK)->SetWindowText("����");
			break;
		case 4:    //ʤ��
			m_DataSource.GetWindowText(srcfilename);
			GetDlgItem(IDOK)->SetWindowText("ȡ��");
			InstallShenglong(srcfilename, BeginDate, EndDate);
			m_nImportorcancel=0;
			GetDlgItem(IDOK)->SetWindowText("����");
			break;

		default:
			AfxMessageBox("û��ѡ��װ���ݵ�����!");
			break;
		}
	
		}
		else if( ((CComboBox*)GetDlgItem(1011))->GetCurSel()==1  )  //��װ������� 
		{
		InstallMinteData();
		m_nImportorcancel=0;

		}
		else 
		{
			CString SrcFileName;
		    m_DataSource.GetWindowText(SrcFileName);
			CFileFind finder;
			if(!finder.FindFile(SrcFileName+"\\*.1mn"))
			{
				AfxMessageBox("�Ҳ����ļ�!");
				m_nImportorcancel=0;
		        return ;
			}
		    InstallMin(SrcFileName);
			m_progress1.SetRange(0,100);
			m_progress1.SetPos(100);
			m_progress1.SetPos(0);
			m_nImportorcancel=0;
		}

		if( IsChange )
			pDoc->m_systemOption.autoday=FALSE;
     
}



void CImportData::OnButtonSearchfixdata() 
{
    UpdateData(TRUE);
	CString filename;
	if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==2||(m_ComboDatatype.GetCurSel()==1||m_ComboDatatype.GetCurSel()==3||m_ComboDatatype.GetCurSel()==4)
		&&((CComboBox*)GetDlgItem(1011))->GetCurSel()!=1)
	{
        CSBDestination sb(m_hWnd, IDS_BFF_TITLE);
	    sb.SetFlags(BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT);
	    sb.SetInitialSelection(m_news);
	    if (sb.SelectFolder())
		{
		   m_news.TrimRight (" ");
		   m_news.TrimRight ("\r\n");
		   m_news.TrimRight (";");
		   m_news.TrimRight (" ");

		   CString sNews = sb.GetSelectedFolder();
           //		if(m_news =="")
		   m_news = sNews;
	   
		}
	}
    else
	{
		CString DefExt="*.day";
//		CString DefExt="*.day,*.dad,*.psd,*.min";
		CString FileName;
//		CString Filter="line day data file(*.day)|*.day|(*.dad)|*.dad|(*.psd)|*.psd|(*.min)|*.min||";  //.�ļ���չ��������
		CString Filter="��������(*.day)|*.day||";  //.�ļ���չ��������
		if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==0)
		{
			FileName=pDoc->m_Directory.CurrentDir;
			FileName+="StockData.day";
		}
		else if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==1)
		{
			FileName=pDoc->m_Directory.CurrentDir;
			FileName+="StockData.min";
			DefExt="*.min";
			Filter="5����K������(*.min)|*.min||";
		}
		int nSel = m_ComboDatatype.GetCurSel();
		if(nSel == 2)
		{
			FileName=pDoc->m_Directory.CurrentDir;
			FileName+="StockData.dad";
			DefExt="*.dad";
			Filter="��������������(*.dad)|*.dad||";
		}
		CFileDialog bOpenFileDialog(TRUE,(LPCSTR)DefExt,
	    		(LPCTSTR)FileName,
				 OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,
				(LPCSTR)Filter,this);
		if(bOpenFileDialog.DoModal()==IDOK)
		{
			 filename=bOpenFileDialog.GetPathName();
			 CString fname=bOpenFileDialog.GetFileName();
			 CString NewFileName=filename;
			 NewFileName.TrimRight(fname);
			 strcpy(pDoc->m_Directory.CurrentDir,NewFileName.GetBuffer(0));
		}
	}
	if(((CComboBox*)GetDlgItem(1011))->GetCurSel()==2||m_ComboDatatype.GetCurSel()==1||m_ComboDatatype.GetCurSel()==3||m_ComboDatatype.GetCurSel()==4)
		filename=m_news;
	m_DataSource.SetSel(0,-1);
	m_DataSource.ReplaceSel(filename);
	m_DataSource.GetWindowText(SourceFileName);
    UpdateData(FALSE);
	
}


BOOL CImportData::InstallFromFloppyDisk(CString FloppyFileName,bool bDayKline)
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
		AfxMessageBox("���ļ�����!");
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
			AfxMessageBox("���ļ�����!");
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

//////////////////////////////////////////////////////////////////////
//��װ������ʽ�ĵ����ļ�
void CImportData::InstallWanshen(CString srcfilename, int BeginDate, int EndDate) 
{
	{//begin*************  ****************
	//��������  
	//Դ�����ļ������ƺ�·��.
	BOOL IsSoft=FALSE;	
	CString SourceFilename=srcfilename;
	//UpdateData(TRUE);	
	m_DataSource.GetWindowText(SourceFilename);

	//���������������.
	if( SourceFilename[0]=='a'||SourceFilename[0]=='b'||SourceFilename[0]=='A'||SourceFilename[0]=='B')
	{
		SourceFilename="c:\\source.day";
		IsSoft=TRUE;
	}
	//�����Ӳ�̷ֽ��ļ�����
	CString StrName=SourceFilename;
	StrName.MakeLower();
	if( StrName.Find("tl_disk") != -1 )
	{
		SourceFilename="c:\\source.day";
		IsSoft=TRUE;
	}
	CFile SourceFile;
	if(!SourceFile.Open(SourceFilename,CFile::modeRead))
	{
		AfxMessageBox("·���а��������·��������������");
		return;
	}
	
	//����װ��֤ȯ����
	int NumStock=0 ;             
	int FileID;  //�ļ���ʶ
	SourceFile.Read(&FileID,4);
	if( FileID!=FILEID22)
	{
		AfxMessageBox("��"+g_strCompanyName+"�������ݸ�ʽ!");
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
	//  add begin46
	int wMarketType;
	//  add end46
	for( int nStock=0; nStock<NumStock; nStock++)
	{

		MSG message;
		if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}  
        if(m_nImportorcancel==2)
			return;
		//show the progress
		m_progress1.SetPos(nStock);
		m_InstallArray.RemoveAll();

		//����װ������
		SourceFile.Read(StockName,8);
		//  add begin46
		SourceFile.Read(&wMarketType,sizeof(int));
		//  add end46
		SourceFile.Read(&NumKline,4);
		//TRACE("StockName=%s,NumKline=%d\n",StockName,NumKline);
		for( int nKline=0; nKline<NumKline; nKline++)
		{
			SourceFile.Read( &wanshen,sizeof(Kline) );
			m_InstallArray.Add(wanshen);
		}
		CString symbol(StockName);
		/*if(symbol.Mid(0,2)=="2B")
		{
			int test=2;
			test++;
		}*/
		//ȥ��8k
		if(StockName[0]=='8'&&(StockName[1]=='k'||StockName[1]=='K'))
			continue;
		//if(wMarketType==SZ_MARKET_EX)
		InstallStockKlineEach( symbol,wMarketType, BeginDate, EndDate);
	}

	////////////////////////////////////////////////////////////////
	//�ر�����Դ�ļ��ͱ����ļ�
	SourceFile.Close();
    m_progress1.SetPos(0);
	if(IsSoft)
		SourceFile.Remove(SourceFilename);
	}
}

int CImportData::InstallFromSeveralDiskfile(CString FileName,bool bDayKline)
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
		AfxMessageBox("���ļ�����!");
		return 0;
	}

	int IndexOfDisk=1;  //��������.
	int nRepeat=0;      //�ظ�����.
	//do-whileѭ�����а�װ.
	do
	{
		CString str;
		str.Format("%d",IndexOfDisk);
	    if(IndexOfDisk<11)
		   FileName.Delete( FileName.GetLength()-5 );
		else 
			FileName.Delete( FileName.GetLength()-6,2 );
		FileName.Insert( FileName.GetLength()-4, str );
		if( !SrcFile.Open(FileName,CFile::modeRead) )
		{
			AfxMessageBox("���ļ�����!");
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
		/*int Id;
	    SrcFile.Read(&Id,4);*/
		SrcFile.Read(Buffer,LengthOfFile);  //�������϶�����.
		SrcFile.Close();
		int nid = *((int*)Buffer);
		DestFile.Write(Buffer,LengthOfFile);//��Ӳ����д����.
		delete []Buffer;
		
		IndexOfDisk++;
		nRepeat=0;

	}while(NumOfDisks>nDisk); //��������ѭ������.

	DestFile.Close();

	return 1;

}
void CImportData::InstallStockKlineEach(CString symbol,int wMarketType,int BeginDate,int EndDate,bool bDayKline)
{
		//Get the pointer of kline file .
		//if(m_Shanghai==TRUE &&m_Shenzhen==FALSE&& CSharesCompute::GetMarketKind(symbol) == SZ_MARKET_EX)
		//	return;
		//else if (m_Shenzhen==TRUE && m_Shanghai==FALSE&&CSharesCompute::GetMarketKind(symbol) == SH_MARKET_EX)
		//	return;
	if(symbol == "") return;

		char symb[10];
		strcpy(symb,symbol);
		DWORD dwKind=CSharesInformation::GetStockKind(wMarketType,symb);

		if(wMarketType == 0) dwKind = CFormularCompute::GetStockKind(symb);

		CTaiKlineFileKLine* pFile = CTaiKlineFileKLine::GetFilePointer (symbol,dwKind,bDayKline);
		
		CTime tm;
		//ѡ��װ��ʽ.
		int nFixStyle=GetCheckedRadioButton(1013,1014);
		switch(nFixStyle)
		{
		case 1014:  //ָ��ʱ�ΰ�װ
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

		case 1013 :   //ȫ������
			pFile ->WriteKLine(symbol,m_InstallArray.GetData(),m_InstallArray.GetSize(),CTaiKlineFileKLine::overWriteAll);
			break;
		}
}

////////////////////////////////////////////////////////////////
//��װǮ����ʽ�ĵ����ļ�
void CImportData::InstallQianlong(CString srcfilename, int BeginDate, int EndDate) 
{
	if (shijianflag==1)
	if(  BeginDate<19700101 ||BeginDate>20380101 ||
		 EndDate  <19700101 ||EndDate  >20380101 || BeginDate > EndDate )
	{
		AfxMessageBox("����������ȷ��ʱ��!",MB_ICONASTERISK);
		return;
	}
	//�������� Shanghai//////////////////////////////////////////
	{
		//�����Ŀ¼��, �����������ļ��ж���
		int NumFile=0;
		CString FinderName;
		CFileFind finder;
		m_DataSource.GetWindowText(FinderName);
		if(!finder.FindFile(FinderName+"\\*.*"))
		{
			AfxMessageBox("���ļ�����,�����������!");
			return;
		}
		BOOL bwork = finder.FindFile(FinderName+"\\*.*");
		while( bwork )
		{
			bwork=finder.FindNextFile();
			NumFile++;
		}
		if( !NumFile )
		{
			AfxMessageBox("û��Ҫ��װ������!");
			return;
		}

		m_progress1.SetRange32(0,NumFile);
		m_progress1.SetPos(0);

		Kline Wanshen;
		CString StockName;  //��Ʊ����	  
		CString szTime;
		CTime tm;

		//������Ŀ¼�µ������ļ�,���а�װ
		BOOL bWorking = finder.FindFile(srcfilename+"\\*.day");
		while( bWorking )
		{
			MSG message;
			if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}  
			if(m_nImportorcancel==2)
				return;
			//
			m_InstallArray.RemoveAll();
			m_progress1.OffsetPos(1);

			bWorking=finder.FindNextFile();
  
			//Ǯ���洢�����ļ�, �����ǵ����������ļ�, ��Ϊ�����������ļ���ѹ����.
			CString QianlongFileName=finder.GetFilePath();
			CFile QianlongFile((LPCTSTR)QianlongFileName,CFile::modeRead);
			int FileLength=QianlongFile.GetLength();
			
			StockName=finder.GetFileTitle();
			
			if( StockName.GetLength() !=6&&StockName.GetLength() !=4 )		
				continue;
			
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
			//��ʱע�͵�  begin46
			InstallStockKlineEach( StockName, 0,BeginDate, EndDate);
			//��ʱע�͵�  end46
		}
		m_progress1.SetPos(0);
	}

}
//�õ������� 
BOOL CImportData::GetStockTime(int ymd, LONG& tm)
{
	if( ymd<20380101 && ymd > 19901201 )
	{
		CTime time( ymd/10000, (ymd%10000)/100, ymd%100, 9, 30, 0);
		tm=time.GetTime();
		return 1;
	}
	else
		return 0;

}

/////////////////////////////////////////////////////////////////////
// ��װ�����ҵ����ݸ�ʽ
void CImportData::InstallFenxijia(CString srcfilename ,int BeginDate, int EndDate) 
{
	//�ӷ����ҵ������ļ��ж�����
	CFile SourceFile;
	if(IsFileType(srcfilename)!=3)
	{
		AfxMessageBox("���Ƿ��������������ļ���ʽ!");
		return;
	}
	if(!SourceFile.Open(srcfilename,CTaiKlineMemFile::modeRead))
	{
		AfxMessageBox("���ļ�����,�����������!");
		return;
	}
	int NumStock=0;//����װ��֤ȯ����
    int Flag1;
	SourceFile.Read(&Flag1,4);
	if(Flag1!=872159628)
	{
		AfxMessageBox("���Ƿ��������������ļ���ʽ!");
		return;
	}
	//SourceFile.Read(&Flag2,4);
	SourceFile.Seek(8,CFile::begin);
	SourceFile.Read(&NumStock,sizeof(int));

	int NumKlinesOfFxj =  (SourceFile.GetLength()-16)/32;

	if( NumKlinesOfFxj<0 )
		return;

	SourceFile.Seek(16,CFile::begin);
	Kline *tianlong=new Kline[NumKlinesOfFxj]; //������Ҫ�ͷ�.
	SourceFile.Read(tianlong,32*NumKlinesOfFxj);

	m_progress1.SetRange32(0,NumStock);    //���ý������ķ�Χ

	CTime tm;   //��װʱ�� 
	tm=CTime::GetCurrentTime();
	int CurrentDate=atoi(tm.Format("%Y%m%d"));

	int nFixStyle=GetCheckedRadioButton(IDC_RADIO_COVER,IDC_RADIO_INSTALLDATE); //ѡ��װ��ʽ.

	{//
	CString StockSymbol,PreStockSymbol;
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
           if(m_nImportorcancel==2)
		   {
				SourceFile.Close();   //�رշ����ҵ������ļ�.
				delete []tianlong;   //�ͷ��ڴ�.
			   return;
		   }
		  StockSymbol=(char*)&tianlong[i]+6;
			if(StockSymbol.GetLength () == 6||StockSymbol.GetLength () == 4) 
			{
				//��ʱע�͵�  begin46
				InstallStockKlineEach( PreStockSymbol,0, BeginDate, EndDate);
				//��ʱע�͵�  end46
				m_InstallArray.RemoveAll();
			}
		  PreStockSymbol=StockSymbol;
		}
		else
		{
			tm=tianlong[i].day;
			if( tm>0 && atoi(tm.Format("%Y%m%d")) <=CurrentDate )
				m_InstallArray.Add(tianlong[i]);
		}
	}
	}//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	delete []tianlong;   //�ͷ��ڴ�.
    m_progress1.SetPos(0);
	SourceFile.Close();   //�رշ����ҵ������ļ�.

}
void CImportData::InstallHuijin(CString srcfilename, int BeginDate, int EndDate) 
{
	if (shijianflag==1)
	{
		if(  BeginDate<19700101 ||BeginDate>20380101||
			 EndDate  <19700101 ||EndDate>20380101 )
		{
			AfxMessageBox("����������ȷ��ʱ��!");
			return;
		}
		if( BeginDate>EndDate)
		{
			AfxMessageBox("����ʱ��Ƿ�!",MB_ICONASTERISK);
			return;
		}
	}
	{// //////////////////////////////////////////////////////

		//�����Ŀ¼��, �����������ļ��ж���
		int NumFile=0;
		CString FinderName;
		CFileFind finder;
		m_DataSource.GetWindowText(FinderName);
	    if(!finder.FindFile(FinderName+"\\*.*"))
		{
			AfxMessageBox("���ļ�����,�����������!");
			return;
		}
		BOOL bwork = finder.FindFile(FinderName+"\\*.*");
		while( bwork )
		{
			bwork=finder.FindNextFile();
			NumFile++;
		}

		if( !NumFile )
		{
			AfxMessageBox("û��Ҫ��װ������!");
			return;
		}

		m_progress1.SetRange32(0,NumFile);

		//Fenxijia data file.
		Kline Tianlong;
		CString StockName;  //��Ʊ����	  
		CString szTime;
		CTime tm;

		//������Ŀ¼�µ������ļ�,���а�װ
		BOOL bWorking = finder.FindFile(srcfilename+"\\*.*");
		while( bWorking )
		{
			MSG message;
			if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}  
		    
			if(m_nImportorcancel==2)
				return;
			//
			m_InstallArray.RemoveAll();
			m_progress1.OffsetPos(1);

			bWorking=finder.FindNextFile();

			//Huijin�洢�����ļ�, �����ǵ����������ļ�.
			CString HuijinFileName=finder.GetFilePath();
			CFile HuijinFile(HuijinFileName,CFile::modeRead);
			int FileLength=HuijinFile.GetLength();
			
			StockName=finder.GetFileTitle();
			if( StockName.GetLength()!=8 && StockName.GetLength()!=6) 	continue;  //if not shanghai , then return.
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
			//��ʱע�͵�  begin46
			InstallStockKlineEach( StockName,0, BeginDate, EndDate);
			//��ʱע�͵�  end46
		}
			
	  
		}
        m_progress1.SetPos(0);

}


//��װʤ��������
void CImportData::InstallShenglong(CString srcfilename, int BeginDate, int EndDate)
{
	//ʱ�����
	if (shijianflag==1)
	if(  BeginDate<19700101 ||BeginDate>20380101||
		 EndDate  <19700101 ||EndDate  >20380101|| 
		 BeginDate>EndDate   )
	{
		AfxMessageBox("�Ƿ�����!����������ȷ��ʱ��!");
		return;
	}

	{ //shanghai shanghai shanghai shanghai shanghai 
		//�����Ŀ¼��, �����������ļ��ж���
		int NumFile=0;
		CString FinderName;
		CFileFind finder;
		//�ӱ༭���ж���Ҫ���ҵ��ļ�����, ���� D:\SLON\DATA\SH\DAY\*.day
		m_DataSource.GetWindowText(FinderName);
		if(!finder.FindFile(FinderName+"\\*.day"))
		{
			AfxMessageBox("���ļ�����,�����������!");
			return;
		}
		BOOL bwork = finder.FindFile(FinderName+"\\*.day");
		while( bwork )
		{
			bwork=finder.FindNextFile();
			NumFile++;
		}

		if( !NumFile )
		{
			AfxMessageBox("û��Ҫ��װ������!");
			return;
		}


		//���ý������Ĺ�����Χ.
		m_progress1.SetRange32(0,NumFile);
		m_progress1.SetPos(0);

		CString StockName, /*��Ʊ����*/ 	szTime;
		CTime tm;

		//������Ŀ¼�µ������ļ�,���а�װ
		BOOL bWorking = finder.FindFile(srcfilename+"\\*.day");
		while( bWorking )
		{
			//���ȴ���Windows��Ϣ.
			MSG message;
			if( PeekMessage( & message, NULL,0,0,PM_REMOVE) )
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}  
			
			if(m_nImportorcancel==2)
				return;
			//ÿ��װһֻ��Ʊ,����Ҫ������е�Ԫ��.
			m_InstallArray.RemoveAll();
			m_progress1.OffsetPos(1);

			bWorking=finder.FindNextFile();

			//ʤ���洢�����ļ�, �����ǵ����������ļ�
			CString ShenglongFileName=finder.GetFilePath();
			CFile ShenglongFile(ShenglongFileName,CFile::modeRead);
			int FileLength=ShenglongFile.GetLength();
			
			StockName=finder.GetFileTitle();
			if( StockName.GetLength() != 6&&StockName.GetLength() != 4 ) continue;
						
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
			//��ʱע�͵�  begin46
			InstallStockKlineEach( StockName,0, BeginDate, EndDate);
			//��ʱע�͵�  end46
	
		} //shanghai shanghai shanghai shanghai shanghai 
	}
	
	m_progress1.SetPos(0);

}
BOOL CImportData::ShenglongToTianlong(SHENGLONG shenglong,Kline& Wanshen)
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
		Wanshen.day    = tm; 
		Wanshen.open   = float(shenglong.oprice/1000.0);
		Wanshen.high   = float(shenglong.hprice/1000.0);
		Wanshen.low    = float(shenglong.lprice/1000.0);
		Wanshen.close  = float(shenglong.cprice/1000.0);
		Wanshen.vol    = float(shenglong.tohands);
		Wanshen.amount = float(shenglong.tovalue*1000.0);
		Wanshen.advance=	 0;
		Wanshen.decline=	 0;
		return 1;
	}

	return 0;

	
}
void CImportData::InstallMinteData()
{
	//��������
	//Դ�����ļ������ƺ�·��.
	CString SourceFilename,srcfilename;
	m_DataSource.GetWindowText(SourceFilename);
	srcfilename = SourceFilename;
	BOOL IsSoft=FALSE;
	//�Ƿ��������
	if(IsFileType(srcfilename)!=1)
	{
		AfxMessageBox("�ǺϷ����������!");
		return;
	}
	//���������������.
	if( srcfilename.Mid(0,3)=="a:\\"||srcfilename.Mid(0,3)=="b:\\"||srcfilename.Mid(0,3)=="A:\\"||srcfilename.Mid(0,3)=="B:\\")
	{
		if( !InstallFromFloppyDisk(srcfilename,false) )
		{
			AfxMessageBox("��װû����������!",MB_ICONASTERISK);
			return;
		}
		IsSoft=TRUE;
		srcfilename="c:\\source.min";
	}
	else //��Ӳ���ϵķֽ��ļ����а�װ.
	{
		srcfilename.MakeLower();
		if( srcfilename.Find("tl_disk") != -1 )
		{
			if( ! InstallFromSeveralDiskfile(srcfilename,false) )
			{
				AfxMessageBox("��װû����������!",MB_ICONASTERISK);
				return;
			}
			IsSoft=TRUE;
			srcfilename="c:\\source.min";
		}
	}

	UpdateData(TRUE);	

	CFile SourceFile;
	if(!SourceFile.Open(srcfilename,CFile::modeRead))
	{
		AfxMessageBox("·���а��������·��,����������!");
		return;
	}
	
	//����װ��֤ȯ����
	int NumStock=0 ;             
	int FileID;  //�ļ���ʶ
	SourceFile.Read(&FileID,4);
	if( FileID!=FILEID22)
	{
		AfxMessageBox("��"+g_strCompanyName+"��������ݸ�ʽ!");
		return;
	}
	SourceFile.Read(&NumStock,sizeof(int));
	//SourceFile.Seek(16,CFile::begin);
	
	//���ý������ķ�Χ
	m_progress1.SetRange32(0,NumStock);
	
	//�˹�Ʊ��K����Ŀ
	int NumKline=0;
	char StockName[8];	
	Kline wanshen;
	CTime tm;       //��װʱ�� 
	//  add begin46
	int wMarketType;
	//  add end46
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
		SourceFile.Read(StockName,7);
		//  add begin46
		SourceFile.Read(&wMarketType,sizeof(int));
		//  add end46
		SourceFile.Read(&NumKline,4);
		SourceFile.Seek(8,CFile::current);
		for( int nKline=0; nKline<NumKline*48; nKline++)
		{
			SourceFile.Read( &wanshen,sizeof(Kline) );
			m_InstallArray.Add(wanshen);
		}
		//ASSERT(NumKline>0);
        if(NumKline<=0)
			continue;
		CString symbol(StockName);
		//ȥ��8k
		//TRACE("StockName=%s,NumKline=%d\n",StockName,NumKline);
		if(StockName[0]=='8'&&(StockName[1]=='k'||StockName[1]=='K'))
			continue;
		CTime tm(m_InstallArray[0].day);
		int BeginDate =  atoi(tm.Format ("%Y%m%d"));
		tm = (m_InstallArray[m_InstallArray.GetSize()-1].day);
		int EndDate =  atoi(tm.Format ("%Y%m%d"));
		InstallStockKlineEach( symbol,wMarketType ,BeginDate, EndDate,false);
	
	}
	////////////////////////////////////////////////////////////////
	//�ر�����Դ�ļ��ͱ����ļ�
	SourceFile.Close();
    if(IsSoft==TRUE)
        SourceFile.Remove(srcfilename);
	m_progress1.SetPos(0);

}

void CImportData::InstallMin(CString lpStockSymbol)
{
    CFileFind finder;
	BOOL bWorking;
	CString StockOpenPath;
	if(!(bWorking=finder.FindFile(lpStockSymbol+"\\*.1mn")))
	{
		AfxMessageBox(" �����ڷ�ʱ�����ļ�!");
	    return ;
	}
	//
	//�����Ŀ¼��, �����������ļ��ж���
	int NumFile=0;
	while(bWorking)
	{
		bWorking=finder.FindNextFile();
		NumFile++;
	}
	if( !NumFile )
	{
		AfxMessageBox("û��Ҫ��װ������!");
		return;
	}
    bWorking=finder.FindFile(lpStockSymbol+"\\*.1mn");
	//
	m_progress1.SetRange(0,NumFile);
	m_progress1.SetPos(0);
	while(bWorking)
	{
        bWorking=finder.FindNextFile();
		StockOpenPath=finder.GetFilePath();
	
        CString Stocklen=finder.GetFileTitle();
//        if (m_Shanghai==TRUE &&m_Shenzhen==FALSE&& (CSharesCompute::GetMarketKind(Stocklen) == SZ_MARKET_EX))
//	       continue;
//        if(m_Shenzhen==TRUE &&m_Shanghai==FALSE&& (CSharesCompute::GetMarketKind(Stocklen) == SH_MARKET_EX))
//	       continue;
        InstallOneMinStock(StockOpenPath);
	    m_progress1.OffsetPos(1);
		//Sleep(1000);
	}
	//m_progress1.SetPos(0);
}

void CImportData::InstallOneMinStock(CString FilePath)
{
    CFile StockFile;
	CReportData* pcdat1;
	CdatOld  pcdat2;
    if(!StockFile.Open(FilePath,CFile::modeRead|CFile::shareDenyNone))
	{
		//StockFile.Close();
		return;
	}
	StockFile.Read(&pcdat2,sizeof(CdatOld));
	StockFile.Close();
    if( ! CMainFrame::m_pDoc->m_sharesInformation.Lookup(pcdat2.id, pcdat1,pcdat2.kind) )
	    return;
    pcdat1->lastclmin=239;

	for (int i = 0;i < 240;i++)
		pcdat1->rdp[i] = pcdat2.rdp[i];           //      ���ڼ������ǿ��ָ��
	strcpy(pcdat1->id,pcdat2.id); 	//      ֤ȯ����
	//  add begin46
	pcdat1->kind=pcdat2.kind;
	//  add end46
	strcpy(pcdat1->name,pcdat2.name);           //      ֤ȯ���� 
	strcpy(pcdat1->Gppyjc,pcdat2.Gppyjc);         //      ֤ȯ����ƴ�����
    //		pcdat1->serl=pcdat2.sel;
	pcdat1->ystc=pcdat2.ystc;              //      ��������
	pcdat1->opnp=pcdat2.opnp;              //      ���տ��� 
	pcdat1->higp=pcdat2.higp;              //      �������
	pcdat1->lowp=pcdat2.lowp;              //      �������
	pcdat1->nowp=pcdat2.nowp;              //      ���¼۸�
	pcdat1->nowv=pcdat2.nowv;              //      ���³ɽ���
	pcdat1->totv=pcdat2.totv;              //      �ܳɽ���
	pcdat1->totp=pcdat2.totp;              //      �ܳɽ����
	pcdat1->pbuy1=pcdat2.pbuy1;             //      �����һ
	pcdat1->vbuy1=pcdat2.vbuy1;             //      ������һ
	pcdat1->pbuy2=pcdat2.pbuy2;             //      ����۶�  
	pcdat1->vbuy2=pcdat2.vbuy2;             //      ��������
	pcdat1->pbuy3=pcdat2.pbuy3;             //      ������� 
	pcdat1->vbuy3=pcdat2.vbuy3;             //      ��������      
	pcdat1->psel1=pcdat2.psel1;             //      ������һ
	pcdat1->vsel1=pcdat2.vsel1;             //      ������һ
	pcdat1->psel2=pcdat2.psel2;             //      �����۶� 
	pcdat1->vsel2=pcdat2.vsel2;             //      ��������   
	pcdat1->psel3=pcdat2.psel3;             //      ��������
	pcdat1->vsel3=pcdat2.vsel3;             //      �������� 
	pcdat1->psel4=pcdat2.psel4;             //      ��������
	pcdat1->vsel4=pcdat2.vsel4;             //      �������� 
	pcdat1->psel5=pcdat2.psel5;             //      ��������
	pcdat1->vsel5=pcdat2.vsel5;             //      �������� 
	pcdat1->accb=pcdat2.accb;              //      ����ҵ��ͣ���������ҵ��ͣ�
	pcdat1->accs=pcdat2.accs;              //      �����ҵ��ͣ����������ҵ��ͣ�
	pcdat1->volume5=pcdat2.volume5;           //      5�����
	pcdat1->rvol=pcdat2.rvol;              //      ����
	pcdat1->dvol=pcdat2.dvol;              //      ����
	pcdat1->lastclmin=pcdat2.lastclmin;         //      �ϴδ��������
	pcdat1->initdown=pcdat2.initdown;          //      ��Ʊ��ʼ����־   0 = δ�ܳ�ʼ����1=�ѳ�ʼ��   
	pcdat1->InOut=pcdat2.InOut;
//	pcdat1->StartP=pcdat2.StartP;        //      ��ʼʱ��۸� 
//	pcdat1->EndP=pcdat2.EndP;          //      ����ʱ��۸�
	//memcpy(pcdat1->m_Kdata1,pcdat2.m_Kdata1,240*sizeof(Kdata1));
    for(int i=0;i<240;i++)
   {
	   pcdat1->m_Kdata1[i].Price=pcdat2.m_Kdata[i].Price;
	   pcdat1->m_Kdata1[i].Amount=pcdat2.m_Kdata[i].Amount;
	   pcdat1->m_Kdata1[i].Volume=pcdat2.m_Kdata[i].Volume;
   }
}


int CImportData::IsFileType(CString Filename)
{
    Filename.TrimRight(" ");
    if(Filename.Right(4)==".day"||Filename.Right(4)==".DAY")
		return 0;
	else if(Filename.Right(4)==".min"||Filename.Right(4)==".min")
		return 1;
	else if(Filename.Right(4)==".1mn"||Filename.Right(4)==".1MN")
		return 2;
	else if(Filename.Right(4)==".dad"||Filename.Right(4)==".DAD")
		return 3;
	else 
		return 4;
}

void CImportData::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    int ItemIndex=m_listfw.GetNextItem(-1,LVNI_SELECTED);
	if (ItemIndex==-1)
		return;
    LVITEM myitem;
	myitem.mask=LVIF_TEXT|LVIF_IMAGE;
	myitem.iSubItem=0;

	if(m_stockrange.GetAt(ItemIndex)==TRUE)
	{
		myitem.iImage=9;
		m_stockrange.SetAt(ItemIndex,FALSE);
	}//
	else
	{
		myitem.iImage=6;
		m_stockrange.SetAt(ItemIndex,TRUE);
	}
    CString stockgroup[]={"�Ϻ�","����","��ҵ"};	
	myitem.iItem=ItemIndex;
	myitem.pszText=(LPTSTR)stockgroup[ItemIndex].GetBuffer(stockgroup[ItemIndex].GetLength());
	//
	m_listfw.SetItem(&myitem);
	m_listfw.Update(ItemIndex);

	*pResult = 0;
}

BOOL CImportData::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	DoHtmlHelp(this);return TRUE;
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,IDD_DATAIMPORT);
//	return TRUE;	

}
