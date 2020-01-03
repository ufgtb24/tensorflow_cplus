
# include <iostream>
# include "checkDevice.h"
using namespace std;

//int main()
//{
//	cout << "support CUDA?: " << checkDevice() << endl;
//
//	float gpu_mem_total, gpu_mem_free, gpu_mem_used;
//	checkGpuMem(gpu_mem_total, gpu_mem_free, gpu_mem_used);
//	cout << gpu_mem_total << "  " << gpu_mem_free << "  " << gpu_mem_used << endl;
//
//	float mem1, mem2, mem3;
//	checkmem(mem1, mem2, mem3);
//	cout << mem1 << "  " << mem2 << "  " << mem3 << endl;
//
//	return 0;
//}

int main0()
{
	//cout<<checkDevice()<<endl;
	//getchar();

	CPUInfo cinfo;

	cout << "CPU vendor = " << cinfo.vendor() << endl;
	cout << "CPU Brand String = " << cinfo.model() << endl;
	cout << "# of cores = " << cinfo.cores() << endl;
	cout << "# of logical cores = " << cinfo.logicalCpus() << endl;
	cout << "Is CPU Hyper threaded = " << cinfo.isHyperThreaded() << endl;
	cout << "CPU SSE = " << cinfo.isSSE() << endl;
	cout << "CPU SSE2 = " << cinfo.isSSE2() << endl;
	cout << "CPU SSE3 = " << cinfo.isSSE3() << endl;
	cout << "CPU SSE41 = " << cinfo.isSSE41() << endl;
	cout << "CPU SSE42 = " << cinfo.isSSE42() << endl;
	cout << "CPU AVX = " << cinfo.isAVX() << endl;
	cout << "CPU AVX2 = " << cinfo.isAVX2() << endl;

	return 0;
}