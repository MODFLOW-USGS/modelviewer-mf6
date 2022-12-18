#include "mvGridShell.h"
#include "vtkGeometryFilter.h"
#include "vtkProperty.h"

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvGridShell::mvGridShell()
{
    m_GeometryFilter = vtkGeometryFilter::New();
    //	SetMapperInputData(m_GeometryFilter->GetOutput());
    SetMapperInputConnection(m_GeometryFilter->GetOutputPort());
    m_Mapper->ScalarVisibilityOff();
    m_Actor->GetProperty()->SetColor(1.0, 0.8, 0.6);
    m_Actor->GetProperty()->SetOpacity(.2);
    m_LODActor->GetProperty()->SetColor(1.0, 0.8, 0.6);
    m_LODActor->GetProperty()->SetOpacity(.2);
}

mvGridShell::~mvGridShell()
{
    m_GeometryFilter->Delete();
}

void mvGridShell::SetInputData(vtkDataSet *dataSet)
{
    m_GeometryFilter->SetInputData(dataSet);
}

void mvGridShell::SetInputConnection(vtkAlgorithmOutput *input)
{
    m_GeometryFilter->SetInputConnection(input);
}

vtkDataSet *mvGridShell::GetInput()
{
    return (vtkDataSet *)m_GeometryFilter->GetInput();
}

vtkAlgorithmOutput *mvGridShell::GetOutputPort()
{
    return m_GeometryFilter->GetOutputPort();
}