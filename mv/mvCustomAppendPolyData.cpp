#include "mvCustomAppendPolyData.h"
#include "vtkDoubleArray.h"
#include "vtkInformation.h"

#include <vtkObjectFactory.h>

vtkStandardNewMacro(mvCustomAppendPolyData);

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
