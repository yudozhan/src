// alcIndex.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineFileHS.h"
#include "alcIndex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_USER_CALC  WM_USER+199


/////////////////////////////////////////////////////////////////////////////
// CCalcIndex dialog


CCalcIndex::CCalcIndex(CWnd* pParent /*=NULL*/)
	: CTaiScreenParent(CCalcIndex::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalcIndex)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bIsCalc=FALSE;
}

CCalcIndex::CCalcIndex(Index_Technique *pIndex,UINT nID,CWnd* pParent )   // standard constructor
    : CTaiScreenParent(CCalcIndex::IDD, pParent)
{
	memcpy(&m_pIndexTechnique,pIndex,sizeof(Index_Technique));
	m_nColumnId=nID;

}


void CCalcIndex::DoDataExchange(CDataExchange* pDX)
{
	CTaiScreenParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalcIndex)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalcIndex, CTaiScreenParent)
	//{{AFX_MSG_MAP(CCalcIndex)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_CALC,OnCalcIndex)
END_MESSAGE_MAP()

//////////////////////////////// /////////////////////////////////////////////
// CCalcIndex message handlers

BOOL CCalcIndex::OnInitDialog() 
{
	CTaiScreenParent::OnInitDialog();
	
	char temp[50];
	sprintf(temp,"%s %s",m_pIndexTechnique.sIndex_name,"����ָ��"); 
	this->SetWindowText(temp); 

	// TODO: Add extra initialization here
	this->CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
LRESULT CCalcIndex::OnCalcIndex(WPARAM wParam, LPARAM lParam)
{
	m_bIsCalc=TRUE;
	CMenu *pMenu=GetSystemMenu(FALSE);
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); 

	CalcIndexIndexData();
	m_bIsCalc=FALSE;
	pMenu->EnableMenuItem(SC_CLOSE,MF_BYCOMMAND|MF_ENABLED); 

	this->OnOK();
	return 1L;
}

void CCalcIndex::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CTaiScreenParent::OnShowWindow(bShow, nStatus);
	PostMessage(WM_USER_CALC,0,0); 
	// TODO: Add your message handler code here
}
void CCalcIndex::CalcIndexIndexData()
{
	MSG message;
	IndexDataInfo nIndexSave;
    memcpy(&nIndexSave,&m_pIndexTechnique,sizeof(IndexDataInfo));
	int nDays=GetCountValid(&nIndexSave,pDoc, 10);
	int m_totalCount=pDoc->m_sharesInformation.GetCount();
	long m_index=0;
    for(int i=0;i<STOCKTYPENUM;i++)
	{
		int temp=pDoc->m_sharesInformation.GetStockTypeCount(i);
		for(int j=0;j<temp;j++)
		{
			CReportData *Cdat;
			CString StockId ; 
			bool bSucc;
			Kline *pKline=NULL;
			CBuySellList l_BuySellList;
			pDoc->m_sharesInformation.GetStockItem(i,j,Cdat);
			if(Cdat==NULL)
				continue;
			StockId =Cdat->id ;

            int GetDays=ReadKLine(StockId,i, pKline, nDays, nLKind[nIndexSave.iDataType],&l_BuySellList, 48 ,TRUE);
		
			Cdat->Index[this->m_nColumnId-9000]=FormularComputeShares(StockId,i,GetDays,&nIndexSave,bSucc,pKline, &l_BuySellList );
            if(!bSucc)
			{
//				AfxMessageBox("error");
//                Cdat->Index[this->m_nColumnId-9000]=0;
			}
			if(pKline)
			{
			    delete pKline;        //�ͷ��ڴ�
				pKline=NULL;
			}
			CTaiKlineFileHS::RemoveHs(l_BuySellList);
	//************��ʾ��Ʊ������Ϣ**************************************************
			m_index++;
			int spos=100*m_index /m_totalCount   ;
			this->m_Progress.SetPos(spos);
	//**************************************************************
			if(PeekMessage(&message,NULL,0,0,PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
	}
}

void CCalcIndex::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bIsCalc)
	{
		AfxMessageBox("�����������㲻�ܽ����˳�����!");
		return;
	}
	CTaiScreenParent::OnClose();
}
void CCalcIndex::OnCancel()
{
	if(m_bIsCalc)
	{
		AfxMessageBox("�����������㲻�ܽ����˳�����!");
		return;
	}
	CDialog::OnCancel();
}
