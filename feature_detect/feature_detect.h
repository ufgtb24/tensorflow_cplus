#pragma once
#ifdef FEATURE_DETECT_EXPORTS
#define FEATURE_DETECT_API __declspec(dllexport)
#else
#define FEATURE_DETECT_API __declspec(dllimport)
#endif

#include <vector>
#include <string>
#include <map>
using namespace std;
#include "vtkImageData.h"
#include "vtkSmartPointer.h"

#define MAX_NUM 14

class Tensor;
class Session;

enum Teeth_Group
{
	up = 0, 
	low = 1, 
};
// This class is exported from the feature_detect.dll
class FEATURE_DETECT_API Feature_detector {
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
		int* teeh_type_output,
		float** coord_output,
		int feature_size
	);



private:
	int len;
	int image_size;
	int seg_size,seg_num,mod_image_num;
	unsigned char *cImage_all;
	unsigned char *cImage;
	map<Teeth_Group, Session*> sessions;
	vector<Tensor> exportImage(vtkSmartPointer<vtkImageData> assignImage[],int num);
};

extern FEATURE_DETECT_API int nfeature_detect;

