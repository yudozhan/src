// CTaiKlineExportExecel.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiShanDoc.h"
#include "MainFrm.h"
#include "CTaiKlineExportExecel.h"
#include "CTaiShanKlineShowView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineExportExecel dialog


CTaiKlineExportExecel::CTaiKlineExportExecel(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineExportExecel::IDD, pParent)
{

	pView = (CTaiShanKlineShowView*)pParent ;
	m_nFiguer = 0;
	ASSERT(pView!=NULL);
	//{{AFX_DATA_INIT(CTaiKlineExportExecel)
	m_bShowExcel = FALSE;
	//}}AFX_DATA_INIT
}


void CTaiKlineExportExecel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineExportExecel)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Check(pDX, IDC_CHECK1, m_bShowExcel);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_gridDataOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineExportExecel, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineExportExecel)
	ON_BN_CLICKED(IDOK, OnExport)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineExportExecel message handlers

BOOL CTaiKlineExportExecel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	pView->OutDataExcel (&(this->m_gridDataOut) ,m_nFiguer);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaiKlineExportExecel::OnExport() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	//write file
	CLongString longStr;

	int nPerLine=0,nOutLine=0;
	nPerLine = this->m_gridDataOut .GetRows();
	nOutLine = this->m_gridDataOut .GetCols();

	int i,j;
	for(j=0;j<nPerLine;j++)
	{
		for(i =0;i<nOutLine;i++)
		{
			CString s ;
			s = m_gridDataOut.GetTextMatrix(j,i);
			if(i==nOutLine-1)
				s+="\r\n";
			else
				s+="\t";
			longStr + s;
		}
	}

    CString     defaultname="outputData.xls";

	CString sArray="*.xls||";        //指标公式   //条件选股公式 //K线模式公式

	CFileDialog  m_filedia(FALSE,0,defaultname,OFN_CREATEPROMPT|OFN_HIDEREADONLY|OFN_NOCHANGEDIR,sArray);
	m_filedia.m_ofn .lpstrInitialDir = CMainFrame::m_pDoc ->m_CurrentWorkDirectory;
	                                    //将当前目录设为对话框的缺省目录
	CString filename="";
	if(m_filedia.DoModal()==IDOK)
	{
		filename=m_filedia.GetPathName();

		CFile   file;
		if(file.Open(filename,CFile::modeRead)!=0)
		{
			if(AfxMessageBox(filename+"文件已存在，要覆盖吗？",MB_YESNO|MB_ICONQUESTION)==IDNO)
			{
				file.Close();
				//::SetCurrentDirectory (CMainFrame::m_pDoc ->m_CurrentWorkDirectory);
				return;
			}
			file.Close();
		}

		if(file.Open(filename,CFile::modeWrite|CFile::modeCreate))
		{
			int n = longStr.GetLength ()+1;
			file.Write(longStr.m_pData ,n);
			file.Close();
		}
		else
		{
			AfxMessageBox("文件正在使用，请首先关闭文件！");
			return;
		}

		//execute execel
		if(m_bShowExcel == TRUE)
		{
			  ShellExecute(
				NULL, 
				"open",
				filename,//"http://www.e-stock2000.com/",  
				NULL, 
				NULL,
				SW_SHOW);
		}
	}
	
	//::SetCurrentDirectory (CMainFrame::m_pDoc ->m_CurrentWorkDirectory);


}

BOOL CTaiKlineExportExecel::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,CTaiKlineExportExecel::IDD);
//	return CDialog::OnHelpInfo(pHelpInfo);
	DoHtmlHelp(this);return TRUE;
}
