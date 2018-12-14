#pragma once
# include "feature_detect.h"
// # include "feature_detect_cpu.h"
# include"test.h"
# include <map>
# include <iostream>
# include <windows.h>
# include "checkDevice.h"
using namespace std;
#define IMAGE_NUM_UP 14
#define IMAGE_NUM_LOW 14


Feature_detector* get_FD_Obj(int box_size,
	Teeth_Group group_id[],
	char* group_path[],
	int group_num,
	LPCSTR dllpath
// 	LPCWSTR dllpath
	) {

	typedef Feature_detector*(*lpAddFun)(int box_size,
		Teeth_Group group_id[],
		char* group_path[],
		int group_num); //宏定义函数指针类型

	HINSTANCE hDll; //DLL句柄 
	hDll = LoadLibrary(dllpath);

	if (hDll == NULL) {
		int dwError = GetLastError();
		cout << "load failed \n" << dwError;
	}
	lpAddFun getFDObj = (lpAddFun)GetProcAddress(hDll, "getFDObj");
	return getFDObj(box_size, group_id, group_path, group_num);
}

int main(int, char *[])
{


	vtkSmartPointer<vtkImageData> vtkImageVec_up[71];
	vtkSmartPointer<vtkImageData> vtkImageVec_low[14];
	//vtkSmartPointer<vtkImageData> tmp= loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel3");
	//for (int i = 0; i < IMAGE_NUM_UP; i++) {
	//	vtkImageVec_up[i] = tmp;
	//}
	int up_num = 14;
	for (int i = 0; i < up_num; i++) {
		vtkImageVec_up[i] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel2");
	}

	int low_num = 7;

	char* model_path[] = 
	{
	  "E://TensorFlowCplusplus//feature_detect//x64//Release//up_graph.pb",
	  "E://TensorFlowCplusplus//feature_detect//x64//Release//low_graph.pb"
	};

	Teeth_Group model_id[] = { up,low};
	LPCSTR dllpath;
	if (checkDevice()) 
		dllpath = "feature_detect.dll";
	else
		dllpath = "feature_detect_cpu.dll";

	cout <<"dllpath = "<< dllpath << endl;

	Feature_detector* fd = get_FD_Obj(128, model_id, model_path, 2, dllpath);


// 	if (!checkDevice())
// 		fd = new Feature_detector(128, model_id, model_path, 2);
// 
//   	else
//   		fd = new Feature_detector_cpu(128, model_id, model_path, 2);

	float** coord = new float*[up_num];
	for (int i = 0; i < up_num; i++) {
		coord[i] = new float[21];
	}
	//int use_0 = checkGpuMem();
	//cout << endl << "up detect------------------";
	time_t start, stop;
	start = timeGetTime();
	fd->detect(up, vtkImageVec_up, up_num, coord, 27);
	cout << " single arch finish \n";
// 	fd->detect(low, vtkImageVec_up, up_num, coord, 27);
	stop = timeGetTime();
	printf("Use Time:%ld\n", (stop - start)/1000);

	//int use_up = checkGpuMem() - use_0;
	//cout << "use_up:  "<< use_up << endl;


	for (int i = 0; i < up_num; i++) {
		for(int j=0;j<27;j++)
			cout<<int(coord[i][j])<<"   ";
		cout << endl;
	}
	return 0;
}
      