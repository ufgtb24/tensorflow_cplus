// feature_detect.cpp : Defines the exported functions for the DLL application.
//
#define COMPILER_MSVC
#define NOMINMAX
#include "tensorflow/core/public/session.h"
#include "tensorflow/core/platform/env.h"
#include "tensorflow/core/framework/tensor.h"
#include  "tensorflow/c/c_api.h"
#include "tensorflow/c/c_api_internal.h"

using namespace tensorflow;
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

//int Feature_detector::detect(string graph_path, vtkSmartPointer<vtkImageData> assignImage, int* coord , int len) {
//	//len: the size of output,different features are concatatented to one whole,e.g. two features are consist of six element int the coord
//	//
//		Session* session;
//	Status status = NewSession(SessionOptions(), &session);
//	if (!status.ok()) {
//		std::cout << status.ToString() << "\n";
//		return 1;
//	}
//
//	// Read in the protobuf graph we exported
//	// (The path seems to be relative to the cwd. Keep this in mind
//	// when using `bazel run` since the cwd isn't where you call
//	// `bazel run` but from inside a temp folder.)
//	GraphDef graph_def;
//	status = ReadBinaryProto(Env::Default(), graph_path, &graph_def);
//	if (!status.ok()) {
//		std::cout << status.ToString() << "\n";
//		return 1;
//	}
//
//	// Add the graph to the session
//	status = session->Create(graph_def);
//	if (!status.ok()) {
//		std::cout << status.ToString() << "\n";
//		return 1;
//	}
//	Tensor input_tensor(DT_FLOAT, TensorShape({ 1,height,width,depth }));
//	auto input_tensor_mapped = input_tensor.tensor<float, 4>();
//
//	for (int y = 0; y < height; ++y) {
//		for (int x = 0; x < width; ++x) {
//			for (int c = 0; c < depth; ++c) {
//				input_tensor_mapped(0, y, x, c) = *(unsigned char*)assignImage->GetScalarPointer(y, x, c);
//			}
//		}
//	}
//
//	// Setup inputs and outputs:
//
//	// Our graph doesn't require any inputs, since it specifies default values,
//	// but we'll change an input to demonstrate.
//
//	Tensor keep_prob(DT_FLOAT, TensorShape());
//	keep_prob.scalar<float>()() = 1.0;
//
//	Tensor phase(DT_BOOL, TensorShape());
//	phase.scalar<bool>()() = false;
//
//	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
//		{ "input_box", input_tensor },
//		{ "keep_prob_input", keep_prob },
//		{ "phase_input", phase },
//	};
//
//
//	// The session will initialize the outputs
//	std::vector<tensorflow::Tensor> outputs;
//
//	// Run the session, evaluating our "c" operation from the graph
//	status = session->Run(inputs, { "output_node" }, {}, &outputs);
//	if (!status.ok()) {
//		std::cout << status.ToString() << "\n";
//		return 1;
//	}
//
//	// Grab the first output (we only evaluated one graph node: "c")
//	// and convert the node to a scalar representation.
//	//auto output_c = outputs[0].flat<int>();
//	//auto output_c = outputs[0].flat<int>();
//
//	auto output_c= outputs[0].flat<int>();
//	auto array = output_c.data();
//	int* int_array = static_cast<int*>(array);
//
//	for (int i = 0; i < len; i++) {
//		coord[i] = int_array[i];
//	}
//
//	// (There are similar methods for vectors and matrices here:
//	// https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/public/tensor.h)
//
//	//// Print the results
//	//std::cout << outputs[0].DebugString() << "\n"; // Tensor<type: float shape: [] values: 30>
//
//									 // Free any resources used by the session
//	session->Close();
//	return 0;
//
//
//	return 1;
//}



Tensor Feature_detector::exportImage(vtkSmartPointer<vtkImageData> assignImage, unsigned char *cImage) {

	vtkSmartPointer<vtkImageExport> exporter =
		vtkSmartPointer<vtkImageExport>::New();
	#if VTK_MAJOR_VERSION <= 5
		exporter->SetInput(assignImage);
	#else
		exporter->SetInputData(assignImage);
	#endif
		exporter->ImageLowerLeftOn();
		exporter->Update();
		exporter->Export(cImage);

	const int64_t tensorDims[4] = { 1,width,height,depth };
	int *imNumPt = new int(1);

	TF_Tensor*  tftensor=TF_NewTensor(TF_DataType::TF_UINT8, tensorDims, 4,
		cImage, width*height*depth,
		NULL, imNumPt);

	return TensorCApi::MakeTensor(tftensor->dtype, tftensor->shape, tftensor->buffer);
}


int Feature_detector::detect(string graph_path, vtkSmartPointer<vtkImageData> assignImage, int* coord, int len) {
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
	unsigned char *cImage = new unsigned char[width*depth*height];

	Tensor input_tensor = exportImage(assignImage, cImage);

	Tensor keep_prob(DT_FLOAT, TensorShape());
	keep_prob.scalar<float>()() = 1.0;

	Tensor phase(DT_BOOL, TensorShape());
	phase.scalar<bool>()() = false;



	std::vector<std::pair<string, tensorflow::Tensor>> inputs = {
		{ "input_box", input_tensor },
		{ "keep_prob_input", keep_prob },
		{ "phase_input", phase },
	};


	// The session will initialize the outputs
	std::vector<tensorflow::Tensor> outputs;

	// Run the session, evaluating our "c" operation from the graph
	status = session->Run(inputs, { "output_node" }, {}, &outputs);
	if (!status.ok()) {
		std::cout << status.ToString() << "\n";
		return 1;
	}
	delete[] cImage;
	// Grab the first output (we only evaluated one graph node: "c")
	// and convert the node to a scalar representation.
	//auto output_c = outputs[0].flat<int>();
	//auto output_c = outputs[0].flat<int>();

	auto output_c = outputs[0].flat<int>();
	auto array = output_c.data();
	int* int_array = static_cast<int*>(array);

	for (int i = 0; i < len; i++) {
		coord[i] = int_array[i];
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





