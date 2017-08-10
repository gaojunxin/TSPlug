#pragma once
#include "TSMessage.h"
#include "TSFindPicture.h"
typedef bool (__stdcall  *  MySetWindowHook)(BOOL,DWORD); 

//TSMOUSE_MODE 鼠标模式
#define MOUSE_NORMAL    0x00000001L
#define MOUSE_WINDOW    0x00000002L
#define MOUSE_API       0x00000004L

//TSKEYBOARD_MODE 窗口模式
#define KEY_NORMAL      0x00000001L
#define KEY_WINDOW      0x00000002L
#define KEY_API         0x00000004L
#define KEY_STATEAPI    0x00000008L
#define KEY_LOCKAPI     0x00000010L


//TSDISPLAY_MODE 显示模式
#define DISPLAY_GDI     0x00000001L
#define DISPLAY_DX_2D   0x00000002L
#define DISPLAY_DX_3D   0x00000004L
#define DISPLAY_NORMAL	0x00000008L
#define DISPLAY_DX2		0x00000010L
#define DISPLAY_GDI2	0x00000020L

//定义创建内存映射的名字宏
#define TS_MAPVIEW_NAME	 "TSMAPVIEW:REMOTECTRL"

//定义创建绑定窗口标识名字宏

//定义创建绑定窗口标识名字宏
#define TS_BINDWINDOW_GDIWINDOW	 "TSBINDWINDOWNAME_GDIWINDOW:"

#define TS_BINDWINDOW_SETWINDOWHOOK	 "TSBINDWINDOWNAME_SETWINDOWHOOK:"

#define TS_BINDWINDOW_IME	 "TSBINDWINDOWNAME_IME:"

#define TS_BINDWINDOW_IME101	 "TSBINDWINDOWNAME_IME101:"

#define TS_BINDWINDOW_103	 "TSBINDWINDOWNAME_103:"

#define TS_EVENT_IMEHOOK	 "TSEVENT:IMEHOOK:"

#define TS_EVENT_WRITEMEM	 L"TSEVENT:WRITEMEM:"

//定义一个事件内核对象名,用于处理图色处理API
#define TS_EVENT_NAME L"TSEVENT:DISPLAY"

#define TS_EVENT_FINDPIC L"TSEVENT:FINDPIC"
#define TS_EVENT_FINDSTR L"TSEVENT:FINDSTR"
#define TS_EVENT_FINDCOLOR L"TSEVENT:FINDCOLOR"

#define TS_EVENT_KEYHOOKNAME L"TSEVENT:KEYHOOK"

#define TS_IMEUNHOOKAPI_NAME L"TSEVENT:IMEUNHOOKAPI"

#define TS_BIND201_NAME L"TSEVENT:BIND201"

//定义内存映射最大值
#define TS_MAFVIEW_MAXSIZE  1024*5
//绑定模式

enum BIND_MODE
{
	BIND_WINHOOK,
	BIND_IME,
	BIND_IME101 = 101,
	BIND_103	=103,
	BIND_201	=201,
	BIND_203	=203,
    BIND_MAX,
};
////绑定的显示模式
//enum DISPLAY_MODE
//{
//	DISPLAY_GDI,
//	DISPLAY_DX,
//	DISPLAY_MAX,
//};

class DXBind
{
public:
    //////////////////////////////////这里是本地使用的API接口////////////////////////////////////
	//MyFindPictureClass  fpic;

	//绑定操作
	bool bind(HWND wnd,DWORD keyMod,DWORD mouseMod,DWORD disMod,BIND_MODE bindMode);

	bool UnBind();

	 //发送消息
     LRESULT SendMsg(UINT Msg, WPARAM wParam=0, LPARAM lParam=0) {
         return ::SendMessage(m_mousekeyhwnd, Msg, wParam, lParam);
     }

	 bool SetWindowHookExInject(HWND wnd);

	//输入法注入
	bool IMEInject(HWND wnd);

	//201注入
	bool Inject201();

	//
	int findPic(LONG x1, LONG y1, LONG x2, LONG y2, wchar_t* pic_name,  wchar_t *color, DOUBLE sim, LONG dir, LONG &intX, LONG &intY,wchar_t *retstring=NULL,int type=0);

	//
	int findColor(LONG x1, LONG y1, LONG x2, LONG y2, int color, DOUBLE sim, LONG dir, LONG &intX, LONG &intY,wchar_t *colorstr=NULL,wchar_t *retstring=NULL,wchar_t *MultiColor=NULL);

	int GetColor(LONG x,LONG y,BSTR sColor);

	bool CmpColor(LONG x,LONG y,BSTR sColor,DOUBLE sim);

	bool TSSetpath(wchar_t *path);
	//
	void setIsBind(bool isbind);

	int Capture(LONG x1, LONG y1, LONG x2, LONG y2, wchar_t* pic_path,int pictype);

	int CaptureGif(LONG x1, LONG y1, LONG x2, LONG y2, wchar_t* pic_path,int delay,int time);

	bool TSSetDict(int index,wchar_t *filepath);

	bool TSUseDict(int index);

	int Ocr(LONG x1, LONG y1, LONG x2, LONG y2, wchar_t *color, DOUBLE sim,int type=0);

	//Findstrtype=0,Findstr=Findstr;Findstrtype=1,Findstr=FindstrFast;
	int Findstr(LONG x1, LONG y1, LONG x2, LONG y2, wchar_t* findstring, wchar_t *color, DOUBLE sim, LONG &intX, LONG &intY,BYTE Findstrtype,wchar_t *retstring=NULL,int type=0);

	 //判断是否卡屏
	int IsDisplayDead(LONG x1, LONG y1, LONG x2, LONG y2,int times);

	bool TSGetScreenData(LONG x1, LONG y1, LONG x2, LONG y2);
	//
	bool isBind();
	//////////////////////////////////这里是注入游戏远程使用的API接口///////////////////////////////
	//拦截API,会起一个循环检测的线程
	bool hookApi();

	bool DownCpu(DWORD sleeptime);

	bool TSMatchPicName(wchar_t *pic_name,wchar_t*ret_name);

	bool TSGetMachineCode(wchar_t* retcode);

	void mysleep(DWORD delaytime);

	void InitGdiNormal();

	bool TSEnableRealMouse(LONG X,LONG Y);

	DWORD TSRealKeyMousSleepTime(DWORD type);

	void TSMoveToEx(ULONG x,ULONG y,ULONG w,ULONG h,wchar_t *retstring);

public:
	DWORD ColorDataInfo[2000][2000];
	COLORREF** pWndBmpBuffer;
	short **pWndBmpStrxy;
	BYTE * pBuffer;
	DWORD winver;
	DWORD nPid;
	DWORD KeyDownsleep;//键盘单击延时
	DWORD NormalKeysleep;
	DWORD MouseClicksleep;//鼠标单击延时
	DWORD WindowsKeyMouseClicksleep;// Windows模式键盘\鼠标单击延时
	DWORD NormalClicksleep;
	HWND m_hwnd;            //被绑定的窗口的句柄
	HWND m_mousekeyhwnd;	//鼠标键盘后台窗口绑定
	HWND m_parenthwnd;		//父窗口句柄
	HANDLE m_eventMsg;
	int BindMode;
	MySetWindowHook mSetHook; 
	wchar_t m_SetPath[MAX_PATH];
	wchar_t m_Retstr[MAX_PATH*100];
	short LastMouseMoveX;
	short LastMouseMoveY;
	bool m_isBind;
	DWORD m_displayMode;
	DWORD m_keyMode;
	DWORD m_mouseMode;
	DWORD m_dwExStyle;
	bool g_IsSYSKEY;
	bool IsNpProtect;
	CMessageData *pData;
	bool Isdxkmprotect;  //dx键鼠不被恶意检测盾
	bool Isdxantiapi;//开启dx.public.anti.api保护
	pMyDictInfo MyDictCount[20]; //存储当前所有字库,默认最大字库个数为20个
	int nMaxStrHight[20]; //记录每个字库中,最高的字高.
	int nMyDictCountsize[20]; //记录每个字库的数量
	int NowUsingDictIndex;//记录当前正在使用字库的下标
	bool Isgraphicprotect;//防止图色检测盾
	bool isdxpic;
	BYTE CallCode[0x50];//201注入的代码
	bool IsTSEnableRealMouse;
	bool IsTSEnableRealKeypad;
	LONG mousedelay;
	LONG mousestep;
	DWORD GDI_dwExStyle;
private:
	
	int CallCodelen;
	LPVOID Alloaddr;//201注入的申请到的内存地址
	HKL  MSetUpIME;
	HKL  SysIME;
	int   Colors[MAX_PATH];//颜色
	BYTE       m_colorOffR[MAX_PATH]; // R的偏移量
	BYTE       m_colorOffG[MAX_PATH]; // G的偏移量
	BYTE       m_colorOffB[MAX_PATH]; // B的偏移量
	
public:
	DXBind(void);
	~DXBind(void);
};
