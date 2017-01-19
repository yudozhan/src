// CTaiChuQuanInDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiChuQuanInDlg.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiChuQuanInDlg dialog


CTaiChuQuanInDlg::CTaiChuQuanInDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiChuQuanInDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiChuQuanInDlg)
	m_InFile = _T("");
	m_FormName = _T("");
	//}}AFX_DATA_INIT
	pDoc=CMainFrame::m_pDoc;
}


void CTaiChuQuanInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiChuQuanInDlg)
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_READ, m_read);
	DDX_Control(pDX, IDC_IMPORT, m_import);
	DDX_Control(pDX, 1011, m_PowerForm);
	DDX_Text(pDX, IDC_INFILE, m_InFile);
	DDX_Text(pDX, IDC_STATIC_NAME, m_FormName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiChuQuanInDlg, CDialog)
	//{{AFX_MSG_MAP(CTaiChuQuanInDlg)
	ON_BN_CLICKED(IDC_IMPORT, OnImport)
	ON_CBN_SELCHANGE(1011, OnSelchangeGeshi)
	ON_BN_CLICKED(IDC_READ, OnRead)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiChuQuanInDlg message handlers

void CTaiChuQuanInDlg::OnImport() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	FXJPOWER power_fxj;
//	POWER Power;
//	char strStockCode[8]; 
	int nSplitMask=0;
	int nTotalCount=0;
	int nEachChuQuanNo=0;


	CFile InFile;
	if(!(InFile.Open(m_InFile,CFile::modeRead,NULL)))
	{
		MessageBox("引入除权文件不可读!","警告",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	CString StockSymbol;      //股票代码
	int Marker;               //隔离标记
	//short int nCount=0;

	m_FormName="正在引入除权数据...";
//	m_Progress.ShowWindow(SW_SHOW);
    GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_FormName);
	Split pSplit[80];
	BOOL IsSucc;
	if( m_PowerForm.GetCurSel() == 0 )
	{
		InFile.Close();
		IsSucc=InstallPowerData(m_InFile);
		
	}

	//从分析家的除权数据文件导入
	else 
	{
		if( m_PowerForm.GetCurSel() == 1 )
		{
			CString StockCode;
			WORD nFlag=0;
			InFile.Read(&nFlag,2);
			if( nFlag!=51250 )
			{
			   AfxMessageBox("不是分析家的除权数据格式!");
			   InFile.Close();
			   IsSucc=FALSE;
			   return;
			}
			InFile.Seek(8,CFile::begin);
			nEachChuQuanNo=0;
        	BOOL bFirstTime=TRUE;
			bool bSh = true;
		    while( InFile.Read(&Marker,4) == 4 )
			{
				if( Marker == 0xFFFFFFFF )
				{
					if(bFirstTime==TRUE)
					   bFirstTime=FALSE;  
					else
					{
						//	将上次pSplit写进内存；
						int stkKind = 0;
						if(bSh == true)
							stkKind = pDoc->m_sharesInformation.GetStockKind(SH_MARKET_EX,StockSymbol.GetBuffer(0));
						else
							stkKind = pDoc->m_sharesInformation.GetStockKind(SZ_MARKET_EX,StockSymbol.GetBuffer(0));
						if(!pDoc->m_sharesInformation.ImportChuQuanInfo(StockSymbol,pSplit,nEachChuQuanNo,stkKind))
						{
							return;
						}
					}
		        		//清空PSPLIT；
						//读出除权数据；写进pSplit；
				   ::ZeroMemory(pSplit,80*sizeof(Split));
				   nEachChuQuanNo=0;
				   InFile.Read(StockSymbol.GetBuffer(12),12);
				   StockSymbol.ReleaseBuffer(); 
				   InFile.Seek(4,CFile::current);

				   //lmb add
				   StockSymbol.MakeLower ();
				   bSh = false;
				   if(StockSymbol.Left (2) == "sh") bSh = true;

				   if( StockSymbol.GetLength() == 8 )
                      StockSymbol=StockSymbol.Right(6);
				   else if(StockSymbol.GetLength() == 6)
					  StockSymbol=StockSymbol.Right(4);
				   //InFile.Seek(-8,CFile::current);
				   InFile.Read(&power_fxj, sizeof(FXJPOWER));
				   //InFile.Read(&(pSplit[nEachChuQuanNo].Free),8); 
				   //InFile.Read(&(pSplit[nEachChuQuanNo].nFlags),4);
				   pSplit[nEachChuQuanNo].nTime=power_fxj.nTime;
				   pSplit[nEachChuQuanNo].Give=power_fxj.fGive;
				   pSplit[nEachChuQuanNo].Allocate=power_fxj.fAlloc;
				   pSplit[nEachChuQuanNo].AllocatePrice=power_fxj.fAllocPrice;
				   pSplit[nEachChuQuanNo].Bonus=power_fxj.fDividend;
				   nEachChuQuanNo++;
				}
				else 
				{
					//指针后退4；写进pSplit；
					InFile.Seek(-4,CFile::current);
					InFile.Read(&power_fxj, sizeof(FXJPOWER));
					//InFile.Read(&(pSplit[nEachChuQuanNo].Free),8); 
					//InFile.Read(&(pSplit[nEachChuQuanNo].nFlags),4);
					pSplit[nEachChuQuanNo].nTime=power_fxj.nTime;
					pSplit[nEachChuQuanNo].Give=power_fxj.fGive;
					pSplit[nEachChuQuanNo].Allocate=power_fxj.fAlloc;
					pSplit[nEachChuQuanNo].AllocatePrice=power_fxj.fAllocPrice;
					pSplit[nEachChuQuanNo].Bonus=power_fxj.fDividend;
					nEachChuQuanNo++;
				}
			}
		}
		   //将最后一次pSplit写进内存；
		   InFile.Close();
		   IsSucc=TRUE;
	}
    //	m_Progress.ShowWindow(SW_HIDE);
	if(IsSucc)
	   m_FormName="引入完毕!";
	else
		m_FormName="引入失败!";
	UpdateData(FALSE);
	
}

BOOL CTaiChuQuanInDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_PowerForm.InsertString(0,""+g_strCompanyName+"除权数据格式");
	m_PowerForm.InsertString(1,"分析家除权数据格式");
	m_PowerForm.SetCurSel(0);
	m_InFile="C:\\dest.pwr";
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaiChuQuanInDlg::OnSelchangeGeshi() 
{
	// TODO: Add your control notification handler code here
	switch( m_PowerForm.GetCurSel() )
	{
		case 0:
			m_InFile="C:\\dest.pwr";
			m_FormName="必须是"+g_strCompanyName+"格式的数据!";
			//GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_FormName);
			UpdateData(FALSE);
			break;
		case 1:
			m_InFile="C:\\split.pwr";
			m_FormName="必须是分析家格式的数据!";
			//GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_FormName);
			UpdateData(FALSE);
			break;
	}
	
}

void CTaiChuQuanInDlg::OnRead() 
{
	// TODO: Add your control notification handler code here
    CString DefExt,FileName,Filter;
	if(m_PowerForm.GetCurSel()==0)
	{
	    DefExt="*.pwr";
	    FileName="dest.pwr";
	    Filter="split file (*.pwr)|*.pwr||";
	}
	else
	{
	    DefExt="*.pwr";
	    FileName="split.pwr";
	    Filter="split file (*.pwr)|*.pwr||";
	}
	CFileDialog bOpenFileDialog(TRUE,(LPCSTR)DefExt,(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,(LPCSTR)Filter,this);
	if( bOpenFileDialog.DoModal() ==IDOK )
		m_InFile=bOpenFileDialog.GetPathName();

	UpdateData(FALSE);
//	CTaiShanDoc * pDoc=GetDocument();
//	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);
	
}

BOOL CTaiChuQuanInDlg::InstallPowerData(CString FilePath)
{
	CFile InFile;
	if(!InFile.Open(FilePath,CFile::modeRead))
		return FALSE;
    //CString StockSymbol;      //股票代码
	int Marker;               //隔离标记
	//POWER Power;
	int nEachChuQuanNo=0;
	char strStockCode[8]; 
	int nKind;
	//BYTE nKind;
    Split pSplit[80];
    int nFlag=0;
	InFile.Read(&nFlag,4);
    if(nFlag!=FILEID22)
	{
		AfxMessageBox("不是"+g_strCompanyName+"除权数据格式!");
		InFile.Close();
		return FALSE;
	}
	::ZeroMemory(pSplit,80*sizeof(Split));
	InFile.Seek(8,CFile::begin);
	BOOL bFirstTime=TRUE;
	//pDoc=CMainFrame::m_pDoc;
	while( InFile.Read(&Marker,4) == 4 )
	{
		if( Marker == 0xFFFFFFFF )
		{
			if(bFirstTime==TRUE)
                bFirstTime=FALSE;  
            else//	将上次pSplit写进内存；
	 	        CMainFrame::m_pDoc->m_sharesInformation.ImportChuQuanInfo(strStockCode,pSplit,nEachChuQuanNo,nKind);
			//	if(!pDoc->m_sharesInformation.ImportChuQuanInfo(strStockCode,pSplit,nEachChuQuanNo))
			//	{
		      //	return;
			  //}
				
			//清空PSPLIT；
			//读出除权数据；写进pSplit；
			::ZeroMemory(pSplit,80*sizeof(Split));
			nEachChuQuanNo=0;
            InFile.Read(strStockCode,8*sizeof(char));
			InFile.Read(&nKind,4);
            InFile.Seek(8,CFile::current);			
			//InFile.Read(&(pSplit[nEachChuQuanNo].Free),8); 
			InFile.Read(&(pSplit[nEachChuQuanNo].nFlags),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].nTime),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Give),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Allocate),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].AllocatePrice),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Bonus),4);
			nEachChuQuanNo++;
			}
		    else 
			{
				//指针后退4；写进pSplit；
			InFile.Seek(-4,CFile::current);			
			InFile.Seek(8,CFile::current);			
			//InFile.Read(&(pSplit[nEachChuQuanNo].Free),8); 
			InFile.Read(&(pSplit[nEachChuQuanNo].nFlags),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].nTime),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Give),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Allocate),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].AllocatePrice),4);
			InFile.Read(&(pSplit[nEachChuQuanNo].Bonus),4);
            nEachChuQuanNo++;
			}
		}
		//将最后一次pSplit写进内存；
		CMainFrame::m_pDoc->m_sharesInformation.ImportChuQuanInfo(strStockCode,pSplit,nEachChuQuanNo,nKind);  
       InFile.Close();
	   return TRUE;
}
