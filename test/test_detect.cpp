# include "feature_detect.h"
#include"test.h"
#include <map>
# include <iostream>

using namespace std;
#define IMAGE_NUM_UP 14
#define IMAGE_NUM_LOW 14
int main(int, char *[])
{
	vtkSmartPointer<vtkImageData> vtkImageVec_up[14];
	vtkSmartPointer<vtkImageData> vtkImageVec_low[14];
	//vtkSmartPointer<vtkImageData> tmp= loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel3");
	//for (int i = 0; i < IMAGE_NUM_UP; i++) {
	//	vtkImageVec_up[i] = tmp;
	//}

	vtkImageVec_up[0] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel2");
	vtkImageVec_up[1] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel3");
	vtkImageVec_up[2] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel4");
	//vtkImageVec_up[3] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel5");
	//vtkImageVec_up[4] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel6");
	//vtkImageVec_up[5] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel7");
	//vtkImageVec_up[6] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel8");
	//vtkImageVec_up[7] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel2");
	//vtkImageVec_up[8] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel3");
	//vtkImageVec_up[9] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel4");
	//vtkImageVec_up[10] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel5");
	//vtkImageVec_up[11] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel6");
	//vtkImageVec_up[12] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel7");
	//vtkImageVec_up[13] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel8");
	int up_num = 3;

	vtkImageVec_low[0] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel25");
	vtkImageVec_low[1] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel26");
	vtkImageVec_low[2] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel27");
	vtkImageVec_low[3] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel28");
	vtkImageVec_low[4] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel29");
	vtkImageVec_low[5] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel30");
	vtkImageVec_low[6] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel31");
	//vtkImageVec_low[7] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel25");
	//vtkImageVec_low[8] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel26");
	//vtkImageVec_low[9] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel27");
	//vtkImageVec_low[10] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel28");
	//vtkImageVec_low[11] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel29");
	//vtkImageVec_low[12] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel30");
	//vtkImageVec_low[13] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel31");
	int low_num = 7;


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
	int use_0 = checkGpuMem();
	cout << endl << "up detect------------------";

	fd->detect(up, vtkImageVec_up,up_num, teethType, coord, 21);
	int use_up = checkGpuMem() - use_0;
	cout << "use_up:  "<< use_up << endl;




	//for (int i = 0; i < up_num; i++) {
	//	cout <<"teeth_type is : "<< teethType[i] << endl;
	//	for(int j=0;j<21;j++)
	//		cout<<int(coord[i][j])<<"   ";
	//	cout << endl;
	//}

	cout << endl<<"low detect------------------";
	fd->detect(low, vtkImageVec_low, low_num, teethType, coord, 21);
	int use_low = checkGpuMem() - use_0;
	cout << "use_low:  " << use_low << endl;

	//for (int i = 0; i < low_num; i++) {
	//	cout << "teeth_type is : " << teethType[i] << endl;
	//	for (int j = 0; j<21; j++)
	//		cout << int(coord[i][j]) << "   ";
	//	cout << endl;
	//}

	delete fd;


	//cout << endl;
}
      