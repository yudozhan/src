

// FtpGetSend.cpp: implementation of the FtpGetSend class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FtpGetSend.h"
#include "FtpParaClass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL FtpGetSend::IsFree = FALSE;
CWnd* FtpGetSend::pFtpInfoView = NULL;
CWnd* FtpGetSend::pLocView = NULL;
CWnd* FtpGetSend::pServView = NULL;
char FtpGetSend::szAppName[256]="";
char FtpGetSend::szFtpName[256]="";
char FtpGetSend::szFtpUser[20]="";
char FtpGetSend::szFtpPassword[20]="";
char FtpGetSend::szFtpDirectory[MAX_PATH]="";
char FtpGetSend::szLocDirectory[MAX_PATH]="";
int FtpGetSend::nPort = 21;
int FtpGetSend::nCount = 0;

IMPLEMENT_DYNCREATE(FtpGetSend, CWinThread)

FtpGetSend::FtpGetSend()
{
}

FtpGetSend::~FtpGetSend()
{
}

BOOL FtpGetSend::InitInstance()
{
// TODO: perform and per-thread initialization here
return TRUE;
}

int FtpGetSend::ExitInstance()
{
// TODO: perform any per-thread cleanup here
return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(FtpGetSend, CWinThread)
//{{AFX_MSG_MAP(FtpGetSend)
// NOTE - the ClassWizard will add and remove mapping macros here.
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FtpGetSend message handlers

UINT FtpGetSend::FindLocFileThread(LPVOID lParam)
{
	ASSERT(FALSE);
	return 0;

	FtpParaClass:: FILE_FTP_INFO* pInfo=new (FtpParaClass::FILE_FTP_INFO);
	CFileFind fileFind;
	UINT i=0;
	CTime time;
	BOOL bContinue;
	::SetCurrentDirectory(szLocDirectory);
	SendMessageToLocalWnd(WM_SETDIRECTORYDISPLAY,0,0);
	bContinue=fileFind.FindFile("*.*");
	if(!bContinue)
	return 0L;
	CString str;
	while(bContinue)
	{
	bContinue=fileFind.FindNextFile();
	if(fileFind.IsHidden())continue;
	str=fileFind.GetFileName();
	strcpy(pInfo->szFileName,str);
	fileFind.GetLastWriteTime(time);
	str=time.Format("%x");
	strcpy(pInfo->szFileDate,str);
	if(fileFind.IsDirectory())
	{
	strcpy(pInfo->szFileSize,"");
	pInfo->nType=DIRECTORYICON;
	SendMessageToLocalWnd(WM_SETFILE,(WPARAM)pInfo,
	(LPARAM)LOCFILE);
	}
	else
	{
	int i=fileFind.GetLength();
	if(i>1024)
	{
	str.Format("%d",i/1024);
	str+="KB";
	}
	else
	str.Format("%d",i);
	strcpy(pInfo->szFileSize,str);
	pInfo->nType=FILEICON;
	SendMessageToLocalWnd(WM_SETFILE,(WPARAM)pInfo,
	(LPARAM)LOCFILE);
	}
	}
	SendMessageToLocalWnd(WM_SETREDRAWFLAG,0,0);
	delete pInfo;
	fileFind.Close();
	return 0L;
}

UINT FtpGetSend::FindServFileThread(LPVOID lParam)
{
FtpParaClass::FILE_FTP_INFO* pInfo=new(FtpParaClass::FILE_FTP_INFO);
FILETIME fileTime;
CString str,szFile,szFtpInfo;
WIN32_FIND_DATA findData;
HINTERNET hFind;
HINTERNET hInetSession;
HINTERNET hFtpConn;
if(!FtpGetSend::OpenInternet(hInetSession, hFtpConn))
return 0;
DWORD dwLength=MAX_PATH;
if(szFtpDirectory!=0)
FtpSetCurrentDirectory(hFtpConn,szFtpDirectory);
FtpGetCurrentDirectory(hFtpConn,szFtpDirectory,&dwLength);
if(pServView)
::PostMessage(pServView->m_hWnd,WM_SETDIRECTORYDISPLAY,0,0);
szFtpInfo="&Otilde;&yacute;&Ocirc;&Uacute;&sup2;¨¦&Otilde;&Ograve;&Icirc;&Auml;&frac14;&thorn;";
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
if(!(hFind=FtpFindFirstFile(hFtpConn,_T("*"),&findData,0,0)))
{
if (GetLastError() == ERROR_NO_MORE_FILES) 
{
AfxMessageBox("&Auml;&iquest;&Acirc;&frac14;&Icirc;&ordf;&iquest;&Otilde;&pound;&not;&Atilde;&raquo;&Oacute;&ETH;&para;¨¤&Oacute;¨¤&micro;&Auml;&Auml;&Uacute;&Egrave;&Yacute;");
goto end;
}
else
{
szFtpInfo="&Icirc;&Auml;&frac14;&thorn;&sup2;¨¦&Otilde;&Ograve;&sup3;&ouml;&acute;¨ª&pound;&not;&Iacute;&Euml;&raquo;&Oslash;&pound;&not;&Ccedil;&euml;&sup2;¨¦&Atilde;¡Â&Ocirc;&shy;&Ograve;¨°";
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
goto end;
}
}
do{
szFile=findData.cFileName;
strcpy(pInfo->szFileName,szFile);
fileTime=findData.ftLastWriteTime;
CTime time=CTime(fileTime);
CString str=time.Format("%x");
strcpy(pInfo->szFileDate,str);
if(findData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
{
strcpy(pInfo->szFileSize,"");
pInfo->nType=DIRECTORYICON;
}
else
{
DWORD i=findData.nFileSizeLow;
if(i>1024)
{
str.Format("%ld",i/1024);
str+="KB";
}
else
str.Format("%ld",i);
strcpy(pInfo->szFileSize,str);
pInfo->nType=FILEICON;
}
if(pServView)
::SendMessage(pServView->m_hWnd,WM_SETFILE,(WPARAM)pInfo,(LPARAM)SERVFILE);

}while(InternetFindNextFile(hFind,&findData));

if(pServView)
::SendMessage(pServView->m_hWnd,WM_SETREDRAWFLAG,0,0);
InternetCloseHandle(hFind);
end:
szFtpInfo="&sup2;¨¦&Otilde;&Ograve;&Icirc;&Auml;&frac14;&thorn;&frac12;¨¢&Ecirc;&oslash;";
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
::Sleep(10);
delete pInfo;
InternetCloseHandle(hFtpConn);
InternetCloseHandle(hInetSession);
return 0L; //Text_EditCopiesDiscWizardCopy
}

UINT FtpGetSend::SendFileThread(LPVOID lParam)
{
	IsFree=TRUE;
	FtpParaClass:: FILE_FTP_INFO* pInf=new (FtpParaClass::FILE_FTP_INFO);
	FtpParaClass::FILE_COUNT_INFO* pInfo=(FtpParaClass::FILE_COUNT_INFO*)lParam;
	BOOL Result;
	CInternetSession* lpInetSession;
	CFtpConnection* lpFtpConnection;
	CString str,strFtp,OldLoc,OldFtp,szFtpInfo;
	lpInetSession=new CInternetSession(szAppName,1,PRE_CONFIG_INTERNET_ACCESS);
	if(!lpInetSession)
	{
		if(pInfo) delete [] pInfo;
		if(pInf) delete pInf;
		
		return 0;
	}
	try
	{
		lpFtpConnection=lpInetSession->GetFtpConnection(szFtpName,szFtpUser,szFtpPassword,nPort);
	}
	catch(CInternetException* lpEx)
	{
//		lpEx->ReportError();
		lpEx->Delete();
		delete lpInetSession;
		if(pInfo) delete [] pInfo;
		if(pInf) delete pInf;
		return 0;
	}
	lpFtpConnection->SetCurrentDirectory(szFtpDirectory);
//	::SetCurrentDirectory(szLocDirectory);
	OldLoc=szLocDirectory;
	OldFtp=szFtpDirectory;
	for(int i=0;i<nCount;i++)
	{
		str=OldLoc;
		strFtp=OldFtp;
		CString DirName=pInfo[i].fileName;
		CString servFileName = GetFileNameOnly(DirName);
		if(pInfo[i].ufileFlag==FILEICON)
		{
			szFtpInfo="";
			szFtpInfo+=DirName;
			PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);

			if(lpFtpConnection->PutFile(DirName,servFileName))
			{
				if(IsFree)
				{
					IsFree=FALSE;
					strcpy(pInf->szFileName,DirName);
					strcpy(pInf->szFileDate,"");
					strcpy(pInf->szFileSize,"");
					pInf->nType=FILEICON;
					if(pServView)
						::PostMessage(pServView->m_hWnd,WM_SETFILE,(WPARAM)pInf,SERVFILE);
				}
				continue;
			}
			else
			{
				szFtpInfo="";
				szFtpInfo+=DirName;
				szFtpInfo+="";
				PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
				break;
			}
		}
		else
		{
		if(str.Right(1)=="\\")
			str+=servFileName;
		else
			str+="\\"+servFileName;
		if(strFtp.Right(1)=='/')
			strFtp+=servFileName;
		else
			strFtp+="/"+servFileName;
		if(Result=PreSendFile(lpFtpConnection,str,strFtp,(LPVOID)pInfo))
			continue;
		else
		{
		break;
		}
		}
	}

	szFtpInfo="&Icirc;&Auml;&frac14;&thorn;¡¤&cent;&Euml;&Iacute;&frac12;¨¢&Ecirc;&oslash;,&Iacute;&Euml;&raquo;&Oslash;";
	szFtpInfo+=OldFtp;
	PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	::Sleep(10);
	if(IsFree)
	if(pServView)
	::SendMessage(pServView->m_hWnd,WM_REDISPLAYFILE,(WPARAM)(LPCTSTR)OldFtp,0);
	
	if(pInfo)
	{
		if(nCount<=1)
			delete pInfo;
		else
			delete[] pInfo;
	}
 	delete pInf;
	delete lpFtpConnection;
	delete lpInetSession;
	return 0L;

}


BOOL FtpGetSend::PreSendFile(CFtpConnection* lpFtp,CString& str,CString& strFtp,LPVOID pInf)
{
	FtpParaClass:: FILE_COUNT_INFO* pInfo=(FtpParaClass::FILE_COUNT_INFO*)pInf;
	BOOL result;
	UINT uSize;
	CFileFind fileFind;
	HANDLE hHeap;
	LPSTR lpBuffer;
	CString lpBufferFtp,szFtpInfo;
	CFtpConnection* lpFtpConnection=lpFtp;
	uSize=(GetCurrentDirectory(0,NULL)*sizeof(TCHAR));
	hHeap=GetProcessHeap();
	lpBuffer=(LPSTR)HeapAlloc(hHeap,HEAP_ZERO_MEMORY,uSize);
	lpFtpConnection->GetCurrentDirectory(lpBufferFtp);
	GetCurrentDirectory(uSize,(LPTSTR)(LPCTSTR)lpBuffer);

//	static CString strLocalPre = "";
//	if(str!=strLocalPre)
	{
	//	SetCurrentDirectory(str);
		szFtpInfo="";
		szFtpInfo+=strFtp;
		szFtpInfo+="";
		PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
		lpFtpConnection->CreateDirectory(strFtp);
		lpFtpConnection->SetCurrentDirectory(strFtp);
		szFtpInfo="";
		szFtpInfo+=strFtp;
		szFtpInfo+="";
		PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
		if(pServView)
		::SendMessage(pServView->m_hWnd,WM_SETDIR,0,(LPARAM)(LPCTSTR)strFtp);;
	}
	UINT bContinue=fileFind.FindFile(str+"\\*");
	CString tFile;
	CString tFilePath;
	if(bContinue)
	{
	do
	{
		bContinue=fileFind.FindNextFile();
		tFile=fileFind.GetFileName();
		tFilePath = fileFind.GetFilePath ();

		if(tFile=="."||tFile==".."||fileFind.IsHidden())continue;
		if(fileFind.IsDirectory())
		{
			if(str.Right(1)=='\\')
			{
				if(strFtp.Right(1)=='/')
				PreSendFile(lpFtpConnection,str+tFile,strFtp+tFile,(LPVOID)pInfo);
				else
				PreSendFile(lpFtpConnection,str+tFile,strFtp+"/"+tFile,(LPVOID)pInfo);
			}
			else
			{
				if(strFtp.Right(1)=='/')
				result=PreSendFile(lpFtpConnection,str+"\\"+tFile,strFtp+tFile,(LPVOID)pInfo);
				else
				result=PreSendFile(lpFtpConnection,str+"\\"+tFile,strFtp+"/"+tFile,(LPVOID)pInfo);
			}
		}
		else
		{
			szFtpInfo="";
			szFtpInfo+=tFile;
			PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
			result=(lpFtpConnection->PutFile(tFilePath,tFile))?TRUE:FALSE;
			if(IsFree)
			{
				IsFree=FALSE;
/*				strcpy(pInfo->szFileName,tFile);
				strcpy(pInfo->szFileDate,"");
				strcpy(pInfo->szFileSize,"");
				pInfo->nType=FILEICON;
				if(pServView)
				::PostMessage(pServView->m_hWnd,WM_SETFILE,(WPARAM)pInf,SERVFILE);*/
			}
			continue;
		}
	}while(bContinue);

	fileFind.Close();
//	SetCurrentDirectory(lpBuffer);
	szFtpInfo="&Iacute;&Euml;&raquo;&Oslash;";
	szFtpInfo+=lpBufferFtp;
	PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	lpFtpConnection->SetCurrentDirectory(lpBufferFtp);
	if(pServView)
	::SendMessage(pServView->m_hWnd,WM_SETDIR,0,(LPARAM)(LPCTSTR)lpBufferFtp);
	HeapFree(hHeap,HEAP_NO_SERIALIZE,lpBuffer);
	return result;

	}
	fileFind.Close();
//	SetCurrentDirectory(lpBuffer);
	lpFtpConnection->SetCurrentDirectory(lpBufferFtp);
	if(pServView)
	::SendMessage(pServView->m_hWnd,WM_SETDIR,0,(LPARAM)(LPCTSTR)lpBufferFtp);
	HeapFree(hHeap,HEAP_NO_SERIALIZE,lpBuffer);
	return FALSE;
}

UINT FtpGetSend::ReceiveFileThread(LPVOID lParam)
{
IsFree=TRUE;
CString szFtpInfo;
FtpParaClass:: FILE_FTP_INFO* pInf=new (FtpParaClass::FILE_FTP_INFO);
FtpParaClass::FILE_COUNT_INFO* pInfo=(FtpParaClass::FILE_COUNT_INFO*)lParam;
BOOL Result;
CString str,strFtp,OldLoc,OldFtp;
HINTERNET hInetSession=InternetOpen(szAppName,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
HINTERNET hFtpConn=InternetConnect(hInetSession,szFtpName,nPort,
szFtpUser,szFtpPassword,INTERNET_SERVICE_FTP,INTERNET_FLAG_PASSIVE,0);
if(!hFtpConn)
{
AfxMessageBox("&frac12;¡§&Aacute;&cent;&Aacute;&ordf;&frac12;&Oacute;&sup3;&ouml;&acute;¨ª&pound;&not;¡¤&micro;&raquo;&Oslash;");
InternetCloseHandle(hInetSession);
return 0L;
}
szFtpInfo="&Otilde;&yacute;&Ocirc;&Uacute;&frac12;&oslash;&Egrave;&euml;";
szFtpInfo+=szFtpDirectory;
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
FtpSetCurrentDirectory(hFtpConn,szFtpDirectory);
::SetCurrentDirectory(szLocDirectory);
OldLoc=szLocDirectory;
OldFtp=szFtpDirectory;
for(int i=0;i<nCount;i++)
{
str=OldLoc;
strFtp=OldFtp;
CString DirName=pInfo[i].fileName;
if(pInfo[i].ufileFlag==FILEICON)
{
szFtpInfo="&Otilde;&yacute;&Ocirc;&Uacute;&frac12;&Oacute;&Ecirc;&Otilde;";
szFtpInfo+=DirName;
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
if(FtpGetFile(hFtpConn,DirName,DirName,FALSE,FILE_ATTRIBUTE_NORMAL,FTP_TRANSFER_TYPE_BINARY |
INTERNET_FLAG_NO_CACHE_WRITE,0))
{
if(IsFree)
{
IsFree=FALSE;
strcpy(pInf->szFileName,DirName);
strcpy(pInf->szFileDate,"");
strcpy(pInf->szFileSize,"");
pInf->nType=FILEICON;
SendMessageToLocalWnd(WM_SETFILE,(WPARAM)pInf,(LPARAM)SERVFILE);
}
szFtpInfo="&frac12;&Oacute;&Ecirc;&Otilde;";
szFtpInfo+=DirName;
szFtpInfo+="&Iacute;¨º¡À&Iuml;&pound;&not;&Iuml;&Acirc;&Ograve;&raquo;&cedil;&ouml;";
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
continue;
}
else
{
szFtpInfo="&frac12;&Oacute;&Ecirc;&Otilde;";
szFtpInfo+=DirName;
szFtpInfo+="&sup3;&ouml;&acute;¨ª&pound;&not;&Iacute;&Euml;&sup3;&ouml;&sup2;¨¦&Otilde;&Ograve;&Ocirc;&shy;&Ograve;¨°";
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
break;
}
}
else
{
if(str.Right(1)=="\\")
str+=DirName;
else
str+="\\"+DirName;
if(strFtp.Right(1)=='/')
strFtp+=DirName;
else
strFtp+="/"+DirName;
if(Result=PreReceiveFile(hInetSession,str,strFtp,(LPVOID)pInf))
continue;
else
{
AfxMessageBox("¡¤&cent;&Euml;&Iacute;&Oacute;&ETH;&acute;¨ª,&Ccedil;&euml;&sup2;¨¦&Atilde;¡Â&Ocirc;&shy;&Ograve;¨°");
break;
}
}
}
szFtpInfo="&Icirc;&Auml;&frac14;&thorn;&frac12;&Oacute;&Ecirc;&Otilde;&frac12;¨¢&Ecirc;&oslash;&pound;&not;&Iacute;&Euml;&raquo;&Oslash;";
szFtpInfo+=OldLoc;
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
::Sleep(20);
if(IsFree)
{
SendMessageToLocalWnd(WM_REDISPLAYFILE,(WPARAM)(LPCTSTR)OldLoc,0);
}

delete[] pInfo;
delete pInf;
InternetCloseHandle(hFtpConn);
InternetCloseHandle(hInetSession);
return 0L;

}

UINT FtpGetSend::DeleteFileThread(LPVOID lParam)
{
IsFree=TRUE;
CString szFtpInfo;
FtpParaClass::FILE_COUNT_INFO* pInfo=(FtpParaClass::FILE_COUNT_INFO*)lParam;
BOOL Result;
CString strFtp,OldFtp;
HINTERNET hInetSession;
HINTERNET hFtpConn;
if(!OpenInternet(hInetSession, hFtpConn))
return 0;

FtpSetCurrentDirectory(hFtpConn,szFtpDirectory);
OldFtp=szFtpDirectory;
for(int i=0;i<nCount;i++)
{
strFtp=OldFtp;
CString DirName=pInfo[i].fileName;
if(pInfo[i].ufileFlag==FILEICON)
{
szFtpInfo="&Otilde;&yacute;&Ocirc;&Uacute;&Eacute;&frac34;&sup3;&yacute;";
szFtpInfo+=DirName;
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
if(FtpDeleteFile(hFtpConn,DirName))
{
continue;
}
else
{
szFtpInfo="&Eacute;&frac34;&sup3;&yacute;";
szFtpInfo+=DirName;
szFtpInfo+="&sup3;&ouml;&acute;¨ª&pound;&not;¡¤&micro;&raquo;&Oslash;";
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
break;
}
}
else
{
if(strFtp.Right(1)=='/')
strFtp+=DirName;
else
strFtp+="/"+DirName;
if(Result=PreDeleteFile(hInetSession,strFtp,OldFtp))
continue;
else
{
AfxMessageBox("&Eacute;&frac34;&sup3;&yacute;&sup3;&ouml;&acute;¨ª,&Ccedil;&euml;&sup2;¨¦&Atilde;¡Â&Ocirc;&shy;&Ograve;¨°");
break;
}
}
}
szFtpInfo="&Icirc;&Auml;&frac14;&thorn;&Eacute;&frac34;&sup3;&yacute;&frac12;¨¢&Ecirc;&oslash;";
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
::Sleep(10);
if(IsFree)
if(pServView)
::SendMessage(pServView->m_hWnd,WM_REDISPLAYFILE,(WPARAM)(LPCTSTR)OldFtp,0);
delete[] pInfo;
InternetCloseHandle(hFtpConn);
InternetCloseHandle(hInetSession);
return 0L;

}

UINT FtpGetSend::CreateDirectorServThread(LPVOID lParam)
{
IsFree=TRUE;
CString szFtpInfo;
FtpParaClass::FILE_COUNT_INFO* pInfo=(FtpParaClass::FILE_COUNT_INFO*)lParam;
CString strFtp,OldFtp;
HINTERNET hInetSession;
HINTERNET hFtpConn;
if(!OpenInternet(hInetSession, hFtpConn))
return 0;

FtpSetCurrentDirectory(hFtpConn,szFtpDirectory);
OldFtp=szFtpDirectory;
int nCount2 = 1;
for(int i=0;i<nCount2;i++)
{
strFtp=OldFtp;
CString DirName=pInfo[i].fileName;
// if(pInfo[i].ufileFlag==FILEICON)
{
szFtpInfo="&Otilde;&yacute;&Ocirc;&Uacute;&acute;&acute;&frac12;¡§&ETH;&Acirc;&Auml;&iquest;&Acirc;&frac14;:";
szFtpInfo+=DirName;
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
// DirName = "test";
if(FtpCreateDirectory(hFtpConn,DirName))
{
continue;
}
else
{
szFtpInfo="&acute;&acute;&frac12;¡§&ETH;&Acirc;&Auml;&iquest;&Acirc;&frac14;";
szFtpInfo+=DirName;
szFtpInfo+="&sup3;&ouml;&acute;¨ª&pound;&not;¡¤&micro;&raquo;&Oslash;";
DWORD n = GetLastError();
char chp[512];
DWORD dw = 512;
InternetGetLastResponseInfo( &n,chp,&dw);
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
break;
}
}
}
szFtpInfo="&acute;&acute;&frac12;¡§&ETH;&Acirc;&Auml;&iquest;&Acirc;&frac14;&frac12;¨¢&Ecirc;&oslash;";
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
::Sleep(10);
if(IsFree)
if(pServView)
::SendMessage(pServView->m_hWnd,WM_REDISPLAYFILE,(WPARAM)(LPCTSTR)OldFtp,0);
delete[] pInfo;
InternetCloseHandle(hFtpConn);
InternetCloseHandle(hInetSession);
return 0L;

}

UINT FtpGetSend::RenameFileThread(LPVOID lParam)
{
CString szFtpInfo;
FtpParaClass::FILE_CHANGE_INFO* pInfo=(FtpParaClass::FILE_CHANGE_INFO*)lParam;
HINTERNET hInetSession;
HINTERNET hFtpConn;
if(!OpenInternet(hInetSession, hFtpConn))
return 0;
szFtpInfo="&Iuml;&Ouml;&frac12;&oslash;&ETH;&ETH;&cedil;&Auml;&Atilde;&ucirc;&sup2;&Ugrave;¡Á¡Â";
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
FtpSetCurrentDirectory(hFtpConn,szFtpDirectory);
szFtpInfo="&Otilde;&yacute;&Ocirc;&Uacute;¡ã&Ntilde;";
szFtpInfo+=pInfo->OldName;
szFtpInfo+="&cedil;&Auml;&Icirc;&ordf;";
szFtpInfo+=pInfo->NewName;
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
FtpRenameFile(hFtpConn,pInfo->OldName,pInfo->NewName);
szFtpInfo="&cedil;&Auml;&Atilde;&ucirc;&sup3;&Eacute;&sup1;&brvbar;";
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
::Sleep(10);
delete pInfo;
InternetCloseHandle(hFtpConn);
InternetCloseHandle(hInetSession);
return 0L;

}

BOOL FtpGetSend::PreDeleteFile(HINTERNET hInetSession, CString& strFtp,CString& OldDir)
{
CString szFtpInfo;
BOOL result;
DWORD dwSize=MAX_PATH;
CString tFile;
WIN32_FIND_DATA findData;
HINTERNET hFind;
HINTERNET hFtpConn=InternetConnect(hInetSession,szFtpName,nPort,szFtpUser,szFtpPassword,INTERNET_SERVICE_FTP,INTERNET_FLAG_PASSIVE,0);
if(!hFtpConn)
{
AfxMessageBox("&frac12;¡§&Aacute;&cent;&Aacute;&ordf;&frac12;&Oacute;&sup3;&ouml;&acute;¨ª&pound;&not;¡¤&micro;&raquo;&Oslash;");
InternetCloseHandle(hInetSession);
return FALSE;
}
szFtpInfo="&Otilde;&yacute;&Ocirc;&Uacute;&frac12;&oslash;&Egrave;&euml;";
szFtpInfo+=strFtp;
szFtpInfo+="&Auml;&iquest;&Acirc;&frac14;";
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
FtpSetCurrentDirectory(hFtpConn,strFtp);
szFtpInfo="&frac12;&oslash;&Egrave;&euml;";
szFtpInfo+=strFtp;
szFtpInfo+="&Auml;&iquest;&Acirc;&frac14;&sup3;&Eacute;&sup1;&brvbar;";
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
if(!(hFind=FtpFindFirstFile(hFtpConn,_T("*"),&findData,0,0)))
{
if (GetLastError()!= ERROR_NO_MORE_FILES)
result=FALSE;
else
result=TRUE;
goto end;
}
do
{
tFile=findData.cFileName;
if(tFile=="."||tFile=="..")continue;
if(findData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
{
if(!(result=PreDeleteFile(hInetSession,strFtp+"/"+tFile,strFtp)))
{
AfxMessageBox("&Eacute;&frac34;&sup3;&yacute;&sup3;&ouml;&acute;¨ª&pound;&not;¡¤&micro;&raquo;&Oslash;");
break;
}
}
else
{
szFtpInfo="&Otilde;&yacute;&Ocirc;&Uacute;&Eacute;&frac34;&sup3;&yacute;";
szFtpInfo+=tFile;
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
result=(FtpDeleteFile(hFtpConn,tFile))?TRUE:FALSE;
if(!result)
{
szFtpInfo="&Eacute;&frac34;&sup3;&yacute;"+tFile+"&sup3;&ouml;&acute;¨ª";
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
break;
}
}
}while(InternetFindNextFile(hFind,&findData));
FtpSetCurrentDirectory(hFtpConn,OldDir);
szFtpInfo="&Otilde;&yacute;&Ocirc;&Uacute;&Eacute;&frac34;&sup3;&yacute;"+strFtp+"&Auml;&iquest;&Acirc;&frac14;";
PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
FtpRemoveDirectory(hFtpConn,strFtp);
InternetCloseHandle(hFind);
InternetCloseHandle(hFtpConn);
return result;

end:
InternetCloseHandle(hFtpConn);
return result;

}
BOOL FtpGetSend::PreReceiveFile(HINTERNET hInetSession, CString& str,CString& strFtp,LPVOID pInf)
{
FtpParaClass:: FILE_FTP_INFO* pInfo=(FtpParaClass:: FILE_FTP_INFO*)pInf;
BOOL result;
UINT uSize;
DWORD dwSize=MAX_PATH;
HANDLE hHeap;
LPSTR lpBuffer;
CString lpBufferFtp,tFile;
WIN32_FIND_DATA findData;
HINTERNET hFind;
HINTERNET hFtpConn=InternetConnect(hInetSession,szFtpName,nPort,szFtpUser,szFtpPassword,INTERNET_SERVICE_FTP,INTERNET_FLAG_PASSIVE,0);
if(!hFtpConn)
{
AfxMessageBox("&frac12;¡§&Aacute;&cent;&Aacute;&ordf;&frac12;&Oacute;&sup3;&ouml;&acute;¨ª&pound;&not;¡¤&micro;&raquo;&Oslash;");
InternetCloseHandle(hInetSession);
return FALSE;
}
uSize=(GetCurrentDirectory(0,NULL)*sizeof(TCHAR));
hHeap=GetProcessHeap();
lpBuffer=(LPSTR)HeapAlloc(hHeap,HEAP_ZERO_MEMORY,uSize);
GetCurrentDirectory(uSize,lpBuffer);

FtpSetCurrentDirectory(hFtpConn,strFtp);
::CreateDirectory(str,NULL);
::SetCurrentDirectory(str); 
SendMessageToLocalWnd(WM_SETDIR,0,(LPARAM)(LPCTSTR)str);
if(!(hFind=FtpFindFirstFile(hFtpConn,_T("*"),&findData,0,0)))
{
if (GetLastError()!= ERROR_NO_MORE_FILES)
result=FALSE;
else
result=TRUE;
goto end;
}
do
{
tFile=findData.cFileName;
if(tFile=="."||tFile==".."||findData.dwFileAttributes==FILE_ATTRIBUTE_HIDDEN)continue;
if(findData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
{
if(!(result=PreReceiveFile(hInetSession,str+"\\"+tFile,strFtp+"/"+tFile,pInf)))
{
AfxMessageBox("&Icirc;&Auml;&frac14;&thorn;¡¤&cent;&Euml;&Iacute;&sup3;&ouml;&acute;¨ª&pound;&not;¡¤&micro;&raquo;&Oslash;");
break;
}
}
else
{
result=(FtpGetFile(hFtpConn,tFile,tFile,FALSE,FILE_ATTRIBUTE_NORMAL,FTP_TRANSFER_TYPE_BINARY|
INTERNET_FLAG_NO_CACHE_WRITE,0))?TRUE:FALSE;
if(!result)
{
AfxMessageBox("&Icirc;&Auml;&frac14;&thorn;¡¤&cent;&Euml;&Iacute;&sup3;&ouml;&acute;¨ª&pound;&not;¡¤&micro;&raquo;&Oslash;");
break;
}
if(IsFree)
{
IsFree=FALSE;
strcpy(pInfo->szFileName,tFile);
strcpy(pInfo->szFileDate,"");
strcpy(pInfo->szFileSize,"");
pInfo->nType=FILEICON;
SendMessageToLocalWnd(WM_SETFILE,(WPARAM)pInfo,SERVFILE);
}
}
}while(InternetFindNextFile(hFind,&findData));
SetCurrentDirectory(lpBuffer);
SendMessageToLocalWnd(WM_SETDIR,0,(LPARAM)lpBuffer);
InternetCloseHandle(hFind);
HeapFree(hHeap,HEAP_NO_SERIALIZE,lpBuffer);
InternetCloseHandle(hFtpConn);
return result;

end:
::SetCurrentDirectory(lpBuffer);
SendMessageToLocalWnd(WM_SETDIR,0,(LPARAM)lpBuffer);
HeapFree(hHeap,HEAP_NO_SERIALIZE,lpBuffer);
InternetCloseHandle(hFtpConn);
return result;
}

BOOL FtpGetSend::BeginAThread(AFX_THREADPROC pFunc,LPVOID lParam)
{
CWinThread * pThread;
if(pThread=AfxBeginThread(pFunc,lParam,THREAD_PRIORITY_ABOVE_NORMAL,
0,CREATE_SUSPENDED,NULL))
{
pThread->m_bAutoDelete=TRUE;
pThread->ResumeThread();
return TRUE;
}
return FALSE;
}
int FtpGetSend::OpenInternet(HINTERNET& hInetSession,HINTERNET& hFtpConn)
{
	CString szFtpInfo;
	hInetSession=InternetOpen(szAppName,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	szFtpInfo="";
	szFtpInfo+=szFtpName;
	szFtpInfo+="";
	PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	hFtpConn=InternetConnect(hInetSession,szFtpName,nPort,
	szFtpUser,szFtpPassword,INTERNET_SERVICE_FTP,INTERNET_FLAG_PASSIVE,0);

	if(!hFtpConn)
	{
		szFtpInfo=szFtpName;
		szFtpInfo+="¡¤&thorn;&Icirc;&ntilde;&AElig;¡Â&Icirc;&acute;&frac12;&Oacute;&Eacute;&Iuml;";
		PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
		InternetCloseHandle(hInetSession);
		::Sleep(10);
		return 0;
	}
	szFtpInfo=szFtpName;
	szFtpInfo+="";
	szFtpInfo+=szFtpUser;
	szFtpInfo+="";
	PostMessageToInfoWnd(WM_RECORDFTPINFO,0,(LPARAM)(LPCTSTR)szFtpInfo);
	return 1;

}
BOOL FtpGetSend::PostMessageToInfoWnd( UINT message, WPARAM wParam , LPARAM lParam )
{
	if(pFtpInfoView)
	{
	return pFtpInfoView->SendMessage(message,wParam,lParam);
	}
	else
	return FALSE;

}
BOOL FtpGetSend::SendMessageToLocalWnd( UINT message, WPARAM wParam , LPARAM lParam )
{
	if(pLocView)
	{
	return ::SendMessage(pLocView->m_hWnd,message,wParam,lParam);
	}
	else
	return FALSE;

}
 

void FtpGetSend::PrepareFileSend(FtpParaClass::FILE_COUNT_INFO *pInfo, int nItem,CString sLocalCurrentDirectory,CString sServerCurrentDirectory)
{
	CString str;
	POSITION iPos;
	FtpParaClass::FILE_COUNT_INFO* pFileCount=pInfo;
	strcpy(szAppName,AfxGetAppName());
//	strcpy(szFtpName,m_pMainFrame->GetFtpName());
//	strcpy(szFtpUser,m_pMainFrame->GetFtpUser());
//	nPort=m_pMainFrame->GetFtpPort();
//	strcpy(szFtpPassword,m_pMainFrame->GetFtpPassword());
	strcpy(szFtpDirectory,sServerCurrentDirectory);

	strcpy(szLocDirectory, sLocalCurrentDirectory);
	nCount=nItem;

	//begin upload
	BeginAThread((AFX_THREADPROC) FtpGetSend::SendFileThread,(LPVOID) pInfo);
}

CString FtpGetSend::GetFileNameOnly(CString sFileFullPath)
{
	int n1 = sFileFullPath.ReverseFind ('\\');
	int n2 = sFileFullPath.ReverseFind ('/');
	CString s = sFileFullPath;
	if(n2>n1)
		n1 = n2;
	if(n1 == -1)
	{
		ASSERT(FALSE);
		return s;
	}
	ASSERT(sFileFullPath.GetLength ()>n1);

	s = sFileFullPath.Right (sFileFullPath.GetLength ()-n1-1);
	return s;

}
