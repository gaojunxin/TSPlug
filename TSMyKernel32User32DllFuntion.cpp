#include "StdAfx.h"
#include "TSMyUser32DllFuntion.h"
#include "TSMyKernel32DllFuntion.h"
#include "TSRuntime.h"

myWindowFromPoint  my_WindowFromPoint=NULL;
myGetWindowLong my_GetWindowLong=NULL;
myFindWindowEx my_FindWindowEx=NULL;
mySendMessage my_SendMessage=NULL;
myPostMessage my_PostMessage=NULL;
myGetWindowThreadProcessId my_GetWindowThreadProcessId=NULL;
myFindWindow my_FindWindow=NULL;
myGetWindow my_GetWindow=NULL;
myScreenToClient my_ScreenToClient=NULL;
myClientToScreen my_ClientToScreen=NULL;
myGetCursorPos my_GetCursorPos=NULL;
myIsWindow   my_IsWindow=NULL;

myZwOpenProcess my_ZwOpenProcess=NULL;
myOpenProcess my_OpenProcess=NULL;
myVirtualProtect my_VirtualProtect=NULL;
myVirtualProtectEx my_VirtualProtectEx=NULL;
myReadProcessMemory my_ReadProcessMemory=NULL;
myWriteProcessMemory my_WriteProcessMemory=NULL;
myTerminateProcess my_TerminateProcess=NULL;
myCreateRemoteThread my_CreateRemoteThread=NULL;
myDeviceIoControl my_DeviceIoControl=NULL;
myGetTickCount my_myGetTickCount=NULL;
mySleep my_sleep=NULL;

bool InitialMykernel32DllWIN7()
	{
		bool bret=false;
		//my_ZwOpenProcess
		HMODULE hmod=::LoadLibrary(L"ntdll.dll");
		if(hmod)
			{
				my_ZwOpenProcess=(myZwOpenProcess)::GetProcAddress(hmod,"ZwOpenProcess");
				if(my_ZwOpenProcess!=NULL)
				   bret=true;
			}

		return bret;

	}

bool InitialMykernel32Dll()
	{
	bool bret=false;

	my_OpenProcess=(myOpenProcess)TSRuntime::GetPassR3HookFuntionAddr("kernel32.dll","OpenProcess");
	my_VirtualProtect=(myVirtualProtect)TSRuntime::GetPassR3HookFuntionAddr("kernel32.dll","VirtualProtect");
	my_ReadProcessMemory=(myReadProcessMemory)TSRuntime::GetPassR3HookFuntionAddr("kernel32.dll","ReadProcessMemory");
	my_WriteProcessMemory=(myWriteProcessMemory)TSRuntime::GetPassR3HookFuntionAddr("kernel32.dll","WriteProcessMemory");
	my_TerminateProcess=(myTerminateProcess)TSRuntime::GetPassR3HookFuntionAddr("kernel32.dll","TerminateProcess");
	my_VirtualProtectEx=(myVirtualProtectEx)TSRuntime::GetPassR3HookFuntionAddr("kernel32.dll","VirtualProtectEx");
	my_CreateRemoteThread=(myCreateRemoteThread)TSRuntime::GetPassR3HookFuntionAddr("kernel32.dll","CreateRemoteThread");
	my_DeviceIoControl=(myDeviceIoControl)TSRuntime::GetPassR3HookFuntionAddr("kernel32.dll","DeviceIoControl");
	my_myGetTickCount=(myGetTickCount)TSRuntime::GetPassR3HookFuntionAddr("kernel32.dll","GetTickCount");
	my_sleep=(mySleep)TSRuntime::GetPassR3HookFuntionAddr("kernel32.dll","Sleep");
	//my_ScreenToClient=(myScreenToClient)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","ScreenToClient");
	//my_GetWindow=(myGetWindow)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","GetWindow");
	//my_FindWindow=(myFindWindow)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","FindWindowW");
	//my_WindowFromPoint=(myWindowFromPoint)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","WindowFromPoint");
	//my_GetWindowLong=(myGetWindowLong)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","GetWindowLongW");
	//my_FindWindowEx=(myFindWindowEx)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","FindWindowExW");

	//TSRuntime::add_log( "my_GetCursorPos:%d,my_ClientToScreen:%d,my_ScreenToClient:%d,my_GetWindow:%d,my_FindWindow:%d,my_WindowFromPoint:%d,my_GetWindowLong:%d,my_FindWindowEx:%d",my_GetCursorPos,my_ClientToScreen
	//	,my_ScreenToClient,my_GetWindow,my_FindWindow,my_WindowFromPoint,my_GetWindowLong,my_FindWindowEx);
	if(my_OpenProcess==NULL
		||my_VirtualProtect==NULL
		||my_WriteProcessMemory==NULL
		||my_ReadProcessMemory==NULL
		||my_VirtualProtectEx==NULL
		||my_TerminateProcess==NULL
		||my_CreateRemoteThread==NULL)
		return bret;
	else
		bret=true;

	return bret;
	}


bool InitialMyUser32Dll()
	{
	bool bret=false;
	my_GetCursorPos=(myGetCursorPos)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","GetCursorPos");
	my_ClientToScreen=(myClientToScreen)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","ClientToScreen");
	my_ScreenToClient=(myScreenToClient)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","ScreenToClient");
	my_GetWindow=(myGetWindow)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","GetWindow");
	my_FindWindow=(myFindWindow)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","FindWindowW");
	my_WindowFromPoint=(myWindowFromPoint)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","WindowFromPoint");
	my_GetWindowLong=(myGetWindowLong)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","GetWindowLongW");
	my_FindWindowEx=(myFindWindowEx)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","FindWindowExW");
	my_SendMessage=(mySendMessage)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","SendMessageW");
	my_PostMessage=(myPostMessage)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","PostMessageW");
	my_GetWindowThreadProcessId=(myGetWindowThreadProcessId)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","GetWindowThreadProcessId");
	my_IsWindow=(myIsWindow)TSRuntime::GetPassR3HookFuntionAddr("user32.dll","IsWindow");

	//TSRuntime::add_log( "my_GetCursorPos:%d,my_ClientToScreen:%d,my_ScreenToClient:%d,my_GetWindow:%d,my_FindWindow:%d,my_WindowFromPoint:%d,my_GetWindowLong:%d,my_FindWindowEx:%d",my_GetCursorPos,my_ClientToScreen
	//	,my_ScreenToClient,my_GetWindow,my_FindWindow,my_WindowFromPoint,my_GetWindowLong,my_FindWindowEx);
	if(my_GetCursorPos==NULL
		||my_ClientToScreen==NULL
		||my_ScreenToClient==NULL
		||my_GetWindow==NULL
		||my_FindWindow==NULL
		||my_WindowFromPoint==NULL
		||my_GetWindowLong==NULL
		||my_FindWindowEx==NULL
		||my_SendMessage==NULL
		||my_PostMessage==NULL)
		return bret;
	else
		bret=true;

	return bret;
	}