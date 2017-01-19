// ParamEdit.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "ParamEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParamEdit

CParamEdit::CParamEdit()
{
//	m_textPre="";
}

CParamEdit::CParamEdit(CListCtrl *list,int Item)
{
	m_bTestMax = true;
  m_plist = list;
  nItem = Item;
  char tmp[30];
  m_plist->GetItemText(nItem,1,tmp,30);
  m_textPre = tmp;

  bEscapeKey=FALSE;
//  list->GetItemText(nItem,2,tmp,30);
//  nMin=atof(tmp);
//  list->GetItemText(nItem,3,tmp,30);
//  nMax=atof(tmp);



  //GetWindowText(m_str);
}

CParamEdit::~CParamEdit()
{
}


BEGIN_MESSAGE_MAP(CParamEdit, CFloatEdit)
	//{{AFX_MSG_MAP(CParamEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParamEdit message handlers
int CParamEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFloatEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	this->SetLimitText(10);
	return 0;
}

BOOL CParamEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->wParam == VK_RETURN)
	{
		m_plist->SetFocus ();
/*		CString temp;
	    this->GetWindowText(temp);
		int tmp=atol(temp.GetBuffer(0));
		if(!CheckNumber(temp))
			tmp=-9999;
		if(tmp<nMin||tmp>nMax)
        {
			AfxMessageBox("输入数据不正确!");
			PostMessage(WM_CLOSE, 0, 0);
			return TRUE;
		}
		PostMessage(WM_CLOSE, 0, 0);*/
		return TRUE;
	}
	else if (pMsg->wParam == VK_ESCAPE)
	{
		PostMessage(WM_CLOSE, 0, 0);
		return bEscapeKey = TRUE;
	}
	
	return CFloatEdit::PreTranslateMessage(pMsg);
}

void CParamEdit::InsertItem()
{
  	   CString temp;
	   this->GetWindowText(temp);
	   if(m_plist == 0)
		   return;
	   if(m_plist && temp.GetLength())
	   {
		   m_plist->SetItemText(nItem,1,temp);
		   m_plist->Invalidate();
		   m_plist->SetFocus();
	   }
}
void CParamEdit::PostNcDestroy() 
{
	CFloatEdit::PostNcDestroy();
	delete this;
}

void CParamEdit::OnKillFocus(CWnd* pNewWnd) 
{
	if (!bEscapeKey)
	{
		if(m_bTestMax == true)
		{
			CString temp;
			this->GetWindowText(temp);
			int tmp=atol(temp.GetBuffer(0) );
/*			if(tmp<nMin||tmp>nMax)
			{
				AfxMessageBox("输入数据不正确!");
				PostMessage(WM_CLOSE, 0, 0);
				m_plist->SetItemText(nItem,1,m_textPre.GetBuffer(0));
				m_plist->Invalidate();
				return;
			}*/
		}
		InsertItem();
	}
	PostMessage(WM_CLOSE, 0, 0);
}
BOOL CParamEdit::CheckNumber(CString Num)
{
	return FALSE;
}
