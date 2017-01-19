// WHDoc.cpp : implementation of the CWHDoc class
//

#include "stdafx.h"
#include "WH.h"

#include "WHDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWHDoc

IMPLEMENT_DYNCREATE(CWHDoc, CDocument)

BEGIN_MESSAGE_MAP(CWHDoc, CDocument)
	//{{AFX_MSG_MAP(CWHDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWHDoc construction/destruction

CWHDoc::CWHDoc()
{
	// TODO: add one-time construction code here

}

CWHDoc::~CWHDoc()
{
}

BOOL CWHDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWHDoc serialization

void CWHDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWHDoc diagnostics

#ifdef _DEBUG
void CWHDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWHDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWHDoc commands
