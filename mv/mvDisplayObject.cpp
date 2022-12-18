#include "mvDisplayObject.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkAlgorithmOutput.h"
#include "vtkAlgorithm.h"
#include "vtkDataObject.h"

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvDisplayObject::mvDisplayObject()
{
    m_Mapper = 0;
    m_Actor  = vtkActor::New();
    m_Actor->VisibilityOff();
    m_LODActor = vtkLODActor::New();
    m_LODActor->VisibilityOff();
    m_ActiveActor = m_Actor;
}

mvDisplayObject::~mvDisplayObject()
{
    if (m_Mapper)
    {
        m_Mapper->Delete();
    }
    m_Actor->Delete();
    m_LODActor->Delete();
}

void mvDisplayObject::VisibilityOn()
{
    m_ActiveActor->VisibilityOn();
}

void mvDisplayObject::VisibilityOff()
{
    m_ActiveActor->VisibilityOff();
}

void mvDisplayObject::SetVisibility(int v)
{
    m_ActiveActor->SetVisibility(v);
}

int mvDisplayObject::GetVisibility()
{
    return m_ActiveActor->GetVisibility();
}

void mvDisplayObject::SetMapperInputData(vtkPolyData *in)
{
    if (m_Mapper == 0)
    {
        m_Mapper = vtkPolyDataMapper::New();
        m_Actor->SetMapper(m_Mapper);
        m_LODActor->SetMapper(m_Mapper);
    }
    ((vtkPolyDataMapper *)m_Mapper)->SetInputData(in);
}

void mvDisplayObject::SetMapperInputData(vtkDataSet *in)
{
    if (m_Mapper == 0)
    {
        m_Mapper = vtkDataSetMapper::New();
        m_Actor->SetMapper(m_Mapper);
        m_LODActor->SetMapper(m_Mapper);
    }
    ((vtkDataSetMapper *)m_Mapper)->SetInputData(in);
}

void mvDisplayObject::SetMapperInputConnection(vtkAlgorithmOutput *in)
{
    if (in->GetProducer()->GetOutputDataObject(0)->GetDataObjectType() == VTK_POLY_DATA)
    {
        if (m_Mapper == 0)
        {
            m_Mapper = vtkPolyDataMapper::New();
            m_Actor->SetMapper(m_Mapper);
            m_LODActor->SetMapper(m_Mapper);
        }
        ((vtkPolyDataMapper *)m_Mapper)->SetInputConnection(in);
    }
    else
    {
        if (m_Mapper == 0)
        {
            m_Mapper = vtkDataSetMapper::New();
            m_Actor->SetMapper(m_Mapper);
            m_LODActor->SetMapper(m_Mapper);
        }
        ((vtkDataSetMapper *)m_Mapper)->SetInputConnection(in);
    }
}

void mvDisplayObject::SetColor(double red, double green, double blue)
{
    m_Actor->GetProperty()->SetColor(red, green, blue);
    m_LODActor->GetProperty()->SetColor(red, green, blue);
}

const double *mvDisplayObject::GetColor()
{
    return m_ActiveActor->GetProperty()->GetColor();
}

void mvDisplayObject::SetOpacity(double p)
{
    m_Actor->GetProperty()->SetOpacity(p);
    m_LODActor->GetProperty()->SetOpacity(p);
}

double mvDisplayObject::GetOpacity()
{
    return m_ActiveActor->GetProperty()->GetOpacity();
}

void mvDisplayObject::SetDiffuse(double d)
{
    m_Actor->GetProperty()->SetDiffuse(d);
    m_LODActor->GetProperty()->SetDiffuse(d);
}

void mvDisplayObject::SetAmbient(double a)
{
    m_Actor->GetProperty()->SetAmbient(a);
    m_LODActor->GetProperty()->SetAmbient(a);
}

void mvDisplayObject::SetSpecular(double s)
{
    m_Actor->GetProperty()->SetSpecular(s);
    m_LODActor->GetProperty()->SetSpecular(s);
}

void mvDisplayObject::SetSpecularPower(double sp)
{
    m_Actor->GetProperty()->SetSpecularPower(sp);
    m_LODActor->GetProperty()->SetSpecularPower(sp);
}

double mvDisplayObject::GetDiffuse()
{
    return m_ActiveActor->GetProperty()->GetDiffuse();
}

double mvDisplayObject::GetAmbient()
{
    return m_ActiveActor->GetProperty()->GetAmbient();
}

double mvDisplayObject::GetSpecular()
{
    return m_ActiveActor->GetProperty()->GetSpecular();
}

double mvDisplayObject::GetSpecularPower()
{
    return m_ActiveActor->GetProperty()->GetSpecularPower();
}

void mvDisplayObject::SetScale(double xScale, double yScale, double zScale)
{
    m_Actor->SetScale(xScale, yScale, zScale);
    m_LODActor->SetScale(xScale, yScale, zScale);
}

#if ((VTK_MAJOR_VERSION == 8) && (VTK_MINOR_VERSION <= 1) || (VTK_MAJOR_VERSION < 8))  //  https://vtk.org/Wiki/VTK/API_Changes_8_0_1_to_8_1_0
void mvDisplayObject::SetImmediateModeRendering(int b)
{
    m_Mapper->SetImmediateModeRendering(b);  // deprecated vtk 8.1
}
#endif

void mvDisplayObject::UseLODActor(int b)
{
    if (b)
    {
        if (m_ActiveActor == m_Actor)
        {
            m_LODActor->SetVisibility(m_Actor->GetVisibility());
        }
        m_ActiveActor = m_LODActor;
    }
    else
    {
        if (m_ActiveActor == m_LODActor)
        {
            m_Actor->SetVisibility(m_LODActor->GetVisibility());
        }
        m_ActiveActor = m_Actor;
    }
}

void mvDisplayObject::SetNumberOfCloudPoints(int n)
{
    m_LODActor->SetNumberOfCloudPoints(n);
}

void mvDisplayObject::SetScalarRange(double min, double max)
{
    if (m_Mapper != 0)
    {
        m_Mapper->SetScalarRange(min, max);
    }
}
