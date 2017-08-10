// TSPlugInterFace.h : CTSPlugInterFace 的声明

#pragma once
#include "resource.h"       // 主符号

#include "TSPlug_i.h"
#include "DXBind.h"
#include "TSRuntime.h"
#include "TSWindowsAPI.h"
#include "TSMemoryAPI.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

// CTSPlugInterFace
class ATL_NO_VTABLE CTSPlugInterFace :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTSPlugInterFace, &CLSID_TSPlugInterFace>,
	public IDispatchImpl<ITSPlugInterFace, &IID_ITSPlugInterFace, &LIBID_TSPlugLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTSPlugInterFace()
	{
		  
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TSPLUGINTERFACE)


BEGIN_COM_MAP(CTSPlugInterFace)
	COM_INTERFACE_ENTRY(ITSPlugInterFace)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	DXBind gDxObj; 
	TSWindowsAPI  gWindowObj;
	TSMemoryAPI   gMemoryObj;
	STDMETHOD(BindWindow)(LONG hwnd, BSTR display, BSTR mouse, BSTR keypad, LONG mode, LONG* nret);
	STDMETHOD(BindWindowEx)(LONG hwnd, BSTR display, BSTR mouse, BSTR keypad, BSTR publics, LONG mode, LONG* nret);
	STDMETHOD(UnBindWindow)(LONG* nret);
	STDMETHOD(KeyDown)(LONG vk_code, LONG* nret);
	STDMETHOD(KeyUp)(LONG vk_code, LONG* nret);
	STDMETHOD(KeyPress)(LONG vk_code, LONG* nret);
	STDMETHOD(LeftDown)(LONG* nret);
	STDMETHOD(LeftUp)(LONG* nret);
	STDMETHOD(LeftClick)(LONG* nret);
	STDMETHOD(MoveTo)(LONG x, LONG y, LONG* nret);
	STDMETHOD(FindPic)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR pic_name, BSTR delta_color, DOUBLE sim, LONG dir, VARIANT* intX, VARIANT* intY, LONG* nret);
	STDMETHOD(RightClick)(LONG* nret);
	STDMETHOD(RightDown)(LONG* nret);
	STDMETHOD(FindColor)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR color, DOUBLE sim, LONG dir, VARIANT* intX, VARIANT* intY, LONG* nret);
	STDMETHOD(RightUp)(LONG* nret);
	STDMETHOD(GetColor)(LONG x, LONG y, BSTR* RetStrColor);
	STDMETHOD(GetCursorShape)(BSTR* RetCursorShape);
	STDMETHOD(SetPath)(BSTR path, LONG* nret);
	STDMETHOD(TSGuardProtect)(LONG enable, BSTR type,LONG* nret);
	STDMETHOD(KeyPressStr)(BSTR key_str, LONG delay, LONG* nret);
	STDMETHOD(SendString)(LONG hwnd, BSTR str, LONG* nret);
	STDMETHOD(SendString2)(LONG hwnd, BSTR str, LONG* nret);
	STDMETHOD(KeyPressChar)(BSTR key_str, LONG* nret);
	STDMETHOD(KeyDownChar)(BSTR key_str, LONG* nret);
	STDMETHOD(KeyUpChar)(BSTR key_str, LONG* nret);
	STDMETHOD(GetCursorPos)(VARIANT* x, VARIANT* y, LONG* nret);
	STDMETHOD(MoveR)(LONG rx, LONG ry, LONG* rnet);
	STDMETHOD(Ver)(BSTR* retver);
	STDMETHOD(GetPath)(BSTR* retpath);
	STDMETHOD(MiddleClick)(LONG* nret);
	STDMETHOD(WheelDown)(LONG* nret);
	STDMETHOD(WheelUp)(LONG* nret);
	STDMETHOD(Capture)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR file, LONG* nret);
	STDMETHOD(CaptureJpg)(LONG x1, LONG y1, LONG x2, LONG y2,BSTR file, LONG* nret);
	STDMETHOD(CapturePng)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR file, LONG* nret);
	STDMETHOD(LockInput)(LONG lock, LONG* nret);
	STDMETHOD(Ocr)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR color_format, FLOAT sim, BSTR* retstr);
	STDMETHOD(SetDict)(LONG index, BSTR file, LONG* nret);
	STDMETHOD(UseDict)(LONG index, LONG* nret);
	STDMETHOD(ClearDict)(LONG index, LONG* nret);
	STDMETHOD(FindStr)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, FLOAT sim, VARIANT* intX, VARIANT* intY, LONG* nret);
	STDMETHOD(FindStrFast)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, FLOAT sim, VARIANT* intX, VARIANT* intY, LONG* nret);
	STDMETHOD(GetNowDict)(LONG* nret);
	STDMETHOD(GetBasePath)(BSTR* retpath);
	STDMETHOD(IsDisplayDead)(LONG x1, LONG y1, LONG x2, LONG y2, LONG t, LONG* bret);
	STDMETHOD(FindPicEx)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR pic_name, BSTR delta_color, DOUBLE sim, LONG dir, BSTR* retstring);
	STDMETHOD(FindStrEx)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, DOUBLE sim, BSTR* retstring);
	STDMETHOD(FindStrFastEx)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, DOUBLE sim, BSTR* retstring);
	STDMETHOD(CaptureGif)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR file, LONG delay, LONG time, LONG* nret);
	STDMETHOD(DownCpu)(LONG rate, LONG* nret);
	STDMETHOD(SetKeypadDelay)(BSTR type, LONG delay, LONG* nret);
	STDMETHOD(SetMouseDelay)(BSTR type, LONG delay, LONG* nret);
	STDMETHOD(CmpColor)(LONG x, LONG y, BSTR color, DOUBLE sim, LONG* nret);
	STDMETHOD(SendStringIme)(BSTR str, LONG* nret);
	STDMETHOD(FindColorEx)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR color, DOUBLE sim, LONG dir, BSTR* retstring);
	STDMETHOD(EnumWindow)(LONG parent, BSTR title, BSTR class_name, LONG filter, BSTR* retstr);
	STDMETHOD(EnumWindowByProcess)(BSTR process_name, BSTR title, BSTR class_name, LONG filter, BSTR* retstring);
	STDMETHOD(EnumProcess)(BSTR name, BSTR* retstring);
	STDMETHOD(ClientToScreen)(LONG ClientToScreen, VARIANT* x, VARIANT* y, LONG* bret);
	STDMETHOD(FindWindow)(BSTR class_name,BSTR title,LONG* rethwnd);
	STDMETHOD(FindWindowByProcess)(BSTR process_name, BSTR class_name, BSTR title, LONG* rethwnd);
	STDMETHOD(FindWindowByProcessId)(LONG process_id, BSTR class_name, BSTR title, LONG* rethwnd);
	STDMETHOD(FindWindowEx)(LONG parent,BSTR class_name,BSTR title,LONG* rethwnd);
	STDMETHOD(GetClientRect)(LONG hwnd, VARIANT* x1, VARIANT* y1, VARIANT* x2, VARIANT* y2, LONG* nret);
	STDMETHOD(GetClientSize)(LONG hwnd, VARIANT* width, VARIANT* height, LONG* nret);
	STDMETHOD(GetForegroundFocus)(LONG* rethwnd);
	STDMETHOD(GetForegroundWindow)(LONG* rethwnd);
	STDMETHOD(GetMousePointWindow)(LONG* rethwnd);
	STDMETHOD(GetPointWindow)(LONG x, LONG y, LONG* rethwnd);
	STDMETHOD(GetProcessInfo)(LONG pid, BSTR* retstring);
	STDMETHOD(GetSpecialWindow)(LONG flag, LONG* rethwnd);
	STDMETHOD(GetWindow)(LONG hwnd, LONG flag, LONG* nret);
	STDMETHOD(GetWindowClass)(LONG hwnd, BSTR* retstring);
	STDMETHOD(GetWindowProcessId)(LONG hwnd, LONG* nretpid);
	STDMETHOD(GetWindowProcessPath)(LONG hwnd, BSTR* retstring);
	STDMETHOD(GetWindowRect)(LONG hwnd, VARIANT* x1, VARIANT* y1, VARIANT* x2, VARIANT* y2, LONG* nret);
	STDMETHOD(GetWindowState)(LONG hwnd, LONG flag, LONG* rethwnd);
	STDMETHOD(GetWindowTitle)(LONG hwnd, BSTR* rettitle);
	STDMETHOD(MoveWindow)(LONG hwnd, LONG x, LONG y, LONG* nret);
	STDMETHOD(ScreenToClient)(LONG hwnd, VARIANT* x, VARIANT* y,LONG* nret);
	STDMETHOD(SendPaste)(LONG hwnd, LONG* nret);
	STDMETHOD(SetClientSize)(LONG hwnd, LONG width, LONG hight, LONG* nret);
	STDMETHOD(SetWindowState)(LONG hwnd, LONG flag, LONG* nret);
	STDMETHOD(SetWindowSize)(LONG hwnd, LONG width, LONG height, LONG* nret);
	STDMETHOD(SetWindowText)(LONG hwnd, BSTR title, LONG* nret);
	STDMETHOD(SetWindowTransparent)(LONG hwnd, LONG trans, LONG* nret);
	STDMETHOD(SetClipboard)(BSTR value, LONG* nret);
	STDMETHOD(GetClipboard)(BSTR* retstring);
	STDMETHOD(DoubleToData)(DOUBLE value, BSTR* retstring);
	STDMETHOD(FloatToData)(FLOAT value, BSTR* retstring);
	STDMETHOD(IntToData)(LONG value, LONG type, BSTR* retstring);
	STDMETHOD(StringToData)(BSTR value, LONG type, BSTR* retstring);
	STDMETHOD(FindData)(LONG hwnd, BSTR addr_range, BSTR data, BSTR* retstring);
	STDMETHOD(FindDouble)(LONG hwnd, BSTR addr_range, DOUBLE double_value_min, DOUBLE double_value_max, BSTR* retstring);
	STDMETHOD(FindFloat)(LONG hwnd, BSTR addr_range, FLOAT float_value_min, FLOAT float_value_max, BSTR* retstring);
	STDMETHOD(FindInt)(LONG hwnd, BSTR addr_range, LONG int_value_min, LONG int_value_max, LONG type, BSTR* retstring);
	STDMETHOD(FindString)(LONG hwnd, BSTR addr_range, BSTR string_value, LONG type, BSTR* retstring);
	STDMETHOD(ReadData)(LONG hwnd, BSTR addr, LONG len, BSTR* retstring);
	STDMETHOD(OcrEx)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR color_format, DOUBLE sim, BSTR* retstring);
	STDMETHOD(ReadDouble)(LONG hwnd, BSTR addr, DOUBLE* retdouble);
	STDMETHOD(ReadFloat)(LONG hwnd, BSTR addr, FLOAT* retfloat);
	STDMETHOD(ReadInt)(LONG hwnd, BSTR addr, LONG type, LONG* retint);
	STDMETHOD(ReadString)(LONG hwnd, BSTR addr, LONG type, LONG len, BSTR* retstring);
	STDMETHOD(TerminateProcess)(LONG pid, LONG* bret);
	STDMETHOD(VirtualAllocEx)(LONG hwnd, LONG addr, LONG size, LONG type, LONG* bret);
	STDMETHOD(VirtualFreeEx)(LONG hwnd, LONG addr, LONG* bret);
	STDMETHOD(WriteDouble)(LONG hwnd, BSTR addr, DOUBLE v, LONG* bret);
	STDMETHOD(WriteFloat)(LONG hwnd, BSTR addr, FLOAT v, LONG* bret);
	STDMETHOD(WriteInt)(LONG hwnd, BSTR addr, LONG type, LONG v, LONG* bret);
	STDMETHOD(WriteString)(LONG hwnd, BSTR addr, LONG type, BSTR v, LONG* bret);
	STDMETHOD(WriteData)(LONG hwnd, BSTR addr, BSTR data, LONG* nret);
	STDMETHOD(IsBind)(LONG hwnd, LONG* nret);
	STDMETHOD(FindFloatEx)(LONG hwnd, BSTR addr_range, FLOAT float_value_min, FLOAT float_value_max, LONG step, LONG multi_thread, LONG mode, BSTR* retstring);
	STDMETHOD(FindDoubleEx)(LONG hwnd, BSTR addr_range, DOUBLE double_value_min, DOUBLE double_value_max, LONG step, LONG multi_thread, LONG mode, BSTR* retstring);
	STDMETHOD(FindIntEx)(LONG hwnd, BSTR addr_range, LONG int_value_min, LONG int_value_max, LONG type, LONG step, LONG multi_thread, LONG mode, BSTR* retstring);
	STDMETHOD(FindDataEx)(LONG hwnd, BSTR addr_range, BSTR data, LONG step, LONG multi_thread, LONG mode, BSTR* retstring);
	STDMETHOD(FindStringEx)(LONG hwnd, BSTR addr_range, BSTR string_value, LONG type, LONG step, LONG multi_thread, LONG mode, BSTR* retstring);
	STDMETHOD(GetModuleBaseAddr)(LONG hwnd, BSTR modulename, LONG* nret);
	STDMETHOD(GetCommandLine)(LONG hwnd, BSTR* retstring);
	STDMETHOD(AsmAdd)(BSTR asm_ins, LONG* nret);
	STDMETHOD(AsmCall)(LONG hwnd, LONG mode, LONG* nret);
	STDMETHOD(AsmClear)(LONG* nret);
	STDMETHOD(AsmCode)(LONG base_addr, BSTR* retstring);
	STDMETHOD(Assemble)(BSTR asm_code, LONG base_addr, LONG is_upper, BSTR* retstring);
	STDMETHOD(MatchPicName)(BSTR pic_name, BSTR* retstring);
	STDMETHOD(SetShowErrorMsg)(LONG show, LONG* nret);
	STDMETHOD(Reg)(BSTR reg_code, LONG type, LONG* nret);
	STDMETHOD(GetMachineCode)(BSTR* retstring);
	STDMETHOD(TSGuardProtectToHide)(LONG enble, LONG* nret);
	STDMETHOD(TSGuardProtectToHide2)(LONG enable, LONG* nret);
	STDMETHOD(TSGuardProtectToNP)(LONG enable, LONG* nret);
	STDMETHOD(Delay)(LONG mis, LONG* nret);
	STDMETHOD(FindStrS)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, DOUBLE sim, VARIANT* intX, VARIANT* intY, BSTR* retstring);
	STDMETHOD(FindStrFastS)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, DOUBLE sim, VARIANT* intX, VARIANT* intY, BSTR* retstring);
	STDMETHOD(FindStrExS)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, DOUBLE sim, BSTR* retstring);
	STDMETHOD(FindStrFastExS)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR string, BSTR color_format, DOUBLE sim, BSTR* retstring);
	STDMETHOD(FindPicS)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR pic_name, BSTR delta_color, DOUBLE sim, LONG dir, VARIANT* intX, VARIANT* intY, BSTR* retstring);
	STDMETHOD(FindPicExS)(LONG x1, LONG y1, LONG x2, LONG y2, BSTR pic_name, BSTR delta_color, DOUBLE sim, LONG dir, BSTR* retstring);
	STDMETHOD(SetDictPwd)(BSTR pwd, LONG* nret);
	STDMETHOD(SetPicPwd)(BSTR pwd, LONG* nret);
	STDMETHOD(LeftDoubleClick)(LONG* nret);
	STDMETHOD(FreeProcessMemory)(LONG hwnd, LONG* nret);
	STDMETHOD(TSDXGraphicProtect)(LONG enable, LONG* nret);
	STDMETHOD(TSDXKmProtect)(LONG enable, BSTR type, LONG* nret);
	STDMETHOD(CheckFontSmooth)(LONG* nret);
	STDMETHOD(DisableFontSmooth)(LONG* nret);
	STDMETHOD(GetScreenData)(LONG x1, LONG y1, LONG x2, LONG y2, LONG* RetAddr);
	STDMETHOD(EnableRealMouse)(ULONG enable, ULONG mousedelay, ULONG mousestep, ULONG* nret);
	STDMETHOD(EnableRealKeypad)(ULONG enable, ULONG* nret);
	STDMETHOD(MoveToEx)(ULONG x, ULONG y, ULONG w, ULONG h, BSTR* retstring);
	STDMETHOD(CheckUAC)(ULONG* nret);
	STDMETHOD(SetUAC)(ULONG enable,ULONG* nret);
	STDMETHOD(WaitKey)(ULONG vk_code, ULONG time_out, ULONG* nret);
	STDMETHOD(FindMultiColor)(ULONG x1, ULONG y1, ULONG x2, ULONG y2, BSTR first_color, BSTR offset_color, DOUBLE sim, ULONG dir, VARIANT* intX, VARIANT* intY, ULONG* nret);
	STDMETHOD(FindMultiColorEx)(ULONG x1, ULONG y1, ULONG x2, ULONG y2, BSTR first_color, BSTR offset_color, DOUBLE sim, ULONG dir, BSTR* retstring);
	STDMETHOD(SetSimMode)(ULONG mode, ULONG* nret);
};

OBJECT_ENTRY_AUTO(__uuidof(TSPlugInterFace), CTSPlugInterFace)
