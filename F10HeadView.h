#pragma once

#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class CTaiTestRichView;
// CF10HeadView 窗体视图

class CF10HeadView : public CFormView
{
	DECLARE_DYNCREATE(CF10HeadView)

protected:
	CF10HeadView();           // 动态创建所使用的受保护的构造函数
	virtual ~CF10HeadView();

public:
	enum { IDD = IDD_F10_HEAD };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
	typedef struct _ItemPosition
	{
		CString strName;
		__int64 iStartPos;
		__int64 iLen;
	} ItemPosition;


	CString m_sCurrSymbol;
	CString m_sPrevSymbol;
	CString m_sNextSymbol;

	CString m_sCurrItemName;

	vector<int>	m_vAllF10FileName;
	vector<CString>	m_vAllStockSymbol;

	CMap<UINT,UINT,CString,CString>	m_mapID_ItemName;
	CStringArray	m_arrItemName;
	CButton			m_myButton[30];
	char*			m_pF10Data;
	CArray<ItemPosition,ItemPosition> m_arrItemPos;


	CTaiTestRichView * m_pTaiTestRichView;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnBnClickedRadioItem(UINT nID);
public:
	void GetAllF10FileName(void);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	void GetFileItems(void);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	void CreateButton(void);
	void MoveButton(void);

public:
	void GetPrevNextSymbol(void);
	void SetPrevNextButtonText(void);

	char* SymbolToName(char *pSymbol);
public:
	afx_msg void OnBnClickedF10Prev();
public:
	afx_msg void OnBnClickedF10Next();
public:
	void ReleaseSources(void);
	void GetTotalStockId(void);
	void TransCurrStockSymbol(void);
	void ShowFileText(CString strItemName = "");
	BOOL GetItemPos(ItemPosition& item);
	void ShowF10();


};


