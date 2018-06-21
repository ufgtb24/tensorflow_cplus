# include "feature_detect.h"
#include"test.h"
#include <map>
# include <iostream>
using namespace std;
#define IMAGE_NUM 1
int main(int, char *[])
{
	vtkSmartPointer<vtkImageData> vtkImageVec[IMAGE_NUM];

	vtkImageVec[0] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel26");

	//makeVtkImages(num, vtkImageVec);





	char* type_path[] = 
	{"E://TensorFlowCplusplus//feature_detect//x64//Release//low_graph.pb"};

	Teeth_Group type_id[] = { up };

	Feature_detector* fd = new Feature_detector(128, type_id,type_path,1);
	int teethType[10];
	float** coord = new float*[IMAGE_NUM];
	for (int i = 0; i < IMAGE_NUM; i++) {
		coord[i] = new float[21];
	}
	fd->detect(up, vtkImageVec, IMAGE_NUM, teethType, coord, 21);
	for (int i = 0; i < IMAGE_NUM; i++) {
		cout <<"teeth_type is : "<< teethType[i] << endl;
		for(int j=0;j<21;j++)
			cout<<coord[i][j]<<"   ";
	}
	cout << endl;
}
      