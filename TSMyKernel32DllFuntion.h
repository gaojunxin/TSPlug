#pragma once
#include "TSMemoryAPI.h"

typedef HANDLE(__stdcall  *myOpenProcess)(   DWORD fdwAccess,   BOOL fInherit,   DWORD IDProcess);


typedef bool (__stdcall  *  myVirtualProtect)(LPVOID lpAddress,DWORD dwSize,DWORD flNewProtect, PDWORD lpflOldProtect); 

typedef BOOL (__stdcall  * myVirtualProtectEx)( HANDLE hProcess, LPVOID lpAddress,SIZE_T dwSize,DWORD flNewProtect, PDWORD lpflOldProtect);

typedef BOOL(__stdcall *myReadProcessMemory)(   HANDLE hProcess,   LPCVOID lpBaseAddress, 
					   LPVOID lpBuffer,   DWORD nSize,   LPDWORD lpNumberOfBytesRead );



typedef bool (__stdcall  *  myWriteProcessMemory)( HANDLE hProcess, 
												  LPVOID lpBaseAddress, 
												  LPVOID lpBuffer, 
												  DWORD nSize, 
												  LPDWORD lpNumberOfBytesWritten); 

typedef BOOL (__stdcall  *  myTerminateProcess)(  HANDLE hProcess,   DWORD uExitCode);

typedef HANDLE (__stdcall  *  myCreateRemoteThread)( __in          HANDLE hProcess,
													__in          LPSECURITY_ATTRIBUTES lpThreadAttributes,
													__in          SIZE_T dwStackSize,
													__in          LPTHREAD_START_ROUTINE lpStartAddress,
													__in          LPVOID lpParameter,
													__in          DWORD dwCreationFlags,
													__out         LPDWORD lpThreadId
													);

typedef BOOL (__stdcall  * myDeviceIoControl)(  HANDLE hDevice,   DWORD dwIoControlCode, 
												LPVOID lpInBuffer,   DWORD nInBufferSize,   LPVOID lpOutBuffer, 
												 DWORD nOutBufferSize,   LPDWORD lpBytesReturned,   LPOVERLAPPED lpOverlapped);

typedef NTSTATUS (__stdcall  *myZwOpenProcess) (__out PHANDLE  ProcessHandle,__in ACCESS_MASK  DesiredAccess,
												__in POBJECT_ATTRIBUTES  ObjectAttributes,__in_opt PCLIENT_ID  ClientId);

typedef DWORD (__stdcall  *myGetTickCount)(); 

typedef void (__stdcall  *mySleep)(DWORD dwMilliseconds); 



bool InitialMykernel32Dll();
bool InitialMykernel32DllWIN7();


