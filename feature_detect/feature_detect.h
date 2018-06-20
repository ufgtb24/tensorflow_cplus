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
//
//class Tensor;
//class Session;

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
		int len,
		Teeth_Group type_id[],
		char* type_path[], 
		int type_num
	);

	~Feature_detector();
	int detect(
		Teeth_Group task_type,
		vector<vtkSmartPointer<vtkImageData>> assignImages,
		int* teeh_type,
		float** coord,
		int feature_size
	);



private:
	int len;
	int image_size;

	unsigned char *cImage_all;
	unsigned char *cImage;
	map<Teeth_Group, Session*> sessions;
	Tensor exportImage(vector<vtkSmartPointer<vtkImageData>> assignImage);
};

extern FEATURE_DETECT_API int nfeature_detect;

