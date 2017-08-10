/*
本源码由TC简单软件科技有限公司开源,功能可以自由修改、发布、
长沙简单软件科技有限公司对于源码不做后期维护,,请大家在使用过程中遵循开源协议
*/
#include "StdAfx.h"
#include <time.h>
#include "TSRuntime.h"
#include "DXBind.h"
//#include "TSHookFuntion.h"
#include <winioctl.h>
#include <TLHELP32.H>
#include "TSMyKernel32DllFuntion.h"
#include "TSPlugInterFace.h"
#include <dinput.h>

MyMouseNamespace    TSRuntime::g_Mouse;
MyKeyboardNamespace TSRuntime::g_KeyBoard;
DXBind TSRuntime::g_DxObj; 
CMessageData *TSRuntime::pData=NULL;
HANDLE TSRuntime::MapViewThread=NULL;
DWORD TSRuntime::m_displayMode=0;
DWORD   TSRuntime::m_dwExStyle=0;
wchar_t TSRuntime::RetStr[MAX_PATH*4]={0};
wchar_t TSRuntime::SetPath[MAX_PATH]={0};
LPOLESTR TSRuntime::KeyPressCharMap[0x100]={0};	  //256个键位
BYTE TSRuntime::VirtualToASCIIMap[0x100]={0};  //256个键位
BYTE TSRuntime::keyMap[0x100]={0};
bool TSRuntime::IsVipLogin=false;
bool TSRuntime::IsRegLogin=false;
bool TSRuntime::IsStartProtect=false;
bool TSRuntime::g_IsSYSKEY=false;  //按下系统键例如:ATL
pMyDictInfo TSRuntime::MyDictCount[20]={0}; //存储当前所有字库,默认最大字库个数为20个
int TSRuntime::nMaxStrHight[20]={0}; //记录每个字库中,最高的字高.
int TSRuntime::nMyDictCountsize[20]={0}; //记录每个字库的数量
int TSRuntime::NowUsingDictIndex=-1;//记录当前正在使用字库的下标
char TSRuntime::bytestr[1024*3]={0};//存储点阵二进制信息最大存储为250*11
bool TSRuntime::IsVirtualProtectEx=false;//是否驱动模式远程修改注入进程的页面保护属性
HWND TSRuntime::hookHwnd=0;//记录要注入的窗口句柄
HMEMORYLOADER TSRuntime::myUser32module=NULL;
HMEMORYLOADER TSRuntime::mykernel32module=NULL;
bool TSRuntime::IsInitialmyUser32module=false;
bool TSRuntime::IsInitialmykernel32module=false;
wchar_t TSRuntime::membstr[MAX_PATH*400]={0};
bool TSRuntime::IsShowErrorMsg=TRUE;
bool TSRuntime::IsNpProtect=false;
BYTE TSRuntime::KeyPressMap[0x100]={0};
BYTE TSRuntime::KeyPressMap_Mouse[0x100]={0};
BYTE TSRuntime::RawInputKeyPressMap[0x100]={0};
wchar_t  TSRuntime::Regcode[MAX_PATH]={0};//记录注册码信息
wchar_t  TSRuntime::VipCountCode[MAX_PATH]={0};//记录VIP用户账户信息
DWORD TSRuntime::g_nChecktime=0;
HANDLE TSRuntime::h_checkthread=NULL;   //验证线程句柄
INT64  TSRuntime::checkthreadtime=0;//校验线程时间
//CComBSTR TSRuntime::newbstr;
COLORREF TSRuntime::pLoadstrBuffer[11][MAX_PATH]={0};//存储字库点阵还原图片颜色矩阵COLORREF
DWORD TSRuntime::ZwProtectVirtualMemoryRet=0;
bool TSRuntime::IsWin7X64=false;
bool TSRuntime::IsWin8X64=false;
DWORD TSRuntime::ProtectPid[20]={0};
DWORD TSRuntime::IsProid=0;
wchar_t TSRuntime::SetDictPwdString[MAX_PATH]={0};
wchar_t TSRuntime::SetPicPwdString[MAX_PATH]={0};
wchar_t TSRuntime::MachineCode[MAX_PATH]={0};
bool TSRuntime::IsLoginlog=false;
BYTE TSRuntime::SetSimModeType=0;
bool TSRuntime::g_IsLoadDrive=false;

extern mySleep my_sleep;
extern myGetTickCount my_myGetTickCount;
extern  TCHAR gDLLFolder[MAX_PATH + 1];

void CheckThread(void *para)//验证线程
{
	VMPBEGIN  //VMP保护
		INT64 lasttime=(DWORD)para;//获取注册码剩余时间
	INT64 threadtime=0;
	while(TRUE)
	{
		if(my_myGetTickCount)
		{
			TSRuntime::checkthreadtime=my_myGetTickCount();
			if(threadtime==0)
				threadtime=my_myGetTickCount();
		}
		else
		{
			TSRuntime::checkthreadtime=::GetTickCount();
			if(threadtime==0)
				threadtime=::GetTickCount();
		}

		if(TSRuntime::IsVipLogin==false&&lasttime==0) //试用验证
		{
			if((TSRuntime::checkthreadtime-threadtime)>=60*1000*30)//试用30分钟后停止服务
				//if((TSRuntime::checkthreadtime-threadtime)>=60*1000)//试用1分钟后停止服务
			{
				threadtime=0;//重置时间
				TSRuntime::checkthreadtime=0;
				if(TSRuntime::IsStartProtect)
					TSRuntime::StartProtect(0,0,-1);
				//TSRuntime::h_checkthread=NULL;
				return;
			}
		}
		else  //注册码验证
		{
			INT64 time1=(lasttime*60*60*1000+threadtime)-TSRuntime::checkthreadtime;
			if(time1<=0)//注册码返回的是小时,然后换算为毫秒
			{
				threadtime=0;//重置时间
				TSRuntime::checkthreadtime=0;
				if(TSRuntime::IsStartProtect)
					TSRuntime::StartProtect(0,0,-1);
				//TSRuntime::h_checkthread=NULL;
				return;
			}
		}

		//if(my_sleep)
		//	my_sleep(60*1000);   //1分钟校验一次
		//else
		//	::Sleep(60*1000);	  //1分钟校验一次
		if(my_sleep)
			my_sleep(60*1000*5);   //5分钟校验一次
		else
			::Sleep(60*1000*5);	  //5分钟校验一次
	}
	VMPEND
}

BOOL CALLBACK MyEnumProc(HWND hWnd,LPARAM lParam) 
{
	DWORD dwProcessId;
	GetWindowThreadProcessId(hWnd,&dwProcessId);
	LPWNDINFO pInfo=(LPWNDINFO)lParam;
	if(dwProcessId==pInfo->dwProcessId)
	{
		pInfo->hWnd=hWnd;
		return FALSE; 
	}
	return TRUE;
}
HWND  TSRuntime::GetProcessMainWnd(DWORD  dwProcessId)	 //获取自身窗口句柄
{
	WNDINFO  wi;
	wi.dwProcessId=dwProcessId;
	wi.hWnd=NULL;
	EnumWindows(MyEnumProc,(LPARAM)&wi);
	return wi.hWnd;
}

bool TSRuntime::GetRemoteModulePath(const wchar_t *module, short nPID,wchar_t *modulepath)
{
	HANDLE tlh = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, nPID);
	MODULEENTRY32 modEntry;
	modEntry.dwSize = sizeof(MODULEENTRY32);

	Module32First(tlh, &modEntry);
	do
	{
		if(!wcsicmp(module, modEntry.szModule))
		{
			CloseHandle(tlh);
			wcscpy(modulepath,modEntry.szExePath);
			return TRUE;
		}
	}
	while(Module32Next(tlh, &modEntry));
	CloseHandle(tlh);

	return FALSE;
}

DWORD TSRuntime::GetRemoteModuleBaseAddr(const wchar_t *module, short nPID)
{
	HANDLE tlh = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, nPID);
	MODULEENTRY32 modEntry;
	modEntry.dwSize = sizeof(MODULEENTRY32);

	Module32First(tlh, &modEntry);
	do
	{
		if(!wcsicmp(module, modEntry.szModule))
		{
			CloseHandle(tlh);
			return (DWORD)modEntry.modBaseAddr;
		}
	}
	while(Module32Next(tlh, &modEntry));
	CloseHandle(tlh);

	return NULL;
}

//获取指定进程ID的DLL模块地址
HMODULE TSRuntime::GetRemoteModuleHandle(const wchar_t *module, short nPID)
{
	HANDLE tlh = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, nPID);
	MODULEENTRY32 modEntry;
	modEntry.dwSize = sizeof(MODULEENTRY32);

	Module32First(tlh, &modEntry);
	do
	{
		if(!wcsicmp(module, modEntry.szModule))
		{
			CloseHandle(tlh);
			return modEntry.hModule;
		}
	}
	while(Module32Next(tlh, &modEntry));
	CloseHandle(tlh);

	return NULL;
}

//获取指定进程ID的DLL模块地址
HMODULE TSRuntime::GetRemoteModuleBase(const wchar_t *module, short nPID)
{
	HANDLE tlh = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, nPID);
	MODULEENTRY32 modEntry;
	modEntry.dwSize = sizeof(MODULEENTRY32);

	Module32First(tlh, &modEntry);
	do
	{
		if(!wcsicmp(module, modEntry.szModule))
		{
			CloseHandle(tlh);
			return (HMODULE)modEntry.modBaseAddr;
		}
	}
	while(Module32Next(tlh, &modEntry));
	CloseHandle(tlh);

	return NULL;
}

//获取指定进程ID的DLL模块函数的地址
FARPROC TSRuntime::GetRemoteProcAddress( wchar_t *module, const char *func, short nPID)
{
	HMODULE remoteMod = GetRemoteModuleHandle(module, nPID);
	HMODULE localMod = GetModuleHandle(module);

	//	If the module isn't already loaded, we load it, but since many of the 
	//	modules we'll probably be loading will do nasty things like modify
	//	memory and hook functions, we use the DONT_RESOLVE_DLL_REFERENCES flag,
	//	so that LoadLibraryEx only loads the dll, but doesn't execute it.
	if(!localMod) localMod = LoadLibraryEx(module, NULL, DONT_RESOLVE_DLL_REFERENCES);

	//	Account for potential differences in base address
	//	of modules in different processes.
	int delta = MakeDelta(int, remoteMod, localMod);

	FARPROC LocalFunctionAddress = GetProcAddress(localMod, func);

	return MakePtr(FARPROC, LocalFunctionAddress, delta);
}

BOOL TSRuntime::MByteToWChar(LPCSTR lpcszStr,LPWSTR lpwszStr,DWORD dwSize)
{
	DWORD dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_ACP, 0, lpcszStr, -1, NULL, 0);
	if(dwSize < dwMinSize)
	{
		return FALSE;
	}
	MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize); 
	return TRUE;
}

BOOL TSRuntime::WCharToMByte(LPCWSTR lpcwszStr,LPSTR lpszStr,DWORD dwSize)
{
	DWORD dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,NULL,0,NULL,FALSE);
	if(dwSize < dwMinSize)
	{
		return FALSE;
	}
	WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,lpszStr,dwSize,NULL,FALSE);
	return TRUE;
}

bool TSRuntime::MyLoadLibrary()
{
	return true;
}

////重载系统模块DLL进入系统,重新定位模块DLL的函数地址,然后返回新的函数地址达到防HOOK的步骤
DWORD TSRuntime::GetPassR3HookFuntionAddr(char *modename,char*funname) 
{

	return	true;
}

//驱动层写内存
bool  TSRuntime::MyWriteMemory(IN PWriteMemoryInfo WriteInfo)
{

	return 1;

}

bool TSRuntime::StartProtect(long enable,long type,int Pid)
{

	return 0;
	
}

bool TSRuntime::GetComPath(TCHAR * Path)
{
	// TODO: 在此添加控件通知处理程序代码
	CLSID clsid;
	IDispatch   *pDisp;
	HRESULT hr=NULL;
	hr=CLSIDFromProgID(L"TS.TSsoft",&clsid);
	wchar_t compath[MAX_PATH]={0};
	if(hr==S_OK)
	{
		LPCTSTR data_Set=L"CLSID\\";

		TCHAR clsidstr[MAX_PATH]={0};

		TCHAR clsidreg[MAX_PATH]={0};

		HKEY hKey;

		DWORD size=MAX_PATH*sizeof(TCHAR),dwType=0;;

		::swprintf(clsidstr,L"{%X-%X-%X-%X-%X%X}",	clsid.Data1,   clsid.Data2,   clsid.Data3,   
			clsid.Data4[0]<<8|clsid.Data4[1], 
			clsid.Data4[2]<<8|clsid.Data4[3], 
			clsid.Data4[4]<<24|clsid.Data4[5]<<16| 
			clsid.Data4[6]<<8|clsid.Data4[7]);

		swprintf(clsidreg,L"%s%s\\InprocServer32",data_Set,clsidstr);

		if(RegOpenKey(HKEY_CLASSES_ROOT,clsidreg,&hKey)!=ERROR_SUCCESS)
			//if(RegOpenKeyEx(HKEY_CLASSES_ROOT, clsidreg, 0, KEY_WRITE|KEY_READ,&hKey)!=ERROR_SUCCESS);
			return 0;

		if(RegQueryValue(hKey,L"",Path,(PLONG)&size)!=ERROR_SUCCESS)
			//if(RegQueryValueA(hKey,"",compath,(PLONG)&size)!=ERROR_SUCCESS)
			//if(RegQueryValueEx(hKey,_T(""),NULL,&dwType,(LPBYTE)compath,&size)!=ERROR_SUCCESS);
			return 0;

		RegCloseKey(hKey);
	}

	return compath;

}

int TSRuntime::VipLogin(wchar_t *regcode,int type,bool isvip)
{
	if(regcode==NULL||type<0||type>1)
	{
		return false;
	}
	VMPBEGIN   //VMP保护
		int ret=0;
	if(TSRuntime::h_checkthread==NULL||isvip==true)//第一次试用或者验证注册验证码的时候连接服务器
		
	if(TSRuntime::h_checkthread==NULL||isvip==true)	//试用或者验证注册码的时候   启用验证线程
	{
		if(TSRuntime::h_checkthread==NULL&&isvip==false&&ret>0)//第一次试用
			TSRuntime::h_checkthread=(HANDLE)_beginthread(CheckThread, 0 ,0 );
		else if(true==isvip&&ret>0)
		{
			if(TSRuntime::h_checkthread!=NULL)//如果启用了验证线程
			{
				::TerminateThread(TSRuntime::h_checkthread,0);//结束试用验证线程
				TSRuntime::h_checkthread=NULL;
			}
			if(type==1) //注册码验证线程
				TSRuntime::h_checkthread=(HANDLE)_beginthread(CheckThread, 0 ,(void*)ret);

		}
		Sleep(50);//等待线程函数启动后在返回
	}

	VMPEND
	return ret;
	

}

void TSRuntime::my_split(TCHAR* sourcestr, TCHAR *oldstr, TCHAR (*temp)[28], int &rlen)
{
	TCHAR sourcebak[1024]={0};//不改变源串拷贝一份
	wcscpy(sourcebak,sourcestr);
	TCHAR *pch=wcstok(sourcebak,oldstr);
	rlen=0;
	while(pch!=NULL)
	{
		wcscpy(temp[rlen],pch);
		pch=wcstok(NULL,oldstr);
		rlen++;
	}
}

bool TSRuntime::EnablePrivilege(LPCTSTR pszPrivName, bool fEnable ) 
{    
	bool fOk = false;    
	// Assume function fails    
	HANDLE hToken;    
	// Try to open this process's access token   
	if (OpenProcessToken(GetCurrentProcess(),   
		TOKEN_ADJUST_PRIVILEGES, &hToken))  
	{            
		TOKEN_PRIVILEGES tp = { 1 };       

		//如果成功查找到特权值
		if( LookupPrivilegeValue(NULL, pszPrivName, &tp.Privileges[0].Luid) )
		{
			tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED : 0;

			AdjustTokenPrivileges(hToken, false, &tp,          
				sizeof(tp), NULL, NULL);

			fOk = (GetLastError() == ERROR_SUCCESS);  
		}
		CloseHandle(hToken); 
	} 
	return(fOk);
}

int TSRuntime::InitialWindowsVersion()
{
	OSVERSIONINFO Version;
	ZeroMemory(&Version,sizeof(OSVERSIONINFO));
	Version.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&Version); 
	if (Version.dwPlatformId==VER_PLATFORM_WIN32_NT)  
	{
		if((Version.dwMajorVersion==5)&&(Version.dwMinorVersion==0))
		{
			return 0;//Win2000;
		}else if((Version.dwMajorVersion==5)&&(Version.dwMinorVersion==1))
		{
			return 1; //WinXP;
		}else if((Version.dwMajorVersion==5)&&(Version.dwMinorVersion==2))
		{
			return 2; //Win2003;
		}else if((Version.dwMajorVersion==6)&&(Version.dwMinorVersion==0))
		{
			return 3; //VISTA;
		}
		else if((Version.dwMajorVersion>=6))
		{
			BOOL bIsWow64 = FALSE;  

			typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);  
			LPFN_ISWOW64PROCESS pfnIsWow64 = NULL;  

			pfnIsWow64 = (LPFN_ISWOW64PROCESS)GetProcAddress(  
				GetModuleHandle(_T("kernel32.dll")), "IsWow64Process");  
			if (pfnIsWow64)  
			{  
				pfnIsWow64(GetCurrentProcess(), &bIsWow64);  
			} 

			if(Version.dwMinorVersion==2&&bIsWow64==false)   //WIN8
				return 5;
			else if(Version.dwMinorVersion==2&&bIsWow64==true)
			{
				TSRuntime::IsWin8X64=true;
				return 5;
			}
			//如果是win7_64系统ret返回true
			if(bIsWow64)
				TSRuntime::IsWin7X64=true;//win7x64
			return 4;   //win7x64
		}
	}
	return -1;
}

void  TSRuntime::add_log( const char * fmt, ... )
{
	va_list		va_alist; 
	char		buf[256]; 
	char		logbuf[1024]; 
	char		cDirectory[600];
	FILE*		file; 
	struct tm*	current_tm; 
	time_t	current_time;

	time( &current_time ); 
	current_tm = localtime( &current_time );
	sprintf( logbuf, "//%02d:%02d:%02d: ", current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec );
	va_start( va_alist, fmt ); 
	vsprintf( buf, fmt, va_alist ); 
	va_end( va_alist );

	strcat( logbuf, buf ); 
	strcat( logbuf, "\n" );
	strcpy( cDirectory, "d:\\" );
	strcat( cDirectory, "Tha Log.txt" );

	if( ( file = fopen( cDirectory, "a+" ) ) != NULL )
	{
		fputs( logbuf, file ); 
		fclose( file );
	}
}

int TSRuntime::retrunLparamDown(int key)
{
	int lparamdown=0X00000000;
	switch(key)
	{
	case VK_RETURN:lparamdown=0X001C0001;break;
	case VK_ESCAPE:lparamdown=0X00010001;break;

		//按下大写键
	case VK_CAPITAL:lparamdown=0X003A0001;break;
	case VK_BACK:lparamdown=0X000E0001;break;
	case VK_TAB:lparamdown=0X000F0001;break;
	case VK_SPACE:lparamdown=0X00390001;break;
	case VK_CONTROL:lparamdown=0X001D0001;break;

	case 'A':lparamdown=0X001E0001;break;
	case 'D':lparamdown=0X00200001;break;
	case 'S':lparamdown=0X001F0001;break;
	case 'Q':lparamdown=0X00100001;break;
	case 'W':lparamdown=0X00110001;break;
	case 'E':lparamdown=0X00120001;break;
	case 'V':lparamdown=0X002F0001;break;
	case 'R':lparamdown=0X00130001;break;
	case 'Y':lparamdown=0X00150001;break;
	case 'U':lparamdown=0X00160001;break;
	case 'I':lparamdown=0X00170001;break;
	case 'O':lparamdown=0X00180001;break;
	case 'P':lparamdown=0X00190001;break;
	case 'T':lparamdown=0X00140001;break;
	case 'F':lparamdown=0X00210001;break;
	case 'H':lparamdown=0X00230001;break;
	case 'J':lparamdown=0X00240001;break;
	case 'C':lparamdown=0X002E0001;break;
	case 'K':lparamdown=0X00250001;break;
	case 'B':lparamdown=0X00300001;break;
	case 'L':lparamdown=0X00260001;break;
	case 'M':lparamdown=0X00320001;break;
	case 'Z':lparamdown=0X002C0001;break;
	case 'X':lparamdown=0X002D0001;break;

	case '1':lparamdown=0X00020001;break;
	case '2':lparamdown=0X00030001;break;
	case '3':lparamdown=0X00040001;break;
	case '4':lparamdown=0X00050001;break;
	case '5':lparamdown=0X00060001;break;
	case '6':lparamdown=0X00070001;break;
	case '7':lparamdown=0X00080001;break;
	case '8':lparamdown=0X00090001;break;
	case '9':lparamdown=0X000A0001;break;
	case '0':lparamdown=0X000B0001;break;

	case VK_F1:lparamdown=0X003B0001;break;
	case VK_F2:lparamdown=0X003C0001;break;
	case VK_F3:lparamdown=0X003D0001;break;
	case VK_F4:lparamdown=0X003E0001;break;
	case VK_F5:lparamdown=0X003F0001;break;
	case VK_F6:lparamdown=0X00400001;break;
	case VK_F7:lparamdown=0X00410001;break;
	case VK_F8:lparamdown=0X00420001;break;
	case VK_F9:lparamdown=0X00430001;break;
	case VK_F10:lparamdown=0X00440001;break;
	case VK_F11:lparamdown=0X00570001;break;
	case VK_F12:lparamdown=0X00580001;break;

	case VK_NUMPAD1:lparamdown=0X004F0001;break;
	case VK_NUMPAD2:lparamdown=0X00500001;break;
	case VK_NUMPAD3:lparamdown=0X00510001;break;
	case VK_NUMPAD4:lparamdown=0X004B0001;break;
	case VK_NUMPAD5:lparamdown=0X004C0001;break;
	case VK_NUMPAD6:lparamdown=0X004D0001;break;
	case VK_NUMPAD7:lparamdown=0X00470001;break;
	case VK_NUMPAD8:lparamdown=0X00480001;break;
	case VK_NUMPAD9:lparamdown=0X00490001;break;
	case VK_NUMPAD0:lparamdown=0X00520001;break;

	case VK_UP:lparamdown=0X01480001;break;
	case VK_DOWN:lparamdown=0X01500001;break;
	case VK_LEFT:lparamdown=0X014B0001;break;
	case VK_RIGHT:lparamdown=0X014D0001;break;
	case VK_SHIFT:lparamdown=0X002A0001;break;
	case VK_MENU:lparamdown=0X20380001;break;

	case VK_NUMLOCK:lparamdown=0X01450001;break;
	case VK_DIVIDE:lparamdown=0X01350001;break;
	case VK_MULTIPLY:lparamdown=0X00370001;break;
	case VK_SUBTRACT:lparamdown=0X004A0001;break;

	case VK_ADD:lparamdown=0X004E0001;break;
	case VK_DECIMAL:lparamdown=0X00110001;break;
	case VK_OEM_MINUS:lparamdown=0X000C0001;break;
	case VK_OEM_PLUS:lparamdown=0X000D0001;break;
	case VK_OEM_3:lparamdown=0X00290001;break;
	case VK_OEM_5:lparamdown=0X002B0001;break;

	}
	return lparamdown;
}

void TSRuntime::InitKeyPressMap()
{
	ZeroMemory(KeyPressMap,0x100);
	ZeroMemory(KeyPressMap_Mouse,0x100);
}

void TSRuntime::InitKeyMap()
{
	ZeroMemory(keyMap,0x100);
	keyMap[0x2] ='1';
	keyMap[0x3] ='2';
	keyMap[0x4] ='3';
	keyMap[0x5] ='4';
	keyMap[0x6] ='5';
	keyMap[0x7] ='6';
	keyMap[0x8] ='7';
	keyMap[0x9] ='8';
	keyMap[0xa] ='9';
	keyMap[0xb] ='0';
	keyMap[0xc] ='-';
	keyMap[0xd] ='=';
	keyMap[0x1e]  ='A';
	keyMap[0x30]  ='B';
	keyMap[0x2e]  ='C';
	keyMap[0x20]  ='D';
	keyMap[0x12]  ='E';
	keyMap[0x21]  ='F';
	keyMap[0x22]  ='G';
	keyMap[0x23]  ='H';
	keyMap[0x17]  ='I';
	keyMap[0x24]  ='J';
	keyMap[0x25]  ='K';
	keyMap[0x26]  ='L';
	keyMap[0x32]  ='M';
	keyMap[0x31]  ='N';
	keyMap[0x18]  ='O';
	keyMap[0x19]  ='P';
	keyMap[0x10]  ='Q';
	keyMap[0x13]  ='R';
	keyMap[0x1f]  ='S';
	keyMap[0x14]  ='T';
	keyMap[0x16]  ='U';
	keyMap[0x2f]  ='V';
	keyMap[0x11]  ='W';
	keyMap[0x2d]  ='X';
	keyMap[0x15]  ='Y';
	keyMap[0x2c]  ='Z';

	keyMap[0x1e]  ='a';
	keyMap[0x30]  ='b';
	keyMap[0x2e]  ='c';
	keyMap[0x20]  ='d';
	keyMap[0x12]  ='e';
	keyMap[0x21]  ='f';
	keyMap[0x22]  ='g';
	keyMap[0x23]  ='h';
	keyMap[0x17]  ='i';
	keyMap[0x24]  ='j';
	keyMap[0x25]  ='k';
	keyMap[0x26]  ='l';
	keyMap[0x32]  ='m';
	keyMap[0x31]  ='n';
	keyMap[0x18]  ='o';
	keyMap[0x19]  ='p';
	keyMap[0x10]  ='q';
	keyMap[0x13]  ='r';
	keyMap[0x1f]  ='s';
	keyMap[0x14]  ='t';
	keyMap[0x16]  ='u';
	keyMap[0x2f]  ='v';
	keyMap[0x11]  ='w';
	keyMap[0x2d]  ='x';
	keyMap[0x15]  ='y';
	keyMap[0x2c]  ='z';

	//keyMap[DIK_NUMLOCK] =VK_NUMLOCK;//小键盘
	//keyMap[DIK_NUMPAD0] =VK_NUMPAD0;//小键盘;
	//keyMap[DIK_NUMPAD1] =VK_NUMPAD1;//小键盘;
	//keyMap[DIK_NUMPAD2] =VK_NUMPAD2;//小键盘;
	//keyMap[DIK_NUMPAD3] =VK_NUMPAD3;////小键盘;
	//keyMap[DIK_NUMPAD4] =VK_NUMPAD4;////小键盘;
	//keyMap[DIK_NUMPAD5] =VK_NUMPAD5;////小键盘;
	//keyMap[DIK_NUMPAD6] =VK_NUMPAD6;///小键盘;
	//keyMap[DIK_NUMPAD7] =VK_NUMPAD7;////小键盘;
	//keyMap[DIK_NUMPAD8] =VK_NUMPAD9;////小键盘
	//keyMap[DIK_NUMPAD9] =VK_NUMPAD9;////小键盘
	//keyMap[DIK_ADD] =VK_ADD;//+ 小键盘
	//keyMap[DIK_SUBTRACT] =VK_SUBTRACT;//- 小键盘"
	//keyMap[DIK_MULTIPLY] =VK_MULTIPLY;//* 小键盘
	//keyMap[DIK_DIVIDE] =VK_DIVIDE;//'/' 小键盘"
	//keyMap[DIK_DECIMAL] =VK_DECIMAL;//"."小键盘

	keyMap[0x3B]  =VK_F1;
	keyMap[0x3C]  =VK_F2;
	keyMap[0x3D]  =VK_F3;
	keyMap[0x3E]  =VK_F4;
	keyMap[0x3F]  =VK_F5;
	keyMap[0x40]  =VK_F6;
	keyMap[0x41]  =VK_F7;
	keyMap[0x42]  =VK_F8;
	keyMap[0x43]  =VK_F9;
	keyMap[0x44]  =VK_F10;
	keyMap[0x57]  =VK_F11;
	keyMap[0x58]  =VK_F12;
	keyMap[0x1c]  =VK_RETURN;
	keyMap[0xF]   =VK_TAB;
	keyMap[0x2A]  =VK_SHIFT;
	keyMap[0x1]   =VK_ESCAPE;
	keyMap[0x1D]  =VK_CONTROL;
	keyMap[0x39]  =VK_SPACE;
	keyMap[0xb8]  =VK_MENU;	   //alt
	keyMap[0xe]   =VK_BACK;    //退格
	keyMap[0x3A]  =VK_CAPITAL; // Caps Lock 
	keyMap[0xD3]  =VK_DELETE;
	keyMap[0xc8]  =VK_UP;
	keyMap[0xd0]  =VK_DOWN;
	keyMap[0xcb]  =VK_LEFT;
	keyMap[0xcd]  =VK_RIGHT;
}							   

void TSRuntime::InitVirtualToASCIIMap()
{
	ZeroMemory(VirtualToASCIIMap,0x100);
	VirtualToASCIIMap[0x8]=0x8;//back
	VirtualToASCIIMap[0xD]=0xD;//enter
	VirtualToASCIIMap[0x20]=0x20;//space
	VirtualToASCIIMap[17]=17;   //ctrl
	VirtualToASCIIMap[18]=18;	//alt
	VirtualToASCIIMap[16]=16;	//shift
	VirtualToASCIIMap[0xa4]=0xa4;	//左Alt键
	VirtualToASCIIMap[0xA0]=0xA0;	//左Shift键
	VirtualToASCIIMap[0xA2]=0xA2;	//左Ctrl键
	VirtualToASCIIMap[32]=32;   //space
	VirtualToASCIIMap[91]=91;	//win
	VirtualToASCIIMap[20]=20;	//cap
	VirtualToASCIIMap[9]=9;		//tab
	VirtualToASCIIMap[27]=27;	//esc

	VirtualToASCIIMap[38]=38;	//up
	VirtualToASCIIMap[40]=40;	//down
	VirtualToASCIIMap[37]=37;	//left
	VirtualToASCIIMap[39]=39;	//right
	VirtualToASCIIMap[93]=93;   //option

	VirtualToASCIIMap[44]=44;   //print
	VirtualToASCIIMap[46]=46;   //delete
	VirtualToASCIIMap[36]=36;   //home
	VirtualToASCIIMap[35]=35;   //end
	VirtualToASCIIMap[33]=33;   //pgup
	VirtualToASCIIMap[34]=34;   //pgdn


	VirtualToASCIIMap[0x41]=0x61;//A
	VirtualToASCIIMap[0x42]=0x62;//B
	VirtualToASCIIMap[0x43]=0x63;//C
	VirtualToASCIIMap[0x44]=0x64;//D
	VirtualToASCIIMap[0x45]=0x65;//E
	VirtualToASCIIMap[0x46]=0x66;//F
	VirtualToASCIIMap[0x47]=0x67;//G
	VirtualToASCIIMap[0x48]=0x68;//H
	VirtualToASCIIMap[0x49]=0x69;//I
	VirtualToASCIIMap[0x4A]=0x6A;//J
	VirtualToASCIIMap[0x4B]=0x6B;//K
	VirtualToASCIIMap[0x4C]=0x6C;//L
	VirtualToASCIIMap[0x4D]=0x6D;//M
	VirtualToASCIIMap[0x4E]=0x6E;//N
	VirtualToASCIIMap[0x4F]=0x6F;//O
	VirtualToASCIIMap[0x50]=0x70;//P
	VirtualToASCIIMap[0x51]=0x71;//Q
	VirtualToASCIIMap[0x52]=0x72;//R
	VirtualToASCIIMap[0x53]=0x73;//S
	VirtualToASCIIMap[0x54]=0x74;//T
	VirtualToASCIIMap[0x55]=0x75;//U
	VirtualToASCIIMap[0x56]=0x76;//V
	VirtualToASCIIMap[0x57]=0x77;//W
	VirtualToASCIIMap[0x58]=0x78;//X
	VirtualToASCIIMap[0x59]=0x79;//Y
	VirtualToASCIIMap[0x5A]=0x7A;//Z

	//VirtualToASCIIMap[0x41]=0x41;//a
	//VirtualToASCIIMap[0x42]=0x42;//b
	//VirtualToASCIIMap[0x43]=0x43;//c
	//VirtualToASCIIMap[0x44]=0x44;//d
	//VirtualToASCIIMap[0x45]=0x45;//e
	//VirtualToASCIIMap[0x46]=0x46;//f
	//VirtualToASCIIMap[0x47]=0x47;//g
	//VirtualToASCIIMap[0x48]=0x48;//h
	//VirtualToASCIIMap[0x49]=0x49;//I
	//VirtualToASCIIMap[0x4A]=0x4A;//J
	//VirtualToASCIIMap[0x4B]=0x4B;//K
	//VirtualToASCIIMap[0x4C]=0x4C;//L
	//VirtualToASCIIMap[0x4D]=0x4D;//M
	//VirtualToASCIIMap[0x4E]=0x4E;//N
	//VirtualToASCIIMap[0x4F]=0x4F;//O
	//VirtualToASCIIMap[0x50]=0x50;//P
	//VirtualToASCIIMap[0x51]=0x51;//Q
	//VirtualToASCIIMap[0x52]=0x52;//R
	//VirtualToASCIIMap[0x53]=0x53;//S
	//VirtualToASCIIMap[0x54]=0x54;//T
	//VirtualToASCIIMap[0x55]=0x55;//U
	//VirtualToASCIIMap[0x56]=0x56;//V
	//VirtualToASCIIMap[0x57]=0x57;//W
	//VirtualToASCIIMap[0x58]=0x58;//X
	//VirtualToASCIIMap[0x59]=0x59;//Y
	//VirtualToASCIIMap[0x5A]=0x5A;//Z
	VirtualToASCIIMap[0x61]=0x61;//a
	VirtualToASCIIMap[0x62]=0x62;//b
	VirtualToASCIIMap[0x63]=0x63;//c
	VirtualToASCIIMap[0x64]=0x64;//d
	VirtualToASCIIMap[0x65]=0x65;//e
	VirtualToASCIIMap[0x66]=0x66;//f
	VirtualToASCIIMap[0x67]=0x67;//g
	VirtualToASCIIMap[0x68]=0x68;//h
	VirtualToASCIIMap[0x69]=0x69;//I
	VirtualToASCIIMap[0x6A]=0x6A;//J
	VirtualToASCIIMap[0x6B]=0x6B;//K
	VirtualToASCIIMap[0x6C]=0x6C;//L
	VirtualToASCIIMap[0x6D]=0x6D;//M
	VirtualToASCIIMap[0x6E]=0x6E;//N
	VirtualToASCIIMap[0x6F]=0x6F;//O
	VirtualToASCIIMap[0x70]=0x70;//P
	VirtualToASCIIMap[0x71]=0x71;//Q
	VirtualToASCIIMap[0x72]=0x72;//R
	VirtualToASCIIMap[0x73]=0x73;//S
	VirtualToASCIIMap[0x74]=0x74;//T
	VirtualToASCIIMap[0x75]=0x75;//U
	VirtualToASCIIMap[0x76]=0x76;//V
	VirtualToASCIIMap[0x77]=0x77;//W
	VirtualToASCIIMap[0x78]=0x78;//X
	VirtualToASCIIMap[0x79]=0x79;//Y
	VirtualToASCIIMap[0x7A]=0x7A;//Z

	VirtualToASCIIMap[48] ='0';//L"0";
	VirtualToASCIIMap[49] ='1';//L"1";
	VirtualToASCIIMap[50] ='2';//L"2";
	VirtualToASCIIMap[51] ='3';//L"3";
	VirtualToASCIIMap[52] ='4';//L"4";
	VirtualToASCIIMap[53] ='5';//L"5";
	VirtualToASCIIMap[54] ='6';//L"6";
	VirtualToASCIIMap[55] ='7';//L"7";
	VirtualToASCIIMap[56] ='8';//L"8";
	VirtualToASCIIMap[57] ='9';//L"9";


	VirtualToASCIIMap[VK_F1] =VK_F1;//L"F1";
	VirtualToASCIIMap[VK_F2] =VK_F2;//L"F2";
	VirtualToASCIIMap[VK_F3] =VK_F3;//L"F3";
	VirtualToASCIIMap[VK_F4] =VK_F4;//L"F4";
	VirtualToASCIIMap[VK_F5] =VK_F5;//L"F5";
	VirtualToASCIIMap[VK_F6] =VK_F6;//L"F6";
	VirtualToASCIIMap[VK_F7] =VK_F7;//L"F7";
	VirtualToASCIIMap[VK_F8] =VK_F8;//L"F8";
	VirtualToASCIIMap[VK_F9] =VK_F9;//L"F9"
	VirtualToASCIIMap[VK_F10] =VK_F10;//L"F10"
	VirtualToASCIIMap[VK_F11] =VK_F11;//L"F11"
	VirtualToASCIIMap[VK_F12] =VK_F12;//L"F12"

	//VirtualToASCIIMap[VK_NUMLOCK] =VK_NUMLOCK;//小键盘;
	//VirtualToASCIIMap[VK_NUMPAD0] =VK_NUMPAD0;//小键盘;
	//VirtualToASCIIMap[VK_NUMPAD1] =VK_NUMPAD1;//小键盘;
	//VirtualToASCIIMap[VK_NUMPAD2] =VK_NUMPAD2;//小键盘;
	//VirtualToASCIIMap[VK_NUMPAD3] =VK_NUMPAD3;////小键盘;
	//VirtualToASCIIMap[VK_NUMPAD4] =VK_NUMPAD4;////小键盘;
	//VirtualToASCIIMap[VK_NUMPAD5] =VK_NUMPAD5;////小键盘;
	//VirtualToASCIIMap[VK_NUMPAD6] =VK_NUMPAD6;///小键盘;
	//VirtualToASCIIMap[VK_NUMPAD7] =VK_NUMPAD7;////小键盘;
	//VirtualToASCIIMap[VK_NUMPAD8] =VK_NUMPAD9;////小键盘
	//VirtualToASCIIMap[VK_NUMPAD9] =VK_NUMPAD9;////小键盘
	//VirtualToASCIIMap[VK_ADD] =VK_ADD;//+ 小键盘
	//VirtualToASCIIMap[VK_SUBTRACT] =VK_SUBTRACT;//- 小键盘"
	//VirtualToASCIIMap[VK_MULTIPLY] =VK_MULTIPLY;//* 小键盘
	//VirtualToASCIIMap[VK_DIVIDE] =VK_DIVIDE;//'/' 小键盘"
	//VirtualToASCIIMap[VK_DECIMAL] =VK_DECIMAL;//"."小键盘
	//VirtualToASCIIMap[VK_SEPARATOR] =VK_SEPARATOR;// 小键盘"

	VirtualToASCIIMap[0xBA]=0x3B;//;
	VirtualToASCIIMap[0xBC]=0x2C;//,
	VirtualToASCIIMap[0xBE]=0x2E;//.
	VirtualToASCIIMap[0xBF]=0x2F;///
	VirtualToASCIIMap[0xC0]=0x7E;//~
	VirtualToASCIIMap[0xDB]=0x2D;//-
	VirtualToASCIIMap[0xBB]=0x3D;//=
	VirtualToASCIIMap[0xDB]=0x5B;//[
	VirtualToASCIIMap[0xDC]=0x5C;//\
	VirtualToASCIIMap[0xDD]=0x5D;//]
	VirtualToASCIIMap[0xDE]=0x27;//'
}

void TSRuntime::InitKeyPressCharMap() //初始化KeyPressCharMap用于直接将虚拟键码相应的字符串转换到相应的ASCII码
{
	ZeroMemory(KeyPressCharMap,0x100);
	KeyPressCharMap[49] =L"1";
	KeyPressCharMap[50] =L"2";
	KeyPressCharMap[51] =L"3";
	KeyPressCharMap[52] =L"4";
	KeyPressCharMap[53] =L"5";
	KeyPressCharMap[54] =L"6";
	KeyPressCharMap[55] =L"7";
	KeyPressCharMap[56] =L"8";
	KeyPressCharMap[57] =L"9";
	KeyPressCharMap[48] =L"0";
	KeyPressCharMap[45] =L"-";
	KeyPressCharMap[61] =L"=";
	KeyPressCharMap[8] = L"back";     
	KeyPressCharMap[65]  =L"a";
	KeyPressCharMap[66]  =L"b";
	KeyPressCharMap[67]  =L"c";
	KeyPressCharMap[68]  =L"d";
	KeyPressCharMap[69]  =L"e";
	KeyPressCharMap[70]  =L"f";
	KeyPressCharMap[71]  =L"g";
	KeyPressCharMap[72]  =L"h";
	KeyPressCharMap[73]  =L"i";
	KeyPressCharMap[74]  =L"j";
	KeyPressCharMap[75]  =L"k";
	KeyPressCharMap[76]  =L"l";
	KeyPressCharMap[77]  =L"m";
	KeyPressCharMap[78]  =L"n";
	KeyPressCharMap[79]  =L"o";
	KeyPressCharMap[80]  =L"p";
	KeyPressCharMap[81]  =L"q";
	KeyPressCharMap[82]  =L"r";
	KeyPressCharMap[83]  =L"s";
	KeyPressCharMap[84]  =L"t";
	KeyPressCharMap[85]  =L"u";
	KeyPressCharMap[86]  =L"v";
	KeyPressCharMap[87]  =L"w";
	KeyPressCharMap[88]  =L"x";
	KeyPressCharMap[89]  =L"y";
	KeyPressCharMap[90]  =L"z";
	KeyPressCharMap[112]  =L"f1";
	KeyPressCharMap[113]  =L"f2";
	KeyPressCharMap[114]  =L"f3";
	KeyPressCharMap[115]  =L"f4";
	KeyPressCharMap[116]  =L"f5";
	KeyPressCharMap[117]  =L"f6";
	KeyPressCharMap[118]  =L"f7";
	KeyPressCharMap[119]  =L"f8";
	KeyPressCharMap[120]  =L"f9";
	KeyPressCharMap[121]  =L"f10";
	KeyPressCharMap[122]  =L"f11";
	KeyPressCharMap[123]  =L"f12";
	KeyPressCharMap[17]   =L"ctrl";
	KeyPressCharMap[18]   =L"alt";
	KeyPressCharMap[16]   =L"shift";
	KeyPressCharMap[91]   =L"win";
	KeyPressCharMap[32]   =L"space";
	KeyPressCharMap[20]   =L"cap";
	KeyPressCharMap[9]    =L"tab";
	KeyPressCharMap[192]  =L"~";	   
	KeyPressCharMap[27]   =L"esc";
	KeyPressCharMap[13]   =L"enter";
	KeyPressCharMap[38]   =L"up";  
	KeyPressCharMap[40]   =L"down";
	KeyPressCharMap[37]   =L"left";
	KeyPressCharMap[39]   =L"right";
	KeyPressCharMap[93]   =L"option";
	KeyPressCharMap[44]   =L"print";
	KeyPressCharMap[46]   =L"delete";
	KeyPressCharMap[36]   =L"home";
	KeyPressCharMap[35]   =L"end";
	KeyPressCharMap[33]   =L"pgup";
	KeyPressCharMap[34]   =L"pgdn";
	KeyPressCharMap[219]  =L"[";
	KeyPressCharMap[221]  =L"]";
	KeyPressCharMap[220]  =L"\\";
	KeyPressCharMap[186]  =L";";
	KeyPressCharMap[222]  =L"'";
	KeyPressCharMap[188]  =L",";
	KeyPressCharMap[190]  =L".";
	KeyPressCharMap[191]  =L"/";
}


void TSRuntime::getMode(wchar_t *mouse,wchar_t *key,wchar_t *display,DWORD &dwMouse,DWORD &dwKey,DWORD &dwDisplay)
{
	//TSRuntime::add_log( "获得模式:%s",display);
	wchar_t tmp[16][28]={0};
	int rlen=0;
	//memset(tmp,0,16*28*sizeof(wchar_t));

	if ((_wcsicmp(display,L"gdi")==0))
	{
		//TSRuntime::add_log( "gdi模式赋值");
		dwDisplay |= DISPLAY_GDI;
	}
	else if((_wcsicmp(display,L"dx2")==0))
	{
		//TSRuntime::add_log( "dx模式赋值");
		dwDisplay |= DISPLAY_DX2;
	}
	else if((_wcsicmp(display,L"gdi2")==0))
	{
		//TSRuntime::add_log( "dx模式赋值");
		dwDisplay |= DISPLAY_GDI2;
	}
	else if((_wcsicmp(display,L"dx")==0))
	{
		//TSRuntime::add_log( "dx模式赋值");
		dwDisplay |= DISPLAY_DX_3D;
	}
	else if((_wcsicmp(display,L"normal")==0))
	{
		//TSRuntime::add_log( "dx模式赋值");
		dwDisplay |= DISPLAY_NORMAL;
	}
	else
		dwDisplay=-1;


	if ((_wcsicmp(mouse,L"normal")==0))
	{
		//TSRuntime::add_log( "windows模式赋值");
		dwMouse |= MOUSE_NORMAL;
	}
	else if ((_wcsicmp(mouse,L"windows")==0))
	{
		//TSRuntime::add_log( "windows模式赋值");
		dwMouse |= MOUSE_WINDOW;
	}
	else if((_wcsicmp(mouse,L"dx")==0))
	{
		//TSRuntime::add_log( "dx模式赋值");
		dwMouse |= MOUSE_API;
	}
	else
		dwMouse=-1;


	if ((_wcsicmp(key,L"normal")==0))
	{
		//TSRuntime::add_log( "windows模式赋值");
		dwKey |= KEY_NORMAL;
	}
	else if ((_wcsicmp(key,L"windows")==0))
	{
		//TSRuntime::add_log( "windows模式赋值");
		dwKey |= KEY_WINDOW;
	}
	else if((_wcsicmp(key,L"dx")==0))
	{
		//TSRuntime::add_log( "dx模式赋值");
		dwKey |= KEY_API;
	}
	else
		dwKey=-1;
}




//TsMutex::TsMutex()
//{
//	InitializeCriticalSection(&m_mutex);
//}
//
//TsMutex::~TsMutex()
//{
//	DeleteCriticalSection(&m_mutex);
//}
//
//void TsMutex::lock()
//{
//	EnterCriticalSection(&m_mutex);
//}
//void TsMutex::unlock()
//{
//	LeaveCriticalSection(&m_mutex);
//}
//
//TsMutexlock::TsMutexlock(TsMutex *ptcmutex)
//{
//	m_ptcmutex=	 ptcmutex;
//	m_ptcmutex->lock();
//}
//
//TsMutexlock::~TsMutexlock()
//{
//	m_ptcmutex->unlock();
//}

