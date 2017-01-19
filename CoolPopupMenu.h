//////////////////////////////////////////////////////////////////////
// CoolPopupMenu.h: interface for the CCoolPopupMenu class.
//
// CCoolPopupMenu  是一个制作漂亮的弹出菜单的类，欢迎大家的使用！
//
// 如果大家有什么问题或者建议，可以和本人联系：
// xb9424@163.net(xb9424@263.net、xb9424@sina.com)
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COOLPOPUPMENU_H__50444B2C_1510_11D4_9E06_0050BABF8C7F__INCLUDED_)
#define AFX_COOLPOPUPMENU_H__50444B2C_1510_11D4_9E06_0050BABF8C7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Afxtempl.h"


struct MenuItem
{
	CString  m_strMenuText;
	int      m_iIndex;
};

class CCoolPopupMenu : public CMenu  
{
public:
	CCoolPopupMenu();
	virtual ~CCoolPopupMenu();

// Operations
public:
	void SetImage(UINT nID,int cx,int row);   
	void AppendCoolMenu(UINT nID,CString text);
	void SetBackImage(UINT nID,int cx,int row);

public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT  lpDrawItemStruct) ;
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );

public:
	void  SetMenuWidth(int width)  { m_iMenuWidth=width;   }
	int   GetMenuWidth()           { return  m_iMenuWidth; }

	void  SetMenuHeight(int height){ m_iMenuHeight=height; }
	int   GetMenuHeight()          { return  m_iMenuHeight;}

private:
	int    m_iMenuWidth;    //菜单宽度
	int    m_iMenuHeight;   //菜单高度

	int    m_iBackImageWidth;  //背景图象的宽度
	int    m_iBackImageHeight; //背景图象的高度

	int    m_iImageWidth;      //图象的宽度
	int    m_iImageHeight;     //图象的宽度
private:
	CImageList  m_BackImageList;
	CImageList  m_ImageList;
	CArray<CString,CString>m_MenuText;   //保存每一项菜单的数据	
	CMap<UINT,UINT,MenuItem,MenuItem>  m_MapMenu;  //保存菜单的数据
	int      m_iItemNum;
	int      m_iUpHeight;
};

#endif // !defined(AFX_COOLPOPUPMENU_H__50444B2C_1510_11D4_9E06_0050BABF8C7F__INCLUDED_)
