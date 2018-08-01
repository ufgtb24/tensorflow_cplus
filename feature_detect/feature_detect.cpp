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
#include "feature_detect.h"
#include "vtkSmartPointer.h"
#include "vtkImageData.h"
#include <vtkImageExport.h>
#include <math.h>
#include <cuda_runtime_api.h>
#include <cuda.h>
#include <iostream>
using namespace std;
int checkGpuMem()

{

	float free_m, total_m, used_m;

	size_t free_t, total_t;

	cudaMemGetInfo(&free_t, &total_t);
	//(unsigned int)
	free_m = (int)(free_t / 1048576.0);

	total_m = (int)(total_t / 1048576.0);

	used_m = total_m - free_m;
	//cout <<"\nfree_m:"<< free_m << "    total_m:" << total_m << "    used_m:" << used_m<<endl;
	return free_m;
}




Feature_detector::Feature_detector(int len, Teeth_Group group_id[],char* group_path[],int group_num) :
	len(len), image_size(len*len*len)
{
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

	int f=checkGpuMem();
	seg_size =ceil((f-284)/128.0);
	cout <<"number of teeth in one segment: "<< seg_size << endl;
	//:input_tensor(DT_FLOAT, TensorShape({ 1,w,h,d }))
	return;
}

Feature_detector::~Feature_detector() {
	delete[] cImage_all;
	delete[] cImage;

	for (auto iter = sessions.cbegin(); iter != sessions.cend(); iter++)
	{
		iter->second->Close();
		delete iter->second;
	}
}



vector<Tensor> Feature_detector::exportImage(vtkSmartPointer<vtkImageData> assignImage[],int num)

{
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
	return seg_tensors;
}


int Feature_detector::detect(Teeth_Group task_type,
	vtkSmartPointer<vtkImageData> assignImages[],
	int imageNum,
	int* teeh_type,
	float** coord,
	int feature_dim) {
	//len: the size of output,different features are concatatented to one whole,
	//e.g. two features are consist of six element int the coord
	if (imageNum < seg_size) { 
		seg_size = imageNum; 
		seg_num = 1;
		mod_image_num = 0;
	}
	else {
		seg_num = imageNum / seg_size;
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
		// Run the session, evaluating our "c" operation from the graph
		Status status = sessions[task_type]->Run(inputs, { "detector/output_node" }, {}, &outputs);
		cout << "finish run session! " << endl;

		if (!status.ok()) {
			std::cout << status.ToString() << "\n";
			return 1;
		}


		auto output_c = outputs[0].flat<float>();
		if (s == seg_num - 1 && mod_image_num != 0)sample_size = mod_image_num;
		for (int i = 0; i < sample_size; i++) {
			teeh_type[s*seg_size+i] = output_c(i*(feature_dim+1));

			for (int j = 0; j < feature_dim; j++) {
				coord[s*seg_size + i][j] = output_c(i*(feature_dim+1) + j+1);
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





