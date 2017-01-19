#include "stdafx.h"
#include "Global.h"

void ProcMessage()
{
	MSG msg;
	while(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	{
		if(msg.message == WM_QUIT)
		{
			::PostQuitMessage(-1);
		}
		if(!AfxGetApp()->PreTranslateMessage(&msg))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		AfxGetApp()->OnIdle(0);
		AfxGetApp()->OnIdle(1);
	}
}
template<class DATATYPE>
BOOL GetMemory(int iLineNum, UINT uiSection,DATATYPE**& pData)
{
	UINT i,j;
	pData = (DATATYPE**)malloc(iLineNum * sizeof(DATATYPE*));
	if(!pData)
	{
		AfxMessageBox("分配内存错误!");
		return FALSE;
	}
	for(i = 0;i < iLineNum;i++)
	{
		*(pData + i) = (DATATYPE*)malloc(uiSection * sizeof(DATATYPE));
		if(!*(pData + i))
		{
			for(j = 0;j < i;j++)
			{
				free(*(pData + j));
			}
			free(pData);
			pData = NULL;
			AfxMessageBox("分配内存错误!");
			return FALSE;
		}
	}
	for(i = 0;i < iLineNum;i++)
		::ZeroMemory(*(pData + i),uiSection);
	return TRUE;
}

template<class DATATYPE>
BOOL ReleaseMemory(int iLineNum, DATATYPE**& pData)
{
	int j;
	if(pData)
	{
		for(j = 0;j < iLineNum;j++)
		{
			free(*(pData + j));
		}
		free(pData);
		pData = NULL;
	}
	return 1;
}
template BOOL GetMemory(int , UINT ,float**& );
template BOOL ReleaseMemory(int , float**& );

template BOOL GetMemory(int , UINT ,int**& );
template BOOL ReleaseMemory(int , int**& );

template BOOL GetMemory(int , UINT ,short**& );
template BOOL ReleaseMemory(int , short**& );

template BOOL GetMemory(int , UINT ,BYTE**& );
template BOOL ReleaseMemory(int , BYTE**& );
//大小比较
int compare(const void * value0,const void * value1)
{
	if(*(double*)value0 < *(double*)value1)
		return -1;
	else if(*(double*)value0 == *(double*)value1)
		return 0;
	else
		return 1;
}

