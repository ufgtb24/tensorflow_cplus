//#include <stdio.h>
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <iostream>
using namespace std;
int checkGpuMem()

{

	float free_m, total_m, used_m;

	size_t free_t, total_t;

	cudaMemGetInfo(&free_t, &total_t);
	//(unsigned int)
	free_m = (int)(free_t / 1048576.0);

	total_m = (int)(total_t / 1048576.0);

	used_m = total_m - free_m;
	//cout <<"\nfree_m:"<< free_m << "    total_m:" << total_m << "    used_m:" << used_m<<endl;
	return used_m;
}
int main_1(int, char *[]) {
	checkGpuMem();
}
