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







#include <algorithm>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <limits.h>
#include <intrin.h>
typedef unsigned __int32  uint32_t;
#else
#include <stdint.h>
#endif
using namespace std;

#define MAX_INTEL_TOP_LVL 4


//https://gist.github.com/9prady9/a5e1e8bdbc9dc58b3349


class CHECK_DEVICE_API CPUID {
	uint32_t regs[4];

public:
	explicit CPUID(unsigned funcId, unsigned subFuncId) {
#ifdef _WIN32
		__cpuidex((int*)regs, (int)funcId, (int)subFuncId);

#else
		asm volatile
			("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
				: "a" (funcId), "c" (subFuncId));
		// ECX is set to zero for CPUID function 4
#endif
	}

	const uint32_t& EAX() const { return regs[0]; }
	const uint32_t& EBX() const { return regs[1]; }
	const uint32_t& ECX() const { return regs[2]; }
	const uint32_t& EDX() const { return regs[3]; }
};

class CHECK_DEVICE_API CPUInfo {
public:
	CPUInfo();
	string  vendor()            const { return mVendorId; }
	string  model()             const { return mModelName; }
	int     cores()             const { return mNumCores; }
	float   cpuSpeedInMHz()     const { return mCPUMHz; }
	bool    isSSE()             const { return mIsSSE; }
	bool    isSSE2()            const { return mIsSSE2; }
	bool    isSSE3()            const { return mIsSSE3; }
	bool    isSSE41()           const { return mIsSSE41; }
	bool    isSSE42()           const { return mIsSSE42; }
	bool    isAVX()             const { return mIsAVX; }
	bool    isAVX2()            const { return mIsAVX2; }
	bool    isHyperThreaded()   const { return mIsHTT; }
	int     logicalCpus()       const { return mNumLogCpus; }

private:
	// Bit positions for data extractions
	static const uint32_t SSE_POS = 0x02000000;
	static const uint32_t SSE2_POS = 0x04000000;
	static const uint32_t SSE3_POS = 0x00000001;
	static const uint32_t SSE41_POS = 0x00080000;
	static const uint32_t SSE42_POS = 0x00100000;
	static const uint32_t AVX_POS = 0x10000000;
	static const uint32_t AVX2_POS = 0x00000020;
	static const uint32_t LVL_NUM = 0x000000FF;
	static const uint32_t LVL_TYPE = 0x0000FF00;
	static const uint32_t LVL_CORES = 0x0000FFFF;

	// Attributes
	string mVendorId;
	string mModelName;
	int    mNumSMT;
	int    mNumCores;
	int    mNumLogCpus;
	float  mCPUMHz;
	bool   mIsHTT;
	bool   mIsSSE;
	bool   mIsSSE2;
	bool   mIsSSE3;
	bool   mIsSSE41;
	bool   mIsSSE42;
	bool   mIsAVX;
	bool   mIsAVX2;
};

CHECK_DEVICE_API int checkGpuMem(float& total_m, float& free_m, float& used_m);
CHECK_DEVICE_API bool checkDevice(void);
CHECK_DEVICE_API void checkmem(float& total_mem, float& avail_mem, float& used_mem);
