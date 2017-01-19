// CTaiKlineDrawLine.h: interface for the CTaiKlineDrawing class.
//		
// �ļ�����CTaiKlineDrawLine.h

// �����ߣ�

// ����ʱ�䣺

// ����������
//		�����Ի������ݼ����л��߲�����
//		
//		��������룺CTaiShanKlineShowView��ͼ���ָ�뼰��Ӧ���ĵ�ָ�롣
//		
//		�������������Ի��߻�����Ļ�ϣ����Ի�������д���ļ��С�
//		
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LDRAWLINE_H__75FC8BA1_C4DC_11D2_91DE_0080C8E1242B__INCLUDED_)
#define AFX_LDRAWLINE_H__75FC8BA1_C4DC_11D2_91DE_0080C8E1242B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef CArray<INFO_LINESELF,INFO_LINESELF&> infoLineself;

class CTaiShanDoc;//CTaiShanDoc
class CTaiShanKlineShowView;
class CTaiKlineDrawing :public CObject 
{
public:

	bool IsTrendLine();
	void SetAlarmFlag(int nLine,BYTE bAlarm);
	void SetCurrentLineProp(COLORREF rgb=0, int nLineType=0, int nLineThick=0, bool bAlarm=false,int nParam = 0);
	bool GetCurrentLineProp(COLORREF & rgb,int& nLineType,int& nLineThick,bool& bAlarm);
	//����
	void CaclTwoPointXD(CPoint &p1, CPoint &p2);
	//����:�����߶ε�������
	//�������:P1,P2Ϊ���������������
	void ArcInRect(CDC* pDC,int  x1,int y1,int x2,int y2);
	//����:��Բ��
	//�������:X1,Y1,X2,Y2Ϊ������������x��y��������
	bool IsCrossSideFloat(int nSide,float x1, float y1, float x2, float y2,CPoint &pOut);//nSide = 0 is top,1 is right,2 is bottom ,3 is left
	//����:�ж�ֱ�ߵ�λ�ã��Ƿ�ɼ�
	//�������:X1,Y1,X2,Y2Ϊ������������x��y�������ݣ�nSide = 0 is ��,1 is ��,2 is �� ,3 is ��
	//�������:pOut������ĵ�
	void CaclTwoPointFloat(float x1,float y1,float x2,float y2);
	//����:�����߶ε�������
	//�������:X1,Y1,X2,Y2Ϊ������������x��y��������

	bool IsCrossSide(int nSide,CPoint &p1, CPoint &p2,CPoint &pOut);//nSide = 0 is top,1 is right,2 is bottom ,3 is left
	//����:�ж�ֱ�ߵ�λ�ã��Ƿ�ɼ�
	//�������:X1,Y1,X2,Y2Ϊ������������x��y�������ݣ�nSide = 0 is ��,1 is ��,2 is �� ,3 is ��
	//�������:pOut������ĵ�

	void ShowText(CDC* pDC,float in,int y,int nLine = -1);
	//����:��ʾ�ı�
	//�������:YΪ����ĵ��y�������ݣ�inΪ���������
	void SetPointHuiGui(int foot,int footOld);//�������Իع��ߵĵ�
	//�������:footΪ����ĵڶ��������±꣬footOldΪ����ĵ�һ�������±�
	//---����
	int SetPointPos(CPoint& point);//ȷ�����ߵĶ˵�
	//�������:point�����
	void DrawPointBE(CDC* pDC);//���Ի��ߵĶ˵����
	void DrawXianDuan(CDC* pDC,CPoint& p1,CPoint& p2);//���߶�
	//�������:p1��p2Ϊ���������
	void DrawXianDuanVirtul(CDC *pDC,CPoint& p1,CPoint& p2,COLORREF* clr = NULL);//�����߶�
	//�������:p1��p2Ϊ���������

	void DeleteLineAll();//ɾ�����л���
	void DeleteLine();//ɾ������
	void AddLine();//��һ������
	int FloatToY(float val,float& out,int flag=0);//�Ѹ�����ת��ΪY�������֮
	//�������:valΪ����ĸ�����
	//�������������y�����꣬outΪ����ĸ�����
	int TimeToX(int time,int flag=0);//��ʱ��ת��ΪX�������֮ 
	void DrawTotal(CDC* pDC);//�����л���
	int ScreenToFile(int n,bool isToFile,bool bPrinting=false);//����Ļ����ת��Ϊ���λ��
	//�������:nΪ������±꣬isToFileΪ�ж�ת������ı�־��
	//������������ش�����
	void DrawText(CDC* pDC,CPoint* pp=NULL,CString * s=NULL);//���ı�
	void DrawArrowUp(UINT id,CDC* pDC,int nPnt=0,int nWidth = 23);//����ͷ
	void DrawHuiGuiTop(CDC* pDC,int flag=0,COLORREF* clr=NULL);//���ع������������
	void CaclTwoPoint(CPoint& p1,CPoint& p2,int flag=0);//����ֱ���뻭ͼ���εĽ���
	//����:�����߶ε�������
	//�������:X1,Y1,X2,Y2Ϊ������������x��y��������
	int DrawSelfLine(CDC* pDC,bool bSetProp = false);//��һ���Ի���
	void DrawRect(CDC* pDC,CRect &r);//������
	int IsOnPoint(CPoint &p1, CPoint &p2, CPoint &pointIn, int flag);//�Ƿ��ڵ�������
	//�������:p1��p2Ϊ��������˵㣬pointInΪҪ�жϵ������
	//���������������������ڵ�λ��
	int IsOnLine(CPoint &p1, CPoint &p2, CPoint &pointIn, int flag);//������Ƿ�������
	//�������:p1��p2Ϊ��������˵㣬pointInΪҪ�жϵ������
	//���������������������ڵ�λ��
	int IsOnSelfLine(CPoint& point);//������Ƿ����Ի����� 
	//�������:point�����
	int IsOnSelfLineTotal(CPoint& point);//������Ƿ����Ի�����
	//�������:point�����
	int SetPointMoved(int cx,int cy);//�Ի����ƶ�ʱ��������ʼ��
	//��ڲ�����cx��cyΪ��Ⱥ͸߶�
	void SetDrawLineRect();//��ʼ�����߾���

	//--- 
	int WriteLineself();//�ѻ���д���ļ���
	int ReadLineself();//���ļ��ж�����������

	//---�й��ļ������� 
	int m_nPosStock;//��Ʊ�ڻ����ļ��е�������
	int m_nLine;//����Ʊ�Ļ��ߵĸ���
	int m_nShares;//�����ļ��Ĺ�Ʊ���ܸ���
	infoLineself	m_info_lineself;//�Ի�������


	int m_nCurrentLineClicked;//���н�����߶���
	int m_nLineTypeChanging;// ���ڱ���������
	int m_nZhongXianDrawing;//���ڻ�ʱ�ĵڼ���
	CFile* m_fileSh;//�����ļ�
	CFile* m_fileSz;//�����ļ�

	//---���߲���������
	bool m_bSelected;//��ǰ�����Ƿ�ѡ��
	bool m_bLineValidate;//��ǰ���ڻ������Ƿ���Ч
	int m_nCurrentLine;//��ǰѡ�е��ߵ�����

	int m_bMoveZhongXian;//�������͵ĵڼ���
	int m_nZhongXian;//�������͵ĵڼ���
	int m_lenTextNow;//�ı��ĳ���
	CString  m_textNow;//�������ı�
	int m_nPntGroup[30];//���ֻ������͵Ķ˵����Ŀ
	int m_lineTypeOld;//��һ���Ի��ߵ�����
	int m_lineType;//�Ի��ߵ�����
	bool m_bDrawLine;//�Ƿ�����ִ�л��߲���
	int m_nPointCurrent;//��ǰ���ڻ��ڼ�����
	CPoint m_ptOldPingXing[2];//ƽ���߲����У�ԭ�����߶ε�����
	CPoint m_ptOldDrawing[3];//���߲����У�ԭ�����߶ε�����
	CPoint m_ptOld[3];//����ƶ������У�ԭ���ĵ������
	CPoint m_pt[3];//�Ի��ߵĶ˵����Ļ����
	CRect m_rtDrawLine;//�������ڵľ���
	float m_widthPer;//ÿ��K�ߵĿ��

	CTaiShanDoc* pDoc;		//CTaiShanDoc��ָ��
 	CTaiShanKlineShowView* pView;		//CTaiShanKlineShowView��ָ��

	CTaiKlineDrawing();
	CTaiKlineDrawing(CTaiShanKlineShowView* bsView,CTaiShanDoc* bsDoc);
	virtual ~CTaiKlineDrawing();

};

#endif // !defined(AFX_LDRAWLINE_H__75FC8BA1_C4DC_11D2_91DE_0080C8E1242B__INCLUDED_)
