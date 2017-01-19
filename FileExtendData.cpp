// FileExtendData.cpp: implementation of the FileExtendData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndicatorExtendData.h"
#include "FileExtendData.h"
#include "TechFileExtData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMap<int,int,IndicatorInfo*,IndicatorInfo*&> FileExtendData::m_map;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void FileExtendData::ClearMap()
{
	POSITION	pos=FileExtendData::m_map.GetStartPosition();
	while(pos)
	{
		IndicatorInfo* rValue;
		int rKey;
		FileExtendData::m_map.GetNextAssoc( pos, rKey,rValue ) ;
		delete rValue;
	}
	FileExtendData::m_map.RemoveAll();
}

FileExtendData::FileExtendData()
{

}

FileExtendData::~FileExtendData()
{

}
bool FileExtendData::GetIndicatorInfo(IndicatorInfo& data,int nIndicatorExtendID)
{
	IndicatorInfo* p = NULL;
	if(m_map.Lookup(nIndicatorExtendID,p))
		memcpy(&data,p,sizeof(IndicatorInfo));
	else//read from file
	{
		CFile fl;
		CString s;
		s.Format ("%d.ifo",nIndicatorExtendID);
		CString s2 = g_chIndicatorPath;
		if(fl.Open(s2+s,CFile::modeRead|CFile::shareDenyNone))
		{
			int  nRead = fl.Read (&data,sizeof(IndicatorInfo));
			if(nRead == sizeof(IndicatorInfo))
			{
				p = new IndicatorInfo;
				memcpy(p,&data,nRead);
				m_map[nIndicatorExtendID] = p;
			}
			fl.Close();
			return true;
		}
		return false;
	}
	return true;
}
bool FileExtendData::SetIndicatorInfo(IndicatorInfo& data,int nIndicatorExtendID)
{
	CFile fl;
	CString s;
	s.Format ("%d",nIndicatorExtendID);
	CString s2 = g_chIndicatorPath;
//	CreateDirectory(s2+s,NULL);
	IndicatorInfo* p = NULL;

	if(fl.Open(s2+s+".ifo",CFile::modeNoTruncate|CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		fl.Write (&data,sizeof(IndicatorInfo));

		//clear all
		ClearMap();

		fl.Close();
		return true;
	}
	return false;
}
//指标扩展数据写入，
int FileExtendData::WriteData(RecordIndicator* pData,CString sSymbol,int nIndicatorExtendID,int nCount) 
{
	return CTechFileExtData::WriteIn( pData, sSymbol, nIndicatorExtendID, nCount) ;
}
int FileExtendData::ReadData(RecordIndicator*& pData,CString sSymbol,int nIndicatorExtendID,int nDays) //nDays ==-1 is total
{
	return CTechFileExtData::ReadOut(pData, sSymbol, nIndicatorExtendID, nDays);//nDays ==-1 is total
}
int FileExtendData::ReadData(RecordIndicator*& pData,CString sSymbol,int nIndicatorExtendID,CTime& tmBegin,CTime& tmEnd) 
{
	int n = ReadData( pData, sSymbol, nIndicatorExtendID,-1);
	if(n<=0) return 0;

	bool bDay = true;
	IndicatorInfo data;
	if(GetIndicatorInfo( data, nIndicatorExtendID) == false)
		return 0;

	int b = tmBegin.GetTime();
	int e = tmEnd.GetTime();
	if(data.m_nKlineKind == 1) bDay = false;
	b = RegularTime(b,bDay);
	e = RegularTime(e,bDay);

	int b1 = -1;
	int e1 = -1;
	for(int i = 0;i<n;i++)
	{
		int time = RegularTime(pData[i].m_time ,bDay);
		if(time>=b && b1 == -1)
			b1 = i;
		if(time>e && e1 == -1)
			e1 = i;
	}
	if(b1 == -1) return 0;
	if(e1 == -1) e1 = n;

	int nValid = e1-b1;
	memmove(pData,pData+b1,sizeof(RecordIndicator)*nValid);
	;
	return nValid;
}
ExtHeadData	FileExtendData::GetFileHead(CFile& fl)
{
	ExtHeadData head;
	fl.SeekToBegin ();
	fl.Read (&head,sizeof(ExtHeadData));
	return head;
}
void	FileExtendData::SetFileHead(ExtHeadData& head,CFile& fl)
{
	ASSERT(FALSE);
}
bool	FileExtendData::GetFile(CFile& fl,CString sSymbol,int nIndicatorExtendID)
{
	ASSERT(FALSE);
	return false;
}
int FileExtendData::RegularTime(int time,bool bDay)
{
	if(bDay == false)
		return time;
	CTime tm(time);
	CTime tm2(tm.GetYear (),tm.GetMonth (),tm.GetDay (),8,0,0);
	return (int)(tm2.GetTime ());
}



void FileExtendData::GetIndicatorInfo(indicatorInfoArr &infoArr)
{
//	CString sParen;
//	CStringArray sArrOut;

	int i;

	CFileFind finder;

	// build a string with wildcards
	CString strWildcard = g_chIndicatorPath;
	strWildcard += _T("*.ifo");

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
	   bWorking = finder.FindNextFile();

	   // skip . and .. files; otherwise, we'd
	   if (finder.IsDots())
		  continue;

	   {
		  CString str = finder.GetFileName();
		  str.MakeLower();

		  str.TrimRight(".ifo");
		  int nIndicatorExtendID = atoi(str);

		  IndicatorInfo data;

		  bool b = GetIndicatorInfo( data, nIndicatorExtendID);
		  if(b)
		  {
			  int nAll = infoArr.GetSize ();
			  for(i=0;i<nAll;i++)
			  {
				  if(infoArr[i].m_nID > data.m_nID )
				  {
					  infoArr.InsertAt(i,data);
					  break;
				  }
				  else if(infoArr[i].m_nID == data.m_nID )
					  break;
			  }
			  if(i == nAll)
				  infoArr.Add(data);
		  }
	   }
	}

   finder.Close();

}
void FileExtendData::Remove(int nIndicatorExtendID)
{
	CTechFileExtData::CloseNow();

	ClearMap();

	CString strWildcard;
	strWildcard .Format ("%s%d",g_chIndicatorPath,nIndicatorExtendID);


	try
	{
	   CString str = strWildcard+".ifo";
	   CFile::Remove (str);
	   str = strWildcard+".ext";
	   CFile::Remove (str);
	}
	catch(...)
	{
	}

}
void FileExtendData::CloseNow()
{
	CTechFileExtData::CloseNow();
}