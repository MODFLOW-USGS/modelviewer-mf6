#include "mvAxes.h"
#include "vtkAxes.h"
#include "vtkTubeFilter.h"
#include "vtkLookupTable.h"
#include "vtkProperty.h"

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvAxes::mvAxes()
{
    m_DefaultSize           = 1;
    m_DefaultTubeDiameter   = 1;
    m_DefaultPositions[0]   = 0;
    m_DefaultPositions[1]   = 1;
    m_DefaultPositions[2]   = 0;
    m_DefaultPositions[3]   = 1;
    m_DefaultPositions[4]   = 0;
    m_DefaultPositions[5]   = 1;
    m_NormalizedPosition[0] = 0;
    m_NormalizedPosition[1] = 0;
    m_NormalizedPosition[2] = 0;
    m_Axes                  = vtkAxes::New();
    m_Tube                  = vtkTubeFilter::New();
    m_Tube->SetNumberOfSides(10);
    m_Tube->SetInputConnection(m_Axes->GetOutputPort());
    m_LookupTable = vtkLookupTable::New();
    m_LookupTable->SetNumberOfColors(3);
    m_LookupTable->SetTableValue(0, 1, 0, 0, 1);
    m_LookupTable->SetTableValue(1, 0, 1, 0, 1);
    m_LookupTable->SetTableValue(2, 0, 0, 1, 1);
    m_LookupTable->Build();
    //	SetMapperInputData(m_Tube->GetOutput());
    SetMapperInputConnection(m_Tube->GetOutputPort());
    m_Mapper->SetLookupTable(m_LookupTable);
    m_Mapper->SetScalarRange(0, 0.5);
}

mvAxes::~mvAxes()
{
    m_Axes->Delete();
    m_Tube->Delete();
    m_LookupTable->Delete();
}

void mvAxes::SetDefaultPositions(const double *p)
{
    m_DefaultPositions[0] = p[0];
    m_DefaultPositions[1] = p[1];
    m_DefaultPositions[2] = p[2];
    m_DefaultPositions[3] = p[3];
    m_DefaultPositions[4] = p[4];
    m_DefaultPositions[5] = p[5];

    double xo             = m_NormalizedPosition[0] * (p[1] - p[0]) + p[0];
    double yo             = m_NormalizedPosition[1] * (p[3] - p[2]) + p[2];
    double zo             = m_NormalizedPosition[2] * (p[5] - p[4]) + p[4];

    m_Axes->SetOrigin(xo, yo, zo);
}

void mvAxes::SetDefaultSize(double s)
{
    m_Axes->SetScaleFactor(GetNormalizedSize() * s);
    m_DefaultSize = s;
}

void mvAxes::SetDefaultTubeDiameter(double d)
{
    m_Tube->SetRadius(GetNormalizedTubeDiameter() * d / 2);
    m_DefaultTubeDiameter = d;
}

void mvAxes::SetNormalizedPosition(double xn, double yn, double zn)
{
    m_NormalizedPosition[0] = xn;
    m_NormalizedPosition[1] = yn;
    m_NormalizedPosition[2] = zn;

    double xo               = xn * (m_DefaultPositions[1] - m_DefaultPositions[0]) + m_DefaultPositions[0];
    double yo               = yn * (m_DefaultPositions[3] - m_DefaultPositions[2]) + m_DefaultPositions[2];
    double zo               = zn * (m_DefaultPositions[5] - m_DefaultPositions[4]) + m_DefaultPositions[4];

    m_Axes->SetOrigin(xo, yo, zo);
}

void mvAxes::SetNormalizedSize(double sn)
{
    m_Axes->SetScaleFactor(sn * m_DefaultSize);
}

void mvAxes::SetNormalizedTubeDiameter(double dn)
{
    m_Tube->SetRadius(dn * m_DefaultTubeDiameter / 2);
}

void mvAxes::SetRepresentationToTube()
{
    //	SetMapperInputData(m_Tube->GetOutput());
    SetMapperInputConnection(m_Tube->GetOutputPort());
    m_Actor->GetProperty()->SetAmbient(0.0);
    m_Actor->GetProperty()->SetDiffuse(1.0);
}

void mvAxes::SetRepresentationToLine()
{
    //	SetMapperInputData(m_Axes->GetOutput());
    SetMapperInputConnection(m_Axes->GetOutputPort());
    m_Actor->GetProperty()->SetAmbient(1.0);
    m_Actor->GetProperty()->SetDiffuse(0.0);
}

double mvAxes::GetNormalizedSize() const
{
    return (m_Axes->GetScaleFactor() / m_DefaultSize);
}

double mvAxes::GetNormalizedTubeDiameter() const
{
    return (2 * m_Tube->GetRadius() / m_DefaultTubeDiameter);
}

const double *mvAxes::GetNormalizedPosition() const
{
    return m_NormalizedPosition;
}

int mvAxes::GetRepresentation() const
{
    if (m_Mapper->GetInput() == m_Tube->GetOutput())
    {
        return 0; // tube
    }
    else
    {
        return 1; // line
    }
}
