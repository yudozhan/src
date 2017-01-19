// WHView.cpp : implementation of the CWHView class
//

#include "stdafx.h"
#include "WH.h"

#include "WHDoc.h"
#include "WHView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWHView

IMPLEMENT_DYNCREATE(CWHView, CView)

BEGIN_MESSAGE_MAP(CWHView, CView)
	//{{AFX_MSG_MAP(CWHView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWHView construction/destruction

CWHView::CWHView()
{
	// TODO: add construction code here

}

CWHView::~CWHView()
{
}

BOOL CWHView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWHView drawing

void CWHView::OnDraw(CDC* pDC)
{
	CWHDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CWHView printing

BOOL CWHView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWHView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWHView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CWHView diagnostics

#ifdef _DEBUG
void CWHView::AssertValid() const
{
	CView::AssertValid();
}

void CWHView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWHDoc* CWHView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWHDoc)));
	return (CWHDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWHView message handlers
