#pragma once



#if defined(D3D11GRAPHICSKERNEL_EXPORTS)
#		define PLUGIN_EXTERN_API extern "C" __declspec(dllexport)
#		define PLUGIN_API __declspec(dllexport)
#else 
#		define PLUGIN_EXTERN_API extern "C" __declspec(dllimport)
#		define PLUGIN_API __declspec(dllimport)
#endif 