/*
本源码由TC简单软件科技有限公司开源,功能可以自由修改、发布、
长沙简单软件科技有限公司对于源码不做后期维护,,请大家在使用过程中遵循开源协议
*/

#pragma once

#include "TSMessage.h"
#include "KeyMouse/MyKeyboardNamespace.h"
#include "KeyMouse/MyMouseNamespace.h"

////TS保护盾的其他属性
#define TSPROTECT_HIDE    0x00000001L  //"phide [pid]" : 保护指定进程以及进程内的窗口不被非法访问.默认保护当前进程.(此模式需要加载驱动,目前仅支持32位系统)
#define TSPROTECT_HIDE2    0x00000002L	//"phide2 [pid]" : 同phide. 只是进程不隐藏(可在任务管理器中操作)

typedef void *HMEMORYLOADER;
void MemoryFreeLibrary(HMEMORYLOADER mod);
class DXBind;
class TSRuntime
{
public:
	static MyMouseNamespace    g_Mouse;
	static MyKeyboardNamespace g_KeyBoard;
	static DXBind g_DxObj;  //DX后台操作对象,全局唯一,就是一个com只能绑定一个窗口,多窗口就要多实例化几次了          
	static CMessageData *pData;	      //定义为静态类成员,方便保存共享数据指针
	static HANDLE MapViewThread;      //保存检测共享内存线程,方便卸载时调用TerminateThread结束线程
	static DWORD m_displayMode;
	static DWORD m_dwExStyle;
	static wchar_t RetStr[MAX_PATH*4];
	static wchar_t  SetPath[MAX_PATH];
	static LPOLESTR KeyPressCharMap[0x100];	  //256个键位
	static BYTE VirtualToASCIIMap[0x100];  //256个键位
	static BYTE keyMap[0x100];
	static BYTE KeyPressMap[0x100];
	static BYTE RawInputKeyPressMap[0x100];
	static BYTE KeyPressMap_Mouse[0x100];
	static bool IsVipLogin;
	static bool IsRegLogin;
	static DWORD g_nChecktime;//记录时间
	static bool IsStartProtect;
	static HANDLE h_checkthread;//验证线程句柄
	static INT64 checkthreadtime;//校验线程时间
	static bool g_IsSYSKEY;//按下系统键例如:ATL
	static pMyDictInfo MyDictCount[20]; //存储当前所有字库,默认最大字库个数为20个
	static int nMaxStrHight[20]; //记录每个字库中,最高的字高.
	static int nMyDictCountsize[20]; //记录每个字库的数量
	static int NowUsingDictIndex;//记录当前正在使用字库的下标
	static COLORREF pLoadstrBuffer[11][MAX_PATH];//存储字库点阵还原图片颜色矩阵COLORREF
	static char bytestr[1024*3]; //存储点阵二进制信息最大存储为250*11
	static bool IsVirtualProtectEx;//是否驱动模式远程修改注入进程的页面保护属性
	static HWND hookHwnd;//记录要注入的窗口句柄
	static HMEMORYLOADER myUser32module;
	static HMEMORYLOADER mykernel32module;
	static bool IsInitialmyUser32module;
	static bool IsInitialmykernel32module;
	static wchar_t membstr[MAX_PATH*400];
	static bool IsShowErrorMsg;
	static bool IsNpProtect;
	static bool IsWin7X64;
	static bool IsWin8X64;
	static DWORD IsProid;
	static DWORD ProtectPid[20];
	static wchar_t Regcode[MAX_PATH];//记录注册码信息
	static wchar_t VipCountCode[MAX_PATH];//记录VIP用户账户信息
	static DWORD ZwProtectVirtualMemoryRet;
	static wchar_t SetDictPwdString[MAX_PATH];
	static wchar_t SetPicPwdString[MAX_PATH];
	static wchar_t MachineCode[MAX_PATH];//
	static bool IsLoginlog;
	static BYTE SetSimModeType;
	static bool g_IsLoadDrive;
	//static CComBSTR newbstr;
public:
	static HWND  GetProcessMainWnd(DWORD  dwProcessId);	 //获取自身窗口句柄
	static void getMode(wchar_t *mouse,wchar_t *key,wchar_t *display,DWORD &dwMouse,DWORD &dwKey,DWORD &dwDisplay);
    static void my_split(TCHAR* sourcestr, TCHAR *oldstr, TCHAR (*temp)[28], int &rlen);
	static void add_log( const char * fmt, ... );
	static int  InitialWindowsVersion();//获取系统版本
	static bool EnablePrivilege(LPCTSTR pszPrivName, bool fEnable );
	static void initial();
	static bool  MyWriteMemory(IN PWriteMemoryInfo WriteInfo);
	static int VipLogin(wchar_t *regcode,int type,bool isvip=true);
	static bool GetComPath(TCHAR * Path);
	static bool StartProtect(long enable,long type,int Pid=0);
	static void InitKeyPressCharMap();//初始化KeyPressCharMap
	static void InitVirtualToASCIIMap();
	static void InitKeyMap();
	static void InitKeyPressMap();
	static int retrunLparamDown(int key);
	static BOOL MByteToWChar(LPCSTR lpcszStr,LPWSTR lpwszStr,DWORD dwSize);
	static BOOL WCharToMByte(LPCWSTR lpcwszStr,LPSTR lpszStr,DWORD dwSize);
	static DWORD GetPassR3HookFuntionAddr(char *modename,char*funname);
	static HMODULE GetRemoteModuleHandle(const wchar_t *module, short nPID);//获取指定进程ID的DLL模块句柄
	static HMODULE TSRuntime::GetRemoteModuleBase(const wchar_t *module, short nPID);
	static DWORD GetRemoteModuleBaseAddr(const wchar_t *module, short nPID);//获取指定进程ID的DLL模块地址
	static bool GetRemoteModulePath(const wchar_t *module, short nPID,wchar_t *modulepath);//获取指定进程ID的DLL模块路径
	static FARPROC GetRemoteProcAddress( wchar_t *module, const char *func, short nPID);//获取指定进程ID的DLL模块函数的地址
	static bool MyLoadLibrary();
private:
	TSRuntime(void);
	~TSRuntime(void);
};

////访问控制
//class TsMutex//创建临界区和销毁临界区
//{
//public:
//   TsMutex();
//   ~TsMutex();
//   CRITICAL_SECTION m_mutex;
//   void lock();
//   void unlock();
//
//};
//
//class TsMutexlock//进入和离开临界区
//{
//public:
//	TsMutexlock(TsMutex *ptcmutex);
//	~TsMutexlock();
//	TsMutex *m_ptcmutex;
//};
