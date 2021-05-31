#pragma once

// Platform-specific stuff
#include "ShPlatformDefines.h"

// Common macro defines
#include "ShCommonDefines.h"

#if SH_COMPILER == SH_COMPILER_MSVC

// TODO - This is not deactivated anywhere, therefore it applies to any file that includes this header.
//      - Right now I don't have an easier way to apply these warnings globally so I'm keeping it this way.

// Secure versions aren't multi-platform, so we won't be using them
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#	pragma warning (disable: 4251)

// disable: 'X' Function call with parameters that may be unsafe
#	pragma warning(disable: 4996)

// disable: decorated name length exceeded, name was truncated
// Happens with really long type names. Even fairly standard use
// of std::unordered_map with custom parameters, meaning I can't
// really do much to avoid it. It shouldn't effect execution
// but might cause problems if you compile library
// with one compiler and use it in another.
#	pragma warning(disable: 4503)

// disable: C++ exception handler used, but unwind semantics are not enabled
// We don't care about this as any exception is meant to crash the program.
#	pragma warning(disable: 4530)

#endif

#if SH_DEBUG_MODE
#	define SH_DEBUG_ONLY(x) x
#	define SH_ASSERT(x) assert(x)
#	define SH_MEM_USE_TRACE 1
#else
#	define SH_DEBUG_ONLY(x)
#	define SH_ASSERT(x)
#	define SH_MEM_USE_TRACE 0
#endif

// Short-hand names for various built-in types
#include "ShTypes.h"

// Standard memory allocators
#include "Foundation/ShMemoryAllocator.h"

// Standard string 
#include "Foundation/ShString.h"

// Commonly used standard headers
#include "ShStandardHeaders.h"