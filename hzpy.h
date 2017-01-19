//Author:ltr  Date:2004-11-27
class CSpz
{
private:
	char (*m_strData)[4];		//字符串数组
	int  m_nCount;				//字符串数组的个数
public:
	//构造函数
	CSpz()
	{
		m_strData=NULL;
		m_nCount=0;
	}

	//打开数据文件，填充数据
	BOOL Open(const char* strFileName=NULL);
	
	//返回数组中项的个数
	UINT GetCount()
	{
		return m_nCount;
	}

	//根据汉字得到拼音,strHz--表示汉字，strPy--根据汉字所得到的拼音数组
	//返回:TRUE表示得到拼音,FALSE表示未得到拼音
	BOOL GetPy(const char* strHz, char &sPy);
	BOOL GetHzpy(int Index, CString &strHz, CString& strPy);

	//目的：清除所占内存
	void Close();

	//快速排序
	BOOL QuickSort(int low,int high);
	//析构函数
	~CSpz(){}
};

typedef struct _tagDYZSTRUCT
{
	char strHz[9];		//汉字名称
	char strPy[7];		//拼音简写
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

	//打开数据文件，填充数据
	BOOL Open(const char* strFileName=NULL);
	
	//返回数组中项的个数
	UINT GetCount()
	{
		return m_nCount;
	}

	//根据汉字得到拼音,strName--表示汉字或词组，sPy--根据汉字所得到的拼音数组
	//返回:TRUE表示得到了拼音,FALSE表示未得到拼音
	BOOL GetPy(const char* strName, char* strPy);
	BOOL GetHzpy(int Index, CString &strHz, CString& strPy);
	//目的：清除所占内存
	void Close();

	//快速排序
	BOOL QuickSort(int low,int high);

	//析构函数
	~CDyz(){}
};