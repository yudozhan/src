// AfficheFile.h: interface for the CAfficheFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AFFICHEFILE_H__E2EED2F8_9BA5_4CC2_B26A_39B6DA1981E0__INCLUDED_)
#define AFX_AFFICHEFILE_H__E2EED2F8_9BA5_4CC2_B26A_39B6DA1981E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct _tagAfficheFile
{
	CString		sCode;								// code
	CString		sHead;								// Title
	CString		sTime;								// Date
	CString		sContext;							// Context

	int GetSize()
	{
		int size = sHead.GetLength() + sTime.GetLength() + sContext.GetLength();
		return size;
	};
}AFFICHEFILE;
#define PAFFICHEFILE AFFICHEFILE*

class CAfficheFile  
{
public:
	CAfficheFile(LPCTSTR lpsz = NULL);
	virtual ~CAfficheFile();
// Arribute
public:
	CFile m_File;												// file object
	CMap <int, int, PAFFICHEFILE, PAFFICHEFILE> m_FileMap;		// file map

protected:
	CString sFileName;				// file name		
	BOOL	m_bOpen;

// Implement
public:
	void OnAddNew(char* lpsz, int size);
	void OnDeleteAll();
	void OnRefashFile();
	void SetFileName(LPCTSTR lpsz);



protected:
	BOOL OnOpenFile();
	PAFFICHEFILE OnTransStruct(char *lp, int size);
	void OnRemoveFileAll();

};

#endif // !defined(AFX_AFFICHEFILE_H__E2EED2F8_9BA5_4CC2_B26A_39B6DA1981E0__INCLUDED_)
