#if !defined(MYMOUSENAMESPACE_H)
#define MYMOUSENAMESPACE_H
//鼠标功能类
class MyMouseNamespace
{
private:
	//鼠标按键状态枚举 
	enum MouseMode
	{
		 LEFTCLICK   =0    //左键单击
		,RIGHTCLICK        //右键单击
		,MIDDLECLICK       //中键单击
		,LEFTDOWN          //左键按下
		,LEFTUP            //左键弹起
		,RIGHTDOWN         //右键按下
		,RIGHTUP           //右键弹起
		,MIDDLEDOWN        //中键按下
		,MIDDLEUP          //中键弹起
		,LEFTDOUBLECLICK   //左键双击
		,RIGHTDOUBLECLICK  //右键双击
	};

	//鼠标滑轮方向
	enum MouseWheel
	{
		 WHEEL_UP   =0       //向上滑动
		,WHEEL_DOWN          //向下滑动
	};

	//方法描述:鼠标单击双击模式以及具体的执行次数
    //返回值:BOOL类型
    //_TRUE表示成功,_FALSE表示失败
    //
    //参数说明:
    //参数名称        参数类型       说明
    //mousemode       MouseMode *   传进来鼠标单击双击模式
    //nEcute          int           执行单击或者双击几次
	void mouseAction(MouseMode mousemode, int nEcute=1);

	//方法描述:滚轮的实现   
    //返回值:BOOL类型
    //_TRUE表示成功,_FALSE表示失败
    //
    //参数说明:
    //参数名称        参数类型       说明
	//direction       MouseWheel     鼠标中键模式
	//nMove           int            鼠标中键向上还是向下具体移动多少值
	void mouseMidleWheel(MouseWheel direction,int  nMove=1);

public:
	//方法描述:鼠标移动的相对位置
    //返回值:BOOL类型
    //_TRUE表示成功,_FALSE表示失败
    //
    //参数说明:
    //参数名称        参数类型       说明
    //list            MyDataList *   传进来函数的参数,鼠标的x,y值
    //ret             _variant_t *      接收返回值传出去
	 bool  mouseMvrelative(DWORD x_pos,DWORD y_pos);

	//方法描述:鼠标移动的绝对位置
    //返回值:BOOL类型
    //_TRUE表示成功,_FALSE表示失败
    //
    //参数说明:
    //参数名称        参数类型       说明
    //list            MyDataList *   传进来函数的参数,鼠标的x,y值
    //ret             _variant_t *      接收返回值传出去
	 bool   mouseMove(DWORD x_pos,DWORD y_pos);

	//方法描述:获取当前鼠标的当前位置坐标
	//返回值:BOOL类型
    //_TRUE表示成功,_FALSE表示失败
    //
    //参数说明:
    //参数名称        参数类型       说明
    //ret             _variant_t *      接收返回值传出去
	 bool GetmousePoint(DWORD &x_pos,DWORD &y_pos);
	////////////////////////获取鼠标形状特征码 ///////////////
	bool getmouseshape(DWORD &dwshape);

	////////等待鼠标任意操作//////////////////
	//static bool waitClick(MyDataList * list,_variant_t * ret);

	// 获取上一次鼠标的动作
	//bool getLastClick(MyDataList * list,_variant_t * ret);

	 bool leftClick();
	 bool leftDoubleClick();
	 bool leftDown();
	 bool leftUp();

	 bool rightClick();
	 bool rightDoubleClick();
	 bool rightDown();
	 bool rightUp();

	 bool middleClick();
	 bool middleDown();
	 bool middleUp();
	// 向下滚动
	 bool mouseSheetDown();
	// 向上滚动
	 bool mouseSheetUp();
	 //鼠标功能类的构造函数
	 MyMouseNamespace();
	 //鼠标功能类的析构函数
	 ~MyMouseNamespace();
public:
	DWORD delaytimes;

};


#endif
