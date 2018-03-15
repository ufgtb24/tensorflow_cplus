﻿#include <vtkImageData.h>
#include <vtkImageExport.h>
#include <vtkSmartPointer.h>
//#include <vtkVersion.h>
#include <vector>
using namespace std;
int dims[3] = { 2,3,1 };
void makeVtkImages(int num, vector<vtkSmartPointer<vtkImageData>>& vtkImageVec) {
	for (int i = 0; i < num; i++) {
		vtkSmartPointer<vtkImageData> imageData =
			vtkSmartPointer<vtkImageData>::New();

		// Specify the size of the image data

		imageData->SetDimensions(dims[0], dims[1], dims[2]);
#if VTK_MAJOR_VERSION <= 5
		imageData->SetNumberOfScalarComponents(1);
		imageData->SetScalarTypeToUnsignedChar();
#else
		imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
#endif

		unsigned char value = 0;
		for (int row = 0; row < dims[0]; ++row)
		{
			for (int col = 0; col < dims[1]; ++col)
			{
				unsigned char* pixel =
					static_cast<unsigned char*>(imageData->GetScalarPointer(row, col, 0));
				pixel[0] = value;
				value += 10;
			}
		}
		vtkImageVec.push_back(imageData);
	}

}

int main(int, char *[])
{
	int num = 5;
	vector<vtkSmartPointer<vtkImageData>> vtkImageVec;
	makeVtkImages(num, vtkImageVec);

	// Create the c-style image to convert the VTK image to
	unsigned char *cImage = new unsigned char[dims[0] * dims[1] * dims[2]];
	unsigned char *cImage_all = new unsigned char[num*dims[0] * dims[1] * dims[2]];

	vtkSmartPointer<vtkImageExport> exporter =
		vtkSmartPointer<vtkImageExport>::New();


	/////////////////////
//	vtkSmartPointer<vtkImageData> imageData =
//		vtkSmartPointer<vtkImageData>::New();
//
//	// Specify the size of the image data
//
//	imageData->SetDimensions(dims[0], dims[1], dims[2]);
//#if VTK_MAJOR_VERSION <= 5
//	imageData->SetNumberOfScalarComponents(1);
//	imageData->SetScalarTypeToUnsignedChar();
//#else
//	imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
//#endif
//
//	unsigned char value = 0;
//	for (int row = 0; row < dims[0]; ++row)
//	{
//		for (int col = 0; col < dims[1]; ++col)
//		{
//			unsigned char* pixel =
//				static_cast<unsigned char*>(imageData->GetScalarPointer(row, col, 0));
//			pixel[0] = value;
//			value += 10;
//		}
//	}
//

	/////////////////////

	int index = 0;
	for (auto iter = vtkImageVec.cbegin(); iter != vtkImageVec.cend(); iter++)

	{
		exporter->SetInputData(*iter);
		exporter->ImageLowerLeftOn();
		exporter->Update();
		exporter->Export(cImage);

		for (int row = 0; row < dims[0]; ++row)
		{
			for (int col = 0; col < dims[1]; ++col)
			{
				std::cout << static_cast<int>(cImage[col * dims[0] + row]) << " ";
			}
			std::cout << std::endl;
		}
		cout << '\n';



		cout << "begin memcpy\n";
		memcpy(cImage_all + index*dims[0] * dims[1] * dims[2], cImage, dims[0] * dims[1] * dims[2]);
		cout << "finish memcpy\n";
		index++;

	}

	delete[] cImage;
	delete[] cImage_all;


	return EXIT_SUCCESS;
}


//int main(int, char *[])
//{
//	// Create an image data
//	vtkSmartPointer<vtkImageData> imageData =
//		vtkSmartPointer<vtkImageData>::New();
//
//	// Specify the size of the image data
//	int dims[3] = { 2,3,1 };
//
//	imageData->SetDimensions(dims[0], dims[1], dims[2]);
//#if VTK_MAJOR_VERSION <= 5
//	imageData->SetNumberOfScalarComponents(1);
//	imageData->SetScalarTypeToUnsignedChar();
//#else
//	imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
//#endif
//
//	unsigned char value = 0;
//	for (int row = 0; row < dims[0]; ++row)
//	{
//		for (int col = 0; col < dims[1]; ++col)
//		{
//			unsigned char* pixel =
//				static_cast<unsigned char*>(imageData->GetScalarPointer(row, col, 0));
//			pixel[0] = value;
//			value += 10;
//		}
//	}
//
//	// Create the c-style image to convert the VTK image to
//	unsigned char *cImage = new unsigned char[dims[0] * dims[1] * dims[2]];
//
//	vtkSmartPointer<vtkImageExport> exporter =
//		vtkSmartPointer<vtkImageExport>::New();
//#if VTK_MAJOR_VERSION <= 5
//	exporter->SetInput(imageData);
//#else
//	exporter->SetInputData(imageData);
//#endif
//	exporter->ImageLowerLeftOn();
//	exporter->Update();
//	exporter->Export(cImage);
//
//	// Output the raw c-style image
//	for (int row = 0; row < dims[0]; ++row)
//	{
//		for (int col = 0; col < dims[1]; ++col)
//		{
//			std::cout << static_cast<int>(cImage[col * dims[0] + row]) << " ";
//		}
//		std::cout << std::endl;
//	}
//	delete[] cImage;
//
//	return EXIT_SUCCESS;
//}