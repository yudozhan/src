// ByjDrawRightRect.h: interface for the ByjDrawRightRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BYJDRAWRIGHTRECT_H__A20CC169_C2B1_43EC_8385_188CE681B1E9__INCLUDED_)
#define AFX_BYJDRAWRIGHTRECT_H__A20CC169_C2B1_43EC_8385_188CE681B1E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTaiShanKlineShowView;
class CTaiShanDoc;

class ByjDrawRightRect  
{
public:
	void DrawCaption(CDC* pDC,CFormularContent* pIndex);
	void GetMaxMin(int j,bool bFirst);
	static void SaveName(CString name);
	static bool GetScale(int n1, int n2, float f1, float f2, float& fOut);
	ByjDrawRightRect();
	virtual ~ByjDrawRightRect();
	ByjDrawRightRect(CTaiShanKlineShowView* bsView,CTaiShanDoc* bsDoc);

	CTaiShanDoc* pDoc;		//CTaiShanDoc��ָ��
 	CTaiShanKlineShowView* pView;		//CTaiShanKlineShowView��ָ��
	CRect m_rectDrawLine;		//���ڿͻ��������������л�ͼ�ľ���
	void CalcSonFiguer(CDC* pDC,CRect &rt);
	static CString m_nameIndex;
	void DrawSonFiguer(CDC* pDC,int j);

private:
	int m_nValidIndicator;//ָ���ڵ�ǰ�����Ƿ���Ч
	ARRAY_JISHU		m_dataFormular;	//������ͼ�ļ��������������ݼ�����
	CString m_lineName[18];//�ߵ�����
	float m_min_sun;			//�ڴ��ڿͻ�����ĳ��ͼ�ڣ���ͼ���������ݵ���Сֵ
	float m_max_sun;			//�ڴ��ڿͻ�����ĳ��ͼ�ڣ���ͼ���������ݵ����ֵ
//
};

#endif // !defined(AFX_BYJDRAWRIGHTRECT_H__A20CC169_C2B1_43EC_8385_188CE681B1E9__INCLUDED_)
