#pragma once

#include "vtkImageData.h"
#include "vtkSmartPointer.h"

class Feature_Detect;
class Feature_detector_cpu;
enum Teeth_Group
{
	up = 0,
	low = 1,
};

class Feature_Detect_Dev
{
private:
	Feature_Detect* Fd;
	Feature_detector_cpu* Fd_c;

public:
	int detect(
		Teeth_Group group_id,
		vtkSmartPointer<vtkImageData> assignImages[],
		int teeth_num,
		float** coord_output,
		int feature_size
	);

	Feature_Detect_Dev();
	~Feature_Detect_Dev();
};

