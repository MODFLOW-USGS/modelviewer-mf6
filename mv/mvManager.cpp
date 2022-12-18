#include "mvManager.h"

#include "mvAxes.h"
#include "mvBoundingBox.h"
#include "mvClipBox.h"
#include "mvColorBar.h"
#include "mvColorTable.h"
#include "mvCustomAppendPolyData.h"
#include "mvDisplayText.h"
#include "mvGridLayer.h"
#include "mvGridLines.h"
#include "mvGridShell.h"
#include "mvGUISettings.h"
#include "mvHashTable.h"
#include "mvLogColorTable.h"
#include "mvModelFeatures.h"
#include "mvModelList.h"
#include "mvOverlay.h"
#include "mvPathlines.h"
#include "mvSaveCurrentDirectory.h"
#include "mvUtil.h"

#include "vtkActor.h"
#include "vtkAlgorithmOutput.h"
#include "vtkBandedPolyDataContourFilter.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkClipPolyData.h"
#include "vtkContourFilter.h"
#include "vtkCubeSource.h"
#include "vtkCutter.h"
#include "vtkDataSetMapper.h"
#include "vtkDoubleArray.h"
#include "vtkExtractCells.h"
#include "vtkExtractGeometry.h"
#include "vtkExtractGrid.h"
#include "vtkGeometryFilter.h"
#include "vtkGlyph3D.h"
#include "vtkHedgeHog.h"
#include "vtkHexagonalPrism.h"
#include "vtkHexahedron.h"
#include "vtkLogLookupTable.h"
#include "vtkLookupTable.h"
#include "vtkMaskPoints.h"
#include "vtkPentagonalPrism.h"
#include "vtkPlane.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPropCollection.h"
#include "vtkProperty.h"
#include "vtkProperty2D.h"
#include "vtkStructuredGrid.h"
#include "vtkTextProperty.h"
#include "vtkThreshold.h"
#include "vtkThresholdPoints.h"
#include "vtkUnstructuredGrid.h"
#include "vtkWedge.h"

#include "mvDefine.h"

#if defined(QT_GUI_LIB)
#include <QDir>
#endif

#include <sstream>

using std::log;
using std::ofstream;
using std::pow;

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER)
//#include <afx.h>
#if defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

#if !defined(MAX_PATH)
constexpr auto MAX_PATH = 4096;
#endif

#if !defined(VERIFY)
#define VERIFY(f) ((void)(f))
#endif

mvManager::mvManager()
//{{
    : m_ColorBarDataSource{0}
//}}
{
    // initial values
    m_ActiveDataType             = 0;
    m_DoVectorThreshold          = 0;
    m_NumScalarDataTypes         = 0;
    m_PathlineTimeClippingMode   = 0;
    m_ShowCroppedAwayPieces      = 0;
    m_TimePointIndex             = 0;
    m_VectorGlyphActivated       = 0;
    m_CropAngle                  = 0;
    m_PathlineClipTimeMax        = 0;
    m_PathlineClipTimeMin        = 0;
    m_ActiveScalarRange[0]       = 0;
    m_ActiveScalarRange[1]       = 0;
    m_VectorLogMagnitudeRange[0] = 0;
    m_VectorLogMagnitudeRange[1] = 0;
    m_VectorMagnitudeRange[0]    = 0;
    m_VectorMagnitudeRange[1]    = 0;
    m_VectorSubsampleOffset[0]   = 0;
    m_VectorSubsampleOffset[1]   = 0;
    m_VectorSubsampleOffset[2]   = 0;
    m_CropBounds[0]              = 0;
    m_CropBounds[1]              = 1;
    m_CropBounds[2]              = 0;
    m_CropBounds[3]              = 1;
    m_CropBounds[4]              = 0;
    m_CropBounds[5]              = 1;
    m_GridDisplayMode            = MV_GRID_INTERPOLATED;
    m_WarningMessage[0]          = '\0';

    // pointers to int arrays
    m_ColorBarLabelPrecision     = 0;
    m_DoSolidThreshold           = 0;
    m_NumColorBarLabels          = 0;
    m_NumberOfColorBands         = 0;
    m_NumberOfCustomIsosurfaces  = 0;
    m_NumberOfRegularIsosurfaces = 0;
    m_SolidDisplayMode           = 0;
    m_UseLogColorBar             = 0;
    m_UseRegularIsosurface       = 0;

    // pointers to double arrays
    m_ColorBarValueBlue          = 0;
    m_ColorBarValueRed           = 0;
    m_RegularIsosurfaceMin       = 0;
    m_RegularIsosurfaceMax       = 0;
    m_ScaledVectorArray          = 0;
    m_SolidThresholdMax          = 0;
    m_SolidThresholdMin          = 0;
    m_VectorMagnitudeArray       = 0;
    m_VectorLogMagnitudeArray    = 0;
    m_CustomIsosurfaceValues     = 0;

    // Data source will be created after model is loaded
    m_DataSource                 = 0;

    // Paths Data Set
    m_PathlineDataSet            = vtkSmartPointer<vtkPolyData>::New();
    m_PathlinePoints             = vtkSmartPointer<vtkPoints>::New();
    m_PathlineLines              = vtkSmartPointer<vtkCellArray>::New();
    m_PathlineScalars            = vtkSmartPointer<vtkDoubleArray>::New();
    m_PathlineScalars->SetNumberOfComponents(1);
    m_PathlineDataSet->SetPoints(m_PathlinePoints);
    m_PathlineDataSet->SetLines(m_PathlineLines);
    m_PathlineDataSet->GetPointData()->SetScalars(m_PathlineScalars);

    // Filter to extract a sub grid from a structured grid
    m_ExtractStructuredGridForSubgrid = vtkSmartPointer<vtkExtractGrid>::New();

    // Filter to extract a portion of a blocky grid
    m_ExtractCellsForSubgrid          = vtkSmartPointer<vtkExtractCells>::New();

    // The data set consisting of active cells only.
    m_ActiveScalarDataSet             = vtkSmartPointer<vtkThreshold>::New();
    m_ActiveScalarDataSet->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, vtkDataSetAttributes::SCALARS);
    m_ActiveScalarDataSetGeometry = vtkSmartPointer<vtkGeometryFilter>::New();
    m_ActiveScalarDataSetGeometry->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());

    // The outer shell of the active scalar data set
    m_GridShell = new mvGridShell;
    m_GridShell->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());

    // Filter to create color bands
    m_ColorBandFilter = vtkSmartPointer<vtkBandedPolyDataContourFilter>::New();
    m_ColorBandFilter->SetScalarModeToValue();

    // The filter to create isosurfaces.
    m_Isosurface = vtkSmartPointer<vtkContourFilter>::New();
    m_Isosurface->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());

    // Lookup tables
    m_LutBlueToRed = mvColorTable::New();
    dynamic_cast<mvColorTable *>(m_LutBlueToRed)->SetDefaultColorScheme();
    m_LutRedToBlue = mvColorTable::New();
    dynamic_cast<mvColorTable *>(m_LutRedToBlue)->SetReversedDefaultColorScheme();

    m_LutModifiedBlueToRed = mvColorTable::New();
    dynamic_cast<mvColorTable *>(m_LutModifiedBlueToRed)->SetModifiedColorScheme();
    m_LutModifiedRedToBlue = mvColorTable::New();
    dynamic_cast<mvColorTable *>(m_LutModifiedRedToBlue)->SetReversedModifiedColorScheme();

    m_LutCustomScale = mvColorTable::New();
    dynamic_cast<mvColorTable *>(m_LutCustomScale)->SetFirstCustomColor(0x5A5A5A);
    dynamic_cast<mvColorTable *>(m_LutCustomScale)->SetLastCustomColor(0xf0f0f0);
    dynamic_cast<mvColorTable *>(m_LutCustomScale)->SetCustomColorScheme();

    m_LutReversedCustomScale = mvColorTable::New();
    dynamic_cast<mvColorTable *>(m_LutReversedCustomScale)->SetFirstCustomColor(0x5A5A5a);
    dynamic_cast<mvColorTable *>(m_LutReversedCustomScale)->SetLastCustomColor(0xf0f0f0);
    dynamic_cast<mvColorTable *>(m_LutReversedCustomScale)->SetReversedCustomColorScheme();

    // Log Lookup tables
    // Despite appearances to the contrary, the reversed color scheme is correct for
    // m_LogLutBlueToRed and the unreversed color scheme is correct for m_LogLutRedToBlue
    m_LogLutBlueToRed = mvLogColorTable::New();
    dynamic_cast<mvLogColorTable *>(m_LogLutBlueToRed)->SetReversedDefaultColorScheme();
    m_LogLutRedToBlue = mvLogColorTable::New();
    dynamic_cast<mvLogColorTable *>(m_LogLutRedToBlue)->SetDefaultColorScheme();

    m_LogLutModifiedBlueToRed = mvLogColorTable::New();
    dynamic_cast<mvLogColorTable *>(m_LogLutModifiedBlueToRed)->SetModifiedColorScheme();
    m_LogLutModifiedRedToBlue = mvLogColorTable::New();
    dynamic_cast<mvLogColorTable *>(m_LogLutModifiedRedToBlue)->SetReversedModifiedColorScheme();

    m_LogLutCustomScale = mvLogColorTable::New();
    dynamic_cast<mvLogColorTable *>(m_LogLutCustomScale)->SetFirstCustomColor(0x5A5A5a);
    dynamic_cast<mvLogColorTable *>(m_LogLutCustomScale)->SetLastCustomColor(0xf0f0f0);
    dynamic_cast<mvLogColorTable *>(m_LogLutCustomScale)->SetCustomColorScheme();

    m_LogLutReversedCustomScale = mvLogColorTable::New();
    dynamic_cast<mvLogColorTable *>(m_LogLutReversedCustomScale)->SetFirstCustomColor(0x5A5A5a);
    dynamic_cast<mvLogColorTable *>(m_LogLutReversedCustomScale)->SetLastCustomColor(0xf0f0f0);
    dynamic_cast<mvLogColorTable *>(m_LogLutReversedCustomScale)->SetReversedCustomColorScheme();

    // Solid mapper and actor. Note that input to the solid mapper is
    // determined by the method BuildPipelineForSolid.
    m_SolidMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_SolidMapper->SetLookupTable(m_LutBlueToRed);
    m_SolidActor = vtkSmartPointer<vtkActor>::New();
    m_SolidActor->SetMapper(m_SolidMapper);
    m_SolidActor->VisibilityOff();

    // Isosurface Mapper and Actor. Note that the input to the isosurface
    // mapper is determined by the method BuildPipelineForIsosurface
    m_IsosurfaceMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_IsosurfaceMapper->SetLookupTable(m_LutBlueToRed);
    m_IsosurfaceActor = vtkSmartPointer<vtkActor>::New();
    m_IsosurfaceActor->SetMapper(m_IsosurfaceMapper);
    m_IsosurfaceActor->VisibilityOff();

    // Cropped away pieces mapper and actor
    m_CroppedAwayPieces       = mvCustomAppendPolyData::New();
    m_CroppedAwayPiecesMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_CroppedAwayPiecesMapper->SetInputConnection(m_CroppedAwayPieces->GetOutputPort());
    m_CroppedAwayPiecesMapper->ScalarVisibilityOff();
    m_CroppedAwayPiecesActor = vtkSmartPointer<vtkActor>::New();
    m_CroppedAwayPiecesActor->SetMapper(m_CroppedAwayPiecesMapper);
    m_CroppedAwayPiecesActor->GetProperty()->SetColor(1, .8, .6);
    m_CroppedAwayPiecesActor->GetProperty()->SetOpacity(.2);
    m_CroppedAwayPiecesActor->VisibilityOff();

    // Pathlines
    m_Pathlines = new mvPathlines;
    m_Pathlines->SetInputData(m_PathlineDataSet);

    // Flow or velocity vectors
    m_ExtractStructuredGridVector = vtkSmartPointer<vtkExtractGrid>::New();
    //	m_ExtractUnstructuredGridVector = vtkSmartPointer<vtkMaskPoints>::New();
    m_CropVectors                 = vtkSmartPointer<vtkExtractGeometry>::New();
    m_CropVectors->ExtractInsideOff();
    m_CropBoxForVectors = mvClipBox::New();
    m_CropVectors->SetImplicitFunction(m_CropBoxForVectors);

    m_ActiveVectorDataSet = vtkSmartPointer<vtkThresholdPoints>::New();
    // input connection for m_ActiveVectorDataSet will be set after model is loaded
    m_ActiveVectorDataSet->ThresholdByUpper(-1);
    m_VectorThreshold = vtkSmartPointer<vtkThresholdPoints>::New();
    m_VectorThreshold->SetInputConnection(m_ActiveVectorDataSet->GetOutputPort());
    m_HedgeHog = vtkSmartPointer<vtkHedgeHog>::New();
    m_HedgeHog->SetInputConnection(m_ActiveVectorDataSet->GetOutputPort());
    m_VectorMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_VectorMapper->ScalarVisibilityOff();
    m_VectorMapper->SetInputConnection(m_HedgeHog->GetOutputPort());
    m_VectorActor = vtkSmartPointer<vtkActor>::New();
    m_VectorActor->SetMapper(m_VectorMapper);
    m_VectorActor->GetProperty()->SetAmbient(1.0);
    m_VectorActor->GetProperty()->SetDiffuse(0.0);
    m_VectorActor->GetProperty()->SetColor(0, 0, 0);
    m_VectorActor->VisibilityOff();

    // Vector Base
    m_CubeSource  = vtkSmartPointer<vtkCubeSource>::New();
    m_VectorGlyph = vtkSmartPointer<vtkGlyph3D>::New();
    m_VectorGlyph->SetSourceConnection(m_CubeSource->GetOutputPort());
    m_VectorGlyph->SetInputConnection(m_ActiveVectorDataSet->GetOutputPort());
    m_VectorGlyph->ScalingOff();
    m_VectorGlyph->OrientOff();
    m_VectorGlyphMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_VectorGlyphMapper->SetInputConnection(m_VectorGlyph->GetOutputPort());
    m_VectorGlyphMapper->ScalarVisibilityOff();
    m_VectorGlyphActor = vtkSmartPointer<vtkActor>::New();
    m_VectorGlyphActor->SetMapper(m_VectorGlyphMapper);
    m_VectorGlyphActor->GetProperty()->SetColor(0, 0, 0);
    m_VectorGlyphActor->VisibilityOff();

    // Vector crop
    m_VectorClippingXMin  = 0;
    m_VectorClippingXMax  = 1;
    m_VectorClippingYMin  = 0;
    m_VectorClippingYMax  = 1;
    m_VectorClippingZMin  = 0;
    m_VectorClippingZMax  = 1;
    m_VectorClippingAngle = 0;

    // Grid Lines
    for (int i = 0; i < 3; i++)
    {
        m_GridLines[i]          = new mvGridLines;
        // input data for m_GridLines[i] will be set after model is loaded
        m_GridLinesActivated[i] = 1;
    }
    m_ActivatedGridLinesVisibility = 0;

    // GridLayer
    m_GridLayer                    = new mvGridLayer;

    // Bounding box
    m_BoundingBox                  = new mvBoundingBox;

    // Time Label
    m_TimeLabel                    = new mvDisplayText;

    // Title
    m_Title                        = new mvDisplayText;

    // Color Bar
    m_ColorBar                     = mvColorBar::New();
    m_ColorBar->SetLookupTable(m_LutBlueToRed);

    // Axes
    m_Axes          = new mvAxes;

    // ModelFeatures
    m_ModelFeatures = new mvModelFeatures;

    // Overlay
    m_Overlay       = new mvOverlay;
    // The SetFullGrid method of m_Overlay will be invoked after model is loaded
    m_Overlay->SetSubgrid(m_ExtractStructuredGridForSubgrid->GetOutput());

    // All the actors and actor2D's
    m_PropCollection = vtkSmartPointer<vtkPropCollection>::New();
    m_PropCollection->AddItem(m_SolidActor);
    m_PropCollection->AddItem(m_IsosurfaceActor);
    m_PropCollection->AddItem(m_VectorActor);
    m_PropCollection->AddItem(m_VectorGlyphActor);
    m_PropCollection->AddItem(m_Pathlines->GetActor());
    m_PropCollection->AddItem(m_GridLines[0]->GetActor());
    m_PropCollection->AddItem(m_GridLines[1]->GetActor());
    m_PropCollection->AddItem(m_GridLines[2]->GetActor());
    m_PropCollection->AddItem(m_ModelFeatures->GetActor());
    m_PropCollection->AddItem(m_BoundingBox->GetActor());
    m_PropCollection->AddItem(m_Axes->GetActor());
    m_PropCollection->AddItem(m_CroppedAwayPiecesActor);
    m_PropCollection->AddItem(m_GridShell->GetActor());
    m_PropCollection->AddItem(m_TimeLabel->GetActor2D());
    m_PropCollection->AddItem(m_Title->GetActor2D());
    m_PropCollection->AddItem(m_ColorBar);
    m_PropCollection->AddItem(m_Overlay->GetActor());

    m_PropCollection->AddItem(m_GridLayer->GetActor());

    // Smooth solid. A series of filters to create the solid after
    // values outside the min-max range are clipped. The Smooth Solid
    // consists of the clipped grid shell (values outside the min-max
    // range are removed) and the 2 isosurfaces corresponding to the
    // min and max values.
    m_GridShellClipMin = vtkSmartPointer<vtkClipPolyData>::New();
    m_GridShellClipMin->SetInputConnection(m_GridShell->GetOutputPort());
    m_GridShellClipMax = vtkSmartPointer<vtkClipPolyData>::New();
    m_GridShellClipMax->SetInputConnection(m_GridShellClipMin->GetOutputPort());
    m_GridShellClipMax->InsideOutOn();
    m_SmoothSolidIsosurface = vtkSmartPointer<vtkContourFilter>::New();
    m_SmoothSolidIsosurface->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());
    m_SmoothSolid = mvCustomAppendPolyData::New();
    m_SmoothSolid->AddInputConnection(m_GridShellClipMax->GetOutputPort());
    m_SmoothSolid->AddInputConnection(m_SmoothSolidIsosurface->GetOutputPort());

    // Blocky Solid. Consists of entire cells whose scalar values are
    // within the min-max range. Cells having scalar values outside the
    // range are removed.
    m_BlockySolidThreshold = vtkSmartPointer<vtkThreshold>::New();
    m_BlockySolidThreshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, vtkDataSetAttributes::SCALARS);
    m_BlockySolid = vtkSmartPointer<vtkGeometryFilter>::New();
    m_BlockySolid->SetInputConnection(m_BlockySolidThreshold->GetOutputPort());

    // The cropping planes. 2 are normal to x, at min-max positions.
    // Similar for y and z.
    for (int i = 0; i < 6; i++)
    {
        m_Plane[i] = vtkSmartPointer<vtkPlane>::New();
    }
    m_Plane[0]->SetNormal(1, 0, 0);
    m_Plane[1]->SetNormal(-1, 0, 0);
    m_Plane[2]->SetNormal(0, 1, 0);
    m_Plane[3]->SetNormal(0, -1, 0);
    m_Plane[4]->SetNormal(0, 0, 1);
    m_Plane[5]->SetNormal(0, 0, -1);

    // The cropper is a series of filters to crop either solid or the
    // isosurfaces. Cropping means removal of portions that lie beyond
    // cropping planes, which are normal to the x, y and z directions.
    for (int i = 0; i < 6; i++)
    {
        m_Cropper[i] = vtkSmartPointer<vtkClipPolyData>::New();
        m_Cropper[i]->SetClipFunction(m_Plane[i]);
        m_Cropper[i]->GenerateClippedOutputOn();
    }

    // Cropped Solid. The cropped solid is either the smooth or blocky
    // solid that has been cropped.
    m_CroppedSolid = mvCustomAppendPolyData::New();

    // Filters to extract "faces" corresponding to the cropping planes.
    for (int i = 0; i < 6; i++)
    {
        m_ExtractFace[i] = vtkSmartPointer<vtkCutter>::New();
        m_ExtractFace[i]->SetCutFunction(m_Plane[i]);
        m_ExtractFace[i]->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());
    }

    // Filters to crop the extracted faces. Each extracted face must be
    // cropped 4 times. For example, the face extracted on a plane normal
    // to x must be cropped in the y and z extents at the min and max positions.
    for (int i = 0; i < 24; i++)
    {
        m_FaceCrop[i] = vtkSmartPointer<vtkClipPolyData>::New();
        m_FaceCrop[i]->SetClipFunction(m_Plane[i / 4]);
    }

    // The cropped faces are appended into a single data set (m_Faces) for
    // clipping.
    m_Faces        = mvCustomAppendPolyData::New();

    // If smooth solid is displayed, and solid thresholding is turned on,
    // then the extracted faces must be clipped (values outside the min-max
    // range removed).
    m_FacesClipMin = vtkSmartPointer<vtkClipPolyData>::New();
    m_FacesClipMax = vtkSmartPointer<vtkClipPolyData>::New();
    m_FacesClipMax->SetInputConnection(m_FacesClipMin->GetOutputPort());
    m_FacesClipMax->InsideOutOn();

    // If blocky solid is displayed, and solid thresholding is turned on,
    // we keep only those face cells within the max-min range.
    m_FacesThreshold = vtkSmartPointer<vtkThreshold>::New();
    m_FacesThreshold->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, vtkDataSetAttributes::SCALARS);
    m_FacesThresholdGeometry = vtkSmartPointer<vtkGeometryFilter>::New();
    m_FacesThresholdGeometry->SetInputConnection(m_FacesThreshold->GetOutputPort());

    for (int i = 0; i < 3; i++)
    {
        m_IsosurfaceCutter[i] = vtkSmartPointer<vtkCutter>::New();
    }

    // SetImmediateModeRendering(1);
}

mvManager::~mvManager()
{
    int i;
    if (m_DataSource != 0)
    {
        delete m_DataSource;
    }
    m_CroppedAwayPieces->Delete();
    m_LutBlueToRed->Delete();
    m_LutRedToBlue->Delete();
    m_LutModifiedRedToBlue->Delete();
    m_LutModifiedBlueToRed->Delete();
    m_LutCustomScale->Delete();
    m_LutReversedCustomScale->Delete();
    m_LogLutBlueToRed->Delete();
    m_LogLutRedToBlue->Delete();
    m_LogLutModifiedRedToBlue->Delete();
    m_LogLutModifiedBlueToRed->Delete();
    m_LogLutCustomScale->Delete();
    m_LogLutReversedCustomScale->Delete();

    m_SmoothSolid->Delete();
    m_CroppedSolid->Delete();
    m_Faces->Delete();
    m_CropBoxForVectors->Delete();
    m_ColorBar->Delete();

    // mv display objects
    delete m_Pathlines;
    for (i = 0; i < 3; i++)
    {
        delete m_GridLines[i];
    }
    delete m_GridLayer;
    delete m_GridShell;
    delete m_BoundingBox;
    delete m_Axes;
    delete m_TimeLabel;
    delete m_Title;

    delete m_ModelFeatures;

    delete m_Overlay;

    if (m_ColorBarValueBlue)
    {
        delete[] m_ColorBarValueBlue;
        m_ColorBarValueBlue = 0;
    }
    if (m_ColorBarValueRed)
    {
        delete[] m_ColorBarValueRed;
        m_ColorBarValueRed = 0;
    }
    if (m_UseLogColorBar)
    {
        delete[] m_UseLogColorBar;
        m_UseLogColorBar = 0;
    }
    if (m_NumColorBarLabels)
    {
        delete[] m_NumColorBarLabels;
        m_NumColorBarLabels = 0;
    }
    if (m_ColorBarLabelPrecision)
    {
        delete[] m_ColorBarLabelPrecision;
        m_ColorBarLabelPrecision = 0;
    }
    if (m_SolidDisplayMode)
    {
        delete[] m_SolidDisplayMode;
        m_SolidDisplayMode = 0;
    }
    if (m_DoSolidThreshold)
    {
        delete[] m_DoSolidThreshold;
        m_DoSolidThreshold = 0;
    }
    if (m_SolidThresholdMax)
    {
        delete[] m_SolidThresholdMax;
        m_SolidThresholdMax = 0;
    }
    if (m_SolidThresholdMin)
    {
        delete[] m_SolidThresholdMin;
        m_SolidThresholdMin = 0;
    }
    if (m_NumberOfColorBands)
    {
        delete[] m_NumberOfColorBands;
        m_NumberOfColorBands = 0;
    }
    if (m_UseRegularIsosurface)
    {
        delete[] m_UseRegularIsosurface;
        m_UseRegularIsosurface = 0;
    }
    if (m_NumberOfRegularIsosurfaces)
    {
        delete[] m_NumberOfRegularIsosurfaces;
        m_NumberOfRegularIsosurfaces = 0;
    }
    if (m_RegularIsosurfaceMax)
    {
        delete[] m_RegularIsosurfaceMax;
        m_RegularIsosurfaceMax = 0;
    }
    if (m_RegularIsosurfaceMin)
    {
        delete[] m_RegularIsosurfaceMin;
        m_RegularIsosurfaceMin = 0;
    }
    if (m_NumberOfCustomIsosurfaces)
    {
        delete[] m_NumberOfCustomIsosurfaces;
        m_NumberOfCustomIsosurfaces = 0;
    }
    if (m_CustomIsosurfaceValues)
    {
        for (int i = 0; i < m_NumScalarDataTypes; i++)
        {
            if (m_CustomIsosurfaceValues != 0)
            {
                delete[] m_CustomIsosurfaceValues[i];
            }
        }
        delete[] m_CustomIsosurfaceValues;
        m_CustomIsosurfaceValues = 0;
    }
    if (m_ScaledVectorArray != 0)
    {
        delete[] m_ScaledVectorArray;
        m_ScaledVectorArray = 0;
    }
    if (m_VectorMagnitudeArray != 0)
    {
        delete[] m_VectorMagnitudeArray;
        m_VectorMagnitudeArray = 0;
    }
    if (m_VectorLogMagnitudeArray != 0)
    {
        delete[] m_VectorLogMagnitudeArray;
        m_VectorLogMagnitudeArray = 0;
    }
}

const char *mvManager::LoadData(char *modelName, char *dataFileList)
{
    // Reading data from a file is done in "Deserialize".

    mvDataSource *newDataSource = mvModelList::CreateDataSource(modelName);
    if (newDataSource == 0)
    {
        return "Model is not supported by this program";
    }

    m_WarningMessage[0] = '\0';

    const char *errMsg  = newDataSource->LoadData(dataFileList);
    if (errMsg != nullptr)
    {
        delete newDataSource;
        return errMsg;
    }

    // Data files are loaded OK. We keep the new data source object
    m_DataSource             = newDataSource;

    int numVTKPoints         = m_DataSource->GetNumVTKPoints();
    int numVTKCells          = m_DataSource->GetNumVTKCells();

    // Create Scalar Data Set
    m_InterpolatedGridPoints = vtkSmartPointer<vtkPoints>::New();
    m_StairsteppedGridPoints = vtkSmartPointer<vtkPoints>::New();
    m_PointScalars           = vtkSmartPointer<vtkDoubleArray>::New();
    m_PointScalars->SetNumberOfComponents(1);
    m_CellScalars = vtkSmartPointer<vtkDoubleArray>::New();
    m_CellScalars->SetNumberOfComponents(1);
    if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        // DIS grid - interpolated
        m_ScalarLayeredGrid      = 0;
        m_ScalarUnstructuredGrid = 0;
        m_ScalarStructuredGrid   = vtkSmartPointer<vtkStructuredGrid>::New();
        m_ScalarStructuredGrid->SetPoints(m_InterpolatedGridPoints);
        m_ScalarStructuredGrid->GetPointData()->SetScalars(m_PointScalars);
        m_ScalarStructuredGrid->GetCellData()->SetScalars(m_CellScalars);
        m_ActiveScalarDataSet->SetInputData(m_ScalarStructuredGrid);
        m_GridDisplayMode = MV_GRID_INTERPOLATED;

        m_ExtractStructuredGridForSubgrid->SetInputData(m_ScalarStructuredGrid);
        m_BlockySolidThreshold->SetInputData(m_ScalarStructuredGrid);
        const int *sdim = m_DataSource->GetScalarGridDimensions();
        m_ScalarStructuredGrid->SetDimensions(sdim[0], sdim[1], sdim[2]);
        for (int i = 0; i < 3; i++)
        {
            m_GridLines[i]->SetInputData(m_ScalarStructuredGrid);
        }
        m_Overlay->SetFullGrid(m_ScalarStructuredGrid);

        // DIS grid - stairstepped
        m_StairsteppedGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        m_StairsteppedGrid->SetPoints(m_StairsteppedGridPoints);
        m_StairsteppedGrid->GetCellData()->SetScalars(m_CellScalars);

        int                           i;
        int                           vtk_cell_index = 0;
        int                           j              = 0;
        int                          *cellType       = new int[numVTKCells];
        vtkSmartPointer<vtkCellArray> cellArray      = vtkSmartPointer<vtkCellArray>::New();
        for (i = 0; i < numVTKCells; i++)
        {
            cellType[vtk_cell_index]            = VTK_HEXAHEDRON;
            vtkSmartPointer<vtkHexahedron> cell = vtkSmartPointer<vtkHexahedron>::New();
            cell->GetPointIds()->SetId(3, j++);
            cell->GetPointIds()->SetId(2, j++);
            cell->GetPointIds()->SetId(1, j++);
            cell->GetPointIds()->SetId(0, j++);
            cell->GetPointIds()->SetId(7, j++);
            cell->GetPointIds()->SetId(6, j++);
            cell->GetPointIds()->SetId(5, j++);
            cell->GetPointIds()->SetId(4, j++);
            cellArray->InsertNextCell(cell);
            vtk_cell_index++;
        }
        m_StairsteppedGrid->SetCells(cellType, cellArray);
        delete[] cellType;
    }
    else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        // DISV grid - interpolated
        m_ScalarStructuredGrid   = 0;
        m_ScalarUnstructuredGrid = 0;
        m_ScalarLayeredGrid      = vtkSmartPointer<vtkUnstructuredGrid>::New();
        m_ScalarLayeredGrid->SetPoints(m_InterpolatedGridPoints);
        m_ScalarLayeredGrid->GetPointData()->SetScalars(m_PointScalars);
        m_ScalarLayeredGrid->GetCellData()->SetScalars(m_CellScalars);
        m_ActiveScalarDataSet->SetInputData(m_ScalarLayeredGrid);
        m_GridDisplayMode = MV_GRID_INTERPOLATED;

        // fileter to extract layers
        m_ExtractCellsForSubgrid->SetInputData(m_ScalarLayeredGrid);

        int                          *connectivity   = m_DataSource->GetConnectivityArray();
        vtkSmartPointer<vtkCellArray> cellArray      = vtkSmartPointer<vtkCellArray>::New();
        int                          *cellType       = new int[numVTKCells];
        int                           k              = 0;
        int                           vtk_cell_index = 0;
        for (int i = 0; i < numVTKCells; i++)
        {
            vtkCell3D *cell;
            int        nvert = connectivity[k++];
            if (nvert > 0) // vtk cells are built only for modflow cells with idomain > 0.
            {
                switch (nvert)
                {
                case 6:
                    cellType[vtk_cell_index++] = VTK_WEDGE;
                    cell                       = vtkWedge::New();
                    break;
                case 8:
                    cellType[vtk_cell_index++] = VTK_HEXAHEDRON;
                    cell                       = vtkHexahedron::New();
                    break;
                case 10:
                    cellType[vtk_cell_index++] = VTK_PENTAGONAL_PRISM;
                    cell                       = vtkPentagonalPrism::New();
                    break;
                case 12:
                    cellType[vtk_cell_index++] = VTK_HEXAGONAL_PRISM;
                    cell                       = vtkHexagonalPrism::New();
                }
                for (int j = 0; j < nvert; j++)
                {
                    cell->GetPointIds()->SetId(j, connectivity[k++]);
                }
                cellArray->InsertNextCell(cell);
                cell->Delete();
            }
        }
        m_ScalarLayeredGrid->SetCells(cellType, cellArray);
        // Note the data in cellType are copied by m_ScalarUnstructuredGrid.
        // Because cellType is not referenced inside m_ScalarLayeredGrid, cellType can be deleted.
        delete[] cellType;

        // Grid Layer lines
        m_BlockySolidThreshold->SetInputData(m_ScalarLayeredGrid);
        m_GridLayer->SetDataSource(m_DataSource);
        m_GridLayer->Build();
        m_GridLayer->SetLayerPosition(0);

        // DISV grid -- staristepped
        m_StairsteppedGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        m_StairsteppedGrid->SetPoints(m_StairsteppedGridPoints);
        m_StairsteppedGrid->GetCellData()->SetScalars(m_CellScalars);

        connectivity   = m_DataSource->GetStairsteppedGridConnectivity();
        cellArray      = vtkSmartPointer<vtkCellArray>::New();
        cellType       = new int[numVTKCells];

        vtk_cell_index = 0;
        k              = 0;
        for (int i = 0; i < numVTKCells; i++)
        {
            vtkCell3D *cell;
            int        nvert = connectivity[k++];
            if (nvert > 0)
            {
                switch (nvert)
                {
                case 6:
                    cellType[vtk_cell_index++] = VTK_WEDGE;
                    cell                       = vtkWedge::New();
                    break;
                case 8:
                    cellType[vtk_cell_index++] = VTK_HEXAHEDRON;
                    cell                       = vtkHexahedron::New();
                    break;
                case 10:
                    cellType[vtk_cell_index++] = VTK_PENTAGONAL_PRISM;
                    cell                       = vtkPentagonalPrism::New();
                    break;
                case 12:
                    cellType[vtk_cell_index++] = VTK_HEXAGONAL_PRISM;
                    cell                       = vtkHexagonalPrism::New();
                }
                for (int j = 0; j < nvert; j++)
                {
                    cell->GetPointIds()->SetId(j, connectivity[k++]);
                }
                cellArray->InsertNextCell(cell);
                cell->Delete();
            }
        }
        m_StairsteppedGrid->SetCells(cellType, cellArray);
        delete[] cellType;
    }
    else if (m_DataSource->GetGridType() == GridType::MV_UNSTRUCTURED_GRID)
    {
        // DISU grid
        m_ScalarStructuredGrid   = 0;
        m_ScalarLayeredGrid      = 0;
        m_ScalarUnstructuredGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        m_ScalarUnstructuredGrid->SetPoints(m_StairsteppedGridPoints);
        m_ScalarUnstructuredGrid->GetPointData()->SetScalars(m_PointScalars);
        m_ScalarUnstructuredGrid->GetCellData()->SetScalars(m_CellScalars);
        m_ActiveScalarDataSet->SetInputData(m_ScalarUnstructuredGrid);
        m_GridDisplayMode = MV_GRID_STAIRSTEPPED;

        m_BlockySolidThreshold->SetInputData(m_ScalarUnstructuredGrid);
        // no subgrid capability for unstructured grid

        int                          *connectivity   = m_DataSource->GetStairsteppedGridConnectivity();
        vtkSmartPointer<vtkCellArray> cellArray      = vtkSmartPointer<vtkCellArray>::New();
        int                          *cellType       = new int[numVTKCells];
        int                           k              = 0;
        int                           vtk_cell_index = 0;
        for (int i = 0; i < numVTKCells; i++)
        {
            vtkCell3D *cell;
            int        nvert = connectivity[k++];
            switch (nvert)
            {
            case 6:
                cellType[vtk_cell_index++] = VTK_WEDGE;
                cell                       = vtkWedge::New();
                break;
            case 8:
                cellType[vtk_cell_index++] = VTK_HEXAHEDRON;
                cell                       = vtkHexahedron::New();
                break;
            case 10:
                cellType[vtk_cell_index++] = VTK_PENTAGONAL_PRISM;
                cell                       = vtkPentagonalPrism::New();
                break;
            case 12:
                cellType[vtk_cell_index++] = VTK_HEXAGONAL_PRISM;
                cell                       = vtkHexagonalPrism::New();
            }
            for (int j = 0; j < nvert; j++)
            {
                cell->GetPointIds()->SetId(j, connectivity[k++]);
            }
            cellArray->InsertNextCell(cell);
            cell->Delete();
        }
        m_ScalarUnstructuredGrid->SetCells(cellType, cellArray);
        // Note the data in cellType are copied by m_ScalarUnstructuredGrid.
        // Because cellType is not referenced inside m_ScalarUnstructuredGrid, cellType can be deleted.
        delete[] cellType;
    }

    // Vector Data Set
    m_VectorGridPoints = vtkSmartPointer<vtkPoints>::New();
    m_Vectors          = vtkSmartPointer<vtkDoubleArray>::New();
    m_Vectors->SetNumberOfComponents(3);
    m_VectorMagnitudes = vtkSmartPointer<vtkDoubleArray>::New();
    m_VectorMagnitudes->SetNumberOfComponents(1);
    if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        m_VectorUnstructuredGrid = 0;
        m_VectorDataSet          = vtkSmartPointer<vtkStructuredGrid>::New();
        m_VectorDataSet->SetPoints(m_VectorGridPoints);
        m_VectorDataSet->GetPointData()->SetVectors(m_Vectors);
        m_VectorDataSet->GetPointData()->SetScalars(m_VectorMagnitudes);
        m_ExtractStructuredGridVector->SetInputData(m_VectorDataSet);
        m_ActiveVectorDataSet->SetInputConnection(m_ExtractStructuredGridVector->GetOutputPort());
    }
    else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        m_VectorDataSet          = 0;
        m_VectorUnstructuredGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
        m_VectorUnstructuredGrid->SetPoints(m_VectorGridPoints);
        m_VectorUnstructuredGrid->GetPointData()->SetVectors(m_Vectors);
        m_VectorUnstructuredGrid->GetPointData()->SetScalars(m_VectorMagnitudes);
        //		m_ExtractUnstructuredGridVector->SetInputData(m_VectorUnstructuredGrid);
        //		m_ActiveVectorDataSet->SetInputConnection(m_ExtractUnstructuredGridVector->GetOutputPort());
        m_ActiveVectorDataSet->SetInputData(m_VectorUnstructuredGrid);
    }
    else
    {
    }

    m_Pathlines->SetMinPositiveValue(m_DataSource->GetMinPositiveTime());

    // Set the point coordinates
    vtkSmartPointer<vtkDoubleArray> doubleArray = vtkSmartPointer<vtkDoubleArray>::New();
    doubleArray->SetNumberOfComponents(3);
    doubleArray->SetArray(m_DataSource->GetInterpolatedGridCoordinates(), 3 * numVTKPoints, 1);
    m_InterpolatedGridPoints->SetDataTypeToDouble();
    m_InterpolatedGridPoints->SetData(doubleArray);

    if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        vtkSmartPointer<vtkDoubleArray> doubleArray = vtkSmartPointer<vtkDoubleArray>::New();
        doubleArray->SetNumberOfComponents(3);
        doubleArray->SetArray(m_DataSource->GetStairsteppedGridCoordinates(), 24 * numVTKCells, 1);
        m_StairsteppedGridPoints->SetDataTypeToDouble();
        m_StairsteppedGridPoints->SetData(doubleArray);
    }
    else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        vtkSmartPointer<vtkDoubleArray> doubleArray = vtkSmartPointer<vtkDoubleArray>::New();
        doubleArray->SetNumberOfComponents(3);
        int size = m_DataSource->GetStairsteppedGridCoordinatesSize();
        doubleArray->SetArray(m_DataSource->GetStairsteppedGridCoordinates(), size, 1);
        m_StairsteppedGridPoints->SetDataTypeToDouble();
        m_StairsteppedGridPoints->SetData(doubleArray);
    }
    else if (m_DataSource->GetGridType() == GridType::MV_UNSTRUCTURED_GRID)
    {
        vtkSmartPointer<vtkDoubleArray> doubleArray = vtkSmartPointer<vtkDoubleArray>::New();
        doubleArray->SetNumberOfComponents(3);
        doubleArray->SetArray(m_DataSource->GetStairsteppedGridCoordinates(), 3 * numVTKPoints, 1);
        m_StairsteppedGridPoints->SetDataTypeToDouble();
        m_StairsteppedGridPoints->SetData(doubleArray);
    }

    // Set Point Data
    m_PointScalars->SetArray(m_DataSource->GetScalarArray(), numVTKPoints, 1);
    m_PointScalars->Modified();

    // Set Cell data
    if (m_DataSource->GetPrimaryScalarMode() == ScalarMode::MV_CELL_SCALARS)
    {
        m_CellScalars->SetArray(m_DataSource->GetScalarArray() + numVTKPoints, numVTKCells, 1);
        m_CellScalars->Modified();
        m_ActiveScalarDataSet->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS, vtkDataSetAttributes::SCALARS);
        m_GridLines[0]->DoThresholdUsingCellData();
        m_GridLines[1]->DoThresholdUsingCellData();
        m_GridLines[2]->DoThresholdUsingCellData();
    }
    else
    {
        // if primary scalar mode is point scalars, then we don't need
        // cell scalars but we have to define them so vtk won't crash.
        // so we just use the point scalars defined earlier.
        m_CellScalars->SetArray(m_DataSource->GetScalarArray(), numVTKPoints, 1);
        m_CellScalars->Modified();
        m_ActiveScalarDataSet->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, vtkDataSetAttributes::SCALARS);
        m_GridLines[0]->DoThresholdUsingPointData();
        m_GridLines[1]->DoThresholdUsingPointData();
        m_GridLines[2]->DoThresholdUsingPointData();
    }
    ComputeActiveScalarRange();

    if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        m_ScalarStructuredGrid->Modified();
    }
    else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        m_ScalarLayeredGrid->Modified();
    }
    else if (m_DataSource->GetGridType() == GridType::MV_UNSTRUCTURED_GRID)
    {
        m_ScalarUnstructuredGrid->Modified();
    }

    // Define the cutoff value that indicates inactive cells.
    double cutoff = m_DataSource->GetInactiveCellValue() * 0.999;
#if ((VTK_MAJOR_VERSION == 9) && (VTK_MINOR_VERSION < 1) || (VTK_MAJOR_VERSION < 9))
    m_ActiveScalarDataSet->ThresholdByLower(cutoff);    // deprecated as of VTK 9.1
#else
    m_ActiveScalarDataSet->SetThresholdFunction(vtkThreshold::THRESHOLD_LOWER);
    m_ActiveScalarDataSet->SetLowerThreshold(cutoff);
#endif

    // Bounding box
    SetBoundingBoxBounds();
    //	if (m_DataSource->GetPrimaryScalarMode() == MV_CELL_SCALARS)
    //	{
    //		m_ActiveScalarDataSet->Update();
    //		m_ActiveScalarDataSet->GetOutput()->ComputeBounds();
    //		m_ActiveScalarDataSet->GetOutput()->GetBounds(bounds);
    //		m_BoundingBox->SetBounds(bounds);
    //	}
    //	else
    //	{
    //		m_ScalarStructuredGrid->ComputeBounds();
    //		m_ScalarStructuredGrid->GetBounds(bounds);
    //		m_BoundingBox->SetBounds(bounds);
    //	}

    // Grid lines
    m_GridLines[0]->SetThresholdMin(cutoff);
    m_GridLines[1]->SetThresholdMin(cutoff);
    m_GridLines[2]->SetThresholdMin(cutoff);

    // Grid Layer
    //   m_GridLayer->Build(m_DataSource->GetGridLayerPolyData());

    // default Axes
    double bounds[6];
    if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        m_ScalarStructuredGrid->ComputeBounds();
        m_ScalarStructuredGrid->GetBounds(bounds);
    }
    else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        m_ScalarLayeredGrid->ComputeBounds();
        m_ScalarLayeredGrid->GetBounds(bounds);
    }
    else if (m_DataSource->GetGridType() == GridType::MV_UNSTRUCTURED_GRID)
    {
        m_ScalarUnstructuredGrid->ComputeBounds();
        m_ScalarUnstructuredGrid->GetBounds(bounds);
    }
    double defaultAxesSize = (bounds[1] - bounds[0] + bounds[3] - bounds[2] + bounds[5] - bounds[4]) / 12;
    m_Axes->SetDefaultPositions(bounds);
    m_Axes->SetDefaultSize(defaultAxesSize);
    m_Axes->SetDefaultTubeDiameter(defaultAxesSize * 0.1);

    m_Pathlines->SetDefaultTubeDiameter(defaultAxesSize * 0.1);

    // Vector data
    if (m_DataSource->GetVectorArray() != 0)
    {
        int np;
        if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
        {
            const int *vdim = m_DataSource->GetVectorGridDimensions();
            m_VectorDataSet->SetDimensions(vdim[0], vdim[1], vdim[2]);
            np = vdim[0] * vdim[1] * vdim[2];
        }
        else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
        {
            np = m_DataSource->GetNumModelCells();
            // vtkCellArray *connectivity = vtkCellArray::New();
            // int *types = new int[numCells];
            // m_DataSource->BuildCellsForDisvGrid(types, connectivity,
            // m_VectorUnstructuredGrid->SetCells(types, connectivity);
            // delete [] types;
        }
        else
        {
        }

        // Set the point coordinates
        doubleArray = vtkDoubleArray::New();
        doubleArray->SetNumberOfComponents(3);
        doubleArray->SetArray(m_DataSource->GetVectorGridCoordinates(), 3 * np, 1);
        m_VectorGridPoints->SetData(doubleArray);
        doubleArray->Delete();

        // Set the scaled vector data
        m_ScaledVectorArray = new double[3 * np];
        m_Vectors->SetArray(m_ScaledVectorArray, 3 * np, 1);
        m_Vectors->Modified();

        // The scalar point data are the vector magnitudes. These will
        // be used for thresholding
        m_VectorMagnitudeArray    = new double[np];
        m_VectorLogMagnitudeArray = new double[np];
        m_VectorMagnitudes->SetArray(m_VectorMagnitudeArray, np, 1);
        m_VectorMagnitudes->Modified();
    }

    // Pathlines
    /*
     int np = m_DataSource->GetNumberOfPathlines();
     int nc = m_DataSource->GetNumberOfPathlineCoordinates();
     if (np > 0)
     {
         doubleArray = vtkDoubleArray::New();
         doubleArray->SetNumberOfComponents(3);
         doubleArray->SetArray(m_DataSource->GetPathlineCoordinates(), 3*nc, 1);
         m_PathlinePoints->SetData(doubleArray);
         doubleArray->Delete();

         UpdatePathlineScalars();

         double *range = m_PathlineScalars->GetValueRange();
         m_Pathlines->SetScalarRange(range[0], range[1]);

         vtkIdTypeArray *idTypeArray = vtkIdTypeArray::New();
         idTypeArray->SetNumberOfTuples(1);
         idTypeArray->SetArray(m_DataSource->GetPathlinePointArray(), np+nc, 1);
         m_PathlineLines->SetCells(np, idTypeArray);
         idTypeArray->Delete();
     }
    */

    m_ModelFeatures->SetNumberOfModelFeatureTypes(m_DataSource->GetNumberOfModelFeatureTypes());
    if (m_DataSource->GetModelFeatureArray())
    {
        double rgba[4];
        if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
        {
            m_ModelFeatures->SetGridTypeToStructuredGrid();
            m_ModelFeatures->SetFullGridDimensions(m_DataSource->GetScalarGridDimensions());
            m_ModelFeatures->SetGridPoints(m_InterpolatedGridPoints);
        }
        else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
        {
            m_ModelFeatures->SetGridTypeToUnstructuredGrid();
            // Send the cell (connectivity) data from m_ScalarLayeredGrid to m_ModelFeatures
            m_ModelFeatures->SetCellArrayForUnstructuredGrid(m_ScalarLayeredGrid->GetCellTypesArray(),
                                                             m_ScalarLayeredGrid->GetCellLocationsArray(),
                                                             m_ScalarLayeredGrid->GetCells());
            m_ModelFeatures->SetGridPoints(m_InterpolatedGridPoints);
        }
        else if (m_DataSource->GetGridType() == GridType::MV_UNSTRUCTURED_GRID)
        {
            m_ModelFeatures->SetGridTypeToUnstructuredGrid();
            // Send the cell (connectivity) data from m_ScalarUnstructuredGrid to m_ModelFeatures
            m_ModelFeatures->SetCellArrayForUnstructuredGrid(m_ScalarUnstructuredGrid->GetCellTypesArray(),
                                                             m_ScalarUnstructuredGrid->GetCellLocationsArray(),
                                                             m_ScalarUnstructuredGrid->GetCells());
            m_ModelFeatures->SetGridPoints(m_StairsteppedGridPoints);
        }
        m_ModelFeatures->SetDisplayMode(m_DataSource->GetModelFeatureDisplayMode());
        for (int i = 0; i < m_DataSource->GetNumberOfModelFeatureTypes(); i++)
        {
            m_DataSource->GetDefaultModelFeatureColor(i, rgba);
            m_ModelFeatures->SetColor(i, rgba);
        }
        m_ModelFeatures->SetModelFeatureArray(m_DataSource->GetModelFeatureArray());
        m_ModelFeatures->Build();

        double bounds[6];
        m_BoundingBox->GetBounds(bounds);
        double dx   = bounds[1] - bounds[0];
        double dy   = bounds[3] - bounds[2];
        double dz   = bounds[5] - bounds[4];
        double diag = sqrt(dx * dx + dy * dy + dz * dz);
        m_ModelFeatures->SetDefaultGlyphSize(diag / 300);
    }

    // if (m_DataSource->AreAllCellsActive())
    //{
    //  all cells active
    //	m_GridShell->SetInputData(m_ScalarStructuredGrid);
    //	m_Isosurface->SetInputData(m_ScalarStructuredGrid);
    //	m_SmoothSolidIsosurface->SetInputData(m_ScalarStructuredGrid);
    //	for (int i=0; i<6; i++)
    //	{
    //		m_ExtractFace[i]->SetInputData(m_ScalarStructuredGrid);
    //	}
    //   m_GridLines[0]->SetAllCellsAreActive(1);
    //   m_GridLines[1]->SetAllCellsAreActive(1);
    //   m_GridLines[2]->SetAllCellsAreActive(1);
    //}
    // else
    //{
    // not all cells are active
    m_GridShell->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());
    m_Isosurface->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());
    m_SmoothSolidIsosurface->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());
    for (int i = 0; i < 6; i++)
    {
        m_ExtractFace[i]->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());
    }
    //   m_GridLines[0]->SetAllCellsAreActive(0);
    //   m_GridLines[1]->SetAllCellsAreActive(0);
    //   m_GridLines[2]->SetAllCellsAreActive(0);
    //}

    // Create arrays to hold tool parameters for each scalar data type
    m_NumScalarDataTypes         = m_DataSource->GetNumberOfScalarDataTypes();
    m_ColorBarValueBlue          = new double[m_NumScalarDataTypes];
    m_ColorBarValueRed           = new double[m_NumScalarDataTypes];
    m_UseLogColorBar             = new int[m_NumScalarDataTypes];
    m_NumColorBarLabels          = new int[m_NumScalarDataTypes];
    m_ColorBarLabelPrecision     = new int[m_NumScalarDataTypes];
    m_SolidDisplayMode           = new SolidDisplayType[m_NumScalarDataTypes];
    m_DoSolidThreshold           = new int[m_NumScalarDataTypes];
    m_SolidThresholdMax          = new double[m_NumScalarDataTypes];
    m_SolidThresholdMin          = new double[m_NumScalarDataTypes];
    m_NumberOfColorBands         = new int[m_NumScalarDataTypes];
    m_UseRegularIsosurface       = new int[m_NumScalarDataTypes];
    m_NumberOfRegularIsosurfaces = new int[m_NumScalarDataTypes];
    m_RegularIsosurfaceMax       = new double[m_NumScalarDataTypes];
    m_RegularIsosurfaceMin       = new double[m_NumScalarDataTypes];
    m_NumberOfCustomIsosurfaces  = new int[m_NumScalarDataTypes];
    m_CustomIsosurfaceValues     = new double *[m_NumScalarDataTypes];

    // Assign default tool parameters
    for (int i = 0; i < m_NumScalarDataTypes; i++)
    {
        m_ColorBarValueBlue[i]      = 0;
        m_ColorBarValueRed[i]       = 1;
        m_UseLogColorBar[i]         = 0;
        m_NumColorBarLabels[i]      = 5;
        m_ColorBarLabelPrecision[i] = 3;
        if (m_DataSource->GetGridType() == GridType::MV_UNSTRUCTURED_GRID)
        {
            m_SolidDisplayMode[i] = SolidDisplayType::MV_SOLID_BLOCKY;
        }
        else
        {
            m_SolidDisplayMode[i] = SolidDisplayType::MV_SOLID_SMOOTH;
        }
        m_DoSolidThreshold[i]           = 0;
        m_SolidThresholdMax[i]          = 1;
        m_SolidThresholdMin[i]          = 0;
        m_NumberOfColorBands[i]         = 10;
        m_UseRegularIsosurface[i]       = 1;
        m_NumberOfRegularIsosurfaces[i] = 0;
        m_RegularIsosurfaceMax[i]       = 1;
        m_RegularIsosurfaceMin[i]       = 0;
        m_NumberOfCustomIsosurfaces[i]  = 0;
        m_CustomIsosurfaceValues[i]     = 0;
    }

    if (m_DataSource->GetGridType() == GridType::MV_UNSTRUCTURED_GRID)
    {
        SetSolidDisplayToBlocky();
    }

    return nullptr;
}

const mvDataSource *mvManager::GetDataSource() const
{
    return m_DataSource;
}

void mvManager::ApplyDefaultSettings()
{
    // Hide Everything
    HideScalarData();
    HideCroppedAwayPieces();
    HideVectors();
    ActivateVectorGlyph(0);
    SetLogTransformVector(0);
    HidePathlines();
    HideGridShell();
    HideGridLines();
    HideAxes();
    HideBoundingBox();
    HideModelFeatures();
    HideTimeLabel();
    HideTitle();
    HideColorBar();
    HideOverlay();

    // Color Bar
    SetColorBarWidth(20);
    SetColorBarHeight(200);
    SetColorBarOffset(100);
    SetColorBarFontSize(14);

    // Surface lighting
    SetDiffuseLighting(1);
    SetAmbientLighting(0);
    SetSpecularLighting(0);
    SetSpecularPower(1);

    // Grid Shell
    SetGridShellColor(1, 0.8, 0.6);
    SetGridShellOpacity(0.2);

    // Scale
    SetScale(1, 1, 1);

    // Axes
    SetAxesRepresentationToTube();
    SetAxesNormalizedSize(1);
    SetAxesNormalizedPosition(0, 0, 0);
    SetAxesNormalizedTubeDiameter(1);

    // Bounding Box
    SetBoundingBoxColor(0, 0, 0);

    // Crop
    Crop(0, 1, 0, 1, 0, 1, 0);
    SetCroppedAwayPiecesColor(1, 0.8, 0.6);
    SetCroppedAwayPiecesOpacity(0.2);

    // Subgrid
    if (GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        int *sdim = m_DataSource->GetScalarGridDimensions();
        SetScalarSubgridExtent(0, sdim[0] - 1, 0, sdim[1] - 1, 0, sdim[2] - 1);
        ScalarSubgridOff();
    }

    // Grid Lines, bounding box
    SetGridLineColor(0, 0, 0);
    if (m_DataSource->GetInitialGridDisplayPolicy() ==
        MV_INITIAL_DISPLAY_GRID_OUTLINE)
    {
        SetGridLinePositions(0, 0, 0);
        DeactivateGridLines(0);
        DeactivateGridLines(1);
        DeactivateGridLines(2);
    }
    else
    {
        if (m_DataSource->GetGridType() != GridType::MV_LAYERED_GRID)
        {
            int *sdim = m_DataSource->GetScalarGridDimensions();
            //		   SetGridLinePositions(sdim[0]/2, sdim[1]/2, sdim[2]/2);
            SetGridLinePositions(0, sdim[1] - 1, 0);
            ActivateGridLines(0);
            ActivateGridLines(1);
            ActivateGridLines(2);
        }
        else
        {
            // Modflow DISV grid
            SetGridLayerPosition(m_DataSource->GetNumberOfCellLayers());
        }
        ShowBoundingBox();
    }

    int temp = m_ActiveDataType;
    for (int i = 0; i < m_NumScalarDataTypes; i++)
    {
        SetScalarDataTypeTo(i);

        // Color Bar
        double range[2];
        GetScalarDataRange(range);
        if (range[0] == range[1])
        {
            range[0]--;
            range[1]++;
        }
        SetColorBarEndPoints(range[0], range[1]);
        UseLinearColorBar();
        SetColorBarNumberOfLabels(5);
        SetColorBarLabelPrecision(3);

        // Solid amd Grid Display
        if (m_DataSource->GetGridType() == GridType::MV_UNSTRUCTURED_GRID)
        {
            SetSolidDisplayToBlocky();
            SetGridDisplayToStairstepped();
        }
        else
        {
            SetSolidDisplayToSmooth();
            SetGridDisplayToInterpolated();
        }
        //      SetSolidDisplayToBlocky();
        SetSolidThresholdLimits(range[0], range[1]);
        SolidThresholdOff();

        // Isosurface
        double value = (range[0] + range[1]) / 2;
        SetCustomIsosurfaces(1, &value);
        SetRegularIsosurfaces(5, range[0], range[1]);
    }
    SetScalarDataTypeTo(temp);

    // Vector
    if (m_DataSource->GetVectorArray() != 0)
    {
        const int *vdim = m_DataSource->GetVectorGridDimensions();
        SubsampleVectors(0, vdim[0] - 1, 1, 0, vdim[1] - 1, 1, 0, vdim[2] - 1, 1);
        SetVectorSizeToOptimal();
        if (m_VectorLog10Transform)
        {
            SetVectorThresholdLimits(m_VectorLogMagnitudeRange[0], m_VectorLogMagnitudeRange[1]);
        }
        {
            SetVectorThresholdLimits(m_VectorMagnitudeRange[0], m_VectorMagnitudeRange[1]);
        }
        VectorThresholdOff();
    }
    else
    {
        SubsampleVectors(0, 0, 1, 0, 0, 1, 0, 0, 1);
        SetVectorScaleFactor(0);
        m_CubeSource->SetXLength(0);
        m_CubeSource->SetYLength(0);
        m_CubeSource->SetZLength(0);
    }
    SetVectorColor(0, 0, 0);
    m_VectorBounds[0]     = 0;
    m_VectorBounds[1]     = 1;
    m_VectorBounds[2]     = 0;
    m_VectorBounds[3]     = 1;
    m_VectorBounds[4]     = 0;
    m_VectorBounds[5]     = 1;
    m_VectorClippingAngle = 0;

    // Pathlines
    SetPathlineRepresentationToLine();
    SetPathlineTubeDiameter(1);
    SetPathlineTimeClippingMode(0);
    if (m_DataSource->GetNumberOfPathlines() > 0)
    {
        double range[2];
        GetPathlineTimeRange(range);
        SetPathlineColorBarEndPoints(range[0], range[1]);
        SetPathlineTimeClippingRange(range[0], range[1]);
    }
    else
    {
        SetPathlineColorBarEndPoints(0, 1);
        SetPathlineTimeClippingRange(0, 0);
    }

    // Overlay
    SetOverlayType(MV_ESRI_SHAPEFILE_OVERLAY);
    SetOverlayCoordinatesAtGridOrigin(0, 0);
    SetOverlayToGridScale(1);
    SetOverlayAngle(0);
    SetOverlayDrape(0);
    SetOverlayTrim(0);
    SetOverlayCrop(0);
    SetOverlayElevation(0);
    SetOverlayDrapeGap(0);

    // Grid Display
}

#if ((VTK_MAJOR_VERSION == 8) && (VTK_MINOR_VERSION <= 1) || (VTK_MAJOR_VERSION < 8))  //  https://vtk.org/Wiki/VTK/API_Changes_8_0_1_to_8_1_0
void mvManager::SetImmediateModeRendering(int b)  // deprecated vtk 8.1
{
    m_SolidMapper->SetImmediateModeRendering(b);
    m_IsosurfaceMapper->SetImmediateModeRendering(b);
    m_VectorMapper->SetImmediateModeRendering(b);
    m_VectorGlyphMapper->SetImmediateModeRendering(b);
    m_GridShell->SetImmediateModeRendering(b);
    m_BoundingBox->SetImmediateModeRendering(b);
    m_Axes->SetImmediateModeRendering(b);
    for (int i = 0; i < 3; i++)
    {
        m_GridLines[i]->SetImmediateModeRendering(b);
    }
}
#endif

void mvManager::SetReleaseDataFlag(int b)
{
    int i;
    m_GridShellClipMin->SetReleaseDataFlag(b);
    m_GridShellClipMax->SetReleaseDataFlag(b);
    m_SmoothSolid->SetReleaseDataFlag(b);
    m_CroppedSolid->SetReleaseDataFlag(b);
    m_Faces->SetReleaseDataFlag(b);
    m_FacesClipMin->SetReleaseDataFlag(b);
    m_FacesClipMax->SetReleaseDataFlag(b);
    for (i = 0; i < 3; i++)
    {
        m_IsosurfaceCutter[i]->SetReleaseDataFlag(b);
    }
    for (i = 0; i < 6; i++)
    {
        m_Cropper[i]->SetReleaseDataFlag(b);
    }
    for (i = 0; i < 24; i++)
    {
        m_FaceCrop[i]->SetReleaseDataFlag(b);
    }
}

GridType mvManager::GetGridType() const
{
    if (m_DataSource != 0)
    {
        return m_DataSource->GetGridType();
    }
    else
    {
        return GridType::MV_GRID_NOT_DEFINED;
    }
}

const int *mvManager::GetScalarGridDimensions() const
{
    if (m_DataSource != 0)
    {
        return m_DataSource->GetScalarGridDimensions();
    }
    else
    {
        return 0;
    }
}

const int *mvManager::GetVectorGridDimensions() const
{
    if (m_DataSource != 0)
    {
        return m_DataSource->GetVectorGridDimensions();
    }
    else
    {
        return 0;
    }
}

int mvManager::GetNumberOfTimePoints() const
{
    if (!m_DataSource)
    {
        return 0;
    }
    else
    {
        return m_DataSource->GetNumberOfTimePoints();
    }
}

char **mvManager::GetTimePointLabels()
{
    if (!m_DataSource)
    {
        return 0;
    }
    else
    {
        return m_DataSource->GetTimePointLabels();
    }
}

int mvManager::GetNumberOfScalarDataTypes() const
{
    if (!m_DataSource)
    {
        return 0;
    }
    else
    {
        return m_DataSource->GetNumberOfScalarDataTypes();
    }
}

char **mvManager::GetDataTypeLabels() const
{
    if (!m_DataSource)
    {
        return 0;
    }
    else
    {
        return m_DataSource->GetDataTypeLabels();
    }
}

const char *mvManager::GetActiveScalarDataName() const
{
    if (!m_DataSource)
    {
        return "";
    }
    else
    {
        return (m_DataSource->GetDataTypeLabels())[m_ActiveDataType];
    }
}

ScalarMode mvManager::GetPrimaryScalarMode() const
{
    if (m_DataSource)
    {
        return m_DataSource->GetPrimaryScalarMode();
    }
    else
    {
        return ScalarMode::MV_CELL_SCALARS;
    }
}

const char *mvManager::GetModelName() const
{
    if (m_DataSource)
    {
        return m_DataSource->GetName();
    }
    else
    {
        return nullptr;
    }
}

int mvManager::GetTimeLabelOption() const
{
    if (m_DataSource)
    {
        return m_DataSource->GetTimeLabelOption();
    }
    else
    {
        return 0;
    }
}

void mvManager::HideScalarData()
{
    m_SolidActor->VisibilityOff();
    m_IsosurfaceActor->VisibilityOff();
    m_CroppedAwayPiecesActor->VisibilityOff();
}

void mvManager::ShowScalarDataAsSolid()
{
    m_SolidActor->VisibilityOn();
    m_IsosurfaceActor->VisibilityOff();
    if (m_ShowCroppedAwayPieces)
    {
        m_CroppedAwayPiecesActor->VisibilityOn();
    }
    BuildPipelineForSolid();
}

int mvManager::IsSolidVisible() const
{
    return m_SolidActor->GetVisibility();
}

void mvManager::ShowScalarDataAsIsosurfaces()
{
    m_SolidActor->VisibilityOff();
    m_IsosurfaceActor->VisibilityOn();
    if (m_ShowCroppedAwayPieces)
    {
        m_CroppedAwayPiecesActor->VisibilityOn();
    }
    BuildPipelineForIsosurface();
}

int mvManager::AreIsosurfacesVisible() const
{
    return m_IsosurfaceActor->GetVisibility();
}

void mvManager::SetNumberOfColorBands(int numColorBands)
{
    m_NumberOfColorBands[m_ActiveDataType] = numColorBands;
    UpdateColorBands();
}

int mvManager::GetNumberOfColorBands() const
{
    if (m_NumberOfColorBands != 0)
    {
        return m_NumberOfColorBands[m_ActiveDataType];
    }
    else
    {
        return 0;
    }
}

void mvManager::UpdateColorBands()
{
    if (m_NumberOfColorBands[m_ActiveDataType] == 0)
    {
        //		m_ColorBandFilter->SetValues(0, 0);
        m_ColorBandFilter->GenerateValues(0, 0., 0.);
    }
    else if (m_ColorBarDataSource == 0)
    {
        double *values = new double[m_NumberOfColorBands[m_ActiveDataType] + 1];
        double  v1     = GetColorBarValueBlue();
        double  v2     = GetColorBarValueRed();
        if (v1 > v2)
        {
            double temp = v1;
            v1          = v2;
            v2          = temp;
        }
        if (!IsColorBarLinear())
        {
            v1 = log(v1) / log(10.0);
            v2 = log(v2) / log(10.0);
        }
        double delta = (v2 - v1) / m_NumberOfColorBands[m_ActiveDataType];
        values[0]    = v1;
        for (int i = 1; i <= m_NumberOfColorBands[m_ActiveDataType]; i++)
        {
            values[i] = v1 + i * delta;
        }
        if (!IsColorBarLinear())
        {
            for (int i = 0; i <= m_NumberOfColorBands[m_ActiveDataType]; i++)
            {
                values[i] = pow(10, values[i]);
            }
        }
        //		m_ColorBandFilter->SetValues(values, m_NumberOfColorBands[m_ActiveDataType] + 1);
        m_ColorBandFilter->SetNumberOfContours(m_NumberOfColorBands[m_ActiveDataType] + 1);
        for (int i = 0; i <= m_NumberOfColorBands[m_ActiveDataType]; i++)
        {
            m_ColorBandFilter->SetValue(i, values[i]);
        }
        delete[] values;
    }

    if (m_SolidDisplayMode[m_ActiveDataType] == SolidDisplayType::MV_SOLID_SMOOTH)
    {
        m_SolidMapper->SetScalarModeToUsePointData();
    }
    else // For both block and banded display modes, the mapper scalar mode uses cell data.
    {
        m_SolidMapper->SetScalarModeToUseCellData();
    }
    BuildPipelineForSolid();
}

void mvManager::SetScalarSubgridExtent(int imin, int imax, int jmin, int jmax, int kmin, int kmax)
{
    if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        // note that imin, imax, jmin, etc refer to point indices, starting from zero, and using vtk convention
        // In particular, k goes from bottom to top.
        // interpolated grid - extract a VOI from m_ScalarStructuredGrid
        m_ExtractStructuredGridForSubgrid->SetVOI(imin, imax, jmin, jmax, kmin, kmax);
        m_ModelFeatures->SetSubgridExtent(imin, imax, jmin, jmax, kmin, kmax);
        // stairstepped grid - extract cells from m_StairsteppedGrid.
        const int                 *sdim            = m_DataSource->GetScalarGridDimensions();
        int                        numRow          = sdim[1] - 1;
        int                        numCol          = sdim[0] - 1;
        int                        numCellPerLayer = numRow * numCol;
        vtkSmartPointer<vtkIdList> cellList        = vtkSmartPointer<vtkIdList>::New();
        // note that kc, jc, and ic are cell indices
        for (int kc = kmin; kc < kmax; kc++)
        {
            for (int jc = jmin; jc < jmax; jc++)
            {
                for (int ic = imin; ic < imax; ic++)
                {
                    cellList->InsertNextId(kc * numCellPerLayer + jc * numCol + ic);
                }
            }
        }
        m_ExtractCellsForSubgrid->SetCellList(cellList);
    }
    else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        // kmin = top layer in subgrid
        // kmax = bottom layer in subgrid
        vtkSmartPointer<vtkIdList> cellList = vtkSmartPointer<vtkIdList>::New();
        int                        cellId   = 0;
        for (int kc = 0; kc < kmin; kc++)
        {
            cellId += m_DataSource->GetNumberOfVTKCellsInLayer(kc);
        }
        for (int kc = kmin; kc <= kmax; kc++)
        {
            for (int ic = 0; ic < m_DataSource->GetNumberOfVTKCellsInLayer(kc); ic++)
            {
                cellList->InsertNextId(cellId++);
            }
        }
        m_ExtractCellsForSubgrid->SetCellList(cellList);
    }
}

const int *mvManager::GetScalarSubgridExtent()
{
    if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        return m_ExtractStructuredGridForSubgrid->GetVOI();
    }
    else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        // TO DO
        return 0;
    }
    return 0;
}

void mvManager::ScalarSubgridOn()
{
    if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        const int *sdim = m_DataSource->GetScalarGridDimensions();
        int       *v    = m_ExtractStructuredGridForSubgrid->GetVOI();
        const int *g0   = m_GridLines[0]->GetExtent();
        const int *g1   = m_GridLines[1]->GetExtent();
        const int *g2   = m_GridLines[2]->GetExtent();
        if (v[0] == g2[0] && v[1] == g2[1] && v[2] == g2[2] && v[3] == g2[3] && v[4] == g1[4] && v[5] == g1[5])
        {
            return;
        }

        if (m_GridDisplayMode == MV_GRID_INTERPOLATED)
        {
            // Work with the sub data set.
            m_ActiveScalarDataSet->SetInputConnection(m_ExtractStructuredGridForSubgrid->GetOutputPort());
            m_BlockySolidThreshold->SetInputConnection(m_ExtractStructuredGridForSubgrid->GetOutputPort());
            //	if (m_DataSource->AreAllCellsActive())
            //	{
            //		m_GridShell->SetInputConnection(m_ExtractStructuredGridForSubgrid->GetOutputPort());
            //		m_Isosurface->SetInputConnection(m_ExtractStructuredGridForSubgrid->GetOutputPort());
            //		m_SmoothSolidIsosurface->SetInputConnection(m_ExtractStructuredGridForSubgrid->GetOutputPort());
            //		for (int i=0; i<6; i++)
            //		{
            //			m_ExtractFace[i]->SetInputConnection(m_ExtractStructuredGridForSubgrid->GetOutputPort());
            //		}
            //	}
            //	else
            //	{
            m_GridShell->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());
            m_Isosurface->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());
            m_SmoothSolidIsosurface->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());
            for (int i = 0; i < 6; i++)
            {
                m_ExtractFace[i]->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());
            }
            //	}
            int p = max(v[0], min(g0[0], v[1]));
            m_GridLines[0]->SetExtent(p, p, v[2], v[3], v[4], v[5]);
            p = max(v[2], min(g1[2], v[3]));
            m_GridLines[1]->SetExtent(v[0], v[1], p, p, v[4], v[5]);
            p = max(v[4], min(g2[4], v[5]));
            m_GridLines[2]->SetExtent(v[0], v[1], v[2], v[3], p, p);

            // Limit the vector display
            if (HasVectorData())
            {
                int *vectorVOI = m_ExtractStructuredGridVector->GetVOI();
                int *scalarVOI = m_ExtractStructuredGridForSubgrid->GetVOI();
                int  v0, v1, v2, v3, v4, v5;
                if (vectorVOI[0] > scalarVOI[1] - 1)
                {
                    v0 = scalarVOI[1] - 1;
                    v1 = v0;
                }
                else if (vectorVOI[1] < scalarVOI[0])
                {
                    v0 = scalarVOI[0];
                    v1 = v0;
                }
                else
                {
                    v0 = max(scalarVOI[0], vectorVOI[0]);
                    v1 = min(scalarVOI[1] - 1, vectorVOI[1]);
                }
                if (vectorVOI[2] > scalarVOI[3] - 1)
                {
                    v2 = scalarVOI[3] - 1;
                    v3 = v2;
                }
                else if (vectorVOI[3] < scalarVOI[2])
                {
                    v2 = scalarVOI[2];
                    v3 = v2;
                }
                else
                {
                    v2 = max(scalarVOI[2], vectorVOI[2]);
                    v3 = min(scalarVOI[3] - 1, vectorVOI[3]);
                }
                if (vectorVOI[4] > scalarVOI[5] - 1)
                {
                    v4 = scalarVOI[5] - 1;
                    v5 = v4;
                }
                else if (vectorVOI[5] < scalarVOI[4])
                {
                    v4 = scalarVOI[4];
                    v5 = v4;
                }
                else
                {
                    v4 = max(scalarVOI[4], vectorVOI[4]);
                    v5 = min(scalarVOI[5] - 1, vectorVOI[5]);
                }

                m_ExtractStructuredGridVector->SetVOI(v0, v1, v2, v3, v4, v5);
            }

            // Limit the model features
            m_ModelFeatures->SubgridOn();

            // Crop the overlay
            m_Overlay->SubgridOn();
        }
        else
        {
            m_ActiveScalarDataSet->SetInputConnection(m_ExtractCellsForSubgrid->GetOutputPort());
            m_BlockySolidThreshold->SetInputConnection(m_ExtractCellsForSubgrid->GetOutputPort());
        }
    }
    else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        m_ActiveScalarDataSet->SetInputConnection(m_ExtractCellsForSubgrid->GetOutputPort());
        m_BlockySolidThreshold->SetInputConnection(m_ExtractCellsForSubgrid->GetOutputPort());
    }
    else
    {
        return;
    }

    // Recompute bounds for the bounding box
    double bounds[6];
    m_ActiveScalarDataSet->Update();
    m_ActiveScalarDataSet->GetOutput()->ComputeBounds();
    m_ActiveScalarDataSet->GetOutput()->GetBounds(bounds);
    m_BoundingBox->SetBounds(bounds);

    // Crop the pathlines
    if (HasPathlineData())
    {
        m_Pathlines->DoCrop(bounds);
    }

    // Update the axes position.
    const double *s = GetScale();
    bounds[0] *= s[0];
    bounds[1] *= s[0];
    bounds[2] *= s[1];
    bounds[3] *= s[1];
    bounds[4] *= s[2];
    bounds[5] *= s[2];
    m_Axes->SetDefaultPositions(bounds);

    // Update the cropping positions
    UpdateCrop();

    if (m_SolidActor->GetVisibility())
    {
        BuildPipelineForSolid();
    }
    else if (m_IsosurfaceActor->GetVisibility())
    {
        BuildPipelineForIsosurface();
    }
}

void mvManager::ScalarSubgridOff()
{
    if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        if (m_GridDisplayMode == MV_GRID_INTERPOLATED)
        {
            m_ActiveScalarDataSet->SetInputData(m_ScalarStructuredGrid);
            m_BlockySolidThreshold->SetInputData(m_ScalarStructuredGrid);
        }
        else
        {
            m_ActiveScalarDataSet->SetInputData(m_StairsteppedGrid);
            m_BlockySolidThreshold->SetInputData(m_StairsteppedGrid);
        }
        const int *sdim = m_DataSource->GetScalarGridDimensions();
        const int *g    = m_GridLines[0]->GetExtent();
        m_GridLines[0]->SetExtent(g[0], g[0], 0, sdim[1] - 1, 0, sdim[2] - 1);
        g = m_GridLines[1]->GetExtent();
        m_GridLines[1]->SetExtent(0, sdim[0] - 1, g[2], g[2], 0, sdim[2] - 1);
        g = m_GridLines[2]->GetExtent();
        m_GridLines[2]->SetExtent(0, sdim[0] - 1, 0, sdim[1] - 1, g[4], g[4]);
        m_ModelFeatures->SubgridOff();
        m_Overlay->SubgridOff();
        m_GridShell->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());
        m_Isosurface->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());
        m_SmoothSolidIsosurface->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());
        for (int i = 0; i < 6; i++)
        {
            m_ExtractFace[i]->SetInputConnection(m_ActiveScalarDataSet->GetOutputPort());
        }
    }
    else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        if (m_GridDisplayMode == MV_GRID_INTERPOLATED)
        {
            m_ActiveScalarDataSet->SetInputData(m_ScalarLayeredGrid);
            m_BlockySolidThreshold->SetInputData(m_ScalarLayeredGrid);
        }
        else
        {
            m_ActiveScalarDataSet->SetInputData(m_StairsteppedGrid);
            m_BlockySolidThreshold->SetInputData(m_StairsteppedGrid);
        }
    }
    // Recompute the bounds for the bounding box
    double bounds[6];
    // if (m_DataSource->AreAllCellsActive())
    //{
    //	m_ScalarStructuredGrid->GetBounds(bounds);
    //	m_BoundingBox->SetBounds(bounds);
    // }
    // else
    //{
    m_ActiveScalarDataSet->Update();
    m_ActiveScalarDataSet->GetOutput()->ComputeBounds();
    m_ActiveScalarDataSet->GetOutput()->GetBounds(bounds);
    m_BoundingBox->SetBounds(bounds);
    //}

    // Update the axes position.
    const double *s = GetScale();
    bounds[0] *= s[0];
    bounds[1] *= s[0];
    bounds[2] *= s[1];
    bounds[3] *= s[1];
    bounds[4] *= s[2];
    bounds[5] *= s[2];
    m_Axes->SetDefaultPositions(bounds);

    m_Pathlines->CropOff();

    // Update the cropping bounds
    UpdateCrop();

    if (m_SolidActor->GetVisibility())
    {
        BuildPipelineForSolid();
    }
    else if (m_IsosurfaceActor->GetVisibility())
    {
        BuildPipelineForIsosurface();
    }
}

int mvManager::IsScalarSubgridOn() const
{
    if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        if (m_GridDisplayMode == MV_GRID_INTERPOLATED)
        {
            return (m_ActiveScalarDataSet->GetInput() == m_ExtractStructuredGridForSubgrid->GetOutput());
        }
        else if (m_GridDisplayMode == MV_GRID_STAIRSTEPPED)
        {
            return (m_ActiveScalarDataSet->GetInput() == m_ExtractCellsForSubgrid->GetOutput());
        }
    }
    else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        return (m_ActiveScalarDataSet->GetInput() == m_ExtractCellsForSubgrid->GetOutput());
    }
    return 0;
}

void mvManager::ShowVectors()
{
    m_VectorActor->VisibilityOn();
    if (m_VectorGlyphActivated)
    {
        m_VectorGlyphActor->VisibilityOn();
    }
}

void mvManager::HideVectors()
{
    m_VectorActor->VisibilityOff();
    m_VectorGlyphActor->VisibilityOff();
}

int mvManager::AreVectorsVisible() const
{
    return m_VectorActor->GetVisibility();
}

double mvManager::GetVectorLineWidth()
{
    return m_VectorActor->GetProperty()->GetLineWidth();
}

void mvManager::SetVectorLineWidth(double width)
{
    m_VectorActor->GetProperty()->SetLineWidth(width);
}

void mvManager::SetVectorColor(double red, double green, double blue)
{
    m_VectorActor->GetProperty()->SetColor(red, green, blue);
    m_VectorGlyphActor->GetProperty()->SetColor(red, green, blue);
}

const double *mvManager::GetVectorColor() const
{
    return m_VectorActor->GetProperty()->GetColor();
}

void mvManager::SetVectorScaleFactor(double scaleFactor)
{
    m_HedgeHog->SetScaleFactor(scaleFactor);
}

double mvManager::GetVectorScaleFactor() const

{
    return m_HedgeHog->GetScaleFactor();
}

double mvManager::ComputeOptimalVectorSize()
{
    double bounds[6];
    m_BoundingBox->GetBounds(bounds);
    double dx   = bounds[1] - bounds[0];
    double dy   = bounds[3] - bounds[2];
    double dz   = bounds[5] - bounds[4];
    double diag = sqrt(dx * dx + dy * dy + dz * dz);
    double upperRange;
    if (m_VectorLog10Transform)
    {
        upperRange = m_VectorLogMagnitudeRange[1];
    }
    else
    {
        upperRange = m_VectorMagnitudeRange[1];
    }
    if (upperRange > 0)
    {
        double f = diag / upperRange / 10;
        int    e = floor(log(f) / log(10.0));
        int    d = ceil(f * pow(10.0, -e));
        return d * pow(10.0, e);
    }
    else
    {
        return 0;
    }
    return 0;
}

void mvManager::SetVectorSizeToOptimal()
{
    SetVectorScaleFactor(ComputeOptimalVectorSize());

    double bounds[6];
    m_BoundingBox->GetBounds(bounds);
    double  dx       = bounds[1] - bounds[0];
    double  dy       = bounds[3] - bounds[2];
    double  dz       = bounds[5] - bounds[4];
    double  diag     = sqrt(dx * dx + dy * dy + dz * dz);

    double  cubeSize = diag / 300;
    double *s        = m_SolidActor->GetScale();
    m_CubeSource->SetXLength(cubeSize / s[0]);
    m_CubeSource->SetYLength(cubeSize / s[1]);
    m_CubeSource->SetZLength(cubeSize / s[2]);
}

void mvManager::VectorThresholdOn()
{
    m_DoVectorThreshold = 1;
    m_HedgeHog->SetInputConnection(m_VectorThreshold->GetOutputPort());
    m_VectorGlyph->SetInputConnection(m_VectorThreshold->GetOutputPort());
}

void mvManager::VectorThresholdOff()
{
    m_DoVectorThreshold = 0;
    m_HedgeHog->SetInputConnection(m_ActiveVectorDataSet->GetOutputPort());
    m_VectorGlyph->SetInputConnection(m_ActiveVectorDataSet->GetOutputPort());
}

int mvManager::IsVectorThresholdOn() const
{
    return m_DoVectorThreshold;
}

void mvManager::SetVectorThresholdLimits(double minValue, double maxValue)
{
    if (minValue < 0)
    {
        minValue = 0;
    }
    // a hack to make the pipeline work
    m_VectorThreshold->ThresholdBetween(-20, -10);
    if (m_VectorLog10Transform)
    {
        double minRange = m_MinPositiveVector / 2;
        if (minRange <= 0)
        {
            minRange = 1;
        }
        if (minValue > 0)
        {
            minValue = log10(minValue / minRange);
        }
        if (maxValue > 0)
        {
            maxValue = log10(maxValue / minRange);
        }
    }
    m_VectorThreshold->ThresholdBetween(minValue, maxValue);
}

void mvManager::GetVectorThresholdLimits(double *limits) const
{
    limits[0] = m_VectorThreshold->GetLowerThreshold();
    limits[1] = m_VectorThreshold->GetUpperThreshold();
}

void mvManager::CropVectors(double xmin, double xmax,
                            double ymin, double ymax, double zmin, double zmax, int cropAngle)
{
    int    ShouldCrop = ((xmin != 0.) || (xmax != 1.) || (ymin != 0.) || (ymax != 1.) || (zmin != 0.) || (zmax != 1.));

    double bounds[6];
    m_BoundingBox->GetBounds(bounds);

    m_VectorClippingXMin = xmin;
    m_VectorClippingXMax = xmax;
    m_VectorClippingYMin = ymin;
    m_VectorClippingYMax = ymax;
    m_VectorClippingZMin = zmin;
    m_VectorClippingZMax = zmax;
    if (cropAngle > 45)
    {
        m_VectorClippingAngle = 45;
    }
    else if (cropAngle < -45)
    {
        m_VectorClippingAngle = -45;
    }
    else
    {
        m_VectorClippingAngle = cropAngle;
    }

    m_VectorBounds[0] = xmin;
    m_VectorBounds[1] = xmax;
    m_VectorBounds[2] = ymin;
    m_VectorBounds[3] = ymax;
    m_VectorBounds[4] = zmin;
    m_VectorBounds[5] = zmax;

    if (ShouldCrop)
    {
        m_CropBoxForVectors->SetModelBounds(bounds);
        m_CropBoxForVectors->SetBounds(m_VectorBounds);
        m_CropBoxForVectors->SetAngle(m_VectorClippingAngle);
        if (GetGridType() == GridType::MV_STRUCTURED_GRID)
        {
            m_CropVectors->SetInputConnection(m_ExtractStructuredGridVector->GetOutputPort());
        }
        else
        {
            //			m_CropVectors->SetInputConnection(m_ExtractUnstructuredGridVector->GetOutputPort());
            m_CropVectors->SetInputData(m_VectorUnstructuredGrid);
        }
        m_ActiveVectorDataSet->SetInputConnection(m_CropVectors->GetOutputPort());
    }
    else
    {
        if (GetGridType() == GridType::MV_STRUCTURED_GRID)
        {
            m_ActiveVectorDataSet->SetInputConnection(m_ExtractStructuredGridVector->GetOutputPort());
        }
        else
        {
            //			m_ActiveVectorDataSet->SetInputConnection(m_ExtractUnstructuredGridVector->GetOutputPort());
            m_ActiveVectorDataSet->SetInputData(m_VectorUnstructuredGrid);
        }
    };
}

void mvManager::SubsampleVectors(int imin, int imax, int irate, int jmin, int jmax, int jrate,
                                 int kmin, int kmax, int krate)
{
    m_VectorSubsampleOffset[0] = 0;

    if (jrate > 1)
    {
        m_VectorSubsampleOffset[1] = jmax - ((jmax - jmin) / jrate) * jrate - jmin;
    }
    else
    {
        m_VectorSubsampleOffset[1] = 0;
    }

    if (krate > 1)
    {
        m_VectorSubsampleOffset[2] = kmax - ((kmax - kmin) / krate) * krate - kmin;
    }
    else
    {
        m_VectorSubsampleOffset[2] = 0;
    }

    m_ExtractStructuredGridVector->SetVOI(imin, imax,
                                          jmin + m_VectorSubsampleOffset[1], jmax,
                                          kmin + m_VectorSubsampleOffset[2], kmax);
    m_ExtractStructuredGridVector->SetSampleRate(irate, jrate, krate);
}

void mvManager::GetVectorSubsampleExtentAndRate(int *extent, int *rate)
{
    const int *e = m_ExtractStructuredGridVector->GetVOI();
    const int *r = m_ExtractStructuredGridVector->GetSampleRate();
    extent[0]    = e[0] - m_VectorSubsampleOffset[0];
    extent[1]    = e[1];
    extent[2]    = e[2] - m_VectorSubsampleOffset[1];
    extent[3]    = e[3];
    extent[4]    = e[4] - m_VectorSubsampleOffset[2];
    extent[5]    = e[5];
    rate[0]      = r[0];
    rate[1]      = r[1];
    rate[2]      = r[2];
}

void mvManager::ActivateVectorGlyph(int active)
{
    m_VectorGlyphActivated = active;
    if (m_VectorActor->GetVisibility())
    {
        if (active)
        {
            m_VectorGlyphActor->VisibilityOn();
        }
        else
        {
            m_VectorGlyphActor->VisibilityOff();
        }
    }
}

int mvManager::IsVectorGlyphActivated() const
{
    return m_VectorGlyphActivated;
}

void mvManager::EnlargeVectorGlyph()
{
    m_CubeSource->SetXLength(m_CubeSource->GetXLength() * 1.5);
    m_CubeSource->SetYLength(m_CubeSource->GetYLength() * 1.5);
    m_CubeSource->SetZLength(m_CubeSource->GetZLength() * 1.5);
}

void mvManager::ShrinkVectorGlyph()
{
    m_CubeSource->SetXLength(m_CubeSource->GetXLength() / 1.5);
    m_CubeSource->SetYLength(m_CubeSource->GetYLength() / 1.5);
    m_CubeSource->SetZLength(m_CubeSource->GetZLength() / 1.5);
}

void mvManager::ShowPathlines()
{
    m_Pathlines->VisibilityOn();
}

void mvManager::HidePathlines()
{
    m_Pathlines->VisibilityOff();
}

int mvManager::ArePathlinesVisible() const
{
    return m_Pathlines->GetVisibility();
}

void mvManager::SetPathlineRepresentationToLine()
{
    m_Pathlines->SetRepresentationToLine();
}

void mvManager::SetPathlineRepresentationToTube()
{
    m_Pathlines->SetRepresentationToTube();
}

int mvManager::GetPathlineRepresentation() const
{
    return m_Pathlines->GetRepresentation();
}

void mvManager::GetPathlineTimeRange(double *range) const
{
    if (m_DataSource->GetNumberOfPathlines() > 0)
    {
        double *r = m_PathlineScalars->GetRange();
        range[0]  = r[0];
        range[1]  = r[1];
    }
}

void mvManager::SetPathlineTubeDiameter(double diameter)
{
    m_Pathlines->SetNormalizedTubeDiameter(diameter);
}

double mvManager::GetPathlineTubeDiameter() const
{
    return m_Pathlines->GetNormalizedTubeDiameter();
}

void mvManager::SetPathlineTimeClippingMode(int mode)
{
    m_PathlineTimeClippingMode = mode;
    switch (mode)
    {
    case 0:
        // no time clipping.
        if (m_DataSource->GetPathlineScalarMode() != MP_TRAVEL_TIME)
        {
            m_DataSource->SetPathlineScalarMode(MP_TRAVEL_TIME);
            UpdatePathlineScalars();
        }
        m_Pathlines->TimeClippingOff();
        break;
    case 1:
        // clip to starting time and current time.
        {
            if (m_DataSource->GetPathlineScalarMode() != MP_TRAVEL_TIME)
            {
                m_DataSource->SetPathlineScalarMode(MP_TRAVEL_TIME);
                UpdatePathlineScalars();
            }

            char **labels      = m_DataSource->GetTimePointLabels();
            double clipTimeMax = atof(labels[m_TimePointIndex]);
            m_Pathlines->SetTimeClippingRange(0, clipTimeMax);
            m_Pathlines->TimeClippingOn();
            break;
        }
    case 2:
        // clip to specified starting and ending times.
        if (m_DataSource->GetPathlineScalarMode() != MP_TRAVEL_TIME)
        {
            m_DataSource->SetPathlineScalarMode(MP_TRAVEL_TIME);
            UpdatePathlineScalars();
        }
        m_Pathlines->SetTimeClippingRange(m_PathlineClipTimeMin, m_PathlineClipTimeMax);
        m_Pathlines->TimeClippingOn();
        break;
    case 3:
        // clip to minimum time.
        if (m_DataSource->GetPathlineScalarMode() != MP_MIN_TRAVEL_TIME)
        {
            m_DataSource->SetPathlineScalarMode(MP_MIN_TRAVEL_TIME);
            UpdatePathlineScalars();
        }
        m_Pathlines->SetTimeClippingRange(m_PathlineClipTimeMin, m_PathlineClipTimeMax);
        m_Pathlines->TimeClippingOn();
        break;
    case 4:
        // clip to maximum time.
        if (m_DataSource->GetPathlineScalarMode() != MP_MAX_TRAVEL_TIME)
        {
            m_DataSource->SetPathlineScalarMode(MP_MAX_TRAVEL_TIME);
            UpdatePathlineScalars();
        }
        m_Pathlines->SetTimeClippingRange(m_PathlineClipTimeMin, m_PathlineClipTimeMax);
        m_Pathlines->TimeClippingOn();
        break;
    }
}

void mvManager::SetPathlineTimeClippingRange(double minTime, double maxTime)
{
    m_PathlineClipTimeMin = minTime;
    m_PathlineClipTimeMax = maxTime;
    m_Pathlines->SetTimeClippingRange(minTime, maxTime);
}

double mvManager::GetPathlineClipTimeMin() const
{
    return m_PathlineClipTimeMin;
}

double mvManager::GetPathlineClipTimeMax() const
{
    return m_PathlineClipTimeMax;
}

double mvManager::GetPathlineTimeBlue() const
{
    return m_Pathlines->GetTimeBlue();
}

double mvManager::GetPathlineTimeRed() const
{
    return m_Pathlines->GetTimeRed();
}

void mvManager::SetPathlineColorBarEndPoints(double valueBlue, double valueRed)
{
    m_Pathlines->SetColorBarEndPoints(valueBlue, valueRed);
}

int mvManager::GetPathlineLogTransform() const
{
    return m_Pathlines->GetLogTransform();
}

void mvManager::SetPathlineLogTransform(int Value)
{
    m_Pathlines->SetLogTransform(Value);
}

void mvManager::ShowModelFeatures()
{
    m_ModelFeatures->VisibilityOn();
}

void mvManager::HideModelFeatures()
{
    m_ModelFeatures->VisibilityOff();
}

int mvManager::AreModelFeaturesVisible() const
{
    return m_ModelFeatures->GetVisibility();
}

void mvManager::SetModelFeatureDisplayOrder(int *displayOrder)
{
    m_ModelFeatures->SetDisplayOrder(displayOrder);
}

int mvManager::HasModelFeatures() const
{
    return (m_DataSource && m_DataSource->GetModelFeatureArray());
}

int mvManager::GetNumberOfModelFeatureTypes() const
{
    if (m_DataSource)
    {
        return m_DataSource->GetNumberOfModelFeatureTypes();
    }
    return 0;
}

const char *mvManager::GetModelFeatureLabels() const
{
    if (m_DataSource)
    {
        return m_DataSource->GetModelFeatureLabels();
    }
    return nullptr;
}

int *mvManager::GetModelFeatureDisplayOrder()
{
    return m_ModelFeatures->GetDisplayOrder();
}

void mvManager::EnlargeModelFeatureGlyphs()
{
    m_ModelFeatures->EnlargeGlyphs();
}

void mvManager::ShrinkModelFeatureGlyphs()
{
    m_ModelFeatures->ShrinkGlyphs();
}

int mvManager::GetModelFeatureDisplayMode() const
{
    return m_DataSource->GetModelFeatureDisplayMode();
}

void mvManager::SetModelFeatureColor(const char *modelFeatureName, double *rgba)
{
    char *names = m_DataSource->GetModelFeatureLabels();
    for (int i = 0; i < m_DataSource->GetNumberOfModelFeatureTypes(); i++)
    {
        if (strncmp(names + 40 * i, modelFeatureName, 40) == 0)
        {
            m_ModelFeatures->SetColor(i, rgba);
            return;
        }
    }
}

void mvManager::GetModelFeatureColor(const char *modelFeatureName, double *rgba)
{
    char *names = m_DataSource->GetModelFeatureLabels();
    for (int i = 0; i < m_DataSource->GetNumberOfModelFeatureTypes(); i++)
    {
        if (strncmp(names + 40 * i, modelFeatureName, 40) == 0)
        {
            m_ModelFeatures->GetColor(i, rgba);
            return;
        }
    }
}

int mvManager::IsGridShellVisible() const
{
    return m_GridShell->GetVisibility();
}

void mvManager::ShowGridShell()
{
    m_GridShell->VisibilityOn();
}

void mvManager::HideGridShell()
{
    m_GridShell->VisibilityOff();
}

void mvManager::SetGridShellColor(double red, double green, double blue)
{
    m_GridShell->SetColor(red, green, blue);
}

void mvManager::SetGridShellOpacity(double opacity)
{
    m_GridShell->SetOpacity(opacity);
}

const double *mvManager::GetGridShellColor() const
{
    return m_GridShell->GetColor();
}

double mvManager::GetGridShellOpacity() const
{
    return m_GridShell->GetOpacity();
}

int mvManager::IsBoundingBoxVisible() const
{
    return m_BoundingBox->GetVisibility();
}

void mvManager::ShowBoundingBox()
{
    m_BoundingBox->VisibilityOn();
}

void mvManager::HideBoundingBox()
{
    m_BoundingBox->VisibilityOff();
}

void mvManager::SetBoundingBoxColor(double r, double g, double b)
{
    m_BoundingBox->SetColor(r, g, b);
}

const double *mvManager::GetBoundingBoxColor() const
{
    return m_BoundingBox->GetColor();
}

int mvManager::AreActivatedGridLinesVisible() const
{
    return m_ActivatedGridLinesVisibility;
}

void mvManager::ShowActivatedGridLines()
{
    m_ActivatedGridLinesVisibility = 1;
    for (int i = 0; i < 3; i++)
    {
        if (m_GridLinesActivated[i])
        {
            m_GridLines[i]->VisibilityOn();
        }
    }
}

void mvManager::HideGridLines()
{
    m_ActivatedGridLinesVisibility = 0;
    for (int i = 0; i < 3; i++)
    {
        m_GridLines[i]->VisibilityOff();
    }
}

void mvManager::ActivateGridLines(int i)
{
    if ((m_DataSource == 0) || (GetGridType() != GridType::MV_LAYERED_GRID))
    {
        m_GridLinesActivated[i] = 1;
        if (m_ActivatedGridLinesVisibility == 1)
        {
            m_GridLines[i]->VisibilityOn();
        }
    }
}

void mvManager::DeactivateGridLines(int i)
{
    m_GridLinesActivated[i] = 0;
    m_GridLines[i]->VisibilityOff();
}

void mvManager::SetGridLinePositions(int posX, int posY, int posZ)
{
    // customized for Modflow to use column, row, and layer indexing
    const int *sdim = m_DataSource->GetScalarGridDimensions();
    if (IsScalarSubgridOn())
    {
        const int *g = m_GridLines[0]->GetExtent();
        m_GridLines[0]->SetExtent(posX, posX, g[2], g[3], g[4], g[5]);
        g = m_GridLines[1]->GetExtent();
        m_GridLines[1]->SetExtent(g[0], g[1], posY, posY, g[4], g[5]);
        g = m_GridLines[2]->GetExtent();
        m_GridLines[2]->SetExtent(g[0], g[1], g[2], g[3], posZ, posZ);
    }
    else
    {
        m_GridLines[0]->SetExtent(posX, posX, 0, sdim[1] - 1, 0, sdim[2] - 1);
        m_GridLines[1]->SetExtent(0, sdim[0] - 1, posY, posY, 0, sdim[2] - 1);
        m_GridLines[2]->SetExtent(0, sdim[0] - 1, 0, sdim[1] - 1, posZ, posZ);
    }
}

void mvManager::SetGridLineColor(double r, double g, double b)
{
    for (int i = 0; i < 3; i++)
    {
        m_GridLines[i]->SetColor(r, g, b);
    }
    m_GridLayer->SetColor(r, g, b);
}

int mvManager::AreGridLinesActive(int i) const
{
    if ((m_DataSource != 0) && (GetGridType() == GridType::MV_LAYERED_GRID))
    {
        return false;
    }
    else
    {
        return m_GridLinesActivated[i];
    }
}

void mvManager::GetGridLinePositions(int *p) const
{
    const int *e0 = m_GridLines[0]->GetExtent();
    const int *e1 = m_GridLines[1]->GetExtent();
    const int *e2 = m_GridLines[2]->GetExtent();
    p[0]          = e0[0];
    p[1]          = e1[2];
    p[2]          = e2[4];
}

const double *mvManager::GetGridLineColor() const
{
    return m_GridLines[0]->GetColor();
}

// Sets the xyz scale for actors.
void mvManager::SetScale(double xScale, double yScale, double zScale)
{
    double s[3];
    m_SolidActor->GetScale(s);
    if (s[0] == xScale && s[1] == yScale && s[2] == zScale)
    {
        return;
    }
    m_SolidActor->SetScale(xScale, yScale, zScale);
    m_IsosurfaceActor->SetScale(xScale, yScale, zScale);
    m_CroppedAwayPiecesActor->SetScale(xScale, yScale, zScale);
    m_VectorActor->SetScale(xScale, yScale, zScale);
    m_VectorGlyphActor->SetScale(xScale, yScale, zScale);
    m_GridShell->SetScale(xScale, yScale, zScale);
    m_BoundingBox->SetScale(xScale, yScale, zScale);
    m_CubeSource->SetXLength(m_CubeSource->GetXLength() * s[0] / xScale);
    m_CubeSource->SetYLength(m_CubeSource->GetYLength() * s[1] / yScale);
    m_CubeSource->SetZLength(m_CubeSource->GetZLength() * s[2] / zScale);
    for (int i = 0; i < 3; i++)
    {
        m_GridLines[i]->SetScale(xScale, yScale, zScale);
    }
    m_GridLayer->SetScale(xScale, yScale, zScale);

    double bounds[6];
    m_BoundingBox->GetBounds(bounds);
    bounds[0] *= xScale;
    bounds[1] *= xScale;
    bounds[2] *= yScale;
    bounds[3] *= yScale;
    bounds[4] *= zScale;
    bounds[5] *= zScale;
    m_Axes->SetDefaultPositions(bounds);
    m_Pathlines->SetScale(xScale, yScale, zScale);
    m_ModelFeatures->SetScale(xScale, yScale, zScale);
    if (m_DataSource->GetVectorArray() != 0)
    {
        UpdateScaledVectorArray();
    }
    m_Overlay->SetScale(xScale, yScale, zScale);
}

const double *mvManager::GetScale() const
{
    return m_SolidActor->GetScale();
}

int mvManager::AreAxesVisible() const
{
    return m_Axes->GetVisibility();
}

void mvManager::ShowAxes()
{
    m_Axes->VisibilityOn();
}

void mvManager::HideAxes()
{
    m_Axes->VisibilityOff();
}

void mvManager::SetAxesNormalizedSize(double sn)
{
    m_Axes->SetNormalizedSize(sn);
}

void mvManager::SetAxesNormalizedTubeDiameter(double dn)
{
    m_Axes->SetNormalizedTubeDiameter(dn);
}

void mvManager::SetAxesNormalizedPosition(double xn, double yn, double zn)
{
    m_Axes->SetNormalizedPosition(xn, yn, zn);
}

void mvManager::SetAxesRepresentationToTube()
{
    m_Axes->SetRepresentationToTube();
}

void mvManager::SetAxesRepresentationToLine()
{
    m_Axes->SetRepresentationToLine();
}

double mvManager::GetAxesNormalizedSize() const
{
    return m_Axes->GetNormalizedSize();
    return 0;
}

double mvManager::GetAxesNormalizedTubeDiameter() const
{
    return m_Axes->GetNormalizedTubeDiameter();
    return 0;
}

const double *mvManager::GetAxesNormalizedPosition() const
{
    return m_Axes->GetNormalizedPosition();
    return 0;
}

int mvManager::GetAxesRepresentation() const
{
    return m_Axes->GetRepresentation();
    return 0;
}

int mvManager::IsTimeLabelVisible() const
{
    return m_TimeLabel->GetVisibility();
}

void mvManager::ShowTimeLabel()
{
    m_TimeLabel->VisibilityOn();
}

void mvManager::HideTimeLabel()
{
    m_TimeLabel->VisibilityOff();
}

void mvManager::SetTimeLabelPosition(double x, double y)
{
    m_TimeLabel->SetPosition(x, y);
}

const double *mvManager::GetTimeLabelPosition() const
{
    return m_TimeLabel->GetPosition();
}

void mvManager::SetTimeLabelFontSize(int size)
{
    m_TimeLabel->SetFontSize(size);
}

int mvManager::GetTimeLabelFontSize() const
{
    return m_TimeLabel->GetFontSize();
}

void mvManager::SetTimeLabelColor(double r, double g, double b)
{
    m_TimeLabel->SetColor(r, g, b);
}

const double *mvManager::GetTimeLabelColor() const
{
    return m_TimeLabel->GetColor();
}

int mvManager::IsTitleVisible() const
{
    return m_Title->GetVisibility();
}

void mvManager::ShowTitle()
{
    m_Title->VisibilityOn();
}

void mvManager::HideTitle()
{
    m_Title->VisibilityOff();
}

void mvManager::SetTitlePosition(double x, double y)
{
    m_Title->SetPosition(x, y);
}

const double *mvManager::GetTitlePosition() const
{
    return m_Title->GetPosition();
}

void mvManager::SetTitleFontSize(int size)
{
    m_Title->SetFontSize(size);
}

int mvManager::GetTitleFontSize() const
{
    return m_Title->GetFontSize();
}

void mvManager::SetTitleColor(double r, double g, double b)
{
    m_Title->SetColor(r, g, b);
}

const double *mvManager::GetTitleColor() const
{
    return m_Title->GetColor();
}

int mvManager::IsColorBarVisible() const
{
    return m_ColorBar->GetVisibility();
}

void mvManager::ShowColorBar()
{
    m_ColorBar->VisibilityOn();
}

void mvManager::HideColorBar()
{
    m_ColorBar->VisibilityOff();
}

void mvManager::AdvanceOneTimePoint()
{
    if (m_DataSource == 0)
    {
        return;
    }

    m_DataSource->AdvanceOneTimePoint();
    m_TimePointIndex++;
    OnDataModified();
}

void mvManager::SetTimePointTo(int timePointIndex)
{
    if (m_DataSource == 0)
    {
        return;
    }
    m_DataSource->SetTimePointTo(timePointIndex);
    m_TimePointIndex = timePointIndex;
    OnDataModified();
}

int mvManager::GetInitialDisplayTimePoint()
{
    if (m_DataSource == 0)
    {
        return 0;
    }
    else
    {
        return m_DataSource->GetInitialDisplayTimePoint();
    }
}

void mvManager::OnDataModified()
{
    m_PointScalars->Modified();
    //	if (!m_DataSource->AreAllCellsActive())
    //	{
    //		m_ScalarStructuredGrid->Modified();
    //	}
    if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        m_ScalarStructuredGrid->Modified();
    }
    else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        m_ScalarLayeredGrid->Modified();
    }
    else if (m_DataSource->GetGridType() == GridType::MV_UNSTRUCTURED_GRID)
    {
        m_ScalarUnstructuredGrid->Modified();
    }
    if (m_DataSource->GetPrimaryScalarMode() == ScalarMode::MV_CELL_SCALARS)
    {
        m_CellScalars->Modified();
    }
    ComputeActiveScalarRange();

    char timeLabel[50];
    if (m_DataSource->GetTimeLabelOption() == 0)
    {
        sprintf(timeLabel, "Time = %s", (m_DataSource->GetTimePointLabels())[m_TimePointIndex]);
    }
    else
    {
        sprintf(timeLabel, "Time Step %s", (m_DataSource->GetTimePointLabels())[m_TimePointIndex]);
    }

    m_TimeLabel->SetText(timeLabel);

    if (m_DataSource->GetVectorArray() != 0)
    {
        ComputeVectorMagnitudes();
        UpdateScaledVectorArray();
    }

    if (m_DataSource->GetNumberOfPathlines() > 0 && m_PathlineTimeClippingMode == 1)
    {
        char **labels      = m_DataSource->GetTimePointLabels();
        double clipTimeMax = atof(labels[m_TimePointIndex]);
        m_Pathlines->SetTimeClippingRange(0, clipTimeMax);
    }

    if (m_DataSource->GetModelFeatureArray())
    {
        m_ModelFeatures->SetModelFeatureArray(m_DataSource->GetModelFeatureArray());
    }
}

void mvManager::UpdatePathlineScalars()
{
    int nc = m_DataSource->GetNumberOfPathlineCoordinates();
    m_PathlineScalars->SetArray(m_DataSource->GetPathlineScalarArray(), nc, 1);
    m_PathlineScalars->Modified();
}

void mvManager::UpdateScaledVectorArray()
{
    double *v = m_DataSource->GetVectorArray();
    if (!v) return;
    const double *s  = GetScale();
    //	const int *vdim = m_DataSource->GetVectorGridDimensions();
    //	int np = vdim[0] * vdim[1] * vdim[2];
    int           np = m_DataSource->GetNumModelCells();
    if (s[0] == 1 && s[1] == 1 && s[2] == 1 && !m_VectorLog10Transform)
    {
        memcpy(m_ScaledVectorArray, v, 3 * np * sizeof(double));
    }
    else
    {
        double InactiveCellValue = m_DataSource->GetInactiveCellValue();
        double vx, vy, vz, vv, factor;
        for (int i = 0; i < np; i++)
        {
            vx = v[3 * i];
            vy = v[3 * i + 1];
            vz = v[3 * i + 2];
            if ((vx == InactiveCellValue) || (vy == InactiveCellValue) || (vz == InactiveCellValue))
            {
                vv = 0;
            }
            else
            {
                vv = vx * vx + vy * vy + vz * vz;
            }
            if (m_VectorLog10Transform && (vv > 0))
            {
                double v   = sqrt(vv);
                double lgv = log10(v);
                if (vx > 0)
                {
                    vx = fabs((vx / v) * lgv);
                }
                else
                {
                    vx = -fabs((vx / v) * lgv);
                }

                if (vy > 0)
                {
                    vy = fabs((vy / v) * lgv);
                }
                else
                {
                    vy = -fabs((vy / v) * lgv);
                }

                if (vz > 0)
                {
                    vz = fabs((vz / v) * lgv);
                }
                else
                {
                    vz = -fabs((vz / v) * lgv);
                }

                vv = vx * vx + vy * vy + vz * vz;
            }

            if (vv > 0)
            {
                factor = sqrt(vv / (vx * vx * s[0] * s[0] +
                                    vy * vy * s[1] * s[1] + vz * vz * s[2] * s[2]));
            }
            else
            {
                factor = 0;
            }
            m_ScaledVectorArray[3 * i]     = vx * factor;
            m_ScaledVectorArray[3 * i + 1] = vy * factor;
            m_ScaledVectorArray[3 * i + 2] = vz * factor;
        }
    }
    m_Vectors->Modified();
}

void mvManager::SetScalarDataTypeTo(int dataTypeIndex)
{
    if (m_DataSource == 0)
    {
        return;
    }
    m_DataSource->SetScalarDataTypeTo(dataTypeIndex);

    int numPoints;
    int numCells;
    if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        numPoints = m_ScalarStructuredGrid->GetNumberOfPoints();
        numCells  = m_ScalarStructuredGrid->GetNumberOfCells();
    }
    else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        numPoints = m_ScalarLayeredGrid->GetNumberOfPoints();
        numCells  = m_ScalarLayeredGrid->GetNumberOfCells();
    }
    else if (m_DataSource->GetGridType() == GridType::MV_UNSTRUCTURED_GRID)
    {
        numPoints = m_ScalarUnstructuredGrid->GetNumberOfPoints();
        numCells  = m_ScalarUnstructuredGrid->GetNumberOfCells();
    }

    // Set Point Data
    m_PointScalars->SetArray(m_DataSource->GetScalarArray(), numPoints, 1);
    m_PointScalars->Modified();

    if (m_DataSource->GetPrimaryScalarMode() == ScalarMode::MV_CELL_SCALARS)
    {
        // Set Cell data
        m_CellScalars->SetArray(m_DataSource->GetScalarArray() + numPoints, numCells, 1);
        m_CellScalars->Modified();
    }
    else
    {
        m_CellScalars->SetArray(m_DataSource->GetScalarArray(), numPoints, 1);
        m_CellScalars->Modified();
    }
    ComputeActiveScalarRange();

    if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        m_ScalarStructuredGrid->Modified();
    }
    else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        m_ScalarLayeredGrid->Modified();
    }
    else if (m_DataSource->GetGridType() == GridType::MV_UNSTRUCTURED_GRID)
    {
        m_ScalarUnstructuredGrid->Modified();
    }

    m_ActiveDataType = dataTypeIndex;

    if (m_ColorBarDataSource == 0)
    {
        SetColorBarEndPoints(m_ColorBarValueBlue[dataTypeIndex], m_ColorBarValueRed[dataTypeIndex]);
    }
    if (m_UseLogColorBar[dataTypeIndex])
    {
        UseLogColorBar();
    }
    else
    {
        UseLinearColorBar();
    }
    SetColorBarNumberOfLabels(m_NumColorBarLabels[dataTypeIndex]);
    SetColorBarLabelPrecision(m_ColorBarLabelPrecision[dataTypeIndex]);

    if (m_SolidDisplayMode[dataTypeIndex] == SolidDisplayType::MV_SOLID_BLOCKY)
    {
        SetSolidDisplayToBlocky();
    }
    else if (m_SolidDisplayMode[dataTypeIndex] == SolidDisplayType::MV_SOLID_BANDED)
    {
        SetSolidDisplayToBanded();
    }
    else
    {
        SetSolidDisplayToSmooth();
    }
    SetNumberOfColorBands(m_NumberOfColorBands[dataTypeIndex]);
    SetSolidThresholdLimits(m_SolidThresholdMin[dataTypeIndex], m_SolidThresholdMax[dataTypeIndex]);

    if (m_DoSolidThreshold[dataTypeIndex])
    {
        SolidThresholdOn();
    }
    else
    {
        SolidThresholdOff();
    }

    if (m_UseRegularIsosurface[dataTypeIndex])
    {
        m_Isosurface->GenerateValues(m_NumberOfRegularIsosurfaces[dataTypeIndex],
                                     m_RegularIsosurfaceMin[dataTypeIndex],
                                     m_RegularIsosurfaceMax[dataTypeIndex]);
    }
    else
    {
        m_Isosurface->SetNumberOfContours(m_NumberOfCustomIsosurfaces[dataTypeIndex]);
        for (int i = 0; i < m_NumberOfCustomIsosurfaces[dataTypeIndex]; i++)
        {
            m_Isosurface->SetValue(i, m_CustomIsosurfaceValues[dataTypeIndex][i]);
        }
    }

    // If cropping isosurfaces, we have to rebuild the pipeline. (Not sure
    // why this is needed, but this fixes a bug that occurs if we don't rebuild
    // the pipeline. ph - Jan 12, 2001)
    if ((m_CropBounds[0] > 0 || m_CropBounds[1] < 1 || m_CropBounds[2] > 0 || m_CropBounds[3] < 1 || m_CropBounds[4] > 0 || m_CropBounds[5] < 1) && m_IsosurfaceActor->GetVisibility())
    {
        BuildPipelineForIsosurface();
    }
}

void mvManager::SetSolidDisplayToBlocky()
{
    m_SolidDisplayMode[m_ActiveDataType] = SolidDisplayType::MV_SOLID_BLOCKY;
    m_SolidMapper->SetScalarModeToUseCellData();
    BuildPipelineForSolid();
}

void mvManager::SetSolidDisplayToSmooth()
{
    m_SolidDisplayMode[m_ActiveDataType] = SolidDisplayType::MV_SOLID_SMOOTH;
    m_SolidMapper->SetScalarModeToUsePointData();
    BuildPipelineForSolid();
}

void mvManager::SetSolidDisplayToBanded()
{
    m_SolidDisplayMode[m_ActiveDataType] = SolidDisplayType::MV_SOLID_BANDED;
    m_SolidMapper->SetScalarModeToUseCellData();
    BuildPipelineForSolid();
}

SolidDisplayType mvManager::GetSolidDisplayMode() const
{
    if (m_SolidDisplayMode != 0)
    {
        return m_SolidDisplayMode[m_ActiveDataType];
    }
    else
    {
        return SolidDisplayType::MV_SOLID_SMOOTH;
    }
}

int mvManager::IsSolidThresholdOn() const
{
    return m_DoSolidThreshold[m_ActiveDataType];
}

void mvManager::GetScalarDataRange(double *range) const
{
    // if (m_DataSource->AreAllCellsActive())
    //{
    //	if (m_DataSource->GetDataSetToUseForRange() == MV_USE_CELL_DATA_FOR_RANGE)
    //	{
    //		m_CellScalars->GetValueRange(range);
    //	}
    //	else
    //	{
    //		m_PointScalars->GetValueRange(range);
    //	}
    // }
    // else
    //{
    range[0] = m_ActiveScalarRange[0];
    range[1] = m_ActiveScalarRange[1];
    //}
}

void mvManager::GetVectorMagnitudeRange(double *range) const
{
    range[0] = m_VectorMagnitudeRange[0];
    range[1] = m_VectorMagnitudeRange[1];
}

void mvManager::SetColorBarEndPoints(double valueBlue, double valueRed)
{
    if (m_NumberOfColorBands == 0)
    {
        return;
    }
    if (m_ColorBarDataSource == 1)
    {
        SetPathlineColorBarEndPoints(valueBlue, valueRed);
    }
    double range[2];
    if (valueRed > valueBlue)
    {
        range[0] = valueBlue;
        range[1] = valueRed;
        m_LutBlueToRed->SetRange(range[0], range[1]);
        m_LutModifiedBlueToRed->SetRange(range[0], range[1]);
        m_LutCustomScale->SetRange(range[0], range[1]);
        if (range[0] * range[1] > 0)
        {
            m_LogLutBlueToRed->SetRange(range[0], range[1]);
            m_LogLutModifiedBlueToRed->SetRange(range[0], range[1]);
            m_LogLutCustomScale->SetRange(range[0], range[1]);
        }
        if (m_ColorBarDataSource == 1)
        {
            if (m_Pathlines->GetLogTransform() == 0)
            {
                m_ColorBar->SetLookupTable(m_LutBlueToRed);
            }
            else
            {
                m_ColorBar->SetLookupTable(m_LogLutBlueToRed);
            }
        }
        else if (m_SolidMapper->GetLookupTable() == m_LutBlueToRed ||
                 m_SolidMapper->GetLookupTable() == m_LutRedToBlue)
        {
            m_SolidMapper->SetLookupTable(m_LutBlueToRed);
            m_IsosurfaceMapper->SetLookupTable(m_LutBlueToRed);
            m_ColorBar->SetLookupTable(m_LutBlueToRed);
        }
        else if (m_SolidMapper->GetLookupTable() == m_LutModifiedRedToBlue ||
                 m_SolidMapper->GetLookupTable() == m_LutModifiedBlueToRed)
        {
            m_SolidMapper->SetLookupTable(m_LutModifiedBlueToRed);
            m_IsosurfaceMapper->SetLookupTable(m_LutModifiedBlueToRed);
            m_ColorBar->SetLookupTable(m_LutModifiedBlueToRed);
        }
        else if (m_SolidMapper->GetLookupTable() == m_LutCustomScale ||
                 m_SolidMapper->GetLookupTable() == m_LutReversedCustomScale)
        {
            m_SolidMapper->SetLookupTable(m_LutCustomScale);
            m_IsosurfaceMapper->SetLookupTable(m_LutCustomScale);
            m_ColorBar->SetLookupTable(m_LutCustomScale);
        }

        else if (m_SolidMapper->GetLookupTable() == m_LogLutRedToBlue ||
                 m_SolidMapper->GetLookupTable() == m_LogLutBlueToRed)
        {
            m_SolidMapper->SetLookupTable(m_LogLutBlueToRed);
            m_IsosurfaceMapper->SetLookupTable(m_LogLutBlueToRed);
            m_ColorBar->SetLookupTable(m_LogLutBlueToRed);
        }
        else if (m_SolidMapper->GetLookupTable() == m_LogLutModifiedRedToBlue ||
                 m_SolidMapper->GetLookupTable() == m_LogLutModifiedBlueToRed)
        {
            m_SolidMapper->SetLookupTable(m_LogLutModifiedBlueToRed);
            m_IsosurfaceMapper->SetLookupTable(m_LogLutModifiedBlueToRed);
            m_ColorBar->SetLookupTable(m_LogLutModifiedBlueToRed);
        }
        else
        {
            m_SolidMapper->SetLookupTable(m_LogLutCustomScale);
            m_IsosurfaceMapper->SetLookupTable(m_LogLutCustomScale);
            m_ColorBar->SetLookupTable(m_LogLutCustomScale);
        }
    }
    else
    {
        range[0] = valueRed;
        range[1] = valueBlue;
        m_LutRedToBlue->SetRange(range[0], range[1]);
        m_LutModifiedRedToBlue->SetRange(range[0], range[1]);
        m_LutReversedCustomScale->SetRange(range[0], range[1]);
        if (range[0] * range[1] > 0)
        {
            m_LogLutRedToBlue->SetRange(range[0], range[1]);
            m_LogLutModifiedRedToBlue->SetRange(range[0], range[1]);
            m_LogLutReversedCustomScale->SetRange(range[0], range[1]);
        }
        if (m_ColorBarDataSource == 1)
        {
            if (m_Pathlines->GetLogTransform() == 0)
            {
                m_ColorBar->SetLookupTable(m_LutRedToBlue);
            }
            else
            {
                m_ColorBar->SetLookupTable(m_LogLutRedToBlue);
            }
        }
        else if (m_SolidMapper->GetLookupTable() == m_LutBlueToRed ||
                 m_SolidMapper->GetLookupTable() == m_LutRedToBlue)
        {
            m_SolidMapper->SetLookupTable(m_LutRedToBlue);
            m_IsosurfaceMapper->SetLookupTable(m_LutRedToBlue);
            m_ColorBar->SetLookupTable(m_LutRedToBlue);
        }
        else if (m_SolidMapper->GetLookupTable() == m_LutModifiedRedToBlue ||
                 m_SolidMapper->GetLookupTable() == m_LutModifiedBlueToRed)
        {
            m_SolidMapper->SetLookupTable(m_LutModifiedRedToBlue);
            m_IsosurfaceMapper->SetLookupTable(m_LutModifiedRedToBlue);
            m_ColorBar->SetLookupTable(m_LutModifiedRedToBlue);
        }
        else if (m_SolidMapper->GetLookupTable() == m_LutCustomScale ||
                 m_SolidMapper->GetLookupTable() == m_LutReversedCustomScale)
        {
            m_SolidMapper->SetLookupTable(m_LutReversedCustomScale);
            m_IsosurfaceMapper->SetLookupTable(m_LutReversedCustomScale);
            m_ColorBar->SetLookupTable(m_LutReversedCustomScale);
        }
        else if (m_SolidMapper->GetLookupTable() == m_LogLutRedToBlue ||
                 m_SolidMapper->GetLookupTable() == m_LogLutBlueToRed)
        {
            m_SolidMapper->SetLookupTable(m_LogLutRedToBlue);
            m_IsosurfaceMapper->SetLookupTable(m_LogLutRedToBlue);
            m_ColorBar->SetLookupTable(m_LogLutRedToBlue);
        }
        else if (m_SolidMapper->GetLookupTable() == m_LogLutModifiedRedToBlue ||
                 m_SolidMapper->GetLookupTable() == m_LogLutModifiedBlueToRed)
        {
            m_SolidMapper->SetLookupTable(m_LogLutModifiedRedToBlue);
            m_IsosurfaceMapper->SetLookupTable(m_LogLutModifiedRedToBlue);
            m_ColorBar->SetLookupTable(m_LogLutModifiedRedToBlue);
        }
        else
        {
            m_SolidMapper->SetLookupTable(m_LogLutReversedCustomScale);
            m_IsosurfaceMapper->SetLookupTable(m_LogLutReversedCustomScale);
            m_ColorBar->SetLookupTable(m_LogLutReversedCustomScale);
        }
    }
    m_ColorBar->SetDataSourceIndex(m_ColorBarDataSource);
    if (m_ColorBarDataSource == 0)
    {
        m_SolidMapper->SetScalarRange(range);
        m_IsosurfaceMapper->SetScalarRange(range);
        if (m_NumberOfColorBands[m_ActiveDataType] > 0)
        {
            UpdateColorBands();
        }
        m_ColorBarValueBlue[m_ActiveDataType] = valueBlue;
        m_ColorBarValueRed[m_ActiveDataType]  = valueRed;
    }
    else
    {
        if (valueBlue <= 0)
        {
            valueBlue = 1e-40;
        }
        if (valueRed <= 0)
        {
            valueRed = 1e-40;
        }
        m_ColorBar->SetScalarRange(valueBlue, valueRed);
    }
}

void mvManager::UseLinearColorBar()
{
    if (m_SolidMapper->GetLookupTable() == m_LogLutBlueToRed)
    {
        m_SolidMapper->SetLookupTable(m_LutBlueToRed);
        m_IsosurfaceMapper->SetLookupTable(m_LutBlueToRed);
        m_ColorBar->SetLookupTable(m_LutBlueToRed);
        if (m_NumberOfColorBands[m_ActiveDataType] > 0)
        {
            UpdateColorBands();
        }
    }
    else if (m_SolidMapper->GetLookupTable() == m_LogLutRedToBlue)
    {
        m_SolidMapper->SetLookupTable(m_LutRedToBlue);
        m_IsosurfaceMapper->SetLookupTable(m_LutRedToBlue);
        m_ColorBar->SetLookupTable(m_LutRedToBlue);
        if (m_NumberOfColorBands[m_ActiveDataType] > 0)
        {
            UpdateColorBands();
        }
    }
    else if (m_SolidMapper->GetLookupTable() == m_LogLutModifiedRedToBlue)
    {
        m_SolidMapper->SetLookupTable(m_LutModifiedRedToBlue);
        m_IsosurfaceMapper->SetLookupTable(m_LutModifiedRedToBlue);
        m_ColorBar->SetLookupTable(m_LutModifiedRedToBlue);
        if (m_NumberOfColorBands[m_ActiveDataType] > 0)
        {
            UpdateColorBands();
        }
    }
    else if (m_SolidMapper->GetLookupTable() == m_LogLutModifiedBlueToRed)
    {
        m_SolidMapper->SetLookupTable(m_LutModifiedBlueToRed);
        m_IsosurfaceMapper->SetLookupTable(m_LutModifiedBlueToRed);
        m_ColorBar->SetLookupTable(m_LutModifiedBlueToRed);
        if (m_NumberOfColorBands[m_ActiveDataType] > 0)
        {
            UpdateColorBands();
        }
    }
    else if (m_SolidMapper->GetLookupTable() == m_LogLutCustomScale)
    {
        m_SolidMapper->SetLookupTable(m_LutCustomScale);
        m_IsosurfaceMapper->SetLookupTable(m_LutCustomScale);
        m_ColorBar->SetLookupTable(m_LutCustomScale);
        if (m_NumberOfColorBands[m_ActiveDataType] > 0)
        {
            UpdateColorBands();
        }
    }
    else if (m_SolidMapper->GetLookupTable() == m_LogLutReversedCustomScale)
    {
        m_SolidMapper->SetLookupTable(m_LutReversedCustomScale);
        m_IsosurfaceMapper->SetLookupTable(m_LutReversedCustomScale);
        m_ColorBar->SetLookupTable(m_LutReversedCustomScale);
        if (m_NumberOfColorBands[m_ActiveDataType] > 0)
        {
            UpdateColorBands();
        }
    }
    m_UseLogColorBar[m_ActiveDataType] = 0;
}

void mvManager::UseLogColorBar()
{
    if (m_SolidMapper->GetLookupTable() == m_LutBlueToRed)
    {
        m_SolidMapper->SetLookupTable(m_LogLutBlueToRed);
        m_IsosurfaceMapper->SetLookupTable(m_LogLutBlueToRed);
        m_ColorBar->SetLookupTable(m_LogLutBlueToRed);
        if (m_NumberOfColorBands[m_ActiveDataType] > 0)
        {
            UpdateColorBands();
        }
    }
    else if (m_SolidMapper->GetLookupTable() == m_LutRedToBlue)
    {
        m_SolidMapper->SetLookupTable(m_LogLutRedToBlue);
        m_IsosurfaceMapper->SetLookupTable(m_LogLutRedToBlue);
        m_ColorBar->SetLookupTable(m_LogLutRedToBlue);
        if (m_NumberOfColorBands[m_ActiveDataType] > 0)
        {
            UpdateColorBands();
        }
    }
    else if (m_SolidMapper->GetLookupTable() == m_LutModifiedRedToBlue)
    {
        m_SolidMapper->SetLookupTable(m_LogLutModifiedRedToBlue);
        m_IsosurfaceMapper->SetLookupTable(m_LogLutModifiedRedToBlue);
        m_ColorBar->SetLookupTable(m_LogLutModifiedRedToBlue);
        if (m_NumberOfColorBands[m_ActiveDataType] > 0)
        {
            UpdateColorBands();
        }
    }
    else if (m_SolidMapper->GetLookupTable() == m_LutModifiedBlueToRed)
    {
        m_SolidMapper->SetLookupTable(m_LogLutModifiedBlueToRed);
        m_IsosurfaceMapper->SetLookupTable(m_LogLutModifiedBlueToRed);
        m_ColorBar->SetLookupTable(m_LogLutModifiedBlueToRed);
        if (m_NumberOfColorBands[m_ActiveDataType] > 0)
        {
            UpdateColorBands();
        }
    }
    else if (m_SolidMapper->GetLookupTable() == m_LutCustomScale)
    {
        m_SolidMapper->SetLookupTable(m_LogLutCustomScale);
        m_IsosurfaceMapper->SetLookupTable(m_LogLutCustomScale);
        m_ColorBar->SetLookupTable(m_LogLutCustomScale);
        if (m_NumberOfColorBands[m_ActiveDataType] > 0)
        {
            UpdateColorBands();
        }
    }
    else if (m_SolidMapper->GetLookupTable() == m_LutReversedCustomScale)
    {
        m_SolidMapper->SetLookupTable(m_LogLutReversedCustomScale);
        m_IsosurfaceMapper->SetLookupTable(m_LogLutReversedCustomScale);
        m_ColorBar->SetLookupTable(m_LogLutReversedCustomScale);
        if (m_NumberOfColorBands[m_ActiveDataType] > 0)
        {
            UpdateColorBands();
        }
    }
    m_UseLogColorBar[m_ActiveDataType] = 1;
}

double mvManager::GetColorBarValueBlue() const
{
    if (m_ColorBarDataSource == 1)
    {
        return GetPathlineTimeBlue();
    }
    else
    {
        double *range = m_SolidMapper->GetScalarRange();
        if (
            (m_SolidMapper->GetLookupTable() == m_LutBlueToRed) ||
            (m_SolidMapper->GetLookupTable() == m_LutModifiedBlueToRed) ||
            (m_SolidMapper->GetLookupTable() == m_LutReversedCustomScale) ||
            (m_SolidMapper->GetLookupTable() == m_LogLutBlueToRed) ||
            (m_SolidMapper->GetLookupTable() == m_LogLutModifiedBlueToRed) ||
            (m_SolidMapper->GetLookupTable() == m_LogLutReversedCustomScale))
        {
            return range[0];
        }
        else
        {
            return range[1];
        }
    }
}

double mvManager::GetColorBarValueRed() const
{
    if (m_ColorBarDataSource == 1)
    {
        return GetPathlineTimeRed();
    }
    else
    {
        double *range = m_SolidMapper->GetScalarRange();
        if (
            (m_SolidMapper->GetLookupTable() == m_LutBlueToRed) ||
            (m_SolidMapper->GetLookupTable() == m_LutModifiedBlueToRed) ||
            (m_SolidMapper->GetLookupTable() == m_LutReversedCustomScale) ||
            (m_SolidMapper->GetLookupTable() == m_LogLutBlueToRed) ||
            (m_SolidMapper->GetLookupTable() == m_LogLutModifiedBlueToRed) ||
            (m_SolidMapper->GetLookupTable() == m_LogLutReversedCustomScale))
        {
            return range[1];
        }
        else
        {
            return range[0];
        }
    }
}

int mvManager::IsColorBarLinear() const
{
    if (m_ColorBarDataSource == 0)
    {
        return ((m_SolidMapper->GetLookupTable() == m_LutBlueToRed) ||
                (m_SolidMapper->GetLookupTable() == m_LutRedToBlue) ||
                (m_SolidMapper->GetLookupTable() == m_LutModifiedRedToBlue) ||
                (m_SolidMapper->GetLookupTable() == m_LutModifiedBlueToRed) ||
                (m_SolidMapper->GetLookupTable() == m_LutCustomScale) ||
                (m_SolidMapper->GetLookupTable() == m_LutReversedCustomScale));
    }
    else
    {
        return !m_Pathlines->GetLogTransform();
    }
}

bool mvManager::IsColorBarNormal() const
{
    return ((m_SolidMapper->GetLookupTable() == m_LutBlueToRed) ||
            (m_SolidMapper->GetLookupTable() == m_LutModifiedBlueToRed) ||
            (m_SolidMapper->GetLookupTable() == m_LogLutBlueToRed) ||
            (m_SolidMapper->GetLookupTable() == m_LogLutModifiedBlueToRed) ||
            (m_SolidMapper->GetLookupTable() == m_LutCustomScale) ||
            (m_SolidMapper->GetLookupTable() == m_LogLutCustomScale));
}

void mvManager::SetColorBarWidth(int w)
{
    m_ColorBar->SetWidth(w);
}

void mvManager::SetColorBarHeight(int h)
{
    m_ColorBar->SetHeight(h);
}

void mvManager::SetColorBarFontSize(int f)
{
    m_ColorBar->SetFontSize(f);
}

void mvManager::SetColorBarOffset(int r)
{
    m_ColorBar->SetOffset(r);
}

void mvManager::SetColorBarTextColor(double r, double g, double b)
{
    m_ColorBar->SetTextColor(r, g, b);
}

void mvManager::SetColorBarNumberOfLabels(int n)
{
    m_ColorBar->SetNumberOfLabels(n);
    if (m_NumColorBarLabels) m_NumColorBarLabels[m_ActiveDataType] = n;
}

void mvManager::SetColorBarLabelPrecision(int d)
{
    m_ColorBar->SetLabelPrecision(d);
    if (m_ColorBarLabelPrecision) m_ColorBarLabelPrecision[m_ActiveDataType] = d;
}

void mvManager::SetColorBarColorScheme(int value)
{
    m_ColorBar->SetColorScheme(value);
    switch (value)
    {
    case MV_DEFAULT_COLOR_SCHEME:
        if (IsColorBarNormal())
        {
            if (IsColorBarLinear())
            {
                m_SolidMapper->SetLookupTable(m_LutBlueToRed);
                m_IsosurfaceMapper->SetLookupTable(m_LutBlueToRed);
                m_ColorBar->SetLookupTable(m_LutBlueToRed);
            }
            else
            {
                m_SolidMapper->SetLookupTable(m_LogLutBlueToRed);
                m_IsosurfaceMapper->SetLookupTable(m_LogLutBlueToRed);
                m_ColorBar->SetLookupTable(m_LogLutBlueToRed);
            }
        }
        else
        {
            if (IsColorBarLinear())
            {
                m_SolidMapper->SetLookupTable(m_LutRedToBlue);
                m_IsosurfaceMapper->SetLookupTable(m_LutRedToBlue);
                m_ColorBar->SetLookupTable(m_LutRedToBlue);
            }
            else
            {
                m_SolidMapper->SetLookupTable(m_LogLutRedToBlue);
                m_IsosurfaceMapper->SetLookupTable(m_LogLutRedToBlue);
                m_ColorBar->SetLookupTable(m_LogLutRedToBlue);
            }
        }
        break;
    case MV_MODIFIED_COLOR_SCHEME:
        if (IsColorBarNormal())
        {
            if (IsColorBarLinear())
            {
                m_SolidMapper->SetLookupTable(m_LutModifiedBlueToRed);
                m_IsosurfaceMapper->SetLookupTable(m_LutModifiedBlueToRed);
                m_ColorBar->SetLookupTable(m_LutModifiedBlueToRed);
            }
            else
            {
                m_SolidMapper->SetLookupTable(m_LogLutModifiedBlueToRed);
                m_IsosurfaceMapper->SetLookupTable(m_LogLutModifiedBlueToRed);
                m_ColorBar->SetLookupTable(m_LogLutModifiedBlueToRed);
            }
        }
        else
        {
            if (IsColorBarLinear())
            {
                m_SolidMapper->SetLookupTable(m_LutModifiedRedToBlue);
                m_IsosurfaceMapper->SetLookupTable(m_LutModifiedRedToBlue);
                m_ColorBar->SetLookupTable(m_LutModifiedRedToBlue);
            }
            else
            {
                m_SolidMapper->SetLookupTable(m_LogLutModifiedRedToBlue);
                m_IsosurfaceMapper->SetLookupTable(m_LogLutModifiedRedToBlue);
                m_ColorBar->SetLookupTable(m_LogLutModifiedRedToBlue);
            }
        }
        break;
    case MV_CUSTOM_COLOR_SCHEME:
        if (IsColorBarNormal())
        {
            if (IsColorBarLinear())
            {
                m_SolidMapper->SetLookupTable(m_LutCustomScale);
                m_IsosurfaceMapper->SetLookupTable(m_LutCustomScale);
                m_ColorBar->SetLookupTable(m_LutCustomScale);
            }
            else
            {
                m_SolidMapper->SetLookupTable(m_LogLutCustomScale);
                m_IsosurfaceMapper->SetLookupTable(m_LogLutCustomScale);
                m_ColorBar->SetLookupTable(m_LogLutCustomScale);
            }
        }
        else
        {
            if (IsColorBarLinear())
            {
                m_SolidMapper->SetLookupTable(m_LutReversedCustomScale);
                m_IsosurfaceMapper->SetLookupTable(m_LutReversedCustomScale);
                m_ColorBar->SetLookupTable(m_LutReversedCustomScale);
            }
            else
            {
                m_SolidMapper->SetLookupTable(m_LogLutReversedCustomScale);
                m_IsosurfaceMapper->SetLookupTable(m_LogLutReversedCustomScale);
                m_ColorBar->SetLookupTable(m_LogLutReversedCustomScale);
            }
        }
        break;
    }
}

int mvManager::GetColorBarWidth() const
{
    return m_ColorBar->GetWidth();
}

int mvManager::GetColorBarHeight() const
{
    return m_ColorBar->GetHeight();
}

int mvManager::GetColorBarFontSize() const
{
    return m_ColorBar->GetFontSize();
}

int mvManager::GetColorBarOffset() const
{
    return m_ColorBar->GetOffset();
}

const double *mvManager::GetColorBarTextColor() const
{
    return m_ColorBar->GetTextColor();
}

int mvManager::GetColorBarNumberOfLabels() const
{
    return m_ColorBar->GetNumberOfLabels();
}

int mvManager::GetColorBarLabelPrecision() const
{
    return m_ColorBar->GetLabelPrecision();
}

int mvManager::GetColorBarColorScheme() const
{
    return m_ColorBar->GetColorScheme();
}

std::uint32_t mvManager::GetColorBarFirstCustomColor() const
{
    return dynamic_cast<mvColorTable *>(m_LutCustomScale)->GetFirstCustomColor();
}

std::uint32_t mvManager::GetColorBarLastCustomColor() const
{
    return dynamic_cast<mvColorTable *>(m_LutCustomScale)->GetLastCustomColor();
}

void mvManager::SetColorBarFirstCustomColor(std::uint32_t value)
{
    dynamic_cast<mvColorTable *>(m_LutCustomScale)->SetFirstCustomColor(value);
    dynamic_cast<mvColorTable *>(m_LutCustomScale)->SetCustomColorScheme();

    dynamic_cast<mvColorTable *>(m_LutReversedCustomScale)->SetFirstCustomColor(value);
    dynamic_cast<mvColorTable *>(m_LutReversedCustomScale)->SetReversedCustomColorScheme();

    dynamic_cast<mvLogColorTable *>(m_LogLutCustomScale)->SetFirstCustomColor(value);
    dynamic_cast<mvLogColorTable *>(m_LogLutCustomScale)->SetCustomColorScheme();

    dynamic_cast<mvLogColorTable *>(m_LogLutReversedCustomScale)->SetFirstCustomColor(value);
    dynamic_cast<mvLogColorTable *>(m_LogLutReversedCustomScale)->SetReversedCustomColorScheme();
}

void mvManager::SetColorBarLastCustomColor(std::uint32_t value)
{
    dynamic_cast<mvColorTable *>(m_LutCustomScale)->SetLastCustomColor(value);
    dynamic_cast<mvColorTable *>(m_LutCustomScale)->SetCustomColorScheme();

    dynamic_cast<mvColorTable *>(m_LutReversedCustomScale)->SetLastCustomColor(value);
    dynamic_cast<mvColorTable *>(m_LutReversedCustomScale)->SetReversedCustomColorScheme();

    dynamic_cast<mvLogColorTable *>(m_LogLutCustomScale)->SetLastCustomColor(value);
    dynamic_cast<mvLogColorTable *>(m_LogLutCustomScale)->SetCustomColorScheme();

    dynamic_cast<mvLogColorTable *>(m_LogLutReversedCustomScale)->SetLastCustomColor(value);
    dynamic_cast<mvLogColorTable *>(m_LogLutReversedCustomScale)->SetReversedCustomColorScheme();
}

int mvManager::GetColorBarSource() const
{
    return m_ColorBarDataSource;
}

void mvManager::SetColorBarSource(int value)
{
    m_ColorBarDataSource = value;
}

void mvManager::SetDiffuseLighting(double diffuse)
{
    m_SolidActor->GetProperty()->SetDiffuse(diffuse);
    m_IsosurfaceActor->GetProperty()->SetDiffuse(diffuse);
    m_CroppedAwayPiecesActor->GetProperty()->SetDiffuse(diffuse);
    m_GridShell->SetDiffuse(diffuse);
    m_ModelFeatures->SetDiffuse(diffuse);
}

void mvManager::SetAmbientLighting(double ambient)
{
    m_SolidActor->GetProperty()->SetAmbient(ambient);
    m_IsosurfaceActor->GetProperty()->SetAmbient(ambient);
    m_CroppedAwayPiecesActor->GetProperty()->SetAmbient(ambient);
    m_GridShell->SetAmbient(ambient);
    m_ModelFeatures->SetAmbient(ambient);
}

void mvManager::SetSpecularLighting(double specular)
{
    m_SolidActor->GetProperty()->SetSpecular(specular);
    m_IsosurfaceActor->GetProperty()->SetSpecular(specular);
    m_CroppedAwayPiecesActor->GetProperty()->SetSpecular(specular);
    m_GridShell->SetSpecular(specular);
    m_ModelFeatures->SetSpecular(specular);
}

void mvManager::SetSpecularPower(double specularPower)
{
    m_SolidActor->GetProperty()->SetSpecularPower(specularPower);
    m_IsosurfaceActor->GetProperty()->SetSpecularPower(specularPower);
    m_CroppedAwayPiecesActor->GetProperty()->SetSpecularPower(specularPower);
    m_GridShell->SetSpecularPower(specularPower);
    m_ModelFeatures->SetSpecularPower(specularPower);
}

double mvManager::GetDiffuseLighting() const
{
    return m_SolidActor->GetProperty()->GetDiffuse();
}

double mvManager::GetAmbientLighting() const
{
    return m_SolidActor->GetProperty()->GetAmbient();
}

double mvManager::GetSpecularLighting() const
{
    return m_SolidActor->GetProperty()->GetSpecular();
}

double mvManager::GetSpecularPower() const
{
    return m_SolidActor->GetProperty()->GetSpecularPower();
}

void mvManager::ComputeActiveScalarRange()
{
    vtkDoubleArray *a;
    double          inactive = m_DataSource->GetInactiveCellValue();
    if (m_DataSource->GetDataSetToUseForRange() == MV_USE_CELL_DATA_FOR_RANGE)
    {
        a = m_CellScalars;
    }
    else
    {
        a = m_PointScalars;
    }
    m_ActiveScalarRange[0] = 0;
    m_ActiveScalarRange[1] = 0;
    double v;
    int    start = 1;
    for (int i = 0; i < a->GetNumberOfTuples(); i++)
    {
        v = a->GetValue(i);
        if (v != inactive)
        {
            if (start)
            {
                m_ActiveScalarRange[0] = v;
                m_ActiveScalarRange[1] = v;
                start                  = 0;
            }
            else
            {
                if (v < m_ActiveScalarRange[0])
                {
                    m_ActiveScalarRange[0] = v;
                }
                else if (v > m_ActiveScalarRange[1])
                {
                    m_ActiveScalarRange[1] = v;
                }
            }
        }
    }
}

void mvManager::SetLogTransformVector(int Value)
{
    if (m_VectorLog10Transform != Value)
    {
        double scalefactor        = GetVectorScaleFactor();
        double optimalscalefactor = ComputeOptimalVectorSize();
        double ratio;
        if (optimalscalefactor > 0)
        {
            ratio = scalefactor / optimalscalefactor;
        }
        else
        {
            ratio = 0;
        }

        m_VectorLog10Transform = Value;

        int np                 = m_DataSource->GetNumModelCells();
        if (m_VectorLog10Transform)
        {
            m_VectorMagnitudes->SetArray(m_VectorLogMagnitudeArray, np, 1);
        }
        else
        {
            m_VectorMagnitudes->SetArray(m_VectorMagnitudeArray, np, 1);
        }
        m_VectorMagnitudes->Modified();

        optimalscalefactor = ComputeOptimalVectorSize();
        scalefactor        = optimalscalefactor * ratio;

        UpdateScaledVectorArray();

        SetVectorScaleFactor(scalefactor);
        double limits[2];
        GetVectorThresholdLimits(limits);

        SetVectorThresholdLimits(limits[0], limits[1]);
    }
}

void mvManager::ComputeVectorMagnitudes()
{
    m_VectorMagnitudeRange[0]    = 0;
    m_VectorMagnitudeRange[1]    = 0;
    m_VectorLogMagnitudeRange[0] = 0;
    m_VectorLogMagnitudeRange[1] = 0;
    double  minPosValue          = 0;
    double *v                    = m_DataSource->GetVectorArray();
    if (v == 0)
    {
        return;
    }

    int    np       = m_DataSource->GetNumModelCells();
    double inactive = m_DataSource->GetInactiveCellValue();
    int    m;
    double mag;

    int    start    = 1;
    int    startmin = 1;
    for (int i = 0; i < np; i++)
    {
        m = 3 * i;
        if (v[m] == inactive)
        {
            m_VectorMagnitudeArray[i] = -2;
        }
        else
        {
            mag                       = sqrt(v[m] * v[m] + v[m + 1] * v[m + 1] + v[m + 2] * v[m + 2]);
            m_VectorMagnitudeArray[i] = mag;
            if (start)
            {
                m_VectorMagnitudeRange[0] = mag;
                m_VectorMagnitudeRange[1] = mag;
                start                     = 0;
            }
            else
            {
                if (mag < m_VectorMagnitudeRange[0])
                {
                    m_VectorMagnitudeRange[0] = mag;
                }
                else if (mag > m_VectorMagnitudeRange[1])
                {
                    m_VectorMagnitudeRange[1] = mag;
                }
            }
            if (mag > 0)
            {
                if (startmin)
                {
                    minPosValue = mag;
                    startmin    = 0;
                }
                else
                {
                    if (mag < minPosValue)
                    {
                        minPosValue = mag;
                    }
                }
            }
        }
    }

    m_MinPositiveVector = minPosValue;
    minPosValue         = minPosValue / 2;
    for (int i = 0; i < np; i++)
    {
        m = 3 * i;
        if ((v[m] != inactive) && (minPosValue > 0) && (m_VectorMagnitudeArray[i] > 0))
        {
            m_VectorLogMagnitudeArray[i] = log10(m_VectorMagnitudeArray[i] / minPosValue);
        }
        else
        {
            m_VectorLogMagnitudeArray[i] = 0;
        }
    }
    if (m_VectorMagnitudeRange[0] > 0)
    {
        m_VectorLogMagnitudeRange[0] = log10(m_VectorMagnitudeRange[0] / minPosValue);
    }
    else
    {
        m_VectorLogMagnitudeRange[0] = 0;
    }
    if (m_VectorMagnitudeRange[1] > 0)
    {
        m_VectorLogMagnitudeRange[1] = log10(m_VectorMagnitudeRange[1] / minPosValue);
    }
    else
    {
        m_VectorLogMagnitudeRange[1] = 0;
    }
}

void mvManager::SolidThresholdOn()
{
    m_DoSolidThreshold[m_ActiveDataType] = 1;
    BuildPipelineForSolid();
}

void mvManager::SolidThresholdOff()
{
    m_DoSolidThreshold[m_ActiveDataType] = 0;
    BuildPipelineForSolid();
}

void mvManager::SetSolidThresholdLimits(double minValue, double maxValue)
{
#if ((VTK_MAJOR_VERSION == 9) && (VTK_MINOR_VERSION < 1) || (VTK_MAJOR_VERSION < 9))
    m_BlockySolidThreshold->ThresholdBetween(minValue, maxValue);    // deprecated as of VTK 9.1
#else
    m_BlockySolidThreshold->SetThresholdFunction(vtkThreshold::THRESHOLD_BETWEEN);
    m_BlockySolidThreshold->SetLowerThreshold(minValue);
    m_BlockySolidThreshold->SetUpperThreshold(maxValue);
#endif
    m_GridShellClipMin->SetValue(minValue);
    m_GridShellClipMax->SetValue(maxValue);
    m_SmoothSolidIsosurface->GenerateValues(2, minValue, maxValue);
    m_FacesClipMin->SetValue(minValue);
    m_FacesClipMax->SetValue(maxValue);
#if ((VTK_MAJOR_VERSION == 9) && (VTK_MINOR_VERSION < 1) || (VTK_MAJOR_VERSION < 9))
    m_FacesThreshold->ThresholdBetween(minValue, maxValue);    // deprecated as of VTK 9.1
#else
    m_FacesThreshold->SetThresholdFunction(vtkThreshold::THRESHOLD_BETWEEN);
    m_FacesThreshold->SetLowerThreshold(minValue);
    m_FacesThreshold->SetUpperThreshold(maxValue);
#endif
    m_SolidThresholdMin[m_ActiveDataType] = minValue;
    m_SolidThresholdMax[m_ActiveDataType] = maxValue;
}

void mvManager::GetSolidThresholdLimits(double *limits) const
{
    limits[0] = m_BlockySolidThreshold->GetLowerThreshold();
    limits[1] = m_BlockySolidThreshold->GetUpperThreshold();
}

void mvManager::Crop(double xMin, double xMax, double yMin, double yMax,
                     double zMin, double zMax, double cropAngle)
{
    m_CropBounds[0] = xMin;
    m_CropBounds[1] = xMax;
    m_CropBounds[2] = yMin;
    m_CropBounds[3] = yMax;
    m_CropBounds[4] = zMin;
    m_CropBounds[5] = zMax;
    if (cropAngle > 45)
    {
        m_CropAngle = 45;
    }
    else if (cropAngle < -45)
    {
        m_CropAngle = -45;
    }
    else
    {
        m_CropAngle = cropAngle;
    }
    UpdateCrop();
}

void mvManager::UpdateCrop()
{
    const double *bounds = m_BoundingBox->GetBounds();
    double        dx     = bounds[1] - bounds[0];
    double        dy     = bounds[3] - bounds[2];
    double        dz     = bounds[5] - bounds[4];
    double        midx   = bounds[0] + dx / 2;
    double        midy   = bounds[2] + dy / 2;
    double        midz   = bounds[4] + dz / 2;
    double        sn     = sin(m_CropAngle * 1.745329e-2);
    double        cs     = cos(m_CropAngle * 1.745329e-2);
    m_Plane[0]->SetNormal(cs, sn, 0);
    m_Plane[1]->SetNormal(-cs, -sn, 0);
    m_Plane[2]->SetNormal(-sn, cs, 0);
    m_Plane[3]->SetNormal(sn, -cs, 0);
    m_Overlay->SetCropAngle(m_CropAngle);
    double sy = fabs(sn * dy);
    double sx = fabs(sn * dx);
    if (m_CropBounds[0] == 0)
    {
        m_Plane[0]->SetOrigin(bounds[0] - sy / 2, midy, midz);
        m_Overlay->SetCropperPlaneOrigin(0, bounds[0] - sy / 2, midy, midz);
    }
    else
    {
        m_Plane[0]->SetOrigin(bounds[0] - sy / 2 + m_CropBounds[0] * (dx + sy), midy, midz);
        m_Overlay->SetCropperPlaneOrigin(0, bounds[0] - sy / 2 + m_CropBounds[0] * (dx + sy), midy, midz);
    }
    if (m_CropBounds[1] == 1)
    {
        m_Plane[1]->SetOrigin(bounds[1] + sy / 2, midy, midz);
        m_Overlay->SetCropperPlaneOrigin(1, bounds[1] + sy / 2, midy, midz);
    }
    else
    {
        m_Plane[1]->SetOrigin(bounds[0] - sy / 2 + m_CropBounds[1] * (dx + sy), midy, midz);
        m_Overlay->SetCropperPlaneOrigin(1, bounds[0] - sy / 2 + m_CropBounds[1] * (dx + sy), midy, midz);
    }

    if (m_CropBounds[2] == 0)
    {
        m_Plane[2]->SetOrigin(midx, bounds[2] - sx / 2, midz);
        m_Overlay->SetCropperPlaneOrigin(2, midx, bounds[2] - sx / 2, midz);
    }
    else
    {
        m_Plane[2]->SetOrigin(midx, bounds[2] - sx / 2 + m_CropBounds[2] * (dy + sx), midz);
        m_Overlay->SetCropperPlaneOrigin(2, midx, bounds[2] - sx / 2 + m_CropBounds[2] * (dy + sx), midz);
    }
    if (m_CropBounds[3] == 1)
    {
        m_Plane[3]->SetOrigin(midx, bounds[3] + sx / 2, midz);
        m_Overlay->SetCropperPlaneOrigin(3, midx, bounds[3] + sx / 2, midz);
    }
    else
    {
        m_Plane[3]->SetOrigin(midx, bounds[2] - sx / 2 + m_CropBounds[3] * (dy + sx), midz);
        m_Overlay->SetCropperPlaneOrigin(3, midx, bounds[2] - sx / 2 + m_CropBounds[3] * (dy + sx), midz);
    }

    if (m_CropBounds[4] == 0)
    {
        m_Plane[4]->SetOrigin(midx, midy, bounds[4]);
    }
    else
    {
        m_Plane[4]->SetOrigin(midx, midy, bounds[4] + m_CropBounds[4] * (bounds[5] - bounds[4]));
    }
    if (m_CropBounds[5] == 1)
    {
        m_Plane[5]->SetOrigin(midx, midy, bounds[5]);
    }
    else
    {
        m_Plane[5]->SetOrigin(midx, midy, bounds[4] + m_CropBounds[5] * (bounds[5] - bounds[4]));
    }

    if (m_SolidActor->GetVisibility())
    {
        BuildPipelineForSolid();
    }
    else if (m_IsosurfaceActor->GetVisibility())
    {
        BuildPipelineForIsosurface();
    }
}

double mvManager::GetHorizontalCropAngle() const
{
    return m_CropAngle;
}

void mvManager::ShowCroppedAwayPieces()
{
    m_CroppedAwayPiecesActor->VisibilityOn();
    m_ShowCroppedAwayPieces = 1;
}

void mvManager::HideCroppedAwayPieces()
{
    m_CroppedAwayPiecesActor->VisibilityOff();
    m_ShowCroppedAwayPieces = 0;
}

int mvManager::AreCroppedAwayPiecesShown() const
{
    return m_ShowCroppedAwayPieces;
}

void mvManager::SetCroppedAwayPiecesColor(double red, double green, double blue)
{
    m_CroppedAwayPiecesActor->GetProperty()->SetColor(red, green, blue);
}

void mvManager::SetCroppedAwayPiecesOpacity(double opacity)
{
    m_CroppedAwayPiecesActor->GetProperty()->SetOpacity(opacity);
}

const double *mvManager::GetCroppedAwayPiecesColor() const
{
    return m_CroppedAwayPiecesActor->GetProperty()->GetColor();
}

double mvManager::GetCroppedAwayPiecesOpacity() const
{
    return m_CroppedAwayPiecesActor->GetProperty()->GetOpacity();
}

void mvManager::SetRegularIsosurfaces(int numIsosurface, double minValue, double maxValue)
{
    m_UseRegularIsosurface[m_ActiveDataType]       = 1;
    m_NumberOfRegularIsosurfaces[m_ActiveDataType] = numIsosurface;
    m_RegularIsosurfaceMin[m_ActiveDataType]       = minValue;
    m_RegularIsosurfaceMax[m_ActiveDataType]       = maxValue;
    m_Isosurface->GenerateValues(numIsosurface, minValue, maxValue);
}

int mvManager::GetNumberOfRegularIsosurfaces() const
{
    return m_NumberOfRegularIsosurfaces[m_ActiveDataType];
}

int mvManager::GetNumberOfCustomIsosurfaces() const
{
    return m_NumberOfCustomIsosurfaces[m_ActiveDataType];
}

void mvManager::GetRegularIsosurfaceRange(double *range) const
{
    range[0] = m_RegularIsosurfaceMin[m_ActiveDataType];
    range[1] = m_RegularIsosurfaceMax[m_ActiveDataType];
}

const double *mvManager::GetCustomIsosurfaceValues() const
{
    return m_CustomIsosurfaceValues[m_ActiveDataType];
}

int mvManager::UsingRegularIsosurfaces() const
{
    return m_UseRegularIsosurface[m_ActiveDataType];
}

void mvManager::SetCustomIsosurfaces(int count, const double *values)
{
    m_UseRegularIsosurface[m_ActiveDataType]      = 0;
    m_NumberOfCustomIsosurfaces[m_ActiveDataType] = count;
    m_Isosurface->SetNumberOfContours(count);
    if (m_CustomIsosurfaceValues[m_ActiveDataType] != 0)
    {
        delete[] m_CustomIsosurfaceValues[m_ActiveDataType];
    }
    if (count > 0)
    {
        m_CustomIsosurfaceValues[m_ActiveDataType] = new double[count];
    }
    else
    {
        m_CustomIsosurfaceValues[m_ActiveDataType] = 0;
        return;
    }
    for (int i = 0; i < count; i++)
    {
        m_CustomIsosurfaceValues[m_ActiveDataType][i] = values[i];
        m_Isosurface->SetValue(i, values[i]);
    }
}

void mvManager::BuildPipelineForSolid()
{
    int                 i;

    // Step 1: Determine which type of solid to start with.
    vtkAlgorithmOutput *previousAlgorithmOutput;
    if (m_DoSolidThreshold[m_ActiveDataType])
    {
        if (m_SolidDisplayMode[m_ActiveDataType] == SolidDisplayType::MV_SOLID_SMOOTH || m_SolidDisplayMode[m_ActiveDataType] == SolidDisplayType::MV_SOLID_BANDED)
        {
            previousAlgorithmOutput = m_SmoothSolid->GetOutputPort();
        }
        else
        {
            previousAlgorithmOutput = m_BlockySolid->GetOutputPort();
        }
    }
    else
    {
        previousAlgorithmOutput = m_ActiveScalarDataSetGeometry->GetOutputPort();
    }

    m_CroppedSolid->Delete();
    m_Faces->Delete();
    m_CroppedAwayPieces->Delete();
    m_CroppedSolid      = mvCustomAppendPolyData::New();
    m_Faces             = mvCustomAppendPolyData::New();
    m_CroppedAwayPieces = mvCustomAppendPolyData::New();
    m_CroppedAwayPiecesMapper->SetInputConnection(m_CroppedAwayPieces->GetOutputPort());

    // Step 3: Crop the solid and extract faces at cropping planes
    vtkAlgorithmOutput *face[6];
    for (i = 0; i < 6; i++)
    {
        face[i] = 0;
    }
    int hasFaces = 0;
    if (m_CropBounds[0] > 0)
    {
        m_Cropper[0]->SetInputConnection(previousAlgorithmOutput);
        previousAlgorithmOutput = m_Cropper[0]->GetOutputPort();
        m_CroppedAwayPieces->AddInputData(m_Cropper[0]->GetClippedOutput());
        face[0]  = m_ExtractFace[0]->GetOutputPort();
        hasFaces = 1;
    }
    if (m_CropBounds[1] < 1)
    {
        m_Cropper[1]->SetInputConnection(previousAlgorithmOutput);
        previousAlgorithmOutput = m_Cropper[1]->GetOutputPort();
        m_CroppedAwayPieces->AddInputData(m_Cropper[1]->GetClippedOutput());
        face[1]  = m_ExtractFace[1]->GetOutputPort();
        hasFaces = 1;
    }
    if (m_CropBounds[2] > 0)
    {
        m_Cropper[2]->SetInputConnection(previousAlgorithmOutput);
        previousAlgorithmOutput = m_Cropper[2]->GetOutputPort();
        m_CroppedAwayPieces->AddInputData(m_Cropper[2]->GetClippedOutput());
        face[2]  = m_ExtractFace[2]->GetOutputPort();
        hasFaces = 1;
    }
    if (m_CropBounds[3] < 1)
    {
        m_Cropper[3]->SetInputConnection(previousAlgorithmOutput);
        previousAlgorithmOutput = m_Cropper[3]->GetOutputPort();
        m_CroppedAwayPieces->AddInputData(m_Cropper[3]->GetClippedOutput());
        face[3]  = m_ExtractFace[3]->GetOutputPort();
        hasFaces = 1;
    }
    if (m_CropBounds[4] > 0)
    {
        m_Cropper[4]->SetInputConnection(previousAlgorithmOutput);
        previousAlgorithmOutput = m_Cropper[4]->GetOutputPort();
        m_CroppedAwayPieces->AddInputData(m_Cropper[4]->GetClippedOutput());
        face[4]  = m_ExtractFace[4]->GetOutputPort();
        hasFaces = 1;
    }
    if (m_CropBounds[5] < 1)
    {
        m_Cropper[5]->SetInputConnection(previousAlgorithmOutput);
        previousAlgorithmOutput = m_Cropper[5]->GetOutputPort();
        m_CroppedAwayPieces->AddInputData(m_Cropper[5]->GetClippedOutput());
        face[5]  = m_ExtractFace[5]->GetOutputPort();
        hasFaces = 1;
    }

    // Step 4: Crop the faces. Each face has to be cropped 4 times.
    if (hasFaces)
    {
        m_CroppedSolid->AddInputConnection(previousAlgorithmOutput);

        if (m_CropBounds[0] == m_CropBounds[1])
        {
            face[2] = 0;
            face[3] = 0;
            face[4] = 0;
            face[5] = 0;
        }
        else if (m_CropBounds[2] == m_CropBounds[3])
        {
            face[0] = 0;
            face[1] = 0;
            face[4] = 0;
            face[5] = 0;
        }
        else if (m_CropBounds[4] == m_CropBounds[5])
        {
            face[0] = 0;
            face[1] = 0;
            face[2] = 0;
            face[3] = 0;
        }
        if (m_CropBounds[0] > 0)
        {
            if (face[2] != 0)
            {
                m_FaceCrop[0]->SetInputConnection(face[2]);
                face[2] = m_FaceCrop[0]->GetOutputPort();
            }
            if (face[3] != 0)
            {
                m_FaceCrop[1]->SetInputConnection(face[3]);
                face[3] = m_FaceCrop[1]->GetOutputPort();
            }
            if (face[4] != 0)
            {
                m_FaceCrop[2]->SetInputConnection(face[4]);
                face[4] = m_FaceCrop[2]->GetOutputPort();
            }
            if (face[5] != 0)
            {
                m_FaceCrop[3]->SetInputConnection(face[5]);
                face[5] = m_FaceCrop[3]->GetOutputPort();
            }
        }
        if (m_CropBounds[1] < 1)
        {
            if (face[2] != 0)
            {
                m_FaceCrop[4]->SetInputConnection(face[2]);
                face[2] = m_FaceCrop[4]->GetOutputPort();
            }
            if (face[3] != 0)
            {
                m_FaceCrop[5]->SetInputConnection(face[3]);
                face[3] = m_FaceCrop[5]->GetOutputPort();
            }
            if (face[4] != 0)
            {
                m_FaceCrop[6]->SetInputConnection(face[4]);
                face[4] = m_FaceCrop[6]->GetOutputPort();
            }
            if (face[5] != 0)
            {
                m_FaceCrop[7]->SetInputConnection(face[5]);
                face[5] = m_FaceCrop[7]->GetOutputPort();
            }
        }
        if (m_CropBounds[2] > 0)
        {
            if (face[0] != 0)
            {
                m_FaceCrop[8]->SetInputConnection(face[0]);
                face[0] = m_FaceCrop[8]->GetOutputPort();
            }
            if (face[1] != 0)
            {
                m_FaceCrop[9]->SetInputConnection(face[1]);
                face[1] = m_FaceCrop[9]->GetOutputPort();
            }
            if (face[4] != 0)
            {
                m_FaceCrop[10]->SetInputConnection(face[4]);
                face[4] = m_FaceCrop[10]->GetOutputPort();
            }
            if (face[5] != 0)
            {
                m_FaceCrop[11]->SetInputConnection(face[5]);
                face[5] = m_FaceCrop[11]->GetOutputPort();
            }
        }
        if (m_CropBounds[3] < 1)
        {
            if (face[0] != 0)
            {
                m_FaceCrop[12]->SetInputConnection(face[0]);
                face[0] = m_FaceCrop[12]->GetOutputPort();
            }
            if (face[1] != 0)
            {
                m_FaceCrop[13]->SetInputConnection(face[1]);
                face[1] = m_FaceCrop[13]->GetOutputPort();
            }
            if (face[4] != 0)
            {
                m_FaceCrop[14]->SetInputConnection(face[4]);
                face[4] = m_FaceCrop[14]->GetOutputPort();
            }
            if (face[5] != 0)
            {
                m_FaceCrop[15]->SetInputConnection(face[5]);
                face[5] = m_FaceCrop[15]->GetOutputPort();
            }
        }
        if (m_CropBounds[4] > 0)
        {
            if (face[0] != 0)
            {
                m_FaceCrop[16]->SetInputConnection(face[0]);
                face[0] = m_FaceCrop[16]->GetOutputPort();
            }
            if (face[1] != 0)
            {
                m_FaceCrop[17]->SetInputConnection(face[1]);
                face[1] = m_FaceCrop[17]->GetOutputPort();
            }
            if (face[2] != 0)
            {
                m_FaceCrop[18]->SetInputConnection(face[2]);
                face[2] = m_FaceCrop[18]->GetOutputPort();
            }
            if (face[3] != 0)
            {
                m_FaceCrop[19]->SetInputConnection(face[3]);
                face[3] = m_FaceCrop[19]->GetOutputPort();
            }
        }
        if (m_CropBounds[5] < 1)
        {
            if (face[0] != 0)
            {
                m_FaceCrop[20]->SetInputConnection(face[0]);
                face[0] = m_FaceCrop[20]->GetOutputPort();
            }
            if (face[1] != 0)
            {
                m_FaceCrop[21]->SetInputConnection(face[1]);
                face[1] = m_FaceCrop[21]->GetOutputPort();
            }
            if (face[2] != 0)
            {
                m_FaceCrop[22]->SetInputConnection(face[2]);
                face[2] = m_FaceCrop[22]->GetOutputPort();
            }
            if (face[3] != 0)
            {
                m_FaceCrop[23]->SetInputConnection(face[3]);
                face[3] = m_FaceCrop[23]->GetOutputPort();
            }
        }

        // Append all faces
        int faceCount = 0;
        for (i = 0; i < 6; i++)
        {
            if (face[i] != 0)
            {
                m_Faces->AddInputConnection(face[i]);
                faceCount++;
            }
        }
        // If there are faces, then append them to the cropped solid
        if (faceCount > 0)
        {
            // If solid threshold is on, we need to clip the faces
            if (m_DoSolidThreshold[m_ActiveDataType])
            {
                // for smooth or banded solid, we clip
                if (m_SolidDisplayMode[m_ActiveDataType] == SolidDisplayType::MV_SOLID_SMOOTH || m_SolidDisplayMode[m_ActiveDataType] == SolidDisplayType::MV_SOLID_BANDED)
                {
                    m_FacesClipMin->SetInputConnection(m_Faces->GetOutputPort());
                    m_CroppedSolid->AddInputConnection(m_FacesClipMax->GetOutputPort());
                }
                // for blocky solid, we pull out entire cells
                else
                {
                    // Apply threshold only when there are cells. Otherwise an error
                    // occurs in vtkThreshold
                    m_Faces->Update();
                    if (m_Faces->GetOutput()->GetNumberOfCells() > 0)
                    {
                        m_FacesThreshold->SetInputConnection(m_Faces->GetOutputPort());
                        m_CroppedSolid->AddInputConnection(m_FacesThresholdGeometry->GetOutputPort());
                    }
                }
            }
            else
            {
                m_CroppedSolid->AddInputConnection(m_Faces->GetOutputPort());
            }
        }
        previousAlgorithmOutput = m_CroppedSolid->GetOutputPort();
    }

    // Create color bands
    if (m_SolidDisplayMode[m_ActiveDataType] == SolidDisplayType::MV_SOLID_BANDED)
    {
        m_ColorBandFilter->SetInputConnection(previousAlgorithmOutput);
        previousAlgorithmOutput = m_ColorBandFilter->GetOutputPort();
    }

    m_SolidMapper->SetInputConnection(previousAlgorithmOutput);
}

void mvManager::BuildPipelineForIsosurface()
{
    vtkAlgorithmOutput *previousAlgorithmOutput = m_Isosurface->GetOutputPort();
    if (m_CropBounds[0] == m_CropBounds[1])
    {
        m_IsosurfaceCutter[0]->SetInputConnection(previousAlgorithmOutput);
        previousAlgorithmOutput = m_IsosurfaceCutter[0]->GetOutputPort();
        if (m_CropBounds[0] == 0)
        {
            m_IsosurfaceCutter[0]->SetCutFunction(m_Plane[0]);
        }
        else
        {
            m_IsosurfaceCutter[0]->SetCutFunction(m_Plane[1]);
        }
    }
    else
    {
        if (m_CropBounds[0] > 0)
        {
            m_Cropper[0]->SetInputConnection(previousAlgorithmOutput);
            previousAlgorithmOutput = m_Cropper[0]->GetOutputPort();
        }
        if (m_CropBounds[1] < 1)
        {
            m_Cropper[1]->SetInputConnection(previousAlgorithmOutput);
            previousAlgorithmOutput = m_Cropper[1]->GetOutputPort();
        }
    }
    if (m_CropBounds[2] == m_CropBounds[3])
    {
        m_IsosurfaceCutter[1]->SetInputConnection(previousAlgorithmOutput);
        previousAlgorithmOutput = m_IsosurfaceCutter[1]->GetOutputPort();
        if (m_CropBounds[2] == 0)
        {
            m_IsosurfaceCutter[1]->SetCutFunction(m_Plane[2]);
        }
        else
        {
            m_IsosurfaceCutter[1]->SetCutFunction(m_Plane[3]);
        }
    }
    else
    {
        if (m_CropBounds[2] > 0)
        {
            m_Cropper[2]->SetInputConnection(previousAlgorithmOutput);
            previousAlgorithmOutput = m_Cropper[2]->GetOutputPort();
        }
        if (m_CropBounds[3] < 1)
        {
            m_Cropper[3]->SetInputConnection(previousAlgorithmOutput);
            previousAlgorithmOutput = m_Cropper[3]->GetOutputPort();
        }
    }
    if (m_CropBounds[4] == m_CropBounds[5])
    {
        m_IsosurfaceCutter[2]->SetInputConnection(previousAlgorithmOutput);
        previousAlgorithmOutput = m_IsosurfaceCutter[2]->GetOutputPort();
        if (m_CropBounds[4] == 0)
        {
            m_IsosurfaceCutter[2]->SetCutFunction(m_Plane[4]);
        }
        else
        {
            m_IsosurfaceCutter[2]->SetCutFunction(m_Plane[5]);
        }
    }
    else
    {
        if (m_CropBounds[4] > 0)
        {
            m_Cropper[4]->SetInputConnection(previousAlgorithmOutput);
            previousAlgorithmOutput = m_Cropper[4]->GetOutputPort();
        }
        if (m_CropBounds[5] < 1)
        {
            m_Cropper[5]->SetInputConnection(previousAlgorithmOutput);
            previousAlgorithmOutput = m_Cropper[5]->GetOutputPort();
        }
    }
    m_IsosurfaceMapper->SetInputConnection(previousAlgorithmOutput);

    // cropped-away pieces
    m_CroppedAwayPieces->Delete();
    m_CroppedAwayPieces = mvCustomAppendPolyData::New();
    m_CroppedAwayPiecesMapper->SetInputConnection(m_CroppedAwayPieces->GetOutputPort());
    if (m_CropBounds[0] == m_CropBounds[1] || m_CropBounds[2] == m_CropBounds[3] || m_CropBounds[4] == m_CropBounds[5])
    {
        m_CroppedAwayPieces->AddInputConnection(m_Isosurface->GetOutputPort());
    }
    else
    {
        if (m_CropBounds[0] > 0)
        {
            m_CroppedAwayPieces->AddInputData(m_Cropper[0]->GetClippedOutput());
        }
        if (m_CropBounds[1] < 1)
        {
            m_CroppedAwayPieces->AddInputData(m_Cropper[1]->GetClippedOutput());
        }
        if (m_CropBounds[2] > 0)
        {
            m_CroppedAwayPieces->AddInputData(m_Cropper[2]->GetClippedOutput());
        }
        if (m_CropBounds[3] < 1)
        {
            m_CroppedAwayPieces->AddInputData(m_Cropper[3]->GetClippedOutput());
        }
        if (m_CropBounds[4] > 0)
        {
            m_CroppedAwayPieces->AddInputData(m_Cropper[4]->GetClippedOutput());
        }
        if (m_CropBounds[5] < 1)
        {
            m_CroppedAwayPieces->AddInputData(m_Cropper[5]->GetClippedOutput());
        }
    }
}

// Write parameters to file
char *mvManager::Serialize(const char *fileName, mvGUISettings *gui) const
{
    // Reading data is done in "Deserialize".

    // Open an output file stream
    ofstream out(fileName);

    // Header
    out << "Application name = Model Viewer\n";
    out << "Version = " << MV_VERSION << "\n";

    out << "Title = \n";

    // Model name and data files
    out << "Model name = " << m_DataSource->GetName() << endl;
    // Copy the model input file names from the data source.
    char *fnames = new char[strlen(m_DataSource->GetDataFileList()) + 1];
    strcpy(fnames, m_DataSource->GetDataFileList());
    // Write the number of file codes. This can be determined
    // by searching the number of occurences of '\n' in the file name string.
    char *cr    = fnames;
    int   ncode = 0;
    while (1)
    {
        cr = strchr(cr, '\n');
        if (!cr)
        {
            break;
        }
        ncode++;
        cr++;
    }
    out << "File code count = " << ncode << endl;

    // Model data files
    char *code = fnames;
    for (int i = 0; i < ncode; i++)
    {
        cr    = strchr(code, '\n');
        *(cr) = '\0';
        // Get relative paths for all file codes
        std::string relative = mvSaveCurrentDirectory::GetRelativePath(fileName, code);
        out << "File code " << (i + 1) << " = " << relative.c_str() << endl;
        code = cr + 1;
    }

    // Data types
    delete[] fnames;
    int numDataTypes = m_DataSource->GetNumberOfScalarDataTypes();
    out << "Data type count = " << numDataTypes << endl;
    out << "Data type active= " << m_ActiveDataType << endl;

    // Color bar, solid and isosurface control
    const double *rgb = GetColorBarTextColor();
    out << "Color bar data source = " << GetColorBarSource() << endl;
    out << "Color bar width = " << GetColorBarWidth() << endl;
    out << "Color bar height = " << GetColorBarHeight() << endl;
    out << "Color bar offset = " << GetColorBarOffset() << endl;
    out << "Color bar font size = " << GetColorBarFontSize() << endl;
    out << "Color bar label color option = " << (int)(rgb[0] * 2 + .1) << endl;
    out << "Color bar color scheme = " << GetColorBarColorScheme() << endl;

    std::uint32_t color = GetColorBarFirstCustomColor();
    std::uint32_t red, green, blue;
    // red
    red   = color;
    red   = red << 24;
    red   = red >> 24;
    // green
    green = color;
    green = green << 16;
    green = green >> 24;
    // blue
    blue  = color;
    blue  = blue << 8;
    blue  = blue >> 24;
    out << "Color bar first custom color red = " << (int)red << endl;
    out << "Color bar first custom color green = " << (int)green << endl;
    out << "Color bar first custom color blue = " << (int)blue << endl;

    color = GetColorBarLastCustomColor();
    red   = color;
    red   = red << 24;
    red   = red >> 24;
    // green
    green = color;
    green = green << 16;
    green = green >> 24;
    // blue
    blue  = color;
    blue  = blue << 8;
    blue  = blue >> 24;

    out << "Color bar last custom color red = " << (int)red << endl;
    out << "Color bar last custom color green = " << (int)green << endl;
    out << "Color bar last custom color blue = " << (int)blue << endl;

    for (int i = 0; i < numDataTypes; i++)
    {
        // Color Bar
        out << "Color bar " << (i + 1) << " value blue = " << m_ColorBarValueBlue[i] << endl;
        out << "Color bar " << (i + 1) << " value red = " << m_ColorBarValueRed[i] << endl;
        out << "Color bar " << (i + 1) << " logarithmic scale = " << m_UseLogColorBar[i] << endl;
        out << "Color bar " << (i + 1) << " number of labels = " << m_NumColorBarLabels[i] << endl;
        out << "Color bar " << (i + 1) << " label precision = " << m_ColorBarLabelPrecision[i] << endl;

        // Solid Control
        out << "Solid " << (i + 1) << " display mode = " << static_cast<int>(m_SolidDisplayMode[i]) << endl;
        out << "Solid " << (i + 1) << " apply threshold = " << m_DoSolidThreshold[i] << endl;
        out << "Solid " << (i + 1) << " threshold lower limit = " << m_SolidThresholdMin[i] << endl;
        out << "Solid " << (i + 1) << " threshold upper limit = " << m_SolidThresholdMax[i] << endl;
        out << "Solid " << (i + 1) << " number of color bands = " << m_NumberOfColorBands[i] << endl;

        // Isosurface
        out << "Isosurfaces " << (i + 1) << " show regular = " << m_UseRegularIsosurface[i] << endl;
        out << "Isosurfaces " << (i + 1) << " regular count = " << m_NumberOfRegularIsosurfaces[i] << endl;
        out << "Isosurfaces " << (i + 1) << " regular min  = " << m_RegularIsosurfaceMin[i] << endl;
        out << "Isosurfaces " << (i + 1) << " regular max  = " << m_RegularIsosurfaceMax[i] << endl;
        out << "Isosurfaces " << (i + 1) << " custom count = " << m_NumberOfCustomIsosurfaces[i] << endl;
        for (int j = 0; j < m_NumberOfCustomIsosurfaces[i]; j++)
        {
            out << "Isosurfaces " << (i + 1) << " custom value " << (j + 1) << " = " << m_CustomIsosurfaceValues[i][j] << endl;
        }
    }

    // Vector
    out << "Vector data = " << HasVectorData() << endl;
    if (HasVectorData())
    {
        int *voi   = m_ExtractStructuredGridVector->GetVOI();
        int *srate = m_ExtractStructuredGridVector->GetSampleRate();
        rgb        = m_VectorActor->GetProperty()->GetColor();
        out << "Vector subsample i min = " << voi[0] - m_VectorSubsampleOffset[0] << endl;
        out << "Vector subsample i max = " << voi[1] << endl;
        out << "Vector subsample i rate = " << srate[0] << endl;
        out << "Vector subsample j min = " << voi[2] - m_VectorSubsampleOffset[1] << endl;
        out << "Vector subsample j max = " << voi[3] << endl;
        out << "Vector subsample j rate = " << srate[1] << endl;
        out << "Vector subsample k min = " << voi[4] - m_VectorSubsampleOffset[2] << endl;
        out << "Vector subsample k max = " << voi[5] << endl;
        out << "Vector subsample k rate = " << srate[2] << endl;
        out << "Vector scale factor = " << GetVectorScaleFactor() << endl;
        out << "Vector threshold apply = " << m_DoVectorThreshold << endl;
        out << "Vector threshold lower limit = " << m_VectorThreshold->GetLowerThreshold() << endl;
        out << "Vector threshold upper limit = " << m_VectorThreshold->GetUpperThreshold() << endl;
        out << "Vector glyph active = " << IsVectorGlyphActivated() << endl;
        out << "Vector glyph x length = " << m_CubeSource->GetXLength() << endl;
        out << "Vector glyph y length = " << m_CubeSource->GetYLength() << endl;
        out << "Vector glyph z length = " << m_CubeSource->GetZLength() << endl;
        out << "Vector color option = " << (int)(rgb[0] * 2 + .1) << endl;

        out << "Vector Crop bounds x min = " << m_VectorClippingXMin << endl;
        out << "Vector Crop bounds x max = " << m_VectorClippingXMax << endl;
        out << "Vector Crop bounds y min = " << m_VectorClippingYMin << endl;
        out << "Vector Crop bounds y max = " << m_VectorClippingYMax << endl;
        out << "Vector Crop bounds z min = " << m_VectorClippingZMin << endl;
        out << "Vector Crop bounds z max = " << m_VectorClippingZMax << endl;
        out << "Vector Crop angle = " << m_VectorClippingAngle << endl;

        out << "Vector Log transform = " << m_VectorLog10Transform << endl;
        out << "Vector Line Width = " << m_VectorActor->GetProperty()->GetLineWidth() << endl;
    }

    // Pathlines
    out << "Pathline data = " << HasPathlineData() << endl;
    out << "Pathline representation = " << GetPathlineRepresentation() << endl;
    out << "Pathline tube diameter = " << GetPathlineTubeDiameter() << endl;
    out << "Pathline time blue = " << m_Pathlines->GetTimeBlue() << endl;
    out << "Pathline time red = " << m_Pathlines->GetTimeRed() << endl;
    out << "Pathline threshold mode = " << m_PathlineTimeClippingMode << endl;
    out << "Pathline threshold time min = " << m_PathlineClipTimeMin << endl;
    out << "Pathline threshold time max = " << m_PathlineClipTimeMax << endl;
    out << "Pathline log transform = " << GetPathlineLogTransform() << endl;

    // Model Features
    int    nmft = m_DataSource->GetNumberOfModelFeatureTypes();
    double rgba[4];
    out << "Number of model feature types = " << nmft << endl;
    int *displayOrder = m_ModelFeatures->GetDisplayOrder();
    for (int i = 0; i < nmft; i++)
    {
        out << "Model feature type " << i << " display order = " << displayOrder[i] << endl;
        m_ModelFeatures->GetColor(i, rgba);
        out << "Model feature type " << i << " red = " << rgba[0] << endl;
        out << "Model feature type " << i << " green = " << rgba[1] << endl;
        out << "Model feature type " << i << " blue = " << rgba[2] << endl;
        out << "Model feature type " << i << " alpha = " << rgba[3] << endl;
    }
    out << "Model feature glyph size = " << m_ModelFeatures->GetGlyphSize() << endl;

    if (GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        // Subgrid
        int *subg = m_ExtractStructuredGridForSubgrid->GetVOI();
        out << "Subgrid active = " << IsScalarSubgridOn() << endl;
        out << "Subgrid i min = " << subg[0] << endl;
        out << "Subgrid i max = " << subg[1] << endl;
        out << "Subgrid j min = " << subg[2] << endl;
        out << "Subgrid j max = " << subg[3] << endl;
        out << "Subgrid k min = " << subg[4] << endl;
        out << "Subgrid k max = " << subg[5] << endl;
    }

    // Surface lighting
    out << "Lighting diffuse = " << m_SolidActor->GetProperty()->GetDiffuse() << endl;
    out << "Lighting ambient = " << m_SolidActor->GetProperty()->GetAmbient() << endl;
    out << "Lighting specular = " << m_SolidActor->GetProperty()->GetSpecular() << endl;
    out << "Lighting Specular power = " << m_SolidActor->GetProperty()->GetSpecularPower() << endl;

    // Grid Shell
    rgb = m_GridShell->GetColor();
    out << "Grid shell color red = " << rgb[0] << endl;
    out << "Grid shell color green = " << rgb[1] << endl;
    out << "Grid shell color blue = " << rgb[2] << endl;
    out << "Grid shell color opacity = " << m_GridShell->GetOpacity() << endl;

    // Grid Lines
    int ibuff[3];
    GetGridLinePositions(ibuff);
    rgb = m_GridLines[0]->GetColor();
    out << "Grid lines Position x = " << ibuff[0] << endl;
    out << "Grid lines Position y = " << ibuff[1] << endl;
    out << "Grid lines Position z = " << ibuff[2] << endl;
    out << "Grid lines activation x = " << m_GridLinesActivated[0] << endl;
    out << "Grid lines activation y = " << m_GridLinesActivated[1] << endl;
    out << "Grid lines activation z = " << m_GridLinesActivated[2] << endl;
    out << "Grid lines color option = " << (int)(rgb[0] * 2 + .1) << endl;

    // Grid Layer
    rgb = m_GridLayer->GetColor();
    out << "Grid layer Position = " << GetGridLayerPosition() << endl;
    out << "Grid layer color option = " << (int)(rgb[0] * 2 + .1) << endl;

    // Grid display
    out << "Grid display mode = " << m_GridDisplayMode << endl;

    // Scale
    double scale[3];
    m_SolidActor->GetScale(scale);
    out << "Scale x = " << scale[0] << endl;
    out << "Scale y = " << scale[1] << endl;
    out << "Scale z = " << scale[2] << endl;

    // Axes
    const double *pn = m_Axes->GetNormalizedPosition();
    out << "Axes representation = " << GetAxesRepresentation() << endl;
    out << "Axes normalized size = " << GetAxesNormalizedSize() << endl;
    out << "Axes normalized tube diameter = " << GetAxesNormalizedTubeDiameter() << endl;
    out << "Axes normalized position x = " << pn[0] << endl;
    out << "Axes normalized position y = " << pn[1] << endl;
    out << "Axes normalized position z = " << pn[2] << endl;

    //	out << "Log transform x axis = " << m_DataSource->GetLogTransformXAxis() << endl;
    //	out << "Log transform y axis = " << m_DataSource->GetLogTransformYAxis() << endl;
    //	out << "Log transform z axis = " << m_DataSource->GetLogTransformZAxis() << endl;

    // Bounding Box
    rgb = m_BoundingBox->GetColor();
    out << "Bounding box color option = " << (int)(rgb[0] * 2 + .1) << endl;

    // Crop
    rgb = m_CroppedAwayPiecesActor->GetProperty()->GetColor();
    out << "Crop bounds x min = " << m_CropBounds[0] << endl;
    out << "Crop bounds x max = " << m_CropBounds[1] << endl;
    out << "Crop bounds y min = " << m_CropBounds[2] << endl;
    out << "Crop bounds y max = " << m_CropBounds[3] << endl;
    out << "Crop bounds z min = " << m_CropBounds[4] << endl;
    out << "Crop bounds z max = " << m_CropBounds[5] << endl;
    out << "Crop angle = " << m_CropAngle << endl;
    out << "Cropped away pieces red = " << rgb[0] << endl;
    out << "Cropped away pieces green = " << rgb[1] << endl;
    out << "Cropped away pieces blue = " << rgb[2] << endl;
    out << "Cropped away pieces opacity = " << m_CroppedAwayPiecesActor->GetProperty()->GetOpacity() << endl;

    // Actor visibility
    out << "Visibility Solid = " << m_SolidActor->GetVisibility() << endl;
    out << "Visibility Isosurface = " << m_IsosurfaceActor->GetVisibility() << endl;
    out << "Visibility Cropped away pieces = " << m_CroppedAwayPiecesActor->GetVisibility() << endl;
    out << "Visibility Vector = " << m_VectorActor->GetVisibility() << endl;
    out << "Visibility Pathline = " << m_Pathlines->GetVisibility() << endl;
    out << "Visibility Model features = " << m_ModelFeatures->GetVisibility() << endl;
    out << "Visibility Grid shell = " << m_GridShell->GetVisibility() << endl;
    out << "Visibility Activated grid lines = " << m_ActivatedGridLinesVisibility << endl;
    out << "Visibility Grid layer = " << m_GridLayer->GetVisibility() << endl;
    out << "Visibility Axes = " << m_Axes->GetVisibility() << endl;
    out << "Visibility Bounding box = " << m_BoundingBox->GetVisibility() << endl;
    out << "Visibility Time label = " << m_TimeLabel->GetVisibility() << endl;
    out << "Visibility Title = " << m_Title->GetVisibility() << endl;
    out << "Visibility Color bar = " << m_ColorBar->GetVisibility() << endl;
    out << "Visibility Overlay = " << m_Overlay->GetVisibility() << endl;

    // Overlay
    if (m_Overlay->HasData())
    {
        std::string dir      = mvSaveCurrentDirectory::GetDirName(fileName);
        std::string relative = mvSaveCurrentDirectory::GetRelativePath(dir.c_str(), m_Overlay->GetFileName());
        out << "Overlay file = " << relative << endl;
    }
    out << "Overlay data type = " << m_Overlay->GetType() << endl;
    double x, y;
    int    oldprecision = out.precision(12);
    m_Overlay->GetCoordinatesAtGridOrigin(x, y);
    out << "Overlay x coordinate at grid origin = " << x << endl;
    out << "Overlay y coordinate at grid origin = " << y << endl;
    out << "Overlay to grid scale = " << m_Overlay->GetOverlayToGridScale() << endl;
    out << "Overlay angle = " << m_Overlay->GetAngle() << endl;
    out << "Overlay drape = " << m_Overlay->GetDrape() << endl;
    out << "Overlay trim = " << m_Overlay->GetTrim() << endl;
    out << "Overlay crop = " << m_Overlay->GetCrop() << endl;
    out << "Overlay elevation = " << m_Overlay->GetElevation() << endl;
    out << "Overlay drape gap = " << m_Overlay->GetDrapeGap() << endl;
    out.precision(oldprecision);

    // GUI settings
    gui->Serialize(&out);

    out.close();
    return 0;
}

// Read parameters from file
void mvManager::Deserialize(const char *fileName, mvGUISettings *gui, std::string &errorMsg)
{
    int    i, ivalue, i1, i2, i3, i4, i5, i6, i7, i8, i9;
    double fvalue, f1, f2, f3, f4, f5, f6;
    char   key[100], buffer[1024];

    // Open an input stream
    ifstream in(fileName, std::ifstream::in);

    if (!in.is_open())
    {
        errorMsg = "Unable to open the Model Viewer file.";
        return;
    }

    // Create a hash table and read the data file into the hash table
    mvHashTable *hashTable = new mvHashTable;
    while (!in.eof())
    {
        if (mvUtil::ExtractLabelAndValue(&in, key, buffer))
        {
            hashTable->AddHashEntry(key, buffer);
        }
    }
    in.close();

    // Check header
    buffer[0] = '\0';
    hashTable->GetHashTableValue("Application name", buffer);
    if (strcmp(buffer, "Model Viewer") != 0)
    {
        delete hashTable;
        errorMsg = "This file does not contain data for Model Viewer";
        return;
    }
    buffer[0] = '\0';
    hashTable->GetHashTableValue("Version", buffer);
    if (strcmp(buffer, MV_VERSION) != 0)
    {
        delete hashTable;
        std::ostringstream oss;
        oss << "This file does not contain data for Model Viewer version " << MV_VERSION;
        errorMsg = oss.str();
        return;
    }

    // Read data file names
    char modelName[1024];
    modelName[0] = '\0';
    hashTable->GetHashTableValue("Model name", modelName);
    int ncode = 0;
    hashTable->GetHashTableValue("File code count", ncode);
    if (ncode == 0)
    {
        delete hashTable;
        errorMsg = "The Model Viewer file is damaged and cannot be loaded.";
        return;
    }

    // Get absolute paths for all file codes
    std::string dirname = mvSaveCurrentDirectory::GetDirName(fileName);
    char        szDest[MAX_PATH];
    char        fullpath[MAX_PATH];
    char *      dataFileList = new char[ncode * 1024];
    dataFileList[0]          = '\0';
    for (i = 0; i < ncode; i++)
    {
        buffer[0] = '\0';
        sprintf(key, "File code %u", i + 1);
        if (!hashTable->GetHashTableValue(key, buffer))
        {
            delete hashTable;
            delete[] dataFileList;
            errorMsg = "The Model Viewer file is damaged and cannot be loaded.";
            return;
        }
        if (strlen(buffer))
        {
#if defined(QT_GUI_LIB)
            QString dest(dirname.c_str());
            dest = mvUtil::PathAppendA(dest, buffer);
            QString fullPath = mvUtil::PathCanonicalizeA(dest);
            if (!mvUtil::PathFileExistsA(fullPath))
            {
                delete hashTable;
                delete[] dataFileList;
                std::ostringstream oss;
                oss << "Unable to open \"" << fullpath << "\".";
                errorMsg = oss.str();
                return;
            }
            strcat(dataFileList, mvUtil::toNativeSeparators(fullPath).toLocal8Bit().data());
#else
            strcpy(szDest, dirname.c_str());
            mvUtil::PathAppendA(szDest, buffer);
            mvUtil::PathCanonicalizeA(fullpath, szDest);
            if (!mvUtil::PathFileExistsA(fullpath))
            {
                delete hashTable;
                delete[] dataFileList;
                std::ostringstream oss;
                oss << "Unable to open \"" << fullpath << "\".";
                errorMsg = oss.str();
                return;
            }
            strcat(dataFileList, fullpath);
#endif
        }
        else
        {
            strcat(dataFileList, "");
        }
        strcat(dataFileList, "\n");
    }

    std::shared_ptr<mvSaveCurrentDirectory> spCurDir;
    if (ncode == 1)
    {
        // save current directory and cd to dataFileList
        spCurDir.reset(new mvSaveCurrentDirectory(dataFileList));
    }

    // Load the data and set up the visualization pipeline

    const char *err = LoadData(modelName, dataFileList);
    delete[] dataFileList;

    if (err)
    {
        delete hashTable;
        errorMsg = err;
        return;
    }

    SetTimePointTo(m_DataSource->GetInitialDisplayTimePoint());
    SetScalarDataTypeTo(0);
    ApplyDefaultSettings();

    // Actor Visibility
    if (hashTable->GetHashTableValue("Visibility Solid", ivalue))
    {
        m_SolidActor->SetVisibility(ivalue);
    }
    if (hashTable->GetHashTableValue("Visibility Isosurface", ivalue))
    {
        m_IsosurfaceActor->SetVisibility(ivalue);
    }
    if (hashTable->GetHashTableValue("Visibility Cropped away pieces", ivalue))
    {
        m_ShowCroppedAwayPieces = ivalue;
        m_CroppedAwayPiecesActor->SetVisibility(m_ShowCroppedAwayPieces);
    }
    if (hashTable->GetHashTableValue("Visibility Vector", ivalue))
    {
        m_VectorActor->SetVisibility(ivalue);
    }
    if (hashTable->GetHashTableValue("Visibility Pathline", ivalue))
    {
        m_Pathlines->SetVisibility(ivalue);
    }
    if (hashTable->GetHashTableValue("Visibility Model features", ivalue))
    {
        m_ModelFeatures->SetVisibility(ivalue);
    }
    if (hashTable->GetHashTableValue("Visibility Grid shell", ivalue))
    {
        m_GridShell->SetVisibility(ivalue);
    }
    if (hashTable->GetHashTableValue("Visibility Axes", ivalue))
    {
        m_Axes->SetVisibility(ivalue);
    }
    if (hashTable->GetHashTableValue("Visibility Bounding box", ivalue))
    {
        m_BoundingBox->SetVisibility(ivalue);
    }
    if (hashTable->GetHashTableValue("Visibility Time label", ivalue))
    {
        m_TimeLabel->SetVisibility(ivalue);
    }
    if (hashTable->GetHashTableValue("Visibility Title", ivalue))
    {
        m_Title->SetVisibility(ivalue);
    }
    if (hashTable->GetHashTableValue("Visibility Color bar", ivalue))
    {
        m_ColorBar->SetVisibility(ivalue);
    }
    if (hashTable->GetHashTableValue("Visibility Overlay", ivalue))
    {
        m_Overlay->SetVisibility(ivalue);
    }

    // Grid line activation
    HideGridLines();
    if (hashTable->GetHashTableValue("Grid lines activation x", ivalue) && ivalue)
    {
        ActivateGridLines(0);
    }
    else
    {
        DeactivateGridLines(0);
    }
    if (hashTable->GetHashTableValue("Grid lines activation y", ivalue) && ivalue)
    {
        ActivateGridLines(1);
    }
    else
    {
        DeactivateGridLines(1);
    }
    if (hashTable->GetHashTableValue("Grid lines activation z", ivalue) && ivalue)
    {
        ActivateGridLines(2);
    }
    else
    {
        DeactivateGridLines(2);
    }

    if (hashTable->GetHashTableValue("Visibility Activated grid lines", ivalue) && ivalue)
    {
        ShowActivatedGridLines();
    }

    if (hashTable->GetHashTableValue("Visibility Grid layer", ivalue) && ivalue)
    {
        ShowGridLayer();
    }

    // Color Bar
    if (hashTable->GetHashTableValue("Color bar data source", ivalue))
    {
        SetColorBarSource(ivalue);
        if (ivalue)
        {
            double range[2];
            GetPathlineTimeRange(range);
            SetColorBarEndPoints(range[0], range[1]);
        }
    }
    if (hashTable->GetHashTableValue("Color bar width", ivalue))
    {
        SetColorBarWidth(ivalue);
    }
    if (hashTable->GetHashTableValue("Color bar height", ivalue))
    {
        SetColorBarHeight(ivalue);
    }
    if (hashTable->GetHashTableValue("Color bar offset", ivalue))
    {
        SetColorBarOffset(ivalue);
    }
    if (hashTable->GetHashTableValue("Color bar font size", ivalue))
    {
        SetColorBarFontSize(ivalue);
    }
    if (hashTable->GetHashTableValue("Color bar label color option", fvalue))
    {
        double v = fvalue * 0.5;
        SetColorBarTextColor(v, v, v);
    }
    if (hashTable->GetHashTableValue("Color bar color scheme", ivalue))
    {
        SetColorBarColorScheme(ivalue);
    }
    std::uint32_t color, red, green, blue;
    if (hashTable->GetHashTableValue("Color bar first custom color red", ivalue))
    {
        red = ivalue;
    }
    else
    {
        red = 0x5A;
    }
    if (hashTable->GetHashTableValue("Color bar first custom color green", ivalue))
    {
        green = ivalue;
    }
    else
    {
        green = 0x5A;
    }
    if (hashTable->GetHashTableValue("Color bar first custom color blue", ivalue))
    {
        blue = ivalue;
    }
    else
    {
        blue = 0x5A;
    }
    green = green << 8;
    blue  = blue << 16;
    color = red + green + blue;
    SetColorBarFirstCustomColor(color);
    if (hashTable->GetHashTableValue("Color bar last custom color red", ivalue))
    {
        red = ivalue;
    }
    else
    {
        red = 0xF0;
    }
    if (hashTable->GetHashTableValue("Color bar last custom color green", ivalue))
    {
        green = ivalue;
    }
    else
    {
        green = 0xF0;
    }
    if (hashTable->GetHashTableValue("Color bar last custom color blue", ivalue))
    {
        blue = ivalue;
    }
    else
    {
        blue = 0xF0;
    }
    green = green << 8;
    blue  = blue << 16;
    color = red + green + blue;
    SetColorBarLastCustomColor(color);

    // If the saved number of data types is the same as the
    // number of data types in the data source, then
    // delete the arrays allocated in the LoadData method
    // and replace by the deserialized ones
    if (hashTable->GetHashTableValue("Data type count", ivalue))
    {
        int ierror       = 0;
        int numDataTypes = ivalue;
        if (numDataTypes == m_DataSource->GetNumberOfScalarDataTypes())
        {
            double  *colorBarValueBlue          = new double[numDataTypes];
            double  *colorBarValueRed           = new double[numDataTypes];
            int     *useLogLut                  = new int[numDataTypes];
            int     *numColorBarLabels          = new int[numDataTypes];
            int     *colorBarLabelPrecision     = new int[numDataTypes];
            int     *solidDisplayMode           = new int[numDataTypes];
            int     *applySolidThreshold        = new int[numDataTypes];
            int     *numColorBands              = new int[numDataTypes];
            double  *solidThresholdMin          = new double[numDataTypes];
            double  *solidThresholdMax          = new double[numDataTypes];
            int     *usingRegularIsosurfaces    = new int[numDataTypes];
            int     *numberOfRegularIsosurfaces = new int[numDataTypes];
            double  *regularIsosurfaceMin       = new double[numDataTypes];
            double  *regularIsosurfaceMax       = new double[numDataTypes];
            int     *numberOfCustomIsosurfaces  = new int[numDataTypes];
            double **customIsosurfaceValues     = new double *[numDataTypes];
            for (i = 0; i < numDataTypes; i++)
            {
                customIsosurfaceValues[i] = 0;
            }
            char key[100];
            for (i = 0; i < numDataTypes; i++)
            {
                // Color Bar
                sprintf(key, "Color bar %u value blue", i + 1);
                if (!hashTable->GetHashTableValue(key, colorBarValueBlue[i]))
                {
                    ierror = 1;
                    break;
                }
                sprintf(key, "Color bar %u value red", i + 1);
                if (!hashTable->GetHashTableValue(key, colorBarValueRed[i]))
                {
                    ierror = 1;
                    break;
                }
                sprintf(key, "Color bar %u logarithmic scale", i + 1);
                if (!hashTable->GetHashTableValue(key, useLogLut[i]))
                {
                    ierror = 1;
                    break;
                }
                sprintf(key, "Color bar %u number of labels", i + 1);
                if (!hashTable->GetHashTableValue(key, numColorBarLabels[i]))
                {
                    ierror = 1;
                    break;
                }
                sprintf(key, "Color bar %u label precision", i + 1);
                if (!hashTable->GetHashTableValue(key, colorBarLabelPrecision[i]))
                {
                    ierror = 1;
                    break;
                }

                // Solid
                sprintf(key, "Solid %u display mode", i + 1);
                if (!hashTable->GetHashTableValue(key, solidDisplayMode[i]))
                {
                    ierror = 1;
                    break;
                }
                sprintf(key, "Solid %u apply threshold", i + 1);
                if (!hashTable->GetHashTableValue(key, applySolidThreshold[i]))
                {
                    ierror = 1;
                    break;
                }
                sprintf(key, "Solid %u threshold lower limit", i + 1);
                if (!hashTable->GetHashTableValue(key, solidThresholdMin[i]))
                {
                    ierror = 1;
                    break;
                }
                sprintf(key, "Solid %u threshold upper limit", i + 1);
                if (!hashTable->GetHashTableValue(key, solidThresholdMax[i]))
                {
                    ierror = 1;
                    break;
                }
                sprintf(key, "Solid %u number of color bands", i + 1);
                if (!hashTable->GetHashTableValue(key, numColorBands[i]))
                {
                    ierror = 1;
                    break;
                }

                // update for version 1.0rc2 and earlier
                if (numColorBands[i] == 0)
                {
                    numColorBands[i] = 10;
                }

                // Isosurface
                sprintf(key, "Isosurfaces %u show regular", i + 1);
                if (!hashTable->GetHashTableValue(key, usingRegularIsosurfaces[i]))
                {
                    ierror = 1;
                    break;
                }
                sprintf(key, "Isosurfaces %u regular count", i + 1);
                if (!hashTable->GetHashTableValue(key, numberOfRegularIsosurfaces[i]))
                {
                    ierror = 1;
                    break;
                }
                sprintf(key, "Isosurfaces %u regular min", i + 1);
                if (!hashTable->GetHashTableValue(key, regularIsosurfaceMin[i]))
                {
                    ierror = 1;
                    break;
                }
                sprintf(key, "Isosurfaces %u regular max", i + 1);
                if (!hashTable->GetHashTableValue(key, regularIsosurfaceMax[i]))
                {
                    ierror = 1;
                    break;
                }
                sprintf(key, "Isosurfaces %u custom count", i + 1);
                if (!hashTable->GetHashTableValue(key, numberOfCustomIsosurfaces[i]))
                {
                    ierror = 1;
                    break;
                }
                if (numberOfCustomIsosurfaces[i] > 0)
                {
                    customIsosurfaceValues[i] = new double[numberOfCustomIsosurfaces[i]];
                    for (int j = 0; j < numberOfCustomIsosurfaces[i]; j++)
                    {
                        sprintf(key, "Isosurfaces %u custom value %u", i + 1, j + 1);
                        if (!hashTable->GetHashTableValue(key, customIsosurfaceValues[i][j]))
                        {
                            ierror = 1;
                            break;
                        }
                    }
                    if (ierror) { break; }
                }
            }
            if (!ierror)
            {
                delete[] m_ColorBarValueBlue;
                delete[] m_ColorBarValueRed;
                delete[] m_UseLogColorBar;
                delete[] m_NumColorBarLabels;
                delete[] m_ColorBarLabelPrecision;

                delete[] m_SolidDisplayMode;
                delete[] m_DoSolidThreshold;
                delete[] m_SolidThresholdMax;
                delete[] m_SolidThresholdMin;
                delete[] m_NumberOfColorBands;

                delete[] m_UseRegularIsosurface;
                delete[] m_NumberOfRegularIsosurfaces;
                delete[] m_RegularIsosurfaceMin;
                delete[] m_RegularIsosurfaceMax;
                delete[] m_NumberOfCustomIsosurfaces;
                if (m_CustomIsosurfaceValues != 0)
                {
                    for (int i = 0; i < m_NumScalarDataTypes; i++)
                    {
                        if (m_CustomIsosurfaceValues[i] != 0)
                        {
                            delete[] m_CustomIsosurfaceValues[i];
                        }
                    }
                }

                delete[] m_CustomIsosurfaceValues;

                m_ColorBarValueBlue          = colorBarValueBlue;
                m_ColorBarValueRed           = colorBarValueRed;
                m_UseLogColorBar             = useLogLut;
                m_NumColorBarLabels          = numColorBarLabels;
                m_ColorBarLabelPrecision     = colorBarLabelPrecision;

                m_SolidDisplayMode           = new SolidDisplayType[numDataTypes];
                for (i = 0; i < numDataTypes; ++i)
                {
                    m_SolidDisplayMode[i] = static_cast<SolidDisplayType>(solidDisplayMode[i]);
                }
                delete[] solidDisplayMode;

                m_DoSolidThreshold           = applySolidThreshold;
                m_SolidThresholdMax          = solidThresholdMax;
                m_SolidThresholdMin          = solidThresholdMin;
                m_NumberOfColorBands         = numColorBands;

                m_UseRegularIsosurface       = usingRegularIsosurfaces;
                m_NumberOfRegularIsosurfaces = numberOfRegularIsosurfaces;
                m_RegularIsosurfaceMin       = regularIsosurfaceMin;
                m_RegularIsosurfaceMax       = regularIsosurfaceMax;
                m_NumberOfCustomIsosurfaces  = numberOfCustomIsosurfaces;
                m_CustomIsosurfaceValues     = customIsosurfaceValues;

                if (hashTable->GetHashTableValue("Data type active", ivalue))
                {
                    SetScalarDataTypeTo(ivalue);
                }
            }
            else
            {
                delete[] colorBarValueBlue;
                delete[] colorBarValueRed;
                delete[] useLogLut;
                delete[] numColorBarLabels;
                delete[] colorBarLabelPrecision;
                delete[] solidDisplayMode;
                delete[] applySolidThreshold;
                delete[] numColorBands;
                delete[] solidThresholdMin;
                delete[] solidThresholdMax;
                delete[] usingRegularIsosurfaces;
                delete[] numberOfRegularIsosurfaces;
                delete[] regularIsosurfaceMin;
                delete[] regularIsosurfaceMax;
                delete[] numberOfCustomIsosurfaces;
                for (i = 0; i < numDataTypes; i++)
                {
                    if (customIsosurfaceValues[i])
                    {
                        delete[] customIsosurfaceValues[i];
                    }
                }
                delete[] customIsosurfaceValues;
            }
        }
    }

    // Surface lighting
    SetDiffuseLighting(1.0); // diffuse lighting is hard wired
    // if (hashTable->GetHashTableValue("Lighting diffuse", fvalue))
    // {
    //     SetDiffuseLighting(fvalue);
    // }
    if (hashTable->GetHashTableValue("Lighting ambient", fvalue))
    {
        SetAmbientLighting(fvalue);
    }
    if (hashTable->GetHashTableValue("Lighting specular", fvalue))
    {
        SetSpecularLighting(fvalue);
    }
    if (hashTable->GetHashTableValue("Lighting Specular power", fvalue))
    {
        SetSpecularPower(fvalue);
    }

    // Grid Shell
    if (hashTable->GetHashTableValue("Grid shell color red", f1) && hashTable->GetHashTableValue("Grid shell color green", f2) && hashTable->GetHashTableValue("Grid shell color blue", f3))
    {
        SetGridShellColor(f1, f2, f3);
    }
    if (hashTable->GetHashTableValue("Grid shell color opacity", fvalue))
    {
        SetGridShellOpacity(fvalue);
    }

    // Scale
    if (hashTable->GetHashTableValue("Scale x", f1) && hashTable->GetHashTableValue("Scale y", f2) && hashTable->GetHashTableValue("Scale z", f3))
    {
        SetScale(f1, f2, f3);
    }

    // Bounding box
    if (hashTable->GetHashTableValue("Bounding box color option", fvalue))
    {
        double v = (fvalue)*0.5;
        SetBoundingBoxColor(v, v, v);
    }

    // Subgrid
    if (hashTable->GetHashTableValue("Subgrid i min", i1) && hashTable->GetHashTableValue("Subgrid i max", i2) && hashTable->GetHashTableValue("Subgrid j min", i3) && hashTable->GetHashTableValue("Subgrid j max", i4) && hashTable->GetHashTableValue("Subgrid k min", i5) && hashTable->GetHashTableValue("Subgrid k max", i6))
    {
        SetScalarSubgridExtent(i1, i2, i3, i4, i5, i6);
    }
    if (hashTable->GetHashTableValue("Subgrid active", ivalue) && ivalue)
    {
        ScalarSubgridOn();
    }
    else
    {
        ScalarSubgridOff();
    }

    // Axes
    if (hashTable->GetHashTableValue("Axes representation", ivalue) && ivalue == 0)
    {
        SetAxesRepresentationToTube();
    }
    else
    {
        SetAxesRepresentationToLine();
    }
    if (hashTable->GetHashTableValue("Axes normalized size", fvalue))
    {
        SetAxesNormalizedSize(fvalue);
    }
    if (hashTable->GetHashTableValue("Axes normalized tube diameter", fvalue))
    {
        SetAxesNormalizedTubeDiameter(fvalue);
    }

    // Grid Lines
    if (hashTable->GetHashTableValue("Grid lines Position x", f1) && hashTable->GetHashTableValue("Grid lines Position y", f2) && hashTable->GetHashTableValue("Grid lines Position z", f3))
    {
        SetGridLinePositions(f1, f2, f3);
    }
    if (hashTable->GetHashTableValue("Grid lines color option", fvalue))
    {
        double v = fvalue * 0.5;
        SetGridLineColor(v, v, v);
    }

    // Grid Layer
    if (hashTable->GetHashTableValue("Grid layer Position", ivalue))
    {
        SetGridLayerPosition(ivalue);
    }

    // Grid Display
    if (hashTable->GetHashTableValue("Grid display mode", ivalue))
    {
        if (ivalue == MV_GRID_INTERPOLATED)
        {
            SetGridDisplayToInterpolated();
        }
        else if (ivalue == MV_GRID_STAIRSTEPPED)
        {
            SetGridDisplayToStairstepped();
        }
    }

    // Crop
    if (hashTable->GetHashTableValue("Crop bounds x min", f1) && hashTable->GetHashTableValue("Crop bounds x max", f2) && hashTable->GetHashTableValue("Crop bounds y min", f3) && hashTable->GetHashTableValue("Crop bounds y max", f4) && hashTable->GetHashTableValue("Crop bounds z min", f5) && hashTable->GetHashTableValue("Crop bounds z max", f6) && hashTable->GetHashTableValue("Crop angle", fvalue))
    {
        Crop(f1, f2, f3, f4, f5, f6, fvalue);
    }
    if (hashTable->GetHashTableValue("Cropped away pieces red", f1) && hashTable->GetHashTableValue("Cropped away pieces green", f2) && hashTable->GetHashTableValue("Cropped away pieces blue", f3))
    {
        SetCroppedAwayPiecesColor(f1, f2, f3);
    }
    if (hashTable->GetHashTableValue("Cropped away pieces opacity", fvalue))
    {
        SetCroppedAwayPiecesOpacity(fvalue);
    }

    // Vector

    if (hashTable->GetHashTableValue("Vector data", ivalue) && ivalue && m_DataSource->GetVectorArray())
    {
        if (hashTable->GetHashTableValue("Vector subsample i min", i1) && hashTable->GetHashTableValue("Vector subsample i max", i2) && hashTable->GetHashTableValue("Vector subsample i rate", i3) && hashTable->GetHashTableValue("Vector subsample j min", i4) && hashTable->GetHashTableValue("Vector subsample j max", i5) && hashTable->GetHashTableValue("Vector subsample j rate", i6) && hashTable->GetHashTableValue("Vector subsample k min", i7) && hashTable->GetHashTableValue("Vector subsample k max", i8) && hashTable->GetHashTableValue("Vector subsample k rate", i9))
        {
            SubsampleVectors(i1, i2, i3, i4, i5, i6, i7, i8, i9);
        }
        if (hashTable->GetHashTableValue("Vector threshold apply", ivalue) && ivalue)
        {
            VectorThresholdOn();
        }
        else
        {
            VectorThresholdOff();
        }
        if (hashTable->GetHashTableValue("Vector threshold lower limit", f1) && hashTable->GetHashTableValue("Vector threshold upper limit", f2))
        {
            SetVectorThresholdLimits(f1, f2);
        }
        if (hashTable->GetHashTableValue("Vector glyph active", ivalue))
        {
            ActivateVectorGlyph(ivalue);
        }
        if (hashTable->GetHashTableValue("Vector glyph x length", f1) && hashTable->GetHashTableValue("Vector glyph y length", f2) && hashTable->GetHashTableValue("Vector glyph z length", f3))
        {
            m_CubeSource->SetXLength(f1);
            m_CubeSource->SetYLength(f2);
            m_CubeSource->SetZLength(f3);
        }
        if (hashTable->GetHashTableValue("Vector color option", fvalue))
        {
            double v = (fvalue)*0.5;
            SetVectorColor(v, v, v);
        }

        if (hashTable->GetHashTableValue("Vector Crop bounds x min", f1) && hashTable->GetHashTableValue("Vector Crop bounds x max", f2) && hashTable->GetHashTableValue("Vector Crop bounds y min", f3) && hashTable->GetHashTableValue("Vector Crop bounds y max", f4) && hashTable->GetHashTableValue("Vector Crop bounds z min", f5) && hashTable->GetHashTableValue("Vector Crop bounds z max", f6) && hashTable->GetHashTableValue("Vector Crop angle", ivalue))
        {
            CropVectors(f1, f2, f3, f4, f5, f6, ivalue);
        }
        else
        {
            CropVectors(0, 1, 0, 1, 0, 1, 0);
        }
        if (hashTable->GetHashTableValue("Vector Log transform", ivalue))
        {
            SetLogTransformVector(ivalue);
        }
        if (hashTable->GetHashTableValue("Vector Line Width", fvalue))
        {
            SetVectorLineWidth(fvalue);
        }
        if (hashTable->GetHashTableValue("Vector scale factor", fvalue))
        {
            SetVectorScaleFactor(fvalue);
        }
    }

    // Pathline
    if (hashTable->GetHashTableValue("Pathline data", ivalue) && ivalue)
    {
        if (hashTable->GetHashTableValue("Pathline representation", ivalue) && ivalue == MV_PATHLINE_TUBE)
        {
            SetPathlineRepresentationToTube();
        }
        else
        {
            SetPathlineRepresentationToLine();
        }
        if (hashTable->GetHashTableValue("Pathline tube diameter", fvalue))
        {
            SetPathlineTubeDiameter(fvalue);
        }
        if (hashTable->GetHashTableValue("Pathline time blue", f1) && hashTable->GetHashTableValue("Pathline time red", f2))
        {
            SetPathlineColorBarEndPoints(f1, f2);
        }
        if (hashTable->GetHashTableValue("Pathline threshold time min", f1) && hashTable->GetHashTableValue("Pathline threshold time max", f2))
        {
            SetPathlineTimeClippingRange(f1, f2);
        }
        if (hashTable->GetHashTableValue("Pathline threshold mode", ivalue))
        {
            SetPathlineTimeClippingMode(ivalue);
        }
        if (hashTable->GetHashTableValue("Pathline log transform", ivalue))
        {
            SetPathlineLogTransform(ivalue);
        }
        else
        {
            SetPathlineLogTransform(0);
        }
    }

    // Model Features
    if (hashTable->GetHashTableValue("Number of model feature types", ivalue) && ivalue)
    {
        int      numberOfModelFeatureTypes = ivalue;
        int     *modelFeatureDisplayOrder  = new int[numberOfModelFeatureTypes];
        double **modelFeatureRgba          = new double *[numberOfModelFeatureTypes];
        for (i = 0; i < numberOfModelFeatureTypes; i++)
        {
            modelFeatureRgba[i] = 0;
        }
        char key[100];
        int  ierror = 0;
        for (i = 0; i < numberOfModelFeatureTypes; i++)
        {
            sprintf(key, "Model feature type %u display order", i);
            if (!hashTable->GetHashTableValue(key, modelFeatureDisplayOrder[i]))
            {
                ierror = 1;
                break;
            }
            modelFeatureRgba[i] = new double[4];
            sprintf(key, "Model feature type %u red", i);
            if (!hashTable->GetHashTableValue(key, modelFeatureRgba[i][0]))
            {
                ierror = 1;
                break;
            }
            sprintf(key, "Model feature type %u green", i);
            if (!hashTable->GetHashTableValue(key, modelFeatureRgba[i][1]))
            {
                ierror = 1;
                break;
            }
            sprintf(key, "Model feature type %u blue", i);
            if (!hashTable->GetHashTableValue(key, modelFeatureRgba[i][2]))
            {
                ierror = 1;
                break;
            }
            sprintf(key, "Model feature type %u alpha", i);
            if (!hashTable->GetHashTableValue(key, modelFeatureRgba[i][3]))
            {
                ierror = 1;
                break;
            }
        }

        if (!ierror && numberOfModelFeatureTypes == m_DataSource->GetNumberOfModelFeatureTypes())
        {
            int *displayOrder = m_ModelFeatures->GetDisplayOrder();
            int  consistent   = 1;
            for (i = 0; i < numberOfModelFeatureTypes; i++)
            {
                if ((displayOrder[i] != -2 && modelFeatureDisplayOrder[i] == -2) ||
                    (displayOrder[i] == -2 && modelFeatureDisplayOrder[i] != -2))
                {
                    consistent = 0;
                    break;
                }
            }
            if (consistent)
            {
                m_ModelFeatures->SetDisplayOrder(modelFeatureDisplayOrder);
            }
            for (i = 0; i < numberOfModelFeatureTypes; i++)
            {
                m_ModelFeatures->SetColor(i, modelFeatureRgba[i]);
            }
        }
        for (i = 0; i < numberOfModelFeatureTypes; i++)
        {
            if (modelFeatureRgba[i])
            {
                delete[] modelFeatureRgba[i];
            }
        }
        delete[] modelFeatureDisplayOrder;
        delete[] modelFeatureRgba;
    }
    if (hashTable->GetHashTableValue("Model feature glyph size", fvalue))
    {
        m_ModelFeatures->SetDefaultGlyphSize(fvalue);
    }

    // Overlay
    if (hashTable->GetHashTableValue("Overlay data type", ivalue) && ivalue)
    {
        SetOverlayType(ivalue);
    }
    double dvalue, d1, d2;
    if (hashTable->GetHashTableValue("Overlay x coordinate at grid origin", d1) && hashTable->GetHashTableValue("Overlay y coordinate at grid origin", d2))
    {
        SetOverlayCoordinatesAtGridOrigin(d1, d2);
    }
    if (hashTable->GetHashTableValue("Overlay to grid scale", d1))
    {
        SetOverlayToGridScale(d1);
    }
    if (hashTable->GetHashTableValue("Overlay angle", dvalue))
    {
        SetOverlayAngle(dvalue);
    }
    if (hashTable->GetHashTableValue("Overlay drape", ivalue))
    {
        SetOverlayDrape(ivalue);
    }
    if (hashTable->GetHashTableValue("Overlay trim", ivalue))
    {
        SetOverlayTrim(ivalue);
    }
    if (hashTable->GetHashTableValue("Overlay crop", ivalue))
    {
        SetOverlayCrop(ivalue);
    }
    if (hashTable->GetHashTableValue("Overlay elevation", dvalue))
    {
        SetOverlayElevation(dvalue);
    }
    if (hashTable->GetHashTableValue("Overlay drape gap", dvalue))
    {
        SetOverlayDrapeGap(dvalue);
    }
    char  filename[1024];
    const char *errMsg = nullptr;
    if (hashTable->GetHashTableValue("Overlay file", filename))
    {
        SetOverlayFileName(mvSaveCurrentDirectory::GetFullPath(filename, dirname.c_str()).c_str());
        if (!UpdateOverlay(errMsg))
        {
            strcat(m_WarningMessage, "Unable to load overlay file. Overlay will not be displayed.");
        }
    }

    gui->Deserialize(hashTable);

    delete hashTable;
}

char *mvManager::GetDataFileList() const
{
    if (m_DataSource != 0)
    {
        return m_DataSource->GetDataFileList();
    }
    else
    {
        return 0;
    }
}

int mvManager::HasVectorData() const
{
    if (m_DataSource == 0)
    {
        return 0;
    }
    else
    {
        return (m_DataSource->GetVectorArray() != 0);
    }
}

int mvManager::HasPathlineData() const
{
    if (m_DataSource == 0)
    {
        return 0;
    }
    else
    {
        return (m_DataSource->GetNumberOfPathlines() > 0);
    }
}

int mvManager::HasOverlay()
{
    return m_Overlay->HasData();
}

void mvManager::ShowOverlay()
{
    m_Overlay->VisibilityOn();
}

void mvManager::HideOverlay()
{
    m_Overlay->VisibilityOff();
}

int mvManager::IsOverlayVisible() const
{
    return m_Overlay->GetVisibility();
}

void mvManager::ClearOverlayData()
{
    m_Overlay->ClearData();
}

void mvManager::SetOverlayFileName(const char *filename)
{
    m_Overlay->SetFileName(filename);
}

void mvManager::SetOverlayType(int type)
{
    m_Overlay->SetType(type);
}

void mvManager::SetOverlayCoordinatesAtGridOrigin(double xorig, double yorig)
{
    m_Overlay->SetCoordinatesAtGridOrigin(xorig, yorig);
}

void mvManager::SetOverlayElevation(double elev)
{
    m_Overlay->SetElevation(elev);
}

void mvManager::SetOverlayAngle(double angle)
{
    m_Overlay->SetAngle(angle);
}

void mvManager::SetOverlayToGridScale(double scale)
{
    m_Overlay->SetOverlayToGridScale(scale);
}

void mvManager::SetOverlayDrape(int d)
{
    m_Overlay->SetDrape(d);
}

void mvManager::SetOverlayTrim(int d)
{
    m_Overlay->SetTrim(d);
}

void mvManager::SetOverlayCrop(int d)
{
    m_Overlay->SetCrop(d);
}

void mvManager::SetOverlayDrapeGap(double d)
{
    m_Overlay->SetDrapeGap(d);
}

int mvManager::UpdateOverlay(const char *errMsg)
{
    return m_Overlay->Update(errMsg);
}

char *mvManager::GetOverlayFileName()
{
    return m_Overlay->GetFileName();
}

int mvManager::GetOverlayType()
{
    return m_Overlay->GetType();
}

void mvManager::GetOverlayCoordinatesAtGridOrigin(double &xorig, double &yorig)
{
    m_Overlay->GetCoordinatesAtGridOrigin(xorig, yorig);
}

double mvManager::GetOverlayElevation()
{
    return m_Overlay->GetElevation();
}

double mvManager::GetOverlayAngle()
{
    return m_Overlay->GetAngle();
}

double mvManager::GetOverlayToGridScale()
{
    return m_Overlay->GetOverlayToGridScale();
}

int mvManager::GetOverlayDrape()
{
    return m_Overlay->GetDrape();
}

double mvManager::GetOverlayDrapeGap()
{
    return m_Overlay->GetDrapeGap();
}

int mvManager::GetOverlayTrim()
{
    return m_Overlay->GetTrim();
}

int mvManager::GetOverlayCrop()
{
    return m_Overlay->GetCrop();
}

void mvManager::GetOverlayBounds(double &xmin, double &xmax, double &ymin, double &ymax)
{
    m_Overlay->GetBounds(xmin, xmax, ymin, ymax);
}

void mvManager::RemoveOverlay()
{
    m_Overlay->ClearData();
    m_Overlay->SetFileName(0);
    m_Overlay->SetCoordinatesAtGridOrigin(0, 0);
    m_Overlay->SetOverlayToGridScale(1);
    m_Overlay->SetAngle(0);
    m_Overlay->SetDrape(0);
    m_Overlay->SetTrim(0);
    m_Overlay->SetCrop(0);
    m_Overlay->SetElevation(0);
    m_Overlay->SetDrapeGap(0);
    m_Overlay->VisibilityOff();
}

bool mvManager::GetIsStructuredGrid()
{
    return (GetGridType() == GridType::MV_STRUCTURED_GRID);
};

void mvManager::SetBoundingBoxBounds()
{
    double bounds[6];
    // if (m_DataSource->GetPrimaryScalarMode() == MV_CELL_SCALARS)
    //{
    m_ActiveScalarDataSet->Update();
    m_ActiveScalarDataSet->GetOutput()->ComputeBounds();
    m_ActiveScalarDataSet->GetOutput()->GetBounds(bounds);
    m_BoundingBox->SetBounds(bounds);
    //}
    // else
    //{
    //	m_ScalarStructuredGrid->ComputeBounds();
    //	m_ScalarStructuredGrid->GetBounds(bounds);
    //	m_BoundingBox->SetBounds(bounds);
    //}
}

int mvManager::IsGridLayerVisible()
{
    return m_GridLayer->GetVisibility();
}

void mvManager::ShowGridLayer()
{
    m_GridLayer->SetVisibility(1);
}

void mvManager::HideGridLayer()
{
    m_GridLayer->SetVisibility(0);
}

void mvManager::SetGridLayerPosition(int layerNumber)
{
    m_GridLayer->SetLayerPosition(layerNumber);
}

int mvManager::GetGridLayerPosition() const
{
    return m_GridLayer->GetLayerPosition();
}

void SetGridLayerColor(double r, double g, double b)
{
}

const double *mvManager::GetGridLayerColor() const
{
    return m_GridLayer->GetColor();
}

int mvManager::GetNumberOfLayersInUnstructuredGrid() const
{
    return m_DataSource->GetNumberOfCellLayers();
}

void mvManager::SetGridDisplayToInterpolated()
{
    if (m_DataSource->GetGridType() == GridType::MV_UNSTRUCTURED_GRID)
    {
        return;
    }
    if (m_DataSource->GetGridType() == GridType::MV_STRUCTURED_GRID)
    {
        if (IsScalarSubgridOn())
        {
            m_ActiveScalarDataSet->SetInputConnection(m_ExtractStructuredGridForSubgrid->GetOutputPort());
        }
        else
        {
            m_ActiveScalarDataSet->SetInputData(m_ScalarStructuredGrid);
        }
    }
    else if (m_DataSource->GetGridType() == GridType::MV_LAYERED_GRID)
    {
        m_ExtractCellsForSubgrid->SetInputData(m_ScalarLayeredGrid);
        if (IsScalarSubgridOn())
        {
            m_ActiveScalarDataSet->SetInputConnection(m_ExtractCellsForSubgrid->GetOutputPort());
            m_BlockySolidThreshold->SetInputConnection(m_ExtractCellsForSubgrid->GetOutputPort());
        }
        else
        {
            m_ActiveScalarDataSet->SetInputData(m_ScalarLayeredGrid);
            m_BlockySolidThreshold->SetInputData(m_ScalarLayeredGrid);
        }
        m_ModelFeatures->SetGridPoints(m_InterpolatedGridPoints);
        m_ModelFeatures->SetCellArrayForUnstructuredGrid(m_ScalarLayeredGrid->GetCellTypesArray(),
                                                         m_ScalarLayeredGrid->GetCellLocationsArray(),
                                                         m_ScalarLayeredGrid->GetCells());
        m_ModelFeatures->Update();
    }
    m_GridDisplayMode = MV_GRID_INTERPOLATED;
    BuildPipelineForSolid();
}

void mvManager::SetGridDisplayToStairstepped()
{
    if (m_DataSource->GetGridType() == GridType::MV_UNSTRUCTURED_GRID)
    {
        return;
    }
    m_SolidMapper->SetScalarModeToUseCellData();
    m_ExtractCellsForSubgrid->SetInputData(m_StairsteppedGrid);
    if (IsScalarSubgridOn())
    {
        m_ActiveScalarDataSet->SetInputConnection(m_ExtractCellsForSubgrid->GetOutputPort());
        m_BlockySolidThreshold->SetInputConnection(m_ExtractCellsForSubgrid->GetOutputPort());
    }
    else
    {
        m_ActiveScalarDataSet->SetInputData(m_StairsteppedGrid);
        m_BlockySolidThreshold->SetInputData(m_StairsteppedGrid);
    }
    m_ModelFeatures->SetGridPoints(m_StairsteppedGridPoints);
    m_ModelFeatures->SetCellArrayForUnstructuredGrid(m_StairsteppedGrid->GetCellTypesArray(),
                                                     m_StairsteppedGrid->GetCellLocationsArray(),
                                                     m_StairsteppedGrid->GetCells());
    m_ModelFeatures->Update();

    m_GridDisplayMode = MV_GRID_STAIRSTEPPED;
    // Set solid display mode to blocky for all data types
    for (int i = 0; i < m_NumScalarDataTypes; i++)
    {
        m_SolidDisplayMode[i] = SolidDisplayType::MV_SOLID_BLOCKY;
    }
    BuildPipelineForSolid();
}

int mvManager::GetGridDisplayMode()
{
    return m_GridDisplayMode;
}
