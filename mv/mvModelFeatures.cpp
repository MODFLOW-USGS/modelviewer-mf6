#include "mvModelFeatures.h"
#include "mvDataSource.h"
#include "vtkConeSource.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkGlyph3D.h"
#include "vtkCubeSource.h"
#include "vtkStructuredGrid.h"
#include "vtkThreshold.h"
#include "vtkThresholdPoints.h"
#include "vtkTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDoubleArray.h"
#include "vtkLookupTable.h"
#include "vtkDoubleArray.h"
#include "vtkDataSetAttributes.h"
#include "vtkCellData.h"
#include "vtkUnsignedCharArray.h"
#include "vtkIdTypeArray.h"
#include "vtkCellArray.h"

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvModelFeatures::mvModelFeatures()
{
    m_DisplayMode      = MV_DISPLAY_MODEL_FEATURES_AS_CELLS;
    m_StructuredGrid   = 0;
    m_UnstructuredGrid = 0;
    m_ThresholdCells   = vtkSmartPointer<vtkThreshold>::New();
    m_ThresholdCells->SetInputData(m_StructuredGrid);
#if ((VTK_MAJOR_VERSION == 9) && (VTK_MINOR_VERSION < 1) || (VTK_MAJOR_VERSION < 9))
    m_ThresholdCells->ThresholdBetween(0.5, 10000);    // deprecated as of VTK 9.1
#else
    m_ThresholdCells->SetThresholdFunction(vtkThreshold::THRESHOLD_BETWEEN);
    m_ThresholdCells->SetLowerThreshold(0.5);
    m_ThresholdCells->SetUpperThreshold(10000);
#endif
    m_ThresholdCells->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, vtkDataSetAttributes::SCALARS);
    SetMapperInputConnection(m_ThresholdCells->GetOutputPort());

    m_Lut = vtkSmartPointer<vtkLookupTable>::New();
    m_Mapper->SetLookupTable(m_Lut);

    // Objects for glyph display
    m_PolyData        = vtkSmartPointer<vtkPolyData>::New();
    m_Glyph           = vtkSmartPointer<vtkGlyph3D>::New();
    m_CubeSource      = vtkSmartPointer<vtkCubeSource>::New();
    m_ThresholdPoints = vtkSmartPointer<vtkThresholdPoints>::New();
    m_ThresholdPoints->ThresholdBetween(0.5, 10000);
    m_Transform       = vtkSmartPointer<vtkTransform>::New();
    m_TransformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    m_ThresholdPoints->SetInputData(m_PolyData);
    m_TransformFilter->SetInputConnection(m_ThresholdPoints->GetOutputPort());
    m_TransformFilter->SetTransform(m_Transform);
    m_Glyph->SetInputConnection(m_TransformFilter->GetOutputPort());
    m_Glyph->SetScaleModeToDataScalingOff();
    m_Glyph->SetColorModeToColorByScalar();
    m_Glyph->SetSourceConnection(m_CubeSource->GetOutputPort());
    m_DefaultGlyphSize   = 1;
    m_NumberOfGlyphs     = 0;
    m_Dim[0]             = 0;
    m_Dim[1]             = 0;
    m_Dim[2]             = 0;
    m_Subgrid[0]         = 0;
    m_Subgrid[1]         = 0;
    m_Subgrid[2]         = 0;
    m_Subgrid[3]         = 0;
    m_Subgrid[4]         = 0;
    m_Subgrid[5]         = 0;
    m_GlyphScalarArray   = 0;
    m_DisplayOrder       = 0;
    m_NumberOfCells      = 0;
    m_CellScalarArray    = 0;
    m_ModelFeatureArray  = 0;
    m_SubgridIsActivated = 0;
    m_CellTypes          = 0;
    m_CellLocations      = 0;
    m_Cells              = 0;
}

mvModelFeatures::~mvModelFeatures()
{
    if (m_GlyphScalarArray != 0)
    {
        delete[] m_GlyphScalarArray;
    }
    if (m_DisplayOrder != 0)
    {
        delete[] m_DisplayOrder;
    }
    if (m_CellScalarArray != 0)
    {
        delete[] m_CellScalarArray;
    }
}

void mvModelFeatures::SetDisplayMode(int displayMode)
{
    m_DisplayMode = displayMode;
}

int mvModelFeatures::GetDisplayMode() const
{
    return m_DisplayMode;
}

int *mvModelFeatures::GetDisplayOrder()
{
    return m_DisplayOrder;
}

void mvModelFeatures::GetColor(int i, double *rgba)
{
    if (m_Lut)
    {
        double *color = m_Lut->GetTableValue(i);
        rgba[0]       = color[0];
        rgba[1]       = color[1];
        rgba[2]       = color[2];
        rgba[3]       = color[3];
    }
}

void mvModelFeatures::SetColor(int i, double *rgba)
{
    if (m_Lut)
    {
        m_Lut->SetTableValue(i, rgba);
    }
}

void mvModelFeatures::SetNumberOfModelFeatureTypes(int numModelFeatureTypes)
{
    m_NumberOfModelFeatureTypes = numModelFeatureTypes;
    m_Lut->SetNumberOfColors(m_NumberOfModelFeatureTypes);
}

void mvModelFeatures::SetModelFeatureArray(int *modelFeatureArray)
{
    m_ModelFeatureArray = modelFeatureArray;

    if (m_ModelFeatureArray == 0 || m_CellScalarArray == 0)
    {
        return;
    }

    if (m_DisplayOrder != 0)
    {
        int k = 0;
        for (int i = 0; i < m_NumberOfModelFeatureTypes; i++)
        {
            if (m_ModelFeatureArray[k] >= 0)
            {
                k += m_ModelFeatureArray[k];
            }
            else
            {
                m_DisplayOrder[i] = -2;
            }
            k++;
        }
    }

    if (m_DisplayMode == MV_DISPLAY_MODEL_FEATURES_AS_GLYPHS)
    {
        // TO DO: Implement this
    }
    else
    {
        if (m_StructuredGrid)
        {
            m_StructuredGrid->Modified();
        }
        else if (m_UnstructuredGrid)
        {
            m_UnstructuredGrid->Modified();
        }
        SetDisplayOrderForCells();
    }
}

void mvModelFeatures::SetGridPoints(vtkPoints *gridPoints)
{
    m_GridPoints = gridPoints;
}

void mvModelFeatures::Build()
{
    int i, j, k, m;
    if (m_ModelFeatureArray == 0)
    {
        return;
    }
    m_Mapper->SetScalarRange(1, m_NumberOfModelFeatureTypes);
    if (m_DisplayOrder != 0)
    {
        delete[] m_DisplayOrder;
    }
    m_DisplayOrder = new int[m_NumberOfModelFeatureTypes];
    for (i = 0; i < m_NumberOfModelFeatureTypes; i++)
    {
        m_DisplayOrder[i] = -1;
    }
    m_Lut->Build();
    m_SubgridIsActivated = 0;
    m_Actor->SetScale(1, 1, 1);
    if (m_DisplayMode == MV_DISPLAY_MODEL_FEATURES_AS_GLYPHS)
    {
        SetMapperInputConnection(m_Glyph->GetOutputPort());
        m_ThresholdPoints->ThresholdBetween(0.5, m_NumberOfModelFeatureTypes + 0.5);
        int count = 0;
        k         = 0;
        for (i = 0; i < m_NumberOfModelFeatureTypes; i++)
        {
            count += m_ModelFeatureArray[k];
            k += (m_ModelFeatureArray[k] + 1);
        }
        m_NumberOfGlyphs = count;
        if (m_GlyphScalarArray != 0)
        {
            delete[] m_GlyphScalarArray;
        }
        m_GlyphScalarArray                      = new double[m_NumberOfGlyphs];
        vtkSmartPointer<vtkPoints>      points  = vtkSmartPointer<vtkPoints>::New();
        vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
        m                                       = 0;
        k                                       = 0;
        for (i = 0; i < m_NumberOfModelFeatureTypes; i++)
        {
            int numGlyph = m_ModelFeatureArray[k];
            k++;
            for (j = 0; j < numGlyph; j++)
            {
                double *p = m_GridPoints->GetPoint(m_ModelFeatureArray[k]);
                points->InsertNextPoint(p[0], p[1], p[2]);
                // Note that we start counting from 1. Also, all glyphs are
                // set to be initially invisible.
                m_GlyphScalarArray[m] = (double)(i + 1 + m_NumberOfModelFeatureTypes);
                k++;
                m++;
            }
        }
        scalars->SetArray(m_GlyphScalarArray, m_NumberOfGlyphs, 1);

        m_PolyData->SetPoints(points);
        m_PolyData->GetPointData()->SetScalars(scalars);
    }
    else
    {
        SetMapperInputConnection(m_ThresholdCells->GetOutputPort());
#if ((VTK_MAJOR_VERSION == 9) && (VTK_MINOR_VERSION < 1) || (VTK_MAJOR_VERSION < 9))
        m_ThresholdCells->ThresholdBetween(0.5, m_NumberOfModelFeatureTypes + 0.5);    // deprecated as of VTK 9.1
#else
        m_ThresholdCells->SetThresholdFunction(vtkThreshold::THRESHOLD_BETWEEN);
        m_ThresholdCells->SetLowerThreshold(0.5);
        m_ThresholdCells->SetUpperThreshold(m_NumberOfModelFeatureTypes + 0.5);
#endif
        if (m_UnstructuredGrid)
        {
            m_UnstructuredGrid->SetCells(m_CellTypes, m_CellLocations, m_Cells);
            m_UnstructuredGrid->SetPoints(m_GridPoints);
            m_NumberOfCells = m_Cells->GetNumberOfCells();
        }
        else
        {
            m_StructuredGrid->SetDimensions(m_Dim[0], m_Dim[1], m_Dim[2]);
            m_StructuredGrid->SetPoints(m_GridPoints);
            m_NumberOfCells = (m_Dim[0] - 1) * (m_Dim[1] - 1) * (m_Dim[2] - 1);
        }
        if (m_CellScalarArray != 0)
        {
            delete[] m_CellScalarArray;
        }
        m_CellScalarArray = new double[m_NumberOfCells];
        for (i = 0; i < m_NumberOfCells; i++)
        {
            m_CellScalarArray[i] = 0;
        }
        vtkSmartPointer<vtkDoubleArray> scalars = vtkSmartPointer<vtkDoubleArray>::New();
        scalars->SetNumberOfComponents(1);
        scalars->SetArray(m_CellScalarArray, m_NumberOfCells, 1);
        if (m_UnstructuredGrid)
        {
            m_UnstructuredGrid->GetCellData()->SetScalars(scalars);
        }
        else
        {
            m_StructuredGrid->GetCellData()->SetScalars(scalars);
        }
    }
}

void mvModelFeatures::SetScale(double xScale, double yScale, double zScale)
{
    if (m_DisplayMode == MV_DISPLAY_MODEL_FEATURES_AS_GLYPHS)
    {
        //		m_Actor->SetScale(1, 1, 1);
        double ps[3];
        m_Transform->GetScale(ps);
        m_Transform->Scale(xScale / ps[0], yScale / ps[1], zScale / ps[2]);
    }
    else
    {
        m_Actor->SetScale(xScale, yScale, zScale);
    }
}

void mvModelFeatures::SetDisplayOrder(int *displayOrder)
{
    if (m_ModelFeatureArray == 0 || m_DisplayOrder == 0)
    {
        return;
    }
    for (int i = 0; i < m_NumberOfModelFeatureTypes; i++)
    {
        m_DisplayOrder[i] = displayOrder[i];
    }
    if (m_DisplayMode == MV_DISPLAY_MODEL_FEATURES_AS_GLYPHS)
    {
        SetDisplayOrderForGlyphs();
    }
    else
    {
        SetDisplayOrderForCells();
    }
}

void mvModelFeatures::SetDefaultGlyphSize(double gs)
{
    m_CubeSource->SetXLength(gs);
    m_CubeSource->SetYLength(gs);
    m_CubeSource->SetZLength(gs);
}

void mvModelFeatures::EnlargeGlyphs()
{
    m_CubeSource->SetXLength(m_CubeSource->GetXLength() * 1.5);
    m_CubeSource->SetYLength(m_CubeSource->GetYLength() * 1.5);
    m_CubeSource->SetZLength(m_CubeSource->GetZLength() * 1.5);
}

void mvModelFeatures::ShrinkGlyphs()
{
    m_CubeSource->SetXLength(m_CubeSource->GetXLength() / 1.5);
    m_CubeSource->SetYLength(m_CubeSource->GetYLength() / 1.5);
    m_CubeSource->SetZLength(m_CubeSource->GetZLength() / 1.5);
}

double mvModelFeatures::GetGlyphSize()
{
    return m_CubeSource->GetXLength();
}

void mvModelFeatures::SetFullGridDimensions(const int *dim)
{
    m_Dim[0] = dim[0];
    m_Dim[1] = dim[1];
    m_Dim[2] = dim[2];
}

void mvModelFeatures::SetSubgridExtent(int imin, int imax,
                                       int jmin, int jmax, int kmin, int kmax)
{
    m_Subgrid[0] = imin;
    m_Subgrid[1] = imax;
    m_Subgrid[2] = jmin;
    m_Subgrid[3] = jmax;
    m_Subgrid[4] = kmin;
    m_Subgrid[5] = kmax;
}

void mvModelFeatures::SubgridOn()
{
    int i, j, index, ii, jj, kk;
    if (m_DisplayMode == MV_DISPLAY_MODEL_FEATURES_AS_GLYPHS)
    {
        int m = 0;
        int k = 0;
        for (i = 0; i < m_NumberOfModelFeatureTypes; i++)
        {
            int numGlyph = m_ModelFeatureArray[k];
            k++;
            for (j = 0; j < numGlyph; j++)
            {
                index = m_ModelFeatureArray[k];
                kk    = index / (m_Dim[0] * m_Dim[1]);
                index -= kk * (m_Dim[0] * m_Dim[1]);
                jj = index / m_Dim[0];
                ii = index - jj * m_Dim[0];
                if (ii < m_Subgrid[0] || ii > m_Subgrid[1] ||
                    jj < m_Subgrid[2] || jj > m_Subgrid[3] ||
                    kk < m_Subgrid[4] || kk > m_Subgrid[5])
                {
                    m_GlyphScalarArray[m] = -abs(m_GlyphScalarArray[m]);
                }
                else
                {
                    m_GlyphScalarArray[m] = abs(m_GlyphScalarArray[m]);
                }
                k++;
                m++;
            }
        }
        m_PolyData->Modified();
    }
    else
    {
        if (!m_StructuredGrid) return;
        int i, j, k, m;
        m = 0;
        for (k = 0; k < m_Dim[2] - 1; k++)
        {
            for (j = 0; j < m_Dim[1] - 1; j++)
            {
                for (i = 0; i < m_Dim[0] - 1; i++)
                {
                    if (i < m_Subgrid[0] || i > m_Subgrid[1] - 1 ||
                        j < m_Subgrid[2] || j > m_Subgrid[3] - 1 ||
                        k < m_Subgrid[4] || k > m_Subgrid[5] - 1)
                    {
                        m_CellScalarArray[m] = -abs(m_CellScalarArray[m]);
                    }
                    else
                    {
                        m_CellScalarArray[m] = abs(m_CellScalarArray[m]);
                    }
                    m++;
                }
            }
        }
        m_StructuredGrid->Modified();
    }
    m_SubgridIsActivated = 1;
}

void mvModelFeatures::SubgridOff()
{
    if (m_DisplayMode == MV_DISPLAY_MODEL_FEATURES_AS_GLYPHS)
    {
        for (int i = 0; i < m_NumberOfGlyphs; i++)
        {
            m_GlyphScalarArray[i] = abs(m_GlyphScalarArray[i]);
        }
        m_PolyData->Modified();
    }
    else
    {
        if (!m_StructuredGrid) return;
        for (int i = 0; i < (m_Dim[0] - 1) * (m_Dim[1] - 1) * (m_Dim[2] - 1); i++)
        {
            m_CellScalarArray[i] = abs(m_CellScalarArray[i]);
        }
        m_StructuredGrid->Modified();
    }
    m_SubgridIsActivated = 0;
}

void mvModelFeatures::SetDisplayOrderForGlyphs()
{
    int i, j, k, m, p, q;
    // To begin, hide all glyphs
    for (j = 0; j < m_NumberOfGlyphs; j++)
    {
        if (m_GlyphScalarArray[j] > 0 &&
            m_GlyphScalarArray[j] <= m_NumberOfModelFeatureTypes)
        {
            m_GlyphScalarArray[j] += m_NumberOfModelFeatureTypes;
        }
        else if (m_GlyphScalarArray[j] < 0 &&
                 m_GlyphScalarArray[j] >= -m_NumberOfModelFeatureTypes)
        {
            m_GlyphScalarArray[j] -= m_NumberOfModelFeatureTypes;
        }
    }
    m_PolyData->Modified();
    int  np     = m_GridPoints->GetNumberOfPoints();
    int *marker = new int[np];
    for (i = 0; i < np; i++)
    {
        marker[i] = 0;
    }

    // Turn on glyphs respresnting features that are displayed
    for (p = 0; p < m_NumberOfModelFeatureTypes; p++)
    {
        // p is the display order
        for (j = 0; j < m_NumberOfModelFeatureTypes; j++)
        {
            if (m_DisplayOrder[j] == p)
            {
                // j is the index of the model feature to be displayed
                k = 0;
                m = 0;
                for (i = 0; i < j; i++)
                {
                    m += m_ModelFeatureArray[k];
                    k += m_ModelFeatureArray[k] + 1;
                }
                // m is the starting index of the glyph scalar array where
                // the scalar is to be modified to indicate that the glyph is shown.
                // k is the index of the model feature array containing the
                // number of glyphs representing the feature that is displayed
                for (i = m, q = 1; i < m + m_ModelFeatureArray[k]; i++, q++)
                {
                    // Display a glyph only when the node is unoccupied.
                    int mp = m_ModelFeatureArray[k + q];
                    if (marker[mp] == 0)
                    {
                        // mark the node as occupied.
                        marker[mp] = 1;
                        if (m_GlyphScalarArray[i] > 0)
                        {
                            m_GlyphScalarArray[i] -= m_NumberOfModelFeatureTypes;
                        }
                        else
                        {
                            m_GlyphScalarArray[i] += m_NumberOfModelFeatureTypes;
                        }
                    }
                }
            }
        }
    }
    delete[] marker;
}

void mvModelFeatures::SetDisplayOrderForCells()
{
    int i, j, k, m, p;
    for (j = 0; j < m_NumberOfCells; j++)
    {
        m_CellScalarArray[j] = 0;
    }
    if (m_StructuredGrid)
    {
        m_StructuredGrid->Modified();
    }
    else if (m_UnstructuredGrid)
    {
        m_UnstructuredGrid->Modified();
    }

    // Turn on cells respresnting features that are displayed,
    // starting from last in order toward first
    for (p = m_NumberOfModelFeatureTypes - 1; p >= 0; p--)
    {
        // p is the display order
        for (j = 0; j < m_NumberOfModelFeatureTypes; j++)
        {
            if (m_DisplayOrder[j] == p)
            {
                // j is the index of the model feature to be displayed
                k = 0;
                for (i = 0; i < j; i++)
                {
                    if (m_ModelFeatureArray[k] > 0)
                    {
                        k += m_ModelFeatureArray[k];
                    }
                    k++;
                }
                // k is the index of the model feature array containing
                // how many cells represent the feature that is displayed
                for (i = 1; i <= m_ModelFeatureArray[k]; i++)
                {
                    m = m_ModelFeatureArray[k + i];
                    if (m >= 0)
                    {
                        m_CellScalarArray[m] = j + 1;
                    }
                }
            }
        }
    }
    if (m_SubgridIsActivated)
    {
        SubgridOn();
    }
}

void mvModelFeatures::SetCellArrayForUnstructuredGrid(vtkUnsignedCharArray *cellTypes, vtkIdTypeArray *cellLocations, vtkCellArray *cells)
{
    m_CellTypes     = cellTypes;
    m_CellLocations = cellLocations;
    m_Cells         = cells;
}

void mvModelFeatures::SetGridTypeToStructuredGrid()
{
    m_UnstructuredGrid = 0;
    m_StructuredGrid   = vtkSmartPointer<vtkStructuredGrid>::New();
    m_ThresholdCells->SetInputData(m_StructuredGrid);
    m_ThresholdCells->Modified();
}

void mvModelFeatures::SetGridTypeToUnstructuredGrid()
{
    m_StructuredGrid   = 0;
    m_UnstructuredGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
    m_ThresholdCells->SetInputData(m_UnstructuredGrid);
    m_ThresholdCells->Modified();
}

void mvModelFeatures::Update()
{
    if (m_DisplayMode == MV_DISPLAY_MODEL_FEATURES_AS_GLYPHS)
    {
    }
    else
    {
        if (m_UnstructuredGrid)
        {
            m_UnstructuredGrid->SetCells(m_CellTypes, m_CellLocations, m_Cells);
            m_UnstructuredGrid->SetPoints(m_GridPoints);
            m_UnstructuredGrid->Modified();
        }
        else
        {
            m_StructuredGrid->SetDimensions(m_Dim[0], m_Dim[1], m_Dim[2]);
            m_StructuredGrid->SetPoints(m_GridPoints);
            m_StructuredGrid->Modified();
        }
    }
}