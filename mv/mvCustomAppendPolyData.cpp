#include "mvCustomAppendPolyData.h"
#include "vtkDoubleArray.h"
#include "vtkInformation.h"

mvCustomAppendPolyData::mvCustomAppendPolyData()
{
}

mvCustomAppendPolyData::~mvCustomAppendPolyData()
{
}

int mvCustomAppendPolyData::FillInputPortInformation(int port, vtkInformation *info)
{
    info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
    return vtkAppendPolyData::FillInputPortInformation(port, info);
}
