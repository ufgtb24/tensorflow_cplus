# include "feature_detect.h"
# include"test.h"
# include <map>
# include <iostream>
# include <windows.h>
using namespace std;
#define IMAGE_NUM_UP 14
#define IMAGE_NUM_LOW 14

int main1(int, char *[])
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
	//vtkImageVec_up[1] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel3");
	//vtkImageVec_up[2] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel4");
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

	//vtkImageVec_low[0] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel25");
	//vtkImageVec_low[1] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel26");
	//vtkImageVec_low[2] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel27");
	//vtkImageVec_low[3] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel28");
	//vtkImageVec_low[4] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel29");
	//vtkImageVec_low[5] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel30");
	//vtkImageVec_low[6] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel31");
	//vtkImageVec_low[7] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel25");
	//vtkImageVec_low[8] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel26");
	//vtkImageVec_low[9] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel27");
	//vtkImageVec_low[10] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel28");
	//vtkImageVec_low[11] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel29");
	//vtkImageVec_low[12] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel30");
	//vtkImageVec_low[13] = loadmhd("F://ProjectData//tmp//saved_mhd//_$GR117Final//toothLabel31");
	int low_num = 7;


	char* model_path[] = 
	{
	  "E://TensorFlowCplusplus//feature_detect//x64//Release//up_graph.pb",
	  "E://TensorFlowCplusplus//feature_detect//x64//Release//low_graph.pb"
	};

	Teeth_Group model_id[] = { up,low};

	Feature_detector* fd = new Feature_detector(128, model_id,model_path,2);
	float** coord = new float*[MAX_NUM];
	for (int i = 0; i < MAX_NUM; i++) {
		coord[i] = new float[21];
	}
	//int use_0 = checkGpuMem();
	//cout << endl << "up detect------------------";
	time_t start, stop;
	start = timeGetTime();
	fd->detect(up, vtkImageVec_up, up_num, coord, 27);
	cout << " single arch finish";
	fd->detect(low, vtkImageVec_up, up_num, coord, 27);
	stop = timeGetTime();
	printf("Use Time:%ld\n", (stop - start)/1000);

	//int use_up = checkGpuMem() - use_0;
	//cout << "use_up:  "<< use_up << endl;


	for (int i = 0; i < up_num; i++) {
		for(int j=0;j<27;j++)
			cout<<int(coord[i][j])<<"   ";
		cout << endl;
	}

}
      