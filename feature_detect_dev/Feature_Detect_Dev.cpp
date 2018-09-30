// feature_detect_dev.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Feature_Detect_Dev.h"
#include <cuda_runtime.h>



// This is the constructor of a class that has been exported.
// see feature_detect_dev.h for the class definition
Feature_Detect_Dev::Feature_Detect_Dev()
{
	if (check_device()) {


	}
    return;
}

bool Feature_Detect_Dev::check_device() {
	int count;
	cudaGetDeviceCount(&count);
	cout << count << endl;
	if (count > 0) {
		return true;
	}
	else return false;
}

int Feature_Detect_Dev::detect(
	Teeth_Group group_id,
	vtkSmartPointer<vtkImageData> assignImages[],
	int teeth_num,
	float** coord_output,
	int feature_size
) {

}
