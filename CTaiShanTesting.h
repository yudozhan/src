// CTaiShanTesting.h: interface for the CTaiShanTesting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HJYPINGCE_H__90C60EC1_4989_42EA_B2CF_6DF20D821747__INCLUDED_)
#define AFX_HJYPINGCE_H__90C60EC1_4989_42EA_B2CF_6DF20D821747__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaiShanDoc;
class CTaiShanKlineShowView;
class CTaiShanTesting  
{
private:
	CTaiShanDoc* m_pDoc;
	BOOL m_bRefresh;		//�Ƿ���Ҫ�������⣬FALSE:����Ҫ��TRUE:��Ҫ
	BOOL m_bHaveFormula;
public:
	CString m_strStockID;	//��ǰ��Ʊ����
	CArray<IndexDataInfo,IndexDataInfo> m_strHjyChooseFormula;	//ѡ�ɹ�ʽ
	CArray<IndexDataInfo,IndexDataInfo> m_strChooseFormu;	//ѡ�ɹ�ʽ
	
	CString m_strResultData2;		//ѡ�ɽ��
	CString m_strResultData;		//ѡ�ɽ��
public:
	CTaiShanTesting();		//���캯��
	CTaiShanTesting(CTaiShanDoc* pDoc);	//���캯��
	void SetStock(CString strStockID=_T(""));	//���ù�Ʊ����
	void GetChooseFormula(CTaiShanKlineShowView* pView,BOOL bRefresh=FALSE);	//�õ�ѡ�ɹ�ʽ
	void SelectStock(CTaiShanKlineShowView* pView,BOOL bRefresh=FALSE);		//����ѡ�ɣ��õ�ѡ�ɽ��
	void OnDraw(CTaiTestRichEditCtrl* pRich, COLORREF crBack, COLORREF crText, COLORREF crTitle,CRect &rc);	//�������
	virtual ~CTaiShanTesting();		//��������
};

#endif // !defined(AFX_HJYPINGCE_H__90C60EC1_4989_42EA_B2CF_6DF20D821747__INCLUDED_)
