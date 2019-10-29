// https://www.vtk.org/Wiki/VTK/Examples/Cxx/IO/MetaImageWriter

#include <vtkAutoInit.h>   
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

// headers needed for this example
#include <vtkImageData.h>
#include <vtkImageMapper3D.h>
#include <vtkImageCast.h>
#include <vtkMetaImageWriter.h>
#include <vtkMetaImageReader.h>
#include <vtkImageMandelbrotSource.h>
#include <vtkImageActor.h>

vtkSmartPointer<vtkImageData> loadmhd(std::string mhdPath)
{
	// adapt path !
	//std::string filePathRaw = mhd_path+".raw";
	// Read and display file for verification that it was written correctly
	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName(mhdPath.c_str());
	reader->Update();

	vtkSmartPointer<vtkImageData>  image = reader->GetOutput();

	return image;
}