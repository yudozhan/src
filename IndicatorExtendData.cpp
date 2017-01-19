// IndicatorExtendData.cpp: implementation of the IndicatorExtendData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndicatorExtendData.h"
#include "FileExtendData.h"
#include "FileInOutData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int IndicatorExtendData::m_nMaxCountRecord = 48*100;
CString IndicatorExtendData::m_sCurrentDirctory = "";

IndicatorExtendData::IndicatorExtendData()
{
}

IndicatorExtendData::~IndicatorExtendData()
{

}
void IndicatorExtendData::SetMaxCountRecord(int nMax)
{
	m_nMaxCountRecord = nMax;
}
//指标扩展数据导入导出，
void IndicatorExtendData::WriteSymbolFile(CString sFileName,CString& sArr,int time)
{
}
bool IndicatorExtendData::SetSymbolFile(CString sFileName) 
{
	return true;
}
bool IndicatorExtendData::ImportData(CString sFileName,CProgressCtrl* pCtrl)
{
	FileInOutData out;
	return out.ImportData( sFileName,pCtrl) ;
}
bool IndicatorExtendData::ExportData(CStringArray& sFileName,int* pIndicatorExtendID,int nDays) //nDays ==-1 is total
{
	FileInOutData out;
	return out.ExportData( sFileName, pIndicatorExtendID, nDays)  ;
}
bool IndicatorExtendData::ExportData(CStringArray& sFileName,int* pIndicatorExtendID,CTime& tmBegin,CTime& tmEnd) 
{
	FileInOutData out;
	return out.ExportData( sFileName, pIndicatorExtendID, tmBegin,tmEnd)  ;
}

//指标扩展数据写入，
int IndicatorExtendData::WriteData(RecordIndicator* pData,CString sSymbol,int nIndicatorExtendID,int nCount) 
{
	FileExtendData fl;
	return fl.WriteData (pData,sSymbol,nIndicatorExtendID,nCount);
}
int IndicatorExtendData::ReadData(RecordIndicator*& pData,CString sSymbol,int nIndicatorExtendID,int nDays) //nDays ==-1 is total
{
	FileExtendData fl;
	return fl.ReadData (pData,sSymbol,nIndicatorExtendID,nDays);
}
int IndicatorExtendData::ReadData(RecordIndicator*& pData,CString sSymbol,int nIndicatorExtendID,CTime& tmBegin,CTime& tmEnd) 
{
	FileExtendData fl;
	return fl.ReadData (pData,sSymbol,nIndicatorExtendID,tmBegin,tmEnd);
}

bool IndicatorExtendData::CreateDirectoryToday(bool bClear)
{
	char ch[512];
	::GetCurrentDirectory (512,ch);
	m_sCurrentDirctory = ch;
	m_sCurrentDirctory.TrimRight ("\\");
	m_sCurrentDirctory.TrimRight ("/");
	m_sCurrentDirctory+="\\";
	if(bClear == true)
	{
		char rmdirectory[255];
		SHFILEOPSTRUCT FileOp;
		FileOp.hwnd=NULL;
		FileOp.wFunc=FO_DELETE;
		FileOp.pFrom=rmdirectory;
		FileOp.pTo=NULL;
		FileOp.fFlags=FOF_FILESONLY|FOF_SILENT|FOF_NOCONFIRMATION  ;
		FileOp.fAnyOperationsAborted=NULL;
		FileOp.hNameMappings=NULL; 
		FileOp.lpszProgressTitle=NULL; 
		sprintf(rmdirectory,"%sUpload\\*.*",m_sCurrentDirctory);
		rmdirectory[strlen(rmdirectory)]='\0';
		rmdirectory[strlen(rmdirectory)+1]='\0';
		SHFileOperation(&FileOp);

		sprintf(rmdirectory,"%sUpload\\min\\*.*",m_sCurrentDirctory);
		rmdirectory[strlen(rmdirectory)]='\0';
		rmdirectory[strlen(rmdirectory)+1]='\0';
		SHFileOperation(&FileOp);

		sprintf(rmdirectory,"%sUpload\\indicator\\*.*",m_sCurrentDirctory);
		rmdirectory[strlen(rmdirectory)]='\0';
		rmdirectory[strlen(rmdirectory)+1]='\0';
		SHFileOperation(&FileOp);

		sprintf(rmdirectory,"%sUpload\\indicatorReal\\*.*",m_sCurrentDirctory);
		rmdirectory[strlen(rmdirectory)]='\0';
		rmdirectory[strlen(rmdirectory)+1]='\0';
		SHFileOperation(&FileOp);

		sprintf(rmdirectory,"%sUpload\\other\\*.*",m_sCurrentDirctory);
		rmdirectory[strlen(rmdirectory)]='\0';
		rmdirectory[strlen(rmdirectory)+1]='\0';
		SHFileOperation(&FileOp);
	}

	try{
	::CreateDirectory ("Upload",NULL);
	::CreateDirectory ("Upload\\min",NULL);
	::CreateDirectory ("Upload\\indicator",NULL);
	::CreateDirectory ("Upload\\indicatorReal",NULL);
	::CreateDirectory ("Upload\\other",NULL);
	}catch(...)
	{
		ASSERT(FALSE);
	}
	return true;
}
bool IndicatorExtendData::ExportRealMin5(CStringArray& sSymbol,float * fValue,int nIndicatorExtendID,int nRecord)
{
	FileInOutData out;
	return out.ExportRealMin5( sSymbol, fValue, nIndicatorExtendID, nRecord);
}
void IndicatorExtendData::GetIndicatorInfo(indicatorInfoArr &infoArr)
{
	FileExtendData out;
	out.GetIndicatorInfo(infoArr);
}
