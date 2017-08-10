/*
本源码由TC简单软件科技有限公司开源,功能可以自由修改、发布、
长沙简单软件科技有限公司对于源码不做后期维护,,请大家在使用过程中遵循开源协议
*/
///////////////////////////////////////
#include "stdafx.h"
#include "MyKeyboardNamespace.h"
#include <windows.h>
#include <process.h> 
#include <stdio.h>
#include <conio.h>
/*===================================================================
构造函数
====================================================================*/
MyKeyboardNamespace::MyKeyboardNamespace(void)
{
	delaytime=40;
}


/*===================================================================
析构函数
====================================================================*/
MyKeyboardNamespace::~MyKeyboardNamespace(void)
{

}
/*===================================================================
函数名称 :  keyPressDown
功能描述 :  键盘按下弹起

返回值: 返回值:BOOL类型
_TRUE表示成功,_FALSE表示失败

参数:
名称        类型             描述
-----------------------------------------------
list       MyDataList *   键盘按下弹起什么键以及键盘按下弹起具体的次数
ret        _variant_t *      接收返回值传出去

修改历史 :
修改者   日期           描述
邓媛元   2010-07-18  
邓媛元   2010-07-20     检测    
====================================================================*/
bool MyKeyboardNamespace::keyPress(DWORD keycode)
{
	unsigned char bkeydown=keycode;
    keybd_event(bkeydown,MapVirtualKey(bkeydown,0),0, 0);
    ::Sleep(delaytime);
	//2011-11-02 修改按键延时,有人反应按键太慢
    keybd_event(bkeydown,MapVirtualKey(bkeydown,0),KEYEVENTF_KEYUP, 0);
	//*ret = true;
	return true;
}

/*===================================================================
函数名称 :  keyDown
功能描述 :  键盘按下

返回值: 返回值:BOOL类型
_TRUE表示成功,_FALSE表示失败

参数:
名称        类型             描述
-----------------------------------------------
list       MyDataList *   键盘按下什么键以及键盘按下具体的次数
ret        _variant_t *      接收返回值传出去

修改历史 :
修改者   日期           描述
邓媛元   2010-07-18      
邓媛元   2010-07-20     检测 
====================================================================*/
bool  MyKeyboardNamespace::keyDown(DWORD keycode)
{	
	unsigned char bkeydown=keycode;	
	keybd_event(bkeydown, MapVirtualKey(bkeydown,0), 0, 0);
	return true;
}

/*===================================================================
函数名称 :  keyUp
功能描述 :  键盘松开

返回值: 返回值:BOOL类型
_TRUE表示成功,_FALSE表示失败

参数:
名称        类型             描述
-----------------------------------------------
list       MyDataList *   键盘弹起什么键以及键盘弹起的具体次数
ret        _variant_t *      接收返回值传出去

修改历史 :
修改者   日期           描述
邓媛元   2010-07-18      
邓媛元   2010-07-20     检测 
====================================================================*/
bool  MyKeyboardNamespace::keyUp(DWORD keycode)
{
	unsigned char bkeydown=keycode;
	keybd_event(bkeydown, MapVirtualKey(bkeydown,0), KEYEVENTF_KEYUP, 0);//放开键　
	return true;
}

/*===================================================================
函数名称 :  sendkeyString
功能描述 :  模拟键盘发送字符串

返回值: 返回值:BOOL类型
_TRUE表示成功,_FALSE表示失败

参数:
名称        类型             描述
-----------------------------------------------
list       MyDataList *   传进来键盘按下的字符串
ret        _variant_t *      接收返回值传出去

修改历史 :
修改者   日期           描述
邓媛元   2010-07-18      
邓媛元   2010-07-20     检测 
====================================================================*/
bool  MyKeyboardNamespace::sendkeyString(wchar_t *strs,DWORD delays)
{
	int length = wcslen(strs);
	for (int i = 0; i < length; ++i)
	{
		INPUT   keyin;
		keyin.type=INPUT_KEYBOARD;
		keyin.ki.wVk=0;
		keyin.ki.wScan=strs[i];
		keyin.ki.time=100;
		keyin.ki.dwFlags=KEYEVENTF_UNICODE;
		keyin.ki.dwExtraInfo=GetMessageExtraInfo();
		::SendInput(1,  &keyin, sizeof(INPUT));
		Sleep(delays);
	}
	return true;
}



BYTE keyCode[127]={	VK_BACK,VK_TAB,VK_CLEAR,VK_RETURN,VK_SHIFT,VK_CONTROL,VK_MENU,VK_PAUSE,VK_CAPITAL,VK_KANA,		//10
	VK_JUNJA,VK_FINAL,VK_HANJA,VK_KANJI,VK_ESCAPE,VK_CONVERT,VK_NONCONVERT,VK_ACCEPT,VK_MODECHANGE,VK_SPACE,		//10
	VK_PRIOR,VK_NEXT,VK_END,VK_HOME,VK_LEFT,VK_UP,VK_RIGHT,VK_DOWN,VK_SELECT,VK_PRINT,								//10
	VK_EXECUTE,VK_SNAPSHOT,VK_INSERT,VK_DELETE,VK_HELP,																//5
	'0','1','2','3','4','5','6','7','8','9',																		//10
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',		//26
	VK_LWIN,VK_RWIN,VK_APPS,VK_SLEEP,VK_NUMPAD0,VK_NUMPAD1,VK_NUMPAD2,VK_NUMPAD3,VK_NUMPAD4,VK_NUMPAD5,				//10
	VK_NUMPAD6,VK_NUMPAD7,VK_NUMPAD8,VK_NUMPAD9,VK_MULTIPLY,VK_ADD,VK_SEPARATOR,VK_SUBTRACT,VK_DECIMAL,VK_DIVIDE,	//10
	VK_F1,VK_F2,VK_F3,VK_F4,VK_F5,VK_F6,VK_F7,VK_F8,VK_F9,VK_F10,VK_F11,VK_F12,										//12
	VK_NUMLOCK,VK_SCROLL,VK_OEM_NEC_EQUAL,VK_LSHIFT,VK_RSHIFT,VK_LCONTROL,VK_RCONTROL,VK_LMENU,VK_RMENU,VK_OEM_1,	//10
	VK_OEM_PLUS,VK_OEM_COMMA,VK_OEM_MINUS,VK_OEM_PERIOD,VK_OEM_2,VK_OEM_3,VK_OEM_4,VK_OEM_5,VK_OEM_6,VK_OEM_7,		//10
	VK_OEM_8,VK_OEM_AX,VK_OEM_102
};
/*===================================================================
函数名称 :  waitkeypress
功能描述 :  暂停程序等待任意按键后继续

返回值: 返回值:BOOL类型
_TRUE表示成功,_FALSE表示失败

参数:
名称        类型             描述
-----------------------------------------------
list       MyDataList *   传进来键盘按下的字符串
ret        _variant_t *      接收返回值传出去

修改历史 :
修改者   日期           描述
邓媛元   2010-07-18      
邓媛元   2010-07-20     检测 
====================================================================*/
bool  MyKeyboardNamespace::waitkeypress(DWORD keycode,DWORD delay)
{
	MSG  msg;
	int waittime=0;
	if(delay>0)
		waittime=delay;
	int time1=::GetTickCount();
	while(true)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(WM_QUIT==msg.message)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if(keycode==0)
			{
				for(int i=0;i<127;i++)
				{
					if(0x8000 & GetAsyncKeyState(keyCode[i]))
					{
						return true;
					}
				}
			}
			else
			{
				if(0x8000 & GetAsyncKeyState(keycode))
				{
					return true;
				}

			}
			if(waittime>0)
			{
				if((::GetTickCount()-time1)>=waittime)
					return false;
			}
		}
		::Sleep(5);
	}
	return false;
}
