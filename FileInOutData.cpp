// FileInOutData.cpp: implementation of the FileInOutData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileInOutData.h"
#include "FileExtendData.h"
#include "TechFileExtData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CArray<CString,CString&> FileInOutData::m_stockSymbolArr;
int FileInOutData::m_stockSymbolTime = 3600*1000;

FileInOutData::FileInOutData()
{

}

FileInOutData::~FileInOutData()
{

}
//指标扩展数据导入导出，
void FileInOutData::WriteSymbolFile(CString sFileName,CString& sArr,int time)
{
}
bool FileInOutData::SetSymbolFile(CString sFileName) 
{
/*					//
	m_stockSymbolArr.RemoveAll ();
	CFile fl;
	if(fl.Open (sFileName,CFile::modeRead|CFile::shareDenyNone))
	{
		int n;
		int nR = fl.Read (&n,4);
		if(nR!=4) return false;
		int m_nStockCount;
		nR = fl.Read (&m_nStockCount,4);
		if(nR!=4) return false;
		n = fl.GetLength ();
		fl.Read (&m_stockSymbolTime,4);

		if(n<m_nStockCount*8+12) return false;

		for(int i=0;i<m_nStockCount;i++)
		{
			char ch[8];
			fl.Read (ch,8);
			ch[7] = '\0';
			CString s = ch;
			m_stockSymbolArr.Add(s);
		}
	}*/
	return true;
}
bool FileInOutData::ImportData(CString sFileName,CProgressCtrl* pCtrl) 
{
	FileExtendData fl;
//	int timeToday = FileExtendData::RegularTime(m_stockSymbolTime,true);

	CFile fl2;
	if(fl2.Open (sFileName,CFile::modeRead|CFile::shareDenyNone))
	{
		InOutHeadData head;

		head = GetHead(fl2);
		//read out
		fl2.Seek (sizeof(InOutHeadData),CFile::begin);
		fl.SetIndicatorInfo(head.m_info,head.m_info .m_nID);

		char chSymbol[8];

		if(pCtrl)
		{
			pCtrl->SetRange32 (0,16);
			pCtrl->SetPos (0);
		}
		int k = 0;
		while(true)
		{
			int read = fl2.Read (chSymbol,8);
			if(read!=8) break;
			int nRecord = 0;
			read = fl2.Read (&nRecord,4);
			if(read!=4) break;
			if(nRecord<=0) 
			{
				ASSERT(FALSE);
				continue;
			}
			ASSERT(nRecord<18000);

			chSymbol[7] = '\0';
			CString s = chSymbol;
			RecordIndicator* pData = new RecordIndicator[nRecord];
			read = fl2.Read (pData,nRecord*sizeof(RecordIndicator));
			if(read!=nRecord*sizeof(RecordIndicator)) 
			{
				delete [] pData;
				break;
			}
			fl.WriteData (pData,s,head.m_info .m_nID ,nRecord);
			delete [] pData;

			if(pCtrl)
			{
				k++;
				int nk = k/100;
				nk%=17;
				pCtrl->SetPos (nk);
			}
		}

	}

	return true;
}
bool FileInOutData::ExportData(CStringArray& sFileName,int* pIndicatorExtendID,int nDays) //nDays ==-1 is total
{
	bool b = ExportData( sFileName, pIndicatorExtendID,nDays,NULL,NULL);
	return b;
}
bool FileInOutData::ExportData(CStringArray& sFileName,int* pIndicatorExtendID,CTime& tmBegin,CTime& tmEnd) 
{
	bool b = ExportData( sFileName, pIndicatorExtendID,0,&tmBegin,&tmEnd);
	return b;
}
bool FileInOutData::IsBrief(CString sFileName) 
{
/*	int n = sFileName.GetLength ();
	if(n<7) 
	{
		ASSERT(FALSE);
		return false;
	}

	CString s = sFileName.Right (6);
	if(s.CompareNoCase (".extio") == 0)
		return false;*/
	return true;
}
InOutHeadData FileInOutData::GetHead(CFile & fl)
{
	InOutHeadData head;
	fl.SeekToBegin ();
	fl.Read (&head,sizeof(InOutHeadData));
	return head;
}
void	FileInOutData::SetHead(InOutHeadData& head,CFile & fl)
{
	fl.SeekToBegin ();
	fl.Write (&head,sizeof(InOutHeadData));
}
bool FileInOutData::ExportData(CStringArray& sFileName,int* pIndicatorExtendID,int nDays,CTime* tmBegin,CTime* tmEnd)
{
	CTime t = CTime::GetCurrentTime();
    CTime t3(2003, 1, 30, 0, 0, 0 );
	if(t >= t3)
	{
		return false;
	}
	int n = sFileName.GetSize ();
	FileExtendData fl;
	RecordIndicator* pData = NULL;
//	int timeToday = FileExtendData::RegularTime(m_stockSymbolTime,true);

	for(int i = 0;i<n;i++)
	{
		CFile fl2;
		if(fl2.Open (sFileName[i],CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
		{
			InOutHeadData head;

			fl.GetIndicatorInfo(head.m_info ,pIndicatorExtendID[i]);
			int n2 = nDays;
			if(head.m_info.m_nKlineKind == 1)
				n2*=48;
			if(n2<=-1) n2 = -1;

			//read out
			fl2.Seek (sizeof(InOutHeadData),CFile::begin);

			CTechFileExtData::g_fileExtData.Open (pIndicatorExtendID[i],CTechFileExtData::modeReadWrite);
			int nStock = CTechFileExtData::g_fileExtData.GetStockNumber ();
			for(int j = 0;j<nStock;j++)
			{
				int nRead = 0;
				CString sSm = CTechFileExtData::g_fileExtData.GetSymbol (j);
				if(tmBegin)
					nRead = fl.ReadData ( pData,sSm ,pIndicatorExtendID[i],*tmBegin,*tmEnd);
				else
					nRead = fl.ReadData ( pData,sSm ,pIndicatorExtendID[i],n2);
				if(nRead>0)
				{
					fl2.Write (sSm.GetBuffer (0),8);
					fl2.Write (&nRead,4);
					fl2.Write (pData,sizeof(RecordIndicator)*nRead);
				}
			}

			FileInOutData::SetHead( head,fl2);
		}
	}

	if(pData) delete [] pData;

	return true;
}
bool FileInOutData::ExportRealMin5(CStringArray& sSymbol,float * fValue,int nIndicatorExtendID,int nRecord)
{
	ASSERT(FALSE);
	return false;

	CFile file;
	CString sName ;
	sName.Format ("%supload\\indicatorreal\\%dreal%d.extio", IndicatorExtendData::m_sCurrentDirctory,nIndicatorExtendID,nRecord);
	if(file.Open(sName,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone))
	{
		InOutHeadData head;
		FileExtendData fl;
		fl.GetIndicatorInfo(head.m_info ,nIndicatorExtendID);

		file.Write(&head,sizeof(InOutHeadData));
		int  n = sSymbol.GetSize ();
		int  nRecord = 1;

		CTime tm = CTime::GetCurrentTime ();
		int time = CTime(tm.GetYear (),tm.GetMonth(),tm.GetDay (),9,35,0).GetTime()+nRecord*5*60;
		if(nRecord>=24)
			time+=90*60;
		for(int  i =0;i<n;i++)
		{
			file.Write (sSymbol[i].GetBuffer (8),8);
			sSymbol[i].ReleaseBuffer ();
			file.Write (&nRecord,4);
			file.Write(&time,4);
			file.Write(fValue+i,4);
		}
		file.Close ();
	}
	return true;
}

bool FileInOutData::RemoveFiles(CString &sPath)
{
	// build a string with wildcards
	CString strWildcard;
	strWildcard = sPath+"*.*";
	CFileFind finder;

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
	   bWorking = finder.FindNextFile();

	   // skip . and .. files; otherwise, we'd
	   if (finder.IsDots())
		  continue;

	   if (finder.IsDirectory())
		   continue;

	   CString sName = finder.GetFilePath ();
	   CFile::Remove (sName);
	}

	return true;

}
