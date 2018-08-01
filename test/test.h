#pragma once
#include <vtkImageData.h>
#include <vtkImageExport.h>
#include <vtkSmartPointer.h>
#include <vector>
using namespace std;
extern int dims[3];
vtkSmartPointer<vtkImageData> loadmhd(std::string mhd_path);
int checkGpuMem();
