// CTaiKlineDlgBaseInfo.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "CTaiKlineDlgBaseInfo.h"
#include "CTaiKlineDialogChangKLine.h"
#include "MainFrm.h"
#include "CTaiShanDoc.h"
#include "ChildFrm.h"
#include "CTaiShanKlineShowView.h"
#include "CTaiKlineDoKline.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgBaseInfo dialog


CTaiKlineDlgBaseInfo::CTaiKlineDlgBaseInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CTaiKlineDlgBaseInfo::IDD, pParent)
{
	
	//{{AFX_DATA_INIT(CTaiKlineDlgBaseInfo)
	m_content = _T("");
	m_brief = _T("");
	m_symbol = _T("");
	//}}AFX_DATA_INIT
}


void CTaiKlineDlgBaseInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaiKlineDlgBaseInfo)
	DDX_Control(pDX, IDC_EDIT_BRIEF, m_cBrief);
	DDX_Control(pDX, IDC_EDIT_CONTENT_TOTAL, m_cContent);
	DDX_Text(pDX, IDC_EDIT_CONTENT_TOTAL, m_content);
	DDX_Text(pDX, IDC_EDIT_BRIEF, m_brief);
	DDX_Text(pDX, IDC_EDIT_CONTENT_SYMBOL, m_symbol);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTaiKlineDlgBaseInfo, CDialog)
	//{{AFX_MSG_MAP(CTaiKlineDlgBaseInfo)
	ON_EN_CHANGE(IDC_EDIT_BRIEF, OnChangeEditBrief)
	ON_EN_CHANGE(IDC_EDIT_CONTENT_TOTAL, OnChangeEditContentTotal)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//
/////////////////////////////////////////////////////////////////////////////
// CTaiKlineDlgBaseInfo message handlers

void CTaiKlineDlgBaseInfo::OnChangeEditBrief() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	CString s=m_brief;
	UpdateData(TRUE);
	if(m_brief.GetLength()>=60)
	{
		m_brief=s;
		UpdateData(FALSE);
		AfxMessageBox("���ֻ������60���ַ�",MB_ICONSTOP);
	}
	
	// TODO: Add your control notification handler code here
	
}

void CTaiKlineDlgBaseInfo::OnChangeEditContentTotal() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString s=m_content;
	UpdateData(TRUE);
	if(m_content.GetLength()>=200)
	{
		m_content=s;
		UpdateData(FALSE);
		AfxMessageBox("���ֻ������200���ַ�",MB_ICONSTOP);
	}
	
}

void CTaiKlineDlgBaseInfo::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CTaiKlineDlgBaseInfo::OnOK() 
{
	// TODO: Add extra validation here
	CTaiShanDoc*	pDoc=CMainFrame::m_pDoc ;//(CTaiShanDoc*)pchildfram->GetActiveDocument();

	UpdateData(TRUE);

	//8.3
	m_DateTime.GetTime(m_time_ole);
	{
		bool bAddNew = true;
		try
		{
			if(!pView->m_bs.IsOpen())
			{
				pView->m_bs.Open();
			}

			if(pView->m_bs.GetRecordCount()>0)
			{
				pView->m_bs.MoveFirst ();
				while(!pView->m_bs.IsEOF() )
				{
					//--	����ʱ��
					int nYear2=pView->m_bs.m_time .GetYear ();
					int nMonth2=pView->m_bs.m_time .GetMonth  ();
					int nDay2=pView->m_bs.m_time .GetDay ();
					if(m_time_ole.GetYear()==nYear2&&
						m_time_ole.GetMonth()==nMonth2&&
						m_time_ole.GetDay()==nDay2)
					{
						bAddNew=false;

						if(m_brief=="")
						{
							AfxMessageBox("�˼�¼Ϊ�գ�����ɾ����",MB_ICONSTOP);
							pView->m_bs .Delete();
							if(!pView->m_bs.IsBOF())
								pView->m_bs .MoveFirst();
							break;
						}
						else
						{
						pView->m_bs .Edit();
						pView->m_bs .m_time=m_time_ole;
						pView->m_bs .m_brief=m_brief;
						pView->m_bs .m_symbol=pView->m_sharesSymbol ;
						pView->m_bs .m_content=m_content ;

	//					pView->m_bs .SetFieldDirty(NULL,TRUE);
						pView->m_bs .Update();
						break;
						}
					}
					pView->m_bs.MoveNext ();
				}
			}

			if(bAddNew==true && m_brief != "")
			{
				pView->m_bs .AddNew();

				pView->m_bs .m_time=m_time_ole;
				pView->m_bs .m_brief=m_brief;
				pView->m_bs .m_symbol=pView->m_sharesSymbol ;
				pView->m_bs .m_content=m_content ;

	//			pView->m_bs .SetFieldDirty(NULL,TRUE);
				pView->m_bs .Update();
			}
		}
		catch (CDaoException* e)
		{
			e->Delete();
		}  
	}
	CDialog::OnOK();
}

BOOL CTaiKlineDlgBaseInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CTaiShanDoc*	pDoc=CMainFrame::m_pDoc ;//(CTaiShanDoc*)pchildfram->GetActiveDocument();
//	CTaiShanKlineShowView*	pView=( CTaiShanKlineShowView* )(((CMainFrame*)(AfxGetApp()->m_pMainWnd))->pVwbaseView);
	pView = (CTaiShanKlineShowView* )(((CTaiKlineDialogChangKLine*)GetParent())->pView );
	ASSERT(pView!=NULL);

	VARIANT vart;
	vart=((CTaiKlineDialogChangKLine*)GetParent())->m_dtPicker.GetYear();
	int nYear=vart.lVal ;
	vart=((CTaiKlineDialogChangKLine*)GetParent())->m_dtPicker.GetMonth();
	int nMon=vart.lVal ;
	vart=((CTaiKlineDialogChangKLine*)GetParent())->m_dtPicker.GetDay();
	int nDay=vart.lVal ;
	COleDateTime tmOle(nYear,nMon,nDay,8,0,0);
	m_time_ole=tmOle;
	
	CWnd *pWnd = GetDlgItem(IDC_EDIT_TM);
	CRect rc;
	if(!pWnd) return TRUE;
	pWnd->GetWindowRect(rc);
	pWnd->DestroyWindow(); //pWnd was just a placeholder;
	ScreenToClient(rc);
	m_DateTime.Create(WS_CHILD|WS_VISIBLE|DTS_SHORTDATEFORMAT,rc,this,200200);
	// TODO: Add extra initialization here
	m_DateTime.SetFormat("yyy'/'MM'/'dd'" );

	CTime tm1(1980,1,1,8,0,0,0);
	CTime tm2(2036,1,1,8,0,0,0);
	m_DateTime.SetRange( &tm1,&tm2 );
//	m_time_ole=pView->m_bs .m_time;

	m_DateTime.SetTime(m_time_ole);
	
	m_symbol=pView->m_sharesSymbol ;

	try
	{
	if(!pView->m_bs.IsOpen())
		pView->m_bs.Open();

	pView->m_bs.MoveFirst ();
	while(!pView->m_bs.IsEOF() )
	{
		//--	����ʱ��
		int nYear2=pView->m_bs.m_time .GetYear ();
		int nMonth2=pView->m_bs.m_time .GetMonth  ();
		int nDay2=pView->m_bs.m_time .GetDay ();
		if(m_time_ole.GetYear()==nYear2&&
			m_time_ole.GetMonth()==nMonth2&&
			m_time_ole.GetDay()==nDay2)
		{
			m_brief=pView->m_bs .m_brief;
			m_content=pView->m_bs .m_content ;
			m_symbol=pView->m_bs .m_symbol ;
			UpdateData(FALSE);
			break;
		}
		pView->m_bs.MoveNext ();
	}
	}
	catch (CDaoException* e)
	{
		e->Delete();
	}
	UpdateData(FALSE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CTaiKlineDlgBaseInfo::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
//	HtmlHelp(m_hWnd,"stock.chm",HH_HELP_CONTEXT,CTaiKlineDlgBaseInfo::IDD);
//	return CDialog::OnHelpInfo(pHelpInfo);
	DoHtmlHelp(this);return TRUE;
}
