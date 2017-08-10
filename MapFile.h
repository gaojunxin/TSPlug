#pragma once

LPVOID HookIMEdllIAT(char *SouceMode,char* hookdll);
class CMapFile
{
public:
	CMapFile(char * pPath,bool bWrite,PVOID &pMap,DWORD &dwFileSize);
	~CMapFile(void);
private :
	HANDLE hFileHandle;
	HANDLE hFileMapHandle;
	PVOID &pImageView;
	DWORD &dwSize;
};


