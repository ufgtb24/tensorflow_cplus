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
Feature_detector::Feature_detector(int w, int h, int d):width(w),height(h),depth(d)
{

//:input_tensor(DT_FLOAT, TensorShape({ 1,w,h,d }))
    return;
}




Tensor Feature_detector::exportImage(
						vector<vtkSmartPointer<vtkImageData>> assignImage, 
						unsigned char *cImage_all, 
						unsigned char *cImage,
						int image_size) 

{

	vtkSmartPointer<vtkImageExport> exporter =
		vtkSmartPointer<vtkImageExport>::New();
	int index = 0;
	int batch_size = assignImage.size();
	for (auto iter = assignImage.cbegin(); iter != assignImage.cend(); iter++)
	{
		cout << (*iter) << endl;
	#if VTK_MAJOR_VERSION <= 5
		exporter->SetInput(*iter);
	#else
		exporter->SetInputData(*iter);
	#endif
		exporter->ImageLowerLeftOn();
		exporter->Update();
		exporter->Export(cImage);
		memcpy((unsigned char *)(cImage_all + index*image_size), (unsigned char *)cImage, image_size);
		index++;
	}


	const int64_t tensorDims[5] = { batch_size,width,height,depth,1 };
	int *imNumPt = new int(1);

	TF_Tensor*  tftensor=TF_NewTensor(TF_DataType::TF_UINT8, tensorDims, 5,
		cImage_all, batch_size*width*height*depth,
		NULL, imNumPt);
	return TensorCApi::MakeTensor(tftensor->dtype, tftensor->shape, tftensor->buffer);
}


int Feature_detector::detect(string graph_path,vector<vtkSmartPointer<vtkImageData>> assignImages, float** coord, int feature_size) {
	//len: the size of output,different features are concatatented to one whole,e.g. two features are consist of six element int the coord
	//
	Session* session;
	SessionOptions session_options;
	session_options.config.mutable_gpu_options()->set_allow_growth(true);
	//session_options.config.mutable_gpu_options()->set_per_process_gpu_memory_fraction(0.7);
	//Status status = NewSession(SessionOptions(), &session);
	Status status = NewSession(session_options, &session);

	if (!status.ok()) {
		std::cout << status.ToString() << "\n";
		return 1;
	}

	// Read in the protobuf graph we exported
	// (The path seems to be relative to the cwd. Keep this in mind
	// when using `bazel run` since the cwd isn't where you call
	// `bazel run` but from inside a temp folder.)
	GraphDef graph_def;
	status = ReadBinaryProto(Env::Default(), graph_path, &graph_def);
	if (!status.ok()) {
		std::cout << status.ToString() << "\n";
		return 1;
	}
	
	// Add the graph to the session
	status = session->Create(graph_def);
	if (!status.ok()) {
		std::cout << status.ToString() << "\n";
		return 1;
	}
	int imageNum = assignImages.size();

	unsigned char *cImage_all = new unsigned char[imageNum*width*depth*height];
	unsigned char *cImage = new unsigned char[width*depth*height];
	Tensor input_tensor = exportImage(assignImages, cImage_all, cImage, width*depth*height);

	Tensor is_training(DT_BOOL, TensorShape());
	is_training.scalar<bool>()() = false;



	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
		{ "input_box", input_tensor },
		{ "is_training", is_training },
	};

	// The session will initialize the outputs
	std::vector<tensorflow::Tensor> outputs;

	// Run the session, evaluating our "c" operation from the graph
	status = session->Run(inputs, { "output_node" }, {}, &outputs);
	if (!status.ok()) {
		std::cout << status.ToString() << "\n";
		return 1;
	}
	delete[] cImage_all;
	delete[] cImage;

	// Grab the first output (we only evaluated one graph node: "c")
	// and convert the node to a scalar representation.
	//auto output_c = outputs[0].flat<int>();
	//auto output_c = outputs[0].flat<int>();

	auto output_c = outputs[0].flat<float>();
	//auto array = output_c.data();
	//float* batch_result = static_cast<float*>(array);
	for (int j = 0; j < imageNum; j++) {
		for (int i = 0; i < feature_size; i++) {
			coord[j][i] = output_c(j*feature_size+i);
		}
	}
	// (There are similar methods for vectors and matrices here:
	// https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/public/tensor.h)

	//// Print the results
	//std::cout << outputs[0].DebugString() << "\n"; // Tensor<type: float shape: [] values: 30>

	// Free any resources used by the session
	session->Close();
	delete session;
	return 0;
}





