// CTaiStockInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiStockInputDlg.h"

#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiStockInputDlg dialog


CTaiStockInputDlg::CTaiStockInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiStockInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaiStockInputDlg)
	m_strStockCode = _T("");
	m_strStockName = _T("");
	m_strStockPyjc = _T("");
	//}}AFX_DATA_INIT
	m_bStockType=TRUE;
	m_bEditFlag=FALSE;
}


void CTaiStockInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiStockInputDlg)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, IDC_SJLSTOCKCODE, m_strStockCode);
	DDV_MaxChars(pDX, m_strStockCode, 6);
	DDX_Text(pDX, IDC_SJLSTOCKNAME, m_strStockName);
	DDV_MaxChars(pDX, m_strStockName, 8);
	DDX_Text(pDX, IDC_SJLSTOCKPYJC, m_strStockPyjc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiStockInputDlg, CDialog)
	//{{AFX_MSG_MAP(CTaiStockInputDlg)
	ON_EN_CHANGE(IDC_SJLSTOCKCODE, OnChangeStockcode)
	ON_EN_CHANGE(IDC_SJLSTOCKNAME, OnChangeStockname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaiStockInputDlg message handlers

void CTaiStockInputDlg::OnChangeStockcode() 
{
    CReportData *Cdat;
	CString strCode;
	CTaiShanDoc* pDoc=GetDocument();
	GetDlgItem(IDC_SJLSTOCKCODE)->GetWindowText(strCode);
	
	//Shanghai
	if(!m_bEditFlag)
	{
		if(m_bStockType)
		{
			if( this->nMarketType == SH_MARKET_EX )
			{
				//char tmpCode[8];
				//strcmp(tmpCode,strCode);
				DWORD dwKind=CSharesInformation::GetStockKind(nMarketType,strCode.GetBuffer(0));
				if(pDoc->m_sharesInformation.Lookup(strCode.GetBuffer(0),Cdat,(int)dwKind) == 1 )
				{
					AfxMessageBox("��Ʊ�����ظ�,���������ã�",MB_ICONSTOP);
					GetDlgItem(IDC_SJLSTOCKCODE)->SetWindowText("");
					return;
				}
			}
		}
		else
		{
			if( this->nMarketType == SZ_MARKET_EX )
			{
				//char tmpCode[8];
				//strcmp(tmpCode,strCode);
				DWORD dwKind=CSharesInformation::GetStockKind(nMarketType,strCode.GetBuffer(0));
			//shenzhen
			
				if(  pDoc->m_sharesInformation.Lookup(strCode.GetBuffer(0),Cdat,(int)dwKind) == 1 )
				{
					AfxMessageBox("��Ʊ�����ظ�,���������ã�",MB_ICONSTOP);
					GetDlgItem(IDC_SJLSTOCKCODE)->SetWindowText("");
					return;
				}
			}
		}
	}
}

void CTaiStockInputDlg::OnChangeStockname() 
{
	CTaiShanDoc* pDoc=GetDocument();
	CString strName;
	GetDlgItem(IDC_SJLSTOCKNAME)->GetWindowText(strName);
	char *pName=NULL;
	char strHypy[10];
	pName=strName.GetBuffer(strName.GetLength());
	pDoc->StockNameConvert(pName,strHypy);
	GetDlgItem(IDC_SJLSTOCKPYJC)->SetWindowText(strHypy);
}

void CTaiStockInputDlg::SetStockType(BOOL bShangHai)
{
	m_bStockType=bShangHai;
}

BOOL CTaiStockInputDlg::GetStockType()
{
	return m_bStockType;
}

BOOL CTaiStockInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	POINT pt;
	::GetCursorPos(&pt);
	CRect rect;
	GetClientRect(rect);
	SetWindowPos(NULL,pt.x,pt.y,rect.Width(),rect.Height()+20,SWP_SHOWWINDOW);
	if(m_bEditFlag)
	{
		this->SetWindowText("�޸Ĺ�Ʊ��Ϣ");
        GetDlgItem(IDC_SJLSTOCKCODE)->SetWindowText(m_StockSymbol);
        GetDlgItem(IDC_SJLSTOCKCODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SJLSTOCKNAME)->SetWindowText(m_StockName);
		GetDlgItem(IDC_SJLSTOCKPYJC)->SetWindowText(m_StockPyjc);
	}
	else
	 GetDlgItem(IDC_SJLSTOCKCODE)->EnableWindow(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTaiStockInputDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	if(m_strStockCode.IsEmpty())
	{
		MessageBox("��Ʊ���벻��Ϊ��,������...��'ȡ��'?","����",MB_OK|MB_ICONQUESTION);
		GotoDlgCtrl(GetDlgItem(IDC_SJLSTOCKCODE));
		return;
	}
	if(m_bStockType)
	{
		if(m_strStockCode.GetLength()!=6)
		{
			MessageBox("�Ϻ���Ʊ�������Ϊ��λ����,����������...?","����",MB_OK|MB_ICONQUESTION);
			GotoDlgCtrl(GetDlgItem(IDC_SJLSTOCKCODE));
			return;
		}
	}
	else
	{
		if(m_strStockCode.GetLength()!=6)
		{
			MessageBox("���ڹ�Ʊ�������Ϊ��λ����,����������...?","����",MB_OK|MB_ICONQUESTION);
			GotoDlgCtrl(GetDlgItem(IDC_SJLSTOCKCODE));
			return;
		}
	}

	if(m_strStockName.IsEmpty())
	{
		MessageBox("��Ʊ���Ʋ���Ϊ��,������...��'ȡ��'","����",MB_OK|MB_ICONQUESTION);
		GotoDlgCtrl(GetDlgItem(IDC_SJLSTOCKNAME));
		return;
	}
	CDialog::OnOK();
}

CTaiShanDoc * CTaiStockInputDlg::GetDocument()
{
	CWnd* pMainWnd=AfxGetApp()->m_pMainWnd;
	CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
	CTaiShanDoc* pDoc=(CTaiShanDoc*)pFrameWnd->GetActiveDocument();
	return pDoc;
}

void CTaiStockInputDlg::SetEditFlag(BOOL bEdit)
{
   m_bEditFlag=bEdit;
}

void CTaiStockInputDlg::SetStockInfo(CString Code, CString Name, CString Pyjc)
{
   m_StockSymbol=Code;
   m_StockName=Name;
   m_StockPyjc=Pyjc;
}
