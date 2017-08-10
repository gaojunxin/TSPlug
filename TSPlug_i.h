

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Sun Sep 14 16:36:22 2014
 */
/* Compiler settings for TSPlug.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __TSPlug_i_h__
#define __TSPlug_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITSPlugInterFace_FWD_DEFINED__
#define __ITSPlugInterFace_FWD_DEFINED__
typedef interface ITSPlugInterFace ITSPlugInterFace;

#endif 	/* __ITSPlugInterFace_FWD_DEFINED__ */


#ifndef __TSPlugInterFace_FWD_DEFINED__
#define __TSPlugInterFace_FWD_DEFINED__

#ifdef __cplusplus
typedef class TSPlugInterFace TSPlugInterFace;
#else
typedef struct TSPlugInterFace TSPlugInterFace;
#endif /* __cplusplus */

#endif 	/* __TSPlugInterFace_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ITSPlugInterFace_INTERFACE_DEFINED__
#define __ITSPlugInterFace_INTERFACE_DEFINED__

/* interface ITSPlugInterFace */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ITSPlugInterFace;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F3E95C10-606A-474E-BB4A-B9CCBF7DB559")
    ITSPlugInterFace : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE BindWindow( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR display,
            /* [in] */ BSTR mouse,
            /* [in] */ BSTR keypad,
            /* [in] */ LONG mode,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE BindWindowEx( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR display,
            /* [in] */ BSTR mouse,
            /* [in] */ BSTR keypad,
            /* [in] */ BSTR publics,
            /* [in] */ LONG mode,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UnBindWindow( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyDown( 
            /* [in] */ LONG vk_code,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyUp( 
            /* [in] */ LONG vk_code,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyPress( 
            /* [in] */ LONG vk_code,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LeftDown( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LeftUp( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LeftClick( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveTo( 
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindPic( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR pic_name,
            /* [in] */ BSTR delta_color,
            /* [in] */ DOUBLE sim,
            /* [in] */ LONG dir,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RightClick( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RightDown( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindColor( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR color,
            /* [in] */ DOUBLE sim,
            /* [in] */ LONG dir,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RightUp( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetColor( 
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [retval][out] */ BSTR *RetStrColor) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCursorShape( 
            /* [retval][out] */ BSTR *RetCursorShape) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetPath( 
            /* [in] */ BSTR path,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TSGuardProtect( 
            /* [in] */ LONG enable,
            /* [in] */ BSTR type,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyPressStr( 
            /* [in] */ BSTR key_str,
            /* [in] */ LONG delay,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendString( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR str,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendString2( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR str,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyPressChar( 
            /* [in] */ BSTR key_str,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyDownChar( 
            /* [in] */ BSTR key_str,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE KeyUpChar( 
            /* [in] */ BSTR key_str,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCursorPos( 
            /* [out] */ VARIANT *x,
            /* [out] */ VARIANT *y,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveR( 
            /* [in] */ LONG rx,
            /* [in] */ LONG ry,
            /* [retval][out] */ LONG *rnet) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Ver( 
            /* [retval][out] */ BSTR *retver) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetPath( 
            /* [retval][out] */ BSTR *retpath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MiddleClick( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WheelDown( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WheelUp( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Capture( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR file,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CaptureJpg( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR file,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CapturePng( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR file,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LockInput( 
            /* [in] */ LONG lock,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Ocr( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR color_format,
            /* [in] */ FLOAT sim,
            /* [retval][out] */ BSTR *retstr) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetDict( 
            /* [in] */ LONG index,
            /* [in] */ BSTR file,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE UseDict( 
            /* [in] */ LONG index,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearDict( 
            /* [in] */ LONG index,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindStr( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ FLOAT sim,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindStrFast( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ FLOAT sim,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetNowDict( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetBasePath( 
            /* [retval][out] */ BSTR *retpath) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsDisplayDead( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ LONG t,
            /* [retval][out] */ LONG *bret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindPicEx( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR pic_name,
            /* [in] */ BSTR delta_color,
            /* [in] */ DOUBLE sim,
            /* [in] */ LONG dir,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindStrEx( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ DOUBLE sim,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindStrFastEx( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ DOUBLE sim,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CaptureGif( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR file,
            /* [in] */ LONG delay,
            /* [in] */ LONG time,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DownCpu( 
            /* [in] */ LONG rate,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetKeypadDelay( 
            /* [in] */ BSTR type,
            /* [in] */ LONG delay,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetMouseDelay( 
            /* [in] */ BSTR type,
            /* [in] */ LONG delay,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CmpColor( 
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [in] */ BSTR color,
            /* [in] */ DOUBLE sim,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendStringIme( 
            /* [in] */ BSTR str,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindColorEx( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR color,
            /* [in] */ DOUBLE sim,
            /* [in] */ LONG dir,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnumWindow( 
            /* [in] */ LONG parent,
            /* [in] */ BSTR title,
            /* [in] */ BSTR class_name,
            /* [in] */ LONG filter,
            /* [retval][out] */ BSTR *retstr) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnumWindowByProcess( 
            /* [in] */ BSTR process_name,
            /* [in] */ BSTR title,
            /* [in] */ BSTR class_name,
            /* [in] */ LONG filter,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnumProcess( 
            /* [in] */ BSTR name,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClientToScreen( 
            /* [in] */ LONG ClientToScreen,
            /* [out][in] */ VARIANT *x,
            /* [out][in] */ VARIANT *y,
            /* [retval][out] */ LONG *bret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindWindow( 
            /* [in] */ BSTR class_name,
            /* [in] */ BSTR title,
            /* [retval][out] */ LONG *rethwnd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindWindowByProcess( 
            /* [in] */ BSTR process_name,
            /* [in] */ BSTR class_name,
            /* [in] */ BSTR title,
            /* [retval][out] */ LONG *rethwnd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindWindowByProcessId( 
            /* [in] */ LONG process_id,
            /* [in] */ BSTR class_name,
            /* [in] */ BSTR title,
            /* [retval][out] */ LONG *rethwnd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindWindowEx( 
            /* [in] */ LONG parent,
            /* [in] */ BSTR class_name,
            /* [in] */ BSTR title,
            /* [retval][out] */ LONG *rethwnd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetClientRect( 
            /* [in] */ LONG hwnd,
            /* [out] */ VARIANT *x1,
            /* [out] */ VARIANT *y1,
            /* [out] */ VARIANT *x2,
            /* [out] */ VARIANT *y2,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetClientSize( 
            /* [in] */ LONG hwnd,
            /* [out] */ VARIANT *width,
            /* [out] */ VARIANT *height,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetForegroundFocus( 
            /* [retval][out] */ LONG *rethwnd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetForegroundWindow( 
            /* [retval][out] */ LONG *rethwnd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMousePointWindow( 
            /* [retval][out] */ LONG *rethwnd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetPointWindow( 
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [retval][out] */ LONG *rethwnd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetProcessInfo( 
            /* [in] */ LONG pid,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetSpecialWindow( 
            /* [in] */ LONG flag,
            /* [retval][out] */ LONG *rethwnd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetWindow( 
            /* [in] */ LONG hwnd,
            /* [in] */ LONG flag,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetWindowClass( 
            /* [in] */ LONG hwnd,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetWindowProcessId( 
            /* [in] */ LONG hwnd,
            /* [retval][out] */ LONG *nretpid) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetWindowProcessPath( 
            /* [in] */ LONG hwnd,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetWindowRect( 
            /* [in] */ LONG hwnd,
            /* [out] */ VARIANT *x1,
            /* [out] */ VARIANT *y1,
            /* [out] */ VARIANT *x2,
            /* [out] */ VARIANT *y2,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetWindowState( 
            /* [in] */ LONG hwnd,
            /* [in] */ LONG flag,
            /* [retval][out] */ LONG *rethwnd) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetWindowTitle( 
            /* [in] */ LONG hwnd,
            /* [retval][out] */ BSTR *rettitle) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveWindow( 
            /* [in] */ LONG hwnd,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ScreenToClient( 
            /* [in] */ LONG hwnd,
            /* [out] */ VARIANT *x,
            /* [out] */ VARIANT *y,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SendPaste( 
            /* [in] */ LONG hwnd,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetClientSize( 
            /* [in] */ LONG hwnd,
            /* [in] */ LONG width,
            /* [in] */ LONG hight,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetWindowState( 
            /* [in] */ LONG hwnd,
            /* [in] */ LONG flag,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetWindowSize( 
            /* [in] */ LONG hwnd,
            /* [in] */ LONG width,
            /* [in] */ LONG height,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetWindowText( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR title,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetWindowTransparent( 
            /* [in] */ LONG hwnd,
            /* [in] */ LONG trans,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetClipboard( 
            /* [in] */ BSTR value,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetClipboard( 
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DoubleToData( 
            /* [in] */ DOUBLE value,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FloatToData( 
            /* [in] */ FLOAT value,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IntToData( 
            /* [in] */ LONG value,
            /* [in] */ LONG type,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE StringToData( 
            /* [in] */ BSTR value,
            /* [in] */ LONG type,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindData( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ BSTR data,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindDouble( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ DOUBLE double_value_min,
            /* [in] */ DOUBLE double_value_max,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindFloat( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ FLOAT float_value_min,
            /* [in] */ FLOAT float_value_max,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindInt( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ LONG int_value_min,
            /* [in] */ LONG int_value_max,
            /* [in] */ LONG type,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindString( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ BSTR string_value,
            /* [in] */ LONG type,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadData( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ LONG len,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OcrEx( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR color_format,
            /* [in] */ DOUBLE sim,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadDouble( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [retval][out] */ DOUBLE *retdouble) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadFloat( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [retval][out] */ FLOAT *retfloat) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadInt( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ LONG type,
            /* [retval][out] */ LONG *retint) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadString( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ LONG type,
            /* [in] */ LONG len,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TerminateProcess( 
            /* [in] */ LONG pid,
            /* [retval][out] */ LONG *bret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VirtualAllocEx( 
            /* [in] */ LONG hwnd,
            /* [in] */ LONG addr,
            /* [in] */ LONG size,
            /* [in] */ LONG type,
            /* [retval][out] */ LONG *bret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE VirtualFreeEx( 
            /* [in] */ LONG hwnd,
            /* [in] */ LONG addr,
            /* [retval][out] */ LONG *bret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteDouble( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ DOUBLE v,
            /* [retval][out] */ LONG *bret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteFloat( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ FLOAT v,
            /* [retval][out] */ LONG *bret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteInt( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ LONG type,
            /* [in] */ LONG v,
            /* [retval][out] */ LONG *bret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteString( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ LONG type,
            /* [in] */ BSTR v,
            /* [retval][out] */ LONG *bret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WriteData( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ BSTR data,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsBind( 
            /* [in] */ LONG hwnd,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindFloatEx( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ FLOAT float_value_min,
            /* [in] */ FLOAT float_value_max,
            /* [in] */ LONG step,
            /* [in] */ LONG multi_thread,
            /* [in] */ LONG mode,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindDoubleEx( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ DOUBLE double_value_min,
            /* [in] */ DOUBLE double_value_max,
            /* [in] */ LONG step,
            /* [in] */ LONG multi_thread,
            /* [in] */ LONG mode,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindIntEx( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ LONG int_value_min,
            /* [in] */ LONG int_value_max,
            /* [in] */ LONG type,
            /* [in] */ LONG step,
            /* [in] */ LONG multi_thread,
            /* [in] */ LONG mode,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindDataEx( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ BSTR data,
            /* [in] */ LONG step,
            /* [in] */ LONG multi_thread,
            /* [in] */ LONG mode,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindStringEx( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ BSTR string_value,
            /* [in] */ LONG type,
            /* [in] */ LONG step,
            /* [in] */ LONG multi_thread,
            /* [in] */ LONG mode,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetModuleBaseAddr( 
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR modulename,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetCommandLine( 
            /* [in] */ LONG hwnd,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AsmAdd( 
            /* [in] */ BSTR asm_ins,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AsmCall( 
            /* [in] */ LONG hwnd,
            /* [in] */ LONG mode,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AsmClear( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AsmCode( 
            /* [in] */ LONG base_addr,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Assemble( 
            /* [in] */ BSTR asm_code,
            /* [in] */ LONG base_addr,
            /* [in] */ LONG is_upper,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MatchPicName( 
            /* [in] */ BSTR pic_name,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetShowErrorMsg( 
            /* [in] */ LONG show,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Reg( 
            /* [in] */ BSTR reg_code,
            /* [in] */ LONG type,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetMachineCode( 
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TSGuardProtectToHide( 
            /* [in] */ LONG enble,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TSGuardProtectToHide2( 
            /* [in] */ LONG enable,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TSGuardProtectToNP( 
            /* [in] */ LONG enable,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Delay( 
            /* [in] */ LONG mis,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindStrS( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ DOUBLE sim,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindStrFastS( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ DOUBLE sim,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindStrExS( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ DOUBLE sim,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindStrFastExS( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ DOUBLE sim,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindPicS( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR pic_name,
            /* [in] */ BSTR delta_color,
            /* [in] */ DOUBLE sim,
            /* [in] */ LONG dir,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindPicExS( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR pic_name,
            /* [in] */ BSTR delta_color,
            /* [in] */ DOUBLE sim,
            /* [in] */ LONG dir,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetDictPwd( 
            /* [in] */ BSTR pwd,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetPicPwd( 
            /* [in] */ BSTR pwd,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LeftDoubleClick( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FreeProcessMemory( 
            /* [in] */ LONG hwnd,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TSDXGraphicProtect( 
            /* [in] */ LONG enable,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE TSDXKmProtect( 
            /* [in] */ LONG enable,
            /* [in] */ BSTR type,
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CheckFontSmooth( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE DisableFontSmooth( 
            /* [retval][out] */ LONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetScreenData( 
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [retval][out] */ LONG *RetAddr) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnableRealMouse( 
            /* [in] */ ULONG enable,
            /* [in] */ ULONG mousedelay,
            /* [in] */ ULONG mousestep,
            /* [retval][out] */ ULONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE EnableRealKeypad( 
            /* [in] */ ULONG enable,
            /* [retval][out] */ ULONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MoveToEx( 
            /* [in] */ ULONG x,
            /* [in] */ ULONG y,
            /* [in] */ ULONG w,
            /* [in] */ ULONG h,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CheckUAC( 
            /* [retval][out] */ ULONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetUAC( 
            /* [in] */ ULONG enable,
            /* [retval][out] */ ULONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE WaitKey( 
            /* [in] */ ULONG vk_code,
            /* [in] */ ULONG time_out,
            /* [retval][out] */ ULONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindMultiColor( 
            /* [in] */ ULONG x1,
            /* [in] */ ULONG y1,
            /* [in] */ ULONG x2,
            /* [in] */ ULONG y2,
            /* [in] */ BSTR first_color,
            /* [in] */ BSTR offset_color,
            /* [in] */ DOUBLE sim,
            /* [in] */ ULONG dir,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ ULONG *nret) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FindMultiColorEx( 
            /* [in] */ ULONG x1,
            /* [in] */ ULONG y1,
            /* [in] */ ULONG x2,
            /* [in] */ ULONG y2,
            /* [in] */ BSTR first_color,
            /* [in] */ BSTR offset_color,
            /* [in] */ DOUBLE sim,
            /* [in] */ ULONG dir,
            /* [retval][out] */ BSTR *retstring) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetSimMode( 
            /* [in] */ ULONG mode,
            /* [retval][out] */ ULONG *nret) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct ITSPlugInterFaceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITSPlugInterFace * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITSPlugInterFace * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITSPlugInterFace * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITSPlugInterFace * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITSPlugInterFace * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITSPlugInterFace * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITSPlugInterFace * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *BindWindow )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR display,
            /* [in] */ BSTR mouse,
            /* [in] */ BSTR keypad,
            /* [in] */ LONG mode,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *BindWindowEx )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR display,
            /* [in] */ BSTR mouse,
            /* [in] */ BSTR keypad,
            /* [in] */ BSTR publics,
            /* [in] */ LONG mode,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *UnBindWindow )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyDown )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG vk_code,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyUp )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG vk_code,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyPress )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG vk_code,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LeftDown )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LeftUp )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LeftClick )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveTo )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindPic )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR pic_name,
            /* [in] */ BSTR delta_color,
            /* [in] */ DOUBLE sim,
            /* [in] */ LONG dir,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RightClick )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RightDown )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindColor )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR color,
            /* [in] */ DOUBLE sim,
            /* [in] */ LONG dir,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RightUp )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetColor )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [retval][out] */ BSTR *RetStrColor);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCursorShape )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ BSTR *RetCursorShape);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetPath )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR path,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TSGuardProtect )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG enable,
            /* [in] */ BSTR type,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyPressStr )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR key_str,
            /* [in] */ LONG delay,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SendString )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR str,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SendString2 )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR str,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyPressChar )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR key_str,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyDownChar )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR key_str,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *KeyUpChar )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR key_str,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCursorPos )( 
            ITSPlugInterFace * This,
            /* [out] */ VARIANT *x,
            /* [out] */ VARIANT *y,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveR )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG rx,
            /* [in] */ LONG ry,
            /* [retval][out] */ LONG *rnet);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Ver )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ BSTR *retver);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetPath )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ BSTR *retpath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MiddleClick )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WheelDown )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WheelUp )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Capture )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR file,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CaptureJpg )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR file,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CapturePng )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR file,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LockInput )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG lock,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Ocr )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR color_format,
            /* [in] */ FLOAT sim,
            /* [retval][out] */ BSTR *retstr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetDict )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG index,
            /* [in] */ BSTR file,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *UseDict )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG index,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearDict )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG index,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindStr )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ FLOAT sim,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindStrFast )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ FLOAT sim,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetNowDict )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetBasePath )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ BSTR *retpath);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsDisplayDead )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ LONG t,
            /* [retval][out] */ LONG *bret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindPicEx )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR pic_name,
            /* [in] */ BSTR delta_color,
            /* [in] */ DOUBLE sim,
            /* [in] */ LONG dir,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindStrEx )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ DOUBLE sim,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindStrFastEx )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ DOUBLE sim,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CaptureGif )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR file,
            /* [in] */ LONG delay,
            /* [in] */ LONG time,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DownCpu )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG rate,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetKeypadDelay )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR type,
            /* [in] */ LONG delay,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetMouseDelay )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR type,
            /* [in] */ LONG delay,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CmpColor )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [in] */ BSTR color,
            /* [in] */ DOUBLE sim,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SendStringIme )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR str,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindColorEx )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR color,
            /* [in] */ DOUBLE sim,
            /* [in] */ LONG dir,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnumWindow )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG parent,
            /* [in] */ BSTR title,
            /* [in] */ BSTR class_name,
            /* [in] */ LONG filter,
            /* [retval][out] */ BSTR *retstr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnumWindowByProcess )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR process_name,
            /* [in] */ BSTR title,
            /* [in] */ BSTR class_name,
            /* [in] */ LONG filter,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnumProcess )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR name,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClientToScreen )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG ClientToScreen,
            /* [out][in] */ VARIANT *x,
            /* [out][in] */ VARIANT *y,
            /* [retval][out] */ LONG *bret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindWindow )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR class_name,
            /* [in] */ BSTR title,
            /* [retval][out] */ LONG *rethwnd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindWindowByProcess )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR process_name,
            /* [in] */ BSTR class_name,
            /* [in] */ BSTR title,
            /* [retval][out] */ LONG *rethwnd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindWindowByProcessId )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG process_id,
            /* [in] */ BSTR class_name,
            /* [in] */ BSTR title,
            /* [retval][out] */ LONG *rethwnd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindWindowEx )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG parent,
            /* [in] */ BSTR class_name,
            /* [in] */ BSTR title,
            /* [retval][out] */ LONG *rethwnd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetClientRect )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [out] */ VARIANT *x1,
            /* [out] */ VARIANT *y1,
            /* [out] */ VARIANT *x2,
            /* [out] */ VARIANT *y2,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetClientSize )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [out] */ VARIANT *width,
            /* [out] */ VARIANT *height,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetForegroundFocus )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *rethwnd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetForegroundWindow )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *rethwnd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMousePointWindow )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *rethwnd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetPointWindow )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [retval][out] */ LONG *rethwnd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetProcessInfo )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG pid,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetSpecialWindow )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG flag,
            /* [retval][out] */ LONG *rethwnd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetWindow )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ LONG flag,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetWindowClass )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetWindowProcessId )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [retval][out] */ LONG *nretpid);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetWindowProcessPath )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetWindowRect )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [out] */ VARIANT *x1,
            /* [out] */ VARIANT *y1,
            /* [out] */ VARIANT *x2,
            /* [out] */ VARIANT *y2,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetWindowState )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ LONG flag,
            /* [retval][out] */ LONG *rethwnd);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetWindowTitle )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [retval][out] */ BSTR *rettitle);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveWindow )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ LONG x,
            /* [in] */ LONG y,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ScreenToClient )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [out] */ VARIANT *x,
            /* [out] */ VARIANT *y,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SendPaste )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetClientSize )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ LONG width,
            /* [in] */ LONG hight,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetWindowState )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ LONG flag,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetWindowSize )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ LONG width,
            /* [in] */ LONG height,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetWindowText )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR title,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetWindowTransparent )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ LONG trans,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetClipboard )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR value,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetClipboard )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DoubleToData )( 
            ITSPlugInterFace * This,
            /* [in] */ DOUBLE value,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FloatToData )( 
            ITSPlugInterFace * This,
            /* [in] */ FLOAT value,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IntToData )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG value,
            /* [in] */ LONG type,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *StringToData )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR value,
            /* [in] */ LONG type,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindData )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ BSTR data,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindDouble )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ DOUBLE double_value_min,
            /* [in] */ DOUBLE double_value_max,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindFloat )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ FLOAT float_value_min,
            /* [in] */ FLOAT float_value_max,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindInt )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ LONG int_value_min,
            /* [in] */ LONG int_value_max,
            /* [in] */ LONG type,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindString )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ BSTR string_value,
            /* [in] */ LONG type,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadData )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ LONG len,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OcrEx )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR color_format,
            /* [in] */ DOUBLE sim,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadDouble )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [retval][out] */ DOUBLE *retdouble);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadFloat )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [retval][out] */ FLOAT *retfloat);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadInt )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ LONG type,
            /* [retval][out] */ LONG *retint);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadString )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ LONG type,
            /* [in] */ LONG len,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TerminateProcess )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG pid,
            /* [retval][out] */ LONG *bret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *VirtualAllocEx )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ LONG addr,
            /* [in] */ LONG size,
            /* [in] */ LONG type,
            /* [retval][out] */ LONG *bret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *VirtualFreeEx )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ LONG addr,
            /* [retval][out] */ LONG *bret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteDouble )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ DOUBLE v,
            /* [retval][out] */ LONG *bret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteFloat )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ FLOAT v,
            /* [retval][out] */ LONG *bret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteInt )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ LONG type,
            /* [in] */ LONG v,
            /* [retval][out] */ LONG *bret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteString )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ LONG type,
            /* [in] */ BSTR v,
            /* [retval][out] */ LONG *bret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WriteData )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr,
            /* [in] */ BSTR data,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsBind )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindFloatEx )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ FLOAT float_value_min,
            /* [in] */ FLOAT float_value_max,
            /* [in] */ LONG step,
            /* [in] */ LONG multi_thread,
            /* [in] */ LONG mode,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindDoubleEx )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ DOUBLE double_value_min,
            /* [in] */ DOUBLE double_value_max,
            /* [in] */ LONG step,
            /* [in] */ LONG multi_thread,
            /* [in] */ LONG mode,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindIntEx )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ LONG int_value_min,
            /* [in] */ LONG int_value_max,
            /* [in] */ LONG type,
            /* [in] */ LONG step,
            /* [in] */ LONG multi_thread,
            /* [in] */ LONG mode,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindDataEx )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ BSTR data,
            /* [in] */ LONG step,
            /* [in] */ LONG multi_thread,
            /* [in] */ LONG mode,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindStringEx )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR addr_range,
            /* [in] */ BSTR string_value,
            /* [in] */ LONG type,
            /* [in] */ LONG step,
            /* [in] */ LONG multi_thread,
            /* [in] */ LONG mode,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetModuleBaseAddr )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ BSTR modulename,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetCommandLine )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AsmAdd )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR asm_ins,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AsmCall )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [in] */ LONG mode,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AsmClear )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AsmCode )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG base_addr,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Assemble )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR asm_code,
            /* [in] */ LONG base_addr,
            /* [in] */ LONG is_upper,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MatchPicName )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR pic_name,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetShowErrorMsg )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG show,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Reg )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR reg_code,
            /* [in] */ LONG type,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetMachineCode )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TSGuardProtectToHide )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG enble,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TSGuardProtectToHide2 )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG enable,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TSGuardProtectToNP )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG enable,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Delay )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG mis,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindStrS )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ DOUBLE sim,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindStrFastS )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ DOUBLE sim,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindStrExS )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ DOUBLE sim,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindStrFastExS )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR string,
            /* [in] */ BSTR color_format,
            /* [in] */ DOUBLE sim,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindPicS )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR pic_name,
            /* [in] */ BSTR delta_color,
            /* [in] */ DOUBLE sim,
            /* [in] */ LONG dir,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindPicExS )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [in] */ BSTR pic_name,
            /* [in] */ BSTR delta_color,
            /* [in] */ DOUBLE sim,
            /* [in] */ LONG dir,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetDictPwd )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR pwd,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetPicPwd )( 
            ITSPlugInterFace * This,
            /* [in] */ BSTR pwd,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LeftDoubleClick )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FreeProcessMemory )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG hwnd,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TSDXGraphicProtect )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG enable,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *TSDXKmProtect )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG enable,
            /* [in] */ BSTR type,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CheckFontSmooth )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *DisableFontSmooth )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ LONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetScreenData )( 
            ITSPlugInterFace * This,
            /* [in] */ LONG x1,
            /* [in] */ LONG y1,
            /* [in] */ LONG x2,
            /* [in] */ LONG y2,
            /* [retval][out] */ LONG *RetAddr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnableRealMouse )( 
            ITSPlugInterFace * This,
            /* [in] */ ULONG enable,
            /* [in] */ ULONG mousedelay,
            /* [in] */ ULONG mousestep,
            /* [retval][out] */ ULONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *EnableRealKeypad )( 
            ITSPlugInterFace * This,
            /* [in] */ ULONG enable,
            /* [retval][out] */ ULONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MoveToEx )( 
            ITSPlugInterFace * This,
            /* [in] */ ULONG x,
            /* [in] */ ULONG y,
            /* [in] */ ULONG w,
            /* [in] */ ULONG h,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CheckUAC )( 
            ITSPlugInterFace * This,
            /* [retval][out] */ ULONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetUAC )( 
            ITSPlugInterFace * This,
            /* [in] */ ULONG enable,
            /* [retval][out] */ ULONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *WaitKey )( 
            ITSPlugInterFace * This,
            /* [in] */ ULONG vk_code,
            /* [in] */ ULONG time_out,
            /* [retval][out] */ ULONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindMultiColor )( 
            ITSPlugInterFace * This,
            /* [in] */ ULONG x1,
            /* [in] */ ULONG y1,
            /* [in] */ ULONG x2,
            /* [in] */ ULONG y2,
            /* [in] */ BSTR first_color,
            /* [in] */ BSTR offset_color,
            /* [in] */ DOUBLE sim,
            /* [in] */ ULONG dir,
            /* [out] */ VARIANT *intX,
            /* [out] */ VARIANT *intY,
            /* [retval][out] */ ULONG *nret);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FindMultiColorEx )( 
            ITSPlugInterFace * This,
            /* [in] */ ULONG x1,
            /* [in] */ ULONG y1,
            /* [in] */ ULONG x2,
            /* [in] */ ULONG y2,
            /* [in] */ BSTR first_color,
            /* [in] */ BSTR offset_color,
            /* [in] */ DOUBLE sim,
            /* [in] */ ULONG dir,
            /* [retval][out] */ BSTR *retstring);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetSimMode )( 
            ITSPlugInterFace * This,
            /* [in] */ ULONG mode,
            /* [retval][out] */ ULONG *nret);
        
        END_INTERFACE
    } ITSPlugInterFaceVtbl;

    interface ITSPlugInterFace
    {
        CONST_VTBL struct ITSPlugInterFaceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITSPlugInterFace_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITSPlugInterFace_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITSPlugInterFace_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITSPlugInterFace_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITSPlugInterFace_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITSPlugInterFace_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITSPlugInterFace_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ITSPlugInterFace_BindWindow(This,hwnd,display,mouse,keypad,mode,nret)	\
    ( (This)->lpVtbl -> BindWindow(This,hwnd,display,mouse,keypad,mode,nret) ) 

#define ITSPlugInterFace_BindWindowEx(This,hwnd,display,mouse,keypad,publics,mode,nret)	\
    ( (This)->lpVtbl -> BindWindowEx(This,hwnd,display,mouse,keypad,publics,mode,nret) ) 

#define ITSPlugInterFace_UnBindWindow(This,nret)	\
    ( (This)->lpVtbl -> UnBindWindow(This,nret) ) 

#define ITSPlugInterFace_KeyDown(This,vk_code,nret)	\
    ( (This)->lpVtbl -> KeyDown(This,vk_code,nret) ) 

#define ITSPlugInterFace_KeyUp(This,vk_code,nret)	\
    ( (This)->lpVtbl -> KeyUp(This,vk_code,nret) ) 

#define ITSPlugInterFace_KeyPress(This,vk_code,nret)	\
    ( (This)->lpVtbl -> KeyPress(This,vk_code,nret) ) 

#define ITSPlugInterFace_LeftDown(This,nret)	\
    ( (This)->lpVtbl -> LeftDown(This,nret) ) 

#define ITSPlugInterFace_LeftUp(This,nret)	\
    ( (This)->lpVtbl -> LeftUp(This,nret) ) 

#define ITSPlugInterFace_LeftClick(This,nret)	\
    ( (This)->lpVtbl -> LeftClick(This,nret) ) 

#define ITSPlugInterFace_MoveTo(This,x,y,nret)	\
    ( (This)->lpVtbl -> MoveTo(This,x,y,nret) ) 

#define ITSPlugInterFace_FindPic(This,x1,y1,x2,y2,pic_name,delta_color,sim,dir,intX,intY,nret)	\
    ( (This)->lpVtbl -> FindPic(This,x1,y1,x2,y2,pic_name,delta_color,sim,dir,intX,intY,nret) ) 

#define ITSPlugInterFace_RightClick(This,nret)	\
    ( (This)->lpVtbl -> RightClick(This,nret) ) 

#define ITSPlugInterFace_RightDown(This,nret)	\
    ( (This)->lpVtbl -> RightDown(This,nret) ) 

#define ITSPlugInterFace_FindColor(This,x1,y1,x2,y2,color,sim,dir,intX,intY,nret)	\
    ( (This)->lpVtbl -> FindColor(This,x1,y1,x2,y2,color,sim,dir,intX,intY,nret) ) 

#define ITSPlugInterFace_RightUp(This,nret)	\
    ( (This)->lpVtbl -> RightUp(This,nret) ) 

#define ITSPlugInterFace_GetColor(This,x,y,RetStrColor)	\
    ( (This)->lpVtbl -> GetColor(This,x,y,RetStrColor) ) 

#define ITSPlugInterFace_GetCursorShape(This,RetCursorShape)	\
    ( (This)->lpVtbl -> GetCursorShape(This,RetCursorShape) ) 

#define ITSPlugInterFace_SetPath(This,path,nret)	\
    ( (This)->lpVtbl -> SetPath(This,path,nret) ) 

#define ITSPlugInterFace_TSGuardProtect(This,enable,type,nret)	\
    ( (This)->lpVtbl -> TSGuardProtect(This,enable,type,nret) ) 

#define ITSPlugInterFace_KeyPressStr(This,key_str,delay,nret)	\
    ( (This)->lpVtbl -> KeyPressStr(This,key_str,delay,nret) ) 

#define ITSPlugInterFace_SendString(This,hwnd,str,nret)	\
    ( (This)->lpVtbl -> SendString(This,hwnd,str,nret) ) 

#define ITSPlugInterFace_SendString2(This,hwnd,str,nret)	\
    ( (This)->lpVtbl -> SendString2(This,hwnd,str,nret) ) 

#define ITSPlugInterFace_KeyPressChar(This,key_str,nret)	\
    ( (This)->lpVtbl -> KeyPressChar(This,key_str,nret) ) 

#define ITSPlugInterFace_KeyDownChar(This,key_str,nret)	\
    ( (This)->lpVtbl -> KeyDownChar(This,key_str,nret) ) 

#define ITSPlugInterFace_KeyUpChar(This,key_str,nret)	\
    ( (This)->lpVtbl -> KeyUpChar(This,key_str,nret) ) 

#define ITSPlugInterFace_GetCursorPos(This,x,y,nret)	\
    ( (This)->lpVtbl -> GetCursorPos(This,x,y,nret) ) 

#define ITSPlugInterFace_MoveR(This,rx,ry,rnet)	\
    ( (This)->lpVtbl -> MoveR(This,rx,ry,rnet) ) 

#define ITSPlugInterFace_Ver(This,retver)	\
    ( (This)->lpVtbl -> Ver(This,retver) ) 

#define ITSPlugInterFace_GetPath(This,retpath)	\
    ( (This)->lpVtbl -> GetPath(This,retpath) ) 

#define ITSPlugInterFace_MiddleClick(This,nret)	\
    ( (This)->lpVtbl -> MiddleClick(This,nret) ) 

#define ITSPlugInterFace_WheelDown(This,nret)	\
    ( (This)->lpVtbl -> WheelDown(This,nret) ) 

#define ITSPlugInterFace_WheelUp(This,nret)	\
    ( (This)->lpVtbl -> WheelUp(This,nret) ) 

#define ITSPlugInterFace_Capture(This,x1,y1,x2,y2,file,nret)	\
    ( (This)->lpVtbl -> Capture(This,x1,y1,x2,y2,file,nret) ) 

#define ITSPlugInterFace_CaptureJpg(This,x1,y1,x2,y2,file,nret)	\
    ( (This)->lpVtbl -> CaptureJpg(This,x1,y1,x2,y2,file,nret) ) 

#define ITSPlugInterFace_CapturePng(This,x1,y1,x2,y2,file,nret)	\
    ( (This)->lpVtbl -> CapturePng(This,x1,y1,x2,y2,file,nret) ) 

#define ITSPlugInterFace_LockInput(This,lock,nret)	\
    ( (This)->lpVtbl -> LockInput(This,lock,nret) ) 

#define ITSPlugInterFace_Ocr(This,x1,y1,x2,y2,color_format,sim,retstr)	\
    ( (This)->lpVtbl -> Ocr(This,x1,y1,x2,y2,color_format,sim,retstr) ) 

#define ITSPlugInterFace_SetDict(This,index,file,nret)	\
    ( (This)->lpVtbl -> SetDict(This,index,file,nret) ) 

#define ITSPlugInterFace_UseDict(This,index,nret)	\
    ( (This)->lpVtbl -> UseDict(This,index,nret) ) 

#define ITSPlugInterFace_ClearDict(This,index,nret)	\
    ( (This)->lpVtbl -> ClearDict(This,index,nret) ) 

#define ITSPlugInterFace_FindStr(This,x1,y1,x2,y2,string,color_format,sim,intX,intY,nret)	\
    ( (This)->lpVtbl -> FindStr(This,x1,y1,x2,y2,string,color_format,sim,intX,intY,nret) ) 

#define ITSPlugInterFace_FindStrFast(This,x1,y1,x2,y2,string,color_format,sim,intX,intY,nret)	\
    ( (This)->lpVtbl -> FindStrFast(This,x1,y1,x2,y2,string,color_format,sim,intX,intY,nret) ) 

#define ITSPlugInterFace_GetNowDict(This,nret)	\
    ( (This)->lpVtbl -> GetNowDict(This,nret) ) 

#define ITSPlugInterFace_GetBasePath(This,retpath)	\
    ( (This)->lpVtbl -> GetBasePath(This,retpath) ) 

#define ITSPlugInterFace_IsDisplayDead(This,x1,y1,x2,y2,t,bret)	\
    ( (This)->lpVtbl -> IsDisplayDead(This,x1,y1,x2,y2,t,bret) ) 

#define ITSPlugInterFace_FindPicEx(This,x1,y1,x2,y2,pic_name,delta_color,sim,dir,retstring)	\
    ( (This)->lpVtbl -> FindPicEx(This,x1,y1,x2,y2,pic_name,delta_color,sim,dir,retstring) ) 

#define ITSPlugInterFace_FindStrEx(This,x1,y1,x2,y2,string,color_format,sim,retstring)	\
    ( (This)->lpVtbl -> FindStrEx(This,x1,y1,x2,y2,string,color_format,sim,retstring) ) 

#define ITSPlugInterFace_FindStrFastEx(This,x1,y1,x2,y2,string,color_format,sim,retstring)	\
    ( (This)->lpVtbl -> FindStrFastEx(This,x1,y1,x2,y2,string,color_format,sim,retstring) ) 

#define ITSPlugInterFace_CaptureGif(This,x1,y1,x2,y2,file,delay,time,nret)	\
    ( (This)->lpVtbl -> CaptureGif(This,x1,y1,x2,y2,file,delay,time,nret) ) 

#define ITSPlugInterFace_DownCpu(This,rate,nret)	\
    ( (This)->lpVtbl -> DownCpu(This,rate,nret) ) 

#define ITSPlugInterFace_SetKeypadDelay(This,type,delay,nret)	\
    ( (This)->lpVtbl -> SetKeypadDelay(This,type,delay,nret) ) 

#define ITSPlugInterFace_SetMouseDelay(This,type,delay,nret)	\
    ( (This)->lpVtbl -> SetMouseDelay(This,type,delay,nret) ) 

#define ITSPlugInterFace_CmpColor(This,x,y,color,sim,nret)	\
    ( (This)->lpVtbl -> CmpColor(This,x,y,color,sim,nret) ) 

#define ITSPlugInterFace_SendStringIme(This,str,nret)	\
    ( (This)->lpVtbl -> SendStringIme(This,str,nret) ) 

#define ITSPlugInterFace_FindColorEx(This,x1,y1,x2,y2,color,sim,dir,retstring)	\
    ( (This)->lpVtbl -> FindColorEx(This,x1,y1,x2,y2,color,sim,dir,retstring) ) 

#define ITSPlugInterFace_EnumWindow(This,parent,title,class_name,filter,retstr)	\
    ( (This)->lpVtbl -> EnumWindow(This,parent,title,class_name,filter,retstr) ) 

#define ITSPlugInterFace_EnumWindowByProcess(This,process_name,title,class_name,filter,retstring)	\
    ( (This)->lpVtbl -> EnumWindowByProcess(This,process_name,title,class_name,filter,retstring) ) 

#define ITSPlugInterFace_EnumProcess(This,name,retstring)	\
    ( (This)->lpVtbl -> EnumProcess(This,name,retstring) ) 

#define ITSPlugInterFace_ClientToScreen(This,ClientToScreen,x,y,bret)	\
    ( (This)->lpVtbl -> ClientToScreen(This,ClientToScreen,x,y,bret) ) 

#define ITSPlugInterFace_FindWindow(This,class_name,title,rethwnd)	\
    ( (This)->lpVtbl -> FindWindow(This,class_name,title,rethwnd) ) 

#define ITSPlugInterFace_FindWindowByProcess(This,process_name,class_name,title,rethwnd)	\
    ( (This)->lpVtbl -> FindWindowByProcess(This,process_name,class_name,title,rethwnd) ) 

#define ITSPlugInterFace_FindWindowByProcessId(This,process_id,class_name,title,rethwnd)	\
    ( (This)->lpVtbl -> FindWindowByProcessId(This,process_id,class_name,title,rethwnd) ) 

#define ITSPlugInterFace_FindWindowEx(This,parent,class_name,title,rethwnd)	\
    ( (This)->lpVtbl -> FindWindowEx(This,parent,class_name,title,rethwnd) ) 

#define ITSPlugInterFace_GetClientRect(This,hwnd,x1,y1,x2,y2,nret)	\
    ( (This)->lpVtbl -> GetClientRect(This,hwnd,x1,y1,x2,y2,nret) ) 

#define ITSPlugInterFace_GetClientSize(This,hwnd,width,height,nret)	\
    ( (This)->lpVtbl -> GetClientSize(This,hwnd,width,height,nret) ) 

#define ITSPlugInterFace_GetForegroundFocus(This,rethwnd)	\
    ( (This)->lpVtbl -> GetForegroundFocus(This,rethwnd) ) 

#define ITSPlugInterFace_GetForegroundWindow(This,rethwnd)	\
    ( (This)->lpVtbl -> GetForegroundWindow(This,rethwnd) ) 

#define ITSPlugInterFace_GetMousePointWindow(This,rethwnd)	\
    ( (This)->lpVtbl -> GetMousePointWindow(This,rethwnd) ) 

#define ITSPlugInterFace_GetPointWindow(This,x,y,rethwnd)	\
    ( (This)->lpVtbl -> GetPointWindow(This,x,y,rethwnd) ) 

#define ITSPlugInterFace_GetProcessInfo(This,pid,retstring)	\
    ( (This)->lpVtbl -> GetProcessInfo(This,pid,retstring) ) 

#define ITSPlugInterFace_GetSpecialWindow(This,flag,rethwnd)	\
    ( (This)->lpVtbl -> GetSpecialWindow(This,flag,rethwnd) ) 

#define ITSPlugInterFace_GetWindow(This,hwnd,flag,nret)	\
    ( (This)->lpVtbl -> GetWindow(This,hwnd,flag,nret) ) 

#define ITSPlugInterFace_GetWindowClass(This,hwnd,retstring)	\
    ( (This)->lpVtbl -> GetWindowClass(This,hwnd,retstring) ) 

#define ITSPlugInterFace_GetWindowProcessId(This,hwnd,nretpid)	\
    ( (This)->lpVtbl -> GetWindowProcessId(This,hwnd,nretpid) ) 

#define ITSPlugInterFace_GetWindowProcessPath(This,hwnd,retstring)	\
    ( (This)->lpVtbl -> GetWindowProcessPath(This,hwnd,retstring) ) 

#define ITSPlugInterFace_GetWindowRect(This,hwnd,x1,y1,x2,y2,nret)	\
    ( (This)->lpVtbl -> GetWindowRect(This,hwnd,x1,y1,x2,y2,nret) ) 

#define ITSPlugInterFace_GetWindowState(This,hwnd,flag,rethwnd)	\
    ( (This)->lpVtbl -> GetWindowState(This,hwnd,flag,rethwnd) ) 

#define ITSPlugInterFace_GetWindowTitle(This,hwnd,rettitle)	\
    ( (This)->lpVtbl -> GetWindowTitle(This,hwnd,rettitle) ) 

#define ITSPlugInterFace_MoveWindow(This,hwnd,x,y,nret)	\
    ( (This)->lpVtbl -> MoveWindow(This,hwnd,x,y,nret) ) 

#define ITSPlugInterFace_ScreenToClient(This,hwnd,x,y,nret)	\
    ( (This)->lpVtbl -> ScreenToClient(This,hwnd,x,y,nret) ) 

#define ITSPlugInterFace_SendPaste(This,hwnd,nret)	\
    ( (This)->lpVtbl -> SendPaste(This,hwnd,nret) ) 

#define ITSPlugInterFace_SetClientSize(This,hwnd,width,hight,nret)	\
    ( (This)->lpVtbl -> SetClientSize(This,hwnd,width,hight,nret) ) 

#define ITSPlugInterFace_SetWindowState(This,hwnd,flag,nret)	\
    ( (This)->lpVtbl -> SetWindowState(This,hwnd,flag,nret) ) 

#define ITSPlugInterFace_SetWindowSize(This,hwnd,width,height,nret)	\
    ( (This)->lpVtbl -> SetWindowSize(This,hwnd,width,height,nret) ) 

#define ITSPlugInterFace_SetWindowText(This,hwnd,title,nret)	\
    ( (This)->lpVtbl -> SetWindowText(This,hwnd,title,nret) ) 

#define ITSPlugInterFace_SetWindowTransparent(This,hwnd,trans,nret)	\
    ( (This)->lpVtbl -> SetWindowTransparent(This,hwnd,trans,nret) ) 

#define ITSPlugInterFace_SetClipboard(This,value,nret)	\
    ( (This)->lpVtbl -> SetClipboard(This,value,nret) ) 

#define ITSPlugInterFace_GetClipboard(This,retstring)	\
    ( (This)->lpVtbl -> GetClipboard(This,retstring) ) 

#define ITSPlugInterFace_DoubleToData(This,value,retstring)	\
    ( (This)->lpVtbl -> DoubleToData(This,value,retstring) ) 

#define ITSPlugInterFace_FloatToData(This,value,retstring)	\
    ( (This)->lpVtbl -> FloatToData(This,value,retstring) ) 

#define ITSPlugInterFace_IntToData(This,value,type,retstring)	\
    ( (This)->lpVtbl -> IntToData(This,value,type,retstring) ) 

#define ITSPlugInterFace_StringToData(This,value,type,retstring)	\
    ( (This)->lpVtbl -> StringToData(This,value,type,retstring) ) 

#define ITSPlugInterFace_FindData(This,hwnd,addr_range,data,retstring)	\
    ( (This)->lpVtbl -> FindData(This,hwnd,addr_range,data,retstring) ) 

#define ITSPlugInterFace_FindDouble(This,hwnd,addr_range,double_value_min,double_value_max,retstring)	\
    ( (This)->lpVtbl -> FindDouble(This,hwnd,addr_range,double_value_min,double_value_max,retstring) ) 

#define ITSPlugInterFace_FindFloat(This,hwnd,addr_range,float_value_min,float_value_max,retstring)	\
    ( (This)->lpVtbl -> FindFloat(This,hwnd,addr_range,float_value_min,float_value_max,retstring) ) 

#define ITSPlugInterFace_FindInt(This,hwnd,addr_range,int_value_min,int_value_max,type,retstring)	\
    ( (This)->lpVtbl -> FindInt(This,hwnd,addr_range,int_value_min,int_value_max,type,retstring) ) 

#define ITSPlugInterFace_FindString(This,hwnd,addr_range,string_value,type,retstring)	\
    ( (This)->lpVtbl -> FindString(This,hwnd,addr_range,string_value,type,retstring) ) 

#define ITSPlugInterFace_ReadData(This,hwnd,addr,len,retstring)	\
    ( (This)->lpVtbl -> ReadData(This,hwnd,addr,len,retstring) ) 

#define ITSPlugInterFace_OcrEx(This,x1,y1,x2,y2,color_format,sim,retstring)	\
    ( (This)->lpVtbl -> OcrEx(This,x1,y1,x2,y2,color_format,sim,retstring) ) 

#define ITSPlugInterFace_ReadDouble(This,hwnd,addr,retdouble)	\
    ( (This)->lpVtbl -> ReadDouble(This,hwnd,addr,retdouble) ) 

#define ITSPlugInterFace_ReadFloat(This,hwnd,addr,retfloat)	\
    ( (This)->lpVtbl -> ReadFloat(This,hwnd,addr,retfloat) ) 

#define ITSPlugInterFace_ReadInt(This,hwnd,addr,type,retint)	\
    ( (This)->lpVtbl -> ReadInt(This,hwnd,addr,type,retint) ) 

#define ITSPlugInterFace_ReadString(This,hwnd,addr,type,len,retstring)	\
    ( (This)->lpVtbl -> ReadString(This,hwnd,addr,type,len,retstring) ) 

#define ITSPlugInterFace_TerminateProcess(This,pid,bret)	\
    ( (This)->lpVtbl -> TerminateProcess(This,pid,bret) ) 

#define ITSPlugInterFace_VirtualAllocEx(This,hwnd,addr,size,type,bret)	\
    ( (This)->lpVtbl -> VirtualAllocEx(This,hwnd,addr,size,type,bret) ) 

#define ITSPlugInterFace_VirtualFreeEx(This,hwnd,addr,bret)	\
    ( (This)->lpVtbl -> VirtualFreeEx(This,hwnd,addr,bret) ) 

#define ITSPlugInterFace_WriteDouble(This,hwnd,addr,v,bret)	\
    ( (This)->lpVtbl -> WriteDouble(This,hwnd,addr,v,bret) ) 

#define ITSPlugInterFace_WriteFloat(This,hwnd,addr,v,bret)	\
    ( (This)->lpVtbl -> WriteFloat(This,hwnd,addr,v,bret) ) 

#define ITSPlugInterFace_WriteInt(This,hwnd,addr,type,v,bret)	\
    ( (This)->lpVtbl -> WriteInt(This,hwnd,addr,type,v,bret) ) 

#define ITSPlugInterFace_WriteString(This,hwnd,addr,type,v,bret)	\
    ( (This)->lpVtbl -> WriteString(This,hwnd,addr,type,v,bret) ) 

#define ITSPlugInterFace_WriteData(This,hwnd,addr,data,nret)	\
    ( (This)->lpVtbl -> WriteData(This,hwnd,addr,data,nret) ) 

#define ITSPlugInterFace_IsBind(This,hwnd,nret)	\
    ( (This)->lpVtbl -> IsBind(This,hwnd,nret) ) 

#define ITSPlugInterFace_FindFloatEx(This,hwnd,addr_range,float_value_min,float_value_max,step,multi_thread,mode,retstring)	\
    ( (This)->lpVtbl -> FindFloatEx(This,hwnd,addr_range,float_value_min,float_value_max,step,multi_thread,mode,retstring) ) 

#define ITSPlugInterFace_FindDoubleEx(This,hwnd,addr_range,double_value_min,double_value_max,step,multi_thread,mode,retstring)	\
    ( (This)->lpVtbl -> FindDoubleEx(This,hwnd,addr_range,double_value_min,double_value_max,step,multi_thread,mode,retstring) ) 

#define ITSPlugInterFace_FindIntEx(This,hwnd,addr_range,int_value_min,int_value_max,type,step,multi_thread,mode,retstring)	\
    ( (This)->lpVtbl -> FindIntEx(This,hwnd,addr_range,int_value_min,int_value_max,type,step,multi_thread,mode,retstring) ) 

#define ITSPlugInterFace_FindDataEx(This,hwnd,addr_range,data,step,multi_thread,mode,retstring)	\
    ( (This)->lpVtbl -> FindDataEx(This,hwnd,addr_range,data,step,multi_thread,mode,retstring) ) 

#define ITSPlugInterFace_FindStringEx(This,hwnd,addr_range,string_value,type,step,multi_thread,mode,retstring)	\
    ( (This)->lpVtbl -> FindStringEx(This,hwnd,addr_range,string_value,type,step,multi_thread,mode,retstring) ) 

#define ITSPlugInterFace_GetModuleBaseAddr(This,hwnd,modulename,nret)	\
    ( (This)->lpVtbl -> GetModuleBaseAddr(This,hwnd,modulename,nret) ) 

#define ITSPlugInterFace_GetCommandLine(This,hwnd,retstring)	\
    ( (This)->lpVtbl -> GetCommandLine(This,hwnd,retstring) ) 

#define ITSPlugInterFace_AsmAdd(This,asm_ins,nret)	\
    ( (This)->lpVtbl -> AsmAdd(This,asm_ins,nret) ) 

#define ITSPlugInterFace_AsmCall(This,hwnd,mode,nret)	\
    ( (This)->lpVtbl -> AsmCall(This,hwnd,mode,nret) ) 

#define ITSPlugInterFace_AsmClear(This,nret)	\
    ( (This)->lpVtbl -> AsmClear(This,nret) ) 

#define ITSPlugInterFace_AsmCode(This,base_addr,retstring)	\
    ( (This)->lpVtbl -> AsmCode(This,base_addr,retstring) ) 

#define ITSPlugInterFace_Assemble(This,asm_code,base_addr,is_upper,retstring)	\
    ( (This)->lpVtbl -> Assemble(This,asm_code,base_addr,is_upper,retstring) ) 

#define ITSPlugInterFace_MatchPicName(This,pic_name,retstring)	\
    ( (This)->lpVtbl -> MatchPicName(This,pic_name,retstring) ) 

#define ITSPlugInterFace_SetShowErrorMsg(This,show,nret)	\
    ( (This)->lpVtbl -> SetShowErrorMsg(This,show,nret) ) 

#define ITSPlugInterFace_Reg(This,reg_code,type,nret)	\
    ( (This)->lpVtbl -> Reg(This,reg_code,type,nret) ) 

#define ITSPlugInterFace_GetMachineCode(This,retstring)	\
    ( (This)->lpVtbl -> GetMachineCode(This,retstring) ) 

#define ITSPlugInterFace_TSGuardProtectToHide(This,enble,nret)	\
    ( (This)->lpVtbl -> TSGuardProtectToHide(This,enble,nret) ) 

#define ITSPlugInterFace_TSGuardProtectToHide2(This,enable,nret)	\
    ( (This)->lpVtbl -> TSGuardProtectToHide2(This,enable,nret) ) 

#define ITSPlugInterFace_TSGuardProtectToNP(This,enable,nret)	\
    ( (This)->lpVtbl -> TSGuardProtectToNP(This,enable,nret) ) 

#define ITSPlugInterFace_Delay(This,mis,nret)	\
    ( (This)->lpVtbl -> Delay(This,mis,nret) ) 

#define ITSPlugInterFace_FindStrS(This,x1,y1,x2,y2,string,color_format,sim,intX,intY,retstring)	\
    ( (This)->lpVtbl -> FindStrS(This,x1,y1,x2,y2,string,color_format,sim,intX,intY,retstring) ) 

#define ITSPlugInterFace_FindStrFastS(This,x1,y1,x2,y2,string,color_format,sim,intX,intY,retstring)	\
    ( (This)->lpVtbl -> FindStrFastS(This,x1,y1,x2,y2,string,color_format,sim,intX,intY,retstring) ) 

#define ITSPlugInterFace_FindStrExS(This,x1,y1,x2,y2,string,color_format,sim,retstring)	\
    ( (This)->lpVtbl -> FindStrExS(This,x1,y1,x2,y2,string,color_format,sim,retstring) ) 

#define ITSPlugInterFace_FindStrFastExS(This,x1,y1,x2,y2,string,color_format,sim,retstring)	\
    ( (This)->lpVtbl -> FindStrFastExS(This,x1,y1,x2,y2,string,color_format,sim,retstring) ) 

#define ITSPlugInterFace_FindPicS(This,x1,y1,x2,y2,pic_name,delta_color,sim,dir,intX,intY,retstring)	\
    ( (This)->lpVtbl -> FindPicS(This,x1,y1,x2,y2,pic_name,delta_color,sim,dir,intX,intY,retstring) ) 

#define ITSPlugInterFace_FindPicExS(This,x1,y1,x2,y2,pic_name,delta_color,sim,dir,retstring)	\
    ( (This)->lpVtbl -> FindPicExS(This,x1,y1,x2,y2,pic_name,delta_color,sim,dir,retstring) ) 

#define ITSPlugInterFace_SetDictPwd(This,pwd,nret)	\
    ( (This)->lpVtbl -> SetDictPwd(This,pwd,nret) ) 

#define ITSPlugInterFace_SetPicPwd(This,pwd,nret)	\
    ( (This)->lpVtbl -> SetPicPwd(This,pwd,nret) ) 

#define ITSPlugInterFace_LeftDoubleClick(This,nret)	\
    ( (This)->lpVtbl -> LeftDoubleClick(This,nret) ) 

#define ITSPlugInterFace_FreeProcessMemory(This,hwnd,nret)	\
    ( (This)->lpVtbl -> FreeProcessMemory(This,hwnd,nret) ) 

#define ITSPlugInterFace_TSDXGraphicProtect(This,enable,nret)	\
    ( (This)->lpVtbl -> TSDXGraphicProtect(This,enable,nret) ) 

#define ITSPlugInterFace_TSDXKmProtect(This,enable,type,nret)	\
    ( (This)->lpVtbl -> TSDXKmProtect(This,enable,type,nret) ) 

#define ITSPlugInterFace_CheckFontSmooth(This,nret)	\
    ( (This)->lpVtbl -> CheckFontSmooth(This,nret) ) 

#define ITSPlugInterFace_DisableFontSmooth(This,nret)	\
    ( (This)->lpVtbl -> DisableFontSmooth(This,nret) ) 

#define ITSPlugInterFace_GetScreenData(This,x1,y1,x2,y2,RetAddr)	\
    ( (This)->lpVtbl -> GetScreenData(This,x1,y1,x2,y2,RetAddr) ) 

#define ITSPlugInterFace_EnableRealMouse(This,enable,mousedelay,mousestep,nret)	\
    ( (This)->lpVtbl -> EnableRealMouse(This,enable,mousedelay,mousestep,nret) ) 

#define ITSPlugInterFace_EnableRealKeypad(This,enable,nret)	\
    ( (This)->lpVtbl -> EnableRealKeypad(This,enable,nret) ) 

#define ITSPlugInterFace_MoveToEx(This,x,y,w,h,retstring)	\
    ( (This)->lpVtbl -> MoveToEx(This,x,y,w,h,retstring) ) 

#define ITSPlugInterFace_CheckUAC(This,nret)	\
    ( (This)->lpVtbl -> CheckUAC(This,nret) ) 

#define ITSPlugInterFace_SetUAC(This,enable,nret)	\
    ( (This)->lpVtbl -> SetUAC(This,enable,nret) ) 

#define ITSPlugInterFace_WaitKey(This,vk_code,time_out,nret)	\
    ( (This)->lpVtbl -> WaitKey(This,vk_code,time_out,nret) ) 

#define ITSPlugInterFace_FindMultiColor(This,x1,y1,x2,y2,first_color,offset_color,sim,dir,intX,intY,nret)	\
    ( (This)->lpVtbl -> FindMultiColor(This,x1,y1,x2,y2,first_color,offset_color,sim,dir,intX,intY,nret) ) 

#define ITSPlugInterFace_FindMultiColorEx(This,x1,y1,x2,y2,first_color,offset_color,sim,dir,retstring)	\
    ( (This)->lpVtbl -> FindMultiColorEx(This,x1,y1,x2,y2,first_color,offset_color,sim,dir,retstring) ) 

#define ITSPlugInterFace_SetSimMode(This,mode,nret)	\
    ( (This)->lpVtbl -> SetSimMode(This,mode,nret) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_AsmClear_Proxy( 
    ITSPlugInterFace * This,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_AsmClear_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_AsmCode_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG base_addr,
    /* [retval][out] */ BSTR *retstring);


void __RPC_STUB ITSPlugInterFace_AsmCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_Assemble_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ BSTR asm_code,
    /* [in] */ LONG base_addr,
    /* [in] */ LONG is_upper,
    /* [retval][out] */ BSTR *retstring);


void __RPC_STUB ITSPlugInterFace_Assemble_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_MatchPicName_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ BSTR pic_name,
    /* [retval][out] */ BSTR *retstring);


void __RPC_STUB ITSPlugInterFace_MatchPicName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_SetShowErrorMsg_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG show,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_SetShowErrorMsg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_Reg_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ BSTR reg_code,
    /* [in] */ LONG type,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_Reg_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_GetMachineCode_Proxy( 
    ITSPlugInterFace * This,
    /* [retval][out] */ BSTR *retstring);


void __RPC_STUB ITSPlugInterFace_GetMachineCode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_TSGuardProtectToHide_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG enble,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_TSGuardProtectToHide_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_TSGuardProtectToHide2_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG enable,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_TSGuardProtectToHide2_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_TSGuardProtectToNP_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG enable,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_TSGuardProtectToNP_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_Delay_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG mis,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_Delay_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_FindStrS_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG x1,
    /* [in] */ LONG y1,
    /* [in] */ LONG x2,
    /* [in] */ LONG y2,
    /* [in] */ BSTR string,
    /* [in] */ BSTR color_format,
    /* [in] */ DOUBLE sim,
    /* [out] */ VARIANT *intX,
    /* [out] */ VARIANT *intY,
    /* [retval][out] */ BSTR *retstring);


void __RPC_STUB ITSPlugInterFace_FindStrS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_FindStrFastS_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG x1,
    /* [in] */ LONG y1,
    /* [in] */ LONG x2,
    /* [in] */ LONG y2,
    /* [in] */ BSTR string,
    /* [in] */ BSTR color_format,
    /* [in] */ DOUBLE sim,
    /* [out] */ VARIANT *intX,
    /* [out] */ VARIANT *intY,
    /* [retval][out] */ BSTR *retstring);


void __RPC_STUB ITSPlugInterFace_FindStrFastS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_FindStrExS_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG x1,
    /* [in] */ LONG y1,
    /* [in] */ LONG x2,
    /* [in] */ LONG y2,
    /* [in] */ BSTR string,
    /* [in] */ BSTR color_format,
    /* [in] */ DOUBLE sim,
    /* [retval][out] */ BSTR *retstring);


void __RPC_STUB ITSPlugInterFace_FindStrExS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_FindStrFastExS_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG x1,
    /* [in] */ LONG y1,
    /* [in] */ LONG x2,
    /* [in] */ LONG y2,
    /* [in] */ BSTR string,
    /* [in] */ BSTR color_format,
    /* [in] */ DOUBLE sim,
    /* [retval][out] */ BSTR *retstring);


void __RPC_STUB ITSPlugInterFace_FindStrFastExS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_FindPicS_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG x1,
    /* [in] */ LONG y1,
    /* [in] */ LONG x2,
    /* [in] */ LONG y2,
    /* [in] */ BSTR pic_name,
    /* [in] */ BSTR delta_color,
    /* [in] */ DOUBLE sim,
    /* [in] */ LONG dir,
    /* [out] */ VARIANT *intX,
    /* [out] */ VARIANT *intY,
    /* [retval][out] */ BSTR *retstring);


void __RPC_STUB ITSPlugInterFace_FindPicS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_FindPicExS_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG x1,
    /* [in] */ LONG y1,
    /* [in] */ LONG x2,
    /* [in] */ LONG y2,
    /* [in] */ BSTR pic_name,
    /* [in] */ BSTR delta_color,
    /* [in] */ DOUBLE sim,
    /* [in] */ LONG dir,
    /* [retval][out] */ BSTR *retstring);


void __RPC_STUB ITSPlugInterFace_FindPicExS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_SetDictPwd_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ BSTR pwd,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_SetDictPwd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_SetPicPwd_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ BSTR pwd,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_SetPicPwd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_LeftDoubleClick_Proxy( 
    ITSPlugInterFace * This,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_LeftDoubleClick_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_FreeProcessMemory_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG hwnd,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_FreeProcessMemory_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_TSDXGraphicProtect_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG enable,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_TSDXGraphicProtect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_TSDXKmProtect_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG enable,
    /* [in] */ BSTR type,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_TSDXKmProtect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_CheckFontSmooth_Proxy( 
    ITSPlugInterFace * This,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_CheckFontSmooth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_DisableFontSmooth_Proxy( 
    ITSPlugInterFace * This,
    /* [retval][out] */ LONG *nret);


void __RPC_STUB ITSPlugInterFace_DisableFontSmooth_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_GetScreenData_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ LONG x1,
    /* [in] */ LONG y1,
    /* [in] */ LONG x2,
    /* [in] */ LONG y2,
    /* [retval][out] */ LONG *RetAddr);


void __RPC_STUB ITSPlugInterFace_GetScreenData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_EnableRealMouse_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ ULONG enable,
    /* [in] */ ULONG mousedelay,
    /* [in] */ ULONG mousestep,
    /* [retval][out] */ ULONG *nret);


void __RPC_STUB ITSPlugInterFace_EnableRealMouse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_EnableRealKeypad_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ ULONG enable,
    /* [retval][out] */ ULONG *nret);


void __RPC_STUB ITSPlugInterFace_EnableRealKeypad_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_MoveToEx_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ ULONG x,
    /* [in] */ ULONG y,
    /* [in] */ ULONG w,
    /* [in] */ ULONG h,
    /* [retval][out] */ BSTR *retstring);


void __RPC_STUB ITSPlugInterFace_MoveToEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_CheckUAC_Proxy( 
    ITSPlugInterFace * This,
    /* [retval][out] */ ULONG *nret);


void __RPC_STUB ITSPlugInterFace_CheckUAC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_SetUAC_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ ULONG enable,
    /* [retval][out] */ ULONG *nret);


void __RPC_STUB ITSPlugInterFace_SetUAC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_WaitKey_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ ULONG vk_code,
    /* [in] */ ULONG time_out,
    /* [retval][out] */ ULONG *nret);


void __RPC_STUB ITSPlugInterFace_WaitKey_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_FindMultiColor_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ ULONG x1,
    /* [in] */ ULONG y1,
    /* [in] */ ULONG x2,
    /* [in] */ ULONG y2,
    /* [in] */ BSTR first_color,
    /* [in] */ BSTR offset_color,
    /* [in] */ DOUBLE sim,
    /* [in] */ ULONG dir,
    /* [out] */ VARIANT *intX,
    /* [out] */ VARIANT *intY,
    /* [retval][out] */ ULONG *nret);


void __RPC_STUB ITSPlugInterFace_FindMultiColor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_FindMultiColorEx_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ ULONG x1,
    /* [in] */ ULONG y1,
    /* [in] */ ULONG x2,
    /* [in] */ ULONG y2,
    /* [in] */ BSTR first_color,
    /* [in] */ BSTR offset_color,
    /* [in] */ DOUBLE sim,
    /* [in] */ ULONG dir,
    /* [retval][out] */ BSTR *retstring);


void __RPC_STUB ITSPlugInterFace_FindMultiColorEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ITSPlugInterFace_SetSimMode_Proxy( 
    ITSPlugInterFace * This,
    /* [in] */ ULONG mode,
    /* [retval][out] */ ULONG *nret);


void __RPC_STUB ITSPlugInterFace_SetSimMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITSPlugInterFace_INTERFACE_DEFINED__ */



#ifndef __TSPlugLib_LIBRARY_DEFINED__
#define __TSPlugLib_LIBRARY_DEFINED__

/* library TSPlugLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_TSPlugLib;

EXTERN_C const CLSID CLSID_TSPlugInterFace;

#ifdef __cplusplus

class DECLSPEC_UUID("BCE4A484-C3BC-418B-B1F6-69D6987C126B")
TSPlugInterFace;
#endif
#endif /* __TSPlugLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


