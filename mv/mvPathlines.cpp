#include "mvPathlines.h"
#include "vtkCleanPolyData.h"
#include "vtkClipPolyData.h"
#include "vtkLookupTable.h"
#include "vtkLogLookupTable.h"
#include "vtkPlane.h"
#include "vtkPolyData.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTubeFilter.h"
#include "vtkPointData.h"
#include "mvColorTable.h"
#include "mvLogColorTable.h"
#include "vtkAlgorithmOutput.h"

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvPathlines::mvPathlines()
{
    m_TimeClipping   = 0;
    m_LogTransform   = 0;
    m_Cropping       = 0;
    m_Representation = PATHLINE_LINE;
    m_LutBlueToRed   = mvColorTable::New();
    dynamic_cast<mvColorTable *>(m_LutBlueToRed)->SetDefaultColorScheme();
    m_LutRedToBlue = mvColorTable::New();
    dynamic_cast<mvColorTable *>(m_LutRedToBlue)->SetReversedDefaultColorScheme();

    m_LogLutBlueToRed = mvLogColorTable::New();
    m_LogLutBlueToRed->SetScaleToLog10();
    dynamic_cast<mvLogColorTable *>(m_LogLutBlueToRed)->SetReversedDefaultColorScheme();
    m_LogLutRedToBlue = mvLogColorTable::New();
    m_LogLutRedToBlue->SetScaleToLog10();
    dynamic_cast<mvLogColorTable *>(m_LogLutRedToBlue)->SetDefaultColorScheme();

    m_Input   = vtkPolyData::New();
    m_ClipMin = vtkClipPolyData::New();
    m_ClipMin->SetInputData(m_Input);
    m_ClipMin->SetValue(0);
    m_ClipMax = vtkClipPolyData::New();
    m_ClipMax->SetInputConnection(m_ClipMin->GetOutputPort());
    m_ClipMax->InsideOutOn();
    m_ClipMax->SetValue(1);

    m_Transform       = vtkTransform::New();
    m_TransformFilter = vtkTransformPolyDataFilter::New();
    m_TransformFilter->SetInputData(m_Input);
    m_TransformFilter->SetTransform(m_Transform);
    m_Tube = vtkTubeFilter::New();
    m_Tube->SetInputConnection(m_TransformFilter->GetOutputPort());
    m_Tube->SetNumberOfSides(10);
    SetMapperInputConnection(m_Tube->GetOutputPort());
    m_Mapper->SetLookupTable(m_LutBlueToRed);
    m_TimeBlue            = 0;
    m_TimeRed             = 1;
    m_DefaultTubeDiameter = 1;

    for (int i = 0; i < 6; i++)
    {
        m_Plane[i]   = vtkPlane::New();
        m_Cropper[i] = vtkClipPolyData::New();
        m_Cropper[i]->SetClipFunction(m_Plane[i]);
        if (i > 0)
        {
            m_Cropper[i]->SetInputConnection(m_Cropper[i - 1]->GetOutputPort());
        }
    }
    m_Cropper[0]->SetInputData(m_Input);
    m_Plane[0]->SetNormal(1, 0, 0);
    m_Plane[1]->SetNormal(-1, 0, 0);
    m_Plane[2]->SetNormal(0, 1, 0);
    m_Plane[3]->SetNormal(0, -1, 0);
    m_Plane[4]->SetNormal(0, 0, 1);
    m_Plane[5]->SetNormal(0, 0, -1);
}

mvPathlines::~mvPathlines()
{
    m_LutBlueToRed->Delete();
    m_LutRedToBlue->Delete();
    m_LogLutBlueToRed->Delete();
    m_LogLutRedToBlue->Delete();
    m_Input->Delete();
    m_ClipMin->Delete();
    m_ClipMax->Delete();
    m_Transform->Delete();
    m_TransformFilter->Delete();
    m_Tube->Delete();
    for (int i = 0; i < 6; i++)
    {
        m_Plane[i]->Delete();
        m_Cropper[i]->Delete();
    }
}

void mvPathlines::SetInputData(vtkPolyData *polydata)
{
    m_Input->CopyStructure(polydata);
    m_Input->GetPointData()->SetScalars(polydata->GetPointData()->GetScalars());
}

void mvPathlines::SetRepresentationToLine()
{
    m_Representation = PATHLINE_LINE;
    BuildPipeLine();
}

void mvPathlines::SetRepresentationToTube()
{
    m_Representation = PATHLINE_TUBE;
    BuildPipeLine();
}

int mvPathlines::GetRepresentation() const
{
    return m_Representation;
}

void mvPathlines::SetDefaultTubeDiameter(double d)
{
    m_Tube->SetRadius(GetNormalizedTubeDiameter() * d / 2);
    m_DefaultTubeDiameter = d;
}

void mvPathlines::SetNormalizedTubeDiameter(double dn)
{
    m_Tube->SetRadius(dn * m_DefaultTubeDiameter / 2);
}

double mvPathlines::GetNormalizedTubeDiameter() const
{
    return (2 * m_Tube->GetRadius() / m_DefaultTubeDiameter);
}

void mvPathlines::SetScale(double xScale, double yScale, double zScale)
{
    double ps[3];
    m_Transform->GetScale(ps);
    m_Transform->Scale(xScale / ps[0], yScale / ps[1], zScale / ps[2]);
}

void mvPathlines::DoCrop(const double *bounds)
{
    m_Cropping = 1;
    m_Plane[0]->SetOrigin(bounds[0], 0, 0);
    m_Plane[1]->SetOrigin(bounds[1], 0, 0);
    m_Plane[2]->SetOrigin(0, bounds[2], 0);
    m_Plane[3]->SetOrigin(0, bounds[3], 0);
    m_Plane[4]->SetOrigin(0, 0, bounds[4]);
    m_Plane[5]->SetOrigin(0, 0, bounds[5]);
    BuildPipeLine();
}

void mvPathlines::CropOff()
{
    m_Cropping = 0;
    BuildPipeLine();
}

int mvPathlines::IsCroppingOn() const
{
    return m_Cropping;
}

void mvPathlines::TimeClippingOn()
{
    m_TimeClipping = 1;
    BuildPipeLine();
}

void mvPathlines::TimeClippingOff()
{
    m_TimeClipping = 0;
    BuildPipeLine();
}

int mvPathlines::IsTimeClippingOn() const
{
    return m_TimeClipping;
}

void mvPathlines::SetTimeClippingRange(double minTime, double maxTime)
{
    m_ClipMin->SetValue(minTime);
    m_ClipMax->SetValue(maxTime);
}

void mvPathlines::SetColorBarEndPoints(double valueBlue, double valueRed)
{
    m_TimeBlue = valueBlue;
    m_TimeRed  = valueRed;
    BuildPipeLine();
}

void mvPathlines::SetLogTransform(int Value)
{
    m_LogTransform = Value;
    BuildPipeLine();
}

void mvPathlines::SetMinPositiveValue(double Value)
{
    m_MinPositiveValue = Value;
}

void mvPathlines::BuildPipeLine()
{
    // first set up the pipeline for the input.
    vtkAlgorithmOutput *previousAlgorithmOutput = 0;

    m_ClipMin->SetInputData(m_Input);
    m_ClipMax->SetInputConnection(m_ClipMin->GetOutputPort());
    if (IsTimeClippingOn())
    {
        previousAlgorithmOutput = m_ClipMax->GetOutputPort();
    }

    if (previousAlgorithmOutput)
    {
        m_Cropper[0]->SetInputConnection(previousAlgorithmOutput);
    }
    else
    {
        m_Cropper[0]->SetInputData(m_Input);
    }
    for (int i = 1; i < 6; i++)
    {
        m_Cropper[i]->SetInputConnection(m_Cropper[i - 1]->GetOutputPort());
    }
    if (IsCroppingOn())
    {
        previousAlgorithmOutput = m_Cropper[5]->GetOutputPort();
    }

    if (previousAlgorithmOutput)
    {
        m_TransformFilter->SetInputConnection(previousAlgorithmOutput);
    }
    else
    {
        m_TransformFilter->SetInputData(m_Input);
    }
    previousAlgorithmOutput = m_TransformFilter->GetOutputPort();

    m_Tube->SetInputConnection(previousAlgorithmOutput);

    if (GetRepresentation() == PATHLINE_TUBE)
    {
        previousAlgorithmOutput = m_Tube->GetOutputPort();
    }
    SetMapperInputConnection(previousAlgorithmOutput);

    // Next choose the correct look up table for the colors

    double range[2];
    if (m_TimeRed > m_TimeBlue)
    {
        range[0] = m_TimeBlue;
        range[1] = m_TimeRed;
        if (GetLogTransform())
        {
            if (m_TimeBlue <= 0)
            {
                range[0] = m_MinPositiveValue;
            }
            m_LogLutBlueToRed->SetRange(range[0], range[1]);
            m_Mapper->SetLookupTable(m_LogLutBlueToRed);
        }
        else
        {
            m_LutBlueToRed->SetRange(range[0], range[1]);
            m_Mapper->SetLookupTable(m_LutBlueToRed);
        }
    }
    else
    {
        range[0] = m_TimeRed;
        range[1] = m_TimeBlue;
        if (GetLogTransform())
        {
            if (m_TimeRed <= 0)
            {
                range[0] = m_MinPositiveValue;
            }
            m_LogLutRedToBlue->SetRange(range[0], range[1]);
            m_Mapper->SetLookupTable(m_LogLutRedToBlue);
        }
        else
        {
            m_LutRedToBlue->SetRange(range[0], range[1]);
            m_Mapper->SetLookupTable(m_LutRedToBlue);
        }
    }

    m_Mapper->SetScalarRange(range);
}