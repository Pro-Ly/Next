#pragma once

#include <memory>

#ifdef NX_PLATFORM_WINDOWS
#if NX_DYNAMIC_LINK
	#ifdef NX_BUILD_DLL
		#define NEXT_API __declspec(dllexport)
	#else
		#define NEXT_API __declspec(dllimport)
	#endif
#else
    #define NEXT_API
#endif
#else
	#error Next only supports Windows!
#endif

#ifdef NX_DEBUG
	#define NX_ENABLE_ASSERTS
#endif

#ifdef  NX_ENABLE_ASSERTS
	#define NX_ASSERT(x, ...) { if(!(x)) { NX_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define NX_CORE_ASSERT(x, ...) { if(!(x)) { NX_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define NX_ASSERT(x, ...)
	#define NX_CORE_ASSERT(x, ...)
#endif 

#define BIT(x) (1 << x)

#define NX_BIND_EVENT_FN(fn) std::bind(&fn,this,std::placeholders::_1)


