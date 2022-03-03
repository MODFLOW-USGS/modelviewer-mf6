#include "mvGridLayer.h"
#include "vtkExtractEdges.h"
#include "vtkPolyData.h"
#include "vtkActor.h"
#include "vtkLODActor.h"
#include "vtkProperty.h"
#include "vtkThreshold.h"
#include "mvDataSource.h"
#include "vtkDataSetAttributes.h"
#include "vtkDoubleArray.h"
#include "vtkSmartPointer.h"
#include "vtkCellData.h"
#include "vtkCellArray.h"

mvGridLayer::mvGridLayer()
{
    m_LayerPosition       = 0;
    vtkPolyData *polyData = vtkPolyData::New();
    m_Threshold           = vtkThreshold::New();
    m_Threshold->SetInputData(polyData);
    polyData->Delete();
    m_Threshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, vtkDataSetAttributes::SCALARS);
    m_GridLayer = vtkExtractEdges::New();
    m_GridLayer->SetInputConnection(m_Threshold->GetOutputPort());
    SetMapperInputConnection(m_GridLayer->GetOutputPort());
    m_Mapper->ScalarVisibilityOff();
    m_Actor->GetProperty()->SetColor(0.5, 0.5, 0.5);
    m_Actor->GetProperty()->SetAmbient(1.0);
    m_Actor->GetProperty()->SetDiffuse(0.0);
    m_LODActor->GetProperty()->SetColor(0.5, 0.5, 0.5);
    m_LODActor->GetProperty()->SetAmbient(1.0);
    m_LODActor->GetProperty()->SetDiffuse(0.0);
}

mvGridLayer::~mvGridLayer()
{
    m_Threshold->Delete();
    m_GridLayer->Delete();
}

void mvGridLayer::SetDataSource(mvDataSource *dataSource)
{
    m_DataSource = dataSource;
}

void mvGridLayer::Build()
{
    // Set up the data to draw a grid layer
    // Grid lines outlines the bottom faces of cells in a layer.
    vtkPolyData    *polyData    = vtkPolyData::New();

    vtkPoints      *points      = vtkPoints::New();
    vtkDoubleArray *doubleArray = vtkDoubleArray::New();
    doubleArray->SetNumberOfComponents(3);
    doubleArray->SetArray(m_DataSource->GetInterpolatedGridCoordinates(),
                          3 * m_DataSource->GetNumVTKPoints(), 1);
    points->SetDataTypeToDouble();
    points->SetData(doubleArray);
    doubleArray->Delete();
    polyData->SetPoints(points);
    points->Delete();

    vtkCellArray   *polys       = vtkCellArray::New();
    vtkDoubleArray *cellScalars = vtkDoubleArray::New();

    vtkIdType       id[20];
    int            *layerConnectivityArray = m_DataSource->GetLayerConnectivity();
    int             m                      = 1;
    for (int i = 0; i < layerConnectivityArray[0]; i++)
    {
        int layerIndex = layerConnectivityArray[m++];
        int jmax       = layerConnectivityArray[m++];
        for (int j = 0; j < jmax; j++)
        {
            id[j] = layerConnectivityArray[m++];
        }
        polys->InsertNextCell(jmax, id);
        cellScalars->InsertTuple1(i, layerIndex);
    }

    polyData->GetCellData()->SetScalars(cellScalars);
    polyData->SetPolys(polys);
    polys->Delete();
    cellScalars->Delete();
    m_Threshold->SetInputData(polyData);
    polyData->Delete();
    m_LayerPosition = 0;
    m_Threshold->ThresholdBetween(-0.5, 0.5);
}

void mvGridLayer::SetLayerPosition(int layerPosition)
{
    m_LayerPosition = layerPosition;
    m_Threshold->ThresholdBetween(layerPosition - 0.5, layerPosition + 0.5);
}

int mvGridLayer::GetLayerPosition() const
{
    return m_LayerPosition;
}