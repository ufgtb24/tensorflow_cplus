// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FEATURE_DETECT_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FEATURE_DETECT_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#pragma once
#ifdef FEATURE_DETECT_EXPORTS
#define FEATURE_DETECT_API __declspec(dllexport)
#else
#define FEATURE_DETECT_API __declspec(dllimport)
#endif


#include <string>
using namespace std;
#include "vtkImageData.h"
#include "vtkSmartPointer.h"

// This class is exported from the feature_detect.dll
class FEATURE_DETECT_API Feature_detector {
public:
	Feature_detector(int w,int h,int d);
	int detect(string graph_path, vtkSmartPointer<vtkImageData> whiteImage, int* coord, int len);

private:
	int width, height, depth;
	Tensor Feature_detector::exportImage(vtkSmartPointer<vtkImageData> assignImage, unsigned char *cImage);

	//Tensor input_tensor;

	// TODO: add your methods here.
};

extern FEATURE_DETECT_API int nfeature_detect;

