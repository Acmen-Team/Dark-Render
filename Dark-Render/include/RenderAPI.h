#pragma once

#ifdef DK_PLATFORM_WINDOWS
#if DK_DYNAMIC_LINK
#ifdef DRENDER_BUILD_DLL
#define DRENDER_API _declspec(dllexport)
#else
#define DRENDER_API _declspec(dllimport)
#endif
#else
#define DRENDER_API
#endif
#else
#error Dark Currently Supports Windows Only!
#endif
