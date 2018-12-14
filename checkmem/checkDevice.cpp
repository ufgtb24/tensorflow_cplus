// checkmem.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "checkDevice.h"
#include <cuda_runtime_api.h>
#include <iostream>
using namespace std;


// This is the constructor of a class that has been exported.
// see checkmem.h for the class definition

CHECK_DEVICE_API int checkGpuMem()
{
	float free_m, total_m, used_m;

	size_t free_t, total_t;

	cudaMemGetInfo(&free_t, &total_t);
	//(unsigned int)
	free_m = (int)(free_t / 1048576.0);

	total_m = (int)(total_t / 1048576.0);

	used_m = total_m - free_m;
	cout << "\nfree_m:" << free_m << "    total_m:" << total_m << "    used_m:" << used_m << endl;
	return free_m;
}

CHECK_DEVICE_API bool checkDevice(void)
{
	int count;
	cudaError_t cudaResultCode = cudaGetDeviceCount(&count);
	cout << "cudaResultCode= " << cudaResultCode << endl;
	if (cudaResultCode == cudaSuccess)
	{
		cout << "CUDA Success\n";
		return true;
	}
	else
	{
		cout << "CUDA Fail\n";
		return false;
	}
}


