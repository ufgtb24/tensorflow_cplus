// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FEATURE_DETECT_DEV_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// FEATURE_DETECT_DEV_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef FEATURE_DETECT_DEV_EXPORTS
#define FEATURE_DETECT_DEV_API __declspec(dllexport)
#else
#define FEATURE_DETECT_DEV_API __declspec(dllimport)
#endif

#include "vtkImageData.h"
#include "vtkSmartPointer.h"

class Feature_Detect;
class Feature_detector_cpu;
enum Teeth_Group
{
	up = 0,
	low = 1,
};

// This class is exported from the feature_detect_dev.dll
class FEATURE_DETECT_DEV_API Feature_Detect_Dev {
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

