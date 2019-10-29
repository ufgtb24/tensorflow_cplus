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

vtkSmartPointer<vtkImageData> loadmhd(std::string mhd_path)
{

	// Read and display file for verification that it was written correctly
	vtkSmartPointer<vtkMetaImageReader> reader =
		vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName(mhd_path.c_str());
	reader->Update();

	vtkSmartPointer<vtkImageData>  image = reader->GetOutput();

	//vtkSmartPointer<vtkImageActor> actor =
	//	vtkSmartPointer<vtkImageActor>::New();

	//actor->GetMapper()->SetInputConnection(reader->GetOutputPort());

	//vtkSmartPointer<vtkRenderer> renderer =
	//	vtkSmartPointer<vtkRenderer>::New();
	//vtkSmartPointer<vtkRenderWindow> renderWindow =
	//	vtkSmartPointer<vtkRenderWindow>::New();
	//renderWindow->AddRenderer(renderer);
	//vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
	//	vtkSmartPointer<vtkRenderWindowInteractor>::New();
	//renderWindowInteractor->SetRenderWindow(renderWindow);

	//renderer->AddActor(actor);
	//renderer->SetBackground(.2, .3, .4);

	//renderWindow->Render();
	//renderWindowInteractor->Start();




	return image;
}