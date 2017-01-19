//Author:ltr  Date:2004-11-27
class CSpz
{
private:
	char (*m_strData)[4];		//�ַ�������
	int  m_nCount;				//�ַ�������ĸ���
public:
	//���캯��
	CSpz()
	{
		m_strData=NULL;
		m_nCount=0;
	}

	//�������ļ����������
	BOOL Open(const char* strFileName=NULL);
	
	//������������ĸ���
	UINT GetCount()
	{
		return m_nCount;
	}

	//���ݺ��ֵõ�ƴ��,strHz--��ʾ���֣�strPy--���ݺ������õ���ƴ������
	//����:TRUE��ʾ�õ�ƴ��,FALSE��ʾδ�õ�ƴ��
	BOOL GetPy(const char* strHz, char &sPy);
	BOOL GetHzpy(int Index, CString &strHz, CString& strPy);

	//Ŀ�ģ������ռ�ڴ�
	void Close();

	//��������
	BOOL QuickSort(int low,int high);
	//��������
	~CSpz(){}
};

typedef struct _tagDYZSTRUCT
{
	char strHz[9];		//��������
	char strPy[7];		//ƴ����д
}DYZSTRUCT, *PDYZSTRUCT;

class CDyz
{
private:
	DYZSTRUCT* m_pData;
	int m_nCount;
	static int m_nSize;
public:
	CDyz()
	{
		m_pData=NULL;
		m_nCount=0;
	}

	//�������ļ����������
	BOOL Open(const char* strFileName=NULL);
	
	//������������ĸ���
	UINT GetCount()
	{
		return m_nCount;
	}

	//���ݺ��ֵõ�ƴ��,strName--��ʾ���ֻ���飬sPy--���ݺ������õ���ƴ������
	//����:TRUE��ʾ�õ���ƴ��,FALSE��ʾδ�õ�ƴ��
	BOOL GetPy(const char* strName, char* strPy);
	BOOL GetHzpy(int Index, CString &strHz, CString& strPy);
	//Ŀ�ģ������ռ�ڴ�
	void Close();

	//��������
	BOOL QuickSort(int low,int high);

	//��������
	~CDyz(){}
};