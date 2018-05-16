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
#include <map>
using namespace std;
#include "vtkImageData.h"
#include "vtkSmartPointer.h"

//8 types of detector model
enum Teeth_Group
{
	up_back = 0, //2 3
	up_middle = 1, //4 5
	up_canine = 2, //6
	up_front = 3, //7 8

	low_back = 4, 
	low_middle = 5,
	low_canine = 6,
	low_front = 7,
};
//class Tensor;
//class Session;
// This class is exported from the feature_detect.dll
class FEATURE_DETECT_API Feature_detector {
	/*
	Class aimming to detect the features for teeth belonging to a single arch.

	*/
public:
	Feature_detector(
		int len,
		map<Teeth_Group, string> models
	);
	/*
	Constructor of Feature_detector, through which we import a collection of 'pb'files to
	define the neural network.


	According to the similarity of shape, all teeth is divided into 8 Teeth Groups as follow:

	up_back=['tooth2','tooth3']
	up_middle=['tooth4','tooth5']
	up_canine=['tooth6']
	up_front=['tooth7','tooth8']
	low_back=['tooth30','tooth31']
	low_middle=['tooth28','tooth29']
	low_canine=['tooth27']
	low_front=['tooth25','tooth26']



	Different Teeth Group has different features to be detect, for example, teeth in 'up_back'
	Group need to detect 'groove' and 'facc',while teeth in 'up_canine'Group only need 'facc'.

	We use neural networks to detect feature, and for each of the Teeth Groups, we use a 
	specialized neural network, which can be constructed by loading a '.pb'file. As there are 
	totally 8 Teeth Groups, we need 8 '.pb'files.

	--------------------------------------------------
	--------------------------------------------------

	Args:
	len: Side length of vtkImageData cubic(default to be 128)
	models: Contain all pairs of Teeth Group identifier and corresponding 'pb'file path.
	*/

	~Feature_detector();
	/*
	Deconstructor of Feature_detector
	*/
	int detect(
		Teeth_Group task_type,
		vector<vtkSmartPointer<vtkImageData>> assignImages,
		float** coord,
		int feature_size
	);
	/*
	Detect the feature local coordinate for a set of teeth belonging to a particular Teeth Group.
	-----------------------------------------
	-----------------------------------------
	Args:
	task_type: An enum indicating which Teeth Group the teeth vtkImageData belong to. It helps the class 
			decide which 'pb' file should be load to build a neural network to detect the feature of 
			a particular Teeth Group.

	assignImages: Vector including several teeth vtkImageData belonging to the Teeth Group specified above 
				by task_type.

	coord:  The detection result obtained, which need to be a 2-D array represented by a pointer of pointer. 
			1st dimmension index the vtkImageData in the vector above. 
			2nd dimmension indicates the feature vector of this vtkImageData.
			As for the 'up_back' example above, coord can be a 2 dimmension array of shape [2,21], where '2'
			indicates that there are 2 teeth in the assignImages vetor, and '21' indicates each tooth has a
			feature size of 21 floats. 

	feature_size: The size of feature for the 
	'groove' feature consists of 2 points represented by 6 float numbers
			'facc' feature consists of 5 points represented by 15 float numbers. 
			Since the teeth in 'up_back' Group include both two features above, the detection for 'up_back' teeth
			outputs 21 float numbers.
			Teeth 'Up_canine' Group only has 'facc' feature, so the detection for 'up_canine'Teeth Group outputs 15 float
			numbers.
		 The size of 2nd dimmension of coord argiment above. 21 for 'up_back' Teeth Group and 15 for 
			'Up_canine' Teeth Group

	-------------------------------------------
	-------------------------------------------
	Example:
	detect 'groove'and'facc' of tooth2 and tooth3(both belong to Teeth Group'up_back')

	int teeth_num = 2;  

	int feature_point_num=7;// facc 5  groove 2

	int feature_size = 3 * feature_point_num; 

	float** local_pos = new float*[teeth_num]; 

	for (int i = 0; i < teeth_num; i++){

		local_pos[i] = new float[feature_size];

		}

	fearure_detector->detect(up_back, image_vec, local_pos, feature_size);

	//up_back is the Teeth Group identifier enum
	//image_vec is a vector containing vtkImageData of tooth2 and tooth3

	float* coord_world = new float[feature_size]; // world coordinate

	for (int i = 0; i < batch_size; i++)
	{
		for (int j = 0; j < feature_num; j++){

			exchange(local_pos[i], 3 * j, 3 * j + 2);
			// x and z of each feature point should be switched
		}

		for (int j = 0; j < feature_size; j++){
			coord_world[j] = local_pos[i][j] * 12.0 / 128.0 + origin_vec[i][j % 3];
		}

		draw(coord_world)
	}



	*/


private:
	int len;
	int image_size;

	unsigned char *cImage_all;
	unsigned char *cImage;
	map<Teeth_Group, Session*> sessions;
	Tensor exportImage(vector<vtkSmartPointer<vtkImageData>> assignImage);
};

extern FEATURE_DETECT_API int nfeature_detect;

