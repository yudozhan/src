long APIENTRY UserRegistered(HWND, long *, LPSTR);
long APIENTRY IsUserRegistered(HWND, long *, LPSTR);
long APIENTRY Addwallpaper(HWND hWnd, long *lpValue, char* strFileName);
long APIENTRY GetWinVersion();
void GetID(int *);
bool GetDiskID(CString &, char*IDE, int& nFlag);
long ReadDiskID(CString &,char*);
BOOL IsCreditUser(CString & strSerial,CString &strCredit,CString &strMachine);
BOOL OnlineRegister(HWND hWnd, CString &strSerial, CString &strMachine,char* pIDE);
BOOL ManualRegister(HWND hWnd, CString &strSerial, CString &strMachine, CString &strCredit,char* pIDE);
BOOL QueryIDE(char*IDE);
BOOL SetIDE(char*IDE);
void NextIDEDevice(char*IDE, int nFlag);