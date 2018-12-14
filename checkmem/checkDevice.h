// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CHECKMEM_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CHECKMEM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CHECKMEM_EXPORTS
#define CHECK_DEVICE_API __declspec(dllexport)
#else
#define CHECK_DEVICE_API __declspec(dllimport)
#endif

CHECK_DEVICE_API int checkGpuMem(void);
CHECK_DEVICE_API bool checkDevice(void);
