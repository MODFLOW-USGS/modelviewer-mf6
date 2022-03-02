#ifndef __mvHeader_h
#define __mvHeader_h

#ifdef MV_DLL
#define MV_EXPORT __declspec(dllexport)
#else
#define MV_EXPORT __declspec(dllimport)
#endif

#endif