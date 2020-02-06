#pragma once
# include "feature_detect.h"
// # include "feature_detect_cpu.h"
# include"test.h"
# include <map>
# include <iostream>
#include <sstream>
# include <windows.h>
# include "checkDevice.h"
using namespace std;
//#define IMAGE_NUM_UP 14
//#define IMAGE_NUM_LOW 14




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

//#define cap 14
int main(int, char *[])
{

	///////////////////////
	vtkSmartPointer<vtkImageData> vtkImageVec_up[MAX_NUM];
	vtkSmartPointer<vtkImageData> vtkImageVec_low[MAX_NUM];
	int up_num = 14;
	int low_num = 14;
	for (int i = 0; i < up_num; i++) {
		ostringstream ostr;
		//ostr << "F://ProjectData//feature_detect//ToothLabels//toothLabel"<<i+2<<".mhd";
		ostr << "F://ProjectData//AI_debug//AIOutput (6)//AIOutput//vtkImageData//toothLabel"<< i+2 <<".mhd";
		string file_name = ostr.str();
		vtkSmartPointer<vtkImageData> tmp = loadmhd(file_name);
		vtkImageVec_up[i] = tmp;
	}

	for (int i = 0; i < low_num; i++) {
		ostringstream ostr;
		//ostr << "F://ProjectData//feature_detect//ToothLabels//toothLabel"<<i+18<<".mhd";
		ostr << "F://ProjectData//AI_debug//AIOutput (6)//AIOutput//vtkImageData//toothLabel"<< i+18 <<".mhd";
		string file_name = ostr.str();
		vtkSmartPointer<vtkImageData> tmp = loadmhd(file_name);
		vtkImageVec_low[i] = tmp;
	}

	//for (int i = 0; i < up_num; i++) {
	//	vtkImageVec_up[i] = loadmhd(
	//		"F://ProjectData//feature_detect//ToothLabels//toothLabel19.mhd");
	//}


	char* model_path[] = 
	{
	  "E://TensorFlowCplusplus//feature_detect//x64//Release//up_graph.pb",
	  "E://TensorFlowCplusplus//feature_detect//x64//Release//low_graph.pb"
	};

	Teeth_Group model_id[] = { up,low};
// 	LPCSTR dllpath;
	char* dllpath;
	bool use_gpu = true;
	if (!checkDevice())
	//if (use_gpu)
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
	float** coord2 = new float*[low_num];
	for (int i = 0; i < low_num; i++) {
		coord2[i] = new float[21];
	}


	int status1=fd->detect(up, vtkImageVec_up, up_num, coord, 27);
	cout << status1 << endl;
	for (int i = 0; i < up_num; i++) {
		for (int j = 0; j < 27; j++)
			cout << int(coord[i][j]) << " ";
		cout << endl;
	}
	cout << "up_finish\n";


	int status2 = fd->detect(low, vtkImageVec_low, low_num, coord2, 27);
	cout << status2 << endl;

	for (int i = 0; i < low_num; i++) {
		for (int j = 0; j < 27; j++)
			cout << int(coord2[i][j]) << " ";
		cout << endl;
	}
	cout << "low_finish\n";

	//int use_0 = checkGpuMem();
	//cout << endl << "up detect------------------";
	//time_t start, stop;
	//start = timeGetTime();
	//for (int t = 0; t < 1; t++) {
	//	fd->detect(up, vtkImageVec_up, up_num, coord, 27);


	//	stop = timeGetTime();
	//	printf("Use Time:%ld\n", stop - start);



	//	for (int i = 0; i < up_num; i++) {
	//		for (int j = 0; j < 27; j++)
	//			cout << int(coord[i][j]) << " ";
	//		cout << endl;
	//	}
	//	cout << "FD_finish\n";
	//}

	/////////////////////



	return 0;
}
      