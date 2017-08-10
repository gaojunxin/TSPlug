/*
本源码由TC简单软件科技有限公司开源,功能可以自由修改、发布、
长沙简单软件科技有限公司对于源码不做后期维护,,请大家在使用过程中遵循开源协议
*/
#pragma once

#include <Windows.h>
#include "TSRuntime.h"
#include "d3d9.h"
#include "D3dx9tex.h"
#include <dinput.h>
#include <Ddraw.h>
#include <D3D10.h>
#include <wingdi.h>
#include "TSFindPicture.h"
#include <Dbghelp.h>
#pragma comment ( lib, "Dbghelp.lib" )
#include <psapi.h> 
#pragma comment ( lib, "psapi.lib" )
#pragma comment(lib, "D3dx9.lib")   //
#pragma comment(lib, "D3D9.lib")
#pragma comment(lib, "gdi32.lib")
#include "TsMessage.h"
#include "TSMyKernel32DllFuntion.h"
#include "TSVIPHookFuntion.h"

bool   g_isSetKeyDown=false;      //记录GetKeyStateEvent中是否有按键按下动作
HWND g_InjectHWND=0;   //注入方进程窗口句柄
DWORD InternalCallWinProc_Offset=0;//InternalCallWinProc函数的地址偏移
DWORD GetDeviceStateOBJMouse_Offset=0;
DWORD GetDeviceStateOBJKbd_Offset=0;
HANDLE HookKbdMouse_Thread=NULL;

extern HANDLE g_hthread;   //卸载线程句柄
extern HMODULE g_hInstance;//当前自身DLL模块

extern myVirtualProtect my_VirtualProtect; 
//extern mySendMessage my_SendMessage;
//extern myPostMessage my_PostMessage;
DWORD dwMoveGetKeyboardState=0;	//GetKeyboardState   赋值指令

DWORD HookGetKeyboardStateRet=0;
DWORD HookGetCaptureRet=0;
DWORD HookGetAsyncKeyStateRet=0;
DWORD HookIsIconicRet=0;
DWORD HookImmGetCompositionStringARet=0;
DWORD HookImmGetCompositionStringWRet=0;
DWORD HookGetDeviceStateRet=0;	     //HOOk:Dinput8:GetDeviceState的返回地址
DWORD HookInternalCallWinProcRet=0;	 //HOOk:USER32InternalCallWinProc的返回地址
DWORD HookGetCursorPosRet=0;		 //HOOk:USER32GetCursorPos的返回地址
DWORD HookSetCursorPosRet=0;		 //HOOk:USER32SetCursorPos的返回地址
DWORD HookScreenToClientRet=0;		 //HOOk:USER32ScreenToClient的返回地址
DWORD HookD3D9PresentRet=0;		     //HOOk:D3D9PresentRet的返回地址
DWORD HookGetDeviceStateKbdRet=0;    //HOOk:GetDeviceStateKbdRet的返回地址
DWORD HookGetDeviceStateMouseRet=0;  //HOOk:GetDeviceStateMouseRet的返回地址
BYTE UnhookGetDeviceStateKbd[5]={0};
BYTE UnhookGetDeviceStateMouse[5]={0};
BYTE UnhookInternalCallWinProc[5]={0};
BYTE UnhookGetTopWindow[5]={0};
BYTE UnhookGetActiveWindow[7]={0};	//记录保存原来函数GetActiveWindow的前7个字节
BYTE UnhookGetFocus[7]={0};			//记录保存原来函数GetFocus的前7个字节
BYTE UnhookGetMessagePos[7]={0};
BYTE UnhookWindowFromPoint[5]={0};	//记录保存原来函数WindowFromPoint的前5个字节
BYTE UnhookGetForegroundWindow[5]={0};	//记录保存原来函数GetForegroundWindow的前5个字节
BYTE UnhookGetKeyboardState[5]={0};
BYTE UnhookScreenToClientRet[7]={0};		 //HOOk:USER32ScreenToClient的返回地址
BYTE UnhookSetCursor[5]={0};		 //HOOk:USER32SetCursor的返回地址

BYTE UnhookPeekMessage[5]={0};
DWORD HookPeekMessageARet=0;
DWORD HookPeekMessageWRet=0;

DWORD HookSetCursorRet=0;
DWORD HookGetKeyStateRet=0;	         //HOOk:USER32GetKeyState的返回地址

DWORD HookUserCallWinProcRet=0;
BYTE UnhookUserCallWinProc[7]={0};	//记录保存原来函数GetActiveWindow的前7个字节

DWORD HookGetTopWindowRet=0;
DWORD HookGetActiveWindowRet=0;
DWORD HookGetFocusRet=0;
DWORD HookGetForegroundWindowRet=0;
DWORD HookWindowFromPointRet=0;
DWORD HookIsWindowEnabledRet=0;
DWORD HookGetMessagePosRet=0;

DWORD HookGetRawInputDataRet=0;

DWORD HookSendMessageRet=0;
DWORD HookPostMessageRet=0;
DWORD SendMessageWAddr=0;
DWORD PostMessageWAddr=0;

DWORD GetRegisteredRawInputDevicesAddr=0;
typedef UINT(__stdcall  *  MyGetRegisteredRawInputDevices)( PRAWINPUTDEVICE pRawInputDevices,PUINT puiNumDevices, UINT cbSize );
MyGetRegisteredRawInputDevices My_GetRegisteredRawInputDevices;

HWND g_hwndRawInput=NULL;//RawInput的目标窗口
bool isnotRawInput=false;//判断是否RawInput检测键鼠

DWORD g_dinput8_KbdHOOKaddr=0;	  //处理DNF钩子的内部HOOK
DWORD g_dinput8_MOUSEHOOKaddr=0;

//bool g_IsGetDeviceStatehook=false; //GetDeviceStatehook
//bool g_IsGetCursorPoschook=false;	 //GetCursorPos
//bool g_IsScreenToClienthook=false; //ScreenToClient

bool g_LockInput_kdb=true;//记录封锁外部键盘输入,默认封锁
bool g_LockInput_mouse=true; //记录封锁外部鼠标输入,默认封锁
bool IsSendstrIme=false;//通知用户调用了SendstrIme函数;
bool IsWin7X64=false;
bool IsWin8=false;
bool IsWin7=false;
bool IsWinxp=false;
bool IsWin2003=false;
bool IsWin8X64=false;
bool IsNpProtect=false;
bool IsInternalCallWinProc=false;
bool IsGetDeviceStateMouse=false;
bool IsGetDeviceStateKbd=false;
int SySTpye=0;
DWORD sleeptime=0;
LPDIDEVICEOBJECTDATA g_rgdod;
DIMOUSESTATE *g_mousestate=0;
HWND g_currentHwnd=0;
MSG *GameMsg=NULL;
LPPOINT g_lpPoint=NULL;//GetCursorPos的指针


int g_NowDictIndex=0;//记录SetDict时候的字库下标
//记录上一次鼠标移动的位置
int g_x=-1,g_y=-1;

//记录鼠标移动的位置
bool        g_keymouseevent=false;
bool        g_mouseDevice=false;
bool		g_kbdDevice=false;
bool	    g_keyboardstate=false;
bool		g_kbdDevice_mouse=false;
bool		g_GetKeyState=false;
bool        g_GetRawInputDatahook=false;
//bool		g_GetForegroundWindow=false;
UINT		g_GetRawInputKeyDataSize=sizeof(RAWKEYBOARD)+sizeof(RAWINPUTHEADER);	 //记录键盘数据结构体返回值大小
UINT		g_GetRawInputMouDataSize=sizeof(RAWMOUSE)+sizeof(RAWINPUTHEADER);	 //记录鼠标数据结构体返回值大小
bool        g_Unbind=false;
int         g_message=0;  //记录当前键盘消息
int         g_wParam=0;	  //记录当前VK键值
bool	    ismousedown=false;
LPDIRECTINPUTDEVICE8       g_RealKeyBoardDevice=NULL;//记录DX键盘设备指针
DWORD       g_dwOfs=NULL;
DWORD       g_dwData=NULL;
HCURSOR		g_bkmouseshape=NULL; //记录全局HCURSOR,后台鼠标特征码
DWORD       g_OriSetCursor=0;//xp:0x11FF,2003:0x11fd,
DWORD       g_GetRawInputData=0;//xp:0x11ac,2003:,win7:

bool IsVIPHookandUnhookAPI=false;

void HookandUnhookAPI(void *para);//声明HOOK函数
HRESULT __stdcall MyGetDeviceStateMouse(
										LPDIRECTINPUTDEVICE8 m_pMouse,	  //当前设备指针
										DWORD cbObjectData,
										LPDIDEVICEOBJECTDATA rgdod,
										LPDWORD pdwInOut,
										DWORD dwFlags);	//声明HOOK函数
HRESULT __stdcall MyGetDeviceStateKbd(
									  LPDIRECTINPUTDEVICE8 m_pKeyboard,  //当前设备指针
									  DWORD cbData,            //缓冲区字节大小
									  LPVOID lpvData           //缓冲区指针
									  );  //声明HOOK函数

BOOL __stdcall	MyPeekMessageA(  LPMSG lpMsg,   HWND hWnd,   UINT wMsgFilterMin, 
							   UINT wMsgFilterMax,   UINT wRemoveMsg );
BOOL __stdcall	MyPeekMessageW(  LPMSG lpMsg,   HWND hWnd,   UINT wMsgFilterMin, 
							   UINT wMsgFilterMax,   UINT wRemoveMsg );

DWORD Ntdll_ProtectVirtual=0;
DWORD Ntdll_ProtectVirtualEax=0;
__declspec(naked) NTSTATUS WINAPI MyZwProtectVirtualMemory(IN HANDLE ProcessHandle,
														   IN PVOID *  BaseAddress,
														   IN SIZE_T *     NumberOfBytesToProtect,
														   IN ULONG    NewAccessProtection,
														   OUT PULONG  OldAccessProtection )
{
	_asm
	{
		//mov eax,0x4d ;//WIN7X64 偏移
		mov eax,Ntdll_ProtectVirtualEax;
		push Ntdll_ProtectVirtual;
		ret;
	}
}


//卸载线程
void UnLoadDLLThread(void *para)
{
	while(1)
	{
		if(g_Unbind==true)	 //等待窗口解绑
		{
			FreeLibraryAndExitThread(g_hInstance,0);  //卸载DLL
			return ;
		}
		//如果注入方进程异常退出,自我解绑,卸载DLL
		if(::IsWindow(g_InjectHWND)==false&&g_InjectHWND!=NULL)
		{
			//通知自身窗口解绑
			SendMessage(g_currentHwnd,TS_UNBIND,0,0);
			FreeLibraryAndExitThread(g_hInstance,0);  //卸载DLL
			return ;
		}
		Sleep(10);
	}

}

__declspec(naked) BOOL __stdcall User32PeekMessageW	(LPMSG lpMsg,   HWND hWnd,   UINT wMsgFilterMin, 
													 UINT wMsgFilterMax,   UINT wRemoveMsg )
{
	_asm
	{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节,WIN7下7个字节为2个完整指令,所以多预留2个NOP指令为2个字节
			push ebp
			mov ebp,esp
			jmp HookPeekMessageWRet
			ret;
	}
}

__declspec(naked) BOOL __stdcall User32PeekMessageA	(LPMSG lpMsg,   HWND hWnd,   UINT wMsgFilterMin, 
													 UINT wMsgFilterMax,   UINT wRemoveMsg )
{
	_asm
	{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节,WIN7下7个字节为2个完整指令,所以多预留2个NOP指令为2个字节
			push ebp
			mov ebp,esp
			jmp HookPeekMessageARet
			ret;
	}
}


void IATMess(HINSTANCE hand,LPSTR name,DWORD addrs,DWORD fromaddrs)//IATHOOK
{   
	DWORD size;   
	PIMAGE_IMPORT_DESCRIPTOR import=(PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hand,TRUE,IMAGE_DIRECTORY_ENTRY_IMPORT,&size);   
	char buf[20]={0};   
	while(import->Name!=NULL)   
	{   
		LPSTR dllname=(LPSTR)((DWORD)hand+import->Name);   
		//wsprintf(buf,"%s\n",dllname);   
		//MessageBoxA(NULL,dllname,NULL,MB_OK);
		if(stricmp(dllname,name)==0)   
		{ 
			//TSRuntime::add_log( "找到dllname:%s",dllname);
			//MessageBoxA(NULL,"找到user32.dll",NULL,MB_OK);   
			break;   
		}   
		import++;   
	}   
	PIMAGE_THUNK_DATA pdata=(PIMAGE_THUNK_DATA)((DWORD)hand+import->FirstThunk);   
	while(pdata->u1.Function!=NULL)   
	{   
		DWORD *func=(DWORD*)&pdata->u1.Function;
		if(*func==addrs)   
		{



			MEMORY_BASIC_INFORMATION mbi;   
			::VirtualQuery(func, &mbi, sizeof(MEMORY_BASIC_INFORMATION));   
			// In order to provide writable access to this part of the    
			// memory we need to change the memory protection   
			if (FALSE == ::VirtualProtect(   
				mbi.BaseAddress,   
				mbi.RegionSize,   
				PAGE_READWRITE,   
				&mbi.Protect)   
				)   
				break;   
			// Hook the function.
			*func=fromaddrs;

			// Restore the protection back   
			DWORD dwOldProtect;   
			::VirtualProtect(   
				mbi.BaseAddress,   
				mbi.RegionSize,   
				mbi.Protect,   
				&dwOldProtect   
				);   
			break;   
		}   

		pdata++;   
	}   

}



//BYTE oriapi[]={0x8b,0xff,0x55,0x8b,0xec};//8B FF 55 8B EC
//检测SendMessageW,和PostMessageW是否被HOOK
__declspec(naked) LRESULT __stdcall MySendMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	_asm
	{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节
			push ebp
			mov ebp,esp
			jmp HookSendMessageRet
			ret
	}
}

__declspec(naked) BOOL  __stdcall MyPostMessage(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	_asm
	{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节
			push ebp
			mov ebp,esp
			jmp HookPostMessageRet
			ret
	}
}
bool HookMessageAPI()
{
	if(SendMessageWAddr==0)
	{
		SendMessageWAddr=(DWORD)GetProcAddress(GetModuleHandle(L"user32.dll"),"SendMessageW");
	}
	if(PostMessageWAddr==0)
	{
		PostMessageWAddr=(DWORD)GetProcAddress(GetModuleHandle(L"user32.dll"),"PostMessageW");
	}
	BYTE checkbyteS=*(BYTE*)SendMessageWAddr;
	BYTE checkbyteP=*(BYTE*)PostMessageWAddr;
	if(checkbyteS==0XE9||checkbyteP==0XE9)
	{
		if(checkbyteS==0XE9)
		{
			HookSendMessageRet=SendMessageWAddr+5;
		}
		if(checkbyteP==0XE9)
		{
			HookPostMessageRet=PostMessageWAddr+5;
		}
	}
	//TSRuntime::add_log("checkbyteS:%x,checkbyteP:%x,OpenProcessAddr:%x",checkbyteS,checkbyteP,OpenProcessAddr);
	return true;
}

void DetourFunc(BYTE *src, const BYTE *dst,BYTE *retdest, int len,DWORD &HookFunRet,bool IsHook)	 //HOOK函数
{
	if(IsHook)
	{
		//(DWORD)MyGetDeviceStateMouse
		BYTE *jmp = (BYTE*)malloc(len);
		DWORD dwback;
		DWORD dwmyback;
		if(src[0]==0xe9&&((DWORD)dst==(DWORD)MyGetDeviceStateKbd/*||(DWORD)dst==(DWORD)MyGetDeviceStateMouse*/))	//针对DNF对dinput8.dll的2个函数钩子做特殊处理
		{
			if((DWORD)dst==(DWORD)MyGetDeviceStateKbd)
			{
				g_dinput8_KbdHOOKaddr=(DWORD)src+(*(DWORD*)(src+1))+5;
				src=(BYTE *)g_dinput8_KbdHOOKaddr;
				HookFunRet=g_dinput8_KbdHOOKaddr+6;
				//src=src+5;
				//HookFunRet=(DWORD)src+6;
				len=6;
			}
			//else if((DWORD)dst==(DWORD)MyGetDeviceStateMouse)
			//	{
			//	g_dinput8_MOUSEHOOKaddr=(DWORD)src+(*(DWORD*)(src+1))+5;
			//	src=(BYTE *)g_dinput8_MOUSEHOOKaddr;
			//	HookFunRet=g_dinput8_MOUSEHOOKaddr+5;
			//	len=5;
			//	}
		}
		else if(src[0]==0xe9)	 //判断如果是原来的被HOOK就放弃HOOK
		{
			HookFunRet=-1;   //重置返回地址为-1,标志没有HOOK,方便后面卸载的时候判断
			//TSRuntime::add_log( "src[0]==0xe9");
			return;
		}
		if(retdest)
		{
			DWORD dwback;
			VirtualProtect(retdest, len, PAGE_EXECUTE_READWRITE, &dwback);
			memcpy(jmp, src, len);	
			memcpy(retdest, jmp, len);
			VirtualProtect(retdest, len, dwback, &dwback);
		}
		memset(jmp,0,len);
		if(IsGetDeviceStateMouse||IsGetDeviceStateKbd)
		{
			*(DWORD*)jmp = (DWORD)dst;
		}
		else
		{
			if(IsInternalCallWinProc)
				jmp[0] = 0xE8;
			else
				jmp[0] = 0xE9;
			*(DWORD*)(jmp+1) = (DWORD)(dst - src) - 5;
		}
		bool succ=false;
		succ=VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwback);
		if(succ==false)
		{

			if(IsWin7X64&&TSRuntime::pData->IsNpProtect)
			{
				if(Ntdll_ProtectVirtual==0)
				{
					Ntdll_ProtectVirtual=(DWORD)GetProcAddress(GetModuleHandle(L"ntdll.dll"),"ZwProtectVirtualMemory");
					Ntdll_ProtectVirtual+=5;
				}
				if(Ntdll_ProtectVirtual)
				{
					PVOID Addr=(PVOID)src;
					DWORD length=len;
					MyZwProtectVirtualMemory((HANDLE)0xffffffff,&Addr,&length,PAGE_EXECUTE_READWRITE,&dwback);
					succ=true;
				}
				//bool bret=My_VirtualProtect(src,len,PAGE_EXECUTE_READWRITE,&dwback);
				//TSRuntime::add_log( "VirtualProtect,dwback:%x,bret:%d",dwback,succ);
			}
			else if(TSRuntime::pData->IsNpProtect&&IsNpProtect)
				succ=true;
		}
		//TSRuntime::add_log( "VirtualProtect,succ:%d,dwback:%d,src:%x",succ,dwback,src);
		if(succ==1)
			memcpy(src, jmp, len);
		VirtualProtect(src, len, dwback, &dwback);
		free(jmp);
		FlushInstructionCache(GetCurrentProcess(),src,len);
	}
	else//解绑,卸载钩子
	{
		if((src[0]==0xe9||src[0]==0xe8||IsGetDeviceStateMouse||IsGetDeviceStateKbd)&&HookFunRet!=-1)	 //判断第一个字节是否是JMP,通常被HOOK的函数的第一个字节都是JMP==0xe9指令
		{
			if((DWORD)dst==(DWORD)MyGetDeviceStateKbd&&(g_dinput8_KbdHOOKaddr!=0))
			{
				src=(BYTE *)g_dinput8_KbdHOOKaddr;
				len=6;
			}
			//else if((DWORD)dst==(DWORD)MyGetDeviceStateMouse&&g_dinput8_MOUSEHOOKaddr!=0)
			//	{
			//	src=(BYTE *)g_dinput8_MOUSEHOOKaddr;
			//	len=5;
			//	}
			DWORD dwback;
			BYTE * unhook = (BYTE*)malloc(len);
			memcpy(unhook, retdest, len);   //将返回函数前N个字节保存
			VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwback);
			memcpy(src, unhook, len);
			VirtualProtect(src, len, dwback, &dwback);
			//delete []unhook;
			free(unhook);

			FlushInstructionCache(GetCurrentProcess(),src,len);

		}

	}

}

DWORD GetHookModFuntion(TCHAR *ModName,char *FunName,DWORD FuntionOffsec,DWORD HookFunction,DWORD RetFunction,DWORD &HookFunRet,bool IsHook)	//FuntionOffsec!=0:HOOK指定模块的未导出函数,FuntionOffsec=0HOOK指定模块导出函数
{
	HMODULE hMod=0;
	DWORD addr=0;

	if(TSRuntime::IsVirtualProtectEx)  ////是否驱动模式远程修改注入进程的页面保护属性
	{
		//USES_CONVERSION;
		//TSRuntime::add_log( "TSRuntime::IsVirtualProtectEx,FunName:%s",FunName);
		DWORD pid=0;
		DWORD dwback=0;
		GetWindowThreadProcessId(TSRuntime::hookHwnd,&pid);
		TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
		HANDLE hprocess=OpenProcess(PROCESS_ALL_ACCESS,false,pid);
		DWORD lasserr=::GetLastError();

		//TSRuntime::add_log("pid:%d,lasserr:%d",pid,lasserr);
		if(hprocess)
		{

			//hMod= TSRuntime::GetRemoteModuleHandle(L"user32.dll",pid);
			hMod= TSRuntime::GetRemoteModuleHandle(ModName,pid);
			if(hMod!=0&&FuntionOffsec!=0)
			{
				addr=(DWORD)hMod+FuntionOffsec;
			}
			else if(hMod!=0&&FunName!=NULL)
			{
				addr=(DWORD)TSRuntime::GetRemoteProcAddress(ModName,FunName,pid);
			}
			if(SendMessageWAddr==0)
			{
				SendMessageWAddr=(DWORD)TSRuntime::GetRemoteProcAddress(L"user32.dll","SendMessageW",pid);
				VirtualProtectEx(hprocess,(LPVOID)SendMessageWAddr,7,PAGE_EXECUTE_READWRITE,&dwback);
			}
			if(PostMessageWAddr==0)
			{
				PostMessageWAddr=(DWORD)TSRuntime::GetRemoteProcAddress(L"user32.dll","PostMessageW",pid);
				VirtualProtectEx(hprocess,(LPVOID)SendMessageWAddr,7,PAGE_EXECUTE_READWRITE,&dwback);
			}
			//MEMORY_BASIC_INFORMATION mbi_thunk;
			////查询页信息。
			//VirtualQueryEx(hprocess,(LPVOID)addr, &mbi_thunk, sizeof(MEMORY_BASIC_INFORMATION));
			////改变页保护属性为读写。
			//VirtualProtectEx(hprocess,mbi_thunk.BaseAddress,mbi_thunk.RegionSize,PAGE_EXECUTE_READWRITE,&mbi_thunk.Protect);
			bool succ= VirtualProtectEx(hprocess,(LPVOID)addr,7,PAGE_EXECUTE_READWRITE,&dwback); 
			//TSRuntime::add_log( "VirtualProtectEx,succ:%d,dwback:%d,pid:%x",succ,dwback,pid);

		}
		return 0;
	}
	else////否则进入hook功能代码
	{
		//hMod = LoadLibrary(ModName);
		//TSRuntime::add_log( "GetHookModFuntion-start");
		hMod=::GetModuleHandle(ModName);
		//TSRuntime::add_log( "hMod:%x",hMod);
		if(hMod==NULL)
			return 0;

		if(hMod!=0&&FuntionOffsec!=0)
		{
			addr=(DWORD)hMod+FuntionOffsec;
		}

		else if(hMod!=0&&FunName!=NULL)
		{
			addr=(DWORD)::GetProcAddress(hMod,FunName);
		}

		if(SySTpye==1&&(strcmp("GetFocus",FunName)==0||strcmp("GetActiveWindow",FunName)==0)||strcmp("GetMessagePos",FunName)==0)//XP系统,要HOOK前7字节GetFocus 
		{	
			if(IsHook)	 //HOOK,备份前7个字节
			{
				HookFunRet=addr+7;
				BYTE * jmp=	(BYTE*)RetFunction;
				memcpy(jmp,(void*)addr,7);
				DetourFunc((BYTE*)addr,(BYTE *)HookFunction,NULL,7,HookFunRet,IsHook);
			}
			else
				DetourFunc((BYTE*)addr,(BYTE *)HookFunction,(BYTE *)RetFunction,7,HookFunRet,IsHook);	//卸载,还原

			//TSRuntime::add_log( "FunName:%s,addr:%x,RetFunction:%x",FunName,addr,RetFunction);
			return addr;
		}

		else if((SySTpye==4||IsWin8X64)&&strcmp("ScreenToClient",FunName)==0)	//如果是win7下HOOK的函数是ScreenToClient时候,HookFunRet=+7
		{
			if(IsHook)
				HookFunRet=addr+7;
			DetourFunc((BYTE*)addr,(BYTE *)HookFunction,(BYTE *)RetFunction,7,HookFunRet,IsHook);
			//TSRuntime::add_log( "FunName:%s,addr:%x,RetFunction:%x",FunName,addr,RetFunction);
			return addr;
		}
		else if((strcmp("GetDeviceState",FunName)==0))
		{
			int len=0;
			if(IsGetDeviceStateMouse)
			{
				if(IsHook)
					HookFunRet=(DWORD)hMod+GetDeviceStateOBJMouse_Offset+5;
				len=4;
			}
			else if(IsGetDeviceStateKbd)
			{
				if(IsHook)
					HookFunRet=(DWORD)hMod+GetDeviceStateOBJKbd_Offset+5;
				len=4;
			}
			else
			{
				if(IsHook)
					HookFunRet=addr+5;
				len=5;
			}
			//TSRuntime::add_log( "FunName:%s,addr:%x,RetFunction:%x,IsHook:%d",FunName,addr,RetFunction,IsHook);
			DetourFunc((BYTE*)addr,(BYTE *)HookFunction,(BYTE *)RetFunction,5,HookFunRet,IsHook);
			return addr;
		}
		else if((strcmp("InternalCallWinProc",FunName)==0))
		{
			if(IsInternalCallWinProc)
			{
				if(IsHook)
					HookFunRet=(DWORD)hMod+InternalCallWinProc_Offset+5;
			}
			else
			{
				if(IsHook)
					HookFunRet=addr+5;
			}
			DetourFunc((BYTE*)addr,(BYTE *)HookFunction,(BYTE *)RetFunction,5,HookFunRet,IsHook);
			return addr;
		}
		else if((strcmp("GetForegroundWindow",FunName)==0))
		{
			addr+=5;
			if(IsHook)
				HookFunRet=addr+5;
			DetourFunc((BYTE*)addr,(BYTE *)HookFunction,UnhookGetForegroundWindow,5,HookFunRet,IsHook);
			return addr;
		}
		else if((strcmp("SetCursor",FunName)==0))
		{
			if(IsHook)
				HookFunRet=addr+5;
			DetourFunc((BYTE*)addr,(BYTE *)HookFunction,UnhookSetCursor,5,HookFunRet,IsHook);
			return addr;
		}
		else if((strcmp("NtUserCallOneParam",FunName)==0)||
			(strcmp("NtUserCallTwoParam",FunName)==0)||
			(strcmp("NtUserGetThreadState",FunName)==0)||
			(strcmp("NtUserGetKeyState",FunName)==0)||
			(strcmp("NtUserGetAsyncKeyState",FunName)==0))
		{
			BYTE *paddr=(BYTE *)addr;
			BYTE unhookbyte[5]={0};
			if(strcmp("NtUserCallOneParam",FunName)==0)
			{
				if(EAX_NtUserCallOneParam==0)
				{
					memcpy(NtUserCallOneParamUnhookByte,(BYTE*)addr,5);
					EAX_NtUserCallOneParam=*(DWORD*)(paddr+1);
				}
				else if(!IsHook)
					memcpy(unhookbyte,NtUserCallOneParamUnhookByte,5);

			}
			else if(strcmp("NtUserCallTwoParam",FunName)==0)
			{
				if(EAX_NtUserCallTwoParam==0)
				{
					memcpy(NtUserCallTwoParamUnhookByte,(BYTE*)addr,5);
					EAX_NtUserCallTwoParam=*(DWORD*)(paddr+1);
				}
				else if(!IsHook)
					memcpy(unhookbyte,NtUserCallTwoParamUnhookByte,5);

			}
			else if(strcmp("NtUserGetThreadState",FunName)==0)
			{
				if(EAX_NtUserGetThreadState==0)
				{
					memcpy(NtUserGetThreadStateUnhookByte,(BYTE*)addr,5);
					EAX_NtUserGetThreadState=*(DWORD*)(paddr+1);
				}
				else if(!IsHook)
					memcpy(unhookbyte,NtUserGetThreadStateUnhookByte,5);

			}
			else if(strcmp("NtUserGetKeyState",FunName)==0)
			{
				if(EAX_NtUserGetKeyState==0)
				{
					memcpy(NtUserGetKeyStateUnhookByte,(BYTE*)addr,5);
					EAX_NtUserGetKeyState=*(DWORD*)(paddr+1);
				}
				else if(!IsHook)
					memcpy(unhookbyte,NtUserGetKeyStateUnhookByte,5);
			}
			else if(strcmp("NtUserGetAsyncKeyState",FunName)==0)
			{
				if(EAX_NtUserGetAsyncKeyState==0)
				{
					memcpy(NtUserGetAsyncKeyStateUnhookByte,(BYTE*)addr,5);
					EAX_NtUserGetAsyncKeyState=*(DWORD*)(paddr+1);
				}
				else if(!IsHook)
					memcpy(unhookbyte,NtUserGetAsyncKeyStateUnhookByte,5);
			}
			//TSRuntime::add_log( "FunName:%s,addr:%x,NtUserCallOneParamUnhookByte:%x",FunName,addr,*(DWORD*)NtUserCallOneParamUnhookByte);
			if(IsHook)
			{
				HookFunRet=addr+5;
				DetourFunc((BYTE*)addr,(BYTE *)HookFunction,(BYTE *)RetFunction,5,HookFunRet,IsHook);
			}
			else
				DetourFunc((BYTE*)addr,(BYTE *)HookFunction,unhookbyte,5,HookFunRet,IsHook);

			return addr;
		}
		if(IsHook)
			HookFunRet=addr+5;

		DetourFunc((BYTE*)addr,(BYTE *)HookFunction,(BYTE *)RetFunction,5,HookFunRet,IsHook);
		//TSRuntime::add_log( "FunName:%s,addr:%x,RetFunction:%x",FunName,addr,RetFunction);
		return addr;
	}


}

void SetKeyDown(BYTE vk,char *g_pKetState)
{
	g_pKetState[vk]=0x80;
	//大小写转换
	//if(vk>='A' && vk<='Z')
	//{
	//	vk|=0x20;
	//}
	//for(int cnt=0;cnt<0x100;cnt++)
	//{
	//	if(TSRuntime::keyMap[cnt])
	//	{
	//		if(TSRuntime::keyMap[cnt]==vk)
	//		{
	//			g_pKetState[cnt]=0x80;
	//			//TSRuntime::add_log("SetKeyDown:key:%x,keyMap:%x",vk,cnt);
	//			break;
	//		}
	//	}
	//}
}

void SetKeyUp(BYTE vk,char *g_pKetState)
{
	g_pKetState[vk]=0;
	//ZeroMemory(g_pKetState,0x100);
	//大小写转换
	//if(vk>='A' && vk<='Z')
	//{
	//	vk|=0x20;
	//}
	//for(int cnt=0;cnt<0x100;cnt++)
	//{
	//	if(TSRuntime::keyMap[cnt])
	//	{
	//		if(TSRuntime::keyMap[cnt]==vk)
	//		{
	//			g_pKetState[cnt]=0;
	//			break;
	//		}
	//	}
	//}
}

BOOL __stdcall	MyPeekMessageW(  LPMSG lpMsg,   HWND hWnd,   UINT wMsgFilterMin, 
							   UINT wMsgFilterMax,   UINT wRemoveMsg )
{
	if(GameMsg==NULL)
	{
		if(lpMsg!=NULL)
		{
			if(lpMsg->hwnd==g_currentHwnd)
			{
				GameMsg=lpMsg;
				////卸载钩子
				//GetHookModFuntion(L"user32.dll","PeekMessageA",0,(DWORD)MyPeekMessageA,(DWORD)UnhookPeekMessage,HookPeekMessageARet,false);
				//GetHookModFuntion(L"user32.dll","PeekMessageW",0,(DWORD)MyPeekMessageW,(DWORD)UnhookPeekMessage,HookPeekMessageWRet,false);
				//TSRuntime::add_log( "MyPeekMessageW:GameMsg:%x",lpMsg);
			}

		}

	}
	bool bret=User32PeekMessageW(lpMsg,hWnd,wMsgFilterMin,wMsgFilterMax,wRemoveMsg);
	return bret;
}

BOOL __stdcall	MyPeekMessageA(  LPMSG lpMsg,   HWND hWnd,   UINT wMsgFilterMin, 
							   UINT wMsgFilterMax,   UINT wRemoveMsg )
{
	if(GameMsg==NULL)
	{
		if(lpMsg!=NULL)
		{
			if(lpMsg->hwnd==g_currentHwnd)
			{
				GameMsg=lpMsg;
				//////卸载钩子
				//GetHookModFuntion(L"user32.dll","PeekMessageA",0,(DWORD)MyPeekMessageA,(DWORD)UnhookPeekMessage,HookPeekMessageARet,false);
				//GetHookModFuntion(L"user32.dll","PeekMessageW",0,(DWORD)MyPeekMessageW,(DWORD)UnhookPeekMessage,HookPeekMessageWRet,false);
				//TSRuntime::add_log( "MyPeekMessageA:GameMsg:%x",lpMsg);
			}
		}
	}
	bool bret=User32PeekMessageA(lpMsg,hWnd,wMsgFilterMin,wMsgFilterMax,wRemoveMsg);
	return bret;
}

__declspec(naked)bool   __stdcall USER32IsWindowEnabled(HWND hwnd)
{
	_asm{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节,WIN7下7个字节为2个完整指令,所以多预留2个NOP指令为2个字节
			push ebp
			mov ebp,esp
			jmp HookIsWindowEnabledRet
			ret
	}	
}

__declspec(naked) UINT  __stdcall USER32GetRawInputData(HRAWINPUT hRawInput,
														UINT uiCommand,
														LPVOID pData,
														PUINT pcbSize,
														UINT cbSizeHeader)
{
	_asm{
		mov eax,g_GetRawInputData	//恢复前5个字节,在跳回原函数后5字节
			jmp HookGetRawInputDataRet
			ret
	}	
}

__declspec(naked) HCURSOR  __stdcall USER32SetCursor(HCURSOR  hCursor)
{
	_asm{
		mov eax,g_OriSetCursor	//恢复前5个字节,在跳回原函数后5字节
			jmp HookSetCursorRet
			ret
	}	
}

__declspec(naked) HWND  __stdcall USER32WindowFromPoint(POINT Point)
{
	_asm{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节,WIN7下7个字节为2个完整指令,所以多预留2个NOP指令为2个字节
			push ebp
			mov ebp,esp
			jmp HookWindowFromPointRet
			ret
	}	
}
//__declspec(naked) HWND  __stdcall USER32GetTopWindow(HWND hWnd)
//	{
//	_asm{
//			mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节,WIN7下7个字节为2个完整指令,所以多预留2个NOP指令为2个字节
//			push ebp
//			mov ebp,esp
//			jmp HookGetTopWindowRet
//			ret
//		}	
//	}

__declspec(naked) DWORD  __stdcall USER32GetMessagePos()
{
	_asm{
		NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			jmp HookGetMessagePosRet
			ret
	}	
}

__declspec(naked) HWND  __stdcall USER32GetForegroundWindow()
{
	_asm{
		mov edi,edi
			push ebp
			mov ebp,esp
			jmp HookGetForegroundWindowRet
			ret
	}	
}

__declspec(naked) HWND  __stdcall USER32GetActiveWindow()
{
	_asm{
		//mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节,WIN7下7个字节为2个完整指令,所以多预留2个NOP指令为2个字节
		//push ebp
		//mov ebp,esp
		NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			jmp HookGetActiveWindowRet
			ret
	}	
}

__declspec(naked) HWND  __stdcall USER32GetFocus()
{
	_asm{
		NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP
			jmp HookGetFocusRet
			ret
	}	
}

__declspec(naked) SHORT  __stdcall USER32GetKeyState(int nVirtKey)
{
	_asm{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节,WIN7下7个字节为2个完整指令,所以多预留2个NOP指令为2个字节
			push ebp
			mov ebp,esp
			jmp HookGetKeyStateRet
			ret
	}	
}
//APIENTRY
__declspec(naked) HRESULT __stdcall GetDeviceStateKbd(					  
	LPDIRECTINPUTDEVICE8 m_pKeyboard,  //当前设备指针
	DWORD cbData,            //缓冲区字节大小
	LPVOID lpvData)           //缓冲区指针)//键盘DXINPUT	 

{
	_asm
	{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节
			push ebp
			mov ebp,esp
			NOP
			jmp HookGetDeviceStateKbdRet
			ret
	}
}
__declspec(naked) HRESULT __stdcall GetDeviceStateMouse(
	LPDIRECTINPUTDEVICE8 m_pMouse,
	DWORD cbObjectData,
	LPDIDEVICEOBJECTDATA rgdod,
	LPDWORD pdwInOut,
	DWORD dwFlags)	//鼠标DXINPUT  
{
	_asm{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节
			push ebp
			mov ebp,esp
			jmp HookGetDeviceStateMouseRet
			ret
	}	
}


__declspec(naked) HRESULT  __stdcall D3D9Present(LPDIRECT3DDEVICE9 pDxdevice,//类的this指针 
												 CONST RECT * pSourceRect,//此参数请参考dx sdk 
												 CONST RECT * pDestRect,//同上 
												 HWND hDestWindowOverride,//同上 
												 CONST RGNDATA * pDirtyRegion)//同上 )
{
	_asm{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节
			push ebp
			mov ebp,esp
			jmp HookD3D9PresentRet
			ret
	}	
}


__declspec(naked) BOOL __stdcall USER32ScreenToClient( HWND hWnd,LPPOINT lpPoint)   // screen coordinates
{
	_asm{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节,WIN7下7个字节为2个完整指令,所以多预留2个NOP指令为2个字节
			push ebp
			mov ebp,esp
			NOP
			NOP
			jmp HookScreenToClientRet
			ret
	}	
}


__declspec(naked) BOOL __stdcall USER32GetCursorPos(LPPOINT lpPoint)
{
	_asm{
		mov edi,edi		    //恢复前5个字节,在跳回原函数后5字节
			push ebp
			mov ebp,esp
			jmp HookGetCursorPosRet
			ret
	}
}

__declspec(naked) BOOL __stdcall USER32SetCursorPos(int x,int y)
{
	_asm{
		mov edi,edi		    //恢复前5个字节,在跳回原函数后5字节
			push ebp
			mov ebp,esp
			jmp HookSetCursorPosRet
			ret
	}
}
//

__declspec(naked) LRESULT __stdcall USER32InternalCallWinProc(DWORD fAnsi,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	_asm{
		push EBP     
			Mov EBP,ESP
			push ESI
			push EDI
			jmp HookInternalCallWinProcRet
			ret
	}
}


__declspec(naked) HRESULT APIENTRY kdbGetDeviceState(DWORD OBJDeviceAddr, LPVOID lpvData)
{
	_asm
	{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节
			push ebp
			mov ebp,esp
			jmp HookGetDeviceStateRet
			ret
	}
}

__declspec(naked) LONG __stdcall IMM32ImmGetCompositionStringA( HIMC hIMC,      
															   DWORD dwIndex,  
															   LPVOID lpBuf,   
															   DWORD dwBufLen )
{
	_asm
	{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节
			push ebp
			mov ebp,esp
			jmp HookImmGetCompositionStringARet
			ret
	}

}

__declspec(naked) LONG __stdcall IMM32ImmGetCompositionStringW( HIMC hIMC,      
															   DWORD dwIndex,  
															   LPVOID lpBuf,   
															   DWORD dwBufLen )
{
	_asm
	{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节
			push ebp
			mov ebp,esp
			jmp HookImmGetCompositionStringWRet
			ret
	}

}


__declspec(naked) bool __stdcall USER32IsIconic(HWND hWnd)
{
	_asm
	{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节
			push ebp
			mov ebp,esp
			jmp HookIsIconicRet
			ret
	}

}
__declspec(naked) SHORT __stdcall USER32GetAsyncKeyState( int vKey )
{
	_asm
	{
		mov edi,edi		   //恢复前5个字节,在跳回原函数后5字节
			push ebp
			mov ebp,esp
			jmp HookGetAsyncKeyStateRet
			ret
	}
}

DWORD dwGetCapture=0;
__declspec(naked) HWND __stdcall USER32GetCapture()
{
	_asm
	{
		mov eax,[dwGetCapture]		   //恢复前5个字节,在跳回原函数后5字节
		jmp HookGetCaptureRet
			ret
	}
}

__declspec(naked) HWND __stdcall USER32GetKeyboardState(PBYTE pbKeyState)
{
	_asm
	{
		mov eax,dwMoveGetKeyboardState		   //恢复前5个字节,在跳回原函数后5字节
			jmp HookGetKeyboardStateRet
			ret
	}
}

bool  __stdcall MyIsIconic(HWND hWnd)
{
	if(hWnd==g_currentHwnd)
		return 0;
	else
		return USER32IsIconic(hWnd);

}

BOOL __stdcall MyGetKeyboardState(PBYTE pbKeyState)
{

	//if(g_message==WM_KEYDOWN||g_message==WM_SYSKEYDOWN||g_message==WM_KEYUP||g_message==WM_SYSKEYUP)
	//	TSRuntime::add_log("MyGetKeyboardState:g_message:%x,pbKeyState:%x,g_wParam:%x",g_message,pbKeyState,g_wParam);
	if((g_message==WM_KEYDOWN||g_message==WM_SYSKEYDOWN)&&g_keyboardstate)
	{
		for(int i=0;i<0x100;i++)
		{
			if(1==TSRuntime::KeyPressMap[i])
			{
				//pbKeyState[i] |= 0x80;
				//int key=TSRuntime::VirtualToASCIIMap[i];
				int key=MapVirtualKey(i,0);
				if(i==VK_LEFT)
					key=DIK_LEFT;
				else if(i==VK_RIGHT)
					key=DIK_RIGHT;
				else if(i==VK_DOWN)
					key=DIK_DOWN;
				else if(i==VK_UP)
					key=DIK_UP;
				//pbKeyState[key] |= 0x80;
				SetKeyDown(key,(char*)pbKeyState);
				return true;
			}
		}

		//TSRuntime::add_log("SetKeyDown:g_message:%x,lpvData:%x,Key.vk:%x",g_message,pbKeyState,g_wParam);
	}
	else if((g_message==WM_KEYUP||g_message==WM_SYSKEYUP)&&g_keyboardstate)
	{
		int key=0;
		for(int i=0;i<0x100;i++)
		{
			if(1==TSRuntime::KeyPressMap[i]&&i==g_wParam)
			{
				//pbKeyState[i]= 0;
				//int key=TSRuntime::VirtualToASCIIMap[i];
				key=MapVirtualKey(i,0);
				if(i==VK_LEFT)
					key=DIK_LEFT;
				else if(i==VK_RIGHT)
					key=DIK_RIGHT;
				else if(i==VK_DOWN)
					key=DIK_DOWN;
				else if(i==VK_UP)
					key=DIK_UP;
				SetKeyUp(key,(char*)pbKeyState);
				//pbKeyState[key] = 0;
				TSRuntime::KeyPressMap[i]=0;
				
			}
		}
		//TSRuntime::add_log("MyGetKeyboardState:g_message:%x,key:%x,g_wParam:%x",g_message,key,g_wParam);
		g_isSetKeyDown=false;//重置按键标记
		g_keyboardstate=false;
		return true;
	}
	else if(g_kbdDevice==false)
	{
		if(g_LockInput_kdb)	//是否锁定键盘
		{
			DWORD cbData=sizeof(pbKeyState);
			ZeroMemory(pbKeyState,cbData);
		}
	}
	bool bret=USER32GetKeyboardState(pbKeyState);
	return bret;
}



SHORT __stdcall MyGetAsyncKeyState(   int vKey )
{
	SHORT sret=USER32GetAsyncKeyState(vKey);
	//TSRuntime::add_log("MyGetAsyncKeyState:sret:%d,vKey:%d",sret,vKey);
	if(g_kbdDevice||g_mouseDevice)
	{
		if((g_message==WM_LBUTTONDOWN&&vKey==VK_LBUTTON)
			||(g_message==WM_RBUTTONDOWN&&vKey==VK_RBUTTON)
			||(g_message==WM_MBUTTONDOWN&&vKey==VK_MBUTTON))
		{
			sret=0x8000;
			return sret;
		}
		else if((g_message==WM_LBUTTONUP&&vKey==VK_LBUTTON)
			||(g_message==WM_RBUTTONUP&&vKey==VK_LBUTTON)
			||(g_message==WM_MBUTTONUP&&vKey==VK_MBUTTON))
		{
			sret=0;
			g_mouseDevice=false;
		}
		else if(g_message==WM_KEYDOWN||g_message==WM_KEYUP||g_message==WM_SYSKEYDOWN||g_message==WM_SYSKEYUP)
		{
			bool iskeyevent=false;
			for(int i=0;i<0x100;i++)
			{
				if(1==TSRuntime::KeyPressMap[i])
				{
					//int key=TSRuntime::VirtualToASCIIMap[i];
					if(vKey==i)
					{
						if((g_message==WM_KEYUP || g_message==WM_SYSKEYUP)	&& i == g_wParam)
							TSRuntime::KeyPressMap[i]=0;
						else if(i != g_wParam)
							return 0x8000;
						iskeyevent=true;
						break;
					}
				}

			}
			//TSRuntime::add_log("MyGetAsyncKeyState:iskeyevent:%d,vKey:%d,g_message:%x",iskeyevent,vKey,g_message);
			if(iskeyevent&&(g_message==WM_KEYDOWN||g_message==WM_SYSKEYDOWN))
			{
				sret=0x8000;
				//TSRuntime::add_log("iskeyevent:MyGetAsyncKeyState:sret:%d,vKey:%d",sret,vKey);
				return sret;
			}
			else if(iskeyevent&&(g_message==WM_KEYUP||g_message==WM_SYSKEYUP))
			{
				//g_keydown=0xFFFF8001;//重置
				sret=0;
				g_mouseDevice=false;
			}
		}

	}
	if(g_LockInput_kdb||g_LockInput_mouse)   //封锁外部键盘鼠标
	{
		if(g_LockInput_mouse)
		{
			if(vKey==VK_LBUTTON||vKey==VK_RBUTTON||vKey==VK_MBUTTON)
				sret=0;
		}
		else if(g_LockInput_kdb)
		{
			sret=0;
		}
	}
	//TSRuntime::add_log("MyGetAsyncKeyState:sret:%d,vKey:%d",sret,vKey);
	return sret;
}

LONG __stdcall MyIMM32ImmGetCompositionStringA( HIMC hIMC,      
											   DWORD dwIndex,  
											   LPVOID lpBuf,   
											   DWORD dwBufLen )
{
	ULONG nret=0;
	if(dwIndex==GCS_RESULTSTR&&IsSendstrIme==true)
	{
		int len=strlen(TSRuntime::pData->pmsg);
		char *strbuf=new char[len+1];
		memset(strbuf,0,len);
		strcpy(strbuf,TSRuntime::pData->pmsg);
		dwBufLen=len;
		if(lpBuf!=NULL)
		{
			strcpy((char*)lpBuf,strbuf);
			dwBufLen=strlen((char*)lpBuf);
			IsSendstrIme=false;
		}
		delete []strbuf;
		return	dwBufLen;
	}

	nret=IMM32ImmGetCompositionStringA(hIMC,dwIndex,lpBuf,dwBufLen);

	return nret;

}

LONG __stdcall MyIMM32ImmGetCompositionStringW( HIMC hIMC,      
											   DWORD dwIndex,  
											   LPVOID lpBuf,   
											   DWORD dwBufLen )
{
	ULONG nret=0;
	if(dwIndex==GCS_RESULTSTR&&IsSendstrIme==true)
	{
		USES_CONVERSION;
		int len=strlen(TSRuntime::pData->pmsg)*2;
		wchar_t *strbuf=new wchar_t[len+1];
		memset(strbuf,0,len);
		wcscpy(strbuf,A2W(TSRuntime::pData->pmsg));
		dwBufLen=len;
		if(lpBuf!=NULL)
		{
			wcscpy((wchar_t*)lpBuf,strbuf);
			dwBufLen=wcslen((wchar_t*)lpBuf);
			IsSendstrIme=false;
		}
		delete []strbuf;
		return	dwBufLen;
	}

	nret=IMM32ImmGetCompositionStringW(hIMC,dwIndex,lpBuf,dwBufLen);

	return nret;
}




UINT   __stdcall MyGetRawInputData( HRAWINPUT hRawInput,
								   UINT uiCommand,
								   LPVOID pData,
								   PUINT pcbSize,
								   UINT cbSizeHeader
								   )
{
	UINT nret=0;
	RAWINPUT* raw=NULL;
	if(TSRuntime::pData)
		TSRuntime::pData->iskeyboardhook=true;

	//TSRuntime::add_log("MyGetRawInputData:pData:%x",(DWORD)pData);
	if(pData)
	{
		raw = (RAWINPUT*)pData;
		if(g_GetRawInputDatahook)
		{
			if(g_message==WM_KEYDOWN||g_message==WM_KEYUP||g_message==WM_SYSKEYDOWN||g_message==WM_SYSKEYUP)
			{
				raw->header.dwType=RIM_TYPEKEYBOARD;
				raw->header.dwSize=sizeof(RAWINPUTHEADER);
				//raw->header.hDevice
				raw->data.keyboard.Message=g_message;//TSRuntime::pData->message;
				if(g_message==WM_KEYUP||g_message==WM_SYSKEYUP)//弹起
					raw->data.keyboard.Flags=RI_KEY_BREAK;

				else//按下
					raw->data.keyboard.Flags=RI_KEY_MAKE;

				raw->data.keyboard.ExtraInformation=(ULONG)GetMessageExtraInfo();
				for(int i=0;i<0x100;i++)
				{
					if(1==TSRuntime::RawInputKeyPressMap[i])
					{
						raw->data.keyboard.VKey=i;
						int key=MapVirtualKey(i,0);
						raw->data.keyboard.MakeCode=key;
					}
				}
				if(g_message==WM_KEYUP||g_message==WM_SYSKEYUP)
				{
					for(int i=0;i<0x100;i++)
					{
						if(1==TSRuntime::KeyPressMap[i]&&i==g_wParam)
						{
							TSRuntime::RawInputKeyPressMap[i]=0;
							raw->data.keyboard.VKey=i;
							int key=MapVirtualKey(i,0);
							raw->data.keyboard.MakeCode=key;
						}
					}
					g_GetRawInputDatahook=false;
				}
				*pcbSize=g_GetRawInputKeyDataSize;
				return g_GetRawInputKeyDataSize;//返回键盘数据结构大小
			}
			else//鼠标操作
			{
				raw->header.dwType=RIM_TYPEMOUSE;
				raw->header.dwSize=sizeof(RAWINPUTHEADER);

				raw->data.mouse.lLastX=g_x;
				raw->data.mouse.lLastX=g_y;

				if(g_message==WM_MOUSEMOVE)
					raw->data.mouse.usFlags=MOUSE_MOVE_ABSOLUTE;
				else if(g_message==WM_LBUTTONDOWN)
					raw->data.mouse.usButtonFlags=RI_MOUSE_LEFT_BUTTON_DOWN;//,RI_MOUSE_BUTTON_1_DOWN
				else if(g_message==WM_LBUTTONUP)
					raw->data.mouse.usButtonFlags=RI_MOUSE_LEFT_BUTTON_UP;//,RI_MOUSE_BUTTON_1_UP
				else if(g_message==WM_RBUTTONDOWN)
					raw->data.mouse.usButtonFlags=RI_MOUSE_RIGHT_BUTTON_DOWN;//,RI_MOUSE_BUTTON_2_DOWN
				else if(g_message==WM_RBUTTONUP)
					raw->data.mouse.usButtonFlags=RI_MOUSE_RIGHT_BUTTON_UP;//,RI_MOUSE_BUTTON_2_UP
				else if(g_message==WM_MBUTTONDOWN)
					raw->data.mouse.usButtonFlags=RI_MOUSE_MIDDLE_BUTTON_DOWN;//,RI_MOUSE_BUTTON_3_DOWN
				else if(g_message==WM_MBUTTONUP)
					raw->data.mouse.usButtonFlags=RI_MOUSE_MIDDLE_BUTTON_UP;//,RI_MOUSE_BUTTON_3_UP
				else if(g_message==WM_MOUSEWHEEL)
				{
					raw->data.mouse.usButtonFlags=RI_MOUSE_WHEEL;
					raw->data.mouse.usButtonData=g_dwData;
				}
				raw->data.mouse.ulExtraInformation=(ULONG)GetMessageExtraInfo();

				*pcbSize=g_GetRawInputMouDataSize;

				g_GetRawInputDatahook=false;
				return g_GetRawInputMouDataSize;
			}
		}
	}
	else if(g_message==WM_KEYDOWN||g_message==WM_KEYUP||g_message==WM_SYSKEYDOWN||g_message==WM_SYSKEYUP)//键盘
	{
		*pcbSize=g_GetRawInputKeyDataSize;
		return g_GetRawInputKeyDataSize;//返回键盘数据结构大小
	}
	else//鼠标
	{
		*pcbSize=g_GetRawInputMouDataSize;
		return g_GetRawInputMouDataSize;
	}

	nret=USER32GetRawInputData(hRawInput,uiCommand,pData,pcbSize,cbSizeHeader);
	////if(pData==NULL)
	////{
	////	g_GetRawInputDataRetSize=*pcbSize;
	//	TSRuntime::add_log("g_GetRawInputDataRetSize:%x",g_GetRawInputDataRetSize);
	////}
	//if(pData)
	//	{
	//		raw = (RAWINPUT*)pData;
	//		//if(raw->data.keyboard.Message==WM_KEYDOWN||raw->data.keyboard.Message==WM_KEYUP)
	//		{
	//			TSRuntime::add_log("MyGetRawInputData:pData:%x,VKey:%x,Message:%x,Flags:%x,ExtraInformation:%x,MakeCode:%x,Reserved:%x,nret:%x,",pData,raw->data.keyboard.VKey,raw->data.keyboard.Message,raw->data.keyboard.Flags,raw->data.keyboard.ExtraInformation,raw->data.keyboard.MakeCode,raw->data.keyboard.Reserved,nret);
	//			//TSRuntime::add_log("lLastX:%d,lLastY:%d,ulButtons:%x,ulExtraInformation:%x,ulRawButtons:%x,usButtonData:%x,usButtonFlags:%x,usFlags:%x",raw->data.mouse.lLastX,raw->data.mouse.lLastY,raw->data.mouse.ulButtons,raw->data.mouse.ulExtraInformation,raw->data.mouse.ulRawButtons,raw->data.mouse.usButtonData,raw->data.mouse.usButtonFlags,raw->data.mouse.usFlags);
	//		
	//			//TSRuntime::add_log("dwSize:%x,dwType:%x,hDevice:%x,wParam:%x",raw->header.dwSize,raw->header.dwType,raw->header.hDevice,raw->header.wParam);
	//		}
	//	}
	//else

	//if(pData==NULL&&pcbSize)
	//	{
	//		g_GetRawInputDataRetSize=0x20;
	//	}

	//if(pData&&nret==0x20)
	//	{
	//	raw = (RAWINPUT*)pData;
	//	TSRuntime::add_log("MyGetRawInputData:pData:%x,VKey:%x,Message:%x,Flags:%x,ExtraInformation:%x,MakeCode:%x,Reserved:%x,nret:%x,",pData,raw->data.keyboard.VKey,raw->data.keyboard.Message,raw->data.keyboard.Flags,raw->data.keyboard.ExtraInformation,raw->data.keyboard.MakeCode,raw->data.keyboard.Reserved,nret);
	//	}
	return   nret;
}

DWORD   __stdcall MyGetMessagePos()
{
	//DWORD  ret=USER32GetMessagePos();
	POINT xy;
	GetCursorPos(&xy);
	return  (DWORD)MAKELPARAM(xy.x,xy.y);
}

HCURSOR  __stdcall MySetCursor(HCURSOR hCursor)
{
	HCURSOR rethCursor=USER32SetCursor(hCursor);
	g_bkmouseshape=hCursor;	   //记录游戏鼠标hCursor
	//TSRuntime::add_log("MySetCursor:%x",g_bkmouseshape);
	return   rethCursor;
}


bool  __stdcall MyIsWindowEnabled(HWND hWnd)
{
	//bool bret=USER32IsWindowEnabled(hWnd);
	bool bret=false;
	if( hWnd==g_currentHwnd)
		bret=true;
	//TSRuntime::add_log("MyIsWindowEnabled");
	return bret;
}

HWND  __stdcall MyWindowFromPoint(POINT Point)
{
	//HWND hwnd=USER32WindowFromPoint(Point);
	HWND hwnd;
	//if(g_keymouseevent)
	hwnd=g_currentHwnd;
	//else
	//hwnd=g_InjectHWND;

	//TSRuntime::add_log("MyWindowFromPoint");
	return hwnd;
}

HWND  __stdcall MyGetTopWindow(HWND hWnd)
{
	//HWND rethwnd=USER32GetTopWindow(hWnd);
	//if(g_currentHwnd)
	HWND rethwnd;
	//if(g_keymouseevent)
	rethwnd=g_currentHwnd;
	//else
	//rethwnd=g_InjectHWND;
	//TSRuntime::add_log("MyGetTopWindow");
	return rethwnd;
}

HWND  __stdcall MyGetForegroundWindow()
{
	HWND hwnd;
	//if(g_keymouseevent)
	hwnd=g_currentHwnd;
	//else
	//hwnd=g_InjectHWND;
	return hwnd;
}

HWND  __stdcall MyGetFocus()
{
	//HWND hwnd=USER32GetFocus();
	//HWND hwnd=g_currentHwnd;
	//TSRuntime::add_log("MyGetFocus");
	HWND hwnd;
	//if(g_keymouseevent)
	hwnd=g_currentHwnd;
	//else
	//hwnd=g_InjectHWND;
	return hwnd;
}

HWND __stdcall MyGetCapture()
{
	return g_currentHwnd;
}


HWND  __stdcall MyGetActiveWindow()
{
	//HWND hwnd=USER32GetActiveWindow();
	//HWND hwnd=g_currentHwnd;
	//TSRuntime::add_log("MyGetActiveWindow");
	HWND hwnd;
	//if(g_keymouseevent)
	hwnd=g_currentHwnd;
	//else
	//hwnd=g_InjectHWND;
	return hwnd;
}

COLORREF** pWndBmpBuffer=NULL;
short **pWndBmpStrxy=NULL;
BYTE * pBuffer=NULL;
//int addd=0;
bool UnhookPresent=false;
DWORD DxdeviceAddr=0;
DWORD Ori_DxdeviceAddr=0;//原始DX地址;
HRESULT  __stdcall MyD3D9Present(LPDIRECT3DDEVICE9 pDxdevice,//类的this指针 
								 CONST RECT * pSourceRect,//此参数请参考dx sdk 
								 CONST RECT * pDestRect,//同上 
								 HWND hDestWindowOverride,//同上 
								 CONST RGNDATA * pDirtyRegion)//同上 )
{

	TSRuntime::pData->ispichook=true;
	if(DxdeviceAddr==0&&TSRuntime::pData->isgraphicprotect)
	{
		DxdeviceAddr=*(DWORD*)pDxdevice;
		DxdeviceAddr+=0x44;//偏移0x44取DX真正函数的地址
	}
	if(sleeptime!=0)
		Sleep(sleeptime);

	wchar_t pszEventName[MAX_PATH]={0};
	::wsprintf(pszEventName,L"%s%d",TS_EVENT_NAME,::GetCurrentProcessId());
	HANDLE hEvent=OpenEvent(EVENT_ALL_ACCESS,FALSE,pszEventName);
	//TSRuntime::add_log( "dx:MyD3D9Present");
	if(hEvent)
	{
		//TSRuntime::add_log( "MyD3D9PresentHOOK:event:%d  name %s",hEvent,pszEventName);
		IDirect3DSurface9 *pSourceSurface=NULL;
		D3DLOCKED_RECT d3drect;
		D3DSURFACE_DESC surfaceDesc;
		IDirect3DSurface9  *MySurface=NULL;
		int wide=0;
		int hight=0;
		if(D3D_OK   ==   pDxdevice-> GetRenderTarget(0,&pSourceSurface))   //得到当前游戏缓存数据对象指针
		{
			//TSRuntime::add_log( "dx:GetRenderTarget");
			RECT rect;
			HDC myDc;
			HRESULT hret;
			if(D3D_OK   ==   pSourceSurface->GetDesc(&surfaceDesc))	 //获取当游戏前缓存数据的属性,主要是获取surfaceDesc.Format格式
			{
				//TSRuntime::add_log( "dx:GetDesc");
				if(GetClientRect(g_currentHwnd,&rect))	//获取当前游戏窗口的大小
				{
					wide=  rect.right-rect.left;
					hight=	rect.bottom- rect.top;
					if(surfaceDesc.Height>hight||surfaceDesc.Width>wide)
					{
						wide=surfaceDesc.Width;
						hight=surfaceDesc.Height;
					}
					if(D3D_OK   ==  pDxdevice->CreateOffscreenPlainSurface(wide,hight,surfaceDesc.Format,D3DPOOL_SYSTEMMEM,&MySurface,0)) //创建自己的D3DPOOL=D3DPOOL_SYSTEMMEM的缓存数据对象指针
					{
						if(D3D_OK   ==  pDxdevice->GetRenderTargetData(pSourceSurface,MySurface))  //拷贝当前游戏缓存数据对象给我们自己创建的缓存数据对象
						{

							//hret=MySurface->LockRect(&d3drect,&rect,D3DLOCK_READONLY);   //锁定缓存数据得到图色数据指针
							//TSRuntime::add_log( "dx:LockRect");
							//转换数据
							D3DSURFACE_DESC surfaceDesc;
							if(D3D_OK   == MySurface->GetDesc(&surfaceDesc)	)
							{
								wide=surfaceDesc.Width;
								hight=surfaceDesc.Height;
								//TSRuntime::add_log( "surfaceDesc.Width:%d,surfaceDesc.Height:%d",surfaceDesc.Width,surfaceDesc.Height);
							}

							MyFindPictureClass  fpic;
							if(wcslen(TSRuntime::SetPath)>0)
							{
								wcscpy(fpic.SetPath,TSRuntime::SetPath);
							}
							ST_TSFINDPIC *spic=(ST_TSFINDPIC *)TSRuntime::pData->pmsg;

							RECT pSrcRect;
							pSrcRect.top=spic->top;
							pSrcRect.left=spic->left;
							pSrcRect.right=spic->right;
							pSrcRect.bottom=spic->bottom;
							//TSRuntime::add_log( "pSrcRect.top:%d,pSrcRect.left:%d,pSrcRect.right:%d,pSrcRect.bottom:%d",pSrcRect.top,pSrcRect.left,pSrcRect.right,pSrcRect.bottom);
							HDC mydc=NULL, hMemDC;
							MySurface->GetDC(&mydc);
							//TSRuntime::add_log( "MySurface->GetDC,mydc:%x",(DWORD)mydc);
							HBITMAP hBitmap,hOldBitmap; 
							if(mydc!=NULL)
							{
								int xc = 0;
								int cx = 0;
								int cy = 0;

								//判断边境值
								if(pSrcRect.right<=0)
									pSrcRect.right=wide-1;

								if(pSrcRect.bottom<=0)
									pSrcRect.bottom=hight-1;

								if(pSrcRect.bottom>hight)
									pSrcRect.bottom=hight-1;

								if(pSrcRect.right>wide)
									pSrcRect.right=wide-1;

								//TSRuntime::add_log( "wide:%d,hight:%d",wide,hight);
								//为DX设备描述表创建兼容的内存设备描述表
								hMemDC = CreateCompatibleDC(mydc);
								// 创建一个与屏幕设备描述表兼容的位图
								int picwide=(pSrcRect.right-pSrcRect.left);
								int pichight=(pSrcRect.bottom-pSrcRect.top);
								
								hBitmap=CreateCompatibleBitmap(mydc,picwide,pichight);
								// 把新位图选到内存设备描述表中
								hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);
								// 把屏幕设备描述表拷贝到内存设备描述表中
								//if(TSRuntime::pData->type==TS_CAPTURE||TSRuntime::pData->type==TS_GETSCREENDATA)
								BitBlt(hMemDC,0,0,picwide,pichight,mydc,cx+pSrcRect.left,xc+cy+pSrcRect.top, CAPTUREBLT|SRCCOPY);
								//得到屏幕位图的句柄
								hBitmap=(HBITMAP)SelectObject(hMemDC,hOldBitmap);
								switch(TSRuntime::pData->type)
								{
								case TS_CAPTURE:
									{
										wchar_t savepath[MAX_PATH]={0};
										wchar_t *findpath=wcsstr(spic->path,L":");
										if(wcslen(TSRuntime::SetPath)>1&&findpath==NULL)
										{
											swprintf(savepath,L"%s\\%s",TSRuntime::SetPath,spic->path);
										}
										else
										{
											swprintf(savepath,L"%s",spic->path);
										}
										//HRESULT hsret=D3DXSaveSurfaceToFile(spic->path,(D3DXIMAGE_FILEFORMAT)spic->pictype,pSourceSurface,NULL,&pSrcRect);
										//spic->ret=(int)hsret;
										if(spic->pictype==D3DXIFF_JPG||spic->pictype==D3DXIFF_PNG)
										{
											wchar_t temppath[MAX_PATH]={0};
											wchar_t path[MAX_PATH]={0};
											GetTempPath(MAX_PATH,temppath);
											swprintf(path,L"%s\\TEMPBMP%d.BMP",temppath,::GetCurrentProcessId());
											if(fpic.SaveBitmapToFile(hBitmap,path))
											{
												if(spic->pictype==D3DXIFF_JPG)
													spic->ret=fpic.BMP2JPG(path,savepath);
												else if(spic->pictype==D3DXIFF_PNG)
													spic->ret=fpic.BMP2PNG(path,savepath);
												DeleteFile(path);
											}
										}
										else if(spic->pictype==D3DXIFF_BMP)
										{
											spic->ret=fpic.SaveBitmapToFile(hBitmap,savepath);
										}
										break;
									}


								case TS_GETSCREENDATA:
									{
										// 24位图的BITMAPINFO
										//memset(TSRuntime::pData->ColorDataInfo,0,sizeof(TSRuntime::pData->ColorDataInfo));
										BITMAPINFO *pBITMAPINFO = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER));
										memset(pBITMAPINFO, 0, sizeof(BITMAPINFOHEADER));
										BITMAPINFOHEADER *pInfo_Header = (BITMAPINFOHEADER *)pBITMAPINFO;
										pInfo_Header->biSize = sizeof(BITMAPINFOHEADER);   
										pInfo_Header->biWidth = pSrcRect.right - pSrcRect.left;   
										pInfo_Header->biHeight = pSrcRect.bottom - pSrcRect.top;   
										pInfo_Header->biPlanes = 1;   
										pInfo_Header->biBitCount = 24;   
										pInfo_Header->biCompression = BI_RGB;
										// 获得数据buf
										DWORD bufSize=(pInfo_Header->biWidth * 3 + 3) / 4 * 4 * pInfo_Header->biHeight;
										BYTE * pBuffer = new BYTE[bufSize];

										int aHeight=pInfo_Header->biHeight;

										if(::GetDIBits(hMemDC, hBitmap, 0, aHeight, pBuffer,pBITMAPINFO, DIB_RGB_COLORS) == 0)
										{
											free(pBITMAPINFO);
											delete [] pBuffer;
											break;
										}

										int wndWidth=pBITMAPINFO->bmiHeader.biWidth;
										int wndHeight=pBITMAPINFO->bmiHeader.biHeight;

										//HDC dd = ::GetWindowDC(::GetDesktopWindow());
										
										COLORREF helpcolor=0;
										int pitch=wndWidth%4;
										for(int i=0;i<wndHeight;i++)
										{
											int realPitch=i*pitch;
											for(int j=0;j<wndWidth;j++)
											{
												UCHAR b=pBuffer[(i*wndWidth+j)*3+realPitch];
												UCHAR g=pBuffer[(i*wndWidth+j)*3+1+realPitch];
												UCHAR r=pBuffer[(i*wndWidth+j)*3+2+realPitch];
												helpcolor=RGB(r,g,b);
												//TSRuntime::add_log( "TS_GETSCREENDATA:%x",TSRuntime::pData->ColorDataInfo);
												TSRuntime::pData->ColorDataInfo[wndHeight-i-1][j]=helpcolor;
												//pColorBuffer[wndHeight-i-1][j]=helpcolor;
												//::SetPixel(dd,wndHeight-i-1,j,helpcolor);
											}
										}
										//TSRuntime::add_log( "TS_GETSCREENDATA:%x,wndWidth:%d,wndHeight:%d",TSRuntime::pData->ColorDataInfo,wndWidth,wndHeight);
										free(pBITMAPINFO);
										delete [] pBuffer;
										break;
									}
								}
								DeleteDC(hMemDC);
								DeleteObject(hOldBitmap);
								DeleteObject(hBitmap);
							}
							MySurface->ReleaseDC(mydc);;
						}
						MySurface->Release();											//释放自身缓存对象
					}
				}
			}
			pSourceSurface->Release();													 //释放当前游戏缓存数据对象
		}
		//TSRuntime::pData->ispichook=false;
		SetEvent(hEvent);
		::CloseHandle(hEvent);
	}
	HRESULT  rett=D3D9Present(pDxdevice,pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);
	//TSRuntime::add_log( "MyD3D9PresentHOOK:pDxdevice:%x,pSourceRect:%x,pDestRect:%x,hDestWindowOverride:%x,pDirtyRegion:%x",pDxdevice,pSourceRect,pDestRect,hDestWindowOverride,pDirtyRegion);
	return rett;
}

SHORT   __stdcall MyGetKeyState(int nVirtKey )
{
	//TSRuntime::add_log("NtUserGetKeyState--nVirtKey:%x",nVirtKey);
	ULONG  sret=0;
	if(g_GetKeyState||g_mouseDevice)
	{
		DWORD ALT_KEYCODE=0;
		bool iskeyevent=false;
		for(int i=0;i<0x100;i++)
		{
			if(1==TSRuntime::KeyPressMap[i])
			{
				if(nVirtKey==i)
				{
					iskeyevent=true;
					break;
				}
				else if((i==VK_MENU&&(nVirtKey==VK_LMENU||nVirtKey==VK_RMENU))||(i==VK_SHIFT&&(nVirtKey==VK_LSHIFT||nVirtKey==VK_RSHIFT))
					||(i==VK_CONTROL&&(nVirtKey==VK_LCONTROL||nVirtKey==VK_RCONTROL)))
				{
					iskeyevent=true;
					break;
				}
			}
		}
		if(g_message==WM_LBUTTONDOWN||g_message==WM_RBUTTONDOWN||g_message==WM_MBUTTONDOWN
			||g_message==WM_LBUTTONUP||g_message==WM_RBUTTONUP||g_message==WM_MBUTTONUP)
		{
			if(nVirtKey==VK_LBUTTON||nVirtKey==VK_RBUTTON||nVirtKey==VK_MBUTTON)
			{
				iskeyevent=true;
				if(g_message==WM_LBUTTONDOWN||g_message==WM_RBUTTONDOWN||g_message==WM_MBUTTONDOWN)
					ismousedown=true;
				//TSRuntime::add_log( "MyGetKeyState-1:g_message:%x,nVirtKey:%x,ismousedown:%d",g_message,nVirtKey,ismousedown);
			}
			if(g_message==WM_LBUTTONUP||g_message==WM_RBUTTONUP||g_message==WM_MBUTTONUP)
			{
				ismousedown=false;
				g_mouseDevice=false;
			}
			//TSRuntime::add_log( "MyGetKeyState:g_message:%x,nVirtKey:%x,ismousedown:%d",g_message,nVirtKey,ismousedown);
		}
		//if((g_message==WM_KEYDOWN&&g_wParam==nVirtKey)||(g_message==WM_SYSKEYDOWN&&nVirtKey==VK_MENU)||(g_message==WM_SYSKEYDOWN&&nVirtKey==VK_LMENU) )
		if((((g_message==WM_KEYDOWN)||(g_message==WM_SYSKEYDOWN)
			||(g_message==WM_LBUTTONDOWN)
			||(g_message==WM_RBUTTONDOWN)
			||(g_message==WM_MBUTTONDOWN))&&iskeyevent)||((nVirtKey==VK_LBUTTON||nVirtKey==VK_RBUTTON||nVirtKey==VK_MBUTTON)&&ismousedown))	
		{
			sret=0xffffff80;
			//TSRuntime::add_log( "MyGetKeyState-WM_SYSKEYDOWN:g_message:%x,nVirtKey:%x,ismousedown:%d",g_message,nVirtKey,ismousedown);
			//返回负数说明该键被按下
		}
		//else if((g_message==WM_KEYUP||g_message==WM_SYSKEYUP)&&g_wParam==nVirtKey)
		else if(((g_message==WM_KEYUP||g_message==WM_SYSKEYUP)
			||(g_message==WM_LBUTTONUP)
			||(g_message==WM_RBUTTONUP)
			||(g_message==WM_MBUTTONUP))&&iskeyevent)	
		{
			//TSRuntime::add_log( "MyGetKeyState-WM_SYSKEYUP:g_message:%x,nVirtKey:%x,ismousedown:%d",g_message,nVirtKey,ismousedown);
			sret=1;	//返回正数说明该键被弹起
			g_GetKeyState=false;
			TSRuntime::KeyPressMap[nVirtKey]=0;
			//g_wParam=0;
		}
		//TSRuntime::add_log( "GetKeyState:nVirtKey:%x,sret:%x,g_message:%x",nVirtKey,sret,g_message);
		//TSRuntime::add_log( "MyGetKeyState:nVirtKey:%x",nVirtKey);
		return sret;
	}
	sret=USER32GetKeyState(nVirtKey);
	return sret;
}

bool  UnhookKbd_DeviceState=false;
DWORD Kbd_DeviceStateAddr=0;
DWORD Ori_Kbd_DeviceState=0;//原始DX地址;__stdcall
//偏移0x24
HRESULT  __stdcall  MyGetDeviceStateKbd(
										LPDIRECTINPUTDEVICE8 m_pKeyboard,  //当前设备指针
										DWORD cbData,            //缓冲区字节大小
										LPVOID lpvData           //缓冲区指针
										)
{
	HRESULT hret;

	//TSRuntime::add_log("MyGetDeviceStateKbd");
	hret=  GetDeviceStateKbd(m_pKeyboard,cbData,lpvData);
	if(Kbd_DeviceStateAddr==0)
	{
		DWORD dwback;
		Kbd_DeviceStateAddr=*(DWORD*)m_pKeyboard;
		Kbd_DeviceStateAddr+=0x24;
	}
	//EVENT_GETKEYSTATE_NAME	  
	DIDEVCAPS Caps;
	Caps.dwSize=sizeof(DIDEVCAPS);
	if(m_pKeyboard->GetCapabilities(&Caps)==S_OK)
	{

		if(g_kbdDevice&&cbData==0x100)
		{
			if(Caps.dwDevType==1043&&(g_RealKeyBoardDevice==m_pKeyboard))//dwDevType==274鼠标设备对象  ,dwDevType==1043键盘设备对象
			{
				if(g_message==WM_KEYDOWN||g_message==WM_SYSKEYDOWN)
				{
					for(int i=0;i<0x100;i++)
					{
						if(1==TSRuntime::KeyPressMap[i])
						{
							//i=TSRuntime::VirtualToASCIIMap[i];
							//SetKeyDown(i,(char*)lpvData);
							//int key=TSRuntime::VirtualToASCIIMap[i];	
							int key=MapVirtualKey(i,0);
							if(i==VK_LEFT)
								key=DIK_LEFT;
							else if(i==VK_RIGHT)
								key=DIK_RIGHT;
							else if(i==VK_DOWN)
								key=DIK_DOWN;
							else if(i==VK_UP)
								key=DIK_UP;
							SetKeyDown(key,(char*)lpvData);
							//TSRuntime::add_log("SetKeyDown:key:%x,i:%x",key,i);
							g_isSetKeyDown=true;//标记有按键按下
						}
					}

					//TSRuntime::add_log("VirtualToASCIIMap:g_wParam:%x",g_wParam);
					//g_wParam=TSRuntime::VirtualToASCIIMap[g_wParam];
					//g_wParam=::MapVirtualKey(g_wParam,0);
					//SetKeyDown(g_wParam,(char*)lpvData);
					//g_isSetKeyDown=true;//标记有按键按下
					//TSRuntime::add_log("SetKeyDown:g_message:%x,lpvData:%x,Key.vk:%x",g_message,lpvData,g_wParam);
				}
				else if(g_message==WM_KEYUP||g_message==WM_SYSKEYUP)
				{
					for(int i=0;i<0x100;i++)
					{
						if(1==TSRuntime::KeyPressMap[i]&&i==g_wParam)
						{
							//int key=TSRuntime::VirtualToASCIIMap[i];
							int key=MapVirtualKey(i,0);
							if(i==VK_LEFT)
								key=DIK_LEFT;
							else if(i==VK_RIGHT)
								key=DIK_RIGHT;
							else if(i==VK_DOWN)
								key=DIK_DOWN;
							else if(i==VK_UP)
								key=DIK_UP;
							SetKeyUp(key,(char*)lpvData);
							//TSRuntime::add_log("SetKeyUp:i:%x",key);
							TSRuntime::KeyPressMap[i]=0;
							//i=TSRuntime::VirtualToASCIIMap[i];
							//SetKeyUp(i,(char*)lpvData);
							//TSRuntime::add_log("SetKeyUp:key:%x,i:%x",key,i);

						}
					}
					//TSRuntime::add_log("SetKeyUp:g_message:%x,lpvData:%x,Key.vk:%x",g_message,lpvData,g_wParam);
					//g_wParam=TSRuntime::VirtualToASCIIMap[g_wParam];
					////g_wParam=::MapVirtualKey(g_wParam,0);
					//SetKeyUp(g_wParam,(char*)lpvData);
					g_isSetKeyDown=false;//重置按键标记
					g_kbdDevice=false;

				}
				//TSRuntime::add_log("SetKeyUp:g_message:%x,lpvData:%x,Key.vk:%x",g_message,lpvData,g_wParam);
				return S_OK;

			}
		}
		else if(g_mouseDevice&&cbData==0x14)
		{
			if(Caps.dwDevType==274) //鼠标事件
			{
				g_mousestate=(DIMOUSESTATE *)lpvData;

				if(g_message==WM_LBUTTONDOWN)
					g_mousestate->rgbButtons[0]=0x80;//左键单击
				else if(g_message==WM_LBUTTONUP)
					g_mousestate->rgbButtons[0]=0;//左键单击
				else if(g_message==WM_RBUTTONDOWN)
					g_mousestate->rgbButtons[1]=0x80;//右键单击
				else if(g_message==WM_RBUTTONUP)
					g_mousestate->rgbButtons[1]=0;//右键单击
				else if(g_message==WM_MBUTTONDOWN)
					g_mousestate->rgbButtons[2]=0x80;//中键单击
				else if(g_message==WM_MBUTTONUP)
					g_mousestate->rgbButtons[2]=0;//中键单击
				else if(g_message==WM_MOUSEWHEEL)
					g_mousestate->rgbButtons[3]=g_dwData;//上滚/下滚

				g_mouseDevice=false;
				return S_OK;
			}
		}
		else if(cbData==0x100)
		{
			if(Caps.dwDevType==1043)
			{
				if(g_RealKeyBoardDevice)
				{
					if(g_RealKeyBoardDevice!=m_pKeyboard)//要排除多余的键盘设备,真正的键盘设备指针比多余的键盘设备指针要小
					{
						if((DWORD)g_RealKeyBoardDevice>(DWORD)m_pKeyboard)
							g_RealKeyBoardDevice=m_pKeyboard;
						//TSRuntime::add_log("SetKeyUp:g_RealKeyBoardDevice:%x,lpvData:%x",g_RealKeyBoardDevice,lpvData);
					}
				}
				if(g_RealKeyBoardDevice==NULL)
					g_RealKeyBoardDevice=m_pKeyboard;

			}

		}
	}
	//TSRuntime::add_log("SetDXKey:cbData:%x,lpvData:%x",cbData,lpvData);
	if(g_isSetKeyDown)
		hret= S_OK;

	if(g_LockInput_kdb&&Caps.dwDevType==1043)   //dwDevType==274鼠标设备对象  ,dwDevType==1043键盘设备对象
	{
		ZeroMemory(lpvData,cbData);
		hret= S_FALSE;  //封锁外部键盘事件
	}
	if(g_LockInput_mouse&&Caps.dwDevType==274)
	{
		ZeroMemory(lpvData,cbData);
		hret= S_FALSE;  //封锁外部鼠标事件
	}

	// TSRuntime::add_log("SetKeyUp:cbData:%x,lpvData:%x",cbData,lpvData);
	return	hret;
}

bool  UnhookMouse_DeviceState=false;
DWORD Mouse_DeviceStateAddr=0;
DWORD Ori_Mouse_DeviceState=0;//原始DX地址;
//偏移0x28
HRESULT __stdcall MyGetDeviceStateMouse(
										LPDIRECTINPUTDEVICE8 m_pMouse,	  //当前设备指针
										DWORD cbObjectData,
										LPDIDEVICEOBJECTDATA rgdod,
										LPDWORD pdwInOut,
										DWORD dwFlags)
{
	HRESULT hret;

	hret= GetDeviceStateMouse( m_pMouse,cbObjectData,rgdod,pdwInOut,dwFlags);

	//if(UnhookMouse_DeviceState==false)
	//{
	//	TSRuntime::add_log("MyGetDeviceStateMouse:%x",(DWORD)MyGetDeviceStateMouse);
	//	UnhookMouse_DeviceState=true;
	//}

	if(Mouse_DeviceStateAddr==0)
	{

		Mouse_DeviceStateAddr=*(DWORD*)m_pMouse;
		Mouse_DeviceStateAddr+=0x28;
		//*(DWORD*)Mouse_DeviceStateAddr=(DWORD)MyGetDeviceStateMouse;
		//return hret;
	}

	if(rgdod)
		g_rgdod=rgdod;
	else
		rgdod=g_rgdod;

	DIDEVCAPS Caps;

	if(m_pMouse)
	{
		Caps.dwSize=sizeof(DIDEVCAPS);
		if(m_pMouse->GetCapabilities(&Caps)==S_OK)
		{

			if(Caps.dwDevType==274&&g_mouseDevice)	 //dwDevType==274鼠标设备对象  ,dwDevType==1043键盘设备对象		g_kbdDevice
			{
				rgdod->dwOfs=g_dwOfs;
				rgdod->dwData=g_dwData;
				rgdod->dwTimeStamp=GetTickCount();
				*pdwInOut=1;		   //当有键盘鼠标事件时候,*pdwInOut=1,记录了事件缓存
				g_mouseDevice=false;
				//TSRuntime::add_log( "cbObjectData:%x,rgdod->dwOfs:%x,rgdod->dwData:%x,pdwInOut:%x,dwFlags:%x",cbObjectData,rgdod->dwOfs,rgdod->dwData,*pdwInOut,dwFlags);
				return S_OK;

			}

			else if(Caps.dwDevType==1043&&g_kbdDevice_mouse)	 //dwDevType==274鼠标设备对象  ,dwDevType==1043键盘设备对象		
			{
				//g_wParam=TSRuntime::VirtualToASCIIMap[g_wParam];
				int key=0;
				for(int i=0;i<0x100;i++)
				{
					if(1==TSRuntime::KeyPressMap_Mouse[i])
					{
						key=TSRuntime::VirtualToASCIIMap[i];
						//key=MapVirtualKey(i,0);
						//rgdod->dwOfs=key;
						for(int cnt=0;cnt<0x100;cnt++)
						{
							if(TSRuntime::keyMap[cnt])
							{
								if(TSRuntime::keyMap[cnt]==key)
								{
									rgdod->dwOfs=cnt;
									//TSRuntime::add_log("SetKeyDown:key:%x,i:%x",cnt,cnt);
									break;
								}
							}
						}
						g_isSetKeyDown=true;//标记有按键按下
					}
				}

				rgdod->dwData=g_dwData;
				rgdod->dwTimeStamp=GetTickCount();
				*pdwInOut=1;		   //当有键盘鼠标事件时候,*pdwInOut=1,记录了事件缓存
				if(g_message==WM_KEYUP || g_message==WM_SYSKEYUP)
					TSRuntime::KeyPressMap_Mouse[g_wParam]=0;

				g_kbdDevice_mouse=false;
				//TSRuntime::add_log( "g_message:%x,g_wParam:%x,g_isSetKeyDown:%d",g_message,g_wParam,g_isSetKeyDown);
				//TSRuntime::add_log( "rgdod->uAppData:%x,rgdod->dwTimeStamp:%x,rgdod->dwSequence:%x,rgdod->dwOfs:%x,rgdod->dwData:%x,pdwInOut:%x,dwFlags:%x",rgdod->uAppData,rgdod->dwTimeStamp,rgdod->dwSequence,rgdod->dwOfs,rgdod->dwData,*pdwInOut,dwFlags);
				//TSRuntime::add_log( "m_pMouse:%x,hret:%x,rgdod->dwOfs:%x,rgdod->dwData:%x,pdwInOut:%x,dwFlags:%x",m_pMouse,S_OK,rgdod->dwOfs,rgdod->dwData,*pdwInOut,dwFlags);
				return S_OK;

			}
			if((Caps.dwDevType==274)&&(g_LockInput_mouse))   //锁定鼠标
			{
				rgdod->dwOfs=0;
				rgdod->dwData=0;
				*pdwInOut=0;
				//return DIERR_INPUTLOST;
			}
			if(Caps.dwDevType==1043&&g_LockInput_kdb)	//锁键盘
			{
				rgdod->dwOfs=0;
				rgdod->dwData=0;
				*pdwInOut=0;
				//return DIERR_INPUTLOST;
			}

		}

	}  //hret
	//TSRuntime::add_log( "hret:%x",hret);
	//if(*pdwInOut!=0&&Caps.dwDevType==1043)
	//TSRuntime::add_log( "rgdod->dwTimeStamp:%x,rgdod->dwSequence:%x,rgdod->dwOfs:%x,rgdod->dwData:%x,pdwInOut:%x,dwFlags:%x",rgdod->dwTimeStamp,rgdod->dwSequence,rgdod->dwOfs,rgdod->dwData,*pdwInOut,dwFlags);
	//hret= S_OK;
	return	 hret;
}

BOOL __stdcall MyScreenToClient(HWND hWnd,LPPOINT lpPoint)
{
	//bool ret=USER32ScreenToClient(hWnd,lpPoint);
	lpPoint->x=g_x;
	lpPoint->y=g_y;

	//TSRuntime::add_log( "MyScreenToClient:x:%d, y: %d ",lpPoint->x,lpPoint->y);
	return true;
}

BOOL __stdcall MyGetCursorPos( LPPOINT lpPoint)		//g_IsGetCursorPoschook
{

	bool ret=USER32GetCursorPos(lpPoint);
	g_lpPoint=lpPoint;
	lpPoint->x=g_x;
	lpPoint->y=g_y;
	::ClientToScreen(g_currentHwnd,lpPoint);

	return ret;
}

BOOL __stdcall MySetCursorPos( int x,  int y)  //防止外部鼠标漂移
{

	//USER32SetCursorPos(lpPoint);
	return true;

}

//{0, WM_LBUTTONDOWN, irr::EMIE_LMOUSE_PRESSED_DOWN},
//{1, WM_LBUTTONUP,   irr::EMIE_LMOUSE_LEFT_UP},
//{0, WM_RBUTTONDOWN, irr::EMIE_RMOUSE_PRESSED_DOWN},
//{1, WM_RBUTTONUP,   irr::EMIE_RMOUSE_LEFT_UP},
//{0, WM_MBUTTONDOWN, irr::EMIE_MMOUSE_PRESSED_DOWN},
//{1, WM_MBUTTONUP,   irr::EMIE_MMOUSE_LEFT_UP},
//{2, WM_MOUSEMOVE,   irr::EMIE_MOUSE_MOVED},
//{3, WM_MOUSEWHEEL,  irr::EMIE_MOUSE_WHEEL},
//{-1, 0, 0}

typedef LRESULT (__stdcall  *  My_InternalCallWinProc)(DWORD fAnsi,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); 
My_InternalCallWinProc f_My_InternalCallWinProc;

typedef LRESULT(__stdcall  * InjectWinProc)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
InjectWinProc f_InjectWinProc;

void CALLBACK MyTimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{
	return;
}

DWORD UnHookKbdMouse_time=0;
void UnHookKbdMouse()//还原DX钩子,防检测
{
	//还原DX钩子,防检测
	//修改成我们的地址后,我们就可以恢复DX图色钩子了,这样可以达到防止DX图色被检测
	if(SySTpye==1)//XP
		GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJkeyboardXPoffse,(DWORD)MyGetDeviceStateKbd,(DWORD)GetDeviceStateKbd,HookGetDeviceStateKbdRet,false);  //xp
	else if(SySTpye==2)//win2003
		GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJkeyboardXPoffse,(DWORD)MyGetDeviceStateKbd,(DWORD)GetDeviceStateKbd,HookGetDeviceStateKbdRet,false); //win2003
	else if(SySTpye==4)//win7
		GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJkeyboardWin7offse,(DWORD)MyGetDeviceStateKbd,(DWORD)GetDeviceStateKbd,HookGetDeviceStateKbdRet,false);//win7
	else if(SySTpye==5)//win8
		GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJkeyboardWin8offse,(DWORD)MyGetDeviceStateKbd,(DWORD)GetDeviceStateKbd,HookGetDeviceStateKbdRet,false);  //win8

	//还原DX钩子,防检测
	if(SySTpye==1)//XP
		GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJMouseXPoffse,(DWORD)MyGetDeviceStateMouse,(DWORD)GetDeviceStateMouse,HookGetDeviceStateMouseRet,false);//HOOK:XP系统下的:dinput8.dll的GetDeviceState函数  //xp
	else if(SySTpye==2)//win2003
		GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJMouseXPoffse,(DWORD)MyGetDeviceStateMouse,(DWORD)GetDeviceStateMouse,HookGetDeviceStateMouseRet,false); //win2003
	else if(SySTpye==4)//win7
		GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJMouseWin7offse,(DWORD)MyGetDeviceStateMouse,(DWORD)GetDeviceStateMouse,HookGetDeviceStateMouseRet,false);//win7
	else if(SySTpye==5)//win8
		GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJMouseWin8offse,(DWORD)MyGetDeviceStateMouse,(DWORD)GetDeviceStateMouse,HookGetDeviceStateMouseRet,false);  //win8
}

void HookKbdMouse()//隐藏DX键鼠钩子,防DX键鼠钩子检测盾
{
	DWORD dwback=0,dwwrite=0,time1=0;
	//TSRuntime::add_log("HookKbdMouse,Kbd_DeviceStateAddr:%x,Mouse_DeviceStateAddr:%x",Kbd_DeviceStateAddr,Mouse_DeviceStateAddr);
	if(Kbd_DeviceStateAddr!=0||Mouse_DeviceStateAddr!=0)
	{
		if(Kbd_DeviceStateAddr)
		{
			if(!UnhookKbd_DeviceState)
			{
				::VirtualProtect((void*)Kbd_DeviceStateAddr,5,PAGE_EXECUTE_READWRITE, &dwback);
				Ori_Kbd_DeviceState=*(DWORD*)Kbd_DeviceStateAddr;
				*(DWORD*)Kbd_DeviceStateAddr=(DWORD)MyGetDeviceStateKbd;
				::VirtualProtect((void*)Kbd_DeviceStateAddr,5,dwback, &dwback);
				UnhookKbd_DeviceState=true;
				UnHookKbdMouse();
			}
		}
		if(Mouse_DeviceStateAddr)
		{
			if(!UnhookMouse_DeviceState)
			{
				::VirtualProtect((void*)Mouse_DeviceStateAddr,5,PAGE_EXECUTE_READWRITE, &dwback);
				Ori_Mouse_DeviceState=*(DWORD*)Mouse_DeviceStateAddr;
				*(DWORD*)Mouse_DeviceStateAddr=(DWORD)MyGetDeviceStateMouse;
				::VirtualProtect((void*)Mouse_DeviceStateAddr,5,dwback, &dwback);
				UnhookMouse_DeviceState=true;
				UnHookKbdMouse();
			}
			//TSRuntime::add_log("HookKbdMouse,Mouse_DeviceStateAddr:%x",Mouse_DeviceStateAddr);
		}

	}
}
void InitRawInputDevices()
{
	if(GetRegisteredRawInputDevicesAddr==0)
	{
		GetRegisteredRawInputDevicesAddr=(DWORD)::GetProcAddress(::GetModuleHandle(L"user32.dll"),"GetRegisteredRawInputDevices");
		My_GetRegisteredRawInputDevices=(MyGetRegisteredRawInputDevices)GetRegisteredRawInputDevicesAddr;
	}
	if(g_hwndRawInput==NULL&&isnotRawInput==false)
	{
		UINT numberDevices=10;
		RAWINPUTDEVICE Rid[10];
		memset(	Rid,0,sizeof(Rid));
		UINT uret=My_GetRegisteredRawInputDevices(Rid, &numberDevices, sizeof (RAWINPUTDEVICE));
		//ERROR_INSUFFICIENT_BUFFER
		g_hwndRawInput=Rid[0].hwndTarget;
		isnotRawInput=true;
	}
}

MyFindPictureClass pic;
LRESULT __stdcall MyInternalCallWinProc(DWORD fAnsi,HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT lret=0;
	switch(message)		//WM_ACTIVATEAPP
	{
	case WM_CAPTURECHANGED:
		{					  

			if(g_currentHwnd==(HWND)lParam)
			{
				lParam=0;
				return 0;
			}

			break;
		}
	case WM_WINDOWPOSCHANGING:
		{   
			//if(GameMsg==NULL)
			//	GameMsg=(LPMSG)lParam;//获取游戏内部PeekMessage中的MSG指针

			//lret=USER32InternalCallWinProc(fAnsi,hWnd,message,wParam,lParam);
			//TSRuntime::add_log("WM_WINDOWPOSCHANGING:lParam:%x,lret:%x",lParam,lret);
			//return lret;

			//wp = *(LPWINDOWPOS)lParam;
			//WINDOWPOS* wndPos =&wp;
			//if(wndPos->hwnd==g_currentHwnd)
			//{
			//	wndPos->flags=0x3;
			//	return 0;   
			//}
			//TSRuntime::add_log("WM_WINDOWPOSCHANGING:hwnd:%x,hwndInsertAfter:%x,flags:%x,myhwnd:%x",wndPos->hwnd,wndPos->hwndInsertAfter,wndPos->flags,g_currentHwnd);
			break;
		}
	case WM_WINDOWPOSCHANGED:
		{
			//if(GameMsg==NULL)
			//	GameMsg=(LPMSG)lParam;//获取游戏内部PeekMessage中的MSG指针
			//
			//lret=USER32InternalCallWinProc(fAnsi,hWnd,message,wParam,lParam);
			//TSRuntime::add_log("WM_WINDOWPOSCHANGING:lParam:%x,lret:%x",lParam,lret);
			//return lret;
			//WINDOWPOS wp = *(LPWINDOWPOS)lParam;
			//WINDOWPOS* wndPos =&wp;
			//if(wndPos->hwnd==g_currentHwnd)
			//{
			//	wndPos->flags=0x1803;
			//	return 0;
			//}
			//TSRuntime::add_log("WM_WINDOWPOSCHANGED:hwnd:%x,hwndInsertAfter:%x,flags:%x,myhwnd:%x",wndPos->hwnd,wndPos->hwndInsertAfter,wndPos->flags,g_currentHwnd);
			break;
		}
	case WM_ACTIVATEAPP:   //伪激活
		{
			if(wParam==0)
			{
				wParam=1;
				return 0;
			}

			//WINDOWPOS wp;
			break;
		}
	case WM_ACTIVATE:
		{
			if(LOWORD(wParam)==WA_INACTIVE)//失去焦点时候
			{
				wParam=MAKEWPARAM(WA_ACTIVE,0);
				return 0;
			}
			break;
		}
	case WM_KILLFOCUS:	  //SWP_NOMOVE | SWP_NOSIZE
		{
			if(g_LockInput_mouse==true)
			{
				wParam=(WPARAM)g_currentHwnd;
			}
			break;
		}
	case WM_NCHITTEST://dx.mouse.position.lock.message
		{
			if(g_LockInput_mouse==true)
			{
				DWORD ret=DefWindowProcA(hWnd,message,wParam,lParam);
				if(ret==HTCAPTION)
				{
					//TSRuntime::add_log("WM_NCHITTESTret:%x",ret);
					return	HTCAPTION;
				}
				return HTNOWHERE;
			}
			else
			{
				if(wParam==0x8000)
					wParam=0;
			}

			break;
		}

	case WM_NCACTIVATE:
		{
			//TSRuntime::add_log("hWnd:%x",hWnd);
			if(hWnd==g_currentHwnd&&wParam==0&&g_LockInput_mouse==true)
			{
				wParam=1;
				return 0;
			}
			else if(wParam==0)
			{
				wParam=1;
				return 1;
			}
			break;
		}
	case TS_BIND:
		{
			g_currentHwnd=(HWND)wParam;//TSRuntime::GetProcessMainWnd((DWORD)lParam);//获取注入方进程窗口句柄
			IsNpProtect=(BOOLEAN)lParam;//接收是否开启NP特殊处理
			if(TSRuntime::pData->NowUsingDictIndex!=-1)
				SendMessage(g_currentHwnd,TS_SETDICT,0,0);

			if(TSRuntime::pData->InjectType==103)
				TSRuntime::pData->InjectType=101;

			if(wcslen(TSRuntime::pData->SetPicPwdString)>0)
				wcscpy(TSRuntime::SetPicPwdString,TSRuntime::pData->SetPicPwdString);
			return TRUE;
			break;
		}
	case TS_UNBIND:
		{
			//TSRuntime::add_log( "%x,%x,%x,%x",::GetActiveWindow(),::GetFocus(),::GetTopWindow(g_currentHwnd),GetForegroundWindow());
			//TSRuntime::add_log("ScreenToClient-,lpPoint:x:%d,lpPoint:y:%d",po.x,po.y);
			GameMsg=NULL;
			if(Ori_DxdeviceAddr!=0)
			{
				//TSRuntime::add_log("TS_UNBIND-start,DxdeviceAddr:%x,Ori_DxdeviceAddr:%x,MyD3D9Present:%x",*(DWORD*)DxdeviceAddr,Ori_DxdeviceAddr,(DWORD)MyD3D9Present);
				if(*(DWORD*)DxdeviceAddr==(DWORD)MyD3D9Present)
				{
					DWORD dwback=0;
					::VirtualProtect((void*)DxdeviceAddr,5,PAGE_EXECUTE_READWRITE, &dwback);
					*(DWORD*)DxdeviceAddr=Ori_DxdeviceAddr;//把我们的函数地址修改到真正的函数地址
					::VirtualProtect((void*)DxdeviceAddr,5,dwback, &dwback);
				}
				UnhookPresent=false;
				//TSRuntime::add_log( "TS_ISGRAPHICPROTECT:UnhookPresent:%x,ORI:%x",Ori_DxdeviceAddr,*(DWORD*)DxdeviceAddr);
			}
			if(Ori_Kbd_DeviceState!=0)
			{
				if(*(DWORD*)Kbd_DeviceStateAddr==(DWORD)MyGetDeviceStateKbd)
				{
					DWORD dwback=0;
					//TSRuntime::add_log("Ori_Kbd_DeviceState:%x",Ori_Kbd_DeviceState);
					::VirtualProtect((void*)Kbd_DeviceStateAddr,5,PAGE_EXECUTE_READWRITE, &dwback);
					*(DWORD*)Kbd_DeviceStateAddr=Ori_Kbd_DeviceState;//把我们的函数地址修改到真正的函数地址
					::VirtualProtect((void*)Kbd_DeviceStateAddr,5,dwback, &dwback);
				}
				UnhookKbd_DeviceState=false;
			}
			if(Ori_Mouse_DeviceState!=0)
			{
				if(*(DWORD*)Mouse_DeviceStateAddr==(DWORD)MyGetDeviceStateMouse)
				{
					DWORD dwback=0;
					//TSRuntime::add_log("Ori_Mouse_DeviceState:%x",Ori_Kbd_DeviceState);
					::VirtualProtect((void*)Mouse_DeviceStateAddr,5,PAGE_EXECUTE_READWRITE, &dwback);
					*(DWORD*)Mouse_DeviceStateAddr=Ori_Mouse_DeviceState;//把我们的函数地址修改到真正的函数地址
					::VirtualProtect((void*)Mouse_DeviceStateAddr,5,dwback, &dwback);
				}
				UnhookMouse_DeviceState=false;
			}

			if(TSRuntime::pData->Isdxkmprotect)
				VIPHookandUnhookAPI((void*)false);
			else
				HookandUnhookAPI((void*)false);

			if(TSRuntime::MyDictCount[TSRuntime::NowUsingDictIndex]!=NULL)
				delete [] TSRuntime::MyDictCount[TSRuntime::NowUsingDictIndex];
			g_Unbind=true;
			TSRuntime::pData->NowUsingDictIndex=-1;

			if(TSRuntime::pData->InjectType==101)
				if(IsWindow(g_InjectHWND)==true)
					TSRuntime::pData->InjectType=102;

			//if(TSRuntime::pData->InjectType==205)//203绑定
			//{
			//	if(TSRuntime::pData->RemodeDLLMode!=NULL)//释放上一次加载的DLL模块,因为不能自己释放自己,否则会引起异常
			//		MemoryFreeLibrary(TSRuntime::pData->RemodeDLLMode);
			//	TSRuntime::pData->RemodeDLLMode=g_hInstance;
			//}
			//TSRuntime::add_log("TS_UNBIND--GetForegroundWindow:%d,GetFocus:%d,GetActiveWindow:%d",GetForegroundWindow(),GetFocus(),GetActiveWindow());
			//TSRuntime::add_log("TS_UNBIND-end");
			//解绑操作
			return TRUE;
			break;
		}
	case TS_KEYDOWN:
		{
			if(wParam==VK_MENU)////WM_SYSKEYDOWN
			{
				TSRuntime::g_IsSYSKEY=true;
				TSRuntime::KeyPressMap[0xA4]=1;//左Alt键
				TSRuntime::KeyPressMap_Mouse[0xA4]=1;//左Alt键
			}
			if(TSRuntime::g_IsSYSKEY&&lParam==0)
				message=WM_SYSKEYDOWN;
			else
				message=WM_KEYDOWN;

			if(wParam==VK_CONTROL)
			{
				TSRuntime::KeyPressMap[0xA2]=1;//左Ctrl键
				TSRuntime::KeyPressMap_Mouse[0xA2]=1;
			}
			else if(wParam==VK_SHIFT)
			{
				TSRuntime::KeyPressMap[0xA0]=1;//左Shift键
				TSRuntime::KeyPressMap_Mouse[0xA0]=1;//左Shift键
			}

			g_dwData=0x80;
			g_keymouseevent=true;
			g_message=message;
			g_wParam=wParam;
			TSRuntime::KeyPressMap[wParam]=1;
			TSRuntime::KeyPressMap_Mouse[wParam]=1;
			TSRuntime::RawInputKeyPressMap[wParam]=1;
			g_kbdDevice=true;
			g_keyboardstate=true;
			g_kbdDevice_mouse=true;
			g_GetKeyState=true;
			g_GetRawInputDatahook=true;
			//if(message==WM_KEYDOWN)
			lParam=TSRuntime::retrunLparamDown(wParam);
			if(message==WM_SYSKEYDOWN&&wParam==VK_MENU)
				lParam=0x4000000|lParam;
			else if(message==WM_SYSKEYDOWN)
				lParam=0x4000000|0x20000000|TSRuntime::retrunLparamDown(wParam);
			hWnd=g_currentHwnd;
			if(IsNpProtect)
				HookMessageAPI();

			//"dx.keypad.state.api"//EM_SETMARGINS,(WPARAM)GetCurrentProcessId(),1
			::PostThreadMessageA(0x80000000,EM_GETMARGINS,(WPARAM)GetCurrentProcessId(),(LPARAM)(0x8000|::MapVirtualKeyEx(g_wParam,0,GetKeyboardLayout(0))));
			//if(wParam!=13)	  //等于wParam=13回车键的时候放行
			//	wParam=0x8000|wParam;
			//TSRuntime::add_log("TS_KEYDOWN:message:%x,wParam:%x",message,wParam);//TSRuntime::VirtualToASCIIMap[wParam]
			if(TSRuntime::g_IsSYSKEY&&wParam!=VK_MENU)
			{	
				if(HookPostMessageRet!=0)
					MyPostMessage(g_currentHwnd,message,wParam,lParam);
				else
					::PostMessage(g_currentHwnd,message,wParam,lParam);
			}
			else//
			{
				if(HookPostMessageRet!=0)
					MyPostMessage(g_currentHwnd,message,wParam,lParam);
				else
					::PostMessage(g_currentHwnd,message,wParam,lParam);
			}

			InitRawInputDevices();
			if(g_hwndRawInput!=NULL)
				PostMessageA(g_hwndRawInput,WM_INPUT,0,0x12345678);

			if(GameMsg)
			{
				GameMsg->hwnd=g_currentHwnd;
				GameMsg->message=message;
				GameMsg->wParam=wParam;
				GameMsg->lParam=lParam;
			}
			message=TS_KEYDOWN;
			//TSRuntime::add_log("message:%x,wParam:%x,lParam:%x",message,wParam,lParam);
			//TSRuntime::add_log("hwndTarget:%d,uret:%d,GetLastError:%d",hwndTarget,uret,GetLastError());
			//MessageBoxA(NULL,"aa","ts",NULL);
			return 1;

			break;
		}
	case TS_KEYUP:
		{
			if(TSRuntime::g_IsSYSKEY&&lParam==0)
				message=WM_SYSKEYUP;
			else
				message=WM_KEYUP;	

			g_dwData=0;
			g_message=message;
			g_wParam=wParam;
			g_GetKeyState=true;
			g_keyboardstate=true;
			g_keymouseevent=true;
			g_kbdDevice_mouse=true;
			g_kbdDevice=true;
			//TSRuntime::KeyPressMap[wParam]=0;
			if(wParam==VK_MENU)
				lParam=0XC0380001;
			else if(message==WM_KEYUP)
				lParam=0xC0000000|TSRuntime::retrunLparamDown(wParam);
			else if(message==WM_SYSKEYUP)
				lParam=0xE0000000|TSRuntime::retrunLparamDown(wParam);
			hWnd=g_currentHwnd;

			//"dx.keypad.state.api"
			::PostThreadMessageA(0x80000000,EM_GETMARGINS,(WPARAM)GetCurrentProcessId(),(LPARAM)::MapVirtualKeyEx(g_wParam,0,GetKeyboardLayout(0)));
			//if(wParam!=13)	  //等于wParam=13回车键的时候放行
			//	wParam=0x8000|wParam;
			if(IsNpProtect)
				HookMessageAPI();
			if(HookPostMessageRet!=0)
				MyPostMessage(g_currentHwnd,message,wParam,lParam);
			else
				::PostMessage(g_currentHwnd,message,wParam,lParam);

			if(wParam==VK_MENU)////WM_SYSKEYDOWN
			{
				TSRuntime::KeyPressMap[0xA4]=0;//左Alt键
				TSRuntime::KeyPressMap_Mouse[0xA4]=0;//左Alt键
			}
			else if(wParam==VK_CONTROL)
			{
				TSRuntime::KeyPressMap[0xA2]=0;//左Ctrl键
				TSRuntime::KeyPressMap_Mouse[0xA2]=0;//左Alt键
			}
			else if(wParam==VK_SHIFT)
			{
				TSRuntime::KeyPressMap[0xA0]=0;//左Shift键
				TSRuntime::KeyPressMap_Mouse[0xA0]=0;//左Alt键
			}

			TSRuntime::g_IsSYSKEY=false;
			//TSRuntime::add_log("TS_KEYUP:message:%x,wParam:%x",message,wParam);

			if(GetRegisteredRawInputDevicesAddr==0)
			{
				GetRegisteredRawInputDevicesAddr=(DWORD)::GetProcAddress(::GetModuleHandle(L"user32.dll"),"GetRegisteredRawInputDevices");
				My_GetRegisteredRawInputDevices=(MyGetRegisteredRawInputDevices)GetRegisteredRawInputDevicesAddr;
			}

			InitRawInputDevices();
			if(g_hwndRawInput!=NULL)
				PostMessageA(g_hwndRawInput,WM_INPUT,0,0x12345678);

			if(GameMsg)
			{
				GameMsg->hwnd=g_currentHwnd;
				GameMsg->message=message;
				GameMsg->wParam=wParam;
				GameMsg->lParam=lParam;
			}
			message=TS_KEYUP;
			return 1;
			break;
		}
	case TS_CHAR:
		{
			//TSRuntime::pData->tt="abcddddee";
			message=WM_CHAR;
			lParam=TSRuntime::retrunLparamDown(wParam);
			hWnd=g_currentHwnd;
			g_wParam=wParam;
			//wParam=wParam|0x8000;
			if(IsNpProtect)
				HookMessageAPI();
			if(HookPostMessageRet!=0)
				MyPostMessage(g_currentHwnd,message,wParam,lParam);
			else
				::PostMessage(g_currentHwnd,message,wParam,lParam);
			message=TS_CHAR;
			return true;
			break;
		}
	case TS_MOUSEMOVE:
		{
			g_dwOfs=4;
			g_dwData=0xfffffffe;
			g_mouseDevice=true;
			g_keymouseevent=true;
			g_x=(short)LOWORD(lParam);
			g_y=(short)HIWORD(lParam);
			message=WM_MOUSEMOVE;
			g_message=message;
			g_GetRawInputDatahook=true;

			POINT Point;
			Point.x=g_x;
			Point.y=g_y;
			::ClientToScreen(hWnd,&Point);
			LPARAM lparam=MAKELPARAM(Point.x,Point.y);
			if(IsNpProtect)
				HookMessageAPI();
			if(HookSendMessageRet!=0)
			{
				MySendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
				MySendMessage(g_currentHwnd,WM_SETCURSOR,(WPARAM)g_currentHwnd,MAKELPARAM(HTCLIENT,WM_MOUSEMOVE));
			}
			else
			{
				SendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
				//TSRuntime::add_log("TS_MOUSEMOVE");
				SendMessage(g_currentHwnd,WM_SETCURSOR,(WPARAM)g_currentHwnd,MAKELPARAM(HTCLIENT,WM_MOUSEMOVE));
			}
			//::PostMessage(g_currentHwnd,message,0x8000,lParam);
			if(HookPostMessageRet!=0)
				MyPostMessage(g_currentHwnd,message,0,lParam);
			else
				::PostMessage(g_currentHwnd,message,0,lParam);


			InitRawInputDevices();
			if(g_hwndRawInput!=NULL)
				PostMessageA(g_hwndRawInput,WM_INPUT,0,0x12345678);

			if(GameMsg)
			{
				GameMsg->hwnd=g_currentHwnd;
				GameMsg->message=message;
				GameMsg->wParam=wParam;
				GameMsg->lParam=lParam;
				GameMsg->pt=Point;
			}
			//TSRuntime::add_log("GameMsg:%x",GameMsg);

			message=TS_MOUSEMOVE;
			return 1;
			//hWnd=g_currentHwnd;
			break;
		}
	case TS_MOUSER:
		{
			g_dwOfs=4;
			g_dwData=0xfffffffe;
			g_mouseDevice=true;
			g_keymouseevent=true;
			g_x=g_x+(short)LOWORD(lParam);
			g_y=g_y+(short)HIWORD(lParam);
			lParam=MAKELPARAM(g_x,g_y);
			message=WM_MOUSEMOVE;
			g_message=message;
			g_GetRawInputDatahook=true;
			hWnd=g_currentHwnd;
			POINT Point;
			Point.x=g_x;
			Point.y=g_y;
			::ClientToScreen(hWnd,&Point);
			LPARAM lparam=MAKELPARAM(Point.x,Point.y);
			if(IsNpProtect)
				HookMessageAPI();
			if(HookSendMessageRet!=0)
			{
				MySendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
				MySendMessage(g_currentHwnd,WM_SETCURSOR,(WPARAM)g_currentHwnd,MAKELPARAM(HTCLIENT,WM_MOUSEMOVE));
			}
			else
			{
				SendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
				//TSRuntime::add_log("TS_MOUSEMOVE");
				SendMessage(g_currentHwnd,WM_SETCURSOR,(WPARAM)g_currentHwnd,MAKELPARAM(HTCLIENT,WM_MOUSEMOVE));
			}
			//::PostMessage(g_currentHwnd,message,0x8000,lParam);
			if(HookPostMessageRet!=0)
				MyPostMessage(g_currentHwnd,message,0,lParam);
			else
				::PostMessage(g_currentHwnd,message,0,lParam);

			InitRawInputDevices();
			if(g_hwndRawInput!=NULL)
				PostMessageA(g_hwndRawInput,WM_INPUT,0,0x12345678);

			if(GameMsg)
			{
				GameMsg->hwnd=g_currentHwnd;
				GameMsg->message=message;
				GameMsg->wParam=wParam;
				GameMsg->lParam=lParam;
				GameMsg->pt=Point;
			}

			message=TS_MOUSEMOVE;
			return 1;
			break;
		}
	case TS_MOUSEMOVEOVER:
		{
			g_dwOfs=0;
			g_dwData=0xfffffffe;
			g_message=0;
			g_mouseDevice=true;
			return 1;
			break;
		}
	case TS_LBUTTONDOWN:
		{
			POINT Point;
			Point.x=g_x;
			Point.y=g_y;
			::ClientToScreen(hWnd,&Point);
			LPARAM lparam=MAKELPARAM(Point.x,Point.y);
			if(IsNpProtect)
				HookMessageAPI();
			g_dwOfs=0xc;
			g_dwData=0x80;
			g_mouseDevice=true;
			g_keymouseevent=true;
			g_GetRawInputDatahook=true;
			g_message=WM_LBUTTONDOWN;
			message=WM_LBUTTONDOWN;
			lParam=MAKELPARAM(g_x,g_y);
			hWnd=g_currentHwnd;
			if(wParam==(WPARAM)WM_LBUTTONDBLCLK)
			{
				message=WM_LBUTTONDBLCLK;
				g_message=WM_LBUTTONDBLCLK;
				g_dwOfs=0xf;
				wParam=1;
			}

			//"dx.keypad.state.api"//,(WPARAM)GetCurrentProcessId(),
			::PostThreadMessageA(0x80000000,EM_SETMARGINS,(WPARAM)GetCurrentProcessId(),1);

			if(HookSendMessageRet!=0)
				MySendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			else
				SendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			if(HookPostMessageRet!=0)
				MyPostMessage(g_currentHwnd,message,1,lParam);
			else
				::PostMessage(g_currentHwnd,message,1,lParam);
			//::PostMessage(g_currentHwnd,message,1,lParam);
			InitRawInputDevices();
			if(g_hwndRawInput!=NULL)
				PostMessageA(g_hwndRawInput,WM_INPUT,0,0x12345678);

			if(GameMsg)
			{
				GameMsg->hwnd=g_currentHwnd;
				GameMsg->message=message;
				GameMsg->wParam=wParam;
				GameMsg->lParam=lParam;
				GameMsg->pt=Point;
			}

			message=TS_LBUTTONDOWN;
			return true;
			break;
		}
	case TS_LBUTTOUP:
		{
			POINT Point;
			Point.x=g_x;
			Point.y=g_y;
			::ClientToScreen(hWnd,&Point);
			LPARAM lparam=MAKELPARAM(Point.x,Point.y);
			if(IsNpProtect)
				HookMessageAPI();
			//SendMessage(g_currentHwnd,WM_NCHITTEST,0x8000,lparam);
			g_dwOfs=0xc;
			g_dwData=0;
			g_mouseDevice=true;
			g_keymouseevent=true;
			g_GetRawInputDatahook=true;
			message=WM_LBUTTONUP;
			g_message=message;
			lParam=MAKELPARAM(g_x,g_y);
			hWnd=g_currentHwnd;

			::PostThreadMessageA(0x80000000,EM_SETMARGINS,(WPARAM)GetCurrentProcessId(),0);

			if(HookSendMessageRet!=0)
				MySendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			else
				SendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			if(HookPostMessageRet!=0)
				MyPostMessage(g_currentHwnd,message,0,lParam);
			else
				::PostMessage(g_currentHwnd,message,0,lParam);
			//::PostMessage(g_currentHwnd,message,0,lParam);
			InitRawInputDevices();
			if(g_hwndRawInput!=NULL)
				PostMessageA(g_hwndRawInput,WM_INPUT,0,0x12345678);

			if(GameMsg)
			{
				GameMsg->hwnd=g_currentHwnd;
				GameMsg->message=message;
				GameMsg->wParam=wParam;
				GameMsg->lParam=lParam;
				GameMsg->pt=Point;
			}

			message=TS_LBUTTOUP;
			return true;
			break;
		}
	case TS_RBUTTONDOWN:
		{
			POINT Point;
			Point.x=g_x;
			Point.y=g_y;
			::ClientToScreen(hWnd,&Point);
			LPARAM lparam=MAKELPARAM(Point.x,Point.y);
			if(IsNpProtect)
				HookMessageAPI();
			//SendMessage(g_currentHwnd,WM_NCHITTEST,0x8000,lparam);
			g_dwOfs=0xd;
			g_dwData=0x80;
			g_mouseDevice=true;
			g_keymouseevent=true;
			g_GetRawInputDatahook=true;
			message=WM_RBUTTONDOWN;
			g_message=message;
			lParam=MAKELPARAM(g_x,g_y);
			hWnd=g_currentHwnd;

			::PostThreadMessageA(0x80000000,EM_SETMARGINS,(WPARAM)GetCurrentProcessId(),1);

			if(HookSendMessageRet!=0)
				MySendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			else
				SendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			if(HookPostMessageRet!=0)
				MyPostMessage(g_currentHwnd,message,1,lParam);
			else
				::PostMessage(g_currentHwnd,message,1,lParam);
			//::PostMessage(g_currentHwnd,message,1,lParam);
			InitRawInputDevices();
			if(g_hwndRawInput!=NULL)
				PostMessageA(g_hwndRawInput,WM_INPUT,0,0x12345678);

			if(GameMsg)
			{
				GameMsg->hwnd=g_currentHwnd;
				GameMsg->message=message;
				GameMsg->wParam=wParam;
				GameMsg->lParam=lParam;
				GameMsg->pt=Point;
			}

			message=TS_RBUTTONDOWN;
			return true;
			break;
		}
	case TS_RBUTTONUP:
		{
			POINT Point;
			Point.x=g_x;
			Point.y=g_y;
			::ClientToScreen(hWnd,&Point);
			LPARAM lparam=MAKELPARAM(Point.x,Point.y);
			if(IsNpProtect)
				HookMessageAPI();
			//SendMessage(g_currentHwnd,WM_NCHITTEST,0x8000,lparam);
			g_dwOfs=0xd;
			g_dwData=0;
			g_mouseDevice=true;
			g_keymouseevent=true;
			g_GetRawInputDatahook=true;
			message=WM_RBUTTONUP;
			g_message=message;
			lParam=MAKELPARAM(g_x,g_y);
			hWnd=g_currentHwnd;

			::PostThreadMessageA(0x80000000,EM_SETMARGINS,(WPARAM)GetCurrentProcessId(),0);

			if(HookSendMessageRet!=0)
				MySendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			else
				SendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			if(HookPostMessageRet!=0)
				MyPostMessage(g_currentHwnd,message,0,lParam);
			else
				::PostMessage(g_currentHwnd,message,0,lParam);
			//::PostMessage(g_currentHwnd,message,0,lParam);
			InitRawInputDevices();
			if(g_hwndRawInput!=NULL)
				PostMessageA(g_hwndRawInput,WM_INPUT,0,0x12345678);

			if(GameMsg)
			{
				GameMsg->hwnd=g_currentHwnd;
				GameMsg->message=message;
				GameMsg->wParam=wParam;
				GameMsg->lParam=lParam;
				GameMsg->pt=Point;
			}

			message=TS_RBUTTONUP;
			return true;
			break;
		}
	case TS_MBUTTONDOWN:
		{
			POINT Point;
			Point.x=g_x;
			Point.y=g_y;
			::ClientToScreen(hWnd,&Point);
			LPARAM lparam=MAKELPARAM(Point.x,Point.y);
			if(IsNpProtect)
				HookMessageAPI();
			//SendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			g_dwOfs=0xe;
			g_dwData=0x80;
			g_mouseDevice=true;
			g_keymouseevent=true;
			g_GetRawInputDatahook=true;
			message=WM_MBUTTONDOWN;
			g_message=message;
			lParam=MAKELPARAM(g_x,g_y);
			hWnd=g_currentHwnd;

			::PostThreadMessageA(0x80000000,EM_SETMARGINS,(WPARAM)GetCurrentProcessId(),1);

			if(HookSendMessageRet!=0)
				MySendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			else
				SendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			if(HookPostMessageRet!=0)
				MyPostMessage(g_currentHwnd,message,1,lParam);
			else
				::PostMessage(g_currentHwnd,message,1,lParam);
			//::PostMessage(g_currentHwnd,message,1,lParam);
			InitRawInputDevices();
			if(g_hwndRawInput!=NULL)
				PostMessageA(g_hwndRawInput,WM_INPUT,0,0x12345678);

			if(GameMsg)
			{
				GameMsg->hwnd=g_currentHwnd;
				GameMsg->message=message;
				GameMsg->wParam=wParam;
				GameMsg->lParam=lParam;
				GameMsg->pt=Point;
			}

			message=TS_MBUTTONDOWN;
			return true;
			break;
		}
	case TS_MBUTTONUP:
		{
			POINT Point;
			Point.x=g_x;
			Point.y=g_y;
			::ClientToScreen(hWnd,&Point);
			LPARAM lparam=MAKELPARAM(Point.x,Point.y);
			if(IsNpProtect)
				HookMessageAPI();
			//SendMessage(g_currentHwnd,WM_NCHITTEST,0x8000,lparam);
			g_dwOfs=0xe;
			g_dwData=0;
			g_mouseDevice=true;
			g_keymouseevent=true;
			g_GetRawInputDatahook=true;
			message=WM_MBUTTONUP;
			g_message=message;
			lParam=MAKELPARAM(g_x,g_y);
			hWnd=g_currentHwnd;

			::PostThreadMessageA(0x80000000,EM_SETMARGINS,(WPARAM)GetCurrentProcessId(),0);

			if(HookSendMessageRet!=0)
				MySendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			else
				SendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			if(HookPostMessageRet!=0)
				MyPostMessage(g_currentHwnd,message,0,lParam);
			else
				::PostMessage(g_currentHwnd,message,0,lParam);
			//::PostMessage(g_currentHwnd,message,0,lParam);
			InitRawInputDevices();
			if(g_hwndRawInput!=NULL)
				PostMessageA(g_hwndRawInput,WM_INPUT,0,0x12345678);

			if(GameMsg)
			{
				GameMsg->hwnd=g_currentHwnd;
				GameMsg->message=message;
				GameMsg->wParam=wParam;
				GameMsg->lParam=lParam;
				GameMsg->pt=Point;
			}

			message=TS_MBUTTONUP;
			return true;
			break;
		}
	case TS_GETMOUSESHARE:
		{
			if(g_bkmouseshape)
			{
				return pic.GetBkMouseShape(g_bkmouseshape);
			}
			//TSRuntime::add_log("GetBkMouseShape:%x",g_bkmouseshape);
			return false;
			break;
		}
	case TS_SETPATH:
		{
			memset(TSRuntime::SetPath,0,MAX_PATH);
			wcscpy(TSRuntime::SetPath,TSRuntime::pData->setpath);
			return true;
			break;
		}
	case TS_GETCURSORPOS:
		{
			LONG lparam=MAKELPARAM(g_x,g_y);
			return lparam;
			break;
		}
	case TS_MOUSEWHEEL:// 向上wParam=0x780000,向下wParam=0xff880000
		{
			g_GetRawInputDatahook=true;
			POINT Point;
			Point.x=g_x;
			Point.y=g_y;
			::ClientToScreen(hWnd,&Point);
			LPARAM lparam=MAKELPARAM(Point.x,Point.y);
			if(IsNpProtect)
				HookMessageAPI();
			//SendMessage(g_currentHwnd,WM_NCHITTEST,0x8000,lparam);

			g_dwOfs=8;
			if(wParam==0xff880000)//向下
			{
				g_dwData=0xffffff88;
			}
			else if(wParam==0x780000)  //向上
			{
				g_dwData=0x78;
			}
			wParam=MAKEWPARAM(0x8000,g_dwData);
			g_mouseDevice=true;
			g_keymouseevent=true;
			lParam=MAKELPARAM(g_x,g_y);
			message=WM_MOUSEWHEEL;
			g_message=message;

			::PostThreadMessageA(0x80000000,EM_SETMARGINS,(WPARAM)GetCurrentProcessId(),1);

			if(HookSendMessageRet!=0)
				MySendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			else
				SendMessage(g_currentHwnd,WM_NCHITTEST,0,lparam);
			if(HookPostMessageRet!=0)
				MyPostMessage(g_currentHwnd,message,wParam,lParam);
			else
				::PostMessage(g_currentHwnd,message,wParam,lParam);

			InitRawInputDevices();
			if(g_hwndRawInput!=NULL)
				PostMessageA(g_hwndRawInput,WM_INPUT,0,0x12345678);

			if(GameMsg)
			{
				GameMsg->hwnd=g_currentHwnd;
				GameMsg->message=message;
				GameMsg->wParam=wParam;
				GameMsg->lParam=lParam;
				GameMsg->pt=Point;
			}

			message=TS_MOUSEWHEEL;
			//PostMessage(g_currentHwnd,message,wParam,lparam);
			//hWnd=g_currentHwnd;
			return 1;
			break;
		}
	case TS_SETDICT:
		{
			//如果已经注入成功,者需拷贝一份字库信息到共享内存
			if(TSRuntime::MyDictCount[TSRuntime::NowUsingDictIndex]!=NULL)
				delete [] TSRuntime::MyDictCount[TSRuntime::NowUsingDictIndex];

			TSRuntime::NowUsingDictIndex=TSRuntime::pData->NowUsingDictIndex;
			DWORD MyDictCountaddr=(DWORD)TSRuntime::pData->MyDictCount[TSRuntime::NowUsingDictIndex];
			int ndictsize=TSRuntime::pData->nMyDictCountsize[TSRuntime::NowUsingDictIndex];

			TSRuntime::nMyDictCountsize[TSRuntime::NowUsingDictIndex]=ndictsize;
			TSRuntime::nMaxStrHight[TSRuntime::NowUsingDictIndex]=TSRuntime::pData->nMaxStrHight[TSRuntime::NowUsingDictIndex];	

			pMyDictInfo mydict=new MYDICTINFO[ndictsize+1];
			TSRuntime::MyDictCount[TSRuntime::NowUsingDictIndex]=mydict;

			TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
			DWORD injectpid=0;
			::GetWindowThreadProcessId(TSRuntime::pData->InjectHwnd,&injectpid);
			HANDLE handle=OpenProcess(PROCESS_ALL_ACCESS,false,injectpid);
			if(handle==NULL)
			{

				if(handle==NULL)
					delete [] mydict;
			}
			bool si=ReadProcessMemory(handle,(void*)MyDictCountaddr,mydict,ndictsize*sizeof(MYDICTINFO),NULL);
			if(!si)
			{
				delete [] mydict;
			}
			return true;
			break;
		}
	case TS_USEDICT:
		{
			if(TSRuntime::MyDictCount[TSRuntime::NowUsingDictIndex]!=NULL)
				delete [] TSRuntime::MyDictCount[TSRuntime::NowUsingDictIndex];
			TSRuntime::NowUsingDictIndex=TSRuntime::pData->NowUsingDictIndex;
			DWORD MyDictCountaddr=(DWORD)TSRuntime::pData->MyDictCount[TSRuntime::NowUsingDictIndex];
			int ndictsize=TSRuntime::pData->nMyDictCountsize[TSRuntime::NowUsingDictIndex];

			TSRuntime::nMyDictCountsize[TSRuntime::NowUsingDictIndex]=ndictsize;
			TSRuntime::nMaxStrHight[TSRuntime::NowUsingDictIndex]=TSRuntime::pData->nMaxStrHight[TSRuntime::NowUsingDictIndex];	

			pMyDictInfo mydict=new MYDICTINFO[ndictsize+1];
			TSRuntime::MyDictCount[TSRuntime::NowUsingDictIndex]=mydict;

			TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
			HANDLE handle=OpenProcess(PROCESS_ALL_ACCESS,false,(DWORD)wParam);
			if(handle==NULL)
			{
				if(handle==NULL)
					delete [] mydict;
			}
			bool si=ReadProcessMemory(handle,(void*)MyDictCountaddr,mydict,ndictsize*sizeof(MYDICTINFO),NULL);
			if(!si)
			{
				delete [] mydict;
			}
			return true;
			break;
		}
	case TS_LOCKINPUT:
		{
			if(wParam==0)//1 开启锁定(键盘鼠标都锁定), 0关闭锁定,2 只锁定鼠标,3 只锁定键盘
			{
				g_LockInput_kdb=false;
				g_LockInput_mouse=false;
			}
			else if(wParam==1)
			{
				g_LockInput_kdb=true;
				g_LockInput_mouse=true;
			}
			else if(wParam==2)
			{
				g_LockInput_kdb=false;
				g_LockInput_mouse=true;
			}
			else if(wParam==3)
			{
				g_LockInput_kdb=true;
				g_LockInput_mouse=false;
			}
			return true;
			break;
		}
	case TS_DOWNCPU:
		{
			sleeptime=wParam;
			return true;
			break;
		}
	case TS_SENDSTRINGIME:
		{
			IsSendstrIme=true;
			lParam=(lParam|GCS_RESULTSTR);
			SendMessage(g_currentHwnd,WM_IME_COMPOSITION,wParam,lParam); 
			return true;
			break;
		}
	case TS_SETPICPWD:
		{
			wcscpy(TSRuntime::SetPicPwdString,TSRuntime::pData->SetPicPwdString);
			return true;
		}
	case TS_ISGRAPHICPROTECT://开启防图色检测
		{
			//TSRuntime::add_log( "TS_ISGRAPHICPROTECT:DxdeviceAddr:%x",DxdeviceAddr);
			if(wParam==1&&TSRuntime::pData->isgraphicprotect==true&&DxdeviceAddr!=0)
			{
				Ori_DxdeviceAddr=*(DWORD*)DxdeviceAddr;
				*(DWORD*)DxdeviceAddr=(DWORD)MyD3D9Present;//把真正的函数地址修改到我们的函数地址,

				//TSRuntime::add_log( "TS_ISGRAPHICPROTECT:Ori_DxdeviceAddr:%x,DxdeviceAddr:%x",Ori_DxdeviceAddr,*(DWORD*)DxdeviceAddr);
				if(UnhookPresent==false)
				{
					//修改成我们的地址后,我们就可以恢复DX图色钩子了,这样可以达到防止DX图色被检测
					if(SySTpye==1)//XP
						GetHookModFuntion(L"d3d9.dll","Present",D3D9PresentXPoffse,(DWORD)MyD3D9Present,(DWORD)D3D9Present,HookD3D9PresentRet,false);  //xp
					else if(SySTpye==2)//win2003
						GetHookModFuntion(L"d3d9.dll","Present",D3D9PresentWin2003offse,(DWORD)MyD3D9Present,(DWORD)D3D9Present,HookD3D9PresentRet,false); //win2003
					else if(SySTpye==4)//win7
						GetHookModFuntion(L"d3d9.dll","Present",D3D9PresentWin7offse,(DWORD)MyD3D9Present,(DWORD)D3D9Present,HookD3D9PresentRet,false);//win7
					else if(SySTpye==5)//win8
						GetHookModFuntion(L"d3d9.dll","Present",D3D9PresentWin8offse,(DWORD)MyD3D9Present,(DWORD)D3D9Present,HookD3D9PresentRet,false);  //win8

					UnhookPresent=true;
				}
			}
			else if(Ori_DxdeviceAddr!=0)
			{
				if(*(DWORD*)DxdeviceAddr==(DWORD)MyD3D9Present)
					*(DWORD*)DxdeviceAddr=Ori_DxdeviceAddr;//把我们的函数地址修改到真正的函数地址
				//TSRuntime::add_log( "TS_ISGRAPHICPROTECT:UnhookPresent:%x,ORI:%x",Ori_DxdeviceAddr,*(DWORD*)DxdeviceAddr);
			}
			return true;

		}
	default:
		//TSRuntime::pData->isInject=true;
		break;
	}

	if(TSRuntime::pData)
	{
		if(UnhookPresent)
		{
			if((DWORD)MyD3D9Present!=*(DWORD*)DxdeviceAddr)
				UnhookPresent=false;
		}
		if(TSRuntime::pData->isgraphicprotect==true&&UnhookPresent==false&&TSRuntime::pData->ispichook==true)
			::SendMessage(g_currentHwnd,TS_ISGRAPHICPROTECT,1,0);

		//if((!UnhookKbd_DeviceState&&!UnhookMouse_DeviceState)&&IsVIPHookandUnhookAPI)
		//{
		//	HookKbdMouse();
		//	if(UnHookKbdMouse_time==0)
		//		UnHookKbdMouse_time=::GetTickCount();
		//	else
		//	{
		//		if((UnHookKbdMouse_time-::GetTickCount())>=500)
		//			UnHookKbdMouse();
		//	}
		//}
	}

	//if()

	//if(message==WM_USER+1||message==WM_USER+2||message==0x202||message==WM_KEYDOWN||message==WM_KEYUP)
	//TSRuntime::add_log( "Before HOOK成功:lret:%x,hWnd:%x,message:%x,wParam:%x,lParam:%x,X:%d,Y:%d,g_currentHwnd:%x",lret,hWnd,message,wParam,lParam,(short)LOWORD(lParam),(short)HIWORD(lParam),g_currentHwnd);
	//TSRuntime::add_log( "InternalCallWinProcstart-call:%x,message:%x,wParam:%x,lParam:%d",fAnsi,message,wParam,lParam);

	lret=USER32InternalCallWinProc(fAnsi,hWnd,message,wParam,lParam);

	//TSRuntime::add_log( "InternalCallWinProcstart-end:%x,wParam:%x,lParam:%d",message,wParam,lParam);
	//if(message==WM_SYSKEYDOWN)
	//	TSRuntime::add_log( "wParam:%x,lParam:%x",wParam,lParam);
	//TSRuntime::add_log( "hWnd:%x,message:%x,wParam:%x,LOWORD(lParam):%d,HIWORD(lParam):%d",hWnd,message,wParam,(short)LOWORD(lParam),(short)HIWORD(lParam));

	return lret;
	//return 1;
}


ULONG  __stdcall MyNtUserCallOneParam(DWORD Param, DWORD Routine)
{
	ULONG  ret=0;
	if(Routine==0x9)//GetMessagePos
	{
		POINT xy;
		GetCursorPos(&xy);
		ret=(ULONG)MAKELPARAM(xy.x,xy.y);
		//TSRuntime::add_log("NtUserCallOneParam-GetMessagePos");
		return 1;
	}
	else if(Routine==0x26)//GetCursorPos,ScreenToClient
	{
		LPPOINT lpPoint=(LPPOINT)Param;
		POINT Point;
		Point.x=g_x;
		Point.y=g_y;
		::ClientToScreen(g_currentHwnd,&Point);
		lpPoint->x=Point.x;
		lpPoint->y=Point.y;
		//TSRuntime::add_log("NtUserCallOneParam-Param:%x,lpPoint:x:%d,lpPoint:y:%d,ret:%x",Param,lpPoint->x,lpPoint->y,ret);
		return 1;

	}
	ret=NtUserCallOneParam(Param,Routine);
	//TSRuntime::add_log("NtUserCallOneParam--Param:%x,Routine:%x,ret:%x",Param,Routine,ret);
	return ret;
}

ULONG  __stdcall MyNtUserCallTwoParam( DWORD Param1,DWORD Param2,DWORD Routine)
{
	ULONG  ret=0;
	if((Routine==0x74&&IsWin7)||(Routine==0x7B&&IsWin8)||((IsWinxp||IsWin2003)&&Routine==0x6f))//SetCursorPos
	{
		return 0;
	}
	if((Routine==0x69&&IsWin7)||(Routine==0x70&&IsWin8))//GetCursorPos,ScreenToClient
	{
		LPPOINT lpPoint=(LPPOINT)Param1;
		POINT Point;
		Point.x=g_x;
		Point.y=g_y;
		::ClientToScreen(g_currentHwnd,&Point);
		lpPoint->x=Point.x;
		lpPoint->y=Point.y;
		//TSRuntime::add_log("MyNtUserCallTwoParam-Param:%x,lpPoint:x:%d,lpPoint:y:%d,ret:%x",Param2,lpPoint->x,lpPoint->y,ret);
		return 1;
	}
	ret=NtUserCallTwoParam(Param1,Param1,Routine);
	//TSRuntime::add_log("MyNtUserCallTwoParam-Param:%x,lpPoint:x:%d,lpPoint:y:%d,ret:%x",Param2,lpPoint->x,lpPoint->y,ret);
	return ret;
}

ULONG_PTR  __stdcall MyNtUserGetThreadState( DWORD 	Routine)
{
	ULONG_PTR  ret=0;
	if(Routine==0||Routine==1||Routine==2||Routine==16)//GetFocus,GetActiveWindow,GetCapture,16:UserThreadStateIsForeground
	{
		ret=(ULONG_PTR)g_currentHwnd;
		return ret;
	}
	ret=NtUserGetThreadState(Routine);
	//TSRuntime::add_log("NtUserGetThreadState--Routine:%d,ret:%x",Routine,ret);
	return ret;
}

SHORT  __stdcall MyNtUserGetKeyState( int nVirtKey)
{
	//TSRuntime::add_log("NtUserGetKeyState--nVirtKey:%x",nVirtKey);
	ULONG  sret=0;
	if(g_GetKeyState||g_mouseDevice)
	{
		DWORD ALT_KEYCODE=0;
		bool iskeyevent=false;
		for(int i=0;i<0x100;i++)
		{
			if(1==TSRuntime::KeyPressMap[i])
			{
				if(nVirtKey==i)
				{
					iskeyevent=true;
					break;
				}
				else if((i==VK_MENU&&(nVirtKey==VK_LMENU||nVirtKey==VK_RMENU))||(i==VK_SHIFT&&(nVirtKey==VK_LSHIFT||nVirtKey==VK_RSHIFT))
					||(i==VK_CONTROL&&(nVirtKey==VK_LCONTROL||nVirtKey==VK_RCONTROL)))
				{
					iskeyevent=true;
					break;
				}
			}
		}
		if(g_message==WM_LBUTTONDOWN||g_message==WM_RBUTTONDOWN||g_message==WM_MBUTTONDOWN
			||g_message==WM_LBUTTONUP||g_message==WM_RBUTTONUP||g_message==WM_MBUTTONUP)
		{
			if(nVirtKey==VK_LBUTTON||nVirtKey==VK_RBUTTON||nVirtKey==VK_MBUTTON)
			{
				iskeyevent=true;
				if(g_message==WM_LBUTTONDOWN||g_message==WM_RBUTTONDOWN||g_message==WM_MBUTTONDOWN)
					ismousedown=true;
				//TSRuntime::add_log( "MyGetKeyState-1:g_message:%x,nVirtKey:%x,ismousedown:%d",g_message,nVirtKey,ismousedown);
			}
			if(g_message==WM_LBUTTONUP||g_message==WM_RBUTTONUP||g_message==WM_MBUTTONUP)
			{
				ismousedown=false;
				g_mouseDevice=false;
			}
			//TSRuntime::add_log( "MyGetKeyState:g_message:%x,nVirtKey:%x,ismousedown:%d",g_message,nVirtKey,ismousedown);
		}
		//if((g_message==WM_KEYDOWN&&g_wParam==nVirtKey)||(g_message==WM_SYSKEYDOWN&&nVirtKey==VK_MENU)||(g_message==WM_SYSKEYDOWN&&nVirtKey==VK_LMENU) )
		if((((g_message==WM_KEYDOWN)||(g_message==WM_SYSKEYDOWN)
			||(g_message==WM_LBUTTONDOWN)
			||(g_message==WM_RBUTTONDOWN)
			||(g_message==WM_MBUTTONDOWN))&&iskeyevent)||((nVirtKey==VK_LBUTTON||nVirtKey==VK_RBUTTON||nVirtKey==VK_MBUTTON)&&ismousedown))	
		{
			sret=0xffffff80;
			//TSRuntime::add_log( "MyGetKeyState-WM_SYSKEYDOWN:g_message:%x,nVirtKey:%x,ismousedown:%d",g_message,nVirtKey,ismousedown);
			//返回负数说明该键被按下
		}
		//else if((g_message==WM_KEYUP||g_message==WM_SYSKEYUP)&&g_wParam==nVirtKey)
		else if(((g_message==WM_KEYUP||g_message==WM_SYSKEYUP)
			||(g_message==WM_LBUTTONUP)
			||(g_message==WM_RBUTTONUP)
			||(g_message==WM_MBUTTONUP))&&iskeyevent)	
		{
			//TSRuntime::add_log( "MyGetKeyState-WM_SYSKEYUP:g_message:%x,nVirtKey:%x,ismousedown:%d",g_message,nVirtKey,ismousedown);
			sret=1;	//返回正数说明该键被弹起
			g_GetKeyState=false;
			TSRuntime::KeyPressMap[nVirtKey]=0;
			//g_wParam=0;
		}
		//TSRuntime::add_log( "GetKeyState:nVirtKey:%x,sret:%x,g_message:%x",nVirtKey,sret,g_message);
		//TSRuntime::add_log( "MyGetKeyState:nVirtKey:%x",nVirtKey);
		return sret;
		//TerminateProcess()
		//CloseWindow;
		//::DestroyWindow()
		//ExitThread()
	}
	sret=NtUserGetKeyState(nVirtKey);
	return sret;
}

SHORT  __stdcall MyNtUserGetAsyncKeyState( int vKey)
{
	ULONG  sret=0;
	//TSRuntime::add_log("NtUserGetAsyncKeyState--nVirtKey:%x,g_message:%x,g_mouseDevice:%d",vKey,g_message,g_mouseDevice);
	if(g_kbdDevice||g_mouseDevice)
	{
		if((g_message==WM_LBUTTONDOWN&&vKey==VK_LBUTTON)
			||(g_message==WM_RBUTTONDOWN&&vKey==VK_RBUTTON)
			||(g_message==WM_MBUTTONDOWN&&vKey==VK_MBUTTON))
		{
			sret=0x8000;
			//TSRuntime::add_log("NtUserGetAsyncKeyState--nVirtKey:%x,g_message:%x,g_mouseDevice:%d",vKey,g_message,g_mouseDevice);
			return sret;
		}
		else if((g_message==WM_LBUTTONUP&&vKey==VK_LBUTTON)
			||(g_message==WM_RBUTTONUP&&vKey==VK_LBUTTON)
			||(g_message==WM_MBUTTONUP&&vKey==VK_MBUTTON))
		{
			sret=0;
			g_message=0;
			//g_mouseDevice=false;
		}
		else if(g_message==WM_KEYDOWN||g_message==WM_KEYUP||g_message==WM_SYSKEYDOWN||g_message==WM_SYSKEYUP)
		{
			bool iskeyevent=false;
			for(int i=0;i<0x100;i++)
			{
				if(1==TSRuntime::KeyPressMap[i])
				{
					//int key=TSRuntime::VirtualToASCIIMap[i];
					if(vKey==i)
					{
						if((g_message==WM_KEYUP || g_message==WM_SYSKEYUP)	&& i == g_wParam)
							TSRuntime::KeyPressMap[i]=0;
						else if(i != g_wParam)
							return 0x8000;
						iskeyevent=true;
						break;
					}
				}

			}
			//TSRuntime::add_log("MyGetAsyncKeyState:iskeyevent:%d,vKey:%d,g_message:%x",iskeyevent,vKey,g_message);
			if(iskeyevent&&(g_message==WM_KEYDOWN||g_message==WM_SYSKEYDOWN))
			{
				sret=0x8000;
				//TSRuntime::add_log("iskeyevent:MyGetAsyncKeyState:sret:%d,vKey:%d",sret,vKey);
				return sret;
			}
			else if(iskeyevent&&(g_message==WM_KEYUP||g_message==WM_SYSKEYUP))
			{
				//g_keydown=0xFFFF8001;//重置
				sret=0;
				g_kbdDevice=false;
				//TSRuntime::add_log("iskeyevent:MyGetAsyncKeyState:sret:%d,vKey:%d",sret,vKey);
			}
		}

	}
	if(g_LockInput_kdb||g_LockInput_mouse)   //封锁外部键盘鼠标
	{
		if(g_LockInput_mouse)
		{
			if(vKey==VK_LBUTTON||vKey==VK_RBUTTON||vKey==VK_MBUTTON)
				sret=0;
		}
		else if(g_LockInput_kdb)
		{
			sret=0;
		}
		//TSRuntime::add_log("NtUserGetAsyncKeyState--g_LockInput_kdb:%x,g_LockInput_mouse:%x",g_LockInput_kdb,g_LockInput_mouse);
		return 0;

	}
	sret=NtUserGetAsyncKeyState(vKey);
	return sret;
}

void VIPHookandUnhookAPI(void *para)
{

	//TSRuntime::add_log( "hprocess:%d",::GetCurrentProcess());
	//TSRuntime::add_log( "HookandUnhookAPI-start,TSRuntime::IsVirtualProtectEx:%d",TSRuntime::IsVirtualProtectEx);
	IsVIPHookandUnhookAPI=true;
	bool ishook=(bool)para;
	SySTpye=TSRuntime::InitialWindowsVersion();//先获取系统版本后在HOOK,1:XP,2:2003,4:win7

	//	if(ishook)
	//{
	//	//GetHookModFuntion(L"user32.dll","PeekMessageA",0,(DWORD)MyPeekMessageA,(DWORD)UnhookPeekMessage,HookPeekMessageARet,ishook);
	//	//GetHookModFuntion(L"user32.dll","PeekMessageW",0,(DWORD)MyPeekMessageW,(DWORD)UnhookPeekMessage,HookPeekMessageWRet,ishook);
	//}
	if(SySTpye==1||SySTpye==2)	//SySTpye==1:XP系统,
	{
		if(SySTpye==1)
		{		
			IsWinxp=true;
			//TSRuntime::add_log( "WinXP");
			GetHookModFuntion(L"user32.dll","InternalCallWinProc",USER32InternalCallWinProcXPoffse,(DWORD)MyInternalCallWinProc,(DWORD)UnhookInternalCallWinProc,HookInternalCallWinProcRet,ishook);//HOOK:XP系统下的:USER32.dll的InternalCallWinProc函数

			if((TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ispichook)&&UnhookPresent==false)
			{
				if(UnhookPresent==false&&DxdeviceAddr==0)
					GetHookModFuntion(L"d3d9.dll","Present",D3D9PresentXPoffse,(DWORD)MyD3D9Present,(DWORD)D3D9Present,HookD3D9PresentRet,ishook);
				else if(ishook==true)
				{
					Ori_DxdeviceAddr=*(DWORD*)DxdeviceAddr;
					*(DWORD*)DxdeviceAddr=(DWORD)MyD3D9Present;//把真正的函数地址修改到我们的函数地址,
					UnhookPresent=true;
				}
			}

			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||TSRuntime::pData->iskeyboardhook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserGetThreadState",NtUserGetThreadState_XPOffset,(DWORD)MyNtUserGetThreadState,(DWORD)NtUserGetThreadStateUnhookByte,NtUserGetThreadStateRet,ishook);
			}
			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserCallOneParam",NtUserCallOneParam_XPOffset,(DWORD)MyNtUserCallOneParam,(DWORD)NtUserCallOneParamUnhookByte,NtUserCallOneParamRet,ishook);
				GetHookModFuntion(L"user32.dll","NtUserCallTwoParam",NtUserCallTwoParam_XPOffset,(DWORD)MyNtUserCallTwoParam,(DWORD)NtUserCallTwoParamUnhookByte,NtUserCallTwoParamRet,ishook);
			}
			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->iskeyboardhook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserGetKeyState",NtUserGetKeyState_XPOffset,(DWORD)MyNtUserGetKeyState,(DWORD)NtUserGetKeyStateUnhookByte,NtUserGetKeyStateRet,ishook);
				GetHookModFuntion(L"user32.dll","NtUserGetAsyncKeyState",NtUserGetAsyncKeyState_XPOffset,(DWORD)MyNtUserGetAsyncKeyState,(DWORD)NtUserGetAsyncKeyStateUnhookByte,NtUserGetAsyncKeyStateRet,ishook);  
			}
		}
		else if(SySTpye==2)	//SySTpye==2:Win2003系统
		{
			IsWin2003=true;
			// TSRuntime::add_log( "Win2003");
			GetHookModFuntion(L"user32.dll","InternalCallWinProc",USER32InternalCallWinProcWin2003offse,(DWORD)MyInternalCallWinProc,(DWORD)UnhookInternalCallWinProc,HookInternalCallWinProcRet,ishook);//HOOK:XP系统下的:USER32.dll的InternalCallWinProc函数
			if((TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ispichook)&&UnhookPresent==false)
			{
				if(UnhookPresent==false&&DxdeviceAddr==0)
					GetHookModFuntion(L"d3d9.dll","Present",D3D9PresentWin2003offse,(DWORD)MyD3D9Present,(DWORD)D3D9Present,HookD3D9PresentRet,ishook);
				else if(ishook==true)
				{
					Ori_DxdeviceAddr=*(DWORD*)DxdeviceAddr;
					*(DWORD*)DxdeviceAddr=(DWORD)MyD3D9Present;//把真正的函数地址修改到我们的函数地址,
					UnhookPresent=true;
				}
			}

			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||TSRuntime::pData->iskeyboardhook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserGetThreadState",NtUserGetThreadState_Win2003Offset,(DWORD)MyNtUserGetThreadState,(DWORD)NtUserGetThreadStateUnhookByte,NtUserGetThreadStateRet,ishook);
			}
			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserCallOneParam",NtUserCallOneParam_Win2003Offset,(DWORD)MyNtUserCallOneParam,(DWORD)NtUserCallOneParamUnhookByte,NtUserCallOneParamRet,ishook);
				GetHookModFuntion(L"user32.dll","NtUserCallTwoParam",NtUserCallTwoParam_Win2003Offset,(DWORD)MyNtUserCallTwoParam,(DWORD)NtUserCallTwoParamUnhookByte,NtUserCallTwoParamRet,ishook);
			}
			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->iskeyboardhook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserGetKeyState",NtUserGetKeyState_Win2003Offset,(DWORD)MyNtUserGetKeyState,(DWORD)NtUserGetKeyStateUnhookByte,NtUserGetKeyStateRet,ishook);
				GetHookModFuntion(L"user32.dll","NtUserGetAsyncKeyState",NtUserGetAsyncKeyState_Win2003Offset,(DWORD)MyNtUserGetAsyncKeyState,(DWORD)NtUserGetAsyncKeyStateUnhookByte,NtUserGetAsyncKeyStateRet,ishook);  
			}

		}
		if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||TSRuntime::pData->iskeyboardhook||ishook==false)
		{
			//if(ishook||UnhookKbd_DeviceState||UnhookMouse_DeviceState)
			{
				//if(UnhookKbd_DeviceState||Kbd_DeviceStateAddr==0)
				GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJMouseXPoffse,(DWORD)MyGetDeviceStateMouse,(DWORD)GetDeviceStateMouse,HookGetDeviceStateMouseRet,ishook);////HOOK:dinput8.dll的GetDeviceState函数
				//if(UnhookMouse_DeviceState||Mouse_DeviceStateAddr==0)
				GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJkeyboardXPoffse,(DWORD)MyGetDeviceStateKbd,(DWORD)GetDeviceStateKbd,HookGetDeviceStateKbdRet,ishook);
				//HookKbdMouse_Thread=(HANDLE)_beginthread(HookKbdMouse,0,0);
			}

		}
	}
	else if(SySTpye==4)//win7系统
	{
		IsWin7=true;
		typedef bool (__stdcall  *  myWow64DisableWow64FsRedirection)(LPVOID); 
		myWow64DisableWow64FsRedirection f_myWow64DisableWow64FsRedirection=  NULL; 
		HINSTANCE hlibrary;
		hlibrary = LoadLibrary(_T("Kernel32.dll"));
		f_myWow64DisableWow64FsRedirection=(myWow64DisableWow64FsRedirection)GetProcAddress(hlibrary,"Wow64DisableWow64FsRedirection");
		PVOID vlau=NULL;
		bool ret=f_myWow64DisableWow64FsRedirection(&vlau);
		//如果是win7_64系统ret返回true
		if(!ret)
		{
			//TSRuntime::add_log( "IWProcWIN7");					  //WIN7X86
			GetHookModFuntion(L"user32.dll","InternalCallWinProc",USER32InternalCallProcWin7offse,(DWORD)MyInternalCallWinProc,(DWORD)UnhookInternalCallWinProc,HookInternalCallWinProcRet,ishook);//HOOK:XP系统下的:USER32.dll的InternalCallWinProc函数

			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||TSRuntime::pData->iskeyboardhook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserGetThreadState",NtUserGetThreadState_WIN7X86Offset,(DWORD)MyNtUserGetThreadState,(DWORD)NtUserGetThreadStateUnhookByte,NtUserGetThreadStateRet,ishook);
			}
			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||ishook==false)
			{
				//GetHookModFuntion(L"user32.dll","NtUserCallOneParam",NtUserCallOneParam_WIN7X86Offset,(DWORD)MyNtUserCallOneParam,(DWORD)NtUserCallOneParamUnhookByte,NtUserCallOneParamRet,ishook);
				GetHookModFuntion(L"user32.dll","NtUserCallTwoParam",NtUserCallTwoParam_WIN7X86Offset,(DWORD)MyNtUserCallTwoParam,(DWORD)NtUserCallTwoParamUnhookByte,NtUserCallTwoParamRet,ishook);
			}
			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->iskeyboardhook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserGetKeyState",NtUserGetKeyState_WIN7X86Offset,(DWORD)MyNtUserGetKeyState,(DWORD)NtUserGetKeyStateUnhookByte,NtUserGetKeyStateRet,ishook);
				GetHookModFuntion(L"user32.dll","NtUserGetAsyncKeyState",NtUserGetAsyncKeyState_WIN7X86Offset,(DWORD)MyNtUserGetAsyncKeyState,(DWORD)NtUserGetAsyncKeyStateUnhookByte,NtUserGetAsyncKeyStateRet,ishook);  
			}
		}
		else //win7x64
		{
			//TSRuntime::add_log( "IWProcWIN7X64");
			IsWin7X64=true;

			GetHookModFuntion(L"user32.dll","InternalCallWinProc",USER32InternalCallProcWin7x64offse,(DWORD)MyInternalCallWinProc,(DWORD)UnhookInternalCallWinProc,HookInternalCallWinProcRet,ishook);//HOOK:XP系统下的:USER32.dll的InternalCallWinProc函数	 //
			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||TSRuntime::pData->iskeyboardhook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserGetThreadState",NtUserGetThreadState_WIN7X64Offset,(DWORD)MyNtUserGetThreadState,(DWORD)NtUserGetThreadStateUnhookByte,NtUserGetThreadStateRet,ishook);
			}
			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||ishook==false)
			{
				//GetHookModFuntion(L"user32.dll","NtUserCallOneParam",NtUserCallOneParam_WIN7X64Offset,(DWORD)MyNtUserCallOneParam,(DWORD)NtUserCallOneParamUnhookByte,NtUserCallOneParamRet,ishook);
				GetHookModFuntion(L"user32.dll","NtUserCallTwoParam",NtUserCallTwoParam_WIN7X64Offset,(DWORD)MyNtUserCallTwoParam,(DWORD)NtUserCallTwoParamUnhookByte,NtUserCallTwoParamRet,ishook);
			}
			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->iskeyboardhook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserGetKeyState",NtUserGetKeyState_WIN7X64Offset,(DWORD)MyNtUserGetKeyState,(DWORD)NtUserGetKeyStateUnhookByte,NtUserGetKeyStateRet,ishook);
				GetHookModFuntion(L"user32.dll","NtUserGetAsyncKeyState",NtUserGetAsyncKeyState_WIN7X64Offset,(DWORD)MyNtUserGetAsyncKeyState,(DWORD)NtUserGetAsyncKeyStateUnhookByte,NtUserGetAsyncKeyStateRet,ishook);  
			}
		}
		if((TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ispichook)&&UnhookPresent==false)//02d460a0
		{
			if(UnhookPresent==false&&DxdeviceAddr==0)
				GetHookModFuntion(L"d3d9.dll","Present",D3D9PresentWin7offse,(DWORD)MyD3D9Present,(DWORD)D3D9Present,HookD3D9PresentRet,ishook);
			else if(ishook==true)
			{
				Ori_DxdeviceAddr=*(DWORD*)DxdeviceAddr;
				*(DWORD*)DxdeviceAddr=(DWORD)MyD3D9Present;//把真正的函数地址修改到我们的函数地址,
				UnhookPresent=true;
			}
		}



		if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||TSRuntime::pData->iskeyboardhook||ishook==false)
		{
			//if(ishook||UnhookKbd_DeviceState||UnhookMouse_DeviceState)
			//if(UnhookKbd_DeviceState||Kbd_DeviceStateAddr==0)
			GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJkeyboardWin7offse,(DWORD)MyGetDeviceStateKbd,(DWORD)GetDeviceStateKbd,HookGetDeviceStateKbdRet,ishook);
			//if(UnhookMouse_DeviceState||Mouse_DeviceStateAddr==0)
			GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJMouseWin7offse,(DWORD)MyGetDeviceStateMouse,(DWORD)GetDeviceStateMouse,HookGetDeviceStateMouseRet,ishook);////HOOK:dinput8.dll的GetDeviceState函数
		}	
	}
	else if(SySTpye==5)//win8系统
	{
		IsWin8=true;
		typedef bool (__stdcall  *  myWow64DisableWow64FsRedirection)(LPVOID); 
		myWow64DisableWow64FsRedirection f_myWow64DisableWow64FsRedirection=  NULL; 
		HINSTANCE hlibrary;
		hlibrary = LoadLibrary(_T("Kernel32.dll"));
		f_myWow64DisableWow64FsRedirection=(myWow64DisableWow64FsRedirection)GetProcAddress(hlibrary,"Wow64DisableWow64FsRedirection");
		PVOID vlau=NULL;
		bool ret=f_myWow64DisableWow64FsRedirection(&vlau);
		//如果是win8x64系统ret返回true
		if(!ret)
		{
			//TSRuntime::add_log( "WIN8X86");					  //WIN8X86
			GetHookModFuntion(L"user32.dll","InternalCallWinProc",USER32InternalCallProcWin8offse,(DWORD)MyInternalCallWinProc,(DWORD)UnhookInternalCallWinProc,HookInternalCallWinProcRet,ishook);//HOOK:系统下的:USER32.dll的InternalCallWinProc函数

			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||TSRuntime::pData->iskeyboardhook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserGetThreadState",NtUserGetThreadState_WIN8X86Offset,(DWORD)MyNtUserGetThreadState,(DWORD)NtUserGetThreadStateUnhookByte,NtUserGetThreadStateRet,ishook);
			}
			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserCallTwoParam",NtUserCallTwoParam_WIN8X86Offset,(DWORD)MyNtUserCallTwoParam,(DWORD)NtUserCallTwoParamUnhookByte,NtUserCallTwoParamRet,ishook);
			}
			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->iskeyboardhook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserGetKeyState",NtUserGetKeyState_WIN8X86Offset,(DWORD)MyNtUserGetKeyState,(DWORD)NtUserGetKeyStateUnhookByte,NtUserGetKeyStateRet,ishook);
				GetHookModFuntion(L"user32.dll","NtUserGetAsyncKeyState",NtUserGetAsyncKeyState_WIN8X86Offset,(DWORD)MyNtUserGetAsyncKeyState,(DWORD)NtUserGetAsyncKeyStateUnhookByte,NtUserGetAsyncKeyStateRet,ishook);  
			}
		}
		else //win8x64
		{
			//TSRuntime::add_log( "win8x64");
			IsWin8X64=true;
			GetHookModFuntion(L"user32.dll","InternalCallWinProc",USER32InternalCallProcWin8x64offse,(DWORD)MyInternalCallWinProc,(DWORD)UnhookInternalCallWinProc,HookInternalCallWinProcRet,ishook);//HOOK:系统下的:USER32.dll的InternalCallWinProc函数	 //

			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||TSRuntime::pData->iskeyboardhook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserGetThreadState",NtUserGetThreadState_WIN8X64Offset,(DWORD)MyNtUserGetThreadState,(DWORD)NtUserGetThreadStateUnhookByte,NtUserGetThreadStateRet,ishook);
			}
			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserCallTwoParam",NtUserCallTwoParam_WIN8X64Offset,(DWORD)MyNtUserCallTwoParam,(DWORD)NtUserCallTwoParamUnhookByte,NtUserCallTwoParamRet,ishook);
			}
			if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->iskeyboardhook||ishook==false)
			{
				GetHookModFuntion(L"user32.dll","NtUserGetKeyState",NtUserGetKeyState_WIN8X64Offset,(DWORD)MyNtUserGetKeyState,(DWORD)NtUserGetKeyStateUnhookByte,NtUserGetKeyStateRet,ishook);
				GetHookModFuntion(L"user32.dll","NtUserGetAsyncKeyState",NtUserGetAsyncKeyState_WIN8X64Offset,(DWORD)MyNtUserGetAsyncKeyState,(DWORD)NtUserGetAsyncKeyStateUnhookByte,NtUserGetAsyncKeyStateRet,ishook);  
			}
		}

		if((TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ispichook)&&UnhookPresent==false)
		{
			if(UnhookPresent==false&&DxdeviceAddr==0)
				GetHookModFuntion(L"d3d9.dll","Present",D3D9PresentWin8offse,(DWORD)MyD3D9Present,(DWORD)D3D9Present,HookD3D9PresentRet,ishook);
			else if(ishook==true)
			{
				Ori_DxdeviceAddr=*(DWORD*)DxdeviceAddr;
				*(DWORD*)DxdeviceAddr=(DWORD)MyD3D9Present;//把真正的函数地址修改到我们的函数地址,
				UnhookPresent=true;
			}
		}

		if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||TSRuntime::pData->iskeyboardhook||ishook==false)
		{
			GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJMouseWin8offse,(DWORD)MyGetDeviceStateMouse,(DWORD)GetDeviceStateMouse,HookGetDeviceStateMouseRet,ishook);////HOOK:dinput8.dll的GetDeviceState函数
			GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJkeyboardWin8offse,(DWORD)MyGetDeviceStateKbd,(DWORD)GetDeviceStateKbd,HookGetDeviceStateKbdRet,ishook);
			//HookKbdMouse_Thread=(HANDLE)_beginthread(HookKbdMouse,0,0);
		}
	}

	if(TSRuntime::pData->Isdxantiapi==false)//开启次盾属性可能会导致部分后台功能失效,默认不开启
	{
		GetHookModFuntion(L"user32.dll","GetForegroundWindow",0,(DWORD)MyGetForegroundWindow,(DWORD)UnhookGetForegroundWindow,HookGetForegroundWindowRet,ishook);
		if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||ishook==false)
		{
			GetHookModFuntion(L"user32.dll","SetCursor",0,(DWORD)MySetCursor,(DWORD)USER32SetCursor,HookSetCursorRet,ishook);
		}
		GetHookModFuntion(L"Imm32.dll","ImmGetCompositionStringA",0,(DWORD)MyIMM32ImmGetCompositionStringA,(DWORD)IMM32ImmGetCompositionStringA,HookImmGetCompositionStringARet,ishook);
		GetHookModFuntion(L"Imm32.dll","ImmGetCompositionStringW",0,(DWORD)MyIMM32ImmGetCompositionStringW,(DWORD)IMM32ImmGetCompositionStringW,HookImmGetCompositionStringWRet,ishook); 
	}
	if(TSRuntime::pData)
	{
		TSRuntime::pData->isInject=true;
		TSRuntime::pData->ispichook=false;
		g_InjectHWND=TSRuntime::pData->InjectHwnd;
	}

}

//DWORD       g_GetRawInputData=0;//xp:0x11ac,2003:,win7:
void HookandUnhookAPI(void *para)
{
	//TSRuntime::add_log( "HookandUnhookAPI-start");
	////TSRuntime::add_log( "hprocess:%d",::GetCurrentProcess());
	////TSRuntime::add_log( "HookandUnhookAPI-start,TSRuntime::IsVirtualProtectEx:%d",TSRuntime::IsVirtualProtectEx);
	bool ishook=(bool)para;
	bool isHSProtect=false;//下钩子前检测是否有HS保护的模块
	//SySTpye=TSRuntime::InitialWindowsVersion();//先获取系统版本后在HOOK,1:XP,2:2003,4:win7
	//if(ishook)
	//{
	//	GetHookModFuntion(L"user32.dll","PeekMessageA",0,(DWORD)MyPeekMessageA,(DWORD)UnhookPeekMessage,HookPeekMessageARet,ishook);
	//	GetHookModFuntion(L"user32.dll","PeekMessageW",0,(DWORD)MyPeekMessageW,(DWORD)UnhookPeekMessage,HookPeekMessageWRet,ishook);
	//}
	if(IsWindow(g_InjectHWND)==FALSE&&g_InjectHWND!=NULL)//判断注入进程是否异常退出
	{
		//if(TSRuntime::pData)
		//	delete [] TSRuntime::pData;

		char pszMapName[MAX_PATH]={0};
		sprintf( pszMapName,"%s%d",TS_MAPVIEW_NAME,GetCurrentProcessId());
		HANDLE hFileMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, pszMapName);
		//如果hFileMap句柄不为空并且pData==NULL说明DLL被注入,准备启动注入线程
		if(hFileMap!=NULL)                           
		{ 
			//这里先打开共享内存,共享内存和程序是一对一的关系
			CShareMemory *sm=new CShareMemory(pszMapName);
			TSRuntime::pData=(CMessageData*)sm->GetBuffer();
		}
	}

	HMODULE HSmode=GetModuleHandle(L"EhSvc.dll");
	if(HSmode)
		isHSProtect=true;

	if(SySTpye==1||SySTpye==2)	//SySTpye==1:XP系统,
	{
		if(SySTpye==1)
		{	
			GetHookModFuntion(L"user32.dll","InternalCallWinProc",USER32InternalCallWinProcXPoffse,(DWORD)MyInternalCallWinProc,(DWORD)UnhookInternalCallWinProc,HookInternalCallWinProcRet,ishook);//HOOK:XP系统下的:USER32.dll的InternalCallWinProc函数

			if((TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ispichook)&&UnhookPresent==false)
			{
				if(UnhookPresent==false&&DxdeviceAddr==0)
					GetHookModFuntion(L"d3d9.dll","Present",D3D9PresentXPoffse,(DWORD)MyD3D9Present,(DWORD)D3D9Present,HookD3D9PresentRet,ishook);
				else if(ishook==true)
				{
					*(DWORD*)DxdeviceAddr=(DWORD)MyD3D9Present;//把真正的函数地址修改到我们的函数地址,
					UnhookPresent=true;
				}
			}
			g_OriSetCursor=0x11ff;
			g_GetRawInputData=0x11ac;
			dwMoveGetKeyboardState=0x119e;
			//TSRuntime::add_log( "WinXP");
		}
		else if(SySTpye==2)	//SySTpye==2:Win2003系统
		{
			// TSRuntime::add_log( "Win2003");
			GetHookModFuntion(L"user32.dll","InternalCallWinProc",USER32InternalCallWinProcWin2003offse,(DWORD)MyInternalCallWinProc,(DWORD)UnhookInternalCallWinProc,HookInternalCallWinProcRet,ishook);//HOOK:XP系统下的:USER32.dll的InternalCallWinProc函数
			if((TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ispichook)&&UnhookPresent==false)
			{
				if(UnhookPresent==false&&DxdeviceAddr==0)
					GetHookModFuntion(L"d3d9.dll","Present",D3D9PresentWin2003offse,(DWORD)MyD3D9Present,(DWORD)D3D9Present,HookD3D9PresentRet,ishook);
				else if(ishook==true)
				{
					*(DWORD*)DxdeviceAddr=(DWORD)MyD3D9Present;//把真正的函数地址修改到我们的函数地址,
					UnhookPresent=true;
				}
			}	
			g_OriSetCursor=0x11fd;
			g_GetRawInputData=0x11ab;
			dwMoveGetKeyboardState=0x119d;
		}
		if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||ishook==false)
		{
			GetHookModFuntion(L"user32.dll","ScreenToClient",0,(DWORD)MyScreenToClient,(DWORD)USER32ScreenToClient,HookScreenToClientRet,ishook);//HOOKScreenToClient
		}
		if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||TSRuntime::pData->iskeyboardhook||ishook==false)
		{
			GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJMouseXPoffse,(DWORD)MyGetDeviceStateMouse,(DWORD)GetDeviceStateMouse,HookGetDeviceStateMouseRet,ishook);//HOOK:XP系统下的:dinput8.dll的GetDeviceState函数
			GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJkeyboardXPoffse,(DWORD)MyGetDeviceStateKbd,(DWORD)GetDeviceStateKbd,HookGetDeviceStateKbdRet,ishook);
		}
	}
	else if(SySTpye==4)//win7系统
	{
		if(!TSRuntime::IsWin7X64)
		{
			//TSRuntime::add_log( "IWProcWIN7");					  //WIN7X86
			GetHookModFuntion(L"user32.dll","InternalCallWinProc",USER32InternalCallProcWin7offse,(DWORD)MyInternalCallWinProc,(DWORD)UnhookInternalCallWinProc,HookInternalCallWinProcRet,ishook);//HOOK:XP系统下的:USER32.dll的InternalCallWinProc函数
			g_GetRawInputData=0x11c0;
		}
		else //win7x64
		{
			//TSRuntime::add_log( "IWProcWIN7X64");
			IsWin7X64=true;
			GetHookModFuntion(L"user32.dll","InternalCallWinProc",USER32InternalCallProcWin7x64offse,(DWORD)MyInternalCallWinProc,(DWORD)UnhookInternalCallWinProc,HookInternalCallWinProcRet,ishook);//HOOK:XP系统下的:USER32.dll的InternalCallWinProc函数	 //
			g_GetRawInputData=0x12be;
		}

		//g_OriSetCursor=0x1221;
		//dwMoveGetKeyboardState=0x11b2;
		if((TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ispichook)&&UnhookPresent==false)
		{
			if(UnhookPresent==false&&DxdeviceAddr==0)
				GetHookModFuntion(L"d3d9.dll","Present",D3D9PresentWin7offse,(DWORD)MyD3D9Present,(DWORD)D3D9Present,HookD3D9PresentRet,ishook);
			else if(ishook==true)
			{
				//TSRuntime::add_log( "MyD3D9Present");
				*(DWORD*)DxdeviceAddr=(DWORD)MyD3D9Present;//把真正的函数地址修改到我们的函数地址,
				UnhookPresent=true;
			}
		}

		if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||ishook==false)
		{
			GetHookModFuntion(L"user32.dll","ScreenToClient",0,(DWORD)MyScreenToClient,(DWORD)UnhookScreenToClientRet,HookScreenToClientRet,ishook);//HOOKScreenToClient
		}
		if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||TSRuntime::pData->iskeyboardhook||ishook==false)
		{
			GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJMouseWin7offse,(DWORD)MyGetDeviceStateMouse,(DWORD)GetDeviceStateMouse,HookGetDeviceStateMouseRet,ishook);////HOOK:dinput8.dll的GetDeviceState函数
			GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJkeyboardWin7offse,(DWORD)MyGetDeviceStateKbd,(DWORD)GetDeviceStateKbd,HookGetDeviceStateKbdRet,ishook);
			//MessageBox(NULL,L"dinput8",L"TS",NULL);
		}
	}
	else if(SySTpye==5)//win8系统
	{
		if(!TSRuntime::IsWin8X64)
		{
			//TSRuntime::add_log( "WIN8X86");					  //WIN8X86
			GetHookModFuntion(L"user32.dll","InternalCallWinProc",USER32InternalCallProcWin8offse,(DWORD)MyInternalCallWinProc,(DWORD)UnhookInternalCallWinProc,HookInternalCallWinProcRet,ishook);//HOOK:系统下的:USER32.dll的InternalCallWinProc函数
		}
		else //win8x64
		{
			//TSRuntime::add_log( "win8x64");
			IsWin8X64=true;
			GetHookModFuntion(L"user32.dll","InternalCallWinProc",USER32InternalCallProcWin8x64offse,(DWORD)MyInternalCallWinProc,(DWORD)UnhookInternalCallWinProc,HookInternalCallWinProcRet,ishook);//HOOK:系统下的:USER32.dll的InternalCallWinProc函数	 //
		}

		if((TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ispichook)&&UnhookPresent==false)
		{
			if(UnhookPresent==false&&DxdeviceAddr==0)
				GetHookModFuntion(L"d3d9.dll","Present",D3D9PresentWin8offse,(DWORD)MyD3D9Present,(DWORD)D3D9Present,HookD3D9PresentRet,ishook);
			else if(ishook==true)
			{
				*(DWORD*)DxdeviceAddr=(DWORD)MyD3D9Present;//把真正的函数地址修改到我们的函数地址,
				UnhookPresent=true;
			}
		}

		if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||ishook==false)
		{
			GetHookModFuntion(L"user32.dll","ScreenToClient",0,(DWORD)MyScreenToClient,(DWORD)UnhookScreenToClientRet,HookScreenToClientRet,ishook);//HOOKScreenToClient
		}
		if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||TSRuntime::pData->iskeyboardhook||ishook==false)
		{
			GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJMouseWin8offse,(DWORD)MyGetDeviceStateMouse,(DWORD)GetDeviceStateMouse,HookGetDeviceStateMouseRet,ishook);////HOOK:dinput8.dll的GetDeviceState函数
			GetHookModFuntion(L"dinput8.dll","GetDeviceState",GetDeviceStateOBJkeyboardWin8offse,(DWORD)MyGetDeviceStateKbd,(DWORD)GetDeviceStateKbd,HookGetDeviceStateKbdRet,ishook);
		}
	}
	if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||TSRuntime::pData->iskeyboardhook||ishook==false)
	{
		GetHookModFuntion(L"user32.dll","GetActiveWindow",0,(DWORD)MyGetActiveWindow,(DWORD)UnhookGetActiveWindow,HookGetActiveWindowRet,ishook);
		GetHookModFuntion(L"user32.dll","GetFocus",0,(DWORD)MyGetFocus,(DWORD)UnhookGetFocus,HookGetFocusRet,ishook);
		GetHookModFuntion(L"user32.dll","IsWindowEnabled",0,(DWORD)MyIsWindowEnabled,(DWORD)USER32IsWindowEnabled,HookIsWindowEnabledRet,ishook);
		GetHookModFuntion(L"user32.dll","GetForegroundWindow",0,(DWORD)MyGetForegroundWindow,(DWORD)UnhookGetForegroundWindow,HookGetForegroundWindowRet,ishook);
		GetHookModFuntion(L"user32.dll","GetTopWindow",0,(DWORD)MyGetTopWindow,(DWORD)UnhookGetTopWindow,HookGetTopWindowRet,ishook);
		GetHookModFuntion(L"user32.dll","WindowFromPoint",0,(DWORD)MyWindowFromPoint,(DWORD)UnhookWindowFromPoint,HookWindowFromPointRet,ishook);
		GetHookModFuntion(L"user32.dll","GetAsyncKeyState",0,(DWORD)MyGetAsyncKeyState,(DWORD)USER32GetAsyncKeyState,HookGetAsyncKeyStateRet,ishook);
		GetHookModFuntion(L"user32.dll","GetRawInputData",0,(DWORD)MyGetRawInputData,(DWORD)USER32GetRawInputData,HookGetRawInputDataRet,ishook);
		//GetHookModFuntion(L"user32.dll","GetCapture",0,(DWORD)MyGetCapture,(DWORD)USER32GetCapture,HookGetCaptureRet,ishook);
	}
	if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->ismousehook||ishook==false)
	{
		GetHookModFuntion(L"user32.dll","GetCursorPos",0,(DWORD)MyGetCursorPos,(DWORD)USER32GetCursorPos,HookGetCursorPosRet,ishook);//HOOKGetCursorPos
		GetHookModFuntion(L"user32.dll","SetCursor",0,(DWORD)MySetCursor,(DWORD)USER32SetCursor,HookSetCursorRet,ishook);
		GetHookModFuntion(L"user32.dll","GetMessagePos",0,(DWORD)MyGetMessagePos,(DWORD)UnhookGetMessagePos,HookGetMessagePosRet,ishook);
		GetHookModFuntion(L"user32.dll","SetCursorPos",0,(DWORD)MySetCursorPos,(DWORD)USER32SetCursorPos,HookSetCursorPosRet,ishook);
	}
	if(TSRuntime::IsVirtualProtectEx||TSRuntime::pData->iskeyboardhook||ishook==false)
	{
		GetHookModFuntion(L"user32.dll","GetKeyState",0,(DWORD)MyGetKeyState,(DWORD)USER32GetKeyState,HookGetKeyStateRet,ishook);  
		GetHookModFuntion(L"user32.dll","GetKeyboardState",0,(DWORD)MyGetKeyboardState,(DWORD)UnhookGetKeyboardState,HookGetKeyboardStateRet,ishook);
	}
	GetHookModFuntion(L"Imm32.dll","ImmGetCompositionStringA",0,(DWORD)MyIMM32ImmGetCompositionStringA,(DWORD)IMM32ImmGetCompositionStringA,HookImmGetCompositionStringARet,ishook);
	GetHookModFuntion(L"Imm32.dll","ImmGetCompositionStringW",0,(DWORD)MyIMM32ImmGetCompositionStringW,(DWORD)IMM32ImmGetCompositionStringW,HookImmGetCompositionStringWRet,ishook); 
	if(TSRuntime::pData)
	{
		TSRuntime::pData->isInject=true;
		TSRuntime::pData->ispichook=false;
		g_InjectHWND=TSRuntime::pData->InjectHwnd;
	}
	//TSRuntime::ZwProtectVirtualMemoryRet
	//TSRuntime::add_log( "HookandUnhookAPI-end,TSRuntime::IsVirtualProtectEx:%d",TSRuntime::IsVirtualProtectEx);

}

void ReadShareMemoryThreadFunc(void *para)//创建读写共享数据线程
{
	g_Unbind=false;
	memset(TSRuntime::SetPath,0,MAX_PATH);
	//g_currentHwnd=TSRuntime::GetProcessMainWnd(::GetCurrentProcessId());   //获取自身进程ID的窗口句柄
	g_currentHwnd=TSRuntime::pData->m_hwnd;
	//是否开启dxkmprotect盾,启动不同的钩子
	if(TSRuntime::pData->Isdxkmprotect)
		VIPHookandUnhookAPI(para);
	else
		HookandUnhookAPI(para);
}