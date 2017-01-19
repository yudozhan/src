// GetSetReg.cpp: implementation of the GetSetReg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GetSetReg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GetSetReg::GetSetReg()
{

}

GetSetReg::~GetSetReg()
{

}

int GetSetReg::SetValue(CString strKey, CString strValueName, DWORD dwType, BYTE *lpData, DWORD cbData)
{
	HKEY hKey;
	CString Number;
	char str[20];
	int ec=0;
	DWORD dwDisposition;

//    strKey=_T("SOFTWARE\\SHIJILONG\\SkyerStartNow");
    ec = RegCreateKeyEx(
		 HKEY_LOCAL_MACHINE,
		 strKey,
		 0,
		 NULL,
		 REG_OPTION_NON_VOLATILE,
		 KEY_ALL_ACCESS,
		 NULL,
		 &hKey,
		 &dwDisposition);

	  if (ERROR_SUCCESS == ec)
	  {
		  //set value
		  ec = RegSetValueEx(
				 hKey,
				 strValueName,//"timeID",
				 0,
				 dwType,//REG_BINARY,
				 (BYTE *)lpData,
				 cbData);
		  if(ec!=ERROR_SUCCESS)
		  {
			  ASSERT(FALSE);
			  return ec;
		  }
	  }
	  else
		  return ec;
	  if(RegCloseKey(hKey)!=ERROR_SUCCESS)
		  ASSERT(FALSE);
	  return ec;
}

int GetSetReg::GetValue(CString strKey, CString strValueName, DWORD& dwType, BYTE *lpData, DWORD& cbData)
{
//	CString strKey = "SOFTWARE\\SHIJILONG\\SkyerStartNow";
	HKEY hKey;
	int ec=RegOpenKey(HKEY_LOCAL_MACHINE,(LPCTSTR)strKey,&hKey);
	if(ERROR_SUCCESS != ec)
	{
//		ASSERT(FALSE);
		return ec;
	}
//	ec=RegQueryValueEx(hKey,(LPCTSTR)strValueName,0,&dwType,(unsigned char *)lpData,&cbData);
	ec=RegQueryValueEx(hKey,(LPCTSTR)strValueName,0,&dwType,(unsigned char *)lpData,&cbData);
//	if(ec!=ERROR_SUCCESS)
//		ASSERT(FALSE);
	char ch[1024];
	int nSize;
	void* pVoid = NULL;
	FormatMessage(  FORMAT_MESSAGE_FROM_SYSTEM,      // source and processing options
	  (void*)pVoid,   // message source
	  ec,  // message identifier
	  0, // language identifier
	  ch,    // message buffer
	  1024,        // maximum size of message buffer
	  NULL  // array of message inserts
	  );
	int nn = GetLastError();
	RegCloseKey(hKey);
	return ec;

}

