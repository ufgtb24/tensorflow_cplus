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
Feature_detector::Feature_detector(int len, Teeth_Group group_id[],char* group_path[],int group_num) :
	len(len), image_size(len*len*len)
{
	cImage_all = new unsigned char[8 * image_size];
	cImage = new unsigned char[image_size];
	for (int iter=0; iter<group_num; iter++)
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

		cout << "start read pb\n";
		GraphDef graph_def;
		status = ReadBinaryProto(Env::Default(), group_path[iter], &graph_def);
		cout << status;
		if (!status.ok()) {
			cout << "state judge\n";

			std::cout << status.ToString() << "\n";
			return;
		}
		cout << "end read pb\n";

		status = session->Create(graph_def);
		if (!status.ok()) {
			std::cout << status.ToString() << "\n";
			return;
		}

		
		sessions[group_id[iter]] = session;

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



Tensor Feature_detector::exportImage(vtkSmartPointer<vtkImageData> assignImage[],int num)

{
	vtkSmartPointer<vtkImageExport> exporter =
		vtkSmartPointer<vtkImageExport>::New();
	int index = 0;
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
		memcpy(cImage_all + index*image_size, cImage, image_size);
		index++;
	}

	const int64_t tensorDims[5] = { num,len,len,len,1 };
	int *imNumPt = new int(1);

	TF_Tensor*  tftensor = TF_NewTensor(TF_DataType::TF_UINT8, tensorDims, 5,
		cImage_all, num*image_size,
		NULL, imNumPt);

	return TensorCApi::MakeTensor(tftensor->dtype, tftensor->shape, tftensor->buffer);
}


int Feature_detector::detect(Teeth_Group task_type,
	vtkSmartPointer<vtkImageData> assignImages[],
	int imageNum,
	int* teeh_type,
	float** coord,
	int feature_dim) {
	//len: the size of output,different features are concatatented to one whole,
	//e.g. two features are consist of six element int the coord
	cout << "start detect1\n";
	Tensor input_tensor = exportImage(assignImages, imageNum);
	cout << "start detect2\n";

	Tensor is_training(DT_BOOL, TensorShape());
	cout << "start detect3\n";

	is_training.scalar<bool>()() = false;
	cout << "start detect4\n";



	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
		{ "input_box", input_tensor },
		{ "is_training", is_training },
	};
	cout << "start detect5\n";

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
		teeh_type[i] = output_c(i*(feature_dim+1));

		for (int j = 0; j < feature_dim; j++) {
			coord[i][j] = output_c(i*(feature_dim+1) + j+1);
		}
	}
	// (There are similar methods for vectors and matrices here:
	// https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/public/tensor.h)

	//// Print the results
	//std::cout << outputs[0].DebugString() << "\n"; // Tensor<type: float shape: [] values: 30>

	// Free any resources used by the session
	return 0;
}





