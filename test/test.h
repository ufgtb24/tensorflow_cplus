#pragma once
#include <vtkImageData.h>
#include <vtkImageExport.h>
#include <vtkSmartPointer.h>
#include <vector>
using namespace std;
extern int dims[3];
void makeVtkImages(int num, vector<vtkSmartPointer<vtkImageData>>& vtkImageVec);