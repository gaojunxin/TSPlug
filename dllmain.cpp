// dllmain.cpp : DllMain 的实现。

/*本源码由TC简单软件科技有限公司开源,功能可以自由修改、发布、
长沙简单软件科技有限公司对于源码不做后期维护,,请大家在使用过程中遵循开源协议
*/



#include "stdafx.h"
#include "resource.h"
#include "TSPlug_i.h"
#include "dllmain.h"
#include "DXBind.h"
#include "TSRuntime.h"
#include <psapi.h> 
#pragma comment ( lib, "psapi.lib" )


CTSPlugModule _AtlModule;
TCHAR gDLLFolder[MAX_PATH + 1];
HHOOK g_hSetWindowsHook=NULL;
HMODULE g_hInstance=NULL;
extern bool g_Unbind;
extern HWND g_InjectHWND;
extern HWND g_currentHwnd;
HANDLE g_hthread=NULL;
extern int SySTpye;

//卸载线程
void IMEUnLoadThread(void *para)
{
	while(1)
	{
		if(TSRuntime::pData->InjectType==204)//203注入
		{
			TSRuntime::pData->InjectType=205;
			TSRuntime::MyLoadLibrary();
			FreeLibraryAndExitThread(g_hInstance,0);  //卸载DLL
			return;
		}
		if(g_Unbind==true)	 //等待窗口解绑
			{
			//TSRuntime::add_log( "卸载DLL,InjectType:%d",TSRuntime::pData->InjectType);
			if(TSRuntime::pData->InjectType==1||TSRuntime::pData->InjectType==202||TSRuntime::pData->InjectType==205)
				{
					//TSRuntime::add_log( "卸载DLL,InjectType:%d",TSRuntime::pData->InjectType);
					FreeLibraryAndExitThread(g_hInstance,0);  //卸载DLL
				}
			   return ;
			}
		//如果注入方进程异常退出,自我解绑,卸载DLL
		if(::IsWindow(g_InjectHWND)==false&&g_InjectHWND!=NULL)
			{
			if(TSRuntime::pData->InjectType==0)
				{
					//通知自身窗口解绑
					SendMessage(g_currentHwnd,TS_UNBIND,0,0);
					::UnhookWindowsHookEx(g_hSetWindowsHook);
				}
			else
				{
					//通知自身窗口解绑
					SendMessage(g_currentHwnd,TS_UNBIND,0,0);
					FreeLibraryAndExitThread(g_hInstance,0);  //卸载DLL
				}
				return ;
			}
		Sleep(10);

		//CString scd;
	}

}

DWORD CALLBACK CBFunA(DWORD calldata1, DWORD calldata2,DWORD calldata3)	//输入法注入回调函数
{
	HINSTANCE my_hInstance=(HINSTANCE)calldata1;  //输入法传入自身DLL基址和自身得到的基址验证后才开启线程
	//TSRuntime::add_log( "IME注入");
	if(my_hInstance)
		{
			if(my_hInstance==g_hInstance)
				g_hthread=(HANDLE)_beginthread(IMEUnLoadThread, 0 ,0 );//启动线程等待解绑卸载DLL
		}
	return 0;
}

static HMODULE ModuleFromAddress(PVOID pv) 
{
	MEMORY_BASIC_INFORMATION mbi;
	if(::VirtualQuery(pv, &mbi, sizeof(mbi)) != 0)
		{
			return (HMODULE)mbi.AllocationBase;
		}
	else
		{
			return NULL;
		}
}

static LRESULT WINAPI GetMsgProc(int code, WPARAM wParam, LPARAM lParam) 
{
	
	return ::CallNextHookEx(g_hSetWindowsHook, code, wParam, lParam);
}

BOOL WINAPI CBFunB(BOOL bInstall, DWORD dwThreadId)
{
	BOOL bOk=FALSE;
	if(bInstall)
		{
		g_hSetWindowsHook = ::SetWindowsHookEx(WH_CALLWNDPROC, GetMsgProc,ModuleFromAddress(GetMsgProc), dwThreadId);
		//TSRuntime::add_log( "g_hSetWindowsHook:%x,PID:%d",GetCurrentProcessId());
		if(g_hSetWindowsHook != NULL)
			{
			   bOk = true;
			}
			
		} 
	else 
		{
		 
		 if(g_hSetWindowsHook)
			 {
				//::MessageBox(0,L"Dll Main:UnhookWindowsHookEx",L"TS",0);
				bOk = ::UnhookWindowsHookEx(g_hSetWindowsHook);
			 }
		}
	return bOk;
}

// DLL 入口点
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch(dwReason)
		{
		case DLL_PROCESS_ATTACH:
			{

				TSRuntime::InitKeyPressCharMap();
				TSRuntime::InitVirtualToASCIIMap();
				TSRuntime::InitKeyMap();
				TSRuntime::InitKeyPressMap();
				SySTpye=TSRuntime::InitialWindowsVersion();
				g_hInstance=hInstance;
				//wcscpy(gDLLFolder,TSRuntime::GetComPath()); 
				TSRuntime::GetComPath(gDLLFolder);
				//::MessageBox(0,gDLLFolder,L"TS",0);
				char pszMapName[MAX_PATH]={0};
				sprintf( pszMapName,"%s%d",TS_MAPVIEW_NAME,GetCurrentProcessId());
				HANDLE hFileMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, pszMapName);
					//如果hFileMap句柄不为空说明DLL被注入,准备启动注入线程
				if(hFileMap!=NULL)                           
					{
					  //::MessageBox(0,L"Dll Main:DLL_PROCESS_ATTACH",L"TS",0);
					 //// 这里先打开共享内存,共享内存和程序是一对一的关系
				      CShareMemory *sm=new CShareMemory(pszMapName);
					  TSRuntime::pData=(CMessageData*)sm->GetBuffer();
					 //// 共享内存的初始化数据是不能为空的,为空就不正常
					  if(TSRuntime::pData!=NULL)       
						{
							if(TSRuntime::pData->InjectType==BIND_201||TSRuntime::pData->InjectType==BIND_203)//201模式注入
							{
								if(TSRuntime::pData->InjectType==BIND_201)
									TSRuntime::pData->InjectType=202;
								else if(TSRuntime::pData->InjectType==BIND_203)
									TSRuntime::pData->InjectType=204;
								//TSRuntime::add_log("201模式注入");
								DWORD InternalCallWinProc_Addr=(DWORD)::GetModuleHandle(L"user32.dll");
								if(SySTpye==1)//WinXP
									InternalCallWinProc_Addr+=USER32InternalCallWinProcXPoffse;
								else if(SySTpye==2)//Win2003
									InternalCallWinProc_Addr+=USER32InternalCallWinProcWin2003offse;
								else if(SySTpye==4&&TSRuntime::IsWin7X64)//WIN7X64
									InternalCallWinProc_Addr+=USER32InternalCallProcWin7x64offse;
								else if(SySTpye==4)//WIN7X86
									InternalCallWinProc_Addr+=USER32InternalCallProcWin7offse;
								else if(SySTpye==5&&TSRuntime::IsWin8X64)//WIN8X64
									InternalCallWinProc_Addr+=USER32InternalCallProcWin8x64offse;
								else if(SySTpye==5)//WIN8X86
									InternalCallWinProc_Addr+=USER32InternalCallProcWin8offse;

								BYTE ori[5]={0x55,0x8b,0xec,0x56,0x57};

								//注入完成还原钩子
								memcpy((void*)InternalCallWinProc_Addr,ori,5);
								FlushInstructionCache(GetCurrentProcess(),(void*)InternalCallWinProc_Addr,5);

								////TS_BIND201_NAME
								wchar_t pszEventName[MAX_PATH]={0};
								::wsprintf(pszEventName,L"%s%d",TS_BIND201_NAME,::GetCurrentProcessId());
								HANDLE picEvent=::CreateEvent(NULL,TRUE,FALSE,pszEventName);
								::WaitForSingleObject(picEvent,INFINITE);
								::CloseHandle(picEvent);
								g_Unbind=false;

								_beginthread(IMEUnLoadThread, 0 ,0 );
								if(TSRuntime::pData->InjectType==202)
									TSRuntime::g_DxObj.hookApi();
							}
							else if(TSRuntime::pData->InjectType!=202&&TSRuntime::pData->InjectType!=204) //// 这里根据传入的模式进行函数拦截,兵起一个检测线程进行检测
							{
								if(TSRuntime::pData->InjectType==205)//203绑定
									_beginthread(IMEUnLoadThread, 0 ,0 );	
								TSRuntime::g_DxObj.hookApi();
							}
						}

					}
				
			break;
			}

		}
	return _AtlModule.DllMain(dwReason, lpReserved); 
}

TsMutex::TsMutex(char *pszEventName)
	{
		//InitializeCriticalSection(&m_mutex);
		hEvent=OpenEventA(EVENT_ALL_ACCESS,false,pszEventName);
		if(hEvent==NULL)
			{
				hEvent=CreateEventA(NULL,FALSE,FALSE,pszEventName);
				::SetEvent(hEvent);
			}
		
	}

TsMutex::~TsMutex()
	{
		//DeleteCriticalSection(&m_mutex);
		CloseHandle(hEvent);
	}

void TsMutex::lock()
	{
		//::WaitForSingleObject(hEvent,INFINITE);
		::WaitForSingleObject(hEvent,10000);
		//EnterCriticalSection(&m_mutex);
	}
void TsMutex::unlock()
	{
		::SetEvent(hEvent);
		//LeaveCriticalSection(&m_mutex);
	}

TsMutexlock::TsMutexlock(TsMutex *ptcmutex)
	{
		m_ptcmutex=	 ptcmutex;
		m_ptcmutex->lock();
	}

TsMutexlock::~TsMutexlock()
	{
		m_ptcmutex->unlock();
	}