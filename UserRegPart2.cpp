#include "stdafx.h"
#include "UserRegPart2.h"
#include "secretdrvhyt.h"
#include "GetHardDiskID1.h"

CString strKey = _T("SOFTWARE\\158Stock\\Ver1.0");
CString strItem1 = _T("id");
CString strItem2 = _T("power");
CString strItem3 = _T("Pos");
HANDLE hDevice;
char g_chMachineID[16];

//����ֵ:-1��ʾ����, 0:��ʾδע��, 1:��ʾ��ע��
int IsUserRegisteredPart2(CTime &t)
{
/*	HKEY hKey;
	char pBuffer1[40];
	char pBuffer2[40];
	char pBuffer3[3] = "00";
	char IDE[3];
	char FirstDiskPos[3]="";
	DWORD dwCount1 = 40;
	DWORD dwCount2 = 40;
	DWORD dwCount3 = 3;
	CString strCredit;
	CString strSerial;
	CString strMachine;
	BOOL bSearchAll=FALSE;
	int nFlag=15;
	int nDelay=0;
	int i;

	if(ERROR_SUCCESS == RegCreateKey(HKEY_LOCAL_MACHINE, strKey,&hKey))
	{
		int cc=RegQueryValueEx(hKey, strItem1, 0, 0, (BYTE*)(pBuffer1), &dwCount1);
		if (cc!=ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return 0;
		}
		cc=RegQueryValueEx(hKey, strItem2, 0, 0, (BYTE *)(pBuffer2), &dwCount2);
		if (cc!=ERROR_SUCCESS)
		{
			RegCloseKey(hKey);
			return 0;
		}

		cc=RegQueryValueEx(hKey, strItem3, 0, 0, (BYTE*)(pBuffer3), &dwCount3);
		RegCloseKey(hKey);
		if (cc!=ERROR_SUCCESS)	//�����ǰû�м�¼Ӳ�̵�λ�ã�����Ҫ�������п��ܵ�Ӳ��
		{
			bSearchAll=TRUE;
		}
	}
	else
		return -1;

	if (((pBuffer3[0]!='0') && (pBuffer3[0]!='1')) || ((pBuffer3[1]!='0') && (pBuffer3[1]!='1')) || (pBuffer3[2]!=0))
		bSearchAll=TRUE;
	else
	{
		strcpy(IDE,pBuffer3);
	}

	strCredit = pBuffer2;	//��Ȩ��
	strSerial = pBuffer1;	//���к�
	if ((strCredit.GetLength()!=8) || (strSerial.GetLength()!=16))
		return 0;

#ifdef PRO
	//
	CSecretDrv2 secret;
	bool bSuccess;
	CString strTemp = secret.DesecretSerialID(strSerial, bSuccess);
	if (strTemp[0]==_T('3'))	//��ʾ��ǰ���к���OEM�����к�
		return -2;
#endif

	if (bSearchAll)
	{
		//�����������п��ܵ�Ӳ�̺ţ����Ƿ���ƥ���Ӳ��
		strcpy(IDE,"00");
		for(i=0;i<4;i++)
		{
			if (ReadDiskIDPart2(IDE,strMachine,nFlag,nDelay,t)==1)
			{
				if (IsCreditUserPart2(strSerial,strCredit,strMachine))
				{
					SetIDE(IDE);
					return 1;
				}
				else
				{
					return 0;
				}
			}
			NextIDEDevice(IDE,nFlag);
		}

		//���û��Ӳ�̺���ע�����ƥ��, ��ȡͨ��Ӳ�̺ţ����Ƿ���ע�������ƥ��
		strcpy(IDE,"22");	//ȡͨ��Ӳ�̺�
		ReadDiskIDPart2(IDE,strMachine,nFlag,0,t);
		if (IsCreditUserPart2(strSerial,strCredit,strMachine))
		{
			SetIDE(IDE);
			return 1;
		}

		//���ͨ��Ӳ�̺���ע�����Ҳ��ƥ�䣬
		//���ٴζ�ȡӲ�̺�,��ζ�ȡӲ�̺���Ҫ�ӳ�ʱ��
		nDelay=1;	//���̼��1��
		for(i=0;i<4;i++)
		{
			if (ReadDiskIDPart2(IDE,strMachine,nFlag,nDelay,t)==1)
			{
				if (IsCreditUserPart2(strSerial,strCredit,strMachine))
				{
					SetIDE(IDE);
					return 1;
				}
				else
				{
					return 0;
				}
			}
			NextIDEDevice(IDE,nFlag);
		}

		//�������ȡ��Ӳ�̺ţ�����Ҫע��
		return 0;
	}
	else
	{
		//���ȶ�ȡע����б�ǵ�Ӳ�̺ţ����Ƿ���ע�������ƥ��
		nDelay=0;
		bSearchAll=FALSE;
		if (ReadDiskIDPart2(IDE,strMachine,nFlag,nDelay,t)==1)
		{
			if (IsCreditUserPart2(strSerial,strCredit,strMachine))
			{
				return 1;
			}
			bSearchAll=TRUE;
		}

		//�ӳ�����ʱ��
		if (!bSearchAll)
		{
			nDelay=1;	//���̼��1��
			if (ReadDiskIDPart2(IDE,strMachine,nFlag,nDelay,t)==1)
			{
				if (IsCreditUserPart2(strSerial,strCredit,strMachine))
				{
					return 1;
				}
				bSearchAll=TRUE;
			}
		}

		//��һ���ӳ�����ʱ��
		if (!bSearchAll)
		{
			nDelay=3;	//���̼��3��
			if (ReadDiskIDPart2(IDE,strMachine,nFlag,nDelay,t)==1)
			{
				if (IsCreditUserPart2(strSerial,strCredit,strMachine))
				{
					return 1;
				}
				bSearchAll=TRUE;
			}
		}

		//������е�����˵�������ֿ���,һ����ע�����ָ����Ӳ�̲���ȷ,����ָ����Ӳ�̲��ܶ���Ӳ�̺�
		//���ھ�Ҫ�������е�Ӳ�̣����Ƿ���ƥ���Ӳ��
		strcpy(IDE,"00");
		for(i=0;i<4;i++)
		{
			if (ReadDiskIDPart2(IDE,strMachine,nFlag,nDelay,t)==1)
			{
				if (IsCreditUserPart2(strSerial,strCredit,strMachine))
				{
					SetIDE(IDE);
					return 1;
				}
				else
				{
					return 0;
				}
			}
			NextIDEDevice(IDE,nFlag);
		}

		//���û��Ӳ�̺���ע�����ƥ��, ��ȡͨ��Ӳ�̺ţ����Ƿ���ע�������ƥ��
		strcpy(IDE,"22");	//ȡͨ��Ӳ�̺�
		ReadDiskIDPart2(IDE,strMachine,nFlag,0,t);
		if (IsCreditUserPart2(strSerial,strCredit,strMachine))
		{
			SetIDE(IDE);
			return 1;
		}

		//���ͨ��Ӳ�̺���ע�����Ҳ��ƥ�䣬
		//���ٴζ�ȡӲ�̺�,��ζ�ȡӲ�̺���Ҫ�ӳ�ʱ��
		nDelay=1;
		for(i=0;i<4;i++)
		{
			if (ReadDiskIDPart2(IDE,strMachine,nFlag,nDelay,t)==1)
			{
				if (IsCreditUserPart2(strSerial,strCredit,strMachine))
				{
					SetIDE(IDE);
					return 1;
				}
				else
				{
					return 0;
				}
			}
			NextIDEDevice(IDE,nFlag);
		}

		//�������ȡ��Ӳ�̺ţ�����Ҫע��
//		AfxMessageBox("����ȡ�û�����!");
		return 0;
	}*/
	return 0;
}

bool GetDiskIDPart2(CString &s, const char* IDE, int &nFlag, CTime &t)
{
	BOOL bRet;
	DWORD cbRet;
	int nTryTimes=12;
	char buf[524]; //must be 512 bytes
	int i,j;
	char pBuf[3];
	unsigned char nDevice;

	ASSERT(strlen(IDE)==2);
	strcpy(pBuf,IDE);

	//���ö�ʱ��
	for (i=0;i<nTryTimes;i++)
	{
		buf[0] = 1;
		bRet = DeviceIoControl(hDevice,	(DWORD)0x222001, buf, sizeof(buf), NULL, 0, &cbRet, NULL);
		if (bRet)
			break;
	}
	if (i==nTryTimes)
		return false;

	//��ȡӲ�̺�
	buf[0]=(pBuf[0]=='0') ? 0x00 : 0x01;
	buf[1]=(pBuf[1]=='0') ? 0x00 : 0x01;
	for (i=0;i<nTryTimes;i++)
	{
		bRet = DeviceIoControl(hDevice, (DWORD)0x222000, buf, sizeof(buf),buf,sizeof(buf),&cbRet,NULL);
		if (bRet && cbRet)
			break;
	}

	if (i==nTryTimes)	//���ʼ�ճɹ���ȡIDE�豸���������˳�������Ӳ��æ��
		return false;

	for (i=0;i<524;i++)
		buf[i]=~buf[i];

	nDevice=(unsigned char)(buf[0]);
	if ((nDevice & 0x40)!=0x40)	//���� not removable controller and/or device
	{
		return false;
	}
	else if((nDevice & 0x80)==0x80)	//��removable media device
	{
		return false;
	}

	char str[61];
	for(i=20;i<40;i++) 
		if(i%2==0)
			str[i-20]=buf[i+1];
		else 
			str[i-20]=buf[i-1];

	for (i=54; i<92; i++)
		if (i%2==0)
			str[i-34]=buf[i+1];
		else
			str[i-34]=buf[i-1];
	str[60]=0;

	//�涨Ӳ�̺���ֻ����'a'--'z','A'--'Z','0'--'9'���ַ��������ַ�Ҫ���˵�
	j=0;
	for (i=0; i<60; i++)
	{
		if ((str[i]>='a' && str[i]<='z') || (str[i]>='A' && str[i]<='Z') || (str[i]>='0' && str[i]<='9'))
		{
			str[j++] = str[i];
		}
	}

	//Ӳ�̺ų��ȵ���Ϊ8λ
	if (j<8)
	{
		for (i=j; i<8; i++)
			str[i]='0';
	}
	str[8]=0;
	for(i = 0;i<8;i++)
	{
		BYTE nn = str[i];
		nn = nn%10;
		str[i] = nn + '0';
	}


	s=str;		//����Ӳ�̺�

	//����ʱ��
	int nYear,nMonth,nDay,nHour,nMinute,nSecond;
	nSecond=((buf[512] & 0xF0) >> 4) * 10 + (buf[512] & 0x0F); 
	nMinute=((buf[514] & 0xF0) >> 4) * 10 + (buf[514] & 0x0F); 
	nHour=((buf[516] & 0xF0) >> 4) * 10 + (buf[516] & 0x0F); 
	nDay=((buf[519] & 0xF0) >> 4) * 10 + (buf[519] & 0x0F); 
	nMonth = ((buf[520] & 0xF0) >> 4) * 10 + (buf[520] & 0x0F); 
	nYear=(((buf[522] & 0xF0) >> 4) * 10 + (buf[522] & 0x0F)) * 100 + ((buf[521] & 0xF0) >> 4) * 10 + (buf[521] & 0x0F); 
	try
	{
		CTime t1=CTime::GetCurrentTime();
		t=CTime(t1.GetYear(), nMonth, nDay, nHour, nMinute, nSecond);
		if (t<0)
			t=CTime::GetCurrentTime();
	}
	catch(...)
	{
		t=CTime::GetCurrentTime();
	}
	return true;
}


long ReadDiskIDPart2(const char* IDE, CString &strMachine, int &nFlag, int nDelay, CTime &t)
{
	//���Ŷ�nTryTimes��Ӳ�̺ţ��Ƚ����ڵļ��Σ�
	//�����ͬ�����ʾ�õ���ȷ��Ӳ�̺�,������Ϊ���ܵõ�Ӳ�̺�
	int nTryTimes=10;
	CString strT1=_T("");
	CString strT2=_T("");
	int nSameTimes=0;
	int nMaxSameTimes=1;
	bool bResult;
	int rtn = 0;
	int i;
	//
	if (strcmp(IDE,"22")==0)
	{
		strMachine = "20020101";
		rtn = 1;
		goto END_READ;
	}
	else
	{
		for (i=0;i<nTryTimes;i++)
		{
			bResult=GetDiskIDPart2(strT2,IDE,nFlag,t);
			if (bResult)
			{
				if (strT1 != strT2) 
				{
					nSameTimes=0;
					strT1=strT2;
				}
				else if (++nSameTimes > nMaxSameTimes)
					break;
			}	
			else
			{
				if (i<(nTryTimes-1))
				{
					if (nDelay>0)
						Sleep(nDelay * 1000);
				}
			}
		}
		if (i<nTryTimes)
		{
			strMachine = strT1;
			rtn = 1;
			goto END_READ;
		}
	}
	rtn = 0;
END_READ:

	strcpy(g_chMachineID,strMachine);
	return rtn;
}


BOOL IsCreditUserPart2(CString & strSerial,CString &strCredit,CString &strMachine)
{
	CString strCreditNew;
	CSecretDrv2 secret;
	bool bSuccess;
	if (strSerial.GetLength()!=16)
		return FALSE;
	if (strCredit.GetLength()!=8)
		return FALSE;
	CString strTemp = secret.DesecretSerialID(strSerial, bSuccess);
	if (!bSuccess) 
		return FALSE;
	if (!secret.CreateImpowerID(strTemp,strMachine,strCreditNew))
		return FALSE;
	strCreditNew.MakeUpper();
	strTemp=strCredit;
	strTemp.MakeUpper();
	if (strCreditNew==strTemp)
	{
		return TRUE;
	}
	else
	{
//		AfxMessageBox("IsCreditUserPart2 =0 !");
		return FALSE;
	}
}

BOOL OpenDiskVxdOrSys()
{
	//�����Windows NT/2000��ϵͳװ��ע�����õ�lmbdrv.sys
	//�����Windows 9x, ϵͳװ��ע�����õ�lmbdrv.vxd
	DWORD dwVersion = GetVersion();
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));

	if (dwVersion < 0x80000000)                // Windows NT/2000
	{
		hDevice = CreateFile("\\\\.\\LMBDRV", GENERIC_WRITE | GENERIC_READ,
						  FILE_SHARE_WRITE | FILE_SHARE_READ,
						  NULL, OPEN_EXISTING, 0, NULL);
	}
	else if (dwWindowsMajorVersion > 3)			// Windows 95/98
	{
		hDevice = CreateFile("\\\\.\\LMBVXD.VXD",0,0,NULL, 
					0,FILE_FLAG_DELETE_ON_CLOSE,NULL); 
	}
	else										// Win32s ��ϵͳ��֧��Win32s
		return FALSE;

	if (hDevice == INVALID_HANDLE_VALUE)
		return FALSE;
	else
		return TRUE;
}

void CloseDiskVxdOrSys()
{
	CloseHandle(hDevice);
}
long ReadDiskIDPartCwd()
{
	//���Ŷ�nTryTimes��Ӳ�̺ţ��Ƚ����ڵļ��Σ�
	//�����ͬ�����ʾ�õ���ȷ��Ӳ�̺�,������Ϊ���ܵõ�Ӳ�̺�
	CString sError;
	CString s = CGetHardDiskID::GetID(sError);
/*
    DWORD nVolumeSerialNumber=0;
    DWORD nMaximumComponentLength=0;
    DWORD nFileSystemFlags=0;
	char ch[64]="c:\\";
	char chVolumeNameBuffer[128];
	char chFileSystemNameBuffer[128];
	::GetVolumeInformation(ch,chVolumeNameBuffer,128,&nVolumeSerialNumber
		,&nMaximumComponentLength,&nFileSystemFlags,
		chFileSystemNameBuffer,128);

	nVolumeSerialNumber+=12345678;
	nVolumeSerialNumber%=100000000;
	CString s;
	s.Format ("%d",nVolumeSerialNumber);
	*/
	strcpy(g_chMachineID,s);
	return 1;
}
