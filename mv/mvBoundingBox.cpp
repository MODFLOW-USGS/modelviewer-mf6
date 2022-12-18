#include "mvBoundingBox.h"
#include "vtkOutlineSource.h"
#include "vtkProperty.h"

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvBoundingBox::mvBoundingBox()
{
    m_Outline = vtkOutlineSource::New();
    //	SetMapperInputData(m_Outline->GetOutput());
    //		m_Mapper = vtkPolyDataMapper::New();
    //		m_Actor->SetMapper(m_Mapper);
    //		m_LODActor->SetMapper(m_Mapper);
    //        m_Mapper->SetInputConnection(m_Outline->GetOutputPort());
    SetMapperInputConnection(m_Outline->GetOutputPort());
    m_Mapper->ScalarVisibilityOff();
    m_Actor->GetProperty()->SetColor(0, 0, 0);
    m_Actor->GetProperty()->SetAmbient(1.0);
    m_Actor->GetProperty()->SetDiffuse(0.0);
}

mvBoundingBox::~mvBoundingBox()
{
    m_Outline->Delete();
}

void mvBoundingBox::SetBounds(const double *bounds)
{
    double b[6];
    memcpy(b, bounds, 6 * sizeof(double));
    m_Outline->SetBounds(b);
}

const double *mvBoundingBox::GetBounds() const
{
    return m_Outline->GetBounds();
}

void mvBoundingBox::GetBounds(double *bounds) const
{
    const double *b = m_Outline->GetBounds();
    memcpy(bounds, b, 6 * sizeof(double));
}
