#pragma once
// #ifdef FEATURE_DETECT_EXPORTS
// #define FEATURE_DETECT_API __declspec(dllexport)
// #else
// #define FEATURE_DETECT_API __declspec(dllimport)
// #endif
#include "vtkImageData.h"
#include "vtkSmartPointer.h"

enum Teeth_Group
{
	up = 0,
	low = 1,
};

class  Feature_detector{

public:
	virtual int  detect(
		Teeth_Group group_id,
		vtkSmartPointer<vtkImageData> assignImages[],
		int teeth_num,
		float** coord_output,
		int feature_size
	) = 0;

};


