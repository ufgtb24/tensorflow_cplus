// feature_detect.cpp : Defines the exported functions for the DLL application.
//
#define COMPILER_MSVC
#define NOMINMAX
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/framework/tensor.h"
#include  "tensorflow/c/c_api.h"
#include "tensorflow/c/c_api_internal.h"
#include "tensorflow/cc/ops/array_ops.h"
using namespace tensorflow;
#include "feature_detect_cpu.h"
#include "vtkSmartPointer.h"
#include "vtkImageData.h"
#include <vtkImageExport.h>
#include <math.h>
#include <iostream>
using namespace std;


Feature_detector_cpu::Feature_detector_cpu(int len, Teeth_Group group_id[],char* group_path[],int group_num) :
	len(len), image_size(len*len*len)
{
	cout << "start Feature_detector_cpu initialization!!\n";

	cImage_all = new unsigned char[MAX_NUM * image_size];
	cImage = new unsigned char[image_size];
	for (int iter=0; iter<group_num; iter++)
	{

		Session* session;
		SessionOptions session_options;
		session_options.config.mutable_gpu_options()->set_allow_growth(true);
		//session_options.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(0.7);
		Status status = NewSession(session_options, &session);
		if (!status.ok()) {
			std::cout << status.ToString() << "\n";
			return;
		}

		GraphDef graph_def;
		status = ReadBinaryProto(Env::Default(), group_path[iter], &graph_def);
		if (!status.ok()) {

			std::cout << status.ToString() << "\n";
			return;
		}

		status = session->Create(graph_def);
		if (!status.ok()) {
			std::cout << status.ToString() << "\n";
			return;
		}
		
		sessions[group_id[iter]] = session;

	}
	capacity_once = 4;
	cout << "finish Feature_detector_cpu initialization!!\n";
	return;
}

Feature_detector_cpu::~Feature_detector_cpu() {
	delete[] cImage_all;
	delete[] cImage;

	for (auto iter = sessions.cbegin(); iter != sessions.cend(); iter++)
	{
		iter->second->Close();
		delete iter->second;
	}
}



vector<Tensor> Feature_detector_cpu::exportImage(vtkSmartPointer<vtkImageData> assignImage[],int num)

{
	cout << "start exportImage!!\n";

	vtkSmartPointer<vtkImageExport> exporter =
		vtkSmartPointer<vtkImageExport>::New();
	for (int iter = 0; iter <num; iter++)
	{
#if VTK_MAJOR_VERSION <= 5
		exporter->SetInput(*iter);
#else
		exporter->SetInputData(assignImage[iter]);
#endif
		exporter->ImageLowerLeftOn();
		exporter->Update();
		exporter->Export(cImage);
		memcpy(cImage_all + iter*image_size, cImage, image_size);
	}
	int sample_size = seg_size;
	int *imNumPt = new int(1);
	vector<Tensor> seg_tensors;

	for (int s = 0; s < seg_num; s++) {
		if (s == seg_num-1&& mod_image_num != 0) {
			sample_size = mod_image_num;
		}
		const int64_t tensorDims[5] = { sample_size,len,len,len,1 };


		TF_Tensor*  tftensor = TF_NewTensor(TF_DataType::TF_UINT8, tensorDims, 5,
		cImage_all+s*seg_size*image_size, sample_size*image_size,
		NULL, imNumPt);

		seg_tensors.push_back(TensorCApi::MakeTensor(tftensor->dtype, tftensor->shape, tftensor->buffer));
	}
	cout << "finish exportImage!!\n";

	return seg_tensors;
}


int Feature_detector_cpu::detect(Teeth_Group task_type,
	vtkSmartPointer<vtkImageData> assignImages[],
	int imageNum,
	float** coord,
	int feature_dim) {
	cout << "start detection\n";
	if (imageNum < capacity_once) {
		seg_size = imageNum; 
		seg_num = 1;
		mod_image_num = 0;
	}
	else {
		seg_num = imageNum / capacity_once;
		seg_size = capacity_once;
		mod_image_num = imageNum%seg_size;
		if (mod_image_num != 0)seg_num++;
	}
	
	vector<Tensor> seg_tensors=exportImage(assignImages, imageNum);
	Tensor is_training(DT_BOOL, TensorShape());
	is_training.scalar<bool>()() = false;
	int sample_size = seg_size;

	for (int s = 0; s < seg_num; s++) {
		Tensor input_tensor = seg_tensors[s];

		std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
			{ "detector/input_box", input_tensor },
			{ "detector/is_training", is_training },
		};

		// The session will initialize the outputs
		std::vector<tensorflow::Tensor> outputs;
		cout << "before run session\n";

		//cout << "before detect the free mem is: " << checkGpuMem() << endl;
		// Run the session, evaluating our "c" operation from the graph
		Status status = sessions[task_type]->Run(inputs, { "detector/output_node" }, {}, &outputs);
		cout << "finish run session! " << endl;
		//cout << "after detect the free mem is: " << checkGpuMem() << endl;

		if (!status.ok()) {
			std::cout << status.ToString() << "\n";
			return 1;
		}


		auto output_c = outputs[0].flat<float>();
		if (s == seg_num - 1 && mod_image_num != 0)sample_size = mod_image_num;
		for (int i = 0; i < sample_size; i++) {
			for (int j = 0; j < feature_dim; j++) {
				coord[s*seg_size + i][j] = output_c(i*feature_dim + j);
			}
		}
	}
	// (There are similar methods for vectors and matrices here:
	// https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/public/tensor.h)

	//// Print the results
	//std::cout << outputs[0].DebugString() << "\n"; // Tensor<type: float shape: [] values: 30>

	// Free any resources used by the session
	return 0;
}


extern "C" __declspec(dllexport) Feature_detector* getObj(
	int box_size,
	Teeth_Group group_id[],
	char* group_path[],
	int group_num
)
{
	return new Feature_detector_cpu(box_size,
		group_id,
		group_path,
		group_num
	);
}


