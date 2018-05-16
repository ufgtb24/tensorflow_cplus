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
#include <vector>
using namespace tensorflow;
using namespace std;
#include "feature_detect.h"
#include "vtkSmartPointer.h"
#include "vtkImageData.h"
#include <vtkImageExport.h>
//// This is an example of an exported variable
//FEATURE_DETECT_API int nfeature_detect=0;
//
//// This is an example of an exported function.
//FEATURE_DETECT_API int fnfeature_detect(void)
//{
//    return 42;
//}

// This is the constructor of a class that has been exported.
// see feature_detect.h for the class definition
Feature_detector::Feature_detector(int len, map<Teeth_Group, string> type_path) :
	len(len), image_size(len*len*len)
{
	cImage_all = new unsigned char[8 * image_size];
	cImage = new unsigned char[image_size];
	cout << "construct !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	for (auto iter = type_path.cbegin(); iter != type_path.cend(); iter++)
	{
		cout << "build session !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

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
		status = ReadBinaryProto(Env::Default(), iter->second, &graph_def);
		if (!status.ok()) {
			std::cout << status.ToString() << "\n";
			return;
		}
		status = session->Create(graph_def);
		if (!status.ok()) {
			std::cout << status.ToString() << "\n";
			return;
		}

		
		sessions[iter->first] = session;

	}
	//:input_tensor(DT_FLOAT, TensorShape({ 1,w,h,d }))
	return;
}

Feature_detector::~Feature_detector() {
	delete[] cImage_all;
	delete[] cImage;

	for (auto iter = sessions.cbegin(); iter != sessions.cend(); iter++)
	{
		cout << "close session !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		iter->second->Close();
		delete iter->second;
	}
}



Tensor Feature_detector::exportImage(vector<vtkSmartPointer<vtkImageData>> assignImage)

{
	vtkSmartPointer<vtkImageExport> exporter =
		vtkSmartPointer<vtkImageExport>::New();
	int index = 0;
	int batch_size = assignImage.size();
	for (auto iter = assignImage.cbegin(); iter != assignImage.cend(); iter++)
	{
#if VTK_MAJOR_VERSION <= 5
		exporter->SetInput(*iter);
#else
		exporter->SetInputData(*iter);
#endif
		exporter->ImageLowerLeftOn();
		exporter->Update();
		exporter->Export(cImage);
		memcpy(cImage_all + index*image_size, cImage, image_size);
		index++;
	}

	const int64_t tensorDims[5] = { batch_size,len,len,len,1 };
	int *imNumPt = new int(1);

	TF_Tensor*  tftensor = TF_NewTensor(TF_DataType::TF_UINT8, tensorDims, 5,
		cImage_all, batch_size*image_size,
		NULL, imNumPt);

	return TensorCApi::MakeTensor(tftensor->dtype, tftensor->shape, tftensor->buffer);
}


int Feature_detector::detect(Teeth_Group task_type,
	vector<vtkSmartPointer<vtkImageData>> assignImages,
	float** coord,
	int feature_size) {
	//len: the size of output,different features are concatatented to one whole,e.g. two features are consist of six element int the coord
	//
	int imageNum = assignImages.size();

	Tensor input_tensor = exportImage(assignImages);
	Tensor is_training(DT_BOOL, TensorShape());
	is_training.scalar<bool>()() = false;



	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
		{ "input_box", input_tensor },
		{ "is_training", is_training },
	};

	// The session will initialize the outputs
	std::vector<tensorflow::Tensor> outputs;
	// Run the session, evaluating our "c" operation from the graph
	cout << "run session!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;

	Status status = sessions[task_type]->Run(inputs, { "output_node" }, {}, &outputs);

	if (!status.ok()) {
		std::cout << status.ToString() << "\n";
		return 1;
	}


	auto output_c = outputs[0].flat<float>();

	for (int i = 0; i < imageNum; i++) {
		for (int j = 0; j < feature_size; j++) {
			coord[i][j] = output_c(i*feature_size + j);
		}
	}
	// (There are similar methods for vectors and matrices here:
	// https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/public/tensor.h)

	//// Print the results
	//std::cout << outputs[0].DebugString() << "\n"; // Tensor<type: float shape: [] values: 30>

	// Free any resources used by the session
	return 0;
}





