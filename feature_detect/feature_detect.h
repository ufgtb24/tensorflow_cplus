#pragma once
#ifdef FEATURE_DETECT_EXPORTS
#define FEATURE_DETECT_API __declspec(dllexport)
#else
#define FEATURE_DETECT_API __declspec(dllimport)
#endif
#pragma once


#include "feature_detect_dev.h"

#include <vector>
#include <string>
#include <map>
using namespace std;
#define MAX_NUM 14
namespace tensorflow {
	class Tensor;
	class Session;
}
//

// This class is exported from the feature_detect.dll
class FEATURE_DETECT_API Feature_detector:public Feature_detector_dev {
	/*
	Class aimming to detect the features for teeth belonging to a single arch.

	*/
public:
	Feature_detector(
		int box_size,
		Teeth_Group group_id[],
		char* group_path[],
		int group_num
	);

	~Feature_detector();
	int detect(
		Teeth_Group group_id,
		vtkSmartPointer<vtkImageData> assignImages[],
		int teeth_num,
		float** coord_output,
		int feature_size
	);

// 	bool CheckDevice();

private:
	int len;
	int image_size;
	int capacity_once, seg_size, seg_num, mod_image_num;
	unsigned char *cImage_all;
	unsigned char *cImage;
	map<Teeth_Group, tensorflow::Session*> sessions;
	vector<tensorflow::Tensor> exportImage(vtkSmartPointer<vtkImageData> assignImage[], int num);



};


