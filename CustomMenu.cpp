// CustomMenu.cpp: implementation of the CCustomMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomMenu.h"
#include "Windows.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define TOP_COLOR			RGB(128, 128, 128)
#define BOTTOM_COLOR		RGB(255, 255, 255)

#define TEXT_COLOR			RGB(0, 0, 0)
#define HIGHTEXT_COLOR		RGB(255,255, 255)

#define MEMU_SYSCOLOR		GetSysColor(COLOR_BTNFACE)
//#define MEMU_SYSCOLOR		RGB(0,100,255)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCustomMenu::CCustomMenu()
{
	m_nMenuHeight = 22;
	m_nMenuWidth = 50;
	m_nSepHeight = 5;
	m_nSpace = 4;
	m_ilImageList.DeleteImageList();
	m_uIDArray.RemoveAll();
	m_uBackBmpID = 0;
	m_szMenuBk = CSize(0, 0);
	m_szMenuIcon = CSize(0, 0);
}

CCustomMenu::~CCustomMenu()
{
	ClearMemory();
}


void CCustomMenu::ClearMemory()
{
	POSITION pos = m_obMenuList.GetHeadPosition();
	while(pos)
	{
		LPCOOLMENU lpMenu = (LPCOOLMENU)m_obMenuList.GetNext(pos);
		if(lpMenu)
		{
			delete lpMenu;
			lpMenu = NULL;
		}
	}
	m_obMenuList.RemoveAll();
	m_ilImageList.DeleteImageList();
	m_uIDArray.RemoveAll();
}

BOOL CCustomMenu::ChangeStyle(HMENU hMenu, BOOL bTop)
{
	CMenu *pMenu = CMenu::FromHandle(hMenu);
	if(pMenu != NULL)
	{
		for(UINT i = 0; i < pMenu->GetMenuItemCount(); i ++)
		{
			LPCOOLMENU lpMenu = new COOLMENU;
			lpMenu->nMenuID = pMenu->GetMenuItemID(i);
			if(lpMenu->nMenuID < 0 && bTop)
			{
				lpMenu->nMenuID = -2;
			}
			pMenu->GetMenuString(i, lpMenu->strText, MF_BYPOSITION);
			pMenu->ModifyMenu(i, MF_BYPOSITION | MF_OWNERDRAW,
				lpMenu->nMenuID, LPCTSTR(lpMenu));
			CMenu *pSubMenu = pMenu->GetSubMenu(i);
			if(pSubMenu && lpMenu->nMenuID != -2 && !bTop)
			{
				lpMenu->nMenuID = -1;
			}
			m_obMenuList.AddTail((CObject *)lpMenu);
			if(pSubMenu)
			{
				ChangeStyle(pSubMenu->GetSafeHmenu());
			}
		}
	}
	return TRUE;
}

BOOL CCustomMenu::LoadMenu(UINT uMenuID, UINT uToolBarID, CSize sz)
{
	BOOL bRet = CMenu::LoadMenu(uMenuID);
	ChangeStyle(GetSafeHmenu(), TRUE);
	return bRet;
}

BOOL CCustomMenu::AttachMenu(HMENU hMenu, UINT uToolBarID, CSize sz)
{
	ClearMemory();

	Attach(hMenu);
	ChangeStyle(GetSafeHmenu(), TRUE);

	m_szMenuIcon = sz;

	int nRet = GetImageFromToolBar(uToolBarID, sz, &m_ilImageList, &m_uIDArray);

	return TRUE;
}

BOOL CCustomMenu::DetachMenu()
{
	ClearMemory();
	Detach();

	return TRUE;
}

void CCustomMenu::AppendMenu(UINT uID, CString strText)
{
}

void CCustomMenu::SetBkImage(UINT uID)
{
	m_uBackBmpID = uID;
	CBitmap bmp;
	bmp.LoadBitmap(m_uBackBmpID);
	if(bmp.GetSafeHandle() != NULL)
	{
		BITMAP bm;
		bmp.GetBitmap(&bm);
		m_szMenuBk.cx = bm.bmWidth;
		m_szMenuBk.cy = bm.bmHeight;
	}
	else
	{
		m_szMenuBk = CSize(0, 0);
	}
}

int CCustomMenu::GetIndexByID(UINT uID)
{
	for(int i = 0; i < m_uIDArray.GetSize(); i ++)
	{
		if(uID == m_uIDArray[i])
		{
			ASSERT(i < m_ilImageList.GetImageCount());
			return i;
		}
	}
	return -1;
}

int CCustomMenu::AddToolBar(UINT uToolBarID, CSize sz)
{
	CImageList ilImage;
	CUIntArray uIDArray;
	int nRet = GetImageFromToolBar(uToolBarID, sz, &ilImage, &uIDArray);
	if(nRet > 0)
	{
		for(int i = 0; i < ilImage.GetImageCount(); i ++)
		{
			HICON hIcon = ilImage.ExtractIcon(i);
			if(hIcon != NULL)
			{
				m_ilImageList.Add(hIcon);
				m_uIDArray.Add(uIDArray[i]);
			}
		}
	}
	return nRet;
}

int CCustomMenu::GetImageFromToolBar(UINT uToolBarID, CSize sz, CImageList *pImageList, 
						 CUIntArray *uIDArray)
{
	if(uToolBarID <= 0 || pImageList == NULL || uIDArray == NULL)
		return -1;
	CToolBar wndToolBar;
	if(!wndToolBar.Create(AfxGetMainWnd()) || !wndToolBar.LoadToolBar(uToolBarID))
	{
		return -1;
	}
	int nCount = wndToolBar.GetCount();
	if(nCount <= 0)
	{
		return -1;
	}
	pImageList->Create(uToolBarID, sz.cx, sz.cy, RGB(192, 192, 192));
	int nImageCount = pImageList->GetImageCount();
	for(int i = 0; i < nCount; i ++)
	{
		UINT uID = wndToolBar.GetItemID(i);
		if(uID != ID_SEPARATOR)
		{
			uIDArray->Add(uID);
		}
	}
	int nSize = uIDArray->GetSize();
	ASSERT(nSize == nImageCount);

	return nSize;
}

int CCustomMenu::DrawBmp(CDC *pDC, UINT uID, CRect rect, BOOL bStretch)
{
	HRSRC hSrc = FindResource(NULL, MAKEINTRESOURCE(uID), RT_BITMAP);
	if(hSrc)
	{
		CBitmap bmp;
		bmp.LoadBitmap(uID);
		if(bmp.GetSafeHandle() != NULL)
		{
			BITMAP bm;
			bmp.GetBitmap(&bm);
			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap *pOldBmp = memDC.SelectObject(&bmp);
			if(bStretch)
			{
				pDC->SetStretchBltMode(COLORONCOLOR);
				pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),
					&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			}
			else
			{
				pDC->BitBlt(rect.left, rect.top, bm.bmWidth, bm.bmHeight,
					&memDC, 0, 0, SRCCOPY);
			}
			memDC.SelectObject(&pOldBmp);
			memDC.DeleteDC();
		}
	}
	return 0;
}

void CCustomMenu::DrawGradRect(CDC *pDC, CRect rect, COLORREF cr1, 
							   COLORREF cr2, BOOL bHor)
{
	int r1 = GetRValue(cr1);
	int g1 = GetGValue(cr1);
	int b1 = GetBValue(cr1);

	int r2 = GetRValue(cr2);
	int g2 = GetGValue(cr2);
	int b2 = GetBValue(cr2);

	if(bHor)
	{
		float dr = ((float)(r2 - r1))/(float)(rect.Width());
		float dg = ((float)(g2 - g1))/(float)(rect.Width());
		float db = ((float)(b2 - b1))/(float)(rect.Width());
		
		for(int i = rect.left; i < rect.right; i ++)
		{
			int r = r1 + (int)(dr*((float)(i - rect.left)));
			int g = g1 + (int)(dg*((float)(i - rect.left)));
			int b = b1 + (int)(db*((float)(i - rect.left)));
			CPen pen(PS_SOLID, 1, RGB(r, g, b));
			CPen *old = pDC->SelectObject(&pen);
			pDC->MoveTo(i, rect.top);
			pDC->LineTo(i, rect.bottom);
			pDC->SelectObject(old);
		}
	}
	else
	{
		float dr = ((float)(r2 - r1))/(float)(rect.Height());
		float dg = ((float)(g2 - g1))/(float)(rect.Height());
		float db = ((float)(b2 - b1))/(float)(rect.Height());
		
		for(int i = rect.top; i < rect.bottom; i ++)
		{
			int r = r1 + (int)(dr*((float)(i - rect.top)));
			int g = g1 + (int)(dg*((float)(i - rect.top)));
			int b = b1 + (int)(db*((float)(i - rect.top)));
			CPen pen(PS_SOLID, 1, RGB(r, g, b));
			CPen *old = pDC->SelectObject(&pen);
			pDC->MoveTo(rect.left, i);
			pDC->LineTo(rect.right, i);
			pDC->SelectObject(old);
		}
	}
}

void CCustomMenu::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
//	CMenu::DrawItem(lpDIS);
	
	LPCOOLMENU lpMenu = (LPCOOLMENU)(lpDIS->itemData);
	if(!AfxIsValidAddress(lpMenu, sizeof(COOLMENU)))
		return ;

	CDC *pDC = CDC::FromHandle(lpDIS->hDC);
	const CRect rect = lpDIS->rcItem;

	BOOL bIsSelected = FALSE;
	BOOL bIsChecked  = FALSE;
	BOOL bIsGrayed   = FALSE;
	BOOL bHasImage   = FALSE;
	BOOL bIsFocus    = FALSE;


	//取得菜单状态
	bIsSelected = (lpDIS->itemState & ODS_SELECTED);
	bIsChecked  = (lpDIS->itemState & ODS_CHECKED);
	bIsGrayed   = (lpDIS->itemState & ODS_GRAYED);
	bIsFocus    = (lpDIS->itemState & ODS_FOCUS);

	CRect rc = rect;
	CBrush Brush(MEMU_SYSCOLOR);
	pDC->FillRect(rc, &Brush);
	pDC->SetBkColor(MEMU_SYSCOLOR);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectStockObject(NULL_BRUSH);

	if(lpMenu->nMenuID == 0)// 分隔条
	{
		CRect rc = rect;
		rc.top = rect.Height()/2 + rect.top;
		rc.bottom = rc.top + 2;
		rc.left += (m_szMenuBk.cx + m_nSpace);
		pDC->Draw3dRect(rc, TOP_COLOR, BOTTOM_COLOR);
	}
	else // 显示文字或者图标
	{
		//先画图标
		int nIndex = GetIndexByID(lpMenu->nMenuID);
		if(nIndex >= 0) // 如果有图片标
		{
			bHasImage = TRUE; //设置有图像标志
			rc = rect;
			rc.left += (m_szMenuBk.cx + m_nSpace);
			int n = (rect.Height() - m_szMenuIcon.cy)/2;
			rc.top += (n + 1);
			m_ilImageList.Draw(pDC, nIndex, CPoint(rc.left, rc.top), ILD_TRANSPARENT);
			CRect rc1 = CRect(rc.left, rc.top, rc.left + m_szMenuIcon.cx, 
				rc.top + m_szMenuIcon.cy);
			rc = rc1;
			rc.InflateRect(m_nSpace/2, m_nSpace/2);
			if(bIsChecked) //如果有图像并且是Check状态
			{
				pDC->Draw3dRect(rc, TOP_COLOR, BOTTOM_COLOR);
			}
			else // 没有Check但是有图像
			{
				rc.InflateRect(1, 1);
				if(bIsSelected)
				{
					pDC->Draw3dRect(rc, BOTTOM_COLOR, TOP_COLOR);
				}
				else
				{
					pDC->Draw3dRect(rc, MEMU_SYSCOLOR, MEMU_SYSCOLOR);
				}
			}
		}
		else // 如果没有使用缺省的
		{
			bHasImage = FALSE; //设置无图像标志
			if(bIsChecked)
			{
				bHasImage = TRUE; //设置有图像标志
				rc = rect;
				rc.left += (m_szMenuBk.cx + m_nSpace);
				int n = (rect.Height() - m_szMenuIcon.cy)/2;
				rc.top += (n + 1);
				CRect rc1 = CRect(rc.left, rc.top, rc.left + m_szMenuIcon.cx, 
					rc.top + m_szMenuIcon.cy);
				
				pDC->SetTextColor(TEXT_COLOR);
				pDC->DrawText(_T("√"), &rc1, DT_EXPANDTABS | DT_VCENTER |
					DT_SINGLELINE | DT_CENTER);
				rc = rc1;
				rc.left += 1;
				rc.top += 1;
				pDC->DrawText(_T("√"), &rc, DT_EXPANDTABS | DT_VCENTER |
					DT_SINGLELINE | DT_CENTER);

				rc1.InflateRect(m_nSpace/2, m_nSpace/2);
				pDC->Draw3dRect(rc1, TOP_COLOR, BOTTOM_COLOR);
			}
		}
		//显示菜单文字
		if(bIsGrayed) // 是禁止菜单
		{
			rc = rect;
			rc.left += (m_szMenuBk.cx + m_szMenuIcon.cx + m_nSpace*2);
			rc.DeflateRect(0, m_nSpace/2, 0, m_nSpace/2 - 1);
			if(bIsSelected) // 如果是禁止菜单，并且是被选择
			{
				CRect rc1 = rc;
				if(!bHasImage)
				{
					rc1.left -= (m_szMenuIcon.cx + m_nSpace);
				}
//				DrawGradRect(pDC, rc1, TOP_COLOR, RGB(192, 192, 192));
				if(lpMenu->strText.GetLength() > 0)
				{
					rc = rect;
					rc.left += (m_szMenuBk.cx + m_szMenuIcon.cx + m_nSpace*3);
					pDC->SetTextColor(TOP_COLOR);
					pDC->DrawText(lpMenu->strText, &rc, DT_EXPANDTABS | DT_VCENTER | DT_SINGLELINE);
				}
			}
			else // 如果是禁止菜单但没有被选择
			{
				CRect rc1 = rc;
				if(!bHasImage)
				{
					rc1.left -= (m_szMenuIcon.cx + m_nSpace);
				}
				pDC->FillSolidRect(rc1, MEMU_SYSCOLOR);

				if(lpMenu->strText.GetLength() > 0)
				{
					rc = rect;
					rc.left += (m_szMenuBk.cx + m_szMenuIcon.cx + m_nSpace*3);
					pDC->SetTextColor(BOTTOM_COLOR);
					pDC->DrawText(lpMenu->strText, &rc, DT_EXPANDTABS | DT_VCENTER | DT_SINGLELINE);
					pDC->SetTextColor(TOP_COLOR);
					rc.left -= 1;
					rc.top -= 1;
					pDC->DrawText(lpMenu->strText, &rc, DT_EXPANDTABS | DT_VCENTER | DT_SINGLELINE);
				}
			}
		}
		else // 不是禁止菜单
		{
			if(lpMenu->nMenuID == -2) // 顶层菜单
			{
				rc = rect;
				rc.left += m_nSpace;
				if(bIsSelected) // 是选择状态
				{
					pDC->Draw3dRect(rc, TOP_COLOR, BOTTOM_COLOR);
					pDC->SetTextColor(HIGHTEXT_COLOR);
				}
				else // 不是选择状态
				{
					pDC->Draw3dRect(rc, MEMU_SYSCOLOR, MEMU_SYSCOLOR);
					pDC->SetTextColor(TEXT_COLOR);
				}
				//afxDump << bIsSelected << "--"<< bIsFocus << "--" << bIsChecked << "\n";
				rc.left = rc.left + (rect.Width() - pDC->GetTextExtent(lpMenu->strText).cx)/2;
//				rc.left += m_nSpace;
				if(lpMenu->strText.GetLength() > 0)
				{
					pDC->DrawText(lpMenu->strText, &rc, DT_EXPANDTABS | 
						DT_VCENTER | DT_SINGLELINE);
				}
			}
			else // 不是顶层菜单,是弹出菜单或者一个菜单项目
			{
				rc = rect;
				rc.left += (m_szMenuBk.cx + m_szMenuIcon.cx + m_nSpace*2);
				rc.DeflateRect(0, m_nSpace/2, 0, m_nSpace/2 - 1);
				if(bIsSelected)
				{
					CRect rc1 = rc;
					if(!bHasImage)
					{
						rc1.left -= (m_szMenuIcon.cx + m_nSpace);
					}
//					DrawGradRect(pDC, rc1, TOP_COLOR, RGB(192, 192, 192));
					pDC->SetTextColor(BOTTOM_COLOR);
				}
				else
				{
					CRect rc1 = rc;
					if(!bHasImage)
					{
						rc1.left -= (m_szMenuIcon.cx + m_nSpace);
					}
					pDC->FillSolidRect(rc1, MEMU_SYSCOLOR);
					pDC->SetTextColor(TEXT_COLOR);
				}
				if(lpMenu->strText.GetLength() > 0)
				{
					rc = rect;
					rc.left += (m_szMenuBk.cx + m_szMenuIcon.cx + m_nSpace*3);
					pDC->DrawText(lpMenu->strText, &rc, DT_EXPANDTABS |
						DT_VCENTER | DT_SINGLELINE);
				}
			}
		}
	}
	if((lpDIS->itemAction & ODA_DRAWENTIRE) && lpMenu->nMenuID != -2)
	{
		DrawBmp(pDC, m_uBackBmpID, CRect(1, 1, -1, -1), FALSE);
		DrawGradRect(pDC, CRect(0, 0, m_szMenuBk.cx, rect.bottom), 
			RGB(0, 0, 0), RGB(0, 0, 255), FALSE);
	}
}

void CCustomMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	LPCOOLMENU lpMenu = (LPCOOLMENU)(lpMIS->itemData);
	if(lpMenu->nMenuID == 0)
	{
		lpMIS->itemHeight = m_nSepHeight;
		lpMIS->itemWidth = 50;
	}
	else
	{
		lpMIS->itemWidth = m_nMenuWidth; // default

		CString strText = lpMenu->strText;
		int nLen = strText.GetLength();
		if(nLen > 0)
		{
			CFont fontMenu;
			LOGFONT lf;
			ZeroMemory(&lf, sizeof(LOGFONT));
			NONCLIENTMETRICS nm;
			nm.cbSize = sizeof(NONCLIENTMETRICS);
			VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
				nm.cbSize, &nm, 0)); 
			lf = nm.lfMenuFont;
			fontMenu.CreateFontIndirect(&lf);
			SIZE sz;			
			CDC *pDC = AfxGetMainWnd()->GetDC();
			CFont *old = pDC->SelectObject(&fontMenu);
			::GetTextExtentPoint32(pDC->GetSafeHdc(), strText, strText.GetLength(), &sz);
			if(lpMenu->nMenuID == -2)
			{
//				lpMIS->itemWidth = sz.cx + m_nSpace;
				lpMIS->itemWidth = 45;
			}
			else
			{
				lpMIS->itemWidth = sz.cx + m_szMenuBk.cx + m_szMenuIcon.cx + m_nSpace*4;
			}
			pDC->SelectObject(&old);
		    AfxGetMainWnd()->ReleaseDC(pDC);
			fontMenu.DeleteObject();
		}
		lpMIS->itemHeight = m_nMenuHeight;
	}
}
