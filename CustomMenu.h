// CustomMenu.h: interface for the CCustomMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMMENU_H__72D19424_7CD8_4F7E_8DF4_62E0E5FBE4F4__INCLUDED_)
#define AFX_CUSTOMMENU_H__72D19424_7CD8_4F7E_8DF4_62E0E5FBE4F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct
{
	CString strText;
	int     nMenuID; // -2:顶层菜单条   -1:弹出菜单  0:分隔条  >0:一般的菜单
}COOLMENU, *LPCOOLMENU;

class CCustomMenu : public CMenu  
{
public:
	CCustomMenu();
	virtual ~CCustomMenu();

public:
	BOOL LoadMenu(UINT uMenuID, UINT uToolBarID = 0, CSize sz = CSize(16, 15));
	BOOL AttachMenu(HMENU hMenu, UINT uToolBarID = 0, CSize sz = CSize(16, 15));
	
	BOOL DetachMenu();

	BOOL ChangeStyle(HMENU hMenu, BOOL bTop = FALSE);
	void AppendMenu(UINT uID, CString strText);
	void SetBkImage(UINT uID);
	int GetImageFromToolBar(UINT uToolBarID, CSize sz, 
		CImageList *pImageList, CUIntArray *uIDArray);
	int AddToolBar(UINT uToolBarID, CSize sz);
	int DrawBmp(CDC *pDC, UINT uID, CRect rect, BOOL bStretch = FALSE);

protected:
	void ClearMemory();
	int  GetIndexByID(UINT uID);
	void DrawGradRect(CDC *pDC, CRect rect, COLORREF cr1, COLORREF cr2, BOOL bHor = TRUE); 

protected:
	CImageList m_ilImageList;
	CUIntArray m_uIDArray;
	CObList m_obMenuList;
	int m_nMenuHeight;
	int m_nMenuWidth;
	int m_nSepHeight;
	int m_nSpace;
	CSize m_szMenuBk;
	CSize m_szMenuIcon;
	UINT m_uBackBmpID;

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
};

#endif // !defined(AFX_CUSTOMMENU_H__72D19424_7CD8_4F7E_8DF4_62E0E5FBE4F4__INCLUDED_)
