#pragma once

#include <Windows.h>

#include "ShCommonDefines.h"

SH_NAMESPACE_BEGIN_EX(Windows)

typedef void* HANDLE;
typedef HINSTANCE__* HINSTANCE;	
typedef HINSTANCE HMODULE;
typedef HWND__* HWND;
typedef HKEY__* HKEY;
typedef HDC__* HDC;
typedef HICON__* HICON;
typedef HICON__* HCURSOR;

SH_NAMESPACE_END_EX

//SH_NAMESPACE_BEGIN_EX(WindowsOutputDeviceConstants)
//
//uint32 windowsOutputHandls = STD_OUTPUT_HANDLE;
//uint32 windowsAttachPaterntProcess = ATTACH_PARENT_PROCESS;
//
//SH_NAMESPACE_END_EX
