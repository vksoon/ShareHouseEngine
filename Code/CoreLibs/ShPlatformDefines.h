#pragma once

// Microsoft Window Platform 
#define SH_PLATFORM_MSW				1
// Universal Window platform
#define SH_PLATFORM_UWP				2

#define SH_COMPILER_MSVC			1

#define SH_ARCHITECTURE_x86_32		1
#define SH_ARCHITECTURE_x86_64		2

#if defined(_MSC_VER) 
#	define SH_COMPILER SH_COMPILER_MSVC
#	define SH_COMP_VER _MSC_VER
#	define SH_THREADLOCAL __declspec(thread)
#	define SH_STDCALL __stdcall
#	define SH_CDECL __cdecl
#	define SH_FALLTHROUGH
#	undef __PRETTY_FUNCTION__
#	define __PRETTY_FUNCTION__ __FUNCSIG__
#else
#	pragma error "Unknown compiler. "
#endif

// Find the platform
#if defined( __WIN32__ ) || defined( _WIN32 )
#	define SH_PLATFORM SH_PLATFORM_MSW
#endif

// Find the architecture type
#if defined(__x86_64__) || defined(_M_X64)
#	define SH_ARCH_TYPE SH_ARCHITECTURE_x86_64
#else
#	define SH_ARCH_TYPE SH_ARCHITECTURE_x86_32
#endif

// DLL export
#if defined(CORELIBS_EXPORTS)
#	if SH_COMPILER == SH_COMPILER_MSVC
#		define SH_CORE_EXTERN_API extern "C" __declspec(dllexport)
#		define SH_CORE_API __declspec(dllexport)
#	else 
#		pragma error "Unknown compiler."
#	endif 
#else 
#	if SH_COMPILER == SH_COMPILER_MSVC
#		define SH_CORE_EXTERN_API extern "C" __declspec(dllimport)
#		define SH_CORE_API __declspec(dllimport)
#	else 
#		pragma error "Unknown compiler."
#	endif 
#endif 

// DLL export for plug-in
#if SH_PLATFORM == SH_PLATFORM_MSW 
#	if SH_COMPILER == SH_COMPILER_MSVC
#		define SH_PLUGIN_EXTERN_API extern "C" __declspec(dllexport)
#		define SH_PLUGIN_API __declspec(dllexport)
#	else 
#		pragma error "Unknown compiler."
#	endif 
#endif

// Windows Settings
#if SH_PLATFORM == SH_PLATFORM_MSW
#	if defined(_DEBUG) || defined(DEBUG)
#		define SH_DEBUG_MODE 1
#	else
#		define SH_DEBUG_MODE 0
#	endif
#else 
#		pragma error "Unsupported platform."
#endif 





