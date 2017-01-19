//Author:ltr  Date:2002-11-27
#include "stdafx.h"
#include "hzpy.h"

int CDyz::m_nSize=16;

//打开数据文件，填充数据
BOOL CSpz::Open(const char* strFileName)
{
	char sFileName[255]="hzpy.dat";
	strcpy(sFileName,g_hzpy);
	CFile f;
	DWORD dwFileLen;

	if (strFileName != NULL)
		strcpy(sFileName,strFileName);
	if (!f.Open(sFileName,CFile::modeRead | CFile::typeBinary))
		return FALSE;

	Close();	//清除原有数据
	//检验文件
	dwFileLen=f.GetLength();
	if (dwFileLen <= 4*sizeof(long))
		return TRUE;
	
	//读取文件头信息
	int nResult=0;
	int i,j;
	long nHead[2];
	if (f.Read(nHead,2*sizeof(long)) != 2*sizeof(long))
	{
		f.Close();
		return FALSE;
	}
	if (!((nHead[0] > 0) && (nHead[1] > 0) && (nHead[1] % 3 ==0) 
			&& ((long)dwFileLen >= (nHead[0] + nHead[1] -1))))
	{
		f.Close();
		return FALSE;
	}

	char* pBuf = new char[99];
	char* p;
	int nBufSize = 0;
	long nRest = nHead[1];
	//下面开始读取数据

	j=0;
	try
	{
		m_nCount = nRest / 3;
		m_strData = new char[m_nCount][4];
		f.Seek(nHead[0],CFile::begin);
		nBufSize = (int)((nRest > 99) ? 99 : nRest);
		while ((nResult=f.Read(pBuf, nBufSize))>0)
		{
			p=pBuf;
			for(i=0; i<nResult;)
			{
				m_strData[j][0] = p[i++];
				m_strData[j][1] = p[i++];
				m_strData[j][2] = p[i++];
				m_strData[j++][3] = 0;
			}
			nRest -= nResult;
			if (nRest<=0)
				break;
			nBufSize = (int)((nRest > 99) ? 99 : nRest);
		}
		f.Close();
		delete []pBuf;
	}
	catch (...)
	{
		delete []pBuf;
		f.Close();
		return FALSE;
	}

	//下面对数据进行排序
	if (!QuickSort(0, m_nCount-1))
		Close();
	return TRUE;
}

//根据汉字得到拼音,strHz--表示汉字，strPy--根据汉字所得到的拼音数组
//返回:>0表示所得到的拼音数量,其它未返回拼音
BOOL CSpz::GetPy(const char* strHz, char &sPy)
{
	if ((strlen(strHz)<2) || (m_nCount<=0))
		return FALSE;

	//对输入的字符进行处理，只取前两位
	char* pHz =new char[3];
	pHz[0] = strHz[0];
	pHz[1] = strHz[1];
	pHz[2] = 0;
	
	//使用二分法在数组中查找第一个匹配的项
	int lo = 0;
	int hi = m_nCount;
	int mid = (lo + hi) / 2;
	int nResult;
	BOOL bFound=FALSE;
	while(lo <= hi)
	{
		nResult = strncmp(pHz,m_strData[mid],2);
		if (nResult==0)
		{
			bFound = TRUE;
			break;
		}
		else if (nResult<0)
			hi = mid - 1;
		else
			lo = mid + 1;
		mid = (lo + hi) / 2;
	}

	if (!bFound)
		return FALSE;
	else
	{
		//根据所找到的第一个拼音，在两边进行扩散查找剩余的拼音
		sPy = m_strData[mid][2];
		return TRUE;
	}
}

BOOL CSpz::GetHzpy(int Index, CString &strHz, CString& strPy)
{
	if ((m_nCount>0) && (Index>=0) && (Index<m_nCount))
	{
		char strT1[3];
		char strT2[2];
		strT1[0]=m_strData[Index][0];
		strT1[1]=m_strData[Index][1];
		strT1[2]=0;
		strT2[0]=m_strData[Index][2];
		strT2[1]=0;
		strHz=strT1;
		strPy=strT2;
		return TRUE;
	}
	else
		return FALSE;
}

//目的：清除所占内存
void CSpz::Close()
{
	m_nCount=0;
	if (m_strData != NULL)
	{
		delete []m_strData;
		m_strData = NULL;
	}
}

//快速排序
BOOL CSpz::QuickSort(int low,int high)
{
    int lo = low;
    int hi = high;
	char pivot[4];
    if( hi <= lo ) return FALSE;

	strcpy(pivot,m_strData[(low +high)/2]);

    while( lo <= hi )
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        while (lo < high && strcmp(m_strData[lo], pivot)<0 )
 			++lo;

        // Find an element that is smaller than or equal to  the partition 
        // element starting from the right Index.
		while (hi > low && strcmp(m_strData[hi], pivot)>0 )
 			--hi;

        // If the indexes have not crossed, swap if the items are not equal
        if (lo <= hi)
        {
            // swap only if the items are not equal
            if (strcmp(m_strData[lo], m_strData[hi]) != 0)
            {
				char strTemp[4];
				strcpy(strTemp, m_strData[lo]);
				strcpy(m_strData[lo], m_strData[hi]);
				strcpy(m_strData[hi], strTemp);
            }

            ++lo;
            --hi;
        }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if( low < hi )
        QuickSort(low, hi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( lo < high )
        QuickSort(lo, high);

    return TRUE;
}

BOOL CDyz::Open(const char* strFileName)
{
	char sFileName[255] = "hzpy.dat";
	strcpy(sFileName,g_hzpy);
	CFile f;
	DWORD dwFileLen;

	if (strFileName != NULL)
		strcpy(sFileName,strFileName);
	if (!f.Open(sFileName,CFile::modeRead | CFile::typeBinary))
		return FALSE;

	//检验文件
	Close();	//清除原有数据
	dwFileLen=f.GetLength();
	if (dwFileLen <= 4 * sizeof(long))
		return TRUE;

	//读取文件头
	long nHead[2];
	f.Seek(2*sizeof(long),CFile::begin);
	if (f.Read(nHead,2*sizeof(long)) != 2*sizeof(long))
	{
		f.Close();
		return FALSE;
	}
	if (!((nHead[0] > 0) && (nHead[1] > 0) && (nHead[1] % m_nSize == 0) 
			&& ((long)dwFileLen >= (nHead[0]+nHead[1]-1))))
	{
		f.Close();
		return FALSE;
	}

	//下面开始读取数据
	char* pBuf = new char[m_nSize * 50];
	char *p;
	long nRest = nHead[1];
	int nBufSize;
	int nResult=0;
	int i,j;
	j=0;
	try
	{
		m_nCount = nRest / m_nSize;
		m_pData =new DYZSTRUCT[m_nCount];
		f.Seek(nHead[0],CFile::begin);
		nBufSize=(int)((nRest > m_nSize*50) ? m_nSize*50 : nRest);
		while ((nResult=f.Read(pBuf,nBufSize))>0)
		{
			p=pBuf;
			for(i=0; i<nResult; i+=m_nSize)
			{
				memcpy(&m_pData[j++],p,m_nSize);
				p += m_nSize;
			}
			nRest -= nResult;
			if (nRest<=0)
				break;
			nBufSize=(int)((nRest > m_nSize*50) ? m_nSize*50 : nRest);
		}
		f.Close();
		delete []pBuf;
	}
	catch (CException *e)
	{
		TCHAR str[255];
		e->GetErrorMessage(str,255);
		AfxMessageBox(str);
		e->Delete();
		delete []pBuf;
		Close();
		f.Close();
		return FALSE;
	}

	//下面对数据进行排序
	if (!QuickSort(0, m_nCount-1))
		Close();
	return TRUE;
}


//根据汉字得到拼音,strName--表示汉字或词组，sPy--根据汉字所得到的拼音数组
//返回:TRUE表示得到了拼音,FALSE表示未得到拼音
BOOL CDyz::GetPy(const char* strName, char* strPy)
{
	if ((strlen(strName) == 0) || (m_nCount <= 0))
		return FALSE;

	//使用二分法在数组中查找第一个匹配的项
	int lo = 0;
	int hi = m_nCount;
	int mid = (lo + hi) / 2;
	int nResult;
	BOOL bFound=FALSE;
	while(lo <= hi)
	{
		nResult = strcmp(strName,m_pData[mid].strHz);
		if (nResult==0)
		{
			bFound = TRUE;
			break;
		}
		else if (nResult<0)
			hi = mid - 1;
		else
			lo = mid + 1;
		mid = (lo + hi) / 2;
	}

	if (!bFound)
		return FALSE;
	else
	{
		//根据所找到的第一个拼音，在两边进行扩散查找剩余的拼音
		strcpy(strPy,m_pData[mid].strPy);
		return TRUE;
	}
}

BOOL CDyz::GetHzpy(int Index, CString &strHz, CString& strPy)
{
	if ((m_nCount>0) && (Index>=0) && (Index<m_nCount))
	{
		strHz=m_pData[Index].strHz;
		strPy=m_pData[Index].strPy;
		return TRUE;
	}
	else
		return FALSE;
}

//目的：清除所占内存
void CDyz::Close()
{
	m_nCount = 0;
	if (m_pData != NULL)
	{
		delete []m_pData;
		m_pData=NULL;
	}
}

//快速排序
BOOL CDyz::QuickSort(int low,int high)
{
    int lo = low;
    int hi = high;
	char pivot[9];
    if( hi <= lo ) return FALSE;

	strcpy(pivot,m_pData[(low +high)/2].strHz);

    while( lo <= hi )
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        while (lo < high && strcmp(m_pData[lo].strHz, pivot)<0 )
 			++lo;

        // Find an element that is smaller than or equal to  the partition 
        // element starting from the right Index.
		while (hi > low && strcmp(m_pData[hi].strHz, pivot)>0 )
 			--hi;

        // If the indexes have not crossed, swap if the items are not equal
        if (lo <= hi)
        {
            // swap only if the items are not equal
            if (strcmp(m_pData[lo].strHz, m_pData[hi].strHz) != 0)
            {
				PDYZSTRUCT dyzTemp = new DYZSTRUCT;
				memcpy(dyzTemp,&m_pData[lo],m_nSize);
				memcpy(&m_pData[lo],&m_pData[hi],m_nSize);
				memcpy(&m_pData[hi],dyzTemp,m_nSize);
            }

            ++lo;
            --hi;
        }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if( low < hi )
        QuickSort(low, hi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( lo < high )
        QuickSort(lo, high);

    return TRUE;
}
