# include "feature_detect.h"
#include"test.h"
#include <map>
# include <iostream>
using namespace std;
#define MAX_NUM 14
#define IMAGE_NUM_UP 14
#define IMAGE_NUM_LOW 1
int main(int, char *[])
{
	vtkSmartPointer<vtkImageData> vtkImageVec_up[IMAGE_NUM_UP];
	vtkSmartPointer<vtkImageData> vtkImageVec_low[IMAGE_NUM_LOW];
	//vtkSmartPointer<vtkImageData> tmp= loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel3");
	//for (int i = 0; i < IMAGE_NUM_UP; i++) {
	//	vtkImageVec_up[i] = tmp;
	//}

	vtkImageVec_up[0] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel2");
	vtkImageVec_up[1] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel3");
	vtkImageVec_up[2] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel4");
	vtkImageVec_up[3] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel5");
	vtkImageVec_up[4] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel6");
	vtkImageVec_up[5] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel7");
	vtkImageVec_up[6] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel8");
	vtkImageVec_up[7] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel2");
	vtkImageVec_up[8] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel3");
	vtkImageVec_up[9] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel4");
	vtkImageVec_up[10] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel5");
	vtkImageVec_up[11] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel6");
	vtkImageVec_up[12] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel7");
	vtkImageVec_up[13] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel8");


	vtkImageVec_low[0] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel31");

	//makeVtkImages(num, vtkImageVec);





	char* type_path[] = 
	{"E://TensorFlowCplusplus//feature_detect//x64//Release//up_graph.pb",
	  "E://TensorFlowCplusplus//feature_detect//x64//Release//low_graph.pb"
	};

	Teeth_Group type_id[] = { up,low};

	Feature_detector* fd = new Feature_detector(128, type_id,type_path,2);
	int teethType[MAX_NUM];
	float** coord = new float*[MAX_NUM];
	for (int i = 0; i < MAX_NUM; i++) {
		coord[i] = new float[21];
	}
	cout << endl << "up detect !!!!!";

	fd->detect(up, vtkImageVec_up, IMAGE_NUM_UP, teethType, coord, 21);
	for (int i = 0; i < IMAGE_NUM_UP; i++) {
		cout <<"teeth_type is : "<< teethType[i] << endl;
		for(int j=0;j<21;j++)
			cout<<int(coord[i][j])<<"   ";
		cout << endl;
	}

	//cout << endl<<"low detect !!!!!!!!";

	//fd->detect(low, vtkImageVec_low, IMAGE_NUM_LOW, teethType, coord, 21);
	//for (int i = 0; i < IMAGE_NUM_LOW; i++) {
	//	cout << "teeth_type is : " << teethType[i] << endl;
	//	for (int j = 0; j<21; j++)
	//		cout << coord[i][j] << "   ";
	//}


	//cout << endl;
}
      