#pragma once

#include <Windows.h>
#include "TSRuntime.h"

//XP系统下挂钩位置偏移
//77D184AE __stdcall NtUserCallOneParam(x, x)
#define	 NtUserCallOneParam_XPOffset  0x84AE
//77D298A6 __stdcall NtUserGetThreadState(x)
#define	 NtUserGetThreadState_XPOffset  0x198A6
//77D29F5E __stdcall NtUserGetKeyState(x)
#define	 NtUserGetKeyState_XPOffset  0x19F5E
//77D2A822 __stdcall NtUserGetAsyncKeyState
#define	 NtUserGetAsyncKeyState_XPOffset  0x1A822
//77D2A93A __stdcall NtUserCallTwoParam(x, x, x)
#define	 NtUserCallTwoParam_XPOffset  0x1A93A

//77D184AE __stdcall NtUserCallOneParam(x, x)
#define	 NtUserCallOneParam_Win2003Offset  0x1b626
//77D298A6 __stdcall NtUserGetThreadState(x)
#define	 NtUserGetThreadState_Win2003Offset  0x1cb3c
//77D29F5E __stdcall NtUserGetKeyState(x)
#define	 NtUserGetKeyState_Win2003Offset  0x16348
//77D2A822 __stdcall NtUserGetAsyncKeyState
#define	 NtUserGetAsyncKeyState_Win2003Offset  0xED3A
//77D2A93A __stdcall NtUserCallTwoParam(x, x, x)
#define	 NtUserCallTwoParam_Win2003Offset  0x755F

//WIN7X86系统下挂钩位置偏移
//77D184AE __stdcall NtUserCallOneParam(x, x)
#define	 NtUserCallOneParam_WIN7X86Offset  0x1D8F7
//77D298A6 __stdcall NtUserGetThreadState(x)
#define	 NtUserGetThreadState_WIN7X86Offset  0x13A20
//77D29F5E __stdcall NtUserGetKeyState(x)
#define	 NtUserGetKeyState_WIN7X86Offset  0x12D17
//77D2A822 __stdcall NtUserGetAsyncKeyState
#define	 NtUserGetAsyncKeyState_WIN7X86Offset  0xA2F4
//77D2A93A __stdcall NtUserCallTwoParam(x, x, x)
#define	 NtUserCallTwoParam_WIN7X86Offset  0xDBCC

//WIN8X86系统下挂钩位置偏移
//77D184AE __stdcall NtUserCallOneParam(x, x)
#define	 NtUserCallOneParam_WIN8X86Offset  0x7470
//77D2A93A __stdcall NtUserCallTwoParam(x, x, x)
#define	 NtUserCallTwoParam_WIN8X86Offset  0x8FA7
//77D298A6 __stdcall NtUserGetThreadState(x)
#define	 NtUserGetThreadState_WIN8X86Offset  0xb9d7
//77D29F5E __stdcall NtUserGetKeyState(x)
#define	 NtUserGetKeyState_WIN8X86Offset  0x24F08
//77D2A822 __stdcall NtUserGetAsyncKeyState
#define	 NtUserGetAsyncKeyState_WIN8X86Offset  0xC08D


//WIN7X64系统下挂钩位置偏移
//77D184AE __stdcall NtUserCallOneParam(x, x)
//#define	 NtUserCallOneParam_WIN7X64Offset  0x160cd
//77D298A6 __stdcall NtUserGetThreadState(x)
#define	 NtUserGetThreadState_WIN7X64Offset  0x20DCE
//77D29F5E __stdcall NtUserGetKeyState(x)
#define	 NtUserGetKeyState_WIN7X64Offset  0x229AE
//77D2A822 __stdcall NtUserGetAsyncKeyState
#define	 NtUserGetAsyncKeyState_WIN7X64Offset  0x3EC48
//77D2A93A __stdcall NtUserCallTwoParam(x, x, x)
#define	 NtUserCallTwoParam_WIN7X64Offset  0x17426

//WIN8X64系统下挂钩位置偏移
//77D184AE __stdcall NtUserCallOneParam(x, x)
//#define	 NtUserCallOneParam_WIN7X64Offset  0x160cd
//77D298A6 __stdcall NtUserGetThreadState(x)
#define	 NtUserGetThreadState_WIN8X64Offset  0x8040
//77D29F5E __stdcall NtUserGetKeyState(x)
#define	 NtUserGetKeyState_WIN8X64Offset  0x9c12
//77D2A822 __stdcall NtUserGetAsyncKeyState
#define	 NtUserGetAsyncKeyState_WIN8X64Offset  0x17ad5
//77D2A93A __stdcall NtUserCallTwoParam(x, x, x)
#define	 NtUserCallTwoParam_WIN8X64Offset  0x8012

DWORD   EAX_NtUserCallOneParam=0;
DWORD   EAX_NtUserGetThreadState=0;
DWORD   EAX_NtUserGetKeyState=0;
DWORD   EAX_NtUserGetAsyncKeyState=0;
DWORD   EAX_NtUserCallTwoParam=0;

DWORD   NtUserCallOneParamRet=0;
DWORD   NtUserGetThreadStateRet=0;
DWORD   NtUserGetKeyStateRet=0;
DWORD   NtUserGetAsyncKeyStateRet=0;
DWORD   NtUserCallTwoParamRet=0;

BYTE NtUserCallOneParamUnhookByte[5]={0};
BYTE NtUserGetThreadStateUnhookByte[5]={0};
BYTE NtUserGetKeyStateUnhookByte[5]={0};
BYTE NtUserGetAsyncKeyStateUnhookByte[5]={0};
BYTE NtUserCallTwoParamUnhookByte[5]={0};

void VIPHookandUnhookAPI(void *para);

__declspec(naked) ULONG  __stdcall NtUserCallOneParam(DWORD Param, DWORD Routine)
	{
	_asm{
			mov eax,EAX_NtUserCallOneParam
			//mov eax,0x1143
			jmp NtUserCallOneParamRet
			ret
		}	
	}

__declspec(naked) ULONG_PTR  __stdcall NtUserGetThreadState(DWORD 	Routine)
	{
	_asm{
			mov eax,EAX_NtUserGetThreadState
			jmp NtUserGetThreadStateRet
			ret
		}	
	}

__declspec(naked) SHORT  __stdcall NtUserGetKeyState(int nVirtKey)
	{
	_asm{
			mov eax,EAX_NtUserGetKeyState
			jmp NtUserGetKeyStateRet
			ret
		}	
	}

__declspec(naked) SHORT  __stdcall NtUserGetAsyncKeyState(int nVirtKey)
	{
	_asm{
			mov eax,EAX_NtUserGetAsyncKeyState
			jmp NtUserGetAsyncKeyStateRet
			ret
		}	
	}

__declspec(naked) ULONG  __stdcall NtUserCallTwoParam(  DWORD Param1,DWORD Param2,DWORD Routine)
	{
	_asm{
			mov eax,EAX_NtUserCallTwoParam
			jmp NtUserCallTwoParamRet
			ret
		}	
	}



