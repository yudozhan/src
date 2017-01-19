// RichEditFormular.cpp : implementation file
//

#include "stdafx.h"
#include "WH.h"
#include "RichEditFormular.h"
#include "DialogEditZBGSHS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern dataFunc stritemsArr[];
extern int nItemCountText;
/////////////////////////////////////////////////////////////////////////////
// CRichEditFormular

CRichEditFormular::CRichEditFormular()
{
}

CRichEditFormular::~CRichEditFormular()
{
}


BEGIN_MESSAGE_MAP(CRichEditFormular, CAutoRichEditCtrl)
	//{{AFX_MSG_MAP(CRichEditFormular)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichEditFormular message handlers

void CRichEditFormular::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CAutoRichEditCtrl::OnMouseMove(nFlags, point);

	if(m_TitleTip.m_hWnd == NULL )
	{
		CRect rect(0,0,1,1);
		m_TitleTip.Create (this);
		return;
	}
	int nCharOffset = GetCharFormPos( point );
	if( nCharOffset < 0 )
	{
		m_TitleTip.Hide ();
		return;
	}

	CString strText;
	GetWindowText( strText );
	if( nCharOffset >= strText.GetLength() )
	{
		m_TitleTip.Hide ();
		return;
	}
	CString strTmp = GetWordByOffset(nCharOffset);
	strTmp.TrimRight();
	if( strTmp.IsEmpty() )
	{
		m_TitleTip.Hide ();
		return;
	}

	TRACE("%s\n",strTmp);

	strTmp.MakeUpper ();
	int nLen = strTmp.GetLength ();

	int i;
	for(i = 0;i<nItemCountText;i++)
	{
		CString s = stritemsArr[i].s ;
		s = s.Left (nLen);
		if(s == strTmp)
		{
			if(stritemsArr[i].s[nLen] == '-' ||stritemsArr[i].s[nLen] == '(')
				break;
		}
	}
	if(i>=nItemCountText)
	{
		m_TitleTip.Hide ();
		return;
	}
	CString s2 = stritemsArr[i].s;
	int nFind = s2.Find ("---",0);
	if(nFind>=0)
	{
		strTmp = s2.Left(nFind);
		s2.Delete (0,nFind+3);
	}
	strTmp = strTmp+"\n"+s2+"\n";

	TRACE("22 == %s\n",strTmp);

	m_TitleTip.Show(strTmp,point);

}
///-----------------------------------------------
/// 2002-11-8
///	功能:
///		根据偏移获取单词
///	入口参数:
///		nOffset			偏移
///	返回参数:
///		空字符串，失败
CString CRichEditFormular::GetWordByOffset(int nOffset)
{
	CString strText;
	GetWindowText( strText );
	if( nOffset >= strText.GetLength() )
	{
		strText.Empty();
		return strText;
	}
	int nLeftPos = nOffset;
	while( nLeftPos>=0 )
	{
		char cLeft = strText[ nLeftPos ];
		if(cLeft < 0 || FALSE == ::isalnum(cLeft) )
		{
			nLeftPos ++;
			break;				//	找到
		}
		nLeftPos --;
	}
	
	if( nLeftPos < 0 )
		nLeftPos = 0;

	int nMaxLen = strText.GetLength();
	int nRightPos = nOffset;
	while( nRightPos < nMaxLen )
	{
		char cRight = strText[ nRightPos ];
		if(cRight < 0 || FALSE == ::isalnum(cRight) )
		{
			nRightPos --;
			CString ss = strText.Mid( nLeftPos, nRightPos-nLeftPos+1 );
			ss.MakeLower ();
			if(ss == "finance" || ss == "dynainfo"
				|| ss == "askvol"
				|| ss == "askprice"
				|| ss == "bidvol"
				|| ss == "bidorice"
				)
			{
				bool bFind = false;
				while( nRightPos < nMaxLen )
				{
					cRight = strText[ nRightPos ];
					if(cRight == ')')
					{
						bFind = true;
//						nRightPos --;
						break;
					}
					nRightPos ++;
				}
			}
			break;
		}
		nRightPos ++;
	}
	if( nRightPos >= nMaxLen )
		nRightPos = nMaxLen-1;

	CString ss2 = strText.Mid( nLeftPos, nRightPos-nLeftPos+1 );

	ss2.Replace (" ","");
	
	return ss2;
}

int CRichEditFormular::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CAutoRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect(0,0,1,1);
	m_TitleTip.Create (this);
/*	DWORD dwStyle = WS_BORDER | WS_POPUP; 
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
	m_pParentWnd = pParentWnd;

	return CreateEx( dwExStyle, TITLETIP_CLASSNAME, NULL, dwStyle, 0, 0, 0, 0, 
		NULL, NULL, NULL );
*/	
	return 0;
}
