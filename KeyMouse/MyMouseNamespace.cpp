/*
本源码由TC简单软件科技有限公司开源,功能可以自由修改、发布、
长沙简单软件科技有限公司对于源码不做后期维护,,请大家在使用过程中遵循开源协议
*/
#include "stdafx.h"
#include "MyMouseNamespace.h"

/*===================================================================
//构造函数
====================================================================*/
MyMouseNamespace::MyMouseNamespace()
{
	delaytimes=30;
}
/*===================================================================
//析构函数
====================================================================*/
MyMouseNamespace::~MyMouseNamespace()
{
}

//鼠标移动到相对坐标
/*===================================================================
函数名称 :  mouseMvrelative
功能描述 :  鼠标移动的相对位置
====================================================================*/
bool  MyMouseNamespace::mouseMvrelative(DWORD x_pos,DWORD y_pos)
{
    //移动鼠标的坐标
	POINT pt;
	::GetCursorPos(&pt);
	::SetCursorPos(pt.x+x_pos,pt.y+y_pos);
    return true;
}

//鼠标移动到绝对坐标
/*===================================================================
函数名称 :  mouseMove
功能描述 :  鼠标移动的绝对位置

====================================================================*/
//鼠标移动到绝对位置
bool  MyMouseNamespace::mouseMove(DWORD x_pos,DWORD y_pos)
{
    //移动鼠标的坐标
	::SetCursorPos(x_pos,y_pos);
    return true;
}

//// 鼠标操作
//// LEFTCLICK   =0    //左键单击
////,RIGHTCLICK        //右键单击
////,MIDDLECLICK       //中键单击
////,LEFTDOWN          //左键按下
////,LEFTUP            //左键弹起
////,RIGHTDOWN         //右键按下
////,RIGHTUP           //右键弹起
////,MIDDLEDOWN        //中键按下
////,MIDDLEUP          //中键弹起
////,LEFTDOUBLECLICK   //左键双击
////,RIGHTDOUBLECLICK  //右键双击
//鼠标移动到绝对坐标
/*===================================================================
函数名称 :  mouseAction
功能描述 :  鼠标单击或双击模式以及具体的执行次数


返回值: void类型

参数:
名称        类型             描述
--------- -------------------------------------------
mousemode   MouseMode *    传进来鼠标单击或双击模式
nEcute      int            执行单击或者双击几次


修改历史 :
修改者   日期           描述
邓媛元   2010-07-17     
邓媛元   2010-07-20     修改注释
====================================================================*/
void MyMouseNamespace::mouseAction(MouseMode mousemode, int nEcute)
{
    //Sleep(1000);

    //鼠标状态选取 
    switch (mousemode) 
    {
        //鼠标左键单击
    case LEFTCLICK:
        for(int i = 0; i < nEcute; i++)
        {   //鼠标按下
            //mouse_event(MOUSEEVENTF_LEFTDOWN , 0, 0, 0, 0);
            //::Sleep(100);
            //mouse_event(MOUSEEVENTF_LEFTUP , 0, 0, 0, 0);

			INPUT  MyMouseClick[1];  
			////第一个动作：模拟鼠标左键按下  
			MyMouseClick[0].type   =   INPUT_MOUSE;  
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_LEFTDOWN;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  
			SendInput(1,MyMouseClick,sizeof(INPUT));
			Sleep(delaytimes);
			////第二个动作：模拟鼠标左键释放  
			MyMouseClick[0].type   =   INPUT_MOUSE;   
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_LEFTUP;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  ;
			////发送鼠标动作到消息序列。
			SendInput(1,MyMouseClick,sizeof(INPUT));
        }
        break;
        //鼠标右键单击
    case RIGHTCLICK: 
        for(int i = 0; i < nEcute; i++)
        {   //鼠标按下
            //mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
            //::Sleep(100);
            //mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);

			INPUT  MyMouseClick[1];  
			////第一个动作：模拟鼠标左键按下  
			MyMouseClick[0].type   =   INPUT_MOUSE;  
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_RIGHTDOWN;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  
			SendInput(1,MyMouseClick,sizeof(INPUT));
			Sleep(delaytimes);
			////第二个动作：模拟鼠标左键释放  
			MyMouseClick[0].type   =   INPUT_MOUSE;   
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_RIGHTUP;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  ;
			////发送鼠标动作到消息序列。
			SendInput(1,MyMouseClick,sizeof(INPUT));
        }
        break;
        //鼠标中键单击
    case MIDDLECLICK:
        for(int i = 0; i < nEcute; i++)
        {   //鼠标按下
            //mouse_event(MOUSEEVENTF_MIDDLEDOWN|MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);

			INPUT  MyMouseClick[1];  
			////第一个动作：模拟鼠标左键按下  
			MyMouseClick[0].type   =   INPUT_MOUSE;  
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_MIDDLEDOWN;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  
			SendInput(1,MyMouseClick,sizeof(INPUT));

			Sleep(delaytimes);
			////第二个动作：模拟鼠标左键释放  
			MyMouseClick[0].type   =   INPUT_MOUSE;   
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_MIDDLEUP;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  ;
			////发送鼠标动作到消息序列。
			SendInput(1,MyMouseClick,sizeof(INPUT));
        }
        break;
        //鼠标左键按下
    case LEFTDOWN:
		{
			//左键按下一次
			//mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); 	
			INPUT  MyMouseClick[1];  
			////第一个动作：模拟鼠标左键按下  
			MyMouseClick[0].type   =   INPUT_MOUSE;  
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_LEFTDOWN;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  
			SendInput(1,MyMouseClick,sizeof(INPUT));
		}
        break;
        //鼠标左键弹起
    case LEFTUP:
		{
			//mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			INPUT  MyMouseClick[1]; 
			////第二个动作：模拟鼠标左键释放  
			MyMouseClick[0].type   =   INPUT_MOUSE;   
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_LEFTUP;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  ;
			////发送鼠标动作到消息序列。
			SendInput(1,MyMouseClick,sizeof(INPUT));
		}
        break;
        //鼠标右键按下
    case RIGHTDOWN:
		{
        //mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0,0);
		    INPUT  MyMouseClick[1];  
			////第一个动作：模拟鼠标左键按下  
			MyMouseClick[0].type   =   INPUT_MOUSE;  
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_RIGHTDOWN;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  
			SendInput(1,MyMouseClick,sizeof(INPUT));
		}
        break;
        //鼠标右键盘弹起
    case RIGHTUP:
		{
        //mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0,0);
			INPUT  MyMouseClick[1];  
			MyMouseClick[0].type   =   INPUT_MOUSE;   
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_RIGHTUP;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  ;
			////发送鼠标动作到消息序列。
			SendInput(1,MyMouseClick,sizeof(INPUT));
		}
        break;
        //鼠标中键按下
    case MIDDLEDOWN:
		{
        //mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0,0);
			INPUT  MyMouseClick[1];  
			////第一个动作：模拟鼠标左键按下  
			MyMouseClick[0].type   =   INPUT_MOUSE;  
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_MIDDLEDOWN;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  
			SendInput(1,MyMouseClick,sizeof(INPUT));
		}
        break;
        //鼠标中键盘弹起
    case MIDDLEUP:
		{
        //mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0,0);
			INPUT  MyMouseClick[1]; 			
			MyMouseClick[0].type   =   INPUT_MOUSE;   
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_MIDDLEUP;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  ;
			////发送鼠标动作到消息序列。
			SendInput(1,MyMouseClick,sizeof(INPUT));
		}
        break;
        //鼠标左键双击
	case LEFTDOUBLECLICK:
		for (int i = 0; i < nEcute; i++)
		{   
			INPUT  MyMouseClick[1];  
			//第二个动作：模拟鼠标左键按下  
			MyMouseClick[0].type   =   INPUT_MOUSE;  
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_LEFTDOWN;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  
			Sleep(delaytimes);
			SendInput(1,MyMouseClick,sizeof(INPUT));
			//第三个动作：模拟鼠标左键释放  
			MyMouseClick[0].type   =   INPUT_MOUSE;   
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_LEFTUP;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  
			//发送鼠标动作到消息序列。
			SendInput(1,MyMouseClick,sizeof(INPUT));
			Sleep(delaytimes);
			//第二个动作：模拟鼠标左键按下  
			MyMouseClick[0].type   =   INPUT_MOUSE;  
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_LEFTDOWN;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  
			SendInput(1,MyMouseClick,sizeof(INPUT));
			Sleep(delaytimes);
			//第三个动作：模拟鼠标左键释放  
			MyMouseClick[0].type   =   INPUT_MOUSE;   
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_LEFTUP;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  
			//发送鼠标动作到消息序列。
			SendInput(1,MyMouseClick,sizeof(INPUT));
		}
		break;
        //鼠标右键双击
    case RIGHTDOUBLECLICK:
        for (int i = 0; i < nEcute; i++)
        {
			INPUT  MyMouseClick[1];  
			////第一个动作：模拟鼠标左键按下  
			MyMouseClick[0].type   =   INPUT_MOUSE;  
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_RIGHTDOWN;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  
			SendInput(1,MyMouseClick,sizeof(INPUT));
			Sleep(delaytimes);
			////第二个动作：模拟鼠标左键释放  
			MyMouseClick[0].type   =   INPUT_MOUSE;   
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_RIGHTUP;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  ;
			////发送鼠标动作到消息序列。
			SendInput(1,MyMouseClick,sizeof(INPUT));
			Sleep(delaytimes);
			////第一个动作：模拟鼠标左键按下  
			MyMouseClick[0].type   =   INPUT_MOUSE;  
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_RIGHTDOWN;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  
			SendInput(1,MyMouseClick,sizeof(INPUT));
			Sleep(delaytimes);
			////第二个动作：模拟鼠标左键释放  
			MyMouseClick[0].type   =   INPUT_MOUSE;   
			MyMouseClick[0].mi.dwFlags   =   MOUSEEVENTF_RIGHTUP;  
			MyMouseClick[0].mi.mouseData   =   NULL;  
			MyMouseClick[0].mi.dwExtraInfo   =   NULL;  
			MyMouseClick[0].mi.time   =   NULL;  ;
			////发送鼠标动作到消息序列。
			SendInput(1,MyMouseClick,sizeof(INPUT));

        }
        break;
    default:
        return;
    }
}

/*===================================================================
函数名称 :  GetmousePoint
功能描述 :  获取当前鼠标的位置坐标
====================================================================*/
bool  MyMouseNamespace::GetmousePoint(DWORD &x_pos,DWORD &y_pos)
{
    POINT pt;
    GetCursorPos(&pt);
	x_pos=pt.x;
	y_pos=pt.y;
    return true;
}
/*返回值: void类型

参数:
名称        类型             描述
--------- -------------------------------------------
direction   MouseWheel       接收返回值传出去
nMove       int              控制滚轮移动多少
====================================================================*/
void MyMouseNamespace::mouseMidleWheel(MouseWheel direction,int  nMove)
{
    if (direction == MouseWheel::WHEEL_DOWN) //滚轮向下
    {
        mouse_event(MOUSEEVENTF_WHEEL, 0,0,-nMove, 0);
    }
    else if (direction == MouseWheel::WHEEL_UP) //滚轮向上
    {
        mouse_event(MOUSEEVENTF_WHEEL, 0,0, nMove, 0);
    }
}

HBITMAP BitmapFromIcon(HICON hIcon)
{
    HDC dDC = CreateDC(_T("DISPLAY"),NULL,NULL,NULL);
    HDC hDC = CreateCompatibleDC(dDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hDC, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON));
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDC, hBitmap);
    DrawIcon(hDC, 0, 0, hIcon);
    hBitmap=(HBITMAP)SelectObject(hDC, hOldBitmap);
    DeleteDC(hDC);
    ::DeleteDC(dDC);

    return hBitmap;
}

////////////////////////获取鼠标形状特征码 ///////////////
bool MyMouseNamespace::getmouseshape(DWORD &dwshape)
{
	dwshape=0;
	CURSORINFO pci;
	pci.cbSize=sizeof(CURSORINFO);
	pci.flags=0;
	pci.hCursor=0;
	if(!::GetCursorInfo(&pci))
		return false;
	
	HDC dDC = CreateDC(_T("DISPLAY"),NULL,NULL,NULL);
	HDC hDC = CreateCompatibleDC(dDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(dDC, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON));
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDC, hBitmap);

	DrawIcon(hDC,0,0,pci.hCursor);
	hBitmap=(HBITMAP)SelectObject(hDC, hOldBitmap);

	DWORD           dwPaletteSize=0,dwBmBitsSize;
	BITMAP          Bitmap;              
	BITMAPINFOHEADER   bi;
	//位图信息头结构 
	LPBITMAPINFOHEADER lpbi; 
	HANDLE       hDib;
	HPALETTE     hOldPal=NULL;

	//设置位图信息头结构
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	WORD    wBitCount = Bitmap.bmBitsPixel;
	dwBmBitsSize = ((Bitmap.bmWidth*wBitCount+31)& ~31)/8*Bitmap.bmHeight;
	BYTE* bmpDate = new BYTE[dwBmBitsSize];

	bi.biSize            = sizeof(BITMAPINFOHEADER);
	bi.biWidth           = Bitmap.bmWidth;
	bi.biHeight          = Bitmap.bmHeight;
	bi.biPlanes          = 1;
	bi.biBitCount         = wBitCount;
	bi.biCompression      = BI_RGB;
	bi.biSizeImage        = dwBmBitsSize;
	bi.biXPelsPerMeter     = 0;
	bi.biYPelsPerMeter     = 0;
	bi.biClrUsed         = 0;
	bi.biClrImportant      = 0;

	hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	GetDIBits(hDC,hBitmap,0,(UINT)Bitmap.bmHeight,bmpDate, (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	DWORD num=0;
	int height=Bitmap.bmHeight;
	int width =Bitmap.bmWidth;
	
	BYTE  b0 = bmpDate[0];
	BYTE  g0 = bmpDate[1];
	BYTE  r0 = bmpDate[2];

	// 32位图进行解析
	if(Bitmap.bmBitsPixel==32)
	{
			for(int i=0;i<height;i++)
			{
				for(int j=0;j<width;j++)
				{
					UCHAR b=bmpDate[(i*width+j)*4];
					UCHAR g=bmpDate[(i*width+j)*4+1];
					UCHAR r=bmpDate[(i*width+j)*4+2];
					if( b == b0 && g == g0 && r == r0 )
					{
						continue;
					}
					else
					{
						num++;
					}
				}
			}
	}


	////////////////////////////////////////////////////////////////////////
	DWORD helpnum=num*RGB(255,255,0);
	delete [] bmpDate;
	:: GlobalFree(hDib);
	::DeleteDC(hDC);
	::DeleteDC(dDC);
	::DeleteObject(hBitmap);
	dwshape=helpnum;
	return true;
}

#define MOUSE_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define MOUSE_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

bool MyMouseNamespace::leftClick()
{
	mouseAction(LEFTCLICK);
	return true;
}
bool MyMouseNamespace::leftDoubleClick()
{
	mouseAction(LEFTDOUBLECLICK);
	return true;
}
bool MyMouseNamespace::leftDown()
{
	mouseAction(LEFTDOWN);
	return true;
}
bool MyMouseNamespace::leftUp()
{
	mouseAction(LEFTUP);
	return true;
}

bool MyMouseNamespace::rightClick()
{
	mouseAction(RIGHTCLICK);
	return true;
}
bool MyMouseNamespace::rightDoubleClick()
{
	mouseAction(RIGHTDOUBLECLICK);
	return true;
}
bool MyMouseNamespace::rightDown()
{
	mouseAction(RIGHTDOWN);
	return true;
}
bool MyMouseNamespace::rightUp()
{
	mouseAction(RIGHTUP);
	return true;
}

bool MyMouseNamespace::middleClick()
{
	mouseAction(MIDDLECLICK);
	return true;
}
bool MyMouseNamespace::middleDown()
{
	mouseAction(MIDDLEDOWN);
	return true;
}
bool MyMouseNamespace::middleUp()
{
	mouseAction(MIDDLEUP);
	return true;
}
// 向下滚动
bool MyMouseNamespace::mouseSheetDown()
{
	mouseMidleWheel(WHEEL_DOWN);
	return true;
}
// 向上滚动
bool MyMouseNamespace::mouseSheetUp()
{
	mouseMidleWheel(WHEEL_UP);
	return true;
}

