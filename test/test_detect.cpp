#pragma once
# include "feature_detect.h"
// # include "feature_detect_cpu.h"
# include"test.h"
# include <map>
# include <iostream>
# include <windows.h>
# include "checkDevice.h"
#include "Teeth_Detector.h"
using namespace std;
#define IMAGE_NUM_UP 14
#define IMAGE_NUM_LOW 14

Teeth_Detector* get_TD_Obj(char* graph_path, LPCSTR dllpath
) {
	typedef Teeth_Detector*(*loadFun)(char* graph_path); //宏定义函数指针类型
	HINSTANCE hDll; //DLL句柄 
	cout << "\n inside  :  " << dllpath << endl;
	hDll = LoadLibrary(dllpath);

	if (hDll == NULL) {
		int dwError = GetLastError();
		cout << "load failed \n" << dwError;
	}
	loadFun getObj = (loadFun)GetProcAddress(hDll, "getObj");
	return getObj(graph_path);
}

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
	lpAddFun getObj = (lpAddFun)GetProcAddress(hDll, "getObj");
	return getObj(box_size, group_id, group_path, group_num);
}

int main(int, char *[])
{


// 	LPCSTR dllpath = "TeethDetect_GPU.dll";
// 	cout << dllpath;
// 	checkGpuMem();
// 
// 	Teeth_Detector* td = get_TD_Obj(
// 		"E:/TensorFlowCplusplus/TeethDetect/x64/Release/output_graph.pb", dllpath);
// 	checkGpuMem();
// 
// 	int num, w, h;
// 	float** coord1 = new float*[14];
// 	for (int i = 0; i < 14; i++) {
// 		coord1[i] = new float[4];
// 	}
// 	for (int i = 0; i < 2; i++)
// 	{
// 		td->detect("E:/TensorFlowCplusplus/TeethDetect/x64/Release/up.png", num, coord1, w, h);
// 		cout << "num = " << num << endl;
// // 		for (int i = 0; i < num; ++i) {
// // 			for (int j = 0; j < 4; ++j)
// // 				// using (index_1, index_2, index_3) to access the element in a tensor
// // 				cout << coord1[i][j] << "\t";
// // 			std::cout << std::endl;
// // 		}
// 
// 	}
// 	checkGpuMem();
// 	cout << "\n TF finish!!  \n";


	///////////////////////
	vtkSmartPointer<vtkImageData> vtkImageVec_up[14];
	//vtkSmartPointer<vtkImageData> tmp= loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel3");
	//for (int i = 0; i < IMAGE_NUM_UP; i++) {
	//	vtkImageVec_up[i] = tmp;
	//}
	int up_num = 14;
	for (int i = 0; i < up_num; i++) {
		vtkImageVec_up[i] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel2");
	}


	char* model_path[] = 
	{
	  "E://TensorFlowCplusplus//feature_detect//x64//Release//up_graph.pb",
	  "E://TensorFlowCplusplus//feature_detect//x64//Release//low_graph.pb"
	};

	Teeth_Group model_id[] = { up,low};
// 	LPCSTR dllpath;
	char* dllpath;
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
		checkGpuMem();

// 		cout << " single arch finish \n";
// 		fd->detect(low, vtkImageVec_up, up_num, coord, 27);
// // 		checkGpuMem();

// 		cout << " both arch finish \n\n\n";

	stop = timeGetTime();
	printf("Use Time:%ld\n", stop - start);



	for (int i = 0; i < up_num; i++) {
		for(int j=0;j<27;j++)
			cout<<int(coord[i][j])<<" ";
		cout << endl;
	}
		cout << "FD_finish\n";


	/////////////////////



	return 0;
}
      