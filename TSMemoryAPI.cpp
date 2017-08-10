/*
本源码由TC简单软件科技有限公司开源,功能可以自由修改、发布、
长沙简单软件科技有限公司对于源码不做后期维护,,请大家在使用过程中遵循开源协议
*/
#include "StdAfx.h"
#include "TSMemoryAPI.h"
#include <Tlhelp32.h>
#include <psapi.h> 
#include "TSRuntime.h"
#include "TSMyUser32DllFuntion.h"
#include "TSMyKernel32DllFuntion.h"
//#include "TSVIPHookFuntion.h"
#pragma comment ( lib, "psapi.lib" )

extern myOpenProcess my_OpenProcess;
extern myGetWindowThreadProcessId my_GetWindowThreadProcessId;
extern myReadProcessMemory my_ReadProcessMemory;
extern myWriteProcessMemory my_WriteProcessMemory;
extern myTerminateProcess my_TerminateProcess;
extern myVirtualProtect my_VirtualProtect;
extern myVirtualProtectEx my_VirtualProtectEx;
extern myCreateRemoteThread my_CreateRemoteThread;
extern myZwOpenProcess	my_ZwOpenProcess;

typedef LONG (WINAPI *PROCNTQSIP)(HANDLE,UINT,PVOID,ULONG,PULONG);
PROCNTQSIP NtQueryInformationProcess;


//NtReadVirtualMemory(
//					IN HANDLE               ProcessHandle,
//					IN PVOID                BaseAddress,
//					OUT PVOID               Buffer,
//					IN ULONG                NumberOfBytesToRead,
//					OUT PULONG              NumberOfBytesReaded OPTIONAL );


extern DWORD Ntdll_ProtectVirtual;
extern DWORD Ntdll_ProtectVirtualEax;

extern DWORD ZwWriteVirtualMemoryRet;
extern DWORD ZwWriteVirtualMemoryEax;

DWORD GetWindowThreadProcessIdRet=0;
__declspec(naked) DWORD WINAPI My_GetWindowThreadProcessId(  HWND hWnd,   LPDWORD lpdwProcessId )
{
	_asm
	{
			mov edi,edi
			push ebp
			mov ebp,esp
			jmp GetWindowThreadProcessIdRet;
	}
}

DWORD ZwOpenProcessRet=0;
DWORD ZwOpenProcessEax=0;
__declspec(naked) NTSTATUS WINAPI My_ZwOpenProcess(
											__out PHANDLE  ProcessHandle,
											__in ACCESS_MASK  DesiredAccess,
											__in POBJECT_ATTRIBUTES  ObjectAttributes,
											__in_opt PCLIENT_ID  ClientId
											)
{
	_asm
	{
		//MOV EAX,0x23  //win7
		mov eax,ZwOpenProcessEax;
		jmp ZwOpenProcessRet;
	}
}

HANDLE My_OpenProcess(int proid)
{
	HANDLE ProcessHandle=(HANDLE)0;
	OBJECT_ATTRIBUTES ObjectAttribute={sizeof(OBJECT_ATTRIBUTES), 0,NULL,NULL};
	ObjectAttribute.Attributes=0;
	CLIENT_ID ClientIds;
	ClientIds.UniqueProcess=(HANDLE)proid;
	ClientIds.UniqueThread=(HANDLE)0;
	My_ZwOpenProcess(&ProcessHandle,PROCESS_ALL_ACCESS,&ObjectAttribute,&ClientIds);
	ProcessHandle;

	return ProcessHandle;
}

int GetProcessNumber()//获取CPU个数 ,CPU核数决定线程个数
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return (int)info.dwNumberOfProcessors;
}

TSMemoryAPI::TSMemoryAPI(void)
{
	nPid=0;
	retstringlen=0;
	asmcodearry.clear();
	memset(Asmcalladdr,0,MAX_PATH);
	allocatememory=NULL;

	int sysvion=TSRuntime::InitialWindowsVersion();

	if(GetWindowThreadProcessIdRet==0)
	{
		DWORD addr=(DWORD)GetProcAddress(GetModuleHandle(L"user32.dll"),"GetWindowThreadProcessId");
		GetWindowThreadProcessIdRet=addr+5;
	}

	if(ZwWriteVirtualMemoryRet==0)
	{
		DWORD addr=(DWORD)GetProcAddress(GetModuleHandle(L"ntdll.dll"),"ZwWriteVirtualMemory");
		ZwWriteVirtualMemoryRet=addr+5;
		if(sysvion==4&&TSRuntime::IsWin7X64)//win7X64
			ZwWriteVirtualMemoryEax=0x37;
		else if(sysvion==4)//win7x86
			ZwWriteVirtualMemoryEax=0x18F;
		else if(sysvion==1)//WINXP
			ZwWriteVirtualMemoryEax=0x115;
		else if(sysvion==2)//WIN2003
			ZwWriteVirtualMemoryEax=0x11f;
		else if(sysvion==5&&TSRuntime::IsWin8X64)
			ZwWriteVirtualMemoryEax=0x38;
		else if(sysvion==5)//win8X86
			ZwWriteVirtualMemoryEax=0x2;
	}

	if(Ntdll_ProtectVirtual==0)
	{
		DWORD addr=(DWORD)GetProcAddress(GetModuleHandle(L"ntdll.dll"),"ZwProtectVirtualMemory");
		Ntdll_ProtectVirtual=addr+5;
		if(sysvion==4&&TSRuntime::IsWin7X64)//win7X64
			Ntdll_ProtectVirtualEax=0x4d;
		else if(sysvion==4)//win7x86
			Ntdll_ProtectVirtualEax=0xd7;
		else if(sysvion==1)//WINXP
			Ntdll_ProtectVirtualEax=0x89;
		else if(sysvion==2)//WIN2003
			Ntdll_ProtectVirtualEax=0x8f;
		else if(sysvion==5&&TSRuntime::IsWin8X64)
			Ntdll_ProtectVirtualEax=0x4e;
		else if(sysvion==5)//win8X86
			Ntdll_ProtectVirtualEax=0xC3;
	}
	if(ZwOpenProcessRet==0)
	{
		DWORD addr=(DWORD)GetProcAddress(GetModuleHandle(L"ntdll.dll"),"ZwOpenProcess");
		ZwOpenProcessRet=addr+5;
		if(sysvion==4&&TSRuntime::IsWin7X64)//win7X64
			ZwOpenProcessEax=0x23;
		else if(sysvion==4)//win7x86
			ZwOpenProcessEax=0xBe;
		else if(sysvion==1)//WINXP
			ZwOpenProcessEax=0x7a;
		else if(sysvion==2)//WIN2003
			ZwOpenProcessEax=0X80;
		else if(sysvion==5&&TSRuntime::IsWin8X64)
			ZwOpenProcessEax=0X24;
		else if(sysvion==5)//win8X86
			ZwOpenProcessEax=0XDD;

	}

	// m_mutex=mm_mutex;
}

TSMemoryAPI::~TSMemoryAPI(void)
{

}

void FindDataThread(void *para)
{
	PFindDataInfo info=(PFindDataInfo)para;

	BYTE buffData[4096]={0};
	int retstrlen=0;
	//DWORD count=0;
	DWORD dwRead=0;
	DWORD ibegin=0;
	if(info->FindDataType==FINDDATATYPE_FINDFLOATEX) //FindFloatEx
	{
		for(int n=0;n<info->dwncount;n++)
		{
			EnterCriticalSection(&info->m_mutex);//临界区保护多线程
			for(int k=0;k<info->dwncount;k++)
			{
				if(info->bfindindex[k]==true)
					n=k;
				else
				{
					info->bfindindex[k]=true;
					break;
				}
			}
			LeaveCriticalSection(&info->m_mutex);//临界区保护多线程
			int size=info->dwend[n]-info->dwbegin[n];
			if(size<=4096)
			{
				ibegin=info->dwbegin[n];
				::memset(buffData,0,size);
				if(ReadProcessMemory(info->hprocess,LPVOID(info->dwbegin[n]),buffData,size,&dwRead))
				{
					int i=0;
					for(;i<(int)dwRead;i++)
					{
						float *float_value=(float *)&buffData[i];
						if(*float_value>=info->float_value_min&&*float_value<=info->float_value_max)
						{
							EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
							if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
							{

								wchar_t inx[10]={0};
								if(retstrlen==0)
									retstrlen=wcslen(info->retstr);
								if(retstrlen==0)
									swprintf(inx,L"%X",(ibegin+i));
								else
									swprintf(inx,L"|%X",(ibegin+i));
								wcscat(info->retstr,inx);
							}
							else
							{
								LeaveCriticalSection(&info->m_mutex);
								return;
							}
							info->count++;
							LeaveCriticalSection(&info->m_mutex);
						}
					}

				}
			}
			else
			{
				ibegin=info->dwbegin[n];
				int sizen=4096;
				while(ibegin<info->dwend[n]&&info->dwend[n]>0)
				{
					if(ReadProcessMemory(info->hprocess,LPVOID(ibegin),buffData,sizen,&dwRead))
					{
						int i=0;
						for(;i<(int)dwRead;i++)
						{
							float *float_value=(float *)&buffData[i];
							if(*float_value>=info->float_value_min&&*float_value<=info->float_value_max)
							{
								EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
								if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
								{
									wchar_t inx[10]={0};
									if(retstrlen==0)
										retstrlen=wcslen(info->retstr);
									if(retstrlen==0)
										swprintf(inx,L"%X",(ibegin+i));
									else
										swprintf(inx,L"|%X",(ibegin+i));
									wcscat(info->retstr,inx);
								}
								else
								{
									LeaveCriticalSection(&info->m_mutex);
									return;
								}
								info->count++;
								LeaveCriticalSection(&info->m_mutex);
							}
						}

					}
					if(info->dwend[n]>=ibegin+4096)
						ibegin=ibegin+4096;
					else
					{
						sizen=info->dwend[n]-ibegin;
						ibegin=ibegin+4096;
						::memset(buffData,0,sizen);
					}
				}
			}
		}
	}
	else if(info->FindDataType==FINDDATATYPE_FINDDOUBLEEX)	//FindDoubleEx
	{
		for(int n=0;n<info->dwncount;n++)
		{
			EnterCriticalSection(&info->m_mutex);//临界区保护多线程
			for(int k=0;k<info->dwncount;k++)
			{
				if(info->bfindindex[k]==true)
					n=k;
				else
				{
					info->bfindindex[k]=true;
					break;
				}
			}
			LeaveCriticalSection(&info->m_mutex);//临界区保护多线程
			int size=info->dwend[n]-info->dwbegin[n];
			if(size<=4096)
			{
				ibegin=info->dwbegin[n];
				::memset(buffData,0,size);
				if(ReadProcessMemory(info->hprocess,LPVOID(ibegin),buffData,size,&dwRead))
				{
					int i=0;
					for(;i<(int)dwRead;i++)
					{
						double *double_value=(double *)&buffData[i];
						if(*double_value>=info->double_value_min&&*double_value<=info->double_value_max)
						{
							EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
							if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
							{
								wchar_t inx[10]={0};
								if(retstrlen==0)
									retstrlen=wcslen(info->retstr);
								if(retstrlen==0)
									swprintf(inx,L"%X",(ibegin+i));
								else
									swprintf(inx,L"|%X",(ibegin+i));
								wcscat(info->retstr,inx);
								//swprintf(info->retstr,L"%s|%X",info->retstr,(ibegin+i));
							}
							else
							{
								LeaveCriticalSection(&info->m_mutex);
								return;
							}
							info->count++;
							LeaveCriticalSection(&info->m_mutex);
						}
					}

				}
			}
			else
			{
				ibegin=info->dwbegin[n];
				int sizen=4096;
				while(ibegin<info->dwend[n]&&info->dwend[n]>0)
				{
					if(ReadProcessMemory(info->hprocess,LPVOID(ibegin),buffData,sizen,&dwRead))
					{
						int i=0;
						for(;i<(int)dwRead;i++)
						{
							double *double_value=(double *)&buffData[i];
							if(*double_value>=info->double_value_min&&*double_value<=info->double_value_max)
							{
								EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
								if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
								{
									wchar_t inx[10]={0};
									if(retstrlen==0)
										retstrlen=wcslen(info->retstr);
									if(retstrlen==0)
										swprintf(inx,L"%X",(ibegin+i));
									else
										swprintf(inx,L"|%X",(ibegin+i));
									wcscat(info->retstr,inx);
									//swprintf(info->retstr,L"%s|%X",info->retstr,(ibegin+i));
								}
								else
								{
									LeaveCriticalSection(&info->m_mutex);
									return;
								}
								info->count++;
								LeaveCriticalSection(&info->m_mutex);
							}
						}

					}
					if(info->dwend[n]>=ibegin+4096)
						ibegin=ibegin+4096;
					else
					{
						sizen=info->dwend[n]-ibegin;
						ibegin=ibegin+4096;
						::memset(buffData,0,sizen);
					}
				}
			}
		}
	}
	else if(info->FindDataType==FINDDATATYPE_FINDINTEX)//FindIntEx
	{
		for(int n=0;n<info->dwncount;n++)
		{
			EnterCriticalSection(&info->m_mutex);//临界区保护多线程
			for(int k=0;k<info->dwncount;k++)
			{
				if(info->bfindindex[k]==true)
					n=k;
				else
				{
					info->bfindindex[k]=true;
					break;
				}
			}
			LeaveCriticalSection(&info->m_mutex);//临界区保护多线程
			int size=info->dwend[n]-info->dwbegin[n];
			if(size<=4096)
			{
				ibegin=info->dwbegin[n];
				::memset(buffData,0,size);
				if(ReadProcessMemory(info->hprocess,LPVOID(ibegin),buffData,size,&dwRead))
				{
					int i=0;
					for(;i<(int)dwRead;i++)
					{
						if(info->FindIntType==0)//0 : 32位
						{
							int *value=(int *)&buffData[i];
							if(*value>=info->int_value_min&&*value<=info->int_value_max)
							{
								EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
								if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
								{
									wchar_t inx[10]={0};
									if(retstrlen==0)
										retstrlen=wcslen(info->retstr);
									if(retstrlen==0)
										swprintf(inx,L"%X",(ibegin+i));
									else
										swprintf(inx,L"|%X",(ibegin+i));
									wcscat(info->retstr,inx);
									//swprintf(info->retstr,L"%s|%X",info->retstr,(ibegin+i));
								}
								else
								{
									LeaveCriticalSection(&info->m_mutex);
									return;
								}
								info->count++;
								LeaveCriticalSection(&info->m_mutex);
							}
						}
						else  if(info->FindIntType==1)  //1 : 16 位
						{
							short *value=(short *)&buffData[i];
							if(*value>=info->int_value_min&&*value<=info->int_value_max)
							{
								EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
								if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
								{
									wchar_t inx[10]={0};
									if(retstrlen==0)
										retstrlen=wcslen(info->retstr);
									if(retstrlen==0)
										swprintf(inx,L"%X",(ibegin+i));
									else
										swprintf(inx,L"|%X",(ibegin+i));
									wcscat(info->retstr,inx);
									//swprintf(info->retstr,L"%s|%X",info->retstr,(ibegin+i));
								}
								else
								{
									LeaveCriticalSection(&info->m_mutex);
									return;
								}
								info->count++;
								LeaveCriticalSection(&info->m_mutex);
							}
						}
						else if(info->FindIntType==2)//2 : 8位
						{
							if(buffData[i]>=info->int_value_min&&buffData[i]<=info->int_value_max)
							{
								EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
								if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
								{
									wchar_t inx[10]={0};
									if(retstrlen==0)
										retstrlen=wcslen(info->retstr);
									if(retstrlen==0)
										swprintf(inx,L"%X",(ibegin+i));
									else
										swprintf(inx,L"|%X",(ibegin+i));
									wcscat(info->retstr,inx);
									//swprintf(info->retstr,L"%s|%X",info->retstr,(ibegin+i));
								}
								else
								{
									LeaveCriticalSection(&info->m_mutex);
									return;
								}
								info->count++;
								LeaveCriticalSection(&info->m_mutex);
							}
						}
					}

				}
			}
			else
			{
				ibegin=info->dwbegin[n];
				int sizen=4096;
				while(ibegin<info->dwend[n]&&info->dwend[n]>0)
				{
					if(ReadProcessMemory(info->hprocess,LPVOID(ibegin),buffData,sizen,&dwRead))
					{
						int i=0;
						for(;i<(int)dwRead;i++)
						{
							if(info->FindIntType==0)//0 : 32位
							{
								int *value=(int *)&buffData[i];
								if(*value>=info->int_value_min&&*value<=info->int_value_max)
								{
									EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
									if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
									{
										wchar_t inx[10]={0};
										if(retstrlen==0)
											retstrlen=wcslen(info->retstr);
										if(retstrlen==0)
											swprintf(inx,L"%X",(ibegin+i));
										else
											swprintf(inx,L"|%X",(ibegin+i));
										wcscat(info->retstr,inx);
										//swprintf(info->retstr,L"%s|%X",info->retstr,(ibegin+i));
									}
									else
									{
										LeaveCriticalSection(&info->m_mutex);
										return;
									}
									info->count++;
									LeaveCriticalSection(&info->m_mutex);
								}
							}
							else  if(info->FindIntType==1)  //1 : 16 位
							{
								short *value=(short *)&buffData[i];
								if(*value>=info->int_value_min&&*value<=info->int_value_max)
								{
									EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
									if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
									{
										wchar_t inx[10]={0};
										if(retstrlen==0)
											retstrlen=wcslen(info->retstr);
										if(retstrlen==0)
											swprintf(inx,L"%X",(ibegin+i));
										else
											swprintf(inx,L"|%X",(ibegin+i));
										wcscat(info->retstr,inx);
										//swprintf(info->retstr,L"%s|%X",info->retstr,(ibegin+i));
									}
									else
									{
										LeaveCriticalSection(&info->m_mutex);
										return;
									}
									info->count++;
									LeaveCriticalSection(&info->m_mutex);
								}
							}
							else if(info->FindIntType==2)//2 : 8位
							{
								if(buffData[i]>=info->int_value_min&&buffData[i]<=info->int_value_max)
								{
									EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
									if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
									{
										wchar_t inx[10]={0};
										if(retstrlen==0)
											retstrlen=wcslen(info->retstr);
										if(retstrlen==0)
											swprintf(inx,L"%X",(ibegin+i));
										else
											swprintf(inx,L"|%X",(ibegin+i));
										wcscat(info->retstr,inx);
										//swprintf(info->retstr,L"%s|%X",info->retstr,(ibegin+i));
									}
									else
									{
										LeaveCriticalSection(&info->m_mutex);
										return;
									}
									info->count++;
									LeaveCriticalSection(&info->m_mutex);
								}
							}
						}

					}
					if(info->dwend[n]>=ibegin+4096)
						ibegin=ibegin+4096;
					else
					{
						sizen=info->dwend[n]-ibegin;
						ibegin=ibegin+4096;
						::memset(buffData,0,sizen);
					}
				}
			}
		}
	}
	else if(info->FindDataType==FINDDATATYPE_FINDDATAEX)
	{
		int nindex=0;
		for(int n=0;n<info->dwncount;n++)
		{
			EnterCriticalSection(&info->m_mutex);//临界区保护多线程
			for(int k=0;k<info->dwncount;k++)
			{
				if(info->bfindindex[k]==true)
					n=k;
				else
				{
					info->bfindindex[k]=true;
					break;
				}
			}
			LeaveCriticalSection(&info->m_mutex);//临界区保护多线程
			int size=info->dwend[n]-info->dwbegin[n];
			if(size<=4096)
			{
				ibegin=info->dwbegin[n];
				::memset(buffData,0,size);
				if(ReadProcessMemory(info->hprocess,LPVOID(ibegin),buffData,size,&dwRead))
				{
					int i=0;
					for(;i<(int)dwRead;i++)
					{
						if(buffData[i]==info->Finddata[nindex]||info->Finddata[nindex]==256)// unknow=256;//标记??模糊查询
						{
							if(info->FindIntType==(nindex+1))   //匹配成功
							{	
								EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
								if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
								{
									wchar_t inx[10]={0};
									if(retstrlen==0)
										retstrlen=wcslen(info->retstr);
									if(retstrlen==0)
										swprintf(inx,L"%X",(ibegin+i)-nindex);
									else
										swprintf(inx,L"|%X",(ibegin+i)-nindex);
									wcscat(info->retstr,inx);
									//swprintf(info->retstr,L"%s|%X",info->retstr,(ibegin+i)-nindex);
								}
								else
								{
									LeaveCriticalSection(&info->m_mutex);
									return;
								}
								info->count++;
								nindex=0;
								LeaveCriticalSection(&info->m_mutex);
							}
							nindex++;
						}
						else
							nindex=0;
					}

				}
			}
			else
			{
				ibegin=info->dwbegin[n];
				int sizen=4096;
				while(ibegin<info->dwend[n]&&info->dwend[n]>0)
				{
					if(ReadProcessMemory(info->hprocess,LPVOID(ibegin),buffData,sizen,&dwRead))
					{
						int i=0;
						for(;i<(int)dwRead;i++)
						{
							if(buffData[i]==info->Finddata[nindex]||info->Finddata[nindex]==256)// unknow=256;//标记??模糊查询
							{
								if(info->FindIntType==(nindex+1))   //匹配成功
								{	
									EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
									if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
									{
										wchar_t inx[10]={0};
										if(retstrlen==0)
											retstrlen=wcslen(info->retstr);
										if(retstrlen==0)
											swprintf(inx,L"%X",(ibegin+i)-nindex);
										else
											swprintf(inx,L"|%X",(ibegin+i)-nindex);
										wcscat(info->retstr,inx);
										//swprintf(info->retstr,L"%s|%X",info->retstr,(ibegin+i)-nindex);
									}
									else
									{
										LeaveCriticalSection(&info->m_mutex);
										return;
									}
									info->count++;
									nindex=0;
									LeaveCriticalSection(&info->m_mutex);
								}
								nindex++;
							}
							else
								nindex=0;
						}

					}
					if(info->dwend[n]>=ibegin+4096)
						ibegin=ibegin+4096;
					else
					{
						sizen=info->dwend[n]-ibegin;
						ibegin=ibegin+4096;
						::memset(buffData,0,sizen);
					}
				}
			}
		}
	}
	else if(info->FindDataType==FINDDATATYPE_FINDSTRINGEX)
	{
		int nindex=0;
		//////////////////0：ASCCII编码查找  1：Unicode编码查找//////////////////////////////
		if(info->FindIntType == 1)
		{
			USES_CONVERSION;
			int len=wcslen(info->Findstring);
			char chstr[MAX_PATH*2]={0};
			strcpy(chstr,W2A(info->Findstring));
			len=strlen(chstr);;

			for(int n=0;n<info->dwncount;n++)
			{
				EnterCriticalSection(&info->m_mutex);//临界区保护多线程
				for(int k=0;k<info->dwncount;k++)
				{
					if(info->bfindindex[k]==true)
						n=k;
					else
					{
						info->bfindindex[k]=true;
						break;
					}
				}
				LeaveCriticalSection(&info->m_mutex);//临界区保护多线程
				int size=info->dwend[n]-info->dwbegin[n];
				if(size<=4096)
				{
					ibegin=info->dwbegin[n];
					::memset(buffData,0,size);
					if(ReadProcessMemory(info->hprocess,LPVOID(ibegin),buffData,size,&dwRead))
					{
						int i=0;
						for(;i<(int)dwRead;i++)
						{
							if(buffData[i]==chstr[nindex])
							{
								if(len==(nindex+1))   //匹配成功
								{	
									EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
									if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
									{
										wchar_t inx[10]={0};
										if(retstrlen==0)
											retstrlen=wcslen(info->retstr);
										if(retstrlen==0)
											swprintf(inx,L"%X",(ibegin+i)-nindex);
										else
											swprintf(inx,L"|%X",(ibegin+i)-nindex);
										wcscat(info->retstr,inx);
										//swprintf(info->retstr,L"%s|%X",info->retstr,(ibegin+i)-nindex);
									}
									else
									{
										LeaveCriticalSection(&info->m_mutex);
										return;
									}
									info->count++;
									nindex=0;
									LeaveCriticalSection(&info->m_mutex);
								}
								nindex++;
							}
							else 
								nindex=0;
						}

					}
				}
				else
				{
					ibegin=info->dwbegin[n];
					int sizen=4096;
					while(ibegin<info->dwbegin[n]&&info->dwbegin[n]>0)
					{
						if(ReadProcessMemory(info->hprocess,LPVOID(ibegin),buffData,sizen,&dwRead))
						{
							int i=0;
							for(;i<(int)dwRead;i++)
							{
								if(buffData[i]==chstr[nindex])
								{
									if(len==(nindex+1))   //匹配成功
									{	
										EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
										if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
										{
											wchar_t inx[10]={0};
											if(retstrlen==0)
												retstrlen=wcslen(info->retstr);
											if(retstrlen==0)
												swprintf(inx,L"%X",(ibegin+i)-nindex);
											else
												swprintf(inx,L"|%X",(ibegin+i)-nindex);
											wcscat(info->retstr,inx);
											//swprintf(info->retstr,L"%s|%X",info->retstr,(ibegin+i)-nindex);
										}
										else
										{
											LeaveCriticalSection(&info->m_mutex);
											return;
										}
										info->count++;
										nindex=0;
										LeaveCriticalSection(&info->m_mutex);
									}
									nindex++;
								}
								else 
									nindex=0;
							}

						}
						if(info->dwbegin[n]>=ibegin+4096)
							ibegin=ibegin+4096;
						else
						{
							sizen=info->dwbegin[n]-ibegin;
							ibegin=ibegin+4096;
							::memset(buffData,0,sizen);
						}
					}
				}
			}
		}
		else if(info->FindIntType==0)
		{
			int len=wcslen(info->Findstring);
			for(int n=0;n<info->dwncount;n++)
			{
				EnterCriticalSection(&info->m_mutex);//临界区保护多线程
				for(int k=0;k<info->dwncount;k++)
				{
					if(info->bfindindex[k]==true)
						n=k;
					else
					{
						info->bfindindex[k]=true;
						break;
					}
				}
				LeaveCriticalSection(&info->m_mutex);//临界区保护多线程
				int size=info->dwend[n]-info->dwbegin[n];
				if(size<=4096)
				{
					ibegin=info->dwbegin[n];
					::memset(buffData,0,size);
					if(ReadProcessMemory(info->hprocess,LPVOID(ibegin),buffData,size,&dwRead))
					{
						int i=0;
						for(;i<(int)dwRead;i++)
						{
							if(buffData[i]==info->Findstring[nindex])
							{
								if(len==(nindex+1))   //匹配成功
								{	
									EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
									if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
									{
										wchar_t inx[10]={0};
										if(retstrlen==0)
											retstrlen=wcslen(info->retstr);
										if(retstrlen==0)
											swprintf(inx,L"%X",ibegin+(i-nindex)*2);
										else
											swprintf(inx,L"|%X",ibegin+(i-nindex)*2);
										wcscat(info->retstr,inx);
										//swprintf(info->retstr,L"%s|%X",info->retstr,ibegin+(i-nindex)*2);
									}
									else
									{
										LeaveCriticalSection(&info->m_mutex);
										return;
									}
									info->count++;
									nindex=0;
									LeaveCriticalSection(&info->m_mutex);
								}
								nindex++;
							}
							else 
								nindex=0;
						}

					}
				}
				else
				{
					ibegin=info->dwbegin[n];
					int sizen=4096;
					while(ibegin<info->dwend[n]&&info->dwend[n]>0)
					{
						if(ReadProcessMemory(info->hprocess,LPVOID(ibegin),buffData,sizen,&dwRead))
						{
							int i=0;
							for(;i<(int)dwRead;i++)
							{
								if(buffData[i]==info->Findstring[nindex])
								{
									if(len==(nindex+1))   //匹配成功
									{	
										EnterCriticalSection(&info->m_mutex);//临界区保护多线程对同一个字符串操作
										if(info->count<=20000)	//记录找到地址的上限为20000,防止溢出
										{
											wchar_t inx[10]={0};
											if(retstrlen==0)
												retstrlen=wcslen(info->retstr);
											if(retstrlen==0)
												swprintf(inx,L"%X",ibegin+(i-nindex)*2);
											else
												swprintf(inx,L"|%X",ibegin+(i-nindex)*2);
											wcscat(info->retstr,inx);
											//swprintf(info->retstr,L"%s|%X",info->retstr,ibegin+(i-nindex)*2);
										}
										else
										{
											LeaveCriticalSection(&info->m_mutex);
											return;
										}
										info->count++;
										nindex=0;
										LeaveCriticalSection(&info->m_mutex);
									}
									nindex++;
								}
								else 
									nindex=0;
							}

						}
						if(info->dwend[n]>=ibegin+4096)
							ibegin=ibegin+4096;
						else
						{
							sizen=info->dwend[n]-ibegin;
							ibegin=ibegin+4096;
							::memset(buffData,0,sizen);
						}
					}
				}
			}
		}
	}
}

bool TSMemoryAPI::TSFindInt(LONG hwnd,wchar_t * addr_range,wchar_t *retstr,LONG int_value_min, LONG int_value_max,LONG type,bool threadtype)
{
	bool bret=false;
	LONG ibegin=0;
	LONG ipend=0;
	DWORD MaxAddrs[MAX_PATH*200]={0};//最大寻址的地址个数
	int naddr=0;
	if(TSGetaddr_range(addr_range,ibegin,ipend,MaxAddrs,naddr))
	{
		if(ibegin>=ipend&&naddr==0)
			return bret;

		DWORD Pid=0;
		//if(my_GetWindowThreadProcessId)
		//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
		//else
		My_GetWindowThreadProcessId((HWND)hwnd,&Pid);

		TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
		HANDLE hprocess=NULL;
		//if(my_OpenProcess)
		//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
		//else
		//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
		hprocess=My_OpenProcess(Pid);

		if(hprocess==NULL)
		{
			if(my_ZwOpenProcess)
			{
				CLIENT_ID  Cileid;
				Cileid.UniqueProcess=(HANDLE)Pid;
				my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
			}
			if(hprocess==NULL)
				return bret;
		}


		int retstrlen=0;
		DWORD dwRead=0;
		int count=0;
		if(ibegin>=0&&naddr==0)
		{
			if(ibegin<0x400000)
				ibegin=0x400000;


			DWORD   nbegin[MAX_PATH*10]={0};
			DWORD	npend[MAX_PATH*10]={0}; 
			int naddr=0;
			GetFindaddr(hprocess,(LPVOID)ibegin,(LPVOID)ipend,nbegin,npend,naddr);
			if(naddr==0)
				return bret;

			if(threadtype)//是否多线程模式
			{
				DWORD ncpu=GetProcessNumber();//获取CPU个数,决定多线程的个数
				HANDLE threadhandle[MAX_PATH]={0};
				//双核以下CPU双线程
				if(ncpu<=2)
					ncpu=2;
				else if(ncpu>=8)
					ncpu=8;

				FindDataInfo info;
				info.FindDataType=FINDDATATYPE_FINDINTEX;
				info.hprocess=hprocess;
				info.int_value_min=int_value_min;
				info.int_value_max=int_value_max;
				info.FindIntType=type;
				info.retstr=retstr;
				info.dwncount=naddr;
				memcpy(info.dwbegin,nbegin,naddr*sizeof(DWORD));
				memcpy(info.dwend,npend,naddr*sizeof(DWORD));
				InitializeCriticalSection(&info.m_mutex);
				for(int i=0;i<ncpu;i++)
				{
					threadhandle[i]=(HANDLE)_beginthread(FindDataThread,0,&info);
				}

				::WaitForMultipleObjects(ncpu,threadhandle,TRUE,INFINITE);	//等待所有线程结束后返回
				DeleteCriticalSection(&info.m_mutex);
				return true;

			}

			BYTE buffData[4096]={0};
			for(int n=0;n<naddr;n++)
			{
				int size=npend[n]-nbegin[n];
				if(size<=4096)
				{
					::memset(buffData,0,size);
					if(ReadProcessMemory(hprocess,LPVOID(nbegin[n]),buffData,size,&dwRead))
					{
						int i=0;
						for(;i<(int)dwRead;i++)
						{
							if(type==0)//0 : 32位
							{
								int *value=(int *)&buffData[i];
								if(*value>=int_value_min&&*value<=int_value_max)
								{
									if(count<=20000)
									{
										wchar_t inx[10]={0};
										if(retstrlen==0)
											retstrlen=wcslen(retstr);
										if(retstrlen==0)
											swprintf(inx,L"%X",(ibegin+i));
										else
											swprintf(inx,L"|%X",(ibegin+i));
										wcscat(retstr,inx);
										//swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
									}
									count++;
									bret=true;
								}
							}
							else  if(type==1)  //1 : 16 位
							{
								short *value=(short *)&buffData[i];
								if(*value>=int_value_min&&*value<=int_value_max)
								{
									if(count<=20000)
									{
										wchar_t inx[10]={0};
										if(retstrlen==0)
											retstrlen=wcslen(retstr);
										if(retstrlen==0)
											swprintf(inx,L"%X",(ibegin+i));
										else
											swprintf(inx,L"|%X",(ibegin+i));
										wcscat(retstr,inx);
										//swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
									}
									count++;
									bret=true;
								}
							}
							else if(type==2)//2 : 8位
							{
								if(buffData[i]>=int_value_min&&buffData[i]<=int_value_max)
								{
									if(count<=20000)
									{
										wchar_t inx[10]={0};
										if(retstrlen==0)
											retstrlen=wcslen(retstr);
										if(retstrlen==0)
											swprintf(inx,L"%X",(ibegin+i));
										else
											swprintf(inx,L"|%X",(ibegin+i));
										wcscat(retstr,inx);
										//swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
									}
									count++;
									bret=true;
								}
							}
						}

					}
				}
				else
				{
					ibegin=nbegin[n];
					int sizen=4096;
					while(ibegin<npend[n]&&npend[n]>0)
					{
						if(ReadProcessMemory(hprocess,LPVOID(ibegin),buffData,sizen,&dwRead))
						{
							int i=0;
							for(;i<(int)dwRead;i++)
							{
								if(type==0)//0 : 32位
								{
									int *value=(int *)&buffData[i];
									if(*value>=int_value_min&&*value<=int_value_max)
									{
										if(count<=20000)
										{
											wchar_t inx[10]={0};
											if(retstrlen==0)
												retstrlen=wcslen(retstr);
											if(retstrlen==0)
												swprintf(inx,L"%X",(ibegin+i));
											else
												swprintf(inx,L"|%X",(ibegin+i));
											wcscat(retstr,inx);
											//swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
										}
										count++;
										bret=true;
									}
								}
								else  if(type==1)  //1 : 16 位
								{
									short *value=(short *)&buffData[i];
									if(*value>=int_value_min&&*value<=int_value_max)
									{
										if(count<=20000)
										{
											wchar_t inx[10]={0};
											if(retstrlen==0)
												retstrlen=wcslen(retstr);
											if(retstrlen==0)
												swprintf(inx,L"%X",(ibegin+i));
											else
												swprintf(inx,L"|%X",(ibegin+i));
											wcscat(retstr,inx);
											//swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
										}
										count++;
										bret=true;
									}
								}
								else if(type==2)//2 : 8位
								{
									if(buffData[i]>=int_value_min&&buffData[i]<=int_value_max)
									{
										if(count<=20000)
										{
											wchar_t inx[10]={0};
											if(retstrlen==0)
												retstrlen=wcslen(retstr);
											if(retstrlen==0)
												swprintf(inx,L"%X",(ibegin+i));
											else
												swprintf(inx,L"|%X",(ibegin+i));
											wcscat(retstr,inx);
											//swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
										}
										count++;
										bret=true;
									}
								}
							}

						}
						if(npend[n]>=ibegin+4096)
							ibegin=ibegin+4096;
						else
						{
							sizen=npend[n]-ibegin;
							ibegin=ibegin+4096;
							::memset(buffData,0,sizen);
						}
					}
				}
			}

			//BYTE buffData[4096]={0};
			//while(ibegin<ipend&&ibegin>0)
			//	{
			//	::memset(buffData,0,4096);
			//	if(ReadProcessMemory(hprocess,LPVOID(ibegin),buffData,4096,&dwRead))
			//		{
			//		int i=0;
			//		for(;i<(int)dwRead;i++)
			//			{
			//			if(type==0)//0 : 32位
			//				{
			//				int *value=(int *)&buffData[i];
			//				if(*value>=int_value_min&&*value<=int_value_max)
			//					{
			//						if(count<=20000)
			//							{
			//								if(retstrlen==0)
			//									retstrlen=wcslen(retstr);
			//								if(retstrlen==0)
			//									swprintf(retstr,L"%X",(ibegin+i));
			//								else
			//									swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
			//							}
			//					count++;
			//					bret=true;
			//					}
			//				}
			//			else  if(type==1)  //1 : 16 位
			//				{
			//				short *value=(short *)&buffData[i];
			//				if(*value>=int_value_min&&*value<=int_value_max)
			//					{
			//					if(count<=20000)
			//						{
			//							if(retstrlen==0)
			//								retstrlen=wcslen(retstr);
			//							if(retstrlen==0)
			//								swprintf(retstr,L"%X",(ibegin+i));
			//							else
			//								swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
			//						}
			//					count++;
			//					bret=true;
			//					}
			//				}
			//			else if(type==2)//2 : 8位
			//				{
			//				if(buffData[i]>=int_value_min&&buffData[i]<=int_value_max)
			//					{
			//					if(count<=20000)
			//						{
			//							if(retstrlen==0)
			//								retstrlen=wcslen(retstr);
			//							if(retstrlen==0)
			//								swprintf(retstr,L"%X",(ibegin+i));
			//							else
			//								swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
			//						}
			//					count++;
			//					bret=true;
			//					}
			//				}
			//			}

			//		}
			//	ibegin=ibegin+4096;
			//	}
		}
		else
		{
			if(type==0)//0 : 32位
			{
				int  value=0;
				for(int i=0;i<naddr;i++)
				{
					if(ReadProcessMemory(hprocess,LPVOID(MaxAddrs[i]),&value,sizeof(int),&dwRead))
					{
						if(value>=int_value_min&&value<=int_value_max)
						{
							if(count<=2000)
							{
								wchar_t inx[10]={0};
								if(retstrlen==0)
									retstrlen=wcslen(retstr);
								if(retstrlen==0)
									swprintf(inx,L"%X",MaxAddrs[i]);
								else
									swprintf(inx,L"|%X",MaxAddrs[i]);
								wcscat(retstr,inx);
								//swprintf(retstr,L"%s|%X",retstr,MaxAddrs[i]);
							}
							bret=true;
							count++;
						}
					}
				}
			}
			else if(type==1) //1 : 16 位
			{
				short  value=0;
				for(int i=0;i<naddr;i++)
				{
					if(ReadProcessMemory(hprocess,LPVOID(MaxAddrs[i]),&value,sizeof(short),&dwRead))
					{
						if(value>=int_value_min&&value<=int_value_max)
						{
							if(count<=2000)
							{
								wchar_t inx[10]={0};
								if(retstrlen==0)
									retstrlen=wcslen(retstr);
								if(retstrlen==0)
									swprintf(inx,L"%X",MaxAddrs[i]);
								else
									swprintf(inx,L"|%X",MaxAddrs[i]);
								wcscat(retstr,inx);
								//swprintf(retstr,L"%s|%X",retstr,MaxAddrs[i]);
							}
							bret=true;
							count++;
						}
					}
				}
			}
			else if(type==2)//2 : 8位
			{
				BYTE  value=0;
				for(int i=0;i<naddr;i++)
				{
					if(ReadProcessMemory(hprocess,LPVOID(MaxAddrs[i]),&value,sizeof(BYTE),&dwRead))
					{
						if(value>=int_value_min&&value<=int_value_max)
						{
							if(count<=2000)
							{
								wchar_t inx[10]={0};
								if(retstrlen==0)
									retstrlen=wcslen(retstr);
								if(retstrlen==0)
									swprintf(inx,L"%X",MaxAddrs[i]);
								else
									swprintf(inx,L"|%X",MaxAddrs[i]);
								wcscat(retstr,inx);
								//swprintf(retstr,L"%s|%X",retstr,MaxAddrs[i]);
							}
							bret=true;
							count++;
						}
					}
				}
			}

		}
	}
	return bret;
}

bool TSMemoryAPI::TSFindFloat(LONG hwnd,wchar_t * addr_range,wchar_t *retstr,FLOAT float_value_min, FLOAT float_value_max,bool threadtype)
{

	//	TSRuntime::newstr=NULL;

	bool bret=false;
	LONG ibegin=0;
	LONG ipend=0;
	DWORD MaxAddrs[MAX_PATH*200]={0};//最大寻址的地址个数
	int naddr=0;
	int count=0;
	if(TSGetaddr_range(addr_range,ibegin,ipend,MaxAddrs,naddr))
	{
		if(ibegin>=ipend&&naddr==0)
			return bret;

		DWORD Pid=0;
		//if(my_GetWindowThreadProcessId)
		//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
		//else
		My_GetWindowThreadProcessId((HWND)hwnd,&Pid);

		TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
		HANDLE hprocess=NULL;
		//if(my_OpenProcess)
		//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
		//else
		//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
		hprocess=My_OpenProcess(Pid);

		if(hprocess==NULL)
		{
			if(my_ZwOpenProcess)
			{
				CLIENT_ID  Cileid;
				Cileid.UniqueProcess=(HANDLE)Pid;
				my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
			}
			if(hprocess==NULL)
				return bret;
		}

		int retstrlen=0;
		DWORD dwRead=0;
		if(ibegin>=0&&naddr==0)
		{
			if(ibegin<0x400000)
				ibegin=0x400000;

			DWORD   nbegin[MAX_PATH*10]={0};
			DWORD	npend[MAX_PATH*10]={0}; 
			int naddr=0;
			GetFindaddr(hprocess,(LPVOID)ibegin,(LPVOID)ipend,nbegin,npend,naddr);
			if(naddr==0)
				return bret;

			if(threadtype)//是否多线程模式
			{
				memset(TSRuntime::membstr,0,MAX_PATH*200);
				DWORD ncpu=GetProcessNumber();//获取CPU个数,决定多线程的个数
				HANDLE threadhandle[MAX_PATH]={0};
				//双核以下CPU双线程
				if(ncpu<=2)
					ncpu=2;
				else if(ncpu>=8)
					ncpu=8;

				FindDataInfo info;
				info.FindDataType=FINDDATATYPE_FINDFLOATEX;
				info.hprocess=hprocess;
				info.float_value_min=float_value_min;
				info.float_value_max=float_value_max;
				info.retstr=retstr;
				info.dwncount=naddr;
				memcpy(info.dwbegin,nbegin,naddr*sizeof(DWORD));
				memcpy(info.dwend,npend,naddr*sizeof(DWORD));
				InitializeCriticalSection(&info.m_mutex);
				for(int i=0;i<ncpu;i++)
				{
					threadhandle[i]=(HANDLE)_beginthread(FindDataThread,0,&info);
				}

				::WaitForMultipleObjects(ncpu,threadhandle,TRUE,INFINITE);	//等待所有线程结束后返回
				DeleteCriticalSection(&info.m_mutex);
				return true;

			}

			BYTE buffData[4096]={0};
			for(int n=0;n<naddr;n++)
			{
				int size=npend[n]-nbegin[n];
				if(size<=4096)
				{
					::memset(buffData,0,size);
					if(ReadProcessMemory(hprocess,LPVOID(nbegin[n]),buffData,size,&dwRead))
					{
						int i=0;
						for(;i<(int)dwRead;i++)
						{
							float *float_value=(float *)&buffData[i];
							if(*float_value>=float_value_min&&*float_value<=float_value_max)
							{
								if(count<=20000)	//记录找到地址的上限为5000,防止溢出
								{
									wchar_t inx[10]={0};
									if(retstrlen==0)
										retstrlen=wcslen(retstr);
									if(retstrlen==0)
										swprintf(inx,L"%X",(ibegin+i));
									else
										swprintf(inx,L"|%X",(ibegin+i));
									wcscat(retstr,inx);
									//swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
								}
								count++;
							}
						}

					}
				}
				else
				{
					ibegin=nbegin[n];
					int sizen=4096;
					while(ibegin<npend[n]&&npend[n]>0)
					{
						if(ReadProcessMemory(hprocess,LPVOID(ibegin),buffData,sizen,&dwRead))
						{
							int i=0;
							for(;i<(int)dwRead;i++)
							{
								float *float_value=(float *)&buffData[i];
								if(*float_value>=float_value_min&&*float_value<=float_value_max)
								{
									if(count<=20000)	//记录找到地址的上限为5000,防止溢出
									{
										wchar_t inx[10]={0};
										if(retstrlen==0)
											retstrlen=wcslen(retstr);
										if(retstrlen==0)
											swprintf(inx,L"%X",(ibegin+i));
										else
											swprintf(inx,L"|%X",(ibegin+i));
										wcscat(retstr,inx);
										//swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
									}
									count++;
								}
							}

						}
						if(npend[n]>=ibegin+4096)
							ibegin=ibegin+4096;
						else
						{
							sizen=npend[n]-ibegin;
							ibegin=ibegin+4096;
							::memset(buffData,0,sizen);
						}
					}
				}
			}

			//BYTE buffData[4096]={0};
			//while(ibegin<ipend&&ibegin>0)
			//	{
			//	::memset(buffData,0,4096);
			//	if(ReadProcessMemory(hprocess,LPVOID(ibegin),buffData,4096,&dwRead))
			//		{
			//		int i=0;
			//		for(;i<(int)dwRead;i++)
			//			{
			//				float *float_value=(float *)&buffData[i];
			//				if(*float_value>=float_value_min&&*float_value<=float_value_max)
			//					{
			//					if(count<=10000)
			//						{
			//							if(retstrlen==0)
			//								retstrlen=wcslen(retstr);
			//							if(retstrlen==0)
			//								swprintf(retstr,L"%X",(ibegin+i));
			//							else
			//								swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
			//						}
			//					count++;
			//					bret=true;
			//					}
			//			}

			//		}
			//	ibegin=ibegin+4096;
			//	}
		}
		else
		{
			float buffData=0;
			for(int i=0;i<naddr;i++)
			{
				if(ReadProcessMemory(hprocess,LPVOID(MaxAddrs[i]),&buffData,sizeof(float),&dwRead))
				{
					if(buffData>=float_value_min&&buffData<=float_value_max)
					{
						if(count<=10000)
						{
							wchar_t inx[10]={0};
							if(retstrlen==0)
								retstrlen=wcslen(retstr);
							if(retstrlen==0)
								swprintf(inx,L"%X",MaxAddrs[i]);
							else
								swprintf(inx,L"|%X",MaxAddrs[i]);
							wcscat(retstr,inx);
							//swprintf(retstr,L"%s|%X",retstr,MaxAddrs[i]);
						}
						bret=true;
						count++;
					}
				}
			}

		}

	}

	//if(bret&&count>1600)
	//	TSRuntime::newstr=::SysAllocString(retstr);

	return bret;
}

bool TSMemoryAPI::TSFindDouble(LONG hwnd,wchar_t * addr_range,wchar_t *retstr,double double_value_min,double double_value_max,bool threadtype)
{
	bool bret=false;
	LONG ibegin=0;
	LONG ipend=0;
	DWORD MaxAddrs[MAX_PATH*200]={0};//最大寻址的地址个数
	int naddr=0;
	if(TSGetaddr_range(addr_range,ibegin,ipend,MaxAddrs,naddr))
	{

		if(ibegin>=ipend&&naddr==0)
			return bret;

		DWORD Pid=0;
		//if(my_GetWindowThreadProcessId)
		//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
		//else
		My_GetWindowThreadProcessId((HWND)hwnd,&Pid);

		TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
		HANDLE hprocess=NULL;
		//if(my_OpenProcess)
		//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
		//else
		//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
		hprocess=My_OpenProcess(Pid);

		if(hprocess==NULL)
		{
			if(my_ZwOpenProcess)
			{
				CLIENT_ID  Cileid;
				Cileid.UniqueProcess=(HANDLE)Pid;
				my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
			}
			if(hprocess==NULL)
				return bret;
		}

		int retstrlen=0;
		DWORD dwRead=0;
		int count=0;
		if(ibegin>=0&&naddr==0)
		{
			if(ibegin<0x400000)
				ibegin=0x400000;

			DWORD   nbegin[MAX_PATH*10]={0};
			DWORD	npend[MAX_PATH*10]={0}; 
			int naddr=0;
			GetFindaddr(hprocess,(LPVOID)ibegin,(LPVOID)ipend,nbegin,npend,naddr);
			if(naddr==0)
				return bret;

			if(threadtype)//是否多线程模式
			{
				memset(TSRuntime::membstr,0,MAX_PATH*200);
				DWORD ncpu=GetProcessNumber();//获取CPU个数,决定多线程的个数
				HANDLE threadhandle[MAX_PATH]={0};
				//双核以下CPU双线程
				if(ncpu<=2)
					ncpu=2;
				else if(ncpu>=8)
					ncpu=8;

				FindDataInfo info;
				info.FindDataType=FINDDATATYPE_FINDDOUBLEEX;
				info.hprocess=hprocess;
				info.double_value_min=double_value_min;
				info.double_value_max=double_value_max;
				info.retstr=retstr;
				info.dwncount=naddr;
				memcpy(info.dwbegin,nbegin,naddr*sizeof(DWORD));
				memcpy(info.dwend,npend,naddr*sizeof(DWORD));
				InitializeCriticalSection(&info.m_mutex);
				for(int i=0;i<ncpu;i++)
				{
					threadhandle[i]=(HANDLE)_beginthread(FindDataThread,0,&info);
				}

				::WaitForMultipleObjects(ncpu,threadhandle,TRUE,INFINITE);	//等待所有线程结束后返回
				DeleteCriticalSection(&info.m_mutex);
				return true;

			}

			BYTE buffData[4096]={0};
			for(int n=0;n<naddr;n++)
			{
				int size=npend[n]-nbegin[n];
				if(size<=4096)
				{
					::memset(buffData,0,size);
					if(ReadProcessMemory(hprocess,LPVOID(nbegin[n]),buffData,size,&dwRead))
					{
						int i=0;
						for(;i<(int)dwRead;i++)
						{
							double *double_value=(double *)&buffData[i];
							if(*double_value>=double_value_min&&*double_value<=double_value_max)
							{
								if(count<=20000)	//记录找到地址的上限为5000,防止溢出
								{
									wchar_t inx[10]={0};
									if(retstrlen==0)
										retstrlen=wcslen(retstr);
									if(retstrlen==0)
										swprintf(inx,L"%X",(ibegin+i));
									else
										swprintf(inx,L"|%X",(ibegin+i));
									wcscat(retstr,inx);
									//swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
								}
								count++;
							}
						}

					}
				}
				else
				{
					ibegin=nbegin[n];
					int sizen=4096;
					while(ibegin<npend[n]&&npend[n]>0)
					{
						if(ReadProcessMemory(hprocess,LPVOID(ibegin),buffData,sizen,&dwRead))
						{
							int i=0;
							for(;i<(int)dwRead;i++)
							{
								double *double_value=(double *)&buffData[i];
								if(*double_value>=double_value_min&&*double_value<=double_value_max)
								{
									if(count<=20000)	//记录找到地址的上限为5000,防止溢出
									{
										wchar_t inx[10]={0};
										if(retstrlen==0)
											retstrlen=wcslen(retstr);
										if(retstrlen==0)
											swprintf(inx,L"%X",(ibegin+i));
										else
											swprintf(inx,L"|%X",(ibegin+i));
										wcscat(retstr,inx);
										//swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
									}
									count++;
								}
							}

						}
						if(npend[n]>=ibegin+4096)
							ibegin=ibegin+4096;
						else
						{
							sizen=npend[n]-ibegin;
							ibegin=ibegin+4096;
							::memset(buffData,0,sizen);
						}
					}
				}
			}

			//BYTE buffData[4096]={0};
			//while(ibegin<ipend&&ibegin>0)
			//	{
			//	 ::memset(buffData,0,4096);
			//	if(ReadProcessMemory(hprocess,LPVOID(ibegin),buffData,4096,&dwRead))
			//		{
			//		int i=0;
			//		for(;i<(int)dwRead;i++)
			//			{
			//				double *double_value=(double *)&buffData[i];
			//				if(*double_value>=double_value_min&&*double_value<=double_value_max)
			//					{
			//					if(count<=5000)	//记录找到地址的上限为5000,防止溢出
			//						{
			//							if(retstrlen==0)
			//								retstrlen=wcslen(retstr);
			//							if(retstrlen==0)
			//								swprintf(retstr,L"%X",(ibegin+i));
			//							else
			//								swprintf(retstr,L"%s|%X",retstr,(ibegin+i));
			//						}
			//						count++;
			//					}
			//			}

			//		}
			//	ibegin=ibegin+4096;
			//	}
		}
		else
		{
			double buffData=0;
			for(int i=0;i<naddr;i++)
			{
				if(ReadProcessMemory(hprocess,LPVOID(MaxAddrs[i]),&buffData,sizeof(double),&dwRead))
				{
					if(buffData>=double_value_min&&buffData<=double_value_max)
					{
						if(count<=20000)	//记录找到地址的上限为5000,防止溢出
						{
							wchar_t inx[10]={0};
							if(retstrlen==0)
								retstrlen=wcslen(retstr);
							if(retstrlen==0)
								swprintf(inx,L"%X",MaxAddrs[i]);
							else
								swprintf(inx,L"|%X",MaxAddrs[i]);
							wcscat(retstr,inx);
							//swprintf(retstr,L"%s|%X",retstr,MaxAddrs[i]);
						}
						count++;
					}
				}
			}	
		}

	}
	return bret;
}

bool TSMemoryAPI::TSFindData(LONG hwnd,wchar_t * addr_range,wchar_t *data,wchar_t *retstr,bool threadtype)
{
	bool bret=false;
	LONG ibegin=0;
	LONG ipend=0;
	DWORD MaxAddrs[MAX_PATH*200]={0};//最大寻址的地址个数
	int naddr=0;
	if(wcslen(data)==0||wcslen(addr_range)==0)
		return false;
	if(TSGetaddr_range(addr_range,ibegin,ipend,MaxAddrs,naddr))
	{

		if(ibegin>=ipend&&naddr==0)
			return bret;

		DWORD Pid=0;
		//if(my_GetWindowThreadProcessId)
		//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
		//else
		My_GetWindowThreadProcessId((HWND)hwnd,&Pid);

		TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
		HANDLE hprocess=NULL;
		//if(my_OpenProcess)
		//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
		//else
		//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
		hprocess=My_OpenProcess(Pid);

		if(hprocess==NULL)
		{
			if(my_ZwOpenProcess)
			{
				CLIENT_ID  Cileid;
				Cileid.UniqueProcess=(HANDLE)Pid;
				my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
			}
			if(hprocess==NULL)
				return bret;
		}

		DWORD dwRead=0;
		int nindex=0;
		DWORD Finddata[MAX_PATH]={0};
		int nconut=0;//返回 data的个数
		TSGetDataValue(data,Finddata,nconut);
		int retstrlen=0;
		int count=0;
		if(ibegin>=0&&naddr==0)
		{
			if(ibegin<0x400000)
				ibegin=0x400000;


			DWORD   nbegin[MAX_PATH*10]={0};
			DWORD	npend[MAX_PATH*10]={0}; 
			int naddr=0;
			GetFindaddr(hprocess,(LPVOID)ibegin,(LPVOID)ipend,nbegin,npend,naddr);
			if(naddr==0)
				return bret;

			if(threadtype)//是否多线程模式
			{
				DWORD ncpu=GetProcessNumber();//获取CPU个数,决定多线程的个数
				HANDLE threadhandle[MAX_PATH]={0};
				//双核以下CPU双线程
				if(ncpu<=2)
					ncpu=2;
				else if(ncpu>=8)
					ncpu=8;

				FindDataInfo info;
				info.FindDataType=FINDDATATYPE_FINDDATAEX;
				info.hprocess=hprocess;
				memcpy(info.Finddata,Finddata,sizeof(DWORD)*nconut);
				info.FindIntType=nconut;
				info.retstr=retstr;
				info.dwncount=naddr;
				memcpy(info.dwbegin,nbegin,naddr*sizeof(DWORD));
				memcpy(info.dwend,npend,naddr*sizeof(DWORD));
				InitializeCriticalSection(&info.m_mutex);
				for(int i=0;i<ncpu;i++)
				{
					threadhandle[i]=(HANDLE)_beginthread(FindDataThread,0,&info);
				}

				::WaitForMultipleObjects(ncpu,threadhandle,TRUE,INFINITE);	//等待所有线程结束后返回
				DeleteCriticalSection(&info.m_mutex);
				return true;

			}

			BYTE buffData[4096]={0};
			for(int n=0;n<naddr;n++)
			{
				int size=npend[n]-nbegin[n];
				if(size<=4096)
				{
					::memset(buffData,0,size);
					if(ReadProcessMemory(hprocess,LPVOID(nbegin[n]),buffData,size,&dwRead))
					{
						int i=0;
						for(;i<(int)dwRead;i++)
						{
							if(buffData[i]==Finddata[nindex]||Finddata[nindex]==256)// unknow=256;//标记??模糊查询
							{
								if(nconut==(nindex+1))   //匹配成功
								{	
									if(count<=20000)	//记录找到地址的上限为5000,防止溢出
									{
										wchar_t inx[10]={0};
										if(retstrlen==0)
											retstrlen=wcslen(retstr);
										if(retstrlen==0)
											swprintf(inx,L"%X",(ibegin+i)-nindex);
										else
											swprintf(inx,L"|%X",(ibegin+i)-nindex);
										wcscat(retstr,inx);
										//swprintf(retstr,L"%s|%X",retstr,(ibegin+i)-nindex);
									}
									count++;
									nindex=0;
								}
								nindex++;
							}
							else
								nindex=0;
						}

					}
				}
				else
				{
					ibegin=nbegin[n];
					int sizen=4096;
					while(ibegin<npend[n]&&npend[n]>0)
					{
						if(ReadProcessMemory(hprocess,LPVOID(ibegin),buffData,sizen,&dwRead))
						{
							int i=0;
							for(;i<(int)dwRead;i++)
							{
								if(buffData[i]==Finddata[nindex]||Finddata[nindex]==256)// unknow=256;//标记??模糊查询
								{
									if(nconut==(nindex+1))   //匹配成功
									{	
										if(count<=20000)	//记录找到地址的上限为5000,防止溢出
										{
											wchar_t inx[10]={0};
											if(retstrlen==0)
												retstrlen=wcslen(retstr);
											if(retstrlen==0)
												swprintf(inx,L"%X",(ibegin+i)-nindex);
											else
												swprintf(inx,L"|%X",(ibegin+i)-nindex);
											wcscat(retstr,inx);
											//swprintf(retstr,L"%s|%X",retstr,(ibegin+i)-nindex);
										}
										count++;
										nindex=0;
									}
									nindex++;
								}
								else
									nindex=0;
							}

						}
						if(npend[n]>=ibegin+4096)
							ibegin=ibegin+4096;
						else
						{
							sizen=npend[n]-ibegin;
							ibegin=ibegin+4096;
							::memset(buffData,0,sizen);
						}
					}
				}
			}

			//byte buffData[4096]={0};
			//while(ibegin<ipend&&ibegin>0)
			// {
			//	 ::memset(buffData,4096,sizeof(byte)*4096);
			//	 if(ReadProcessMemory(hprocess,LPVOID(ibegin),&buffData,4096,&dwRead))
			//		 { 
			//			 for(int i=0;i<dwRead;i++)
			//				 {
			//					 if(buffData[i]==Finddata[nindex]||Finddata[nindex]==256)// unknow=256;//标记??模糊查询
			//						 {
			//							if(nconut==(nindex+1))   //匹配成功
			//								{	
			//								if(count<=5000)	//记录找到地址的上限为5000,防止溢出
			//									{
			//										if(retstrlen==0)
			//											retstrlen=wcslen(retstr);
			//										if(retstrlen==0)
			//											swprintf(retstr,L"%X",(ibegin+i)-nindex);
			//										else
			//											swprintf(retstr,L"%s|%X",retstr,(ibegin+i)-nindex);
			//									}
			//									count++;
			//									nindex=0;
			//								}
			//							nindex++;
			//						 }
			//					 else
			//						nindex=0;
			//					 
			//				 }
			//		 }
			//	 ibegin=ibegin+4096;
			// }
		}
		else  //地址是地址集合
		{
			byte buffData[MAX_PATH]={0};
			for(int i=0;i<naddr;i++)
			{
				if(ReadProcessMemory(hprocess,LPVOID(MaxAddrs[i]),&buffData,nconut,&dwRead))
				{
					for(int i=0;i<nconut;i++)
					{
						if(buffData[i]==Finddata[nindex]||Finddata[nindex]==256)// unknow=256;//标记??模糊查询
						{
							if(nconut==(nindex+1))   //匹配成功
							{	
								if(count<=5000)	//记录找到地址的上限为5000,防止溢出
								{
									wchar_t inx[10]={0};
									if(retstrlen==0)
										retstrlen=wcslen(retstr);
									if(retstrlen==0)
										swprintf(inx,L"%X",MaxAddrs[i]);
									else
										swprintf(inx,L"|%X",MaxAddrs[i]);
									wcscat(retstr,inx);
									// swprintf(retstr,L"%s|%X",retstr,MaxAddrs[i]);
								}
								count++;
								nindex=0;
							}
							nindex++;
						}
						else
						{
							nindex=0;
							break;
						}

					}
				}
			}
		}
	}
	return bret;
}

bool TSMemoryAPI::TSGetDataValue(wchar_t* strs,DWORD *Data_range,int &nconut)
{
	int len=wcslen(strs);
	int index=0;
	int n=0;
	int m=0;
	int unknow=0;//标记??模糊查询
	wchar_t s[3]={0};
	for(int i=0;i<len;i++)
	{
		if(strs[i]!=' ')
		{
			s[n]=strs[i];
			if(strs[i]=='?')
				unknow=256;
			if(n==1&&strs[i+1]!=' '&&(i+1)!=len)
			{
				if(unknow==0)
					Data_range[index]=wcstol(s,0,16);
				else
				{
					Data_range[index]=unknow;
					unknow=0;
				}
				memset(s,0,n);
				n=0;
				m=i;
				index++;
				continue;
			}
			n++;
		}
		else if(n!=0)
		{
			if(unknow==0)
				Data_range[index]=wcstol(s,0,16);
			else
			{
				Data_range[index]=unknow;
				unknow=0;
			}
			memset(s,0,n);
			n=0;
			m=i;
			index++;
		}
	}
	Data_range[index]=wcstol(&strs[m+1],0,16);
	nconut=index+1;
	return true;
}

bool TSMemoryAPI::TSGetaddr_range(wchar_t* strs,LONG &begin,LONG &end,DWORD *addr_range,int &nconut)
{
	bool bret=false;
	int len=wcslen(strs);
	wchar_t strbegin[16]={0};
	wchar_t strend[16]={0};
	bool fistrfind=false;
	begin=-1;
	end=-1;
	int index=0;
	int n=0;
	int m=0;
	for(int i=0;i<len;i++,n++)
	{
		if(strs[i]!='-'&&strs[i]!='|')
			strbegin[n]=strs[i];
		if(strs[i]=='-')
		{
			if(i>10)//非法地址
				return false;
			fistrfind=true;
			wcscpy(strend,&strs[i+1]);
			break;
		}
		else if(strs[i]=='|')
		{
			if(index>=(MAX_PATH*200)-1)
				return false;

			if(n<=10)//判断是否是非法地址
			{
				addr_range[index]=wcstol(strbegin,0,16);
				index++;
				bret=true;
			}
			memset(strbegin,0,n);
			n=-1;
			m=i;
		}
	}
	if(fistrfind)
	{
		begin=wcstol(strbegin,0,16);
		end=wcstol(strend,0,16);
		bret=true;
	}
	else
	{
		addr_range[index]=wcstol(&strs[m+1],0,16);   //拷贝最后一个地址
		nconut=index+1;
	}

	return bret;
}


bool TSMemoryAPI::TSValueTypeToData(int type,wchar_t *retstr,double dvalue,float fvalue,int ivalue,wchar_t *svalue,int type1)
{
	bool bret=false;
	if(type==0)//DoubleToData
	{
		BYTE bdouble[8]={0};
		memcpy(bdouble,&dvalue,8);
		for(int i=0;i<8;i++)
		{
			if(i==0)
			{
				if(bdouble[i]<0x10)
					swprintf(retstr,L"0%x",bdouble[i]);
				else
					swprintf(retstr,L"%x",bdouble[i]);
			}
			else
			{
				if(bdouble[i]<0x10)
					swprintf(retstr,L"%s 0%x",retstr,bdouble[i]);
				else
					swprintf(retstr,L"%s %x",retstr,bdouble[i]);
			}
		}
	}
	else if(type==1) //FloatToData
	{
		BYTE bfolat[4]={0};
		memcpy(bfolat,&fvalue,4);
		for(int i=0;i<4;i++)
		{
			if(i==0)
			{
				if(bfolat[i]<0x10)
					swprintf(retstr,L"0%x",bfolat[i]);
				else
					swprintf(retstr,L"%x",bfolat[i]);
			}
			else
			{
				if(bfolat[i]<0x10)
					swprintf(retstr,L"%s 0%x",retstr,bfolat[i]);
				else
					swprintf(retstr,L"%s %x",retstr,bfolat[i]);
			}
		}
		bret=true;
	}
	else if(type==2)//IntToData
	{               
		//0: 4字节整形数 (一般都选这个)
		//1: 2字节整形数
		//2: 1字节整形数
		int n=0;
		BYTE bint[4]={0};
		if(type1==0)
		{
			memcpy(bint,&ivalue,4);
			n=4;
		}
		else if(type1==1)
		{
			memcpy(bint,&ivalue,2);
			n=2;
		}
		else if(type1==2)
		{
			memcpy(bint,&ivalue,1);
			n=1;
		}
		if(n==0)
			return bret;

		for(int i=0;i<n;i++)
		{
			if(i==0)
			{
				if(bint[i]<0x10)
					swprintf(retstr,L"0%x",bint[i]);
				else
					swprintf(retstr,L"%x",bint[i]);
			}
			else
			{
				if(bint[i]<0x10)
					swprintf(retstr,L"%s 0%x",retstr,bint[i]);
				else
					swprintf(retstr,L"%s %x",retstr,bint[i]);
			}
		}
		bret=true;
	}

	else if(type==3)//StringToData
	{
		int len=wcslen(svalue);
		USES_CONVERSION;
		if(type1==0)//0: 返回Ascii表达的字符串
		{
			char *bstr=new char[len*2+1];
			strcpy(bstr,W2A(svalue));
			for(int i=0;i<len;i++)
			{
				if(i==0)
				{
					if(bstr[i]<0x10)
						swprintf(retstr,L"0%x",bstr[i]);
					else
						swprintf(retstr,L"%x",bstr[i]);
				}
				else
				{
					if(bstr[i]<0x10)
						swprintf(retstr,L"%s 0%x",retstr,bstr[i]);
					else
						swprintf(retstr,L"%s %x",retstr,bstr[i]);
				}
			}
			delete [] bstr;
			bret=true;
		}
		else if(type1==1)//1: 返回Unicode表达的字符串
		{
			for(int i=0;i<len;i++)
			{
				if(i==0)
				{
					if(svalue[i]<0x10)
						swprintf(retstr,L"0%x 00",svalue[i]);
					else
						swprintf(retstr,L"%x 00",svalue[i]);
				}
				else
				{
					if(svalue[i]<0x10)
						swprintf(retstr,L"%s 0%x 00",retstr,svalue[i]);
					else
						swprintf(retstr,L"%s %x 00",retstr,svalue[i]);
				}
			}
			bret=true;
		}
	}
	return bret;
}


DWORD TSMemoryAPI::TSGetFindDataAddr(wchar_t* strs, DWORD pid)
{
	DWORD readaddr=0;
	long  longAdd=0;
	wchar_t module[MAX_PATH]={0};
	int moduleindex=0;//记录模块长度
	int nindex=0;//记录指针个数
	int noffset=0;//
	int index=0;
	bool subindex[MAX_PATH]={0};//标记"-"号的位置:true,默认是"+":false
	wchar_t offsetaddrstr[MAX_PATH][16]={0};//存储偏移地址
	DWORD	 offsetaddr[MAX_PATH]={0};
	for(int i=0;i<MAX_PATH;i++)
	{
		memset(offsetaddrstr[i],0,16);
	}
	int len=wcslen(strs);
	if(len<1)
	{
		longAdd=-1;
		return longAdd;
	}
	for(int i=0;i<len;i++)
	{
		if(strs[i]=='<'||moduleindex>0)
		{
			if(strs[i]=='>')
			{
				moduleindex=-1;
			}
			else if(strs[i+1]!='>')
			{
				module[moduleindex]=strs[i+1];
				moduleindex++;
			}
		}
		if(strs[i]=='>'&&moduleindex==0)
		{
			longAdd=-1;
			return longAdd;
		}
		if(strs[i]=='[')
			nindex++;//记录指针个数
		else if(((strs[i]=='+'||index>0)||(strs[i]=='-'||index>0)||moduleindex==0))
		{
			if(strs[i]=='-')//记录-号的下标
				subindex[noffset]=true;
			if(strs[i]==']')
			{
				index=0;
				noffset++;
			}
			else if(strs[i]!=']'&&(strs[i]=='+'||strs[i]=='-'))
			{
				offsetaddrstr[noffset][index]=strs[i+1];
				index++;
				i++;
			}
			else if(strs[i]!=']')
			{
				offsetaddrstr[noffset][index]=strs[i];
				index++;
			}
		}
		else if((noffset==nindex))//记录最后一个偏移地址
		{
			if(strs[i]=='-')//记录-号的下标
				subindex[noffset]=true;
			if(strs[i]=='+'||strs[i]=='-')
			{
				offsetaddrstr[noffset][index]=strs[i+1];
				index++;
			}
			else if(index>0)
			{
				offsetaddrstr[noffset][index]=strs[i+1];
				index++;
			}

		}


	}
	DWORD Baseaddr=0;
	if(moduleindex==-1)//说明有传入<modulename>
	{
		HANDLE tlh = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(MODULEENTRY32);
		Module32First(tlh, &modEntry);
		do
		{
			if(!wcsicmp(module, modEntry.szModule))
			{
				Baseaddr=(DWORD)modEntry.modBaseAddr;
				DWORD handle=modEntry.th32ModuleID;
				break;
			}
		}
		while(Module32Next(tlh, &modEntry));
		CloseHandle(tlh);

		if(Baseaddr==0)
		{
			longAdd=-1;
			return longAdd;
		}
	}

	if(nindex>0) //说明不是一级指针
	{

		TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
		HANDLE hprocess=NULL;
		//if(my_OpenProcess)
		//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
		//else
		//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
		hprocess=My_OpenProcess(pid);

		if(hprocess==NULL)
		{
			if(my_ZwOpenProcess)
			{
				CLIENT_ID  Cileid;
				Cileid.UniqueProcess=(HANDLE)pid;
				my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
			}
			if(hprocess==NULL)
				return longAdd;
		}

		for(int i=0;i<nindex;i++)
		{
			offsetaddr[i]=wcstol(offsetaddrstr[i],0,16);
			if(i==0&&Baseaddr>0)
				readaddr=Baseaddr+offsetaddr[i];
			else if(i==0)
				readaddr=offsetaddr[i];
			else if(readaddr!=0)
			{
				if(subindex[i])
					readaddr=longAdd-offsetaddr[i];
				else
					readaddr=longAdd+offsetaddr[i];
			}

			longAdd=0;
			if(my_ReadProcessMemory)
				my_ReadProcessMemory(hprocess,(void*)readaddr,&longAdd,sizeof(longAdd),long(0));
			else
				::ReadProcessMemory(hprocess,(void*)readaddr,&longAdd,sizeof(longAdd),long(0));
			if(longAdd<=0)//说明读取错误地址
			{
				longAdd=-1;
				return longAdd;
			}


		}
		offsetaddr[nindex]=wcstol(offsetaddrstr[nindex],0,16);
		if(subindex[nindex])
			longAdd=longAdd-offsetaddr[nindex];
		else
			longAdd=longAdd+offsetaddr[nindex];
		CloseHandle(hprocess);
	}
	else if(moduleindex!=-1)//1367DBC
		longAdd=wcstol(strs,0,16);
	else if(moduleindex==-1) //<xx.exe>+1367DBC
	{
		longAdd=wcstol(offsetaddrstr[0],0,16);
		longAdd=Baseaddr+longAdd;
	}
	return longAdd;
}

bool TSMemoryAPI::TSFindString(LONG hwnd,wchar_t * addr_range,wchar_t *retstr,wchar_t*string_value,LONG type,bool threadtype)
{
	bool bret=false;
	LONG ibegin=0;
	LONG ipend=0;
	DWORD MaxAddrs[MAX_PATH*200]={0};//最大寻址的地址个数
	int naddr=0;
	if(TSGetaddr_range(addr_range,ibegin,ipend,MaxAddrs,naddr))
	{
		if(ibegin>=ipend&&naddr==0)
			return bret;

		DWORD Pid=0;
		//if(my_GetWindowThreadProcessId)
		//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
		//else
		My_GetWindowThreadProcessId((HWND)hwnd,&Pid);

		TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
		HANDLE hprocess=NULL;
		//if(my_OpenProcess)
		//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
		//else
		//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
		hprocess=My_OpenProcess(Pid);

		if(hprocess==NULL)
		{
			if(my_ZwOpenProcess)
			{
				CLIENT_ID  Cileid;
				Cileid.UniqueProcess=(HANDLE)Pid;
				my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
			}
			if(hprocess==NULL)
				return bret;
		}

		int retstrlen=0;
		DWORD dwRead=0;
		int count=0;
		int nindex=0;
		if(ibegin>=0&&naddr==0)
		{
			if(ibegin<0x400000)
				ibegin=0x400000;

			DWORD   nbegin[MAX_PATH*10]={0};
			DWORD	npend[MAX_PATH*10]={0}; 
			int naddr=0;
			GetFindaddr(hprocess,(LPVOID)ibegin,(LPVOID)ipend,nbegin,npend,naddr);
			if(naddr==0)
				return bret;

			if(threadtype)//是否多线程模式
			{
				DWORD ncpu=GetProcessNumber();//获取CPU个数,决定多线程的个数
				HANDLE threadhandle[MAX_PATH]={0};
				//双核以下CPU双线程
				if(ncpu<=2)
					ncpu=2;
				else if(ncpu>=8)
					ncpu=8;

				int len=wcslen(string_value);
				if(len>=MAX_PATH||len==0)
					return bret;

				FindDataInfo info;
				info.FindDataType=FINDDATATYPE_FINDSTRINGEX;
				info.hprocess=hprocess;
				wcscpy(info.Findstring,string_value);
				info.FindIntType=type;	////0：ASCCII编码查找  1：Unicode编码查找
				info.retstr=retstr;
				info.dwncount=naddr;
				memcpy(info.dwbegin,nbegin,naddr*sizeof(DWORD));
				memcpy(info.dwend,npend,naddr*sizeof(DWORD));
				InitializeCriticalSection(&info.m_mutex);
				for(int i=0;i<ncpu;i++)
				{
					threadhandle[i]=(HANDLE)_beginthread(FindDataThread,0,&info);
				}

				::WaitForMultipleObjects(ncpu,threadhandle,TRUE,INFINITE);	//等待所有线程结束后返回
				DeleteCriticalSection(&info.m_mutex);
				return true;

			}

			//////////////////0：ASCCII编码查找  1：Unicode编码查找//////////////////////////////
			if(type == 0)
			{
				USES_CONVERSION;
				int len=wcslen(string_value);
				if(len>MAX_PATH||len==0)
					return bret;
				char chstr[MAX_PATH*2]={0};
				strcpy(chstr,W2A(string_value));
				len=strlen(chstr);;

				BYTE buffData[4096]={0};
				for(int n=0;n<naddr;n++)
				{
					int size=npend[n]-nbegin[n];
					if(size<=4096)
					{
						::memset(buffData,0,size);
						if(ReadProcessMemory(hprocess,LPVOID(nbegin[n]),buffData,size,&dwRead))
						{
							int i=0;
							for(;i<(int)dwRead;i++)
							{
								if(buffData[i]==chstr[nindex])
								{
									if(len==(nindex+1))   //匹配成功
									{	
										if(count<=20000)
										{
											wchar_t inx[10]={0};
											if(retstrlen==0)
												retstrlen=wcslen(retstr);
											if(retstrlen==0)
												swprintf(inx,L"%X",ibegin+i-nindex);
											else
												swprintf(inx,L"|%X",ibegin+i-nindex);
											wcscat(retstr,inx);
											//swprintf(retstr,L"%s|%X",retstr,ibegin+i-nindex);
										}
										bret=true;
										count++;
										nindex=0;
									}
									nindex++;
								}
								else 
									nindex=0;
							}

						}
					}
					else
					{
						ibegin=nbegin[n];
						int sizen=4096;
						while(ibegin<npend[n]&&npend[n]>0)
						{
							if(ReadProcessMemory(hprocess,LPVOID(ibegin),buffData,sizen,&dwRead))
							{
								int i=0;
								for(;i<(int)dwRead;i++)
								{
									if(buffData[i]==chstr[nindex])
									{
										if(len==(nindex+1))   //匹配成功
										{	
											if(count<=20000)
											{
												wchar_t inx[10]={0};
												if(retstrlen==0)
													retstrlen=wcslen(retstr);
												if(retstrlen==0)
													swprintf(inx,L"%X",ibegin+(i-nindex)*2);
												else
													swprintf(inx,L"|%X",ibegin+(i-nindex)*2);
												wcscat(retstr,inx);
												//swprintf(retstr,L"%s|%X",retstr,ibegin+i-nindex);
											}
											bret=true;
											count++;
											nindex=0;
										}
										nindex++;
									}
									else 
										nindex=0;
								}

							}
							if(npend[n]>=ibegin+4096)
								ibegin=ibegin+4096;
							else
							{
								sizen=npend[n]-ibegin;
								ibegin=ibegin+4096;
								::memset(buffData,0,sizen);
							}
						}
					}
				}

				//while(ibegin<ipend&&ibegin>0)
				//	{
				//	memset(buffData,0,4096);
				//	if(ReadProcessMemory(hprocess,(PVOID)ibegin,buffData,4096,&dwRead))
				//		{ 
				//		for(int i=0;i<dwRead;i++)
				//			{
				//			if(buffData[i]==chstr[nindex])
				//				{
				//				if(len==(nindex+1))   //匹配成功
				//					{	
				//						if(count<=20000)
				//							{
				//							if(retstrlen==0)
				//								retstrlen=wcslen(retstr);
				//							if(retstrlen==0)
				//								swprintf(retstr,L"%X",ibegin+i-nindex);
				//							else
				//								swprintf(retstr,L"%s|%X",retstr,ibegin+i-nindex);
				//							}
				//						bret=true;
				//						count++;
				//						nindex=0;
				//					}
				//				nindex++;
				//				}
				//			else 
				//				nindex=0;
				//			}
				//		}
				//	ibegin=ibegin+4096;
				//	}
			}
			else if(type==1)
			{
				wchar_t buffData[2048]={0};
				int len=wcslen(string_value);
				if(len>MAX_PATH||len==0)
					return bret;

				//BYTE buffData[4096]={0};
				for(int n=0;n<naddr;n++)
				{
					int size=npend[n]-nbegin[n];
					if(size<=4096)
					{
						::memset(buffData,0,size);
						if(ReadProcessMemory(hprocess,LPVOID(nbegin[n]),buffData,size,&dwRead))
						{
							int i=0;
							for(;i<(int)dwRead;i++)
							{
								if(buffData[i]==string_value[nindex])
								{
									if(len==(nindex+1))   //匹配成功
									{	
										if(count<=20000)
										{
											wchar_t inx[10]={0};
											if(retstrlen==0)
												retstrlen=wcslen(retstr);
											if(retstrlen==0)
												swprintf(inx,L"%X",ibegin+(i-nindex)*2);
											else
												swprintf(inx,L"|%X",ibegin+(i-nindex)*2);
											wcscat(retstr,inx);
											//swprintf(retstr,L"%s|%X",retstr,ibegin+(i-nindex)*2);
										}
										bret=true;
										count++;
										nindex=0;
									}
									nindex++;
								}
								else 
									nindex=0;
							}

						}
					}
					else
					{
						ibegin=nbegin[n];
						int sizen=4096;
						while(ibegin<npend[n]&&npend[n]>0)
						{
							if(ReadProcessMemory(hprocess,LPVOID(ibegin),buffData,sizen,&dwRead))
							{
								int i=0;
								for(;i<(int)dwRead;i++)
								{
									if(buffData[i]==string_value[nindex])
									{
										if(len==(nindex+1))   //匹配成功
										{	
											if(count<=20000)
											{
												wchar_t inx[10]={0};
												if(retstrlen==0)
													retstrlen=wcslen(retstr);
												if(retstrlen==0)
													swprintf(inx,L"%X",ibegin+(i-nindex)*2);
												else
													swprintf(inx,L"|%X",ibegin+(i-nindex)*2);
												wcscat(retstr,inx);
												//swprintf(retstr,L"%s|%X",retstr,ibegin+(i-nindex)*2);
											}
											bret=true;
											count++;
											nindex=0;
										}
										nindex++;
									}
									else 
										nindex=0;
								}

							}
							if(npend[n]>=ibegin+4096)
								ibegin=ibegin+4096;
							else
							{
								sizen=npend[n]-ibegin;
								ibegin=ibegin+4096;
								::memset(buffData,0,sizen);
							}
						}
					}
				}

				//while(ibegin<ipend&&ibegin>0)
				//	{
				//	dwRead=0;
				//	memset(buffData,0,2048*sizeof(wchar_t));
				//	if(ReadProcessMemory(hprocess,(PVOID)ibegin,buffData,2048*sizeof(wchar_t),&dwRead))
				//		{			
				//		for(int i=0;i<dwRead;i++)
				//			{
				//			if(buffData[i]==string_value[nindex])
				//				{
				//				if(len==(nindex+1))   //匹配成功
				//					{	
				//						if(count<=20000)
				//							{
				//							if(retstrlen==0)
				//								retstrlen=wcslen(retstr);
				//							if(retstrlen==0)
				//								swprintf(retstr,L"%X",ibegin+(i-nindex)*2);
				//							else
				//								swprintf(retstr,L"%s|%X",retstr,ibegin+(i-nindex)*2);
				//							}
				//						bret=true;
				//						count++;
				//						nindex=0;
				//					}
				//				nindex++;
				//				}
				//			else 
				//				nindex=0;
				//			}
				//		}
				//	ibegin=ibegin+2048*sizeof(wchar_t);
				//	//ibegin=ibegin+4096-findLength;
				//	}
			}
		}
		else  //重复查找
		{
			//////////////////0：ASCCII编码查找  1：Unicode编码查找//////////////////////////////
			if(type==0)//0 : ASCCII
			{
				char buffData[MAX_PATH*2]={0};
				USES_CONVERSION;
				int len=wcslen(string_value);
				if(len>MAX_PATH)
					return bret;
				char  value[MAX_PATH*2]={0};
				strcpy(value,W2A(string_value));
				len=strlen(value);
				for(int i=0;i<naddr;i++)
				{
					if(ReadProcessMemory(hprocess,LPVOID(MaxAddrs[i]),&buffData,len,&dwRead))
					{
						if(::memcmp(buffData,value,len)==0)
						{
							if(count<=20000)
							{
								wchar_t inx[10]={0};
								if(retstrlen==0)
									retstrlen=wcslen(retstr);
								if(retstrlen==0)
									swprintf(inx,L"%X",MaxAddrs[i]);
								else
									swprintf(inx,L"|%X",MaxAddrs[i]);
								wcscat(retstr,inx);
								//swprintf(retstr,L"%s|%X",retstr,MaxAddrs[i]);
							}
							bret=true;
							count++;
						}
					}
				}
			}
			else if(type==1) //1 :Unicode
			{
				wchar_t buffData[MAX_PATH]={0};
				int len=wcslen(string_value);
				for(int i=0;i<naddr;i++)
				{
					if(ReadProcessMemory(hprocess,LPVOID(MaxAddrs[i]),&buffData,len,&dwRead))
					{
						if(::memcmp(buffData,string_value,len)==0)
						{
							if(count<=20000)
							{
								wchar_t inx[10]={0};
								if(retstrlen==0)
									retstrlen=wcslen(retstr);
								if(retstrlen==0)
									swprintf(inx,L"%X",MaxAddrs[i]);
								else
									swprintf(inx,L"|%X",MaxAddrs[i]);
								wcscat(retstr,inx);
								//swprintf(retstr,L"%s|%X",retstr,MaxAddrs[i]);
							}
							bret=true;
							count++;
						}
					}
				}
			}
		}
	}
	return bret;

}

bool TSMemoryAPI::TSReadData(LONG hwnd,wchar_t*addr,wchar_t *retstr,LONG len)
{
	bool bret=false;

	if(len<0||len>MAX_PATH)
		return bret;

	DWORD pid=0;
	//if(my_GetWindowThreadProcessId)
	//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
	//else
	My_GetWindowThreadProcessId((HWND)hwnd,&pid);

	DWORD findaddr=TSGetFindDataAddr(addr,pid);
	if(findaddr==-1)
		return bret;

	TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
	HANDLE hprocess=NULL;
	//if(my_OpenProcess)
	//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	//else
	//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	hprocess=My_OpenProcess(pid);

	if(hprocess==NULL)
	{
		if(my_ZwOpenProcess)
		{
			CLIENT_ID  Cileid;
			Cileid.UniqueProcess=(HANDLE)pid;
			my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
		}
		if(hprocess==NULL)
			return bret;
	}


	BYTE finddata[MAX_PATH]={0};
	DWORD dwread=0;
	if(my_ReadProcessMemory)
		my_ReadProcessMemory(hprocess, (LPCVOID)findaddr,finddata,len,&dwread);
	else
		::ReadProcessMemory(hprocess, (LPCVOID)findaddr,finddata,len,&dwread);

	if(dwread>0)
	{
		for(int i=0;i<len;i++)
		{
			if(i==0)
			{
				if(finddata[i]<0x10)
				{
					if(finddata[i]>=0)
						swprintf(retstr,L"%s 0%X",retstr,finddata[i]);
					else
					{
						LONG val=finddata[i]-0xffffff00;
						swprintf(retstr,L"%s %X",retstr,val);
					}
				}
				else
					swprintf(retstr,L"%X",finddata[i]);
				bret=true;
			}
			else
			{
				if(finddata[i]<0x10)
				{
					if(finddata[i]>=0)
						swprintf(retstr,L"%s 0%X",retstr,finddata[i]);
					else
					{
						LONG val=finddata[i]-0xffffff00;
						swprintf(retstr,L"%s %X",retstr,val);
					}
				}

				else
					swprintf(retstr,L"%s %X",retstr,finddata[i]);


				bret=true;
			}
		}
	}

	CloseHandle(hprocess);
	return bret;

}

bool TSMemoryAPI::TSReadDouble(LONG hwnd,wchar_t*addr,double &dvalue,float &fvalue,int type)
{
	bool bret=false;

	DWORD pid=0;
	//if(my_GetWindowThreadProcessId)
	//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
	//else
	My_GetWindowThreadProcessId((HWND)hwnd,&pid);



	DWORD findaddr=TSGetFindDataAddr(addr,pid);
	if(findaddr==-1)
		return bret;

	TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
	HANDLE hprocess=NULL;
	//if(my_OpenProcess)
	//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	//else
	//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	hprocess=My_OpenProcess(pid);

	if(hprocess==NULL)
	{
		if(my_ZwOpenProcess)
		{
			CLIENT_ID  Cileid;
			Cileid.UniqueProcess=(HANDLE)pid;
			my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
		}
		if(hprocess==NULL)
			return bret;
	}

	DWORD dwread=0;
	if(type==0)//type0:DOUBLE,1:FLOAT
	{
		double readdouble=0;
		if(my_ReadProcessMemory)
			my_ReadProcessMemory(hprocess, (LPCVOID)findaddr,&readdouble,sizeof(double),&dwread);
		else
			::ReadProcessMemory(hprocess, (LPCVOID)findaddr,&readdouble,sizeof(double),&dwread);

		dvalue=readdouble;
	}
	else if(type==1)
	{
		float readfloat=0;
		if(my_ReadProcessMemory)
			my_ReadProcessMemory(hprocess, (LPCVOID)findaddr,&readfloat,sizeof(float),&dwread);
		else
			::ReadProcessMemory(hprocess, (LPCVOID)findaddr,&readfloat,sizeof(float),&dwread);

		fvalue=readfloat;
	}

	CloseHandle(hprocess);
	if(dwread!=0)
		bret=true;
	return bret;

}

bool TSMemoryAPI::TSReadInt(LONG hwnd,wchar_t*addr,int &ivalue,short &svalue,BYTE &bvalue,int type)
{
	bool bret=false;

	DWORD pid=0;
	//if(my_GetWindowThreadProcessId)
	//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
	//else
	My_GetWindowThreadProcessId((HWND)hwnd,&pid);


	DWORD findaddr=TSGetFindDataAddr(addr,pid);
	if(findaddr==-1)
		return bret;

	TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
	HANDLE hprocess=NULL;
	//if(my_OpenProcess)
	//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	//else
	//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	hprocess=My_OpenProcess(pid);

	if(hprocess==NULL)
	{
		if(my_ZwOpenProcess)
		{
			CLIENT_ID  Cileid;
			Cileid.UniqueProcess=(HANDLE)pid;
			my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
		}
		if(hprocess==NULL)
			return bret;
	}

	DWORD dwread=0;

	//0 : 32位
	//1 : 16 位
	//2 : 8位
	if(type==0)
	{
		int value=0;
		if(my_ReadProcessMemory)
			my_ReadProcessMemory(hprocess, (LPCVOID)findaddr,&value,sizeof(int),&dwread);
		else
			::ReadProcessMemory(hprocess, (LPCVOID)findaddr,&value,sizeof(int),&dwread);

		ivalue=value;
	}
	else if(type==1)
	{
		short value=0;
		if(my_ReadProcessMemory)
			my_ReadProcessMemory(hprocess, (LPCVOID)findaddr,&value,sizeof(short),&dwread);
		else
			::ReadProcessMemory(hprocess, (LPCVOID)findaddr,&value,sizeof(short),&dwread);

		svalue=value;
	}
	else if(type==2)
	{
		BYTE value=0;
		if(my_ReadProcessMemory)
			my_ReadProcessMemory(hprocess, (LPCVOID)findaddr,&value,sizeof(BYTE),&dwread);
		else
			::ReadProcessMemory(hprocess, (LPCVOID)findaddr,&value,sizeof(BYTE),&dwread);

		bvalue=value;
	}

	CloseHandle(hprocess);
	if(dwread!=0)
		bret=true;
	return bret;
}

bool TSMemoryAPI::TSReadString(LONG hwnd,wchar_t*addr,wchar_t *retstr,LONG len,int type)
{
	bool bret=false;

	if(len>MAX_PATH)
		return bret;

	DWORD pid=0;
	//if(my_GetWindowThreadProcessId)
	//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
	//else
	My_GetWindowThreadProcessId((HWND)hwnd,&pid);


	DWORD findaddr=TSGetFindDataAddr(addr,pid);
	if(findaddr==-1)
		return bret;

	TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
	HANDLE hprocess=NULL;
	//if(my_OpenProcess)
	//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	//else
	//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	hprocess=My_OpenProcess(pid);

	if(hprocess==NULL)
	{
		if(my_ZwOpenProcess)
		{
			CLIENT_ID  Cileid;
			Cileid.UniqueProcess=(HANDLE)pid;
			my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
		}
		if(hprocess==NULL)
			return bret;
	}

	DWORD dwread=0;

	////0 : GBK字符串
	////1 :Unicode字符串

	if(type==0)
	{
		char strvalue[MAX_PATH]={0};
		if(my_ReadProcessMemory)
			my_ReadProcessMemory(hprocess, (LPCVOID)findaddr,strvalue,len,&dwread);
		else
			::ReadProcessMemory(hprocess, (LPCVOID)findaddr,strvalue,len,&dwread);

		USES_CONVERSION;
		wcscpy(retstr,A2W(strvalue));
	}
	else if(type==1)
	{
		wchar_t wstrvlue[MAX_PATH]={0};
		if(my_ReadProcessMemory)
			my_ReadProcessMemory(hprocess, (LPCVOID)findaddr,wstrvlue,len*sizeof(wchar_t),&dwread);
		else
			::ReadProcessMemory(hprocess, (LPCVOID)findaddr,wstrvlue,len*sizeof(wchar_t),&dwread);

		wcscpy(retstr,wstrvlue);
	}

	CloseHandle(hprocess);
	if(dwread!=0)
		bret=true;
	return bret;
}

bool TSMemoryAPI::TSTerminateProcess(LONG pid)
{
	bool bret=false;
	TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
	HANDLE hprocess=NULL;
	//if(my_OpenProcess)
	//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	//else
	//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	hprocess=My_OpenProcess(pid);

	if(hprocess==NULL)
	{
		if(my_ZwOpenProcess)
		{
			CLIENT_ID  Cileid;
			Cileid.UniqueProcess=(HANDLE)pid;
			my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
		}
		if(hprocess==NULL)
			return bret;
	}

	if(my_TerminateProcess)
		bret=my_TerminateProcess(hprocess,0);
	else
		bret=::TerminateProcess(hprocess,0);

	CloseHandle(hprocess);
	return bret;
}

bool TSMemoryAPI::TSVirtualAllocEx(LONG hwnd,LONG &addr,LONG size,LONG type)
{
	bool bret=false;

	DWORD pid=0;
	//if(my_GetWindowThreadProcessId)
	//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
	//else
	My_GetWindowThreadProcessId((HWND)hwnd,&pid);


	TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
	HANDLE hprocess=NULL;
	//if(my_OpenProcess)
	//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	//else
	//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	hprocess=My_OpenProcess(pid);

	if(hprocess==NULL)
	{
		if(my_ZwOpenProcess)
		{
			CLIENT_ID  Cileid;
			Cileid.UniqueProcess=(HANDLE)pid;
			my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
		}
		if(hprocess==NULL)
			return bret;
	}

	DWORD dwtype=0;
	if(type==0)//可读可写可执行
	{
		dwtype=PAGE_EXECUTE_READWRITE;
	}
	else//1 : 可读可执行，不可写

	{
		dwtype=PAGE_EXECUTE_READ;
	}
	LPVOID retaddr=NULL;
	if(dwtype!=0)
		retaddr=::VirtualAllocEx(hprocess,(void*)addr,size,MEM_COMMIT,dwtype);

	CloseHandle(hprocess);
	addr=(DWORD)retaddr;

	return bret;
}

bool TSMemoryAPI::TSVirtualFreeEx(LONG hwnd,LONG addr)
{
	bool bret=false;

	DWORD pid=0;
	//if(my_GetWindowThreadProcessId)
	//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
	//else
	My_GetWindowThreadProcessId((HWND)hwnd,&pid);

	TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
	HANDLE hprocess=NULL;
	//if(my_OpenProcess)
	//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	//else
	//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	hprocess=My_OpenProcess(pid);

	if(hprocess==NULL)
	{
		if(my_ZwOpenProcess)
		{
			CLIENT_ID  Cileid;
			Cileid.UniqueProcess=(HANDLE)pid;
			my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
		}
		if(hprocess==NULL)
			return bret;
	}

	bret=::VirtualFreeEx(hprocess,(void*)addr,0,MEM_RELEASE);

	CloseHandle(hprocess);
	return bret;
}

bool TSMemoryAPI::TSWriteData(LONG hwnd,wchar_t * addr,wchar_t *data)
{
	bool bret=false;

	DWORD pid=0;
	//if(my_GetWindowThreadProcessId)
	//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
	//else
	My_GetWindowThreadProcessId((HWND)hwnd,&pid);

	DWORD findaddr=TSGetFindDataAddr(addr,pid);
	if(findaddr==-1)
		return bret;

	TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
	HANDLE hprocess=NULL;
	//if(my_OpenProcess)
	//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	//else
	//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	hprocess=My_OpenProcess(pid);

	if(hprocess==NULL)
	{
		if(my_ZwOpenProcess)
		{
			CLIENT_ID  Cileid;
			Cileid.UniqueProcess=(HANDLE)pid;
			my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
		}
		if(hprocess==NULL)
			return bret;
	}
	DWORD dwwrite=0;
	DWORD Finddata[MAX_PATH]={0};
	int nconut=0;//返回 data的个数
	TSGetDataValue(data,Finddata,nconut);
	if(nconut<=0)
		return bret;


	BYTE strvalue[MAX_PATH*10]={0};
	for(int i=0;i<nconut;i++)
	{
		strvalue[i]=Finddata[i];
	}

	DWORD dwback;
	if(my_VirtualProtectEx)
		my_VirtualProtectEx(hprocess,(void*)findaddr, nconut, PAGE_EXECUTE_READWRITE, &dwback);
	else
		::VirtualProtectEx(hprocess,(void*)findaddr, nconut, PAGE_EXECUTE_READWRITE, &dwback);

	if(my_WriteProcessMemory)
		bret=my_WriteProcessMemory(hprocess, (void*)findaddr,strvalue,nconut,&dwwrite);
	else	  
		bret=::WriteProcessMemory(hprocess, (void*)findaddr,strvalue,nconut,&dwwrite);
	if(my_VirtualProtectEx)
		bret=my_VirtualProtectEx(hprocess,(void*)findaddr, nconut, dwback, &dwback);
	else
		bret=::VirtualProtectEx(hprocess,(void*)findaddr, nconut, dwback, &dwback);

	CloseHandle(hprocess);
	return bret;
}

bool TSMemoryAPI::TSWriteDouble(LONG hwnd,wchar_t *addr,DOUBLE dvalue,FLOAT fvlaue)
{
	bool bret=false;

	DWORD pid=0;
	//if(my_GetWindowThreadProcessId)
	//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
	//else
	My_GetWindowThreadProcessId((HWND)hwnd,&pid);

	DWORD findaddr=TSGetFindDataAddr(addr,pid);
	if(findaddr==-1)
		return bret;

	TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
	HANDLE hprocess=NULL;
	//if(my_OpenProcess)
	//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	//else
	//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	hprocess=My_OpenProcess(pid);

	if(hprocess==NULL)
	{
		if(my_ZwOpenProcess)
		{
			CLIENT_ID  Cileid;
			Cileid.UniqueProcess=(HANDLE)pid;
			my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
		}
		if(hprocess==NULL)
			return bret;
	}

	DWORD dwback;
	if(my_VirtualProtectEx)
		my_VirtualProtectEx(hprocess,(void*)findaddr, sizeof(DOUBLE), PAGE_EXECUTE_READWRITE, &dwback);
	else
		::VirtualProtectEx(hprocess,(void*)findaddr, sizeof(DOUBLE), PAGE_EXECUTE_READWRITE, &dwback);

	DWORD dwwrite=0;
	if(dvalue!=0)
	{
		if(my_WriteProcessMemory)
			bret=my_WriteProcessMemory(hprocess, (void*)findaddr,&dvalue,sizeof(DOUBLE),&dwwrite);
		else
			bret=::WriteProcessMemory(hprocess, (void*)findaddr,&dvalue,sizeof(DOUBLE),&dwwrite);
	}
	else if(fvlaue!=0)
	{
		if(my_WriteProcessMemory)
			bret=my_WriteProcessMemory(hprocess, (void*)findaddr,&fvlaue,sizeof(FLOAT),&dwwrite);
		else
			bret=::WriteProcessMemory(hprocess, (void*)findaddr,&fvlaue,sizeof(FLOAT),&dwwrite);
	}

	if(my_VirtualProtectEx)
		bret=my_VirtualProtectEx(hprocess,(void*)findaddr,sizeof(DOUBLE), dwback, &dwback);
	else
		bret=::VirtualProtectEx(hprocess,(void*)findaddr, sizeof(DOUBLE), dwback, &dwback);

	CloseHandle(hprocess);
	return bret;
}

bool TSMemoryAPI::TSWriteInt(LONG hwnd,wchar_t *addr,int ivalue,short svalue,BYTE bvalue)
{
	bool bret=false;

	DWORD pid=0;
	//if(my_GetWindowThreadProcessId)
	//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
	//else
	My_GetWindowThreadProcessId((HWND)hwnd,&pid);

	DWORD findaddr=TSGetFindDataAddr(addr,pid);
	if(findaddr==-1)
		return bret;

	TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
	HANDLE hprocess=NULL;
	//if(my_OpenProcess)
	//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	//else
	//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	hprocess=My_OpenProcess(pid);

	if(hprocess==NULL)
	{
		if(my_ZwOpenProcess)
		{
			CLIENT_ID  Cileid;
			Cileid.UniqueProcess=(HANDLE)pid;
			my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
		}
		if(hprocess==NULL)
			return bret;
	}
	DWORD dwwrite=0;

	DWORD dwback;
	if(my_VirtualProtectEx)
		my_VirtualProtectEx(hprocess,(void*)findaddr, sizeof(int), PAGE_EXECUTE_READWRITE, &dwback);
	else
		::VirtualProtectEx(hprocess,(void*)findaddr, sizeof(int), PAGE_EXECUTE_READWRITE, &dwback);

	if(ivalue!=0)
	{
		if(my_WriteProcessMemory)
			bret=my_WriteProcessMemory(hprocess, (void*)findaddr,&ivalue,sizeof(int),&dwwrite);
		else
			bret=::WriteProcessMemory(hprocess, (void*)findaddr,&ivalue,sizeof(int),&dwwrite);
	}
	else if(svalue!=0)
	{
		if(my_WriteProcessMemory)
			bret=my_WriteProcessMemory(hprocess, (void*)findaddr,&svalue,sizeof(short),&dwwrite);
		else
			bret=::WriteProcessMemory(hprocess, (void*)findaddr,&svalue,sizeof(short),&dwwrite);
	}
	else if(bvalue!=0)
	{
		if(my_WriteProcessMemory)
			bret=my_WriteProcessMemory(hprocess, (void*)findaddr,&bvalue,sizeof(BYTE),&dwwrite);
		else
			bret=::WriteProcessMemory(hprocess, (void*)findaddr,&bvalue,sizeof(BYTE),&dwwrite);
	}

	if(my_VirtualProtectEx)
		bret= my_VirtualProtectEx(hprocess,(void*)findaddr, sizeof(int), dwback, &dwback);
	else
		bret=::VirtualProtectEx(hprocess,(void*)findaddr, sizeof(int), dwback, &dwback);

	CloseHandle(hprocess);
	return bret;
}

bool TSMemoryAPI::TSWriteString(LONG hwnd,wchar_t*addr,wchar_t* strvalue,LONG type)
{
	bool bret=false;

	int len=wcslen(strvalue);
	if(len==0||len>MAX_PATH)
		return bret;

	DWORD pid=0;
	//if(my_GetWindowThreadProcessId)
	//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
	//else
	My_GetWindowThreadProcessId((HWND)hwnd,&pid);

	DWORD findaddr=TSGetFindDataAddr(addr,pid);
	if(findaddr==-1)
		return bret;

	TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
	HANDLE hprocess=NULL;
	//if(my_OpenProcess)
	//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	//else
	//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	hprocess=My_OpenProcess(pid);

	if(hprocess==NULL)
	{
		if(my_ZwOpenProcess)
		{
			CLIENT_ID  Cileid;
			Cileid.UniqueProcess=(HANDLE)pid;
			my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
		}
		if(hprocess==NULL)
			return bret;
	}
	DWORD dwwrite=0;

	DWORD dwback;
	if(my_VirtualProtectEx)
		my_VirtualProtectEx(hprocess,(void*)findaddr, len, PAGE_EXECUTE_READWRITE, &dwback);
	else
		::VirtualProtectEx(hprocess,(void*)findaddr, len, PAGE_EXECUTE_READWRITE, &dwback);

	if(type==0)//Ascii字符串
	{
		USES_CONVERSION;
		char strchar[MAX_PATH*2]={0};
		strcpy(strchar,W2A(strvalue));
		int len=strlen(strchar);
		if(my_ReadProcessMemory)
			my_WriteProcessMemory(hprocess, (void*)findaddr,strchar,len,&dwwrite);
		else
			::WriteProcessMemory(hprocess, (void*)findaddr,strchar,len,&dwwrite);
	}
	else  if(type==1)
	{
		if(my_ReadProcessMemory)
			bret= my_WriteProcessMemory(hprocess, (void*)findaddr,strvalue,len,&dwwrite);
		else
			bret= ::WriteProcessMemory(hprocess, (void*)findaddr,strvalue,len,&dwwrite);
	}

	if(my_VirtualProtectEx)
		bret=my_VirtualProtectEx(hprocess,(void*)findaddr, len, dwback, &dwback);
	else
		bret= ::VirtualProtectEx(hprocess,(void*)findaddr, len, dwback, &dwback);

	CloseHandle(hprocess);
	return bret;

}

bool TSMemoryAPI::GetFindaddr(HANDLE hprocess,PVOID  lpbegin,PVOID lpend,DWORD *ibegin,DWORD *ipend,int &ncount)
{
	bool bret=false;
	int index=0;
	SYSTEM_INFO si_info;   
	MEMORY_BASIC_INFORMATION   mbi;
	DWORD   rett;  
	GetSystemInfo(&si_info);   
	DWORD   len   =   sizeof(MEMORY_BASIC_INFORMATION);   
	PVOID   addr   =  si_info.lpMinimumApplicationAddress; 
	bool bfind=false;
	do
	{
		rett   =VirtualQueryEx(hprocess,addr,&mbi,len);
		if(mbi.Type==MEM_IMAGE||mbi.Type==MEM_PRIVATE) //找到了块
		{
			//MessageBox(0,L"找到了块",0,0);
			if(lpbegin==0)
				lpbegin=LPVOID(mbi.AllocationBase);
			else if(bfind==false)
				lpbegin=addr;
			if(lpend==0)
			{
				lpend=si_info.lpMaximumApplicationAddress;
			}
			ibegin[index]=int(lpbegin);
			ipend[index]=int(lpend);
			bfind=true;
			//break;
		}
		else
		{
			if(ibegin!=0&&bfind==true)
			{
				ipend[index]=(DWORD)mbi.BaseAddress + mbi.RegionSize;
				bfind=false;
				index++;
				bret=true;
			}
		}
		addr= ((PBYTE)mbi.BaseAddress + mbi.RegionSize);
	}while(rett==len);

	ncount=index;

	for(int i=0;i<index;i++)
	{
		if(((DWORD)ipend[i]-(DWORD)ibegin[i])>0xF00000)
		{
			DWORD end=ipend[i];
			ipend[i]=ibegin[i]+0xF00000;
			DWORD iend=ipend[i];
			while((end-(DWORD)iend)>0xF00000)
			{
				ibegin[ncount]=iend;
				ipend[ncount]=ibegin[ncount]+0xF00000;
				ncount++;
				iend=iend+0xF00000;
			}
			ibegin[ncount]=iend;
			ipend[ncount]=end;
			ncount++;
		}
	}

	return bret;
}

bool TSMemoryAPI::TSGetCmdLine(LONG hwnd,wchar_t *retstr)
{
	bool bret=false;

	DWORD dwPID=0;
	//if(my_GetWindowThreadProcessId)
	//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
	//else
	My_GetWindowThreadProcessId((HWND)hwnd,&dwPID);

	TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
	HANDLE hprocess=NULL;
	//if(my_OpenProcess)
	//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	//else
	//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
	hprocess=My_OpenProcess(dwPID);
	if(hprocess==NULL)
	{
		if(my_ZwOpenProcess)
		{
			CLIENT_ID  Cileid;
			Cileid.UniqueProcess=(HANDLE)dwPID;
			my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
		}
		if(hprocess==NULL)
			return bret;
	}

	NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(GetModuleHandle(L"ntdll"),"NtQueryInformationProcess");


	PROCESS_BASIC_INFORMATION pbis;
	PEB pebs;
	PROCESS_PARAMETERS ppms;
	TCHAR *text = 0;
	NtQueryInformationProcess(hprocess, 0, &pbis, sizeof(PROCESS_BASIC_INFORMATION),0);

	ReadProcessMemory(hprocess, pbis.PebBaseAddress, &pebs, sizeof(PEB), 0);

	ReadProcessMemory(hprocess, pebs.ProcessParameters, &ppms, sizeof(PROCESS_PARAMETERS), 0);

	text = (TCHAR *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ppms.CommandLine.MaximumLength);

	ReadProcessMemory(hprocess, ppms.CommandLine.Buffer, text, ppms.CommandLine.Length, 0);

	if(text)
	{
		wcscpy(retstr,text);
		bret=true;
	}
	::HeapFree(GetProcessHeap(),HEAP_NO_SERIALIZE,text);
	return bret;
}

bool TSMemoryAPI::TSAsmAdd(wchar_t*asm_ins)
{
	bool bret=false;
	char asmcode[MAX_PATH]={0};
	USES_CONVERSION;
	strcpy(asmcode,W2A(asm_ins));
	char erro[MAX_PATH]={0};
	int len=-1;
	char *charcall=strstr(::CharLowerA(asmcode),"call");
	if(charcall!=NULL)//判断是否是Call指令,记住Call地址
	{
		if(strlen(Asmcalladdr)==0)
			strcpy(Asmcalladdr,charcall);
		return true;
	}
	else
		len= tsasm.Assemble(asmcode,NULL,&am,0,0,erro); //将汇编指令转为机器码

	if(len<0)
		return bret;
	else
		bret=true;

	for(int i=0;i<len;i++)
	{
		char  asmcodes[10]={0};
		if(am.code[i]>=0x10)
			sprintf(asmcodes,"%X",am.code[i]);
		else
			sprintf(asmcodes,"0%X",am.code[i]);
		if(am.code[i]!=0)
			asmcodearry=asmcodearry+asmcodes;//这里做拼接
		else
		{
			int serlen=asmcodearry.length();
			serlen=serlen+1;
			asmcodearry.resize(serlen);
		}
	}
	//int charlen=strlen(asmcode);
	//int serlen=asmcodearry.length();
	//if(charlen<len)
	//	asmcodearry.resize(serlen+(len-charlen));
	return bret;
}

DWORD TSMemoryAPI::GetCallstartData(DWORD Allocaddr,DWORD * startaddr,char *code)//获取CALL地址前的汇编指令
{
	CMgAsmBase::t_asmmodel  tam;
	char erro[MAX_PATH]={0};
	char asmcode[MAX_PATH]={0};
	Allocaddr=Allocaddr+0x1000;
	if(code==NULL)
		sprintf(asmcode,"push %x",Allocaddr);
	else
		sprintf(asmcode,"%s",code);
	LONG l=tsasm.Assemble(asmcode,Allocaddr,&tam,0,0,erro); //将汇编指令转为机器码
	for(int i=0;i<l;i++)
	{
		startaddr[i]=tam.code[i];
	}
	return l;
}


bool TSMemoryAPI::TSAsmCall(LONG hwnd,LONG mode)
{
	bool bret=false;
	int len=asmcodearry.length();
	if(strlen(Asmcalladdr)<=0)
		return bret;

	DWORD dwPID=0;
	DWORD treadid=0;
	//if(my_GetWindowThreadProcessId)
	//	my_GetWindowThreadProcessId((HWND)hwnd,&Pid);
	//else
	My_GetWindowThreadProcessId((HWND)hwnd,&dwPID);

	TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
	HANDLE hprocess=NULL;
	if(mode==1)	//1 : 对hwnd指定的进程内执行,注入模式为创建远程线程
	{
		//if(my_OpenProcess)
		//	hprocess=my_OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
		//else
		//	hprocess=::OpenProcess(PROCESS_ALL_ACCESS,false,Pid);
		hprocess=My_OpenProcess(dwPID);
		if(hprocess==NULL)
		{
			if(my_ZwOpenProcess)
			{
				CLIENT_ID  Cileid;
				Cileid.UniqueProcess=(HANDLE)dwPID;
				my_ZwOpenProcess(&hprocess,PROCESS_ALL_ACCESS,false,&Cileid);
			}
			if(hprocess==NULL)
				return bret;
		}
	}
	else if(mode==0) //0 : 在本进程中进行执行，这时hwnd无效
	{
		hprocess=::GetCurrentProcess();
		treadid=GetCurrentThreadId();
	}
	else
	{
		return bret;
	}
	if(allocatememory)
	{
		::VirtualFreeEx(hprocess,allocatememory,0,MEM_RELEASE);
		allocatememory=NULL;
	}

	allocatememory=::VirtualAllocEx(hprocess,NULL,0x2000,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE);

	if(allocatememory)
	{
		char retstring[MAX_PATH*10]={0};
		int retstrlen=0;
		if(len!=0)
		{
			//asmcodearry="9C60"+asmcodearry;//pushfd,pushad;保持堆栈平衡
			len=asmcodearry.length();
			for(int i=0;i<len;i++)
			{
				int stringlen=0;
				{
					stringlen=strlen(&asmcodearry.c_str()[i]);
					if(stringlen>0)
					{
						LONG val=::strtol(&asmcodearry.c_str()[i],0,16);
						if(val>=0x10)
						{
							if(val<=0xff)
							{
								if(retstrlen==0)
									retstrlen=strlen(retstring);
								if(retstrlen==0)
									sprintf(retstring,"%s",&asmcodearry.c_str()[i]);
								else
									sprintf(retstring,"%s %s",retstring,&asmcodearry.c_str()[i]);
							}

							else
							{
								int len=strlen(&asmcodearry.c_str()[i]);
								char str[16]={0};
								strcpy(str,&asmcodearry.c_str()[i]);
								for(int n=0;n<len;n=n+2)
								{
									char nstr[2]={0};
									strncpy(nstr,&str[n],2);
									int strn=strtol(nstr,0,16);
									if(n+2!=len||n+2<len)
									{
										if(retstrlen==0)
											retstrlen=strlen(retstring);
										if(retstrlen==0)
										{
											if(strn>=0x10)
												sprintf(retstring,"%X",strn);
											else
												sprintf(retstring,"0%X",strn);
										}

										else
										{
											if(strn>=0x10)
												sprintf(retstring,"%s %X",retstring,strn);
											else
												sprintf(retstring,"%s 0%X",retstring,strn);
										}
									}
									else
									{
										if(len%2==0)
										{
											if(strn>=0x10)
												sprintf(retstring,"%s %X",retstring,strn);
											else
												sprintf(retstring,"%s 0%X",retstring,strn);
										}
										else
										{
											if(strn>=0x10)
												sprintf(retstring,"%s %X",retstring,strn);
											else
												sprintf(retstring,"%s 0%X",retstring,strn);
										}
									}

								}

							}
						}
						else
							sprintf(retstring,"%s 0%s",retstring,&asmcodearry.c_str()[i]);

						i=i+stringlen-1;
					}

					else
						sprintf(retstring,"%s 00",retstring);
				}
			}
		}
		DWORD datas[MAX_PATH*10]={0};
		int count=0;
		int lenth=0;
		if(strlen(retstring)>0)
		{
			DWORD data[10]={0};
			int len=GetCallstartData((DWORD)allocatememory,data);
			for (int i=0;i<len;i++)
			{
				datas[i] = data[i];
			}
			memset(data,0,len);
			int l=len;
			lenth=lenth+len;
			len=GetCallstartData((DWORD)allocatememory+l,data,"push dword ptr fs:[0]");
			for (int i=0;i<len;i++)
			{
				datas[lenth+i] = data[i];
			}
			memset(data,0,len);
			l=len;
			lenth=lenth+len;
			len=GetCallstartData((DWORD)allocatememory+l,data,"mov dword ptr fs:[0],esp");
			for (int i=0;i<len;i++)
			{
				datas[lenth+i] = data[i];
			}
			lenth=lenth+len;

			USES_CONVERSION;
			wchar_t wretsting[MAX_PATH*10]={0};
			wcscpy(wretsting,A2W(retstring));
			TSGetDataValue(wretsting,&datas[lenth],count);

		}

		BYTE helpByte[MAX_PATH*10] = {0};
		for (int i=0;i<count+lenth;i++)
		{
			helpByte[i] = datas[i];
		}

		char erro[MAX_PATH]={0};
		int calllen= tsasm.Assemble(Asmcalladdr,((DWORD)allocatememory+count+lenth),&am,0,0,erro); //将汇编指令转为机器码
		if(calllen>0)
		{
			memcpy(&helpByte[count+lenth],am.code,calllen);
			calllen=calllen+count+lenth;

			if(count!=0)
			{
				BYTE CallendData[]={0x8b,0x1c,0x24,0x64,0x89,0x1d,0x0,0x0,0x0,0x0,0x81,0xc4,0x08,0x0,0x0,0x0,0xc2,0x04,0x0};
				//CallendData存储的机器码对应汇编指令
				//mov ebx,dword ptr ss:[esp]
				//mov dword ptr fs:[0],ebx
				//add esp,0x8
				//retn 0x4;
				memcpy(&helpByte[calllen],CallendData,sizeof(CallendData));

				calllen=calllen+sizeof(CallendData)+1;
			}

			if(my_WriteProcessMemory)
				bret=my_WriteProcessMemory(hprocess, (void*)allocatememory,(void*)helpByte,calllen,0);
			else
				bret=::WriteProcessMemory(hprocess, (void*)allocatememory,(void*)helpByte,calllen,0);

			HANDLE hthread=NULL;

			////判断是否DX绑定窗口
			//char pszMapName[MAX_PATH]={0};
			//DWORD Pid=0;
			//My_GetWindowThreadProcessId((HWND)hwnd,&Pid);
			//sprintf( pszMapName,"%s%d",TS_MAPVIEW_NAME,Pid);
			//HANDLE hFileMap = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, pszMapName);
			//////如果hFileMap句柄不为空说明DLL绑定注入
			//if(hFileMap!=NULL)  //说明已经DX绑定
			//	hthread=(HANDLE)::SendMessage((HWND)hwnd,TS_ASMCALL,(WPARAM)allocatememory,0);
			//else
			hthread=::CreateRemoteThread(hprocess,NULL,0,(LPTHREAD_START_ROUTINE)(allocatememory),0,0,&treadid);

			//CloseHandle(hFileMap);
			if(hthread!=NULL)
				bret=true;
			DWORD ExitCode=0;
			//Sleep(50);//等待50毫秒，等待线程执行完毕
			::GetExitCodeThread(hthread,&ExitCode);
			if(ExitCode!=STILL_ACTIVE)//判断线程是否退出
				if(allocatememory)
				{
					::VirtualFreeEx(hprocess,allocatememory,0,MEM_RELEASE);
					allocatememory=NULL;
				}

		}

	}

	asmcodearry.clear();
	memset(Asmcalladdr,0,MAX_PATH);

	return bret;
}

bool TSMemoryAPI::TSAsmClear()
{
	asmcodearry.clear();
	memset(Asmcalladdr,0,MAX_PATH);

	return true;
}

bool TSMemoryAPI::TSAsmCode(LONG base_addr,wchar_t *retstr)
{
	bool bret=false;
	int len=asmcodearry.length();
	char retstring[MAX_PATH*10]={0};
	int retstrlen=0;
	for(int i=0;i<len;i++)
	{
		int stringlen=0;
		{
			stringlen=strlen(&asmcodearry.c_str()[i]);
			if(stringlen>0)
			{
				LONG val=::strtol(&asmcodearry.c_str()[i],0,16);
				if(val>=0x10)
				{
					if(val<=0xff)
					{
						if(retstrlen==0)
							retstrlen=strlen(retstring);
						if(retstrlen==0)
							sprintf(retstring,"%s",&asmcodearry.c_str()[i]);
						else
							sprintf(retstring,"%s %s",retstring,&asmcodearry.c_str()[i]);
					}

					else
					{
						int len=strlen(&asmcodearry.c_str()[i]);
						char str[MAX_PATH]={0};
						strcpy(str,&asmcodearry.c_str()[i]);
						for(int n=0;n<len;n=n+2)
						{
							char nstr[2]={0};
							strncpy(nstr,&str[n],2);
							int strn=strtol(nstr,0,16);
							if(n+2!=len||n+2<len)
							{
								if(retstrlen==0)
									retstrlen=strlen(retstring);
								if(retstrlen==0)
								{
									if(strn>=0x10)
										sprintf(retstring,"%X",strn);
									else
										sprintf(retstring,"0%X",strn);
								}

								else
								{
									if(strn>=0x10)
										sprintf(retstring,"%s %X",retstring,strn);
									else
										sprintf(retstring,"%s 0%X",retstring,strn);
								}
							}
							else
							{
								if(len%2==0)
								{
									if(strn>=0x10)
										sprintf(retstring,"%s %X",retstring,strn);
									else
										sprintf(retstring,"%s 0%X",retstring,strn);
								}
								else
								{
									if(strn>=0x10)
										sprintf(retstring,"%s %X",retstring,strn);
									else
										sprintf(retstring,"%s 0%X",retstring,strn);
								}
							}

						}

					}
				}
				else
					sprintf(retstring,"%s 0%s",retstring,&asmcodearry.c_str()[i]);

				i=i+stringlen-1;
			}

			else
				sprintf(retstring,"%s 00",retstring);
		}
		bret=true;
	}
	if(strlen(Asmcalladdr)!=0)
	{
		char erro[MAX_PATH]={0};
		int calllen=tsasm.Assemble(Asmcalladdr,base_addr,&am,0,0,erro); //将汇编指令转为机器码
		for(int i=0;i<calllen;i++)
		{
			if(strlen(retstring)==0)
			{
				if(am.code[i]>=0)
					if(am.code[i]>=0x10)
						sprintf(retstring,"%X",am.code[i]);
					else
						sprintf(retstring,"%s 0%X",retstring,am.code[i]);
				else
				{
					LONG val=am.code[i]-0xffffff00;
					if(val>=0x10)
						sprintf(retstring,"%s %X",retstring,val);
					else
						sprintf(retstring,"%s 0%X",retstring,val);

				}
			}
			else
			{
				if(am.code[i]>=0)
					if(am.code[i]>=0x10)
						sprintf(retstring,"%s %X",retstring,am.code[i]);
					else
						sprintf(retstring,"%s 0%X",retstring,am.code[i]);
				else
				{
					LONG val=am.code[i]-0xffffff00;
					if(val>=0x10)
						sprintf(retstring,"%s %X",retstring,val);
					else
						sprintf(retstring,"%s 0%X",retstring,val);

				}
			};
			bret=true;
		}
	}
	USES_CONVERSION;
	wcscpy(retstr,A2W(retstring));

	return bret;
}

bool TSMemoryAPI::TSAssemble(wchar_t * asm_code,LONG base_addr,LONG is_upper,wchar_t *retstr)
{
	bool bret=false;

	if(wcslen(asm_code)<=0)
		return bret;
	CMgDisasmBase::t_disasm da;
	tsdsm.m_nIDEAL=0; tsdsm.m_nLowercase=is_upper; tsdsm.m_nPutDefSeg=0;

	DWORD datas[MAX_PATH*10]={0};
	int count=0;
	TSGetDataValue(asm_code,datas,count);
	if(count<=0)
		return bret;

	BYTE helpByte[MAX_PATH*10] = {0};
	for (int i=0;i<count;i++)
	{
		helpByte[i] = datas[i];
	}

	ulong l=tsdsm.Disasm((char*)helpByte,count,0,&da,DISASM_CODE);
	USES_CONVERSION;
	wcscpy(retstr,A2W(da.result));

	return bret;
}

bool TSMemoryAPI::TSFreeProcessMemory(LONG hwnd)
{
	DWORD nPid=0;
	My_GetWindowThreadProcessId((HWND)hwnd,&nPid);
	TSRuntime::EnablePrivilege(L"SeDebugPrivilege",true);
	HANDLE hprocess=NULL;
	hprocess=My_OpenProcess(nPid);

	SetProcessWorkingSetSize(hprocess,-1,-1);

	//内存整理
	EmptyWorkingSet(hprocess);
	CloseHandle(hprocess);
	return true;
}