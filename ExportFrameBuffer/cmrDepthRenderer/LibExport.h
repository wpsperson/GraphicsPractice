#pragma once
#pragma warning( disable: 4251 )

#ifdef DEPTHR_ENDERER_SHARED
#define DEPTHR_ENDERER_EXPORT __declspec(dllexport)
#else
#define DEPTHR_ENDERER_EXPORT 
#endif