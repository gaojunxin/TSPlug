// TSPlugInterFace.cpp : CTSPlugInterFace 的实现
/*
本源码由TC简单软件科技有限公司开源,功能可以自由修改、发布、
长沙简单软件科技有限公司对于源码不做后期维护,,请大家在使用过程中遵循开源协议
*/

#include "stdafx.h"
#include "TSPlugInterFace.h"
#include "TSRuntime.h"
#include "DXBind.h"
#include "TsMessage.h"
#include "D3dx9tex.h"
#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")
#include "dllmain.h"
#include "TSMyKernel32DllFuntion.h"
#include "Wininet.h"
#pragma comment(lib,"Wininet.lib")


//绑定互斥
TsMutex gDXbindMutex(TS_BINDEVEN);
//解绑定互斥
TsMutex gDXUnbindMutex(TS_UNBINDEVEN);
//按键按下互斥
TsMutex gDXKeyDownMutex(TS_KEYDOWNEVEN);
//按键弹起互斥
TsMutex gDXKeyUpMutex(TS_KEYUPEVEN);
//按键互斥
TsMutex gDXKeyPressMutex(TS_KEYPRESSEVEN);
//KeyPressStr互斥
TsMutex gDXKeyPressStrMutex(TS_KEYPRESSSTREVEN);
//KeyPressChar互斥
TsMutex gDXKeyPressCharMutex(TS_KEYPRESSCHAREVEN);
//SendString互斥
TsMutex gDXSendStringMutex(TS_SENDSTRINGEVEN);
//SendString互斥
TsMutex gDXSendString2Mutex(TS_SENDSTRING2EVEN);
//SendStringIme互斥
TsMutex gDXSendStringImeMutex(TS_SENDSTRINGIMEEVEN);
//LeftDown互斥		  
TsMutex gDXLeftDownMutex(TS_LEFTDOWNEVEN);
//LeftUp互斥		  
TsMutex gDXLeftUpMutex(TS_LEFTUPEVEN);
//LeftClick互斥		  
TsMutex gDXLeftClickMutex(TS_MOVETOEVEN);
//MoveTo互斥		  
TsMutex gDXMoveToMutex(TS_LEFTCLICKEVEN);
//FindPic互斥		  
TsMutex gDXFindPicMutex(TS_FINDPICEVEN);
////RightClick互斥		  
TsMutex gDXRightClickMutex(TS_RIGHTCLICKEVEN);
//RightDown互斥		  
TsMutex gDXRightDownMutex(TS_RIGHTDOWNEVEN);
//RightUp互斥		  
TsMutex gDXRightUpMutex(TS_RIGHTUPEVEN);
//FindColor互斥		  
TsMutex gDXFindColorMutex(TS_FINDCOLOREVEN);
//GetColor互斥		  
TsMutex gDXGetColorMutex(TS_GETCOLOREVEN);
//GetCursorShape互斥		  
TsMutex gDXGetCursorShapeMutex(TS_GETCURSORSHAPEEVEN);
//SetPath互斥		  
TsMutex gDXSetPathMutex(TS_SETPATHEVEN);
//GetCursorPos互斥		  
TsMutex gDXGetCursorPosMutex(TS_GETCURSORPOSEVEN);
//MoveR互斥		  
TsMutex gDXMoveRMutex(TS_MOVEREVEN);
//MiddleClick互斥		  
TsMutex gDXMiddleClickMutex(TS_MIDDLECLICKEVEN);
//WheelDown互斥						
TsMutex gDXWheelDownMutex(TS_WHEELDOWNEVEN);
//WheelUp互斥						
TsMutex gDXWheelUpMutex(TS_WHEELUPEVEN);
//Capture互斥						
TsMutex gDXCaptureMutex(TS_CAPTUREEVEN);
//LockInput互斥						
TsMutex gDXLockInputMutex(TS_LOCKINPUTEVEN);
//Ocr互斥						
TsMutex gDXOcrMutex(TS_OCREVEN);
//SetDict互斥						
TsMutex gDXSetDictMutex(TS_SETDICTEVEN);
//UseDict互斥						
TsMutex gDXUseDictMutex(TS_USEDICTEVEN);
//ClearDict互斥						
TsMutex gDXClearDictMutex(TS_CLEARDICTEVEN);
//FindStr互斥						
TsMutex gDXFindStrMutex(TS_FINDSTREVEN);
//GetNowDict互斥						
TsMutex gDXGetNowDictMutex(TS_GETNOWDICTEVEN);
//CaptureGif互斥						
TsMutex gDXCaptureGifMutex(TS_CAPTUREGIFEVEN);
//EnumWindow互斥						
TsMutex gDXEnumWindowMutex(TS_ENUMWINDOWEVEN);
//EnumWindowByProcess互斥						
TsMutex gDXEnumWindowByProcessMutex(TS_ENUMWINDOWBYPROCESSEVEN);
//EnumProcess互斥						
TsMutex gDXEnumProcessMutex(TS_ENUMPROCESSEVEN);
//FindData
TsMutex gDXFindDataMutex(TS_FINDDATAEVEN);
//FindDouble
TsMutex gDXFindDoubleMutex(TS_FINDDOUBLEEVEN);
//FindFloat
TsMutex gDXFindFloatMutex(TS_FINDFLOATEVEN);
//FindInt
TsMutex gDXFindIntMutex(TS_FINDINTEVEN);
//FindString
TsMutex gDXFindStringMutex(TS_FINDSTRINGEVEN);

TsMutex gDXMySleepMutex(TS_MYSLEEPEVEN);
//GetScreenData
TsMutex gDXMyGetScreenDataMutex(TS_MYGETSCREENDATAEVEN);
//MoveToEx
TsMutex gDXMoveToExMutex(TS_MOVETOEXEVEN);


extern myGetTickCount my_myGetTickCount;
extern mySleep my_sleep;
//DLL所在的位置
extern TCHAR gDLLFolder[MAX_PATH + 1];

int Loginlog(wchar_t * macode)   //统计使用人数
{																					

	{
		{
			HINTERNET internetopen = InternetOpen(L"TC",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
			if (internetopen==NULL)
			{
				return 0;
			}
			wchar_t url[MAX_PATH]={0};
			swprintf(url,L"http://www.tyuyan.com/tongjiversion.aspx?version=2.5&maccode=%s",macode);
			HINTERNET internetopenurl = InternetOpenUrl(internetopen,url,NULL,0,INTERNET_FLAG_RELOAD,0);
			if (internetopenurl==NULL)
			{    
				InternetCloseHandle(internetopen);
				return 0;
			}
			InternetCloseHandle(internetopenurl);
			InternetCloseHandle(internetopen);
		}
	}

}

// CTSPlugInterFace
STDMETHODIMP CTSPlugInterFace::BindWindow(LONG hwnd, BSTR display, BSTR mouse, BSTR keypad, LONG mode, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXbindMutex);

	gDxObj.m_mousekeyhwnd=(HWND)hwnd;
	gDxObj.m_hwnd=(HWND)hwnd;
	*nret=false;

	VMPBEGIN    //VMP保护
		//if(!TSRuntime::IsVipLogin)
		//	{
		//		//::GetTickCount();
		//		int bret=TSRuntime::VipLogin(L"",0,false);   //试用验证
		//		if(bret<=0)
		//			{
		//				INT64 t=0;
		//				if(my_myGetTickCount)
		//					t=my_myGetTickCount();
		//				else
		//					t=::GetTickCount();
		//				if(t==TSRuntime::checkthreadtime||abs((int)(t-TSRuntime::checkthreadtime))>=60*1000*10||TSRuntime::checkthreadtime==0)	//校验线程是否被挂起或者GetTickCount被人下钩子
		//					{
		//						gDxObj.SendMsg(TS_UNBIND);
		//						Sleep(200);
		//						gDxObj.UnBind();
		//						Sleep(200);
		//						gDxObj.SendMsg(TS_UNBIND);
		//						Sleep(200);
		//						gDxObj.UnBind();
		//						if(TSRuntime::IsStartProtect)
		//							TSRuntime::StartProtect(0,L"");
		//						*nret=0;
		//						TSRuntime::h_checkthread=NULL;
		//						return S_OK;
		//					}
		//				
		//			}
		//	}
	DWORD displayMode=NULL,keyMode=NULL,mouseMode=NULL;
	//TSRuntime::add_log( "获得模式 外部 :%s",display);
	gDxObj.BindMode=0;
	gDxObj.m_keyMode=0;
	gDxObj.m_mouseMode=0;
	TSRuntime::getMode(mouse,keypad,display,mouseMode,keyMode,displayMode);

	if(displayMode==-1||keyMode==-1||mouseMode==-1)
		return false;

	gDxObj.m_displayMode=displayMode;
	gDxObj.m_keyMode=keyMode;
	gDxObj.m_mouseMode=mouseMode;

	gDxObj.BindMode=mode;
	gDxObj.m_mousekeyhwnd=(HWND)hwnd;
	bool sucee=false;
	if((gDxObj.m_displayMode&DISPLAY_DX_3D)==DISPLAY_DX_3D||(gDxObj.m_mouseMode&MOUSE_API)==MOUSE_API||(gDxObj.m_keyMode&KEY_API)==KEY_API)
	{
		sucee = gDxObj.bind(gDxObj.m_hwnd,0,0,gDxObj.m_displayMode,(BIND_MODE)gDxObj.BindMode);

		if(sucee==false&&::IsWindow(gDxObj.m_hwnd))//如果绑定失败,这里调用一次解绑动作
			gDxObj.UnBind();
		*nret=sucee;
	}
	else
		*nret=true;

	if((gDxObj.m_displayMode&DISPLAY_GDI) == DISPLAY_GDI||(gDxObj.m_displayMode&DISPLAY_DX2) == DISPLAY_DX2)
	{
		//TSRuntime::add_log( "DISPLAY_GDI");
		//TSRuntime::add_log( "gdi模式找图初始化");
		//GDI模式需要修改WINDOW样式
		//遍历获取父窗口句柄
		HWND parenthwnd=0;
		HWND hparent=0;
		bool findparent=false;
		do{
			if(parenthwnd==0)
				parenthwnd=GetParent(gDxObj.m_hwnd);
			else
			{
				hparent=GetParent(parenthwnd);
				if(hparent!=0&&hparent!=parenthwnd)
				{
					//gDxObj.m_parenthwnd=hparent;
					//TSRuntime::add_log( "gDxObj.m_parenthwnd:%d",gDxObj.m_parenthwnd);
					hwnd=(LONG)hparent;
					findparent=true;
				}
				parenthwnd=hparent;
			}

		}while(parenthwnd!=NULL);


		char pszBinWindow_GDIWINDOW[MAX_PATH]={0};
		sprintf( pszBinWindow_GDIWINDOW,"%s%d",TS_BINDWINDOW_GDIWINDOW,gDxObj.nPid);
		SetPropA((HWND)hwnd,pszBinWindow_GDIWINDOW,(HANDLE)1);   //设置绑定标识符

		typedef bool (__stdcall  *  myGetLayeredWindowAttributes)( 
			HWND hwnd,
			COLORREF *pcrKey,
			BYTE *pbAlpha,
			DWORD *pdwFlags); 
		myGetLayeredWindowAttributes obj_GetLayeredWindowAttributes= NULL; 
		typedef bool (__stdcall  *  mySetLayeredWindowAttributes)( 
			HWND hwnd,
			COLORREF pcrKey,
			BYTE pbAlpha,
			DWORD pdwFlags); 
		mySetLayeredWindowAttributes obj_SetLayeredWindowAttributes=  NULL; 
		HINSTANCE hlibrary;
		hlibrary = LoadLibrary(_T("user32.dll"));
		obj_GetLayeredWindowAttributes=(myGetLayeredWindowAttributes)GetProcAddress(hlibrary,"GetLayeredWindowAttributes");
		obj_SetLayeredWindowAttributes=(mySetLayeredWindowAttributes)GetProcAddress(hlibrary,"SetLayeredWindowAttributes");
		DWORD dwExStyle = GetWindowLong((HWND)hwnd,GWL_EXSTYLE);
		gDxObj.GDI_dwExStyle=dwExStyle;
		COLORREF crKey=0;
		BYTE bAlpha=0;
		DWORD dwFlags=0;
		bool ret=obj_GetLayeredWindowAttributes((HWND)hwnd, &crKey, &bAlpha, &dwFlags);
		//GDI:crKey=0xff00ff,bAlpha=0,dwFlags=1;//DX2:crKey=0;bAlpha=0XFF;dwFlags=2;
		if(gDxObj.m_displayMode&DISPLAY_GDI)
		{
			crKey=0xff00ff;
			bAlpha=0;
			dwFlags=LWA_COLORKEY;
			::SetWindowLong((HWND)hwnd, GWL_EXSTYLE, 0x80100);
		}
		else if(gDxObj.m_displayMode&DISPLAY_DX2)
		{
			crKey=0;
			bAlpha=0XFF;
			dwFlags=LWA_ALPHA;
			::SetWindowLong((HWND)hwnd, GWL_EXSTYLE, 0x80100);
		}
		obj_SetLayeredWindowAttributes((HWND)hwnd, crKey, bAlpha, dwFlags);
		::UpdateWindow((HWND)hwnd);
		Sleep(100); //疑问 这里等1秒是等待窗口刷新
		gDxObj.m_dwExStyle=dwExStyle;

		//GDI模式需要修改WINDOW样式
	}
	else if(gDxObj.m_displayMode&DISPLAY_NORMAL)
	{
		gDxObj.m_hwnd=GetDesktopWindow();
		if(::IsWindow((HWND)hwnd))
			gDxObj.m_parenthwnd=(HWND)hwnd;
		else
			gDxObj.m_parenthwnd=gDxObj.m_hwnd;//桌面句柄;
	}
	gDxObj.InitGdiNormal();
	if(sucee||*nret==true)
		gDxObj.m_isBind=true;
	VMPEND
		//TSRuntime::add_log( "TS_BIND-end");
		return S_OK;
}

STDMETHODIMP CTSPlugInterFace::BindWindowEx(LONG hwnd, BSTR display, BSTR mouse, BSTR keypad, BSTR publics, LONG mode, LONG* nret)
{
	// TODO: 在此添加实现代码
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::UnBindWindow(LONG* nret)
{
	TsMutexlock ts_mutexlock(&gDXUnbindMutex);
	//TSRuntime::g_DxObj.SendMsg(TS_UNBIND);
	if(gDxObj.m_isBind)
	{
		gDxObj.SendMsg(TS_UNBIND);
		Sleep(200);
		gDxObj.UnBind();
		gDxObj.m_isBind=false;
		if(!TSRuntime::IsLoginlog)
		{
			Loginlog(TSRuntime::MachineCode);
			TSRuntime::IsLoginlog=true;
		}
	}
	* nret=1;
	// TODO: 在此添加实现代码
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::KeyDown(LONG vk_code, LONG* nret)
{
	TsMutexlock ts_mutexlock(&gDXKeyDownMutex);
	// TODO: 在此添加实现代码
	if(vk_code==VK_MENU)
	{
		//TSRuntime::g_IsSYSKEY=true;
		gDxObj.g_IsSYSKEY=true;
	}
	if((gDxObj.m_keyMode&KEY_NORMAL)==KEY_NORMAL)//判断是否是前台模式
	{

	if(TSRuntime::SetSimModeType==0)
			TSRuntime::g_KeyBoard.keyDown(vk_code);
	}
	else
	{
		if(gDxObj.g_IsSYSKEY&&vk_code!=VK_MENU)
			*nret=PostMessage(gDxObj.m_mousekeyhwnd,WM_SYSKEYDOWN,vk_code,1<<29);
		gDxObj.SendMsg(TS_KEYDOWN,vk_code);	
		//*nret=PostMessage(TSRuntime::g_DxObj.m_hwnd,WM_SYSKEYDOWN,vk_code,1<<29);
		if(gDxObj.m_keyMode&KEY_WINDOW)
			PostMessage(gDxObj.m_mousekeyhwnd,WM_KEYDOWN,vk_code,TSRuntime::retrunLparamDown(vk_code));
		//TSRuntime::g_DxObj.SendMsg(WM_KEYDOWN,vk_code);
	}
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::KeyUp(LONG vk_code, LONG* nret)
{
	TsMutexlock ts_mutexlock(&gDXKeyUpMutex);
	// TODO: 在此添加实现代码
	if(vk_code==VK_MENU)
	{
		//TSRuntime::g_IsSYSKEY=false;
		gDxObj.g_IsSYSKEY=false;
	}
	if((gDxObj.m_keyMode&KEY_NORMAL)==KEY_NORMAL)//判断是否是前台模式
	{
		if(TSRuntime::SetSimModeType==0)
			TSRuntime::g_KeyBoard.keyUp(vk_code);
	}
	else
	{
		if(TSRuntime::g_IsSYSKEY)
			*nret=::PostMessage(gDxObj.m_mousekeyhwnd,WM_SYSKEYUP,vk_code,0);
		*nret=gDxObj.SendMsg(TS_KEYUP,vk_code);
		//	*nret=::PostMessage(TSRuntime::g_DxObj.m_hwnd,WM_SYSKEYUP,vk_code,0);
		//*nret=TSRuntime::g_DxObj.SendMsg(TS_KEYUP,vk_code);
		if(gDxObj.m_keyMode&KEY_WINDOW)
			//TSRuntime::g_DxObj.SendMsg(WM_KEYUP,vk_code);
			PostMessage(gDxObj.m_mousekeyhwnd,WM_KEYUP,vk_code,(0xC0000000|TSRuntime::retrunLparamDown(vk_code)));
	}

	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::KeyPress(LONG vk_code, LONG* nret)
{
	TsMutexlock ts_mutexlock(&gDXKeyPressMutex);
	// TODO: 在此添加实现代码

	if((gDxObj.m_keyMode&KEY_NORMAL)==KEY_NORMAL)//判断是否是前台模式
	{
		if(TSRuntime::SetSimModeType==0)
		{
			TSRuntime::g_KeyBoard.delaytime=gDxObj.NormalKeysleep;
			TSRuntime::g_KeyBoard.keyPress(vk_code);
		}
	}
	else
	{
		if(gDxObj.g_IsSYSKEY&&vk_code!=VK_MENU)
			*nret=PostMessage(gDxObj.m_mousekeyhwnd,WM_SYSKEYDOWN,vk_code,1<<29);
		else
			*nret=gDxObj.SendMsg(TS_KEYDOWN,vk_code);

		if(gDxObj.m_keyMode&KEY_WINDOW)
			PostMessage(gDxObj.m_mousekeyhwnd,WM_KEYDOWN,vk_code,TSRuntime::retrunLparamDown(vk_code));

		if(gDxObj.m_keyMode&KEY_WINDOW)	//window模式默认延时))
		{
			if(gDxObj.IsTSEnableRealKeypad)
				Sleep(gDxObj.TSRealKeyMousSleepTime(1));
			else
				Sleep(gDxObj.WindowsKeyMouseClicksleep);
		}
		else
		{
			if(gDxObj.IsTSEnableRealKeypad)
				Sleep(gDxObj.TSRealKeyMousSleepTime(1));
			else
				Sleep(gDxObj.KeyDownsleep);
		}

		*nret=gDxObj.SendMsg(TS_KEYUP,vk_code);

		if(gDxObj.m_keyMode&KEY_WINDOW)
			PostMessage(gDxObj.m_mousekeyhwnd,WM_KEYUP,vk_code,(0xC0000000|TSRuntime::retrunLparamDown(vk_code)));

	}
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::LeftDown(LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXLeftDownMutex);
	if((gDxObj.m_mouseMode&MOUSE_NORMAL)==MOUSE_NORMAL)
	{
		//
		if(TSRuntime::SetSimModeType==0)//普通模式
			TSRuntime::g_Mouse.leftDown();
	}
	else
	{
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)
		{
			SendMessage(gDxObj.m_mousekeyhwnd,WM_SETFOCUS,(WPARAM)gDxObj.m_mousekeyhwnd,0);
			SendMessage(gDxObj.m_mousekeyhwnd,WM_CAPTURECHANGED,(WPARAM)gDxObj.m_mousekeyhwnd,0);
			PostMessage(gDxObj.m_mousekeyhwnd,WM_LBUTTONDOWN,1,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
		}
		else
			*nret=gDxObj.SendMsg(TS_LBUTTONDOWN,1,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
	}

	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::LeftUp(LONG* nret)
{
	// TODO: 在此添加实现代
	TsMutexlock ts_mutexlock(&gDXLeftUpMutex);
	if((gDxObj.m_mouseMode&MOUSE_NORMAL)==MOUSE_NORMAL)
	{
		//TSRuntime::g_Mouse.leftUp();
		if(TSRuntime::SetSimModeType==0)//普通模式
			TSRuntime::g_Mouse.leftUp();
	}
	else
	{
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)
		{
			PostMessage(gDxObj.m_mousekeyhwnd,WM_LBUTTONUP,0,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
			SendMessage(gDxObj.m_mousekeyhwnd,WM_CAPTURECHANGED,0,0);
		}
		else
			*nret=gDxObj.SendMsg(TS_LBUTTOUP,0,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
	}
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::LeftClick(LONG* nret)
{
	TsMutexlock ts_mutexlock(&gDXLeftClickMutex);
	// TODO: 在此添加实现代码

	if((gDxObj.m_mouseMode&MOUSE_NORMAL)==MOUSE_NORMAL)
	{
		TSRuntime::g_Mouse.delaytimes=gDxObj.NormalClicksleep;
		if(TSRuntime::SetSimModeType==0)//普通模式
			TSRuntime::g_Mouse.leftClick();
	}
	else
	{
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)
		{
			SendMessage(gDxObj.m_mousekeyhwnd,WM_SETFOCUS,(WPARAM)gDxObj.m_mousekeyhwnd,0);
			SendMessage(gDxObj.m_mousekeyhwnd,WM_CAPTURECHANGED,(WPARAM)gDxObj.m_mousekeyhwnd,0);
			PostMessage(gDxObj.m_mousekeyhwnd,WM_LBUTTONDOWN,1,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
		}
		else
			*nret=gDxObj.SendMsg(TS_LBUTTONDOWN,1,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)	//window模式默认延时
		{
			if(gDxObj.IsTSEnableRealMouse)
				Sleep(gDxObj.TSRealKeyMousSleepTime(0));
			else
				Sleep(gDxObj.WindowsKeyMouseClicksleep);
		}
		else
		{
			if(gDxObj.IsTSEnableRealMouse)
				Sleep(gDxObj.TSRealKeyMousSleepTime(0));
			else
				Sleep(gDxObj.MouseClicksleep);  //dx模式默认延时
		}
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)
		{
			PostMessage(gDxObj.m_mousekeyhwnd,WM_LBUTTONUP,0,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
			SendMessage(gDxObj.m_mousekeyhwnd,WM_CAPTURECHANGED,0,0);
		}
		else
			*nret=gDxObj.SendMsg(TS_LBUTTOUP,0,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
	}

	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::MoveTo(LONG x, LONG y, LONG* nret)
{
	TsMutexlock ts_mutexlock(&gDXMoveToMutex);
	// TODO: 在此添加实现代码
	POINT pos;
	pos.x=x;
	pos.y=y;
	if(gDxObj.IsTSEnableRealMouse)
	{
		if(IsWindow(gDxObj.m_hwnd))
		{
			::ClientToScreen(gDxObj.m_mousekeyhwnd,&pos);
			gDxObj.TSEnableRealMouse(pos.x,pos.y);
			gDxObj.LastMouseMoveX=pos.x;
			gDxObj.LastMouseMoveY=pos.y;
		}
		else
		{
			gDxObj.TSEnableRealMouse(x,y);
			gDxObj.LastMouseMoveX=x;
			gDxObj.LastMouseMoveY=y;
		}

	}
	else
	{
		if((gDxObj.m_mouseMode&MOUSE_NORMAL)==MOUSE_NORMAL)
		{
			if(IsWindow(gDxObj.m_hwnd))
			{
				::ClientToScreen(gDxObj.m_mousekeyhwnd,&pos);
				if(TSRuntime::SetSimModeType==0)//普通模式
					::SetCursorPos(pos.x,pos.y);
				gDxObj.LastMouseMoveX=pos.x;
				gDxObj.LastMouseMoveY=pos.y;
			}
			else
			{
				//::SetCursorPos(x,y);
				if(TSRuntime::SetSimModeType==0)//普通模式
					::SetCursorPos(x,y);
				gDxObj.LastMouseMoveX=x;
				gDxObj.LastMouseMoveY=y;
			}
		}
		else
		{
			if(gDxObj.m_mouseMode&MOUSE_WINDOW)
				PostMessage(gDxObj.m_mousekeyhwnd,WM_MOUSEMOVE,0,MAKELPARAM(x,y));
			else
			{
				*nret=gDxObj.SendMsg(TS_MOUSEMOVE,0,MAKELPARAM(x,y));
				//*nret=gDxObj.SendMsg(TS_MOUSEMOVEOVER);
			}
			gDxObj.LastMouseMoveX=x;
			gDxObj.LastMouseMoveY=y;
		}
	}

	*nret=1;
	return S_OK;
}

//int addr=0;
STDMETHODIMP CTSPlugInterFace::FindPic(LONG x1, LONG y1, LONG x2, LONG y2, BSTR pic_name, BSTR delta_color, DOUBLE sim, LONG dir, VARIANT* intX, VARIANT* intY, LONG* nret)
{	 //
	TsMutexlock ts_mutexlock(&gDXFindPicMutex);
	LONG x=-1,y=-1,ret=-1;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	//ret=TSRuntime::g_DxObj.findPic(x1,y1,x2,y2,pic_name,delta_color,sim,dir,x,y);
	ret=gDxObj.findPic(x1,y1,x2,y2,pic_name,delta_color,sim,dir,x,y);
	intX->vt=VT_I4;
	intY->vt=VT_I4;
	intX->lVal=x;
	intY->lVal=y;
	if(x==-1||y==-1)
		ret=-1;
	*nret=ret;
	//TSRuntime::add_log( "findPicture,X:%d,Y:%d",x,y);
	//TSRuntime::add_log( "findPicture,X:%d,Y:%d,add:%d",x,y,++addr);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::RightClick(LONG* nret)
{
	TsMutexlock ts_mutexlock(&gDXRightClickMutex);

	// TODO: 在此添加实现代码
	if((gDxObj.m_mouseMode&MOUSE_NORMAL)==MOUSE_NORMAL)
	{
		TSRuntime::g_Mouse.delaytimes=gDxObj.NormalClicksleep;
		//TSRuntime::g_Mouse.rightClick();
		if(TSRuntime::SetSimModeType==0)//普通模式
			TSRuntime::g_Mouse.rightClick();
	}
	else
	{
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)
		{
			PostMessage(gDxObj.m_mousekeyhwnd,WM_RBUTTONDOWN,1,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
			SendMessage(gDxObj.m_mousekeyhwnd,WM_SETFOCUS,(WPARAM)gDxObj.m_hwnd,0);
			SendMessage(gDxObj.m_mousekeyhwnd,WM_CAPTURECHANGED,(WPARAM)gDxObj.m_mousekeyhwnd,0);
		}
		else
			*nret=gDxObj.SendMsg(TS_RBUTTONDOWN,1,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)	//window模式默认延时
		{
			if(gDxObj.IsTSEnableRealMouse)
				Sleep(gDxObj.TSRealKeyMousSleepTime(0));
			else
				Sleep(gDxObj.WindowsKeyMouseClicksleep);
		}
		else
		{
			if(gDxObj.IsTSEnableRealMouse)
				Sleep(gDxObj.TSRealKeyMousSleepTime(0));
			else
				Sleep(gDxObj.MouseClicksleep);  //dx模式默认延时
		}
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)
		{
			PostMessage(gDxObj.m_mousekeyhwnd,WM_RBUTTONUP,0,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
			SendMessage(gDxObj.m_mousekeyhwnd,WM_CAPTURECHANGED,0,0);
		}
		else
			*nret=gDxObj.SendMsg(TS_RBUTTONUP,0,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
	}

	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::RightDown(LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXRightDownMutex);

	if((gDxObj.m_mouseMode&MOUSE_NORMAL)==MOUSE_NORMAL)
	{
		//TSRuntime::g_Mouse.rightDown();
		if(TSRuntime::SetSimModeType==0)//普通模式
			TSRuntime::g_Mouse.rightDown();
	}
	else
	{
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)
		{
			PostMessage(gDxObj.m_mousekeyhwnd,WM_RBUTTONDOWN,1,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
			SendMessage(gDxObj.m_mousekeyhwnd,WM_SETFOCUS,(WPARAM)gDxObj.m_hwnd,0);
			SendMessage(gDxObj.m_mousekeyhwnd,WM_CAPTURECHANGED,(WPARAM)gDxObj.m_mousekeyhwnd,0);
		}
		else
			*nret=gDxObj.SendMsg(TS_RBUTTONDOWN,1,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
	}

	*nret=1;
	return S_OK;
}
STDMETHODIMP CTSPlugInterFace::FindColor(LONG x1, LONG y1, LONG x2, LONG y2, BSTR color, DOUBLE sim, LONG dir, VARIANT* intX, VARIANT* intY, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindColorMutex);
	if(x1>=x2||y1>=y2)
	{
		MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	//wchar_t* p=0;
	DWORD ncolor=0x100;
	//ncolor=::wcstol(color,&p,16);
	//int r=0,g=0,b=0;
	//r=GetRValue(ncolor);
	//g=GetGValue(ncolor);
	//b=GetBValue(ncolor);
	//swprintf(color,L"%x%x%x",r,g,b);//将传入的RGB颜色值转换为BGR进行找色
	//ncolor=::wcstol(color,&p,16);
	LONG x=-1,y=-1,ret;
	//TSRuntime::add_log("FindColor:_wtoi:%x",ncolor);
	//ret=TSRuntime::g_DxObj.findColor(x1,y1,x2,y2,ncolor,sim,dir,x,y);
	ret=gDxObj.findColor(x1,y1,x2,y2,ncolor,sim,dir,x,y,color);
	intX->vt=VT_I4;
	intY->vt=VT_I4;
	intX->lVal=x;
	intY->lVal=y;
	*nret=ret;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::RightUp(LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXRightUpMutex);

	if((gDxObj.m_mouseMode&MOUSE_NORMAL)==MOUSE_NORMAL)
	{
		//TSRuntime::g_Mouse.rightUp();
		if(TSRuntime::SetSimModeType==0)//普通模式
			TSRuntime::g_Mouse.rightUp();
	}
	else
	{
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)
		{
			PostMessage(gDxObj.m_mousekeyhwnd,WM_RBUTTONUP,0,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
			SendMessage(gDxObj.m_mousekeyhwnd,WM_CAPTURECHANGED,0,0);
		}
		else
			*nret=gDxObj.SendMsg(TS_RBUTTONUP,0,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
	}
	*nret=1;
	return S_OK;
}


STDMETHODIMP CTSPlugInterFace::GetColor(LONG x, LONG y, BSTR* RetStrColor)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXGetColorMutex);
	//TSRuntime::g_DxObj.GetColor(x,y,*RetStrColor);

	//memset(TSRuntime::RetStr,0,MAX_PATH);
	gDxObj.GetColor(x,y,TSRuntime::RetStr);
	CComBSTR newbstr;
	newbstr.Append(TSRuntime::RetStr);
	newbstr.CopyTo(RetStrColor);
	//_variant_t strVar();
	//*RetStrColor=strVar.bstrVal;//_bstr_t(TSRuntime::RetStr)
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetCursorShape(BSTR* RetCursorShape)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXGetCursorShapeMutex);

	DWORD mouseshape=0;
	if((gDxObj.m_mouseMode&MOUSE_NORMAL)==MOUSE_NORMAL)
	{
		TSRuntime::g_Mouse.getmouseshape(mouseshape);
	}
	else
		mouseshape=gDxObj.SendMsg(TS_GETMOUSESHARE,0,0);
	if(mouseshape)
	{
		memset(TSRuntime::RetStr,0,MAX_PATH);
		swprintf(TSRuntime::RetStr,L"%x",mouseshape);
		_variant_t strVar(TSRuntime::RetStr);
		*RetCursorShape=strVar.bstrVal;
	}
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SetPath(BSTR path, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXSetPathMutex);
	//*nret=TSRuntime::g_DxObj.TSSetpath(path);
	*nret=gDxObj.TSSetpath(path);
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::TSGuardProtect(LONG enable, BSTR type,LONG* nret)
{
	// TODO: 在此添加实现代码
	// TsMutexlock ts_mutexlock(&gDXbindMutex);
	VMPBEGIN    //VMP保护
	if(!TSRuntime::IsVipLogin)
	{
		if(TSRuntime::IsRegLogin==false&&TSRuntime::IsVipLogin==false)//TS盾不允许试用
		{
			*nret=0;
			return S_OK;
		}
		INT64 t=0;
		if(my_myGetTickCount)
			t=my_myGetTickCount();
		else
			t=::GetTickCount();
		if(t==TSRuntime::checkthreadtime||abs((int)(t-TSRuntime::checkthreadtime))>=60*1000*10||TSRuntime::checkthreadtime==0)	//校验线程是否被挂起或者GetTickCount被人下钩子
		{
			if(TSRuntime::IsStartProtect)
				TSRuntime::StartProtect(0,0,-1);//Pid
			*nret=0;
			TSRuntime::h_checkthread=NULL;
			return S_OK;
		}
	}
	TSRuntime::IsProid=wcstol(type,0,10);
	*nret=TSRuntime::StartProtect(enable,0);
	VMPEND
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::KeyPressStr(BSTR key_str, LONG delay, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXKeyPressStrMutex);
	if((gDxObj.m_keyMode&KEY_NORMAL)==KEY_NORMAL)//判断是否是前台模式
	{
		if(TSRuntime::SetSimModeType==0)
			TSRuntime::g_KeyBoard.sendkeyString(key_str,delay);
	}
	else
	{
		int len=wcslen(key_str);
		for(int i=0;i<len;i++)
		{
			// * nret=TSRuntime::g_DxObj.SendMsg(TS_CHAR,key_str[i]);	
			gDxObj.SendMsg(TS_CHAR,key_str[i]);
			if(gDxObj.m_keyMode&KEY_WINDOW)
				gDxObj.SendMsg(WM_CHAR,i);
			Sleep(delay);
		}
	}
	//std::string dd=gDxObj.pData->tt;
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SendString(LONG hwnd, BSTR str, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXSendStringMutex);
	int len=wcslen(str);
	//MessageBoxA(NULL,tts,tts,NULL);
	for(int i=0;i<len;i++)
	{
		* nret=::SendMessage((HWND)hwnd,WM_CHAR,(WPARAM)str[i],(LPARAM)1);
		Sleep(10);
	}
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SendString2(LONG hwnd, BSTR str, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXSendString2Mutex);
	int len=wcslen(str);
	for(int i=0;i<len;i++)
	{								   
		* nret=::PostMessage((HWND)hwnd,WM_CHAR,(WPARAM)str[i],(LPARAM)1);
		Sleep(10);
	}
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::KeyPressChar(BSTR key_str, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXKeyPressCharMutex);

	for(int i=0;i<0x100;i++)
	{
		if(TSRuntime::KeyPressCharMap[i])
		{
			if(wcscmp(TSRuntime::KeyPressCharMap[i],_wcslwr(key_str))==0)
			{
				if((gDxObj.m_keyMode&KEY_NORMAL)==KEY_NORMAL)//判断是否是前台模式
				{
					if(TSRuntime::SetSimModeType==0)
					{
						TSRuntime::g_KeyBoard.delaytime=gDxObj.NormalKeysleep;
						TSRuntime::g_KeyBoard.keyPress(i);
					}
				}
				else
				{
					//if(TSRuntime::g_IsSYSKEY&&wcscmp(L"alt",TSRuntime::KeyPressCharMap[i])!=0)
					if(gDxObj.g_IsSYSKEY&&wcscmp(L"alt",TSRuntime::KeyPressCharMap[i])!=0&&gDxObj.m_keyMode&KEY_WINDOW)
						*nret=PostMessage(gDxObj.m_mousekeyhwnd,WM_SYSKEYDOWN,i,1<<29);
					//*nret=PostMessage(TSRuntime::g_DxObj.m_hwnd,WM_SYSKEYDOWN,i,1<<29);

					//*nret=TSRuntime::g_DxObj.SendMsg(TS_KEYDOWN,i);
					*nret=gDxObj.SendMsg(TS_KEYDOWN,i);
					if(gDxObj.m_keyMode&KEY_WINDOW)
						//TSRuntime::g_DxObj.SendMsg(WM_KEYDOWN,i,TSRuntime::retrunLparamDown(i));
						PostMessage(gDxObj.m_mousekeyhwnd,WM_KEYDOWN,i,TSRuntime::retrunLparamDown(i));

					if(gDxObj.m_keyMode&KEY_WINDOW)	//window模式默认延时))
					{
						if(gDxObj.IsTSEnableRealKeypad)
							Sleep(gDxObj.TSRealKeyMousSleepTime(1));
						else
							Sleep(gDxObj.WindowsKeyMouseClicksleep);
					}
					else
					{
						if(gDxObj.IsTSEnableRealKeypad)
							Sleep(gDxObj.TSRealKeyMousSleepTime(1));
						else
							Sleep(gDxObj.KeyDownsleep);
					}

					//*nret=TSRuntime::g_DxObj.SendMsg(TS_KEYUP,i);
					*nret=gDxObj.SendMsg(TS_KEYUP,i);
					if(gDxObj.m_keyMode&KEY_WINDOW)
						//TSRuntime::g_DxObj.SendMsg(WM_KEYUP,i,(0xC0000000|TSRuntime::retrunLparamDown(i)));
						PostMessage(gDxObj.m_mousekeyhwnd,WM_KEYUP,i,(0xC0000000|TSRuntime::retrunLparamDown(i)));
				}
				break;

			}


		}
	}
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::KeyDownChar(BSTR key_str, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXKeyDownMutex);
	if(wcscmp(L"alt",_wcslwr(key_str))==0)
	{
		//TSRuntime::g_IsSYSKEY=true;
		gDxObj.g_IsSYSKEY=true;
	}

	for(int i=0;i<0x100;i++)
	{
		if(TSRuntime::KeyPressCharMap[i])
		{
			if(wcscmp(TSRuntime::KeyPressCharMap[i],_wcslwr(key_str))==0)
			{
				if((gDxObj.m_keyMode&KEY_NORMAL)==KEY_NORMAL)//判断是否是前台模式
				{
 if(TSRuntime::SetSimModeType==0)
						TSRuntime::g_KeyBoard.keyDown(i);
				}
				else
				{
					if(gDxObj.g_IsSYSKEY&&wcscmp(L"alt",TSRuntime::KeyPressCharMap[i])!=0)
						*nret=PostMessage(gDxObj.m_mousekeyhwnd,WM_SYSKEYDOWN,i,1<<29);
					*nret=gDxObj.SendMsg(TS_KEYDOWN,i);
					if(gDxObj.m_keyMode&KEY_WINDOW)
						//TSRuntime::g_DxObj.SendMsg(WM_KEYDOWN,i);
						PostMessage(gDxObj.m_mousekeyhwnd,WM_KEYDOWN,i,TSRuntime::retrunLparamDown(i));

				}
				break;
			}
		}
	}
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::KeyUpChar(BSTR key_str, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXKeyUpMutex);
	if(wcscmp(L"alt",_wcslwr(key_str))==0)
		gDxObj.g_IsSYSKEY=false;
	for(int i=0;i<0x100;i++)
	{
		if(TSRuntime::KeyPressCharMap[i]!=NULL)
		{
			if(wcscmp(TSRuntime::KeyPressCharMap[i],_wcslwr( key_str))==0)
			{
				if((gDxObj.m_keyMode&KEY_NORMAL)==KEY_NORMAL)//判断是否是前台模式
				{
 if(TSRuntime::SetSimModeType==0)
						TSRuntime::g_KeyBoard.keyUp(i);
				}
				else
				{
					*nret=gDxObj.SendMsg(TS_KEYUP,i);
					if(gDxObj.m_keyMode&KEY_WINDOW)
						//TSRuntime::g_DxObj.SendMsg(WM_KEYUP,i);
						PostMessage(gDxObj.m_mousekeyhwnd,WM_KEYUP,i,(0xC0000000|TSRuntime::retrunLparamDown(i)));
				}
				break;
			}

		}
	}
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetCursorPos(VARIANT* x, VARIANT* y, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXGetCursorPosMutex);
	//LONG lParam=0;
	//lParam=TSRuntime::g_DxObj.SendMsg(TS_GETCURSORPOS);
	//lParam=gDxObj.SendMsg(TS_GETCURSORPOS);
	x->vt=VT_I4;
	y->vt=VT_I4;
	x->lVal=gDxObj.LastMouseMoveX;
	y->lVal=gDxObj.LastMouseMoveY;
	* nret=1;
	////if(lParam)
	//{
	//	x->lVal=(short)LOWORD(lParam);
	//	y->lVal=(short)HIWORD(lParam);
	//	* nret=1;
	//}
	//else
		//* nret=0;

	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::MoveR(LONG rx, LONG ry, LONG* rnet)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXMoveRMutex);
		
	POINT pos;
	pos.x=rx+gDxObj.LastMouseMoveX;
	pos.y=ry+gDxObj.LastMouseMoveY;
	if(gDxObj.IsTSEnableRealMouse)
	{
		if(IsWindow(gDxObj.m_hwnd))
		{
			::ScreenToClient(gDxObj.m_mousekeyhwnd,&pos);
			gDxObj.TSEnableRealMouse(pos.x,pos.y);
			gDxObj.LastMouseMoveX=pos.x;
			gDxObj.LastMouseMoveY=pos.y;
		}
		else
		{
			gDxObj.TSEnableRealMouse(rx+gDxObj.LastMouseMoveX,ry+gDxObj.LastMouseMoveY);
			gDxObj.LastMouseMoveX=rx+gDxObj.LastMouseMoveX;
			gDxObj.LastMouseMoveY=ry+gDxObj.LastMouseMoveY;
		}
	}
	else
	{
		if((gDxObj.m_mouseMode&MOUSE_NORMAL)==MOUSE_NORMAL)
		{
			if(IsWindow(gDxObj.m_hwnd))
			{
				::ClientToScreen(gDxObj.m_mousekeyhwnd,&pos);
				if(TSRuntime::SetSimModeType==0)//普通模式
					::SetCursorPos(pos.x,pos.y);
				gDxObj.LastMouseMoveX=pos.x;
				gDxObj.LastMouseMoveY=pos.y;
			}
			else
			{
				if(TSRuntime::SetSimModeType==0)//普通模式
					TSRuntime::g_Mouse.mouseMvrelative(rx,ry);
				gDxObj.LastMouseMoveX=rx+gDxObj.LastMouseMoveX;
				gDxObj.LastMouseMoveY=ry+gDxObj.LastMouseMoveY;
			}
		}
		else
		{
			if(gDxObj.m_mouseMode&MOUSE_WINDOW)
			{
				PostMessage(gDxObj.m_mousekeyhwnd,WM_MOUSEMOVE,0,MAKELPARAM(rx+gDxObj.LastMouseMoveX,ry+gDxObj.LastMouseMoveY));
			}
			else
			{
				*rnet=gDxObj.SendMsg(TS_MOUSER,0,MAKELPARAM(rx,ry));
			}
			gDxObj.LastMouseMoveX=rx+gDxObj.LastMouseMoveX;
			gDxObj.LastMouseMoveY=ry+gDxObj.LastMouseMoveY;
		}
	}

	//*rnet=gDxObj.SendMsg(TS_MOUSEMOVEOVER);
	//*rnet=TSRuntime::g_DxObj.SendMsg(TS_MOUSER,0,MAKELPARAM(rx,ry));
	//*rnet=TSRuntime::g_DxObj.SendMsg(TS_MOUSEMOVEOVER);
	*rnet=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::Ver(BSTR* retver)
{
	// TODO: 在此添加实现代码
	//_variant_t strVar(TS_VERSION);
	//*retver=strVar.bstrVal;
	CComBSTR newbstr;
	newbstr.Append(TS_VERSION);
	newbstr.CopyTo(retver);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetPath(BSTR* retpath)
{
	// TODO: 在此添加实现代码
	//_variant_t strVar(TSRuntime::SetPath);
	//strVar.bstrVal;
	//	TsMutexlock ts_mutexlock(&gDXbindMutex);
	CComBSTR newbstr;
	newbstr.Append(gDxObj.m_SetPath);
	newbstr.CopyTo(retpath);

	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::MiddleClick(LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXMiddleClickMutex);

	if((gDxObj.m_mouseMode&MOUSE_NORMAL)==MOUSE_NORMAL)
	{
		TSRuntime::g_Mouse.delaytimes=gDxObj.NormalClicksleep;
		//TSRuntime::g_Mouse.middleClick();
		if(TSRuntime::SetSimModeType==0)//普通模式
			TSRuntime::g_Mouse.middleClick();
	}
	else
	{
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)
			PostMessage(gDxObj.m_mousekeyhwnd,WM_MBUTTONUP,1,0);
		* nret=gDxObj.SendMsg(TS_MBUTTONDOWN,1,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)	//window模式默认延时
		{
			if(gDxObj.IsTSEnableRealMouse)
				Sleep(gDxObj.TSRealKeyMousSleepTime(0));
			else
				Sleep(gDxObj.WindowsKeyMouseClicksleep);
		}
		else
		{
			if(gDxObj.IsTSEnableRealMouse)
				Sleep(gDxObj.TSRealKeyMousSleepTime(0));
			else
				Sleep(gDxObj.MouseClicksleep);  //dx模式默认延时
		}
		PostMessage(gDxObj.m_mousekeyhwnd,WM_MBUTTONUP,0,0);
		* nret=gDxObj.SendMsg(TS_MBUTTONUP);
	}

	*nret=1;
	return S_OK;

}

STDMETHODIMP CTSPlugInterFace::WheelDown(LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXWheelDownMutex);

	if((gDxObj.m_mouseMode&MOUSE_NORMAL)==MOUSE_NORMAL)
	{
		TSRuntime::g_Mouse.mouseSheetDown();
	}
	else
	{
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)
			PostMessage(gDxObj.m_mousekeyhwnd,WM_MOUSEWHEEL,0xff880000,0);
		* nret=gDxObj.SendMsg(TS_MOUSEWHEEL,0xff880000,0);
	}
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::WheelUp(LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXWheelUpMutex);

	if((gDxObj.m_mouseMode&MOUSE_NORMAL)==MOUSE_NORMAL)
	{
		TSRuntime::g_Mouse.mouseSheetUp();
	}
	else
	{
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)
			PostMessage(gDxObj.m_mousekeyhwnd,WM_MOUSEWHEEL,0x780000,0);
		* nret=gDxObj.SendMsg(TS_MOUSEWHEEL,0x780000,0);
	}
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::Capture(LONG x1, LONG y1, LONG x2, LONG y2, BSTR file, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXCaptureMutex);

	//VMPBEGIN    //VMP保护
	//if(!TSRuntime::IsVipLogin)
	// {
	// int bret=TSRuntime::VipLogin(L"",0,false);   //试用验证
	// if(bret<=0)
	//  {		
	//  INT64 t=0;
	//  if(my_myGetTickCount)
	//	  t=my_myGetTickCount();
	//  else
	//	  t=::GetTickCount();
	//  if(t==TSRuntime::checkthreadtime||abs((int)(t-TSRuntime::checkthreadtime))>=60*1000*10||TSRuntime::checkthreadtime==0)	//校验线程是否被挂起或者GetTickCount被人下钩子
	//	  {
	//	  gDxObj.SendMsg(TS_UNBIND);
	//	  Sleep(200);
	//	  gDxObj.UnBind();
	//	  Sleep(200);
	//	  gDxObj.SendMsg(TS_UNBIND);
	//	  Sleep(200);
	//	  gDxObj.UnBind();
	//	  if(TSRuntime::IsStartProtect)
	//		  TSRuntime::StartProtect(0,L"");
	//	  *nret=0;
	//	  TSRuntime::h_checkthread=NULL;
	//	  return S_OK;
	//	  }

	//  }
	// }
	// VMPEND

	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	* nret=gDxObj.Capture(x1,y1,x2,y2,file,D3DXIFF_BMP);
	//* nret=TSRuntime::g_DxObj.Capture(x1,y1,x2,y2,file,D3DXIFF_BMP);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::CaptureJpg(LONG x1, LONG y1, LONG x2, LONG y2,BSTR file, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXCaptureMutex);
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	* nret=gDxObj.Capture(x1,y1,x2,y2,file,D3DXIFF_JPG);
	//* nret=TSRuntime::g_DxObj.Capture(x1,y1,x2,y2,file,D3DXIFF_JPG);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::CapturePng(LONG x1, LONG y1, LONG x2, LONG y2, BSTR file, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXCaptureMutex);
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	* nret=gDxObj.Capture(x1,y1,x2,y2,file,D3DXIFF_PNG);
	//* nret=TSRuntime::g_DxObj.Capture(x1,y1,x2,y2,file,D3DXIFF_PNG);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::LockInput(LONG lock, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXLockInputMutex);
	//* nret=TSRuntime::g_DxObj.SendMsg(TS_LOCKINPUT,lock);
	* nret=gDxObj.SendMsg(TS_LOCKINPUT,lock);
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::Ocr(LONG x1, LONG y1, LONG x2, LONG y2, BSTR color_format, FLOAT sim, BSTR* retstr)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXOcrMutex);
	if(wcslen(color_format)==0)
		return S_OK;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	//TSRuntime::g_DxObj.Ocr(x1,y1,x2,y2,color_format,sim);
	gDxObj.Ocr(x1,y1,x2,y2,color_format,sim);
	//TSRuntime::add_log( "TSRuntime::pData->RetString:%s",TSRuntime::pData->RetString);

	CComBSTR newbstr;

	if(gDxObj.pData)
		//*retstr=_bstr_t();
		newbstr.Append(gDxObj.pData->RetString);
	else
		//*retstr=_bstr_t();
		newbstr.Append(gDxObj.m_Retstr);

	newbstr.CopyTo(retstr);

	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SetDict(LONG index, BSTR file, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXSetDictMutex);
	//*nret=TSRuntime::g_DxObj.TSSetDict(index,file);
	*nret=gDxObj.TSSetDict(index,file);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::UseDict(LONG index, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXUseDictMutex);
	//*nret=TSRuntime::g_DxObj.TSUseDict(index);
	*nret=gDxObj.TSUseDict(index);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::ClearDict(LONG index, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXClearDictMutex);
	if(index>=0&&index<20&&index!=gDxObj.NowUsingDictIndex)
	{
		if(gDxObj.MyDictCount[index]!=NULL)
		{
			gDxObj.nMyDictCountsize[index]=0;
			gDxObj.nMaxStrHight[index]=0;
			delete [] gDxObj.MyDictCount[index];
			gDxObj.MyDictCount[index]=NULL;
			*nret=1;
		}
	}
	*nret=0;

	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindStr(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, FLOAT sim, VARIANT* intX, VARIANT* intY, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindStrMutex);
	LONG x=-1,y=-1,ret=-1;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	ret=gDxObj.Findstr(x1,y1,x2,y2,string,color_format,sim,x,y,0);
	intX->vt=VT_I4;
	intY->vt=VT_I4;
	intX->lVal=x;
	intY->lVal=y;
	if(x==-1||y==-1)
		ret=-1;
	*nret=ret;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindStrFast(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, FLOAT sim, VARIANT* intX, VARIANT* intY, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindStrMutex);
	//TSRuntime::add_log("FindStrFast-start:操作窗口句柄:%d",gDxObj.m_hwnd);
	LONG x=-1,y=-1,ret=-1;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	ret=gDxObj.Findstr(x1,y1,x2,y2,string,color_format,sim,x,y,1);
	intX->vt=VT_I4;
	intY->vt=VT_I4;
	intX->lVal=x;
	intY->lVal=y;
	if(x==-1||y==-1)
		ret=-1;
	*nret=ret;
	//TSRuntime::add_log("FindStrFast-end:操作窗口句柄:%d",gDxObj.m_hwnd);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetNowDict(LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXGetNowDictMutex);
	*nret=gDxObj.NowUsingDictIndex;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetBasePath(BSTR* retpath)
{
	//TsMutexlock ts_mutexlock(&gDXbindMutex);
	// TODO: 在此添加实现代码
	//*retpath=_bstr_t(gDLLFolder);
	CComBSTR newbstr;
	newbstr.Append(gDLLFolder);
	newbstr.CopyTo(retpath);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::IsDisplayDead(LONG x1, LONG y1, LONG x2, LONG y2, LONG t, LONG* bret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindPicMutex);
	LONG x=-1,y=-1,ret=-1;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	ret=gDxObj.IsDisplayDead(x1,y1,x2,y2,t);
	*bret=ret;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindPicEx(LONG x1, LONG y1, LONG x2, LONG y2, BSTR pic_name, BSTR delta_color, DOUBLE sim, LONG dir, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindPicMutex);

	LONG x=-1,y=-1;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	wchar_t retstr[MAX_PATH*100]={0};
	gDxObj.findPic(x1,y1,x2,y2,pic_name,delta_color,sim,dir,x,y,retstr);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindStrEx(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, DOUBLE sim, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindStrMutex);
	LONG x=-1,y=-1;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	wchar_t retstr[MAX_PATH*100]={0};
	gDxObj.Findstr(x1,y1,x2,y2,string,color_format,sim,x,y,0,retstr);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindStrFastEx(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, DOUBLE sim, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindStrMutex);
	LONG x=-1,y=-1;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	wchar_t retstr[MAX_PATH*100]={0};
	gDxObj.Findstr(x1,y1,x2,y2,string,color_format,sim,x,y,1,retstr);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::CaptureGif(LONG x1, LONG y1, LONG x2, LONG y2, BSTR file, LONG delay, LONG time, LONG* nret)
{
	TsMutexlock ts_mutexlock(&gDXCaptureGifMutex);
	// TODO: 在此添加实现代码
	*nret=gDxObj.CaptureGif(x1,y1,x2,y2,file,delay,time);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::DownCpu(LONG rate, LONG* nret)
{
	// TODO: 在此添加实现代码
	*nret=gDxObj.DownCpu(rate);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SetKeypadDelay(BSTR type, LONG delay, LONG* nret)
{
	// TODO: 在此添加实现代码
	if((gDxObj.m_keyMode&KEY_NORMAL)==KEY_NORMAL)
		gDxObj.NormalKeysleep=delay;
	else if(gDxObj.m_keyMode&KEY_WINDOW)	//window模式默认延时)
		gDxObj.WindowsKeyMouseClicksleep=delay;
	else
		gDxObj.KeyDownsleep=delay;
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SetMouseDelay(BSTR type, LONG delay, LONG* nret)
{
	// TODO: 在此添加实现代码
	if((gDxObj.m_mouseMode&MOUSE_NORMAL)==MOUSE_NORMAL)
		gDxObj.NormalClicksleep=delay;
	else if(gDxObj.m_mouseMode&MOUSE_WINDOW)	//window模式默认延时
		gDxObj.WindowsKeyMouseClicksleep=delay;
	else
		gDxObj.MouseClicksleep=delay;
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::CmpColor(LONG x, LONG y, BSTR color, DOUBLE sim, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutex gDXGetColorMutex(TS_GETCOLOREVEN);
	*nret=gDxObj.CmpColor(x,y,color,sim);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SendStringIme(BSTR str, LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXSendStringImeMutex);

	if(wcslen(str)>1024)
	{
		*nret=0;
		return S_OK;
	}
	memset(gDxObj.pData->pmsg,0,2048);
	USES_CONVERSION;
	strcpy(gDxObj.pData->pmsg,W2A(str));
	gDxObj.SendMsg(TS_SENDSTRINGIME);
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindColorEx(LONG x1, LONG y1, LONG x2, LONG y2, BSTR color, DOUBLE sim, LONG dir, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindColorMutex);
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	DWORD ncolor=0x100;
	LONG x=-1,y=-1;
	wchar_t retstr[MAX_PATH*4]={0};
	gDxObj.findColor(x1,y1,x2,y2,ncolor,sim,dir,x,y,color,retstr);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::EnumWindow(LONG parent, BSTR title, BSTR class_name, LONG filter, BSTR* retstr)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXEnumWindowMutex);
	wchar_t retstring[MAX_PATH*200]={0};
	gWindowObj.TSEnumWindow((HWND)parent,title,class_name,filter,retstring);
	//*retstr=_bstr_t(retstring);
	CComBSTR newbstr;
	newbstr.Append(retstring);
	newbstr.CopyTo(retstr);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::EnumWindowByProcess(BSTR process_name, BSTR title, BSTR class_name, LONG filter, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXEnumWindowByProcessMutex);
	wchar_t retstr[MAX_PATH*200]={0};
	gWindowObj.TSEnumWindow((HWND)0,title,class_name,filter,retstr,process_name);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::EnumProcess(BSTR name, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXEnumProcessMutex);
	wchar_t retstr[MAX_PATH*100]={0};
	gWindowObj.TSEnumProcess(name,retstr);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::ClientToScreen(LONG ClientToScreen, VARIANT* x, VARIANT* y, LONG* bret)
{
	// TODO: 在此添加实现代码
	x->vt=VT_I4;
	y->vt=VT_I4;
	*bret=gWindowObj.TSClientToScreen(ClientToScreen,x->lVal,y->lVal);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindWindow(BSTR class_name,BSTR title,LONG* rethwnd)
{
	// TODO: 在此添加实现代码
	gWindowObj.TSFindWindow(class_name,title,*rethwnd);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindWindowByProcess(BSTR process_name, BSTR class_name, BSTR title, LONG* rethwnd)
{
	// TODO: 在此添加实现代码
	gWindowObj.TSFindWindowByProcess(class_name,title,*rethwnd,process_name);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindWindowByProcessId(LONG process_id, BSTR class_name, BSTR title, LONG* rethwnd)
{
	// TODO: 在此添加实现代码
	gWindowObj.TSFindWindowByProcess(class_name,title,*rethwnd,NULL,process_id);
	return S_OK;
}
STDMETHODIMP CTSPlugInterFace::FindWindowEx(LONG parent,BSTR class_name,BSTR title,LONG* rethwnd)
{
	// TODO: 在此添加实现代码
	gWindowObj.TSFindWindow(class_name,title,*rethwnd,parent);
	return S_OK;
}
STDMETHODIMP CTSPlugInterFace::GetClientRect(LONG hwnd, VARIANT* x1, VARIANT* y1, VARIANT* x2, VARIANT* y2, LONG* nret)
{
	// TODO: 在此添加实现代码
	x1->vt=VT_I4;
	y1->vt=VT_I4;
	x2->vt=VT_I4;
	y2->vt=VT_I4;
	* nret=gWindowObj.TSGetClientRect(hwnd,x1->lVal,y1->lVal,x2->lVal,y2->lVal);
	return S_OK;
}


STDMETHODIMP CTSPlugInterFace::GetClientSize(LONG hwnd, VARIANT* width, VARIANT* height, LONG* nret)
{
	// TODO: 在此添加实现代码
	width->vt=VT_I4;
	height->vt=VT_I4;
	* nret=gWindowObj.TSGetClientSize(hwnd,width->lVal,height->lVal);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetForegroundFocus(LONG* rethwnd)
{
	// TODO: 在此添加实现代码
	* rethwnd=(LONG)::GetFocus();
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetForegroundWindow(LONG* rethwnd)
{
	// TODO: 在此添加实现代码
	* rethwnd=(LONG)::GetForegroundWindow();
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetMousePointWindow(LONG* rethwnd)
{
	// TODO: 在此添加实现代码
	//::Sleep(2000);
	gWindowObj.TSGetMousePointWindow(* rethwnd);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetPointWindow(LONG x, LONG y, LONG* rethwnd)
{
	// TODO: 在此添加实现代码
	gWindowObj.TSGetMousePointWindow(* rethwnd,x,y);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetProcessInfo(LONG pid, BSTR* retstring)
{
	// TODO: 在此添加实现代码

	wchar_t retstr[MAX_PATH]={0};
	gWindowObj.TSGetProcessInfo(pid,retstr);
	//* retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetSpecialWindow(LONG flag, LONG* rethwnd)
{
	// TODO: 在此添加实现代码
	*rethwnd=0;
	if(flag==0)
		*rethwnd =(LONG)GetDesktopWindow();
	else if(flag==1)
	{
		*rethwnd=(LONG)::FindWindow(L"Shell_TrayWnd",NULL);
	}

	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetWindow(LONG hwnd, LONG flag, LONG* nret)
{
	// TODO: 在此添加实现代码
	gWindowObj.TSGetWindow(hwnd,flag,* nret);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetWindowClass(LONG hwnd, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	wchar_t classname[MAX_PATH]={0};
	::GetClassName((HWND)hwnd,classname,MAX_PATH);
	//* retstring=_bstr_t(classname);
	CComBSTR newbstr;
	newbstr.Append(classname);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetWindowProcessId(LONG hwnd, LONG* nretpid)
{
	// TODO: 在此添加实现代码
	DWORD pid=0;
	::GetWindowThreadProcessId((HWND)hwnd,&pid);
	*nretpid=pid;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetWindowProcessPath(LONG hwnd, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	DWORD pid=0;
	::GetWindowThreadProcessId((HWND)hwnd,&pid);
	wchar_t process_path[MAX_PATH]={0};
	gWindowObj.GetProcesspath(pid,process_path);
	//* retstring=_bstr_t(process_path);
	CComBSTR newbstr;
	newbstr.Append(process_path);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetWindowRect(LONG hwnd, VARIANT* x1, VARIANT* y1, VARIANT* x2, VARIANT* y2, LONG* nret)
{
	// TODO: 在此添加实现代码
	x1->vt=VT_I4;
	x2->vt=VT_I4;
	y1->vt=VT_I4;
	y2->vt=VT_I4;
	RECT winrect;
	* nret=::GetWindowRect((HWND)hwnd,&winrect);
	x1->intVal=winrect.left;
	y1->intVal=winrect.top;
	x2->intVal=winrect.right;
	y2->intVal=winrect.bottom;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetWindowState(LONG hwnd, LONG flag, LONG* rethwnd)
{
	// TODO: 在此添加实现代码
	* rethwnd=gWindowObj.TSGetWindowState(hwnd,flag);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetWindowTitle(LONG hwnd, BSTR* rettitle)
{
	// TODO: 在此添加实现代码
	wchar_t title[MAX_PATH]={0};
	::GetWindowText((HWND)hwnd,title,MAX_PATH);
	//* rettitle=_bstr_t(title);
	CComBSTR newbstr;
	newbstr.Append(title);
	newbstr.CopyTo(rettitle);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::MoveWindow(LONG hwnd, LONG x, LONG y, LONG* nret)
{
	// TODO: 在此添加实现代码
	RECT winrect;
	::GetWindowRect((HWND)hwnd,&winrect);
	int width=winrect.right-winrect.left;
	int hight=winrect.bottom-winrect.top;
	*nret=::MoveWindow((HWND)hwnd,x,y,width,hight,false);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::ScreenToClient(LONG hwnd, VARIANT* x, VARIANT* y,LONG* nret)
{
	// TODO: 在此添加实现代码
	x->vt=VT_I4;
	y->vt=VT_I4;
	POINT point;
	* nret=::ScreenToClient((HWND)hwnd,&point);
	x->intVal=point.x;
	y->intVal=point.y;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SendPaste(LONG hwnd, LONG* nret)
{
	// TODO: 在此添加实现代码
	* nret=gWindowObj.TSSendPaste(hwnd);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SetClientSize(LONG hwnd, LONG width, LONG hight, LONG* nret)
{
	// TODO: 在此添加实现代码
	*nret=gWindowObj.TSSetWindowSize(hwnd,width,hight);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SetWindowState(LONG hwnd, LONG flag, LONG* nret)
{
	// TODO: 在此添加实现代码  
	*nret=gWindowObj.TSSetWindowState(hwnd,flag);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SetWindowSize(LONG hwnd, LONG width, LONG height, LONG* nret)
{
	// TODO: 在此添加实现代码
	*nret=gWindowObj.TSSetWindowSize(hwnd,width,height,1);
	return S_OK;
}
STDMETHODIMP CTSPlugInterFace::SetWindowText(LONG hwnd,BSTR title, LONG* nret)
{
	// TODO: 在此添加实现代码  
	//*nret=gWindowObj.TSSetWindowState(hwnd,flag);
	* nret=::SetWindowText((HWND)hwnd,title);
	return S_OK;
}
STDMETHODIMP CTSPlugInterFace::SetWindowTransparent(LONG hwnd, LONG trans, LONG* nret)
{
	// TODO: 在此添加实现代码
	* nret=gWindowObj.TSSetWindowTransparent(hwnd,trans);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SetClipboard(BSTR value, LONG* nret)
{
	// TODO: 在此添加实现代码
	* nret=gWindowObj.TSSetClipboard(value);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetClipboard(BSTR* retstring)
{
	// TODO: 在此添加实现代码
	wchar_t retstr[MAX_PATH*4]={0};
	gWindowObj.TSGetClipboard(retstr);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::DoubleToData(DOUBLE value, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	wchar_t retstr[MAX_PATH]={0};
	gMemoryObj.TSValueTypeToData(0,retstr,value);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FloatToData(FLOAT value, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	wchar_t retstr[MAX_PATH]={0};
	gMemoryObj.TSValueTypeToData(1,retstr,NULL,value);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::IntToData(LONG value, LONG type, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	wchar_t retstr[MAX_PATH]={0};
	gMemoryObj.TSValueTypeToData(2,retstr,NULL,NULL,value,NULL,type);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::StringToData(BSTR value, LONG type, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	wchar_t retstr[MAX_PATH]={0};
	gMemoryObj.TSValueTypeToData(3,retstr,NULL,NULL,NULL,value,type);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindData(LONG hwnd, BSTR addr_range, BSTR data, BSTR* retstring)
{
	TsMutexlock ts_mutexlock(&gDXFindDataMutex);
	// TODO: 在此添加实现代码
	//wchar_t retstr[MAX_PATH*200]={0};
	//gMemoryObj.TSFindData(hwnd,addr_range,data,retstr);
	//*retstring=_bstr_t(retstr);
	wchar_t retstr[MAX_PATH*1024]={0};
	CComBSTR newbstr;
	gMemoryObj.TSFindData(hwnd,addr_range,data,retstr);
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindDouble(LONG hwnd, BSTR addr_range, DOUBLE double_value_min, DOUBLE double_value_max, BSTR* retstring)
{
	TsMutexlock ts_mutexlock(&gDXFindDoubleMutex);
	// TODO: 在此添加实现代码
	//wchar_t retstr[MAX_PATH*200]={0};
	//gMemoryObj.TSFindDouble(hwnd,addr_range,retstr,double_value_min,double_value_max);
	//*retstring=_bstr_t(retstr);
	wchar_t retstr[MAX_PATH*1024]={0};
	CComBSTR newbstr;
	gMemoryObj.TSFindDouble(hwnd,addr_range,retstr,double_value_min,double_value_max);
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindFloat(LONG hwnd, BSTR addr_range, FLOAT float_value_min, FLOAT float_value_max, BSTR* retstring)
{
	TsMutexlock ts_mutexlock(&gDXFindFloatMutex);
	// TODO: 在此添加实现代码
	wchar_t retstr[MAX_PATH*1024]={0};
	CComBSTR newbstr;
	gMemoryObj.TSFindFloat(hwnd,addr_range,retstr,float_value_min,float_value_max);
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindInt(LONG hwnd, BSTR addr_range, LONG int_value_min, LONG int_value_max, LONG type, BSTR* retstring)
{
	TsMutexlock ts_mutexlock(&gDXFindIntMutex);
	// TODO: 在此添加实现代码
	//wchar_t retstr[MAX_PATH*200]={0};
	//gMemoryObj.TSFindInt(hwnd,addr_range,retstr,int_value_min,int_value_max,type);
	//*retstring=_bstr_t(retstr);
	wchar_t retstr[MAX_PATH*1024]={0};
	CComBSTR newbstr;
	gMemoryObj.TSFindInt(hwnd,addr_range,retstr,int_value_min,int_value_max,type);
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);

	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindString(LONG hwnd, BSTR addr_range, BSTR string_value, LONG type, BSTR* retstring)
{
	TsMutexlock ts_mutexlock(&gDXFindStringMutex);
	// TODO: 在此添加实现代码
	//wchar_t retstr[MAX_PATH*200]={0};
	//gMemoryObj.TSFindString(hwnd,addr_range,retstr,string_value,type);
	//*retstring=_bstr_t(retstr);
	wchar_t retstr[MAX_PATH*1024]={0};
	CComBSTR newbstr;
	gMemoryObj.TSFindString(hwnd,addr_range,retstr,string_value,type);
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::ReadData(LONG hwnd, BSTR addr, LONG len, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	wchar_t retstr[MAX_PATH]={0};
	gMemoryObj.TSReadData(hwnd,addr,retstr,len);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::OcrEx(LONG x1, LONG y1, LONG x2, LONG y2, BSTR color_format, DOUBLE sim, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXOcrMutex);
	if(wcslen(color_format)==0)
		return S_OK;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	//TSRuntime::g_DxObj.Ocr(x1,y1,x2,y2,color_format,sim);
	gDxObj.Ocr(x1,y1,x2,y2,color_format,sim,1);
	//TSRuntime::add_log( "TSRuntime::pData->RetString:%s",TSRuntime::pData->RetString);
	CComBSTR newbstr;

	if(gDxObj.pData)
		//*retstring=_bstr_t(gDxObj.pData->RetString);
		newbstr.Append(gDxObj.pData->RetString);
	else
		//*retstring=_bstr_t(TSRuntime::RetStr);
		newbstr.Append(gDxObj.m_Retstr);

	newbstr.CopyTo(retstring);

	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::ReadDouble(LONG hwnd, BSTR addr, DOUBLE* retdouble)
{
	// TODO: 在此添加实现代码
	float fvalue=0;
	gMemoryObj.TSReadDouble(hwnd,addr,* retdouble,fvalue);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::ReadFloat(LONG hwnd, BSTR addr, FLOAT* retfloat)
{
	// TODO: 在此添加实现代码
	DOUBLE dvalue=0;
	gMemoryObj.TSReadDouble(hwnd,addr,dvalue,* retfloat,1);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::ReadInt(LONG hwnd, BSTR addr, LONG type, LONG* retint)
{
	// TODO: 在此添加实现代码
	int ivalue=0;
	short svalue=0;
	BYTE bvalue=0;
	gMemoryObj.TSReadInt(hwnd,addr,ivalue,svalue,bvalue,type);
	if(type==0)
		*retint=ivalue;
	else if(type==1)
		*retint=svalue;
	else if(type==2)
		*retint=bvalue;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::ReadString(LONG hwnd, BSTR addr, LONG type, LONG len, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	wchar_t retstr[MAX_PATH]={0};
	gMemoryObj.TSReadString(hwnd,addr,retstr,len,type);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::TerminateProcess(LONG pid, LONG* bret)
{
	// TODO: 在此添加实现代码
	* bret=gMemoryObj.TSTerminateProcess(pid);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::VirtualAllocEx(LONG hwnd, LONG addr, LONG size, LONG type, LONG* bret)
{
	// TODO: 在此添加实现代码
	LONG retaddr=addr;
	gMemoryObj.TSVirtualAllocEx(hwnd,retaddr,size,type);
	* bret=retaddr;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::VirtualFreeEx(LONG hwnd, LONG addr, LONG* bret)
{
	// TODO: 在此添加实现代码
	* bret=gMemoryObj.TSVirtualFreeEx(hwnd,addr);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::WriteDouble(LONG hwnd, BSTR addr, DOUBLE v, LONG* bret)
{
	// TODO: 在此添加实现代码
	* bret=gMemoryObj.TSWriteDouble(hwnd,addr,v);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::WriteFloat(LONG hwnd, BSTR addr, FLOAT v, LONG* bret)
{
	// TODO: 在此添加实现代码
	* bret=gMemoryObj.TSWriteDouble(hwnd,addr,0,v);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::WriteInt(LONG hwnd, BSTR addr, LONG type, LONG v, LONG* bret)
{
	// TODO: 在此添加实现代码
	int ivalue=0;
	short svalue=0;
	BYTE bvalue=0;
	if(type==0)	//0 : 32位
		ivalue=v;
	else if(type==1)//1 : 16 位
		svalue=v;
	else if(type==2) //2 : 8位
		bvalue=v;
	* bret=gMemoryObj.TSWriteInt(hwnd,addr,ivalue,svalue,bvalue);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::WriteString(LONG hwnd, BSTR addr, LONG type, BSTR v, LONG* bret)
{
	// TODO: 在此添加实现代码
	* bret=gMemoryObj.TSWriteString(hwnd,addr,v,type);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::WriteData(LONG hwnd, BSTR addr, BSTR data, LONG* nret)
{
	// TODO: 在此添加实现代码
	* nret=gMemoryObj.TSWriteData(hwnd,addr,data);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::IsBind(LONG hwnd, LONG* nret)
{
	// TODO: 在此添加实现代码

	if(::IsWindow((HWND)hwnd)==false)
	{
		* nret=0;
		return S_OK;
	}
	if(gDxObj.m_isBind==true)
	{
		* nret=1;
		return S_OK;
	}

	//DWORD nPid=0;
	//::GetWindowThreadProcessId((HWND)hwnd,&nPid);

	char pszBinWindow_SetWindowHook[MAX_PATH]={0};
	sprintf( pszBinWindow_SetWindowHook,"%s%d",TS_BINDWINDOW_SETWINDOWHOOK,gDxObj.nPid);
	HANDLE hbindprop=GetPropA((HWND)hwnd,pszBinWindow_SetWindowHook);

	char pszBinWindow_IME[MAX_PATH]={0};
	sprintf( pszBinWindow_IME,"%s%d",TS_BINDWINDOW_IME,gDxObj.nPid);
	HANDLE hbindIMEprop=GetPropA((HWND)hwnd,pszBinWindow_IME);

	char pszBinWindow_IME101[MAX_PATH]={0};
	sprintf( pszBinWindow_IME101,"%s%d",TS_BINDWINDOW_IME101,gDxObj.nPid);
	HANDLE hbindIMEprop101=GetPropA((HWND)hwnd,pszBinWindow_IME101);

	char pszBinWindow_GDIWINDOW[MAX_PATH]={0};
	sprintf( pszBinWindow_GDIWINDOW,"%s%d",TS_BINDWINDOW_GDIWINDOW,gDxObj.nPid);
	HANDLE hbindGdiWindow=GetPropA((HWND)hwnd,pszBinWindow_GDIWINDOW);
	if(hbindGdiWindow==NULL)
	{
		//遍历获取父窗口句柄
		HWND parenthwnd=0;
		HWND hparent=0;
		do{
			if(parenthwnd==0)
				parenthwnd=GetParent((HWND)hwnd);
			else
			{
				hparent=GetParent(parenthwnd);
				if(hparent!=0&&hparent!=parenthwnd)
				{
					hwnd=(LONG)hparent;
				}
				parenthwnd=hparent;
			}

		}while(parenthwnd!=NULL);
		//::GetWindowThreadProcessId((HWND)hwnd,&nPid);
		char pszBinWindow_GDIWINDOW[MAX_PATH]={0};
		sprintf( pszBinWindow_GDIWINDOW,"%s%d",TS_BINDWINDOW_GDIWINDOW,gDxObj.nPid);
		hbindGdiWindow=GetPropA((HWND)hwnd,pszBinWindow_GDIWINDOW);
	}

	if(hbindprop==NULL&&hbindIMEprop==NULL&&hbindIMEprop101==NULL&&hbindGdiWindow==NULL)
		* nret=0;
	else
		* nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindFloatEx(LONG hwnd, BSTR addr_range, FLOAT float_value_min, FLOAT float_value_max, LONG step, LONG multi_thread, LONG mode, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindFloatMutex);
	wchar_t retstr[MAX_PATH*1024]={0};
	CComBSTR newbstr;
	gMemoryObj.TSFindFloat(hwnd,addr_range,retstr,float_value_min,float_value_max,multi_thread);
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindDoubleEx(LONG hwnd, BSTR addr_range, DOUBLE double_value_min, DOUBLE double_value_max, LONG step, LONG multi_thread, LONG mode, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindDoubleMutex);
	wchar_t retstr[MAX_PATH*1024]={0};
	CComBSTR newbstr;
	gMemoryObj.TSFindDouble(hwnd,addr_range,retstr,double_value_min,double_value_max,multi_thread);
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindIntEx(LONG hwnd, BSTR addr_range, LONG int_value_min, LONG int_value_max, LONG type, LONG step, LONG multi_thread, LONG mode, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindIntMutex);
	wchar_t retstr[MAX_PATH*1024]={0};
	CComBSTR newbstr;
	gMemoryObj.TSFindInt(hwnd,addr_range,retstr,int_value_min,int_value_max,type,multi_thread);
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindDataEx(LONG hwnd, BSTR addr_range, BSTR data, LONG step, LONG multi_thread, LONG mode, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindDataMutex);
	wchar_t retstr[MAX_PATH*1024]={0};
	CComBSTR newbstr;
	gMemoryObj.TSFindData(hwnd,addr_range,data,retstr,multi_thread);
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindStringEx(LONG hwnd, BSTR addr_range, BSTR string_value, LONG type, LONG step, LONG multi_thread, LONG mode, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindStringMutex);
	wchar_t retstr[MAX_PATH*1024]={0};
	CComBSTR newbstr;
	gMemoryObj.TSFindString(hwnd,addr_range,retstr,string_value,type,multi_thread);
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetModuleBaseAddr(LONG hwnd, BSTR modulename, LONG* nret)
{
	// TODO: 在此添加实现代码
	DWORD pid=0;
	::GetWindowThreadProcessId((HWND)hwnd,&pid);
	* nret=TSRuntime::GetRemoteModuleBaseAddr(modulename,pid);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetCommandLine(LONG hwnd, BSTR* retstring)
{
	// TODO: 在此添加实现代码;		   
	wchar_t argv[MAX_PATH]={0};
	gMemoryObj.TSGetCmdLine(hwnd,argv);
	//*retstring=_bstr_t(argv);
	CComBSTR newbstr;
	newbstr.Append(argv);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::AsmAdd(BSTR asm_ins, LONG* nret)
{
	// TODO: 在此添加实现代码
	* nret=gMemoryObj.TSAsmAdd(asm_ins);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::AsmCall(LONG hwnd, LONG mode, LONG* nret)
{
	// TODO: 在此添加实现代码
	* nret=gMemoryObj.TSAsmCall(hwnd,mode);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::AsmClear(LONG* nret)
{
	// TODO: 在此添加实现代码

	gMemoryObj.TSAsmClear();
	* nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::AsmCode(LONG base_addr, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	wchar_t  retstr[MAX_PATH*10]={0};
	//std::string	  retstr;
	gMemoryObj.TSAsmCode(base_addr,retstr);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::Assemble(BSTR asm_code, LONG base_addr, LONG is_upper, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	wchar_t  retstr[MAX_PATH*10]={0};
	gMemoryObj.TSAssemble(asm_code,base_addr,is_upper,retstr);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::MatchPicName(BSTR pic_name, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	wchar_t  retstr[MAX_PATH*10]={0};
	gDxObj.TSMatchPicName(pic_name,retstr);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SetShowErrorMsg(LONG show, LONG* nret)
{
	// TODO: 在此添加实现代码
	TSRuntime::IsShowErrorMsg=show;
	* nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::Reg(BSTR reg_code, LONG type, LONG* nret)
{
	// TODO: 在此添加实现代码
	VMPBEGIN
		//::GetModuleHandle(L"TSPlug.dll");
		int ret=TSRuntime::VipLogin(reg_code,type);
	if(ret>0&&type==0)//特权验证
	{
		TSRuntime::IsVipLogin=true;
		ret=1;
	}
	else if(ret>0&&type==1)//注册验证
	{
		TSRuntime::IsRegLogin=true;
		ret=1;
	}
	* nret=ret;

	VMPEND
		return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetMachineCode(BSTR* retstring)
{
	// TODO: 在此添加实现代码
	wchar_t machinecode[MAX_PATH]={0};
	gDxObj.TSGetMachineCode(machinecode);
	//* retstring=_bstr_t(machinecode);
	CComBSTR newbstr;
	newbstr.Append(machinecode);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::TSGuardProtectToHide(LONG enble, LONG* nret)
{
	// TODO: 在此添加实现代码
	VMPBEGIN    //VMP保护
		if(!TSRuntime::IsVipLogin)
		{
			if(TSRuntime::IsRegLogin==false&&TSRuntime::IsVipLogin==false)//TS盾不允许试用
			{
				*nret=0;
				return S_OK;
			}
			INT64 t=0;
			if(my_myGetTickCount)
				t=my_myGetTickCount();
			else
				t=::GetTickCount();
			if(t==TSRuntime::checkthreadtime||abs((int)(t-TSRuntime::checkthreadtime))>=60*1000*10||TSRuntime::checkthreadtime==0)	//校验线程是否被挂起或者GetTickCount被人下钩子
			{
				if(TSRuntime::IsStartProtect)
					TSRuntime::StartProtect(0,0,-1);
				*nret=0;
				TSRuntime::h_checkthread=NULL;
				return S_OK;
			}
		}
		*nret=TSRuntime::StartProtect(enble,1);
		//TSRuntime::add_log( "TSRuntime::IsVipLogin:%d,*nret:%d",TSRuntime::IsVipLogin,*nret);
		VMPEND
			return S_OK;
}

STDMETHODIMP CTSPlugInterFace::TSGuardProtectToHide2(LONG enable, LONG* nret)
{
	// TODO: 在此添加实现代码
	VMPBEGIN    //VMP保护
		if(!TSRuntime::IsVipLogin)
		{
			if(TSRuntime::IsRegLogin==false&&TSRuntime::IsVipLogin==false)//TS盾不允许试用
			{
				*nret=0;
				return S_OK;
			}
			INT64 t=0;
			if(my_myGetTickCount)
				t=my_myGetTickCount();
			else
				t=::GetTickCount();
			if(t==TSRuntime::checkthreadtime||abs((int)(t-TSRuntime::checkthreadtime))>=60*1000*10||TSRuntime::checkthreadtime==0)	//校验线程是否被挂起或者GetTickCount被人下钩子
			{
				if(TSRuntime::IsStartProtect)
					TSRuntime::StartProtect(0,0,-1);
				*nret=0;
				TSRuntime::h_checkthread=NULL;
				return S_OK;
			}
		}
		*nret=TSRuntime::StartProtect(enable,2);
		//TSRuntime::add_log( "TSRuntime::IsVipLogin:%d,*nret:%d",TSRuntime::IsVipLogin,*nret);
		VMPEND
			return S_OK;
}

STDMETHODIMP CTSPlugInterFace::TSGuardProtectToNP(LONG enable, LONG* nret)
{
	// TODO: 在此添加实现代码
	VMPBEGIN    //VMP保护
		if(!TSRuntime::IsVipLogin)
		{
			if(TSRuntime::IsRegLogin==false&&TSRuntime::IsVipLogin==false)//TS盾不允许试用
			{
				*nret=0;
				return S_OK;
			}
			INT64 t=0;
			if(my_myGetTickCount)
				t=my_myGetTickCount();
			else
				t=::GetTickCount();
			if(t==TSRuntime::checkthreadtime||abs((int)(t-TSRuntime::checkthreadtime))>=60*1000*10||TSRuntime::checkthreadtime==0)	//校验线程是否被挂起或者GetTickCount被人下钩子
			{
				if(TSRuntime::IsStartProtect)
					TSRuntime::StartProtect(0,0,-1);
				*nret=0;
				TSRuntime::h_checkthread=NULL;
				return S_OK;
			}
		}
		TSRuntime::InitialWindowsVersion();
		if(TSRuntime::IsWin7X64==false)
			TSRuntime::StartProtect(enable,0);
		if(TSRuntime::IsStartProtect==true||TSRuntime::IsWin7X64==true)
		{
			TSRuntime::IsNpProtect=true;
			*nret=true;
		}
		//TSRuntime::add_log( "TSRuntime::IsVipLogin:%d,*nret:%d",TSRuntime::IsVipLogin,*nret);
		VMPEND
			return S_OK;
}

STDMETHODIMP CTSPlugInterFace::Delay(LONG mis, LONG* nret)
{
	// TODO: 在此添加实现代码
	//TsMutexlock ts_mutexlock(&gDXMySleepMutex);
	gDxObj.mysleep(mis);
	*nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindStrS(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, DOUBLE sim, VARIANT* intX, VARIANT* intY, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindStrMutex);
	LONG x=-1,y=-1,ret=-1;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	wchar_t retstr[MAX_PATH*100]={0};
	ret=gDxObj.Findstr(x1,y1,x2,y2,string,color_format,sim,x,y,0,retstr,2);
	intX->vt=VT_I4;
	intY->vt=VT_I4;
	intX->lVal=x;
	intY->lVal=y;
	if(x==-1||y==-1)
		ret=-1;
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}
STDMETHODIMP CTSPlugInterFace::FindStrFastS(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, DOUBLE sim, VARIANT* intX, VARIANT* intY, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindStrMutex);
	LONG x=-1,y=-1,ret=-1;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	wchar_t retstr[MAX_PATH*100]={0};
	ret=gDxObj.Findstr(x1,y1,x2,y2,string,color_format,sim,x,y,1,retstr,2);
	intX->vt=VT_I4;
	intY->vt=VT_I4;
	intX->lVal=x;
	intY->lVal=y;
	if(x==-1||y==-1)
		ret=-1;
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindStrExS(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, DOUBLE sim, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindStrMutex);
	LONG x=-1,y=-1,ret=-1;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	wchar_t retstr[MAX_PATH*100]={0};
	ret=gDxObj.Findstr(x1,y1,x2,y2,string,color_format,sim,x,y,0,retstr,1);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindStrFastExS(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, DOUBLE sim, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindStrMutex);
	LONG x=-1,y=-1,ret=-1;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	wchar_t retstr[MAX_PATH*100]={0};
	ret=gDxObj.Findstr(x1,y1,x2,y2,string,color_format,sim,x,y,1,retstr,1);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindPicS(LONG x1, LONG y1, LONG x2, LONG y2, BSTR pic_name, BSTR delta_color, DOUBLE sim, LONG dir, VARIANT* intX, VARIANT* intY, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindPicMutex);
	LONG x=-1,y=-1;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	wchar_t retstr[MAX_PATH*100]={0};
	gDxObj.findPic(x1,y1,x2,y2,pic_name,delta_color,sim,dir,x,y,retstr,2);
	intX->vt=VT_I4;
	intY->vt=VT_I4;
	intX->lVal=x;
	intY->lVal=y;
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindPicExS(LONG x1, LONG y1, LONG x2, LONG y2, BSTR pic_name, BSTR delta_color, DOUBLE sim, LONG dir, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindPicMutex);
	LONG x=-1,y=-1;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	wchar_t retstr[MAX_PATH*100]={0};
	gDxObj.findPic(x1,y1,x2,y2,pic_name,delta_color,sim,dir,x,y,retstr,1);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SetDictPwd(BSTR pwd, LONG* nret)
{
	// TODO: 在此添加实现代码
	int len=wcslen(pwd);
	if(len>0&&len<MAX_PATH)
	{
		wcscpy(TSRuntime::SetDictPwdString,pwd);
		* nret=1;
	}
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SetPicPwd(BSTR pwd, LONG* nret)
{
	// TODO: 在此添加实现代码
	int len=wcslen(pwd);
	if(len>0&&len<MAX_PATH)
	{
		wcscpy(TSRuntime::SetPicPwdString,pwd);
		if(gDxObj.pData)
		{
			wcscpy(gDxObj.pData->SetPicPwdString,pwd);
			gDxObj.SendMsg(TS_SETPICPWD);
		}

		* nret=1;
	}
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::LeftDoubleClick(LONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXLeftDownMutex);
	if((gDxObj.m_mouseMode&MOUSE_NORMAL)==MOUSE_NORMAL)
	{
		TSRuntime::g_Mouse.delaytimes=gDxObj.NormalClicksleep;
		//TSRuntime::g_Mouse.leftDoubleClick();
		if(TSRuntime::SetSimModeType==0)//普通模式
			TSRuntime::g_Mouse.leftDoubleClick();
	}
	else
	{
		if(gDxObj.m_mouseMode&MOUSE_WINDOW)
		{
			SendMessage(gDxObj.m_mousekeyhwnd,WM_SETFOCUS,(WPARAM)gDxObj.m_mousekeyhwnd,0);
			SendMessage(gDxObj.m_mousekeyhwnd,WM_CAPTURECHANGED,(WPARAM)gDxObj.m_mousekeyhwnd,0);
			PostMessage(gDxObj.m_mousekeyhwnd,WM_LBUTTONDBLCLK,1,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
		}
		else
			*nret=gDxObj.SendMsg(TS_LBUTTONDOWN,WM_LBUTTONDBLCLK,MAKELPARAM(gDxObj.LastMouseMoveX,gDxObj.LastMouseMoveY));
	}
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FreeProcessMemory(LONG hwnd, LONG* nret)
{
	// TODO: 在此添加实现代码
	* nret=gMemoryObj.TSFreeProcessMemory(hwnd);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::TSDXGraphicProtect(LONG enable, LONG* nret)
{
	// TODO: 在此添加实现代码
	VMPBEGIN    //VMP保护
		if(!TSRuntime::IsVipLogin)
		{
			if(TSRuntime::IsRegLogin==false&&TSRuntime::IsVipLogin==false)//TS盾不允许试用
			{
				*nret=0;
				return S_OK;
			}
			INT64 t=0;
			if(my_myGetTickCount)
				t=my_myGetTickCount();
			else
				t=::GetTickCount();
			if(t==TSRuntime::checkthreadtime||abs((int)(t-TSRuntime::checkthreadtime))>=60*1000*10||TSRuntime::checkthreadtime==0)	//校验线程是否被挂起或者GetTickCount被人下钩子
			{
				if(TSRuntime::IsStartProtect)
					TSRuntime::StartProtect(0,0,-1);
				*nret=0;
				TSRuntime::h_checkthread=NULL;
				return S_OK;
			}
		}
		gDxObj.Isgraphicprotect=enable;
		if(gDxObj.pData)
		{
			if(enable==1)
			{
				gDxObj.pData->isgraphicprotect=true;
				gDxObj.Isgraphicprotect=true;
				Sleep(100);//等待游戏内部图色数据刷新
			}
			*nret=gDxObj.SendMsg(TS_ISGRAPHICPROTECT,enable);
			gDxObj.pData->ispichook=enable;
		}
		*nret=1;
		VMPEND
		return S_OK;
}

STDMETHODIMP CTSPlugInterFace::TSDXKmProtect(LONG enable, BSTR type, LONG* nret)
{
	// TODO: 在此添加实现代码
	VMPBEGIN    //VMP保护
	if(!TSRuntime::IsVipLogin)
	{
		if(TSRuntime::IsRegLogin==false&&TSRuntime::IsVipLogin==false)//TS盾不允许试用
		{
			*nret=0;
			return S_OK;
		}
		INT64 t=0;
		if(my_myGetTickCount)
			t=my_myGetTickCount();
		else
			t=::GetTickCount();
		if(t==TSRuntime::checkthreadtime||abs((int)(t-TSRuntime::checkthreadtime))>=60*1000*10||TSRuntime::checkthreadtime==0)	//校验线程是否被挂起或者GetTickCount被人下钩子
		{
			if(TSRuntime::IsStartProtect)
				TSRuntime::StartProtect(0,0,-1);
			*nret=0;
			TSRuntime::h_checkthread=NULL;
			return S_OK;
		}
	}
	gDxObj.Isdxkmprotect=enable;
	if(wcsicmp(type,L"dx.anti.api")==0)
		gDxObj.Isdxantiapi=true;
	else
		gDxObj.Isdxantiapi=false;

	*nret=1;
	VMPEND
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::CheckFontSmooth(LONG* nret)
{
	// TODO: 在此添加实现代码

	HKEY Reg_type=HKEY_LOCAL_MACHINE;
	HKEY hKey = NULL;
	wchar_t tempstr[MAX_PATH*2]={0};
	DWORD size=MAX_PATH*sizeof(wchar_t),dwType=0;
	LONG bret=0;
	if(RegOpenKeyEx(HKEY_CURRENT_USER, L"Control Panel\\Desktop", 0, KEY_WRITE|KEY_READ,&hKey)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hKey,L"FontSmoothing",NULL,&dwType,(LPBYTE)tempstr,&size)==ERROR_SUCCESS)
		{
			if(_wtoi(tempstr)==2)
				bret=1;
		}

	}
	* nret=bret;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::DisableFontSmooth(LONG* nret)
{
	// TODO: 在此添加实现代码

	HKEY Reg_type=HKEY_LOCAL_MACHINE;
	HKEY hKey = NULL;
	LONG bret=0;
	if(RegOpenKeyEx(HKEY_CURRENT_USER, L"Control Panel\\Desktop", 0, KEY_WRITE|KEY_READ,&hKey)==ERROR_SUCCESS)
	{
		if(RegSetValueEx(hKey,L"FontSmoothing",NULL,REG_SZ,(LPBYTE)L"0",sizeof(wchar_t)*2)==ERROR_SUCCESS)
			bret=1;
	}
	* nret=bret;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::GetScreenData(LONG x1, LONG y1, LONG x2, LONG y2, LONG* RetAddr)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXMyGetScreenDataMutex);
	LONG x=-1,y=-1,ret=-1;
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	gDxObj.TSGetScreenData(x1,y1,x2,y2);
	*RetAddr=(ULONG)gDxObj.ColorDataInfo;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::EnableRealMouse(ULONG enable, ULONG mousedelay, ULONG mousestep, ULONG* nret)
{
	// TODO: 在此添加实现代码
	if(enable)
	{
		gDxObj.IsTSEnableRealMouse=true;
		gDxObj.mousedelay=mousedelay;
		gDxObj.mousestep=mousestep;
	}
	else
	{
		gDxObj.IsTSEnableRealMouse=false;
		gDxObj.mousedelay=0;
		gDxObj.mousestep=0;
	}
	* nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::EnableRealKeypad(ULONG enable, ULONG* nret)
{ 
	// TODO: 在此添加实现代码
	if(enable)
		gDxObj.IsTSEnableRealKeypad=true;
	else
		gDxObj.IsTSEnableRealKeypad=false;
	* nret=1;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::MoveToEx(ULONG x, ULONG y, ULONG w, ULONG h, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXMoveToExMutex);
	wchar_t retstr[16]={0};
	gDxObj.TSMoveToEx(x,y,w,h,retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::CheckUAC(ULONG* nret)
{
	// TODO: 在此添加实现代码
	HKEY Reg_type=HKEY_LOCAL_MACHINE;
	HKEY hKey = NULL;
	DWORD size=sizeof(DWORD),dwType=0,retvalue=0;
	LONG bret=0;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 0, KEY_WRITE|KEY_READ,&hKey)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hKey,L"ConsentPromptBehaviorAdmin",NULL,&dwType,(LPBYTE)&retvalue,&size)==ERROR_SUCCESS)
		{
			if(retvalue!=0)
				bret=1;
		}

	}
	* nret=bret;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SetUAC(ULONG enable,ULONG* nret)
{
	// TODO: 在此添加实现代码
	HKEY Reg_type=HKEY_LOCAL_MACHINE;
	HKEY hKey = NULL;
	LONG bret=0;
	DWORD vlaue=0; 
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System", 0, KEY_WRITE|KEY_READ,&hKey)==ERROR_SUCCESS)
	{
		if(enable==0)//关闭
		{
			if(RegSetValueEx(hKey,L"ConsentPromptBehaviorAdmin",NULL,REG_DWORD,(LPBYTE)&vlaue,sizeof(DWORD))==ERROR_SUCCESS)
				bret=1;
		}
		else//打开
		{
			vlaue=2;
			if(RegSetValueEx(hKey,L"ConsentPromptBehaviorAdmin",NULL,REG_DWORD,(LPBYTE)&vlaue,sizeof(DWORD))==ERROR_SUCCESS)
				bret=1;
		}
			
	}
	* nret=bret;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::WaitKey(ULONG vk_code, ULONG time_out, ULONG* nret)
{
	// TODO: 在此添加实现代码
	* nret=TSRuntime::g_KeyBoard.waitkeypress(vk_code,time_out);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindMultiColor(ULONG x1, ULONG y1, ULONG x2, ULONG y2, BSTR first_color, BSTR offset_color, DOUBLE sim, ULONG dir, VARIANT* intX, VARIANT* intY, ULONG* nret)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindColorMutex);
	if(x1>=x2||y1>=y2)
	{
		MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	//wchar_t* p=0;
	DWORD ncolor=0x100;
	LONG x=-1,y=-1,ret;
	//TSRuntime::add_log("FindColor:_wtoi:%x",ncolor);
	//ret=TSRuntime::g_DxObj.findColor(x1,y1,x2,y2,ncolor,sim,dir,x,y);
	ret=gDxObj.findColor(x1,y1,x2,y2,ncolor,sim,dir,x,y,first_color,NULL,offset_color);
	intX->vt=VT_I4;
	intY->vt=VT_I4;
	intX->lVal=x;
	intY->lVal=y;
	*nret=ret;
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::FindMultiColorEx(ULONG x1, ULONG y1, ULONG x2, ULONG y2, BSTR first_color, BSTR offset_color, DOUBLE sim, ULONG dir, BSTR* retstring)
{
	// TODO: 在此添加实现代码
	TsMutexlock ts_mutexlock(&gDXFindColorMutex);
	if(x1>=x2||y1>=y2)
	{
		if(TSRuntime::IsShowErrorMsg)
			MessageBox(NULL,L"传入坐标已越界!",L"TS",NULL);
		return S_OK;
	}
	DWORD ncolor=0x100;
	LONG x=-1,y=-1;
	wchar_t retstr[MAX_PATH*5]={0};
	gDxObj.findColor(x1,y1,x2,y2,ncolor,sim,dir,x,y,first_color,retstr,offset_color);
	//*retstring=_bstr_t(retstr);
	CComBSTR newbstr;
	newbstr.Append(retstr);
	newbstr.CopyTo(retstring);
	return S_OK;
}

STDMETHODIMP CTSPlugInterFace::SetSimMode(ULONG mode, ULONG* nret)
{
	// TODO: 在此添加实现代码

	return S_OK;
}
