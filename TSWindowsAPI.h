#pragma once

class TSWindowsAPI
	{
	public:
		TSWindowsAPI(void);
		~TSWindowsAPI(void);


	public:
		int retstringlen;
		DWORD WindowVerion;
		DWORD IsEuemprosuccess;
		DWORD npid[MAX_PATH];
		bool TSEnumWindow(HWND parent,wchar_t *title,wchar_t *class_name,LONG filter,wchar_t *retstring,wchar_t  *process_name=NULL );
		bool TSEnumWindowSuper(wchar_t *spec1,LONG flag1,LONG type1,wchar_t *spec2,LONG flag2,LONG type2,LONG sort,wchar_t *retstring=NULL);
		bool TSEnumProcess(wchar_t *name,wchar_t *retstring);
		bool TSClientToScreen(LONG hwnd,LONG &x,LONG &y);
		bool TSFindWindow(wchar_t *class_name,wchar_t*title,LONG &rethwnd,DWORD parent=0);
		bool TSFindWindowByProcess(wchar_t *class_name,wchar_t *titl,LONG &rethwnd,wchar_t *process_name=NULL,DWORD Pid=0);
		bool TSGetClientRect(LONG hwnd,LONG &x,LONG &y,LONG &x1, LONG &y1);
		bool TSGetClientSize(LONG hwnd,LONG &width,LONG &height);
		bool TSGetMousePointWindow(LONG &rethwnd,LONG x=-1,LONG y=-1);
		bool TSGetProcessInfo(LONG pid,wchar_t *retstring);
		bool TSGetWindow(LONG hwnd,LONG flag,LONG &rethwnd);
		bool GetProcesspath(DWORD ProcessID,wchar_t* process_path);
		bool TSGetWindowState(LONG hwnd,LONG flag);
		bool TSSendPaste(LONG hwnd);
		bool TSSetWindowSize(LONG hwnd, LONG width, LONG hight,int type=0);
		bool TSSetWindowState(LONG hwnd,LONG flag,LONG rethwnd=0);
		bool TSSetWindowTransparent(LONG hwnd,LONG trans);
		bool TSSetClipboard(wchar_t *values);
		bool TSGetClipboard(wchar_t *retstr);
	private:
		DWORD  FindChildWnd(HWND hchile, wchar_t *title,wchar_t *classname,wchar_t *retstring,bool isGW_OWNER=false,bool isVisible=false,wchar_t  *process_name=NULL);
		BOOL   EnumProcessbyName(DWORD   dwPID,   LPCWSTR   ExeName,LONG type=0) ;
		int GetProcessNumber();//获取CPU个数
		// 时间格式转换
		__int64 FileTimeToInt64(const FILETIME& time);
		double get_cpu_usage(DWORD ProcessID);	 //获取指定进程CPU使用率
		DWORD GetMemoryInfo(DWORD ProcessID);  //或者指定进程内存使用率
		

	};
