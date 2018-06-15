# include "feature_detect.h"
#include"test.h"
#include <map>
# include <iostream>
using namespace std;
int main(int, char *[])
{
	int num = 5;
	vector<vtkSmartPointer<vtkImageData>> vtkImageVec;
	makeVtkImages(num, vtkImageVec);
	map<Teeth_Group, string> type_path;
	type_path[up] = "E://TensorFlowCplusplus//feature_detect//x64//Release//low_graph.pb";
	Feature_detector* fd = new Feature_detector(128, type_path);
	cout << "finish";
	int teethType[1000];
	float** coord = new float*[5];
	for (int i = 0; i < 5; i++) {
		coord[i] = new float[22];
	}
	fd->detect(up, vtkImageVec, teethType, coord, 21);
	for (int i = 0; i < 5; i++) {
		for(int j=0;j<22;j++)
			cout<<coord[i][j]<<"   ";
	}
	cout << endl;
}
      