// CompanyInfoView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"

#include "CompanyInfoView.h"
#include "MainFrm.h"
#include "KEYBRODEANGEL.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCompanyInfoView

IMPLEMENT_DYNCREATE(CCompanyInfoView, CEditView)

CCompanyInfoView::CCompanyInfoView()
{
}

CCompanyInfoView::~CCompanyInfoView()
{
   //CMainFrame *m_pWnd;
   //m_pWnd = (CMainFrame *)(GetParent()->GetParent());	
   //m_pWnd.EnableF10 = true; 
   ((CMainFrame *)AfxGetMainWnd())->m_EnableF10 = 0;
}


BEGIN_MESSAGE_MAP(CCompanyInfoView, CEditView)
	//{{AFX_MSG_MAP(CCompanyInfoView)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCompanyInfoView drawing

void CCompanyInfoView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CCompanyInfoView diagnostics

#ifdef _DEBUG
void CCompanyInfoView::AssertValid() const
{
	CEditView::AssertValid();
}

void CCompanyInfoView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCompanyInfoView message handlers

int CCompanyInfoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO: Add your specialized creation code here
	/*LOGFONT m_lf;
	memset(&m_lf,0,sizeof(LOGFONT));
	m_lf.lfHeight = -16;
	m_lf.lfWidth=0;
	m_lf.lfWeight=400;
	m_lf.lfCharSet=134;
	strcpy(m_lf.lfFaceName,"宋体");
	VERIFY(m_Font.CreateFontIndirect(&m_lf));
	SetFont(&m_Font,TRUE); */
	
	CFont m_Font;
	LOGFONT m_lf;
	CFontDialog zjh;
	if(zjh.DoModal() == IDOK)
	{
	   zjh.GetCurrentFont(&m_lf);
       memset(&m_lf,0,sizeof(LOGFONT));
	   VERIFY(m_Font.CreateFontIndirect(&m_lf));
	   SetFont(&m_Font,TRUE); 
	}

	return 0;
}

void CCompanyInfoView::OnFilePrintPreview() 
{
    ///////////For Print Preview////////
	CView::OnFilePrintPreview();   
	//////// the CEditView class does not include the
	////// Print_preview function and we add the function 
	////// from the class CView/////////
}


BOOL CCompanyInfoView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
		if(pMsg->message==WM_CHAR&&((pMsg->wParam>='0'&&pMsg->wParam<='9')||(pMsg->wParam>='A'&&pMsg->wParam<='z')))
	{
			CTaiKeyBoardAngelDlg m_diakeybrode;
			if(pMsg->wParam>='a'&&pMsg->wParam<='z')
				pMsg->wParam-=32;
			char input=pMsg->wParam;
			m_diakeybrode.input=input;
			if(pMsg->wParam>='0'&&pMsg->wParam<='9')
				m_diakeybrode.ischar=false;
			else
				m_diakeybrode.ischar=true;

		m_diakeybrode.DoModal();
	    
		//pDoc->m_stkName= stock_name;
		CWnd * pMainWnd = AfxGetApp()->m_pMainWnd;
		CFrameWnd* pFrameWnd=((CFrameWnd*)pMainWnd)->GetActiveFrame();
		CTaiShanDoc* pDoc=(CTaiShanDoc*)pFrameWnd->GetActiveDocument();
		if(m_diakeybrode.isresultok == false || m_diakeybrode.isresultstock == false  )
		{
		 return TRUE;
		}
		pDoc->m_sharesSymbol = m_diakeybrode.result;
		((CMainFrame *)pMainWnd)->ReViewInfo();
		//pDoc->m_stkName = m_diakeybrode.
        //pDoc->ReadKLine(pDoc->m_sharesSymbol);
		//OnInitialUpdate();
		return TRUE;

	}
	return CEditView::PreTranslateMessage(pMsg);
}

void CCompanyInfoView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(nChar == VK_ESCAPE)
	{
	  GetParent()->SendMessage(WM_CLOSE);
	  return;
	}
	CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCompanyInfoView::OnInitialUpdate() 
{
	CEditView::OnInitialUpdate();
//	GetDocument()->SetTitle("上市公司信息");	
}
