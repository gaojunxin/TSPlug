//#if !defined(TSMYUSER32DLLFUNTION_H)
//#define TSMYUSER32DLLFUNTION_H

#pragma once


typedef bool (_stdcall * myGetCursorPos)( LPPOINT lpPoint);


typedef bool (_stdcall * myClientToScreen)(HWND hWnd,LPPOINT lpPoint);


typedef bool (_stdcall * myScreenToClient)(  HWND hWnd,LPPOINT lpPoint);


typedef HWND (_stdcall *myGetWindow)(HWND hWnd,UINT uCmd ); 


typedef HWND (_stdcall *myFindWindow)(   LPCTSTR lpClassName,   LPCTSTR lpWindowName );


typedef HWND (_stdcall *myWindowFromPoint)( POINT Point );


typedef LONG (_stdcall *myGetWindowLong)( HWND hWnd, int nIndex);


typedef HWND (_stdcall *myFindWindowEx)( HWND hwndParent,HWND hwndChildAfter,LPCTSTR lpszClass,LPCTSTR lpszWindow);


typedef LRESULT (_stdcall *mySendMessage)(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam );


typedef bool (_stdcall *myPostMessage)(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam );


typedef DWORD (_stdcall *myGetWindowThreadProcessId)(  HWND hWnd,   LPDWORD lpdwProcessId );

typedef BOOL (_stdcall *myIsWindow)( HWND hWnd );

bool InitialMyUser32Dll();



//#endif

