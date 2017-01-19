// OutInPower.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "OutInPower.h"

#include "MainFrm.h"
#include "CTaiShanDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiChuQuanOutInDlg dialog

CTaiChuQuanOutInDlg::CTaiChuQuanOutInDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiChuQuanOutInDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiChuQuanOutInDlg)
	m_OutFile = _T("c:\\dest.pwr");
	m_InFile = _T("c:\\dest.pwr");
	m_FormName = _T("׼�������������Ȩ���ݣ�");
	//}}AFX_DATA_INIT
}


void CTaiChuQuanOutInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiChuQuanOutInDlg)
	DDX_Control(pDX, IDC_COMBO1, m_PowerForm);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Text(pDX, IDC_OUTFILE, m_OutFile);
	DDX_Text(pDX, IDC_INFILE, m_InFile);
	DDX_Text(pDX, IDC_STATIC_NAME, m_FormName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTaiChuQuanOutInDlg, CDialog)
	//{{AFX_MSG_MAP(CTaiChuQuanOutInDlg)
	ON_BN_CLICKED(IDC_OUT, OnOut)
	ON_BN_CLICKED(IDC_IN, OnIn)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_READ, OnRead)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiChuQuanOutInDlg message handlers

BOOL CTaiChuQuanOutInDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_PowerForm.InsertString(0,""+g_strCompanyName+"��Ȩ���ݸ�ʽ");
	m_PowerForm.InsertString(1,"�����ҳ�Ȩ���ݸ�ʽ");
	m_PowerForm.SetCurSel(0);
	m_Progress.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaiChuQuanOutInDlg::OnSelchangeCombo1() 
{
	switch( m_PowerForm.GetCurSel() )
	{
		case 0:
			m_InFile="C:\\dest.pwr";
			m_FormName="������"+g_strCompanyName+"��ʽ�����ݣ�";
			UpdateData(FALSE);
			break;
		case 1:
			m_InFile="C:\\power.pwr";
			m_FormName="�����Ƿ����Ҹ�ʽ�����ݣ�";
			UpdateData(FALSE);
			break;
	}
}

//���ܣ��������ļ��еĳ�Ȩ���ݵ�����һ���ļ���, �ļ����ƴӶԻ����ж���m_OutFile
//      �ļ���1-4���ֽڴ���ļ���ʶ��5-8���ֽڴ�ŵ����Ĺ�Ʊ����
//      ÿֻ��Ʊ�ĳ�Ȩ��Ϣ������FF FF FF FF ��-1��ͷ��
//      �������δ��ÿ�γ�Ȩ����Ϣ��32���ֽڡ�
void CTaiChuQuanOutInDlg::OnOut() 
{
	CTaiShanDoc * pDoc=GetDocument();
	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);

	UpdateData(TRUE);
	CFile OutFile;
	if(!OutFile.Open(m_OutFile,CFile::modeCreate|CFile::modeWrite,NULL))
	{
		MessageBox("�ǳ���Ǹ,������Ȩ��Ϣʧ��","����",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	int nHeaderFlag=-1;
	int nTotalCount=0;
	POWER Power;
	int nSplitMask=-1;
	//
	//first initialize the file header
	//
	OutFile.Write(&nHeaderFlag,sizeof(int));
	OutFile.Write(&nTotalCount,sizeof(int));
	
	//
	//Get all stock Chuquan info and write to the outfile
	//
	m_Progress.ShowWindow(SW_SHOW);
	m_Progress.SetRange32(0,pDoc->m_sharesInformation.GetCount());
	for(int nStockType=0;nStockType<8;nStockType++)
	{
		for(int i=0;i<pDoc->m_sharesInformation.GetStockTypeCount(nStockType);i++)
		{
			m_Progress.StepIt();
			CReportData *pDat1=NULL;
			pDoc->m_sharesInformation.GetStockItem(nStockType,i,pDat1);
			if(pDat1)
			{
				if(pDat1->pBaseInfo)
				{
					if(pDat1->pBaseInfo->NumSplit>0)
					{
						nTotalCount++;
						OutFile.Write(&nSplitMask,sizeof(int));
						OutFile.Write(&(pDat1->id),8*sizeof(char));
						for(int j=0;j<pDat1->pBaseInfo->NumSplit;j++)
						{
							memcpy(&Power,&(pDat1->pBaseInfo->m_Split[j]),sizeof(POWER));
							OutFile.Write(&Power,sizeof(POWER));
						}
					}
				}
			}
		}
	}
	//�ļ���5-8�ֽ�Ϊ������Ʊ��������
	OutFile.Seek(4, CFile::begin);
	OutFile.Write(&nTotalCount, sizeof(int));
	OutFile.Close();
	m_Progress.ShowWindow(SW_HIDE);
	CString str;
	str.Format("%d", nTotalCount );
	m_FormName="�������, ������"+str+"ֻ��Ʊ��"+"\nλ��"+m_OutFile;
	UpdateData(FALSE);
}

//���ܣ������ǵ����������ļ������ߴӷ����ҵ����������ļ������Ȩ����
void CTaiChuQuanOutInDlg::OnIn() 
{
	CTaiShanDoc * pDoc=GetDocument();
	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);
	UpdateData(TRUE);

	FXJPOWER power_fxj;
	POWER Power;
	char strStockCode[8]; 
	int nSplitMask=0;
	int nTotalCount=0;
	int nEachChuQuanNo=0;

	CFile InFile;
	if(!(InFile.Open(m_InFile,CFile::modeRead,NULL)))
	{
		MessageBox("�����Ȩ�ļ����ɶ�","����",MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	m_FormName="���������Ȩ����...";
	m_Progress.ShowWindow(SW_SHOW);
    GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_FormName);
	Split pSplit[80];
	if( m_PowerForm.GetCurSel() == 0 )
	{
		InFile.Seek(sizeof(int),CFile::begin);
		InFile.Read(&nTotalCount,sizeof(int));

		m_Progress.SetRange(1,nTotalCount);
		for(int i=0;i<nTotalCount;i++)
		{
			nEachChuQuanNo=0;
			::ZeroMemory(pSplit,80*sizeof(Split));
			BOOL bSecondRead=FALSE;
			while(InFile.Read(&nSplitMask,4)==4)
			{
				if(nSplitMask==-1)
				{
					if(bSecondRead)
					{
						InFile.Seek(-4,CFile::current);
						break;
					}
					else
					{
						InFile.Read(strStockCode,8*sizeof(char));
						bSecondRead=TRUE;
					}
				}
				else
				{
					InFile.Seek(-4,CFile::current);
					InFile.Read(&(pSplit[nEachChuQuanNo]),sizeof(POWER));
					nEachChuQuanNo++;
				}
			}
			if(nEachChuQuanNo>0)
			{
				if(!pDoc->m_sharesInformation.ImportChuQuanInfo(strStockCode,pSplit,nEachChuQuanNo))
				{
					MessageBox("д��"+g_strCompanyName+"��Ȩ��Ϣ��ʧ��","����",MB_OK|MB_ICONEXCLAMATION);
					return;
				}
			}
			m_Progress.StepIt();
		}
	}

	//�ӷ����ҵĳ�Ȩ�����ļ�����
	else 
	{
		if( m_PowerForm.GetCurSel() == 1 )
		{
			CString StockCode;
			InFile.Seek(8,CFile::begin);
			nEachChuQuanNo=0;
			m_Progress.SetRange32(0,InFile.GetLength()/(sizeof(FXJPOWER)+sizeof(int)));
			while(InFile.Read(&nSplitMask,4) == 4)
			{
				if( nSplitMask == 0xFFFFFFFF ) 
				{
					if(nEachChuQuanNo>0)
					{
						if(!pDoc->m_sharesInformation.ImportChuQuanInfo(StockCode,pSplit,nEachChuQuanNo))
						{
							MessageBox("д��"+g_strCompanyName+"��Ȩ��Ϣ��ʧ��","����",MB_OK|MB_ICONEXCLAMATION);
							return;
						}
						m_Progress.StepIt();
					}
					nEachChuQuanNo=0;
					InFile.Read(StockCode.GetBuffer(12),12);
					InFile.Seek(4,CFile::current);         //????
					StockCode.ReleaseBuffer();
					if(StockCode.GetLength() == 8 )
						StockCode=StockCode.Right(6);
					else if(StockCode.GetLength() == 6)
						StockCode=StockCode.Right(4);
				}
				else
				{
					memset(&Power,0,sizeof(POWER));
					InFile.Seek(-4,CFile::current);
					InFile.Read(&power_fxj, sizeof(FXJPOWER));
					Power.nTime=power_fxj.nTime;
					Power.fGive=power_fxj.fGive;
					Power.fAllocate=power_fxj.fAlloc;
					Power.fAllocatePrice=power_fxj.fAllocPrice;
					Power.fDividend=power_fxj.fDividend;
					memcpy(&pSplit[nEachChuQuanNo],&Power,sizeof(POWER));
					nEachChuQuanNo++;
				}
			}
			if(nEachChuQuanNo>0)
			{
				if(!pDoc->m_sharesInformation.ImportChuQuanInfo(StockCode,pSplit,nEachChuQuanNo))
				{
					MessageBox("д��"+g_strCompanyName+"��Ȩ��Ϣ��ʧ��","����",MB_OK|MB_ICONEXCLAMATION);
					return;
				}
				m_Progress.StepIt();
			}
		}
	}
	InFile.Close();
	m_Progress.ShowWindow(SW_HIDE);
	m_FormName="�������!";
	UpdateData(FALSE);	
}

void CTaiChuQuanOutInDlg::OnSave() 
{
	CString DefExt="*.pwr";
	CString FileName="dest.pwr";
	CString Filter="power file (*.pwr)|*.PWR||";
	CFileDialog bSaveFileDialog(FALSE,(LPCSTR)DefExt,(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,(LPCSTR)Filter,this);
#ifdef NET_CLIENT
	bSaveFileDialog.m_ofn .lpstrInitialDir = CMainFrame::m_taiShanDoc ->m_sPrivatePath ;
#else
	bSaveFileDialog.m_ofn .lpstrInitialDir = CMainFrame::m_taiShanDoc ->m_CurrentWorkDirectory;
#endif
	if( bSaveFileDialog.DoModal() ==IDOK )
		m_OutFile=bSaveFileDialog.GetPathName();

	UpdateData(FALSE);
	CTaiShanDoc * pDoc=GetDocument();
	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);
}

void CTaiChuQuanOutInDlg::OnRead() 
{
	CString DefExt="*.pwr";
	CString FileName="dest.pwr";
	CString Filter="power file (*.pwr)|*.PWR||";
	CFileDialog bOpenFileDialog(TRUE,(LPCSTR)DefExt,(LPCTSTR)FileName,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,(LPCSTR)Filter,this);
#ifdef NET_CLIENT
	bOpenFileDialog.m_ofn .lpstrInitialDir = CMainFrame::m_taiShanDoc ->m_sPrivatePath ;
#else
	bOpenFileDialog.m_ofn .lpstrInitialDir = CMainFrame::m_taiShanDoc ->m_CurrentWorkDirectory;
#endif
	if( bOpenFileDialog.DoModal() ==IDOK )
		m_InFile=bOpenFileDialog.GetPathName();

	UpdateData(FALSE);
	CTaiShanDoc * pDoc=GetDocument();
	::SetCurrentDirectory(pDoc->m_CurrentWorkDirectory);
}

CTaiShanDoc * CTaiChuQuanOutInDlg::GetDocument()
{
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	CTaiShanDoc* pDoc=(CTaiShanDoc*)pFrameWnd->GetActiveDocument();
	ASSERT_VALID(pDoc);
	return pDoc;
}
