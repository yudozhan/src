// DialogCustomizeRanking.cpp : ????
//

#include "stdafx.h"
#include "WH.h"
#include "mainfrm.h"
//#include "GridCtrl.h"
#include "RankingGridCtrl.h"

#include "CTaiShanDoc.h"
#include "DialogCustomizeRanking.h"

#define ID_CGRID_CUSTOMIZE_RANKING_TIMER 2998
#define ID_CGRID_CUSTOMIZE_RANKING_DIALOG 2999

// CDialogCustomizeRanking ???

IMPLEMENT_DYNAMIC(CDialogCustomizeRanking, CDialog)

CDialogCustomizeRanking::CDialogCustomizeRanking(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCustomizeRanking::IDD, pParent)
{
	m_pGrid     =NULL;
    m_pMapData  = NULL;
}

CDialogCustomizeRanking::~CDialogCustomizeRanking()
{
}

void CDialogCustomizeRanking::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogCustomizeRanking, CDialog)
    	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialogCustomizeRanking

BOOL CDialogCustomizeRanking::OnInitDialog()
{
	CDialog::OnInitDialog();

 	DWORD dwStyle=0;
	dwStyle = WS_CHILD | WS_TABSTOP | WS_VISIBLE|WS_BORDER ;
	dwStyle &= ~WS_HSCROLL;
    dwStyle &= ~WS_VSCROLL;

    this->SetWindowText (_T("5分钟速涨排名") );
	this->CenterWindow();
    
	m_pDoc =((CMainFrame*)AfxGetMainWnd())->m_pDoc ;

    m_pGrid =new CRankingGridCtrl;  // CGridCtrl;
	m_pGrid->EnableTitleTips(FALSE);
	m_pGrid->Create(CRect(0,0,10,10),this,ID_CGRID_CUSTOMIZE_RANKING_DIALOG,dwStyle);
  
	m_pGrid->SetListMode(m_bListMode);
	m_pGrid->SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));
	m_pGrid->SetMainDocument(m_pDoc);

	InitGridColumnTitle();
	InitGridContent();
	int m_Width=0;
	int m_Height=0;
    m_pGrid->SetItemState(1,1, m_pGrid->GetItemState(1,1) | GVIS_READONLY);
	m_pGrid->SetRowHeight(0, int(1.0*m_pGrid->GetRowHeight(0)));
    m_pGrid->AutoSize();

    m_pGrid->SetColumnWidth(0,60);
    m_pGrid->SetColumnWidth(1,80);
    m_pGrid->SetColumnWidth(2,60);
    m_pGrid->SetColumnWidth(3,60);

    m_pGrid->MoveWindow(0,0,m_pGrid->GetVirtualWidth()+15,m_pGrid->GetVirtualHeight(), TRUE);

	m_pGrid->SetEditable(FALSE);
	m_pGrid->SetRowResize(FALSE);
	m_pGrid->SetColumnResize(FALSE);


    m_pGrid->SetFocus();
    m_pGrid->SetFocusRow(1); 
   	m_pGrid->SetMouseState(MOUSE_NOTHINGS);

    m_pGrid->SetGridLines(GVL_NONE);

    //- 检索出涨幅最大20支股票
    UpdateRanking();
    m_pGrid->SetPMdataPtr(m_pRankingData);

    m_pGrid->Invalidate();                            //更新显示

    // 调整錎ialog窗口大小，适配表格
    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);
    int dlogWidth = m_pGrid->GetVirtualWidth()+4;
    int dlogHeight= m_pGrid->GetVirtualHeight()+28;

    this->SetWindowPos( NULL, cx-dlogWidth-20, 50, dlogWidth,  dlogHeight, SWP_NOZORDER );

    //::SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE)^WS_EX_LAYERED); 
    //::SetLayeredWindowAttributes(this->GetSafeHwnd(), 0, 220, LWA_ALPHA); 

	SetTimer(ID_CGRID_CUSTOMIZE_RANKING_TIMER,1500,NULL);
    
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDialogCustomizeRanking::InitGridColumnTitle()
{
	CString str;
	//initialize column title
	str.Format("代 码");
	m_ColumnTitleArray.Add(str);
	str.Format("证券名称");
	m_ColumnTitleArray.Add(str);
	str.Format("涨 幅");
	m_ColumnTitleArray.Add(str);
    str.Format("速 涨");
	m_ColumnTitleArray.Add(str);

	//Get total column	
	m_nCols=m_ColumnTitleArray.GetSize(); //total column

}
int CDialogCustomizeRanking::InitGridCtrl()   	//初始化GridCtrl
{

	return 0;
}
BOOL CDialogCustomizeRanking::InitGridContent()  //初始化Grid
{
    m_pGrid->SetMainDocument(m_pDoc);
	m_nRows = MAX_RANKING_NUM+1;       // only list 20 line in this Diag
	m_nFixRows = 1;
	m_nFixCols = 4;

//xqk move the initalize to the construct function
	try {
 		m_pGrid->SetRowCount(m_nRows);
        m_pGrid->SetRealRow(m_nRows); 
		m_pGrid->SetColumnCount(m_nCols);
		m_pGrid->SetFixedRowCount(m_nFixRows);
		m_pGrid->SetFixedColumnCount(m_nFixCols);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return FALSE;
	}
   	m_pGrid->SetGridColor(m_pDoc->m_colorArray[18],m_pDoc->m_colorArray[17],m_pDoc->m_colorArray[16]);
    m_pGrid->SetFixedBkColor(m_pDoc->m_colorArray[18]);
    
	CFont Font;
	LOGFONT m_lf;
	memset(&m_lf,0,sizeof(LOGFONT));
	m_lf.lfHeight = -12;
	m_lf.lfWidth=0;
	m_lf.lfWeight=400;
	m_lf.lfCharSet=134;
	strcpy(m_lf.lfFaceName,"宋体");
	Font.CreateFontIndirect(&m_lf);

	m_pGrid->SetFont(&Font);
	Font.DeleteObject();
    
	//init cols title
    for(int i=0;i<m_ColumnTitleArray.GetSize();i++)
	{
		CString str=m_ColumnTitleArray.GetAt(i);
        m_pGrid->SetColumnInfo(i,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS,
				str.GetBuffer(0),i);
	}

 	return TRUE;
}

void CDialogCustomizeRanking::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDEvent==ID_CGRID_CUSTOMIZE_RANKING_TIMER)
	{
        //- 检索出涨幅最大20支股票
        UpdateRanking();
        m_pGrid->SetPMdataPtr(m_pRankingData);
        m_pGrid->Invalidate();                            //更新显示
	}

	CDialog::OnTimer(nIDEvent);
}


void CDialogCustomizeRanking::OnCancel() 
{
	// TODO: Add extra cleanup here
    LOGS("CDialogCustomizeRanking::KillTimer()\n");

	KillTimer(ID_CGRID_CUSTOMIZE_RANKING_TIMER);
	CDialog::OnCancel();
    DestroyWindow();	
}

BOOL CDialogCustomizeRanking::isExistInRankArray(CReportData * m_pbData)
{
    int i;

    for( i=0; i<MAX_RANKING_NUM; i++ )
    {
        if( m_pbData == m_pRankingData[i] )
            return TRUE;
    }
    return FALSE;
}
BOOL CDialogCustomizeRanking::needIgnore(CReportData * m_pbData)
{
    switch(m_pbData->kind)
    {
        case SHAG:
        case SZAG:
        case SZZXQY:
        case SZCYB: return FALSE;
        default: return TRUE;
    }
}


BOOL CDialogCustomizeRanking::needSwap(CReportData * m_pbData1, CReportData * m_pbData2)
{
    int rankMin = 5;
    short lastMin;
    float delta1, delta2;

    float temp=0;
    unsigned int mins=0;
    if( m_pbData1->lastclmin > rankMin )
        mins = m_pbData1->lastclmin-rankMin;
    
    if(m_pbData1->m_Kdata1[mins].Price !=0&&m_pbData1->nowp!=0)
        delta1 = 100 * (m_pbData1->nowp - m_pbData1->m_Kdata1[mins].Price )/ m_pbData1->m_Kdata1[mins].Price ;
    else
        delta1 = 0;

    mins=0;
    if( m_pbData2->lastclmin > rankMin )
        mins = m_pbData2->lastclmin-rankMin;
    
    if(m_pbData2->m_Kdata1[mins].Price !=0&&m_pbData2->nowp!=0)
        delta2 = 100 * (m_pbData2->nowp - m_pbData2->m_Kdata1[mins].Price )/ m_pbData2->m_Kdata1[mins].Price ;
    else
        delta2 = 0;

    if( delta2> delta1 )
        return TRUE;
    else
        return FALSE;

}
void CDialogCustomizeRanking::UpdateRanking() 
{
    int i,j;
    CReportData * tmp_pMdata;
    m_pMapData = m_pDoc->m_sharesInformation.GetCReportData();
    totalCount = m_pDoc->m_sharesInformation.GetCount();

    memset( m_pRankingData, 0, sizeof(m_pRankingData) );
    
    for( i=0; i<MAX_RANKING_NUM; i++ )
    {
        for( j=0; j<totalCount; j++ )
        {
            tmp_pMdata = m_pMapData+j;
            if( isExistInRankArray(tmp_pMdata) || needIgnore(tmp_pMdata) )
                continue;
            else
                break;
        }

        for( j=0; j<totalCount; j++ )
        {
            if( needIgnore(m_pMapData+j) )
                continue;
            if( isExistInRankArray(m_pMapData+j) )
                continue;
            if( needSwap( tmp_pMdata, m_pMapData+j ) )
                tmp_pMdata = m_pMapData+j;

        }
        m_pRankingData[i] = tmp_pMdata;
    }
}

 	 / /   ? ? ? ? :   O C X   ? ? ? ? ? 
