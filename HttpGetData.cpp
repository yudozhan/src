// HttpGetData.cpp: implementation of the HttpGetData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HttpGetData.h"

CHtmlGetSession::CHtmlGetSession(LPCTSTR pszAppName, int nMethod)
	: CInternetSession(pszAppName, 1, nMethod)
{
}

void CHtmlGetSession::OnStatusCallback(DWORD /* dwContext */, DWORD dwInternetStatus,
	LPVOID /* lpvStatusInfomration */, DWORD /* dwStatusInformationLen */)
{
//	if (!bProgressMode)
//		return;

	if (dwInternetStatus == INTERNET_STATUS_CONNECTED_TO_SERVER)
		TRACE("Connection made!");
}

/////////////////////////////////////////////////////////////////////////////
// CHtmlGetException -- used if something goes wrong for us

// TEAR will throw its own exception type to handle problems it might
// encounter while fulfilling the user's request.

IMPLEMENT_DYNCREATE(CHtmlGetException, CException)

CHtmlGetException::CHtmlGetException(int nCode)
	: m_nErrorCode(nCode)
{
}

void CHtmlGetException::ThrowTearException(int nCode)
{
	CHtmlGetException* pEx = new CHtmlGetException(nCode);
	throw pEx;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HttpGetData::HttpGetData()
{

}

HttpGetData::~HttpGetData()
{

}

CString HttpGetData::GetHtmlCode(CString url)
{
	DWORD   dwAccessType = PRE_CONFIG_INTERNET_ACCESS;

	DWORD dwHttpRequestFlags =
		INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT;

	const TCHAR szHeaders[] =
		_T("Accept: text/*\r\nUser-Agent: MFC_Tear_Sample\r\n");

	int nRetCode = 0;

	LPCTSTR pszURL = (LPCTSTR)url;
	CString s = "";
	CHtmlGetSession session(_T("TEAR - MFC Sample App"), dwAccessType);
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;
	try
	{
		// check to see if this is a reasonable URL

		CString strServerName;
		CString strObject;
		INTERNET_PORT nPort;
		DWORD dwServiceType;

		if (!AfxParseURL(pszURL, dwServiceType, strServerName, strObject, nPort) ||
			dwServiceType != INTERNET_SERVICE_HTTP)
		{
			TRACE("Error: can only use URLs beginning with http://") ;
			CHtmlGetException::ThrowTearException(1);
		}

//		if (bProgressMode)
		{
//			TRACE("Opening Internet...");
//			VERIFY(session.EnableStatusCallback(TRUE));
		}

		pServer = session.GetHttpConnection(strServerName, nPort);

		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
		pFile->AddRequestHeaders(szHeaders);
		pFile->SendRequest();

		DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);

		// if access was denied, prompt the user for the password

		if (dwRet == HTTP_STATUS_DENIED)
		{
			DWORD dwPrompt;
			dwPrompt = pFile->ErrorDlg(NULL, ERROR_INTERNET_INCORRECT_PASSWORD,
				FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL);

			// if the user cancelled the dialog, bail out

			if (dwPrompt != ERROR_INTERNET_FORCE_RETRY)
			{
				TRACE("Access denied: Invalid password\n");
				CHtmlGetException::ThrowTearException(1);
			}

			pFile->SendRequest();
			pFile->QueryInfoStatusCode(dwRet);
		}

		CString strNewLocation;
		pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);

		// were we redirected?
		// these response status codes come from WININET.H

		if (dwRet == HTTP_STATUS_MOVED ||
			dwRet == HTTP_STATUS_REDIRECT ||
			dwRet == HTTP_STATUS_REDIRECT_METHOD)
		{
			CString strNewLocation;
			pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strNewLocation);

			int nPlace = strNewLocation.Find(_T("Location: "));
			if (nPlace == -1)
			{
				TRACE("Error: Site redirects with no new location") ;
				CHtmlGetException::ThrowTearException(2);
			}

			strNewLocation = strNewLocation.Mid(nPlace + 10);
			nPlace = strNewLocation.Find('\n');
			if (nPlace > 0)
				strNewLocation = strNewLocation.Left(nPlace);

			// close up the redirected site

			pFile->Close();
			delete pFile;
			pServer->Close();
			delete pServer;

//			if (bProgressMode)
			{
//				TRACE("Caution: redirected to %s",strNewLocation);
			}

			// figure out what the old place was
			if (!AfxParseURL(strNewLocation, dwServiceType, strServerName, strObject, nPort))
			{
				TRACE("Error: the redirected URL could not be parsed.") ;
				CHtmlGetException::ThrowTearException(2);
			}

			if (dwServiceType != INTERNET_SERVICE_HTTP)
			{
				TRACE("Error: the redirected URL does not reference a HTTP resource.") ;
				CHtmlGetException::ThrowTearException(2);
			}

			// try again at the new location
			pServer = session.GetHttpConnection(strServerName, nPort);
			pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
				strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
			pFile->AddRequestHeaders(szHeaders);
			pFile->SendRequest();

			pFile->QueryInfoStatusCode(dwRet);
			if (dwRet != HTTP_STATUS_OK)
			{
				TRACE("Error: Got status code %d\n",dwRet)  ;
				CHtmlGetException::ThrowTearException(2);
			}
		}

		TRACE("Status Code is %d\n",dwRet) ;

		TCHAR sz[1024];
		
		while (pFile->ReadString(sz, 1023))
		{
			s+=sz;
//			if (bStripMode)
//				StripTags(sz);
		}

	// NOTE: Since HTTP servers normally spit back plain text, the
	// above code (which reads line by line) is just fine.  However,
	// other data sources (eg, FTP servers) might provide binary data
	// which should be handled a buffer at a time, like this:
/*
#if 0
		while (nRead > 0)
		{
			sz[nRead] = '\0';
			if (bStripMode)
				StripTags(sz);
			cout << sz;
			nRead = pFile->Read(sz, 1023);
		}
#endif
		*/

		pFile->Close();
		pServer->Close();
	}
	catch (CInternetException* pEx)
	{
		// catch errors from WinINet

		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);

		TRACE("Error: (%d) %s\n", pEx->m_dwError,szErr);

		nRetCode = 2;
		pEx->Delete();
	}
	catch (CHtmlGetException* pEx)
	{
		// catch things wrong with parameters, etc

		nRetCode = pEx->m_nErrorCode;
		TRACE1("Error: Exiting with CHtmlGetException(%d)\n", nRetCode);
		pEx->Delete();
	}

	if (pFile != NULL)
		delete pFile;
	if (pServer != NULL)
		delete pServer;
	session.Close();

	if(nRetCode!=0) s = "";
	return s;

}
