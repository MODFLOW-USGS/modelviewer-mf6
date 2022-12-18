#include "mvGridLines.h"
#include "vtkStructuredGrid.h"
#include "vtkStructuredGridGeometryFilter.h"
#include "vtkThreshold.h"
#include "vtkGeometryFilter.h"
#include "vtkFeatureEdges.h"
#include "vtkExtractEdges.h"
#include "vtkProperty.h"
#include "vtkDataSetAttributes.h"
#include "vtkUnstructuredGrid.h"
#include "vtkExtractCells.h"
#include "vtkIdList.h"
#include "mvDefine.h"

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvGridLines::mvGridLines()
{
    m_SGGeometryFilter = 0;
    m_ExtractCells     = 0;
    m_Threshold        = vtkSmartPointer<vtkThreshold>::New();
    // input connection for m_Threshold is set in the method SetInputData.
    // the initial setup assumes that cell scalars are used to identify inactive cells
    m_Threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, vtkDataSetAttributes::SCALARS);
    m_GeometryFilter = vtkSmartPointer<vtkGeometryFilter>::New();
    m_GeometryFilter->SetInputConnection(m_Threshold->GetOutputPort());
    m_WireFrame = vtkSmartPointer<vtkExtractEdges>::New();
    m_WireFrame->SetInputConnection(m_GeometryFilter->GetOutputPort());
    SetMapperInputConnection(m_WireFrame->GetOutputPort());
    m_Mapper->ScalarVisibilityOff();
    m_Actor->GetProperty()->SetColor(0.5, 0.5, 0.5);
    m_Actor->GetProperty()->SetAmbient(1.0);
    m_Actor->GetProperty()->SetDiffuse(0.0);
    m_LODActor->GetProperty()->SetColor(0.5, 0.5, 0.5);
    m_LODActor->GetProperty()->SetAmbient(1.0);
    m_LODActor->GetProperty()->SetDiffuse(0.0);
    m_Extent[0] = -1;
    m_Extent[1] = -1;
    m_Extent[2] = -1;
    m_Extent[3] = -1;
    m_Extent[4] = -1;
    m_Extent[5] = -1;
    m_NumCol    = 0;
    m_NumRow    = 0;
    m_NumLay    = 0;
}

mvGridLines::~mvGridLines()
{
}

void mvGridLines::SetInputData(vtkStructuredGrid *dataSet)
{
    m_SGGeometryFilter = vtkSmartPointer<vtkStructuredGridGeometryFilter>::New();
    m_SGGeometryFilter->SetInputData(dataSet);
    m_Threshold->SetInputConnection(m_SGGeometryFilter->GetOutputPort());
    m_GridDisplayMode = MV_GRID_INTERPOLATED;
}

void mvGridLines::SetInputData(vtkUnstructuredGrid *dataSet, int numCol, int numRow, int numLay)
{
    m_ExtractCells = vtkSmartPointer<vtkExtractCells>::New();
    m_ExtractCells->SetInputData(dataSet);
    m_Threshold->SetInputConnection(m_ExtractCells->GetOutputPort());
    m_GridDisplayMode = MV_GRID_STAIRSTEPPED;
    m_NumCol          = numCol;
    m_NumRow          = numRow;
    m_NumLay          = numLay;
}

void mvGridLines::SetThresholdMin(double value)
{
#if ((VTK_MAJOR_VERSION == 9) && (VTK_MINOR_VERSION < 1) || (VTK_MAJOR_VERSION < 9))
    m_Threshold->ThresholdByLower(value);    // deprecated as of VTK 9.1
#else
    m_Threshold->SetThresholdFunction(vtkThreshold::THRESHOLD_LOWER);
    m_Threshold->SetLowerThreshold(value);
#endif
}

const int *mvGridLines::GetExtent() const
{
    return m_Extent;
}

void mvGridLines::SetExtent(int imin, int imax, int jmin, int jmax, int kmin, int kmax)
{
    if (m_SGGeometryFilter)
    {
        m_SGGeometryFilter->SetExtent(imin, imax, jmin, jmax, kmin, kmax);
    }
    else if (m_ExtractCells)
    {
        // To do
        vtkSmartPointer<vtkIdList> cellList = vtkSmartPointer<vtkIdList>::New();
        // note that kc, jc, and ic are cell indices
        for (int kc = kmin; kc < kmax; kc++)
        {
            for (int jc = jmin; jc < jmax; jc++)
            {
                for (int ic = imin; ic < imax; ic++)
                {
                    cellList->InsertNextId(kc * m_NumCol * m_NumRow + jc * m_NumCol + ic);
                }
            }
        }
        m_ExtractCells->SetCellList(cellList);
    }
    else
    {
        return;
    }
    m_Extent[0] = imin;
    m_Extent[1] = imax;
    m_Extent[2] = jmin;
    m_Extent[3] = jmax;
    m_Extent[4] = kmin;
    m_Extent[5] = kmax;
}

void mvGridLines::DoThresholdUsingCellData()
{
    m_Threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, vtkDataSetAttributes::SCALARS);
}

void mvGridLines::DoThresholdUsingPointData()
{
    m_Threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, vtkDataSetAttributes::SCALARS);
}

void mvGridLines::SetAllCellsAreActive(int b)
{
    if (m_GeometryFilter)
    {
        if (b)
        {
            m_GeometryFilter->SetInputConnection(m_SGGeometryFilter->GetOutputPort());
        }
        else
        {
            m_GeometryFilter->SetInputConnection(m_Threshold->GetOutputPort());
        }
    }
}