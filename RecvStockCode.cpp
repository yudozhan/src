// RecvStockCode.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "cellRange.h"
#include "CTaiShanReportView.h"
#include "Mainfrm.h"
#include "RecvStockCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecvStockCode dialog


CRecvStockCode::CRecvStockCode(CWnd* pParent /*=NULL*/)
	: CDialog(CRecvStockCode::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecvStockCode)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	this->m_pMainFrm=(CMainFrame *)pParent;
	m_SocketID.hWnd=NULL;
    m_SocketID.iFlag=-1;
}


void CRecvStockCode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecvStockCode)
	DDX_Control(pDX, IDC_PROGRESS1, m_MyProgressCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRecvStockCode, CDialog)
	//{{AFX_MSG_MAP(CRecvStockCode)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_RECVSTOCKCODE,OnRecvStockCode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecvStockCode message handlers

BOOL CRecvStockCode::OnInitDialog() 
{
	CDialog::OnInitDialog();
#ifdef WIDE_NET_VERSION
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
LRESULT CRecvStockCode::OnRecvStockCode(WPARAM wParam, LPARAM lParam)
{
#ifdef WIDE_NET_VERSION
    RCV_STKLABEL_STRUCTEx *pStkLabel=(RCV_STKLABEL_STRUCTEx *)wParam;
	int nStkLabelCounts=(int)lParam;
	CWnd *pWnd=this->GetDlgItem(IDC_RECV_INFO);
	pWnd->SetWindowText("���±��ش�����ձ�!");

    pWnd=this->GetDlgItem(IDCANCEL);
	pWnd->EnableWindow(FALSE);
	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 

	pWnd=this->GetDlgItem(IDC_STOCKCODE);
	CTaiShanDoc *pDoc;
    pDoc=this->m_pMainFrm->m_pDoc;
	pDoc->m_sharesInformation.ClearAllRealTimeMarketData();
	pDoc->RefreshKeyBoardAngel(FALSE);
	MSG msg;
    this->m_MyProgressCtrl.SetRange(0,nStkLabelCounts);
	this->m_MyProgressCtrl.SetPos(0);
	for(int i=0;i<nStkLabelCounts;i++)
	{
		CReportData *pCdat;
		int nKind=pDoc->m_sharesInformation.GetStockKind(pStkLabel[i].m_wMarket,pStkLabel[i].m_szLabel);
		if (pDoc->m_sharesInformation.Lookup(pStkLabel[i].m_szLabel,pCdat,nKind) != TRUE)     //���ù�Ʊ�Ƿ��Ѽ�¼���ڴ�ģ����
		{
			if(strlen(pStkLabel[i].m_szLabel)==6||strlen(pStkLabel[i].m_szLabel)==4)
			{
				if(nKind>=0&&nKind<=10)
				{
					if(!pDoc->m_sharesInformation.InsertItem(pStkLabel[i].m_szLabel,pCdat,nKind))
						continue;
					strcpy(pCdat->name ,pStkLabel[i].m_szName);
					strcpy(pCdat->id ,pStkLabel[i].m_szLabel );
                    pCdat->kind=nKind;
					strcpy(pCdat->Gppyjc ,pStkLabel[i].m_szPingYing );
					pDoc->AddStockToKeyboard(pCdat->id,pCdat->kind,pCdat->name,pCdat->Gppyjc);
					BASEINFO *m_pStockBase;
					if(pDoc->m_sharesInformation.LookupBase(pCdat->id,pCdat->kind,m_pStockBase))
					{
					   pCdat->pBaseInfo=m_pStockBase;
					}
				}
			}
		} 
	//************��ʾ��Ʊ������Ϣ**************************************************
		CString m_zqdm;
 		m_zqdm.Format ("%s(%s)",pCdat->name ,pCdat->id);
		pWnd->SetWindowText(m_zqdm);
        this->m_MyProgressCtrl.SetPos(i);
	//**************************************************************
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	pDoc->InitChooseAndStockType();
	pDoc->m_pReportView->ChangeToPage(0);
#endif
	return 1L;
}

void CRecvStockCode::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CRecvStockCode::OnCancel() 
{
	// TODO: Add extra cleanup here
#ifdef WIDE_NET_VERSION
	if(m_SocketID.iFlag!=-1)
	   m_pMainFrm->m_pClientTransmitManageWnd->ReleaseID(this->m_SocketID);
#endif
	CDialog::OnCancel();
}
