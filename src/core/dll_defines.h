#ifndef DLL_EXPORTABLE_H
#define DLL_EXPORTABLE_H

#ifdef DLL_EXPORTABLE
#define dll_export __declspec(dllexport) 
#else
#define dll_export 
#endif //DLL_EXPORTABLE

#define DLL_IMPORT __declspec(dllimport)

#endif //DLL_EXPORTABLE_H