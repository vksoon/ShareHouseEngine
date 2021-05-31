#pragma once

// specific microsoft window platform
#if SH_PLATFORM == SH_PLATFORM_MSW 

struct HDC__;
typedef HDC__* HDC;
struct HWND__;
typedef HWND__* HWND;
struct HMONITOR__;
typedef HMONITOR__* HMONITOR;
typedef int BOOL;
#define CALLBACK __stdcall
typedef struct tagRECT* LPRECT;
struct HINSTANCE__;
typedef HINSTANCE__* HINSTANCE;

#if defined(_WIN64)
typedef __int64 INT_PTR, * PINT_PTR;
typedef unsigned __int64 UINT_PTR, * PUINT_PTR;

typedef __int64 LONG_PTR, * PLONG_PTR;
typedef unsigned __int64 ULONG_PTR, * PULONG_PTR;

#define __int3264   __int64

#else
typedef _W64 int INT_PTR, * PINT_PTR;
typedef _W64 unsigned int UINT_PTR, * PUINT_PTR;

typedef _W64 long LONG_PTR, * PLONG_PTR;
typedef _W64 unsigned long ULONG_PTR, * PULONG_PTR;

#define __int3264   __int32

#endif

typedef LONG_PTR LPARAM;
typedef char CHAR;
typedef _Null_terminated_ CHAR* NPSTR, * LPSTR, * PSTR;

struct HFONT__;
typedef HFONT__* HFONT;

SH_NAMESPACE_BEGIN_EX(Platform)

#ifdef _WIN32
using WND_TYPE = HWND;
#else
using WND_TYPE = int32;
#endif // _WIN32

SH_NAMESPACE_END_EX

#endif