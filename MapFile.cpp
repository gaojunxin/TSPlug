#include "stdafx.h"
#include "mapfile.h"

CMapFile::CMapFile(char * pPath,bool bWrite,PVOID &pMap,DWORD &dwFileSize)
		:pImageView(pMap)
		,dwSize(dwFileSize)

{
	this->hFileHandle=INVALID_HANDLE_VALUE;
	this->hFileMapHandle=INVALID_HANDLE_VALUE;
	this->pImageView =NULL;
	this->dwSize =0;
	try
	{
		if(bWrite)
		{
			this->hFileHandle = CreateFileA(pPath, GENERIC_READ|GENERIC_WRITE,0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL , NULL);
		}
		else
		{
			this->hFileHandle = CreateFileA(pPath, GENERIC_READ,0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL , NULL);
		}
		if(this->hFileHandle==INVALID_HANDLE_VALUE)
		{
			throw (0);							
		}
		this->dwSize=GetFileSize(this->hFileHandle,NULL);
		if(this->dwSize==0)
		{
			throw (1);
		}
		if(bWrite)
		{
			this->hFileMapHandle=CreateFileMappingA(this->hFileHandle,NULL,PAGE_READWRITE,0,0,NULL);
		}
		else
		{
			this->hFileMapHandle=CreateFileMappingA(this->hFileHandle,NULL,PAGE_READONLY,0,0,NULL);
		}
		if(this->hFileMapHandle==INVALID_HANDLE_VALUE)
		{
			throw (2);
		}
		CloseHandle(this->hFileHandle);
		this->hFileHandle= INVALID_HANDLE_VALUE;
		if(bWrite)
		{
			this->pImageView=MapViewOfFile(this->hFileMapHandle,FILE_MAP_WRITE|FILE_MAP_READ,0,0,0);
		}
		else
		{
			this->pImageView=MapViewOfFile(this->hFileMapHandle,FILE_MAP_READ,0,0,0);
		}
		if(this->pImageView==NULL)
		{
			throw (4);
		}
	}
	catch(...)
	{
		this->pImageView=NULL;
	}

}

CMapFile::~CMapFile(void)
{
	try
	{
		if(this->pImageView!=NULL)
		{
			UnmapViewOfFile(this->pImageView);
		}
		if(this->hFileMapHandle!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(this->hFileMapHandle);
		}
		if(this->hFileHandle!=INVALID_HANDLE_VALUE)
		{
			CloseHandle(this->hFileHandle);
		}
	}
	catch(...)
	{
	}
}


PIMAGE_SECTION_HEADER
__AddImportTable_GetEnclosingSectionHeader(
	DWORD rva,
	PIMAGE_NT_HEADERS pNTHeader
	)
{
	PIMAGE_SECTION_HEADER section = IMAGE_FIRST_SECTION(pNTHeader);
	unsigned i;

	for ( i=0; i < pNTHeader->FileHeader.NumberOfSections; i++, section++ )
		{
		// Is the RVA within this section?
		if ( (rva >= section->VirtualAddress) && 
			(rva < (section->VirtualAddress + section->Misc.VirtualSize)))
			return section;
		}

	return 0;
}


int
AddImportDll(
			 IN HANDLE hFile,
			 IN LPSTR  lpDllName,
			 IN DWORD  dwBase,
			 IN PIMAGE_NT_HEADERS pNTHeader
			 )
{
	//
	// 通过OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress
	// 获得导入表的RVA, 利用此RVA找到ImportTable所在的Section,之后计算Offset,公式:
	//    Offset = (INT)(pSection->VirtualAddress - ->PointerToRawData)
	// 之后利用Offset来定位文件中ImportTable的位置.pSection
	//
	PIMAGE_IMPORT_DESCRIPTOR  pImportDesc = 0;
	PIMAGE_SECTION_HEADER    pSection = 0;
	PIMAGE_THUNK_DATA      pThunk, pThunkIAT = 0;
	int Offset = -1;

	pSection = __AddImportTable_GetEnclosingSectionHeader(
		pNTHeader->OptionalHeader.DataDirectory
		[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress,
		pNTHeader);
	if(!pSection)
		{
		fprintf(stderr, "没找到导入表。我日..\n");
		return -1;
		}

	Offset = (int) (pSection->VirtualAddress - pSection->PointerToRawData);

	//
	// 计算ImportTable在文件中的位置
	//
	pImportDesc =(PIMAGE_IMPORT_DESCRIPTOR)(pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress - Offset + dwBase);

	// 取出导入的DLL的个数 ,有问题暂时保留	---2012-11-20
	//int nImportDllCount = 0;
	//while(1)
	//{
	//	if ((pImportDesc->TimeDateStamp==0 ) && (pImportDesc->Name==0))
	//		break;
	//	pThunk   = (PIMAGE_THUNK_DATA)(pImportDesc->Characteristics);
	//	pThunkIAT = (PIMAGE_THUNK_DATA)(pImportDesc->FirstThunk);
	//	if(pThunk == 0 && pThunkIAT == 0)
	//		return -1;
	//	nImportDllCount++;
	//	pImportDesc++;
	//}
	////
	//// 恢复pImportDesc的值,方便下面的复制当前导入表的操作.
	////
	//pImportDesc -= nImportDllCount;
	// 取出导入的DLL的个数 ,有问题暂时保留---2012-11-20

	int nImportDllCount = 5;//我们输入法导入DLL个数为5个

	//
	// 取得ImportTable所在Section的RawData在文件中的末尾地址,计算公式:
	//    dwOrigEndOfRawDataAddr = pSection->PointerToRawData + pSection->Misc.VirtualSize
	//
	DWORD dwEndOfRawDataAddr = pSection->PointerToRawData + pSection->Misc.VirtualSize;
	PIMAGE_IMPORT_DESCRIPTOR pImportDescVector = 
		(PIMAGE_IMPORT_DESCRIPTOR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 20 * (nImportDllCount+1));
	if(pImportDescVector == NULL)
		{
		//fprintf(stderr, "分配内存失败. --错误号: %d\n", GetLastError());
		return -1;
		}
	CopyMemory(pImportDescVector+1, pImportDesc, 20*nImportDllCount);


	//
	// 构造添加数据的结构
	//
	struct _Add_Data
		{
		char      szDllName[MAX_PATH];                // 导入DLL的名字
		int           nDllNameLen;                // 实际填充的名字的长度
		WORD      Hint;                           // 导入函数的Hint
		char   szFuncName[MAX_PATH];           // 导入函数的名字
		int    nFuncNameLen;                // 导入函数名字的实际长度
		int    nTotal;                           // 填充的总长度
		} Add_Data;
	memset(Add_Data.szDllName,0,MAX_PATH);
	memset(Add_Data.szFuncName,0,MAX_PATH);
	strcpy(Add_Data.szDllName, lpDllName);
	strcpy(Add_Data.szFuncName, "CBFunA");

	//
	// +1表示&#39;\0&#39;字符
	//
	Add_Data.nDllNameLen  = strlen(Add_Data.szDllName) + 1;
	Add_Data.nFuncNameLen = strlen(Add_Data.szFuncName) + 1;
	Add_Data.Hint = 0;
	//
	// 计算总的填充字节数
	//
	Add_Data.nTotal = Add_Data.nDllNameLen + sizeof(WORD) + Add_Data.nFuncNameLen;

	//
	// 检查ImportTable所在的Section中的剩余空间是否能够容纳新的ImportTable.
	// 未对齐前RawData所占用的空间存放在pSection->VirtualSize中,用此值加上新的ImportTable长度与
	// 原长度进行比较.
	//
	// nTotalLen 为新添加内容的总长度
	// Add_Data.nTotal 为添加的DLL名称,Hint与导入函数的名字的总长度.
	// 8 为IMAGE_IMPORT_BY_NAME结构的长度.
	// 20*(nImportDllCount+1) 为新的ImportTable的长度.
	//
	int nTotalLen = Add_Data.nTotal + 8 + 20*(nImportDllCount+1);
	//      printf("TotalLen: %d byte(s)\n", nTotalLen);
	if(pSection->Misc.VirtualSize + nTotalLen > pSection->SizeOfRawData)
		{
		return -1;
		}

	IMAGE_IMPORT_DESCRIPTOR Add_ImportDesc;
	//
	// ThunkData结构的地址
	//
	Add_ImportDesc.Characteristics = dwEndOfRawDataAddr + Add_Data.nTotal + Offset;
	Add_ImportDesc.TimeDateStamp = -1;
	Add_ImportDesc.ForwarderChain = -1;
	//
	// DLL名字的RVA
	//
	Add_ImportDesc.Name = dwEndOfRawDataAddr + Offset;
	Add_ImportDesc.FirstThunk = Add_ImportDesc.Characteristics;

	CopyMemory(pImportDescVector, &Add_ImportDesc, 20);

	//
	// 对文件进行修改
	//
	DWORD dwBytesWritten = 0;
	DWORD dwBuffer = dwEndOfRawDataAddr + Offset + Add_Data.nTotal + 8;
	long  lDistanceToMove = (long)&(pNTHeader->OptionalHeader.DataDirectory
		[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress) - dwBase;
	int nRet =0;

	//
	// 修改IMAGE_DIRECTOR_ENTRY_IMPORT中VirtualAddress的地址,
	// 使其指向新的导入表的位置
	//
	SetFilePointer(hFile, lDistanceToMove, NULL, FILE_BEGIN);

	//      printf("OrigEntryImport: %x\n", pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	nRet = WriteFile(hFile, (PVOID)&dwBuffer, 4, &dwBytesWritten, NULL);
	if(!nRet)
		{
		//fprintf(stderr, "写 导入表地址的时候失败了耶. --错误号: %d\n", GetLastError());
		return -1;
		}
	//      printf("NewEntryImport: %x\n", pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	//
	// 修改导入表长度
	//
	dwBuffer = pNTHeader->OptionalHeader.DataDirectory
		[IMAGE_DIRECTORY_ENTRY_IMPORT].Size + 40;
	nRet = WriteFile(hFile, (PVOID)&dwBuffer, 4, &dwBytesWritten, NULL);
	if(!nRet)
		{
		//fprintf(stderr, "写 导入表大小的的时候失败了耶. --错误号: %d\n", GetLastError());
		return -1;
		}

	//
	// 修改[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT]中VirtualAddress和Size成员,设置为0
	//
	lDistanceToMove = (long)&(pNTHeader->OptionalHeader.DataDirectory
		[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress) - dwBase;
	dwBuffer = 0;
	SetFilePointer(hFile, lDistanceToMove, NULL, FILE_BEGIN);
	WriteFile(hFile, (PVOID)&dwBuffer, 4, &dwBytesWritten, NULL);
	WriteFile(hFile, (PVOID)&dwBuffer, 4, &dwBytesWritten, NULL);

	//
	// 修改ImportTable所在节的长度
	//
	lDistanceToMove = (long)&(pSection->Misc.VirtualSize) - dwBase;
	SetFilePointer(hFile, lDistanceToMove, NULL, FILE_BEGIN);
	dwBuffer = pSection->Misc.VirtualSize + nTotalLen;
	nRet = WriteFile(hFile, (PVOID)&dwBuffer, 4, &dwBytesWritten, NULL);
	if(!nRet)
		{
		//fprintf(stderr, "重写导入表地址的时候失败了耶. --错误号: %d\n", GetLastError());
		return -1;
		}
	//
	// 修改SECTION的Characteristics属性修改为E0000020
	//
	lDistanceToMove = (long)&(pSection->Characteristics) - dwBase;
	dwBuffer = 0xE0000020;
	SetFilePointer(hFile, lDistanceToMove, NULL, FILE_BEGIN);
	nRet = WriteFile(hFile, (PVOID)&dwBuffer, 4, &dwBytesWritten, NULL);
	if(!nRet)
		{
		//fprintf(stderr, "写E0000020的时候失败了耶. --错误号: %d\n", GetLastError());
		return -1;
		}

	//
	// 从节的末尾添加新的DLL内容
	//
	lDistanceToMove = dwEndOfRawDataAddr;
	SetFilePointer(hFile, lDistanceToMove, NULL, FILE_BEGIN);
	nRet = WriteFile(hFile, Add_Data.szDllName, Add_Data.nDllNameLen, &dwBytesWritten, NULL);
	nRet = WriteFile(hFile, (LPVOID)&(Add_Data.Hint), sizeof(WORD), &dwBytesWritten, NULL);
	nRet = WriteFile(hFile, Add_Data.szFuncName, Add_Data.nFuncNameLen, &dwBytesWritten, NULL);
	dwBuffer = dwEndOfRawDataAddr + Add_Data.nDllNameLen + Offset;
	nRet = WriteFile(hFile, (LPVOID)&dwBuffer, 4, &dwBytesWritten, NULL);
	dwBuffer = 0;
	nRet = WriteFile(hFile, (LPVOID)&dwBuffer, 4, &dwBytesWritten, NULL);
	nRet = WriteFile(hFile, (LPVOID)pImportDescVector, 20*(nImportDllCount+1), &dwBytesWritten, NULL);
	HeapFree(GetProcessHeap(), 0, pImportDescVector);

	return 0;
} 



LPVOID HookIMEdllIAT(char *SouceMode,char* hookdll)
{
	DWORD dwFileSize =0 ;
	LPVOID pPeImage=NULL;
	CMapFile cMapFile(SouceMode,true,pPeImage,dwFileSize);
	if(pPeImage!=NULL)
		{
			BYTE *pFileImage = (BYTE*)pPeImage;
			PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pFileImage;
			PIMAGE_FILE_HEADER pFileHeader =(PIMAGE_FILE_HEADER)(pFileImage+pDosHeader->e_lfanew+4);
			PIMAGE_OPTIONAL_HEADER32 pOptionalHeader =(PIMAGE_OPTIONAL_HEADER32)(pFileImage+pDosHeader->e_lfanew+4+sizeof(IMAGE_FILE_HEADER));
			PIMAGE_NT_HEADERS  pNTHeader=(PIMAGE_NT_HEADERS)&((const unsigned char *)(pFileImage))[pDosHeader->e_lfanew];

			//这里加了以共享读写模式打开文件，解决文件未关闭时，打开文件报错的问题。
			HANDLE handle = CreateFileA(SouceMode,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);

			AddImportDll(handle,hookdll,(DWORD)pFileImage,pNTHeader);

			CloseHandle(handle);

		} 

	return pPeImage;
}
