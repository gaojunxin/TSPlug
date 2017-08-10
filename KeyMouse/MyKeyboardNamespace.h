///////////////////////////////////////
#if !defined(MYKEYBOARDNAMESPACE_H)
#define MYKEYBOARDNAMESPACE_H

//键盘功能类
class MyKeyboardNamespace
{
private:
	//模拟按键的状态
	//KEY_DOWN  键盘处于按下状态
	//KEY_UP    键盘处于弹起的状态
	//KEY_DOWNANDUP  键盘处于按下弹起的状态
    enum KeyboardEvent
    {
        KEY_DOWN
        ,KEY_UP
        ,KEY_DOWNANDUP
    };

public:
	//方法描述:键盘按下弹起
    //返回值:BOOL类型
    //_TRUE表示成功,_FALSE表示失败
    //
    //参数说明:
    //参数名称        参数类型       说明
    //list            MyDataList *   键盘按下弹起什么键以及键盘按下弹起具体的次数
    //ret             _variant_t *      接收返回值传出去
	 bool keyPress(DWORD keycode);

	//方法描述:键盘按下
    //返回值:BOOL类型
    //_TRUE表示成功,_FALSE表示失败
    //
    //参数说明:
    //参数名称        参数类型       说明
    //list            MyDataList *   键盘按下什么键以及键盘按下具体次数
    //ret             _variant_t *      接收返回值传出去
	 bool keyDown(DWORD keycode);

	//方法描述:键盘弹起
    //返回值:BOOL类型
    //_TRUE表示成功,_FALSE表示失败
    //
    //参数说明:
    //参数名称        参数类型       说明
    //list            MyDataList *   键盘弹起什么键以及键盘弹起的次数
    //ret             _variant_t *      接收返回值传出去
	 bool keyUp(DWORD keycode);
    
    //方法描述:按下键盘发送一串字符串
    //返回值:BOOL类型
    //_TRUE表示成功,_FALSE表示失败
    //
    //参数说明:
    //参数名称        参数类型       说明
    //list            MyDataList *   键盘发送的字符串
    //ret             _variant_t *      接收返回值传出去
	 bool sendkeyString(wchar_t *strs,DWORD delays);

	//方法描述:暂停程序等待任意按键后继续
    //返回值:BOOL类型
    //_TRUE表示成功,_FALSE表示失败
    //
    //参数说明:
    //参数名称        参数类型       说明
    //list            MyDataList *   键盘发送的字符串
    //ret             _variant_t *      接收返回值传出去
	 bool waitkeypress(DWORD keycode,DWORD delay);

	 //键盘功能类的构造函数
	 MyKeyboardNamespace(void);

	 //键盘功能类的析构函数
	 ~MyKeyboardNamespace(void);
public:
	 DWORD delaytime;
};

#endif