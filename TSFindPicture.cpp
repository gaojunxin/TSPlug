/*
本源码由TC简单软件科技有限公司开源,功能可以自由修改、发布、
长沙简单软件科技有限公司对于源码不做后期维护,,请大家在使用过程中遵循开源协议
*/
#include "stdafx.h"
#include "TSRuntime.h"
#include "TSFindPicture.h"
#include "D3dx9tex.h"
#include "DXBind.h"
#include   <GdiPlus.h> 
using   namespace   Gdiplus; 
#pragma comment(lib, "gdiplus.lib")

extern HWND g_currentHwnd;

MyFindPictureClass::MyFindPictureClass(void)
{
	IsLastFindStr=false;
	m_displayMode=0;
	m_hwnd=NULL;
	hDXBitmap=NULL;
	pLoadBmpBuffer=NULL;
	pWndBmpBuffer=NULL;
	pBuffer=NULL;
	pbuffer=NULL;
	loadWidth=0;
	loadHeight=0;
	wndWidth=0;
	wndHeight=0;
	nbuffersize=0;
	m_helpXpoint = 0;
	m_helpYpoint = 0;
	m_parenthwnd=0;
	memset(SetPath,0,MAX_PATH);

	strColor=0;
	//strColoroff=0;
	usingindex=0;
	for(int i=0;i<MAX_PATH*10;i++)
		memset(addrxy[i],0,3);
	memset(strColors,0,MAX_PATH);
	memset(strColoroff,0,MAX_PATH);
	memset(m_colorOffR,0,MAX_PATH);
	memset(m_colorOffG,0,MAX_PATH);
	memset(m_colorOffB,0,MAX_PATH);
	//memset(pWndBmpBuffer,0,sizeof(pWndBmpBuffer));

	//memset(SetPicPwdString,0,MAX_PATH);
	isprocessColor=false;//默认不是找色或者取色
	dictindex=0;
}

MyFindPictureClass::~MyFindPictureClass(void)
{

}

int   MyFindPictureClass::GetEncoderClsid(const   WCHAR*   format,   CLSID*   pClsid) 
{
	// Initialize GDI+.(初始化GDI,否则调用GDI函数会导致异常)
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	// Initialize GDI+.(初始化GDI,否则调用GDI函数会导致异常)

	UINT     num   =   0;                     //   number   of   image   encoders  
	UINT     size   =   0;                   //   size   of   the   image   encoder   array   in   bytes  

	ImageCodecInfo*   pImageCodecInfo   =   NULL;  

	GetImageEncodersSize(&num,   &size);  
	if(size   ==   0)
	{
		return   -1;     //   Failure  
	}

	pImageCodecInfo   =   (ImageCodecInfo*)(malloc(size));  
	if(pImageCodecInfo   ==   NULL)  
		return   -1;     //   Failure  

	GetImageEncoders(num,   size,   pImageCodecInfo);  

	for(UINT   j   =   0;   j   <   num;   ++j)  
	{
		if(   wcscmp(pImageCodecInfo[j].MimeType,   format)   ==   0   )  
		{  
			*pClsid   =   pImageCodecInfo[j].Clsid;  
			free(pImageCodecInfo);  
			return   j;     //   Success  
		}          
	}  
	free(pImageCodecInfo);  
	return   -1;     //   Failure 
} 

bool   MyFindPictureClass::BMP2PNG(wchar_t*   strBMPFile,wchar_t*   strPNGFile) 
{  
	CLSID     encoderClsid;  
	WCHAR     strGuid[39];  
	if(GetEncoderClsid(L"image/png",   &encoderClsid)   >=   0)
		StringFromGUID2(encoderClsid,   strGuid,   39);  

	Image   image(strBMPFile);  
	image.Save(strPNGFile,&encoderClsid,NULL);

	return true;
}

bool   MyFindPictureClass::BMP2JPG(wchar_t*   strBMPFile,wchar_t*   strJPGFile) 
{  
	CLSID     encoderClsid;  
	WCHAR     strGuid[39];  
	if(GetEncoderClsid(L"image/jpeg",   &encoderClsid)   >=   0)
		StringFromGUID2(encoderClsid,   strGuid,   39);  

	Image   image(strBMPFile);  
	image.Save(strJPGFile,&encoderClsid,NULL);

	//关闭GDI
	//GdiplusShutdown(gdiplusToken);
	return true;
} 

bool MyFindPictureClass::loadBitmap(wchar_t* path)
{
	//std::string rstr;
	BYTE *rstr=NULL;
	int rstrlen=0;
	BITMAPFILEHEADER fileheader={0};

	HANDLE file_Handle = CreateFile(path,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);
	if(file_Handle==INVALID_HANDLE_VALUE)
	{
		int err=::GetLastError();
		if(TSRuntime::IsShowErrorMsg)
		{
			char erro[MAX_PATH]={0};
			USES_CONVERSION;
			sprintf(erro,"加载:%s,位图失败",W2A(path));
			::MessageBoxA(NULL,erro,"TS",0);
		}
		CloseHandle(file_Handle);
		return false;
	}
	DWORD dword=0,ReadFileSize=0;
	DWORD fileszie=::GetFileSize(file_Handle,NULL);
	rstr=new BYTE[fileszie+1];
	memset(rstr,0,sizeof(BYTE)*fileszie);
	{
		::ReadFile(file_Handle,rstr,fileszie,&ReadFileSize,NULL);
	}
	CloseHandle(file_Handle);

	BITMAPINFOHEADER head;
	DWORD headSize=sizeof(BITMAPINFOHEADER);
	memcpy(&head,&rstr[sizeof(BITMAPFILEHEADER)],sizeof(BITMAPINFOHEADER));
	loadWidth = head.biWidth;
	loadHeight = head.biHeight;
	WORD biBitCount = head.biBitCount;
	if(biBitCount != 24)
	{
		if(TSRuntime::IsShowErrorMsg)
			::MessageBoxA(NULL,"请选择24位位图！","TS",0);
		return false;
	}

	int totalSize = (loadWidth *biBitCount/8+3)/4*4*loadHeight;
	//BYTE *pBmpBuf = new BYTE[totalSize];
	//memset(pBmpBuf,0,sizeof(BYTE)*totalSize);
	size_t size = 0;
	size=rstrlen-headSize-sizeof(BITMAPFILEHEADER);
	BYTE *pBmpBuf =&rstr[headSize+sizeof(BITMAPFILEHEADER)];
	//memcpy(pBmpBuf,&rstr[headSize+sizeof(BITMAPFILEHEADER)],size);

	pLoadBmpBuffer = new COLORREF*[loadHeight];     ///// 二维数组 用来存储图像的颜色值
	for(int i=0;i<loadHeight;i++)
	{
		pLoadBmpBuffer[i]=new COLORREF[loadWidth];
	}
	COLORREF helpcolor=0;
	int pitch=loadWidth%4;
	//HDC hDC = ::GetWindowDC(::GetDesktopWindow());
	for(int i=0;i<loadHeight;i++)
	{
		int realPitch=i*pitch;
		for(int j=0;j<loadWidth;j++)
		{
			UCHAR b=pBmpBuf[(i*loadWidth+j)*3+realPitch];
			UCHAR g=pBmpBuf[(i*loadWidth+j)*3+1+realPitch];
			UCHAR r=pBmpBuf[(i*loadWidth+j)*3+2+realPitch];
			helpcolor=RGB(r,g,b);
			pLoadBmpBuffer[loadHeight-i-1][j]=helpcolor;	
		}
	}
	delete [] rstr;
	return true;
}

//
//bool MyFindPictureClass::loadBitmap(wchar_t* path)
//	{
//	//TSRuntime::add_log( "loadBitmap");
//	///////////////////////////加载的位图 //////////////////////////
//	HBITMAP hbmp = (HBITMAP)LoadImage(NULL,path,IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
//	if(hbmp==NULL)
//		{
//			//::MessageBox(NULL,L"加载位图失败",path,0);
//			return false;
//		}
//
//	BITMAP bitmap;
//	GetObject(hbmp, sizeof(BITMAP), &bitmap);
//
//	BITMAPINFO info;
//	HDC dc;
//	dc=::CreateDC(L"DISPLAY",NULL,NULL,NULL);
//
//	// 24位图的BITMAPINFO
//	BITMAPINFO *pBITMAPINFO = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER));
//	memset(pBITMAPINFO, 0, sizeof(BITMAPINFOHEADER));
//	BITMAPINFOHEADER *pInfo_Header = (BITMAPINFOHEADER *)pBITMAPINFO;
//	pInfo_Header->biSize = sizeof(BITMAPINFOHEADER);   
//	pInfo_Header->biWidth =bitmap.bmWidth;  
//	pInfo_Header->biHeight = bitmap.bmHeight;  
//	pInfo_Header->biPlanes = 1;  
//	pInfo_Header->biBitCount = 24;  
//	pInfo_Header->biCompression = BI_RGB;
//
//	long width = bitmap.bmWidth;
//	long height = bitmap.bmHeight;
//	info.bmiHeader.biBitCount=24;
//
//	loadWidth = width;
//	loadHeight = height;
//
//	DWORD bufSize = ( width * 3 + 3) / 4 * 4 * height;
//	BYTE *buffer=new BYTE[bufSize];
//	memset(buffer,0,sizeof(BYTE)*bufSize);
//	if(!GetDIBits(dc, hbmp, 0, bitmap.bmHeight, buffer, pBITMAPINFO, DIB_RGB_COLORS))
//		{
//		if(TSRuntime::IsShowErrorMsg)
//			::MessageBox(NULL,L"加载位图失败",L"TC",0);
//		return false;
//		}
//	pLoadBmpBuffer = new COLORREF*[height];     ///// 二维数组 用来存储图像的颜色值
//	for(int i=0;i<height;i++)
//		{
//		pLoadBmpBuffer[i]=new COLORREF[width];
//		}
//	COLORREF helpcolor=0;
//	int pitch=width%4;
//	HDC hDC = ::GetWindowDC(::GetDesktopWindow());
//	for(int i=0;i<height;i++)
//		{
//		int realPitch=i*pitch;
//		for(int j=0;j<width;j++)
//			{
//			UCHAR b=buffer[(i*width+j)*3+realPitch];
//			UCHAR g=buffer[(i*width+j)*3+1+realPitch];
//			UCHAR r=buffer[(i*width+j)*3+2+realPitch];
//			helpcolor=RGB(r,g,b);
//			SetPixel(hDC,j,i,helpcolor);
//			pLoadBmpBuffer[height-i-1][j]=helpcolor;	
//			}
//		}
//	///////////////////////////获得加载位图 颜色成功 //////////////////////////
//	delete [] buffer;
//	::DeleteDC(dc);
//	::free(pBITMAPINFO);
//	::DeleteObject(hbmp);
//	return true;
//	}

bool MyFindPictureClass::SaveGDIBitmap(HWND hWnd,RECT rect,wchar_t *savepath)
{
	HDC hDC;
	//hDC=::GetWindowDC(hWnd);					//定义绘图用的设备环境
	hDC=GetDCEx(hWnd,NULL,DCX_PARENTCLIP );
	HDC hMemDC;					//内存缓冲设备环境
	HBITMAP hbmMem,hbmOld;		//内存缓冲设备环境中的位图
	RECT rc=rect;
	//判断边境值
	RECT clientrc;
	::GetClientRect(hWnd,&clientrc);

	int xc =0;
	int cx =0;
	int cy =0;

	if(rc.bottom>clientrc.bottom)
		rc.bottom=clientrc.bottom;

	if(rc.right>clientrc.right)
		rc.right=clientrc.right;


	//TSRuntime::add_log( "clientrc.right:%d,clientrc.bottom:%d,rect.left:%d,rect.top:%d,rect.right:%d,rect.bottom:%d",clientrc.right, clientrc.bottom,rc.left,rc.top,rc.right,rc.bottom);
	// 24位图的BITMAPINFO
	BITMAPINFO *pBITMAPINFO = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER));
	memset(pBITMAPINFO, 0, sizeof(BITMAPINFOHEADER));
	BITMAPINFOHEADER *pInfo_Header = (BITMAPINFOHEADER *)pBITMAPINFO;
	pInfo_Header->biSize = sizeof(BITMAPINFOHEADER);   
	pInfo_Header->biWidth = rc.right - rc.left;   
	pInfo_Header->biHeight = (rc.bottom - rc.top);   
	pInfo_Header->biPlanes = 1;   
	pInfo_Header->biBitCount = 24;   
	pInfo_Header->biCompression = BI_RGB;

	hMemDC=CreateCompatibleDC(hDC);	//创建内存兼容设备环境
	//创建内存兼容位图
	hbmMem=CreateCompatibleBitmap(hDC,pInfo_Header->biWidth,pInfo_Header->biHeight);

	hbmOld=(HBITMAP)SelectObject(hMemDC,hbmMem);

	if((m_displayMode&DISPLAY_GDI2)==DISPLAY_GDI2)
	{
		RECT winret;
		::GetWindowRect(hWnd,&winret);
		POINT point;
		point.x=0;
		point.y=0;
		::ClientToScreen(hWnd,&point);
		cx=point.x-winret.left;
		cy=point.y-winret.top;
		HDC T_hMemDC=CreateCompatibleDC(hDC);	//创建内存兼容设备环境
		//创建内存兼容位图
		HBITMAP T_hbmMem=CreateCompatibleBitmap(hDC,pInfo_Header->biWidth+cx+rc.left,pInfo_Header->biHeight+cy+rc.top);
		HBITMAP T_hbmOld=(HBITMAP)SelectObject(T_hMemDC,T_hbmMem);
		::UpdateWindow(hWnd);
		::RedrawWindow(hWnd,NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN|RDW_FRAME);
		::PrintWindow(hWnd,T_hMemDC,0);
		BitBlt(hMemDC,0,0,pInfo_Header->biWidth,pInfo_Header->biHeight,T_hMemDC,cx+rc.left,xc+cy+rc.top,CAPTUREBLT|SRCCOPY);
		DeleteDC(T_hMemDC);
		DeleteObject(T_hbmMem);
		DeleteObject(T_hbmOld);
	}
	else
	{
		//将内存设备环境中的内容绘制到物理设备环境   hDC
		BitBlt(hMemDC,0,0,pInfo_Header->biWidth,pInfo_Header->biHeight,hDC,cx+rc.left,xc+cy+rc.top,CAPTUREBLT|SRCCOPY);
	}

	HBITMAP hBitmap=(HBITMAP)SelectObject(hMemDC,hbmOld);

	// 获得数据buf
	DWORD bufSize=(pInfo_Header->biWidth * 3 + 3) / 4 * 4 * pInfo_Header->biHeight;
	BYTE * pBuffer = new BYTE[bufSize];

	int aHeight=pInfo_Header->biHeight;

	if(::GetDIBits(hMemDC, hBitmap, 0, aHeight, pBuffer,pBITMAPINFO, DIB_RGB_COLORS) == 0)
	{
		if(TSRuntime::IsShowErrorMsg)
			::MessageBox(NULL,L"加载位图失败",L"TS",0);
		return false;
	}

	bool bret=SaveBitmapToFile(hBitmap,savepath);


	ReleaseDC(hWnd,hDC);
	//释放资源
	DeleteObject(hbmMem);
	DeleteObject(hbmOld);
	DeleteDC(hMemDC);
	free(pBITMAPINFO);
	::DeleteObject(hBitmap);
	delete [] pBuffer;
	return bret;
}
bool SaveDXBitmapToFile(PVOID pBuffer,DWORD nbuffersize,LPCWSTR lpFileName)
{
	//////////32位BMP转换24位BMP  2012-8-14
	BYTE *pTemp =(BYTE *) pBuffer;    //已知  32位 数据
	BYTE *pData24,*tpData24;  
	DWORD  dwSize32=nbuffersize;   //已知，32位大小		
	DWORD  dwSize24; 
	dwSize24 = (dwSize32*3)/4;	  
	pData24 = new BYTE[dwSize24];	
	tpData24 =pData24; 
	for(int index=0;index<dwSize32/4;index++)   //像素总个数
	{		  
		unsigned char r = *(pTemp++);
		unsigned char g = *(pTemp++);

		unsigned char b= *(pTemp++);

		(pTemp++);    //去掉alpha

		*(pData24++) = r;

		*(pData24++) = g;

		*(pData24++) = b;
	}
	return true;
}

bool MyFindPictureClass::CopyScreenToBitmap(LPRECT lpRect,wchar_t *savepath)
{
	HDC hScrDC, hMemDC;      
	// 屏幕和内存设备描述表
	HBITMAP hBitmap,hOldBitmap;   
	// 位图句柄
	int       nX, nY, nX2, nY2;      
	// 选定区域坐标
	int       nWidth, nHeight;      
	// 位图宽度和高度
	int       xScrn, yScrn;         

	// 屏幕分辨率

	// 确保选定区域不为空矩形
	if (IsRectEmpty(lpRect))
		return NULL;

	//为屏幕创建设备描述表
	hScrDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	//为屏幕设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);
	// 获得选定区域坐标
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	//nX = 10;
	//nY = 10;
	//nX2 = 500;
	//nY2 = 600;

	// 获得屏幕分辨率
	xScrn = GetDeviceCaps(hScrDC, HORZRES);
	yScrn = GetDeviceCaps(hScrDC, VERTRES);
	//确保选定区域是可见的
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	if (nX2 > xScrn)
		nX2 = xScrn;
	if (nY2 > yScrn)
		nY2 = yScrn;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// 创建一个与屏幕设备描述表兼容的位图
	hBitmap=CreateCompatibleBitmap(hScrDC,nWidth,nHeight);
	// 把新位图选到内存设备描述表中
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);
	// 把屏幕设备描述表拷贝到内存设备描述表中
	BitBlt(hMemDC,0,0, nWidth,nHeight,hScrDC, nX, nY, CAPTUREBLT|SRCCOPY);
	//得到屏幕位图的句柄
	hBitmap=(HBITMAP)SelectObject(hMemDC,hOldBitmap);

	bool bret=SaveBitmapToFile(hBitmap,savepath);

	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	DeleteObject(hOldBitmap);
	DeleteObject(hBitmap);
	// 返回位图句柄
	return bret;
}

//LPSTR lpFileName
bool MyFindPictureClass::SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName)
{
	HDC     hDC;         
	//设备描述表
	int     iBits;      
	//当前显示分辨率下每个像素所占字节数
	WORD    wBitCount;
	//位图中每个像素所占字节数
	//定义调色板大小， 位图中像素字节大小 ，  位图文件大小 ， 写入文件字节数
	DWORD           dwPaletteSize=0,dwBmBitsSize,dwDIBSize, dwWritten;
	BITMAP          Bitmap;
	//位图属性结构
	BITMAPFILEHEADER   bmfHdr;        
	//位图文件头结构
	BITMAPINFOHEADER   bi;            
	//位图信息头结构 
	LPBITMAPINFOHEADER lpbi;          
	//指向位图信息头结构
	HANDLE          fh, hDib, hPal;
	HPALETTE     hOldPal=NULL;
	//定义文件，分配内存句柄，调色板句柄

	//计算位图文件每个像素所占字节数
	hDC = CreateDC(L"DISPLAY",NULL,NULL,NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else if (iBits <= 24)
		wBitCount = 24;
	else
		//wBitCount = 32;
		wBitCount = 24;
	//计算调色板大小
	if (wBitCount <= 8)
		dwPaletteSize=(1<<wBitCount)*sizeof(RGBQUAD);

	//设置位图信息头结构
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize            = sizeof(BITMAPINFOHEADER);
	bi.biWidth           = Bitmap.bmWidth;
	bi.biHeight          = Bitmap.bmHeight;
	bi.biPlanes          = 1;
	bi.biBitCount         = wBitCount;
	bi.biCompression      = BI_RGB;
	bi.biSizeImage         = 0;
	bi.biXPelsPerMeter     = 0;
	bi.biYPelsPerMeter     = 0;
	bi.biClrUsed           = 0;
	bi.biClrImportant      = 0;

	dwBmBitsSize = ((Bitmap.bmWidth*wBitCount+31)/32)*4*Bitmap.bmHeight;
	//为位图内容分配内存

	/*xxxxxxxx计算位图大小分解一下(解释一下上面的语句)xxxxxxxxxxxxxxxxxxxx 
	//每个扫描行所占的字节数应该为4的整数倍，具体算法为:
	int biWidth = (Bitmap.bmWidth*wBitCount) / 32;
	if((Bitmap.bmWidth*wBitCount) % 32)
	biWidth++; //不是整数倍的加1
	biWidth *= 4;//到这里，计算得到的为每个扫描行的字节数。
	dwBmBitsSize = biWidth * Bitmap.bmHeight;//得到大小
	xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

	//TSRuntime::add_log("wBitCount:%d",wBitCount);

	hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	// 处理调色板   
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal=SelectPalette(hDC,(HPALETTE)hPal,FALSE);
		RealizePalette(hDC);
	}
	// 获取该调色板下新的像素值
	GetDIBits(hDC,hBitmap,0,(UINT)Bitmap.bmHeight,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER)+dwPaletteSize, (BITMAPINFO *)lpbi,DIB_RGB_COLORS);
	//恢复调色板   
	if (hOldPal)
	{
		SelectPalette(hDC, hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}
	//创建位图文件    
	fh=CreateFile(lpFileName, GENERIC_WRITE,0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh==INVALID_HANDLE_VALUE)
		return FALSE;
	// 设置位图文件头
	bmfHdr.bfType = 0x4D42;  // "BM"
	dwDIBSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize;  
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;

	bool bw1=false;
	bool bw2=false;

	// 写入位图文件头
	bw1=WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	// 写入位图文件其余内容
	bw2=WriteFile(fh, (LPSTR)lpbi, sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize , &dwWritten, NULL); 
	//清除   
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	//return TRUE;
	if(bw1&&bw2==true)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool MyFindPictureClass::getGDIBitmap(HWND hWnd,RECT rc,int typemode)
{
	//TSRuntime::add_log( "getGDIBitmap:hWnd:%d",hWnd);
	//HDC hDC=::GetWindowDC(hWnd);					//定义绘图用的设备环境
	m_hwnd=hWnd;
	HDC hDC;
	if(hWnd!=GetDesktopWindow())
		//hDC=::GetWindowDC(hWnd);					//定义绘图用的设备环境
		hDC=GetDCEx(hWnd,NULL,DCX_PARENTCLIP );
	else
		hDC=CreateDC(L"DISPLAY", NULL, NULL, NULL);
	HDC hMemDC;					//内存缓冲设备环境
	HBITMAP hbmMem,hbmOld;		//内存缓冲设备环境中的位图

	int xc =0;
	int cx =0;
	int cy =0;

	wndWidth=m_Right-m_Left;
	wndHeight=m_bottom-m_Top;

	if(wndWidth>=2000||wndHeight>=2000)//大于边界值就返回错误,以防坐标越界
		return false;

	BITMAPINFO pBITMAPINFO;
	pBITMAPINFO.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);   
	pBITMAPINFO.bmiHeader.biWidth = wndWidth;   
	pBITMAPINFO.bmiHeader.biHeight = wndHeight;   
	pBITMAPINFO.bmiHeader.biPlanes = 1;   
	pBITMAPINFO.bmiHeader.biBitCount = 24;   
	pBITMAPINFO.bmiHeader.biCompression = BI_RGB;

	hMemDC=CreateCompatibleDC(hDC);	//创建内存兼容设备环境
	//创建内存兼容位图
	hbmMem=CreateCompatibleBitmap(hDC,wndWidth,wndHeight);
	hbmOld=(HBITMAP)SelectObject(hMemDC,hbmMem);

	if((m_displayMode&DISPLAY_GDI2)==DISPLAY_GDI2)
	{
		RECT winret;
		::GetWindowRect(hWnd,&winret);
		POINT point;
		point.x=0;
		point.y=0;
		::ClientToScreen(hWnd,&point);
		cx=point.x-winret.left;
		cy=point.y-winret.top;
		HDC T_hMemDC=CreateCompatibleDC(hDC);	//创建内存兼容设备环境
		//创建内存兼容位图
		HBITMAP T_hbmMem=CreateCompatibleBitmap(hDC,wndWidth+cx+rc.left,wndHeight+cy+rc.top);
		HBITMAP T_hbmOld=(HBITMAP)SelectObject(T_hMemDC,T_hbmMem);
		::UpdateWindow(hWnd);
		::RedrawWindow(hWnd,NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN|RDW_FRAME);
		::PrintWindow(hWnd,T_hMemDC,0);
		BitBlt(hMemDC,0,0,wndWidth,wndHeight,T_hMemDC,cx+rc.left,xc+cy+rc.top,CAPTUREBLT|SRCCOPY);
		DeleteDC(T_hMemDC);
		DeleteObject(T_hbmMem);
		DeleteObject(T_hbmOld);
	}
	else
	{
		//将内存设备环境中的内容绘制到物理设备环境   hDC
		BitBlt(hMemDC,0,0,wndWidth,wndHeight,hDC,cx+m_Left,xc+cy+m_Top,CAPTUREBLT|SRCCOPY);
	}
	HBITMAP hBitmap=(HBITMAP)SelectObject(hMemDC,hbmOld);

	// 获得数据buf
	//DWORD bufSize=(pInfo_Header->biWidth * 3 + 3) / 4 * 4 * pInfo_Header->biHeight;
	//BYTE * pBuffer = new BYTE[bufSize];
	int aHeight=wndHeight;

	if(::GetDIBits(hMemDC, hBitmap, 0, aHeight, pBuffer,&pBITMAPINFO, DIB_RGB_COLORS) == 0)
	{
		if(TSRuntime::IsShowErrorMsg)
			::MessageBox(NULL,L"加载位图失败",L"TS",0);
		return false;
	}

	//HDC dd = ::GetWindowDC(::GetDesktopWindow());

	COLORREF helpcolor=0;
	int pitch=wndWidth%4;
	if(typemode==1)//找字
	{
		for(int i=0;i<wndHeight;i++)
		{
			int realPitch=i*pitch;
			for(int j=0;j<wndWidth;j++)
			{
				UCHAR b=pBuffer[(i*wndWidth+j)*3+realPitch];
				UCHAR g=pBuffer[(i*wndWidth+j)*3+1+realPitch];
				UCHAR r=pBuffer[(i*wndWidth+j)*3+2+realPitch];
				//helpcolor=RGB(r,g,b);
				bool dwfind=false;
				for(int n=0;n<ncolors;n++)
				{
					BYTE colorR = GetRValue(strColors[n]);
					BYTE colorG = GetGValue(strColors[n]);
					BYTE colorB = GetBValue(strColors[n]);

					//处理色偏,把图二值化
					////处理色偏
					if(abs(colorR-r)<=m_colorOffR[n]&&abs(colorG-g)<=m_colorOffG[n]&&abs(colorB-b)<=m_colorOffB[n])
					{
						helpcolor=RGB(255,255,255);
						dwfind=true;
					}
					else
					{
						helpcolor=RGB(0,0,0);
					}
					//SetPixel(dd,j,(wndHeight-i-1),helpcolor);
					pWndBmpBuffer[wndHeight-i-1][j]=helpcolor;
					if(dwfind)
						break;
				}
			}
		}
	}
	else //找图
	{
		for(int i=0;i<wndHeight;i++)
		{
			int realPitch=i*pitch;
			for(int j=0;j<wndWidth;j++)
			{
				UCHAR b=pBuffer[(i*wndWidth+j)*3+realPitch];
				UCHAR g=pBuffer[(i*wndWidth+j)*3+1+realPitch];
				UCHAR r=pBuffer[(i*wndWidth+j)*3+2+realPitch];
				helpcolor=RGB(r,g,b);

				pWndBmpBuffer[wndHeight-i-1+m_Top][j+m_Left]=helpcolor;
				//::SetPixel(dd,j,i,helpcolor);
			}
		}
	}

	if(hWnd==GetDesktopWindow())
		DeleteDC(hDC);
	else
		ReleaseDC(hWnd,hDC);

	//释放资源
	DeleteObject(hbmMem);
	DeleteObject(hbmOld);
	DeleteDC(hMemDC);
	DeleteDC(hDC);
	//free(pBITMAPINFO);
	::DeleteObject(hBitmap);
	//delete [] pBuffer;
	return true;
}


bool MyFindPictureClass::getDXBitmap(PVOID pBmp,DWORD (*ColorDataInfo)[2000],int typemode)
{
	if(wndWidth>=2000||wndHeight>=2000)//大于边界值就返回错误,以防坐标越界
		return false;
	//HDC dd = ::GetWindowDC(::GetDesktopWindow());
	//TSRuntime::add_log( "wndHeight:%d wndWidth:%d,m_left:%d,m_top:%d,m_Right:%d,m_bottom:%d",wndHeight,wndWidth,m_Left,m_Top,m_Right,m_bottom);
	COLORREF helpcolor=0;
	//int pitch=wndWidth%4;
	if(typemode==1)//找字
	{
		for(int i=0;i<wndHeight;i++)
		{
			//int realPitch=i*pitch;
			for(int j=0;j<wndWidth;j++)
			{
				UCHAR r=GetRValue(ColorDataInfo[i][j]);
				UCHAR g=GetGValue(ColorDataInfo[i][j]);
				UCHAR b=GetBValue(ColorDataInfo[i][j]);
				bool dwfind=false;
				for(int n=0;n<ncolors;n++)
				{
					BYTE colorR = GetRValue(strColors[n]);
					BYTE colorG = GetGValue(strColors[n]);
					BYTE colorB = GetBValue(strColors[n]);

					//处理色偏,把图二值化
					if(abs(colorR-r)<=m_colorOffR[n]&&abs(colorG-g)<=m_colorOffG[n]&&abs(colorB-b)<=m_colorOffB[n])
					{
						helpcolor=RGB(255,255,255);
						dwfind=true;
					}
					else
					{
						helpcolor=RGB(0,0,0);
					}
					//COLORREF helpcolor1=RGB(r,g,b);
					//SetPixel(dd,j,i,helpcolor1);
					pWndBmpBuffer[i][j]=helpcolor;
					//pWndBmpBuffer[wndHeight-i-1+m_Top][j+m_Left]=helpcolor;
					if(dwfind)
						break;
				}


			}
		}
	}
	else //找图
	{
		for(int i=0;i<wndHeight;i++)
		{
			for(int j=0;j<wndWidth;j++)
			{
				//helpcolor=(COLORREF)ColorDataInfo[i][j];
				//SetPixel(dd,j,i,helpcolor);
				pWndBmpBuffer[m_Top+i][j+m_Left]=(COLORREF)ColorDataInfo[i][j];
			}
		}
	}

	return true;
	///////////////////////////获取窗口位图的 颜色成功 //////////////////////////
}
//
//
//
int MyFindPictureClass::findPicture(PVOID pbuffer,HWND hwnd,int left, int top, int right, int bottom, 
									wchar_t* path,  wchar_t* color, double simi, int dir,
									long &xpos, long &ypos,DWORD (*ColorDataInfo)[2000],wchar_t *retstring,int type)
{
	int index = -1;
	int retIndex=-1;
	bool isFind=false;

	if(hwnd!=0)
	{
		if(::IsIconic(hwnd))
		{
			//::MessageBox(0,L"超过边界值,GDI模式不支持最小化找图",L"TS",0);
			return -1;
		}
	}

	////////处理颜色和色偏
	WCHAR colorR[4]={0};//字的颜色
	WCHAR colorG[4]={0};//字的颜色
	WCHAR colorB[4]={0};//字的颜色
	int count=0;
	int colorL=0;
	while(color[colorL]!=L'\0')
	{
		if(count>6)	//判断是否是非法颜色值
			break;
		if(count<2)
			colorR[count]=color[colorL++];
		else if(count>1&&count<4)
			colorG[count-2]=color[colorL++];
		else if(count>3)
			colorB[count-4]=color[colorL++];
		count++;
	}
	//将传入的RGB转换为BGR
	WCHAR colorBGR[16]={0};//字的颜色
	swprintf(colorBGR,L"%s%s%s",colorB,colorG,colorR);
	strColor=wcstol(colorBGR,NULL,16);//整形颜色值
	m_colorOffsR = GetRValue(strColor);
	m_colorOffsG = GetGValue(strColor);
	m_colorOffsB = GetBValue(strColor);
	////////处理颜色和色偏

	/*m_sim = simi;*/  //未使用该变量
	m_simColor = 0;
	if(simi==0.5)
		m_simColor=49;

	else if(simi==0.6)
		m_simColor=38;

	else if(simi==0.7)
		m_simColor=29;

	else if(simi==0.8)
		m_simColor=18;

	else if(simi==0.9)
		m_simColor=9;
	/////////////////////////////////////

	m_Dir = dir;
	/////////////////////////////控制四点的边界值以防止越界//////////////////////////
	wchar_t pathBuffer[MAX_PATH]={0};
	wchar_t RCPath[MAX_PATH]={0};

	if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())//normol找图
	{
		RECT rec;
		RECT clientrec;
		::GetClientRect(m_parenthwnd,&clientrec);
		int clienthight=clientrec.bottom-clientrec.top;
		int clientwide=clientrec.right-clientrec.left;
		::GetWindowRect(m_parenthwnd,&rec);
		POINT point;
		point.x=0;
		point.y=0;
		::ClientToScreen(m_parenthwnd,&point);
		m_Left=point.x;
		m_Top=point.y;
		m_Right=rec.right;
		m_bottom=rec.bottom;

		if(left<0)
			left=0;
		if(left >= clientwide)
			left = clientwide-1;

		if(top<0)
			top=0;
		if(top >= clienthight)
			top = clienthight-1;

		if(right >= clientwide)
			right=clientwide-1;

		if(bottom >= clienthight)
			bottom=clienthight-1;

		if(m_Left<0)
		{
			if((left+m_Left)<=0)//越界
				left=0;
			else
				left=m_Left+left;
		}
		else
			left=m_Left+left;

		if(m_Top>=0)
			top=m_Top+top;

		if(m_Right >= ::GetSystemMetrics(SM_CXSCREEN))
		{
			if((right+m_Left)>::GetSystemMetrics(SM_CXSCREEN))
				right=::GetSystemMetrics(SM_CXSCREEN)-1;
			else
				right=right+m_Left;
		}
		else
			right=right+m_Left;

		if(m_bottom >= ::GetSystemMetrics(SM_CYSCREEN))
		{
			if((bottom+m_Top)>=::GetSystemMetrics(SM_CYSCREEN))
				bottom=::GetSystemMetrics(SM_CYSCREEN)-1;
			else
				bottom=bottom+m_Top;
		}
		else
			bottom=bottom+m_Top;

		m_Left = left;
		m_Top = top;
		m_Right = right;
		m_bottom = bottom;

	}
	else
	{
		RECT clientrec;
		if(ColorDataInfo!=NULL)//DX
			::GetClientRect(m_hwnd,&clientrec);
		else//gdi
			::GetClientRect(hwnd,&clientrec);
		int clienthight=clientrec.bottom-clientrec.top;
		int clientwide=clientrec.right-clientrec.left;

		m_Left = left;
		m_Top = top;
		m_Right = right;
		m_bottom = bottom;
		//TSRuntime::add_log( "clientwide:%d,clienthight:%d,m_Left:%d,m_Top:%d,m_Right:%d,m_bottom:%d",clientwide,clienthight,left,top,right,bottom);

		if(left<0)
			m_Left = left =0;
		if(left >= clientwide)
			m_Left = clientwide-1;

		if(top<0)
			m_Top = top =0;
		if(top >= clienthight)
			m_Top = clienthight-1;

		if(right >= clientwide)
			m_Right=clientwide-1;

		if(bottom >= clienthight)
			m_bottom=clienthight-1;

		//TSRuntime::add_log( "m_Left:%d,m_Top:%d,m_Right:%d,m_bottom:%d",m_Left,m_Top,m_Right,m_bottom);
	}
	RECT rc;
	rc.bottom=m_bottom;
	rc.left=m_Left;
	rc.right=m_Right;
	rc.top=m_Top;

	if(ColorDataInfo!=NULL)
	{
		wndWidth=m_Right-m_Left;
		wndHeight=m_bottom-m_Top;
		//DX模式找字
		if(!getDXBitmap(pbuffer,ColorDataInfo))
		{
			return retIndex;
		}
	}	
	else
	{
		////GDI模式找字
		if(!getGDIBitmap(hwnd,rc))
		{
			return retIndex;
		}
	}

	/////////////////// 处理多张图 /////////////////////

	int col=0;
	while(*path)
	{
		IsFindPicNext=false;
		if(*path!=L'|')
		{
			pathBuffer[col++]=*path;
			path++;
			continue;
		}
		else
		{
			//StrTrimW(pathBuffer,L" ");//移除前后空格符
			wchar_t *findpath=wcsstr(pathBuffer,L":");
			if(wcslen(SetPath)>0&&findpath==NULL)
			{
				wchar_t pathstr[MAX_PATH]={0};
				wcscpy(pathstr,pathBuffer);
				swprintf(RCPath,L"%s\\%s",SetPath,pathstr);	//如果设置SetPath全路径的时候,在这里拼接图片全路径
			}
			else
				wcscpy(RCPath,pathBuffer);

			xpos_offset=0;
			ypos_offset=0;

			while(true)
			{
				if(findImage(RCPath,xpos,ypos))
				{
					xpos=xpos;
					ypos=ypos;
					if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
					{
						POINT pt;
						pt.x=xpos;
						pt.y=ypos;
						::ScreenToClient(m_parenthwnd,&pt);
						xpos=pt.x;
						ypos=pt.y;
					}
					if((ypos_offset!=0||xpos_offset!=0)&&IsFindPicNext)
					{
						if((xpos_offset+m_Left-1)==xpos&&(ypos_offset+m_Top)==ypos)
						{
							xpos=-1;
							ypos=-1;
							retIndex= -1;
							IsFindPicNext=false;
							break;
						}
					//	TSRuntime::add_log( "xpos_offset:%d,ypos_offset:%d,xpos:%d,ypos:%d",xpos_offset,ypos_offset,xpos,ypos);
					}

					if(!IsFindPicNext)
						index++;
					retIndex=index;

					if(!retstring)
						break;

					if(retstring&&type==2)
					{
						swprintf(retstring,L"%s,%d,%d",pathBuffer,xpos,ypos);
						break;
					}
					if(retstring&&type!=2)
					{
						int len=wcslen(retstring);
						if(len>(MAX_PATH*100-16))//防止找到太多,字符串越界,最多返回是260*4个字符
							break;
						else if(len>1&&len<(MAX_PATH*100-16)&&type==1)   //FindPicExS
							swprintf(retstring,L"%s|%s,%d,%d",retstring,pathBuffer,xpos,ypos);
						else if(len==0&&type==1)//FindPicExS
							swprintf(retstring,L"%s,%d,%d",pathBuffer,xpos,ypos);
						else if(len>1&&len<(MAX_PATH*100-16))//FindPicEx
							swprintf(retstring,L"%s|%d,%d,%d",retstring,index,xpos,ypos);
						else if(len==0)//FindPicEx
							swprintf(retstring,L"%d,%d,%d",index,xpos,ypos);
					}
					else
						break;
				}
				else
				{
					if(!IsFindPicNext)
						index++;
					xpos=-1;
					ypos=-1;
					retIndex= -1;
					IsFindPicNext=false;
					break;
				}

				IsFindPicNext=true;
				if(m_parenthwnd!=0)//normal
				{
					POINT pt;
					pt.x=xpos;
					pt.y=ypos;
					::ClientToScreen(m_parenthwnd,&pt);
					xpos=pt.x;
					ypos=pt.y;
				}
				if(m_Dir==0)//leftToRightFromTop
				{
					xpos_offset=xpos-m_Left+1;
					ypos_offset=ypos-m_Top;
				}
				else if(m_Dir==1)//leftToRightFromBottom
				{
					xpos_offset=xpos-m_Left+1;
					ypos_offset=m_bottom-ypos;
				}
				else if(m_Dir==2)//rightToLeftFromTop
				{
					xpos_offset=m_Right-xpos-1;
					ypos_offset=ypos-m_Top;
				}
				else if(m_Dir==3)//rightToLeftFromBottom
				{
					xpos_offset=m_Right-xpos+1;
					ypos_offset=m_bottom-ypos;
				}
			}
			if((!retstring||type==2)&&retIndex!=-1)
				break;
			::memset(pathBuffer,0,sizeof(wchar_t)*MAX_PATH);
			col=0;
			path++;
			//index++; 
		}
	}

	//////////////最后一张图或者是只有一张图///////////
	if(retIndex==-1||(retstring!=NULL&&type!=2))
	{
		//StrTrimW(pathBuffer,L" ");//移除前后空格符
		wchar_t *findpath=wcsstr(pathBuffer,L":");
		if(wcslen(SetPath)>0&&findpath==NULL)
		{
			if(wcsstr(pathBuffer,L":")==NULL)
			{
				wchar_t pathstr[MAX_PATH]={0};
				wcscpy(pathstr,pathBuffer);
				swprintf(RCPath,L"%s\\%s",SetPath,pathstr);	//如果设置SetPath全路径的时候,在这里拼接图片全路径
			}
		}
		else
			wcscpy(RCPath,pathBuffer);
		IsFindPicNext=false;
		xpos_offset=0;
		ypos_offset=0;
		while(true)
		{
			if(findImage(RCPath,xpos,ypos))
			{
				xpos=xpos;
				ypos=ypos;
				//TSRuntime::add_log( "xpos_offset:%d,ypos_offset:%d,xpos:%d,ypos:%d",xpos_offset,ypos_offset,xpos,ypos);
				if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
				{
					xpos_offset=xpos-m_Left+1;
					ypos_offset=ypos-m_Top;
					POINT pt;
					pt.x=xpos;
					pt.y=ypos;
					::ScreenToClient(m_parenthwnd,&pt);
					xpos=pt.x;
					ypos=pt.y;
				}

				if((ypos_offset!=0||xpos_offset!=0)&&IsFindPicNext)
				{
					if((xpos_offset+m_Left-1)==xpos&&(ypos_offset+m_Top)==ypos)
					{
						xpos=-1;
						ypos=-1;
						retIndex= -1;
						IsFindPicNext=false;
						break;
					}
				}

				if(!IsFindPicNext)
					index++;
				retIndex=index;

				if(!retstring)
					break;

				if(retstring&&type==2)
				{
					swprintf(retstring,L"%s,%d,%d",pathBuffer,xpos,ypos);
					break;
				}
				if(retstring&&type!=2)
				{
					int len=wcslen(retstring);
					if(len>(MAX_PATH*100-16))//防止找到太多,字符串越界,最多返回是260*4个字符
						break;
					else if(len>1&&len<(MAX_PATH*100-16)&&type==1)   //FindPicExS
						swprintf(retstring,L"%s|%s,%d,%d",retstring,pathBuffer,xpos,ypos);
					else if(len==0&&type==1)//FindPicExS
						swprintf(retstring,L"%s,%d,%d",pathBuffer,xpos,ypos);
					else if(len>1&&len<(MAX_PATH*100-16))//FindPicEx
						swprintf(retstring,L"%s|%d,%d,%d",retstring,index,xpos,ypos);
					else if(len==0)//FindPicEx
						swprintf(retstring,L"%d,%d,%d",index,xpos,ypos);
				}
			}
			else
			{
				xpos=-1;
				ypos=-1;
				retIndex= -1;
				IsFindPicNext=false;
				break;
			}

			IsFindPicNext=true;
			if(m_parenthwnd!=0)//normal
			{
				POINT pt;
				pt.x=xpos;
				pt.y=ypos;
				::ClientToScreen(m_parenthwnd,&pt);
				xpos=pt.x;
				ypos=pt.y;
			}
			if(m_Dir==0)//leftToRightFromTop
			{
				xpos_offset=xpos-m_Left+1;
				ypos_offset=ypos-m_Top;
			}
			else if(m_Dir==1)//leftToRightFromBottom
			{
				xpos_offset=xpos-m_Left+1;
				ypos_offset=m_bottom-ypos;
			}
			else if(m_Dir==2)//rightToLeftFromTop
			{
				xpos_offset=m_Right-xpos-1;
				ypos_offset=ypos-m_Top;
			}
			else if(m_Dir==3)//rightToLeftFromBottom
			{
				xpos_offset=m_Right-xpos+1;
				ypos_offset=m_bottom-ypos;
			}

			if(!retstring||type==2)
				break;

		}
		//TSRuntime::add_log( "end,path:%s",W2A(path));
		//::SetCursorPos(xpos,ypos);
	}

endfind:

	return retIndex;


}

bool MyFindPictureClass::findImage(wchar_t* path,long &xpos,long &ypos,int type)
{
	bool isFind=false;

	if(loadBitmap(path)==0)
		return false;

	switch(m_Dir)
	{
	case 0:
		{
			isFind=leftToRightFromTop(xpos,ypos); //0:从左到右,从上倒下
			break;
		}
	case 1:
		isFind=leftToRightFromBottom(xpos,ypos);//1:从右到左,从上倒下
		break;

	case 2:
		isFind=rightToLeftFromTop(xpos,ypos);//2:从左到右,从下倒上
		break;

	case 3:
		isFind=rightToLeftFromBottom(xpos,ypos);//3:从右到左,从下倒上
		break;	
	}
	//TSRuntime::add_log( "findImage-xpos:%d,ypos:%d,xpos_offset:%d,ypos_offset:%d,m_Dir:%d",xpos,ypos,xpos_offset,ypos_offset,m_Dir);
	for(int i=0;i<loadHeight;i++)
	{
		delete [] pLoadBmpBuffer[i];
		pLoadBmpBuffer[i] = NULL;
	}
	delete [] pLoadBmpBuffer;
	pLoadBmpBuffer=NULL;


	//TSRuntime::add_log( "findImage-end");
	return isFind;

}

////////////////// 检索 方式 //////////////////
////////////////////////////////////////////////0：从左到右,从上倒下//////////////////////////
bool MyFindPictureClass::leftToRightFromTop(long &xpos,long &ypos)
{
	//TSRuntime::add_log( "leftToRightFromTop,loadHeight:%d,loadWidth:%d",loadHeight,loadWidth);
	bool isFind=false;
	bool isfirst=true;
	COLORREF pColor = pLoadBmpBuffer[0][0];  //// 目标图片的背景色
	if(pLoadBmpBuffer[0][0]==pLoadBmpBuffer[0][loadWidth-1]&&
		pLoadBmpBuffer[0][0]==pLoadBmpBuffer[loadHeight-1][loadWidth-1]&&
		pLoadBmpBuffer[0][0]==pLoadBmpBuffer[loadHeight-1][0])
	{
		int startx=0,starty=0;
		for(int m=0;m<loadHeight;m++)
		{
			for(int n=0;n<loadWidth;n++)
			{
				if(pLoadBmpBuffer[m][n]==pColor)  
				{
					//////过滤掉不处理的背景透明色
					continue;
				}
				else
				{
					//2012.2.28 修改相似的图无法识别的问题.
					startx=n;
					starty=m;
					goto toSerch;
				}
			}
			if((m+1)==loadWidth)//如果全图是一个颜色，说明不是透明图
				goto Serch;

		}

toSerch:
		for(int i=m_Top+ypos_offset; i<= m_bottom-1;i++)
		{
			if(isfirst)
				isfirst=false;
			else if(xpos_offset!=0)
				xpos_offset=0;

			if(m_bottom-i<loadHeight)
			{
				goto end;
			}
			for(int j=m_Left+xpos_offset;j<=m_Right-1;j++)
			{
				if(m_Right-j<loadWidth)
				{
					break;
				}
				if(comparaLeftToRightFromTop(i,j,startx,starty))
				{
					isFind=true;
					xpos=j;
					ypos=i;
					goto end;
				}
			}
		}
	}
	else
	{
Serch:
		//TSRuntime::add_log( "comparaLeftToRightFromTopEx-start");
		for(int i=m_Top+ypos_offset; i<= m_bottom-1;i++)
		{
			if(isfirst)
				isfirst=false;
			else if(xpos_offset!=0)
				xpos_offset=0;
			
			if(m_bottom-i<loadHeight)
			{
				goto end;
			}
			for(int j=m_Left+xpos_offset;j<=m_Right-1;j++)
			{
				if(m_Right-j<loadWidth)
				{
					break;
				}
				if(comparaLeftToRightFromTopEx(i,j))
				{
					isFind=true;
					xpos=j;
					ypos=i;
					goto end;
				}
			}
		}
	}
end:

	return isFind;
}

////////处理背景色为透明的图片////////////////
bool MyFindPictureClass::comparabmp(int startX,int startY,int endX,int endY)
{
	int fristX=0;
	int fristY=0;
	bool isFrist=true;
	COLORREF pColor = pLoadBmpBuffer[0][0];  //// 目标图片的背景色

	BYTE loadR=0, loadG=0, loadB=0;
	BYTE wndR =0, wndG =0, wndB =0;
	int absResult =0;
	COLORREF pLoadColor = 0,pWndColor = 0;
	for(int i=startY;i<endY;i++)
	{
		for(int j=startX;j<endX;j++)
		{
			if(pLoadBmpBuffer[i][j]==pColor)  
			{
				//////过滤掉不处理的背景透明色
				continue;
			}
			else
			{	
				pLoadColor=pLoadBmpBuffer[i][j];
				loadR = GetRValue(pLoadColor);
				loadG = GetGValue(pLoadColor);
				loadB = GetBValue(pLoadColor);

				pWndColor=pWndBmpBuffer[i][j];
				wndR = GetRValue(pWndColor);
				wndG = GetGValue(pWndColor);
				wndB = GetBValue(pWndColor);
				///////下面的是或关系。
				if(abs(loadR-wndR)==0 && abs(loadG-wndG)==0 && abs(loadB-wndB)==0)
				{
					continue;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return true;
}

////////处理背景色为透明的图片////////////////
bool MyFindPictureClass::comparaLeftToRightFromTop(int h,int w,int startX,int startY)
{
	//TSRuntime::add_log( "comparaLeftToRightFromTop-h:%d,w:%d,startX:%d,startY:%d",h,w,startX,startY);
	int fristX=0;
	int fristY=0;
	bool isFrist=true;
	COLORREF pColor = pLoadBmpBuffer[0][0];  //// 目标图片的背景色

	BYTE loadR=0, loadG=0, loadB=0;
	BYTE wndR =0, wndG =0, wndB =0;
	int absResult =0;
	COLORREF pLoadColor = 0,pWndColor = 0;
	for(int i=startY;i<loadHeight;i++)
	{
		for(int j=0;j<loadWidth;j++)
		{
			if(pLoadBmpBuffer[i][j]==pColor)  
			{
				//////过滤掉不处理的背景透明色
				continue;
			}
			else
			{	
				pLoadColor=pLoadBmpBuffer[i][j];
				loadR = GetRValue(pLoadColor);
				loadG = GetGValue(pLoadColor);
				loadB = GetBValue(pLoadColor);

				pWndColor=pWndBmpBuffer[h+i/*-fristY*/][w+j/*-fristX*/];
				wndR = GetRValue(pWndColor);
				wndG = GetGValue(pWndColor);
				wndB = GetBValue(pWndColor);
				absResult=abs(loadR-wndR)+abs(loadG-wndG)+abs(loadB-wndB);   ///用于相似度比较
				///////下面的是或关系。
				//if(abs(loadR-wndR)<=m_colorOffsR && abs(loadG-wndG)<=m_colorOffsG && abs(loadB-wndB)<=m_colorOffsB ||absResult<=m_simColor)
				if(abs(loadR-wndR)<=(m_colorOffsR+m_simColor) && abs(loadG-wndG)<=(m_colorOffsG+m_simColor) && abs(loadB-wndB)<=(m_colorOffsB+m_simColor) )
				{
					continue;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return true;
}
////////处理背景色为不透明的图片////////////////
bool MyFindPictureClass::comparaLeftToRightFromTopEx(int h,int w)
{

	BYTE loadR=0, loadG=0, loadB=0;
	BYTE wndR =0, wndG =0, wndB =0;
	int absResult =0;
	COLORREF pLoadColor = 0,pWndColor = 0;
	for(int i=0;i<loadHeight;i++)
	{
		for(int j=0;j<loadWidth;j++)
		{

			pLoadColor=pLoadBmpBuffer[i][j];
			loadR = GetRValue(pLoadColor);
			loadG = GetGValue(pLoadColor);
			loadB = GetBValue(pLoadColor);

			pWndColor=pWndBmpBuffer[h+i][w+j];
			wndR = GetRValue(pWndColor);
			wndG = GetGValue(pWndColor);
			wndB = GetBValue(pWndColor);

			//absResult=(loadR-wndR)+abs(loadG-wndG)+abs(loadB-wndB);   ///用于相似度比较
			//if(pLoadBmpBuffer[i][j] == pWndBmpBuffer[h+i][w+j])
			if(abs(loadR-wndR)<=(m_colorOffsR+m_simColor) && abs(loadG-wndG)<=(m_colorOffsG+m_simColor) && abs(loadB-wndB)<=(m_colorOffsB+m_simColor) )
			{
				continue;
			}
			else
			{
				return false;
			}

		}
	}
	return true;
}


///////////////////////////////////////////////1：从左到右,从下倒上//////////////////////////
//bool MyFindPictureClass::leftToRightFromBottom(int &xpos,int &ypos)
bool MyFindPictureClass::leftToRightFromBottom(long &xpos,long &ypos)
{
	bool isFind=false;
	bool isfirst=true;
	COLORREF pColor = pLoadBmpBuffer[0][0];  //// 目标图片的背景色
	if(pLoadBmpBuffer[0][0]==pLoadBmpBuffer[0][loadWidth-1]&&
		pLoadBmpBuffer[0][0]==pLoadBmpBuffer[loadHeight-1][loadWidth-1]&&
		pLoadBmpBuffer[0][0]==pLoadBmpBuffer[loadHeight-1][0])
	{
		int startx=0,starty=0;
		for(int m=loadHeight-1;m>=0;m--)
		{
			for(int n=0;n<loadWidth;n++)
			{
				if(pLoadBmpBuffer[m][n] == pColor)  
				{
					//////过滤掉不处理的背景透明色
					continue;
				}
				else
				{
					//2012.2.28 修改相似的图无法识别的问题.
					startx=n;
					starty=m;
					goto toSerch;
				}
			}
			if((m+1)==loadWidth)//如果全图是一个颜色，说明不是透明图
				goto Serch;
		}

toSerch:
		for(int i=m_bottom-ypos_offset;i>=m_Top-1;i--)
		{
			if(isfirst)
				isfirst=false;
			else if(xpos_offset!=0)
				xpos_offset=0;
			if( i-m_Top < loadHeight)	goto end;

			for(int j=m_Left+xpos_offset;j<m_Right-1;j++)
			{
				if(m_Right-j < loadWidth) break;

				if(comparaLeftToRightFromBottom(i,j,startx,starty))
				{
					isFind=true;
					xpos=j;
					ypos=i-starty;
					goto end;
				}
			}
		}
	}
	else
	{
Serch:
		for(int i=m_bottom-ypos_offset;i>=m_Top-1;i--)
		{
			if(isfirst)
				isfirst=false;
			else if(xpos_offset!=0)
				xpos_offset=0;
			if( i-m_Top < loadHeight)	goto end;

			for(int j=m_Left+xpos_offset;j<m_Right-1;j++)
			{
				if(m_Right-j < loadWidth) break;

				if(comparaLeftToRightFromBottomEx(i,j))
				{
					isFind=true;
					xpos=j;
					ypos=i;
					goto end;
				}
			}
		}
	}

end:
	return isFind;
}
////////处理背景色为透明的图片////////////////
//bool MyFindPictureClass::comparaLeftToRightFromBottom(int h,int w,int startX,int startY)
bool MyFindPictureClass::comparaLeftToRightFromBottom(int h,int w,int startX,int startY)
{
	int fristX=0;
	int fristY=0;
	bool isFrist=true;
	COLORREF pColor = pLoadBmpBuffer[0][0];  //// 目标图片的背景色

	BYTE loadR=0, loadG=0, loadB=0;
	BYTE wndR =0, wndG =0, wndB =0;
	int absResult =0;
	//int colAdd =-1;
	COLORREF pLoadColor = 0,pWndColor = 0;
	for(int i=startY;i>=0;i--)
	{
		//colAdd++;
		for(int j=startY;j<loadWidth;j++)
		{
			if(pLoadBmpBuffer[i][j]==pColor)  
			{
				//////过滤掉不处理的背景透明色
				continue;
			}
			else
			{
				pLoadColor = pLoadBmpBuffer[i][j];
				loadR = GetRValue(pLoadColor);
				loadG = GetGValue(pLoadColor);
				loadB = GetBValue(pLoadColor);

				pWndColor = pWndBmpBuffer[h-(loadHeight-i-1)][w+j];
				wndR = GetRValue(pWndColor);
				wndG = GetGValue(pWndColor);
				wndB = GetBValue(pWndColor);


				absResult=abs(loadR-wndR)+abs(loadG-wndG)+abs(loadB-wndB);   ///用于相似度比较
				//if(pLoadBmpBuffer[i][j] == pWndBmpBuffer[h-colAdd][w+(j-fristX)])
				//if(abs(loadR-wndR)<=m_colorOffsR && abs(loadG-wndG)<=m_colorOffsG && abs(loadB-wndB)<=m_colorOffsB )
				if(abs(loadR-wndR)<=(m_colorOffsR+m_simColor) && abs(loadG-wndG)<=(m_colorOffsG+m_simColor) && abs(loadB-wndB)<=(m_colorOffsB+m_simColor))
				{
					continue;
				}
				else
				{
					return false;
				}
			}
		}
	}
	return true;
}
////////处理背景色为不透明的图片////////////////
//bool MyFindPictureClass::comparaLeftToRightFromBottomEx(int h,int w)
bool  MyFindPictureClass::comparaLeftToRightFromBottomEx(int h,int w)
{
	BYTE loadR=0, loadG=0, loadB=0;
	BYTE wndR =0, wndG =0, wndB =0;
	int absResult =0;
	//int colAdd =-1;
	COLORREF pLoadColor = 0,pWndColor = 0;
	for(int i=0 ;i<loadHeight;i++)
	{
		for(int j=0;j<loadWidth;j++)
		{
			pLoadColor = pLoadBmpBuffer[i][j];
			loadR = GetRValue(pLoadColor);
			loadG = GetGValue(pLoadColor);
			loadB = GetBValue(pLoadColor);

			pWndColor = pWndBmpBuffer[h+i][w+j];
			wndR = GetRValue(pWndColor);
			wndG = GetGValue(pWndColor);
			wndB = GetBValue(pWndColor);

			absResult=(loadR-wndR)+abs(loadG-wndG)+abs(loadB-wndB);   ///用于相似度比较
			//if(pLoadBmpBuffer[i][j] == pWndBmpBuffer[h+i][w+j])
			//if(abs(loadR-wndR)<=m_colorOffsR && abs(loadG-wndG)<=m_colorOffsG && abs(loadB-wndB)<=m_colorOffsB )
			if(abs(loadR-wndR)<=(m_colorOffsR+m_simColor) && abs(loadG-wndG)<=(m_colorOffsG+m_simColor) && abs(loadB-wndB)<=(m_colorOffsB+m_simColor))
			{
				continue;
			}
			else
			{
				return false;
			}

		}
	}
	return true;
}

////////////////////////////////////////////////2：从右到左,从上倒下////////////////////////
//bool MyFindPictureClass::rightToLeftFromTop(int &xpos,int &ypos)
bool   MyFindPictureClass::rightToLeftFromTop(long &xpos,long &ypos)
{
	bool isFind=false;
	bool isfirst=true;
	COLORREF pColor = pLoadBmpBuffer[0][0];  //// 目标图片的背景色
	if(pLoadBmpBuffer[0][0]==pLoadBmpBuffer[0][loadWidth-1]&&
		pLoadBmpBuffer[0][0]==pLoadBmpBuffer[loadHeight-1][loadWidth-1]&&
		pLoadBmpBuffer[0][0]==pLoadBmpBuffer[loadHeight-1][0])
	{
		int startx=0,starty=0;
		for(int m=0;m<loadHeight;m++)
		{
			for(int n=loadWidth-1;n>=0;n--)
			{
				if(pLoadBmpBuffer[m][n]==pColor)  
				{
					//////过滤掉不处理的背景透明色
					continue;
				}
				else
				{
					//2012.2.28 修改相似的图无法识别的问题.
					startx=n;
					starty=m;
					goto toSerch;
				}
			}
			if((m+1)==loadWidth)//如果全图是一个颜色，说明不是透明图
				goto Serch;
		}

toSerch:
		for(int i=m_Top+ypos_offset; i<= m_bottom-1;i++)
		{
			if(isfirst)
				isfirst=false;
			else if(xpos_offset!=0)
				xpos_offset=0;

			if(m_bottom-i < loadHeight) goto end;

			for(int j=m_Right-1-xpos_offset;j>=m_Left-1;j--)
			{
				if(j - m_Left < loadWidth) break;

				if(comparaRightToLeftFromTop(i,j,startx,starty))
				{
					isFind=true;
					//xpos=j/*+m_helpXpoint-loadWidth+1*/;
					xpos=j-loadWidth+1;
					ypos=i/*-m_helpYpoint*/;
					goto end;
				}
			}
		}
	}
	else
	{
Serch:
		for(int i=m_Top+ypos_offset; i<= m_bottom-1;i++)
		{
			if(isfirst)
				isfirst=false;
			else if(xpos_offset!=0)
				xpos_offset=0;

			if(m_bottom-i < loadHeight) goto end;

			for(int j=m_Right-1-xpos_offset;j>=m_Left-1;j--)
			{		
				if(j - m_Left < loadWidth) break;

				if(comparaRightToLeftFromTopEx(i,j))
				{
					isFind=true;
					xpos=j-loadWidth+1;
					ypos=i;
					goto end;
				}
			}
		}
	}

end:

	return isFind;
}
////////处理背景色为透明的图片////////////////
//bool MyFindPictureClass::comparaRightToLeftFromTop(int h,int w,int startX,int startY)
bool MyFindPictureClass::comparaRightToLeftFromTop(int h,int w,int startX,int startY)
{
	int fristX=0;
	int fristY=0;
	bool isFrist=true;
	COLORREF pColor = pLoadBmpBuffer[0][0];  //// 目标图片的背景色

	BYTE loadR=0, loadG=0, loadB=0;
	BYTE wndR =0, wndG =0, wndB =0;
	int absResult =0;

	//int lineAdd =-1;
	COLORREF pLoadColor = 0,pWndColor = 0;
	for(int i=startY;i<loadHeight;i++)
	{
		//lineAdd =-1;
		for(int j=loadWidth-1;j>=0;j--)
		{
			//lineAdd++;
			if(pLoadBmpBuffer[i][j]==pColor)  
			{
				//////过滤掉不处理的背景透明色
				continue;
			}
			pLoadColor = pLoadBmpBuffer[i][j];
			loadR = GetRValue(pLoadColor);
			loadG = GetGValue(pLoadColor);
			loadB = GetBValue(pLoadColor);

			pWndColor = pWndBmpBuffer[h+i][w-(loadWidth-j-1)];
			wndR = GetRValue(pWndColor);
			wndG = GetGValue(pWndColor);
			wndB = GetBValue(pWndColor);

			absResult=(loadR-wndR)+abs(loadG-wndG)+abs(loadB-wndB);   ///用于相似度比较

			//if(pLoadBmpBuffer[i][j] == pWndBmpBuffer[h+i][w+(/ *fristX* /0-lineAdd)])
			//if(abs(loadR-wndR)<=m_colorOffsR && abs(loadG-wndG)<=m_colorOffsG && abs(loadB-wndB)<=m_colorOffsB )
			if(abs(loadR-wndR)<=(m_colorOffsR+m_simColor) && abs(loadG-wndG)<=(m_colorOffsG+m_simColor) && abs(loadB-wndB)<=(m_colorOffsB+m_simColor) )
			{
				continue;
			}
			else
			{
				return false;
			}
			//}
		}
	}
	return true;
}
////////处理背景色为不透明的图片////////////////
//bool MyFindPictureClass::comparaRightToLeftFromTopEx(int h,int w)
bool MyFindPictureClass::comparaRightToLeftFromTopEx(int h,int w)
{
	BYTE loadR=0, loadG=0, loadB=0;
	BYTE wndR =0, wndG =0, wndB =0;
	int absResult =0;

	//int lineAdd =-1;
	COLORREF pLoadColor = 0,pWndColor = 0;
	for(int i=0;i<loadHeight;i++)
	{
		//lineAdd = -1;
		for(int j=loadWidth-1;j>=0;j--)
		{
			//lineAdd++;
			pLoadColor = pLoadBmpBuffer[i][j];
			loadR = GetRValue(pLoadColor);
			loadG = GetGValue(pLoadColor);
			loadB = GetBValue(pLoadColor);

			pWndColor = pWndBmpBuffer[h+i][w-(loadWidth-j-1)];
			wndR = GetRValue(pWndColor);
			wndG = GetGValue(pWndColor);
			wndB = GetBValue(pWndColor);

			absResult=(loadR-wndR)+abs(loadG-wndG)+abs(loadB-wndB);   ///用于相似度比较
			//if(pLoadBmpBuffer[i][j] == pWndBmpBuffer[h+i][w+j])
			//if(abs(loadR-wndR)<=m_colorOffsR && abs(loadG-wndG)<=m_colorOffsG && abs(loadB-wndB)<=m_colorOffsB )
			if(abs(loadR-wndR)<=(m_colorOffsR+m_simColor) && abs(loadG-wndG)<=(m_colorOffsG+m_simColor) && abs(loadB-wndB)<=(m_colorOffsB+m_simColor) )
			{
				continue;
			}
			else
			{
				return false;
			}

		}
	}
	return true;
}

///////////////////////////////////////////////////3：从右到左,从下倒上////////////////////
//bool MyFindPictureClass::rightToLeftFromBottom(int &xpos,int &ypos)
bool MyFindPictureClass::rightToLeftFromBottom(long &xpos,long &ypos)
{
	bool isFind=false;
	bool isfirst=true;
	COLORREF pColor = pLoadBmpBuffer[0][0];  //// 目标图片的背景色
	if(pLoadBmpBuffer[0][0]==pLoadBmpBuffer[0][loadWidth-1]&&
		pLoadBmpBuffer[0][0]==pLoadBmpBuffer[loadHeight-1][loadWidth-1]&&
		pLoadBmpBuffer[0][0]==pLoadBmpBuffer[loadHeight-1][0])
	{
		int startx=0,starty=0;
		for(int m=loadHeight-1;m>=0;m--)
		{
			for(int n=loadWidth-1;n>=0;n--)
			{
				if(pLoadBmpBuffer[m][n]==pColor)  
				{
					//////过滤掉不处理的背景透明色
					continue;
				}
				else
				{
					//2012.2.28 修改相似的图无法识别的问题.
					startx=n;
					starty=m;
					goto toSerch;
				}
			}
			if((m+1)==loadWidth)//如果全图是一个颜色，说明不是透明图
				goto Serch;
		}

toSerch:

		for(int i=m_bottom-ypos_offset;i>=m_Top-1;i--)
		{
			if(isfirst)
				isfirst=false;
			else if(xpos_offset!=0)
				xpos_offset=0;
			if(i - m_Top<loadHeight)
			{
				goto end;
			}

			for(int j=m_Right-xpos_offset;j>=m_Left-1;j--)
			{
				if(j - m_Left<loadWidth)
				{
					break;
				}

				if(comparaRightToLeftFromBottom(i,j,startx,starty))
				{
					isFind=true;
					xpos=j-startx-1;
					ypos=i-starty;
					goto end;
				}
			}
		}
	}
	else
	{
Serch:
		for(int i=m_bottom-ypos_offset;i>=m_Top-1;i--)
		{
			if(isfirst)
				isfirst=false;
			else if(xpos_offset!=0)
				xpos_offset=0;
			if(i - m_Top<loadHeight)
			{
				goto end;
			}

			for(int j=m_Right-xpos_offset;j>=m_Left-1;j--)
			{
				if(j - m_Left<loadWidth)
				{
					break;
				}

				if(comparaRightToLeftFromBottomEx(i,j))
				{
					isFind=true;
					xpos=j;
					ypos=i;
					goto end;
				}
			}
		}
	}
end:
	return isFind;
}
////////处理背景色为透明的图片////////////////
//bool MyFindPictureClass::comparaRightToLeftFromBottom(int h,int w,int startX,int startY)
bool MyFindPictureClass::comparaRightToLeftFromBottom(int h,int w,int startX,int startY)
{
	int fristX=0;
	int fristY=0;
	bool isFrist=true;
	COLORREF pColor = pLoadBmpBuffer[0][0];  //// 目标图片的背景色

	BYTE loadR=0, loadG=0, loadB=0;
	BYTE wndR =0, wndG =0, wndB =0;
	int absResult =0;

	//int lineAdd=-1;
	//int colAdd =-1;
	COLORREF pLoadColor = 0,pWndColor = 0;
	for(int i=startY; i>=0; i--)
	{
		//colAdd ++;
		//lineAdd=-1;
		for(int j=loadWidth-1;j>=0;j--)
		{
			//lineAdd++;
			if(pLoadBmpBuffer[i][j]==pColor)  
			{
				//////过滤掉不处理的背景透明色
				continue;
			}
			else
			{
				pLoadColor=pLoadBmpBuffer[i][j];

				loadR = GetRValue(pLoadColor);
				loadG = GetGValue(pLoadColor);
				loadB = GetBValue(pLoadColor);

				pWndColor=pWndBmpBuffer[h-(loadHeight-i-1)][w-(loadWidth-j-1)];
				wndR = GetRValue(pWndColor);
				wndG = GetGValue(pWndColor);
				wndB = GetBValue(pWndColor);

				if(abs(loadR-wndR)<=(m_colorOffsR+m_simColor) && abs(loadG-wndG)<=(m_colorOffsG+m_simColor) && abs(loadB-wndB)<=(m_colorOffsB+m_simColor) )
				{
					continue;
				}
				else
				{
					return false;
				}
			}
		}
	}

	return true;
}
////////处理背景色为不透明的图片////////////////
//bool MyFindPictureClass::comparaRightToLeftFromBottomEx(int h,int w)
bool MyFindPictureClass::comparaRightToLeftFromBottomEx(int h,int w)
{
	BYTE loadR=0, loadG=0, loadB=0;
	BYTE wndR =0, wndG =0, wndB =0;
	int absResult =0;

	//int lineAdd=-1;
	//int colAdd =-1;
	COLORREF pLoadColor = 0,pWndColor = 0;
	//for(int i=loadHeight-1;i>=0;i--)
	for(int i=0;i<loadHeight;i++)
	{
		//colAdd ++;
		//lineAdd=-1;
		//for(int j=loadWidth-1;j>=0;j--)
		for(int j=0;j<loadWidth;j++)
		{
			//lineAdd++;

			pLoadColor=pLoadBmpBuffer[i][j];
			loadR = GetRValue(pLoadColor);
			loadG = GetGValue(pLoadColor);
			loadB = GetBValue(pLoadColor);

			//COLORREF colorWnd=pWndBmpBuffer[h-colAdd][w-lineAdd];
			pWndColor=pWndBmpBuffer[h+i][w+j];
			wndR = GetRValue(pWndColor);
			wndG = GetGValue(pWndColor);
			wndB = GetBValue(pWndColor);

			//absResult=(loadR-wndR)+abs(loadG-wndG)+abs(loadB-wndB);   ///用于相似度比较
			//if(abs(loadR-wndR)<=m_colorOffsR && abs(loadG-wndG)<=m_colorOffsG && abs(loadB-wndB)<=m_colorOffsB )
			if(abs(loadR-wndR)<=(m_colorOffsR+m_simColor) && abs(loadG-wndG)<=(m_colorOffsG+m_simColor) && abs(loadB-wndB)<=(m_colorOffsB+m_simColor) )
			{
				continue;
			}
			else
			{
				return false;
			}

		}
	}
	return true;
}

/////// 颜色内存处理。。 提高速度 //////////////////////
//bool MyFindPictureClass::processCreenWindow(int color,int left,int top,int right,int bottom,double simi,int dir,int &x,int &y,wchar_t* colorStr,bool isMulitColor)
bool MyFindPictureClass::processCreenWindow(int color,int left,int top,int right,int bottom,double simi,int dir,int &x,int &y,wchar_t* colorStr,bool isMulitColor)
{
	int Width=GetSystemMetrics(SM_CXSCREEN);
	int Height=GetSystemMetrics(SM_CYSCREEN);

	////////////////////////////四点的边界值判断////////////////
	if(right<left || top>bottom)
		return false;

	if(left<0)
		left=0;

	if(top<0)
		top=0;

	if(left>=Width)
		left=Width-1;

	if(top>=Height)
		top=Height-1;

	if(right>=Width)
		right=Width-1;

	if(bottom>=Height)
		bottom=Height-1;
	////////////////////////////////////////////////////////////

	// 24位图的BITMAPINFO
	BITMAPINFO *pBITMAPINFO = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER));
	memset(pBITMAPINFO, 0, sizeof(BITMAPINFOHEADER));
	BITMAPINFOHEADER *pInfo_Header = (BITMAPINFOHEADER *)pBITMAPINFO;
	pInfo_Header->biSize = sizeof(BITMAPINFOHEADER);   
	pInfo_Header->biWidth = right - left;      //宽度
	pInfo_Header->biHeight = (bottom - top);   //长度
	pInfo_Header->biPlanes = 1;				   //目标设备的位平面数
	pInfo_Header->biBitCount = 24; //对于每个像素的字节数 24:16M真彩色图,一般没有调色板,图像数据中每3个字节表示一个像素,每个字节表示一个RGB分量
	pInfo_Header->biCompression = BI_RGB; //现实位图时必须的颜色数 0:表示所有颜色

	HDC dc;
	dc=::CreateDC(L"DISPLAY",NULL,NULL,NULL);

	HBITMAP hBitmap=::CreateCompatibleBitmap(dc,Width,Height);
	HDC tdc;
	tdc=::CreateCompatibleDC(dc);
	HBITMAP hOldBitmap=(HBITMAP)::SelectObject(tdc,hBitmap);
	BitBlt(tdc,0,0,pInfo_Header->biWidth,pInfo_Header->biHeight,dc,left,top,CAPTUREBLT|SRCCOPY);
	hBitmap=(HBITMAP)::SelectObject(tdc,hOldBitmap);

	// 获得数据buf
	DWORD bufSize = (pInfo_Header->biWidth * 3 + 3) / 4 * 4 * pInfo_Header->biHeight;
	BYTE * pBuffer = new BYTE[bufSize];

	int aHeight=pInfo_Header->biHeight;

	if(::GetDIBits(tdc, hBitmap, 0, aHeight, pBuffer,pBITMAPINFO, DIB_RGB_COLORS) == 0)
	{
		::MessageBox(NULL,L"获取Color位图数据失败",L"TC",0);
		return false;
	}

	int width=pBITMAPINFO->bmiHeader.biWidth;
	int height=pBITMAPINFO->bmiHeader.biHeight;

	COLORREF** pColorBuffer = new COLORREF*[height];     ///// 二维数组 用来存储图像的颜色值
	for(int i=0;i<height;i++)
	{
		pColorBuffer[i]=new COLORREF[width];
	}

	int help_simi=0;
	if(simi==0.5)
		help_simi=49;

	else if(simi==0.6)
		help_simi=38;

	else if(simi==0.7)
		help_simi=29;

	else if(simi==0.8)
		help_simi=18;

	else if(simi==0.9)
		help_simi=9;

	COLORREF helpcolor=0;
	/*BYTE r1=0,g1=0,b1=0;*/
	int pitch=width%4;
	for(int i=0;i<height;i++)		//获取范围内所有颜色值,并保存
	{
		int realPitch=i*pitch;
		for(int j=0;j<width;j++)
		{
			UCHAR b=pBuffer[(i*width+j)*3+realPitch];
			UCHAR g=pBuffer[(i*width+j)*3+1+realPitch];
			UCHAR r=pBuffer[(i*width+j)*3+2+realPitch];
			helpcolor=RGB(r,g,b);
			pColorBuffer[height-i-1][j]=helpcolor;	
		}
	}

	//转换传入的颜色值参数
	BYTE rValue=0,gValue=0,bValue=0;
	rValue=GetRValue((DWORD)color);
	gValue=GetGValue((DWORD)color);
	bValue=GetBValue((DWORD)color);

	BYTE rHelpValue=0,gHelpValue=0,bHelpValue=0;

	//////////////////////////////四点的边界值判断////////////////
	//if(left<0)
	//	left=0;
	//if(left>=Width)
	//	left=Width-1;
	//if(top<0)
	//	top=0;
	//if(top>=Height)
	//	top=Height-1;

	//if(right>=Width)
	//	right=Width-1;
	//if(bottom>=Height)
	//	bottom=Height-1;
	//////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////
	if(isMulitColor)  // // //////  多个颜色值的找色方法
	{
		//XMLString::trim(colorStr);
		/////////////////////0 从左到右 从上到下 方式查找///////////////
		int absResult = 0;
		for(int i=0;i<bottom-top;i++)
		{
			for(int j=0;j<right-left;j++)
			{
				rHelpValue=GetRValue(pColorBuffer[i][j]);
				gHelpValue=GetGValue(pColorBuffer[i][j]);
				bHelpValue=GetBValue(pColorBuffer[i][j]);
				absResult=abs(rHelpValue-rValue)+abs(gHelpValue-gValue)+abs(bHelpValue-bValue);
				///////////////////判断第一个颜色值///////////////////////
				if(absResult<=help_simi)
				{
					bool IsTrue = false;
					int  c_col=0,c_xpoint=-1,c_ypoint=-1,c_num=0,colorValue=0,helpColor=0;
					wchar_t pointAndcolor[20]={0};
					//XMLString::trim(colorStr);
					int lenght =wcslen(colorStr);
					//int lenght = XMLString::stringLen(colorStr);
					while(c_col<lenght&&lenght!=0)
					{
						//////第一个x偏移坐标
						while(colorStr[c_col]!='|')
						{
							pointAndcolor[c_num] = colorStr[c_col];
							c_num ++;
							c_col++;
						}
						c_xpoint = ::wcstol(pointAndcolor,NULL,10);
						::memset(pointAndcolor,0,sizeof(wchar_t)*20);
						c_num = 0;
						c_col++;
						//////第二个Y偏移坐标
						while(colorStr[c_col]!='|')
						{
							pointAndcolor[c_num] = colorStr[c_col];
							c_num ++;
							c_col++;
						}
						c_ypoint = ::wcstol(pointAndcolor,NULL,10);
						::memset(pointAndcolor,0,sizeof(wchar_t)*20);
						c_num = 0;
						c_col++;

						//////第三个颜色值
						while(colorStr[c_col]!=','&&colorStr[c_col])
						{
							pointAndcolor[c_num] = colorStr[c_col];
							c_num ++;
							c_col++;
						}
						colorValue = ::wcstol(pointAndcolor,NULL,16);
						::memset(pointAndcolor,0,sizeof(wchar_t)*20);
						c_num = 0;
						c_col++;
						////////坐标点已经超出屏幕的范围 /////////
						if(i+c_ypoint>Height||j+c_xpoint>Width)
							break;
						helpColor = pColorBuffer[i+c_ypoint][j+c_xpoint];
						absResult=abs(GetRValue(helpColor)-GetRValue(colorValue))+abs(GetGValue(helpColor)-GetGValue(colorValue))+abs(GetBValue(helpColor)-GetBValue(colorValue));
						if(absResult<=help_simi)
						{
							IsTrue = true;
						}
						else
						{
							IsTrue = false;
							break;
						}
					}

					if(IsTrue)
					{
						x=j+left;
						y=i+top;
						goto end;
					}
				}
			}
		}
	}
	else
	{
		// // //////  单个颜色值的找色方法
		if(dir == 0)
		{
			/////////////////////0 从左到右 从上到下 方式查找///////////////
			int absResult = 0;
			for(int i=0;i<bottom-top;i++)
			{
				for(int j=0;j<right-left;j++)
				{
					rHelpValue=GetRValue(pColorBuffer[i][j]);
					gHelpValue=GetGValue(pColorBuffer[i][j]);
					bHelpValue=GetBValue(pColorBuffer[i][j]);

					absResult=abs(rHelpValue-rValue)+abs(gHelpValue-gValue)+abs(bHelpValue-bValue);

					if(absResult<=help_simi)
					{
						x=j+left;
						y=i+top;
						goto end;
					}
				}
			}
		}
		else if(dir == 1)
		{
			/////////////////////1 从中心到四周 螺旋方式查找///////////////
			int px = (right - left)/2 ;
			int py = (bottom - top)/2 ;

			int VisitCount=0;
			int count = 1;

			int ii =0;
			int absResult = 0;

			RUNTYPE runType = TurnUP;

			while(VisitCount<(right - left)*(bottom - top))
			{
				for(ii=0; ii<count; ii++)
				{
					if(px>=0 && px<right-left && py>=0 && py<bottom-top)
					{
						VisitCount++;   ///执行次数累加
						rHelpValue=GetRValue(pColorBuffer[py][px]);
						gHelpValue=GetGValue(pColorBuffer[py][px]);
						bHelpValue=GetBValue(pColorBuffer[py][px]);
						absResult =abs(rHelpValue-rValue)+abs(gHelpValue-gValue)+abs(bHelpValue-bValue);
						if(absResult<=help_simi)
						{	// 找到就退出循环
							x=px+left;
							y=py+top;
							goto end;
						}
					}
					//// 控制x,y变化
					switch(runType)
					{
					case TurnUP:
						py--;
						break;

					case TurnDOWN:
						py++;
						break;

					case TurnLEFT:
						px--;
						break;

					case TurnRIGHT:
						px++;
						break;
					}
				}  //endfor
				/////控制 螺旋方式流程的走向
				switch(runType)
				{
				case TurnUP:
					runType = TurnRIGHT;
					break;

				case TurnDOWN:
					runType = TurnLEFT;
					break;

				case TurnLEFT:
					runType = TurnUP;
					count ++;
					break;

				case TurnRIGHT:
					runType = TurnDOWN;
					count ++;
					break;
				}
			} ////endwhile
		}///end else if

		else if(dir == 2)
		{
			/////////////////////0 从右到左 从下到上 方式查找///////////////
			for(int i=bottom-top-1;i>=0;i--)
			{
				for(int j=right-left-1;j>=0;j--)
				{
					rHelpValue=GetRValue(pColorBuffer[i][j]);
					gHelpValue=GetGValue(pColorBuffer[i][j]);
					bHelpValue=GetBValue(pColorBuffer[i][j]);

					int absResult=abs(rHelpValue-rValue)+abs(gHelpValue-gValue)+abs(bHelpValue-bValue);

					if(absResult<=help_simi)
					{
						x=j+left;
						y=i+top;
						goto end;
					}
				}
			}
		}
	}
end:
	free(pBITMAPINFO);
	::DeleteObject(hBitmap);

	::DeleteDC(dc);
	::DeleteDC(tdc);

	for(int i=0;i<height;i++)
	{
		delete [] pColorBuffer[i];
		pColorBuffer[i]=NULL;
	}

	delete [] pColorBuffer;
	pColorBuffer=NULL;

	delete [] pBuffer;

	return true;
}
COLORREF MyFindPictureClass::RgbToBgb(__in COLORREF rgb)
{
	BYTE r;
	BYTE g;
	BYTE b;
	r = GetRValue(rgb);
	g = GetGValue(rgb);
	b = GetBValue(rgb);
	return r<<16 | g <<8 |b;
}

bool MyFindPictureClass::FindMultiColor(wchar_t *MultiColor,int i,int j,int Bottom,int right,int help_simi)//多点找色
{
	bool IsTrue=false;
	int  c_col=0,c_xpoint=-1,c_ypoint=-1,c_num=0,colorValue=0,helpColor=0;
	wchar_t pointAndcolor[20]={0};
	int lenght = wcslen(MultiColor);
	while(c_col<lenght&&lenght!=0)
	{
		//////第一个x偏移坐标
		while(MultiColor[c_col]!='|')
		{
			pointAndcolor[c_num] = MultiColor[c_col];
			c_num ++;
			c_col++;
		}
		c_xpoint = ::wcstol(pointAndcolor,NULL,10);
		::memset(pointAndcolor,0,sizeof(wchar_t)*20);
		c_num = 0;
		c_col++;
		//////第二个Y偏移坐标
		while(MultiColor[c_col]!='|')
		{
			pointAndcolor[c_num] = MultiColor[c_col];
			c_num ++;
			c_col++;
		}
		c_ypoint = ::wcstol(pointAndcolor,NULL,10);
		::memset(pointAndcolor,0,sizeof(wchar_t)*20);
		c_num = 0;
		c_col++;

		int coloroffsetValue=0;//记录偏色
		wchar_t coloroffsetstr[20]={0};//记录偏色字符串
		bool findoffset=false;
		bool isGetAnti=false;//是否取反颜色

		//////第三个颜色值
		while(MultiColor[c_col]!=','&&MultiColor[c_col])
		{
			if(c_num==0&&MultiColor[c_col]=='-')//第一个是否出现‘-’符号决定是否取反
			{
				isGetAnti=true;
			}
			if(MultiColor[c_col]=='-')
			{
				c_num=0;
				c_col++;
				findoffset=true;
			}
			if(!findoffset)//没有找到偏色前记录颜色值
				pointAndcolor[c_num] = MultiColor[c_col];
			else
				coloroffsetstr[c_num]=MultiColor[c_col];
			c_num ++;
			c_col++;
		}
		colorValue = ::wcstol(pointAndcolor,NULL,16);
		coloroffsetValue=::wcstol(coloroffsetstr,NULL,16);
		::memset(pointAndcolor,0,sizeof(wchar_t)*20);
		c_num = 0;
		c_col++;
		////////坐标点已经超出屏幕的范围 /////////
		//if(i+c_ypoint>=Height||j+c_xpoint>=Width)
		if(i+c_ypoint>=Bottom||j+c_xpoint>=right)
			break;

		BYTE rHelpValue=GetRValue(pWndBmpBuffer[i+c_ypoint][j+c_xpoint]);
		BYTE gHelpValue=GetGValue(pWndBmpBuffer[i+c_ypoint][j+c_xpoint]);
		BYTE bHelpValue=GetBValue(pWndBmpBuffer[i+c_ypoint][j+c_xpoint]);

		int BGRcolorValue=RgbToBgb(colorValue);
		BYTE rValue=GetRValue(BGRcolorValue);
		BYTE gValue=GetGValue(BGRcolorValue);
		BYTE bValue=GetBValue(BGRcolorValue);

		int BGRcoloroffsetValue=RgbToBgb(coloroffsetValue);
		BYTE colorOffR=GetRValue(BGRcoloroffsetValue);
		BYTE colorOffG=GetGValue(BGRcoloroffsetValue);
		BYTE colorOffB=GetBValue(BGRcoloroffsetValue);
		//helpColor = RgbToBgb(pWndBmpBuffer[i+c_ypoint][j+c_xpoint]);
		//absResult=abs(GetRValue(helpColor)-GetRValue(colorValue))+abs(GetGValue(helpColor)-GetGValue(colorValue))+abs(GetBValue(helpColor)-GetBValue(colorValue));
		//if(absResult<=help_simi)
		if(abs(rHelpValue-rValue)<=(colorOffR+help_simi) && abs(gHelpValue-gValue)<=(colorOffG+help_simi) && abs(bHelpValue-bValue)<=(colorOffB+help_simi) )
		{
			if(!isGetAnti)//判断是否取反颜色值，默认不是
				IsTrue = true;
			else
			{
				IsTrue = false;
				break;
			}
		}
		else
		{
			if(!isGetAnti)//判断是否取反颜色值，默认不是
			{
				IsTrue = false;
				break;
			}
			else
				IsTrue = true;

		}
	}

	return IsTrue;
}

/////// 后台颜色内存处理。。 提高速度//////////////////////
int MyFindPictureClass::processColor(PVOID pbuffer,HWND src_hWnd,int &color,int left,int top,int right,int bottom,double simi,int dir,long &x,long &y,DWORD (*ColorDataInfo)[2000],wchar_t *colorstr,wchar_t *retstring,wchar_t *MultiColor)
{

	int nret=-1;
	RECT rc;
	rc.bottom=bottom;
	rc.left=left;
	rc.right=right;
	rc.top=top;

	////////////////////////////四点的边界值判断////////////////
	{
		RECT rc;
		if(ColorDataInfo!=NULL)
			::GetClientRect(m_hwnd,&rc);
		else//gdi
			::GetClientRect(src_hWnd,&rc);

		if(left<0)
			left=0;
		if(left>=rc.right)
			left=rc.right-1;

		if(top<0)
			top=0;
		if(top>=rc.bottom)
			top=rc.bottom-1;

		if(right<left|right>=rc.right)
			right=rc.right-1;
		if(bottom<top|bottom>=rc.bottom)
			bottom=rc.bottom-1;
		
	}
	////////////////////////////////////////////////////////////
	if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())//normol找图
	{
		RECT rec;
		RECT clientrec;
		::GetClientRect(m_parenthwnd,&clientrec);
		int clienthight=clientrec.bottom-clientrec.top;
		int clientwide=clientrec.right-clientrec.left;
		::GetWindowRect(m_parenthwnd,&rec);
		POINT point;
		point.x=0;
		point.y=0;
		::ClientToScreen(m_parenthwnd,&point);
		m_Left=point.x;
		m_Top=point.y;

		//TS_GETCOLOR,取色
		if(color==0)
		{
			m_Left = 0;
			m_Top = 0;
			m_Right=rec.right;
			m_bottom=rec.bottom;
		}
		else//找色
		{
			m_Left=point.x;
			m_Top=point.y;
			m_Right=rec.right;
			m_bottom=rec.bottom;

			if(left<0)
				left=0;
			if(left >= clientwide)
				left = clientwide-1;

			if(top<0)
				top=0;
			if(top >= clienthight)
				top = clienthight-1;

			if(right >= clientwide)
				right=clientwide-1;

			if(bottom >= clienthight)
				bottom=clienthight-1;

			if(m_Left<0)
			{
				if((left+m_Left)<=0)//越界
					left=0;
				else
					left=m_Left+left;
			}
			else
				left=m_Left+left;

			if(m_Top>=0)
				top=m_Top+top;

			if(m_Right >= ::GetSystemMetrics(SM_CXSCREEN))
			{
				if((right+m_Left)>::GetSystemMetrics(SM_CXSCREEN))
					right=::GetSystemMetrics(SM_CXSCREEN)-1;
				else
					right=right+m_Left;
			}
			else
				right=right+m_Left;

			if(m_bottom >= ::GetSystemMetrics(SM_CYSCREEN))
			{
				if((bottom+m_Top)>=::GetSystemMetrics(SM_CYSCREEN))
					bottom=::GetSystemMetrics(SM_CYSCREEN)-1;
				else
					bottom=bottom+m_Top;
			}
			else
				bottom=bottom+m_Top;

			m_Left = left;
			m_Top = top;
			m_Right = right;
			m_bottom = bottom;
		}

	}
	else
	{
		m_Left = left;
		m_Top = top;
		m_Right = right;
		m_bottom = bottom;
	}
	rc.bottom=m_bottom;
	rc.left=m_Left;
	rc.right=m_Right;
	rc.top=m_Top;

	//TSRuntime::add_log( "m_Left:%d,m_Top:%d,m_Right:%d,m_bottom:%d",m_Left,m_Top,m_Right,m_bottom);
	if(ColorDataInfo!=NULL)
	{
		wndWidth=m_Right-m_Left;
		wndHeight=m_bottom-m_Top;
		//DX模式找字
		if(!getDXBitmap(pbuffer,ColorDataInfo))
		{
			return nret;
		}
	}	
	else
	{
		////GDI模式找字
		if(!getGDIBitmap(src_hWnd,rc))
		{
			return nret;
		}
	}

	//TS_GETCOLOR,取色
	if(color==0)
	{
		if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())//normal
		{
			POINT pt;
			pt.x=left;
			pt.y=top;
			::ClientToScreen(m_parenthwnd,&pt);
			color=pWndBmpBuffer[pt.y][pt.x];
		}
		else
			color=pWndBmpBuffer[top][left];

		//TSRuntime::add_log( "top:%d,left:%d",top,left);
		goto end;
	}
	


	//处理多个色偏
	int col=0;
	wchar_t colorBuffer[MAX_PATH][16];//存储多个字符串
	for(int i=0;i<MAX_PATH;i++)
	{
		memset(colorBuffer[i],0,16);
	}

	short nstr=0;//要查找色偏的个数  ,最多支持(MAX_PATH)个色偏
	while(*colorstr)
	{
		if(*colorstr!=L'|')
		{
			if(col>13)
				goto end;
			colorBuffer[nstr][col++]=*colorstr;
			colorstr++;
			continue;
		}
		else
		{
			if(col<1)
			{
				colorstr++;
				continue;
			}
			nstr++;
			col=0;
			colorstr++;
		}
	}
	if(col>=1)
		nstr++;

	ncolors=nstr;//记录下色偏个数

	//int t=::GetTickCount();
	for(int i=0;i<nstr;i++)
	{
		WCHAR colorR[4]={0};//字的颜色
		WCHAR colorG[4]={0};//字的颜色
		WCHAR colorB[4]={0};//字的颜色

		WCHAR coloroffR[4]={0};//偏色
		WCHAR coloroffG[4]={0};//偏色
		WCHAR coloroffB[4]={0};//偏色
		int count=0;
		int colorL=0;

		//StrTrimW(colorBuffer[i],L" ");//移除前后空格符

		while(colorBuffer[i][colorL]!=L'-')
		{
			if(count>6)	//判断是否是非法颜色值
				break;
			if(count<2)
				colorR[count]=colorBuffer[i][colorL++];
			else if(count>1&&count<4)
				colorG[count-2]=colorBuffer[i][colorL++];
			else if(count>3)
				colorB[count-4]=colorBuffer[i][colorL++];
			count++;

		}
		//将传入的RGB转换为BGR
		WCHAR colorBGR[16]={0};//字的颜色
		swprintf(colorBGR,L"%s%s%s",colorB,colorG,colorR);
		count=0;
		colorL++;

		while(colorBuffer[colorL]!=L'\0')
		{
			if(count>=6) //判断是否是非法色偏值
				break;
			if(count<2)
				coloroffR[count]=colorBuffer[i][colorL++];
			else if(count>1&&count<4)
				coloroffG[count-2]=colorBuffer[i][colorL++];
			else if(count>3)
				coloroffB[count-4]=colorBuffer[i][colorL++];
			count++;
		}

		//将传入的RGB转换为BGR
		WCHAR coloroffBGR[16]={0};//偏色
		swprintf(coloroffBGR,L"%s%s%s",coloroffB,coloroffG,coloroffR);

		strColors[i]=wcstol(colorBGR,NULL,16);//整形颜色值
		strColoroff[i]=wcstol(coloroffBGR,NULL,16);//整形偏色值
		m_colorOffR[i] = GetRValue(strColoroff[i]);
		m_colorOffG[i] = GetGValue(strColoroff[i]);
		m_colorOffB[i] = GetBValue(strColoroff[i]);

	}

	int help_simi=0;
	if(simi==0.5)
		help_simi=49;

	else if(simi==0.6)
		help_simi=38;

	else if(simi==0.7)
		help_simi=29;

	else if(simi==0.8)
		help_simi=18;

	else if(simi==0.9)
		help_simi=9;

	for(int n=0;n<ncolors;n++)
	{
		color=strColors[n];
		BYTE rValue=0,gValue=0,bValue=0;
		rValue=GetRValue(color);
		gValue=GetGValue(color);
		bValue=GetBValue(color);

		BYTE rHelpValue=0,gHelpValue=0,bHelpValue=0;

		if(MultiColor)//多点找色
		{
			/////////////////////0 从左到右 从上到下 方式查找///////////////
			int absResult = 0;
			if(dir == 0)
			{
				//TSRuntime::add_log("ncolors:%d",ncolors);
				for(int i=top;i<=bottom;i++)
				{
					for(int j=left;j<=right;j++)
					{
						rHelpValue=GetRValue(pWndBmpBuffer[i][j]);
						gHelpValue=GetGValue(pWndBmpBuffer[i][j]);
						bHelpValue=GetBValue(pWndBmpBuffer[i][j]);
						//int absResult=abs(rHelpValue-rValue-m_colorOffR[n])+abs(gHelpValue-gValue-m_colorOffG[n])+abs(bHelpValue-bValue-m_colorOffB[n]);
						if(abs(rHelpValue-rValue)<=(m_colorOffR[n]+help_simi) && abs(gHelpValue-gValue)<=(m_colorOffG[n]+help_simi) && abs(bHelpValue-bValue)<=(m_colorOffB[n]+help_simi) )
						{
							bool IsTrue = false;
							IsTrue=FindMultiColor(MultiColor,i,j,bottom,right,help_simi);
							if(IsTrue)
							{
								nret=1;
								x=j;
								y=i;
								if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
								{
									POINT pt;
									pt.x=j;
									pt.y=i;
									::ScreenToClient(m_parenthwnd,&pt);
									x=pt.x;
									y=pt.y;
								}
								if(retstring==NULL)
									goto end;
								else
								{
									int len=wcslen(retstring);
									if(len>1&&len<(MAX_PATH*4-16))
										swprintf(retstring,L"%s|%d,%d,%d",retstring,n,x,y);
									else if(len==0)
										swprintf(retstring,L"%d,%d,%d",n,x,y);
								}
							}
							else
								nret=0;

						}
					}
				}
			}
			else if(dir == 1)
			{
				/////////////////////1 从左到右,从下到上 方式查找///////////////
				for(int i=bottom;i>=top;i--)
				{
					for(int j=left;j<=right;j++)
					{
						rHelpValue=GetRValue(pWndBmpBuffer[i][j]);
						gHelpValue=GetGValue(pWndBmpBuffer[i][j]);
						bHelpValue=GetBValue(pWndBmpBuffer[i][j]);
						if(abs(rHelpValue-rValue)<=(m_colorOffR[n]+help_simi) && abs(gHelpValue-gValue)<=(m_colorOffG[n]+help_simi) && abs(bHelpValue-bValue)<=(m_colorOffB[n]+help_simi) )
						{
							bool IsTrue = false;
							IsTrue=FindMultiColor(MultiColor,i,j,bottom,right,help_simi);
							if(IsTrue)
							{
								nret=1;
								x=j;
								y=i;
								if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
								{
									POINT pt;
									pt.x=j;
									pt.y=i;
									::ScreenToClient(m_parenthwnd,&pt);
									x=pt.x;
									y=pt.y;
								}
								if(retstring==NULL)
									goto end;
								else
								{
									int len=wcslen(retstring);
									if(len>1&&len<(MAX_PATH*4-16))
										swprintf(retstring,L"%s|%d,%d,%d",retstring,n,x,y);
									else if(len==0)
										swprintf(retstring,L"%d,%d,%d",n,x,y);
								}
							}
							else
								nret=0;
						}
					}
				}
			}
			else if(dir == 2)
			{
				/////////////////////2: 从右到左,从上到下 方式查找///////////////
				for(int i=top;i<=bottom;i++)
				{
					for(int j=right;j>=left;j--)
					{
						rHelpValue=GetRValue(pWndBmpBuffer[i][j]);
						gHelpValue=GetGValue(pWndBmpBuffer[i][j]);
						bHelpValue=GetBValue(pWndBmpBuffer[i][j]);
						//int absResult=abs(rHelpValue-rValue-m_colorOffR[n])+abs(gHelpValue-gValue-m_colorOffG[n])+abs(bHelpValue-bValue-m_colorOffB[n]);
						//if(absResult<=help_simi)
						if(abs(rHelpValue-rValue)<=(m_colorOffR[n]+help_simi) && abs(gHelpValue-gValue)<=(m_colorOffG[n]+help_simi) && abs(bHelpValue-bValue)<=(m_colorOffB[n]+help_simi) )
						{
							bool IsTrue = false;
							IsTrue=FindMultiColor(MultiColor,i,j,bottom,right,help_simi);
							if(IsTrue)
							{
								nret=1;
								x=j;
								y=i;
								if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
								{
									POINT pt;
									pt.x=j;
									pt.y=i;
									::ScreenToClient(m_parenthwnd,&pt);
									x=pt.x;
									y=pt.y;
								}
								if(retstring==NULL)
									goto end;
								else
								{
									int len=wcslen(retstring);
									if(len>1&&len<(MAX_PATH*4-16))
										swprintf(retstring,L"%s|%d,%d,%d",retstring,n,x,y);
									else if(len==0)
										swprintf(retstring,L"%d,%d,%d",n,x,y);
								}
							}
							else
								nret=0;
						}
					}
				}
			}
			else if(dir == 3)
			{
				/////////////////////3: 从右到左,从下到上 方式查找///////////////
				for(int i=bottom;i>=top;i--)
				{
					for(int j=right;j>=left;j--)
					{

						rHelpValue=GetRValue(pWndBmpBuffer[i][j]);
						gHelpValue=GetGValue(pWndBmpBuffer[i][j]);
						bHelpValue=GetBValue(pWndBmpBuffer[i][j]);
						//int absResult=abs(rHelpValue-rValue-m_colorOffR[n])+abs(gHelpValue-gValue-m_colorOffG[n])+abs(bHelpValue-bValue-m_colorOffB[n]);
						//if(absResult<=help_simi)
						if(abs(rHelpValue-rValue)<=(m_colorOffR[n]+help_simi) && abs(gHelpValue-gValue)<=(m_colorOffG[n]+help_simi) && abs(bHelpValue-bValue)<=(m_colorOffB[n]+help_simi) )
						{
							bool IsTrue = false;
							IsTrue=FindMultiColor(MultiColor,i,j,bottom,right,help_simi);
							if(IsTrue)
							{
								nret=1;
								x=j;
								y=i;
								if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
								{
									POINT pt;
									pt.x=j;
									pt.y=i;
									::ScreenToClient(m_parenthwnd,&pt);
									x=pt.x;
									y=pt.y;
								}
								if(retstring==NULL)
									goto end;
								else
								{
									int len=wcslen(retstring);
									if(len>1&&len<(MAX_PATH*4-16))
										swprintf(retstring,L"%s|%d,%d,%d",retstring,n,x,y);
									else if(len==0)
										swprintf(retstring,L"%d,%d,%d",n,x,y);
								}
							}
							else
								nret=0;
						}
					}
				}
			}
		}
		else//正常找色
		{
			if(dir == 0)
			{
				//TSRuntime::add_log("ncolors:%d",ncolors);
				/////////////////////0 从左到右 从上到下 方式查找///////////////
				for(int i=top;i<=bottom;i++)
				{
					for(int j=left;j<=right;j++)
					{
						rHelpValue=GetRValue(pWndBmpBuffer[i][j]);
						gHelpValue=GetGValue(pWndBmpBuffer[i][j]);
						bHelpValue=GetBValue(pWndBmpBuffer[i][j]);
						//int absResult=abs(rHelpValue-rValue-m_colorOffR[n])+abs(gHelpValue-gValue-m_colorOffG[n])+abs(bHelpValue-bValue-m_colorOffB[n]);
						if(abs(rHelpValue-rValue)<=(m_colorOffR[n]+help_simi) && abs(gHelpValue-gValue)<=(m_colorOffG[n]+help_simi) && abs(bHelpValue-bValue)<=(m_colorOffB[n]+help_simi) )
						{
							nret=n;
							x=j;
							y=i;
							if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
							{
								POINT pt;
								pt.x=j;
								pt.y=i;
								::ScreenToClient(m_parenthwnd,&pt);
								x=pt.x;
								y=pt.y;
							}
							if(retstring==NULL)
								goto end;
							else
							{
								int len=wcslen(retstring);
								if(len>1&&len<(MAX_PATH*4-16))
									swprintf(retstring,L"%s|%d,%d,%d",retstring,n,x,y);
								else if(len==0)
									swprintf(retstring,L"%d,%d,%d",n,x,y);
							}

						}
					}
				}
			}
			else if(dir == 1)
			{
				/////////////////////1 从左到右,从下到上 方式查找///////////////
				for(int i=bottom;i>=top;i--)
				{
					for(int j=left;j<=right;j++)
					{
						rHelpValue=GetRValue(pWndBmpBuffer[i][j]);
						gHelpValue=GetGValue(pWndBmpBuffer[i][j]);
						bHelpValue=GetBValue(pWndBmpBuffer[i][j]);
						//int absResult=abs(rHelpValue-rValue-m_colorOffR[n])+abs(gHelpValue-gValue-m_colorOffG[n])+abs(bHelpValue-bValue-m_colorOffB[n]);
						//if(absResult<=help_simi)
						if(abs(rHelpValue-rValue)<=(m_colorOffR[n]+help_simi) && abs(gHelpValue-gValue)<=(m_colorOffG[n]+help_simi) && abs(bHelpValue-bValue)<=(m_colorOffB[n]+help_simi) )
						{
							nret=n;
							x=j;
							y=i;
							if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
							{
								POINT pt;
								pt.x=j;
								pt.y=i;
								::ScreenToClient(m_parenthwnd,&pt);
								x=pt.x;
								y=pt.y;
							}
							if(retstring==NULL)
								goto end;
							else
							{
								int len=wcslen(retstring);
								if(len>1&&len<(MAX_PATH*4-16))
									swprintf(retstring,L"%s|%d,%d,%d",retstring,n,x,y);
								else if(len==0)
									swprintf(retstring,L"%d,%d,%d",n,x,y);
							}
						}
					}
				}
			}
			else if(dir == 2)
			{
				/////////////////////2: 从右到左,从上到下 方式查找///////////////
				for(int i=top;i<=bottom;i++)
				{
					for(int j=right;j>=left;j--)
					{
						rHelpValue=GetRValue(pWndBmpBuffer[i][j]);
						gHelpValue=GetGValue(pWndBmpBuffer[i][j]);
						bHelpValue=GetBValue(pWndBmpBuffer[i][j]);
						//int absResult=abs(rHelpValue-rValue-m_colorOffR[n])+abs(gHelpValue-gValue-m_colorOffG[n])+abs(bHelpValue-bValue-m_colorOffB[n]);
						//if(absResult<=help_simi)
						if(abs(rHelpValue-rValue)<=(m_colorOffR[n]+help_simi) && abs(gHelpValue-gValue)<=(m_colorOffG[n]+help_simi) && abs(bHelpValue-bValue)<=(m_colorOffB[n]+help_simi) )
						{
							nret=n;
							x=j;
							y=i;
							if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
							{
								POINT pt;
								pt.x=j;
								pt.y=i;
								::ScreenToClient(m_parenthwnd,&pt);
								x=pt.x;
								y=pt.y;
							}
							if(retstring==NULL)
								goto end;
							else
							{
								int len=wcslen(retstring);
								if(len>1&&len<(MAX_PATH*4-16))
									swprintf(retstring,L"%s|%d,%d,%d",retstring,n,x,y);
								else if(len==0)
									swprintf(retstring,L"%d,%d,%d",n,x,y);
							}
						}
					}
				}
			}
			else if(dir == 3)
			{
				/////////////////////3: 从右到左,从下到上 方式查找///////////////
				for(int i=bottom;i>=top;i--)
				{
					for(int j=right;j>=left;j--)
					{

						rHelpValue=GetRValue(pWndBmpBuffer[i][j]);
						gHelpValue=GetGValue(pWndBmpBuffer[i][j]);
						bHelpValue=GetBValue(pWndBmpBuffer[i][j]);
						//int absResult=abs(rHelpValue-rValue-m_colorOffR[n])+abs(gHelpValue-gValue-m_colorOffG[n])+abs(bHelpValue-bValue-m_colorOffB[n]);
						//if(absResult<=help_simi)
						if(abs(rHelpValue-rValue)<=(m_colorOffR[n]+help_simi) && abs(gHelpValue-gValue)<=(m_colorOffG[n]+help_simi) && abs(bHelpValue-bValue)<=(m_colorOffB[n]+help_simi) )
						{
							nret=n;
							x=j;
							y=i;
							if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
							{
								POINT pt;
								pt.x=j;
								pt.y=i;
								::ScreenToClient(m_parenthwnd,&pt);
								x=pt.x;
								y=pt.y;
							}
							if(retstring==NULL)
								goto end;
							else
							{
								int len=wcslen(retstring);
								if(len>1&&len<(MAX_PATH*4-16))
									swprintf(retstring,L"%s|%d,%d,%d",retstring,n,x,y);
								else if(len==0)
									swprintf(retstring,L"%d,%d,%d",n,x,y);
							}
						}
					}
				}
			}
			else if(dir == 4)
			{
				/////////////////////4 从中心到四周 螺旋方式查找///////////////
				int px = (right - left)/2 + left;
				int py = (bottom - top)/2 + top;

				int VisitCount=0;
				int count = 1;

				int ii =0;
				int absResult = 0;

				RUNTYPE runType = TurnUP;

				while(VisitCount<(right - left)*(bottom - top))
				{
					for(ii=0; ii<count; ii++)
					{
						if(px>=left && px<right && py>=top && py<bottom)
						{
							VisitCount++;   ///执行次数累加
							rHelpValue=GetRValue(pWndBmpBuffer[py][px]);
							gHelpValue=GetGValue(pWndBmpBuffer[py][px]);
							bHelpValue=GetBValue(pWndBmpBuffer[py][px]);
							//absResult=abs(rHelpValue-rValue-m_colorOffR[n])+abs(gHelpValue-gValue-m_colorOffG[n])+abs(bHelpValue-bValue-m_colorOffB[n]);
							//absResult =abs(rHelpValue-rValue)+abs(gHelpValue-gValue)+abs(bHelpValue-bValue);
							//if(absResult<=help_simi)
							if(abs(rHelpValue-rValue)<=(m_colorOffR[n]+help_simi) && abs(gHelpValue-gValue)<=(m_colorOffG[n]+help_simi) && abs(bHelpValue-bValue)<=(m_colorOffB[n]+help_simi) )
							{	// 找到就退出循环
								nret=n;
								x=px;
								y=py;
								if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
								{
									POINT pt;
									pt.x=x;
									pt.y=y;
									::ScreenToClient(m_parenthwnd,&pt);
									x=pt.x;
									y=pt.y;
								}
								if(retstring==NULL)
									goto end;
								else
								{
									int len=wcslen(retstring);
									if(len>1&&len<(MAX_PATH*4-16))
										swprintf(retstring,L"%s|%d,%d,%d",retstring,n,x,y);
									else if(len==0)
										swprintf(retstring,L"%d,%d,%d",n,x,y);
								}
							}
						}
						//// 控制x,y变化
						switch(runType)
						{
						case TurnUP:
							py--;
							break;

						case TurnDOWN:
							py++;
							break;

						case TurnLEFT:
							px--;
							break;

						case TurnRIGHT:
							px++;
							break;
						}
					}  //endfor
					/////控制 螺旋方式流程的走向
					switch(runType)
					{
					case TurnUP:
						runType = TurnRIGHT;
						break;

					case TurnDOWN:
						runType = TurnLEFT;
						break;

					case TurnLEFT:
						runType = TurnUP;
						count ++;
						break;

					case TurnRIGHT:
						runType = TurnDOWN;
						count ++;
						break;
					}
				} ////endwhile
			}
			else if(dir == 5)
			{
				/////////////////////从上到下,从左到右 方式查找///////////////
				for(int j=left;j<=right;j++)
				{
					for(int i=top;i<=bottom;i++)
					{
						rHelpValue=GetRValue(pWndBmpBuffer[i][j]);
						gHelpValue=GetGValue(pWndBmpBuffer[i][j]);
						bHelpValue=GetBValue(pWndBmpBuffer[i][j]);
						//int absResult=abs(rHelpValue-rValue-m_colorOffR[n])+abs(gHelpValue-gValue-m_colorOffG[n])+abs(bHelpValue-bValue-m_colorOffB[n]);
						//if(absResult<=help_simi)
						if(abs(rHelpValue-rValue)<=(m_colorOffR[n]+help_simi) && abs(gHelpValue-gValue)<=(m_colorOffG[n]+help_simi) && abs(bHelpValue-bValue)<=(m_colorOffB[n]+help_simi) )
						{
							nret=n;
							x=j;
							y=i;
							if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
							{
								POINT pt;
								pt.x=j;
								pt.y=i;
								::ScreenToClient(m_parenthwnd,&pt);
								x=pt.x;
								y=pt.y;
							}
							if(retstring==NULL)
								goto end;
							else
							{
								int len=wcslen(retstring);
								if(len>1&&len<(MAX_PATH*4-16))
									swprintf(retstring,L"%s|%d,%d,%d",retstring,n,x,y);
								else if(len==0)
									swprintf(retstring,L"%d,%d,%d",n,x,y);
							}
						}
					}
				}
			}
			else if(dir == 6)
			{
				/////////////////////6: 从上到下,从右到左 方式查找///////////////
				for(int j=right;j>=left;j--)
				{
					for(int i=top;i<=bottom;i++)
					{
						rHelpValue=GetRValue(pWndBmpBuffer[i][j]);
						gHelpValue=GetGValue(pWndBmpBuffer[i][j]);
						bHelpValue=GetBValue(pWndBmpBuffer[i][j]);
						//int absResult=abs(rHelpValue-rValue-m_colorOffR[n])+abs(gHelpValue-gValue-m_colorOffG[n])+abs(bHelpValue-bValue-m_colorOffB[n]);
						//if(absResult<=help_simi)
						if(abs(rHelpValue-rValue)<=(m_colorOffR[n]+help_simi) && abs(gHelpValue-gValue)<=(m_colorOffG[n]+help_simi) && abs(bHelpValue-bValue)<=(m_colorOffB[n]+help_simi) )
						{
							nret=n;
							x=j;
							y=i;
							if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
							{
								POINT pt;
								pt.x=j;
								pt.y=i;
								::ScreenToClient(m_parenthwnd,&pt);
								x=pt.x;
								y=pt.y;
							}
							if(retstring==NULL)
								goto end;
							else
							{
								int len=wcslen(retstring);
								if(len>1&&len<(MAX_PATH*4-16))
									swprintf(retstring,L"%s|%d,%d,%d",retstring,n,x,y);
								else if(len==0)
									swprintf(retstring,L"%d,%d,%d",n,x,y);
							}
						}
					}
				}
			}
			else if(dir == 7)
			{
				/////////////////////7: 从下到上,从左到右 方式查找///////////////
				for(int j=left;j<=right;j++)
				{
					for(int i=bottom;i>=top;i--)
					{
						rHelpValue=GetRValue(pWndBmpBuffer[i][j]);
						gHelpValue=GetGValue(pWndBmpBuffer[i][j]);
						bHelpValue=GetBValue(pWndBmpBuffer[i][j]);
						//int absResult=abs(rHelpValue-rValue-m_colorOffR[n])+abs(gHelpValue-gValue-m_colorOffG[n])+abs(bHelpValue-bValue-m_colorOffB[n]);
						//if(absResult<=help_simi)
						if(abs(rHelpValue-rValue)<=(m_colorOffR[n]+help_simi) && abs(gHelpValue-gValue)<=(m_colorOffG[n]+help_simi) && abs(bHelpValue-bValue)<=(m_colorOffB[n]+help_simi) )
						{
							nret=n;
							x=j;
							y=i;
							if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
							{
								POINT pt;
								pt.x=j;
								pt.y=i;
								::ScreenToClient(m_parenthwnd,&pt);
								x=pt.x;
								y=pt.y;
							}
							if(retstring==NULL)
								goto end;
							else
							{
								int len=wcslen(retstring);
								if(len>1&&len<(MAX_PATH*4-16))
									swprintf(retstring,L"%s|%d,%d,%d",retstring,n,x,y);
								else if(len==0)
									swprintf(retstring,L"%d,%d,%d",n,x,y);
							}
						}
					}
				}
			}
			else if(dir == 8)
			{
				/////////////////////8: 从下到上,从右到左 方式查找///////////////
				for(int j=right;j>=left;j--)
				{
					for(int i=bottom;i>=top;i--)
					{
						rHelpValue=GetRValue(pWndBmpBuffer[i][j]);
						gHelpValue=GetGValue(pWndBmpBuffer[i][j]);
						bHelpValue=GetBValue(pWndBmpBuffer[i][j]);
						//int absResult=abs(rHelpValue-rValue-m_colorOffR[n])+abs(gHelpValue-gValue-m_colorOffG[n])+abs(bHelpValue-bValue-m_colorOffB[n]);
						//if(absResult<=help_simi)
						if(abs(rHelpValue-rValue)<=(m_colorOffR[n]+help_simi) && abs(gHelpValue-gValue)<=(m_colorOffG[n]+help_simi) && abs(bHelpValue-bValue)<=(m_colorOffB[n]+help_simi) )
						{
							nret=n;
							x=j;
							y=i;
							if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())
							{
								POINT pt;
								pt.x=j;
								pt.y=i;
								::ScreenToClient(m_parenthwnd,&pt);
								x=pt.x;
								y=pt.y;
							}
							if(retstring==NULL)
								goto end;
							else
							{
								int len=wcslen(retstring);
								if(len>1&&len<(MAX_PATH*4-16))
									swprintf(retstring,L"%s|%d,%d,%d",retstring,n,x,y);
								else if(len==0)
									swprintf(retstring,L"%d,%d,%d",n,x,y);
							}
						}
					}
				}
			}
		}
	}

end:

	return nret;
}

DWORD  MyFindPictureClass::GetBkMouseShape(HCURSOR bkmouseshape)//默认bkmouseshape为空时为前台模式, bkmouseshape有值的时候是后台模式
{

	HDC dDC = CreateDC(_T("DISPLAY"),NULL,NULL,NULL);
	HDC hDC = CreateCompatibleDC(dDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(dDC, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON));
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hDC, hBitmap);

	CURSORINFO pci;
	pci.cbSize=sizeof(CURSORINFO);
	pci.flags=0;
	pci.hCursor=0;
	if(bkmouseshape==NULL)	 //前台模式
	{

		if(!GetCursorInfo(&pci))
		{
			return 0;
		}
		DrawIcon(hDC,0,0,pci.hCursor);
	}
	else//后台模式
	{
		DrawIcon(hDC,0,0,bkmouseshape);	
	}

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
	int n=0;//位图深度

	////////////////////////////////////////////////////////////////////////////////////////
	//HDC hDC1 = ::GetWindowDC(::GetDesktopWindow());
	//COLORREF helpcolor=0;
	//int pitch=width%2;
	////int pitch=Pitch/4;
	//for(int i=0;i<height;i++)
	//	{
	//	int realPitch=i*pitch;
	//	for(int j=0;j<width;j++)
	//		{
	//		//UCHAR b=tpData24[(i*wndWidth+j)*3+realPitch];
	//		//UCHAR g=tpData24[(i*wndWidth+j)*3+1+realPitch];
	//		//UCHAR r=tpData24[(i*wndWidth+j)*3+2+realPitch];
	//		UCHAR b=bmpDate[(i*width+j)*2+realPitch];
	//		UCHAR g=bmpDate[(i*width+j)*2+1+realPitch];
	//		UCHAR r=bmpDate[(i*width+j)*2+2+realPitch];
	//		helpcolor=RGB(r,g,b);
	//		::SetPixel(hDC1,width-i-1,j,helpcolor);
	//		}
	//	} 
	//////////////////////////////////////////////////////////////////////////////////////

	BYTE  b0 = bmpDate[0];
	BYTE  g0 = bmpDate[1];
	BYTE  r0 = bmpDate[2];

	if(Bitmap.bmBitsPixel==32)
		n=4;
	else if(Bitmap.bmBitsPixel==24)
		n=3;
	else if(Bitmap.bmBitsPixel==16)
		n=2;
	else if(Bitmap.bmBitsPixel==8)
		n=1;

	//TSRuntime::add_log( "Bitmap.bmBitsPixel:%d,bmpDate:%x",Bitmap.bmBitsPixel,bmpDate);
	if(n!=0)
	{
		int pitch=width%n;
		for(int i=0;i<height;i++)
		{
			int realPitch=i*pitch;
			for(int j=0;j<width;j++)
			{
				UCHAR b=bmpDate[(i*width+j)*n+realPitch];
				UCHAR g=bmpDate[(i*width+j)*n+1+realPitch];
				UCHAR r=bmpDate[(i*width+j)*n+2+realPitch];
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

	return helpnum;
}


//************************************//
//函数名:hextobyte  
//说明:将十六进制字符串转换成二进制字符串
//	参数:hexStr 十六进制字符串
//	参数:byteStr 二进制字符串
//
//************************************//
void MyFindPictureClass::hextobyte(char * hexStr,char * byteStr)
{
	int i=0; 
	while(hexStr[i]!='\0')
	{

		switch(hexStr[i])
		{
		case '0':
			sprintf(byteStr,"%s%s",byteStr,"0000");
			break;
		case '1':
			sprintf(byteStr,"%s%s",byteStr,"0001");
			break;
		case '2':
			sprintf(byteStr,"%s%s",byteStr,"0010");
			break;
		case '3':
			sprintf(byteStr,"%s%s",byteStr,"0011");
			break;
		case '4':
			sprintf(byteStr,"%s%s",byteStr,"0100");
			break;
		case '5':
			sprintf(byteStr,"%s%s",byteStr,"0101");
			break;
		case '6':
			sprintf(byteStr,"%s%s",byteStr,"0110");
			break;
		case '7':
			sprintf(byteStr,"%s%s",byteStr,"0111");
			break;
		case '8':
			sprintf(byteStr,"%s%s",byteStr,"1000");
			break;
		case '9':
			sprintf(byteStr,"%s%s",byteStr,"1001");
			break;
		case 'A':
			sprintf(byteStr,"%s%s",byteStr,"1010");
			break;
		case 'B':
			sprintf(byteStr,"%s%s",byteStr,"1011");
			break;
		case 'C':
			sprintf(byteStr,"%s%s",byteStr,"1100");
			break;
		case 'D':
			sprintf(byteStr,"%s%s",byteStr,"1101");
			break;
		case 'E':
			sprintf(byteStr,"%s%s",byteStr,"1110");
			break;
		case 'F':
			sprintf(byteStr,"%s%s",byteStr,"1111");
			break;
		}
		i++;
	}

}


//************************************//
//函数名:byteTopic  
//说明:把字库信息还原成有色图像
//	参数:hexStr 十六进制字符串
//	参数:color 颜色
//
//************************************//
//void MyFindPictureClass::byteTopic(short strvalue[MAX_PATH],int strwide,COLORREF pLoadstrBuffer[11][MAX_PATH-10])  //MAX_PATH
void MyFindPictureClass::byteTopic(short strvalue[MAX_PATH],int strwide)  //MAX_PATH
{
	//char bytestr[1024*2]={0};
	//memset(TSRuntime::bytestr,0,strwide*11+1);
	memset(TSRuntime::bytestr,0,strwide*11+11);
	for(int n=0;n<strwide;n++)
	{
		char strbyte[12]={0};
		itoa(strvalue[n],strbyte,2);
		int len=strlen(strbyte);
		if(len!=11)
		{
			for(int i=0;i<(11-len);i++)//在字符串前面补齐0
			{
				char Tstrbyte[12]={0};
				sprintf(Tstrbyte,"%s%s","0",strbyte);
				sprintf(strbyte,"%s",Tstrbyte);
			}
		}
		sprintf(TSRuntime::bytestr,"%s%s",TSRuntime::bytestr,strbyte);   //整形还原二进制字符串
	}
	int n=4-(11*strwide)%4;
	if(n==1)
		sprintf(TSRuntime::bytestr,"%s%s",TSRuntime::bytestr,"0");
	else if(n==2)
		sprintf(TSRuntime::bytestr,"%s%s",TSRuntime::bytestr,"00");
	else if(n==3)
		sprintf(TSRuntime::bytestr,"%s%s",TSRuntime::bytestr,"000");

	int hhh=0;
	int tempH=11;
	//正常视觉图
	for (int w=0;w<strwide;w++)
	{

		for(int h=0;h<tempH;h++)
		{
			switch(TSRuntime::bytestr[hhh++])
			{
			case '0'://底色
				TSRuntime::pLoadstrBuffer[h][w]=RGB(0,0,0);
				break;
			case '1'://前景色
				TSRuntime::pLoadstrBuffer[h][w]=RGB(255,255,255);
				break;

			}
		}
	}

}

// 字库信息转二进制
void MyFindPictureClass::dicthextobyte(char * hexStr,char * byteStr,int &hexlen,short &nstrinfo)
{
	int i=0; 
	while(hexStr[i]!='\0')
	{
		//取字库点阵信息的2/3进行匹
		switch(hexStr[i])
		{
		case '0':
			{
				sprintf(byteStr,"%s%s",byteStr,"0000");
			}
			break;
		case '1':
			{
				sprintf(byteStr,"%s%s",byteStr,"0001");
				nstrinfo++;
			}
			break;
		case '2':
			{
				sprintf(byteStr,"%s%s",byteStr,"0010");
				nstrinfo++;
			}
			break;
		case '3':
			{
				sprintf(byteStr,"%s%s",byteStr,"0011");
				nstrinfo=nstrinfo+2;
			}
			break;
		case '4':
			{
				sprintf(byteStr,"%s%s",byteStr,"0100");
				nstrinfo++;
			}
			break;
		case '5':
			{
				sprintf(byteStr,"%s%s",byteStr,"0101");
				nstrinfo=nstrinfo+2;
			}
			break;
		case '6':
			{
				sprintf(byteStr,"%s%s",byteStr,"0110");
				nstrinfo=nstrinfo+2;
			}
			break;
		case '7':
			{
				sprintf(byteStr,"%s%s",byteStr,"0111");
				nstrinfo=nstrinfo+3;
			}
			break;
		case '8':
			{
				sprintf(byteStr,"%s%s",byteStr,"1000");
				nstrinfo++;
			}
			break;
		case '9':
			{
				sprintf(byteStr,"%s%s",byteStr,"1001");
				nstrinfo=nstrinfo+2;
			}
			break;
		case 'A':
			{
				sprintf(byteStr,"%s%s",byteStr,"1010");
				nstrinfo=nstrinfo+2;
			}
			break;
		case 'B':
			{
				sprintf(byteStr,"%s%s",byteStr,"1011");
				nstrinfo=nstrinfo+3;
			}
			break;
		case 'C':
			{
				sprintf(byteStr,"%s%s",byteStr,"1100");
				nstrinfo=nstrinfo+2;
			}
			break;
		case 'D':
			{
				sprintf(byteStr,"%s%s",byteStr,"1101");
				nstrinfo=nstrinfo+3;
			}
			break;
		case 'E':
			{
				sprintf(byteStr,"%s%s",byteStr,"1110");
				nstrinfo=nstrinfo+3;
			}
			break;
		case 'F':
			{
				sprintf(byteStr,"%s%s",byteStr,"1111");
				nstrinfo=nstrinfo+4;
			}
			break;
		}
		i++;
	}
	hexlen=(i+1)*4;//得到二进制的长度
}


bool MyFindPictureClass::comparaStr(short pLoadstrBufferxy[MAX_PATH],int strwide,int strhight,int &RetX,int &RetY,int startX,int startY,float simi,int nstrcount)
{


	return true;
}



bool MyFindPictureClass::GetStringInfo()//获取位置点阵信息
{


	return true;

}

bool MyFindPictureClass::MyUseDict(int Useindex,pMyDictInfo *MyDictCount,int &NowUsingDictIndex)	   //使用字库
{

	if(Useindex>=0&&Useindex<20)
	{
		//if(TSRuntime::MyDictCount[Useindex]!=NULL)
		if(MyDictCount[Useindex]!=NULL)
		{
			//TSRuntime::NowUsingDictIndex=Useindex;
			NowUsingDictIndex=Useindex;
			return true;
		}
	}

	return false;
}


bool MyFindPictureClass::MyDeleteDict(int deleteindex,pMyDictInfo *MyDictCount,int *nMyDictCountsize,int &NowUsingDictIndex)	//删除字库
{
	//如果不是注入模式,直接使用静态类

	if(deleteindex>=0&&deleteindex<20)
	{
		//if(TSRuntime::MyDictCount[deleteindex]!=NULL)
		//	{
		//		TSRuntime::nMyDictCountsize[deleteindex]=0;
		//		TSRuntime::nMaxStrHight[deleteindex]=0;
		//		delete [] TSRuntime::MyDictCount[deleteindex];
		//		TSRuntime::MyDictCount[deleteindex]=NULL;
		//		return true;
		//	}
		if(MyDictCount[deleteindex]!=NULL)
		{
			nMyDictCountsize[deleteindex]=0;
			TSRuntime::nMaxStrHight[deleteindex]=0;
			delete [] MyDictCount[deleteindex];
			MyDictCount[deleteindex]=NULL;
			return true;
		}
	}

	return false;
}

int MyFindPictureClass::FindStrFast(PVOID pbuffer,HWND hwnd,int left, int top, int right, int bottom, wchar_t* strings, wchar_t* color, double simi, long &x,long &y,pMyDictInfo *MyDictCount,int *nMyDictCountsize,int NowUsingDictIndex,DWORD (*ColorDataInfo)[2000],wchar_t *retstring,int type)
{
	wchar_t  retstr[MAX_PATH*100]={0};
	x=-1;
	y=-1;
	int index = 0;
	int retIndex=-1;
	bool isFind=false;
	//得到当前字库的下标
	usingindex=NowUsingDictIndex;
	//得到当前字库个数
	dictindex=nMyDictCountsize[usingindex];

	if(dictindex<=0)  //字库个数==0就退出
		return isFind;

	//处理多个色偏
	int col=0;
	wchar_t colorBuffer[MAX_PATH][16];//存储多个字符串
	for(int i=0;i<MAX_PATH;i++)
	{
		memset(colorBuffer[i],0,16);
	}

	short nstr=0;//要查找色偏的个数  ,最多支持(MAX_PATH)个色偏
	while(*color)
	{
		if(*color!=L'|')
		{
			if(col>13)
				return retIndex;
			colorBuffer[nstr][col++]=*color;
			color++;
			continue;
		}
		else
		{
			if(col<1)
			{
				color++;
				continue;
			}
			nstr++;
			col=0;
			color++;
		}
	}
	if(col>=1)
		nstr++;

	ncolors=nstr;//记录下色偏个数

	//int t=::GetTickCount();
	for(int i=0;i<nstr;i++)
	{
		WCHAR colorR[4]={0};//字的颜色
		WCHAR colorG[4]={0};//字的颜色
		WCHAR colorB[4]={0};//字的颜色

		WCHAR coloroffR[4]={0};//偏色
		WCHAR coloroffG[4]={0};//偏色
		WCHAR coloroffB[4]={0};//偏色
		int count=0;
		int colorL=0;

		//StrTrimW(colorBuffer[i],L" ");//移除前后空格符

		while(colorBuffer[i][colorL]!=L'-')
		{
			if(count>6)	//判断是否是非法颜色值
				return retIndex;
			if(count<2)
				colorR[count]=colorBuffer[i][colorL++];
			else if(count>1&&count<4)
				colorG[count-2]=colorBuffer[i][colorL++];
			else if(count>3)
				colorB[count-4]=colorBuffer[i][colorL++];
			count++;

		}
		//将传入的RGB转换为BGR
		WCHAR colorBGR[16]={0};//字的颜色
		swprintf(colorBGR,L"%s%s%s",colorB,colorG,colorR);
		count=0;
		colorL++;

		while(colorBuffer[colorL]!=L'\0')
		{
			if(count>=6) //判断是否是非法色偏值
				break;
			if(count<2)
				coloroffR[count]=colorBuffer[i][colorL++];
			else if(count>1&&count<4)
				coloroffG[count-2]=colorBuffer[i][colorL++];
			else if(count>3)
				coloroffB[count-4]=colorBuffer[i][colorL++];
			count++;
		}

		//将传入的RGB转换为BGR
		WCHAR coloroffBGR[16]={0};//偏色
		swprintf(coloroffBGR,L"%s%s%s",coloroffB,coloroffG,coloroffR);

		strColors[i]=wcstol(colorBGR,NULL,16);//整形颜色值
		strColoroff[i]=wcstol(coloroffBGR,NULL,16);//整形偏色值
		m_colorOffR[i] = GetRValue(strColoroff[i]);
		m_colorOffG[i] = GetGValue(strColoroff[i]);
		m_colorOffB[i] = GetBValue(strColoroff[i]);
	}

	//处理查找多个字
	col=0;
	short stringslen=0;
	wchar_t StrBuffer[MAX_PATH][16];//存储多个字符串
	for(int i=0;i<MAX_PATH;i++)
	{
		memset(StrBuffer[i],0,16);
	}
	nstr=0;//要查找字的个数  ,最多支持(MAX_PATH)个字
	while(*strings)
	{
		if(*strings!=L'|')
		{
			StrBuffer[nstr][col++]=*strings;
			strings++;
			continue;
			stringslen++;
		}
		else
		{
			if(col<1)
			{
				strings++;
				continue;
			}
			nstr++;
			col=0;
			strings++;
			stringslen++;
		}
	}
	if(col>=1)
		nstr++;

	//TSRuntime::add_log("FindStrFast:重新拼接要查找的字符串-start");
	//处理查找多个字
	//memset(strings,0,wcslen(strings)*sizeof(wchar_t));
	//memset(strings,0,stringslen);
	wchar_t findstrs[MAX_PATH*100]={0};
	for(int i=0;i<nstr;i++)
	{
		swprintf(findstrs,L"%s%s",findstrs,StrBuffer[i]);//重新拼接要查找的字符串
	}

	short slen=wcslen(strings);//得到要查找字符串的长度
	int nstringlen=0;
	int nstringindex[MAX_PATH*4]={0};//记录查找字符在字库中的位置下标
	char finstrings[MAX_PATH*100]={0};
	USES_CONVERSION;
	strcpy(finstrings,W2A(findstrs));

	//TSRuntime::add_log("finstrings：%s,strings:%s",finstrings,W2A(strings));
	//TSRuntime::add_log("FindStrFast:重新拼接要查找的字符串-ing,dictindex:%d,strings:%s",dictindex,W2A(strings));
	/////////////////////////////////////获取传进的要查找的字在字库中的位置(下标)
	for(int n=0;n<dictindex;n++)
	{
		bool bfind=false;
		//if(TSRuntime::MyDictCount[usingindex][n].mydistchar)
		{
			char *strtofind=strstr(finstrings,MyDictCount[usingindex][n].mydistchar);
			if(strtofind!=NULL)
			{
				if(nstringindex[nstringlen]==0)
				{
					nstringindex[nstringlen]=n;
					nstringlen++;
				}
				//TSRuntime::add_log("strtofind：%s",strtofind);
				//TSRuntime::add_log("nstringlen:%d,strtofind:%s",nstringlen,TSRuntime::MyDictCount[usingindex][n].mydistchar);
			}
			//TSRuntime::add_log("strtofind：%s",TSRuntime::MyDictCount[usingindex][n].mydistchar);

		}
	}
	if(nstringlen==0)  //如果字库中没找到用户要找的字,返回失败
	{
		return	-1;
	}
	//TSRuntime::add_log("nstringlen:%d",nstringlen);
	/////////////////////////////////////获取传进的要查找的字在字库中的位置(下标)
	if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())//normol找图
	{
		RECT rec;
		RECT clientrec;
		::GetClientRect(m_parenthwnd,&clientrec);
		int clienthight=clientrec.bottom-clientrec.top;
		int clientwide=clientrec.right-clientrec.left;
		::GetWindowRect(m_parenthwnd,&rec);
		POINT point;
		point.x=0;
		point.y=0;
		::ClientToScreen(m_parenthwnd,&point);
		m_Left=point.x;
		m_Top=point.y;
		m_Right=rec.right;
		m_bottom=rec.bottom;

		if(left<0)
			left=0;
		if(left >= clientwide)
			left = clientwide-1;

		if(top<0)
			top=0;
		if(top >= clienthight)
			top = clienthight-1;

		if(right >= clientwide)
			right=clientwide-1;

		if(bottom >= clienthight)
			bottom=clienthight-1;

		if(m_Left<0)
		{
			if((left+m_Left)<=0)//越界
				left=0;
			else
				left=m_Left+left;
		}
		else
			left=m_Left+left;

		if(m_Top>=0)
			top=m_Top+top;

		if(m_Right >= ::GetSystemMetrics(SM_CXSCREEN))
		{
			if((right+m_Left)>::GetSystemMetrics(SM_CXSCREEN))
				right=::GetSystemMetrics(SM_CXSCREEN)-1;
			else
				right=right+m_Left;
		}
		else
			right=right+m_Left;

		if(m_bottom >= ::GetSystemMetrics(SM_CYSCREEN))
		{
			if((bottom+m_Top)>=::GetSystemMetrics(SM_CYSCREEN))
				bottom=::GetSystemMetrics(SM_CYSCREEN)-1;
			else
				bottom=bottom+m_Top;
		}
		else
			bottom=bottom+m_Top;

		m_Left = left;
		m_Top = top;
		m_Right = right;
		m_bottom = bottom;

	}
	else
	{
		RECT clientrec;
		if(ColorDataInfo!=NULL)
			::GetClientRect(m_hwnd,&clientrec);
		else//gdi
			::GetClientRect(hwnd,&clientrec);
		int clienthight=clientrec.bottom-clientrec.top;
		int clientwide=clientrec.right-clientrec.left;

		m_Left = left;
		m_Top = top;
		m_Right = right;
		m_bottom = bottom;

		if(left<0)
			m_Left = left =0;
		if(left >= clientwide)
			m_Left = clientwide-1;

		if(top<0)
			m_Top = top =0;
		if(top >= clienthight)
			m_Top = clienthight-1;

		if(right >= clientwide)
			m_Right=clientwide-1;

		if(bottom >= clienthight)
			m_bottom=clienthight-1;
	}
	RECT rc;
	rc.bottom=m_bottom;
	rc.left=m_Left;
	rc.right=m_Right;
	rc.top=m_Top;

	if(ColorDataInfo!=NULL)
	{
		wndWidth=m_Right-m_Left;
		wndHeight=m_bottom-m_Top;
		//DX模式找字
		if(!getDXBitmap(pbuffer,ColorDataInfo,1))
		{
			return retIndex;
		}
	}	
	else
	{
		////GDI模式找字
		if(!getGDIBitmap(hwnd,rc,1))
		{
			return retIndex;
		}
	}
	/////////////////////////////控制四点的边界值以防止越界//////////////////////////
	GetStringInfo();
	DWORD Findstrings[MAX_PATH*10]={0};
	bool UnkownStr[MAX_PATH*10]={0};//未知字体

	/////////////////// 处理多张图 /////////////////////
	int indexstr=0;
	int findstrX=-1;
	int findstrY=-1;
	int X=0;
	int Y=0;
	BYTE bfindstr=0;
	for (int i=0;i<nstringlen;i++)
	{
		bool strfind=false;
		if(simi==1.0)//精确查找
		{
			//使用指定字库
			int nstrvaluelen=pWndBmpStr[MyDictCount[usingindex][nstringindex[i]].nstrvalue[0]].size();
			for(int n=0;n<nstrvaluelen;n++)
			{
				findstrX=pWndBmpStr[MyDictCount[usingindex][nstringindex[i]].nstrvalue[0]].at(n).x;
				findstrY=pWndBmpStr[MyDictCount[usingindex][nstringindex[i]].nstrvalue[0]].at(n).y;

				if(findstrX==-1||findstrY==-1)
					continue;

				if(comparaStr(MyDictCount[usingindex][nstringindex[i]].nstrvalue,MyDictCount[usingindex][nstringindex[i]].strwide,MyDictCount[usingindex][nstringindex[i]].strhight,X,Y,findstrX,findstrY,simi,MyDictCount[usingindex][nstringindex[i]].nstrcount))
				{
					strfind=true;
					int swide=MyDictCount[usingindex][nstringindex[i]].strwide;
					int shight=MyDictCount[usingindex][nstringindex[i]].strhight;

					for(int k=0;k<=swide;k++)//判断是否是非正规字体
					{
						if((pWndBmpStrxy[Y-1][X-1]!=0&&pWndBmpStrxy[Y-1][X-1]>=16)&&shight>=11)//左上角
						{
							UnkownStr[indexstr]=true;//非正规字体标识
							break;
						}
						if(pWndBmpStrxy[Y-1][X+k]!=0&&pWndBmpStrxy[Y-1][X+k]>=1024)
						{
							UnkownStr[indexstr]=true;//非正规字体标识
							break;
						}
					}

					//bool UnderlineString=true;//下滑线字体
					//bool bfind=false;
					//for(int k=0;k<=swide;k++)
					//{

					//	if((pWndBmpStrxy[Y][X+k]%2==0)||pWndBmpStrxy[Y][X+k]==0||swide<2||shight>11)//判断字体最后一行是否是下划线
					//		UnderlineString=false;
					//	//if(shight<=2&&pWndBmpStrxy[Y-4][X+k]!=0&&swide>=10) //区分三和一字的重合问题
					//	//	bfind=true;
					//	if(swide>=14&&shight==1&&pWndBmpStrxy[Y-2][X+k]!=0)//判断区别处理字体13号字体的点阵例如:"一"和"查"
					//		bfind=true;
					//	if(pWndBmpStrxy[Y-1][X+k]!=0&&pWndBmpStrxy[Y-1][X+k]>=1024)
					//		bfind=true;
					//	else if((pWndBmpStrxy[Y-1][X+swide]!=0&&pWndBmpStrxy[Y-1][X+swide]>=16)&&shight>=11)	//右上角
					//		bfind=true;
					//	else if((pWndBmpStrxy[Y-1][X-1]!=0&&pWndBmpStrxy[Y-1][X-1]>=16)&&shight>=11)//左上角
					//		bfind=true;
					//	if((pWndBmpStrxy[Y][X-1]!=0||pWndBmpStrxy[Y][X+swide]!=0)&&shight<5)
					//		bfind=true;
					//	if(swide==1&&(pWndBmpStrxy[Y-1][X-1]!=0||pWndBmpStrxy[Y-1][X+swide]!=0))	 //处理特殊符号
					//		bfind=true;

					//	if(bfind)	
					//		break;
					//}
					//if(bfind)
					//{
					//	if(UnderlineString==false)
					//		continue;
					//}

					if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())//normol模式
					{
						POINT pt;
						pt.x=X+m_Left;
						pt.y=Y+m_Top;
						::ScreenToClient(m_parenthwnd,&pt);
						addrxy[indexstr][0]=pt.x;
						addrxy[indexstr][1]=pt.y;
					}
					else
					{
						addrxy[indexstr][0]=X+m_Left;
						addrxy[indexstr][1]=Y+m_Top;
					}
					addrxy[indexstr][2]=swide;
					addrxy[indexstr][3]=shight;
					USES_CONVERSION;
					Findstrings[indexstr]=(DWORD)MyDictCount[usingindex][nstringindex[i]].mydistchar;
					//pWndBmpStr[MyDictCount[usingindex][nstringindex[i]].nstrvalue[0]].at(n).x=-1;
					//pWndBmpStr[MyDictCount[usingindex][nstringindex[i]].nstrvalue[0]].at(n).y=-1;
					indexstr++;

				}
				if(indexstr>=MAX_PATH*10-1)
					break;
			}
			if(indexstr>=MAX_PATH*10-1)
				break;
		}
		else if(simi>0.1&&simi<1.0)//处理相似度
		{
			//使用指定字库
			//int nstrwide=TSRuntime::MyDictCount[usingindex][nstringindex[i]].strwide;
			int nstrwide=MyDictCount[usingindex][nstringindex[i]].strwide;
			//COLORREF pLoadstrBuffer[11][MAX_PATH-10];
			for(int a=0;a<11;a++)
			{
				memset(TSRuntime::pLoadstrBuffer[a],0,nstrwide+1);
			}
			//将点阵信息还原图片矩阵信息
			//byteTopic(TSRuntime::MyDictCount[usingindex][nstringindex[i]].nstrvalue,nstrwide, pLoadstrBuffer);
			//byteTopic(TSRuntime::MyDictCount[usingindex][nstringindex[i]].nstrvalue,nstrwide);
			byteTopic(MyDictCount[usingindex][nstringindex[i]].nstrvalue,nstrwide);
			for(int j=1;j<2048;j++)
			{
				int nstrvaluelen=pWndBmpStr[j].size();
				for(int n=0;n<nstrvaluelen;n++)
				{
					findstrX=pWndBmpStr[j].at(n).x;
					findstrY=pWndBmpStr[j].at(n).y;

					if(findstrX==-1||findstrY==-1)
					{
						continue;
					}

					//if(comparaStrTosim(nstrwide,TSRuntime::MyDictCount[usingindex][nstringindex[i]].strhight,X,Y,findstrX,findstrY,simi,TSRuntime::MyDictCount[usingindex][nstringindex[i]].nstrcount))	
					if(comparaStrTosim(nstrwide,MyDictCount[usingindex][nstringindex[i]].strhight,X,Y,findstrX,findstrY,simi,MyDictCount[usingindex][nstringindex[i]].nstrcount))
					{

						//int swide=MyDictCount[usingindex][nstringindex[i]].strwide;
						//int shight=MyDictCount[usingindex][nstringindex[i]].strhight;

						//for(int k=0;k<=swide;k++)//判断是否是非正规字体
						//{
						//	if((pWndBmpStrxy[Y-1][X-1]!=0&&pWndBmpStrxy[Y-1][X-1]>=16)&&shight>=11)//左上角
						//	{
						//		UnkownStr[indexstr]=true;//非正规字体标识
						//		break;
						//	}
						//	if(pWndBmpStrxy[Y-1][X+k]!=0&&pWndBmpStrxy[Y-1][X+k]>=1024)
						//	{
						//		UnkownStr[indexstr]=true;//非正规字体标识
						//		break;
						//	}
						//}

						USES_CONVERSION;
						//wchar_t *finds=wcsstr(retstr,A2W(TSRuntime::MyDictCount[usingindex][nstringindex[i]].mydistchar));
						wchar_t *finds=wcsstr(retstr,A2W(MyDictCount[usingindex][nstringindex[i]].mydistchar));
						if(finds!=NULL)
						{
							bool bfind=false;
							for(int k=0;k<indexstr;k++)
							{
								//if(abs(addrxy[k][0]-X)<=TSRuntime::MyDictCount[usingindex][nstringindex[i]].strwide&&abs(addrxy[k][1]-Y)<=TSRuntime::MyDictCount[usingindex][nstringindex[i]].strhight)
								if(abs(addrxy[k][0]-X)<=MyDictCount[usingindex][nstringindex[i]].strwide&&abs(addrxy[k][1]-Y)<=MyDictCount[usingindex][nstringindex[i]].strhight)	
								{
									bfind=true;
									break;
								}

							}
							if(bfind)
								continue;
						}
						strfind=true;
						if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())//normol模式
						{
							POINT pt;
							pt.x=X+m_Left;
							pt.y=Y+m_Top;
							::ScreenToClient(m_parenthwnd,&pt);
							addrxy[indexstr][0]=pt.x;
							addrxy[indexstr][1]=pt.y;
						}
						else
						{
							addrxy[indexstr][0]=X+m_Left;
							addrxy[indexstr][1]=Y+m_Top;
						}
						/////////取背景图 点阵坐标

						//wsprintf(retstr,L"%s%s",retstr,A2W(TSRuntime::MyDictCount[usingindex][nstringindex[i]].mydistchar));
						//Findstrings[indexstr]=(DWORD)TSRuntime::MyDictCount[usingindex][nstringindex[i]].mydistchar;
						//for(int p=0;p<TSRuntime::MyDictCount[usingindex][nstringindex[i]].strwide;p++)
						Findstrings[indexstr]=(DWORD)MyDictCount[usingindex][nstringindex[i]].mydistchar;
						//for(int p=0;p<MyDictCount[usingindex][nstringindex[i]].strwide;p++)
						//{
						//	pWndBmpStrxy[Y][X+p]=0;
						//}
						//pWndBmpStr[j].at(n).x=-1;
						//pWndBmpStr[j].at(n).y=-1;
						indexstr++;
					}
					if(indexstr>=MAX_PATH*10-1)
						break;
				}
				if(indexstr>=MAX_PATH*10-1)
					break;
			}
			if(indexstr>=MAX_PATH*10-1)
				break;
		}


		if(!strfind&&nstr==1)
		{
			retIndex=-1;
			x=-1;
			y=-1;
		}
	}

	////TSRuntime::add_log( "TS_OCR-排序-start");
	int addrxyBak[MAX_PATH*10][4]={0};
	wchar_t tempfindstr[256]={0};
	wchar_t tempfindstr1[256]={0};

	for(int i=0;i<indexstr-1;i++)
	{
		retstr[i];
		retstr[i+1];
		if(addrxy[i+1][1]<addrxy[i][1])
		{
			for(int j=i+1;j>0;j--)
			{
				if(addrxy[j][1]<addrxy[j-1][1])
				{
					//这里做交换操作
					int tem[10*2]={0};
					::memcpy(tem,addrxy[j-1],4*4);
					::memcpy(addrxy[j-1],addrxy[j],4*4);
					::memcpy(addrxy[j],tem,4*4);
					DWORD csTemp=Findstrings[j-1];
					Findstrings[j-1]=Findstrings[j];
					Findstrings[j]=csTemp;
					bool bret=UnkownStr[j-1];
					UnkownStr[j-1]=UnkownStr[j];
					UnkownStr[j]=bret;
					addrxyBak[j-1][0]=addrxy[j-1][0];
					addrxyBak[j-1][1]=addrxy[j-1][1];
					addrxyBak[j-1][2]=addrxy[j-1][2];
					addrxyBak[j-1][3]=addrxy[j-1][3];

				}
				else
				{
					addrxyBak[j][0]=addrxy[j][0];
					addrxyBak[j][1]=addrxy[j][1];
					addrxyBak[j][2]=addrxy[j][2];
					addrxyBak[j][3]=addrxy[j][3];
				}

			}
		}

		addrxyBak[i][0]=addrxy[i][0];
		addrxyBak[i][1]=addrxy[i][1];
		addrxyBak[i][2]=addrxy[i][2];
		addrxyBak[i][3]=addrxy[i][3];
	}

	for(int i=0;i<indexstr-1;i++)
	{
		if(abs(addrxy[i+1][1]-addrxy[i][1])<=11)
		{
			//这里做交换操作
			for(int m=i+1;m>0;m--)
			{
				if(abs(addrxy[m][1]-addrxy[m-1][1])<=11)
				{
					if(addrxy[m][0]<addrxy[m-1][0])	
					{
						int tem[10*2]={0};
						::memcpy(tem,addrxy[m-1],4*4);
						::memcpy(addrxy[m-1],addrxy[m],4*4);
						::memcpy(addrxy[m],tem,4*4);
						DWORD csTemp=Findstrings[m-1];
						Findstrings[m-1]=Findstrings[m];
						Findstrings[m]=csTemp;
						bool bret=UnkownStr[m-1];
						UnkownStr[m-1]=UnkownStr[m];
						UnkownStr[m]=bret;
						addrxyBak[m-1][0]=addrxy[m-1][0];
						addrxyBak[m-1][1]=addrxy[m-1][1];
						addrxyBak[m-1][2]=addrxy[m-1][2];
						addrxyBak[m-1][3]=addrxy[m-1][3];
						addrxyBak[m][0]=addrxy[m][0];
						addrxyBak[m][1]=addrxy[m][1];
						addrxyBak[m][2]=addrxy[m][2];
						addrxyBak[m][3]=addrxy[m][3];
					}	
					else
					{
						addrxyBak[m][0]=addrxy[m][0];
						addrxyBak[m][1]=addrxy[m][1];
						addrxyBak[m][2]=addrxy[m][2];
						addrxyBak[m][3]=addrxy[m][3];
					}

				}
				else
				{
					addrxyBak[m][0]=addrxy[m][0];
					addrxyBak[m][1]=addrxy[m][1];
					addrxyBak[m][2]=addrxy[m][2];
					addrxyBak[m][3]=addrxy[m][3];
					break;
				}
			}

		}
	}
	if(indexstr>=1)
	{
		addrxyBak[indexstr-1][0]=addrxy[indexstr-1][0];
		addrxyBak[indexstr-1][1]=addrxy[indexstr-1][1];
		addrxyBak[indexstr-1][2]=addrxy[indexstr-1][2];
		addrxyBak[indexstr-1][3]=addrxy[indexstr-1][3];
	}

	DWORD Strings[MAX_PATH*10]={0};//记录字符串指针地址
	DWORD nindex=0;//记录组合字个数
	DWORD nindexs[MAX_PATH*10]={0};//记录组合字的下标
	for(int i=0;i<indexstr;i++)
	{
		int len=strlen((char *)Findstrings[i]);
		len=len/sizeof(wchar_t);
		if(len>1)   //是组合字
		{
			Strings[nindex]=Findstrings[i];
			nindexs[nindex]=i;
			nindex++;
		}
	}

	if(nindex>0)
	{
		for(int i=0;i<nindex;i++)
		{
			for(int j=0;j<indexstr;j++)
			{
				if(nindexs[i]!=j)
				{
					char *t1=(char *)Strings[i];
					char *t2=(char *)Findstrings[j];
					char *findchar=strstr((char *)Strings[i],(char *)Findstrings[j]);
					if(findchar!=NULL)
					{
						if(addrxyBak[nindexs[i]][0]<=addrxyBak[j][0]&&
							(addrxyBak[nindexs[i]][0]+addrxyBak[nindexs[i]][2])>=(addrxyBak[j][0]+addrxyBak[j][2])&&
							addrxyBak[nindexs[i]][1]==addrxyBak[j][1])
						{
							for(int n=0;n<indexstr-j;n++)
							{
								addrxyBak[j+n][0]=addrxyBak[j+n+1][0];
								addrxyBak[j+n][1]=addrxyBak[j+n+1][1];
								addrxyBak[j+n][2]=addrxyBak[j+n+1][2];
								Findstrings[j+n]=Findstrings[j+n+1];
							}
							indexstr--;
							nindexs[i+1]--;
						}
					}
				}
			}
		}
	}

	//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11
	int nstrs=0;
	for(int i=0;i<indexstr;i++)
	{
		int j=1;
		short hight=addrxyBak[i][3];
		short wide=addrxyBak[i][2];
		short Y=addrxyBak[i][1];
		short X=addrxyBak[i][0];
		bool iscontinuefind=false;
continuefind:
		//char *t=(char *)Findstrings[i];
		//char *tt=(char *)Findstrings[i+j];
		if(addrxyBak[i+j][0]<(X+wide)&&((addrxyBak[i+j][1]-Y)<hight))
		{
			bool bfind=false;
			for(int n=0;n<indexstr-i;n++)
			{
				if(addrxyBak[i+j+n][2]>wide&&bfind==false&&UnkownStr[i+j+n]==false)
				{
					//char *t1=(char *)Findstrings[i+n];
					//char *t2=(char *)Findstrings[i+j+n];
					addrxyBak[i+n][0]=addrxyBak[i+j+n][0];
					addrxyBak[i+n][1]=addrxyBak[i+j+n][1];
					addrxyBak[i+n][2]=addrxyBak[i+j+n][2];
					addrxyBak[i+n][3]=addrxyBak[i+j+n][3];
					UnkownStr[i+n]=UnkownStr[i+j+n];
					Findstrings[i+n]=Findstrings[i+j+n];
				}
				else
				{
					bfind=true;
					//char *t1=(char *)Findstrings[i+j+n];
					//char *t2=(char *)Findstrings[i+j+1+n];
					addrxyBak[i+j+n][0]=addrxyBak[i+j+1+n][0];
					addrxyBak[i+j+n][1]=addrxyBak[i+j+1+n][1];
					addrxyBak[i+j+n][2]=addrxyBak[i+j+1+n][2];
					addrxyBak[i+j+n][3]=addrxyBak[i+j+1+n][3];
					UnkownStr[i+j+n]=UnkownStr[i+j+1+n];
					Findstrings[i+j+n]=Findstrings[i+j+1+n];
				}
			}
			nstrs++;
			int dwnstr=indexstr-nstrs;
			if(dwnstr==0)
			{
				indexstr=i+1;
				break;
			}
			//char tempTetstr[MAX_PATH*100]={0};
			//for(int k=0;k<dwnstr;k++)
			//{
			//	::strcat(tempTetstr,(char *)Findstrings[k]);
			//}
			iscontinuefind=true;
			goto continuefind;
		}
		else if(!iscontinuefind)
			nstrs++;

	}
	//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11

	//TSRuntime::add_log( "TS_OCR-排序-end");


	char tempTet[MAX_PATH*100]={0};
	for(int i=0;i<indexstr;i++)
	{
		::strcat(tempTet,(char *)Findstrings[i]);
	}
	//USES_CONVERSION;
	::wcscpy(retstr,A2W(tempTet));
	//MessageBox(NULL,retstr,L"TS",NULL);
	wchar_t *retstrnext=retstr;
	int findJ=0;
	if(wcslen(retstr)>0)
	{
		////存储最先找到的字符串
		int strfindlast=0;
		int strfind=0;
		short ret_x=-1;
		short ret_y=-1;
		short n_index=-1;
		for(int j=0;j<nstr;j++)
		{
			short len=wcslen(StrBuffer[j]);
			short retx=-1;
			short rety=-1;
			short nindex=-1;
			wchar_t *straddr=NULL;
			int rlen=0;
			for(int i=0;i<len;i++)
			{
				if(findJ!=j)//重置状态
				{
					findJ=j;
					retstrnext=retstr;
				}
				//在找到的字符中查找,是否存在
				wchar_t findchar[2]={0};
				//wcsncpy(findchar,&StrBuffer[j][i],sizeof(wchar_t));
				wcsncpy(findchar,&StrBuffer[j][i],1);
				if(i==0)
					straddr=wcsstr(retstrnext,findchar);
				else if(straddr!=NULL)
					straddr=wcsstr(straddr,findchar);
				if(straddr!=NULL)
				{
					if(i==0)
					{
						wchar_t * stringaddr=wcsstr(retstrnext,StrBuffer[j]);
						if(stringaddr!=NULL)
							straddr=stringaddr;

						strfind=(DWORD)straddr;
						nindex=j;
						retx=addrxyBak[((DWORD)strfind-(DWORD)retstr)/2][0];
						rety=addrxyBak[((DWORD)strfind-(DWORD)retstr)/2][1];
						if(retstring&&type==2)
						{
							swprintf(retstring,L"%s,%d,%d",StrBuffer[j],retx,rety);
						}
						if(retstring&&type!=2)
						{
							rlen=wcslen(retstring);
							bool issamex=false;
							bool issamey=false;
							if(retx!=ret_x)
							{
								ret_x=retx;
								issamex=true;
							}
							if(ret_y!=rety)
							{
								ret_y=rety;
								issamey=true;
							}
							if(n_index!=nindex)
								n_index=nindex;
							else
							{
								if(!issamex&&!issamey)
								{
									retstrnext++;
									if(wcsstr(retstrnext,StrBuffer[j])&&rlen<(MAX_PATH*100-16)&&i==len-1&&len!=1)//继续往后找,如果还找到存在,重置变量j,继续返回坐标
										j--;
									continue;
								}
							}
							if(rlen>1&&rlen<(MAX_PATH*100-16)&&type==1)//FindStrFastExS
								swprintf(retstring,L"%s|%s,%d,%d",retstring,StrBuffer[j],retx,rety);
							else if(rlen==0&&type==1)//FindStrFastExS
								swprintf(retstring,L"%s,%d,%d",StrBuffer[j],retx,rety);
							else if(rlen>1&&rlen<(MAX_PATH*100-16))////FindStrFastEx
								swprintf(retstring,L"%s|%d,%d,%d",retstring,nindex,retx,rety);
							else if(rlen==0)////FindStrFastEx
								swprintf(retstring,L"%d,%d,%d",nindex,retx,rety);
							retstrnext++;

						}
					}
				}
				else
				{
					nindex=-1;
					retx=-1;
					rety=-1;
					break;
				}
				if(wcsstr(retstrnext,StrBuffer[j])&&rlen<(MAX_PATH*100-16)&&i==len-1&&type!=2&&retstring)//继续往后找,如果还找到存在,重置变量j,继续返回坐标,&&len!=1
				{
					if(len==1)
					{
						i--;
						continue;
					}
					j--;
				}


			}
			if(nindex!=-1&&(strfindlast==0||(strfind<strfindlast&&strfind!=0)))
			{
				strfindlast=strfind;
				retIndex=nindex;
				x=retx;
				y=rety;
			}

		}	
	}
	return retIndex;
}



int MyFindPictureClass::FindStr(PVOID pbuffer,HWND hwnd,int left, int top, int right, int bottom, wchar_t* strings, wchar_t* color, double simi, long &x,long &y,pMyDictInfo *MyDictCount,int *nMyDictCountsize,int NowUsingDictIndex,DWORD (*ColorDataInfo)[2000],wchar_t *retstring,int type)
{

	wchar_t  retstr[MAX_PATH*100]={0};
	int index = 0;
	int retIndex=-1;
	bool isFind=false;
	//得到当前字库的下标
	usingindex=NowUsingDictIndex;
	//得到当前字库个数
	dictindex=nMyDictCountsize[usingindex];

	if(dictindex<=0)  //字库个数==0就退出
		return isFind;


	//处理多个色偏
	int col=0;
	wchar_t colorBuffer[MAX_PATH][16];//存储多个字符串
	for(int i=0;i<MAX_PATH;i++)
	{
		memset(colorBuffer[i],0,16);
	}

	short nstr=0;//要查找色偏的个数  ,最多支持(MAX_PATH)个色偏
	while(*color)
	{
		if(*color!=L'|')
		{
			if(col>13)
				return retIndex;
			colorBuffer[nstr][col++]=*color;
			color++;
			continue;
		}
		else
		{
			if(col<1)
			{
				color++;
				continue;
			}
			nstr++;
			col=0;
			color++;
		}
	}
	if(col>=1)
		nstr++;

	ncolors=nstr;//记录下色偏个数

	//int t=::GetTickCount();
	for(int i=0;i<nstr;i++)
	{
		WCHAR colorR[4]={0};//字的颜色
		WCHAR colorG[4]={0};//字的颜色
		WCHAR colorB[4]={0};//字的颜色

		WCHAR coloroffR[4]={0};//偏色
		WCHAR coloroffG[4]={0};//偏色
		WCHAR coloroffB[4]={0};//偏色
		int count=0;
		int colorL=0;

		//StrTrimW(colorBuffer[i],L" ");//移除前后空格符

		while(colorBuffer[i][colorL]!=L'-')
		{
			if(count>6)	//判断是否是非法颜色值
				return retIndex;
			if(count<2)
				colorR[count]=colorBuffer[i][colorL++];
			else if(count>1&&count<4)
				colorG[count-2]=colorBuffer[i][colorL++];
			else if(count>3)
				colorB[count-4]=colorBuffer[i][colorL++];
			count++;

		}
		//将传入的RGB转换为BGR
		WCHAR colorBGR[16]={0};//字的颜色
		swprintf(colorBGR,L"%s%s%s",colorB,colorG,colorR);
		count=0;
		colorL++;

		while(colorBuffer[colorL]!=L'\0')
		{
			if(count>=6) //判断是否是非法色偏值
				break;
			if(count<2)
				coloroffR[count]=colorBuffer[i][colorL++];
			else if(count>1&&count<4)
				coloroffG[count-2]=colorBuffer[i][colorL++];
			else if(count>3)
				coloroffB[count-4]=colorBuffer[i][colorL++];
			count++;
		}

		//将传入的RGB转换为BGR
		WCHAR coloroffBGR[16]={0};//偏色
		swprintf(coloroffBGR,L"%s%s%s",coloroffB,coloroffG,coloroffR);

		strColors[i]=wcstol(colorBGR,NULL,16);//整形颜色值
		strColoroff[i]=wcstol(coloroffBGR,NULL,16);//整形偏色值
		m_colorOffR[i] = GetRValue(strColoroff[i]);
		m_colorOffG[i] = GetGValue(strColoroff[i]);
		m_colorOffB[i] = GetBValue(strColoroff[i]);


	}


	if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())//normol找图
	{
		RECT rec;
		RECT clientrec;
		::GetClientRect(m_parenthwnd,&clientrec);
		int clienthight=clientrec.bottom-clientrec.top;
		int clientwide=clientrec.right-clientrec.left;
		::GetWindowRect(m_parenthwnd,&rec);
		POINT point;
		point.x=0;
		point.y=0;
		::ClientToScreen(m_parenthwnd,&point);
		m_Left=point.x;
		m_Top=point.y;
		m_Right=rec.right;
		m_bottom=rec.bottom;

		if(left<0)
			left=0;
		if(left >= clientwide)
			left = clientwide-1;

		if(top<0)
			top=0;
		if(top >= clienthight)
			top = clienthight-1;

		if(right >= clientwide)
			right=clientwide-1;

		if(bottom >= clienthight)
			bottom=clienthight-1;

		if(m_Left<0)
		{
			if((left+m_Left)<=0)//越界
				left=0;
			else
				left=m_Left+left;
		}
		else
			left=m_Left+left;

		if(m_Top>=0)
			top=m_Top+top;

		if(m_Right >= ::GetSystemMetrics(SM_CXSCREEN))
		{
			if((right+m_Left)>::GetSystemMetrics(SM_CXSCREEN))
				right=::GetSystemMetrics(SM_CXSCREEN)-1;
			else
				right=right+m_Left;
		}
		else
			right=right+m_Left;

		if(m_bottom >= ::GetSystemMetrics(SM_CYSCREEN))
		{
			if((bottom+m_Top)>=::GetSystemMetrics(SM_CYSCREEN))
				bottom=::GetSystemMetrics(SM_CYSCREEN)-1;
			else
				bottom=bottom+m_Top;
		}
		else
			bottom=bottom+m_Top;

		m_Left = left;
		m_Top = top;
		m_Right = right;
		m_bottom = bottom;

	}
	else
	{
		RECT clientrec;
		if(ColorDataInfo!=NULL)
			::GetClientRect(m_hwnd,&clientrec);
		else//gdi
			::GetClientRect(hwnd,&clientrec);
		int clienthight=clientrec.bottom-clientrec.top;
		int clientwide=clientrec.right-clientrec.left;
		m_Left = left;
		m_Top = top;
		m_Right = right;
		m_bottom = bottom;

		if(left<0)
			m_Left = left =0;
		if(left >= clientwide)
			m_Left = clientwide-1;

		if(top<0)
			m_Top = top =0;
		if(top >= clienthight)
			m_Top = clienthight-1;

		if(right >= clientwide)
			m_Right=clientwide-1;

		if(bottom >= clienthight)
			m_bottom=clienthight-1;
	}
	RECT rc;
	rc.bottom=m_bottom;
	rc.left=m_Left;
	rc.right=m_Right;
	rc.top=m_Top;

	if(ColorDataInfo!=NULL)
	{
		wndWidth=m_Right-m_Left;
		wndHeight=m_bottom-m_Top;
		//DX模式找字
		if(!getDXBitmap(pbuffer,ColorDataInfo,1))
		{
			return retIndex;
		}
	}	
	else
	{
		////GDI模式找字
		if(!getGDIBitmap(hwnd,rc,1))
		{
			return retIndex;
		}
	}

	GetStringInfo();

	//处理查找多个字
	col=0;
	wchar_t StrBuffer[MAX_PATH][MAX_PATH];//存储多个字符串
	for(int i=0;i<MAX_PATH;i++)
	{
		memset(StrBuffer[i],0,MAX_PATH);
	}
	nstr=0;//要查找字的个数  ,最多支持(MAX_PATH)个字
	while(*strings)
	{
		if(*strings!=L'|')
		{
			StrBuffer[nstr][col++]=*strings;
			strings++;
			continue;
		}
		else
		{
			if(col<1)
			{
				strings++;
				continue;
			}
			nstr++;
			col=0;
			strings++;
		}
	}
	if(col>=1)
		nstr++;

	//处理查找多个字

	DWORD Findstrings[MAX_PATH*10]={0};
	bool UnkownStr[MAX_PATH*10]={0};//未知字体

	/////////////////// 处理多张图 /////////////////////
	int indexstr=0;
	int findstrX=-1;
	int findstrY=-1;
	int X=0;
	int Y=0;
	x=-1;
	y=-1;
	for (int i=0;i<dictindex;i++)
	{
		if(simi==1.0)
		{
			//使用指定字库
			//int nstrvaluelen=pWndBmpStr[TSRuntime::MyDictCount[usingindex][i].nstrvalue[0]].size();
			int nstrvaluelen=pWndBmpStr[MyDictCount[usingindex][i].nstrvalue[0]].size();
			for(int n=0;n<nstrvaluelen;n++)
			{
				findstrX=pWndBmpStr[MyDictCount[usingindex][i].nstrvalue[0]].at(n).x;
				findstrY=pWndBmpStr[MyDictCount[usingindex][i].nstrvalue[0]].at(n).y;
				if(findstrX==-1||findstrY==-1)
					continue;
				if(comparaStr(MyDictCount[usingindex][i].nstrvalue,MyDictCount[usingindex][i].strwide,MyDictCount[usingindex][i].strhight,X,Y,findstrX,findstrY,simi,MyDictCount[usingindex][i].nstrcount))	
				{	
					int swide=MyDictCount[usingindex][i].strwide;
					int shight=MyDictCount[usingindex][i].strhight;

					for(int k=0;k<=swide;k++)//判断是否是非正规字体
					{
						if((pWndBmpStrxy[Y-1][X-1]!=0&&pWndBmpStrxy[Y-1][X-1]>=16)&&shight>=11)//左上角
						{
							UnkownStr[indexstr]=true;//非正规字体标识
							break;
						}
						if(pWndBmpStrxy[Y-1][X+k]!=0&&pWndBmpStrxy[Y-1][X+k]>=1024)
						{
							UnkownStr[indexstr]=true;//非正规字体标识
							break;
						}
					}

					//bool UnderlineString=true;//下滑线字体
					//bool bfind=false;
					//for(int k=0;k<=swide;k++)
					//{
					//	if((pWndBmpStrxy[Y][X+k]%2==0)||pWndBmpStrxy[Y][X+k]==0||swide<2||shight>11)//判断字体最后一行是否是下划线
					//		UnderlineString=false;
					//	//if(shight<=2&&pWndBmpStrxy[Y-4][X+k]!=0&&swide>=10) //区分三和一字的重合问题
					//	//	bfind=true;
					//	if(swide>=14&&shight==1&&pWndBmpStrxy[Y-2][X+k]!=0)//判断区别处理字体13号字体的点阵例如:"一"和"查"
					//		bfind=true;
					//	if(pWndBmpStrxy[Y-1][X+k]!=0&&pWndBmpStrxy[Y-1][X+k]>=1024)
					//		bfind=true;
					//	else if((pWndBmpStrxy[Y-1][X+swide]!=0&&pWndBmpStrxy[Y-1][X+swide]>=16)&&shight>=11)	//右上角
					//		bfind=true;
					//	else if((pWndBmpStrxy[Y-1][X-1]!=0&&pWndBmpStrxy[Y-1][X-1]>=16)&&shight>=11)//左上角
					//		bfind=true;
					//	if((pWndBmpStrxy[Y][X-1]!=0||pWndBmpStrxy[Y][X+swide]!=0)&&shight<5)
					//		bfind=true;
					//	if(swide==1&&(pWndBmpStrxy[Y-1][X-1]!=0||pWndBmpStrxy[Y-1][X+swide]!=0))	 //处理特殊符号
					//		bfind=true;

					//	if(bfind)	
					//		break;
					//}
					//if(bfind)
					//{
					//	if(UnderlineString==false)
					//		continue;
					//}
					if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())//normol模式
					{
						POINT pt;
						pt.x=X+m_Left;
						pt.y=Y+m_Top;
						::ScreenToClient(m_parenthwnd,&pt);
						addrxy[indexstr][0]=pt.x;
						addrxy[indexstr][1]=pt.y;
					}
					else
					{
						addrxy[indexstr][0]=X+m_Left;
						addrxy[indexstr][1]=Y+m_Top;
					}
					addrxy[indexstr][2]=swide;
					addrxy[indexstr][3]=shight;
					USES_CONVERSION;
					///////取背景图 点阵坐标
					Findstrings[indexstr]=(DWORD)(MyDictCount[usingindex][i].mydistchar);
					////重置背景图坐标,防止找重复图
					//pWndBmpStr[MyDictCount[usingindex][i].nstrvalue[0]].at(n).x=-1;   
					//pWndBmpStr[MyDictCount[usingindex][i].nstrvalue[0]].at(n).y=-1;
					indexstr++;

				}
				if(indexstr>=MAX_PATH*10-1)
					break;
			}
			if(indexstr>=MAX_PATH*10-1)
				break;
		}
		else if(simi>0.1&&simi<1.0)
		{
			//int nstrwide=TSRuntime::MyDictCount[usingindex][i].strwide;
			int nstrwide=MyDictCount[usingindex][i].strwide;
			//COLORREF pLoadstrBuffer[11][MAX_PATH-10];
			for(int a=0;a<11;a++)
			{
				memset(TSRuntime::pLoadstrBuffer[a],0,nstrwide+1);
			}
			//将点阵信息还原图片矩阵信息
			//byteTopic(TSRuntime::MyDictCount[usingindex][i].nstrvalue,nstrwide);
			byteTopic(MyDictCount[usingindex][i].nstrvalue,nstrwide);
			for(int j=1;j<2048;j++)
			{
				//使用指定字库
				int nstrvaluelen=pWndBmpStr[j].size();
				for(int n=0;n<nstrvaluelen;n++)
				{
					findstrX=pWndBmpStr[j].at(n).x;
					findstrY=pWndBmpStr[j].at(n).y;
					if(findstrX==-1||findstrY==-1)
						continue;
					//if(comparaStrTosim(nstrwide,TSRuntime::MyDictCount[usingindex][i].strhight,X,Y,findstrX,findstrY,simi,TSRuntime::MyDictCount[usingindex][i].nstrcount))
					if(comparaStrTosim(nstrwide,MyDictCount[usingindex][i].strhight,X,Y,findstrX,findstrY,simi,MyDictCount[usingindex][i].nstrcount))
					{
						int swide=MyDictCount[usingindex][i].strwide;
						int shight=MyDictCount[usingindex][i].strhight;

						for(int k=0;k<=swide;k++)//判断是否是非正规字体
						{
							if((pWndBmpStrxy[Y-1][X-1]!=0&&pWndBmpStrxy[Y-1][X-1]>=16)&&shight>=11)//左上角
							{
								UnkownStr[indexstr]=true;//非正规字体标识
								break;
							}
							if(pWndBmpStrxy[Y-1][X+k]!=0&&pWndBmpStrxy[Y-1][X+k]>=1024)
							{
								UnkownStr[indexstr]=true;//非正规字体标识
								break;
							}
						}
						//USES_CONVERSION;
						////wchar_t *finds=wcsstr(retstr,A2W(TSRuntime::MyDictCount[usingindex][i].mydistchar));
						//wchar_t *finds=wcsstr(retstr,A2W(MyDictCount[usingindex][i].mydistchar));
						//if(finds!=NULL)
						//{
						//	bool bfind=false;
						//	for(int k=0;k<indexstr;k++)
						//	{
						//		//if(abs(addrxy[k][0]-X)<=TSRuntime::MyDictCount[usingindex][i].strwide&&abs(addrxy[k][1]-Y)<=TSRuntime::MyDictCount[usingindex][i].strhight)
						//		if(abs(addrxy[k][0]-X)<=MyDictCount[usingindex][i].strwide&&abs(addrxy[k][1]-Y)<=MyDictCount[usingindex][i].strhight)
						//		{
						//			bfind=true;
						//			break;
						//		}

						//	}
						//	if(bfind)
						//		continue;
						//}
						if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())//normol模式
						{
							POINT pt;
							pt.x=X+m_Left;
							pt.y=Y+m_Top;
							::ScreenToClient(m_parenthwnd,&pt);
							addrxy[indexstr][0]=pt.x;
							addrxy[indexstr][1]=pt.y;
						}
						else
						{
							addrxy[indexstr][0]=X+m_Left;
							addrxy[indexstr][1]=Y+m_Top;
						}
						addrxy[indexstr][2]=swide;
						addrxy[indexstr][3]=shight;
						///////取背景图 点阵坐标
						Findstrings[indexstr]=(DWORD)(MyDictCount[usingindex][i].mydistchar);
						////重置背景图坐标,防止找重复图
						//pWndBmpStr[j].at(n).x=-1;
						//pWndBmpStr[j].at(n).y=-1;
						indexstr++;
					}
					if(indexstr>=MAX_PATH*10-1)
						break;
				}
				if(indexstr>=MAX_PATH*10-1)
					break;
			}
		}
	}


	////TSRuntime::add_log( "TS_OCR-排序-start");
	int addrxyBak[MAX_PATH*10][4]={0};
	wchar_t tempfindstr[256]={0};
	wchar_t tempfindstr1[256]={0};

	for(int i=0;i<indexstr-1;i++)
	{
		retstr[i];
		retstr[i+1];
		if(addrxy[i+1][1]<addrxy[i][1])
		{
			for(int j=i+1;j>0;j--)
			{
				if(addrxy[j][1]<addrxy[j-1][1])
				{
					//这里做交换操作
					int tem[10*2]={0};
					::memcpy(tem,addrxy[j-1],4*4);
					::memcpy(addrxy[j-1],addrxy[j],4*4);
					::memcpy(addrxy[j],tem,4*4);
					DWORD csTemp=Findstrings[j-1];
					Findstrings[j-1]=Findstrings[j];
					Findstrings[j]=csTemp;
					bool bret=UnkownStr[j-1];
					UnkownStr[j-1]=UnkownStr[j];
					UnkownStr[j]=bret;
					addrxyBak[j-1][0]=addrxy[j-1][0];
					addrxyBak[j-1][1]=addrxy[j-1][1];
					addrxyBak[j-1][2]=addrxy[j-1][2];
					addrxyBak[j-1][3]=addrxy[j-1][3];

				}
				else
				{
					addrxyBak[j][0]=addrxy[j][0];
					addrxyBak[j][1]=addrxy[j][1];
					addrxyBak[j][2]=addrxy[j][2];
					addrxyBak[j][3]=addrxy[j][3];
				}

			}
		}

		addrxyBak[i][0]=addrxy[i][0];
		addrxyBak[i][1]=addrxy[i][1];
		addrxyBak[i][2]=addrxy[i][2];
		addrxyBak[i][3]=addrxy[i][3];
	}

	for(int i=0;i<indexstr-1;i++)
	{
		if(abs(addrxy[i+1][1]-addrxy[i][1])<=11)
		{
			//这里做交换操作
			for(int m=i+1;m>0;m--)
			{
				if(abs(addrxy[m][1]-addrxy[m-1][1])<=11)
				{
					if(addrxy[m][0]<addrxy[m-1][0])	
					{
						int tem[10*2]={0};
						::memcpy(tem,addrxy[m-1],4*4);
						::memcpy(addrxy[m-1],addrxy[m],4*4);
						::memcpy(addrxy[m],tem,4*4);
						DWORD csTemp=Findstrings[m-1];
						Findstrings[m-1]=Findstrings[m];
						Findstrings[m]=csTemp;
						bool bret=UnkownStr[m-1];
						UnkownStr[m-1]=UnkownStr[m];
						UnkownStr[m]=bret;
						addrxyBak[m-1][0]=addrxy[m-1][0];
						addrxyBak[m-1][1]=addrxy[m-1][1];
						addrxyBak[m-1][2]=addrxy[m-1][2];
						addrxyBak[m-1][3]=addrxy[m-1][3];
						addrxyBak[m][0]=addrxy[m][0];
						addrxyBak[m][1]=addrxy[m][1];
						addrxyBak[m][2]=addrxy[m][2];
						addrxyBak[m][3]=addrxy[m][3];
					}	
					else
					{
						addrxyBak[m][0]=addrxy[m][0];
						addrxyBak[m][1]=addrxy[m][1];
						addrxyBak[m][2]=addrxy[m][2];
						addrxyBak[m][3]=addrxy[m][3];
					}

				}
				else
				{
					addrxyBak[m][0]=addrxy[m][0];
					addrxyBak[m][1]=addrxy[m][1];
					addrxyBak[m][2]=addrxy[m][2];
					addrxyBak[m][3]=addrxy[m][3];
					break;
				}
			}

		}
	}
	if(indexstr>=1)
	{
		addrxyBak[indexstr-1][0]=addrxy[indexstr-1][0];
		addrxyBak[indexstr-1][1]=addrxy[indexstr-1][1];
		addrxyBak[indexstr-1][2]=addrxy[indexstr-1][2];
		addrxyBak[indexstr-1][3]=addrxy[indexstr-1][3];
	}

	DWORD Strings[MAX_PATH*10]={0};//记录字符串指针地址
	DWORD nindex=0;//记录组合字个数
	DWORD nindexs[MAX_PATH*10]={0};//记录组合字的下标
	for(int i=0;i<indexstr;i++)
	{
		int len=strlen((char *)Findstrings[i]);
		len=len/sizeof(wchar_t);
		if(len>1)   //是组合字
		{
			Strings[nindex]=Findstrings[i];
			nindexs[nindex]=i;
			nindex++;
		}
	}

	if(nindex>0)
	{
		for(int i=0;i<nindex;i++)
		{
			for(int j=0;j<indexstr;j++)
			{
				if(nindexs[i]!=j)
				{
					char *t1=(char *)Strings[i];
					char *t2=(char *)Findstrings[j];
					char *findchar=strstr((char *)Strings[i],(char *)Findstrings[j]);
					if(findchar!=NULL)
					{
						if(addrxyBak[nindexs[i]][0]<=addrxyBak[j][0]&&
							(addrxyBak[nindexs[i]][0]+addrxyBak[nindexs[i]][2])>=(addrxyBak[j][0]+addrxyBak[j][2])&&
							addrxyBak[nindexs[i]][1]==addrxyBak[j][1])
						{
							for(int n=0;n<indexstr-j;n++)
							{
								addrxyBak[j+n][0]=addrxyBak[j+n+1][0];
								addrxyBak[j+n][1]=addrxyBak[j+n+1][1];
								addrxyBak[j+n][2]=addrxyBak[j+n+1][2];
								Findstrings[j+n]=Findstrings[j+n+1];
							}
							indexstr--;
							nindexs[i+1]--;
						}
					}
				}
			}
		}
	}

	//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11
	int nstrs=0;
	for(int i=0;i<indexstr;i++)
	{
		int j=1;
		short hight=addrxyBak[i][3];
		short wide=addrxyBak[i][2];
		short Y=addrxyBak[i][1];
		short X=addrxyBak[i][0];
		bool iscontinuefind=false;
continuefind:
		//char *t=(char *)Findstrings[i];
		//char *tt=(char *)Findstrings[i+j];
		if(addrxyBak[i+j][0]<(X+wide)&&((addrxyBak[i+j][1]-Y)<hight))
		{
			bool bfind=false;
			for(int n=0;n<indexstr-i;n++)
			{
				if(addrxyBak[i+j+n][2]>wide&&bfind==false&&UnkownStr[i+j+n]==false)
				{
					//char *t1=(char *)Findstrings[i+n];
					//char *t2=(char *)Findstrings[i+j+n];
					addrxyBak[i+n][0]=addrxyBak[i+j+n][0];
					addrxyBak[i+n][1]=addrxyBak[i+j+n][1];
					addrxyBak[i+n][2]=addrxyBak[i+j+n][2];
					addrxyBak[i+n][3]=addrxyBak[i+j+n][3];
					UnkownStr[i+n]=UnkownStr[i+j+n];
					Findstrings[i+n]=Findstrings[i+j+n];
				}
				else
				{
					bfind=true;
					//char *t1=(char *)Findstrings[i+j+n];
					//char *t2=(char *)Findstrings[i+j+1+n];
					addrxyBak[i+j+n][0]=addrxyBak[i+j+1+n][0];
					addrxyBak[i+j+n][1]=addrxyBak[i+j+1+n][1];
					addrxyBak[i+j+n][2]=addrxyBak[i+j+1+n][2];
					addrxyBak[i+j+n][3]=addrxyBak[i+j+1+n][3];
					UnkownStr[i+j+n]=UnkownStr[i+j+1+n];
					Findstrings[i+j+n]=Findstrings[i+j+1+n];
				}
			}
			nstrs++;
			int dwnstr=indexstr-nstrs;
			if(dwnstr==0)
			{
				indexstr=i+1;
				break;
			}
			//char tempTetstr[MAX_PATH*100]={0};
			//for(int k=0;k<dwnstr;k++)
			//{
			//	::strcat(tempTetstr,(char *)Findstrings[k]);
			//}
			iscontinuefind=true;
			goto continuefind;
		}
		else if(!iscontinuefind)
			nstrs++;

	}
	//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11

	//TSRuntime::add_log( "TS_OCR-排序-end");

	USES_CONVERSION;
	char tempTet[MAX_PATH*100]={0};
	for(int i=0;i<indexstr;i++)
	{
		::strcat(tempTet,(char *)Findstrings[i]);
	}
	::wcscpy(retstr,A2W(tempTet));

	//MessageBox(NULL,retstr,L"TS",NULL);
	int strfind=0;//存储最先找到的字符串
	wchar_t *straddr=NULL;
	for(int j=0;j<nstr;j++)
	{
		//在找到的字符中查找,是否存在
		if(straddr==NULL)
			straddr=wcsstr(retstr,StrBuffer[j]);
		else
			straddr=wcsstr(straddr,StrBuffer[j]);
		if(straddr!=NULL)
		{
			if(strfind==0||strfind>((DWORD)straddr))
			{

				strfind=(DWORD)straddr;
				retIndex=j;
				x=addrxyBak[((DWORD)straddr-(DWORD)retstr)/2][0];
				y=addrxyBak[((DWORD)straddr-(DWORD)retstr)/2][1];
				if(type==2&&retstring)//FindStrS
				{
					swprintf(retstring,L"%s,%d,%d",StrBuffer[j],x,y);
				}
			}
			if(retstring&&type!=2)
			{
				retIndex=j;
				x=addrxyBak[((DWORD)straddr-(DWORD)retstr)/2][0];
				y=addrxyBak[((DWORD)straddr-(DWORD)retstr)/2][1];
				int len=wcslen(retstring);
				if(len>1&&len<(MAX_PATH*100-16)&&type==1)//FindStrExS
					swprintf(retstring,L"%s|%s,%d,%d",retstring,StrBuffer[j],x,y);
				else if(len==0&&type==1)//FindStrExS
					swprintf(retstring,L"%s,%d,%d",StrBuffer[j],x,y);
				else if(len>1&&len<(MAX_PATH*100-16))  //FindStrEx
					swprintf(retstring,L"%s|%d,%d,%d",retstring,retIndex,x,y);
				else if(len==0)//FindStrEx
					swprintf(retstring,L"%d,%d,%d",retIndex,x,y);
				straddr++;
				if(wcsstr(straddr,StrBuffer[j])&&len<(MAX_PATH*100-16))//继续往后找,如果还找到存在,重置变量j,继续返回坐标
					j--;
				else
					straddr=NULL;
			}
		}
	}

	if(strfind==0)
	{
		retIndex=-1;
		x=-1;
		y=-1;
	}
	return retIndex;
}

//相似度匹配
bool MyFindPictureClass::comparaStrTosim(int strwide,int strhight,int &RetX,int &RetY,int startX,int startY,float simi,int nstrcount)
//bool MyFindPictureClass::comparaStrTosim(COLORREF pLoadstrBuffer[11][MAX_PATH-10],int strwide,int strhight,int &RetX,int &RetY,int startX,int startY,float simi,int nstrcount)
{

	return true;
}


int MyFindPictureClass::ocr(PVOID pbuffer,HWND hwnd,int left, int top, int right, int bottom, wchar_t * color, double simi,wchar_t * retstr,pMyDictInfo *MyDictCount,int *nMyDictCountsize,int NowUsingDictIndex,DWORD (*ColorDataInfo)[2000],int type)
{

	//TSRuntime::add_log( "TS_OCR-start");
	int index = 0;
	int retIndex=-1;
	bool isFind=false;
	//得到当前字库的下标
	usingindex=NowUsingDictIndex;
	//得到当前字库个数
	dictindex=nMyDictCountsize[usingindex];


	if(dictindex<=0)  //字库个数==0就退出
		return isFind;

	//处理多个色偏
	int col=0;
	wchar_t colorBuffer[MAX_PATH][16];//存储多个字符串
	for(int i=0;i<MAX_PATH;i++)
	{
		memset(colorBuffer[i],0,16);
	}

	short nstr=0;//要查找色偏的个数  ,最多支持(MAX_PATH)个色偏
	while(*color)
	{
		if(*color!=L'|')
		{
			if(col>13)
				return retIndex;
			colorBuffer[nstr][col++]=*color;
			color++;
			continue;
		}
		else
		{
			if(col<1)
			{
				color++;
				continue;
			}
			nstr++;
			col=0;
			color++;
		}
	}
	if(col>=1)
		nstr++;

	ncolors=nstr;//记录下色偏个数

	//int t=::GetTickCount();
	for(int i=0;i<nstr;i++)
	{
		WCHAR colorR[4]={0};//字的颜色
		WCHAR colorG[4]={0};//字的颜色
		WCHAR colorB[4]={0};//字的颜色

		WCHAR coloroffR[4]={0};//偏色
		WCHAR coloroffG[4]={0};//偏色
		WCHAR coloroffB[4]={0};//偏色
		int count=0;
		int colorL=0;

		//StrTrimW(colorBuffer[i],L" ");//移除前后空格符

		while(colorBuffer[i][colorL]!=L'-')
		{
			if(count>6)	//判断是否是非法颜色值
				return retIndex;
			if(count<2)
				colorR[count]=colorBuffer[i][colorL++];
			else if(count>1&&count<4)
				colorG[count-2]=colorBuffer[i][colorL++];
			else if(count>3)
				colorB[count-4]=colorBuffer[i][colorL++];
			count++;

		}
		//将传入的RGB转换为BGR
		WCHAR colorBGR[16]={0};//字的颜色
		swprintf(colorBGR,L"%s%s%s",colorB,colorG,colorR);
		count=0;
		colorL++;

		while(colorBuffer[colorL]!=L'\0')
		{
			if(count>=6) //判断是否是非法色偏值
				break;
			if(count<2)
				coloroffR[count]=colorBuffer[i][colorL++];
			else if(count>1&&count<4)
				coloroffG[count-2]=colorBuffer[i][colorL++];
			else if(count>3)
				coloroffB[count-4]=colorBuffer[i][colorL++];
			count++;
		}

		//将传入的RGB转换为BGR
		WCHAR coloroffBGR[16]={0};//偏色
		swprintf(coloroffBGR,L"%s%s%s",coloroffB,coloroffG,coloroffR);

		strColors[i]=wcstol(colorBGR,NULL,16);//整形颜色值
		strColoroff[i]=wcstol(coloroffBGR,NULL,16);//整形偏色值
		m_colorOffR[i] = GetRValue(strColoroff[i]);
		m_colorOffG[i] = GetGValue(strColoroff[i]);
		m_colorOffB[i] = GetBValue(strColoroff[i]);
		//USES_CONVERSION;
		//TSRuntime::add_log( "TS_OCR-coloroffBGR:%s,strColors:%s",W2A(coloroffBGR),W2A(colorBGR));
	}

	if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())//normol找图
	{
		RECT rec;
		RECT clientrec;
		::GetClientRect(m_parenthwnd,&clientrec);
		int clienthight=clientrec.bottom-clientrec.top;
		int clientwide=clientrec.right-clientrec.left;
		::GetWindowRect(m_parenthwnd,&rec);
		POINT point;
		point.x=0;
		point.y=0;
		::ClientToScreen(m_parenthwnd,&point);
		m_Left=point.x;
		m_Top=point.y;
		m_Right=rec.right;
		m_bottom=rec.bottom;

		if(left<0)
			left=0;
		if(left >= clientwide)
			left = clientwide-1;

		if(top<0)
			top=0;
		if(top >= clienthight)
			top = clienthight-1;

		if(right >= clientwide)
			right=clientwide-1;

		if(bottom >= clienthight)
			bottom=clienthight-1;

		if(m_Left<0)
		{
			if((left+m_Left)<=0)//越界
				left=0;
			else
				left=m_Left+left;
		}
		else
			left=m_Left+left;

		if(m_Top>=0)
			top=m_Top+top;

		if(m_Right >= ::GetSystemMetrics(SM_CXSCREEN))
		{
			if((right+m_Left)>::GetSystemMetrics(SM_CXSCREEN))
				right=::GetSystemMetrics(SM_CXSCREEN)-1;
			else
				right=right+m_Left;
		}
		else
			right=right+m_Left;

		if(m_bottom >= ::GetSystemMetrics(SM_CYSCREEN))
		{
			if((bottom+m_Top)>=::GetSystemMetrics(SM_CYSCREEN))
				bottom=::GetSystemMetrics(SM_CYSCREEN)-1;
			else
				bottom=bottom+m_Top;
		}
		else
			bottom=bottom+m_Top;

		m_Left = left;
		m_Top = top;
		m_Right = right;
		m_bottom = bottom;

	}
	else
	{
		RECT clientrec;
		if(ColorDataInfo!=NULL)
			::GetClientRect(m_hwnd,&clientrec);
		else//gdi
			::GetClientRect(hwnd,&clientrec);
		int clienthight=clientrec.bottom-clientrec.top;
		int clientwide=clientrec.right-clientrec.left;
		m_Left = left;
		m_Top = top;
		m_Right = right;
		m_bottom = bottom;

		if(left<0)
			m_Left = left =0;
		if(left >= clientwide)
			m_Left = clientwide-1;

		if(top<0)
			m_Top = top =0;
		if(top >= clienthight)
			m_Top = clienthight-1;

		if(right >= clientwide)
			m_Right=clientwide-1;

		if(bottom >= clienthight)
			m_bottom=clienthight-1;
	}
	RECT rc;
	rc.bottom=m_bottom;
	rc.left=m_Left;
	rc.right=m_Right;
	rc.top=m_Top;

	if(ColorDataInfo!=NULL)
	{
		wndWidth=m_Right-m_Left;
		wndHeight=m_bottom-m_Top;
		//DX模式找字
		if(!getDXBitmap(pbuffer,ColorDataInfo,1))
		{
			return retIndex;
		}
	}	
	else
	{
		////GDI模式找字
		if(!getGDIBitmap(hwnd,rc,1))
		{
			return retIndex;
		}
	}


	GetStringInfo();

	DWORD Findstrings[MAX_PATH*10]={0};
	bool UnkownStr[MAX_PATH*10]={0};//未知字体

	/////////////////// 处理多张图 /////////////////////
	int indexstr=0;
	int findstrX=-1;
	int findstrY=-1;
	int X=0;
	int Y=0;
	for (int i=0;i<dictindex;i++)
	{
		if(simi==1.0)
		{
			//使用指定字库
			int nstrvaluelen=pWndBmpStr[MyDictCount[usingindex][i].nstrvalue[0]].size();
			for(int n=0;n<nstrvaluelen;n++)
			{
				findstrX=pWndBmpStr[MyDictCount[usingindex][i].nstrvalue[0]].at(n).x;
				findstrY=pWndBmpStr[MyDictCount[usingindex][i].nstrvalue[0]].at(n).y;
				if(findstrX==-1||findstrY==-1)
					continue;
				if(findstrX==42&&findstrY==100&&i==36)
					int aadf=1;

				//if(i==(dictindex-1))//确定是否最后一次循环
				//	IsLastFindStr=true;
				//if(MyDictCount[usingindex][i].strwide==2)
					//int aadd=0;
				//TSRuntime::add_log( "TS_OCR-findstrX:%d,findstrY:%d",findstrX,findstrY);
				if(comparaStr(MyDictCount[usingindex][i].nstrvalue,MyDictCount[usingindex][i].strwide,MyDictCount[usingindex][i].strhight,X,Y,findstrX,findstrY,simi,MyDictCount[usingindex][i].nstrcount))	
				{
					int swide=MyDictCount[usingindex][i].strwide;
					int shight=MyDictCount[usingindex][i].strhight;

					for(int k=0;k<=swide;k++)//判断是否是非正规字体
					{
						if((pWndBmpStrxy[Y-1][X-1]!=0&&pWndBmpStrxy[Y-1][X-1]>=16)&&shight>=11)//左上角
						{
							UnkownStr[indexstr]=true;//非正规字体标识
							break;
						}
						if(pWndBmpStrxy[Y-1][X+k]!=0&&pWndBmpStrxy[Y-1][X+k]>=1024)
						{
							UnkownStr[indexstr]=true;//非正规字体标识
							break;
						}
					}

					if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())//normol模式
					{
						POINT pt;
						pt.x=X+m_Left;
						pt.y=Y+m_Top;
						::ScreenToClient(m_parenthwnd,&pt);
						addrxy[indexstr][0]=pt.x;
						addrxy[indexstr][1]=pt.y;
					}
					else
					{
						addrxy[indexstr][0]=X+m_Left;
						addrxy[indexstr][1]=Y+m_Top;
					}
					addrxy[indexstr][2]=swide;
					addrxy[indexstr][3]=shight;

					//if(X==905&&Y==382)
					//	int aadf=1;
					//if(strcmp("_",MyDictCount[usingindex][i].mydistchar)==0)
					//	int ddf=1;
					///////取背景图 点阵坐标
					//USES_CONVERSION;
					Findstrings[indexstr]=(DWORD)(MyDictCount[usingindex][i].mydistchar);
					indexstr++;
					////重置背景图坐标,防止找重复图
					//pWndBmpStr[MyDictCount[usingindex][i].nstrvalue[0]].at(n).x=-1;   
					//pWndBmpStr[MyDictCount[usingindex][i].nstrvalue[0]].at(n).y=-1;	

				}
				if(indexstr>=MAX_PATH*10-1)
					break;
			}
			if(indexstr>=MAX_PATH*10-1)
				break;

		}
		else if(simi>0.1&&simi<1.0)
		{
			int nstrwide=MyDictCount[usingindex][i].strwide;
			for(int a=0;a<11;a++)
			{
				memset(TSRuntime::pLoadstrBuffer[a],0,nstrwide+1);
			}
			//将点阵信息还原图片矩阵信息
			byteTopic(MyDictCount[usingindex][i].nstrvalue,nstrwide);
			for(int j=1;j<2048;j++)
			{
				//使用指定字库
				int nstrvaluelen=pWndBmpStr[j].size();
				for(int n=0;n<nstrvaluelen;n++)
				{
					findstrX=pWndBmpStr[j].at(n).x;
					findstrY=pWndBmpStr[j].at(n).y;

					if(findstrX==-1||findstrY==-1)
					{
						continue;
					}

					if(comparaStrTosim(nstrwide,MyDictCount[usingindex][i].strhight,X,Y,findstrX,findstrY,simi,MyDictCount[usingindex][i].nstrcount))	
					{
						int swide=MyDictCount[usingindex][i].strwide;
						int shight=MyDictCount[usingindex][i].strhight;
						for(int k=0;k<=swide;k++)//判断是否是非正规字体
						{
							if((pWndBmpStrxy[Y-1][X-1]!=0&&pWndBmpStrxy[Y-1][X-1]>=16)&&shight>=11)//左上角
							{
								UnkownStr[indexstr]=true;//非正规字体标识
								break;
							}
							if(pWndBmpStrxy[Y-1][X+k]!=0&&pWndBmpStrxy[Y-1][X+k]>=1024)
							{
								UnkownStr[indexstr]=true;//非正规字体标识
								break;
							}
						}
						if(m_parenthwnd!=0&&m_parenthwnd!=::GetDesktopWindow())//normol模式
						{
							POINT pt;
							pt.x=X+m_Left;
							pt.y=Y+m_Top;
							::ScreenToClient(m_parenthwnd,&pt);
							addrxy[indexstr][0]=pt.x;
							addrxy[indexstr][1]=pt.y;
						}
						else
						{
							addrxy[indexstr][0]=X+m_Left;
							addrxy[indexstr][1]=Y+m_Top;
						}
						addrxy[indexstr][2]=swide;
						addrxy[indexstr][3]=shight;
						/////取背景图 点阵坐标
						//wsprintf(retstr,L"%s%s",retstr,A2W(TSRuntime::MyDictCount[usingindex][i].mydistchar));
						//Findstrings[indexstr]=(DWORD)(TSRuntime::MyDictCount[usingindex][i].mydistchar);
						Findstrings[indexstr]=(DWORD)(MyDictCount[usingindex][i].mydistchar);
						indexstr++;
						////重置背景图坐标,防止找重复图
						//pWndBmpStr[j].at(n).x=-1;
						//pWndBmpStr[j].at(n).y=-1;
					}
					if(indexstr>=MAX_PATH*10-1)
						break;
				}
				if(indexstr>=MAX_PATH*10-1)
					break;

			}
		}
	}

	////TSRuntime::add_log( "TS_OCR-排序-start");
	int addrxyBak[MAX_PATH*10][4]={0};
	wchar_t tempfindstr[256]={0};
	wchar_t tempfindstr1[256]={0};

	for(int i=0;i<indexstr-1;i++)
	{
		retstr[i];
		retstr[i+1];
		if(addrxy[i+1][1]<addrxy[i][1])
		{
			for(int j=i+1;j>0;j--)
			{
				if(addrxy[j][1]<addrxy[j-1][1])
				{
					//这里做交换操作
					int tem[10*2]={0};
					::memcpy(tem,addrxy[j-1],4*4);
					::memcpy(addrxy[j-1],addrxy[j],4*4);
					::memcpy(addrxy[j],tem,4*4);
					DWORD csTemp=Findstrings[j-1];
					Findstrings[j-1]=Findstrings[j];
					Findstrings[j]=csTemp;
					bool bret=UnkownStr[j-1];
					UnkownStr[j-1]=UnkownStr[j];
					UnkownStr[j]=bret;
					addrxyBak[j-1][0]=addrxy[j-1][0];
					addrxyBak[j-1][1]=addrxy[j-1][1];
					addrxyBak[j-1][2]=addrxy[j-1][2];
					addrxyBak[j-1][3]=addrxy[j-1][3];
					
				}
				else
				{
					addrxyBak[j][0]=addrxy[j][0];
					addrxyBak[j][1]=addrxy[j][1];
					addrxyBak[j][2]=addrxy[j][2];
					addrxyBak[j][3]=addrxy[j][3];
				}

			}
		}

		addrxyBak[i][0]=addrxy[i][0];
		addrxyBak[i][1]=addrxy[i][1];
		addrxyBak[i][2]=addrxy[i][2];
		addrxyBak[i][3]=addrxy[i][3];
	}

	for(int i=0;i<indexstr-1;i++)
	{
		if(abs(addrxy[i+1][1]-addrxy[i][1])<=11)
		{
			//这里做交换操作
			for(int m=i+1;m>0;m--)
			{
				if(abs(addrxy[m][1]-addrxy[m-1][1])<=11)
				{
					if(addrxy[m][0]<addrxy[m-1][0])	
					{
						int tem[10*2]={0};
						::memcpy(tem,addrxy[m-1],4*4);
						::memcpy(addrxy[m-1],addrxy[m],4*4);
						::memcpy(addrxy[m],tem,4*4);
						DWORD csTemp=Findstrings[m-1];
						Findstrings[m-1]=Findstrings[m];
						Findstrings[m]=csTemp;
						bool bret=UnkownStr[m-1];
						UnkownStr[m-1]=UnkownStr[m];
						UnkownStr[m]=bret;
						addrxyBak[m-1][0]=addrxy[m-1][0];
						addrxyBak[m-1][1]=addrxy[m-1][1];
						addrxyBak[m-1][2]=addrxy[m-1][2];
						addrxyBak[m-1][3]=addrxy[m-1][3];
						addrxyBak[m][0]=addrxy[m][0];
						addrxyBak[m][1]=addrxy[m][1];
						addrxyBak[m][2]=addrxy[m][2];
						addrxyBak[m][3]=addrxy[m][3];
					}	
					else
					{
						addrxyBak[m][0]=addrxy[m][0];
						addrxyBak[m][1]=addrxy[m][1];
						addrxyBak[m][2]=addrxy[m][2];
						addrxyBak[m][3]=addrxy[m][3];
					}
					
				}
				else
				{
					addrxyBak[m][0]=addrxy[m][0];
					addrxyBak[m][1]=addrxy[m][1];
					addrxyBak[m][2]=addrxy[m][2];
					addrxyBak[m][3]=addrxy[m][3];
					break;
				}
			}

		}
	}
	if(indexstr>=1)
	{
		addrxyBak[indexstr-1][0]=addrxy[indexstr-1][0];
		addrxyBak[indexstr-1][1]=addrxy[indexstr-1][1];
		addrxyBak[indexstr-1][2]=addrxy[indexstr-1][2];
		addrxyBak[indexstr-1][3]=addrxy[indexstr-1][3];
	}

	DWORD Strings[MAX_PATH*10]={0};//记录字符串指针地址
	DWORD nindex=0;//记录组合字个数
	DWORD nindexs[MAX_PATH*10]={0};//记录组合字的下标
	for(int i=0;i<indexstr;i++)
	{
		int len=strlen((char *)Findstrings[i]);
		len=len/sizeof(wchar_t);
		if(len>1)   //是组合字
		{
			Strings[nindex]=Findstrings[i];
			nindexs[nindex]=i;
			nindex++;
		}
	}

	if(nindex>0)
	{
		for(int i=0;i<nindex;i++)
		{
			for(int j=0;j<indexstr;j++)
			{
				if(nindexs[i]!=j)
				{
					char *t1=(char *)Strings[i];
					char *t2=(char *)Findstrings[j];
					char *findchar=strstr((char *)Strings[i],(char *)Findstrings[j]);
					if(findchar!=NULL)
					{
						if(addrxyBak[nindexs[i]][0]<=addrxyBak[j][0]&&
							(addrxyBak[nindexs[i]][0]+addrxyBak[nindexs[i]][2])>=(addrxyBak[j][0]+addrxyBak[j][2])&&
							addrxyBak[nindexs[i]][1]==addrxyBak[j][1])
						{
							for(int n=0;n<indexstr-j;n++)
							{
								addrxyBak[j+n][0]=addrxyBak[j+n+1][0];
								addrxyBak[j+n][1]=addrxyBak[j+n+1][1];
								addrxyBak[j+n][2]=addrxyBak[j+n+1][2];
								Findstrings[j+n]=Findstrings[j+n+1];
							}
							indexstr--;
							nindexs[i+1]--;
						}
					}
				}
			}
		}
	}

	//char tt[MAX_PATH]={0};
	//for(int i=0;i<indexstr;i++)
	//{
	//	if(type==0)//OCR
	//	{
	//		::strcat(tt,(char *)Findstrings[i]);
	//	}
	//}
 
//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11
	int nstrs=0;
	for(int i=0;i<indexstr;i++)
	{
		int j=1;
		short hight=addrxyBak[i][3];
		short wide=addrxyBak[i][2];
		short Y=addrxyBak[i][1];
		short X=addrxyBak[i][0];
		bool iscontinuefind=false;
continuefind:
		//char *t=(char *)Findstrings[i];
		//char *tt=(char *)Findstrings[i+j];
		if(addrxyBak[i+j][0]<(X+wide)&&((addrxyBak[i+j][1]-Y)<hight))
		{
			bool bfind=false;
			for(int n=0;n<indexstr-i;n++)
			{
				if(addrxyBak[i+j+n][2]>wide&&bfind==false&&UnkownStr[i+j+n]==false)
				{
					//char *t1=(char *)Findstrings[i+n];
					//char *t2=(char *)Findstrings[i+j+n];
					addrxyBak[i+n][0]=addrxyBak[i+j+n][0];
					addrxyBak[i+n][1]=addrxyBak[i+j+n][1];
					addrxyBak[i+n][2]=addrxyBak[i+j+n][2];
					addrxyBak[i+n][3]=addrxyBak[i+j+n][3];
					UnkownStr[i+n]=UnkownStr[i+j+n];
					Findstrings[i+n]=Findstrings[i+j+n];
				}
				else
				{
					bfind=true;
					//char *t1=(char *)Findstrings[i+j+n];
					//char *t2=(char *)Findstrings[i+j+1+n];
					addrxyBak[i+j+n][0]=addrxyBak[i+j+1+n][0];
					addrxyBak[i+j+n][1]=addrxyBak[i+j+1+n][1];
					addrxyBak[i+j+n][2]=addrxyBak[i+j+1+n][2];
					addrxyBak[i+j+n][3]=addrxyBak[i+j+1+n][3];
					UnkownStr[i+j+n]=UnkownStr[i+j+1+n];
					Findstrings[i+j+n]=Findstrings[i+j+1+n];
				}
			}
			nstrs++;
			int dwnstr=indexstr-nstrs;
			if(dwnstr==0)
			{
				indexstr=i+1;
				break;
			}
			//char tempTetstr[MAX_PATH*100]={0};
			//for(int k=0;k<dwnstr;k++)
			//{
			//	::strcat(tempTetstr,(char *)Findstrings[k]);
			//}
			iscontinuefind=true;
			goto continuefind;
		}
		else if(!iscontinuefind)
			nstrs++;
		
	}
//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11//过滤字--2013-5-11

	//TSRuntime::add_log( "TS_OCR-排序-end");
	USES_CONVERSION;
	char tempTet[MAX_PATH*100]={0};
	int len=0;
	for(int i=0;i<indexstr;i++)
	{
		if(type==0)//OCR
		{
			::strcat(tempTet,(char *)Findstrings[i]);
		}
		else//OCREX
		{
			len=strlen(tempTet);
			if(len==0)
			{
				sprintf(tempTet,"%s,%d,%d",(char *)Findstrings[i],addrxyBak[i][0],addrxyBak[i][1]);
			}
			else
			{
				if(len<=(MAX_PATH*100-16))
					sprintf(tempTet,"%s|%s,%d,%d",tempTet,(char *)Findstrings[i],addrxyBak[i][0],addrxyBak[i][1]);
			}

		}
	}
	::wcscpy(retstr,A2W(tempTet));
	retIndex=indexstr;
	//for(int i=0;i<wndHeight;i++)
	//{
	//	delete [] pWndBmpStrxy[i];
	//	//delete [] pWndBmpBuffer[i];
	//	//pWndBmpBuffer[i] = NULL;
	//	pWndBmpStrxy[i]=NULL;
	//}
	//delete []  pWndBmpStrxy;
	////delete [] pWndBmpBuffer;
	//pWndBmpStrxy=NULL;
	////pWndBmpBuffer=NULL;

	//TSRuntime::add_log( "TS_OCR-end");
	return retIndex;
}


bool MyFindPictureClass::MySetDict(int nindex,wchar_t *dictpath,pMyDictInfo *MyDictCount,int *nMyDictCountsize,int &NowUsingDictIndex)//设置字库 加载字库
{
	bool isSet=false;
	BYTE MaxStrHight[MAX_PATH]={0};

	int indexstr=1;
	int col=0;
	HANDLE hfile=NULL;
	DWORD filesize=0,ReadFileSize=0;

	//if(!PathFileExists(dictpath))
	//	return	isSet;

	//if(nindex>=0&&nindex<20&&TSRuntime::MyDictCount[nindex]!=NULL)
	//delete []  TSRuntime::MyDictCount[nindex];
	if(nindex>=0&&nindex<20&&MyDictCount[nindex]!=NULL)
		delete []  MyDictCount[nindex];

	hfile=::CreateFile(dictpath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);//打开文件,
	if(hfile==INVALID_HANDLE_VALUE)
	{
		if(TSRuntime::IsShowErrorMsg)
		{
			wchar_t erro[MAX_PATH*2]={0};
			swprintf(erro,L"打开字库<%s>失败<错误信息=系统找不到指定文件.>",dictpath);
			MessageBox(NULL,erro,L"TC",NULL);
		}
		return false;
	}
	filesize=::GetFileSize(hfile,NULL);
	char *date=new char[filesize+1];
	::ReadFile(	hfile,date,filesize,&ReadFileSize ,NULL);
	if(ReadFileSize<1)
	{
		return false;
	}

	////////////////////根据换行个数计算点阵个数
	int nsize=0;
	int nadd=0;
	char* tdate=date;
	while(*tdate)
	{

		if(*tdate!='\n')
		{
			if(nadd>MAX_PATH*3)
			{
				CloseHandle(hfile);
				return isSet;
			}
			nadd++;
			tdate++;
		}
		else
		{
			nadd=0;
			tdate++;
			nsize++;
		}
	}

	if(nsize==0)
	{
		CloseHandle(hfile);
		return isSet;
	}
	////////////////////根据换行个数计算点阵个数

	pMyDictInfo mydict=new MYDICTINFO[nsize];

	char pathBuffer[MAX_PATH*3]={0};
	int picstr=0;
	bool isFind=false;
	while(*date)
	{

		if(*date!='\n')
		{
			if(col>MAX_PATH*3)
			{
				delete [] mydict;
				return isSet;
			}
			pathBuffer[col++]=*date;
			if((pathBuffer[0]<48 &&pathBuffer[0]<57) || (pathBuffer[0]<65 && pathBuffer[0]>65)) //判断是否到乱码
			{
				date++;
				continue;
			}
			date++;
		}
		else
		{
			if(col<8)
				break;

			char tempstr[MAX_PATH*3]={0};
			int index=0;
			while(pathBuffer[index]!='$')
			{
				if(index==MAX_PATH*3-1)
				{
					delete [] mydict;
					return isSet;
				}

				tempstr[index]=pathBuffer[index];
				index++;
			}
			if((index*4)%11!=0)
				mydict[dictindex].strwide=(index*4-1)/11; //记录当前字体字宽
			else
				mydict[dictindex].strwide=(index*4)/11; //记录当前字体字宽
			index++;
			int counta=0;
			char tempstrstr[16]={0};

			while(pathBuffer[index]!='$')
			{
				tempstrstr[counta]=pathBuffer[index];
				index++;
				counta++;
			}

			char *sfind=strrchr(pathBuffer,'$');
			sfind++;
			mydict[dictindex].strhight=atoi(sfind);//记录当前字体字高 
			MaxStrHight[mydict[dictindex].strhight]=mydict[dictindex].strhight;
			char bytestr[DICTLENGMAX]={0};
			//把字体点阵还原图色矩阵信息，并且存储起来
			dicthextobyte(tempstr,bytestr,index,mydict[dictindex].nstrcount);
			//if(strcmp(tempstrstr,"情")==0)
			//	int aad=0;
			for(int i=0;i<index/11;i++)
			{
				char Tstr1[11+1]={0};
				strncpy(Tstr1,&bytestr[i*11],11);
				mydict[dictindex].nstrvalue[i]=strtol(Tstr1,NULL,2);
			}
			strcpy(mydict[dictindex].mydistchar,tempstrstr);//字
			::memset(pathBuffer,0,MAX_PATH);
			col=0;
			date++;
			dictindex++; 
			picstr=0;
		}

	}

	//存储当前字库  
	if(nindex>=0&&nindex<20)
	{
		isSet=true;
		NowUsingDictIndex=nindex;
		usingindex=nindex;
		//TSRuntime::MyDictCount[nindex]=mydict;
		MyDictCount[nindex]=mydict;
		nMyDictCountsize[nindex]=dictindex;

	}
	else
	{
		delete []  mydict;
		return isSet;
	}

	//记录字库的最高字高
	for(int i=MAX_PATH;i>0;i--)
	{
		if(MaxStrHight[i]!=0&&MaxStrHight[i]<20)
		{
			TSRuntime::nMaxStrHight[nindex]=MaxStrHight[i];
			break;
		}
	}
	if(TSRuntime::nMaxStrHight[nindex]==0)//如果字库中的字,字高都高于20,那设置一个默认字高值为16
		TSRuntime::nMaxStrHight[nindex]=16;

	CloseHandle(hfile);

	return isSet;
}