////////////////////////////////////////////////////////////////////////////////////

#if !defined(MYFINDPICTURECLASS_H)
#define MYFINDPICTURECLASS_H

#include <windows.h>
#include "d3d9.h"
#include <GdiPlus.h>
#include <GdiplusBitmap.h>
#include <vector>

class MyFindPictureClass
{
public:
	MyFindPictureClass(void);
	~MyFindPictureClass(void);

	enum RUNTYPE{TurnUP=0,TurnDOWN,TurnLEFT,TurnRIGHT,TurnERRORR};
	//LPDIRECT3DDEVICE9 m_pDxdevice;
	//HWND              m_Wnd;
	bool isprocessColor;//说明是找色或者取色
	int loadWidth;
	int loadHeight;
	HWND m_parenthwnd;
	HBITMAP hDXBitmap;
	HDC hDXMemDC;
	HWND m_hwnd;
	DWORD m_displayMode;//图色模式
	bool IsLastFindStr;//是否最后一次查找

	//COLORREF pWndBmpBuffer[2000][2000];
	COLORREF** pWndBmpBuffer; 
	//DWORD ColorDataInfo[2000][2000];
	BYTE * pBuffer;
	//COLORREF** pColorBuffer; 
	int wndWidth;
	int wndHeight;
	int nbuffersize;
	PVOID pbuffer;

	bool SaveGDIBitmap(HWND hWnd,RECT rect,wchar_t *savepath);

	bool SaveBitmapToFile(HBITMAP hBitmap, LPCWSTR lpFileName);

	bool CopyScreenToBitmap(LPRECT lpRect,wchar_t *savepath);

	DWORD GetBkMouseShape(HCURSOR bkmouseshape=NULL);//默认bkmouseshape为空时为前台模式, bkmouseshape有值的时候是后台模式

	bool loadBitmap(wchar_t* path);
	//////////////////找图   支持的是24位位图/////////////////////
	////  图片格式错误或者没找到图片，函数返回-1	
	////  参数             说明
	////  hwnd           窗口的句柄,为NULL表示前台查找,不为NULL表示后台查找
	////  left           左上角x
	////  top            左上角y
	////  right          右下角x
	////  bottom         右下角y
	////  path           图片的路径,支持多张图片,使用"|"隔开
	////  colorOffset    色偏 例如"102030"
	////  simi           相似度
	////  dir            检索方式
	////  xpos           返回的图片的左上角x坐标
	////  ypos           返回的图片的左上角y坐标
	int findPicture(PVOID pbuffer,HWND hwnd,int left, int top, int right, int bottom, wchar_t* path, wchar_t *color, double simi, int dir, long &x,long &y,DWORD (*ColorDataInfo)[2000],wchar_t *retstring=NULL,int type=0);

	/////// 颜色内存处理。。 提高速度 //////////////////////
	bool processCreenWindow(int color,int left,int top,int right,int bottom,double simi,int dir,int &x,int &y,wchar_t* colorStr=NULL,bool isMulitColor=false);

	/////// 后台颜色内存处理。。 提高速度 //////////////////////
	int processColor(PVOID pbuffer,HWND hwnd,int &color,int left,int top,int right,int buttom,double simi,int dir,long &x,long &y,DWORD (*ColorDataInfo)[2000],wchar_t *colorstr=NULL,wchar_t *retstring=NULL,wchar_t *MultiColor=NULL);

	//将BMP图转JPG图
	bool   BMP2JPG(wchar_t*   strBMPFile,wchar_t*   strJPGFile);
	//将BMP图转PNG图
	bool   BMP2PNG(wchar_t*   strBMPFile,wchar_t*   strPNGFile);

	//////////////////////////////////////////////////////////////////////////////////////找字算法
	int   strColor;//字的颜色
	int   strColors[MAX_PATH];//字的颜色
	int   strColoroff[MAX_PATH];//字的色偏
	int ncount;//记录位图加载的点阵个数
	int addrxy[MAX_PATH*10][4];
	int dictindex;//记录当前字库的数量
	int usingindex;	//当前使用字库的下标
	std::vector<POINT> pWndBmpStr[2048];
	short **pWndBmpStrxy;//存储当前图片的所有坐标对于的整形值
	wchar_t SetPath[MAX_PATH];
	//wchar_t SetPicPwdString[MAX_PATH];
	//COLORREF **pLoadstrBuffer;
	//COLORREF pLoadstrBuffer[11][MAX_PATH-10];

	void hextobyte(char * hexStr,char * byteStr);//将十六进制字符串转换成二进制字符串
	//void byteTopic(short strvalue[MAX_PATH],int strwide,COLORREF pLoadstrBuffer[11][MAX_PATH-10]);//二进制数值还原成图片MAX_PATH
	void byteTopic(short strvalue[MAX_PATH],int strwide);//二进制数值还原成图片MAX_PATH
	//bool comparaStrTosim(COLORREF pLoadstrBuffer[11][MAX_PATH-10],int strwide,int strhight,int &RetX,int &RetY,int startX,int startY,float simi,int nstrcount);	//相似度匹配
	bool comparaStrTosim(int strwide,int strhight,int &RetX,int &RetY,int startX,int startY,float simi,int nstrcount);	//相似度匹配
	bool comparaStr(short pLoadstrBufferxy[MAX_PATH],int strwide,int strhight,int &RetX,int &RetY,int startX,int startY,float simi,int nstrcount);//匹配字符串
	void dicthextobyte(char * hexStr,char * byteStr,int &hexlen,short &nstrinfo);// 字库信息转二进制
	bool GetStringInfo();//获取位置点阵信息
	//获取位置点阵信息,和GetStringInfo区别是,多了一层X坐标的扫描,排除多余无效的点阵信息
	bool GetStringInfo1();
	bool MySetDict(int nindex,wchar_t *dictpath,pMyDictInfo *MyDictCount,int *nMyDictCountsize,int &NowUsingDictIndex);//设置字库 加载字库
	bool MyUseDict(int Useindex,pMyDictInfo *MyDictCount,int &NowUsingDictIndex);	   //使用字库
	bool MyDeleteDict(int deleteindex,pMyDictInfo *MyDictCount,int *nMyDictCountsize,int &NowUsingDictIndex);	//删除字库
	int ocr(PVOID pbuffer,HWND hwnd,int left, int top, int right, int bottom, wchar_t * color_format, double simi,wchar_t * retstr,pMyDictInfo *MyDictCount,int *nMyDictCountsize,int NowUsingDictIndex,DWORD (*ColorDataInfo)[2000],int type=0);
	int FindStr(PVOID pbuffer,HWND hwnd,int left, int top, int right, int bottom, wchar_t* strings, wchar_t* color, double simi, long &x,long &y,pMyDictInfo *MyDictCount,int *nMyDictCountsize,int NowUsingDictIndex,DWORD (*ColorDataInfo)[2000],wchar_t *retstring=NULL,int type=0);
	int FindStrFast(PVOID pbuffer,HWND hwnd,int left, int top, int right, int bottom, wchar_t* strings, wchar_t* color, double simi, long &x,long &y,pMyDictInfo *MyDictCount,int *nMyDictCountsize,int NowUsingDictIndex,DWORD (*ColorDataInfo)[2000],wchar_t *retstring=NULL,int type=0);
	//pMyDictInfo mydict;
	//////////////////////////////////////////////////////////////////////////////////////找字算法
private:

	bool IsFindPicNext;
	COLORREF** pLoadBmpBuffer;
	//COLORREF** pWndBmpBuffer;
	int		   ncolors;	     //色偏个数
	BYTE       m_colorOffsR; // R的偏移量
	BYTE       m_colorOffsG; // G的偏移量
	BYTE       m_colorOffsB; // B的偏移量
	BYTE       m_colorOffR[MAX_PATH]; // R的偏移量
	BYTE       m_colorOffG[MAX_PATH]; // G的偏移量
	BYTE       m_colorOffB[MAX_PATH]; // B的偏移量
	/*double     m_sim;        // 相似度*/  //未使用该变量
	int        m_Dir;        // 检索方式
	int		   m_simColor;	 //	相似度的颜色边界值

	//////////////////////////////存储传进来的四点的范围/////////////
	int        m_Left;       // 左上点
	int        m_Top ;       // 顶端点
	int        m_Right ;     // 右下点
	int        m_bottom ;    // 底端点

	int		xpos_offset;
	int		ypos_offset;

	///////////////////////////// 助记透明图的处理 /////////////
	int        m_helpXpoint;
	int        m_helpYpoint; 
    //bool catchWindowBitmap();
 	//bool processBKWindows(HWND src_hWnd);

	//配合BMP2JPG使用
	int   GetEncoderClsid(const   WCHAR*   format,   CLSID*   pClsid);
	bool getGDIBitmap(HWND wnd,RECT rc,int typemode=0);//初始默认为找图	,typemode=0
	bool getDXBitmap(PVOID pbuffer,DWORD (*ColorDataInfo)[2000],int typemode=0);//初始默认为找图	,typemode=0
	bool findImage(wchar_t* path,long &x,long &y,int type=0);

	bool comparabmp(int startX,int startY,int endX,int endY);
	//////////////// 检索 方式 //////////////////
	//////////////////////////////////0:从左到右,从上倒下/////////////////////
	bool leftToRightFromTop(long &x,long &y);
	//////处理背景色为透明的图片////////////////
	bool comparaLeftToRightFromTop(int h,int w,int startX,int startY);
	//////处理背景色为不透明的图片////////////////
	bool comparaLeftToRightFromTopEx(int h,int w);
	
	////////////////////////////////////1:从左到右,从下倒上//////////////////
	bool leftToRightFromBottom(long &x,long &y);
	////////处理背景色为透明的图片////////////////
	bool comparaLeftToRightFromBottom(int h,int w,int startX,int startY);
	////////处理背景色为不透明的图片////////////////
	bool comparaLeftToRightFromBottomEx(int h,int w);

	////////////////////////////////////2:从右到左,从上倒下//////////////////
	bool rightToLeftFromTop(long &x,long &y);
	////////处理背景色为透明的图片////////////////
	bool comparaRightToLeftFromTop(int h,int w,int startX,int startY);
	////////处理背景色为不透明的图片////////////////
	bool comparaRightToLeftFromTopEx(int h,int w);	
	
	//////////////////////////////////3:从右到左,从下倒上///////////////////
	bool rightToLeftFromBottom(long &x,long &y);
	////////处理背景色为透明的图片////////////////
	bool comparaRightToLeftFromBottom(int h,int w,int startX,int startY);
	////////处理背景色为不透明的图片////////////////
	bool comparaRightToLeftFromBottomEx(int h,int w);

	COLORREF RgbToBgb(__in COLORREF rgb);

	bool FindMultiColor(wchar_t *MultiColor,int i,int j,int Bottom,int right,int help_simi);
	
};
#endif