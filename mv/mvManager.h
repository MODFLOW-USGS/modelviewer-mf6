#ifndef __mvManager_h
#define __mvManager_h

#include "mvDataSource.h"

#include <vtkSmartPointer.h>

#include <cstdint>
#include <math.h>
#include <math.h>
#include <typeinfo>
#include <algorithm>
#include <string>

class mvCustomAppendPolyData;
class mvGUISettings;
class mvPathlines;
class mvGridLines;
class mvGridLayer;
class mvGridShell;
class mvBoundingBox;
class mvAxes;
class mvModelFeatures;
class mvColorBar;
class mvDisplayText;
class mvOverlay;
class mvClipBox;

class vtkActor;
class vtkBandedPolyDataContourFilter;
class vtkCellArray;
class vtkClipPolyData;
class vtkContourFilter;
class vtkCubeSource;
class vtkCutter;
class vtkDataSetMapper;
class vtkExtractGrid;
class vtkExtractCells;
class vtkGeometryFilter;
class vtkGlyph3D;
class vtkHedgeHog;
class vtkLookupTable;
class vtkLogLookupTable;
class vtkPlane;
class vtkPoints;
class vtkPolyData;
class vtkPolyDataMapper;
class vtkPropCollection;
class vtkDoubleArray;
class vtkStructuredGrid;
class vtkThreshold;
class vtkThresholdPoints;
class vtkPointSet;
class vtkMaskPoints;
class vtkExtractGeometry;
class vtkUnstructuredGrid;

using std::max;
using std::min;

class MV_EXPORT mvManager
{
public:
    mvManager();
    virtual ~mvManager();

    static std::string                 GetDirName(const char *fullPath);
    static std::string                 GetRelativePath(const char *pszFrom, const char *pszTo);

    // Data set and animation control
    // void ClearData();
    const mvDataSource *               GetDataSource() const;
    const char *                       LoadData(char *modelType, char *dataFileList);
    char                              *GetDataFileList() const;
    int                                HasVectorData() const;
    int                                HasPathlineData() const;
    //	int AreAllCellsActive() const;
    GridType                           GetGridType() const;
    bool                               GetIsStructuredGrid();
    void                               ApplyDefaultSettings();
#if ((VTK_MAJOR_VERSION == 8) && (VTK_MINOR_VERSION <= 1) || (VTK_MAJOR_VERSION < 8))  //  https://vtk.org/Wiki/VTK/API_Changes_8_0_1_to_8_1_0
    void                               SetImmediateModeRendering(int b);  // deprecated vtk 8.1
#endif
    void                               SetReleaseDataFlag(int b);
    const int                         *GetScalarGridDimensions() const;
    const int                         *GetVectorGridDimensions() const;
    int                                GetNumberOfLayersInUnstructuredGrid() const;
    vtkSmartPointer<vtkPropCollection> GetPropCollection() const { return m_PropCollection; }
    int                                GetNumberOfTimePoints() const;
    char                             **GetTimePointLabels();
    int                                GetNumberOfScalarDataTypes() const;
    char                             **GetDataTypeLabels() const;
    int                                GetCurrentTimePointIndex() const { return m_TimePointIndex; }
    void                               SetScalarDataTypeTo(int dataTypeIndex);
    int                                GetActiveScalarDataType() const { return m_ActiveDataType; }
    const char *                       GetActiveScalarDataName() const;
    ScalarMode                         GetPrimaryScalarMode() const;
    const char *                       GetModelName() const;
    int                                GetTimeLabelOption() const;
    void                               SetTimePointTo(int timePointIndex);
    void                               AdvanceOneTimePoint();
    int                                GetInitialDisplayTimePoint();
    char                              *GetWarningMessage() { return m_WarningMessage; }
    void                               ClearWarningMessage() { m_WarningMessage[0] = '\0'; }

    // Scalar data display
    void                               HideScalarData();
    void                               ShowScalarDataAsSolid();
    void                               ShowScalarDataAsIsosurfaces();
    int                                IsSolidVisible() const;
    int                                AreIsosurfacesVisible() const;
    void                               SetNumberOfColorBands(int numberOfColorBands);
    int                                GetNumberOfColorBands() const;

    // Scalar Sub grid
    void                               SetScalarSubgridExtent(int imin, int imax, int jmin, int jmax, int kmin, int kmax);
    void                               ScalarSubgridOn();
    void                               ScalarSubgridOff();
    int                                IsScalarSubgridOn() const;
    const int                         *GetScalarSubgridExtent();

    // Vectors
    void                               ShowVectors();
    void                               HideVectors();
    int                                AreVectorsVisible() const;
    void                               SetVectorColor(double red, double green, double blue);
    const double                      *GetVectorColor() const;
    void                               SetVectorScaleFactor(double scaleFactor);
    double                             GetVectorScaleFactor() const;
    double                             GetVectorLineWidth();
    void                               SetVectorLineWidth(double width);

    double                             ComputeOptimalVectorSize();
    void                               SetVectorSizeToOptimal();
    void                               SubsampleVectors(int imin, int imax, int irate, int jmin, int jmax, int jrate,
                                                        int kmin, int kmax, int krate);
    void                               GetVectorSubsampleExtentAndRate(int *extent, int *rate);
    void                               VectorThresholdOn();
    void                               VectorThresholdOff();
    int                                IsVectorThresholdOn() const;
    void                               SetVectorThresholdLimits(double minValue, double maxValue);
    void                               GetVectorThresholdLimits(double *limits) const;
    int                                GetLogTransformVector() { return m_VectorLog10Transform; };
    void                               SetLogTransformVector(int Value);
    void                               CropVectors(double xmin, double xmax, double ymin,
                                                   double ymax, double zmin, double zmax, int cropAngle);
    int                                GetVectorCroppingAngle() { return m_VectorClippingAngle; };
    const double                      *GetVectorCropBounds() const { return m_VectorBounds; }

    // Vector Glyph (The small cube at the base of the vector)
    void                               ActivateVectorGlyph(int active);
    int                                IsVectorGlyphActivated() const;
    void                               EnlargeVectorGlyph();
    void                               ShrinkVectorGlyph();

    // Pathlines
    void                               ShowPathlines();
    void                               HidePathlines();
    int                                ArePathlinesVisible() const;
    void                               SetPathlineRepresentationToLine();
    void                               SetPathlineRepresentationToTube();
    int                                GetPathlineRepresentation() const;
    void                               SetPathlineTubeDiameter(double diameter);
    double                             GetPathlineTubeDiameter() const;
    void                               SetPathlineTimeClippingMode(int mode);
    int                                GetPathlineTimeClippingMode() const { return m_PathlineTimeClippingMode; }
    void                               SetPathlineTimeClippingRange(double minTime, double maxTime);
    void                               SetPathlineColorBarEndPoints(double valueBlue, double valueRed);
    double                             GetPathlineClipTimeMin() const;
    double                             GetPathlineClipTimeMax() const;
    double                             GetPathlineTimeBlue() const;
    double                             GetPathlineTimeRed() const;
    void                               GetPathlineTimeRange(double *range) const;
    void                               UpdatePathlineScalars();
    int                                GetPathlineLogTransform() const;
    void                               SetPathlineLogTransform(int Value);

    // ModelFeatures
    void                               ShowModelFeatures();
    void                               HideModelFeatures();
    int                                AreModelFeaturesVisible() const;
    int                               *GetModelFeatureDisplayOrder();
    void                               SetModelFeatureDisplayOrder(int *displayOrder);
    int                                HasModelFeatures() const;
    int                                GetNumberOfModelFeatureTypes() const;
    const char                        *GetModelFeatureLabels() const;
    void                               EnlargeModelFeatureGlyphs();
    void                               ShrinkModelFeatureGlyphs();
    int                                GetModelFeatureDisplayMode() const;
    void                               SetModelFeatureColor(const char *modelFeatureName, double *rgba);
    void                               GetModelFeatureColor(const char *modelFeatureName, double *rgba);

    // Overlay
    int                                HasOverlay();
    void                               ShowOverlay();
    void                               HideOverlay();
    int                                IsOverlayVisible() const;
    void                               ClearOverlayData();
    void                               SetOverlayFileName(const char *filename);
    void                               SetOverlayType(int type);
    void                               SetOverlayCoordinatesAtGridOrigin(double xorig, double yorig);
    void                               SetOverlayElevation(double elev);
    void                               SetOverlayAngle(double angle);
    void                               SetOverlayToGridScale(double scale);
    void                               SetOverlayDrape(int b);
    void                               SetOverlayDrapeGap(double gap);
    void                               SetOverlayTrim(int b);
    void                               SetOverlayCrop(int b);
    char                              *GetOverlayFileName();
    int                                GetOverlayType();
    void                               GetOverlayCoordinatesAtGridOrigin(double &xorig, double &yorig);
    double                             GetOverlayElevation();
    double                             GetOverlayAngle();
    double                             GetOverlayToGridScale();
    int                                GetOverlayDrape();
    double                             GetOverlayDrapeGap();
    int                                GetOverlayTrim();
    int                                GetOverlayCrop();
    void                               GetOverlayBounds(double &xmin, double &xmax, double &ymin, double &ymax);
    int                                UpdateOverlay(const char *errMsg);
    void                               RemoveOverlay();

    // Grid shell
    int                                IsGridShellVisible() const;
    void                               ShowGridShell();
    void                               HideGridShell();
    void                               SetGridShellColor(double red, double green, double blue);
    void                               SetGridShellOpacity(double opacity);
    const double                      *GetGridShellColor() const;
    double                             GetGridShellOpacity() const;

    // Grid lines
    int                                AreActivatedGridLinesVisible() const;
    void                               ShowActivatedGridLines();
    void                               HideGridLines();
    void                               ActivateGridLines(int i);
    void                               DeactivateGridLines(int i);
    void                               SetGridLinePositions(int posX, int posY, int posZ);
    void                               SetGridLineColor(double r, double g, double b);
    int                                AreGridLinesActive(int i) const;
    void                               GetGridLinePositions(int *p) const;
    const double                      *GetGridLineColor() const;

    // Grid layer
    int                                IsGridLayerVisible();
    void                               ShowGridLayer();
    void                               HideGridLayer();
    void                               SetGridLayerPosition(int layerNumber);
    int                                GetGridLayerPosition() const;
    void                               SetGridLayerColor(double r, double g, double b);
    const double                      *GetGridLayerColor() const;

    // Grid display
    void                               SetGridDisplayToInterpolated();
    void                               SetGridDisplayToStairstepped();
    int                                GetGridDisplayMode();

    // Bounding Box
    int                                IsBoundingBoxVisible() const;
    void                               ShowBoundingBox();
    void                               HideBoundingBox();
    void                               SetBoundingBoxColor(double r, double g, double b);
    const double                      *GetBoundingBoxColor() const;
    void                               SetBoundingBoxBounds();

    // Axes
    int                                AreAxesVisible() const;
    void                               ShowAxes();
    void                               HideAxes();
    double                             GetDefaultAxesSize();
    double                             GetDefaultAxesTubeDiameter();
    void                               SetAxesNormalizedSize(double size);
    void                               SetAxesNormalizedPosition(double x, double y, double z);
    void                               SetAxesNormalizedTubeDiameter(double d);
    double                             GetAxesNormalizedSize() const;
    double                             GetAxesNormalizedTubeDiameter() const;
    const double                      *GetAxesNormalizedPosition() const;
    void                               SetAxesRepresentationToLine();
    void                               SetAxesRepresentationToTube();
    int                                GetAxesRepresentation() const;

    // Time Label
    int                                IsTimeLabelVisible() const;
    void                               ShowTimeLabel();
    void                               HideTimeLabel();
    void                               SetTimeLabelPosition(double x, double y);
    const double                      *GetTimeLabelPosition() const;
    void                               SetTimeLabelFontSize(int size);
    int                                GetTimeLabelFontSize() const;
    void                               SetTimeLabelColor(double r, double g, double b);
    const double                      *GetTimeLabelColor() const;

    // Title Label
    int                                IsTitleVisible() const;
    void                               ShowTitle();
    void                               HideTitle();
    void                               SetTitlePosition(double x, double y);
    const double                      *GetTitlePosition() const;
    void                               SetTitleFontSize(int size);
    int                                GetTitleFontSize() const;
    void                               SetTitleColor(double r, double g, double b);
    const double                      *GetTitleColor() const;

    // Color Bar
    void                               SetColorBarEndPoints(double valueBlue, double valueRed);
    int                                IsColorBarVisible() const;
    void                               ShowColorBar();
    void                               HideColorBar();
    double                             GetColorBarValueBlue() const;
    double                             GetColorBarValueRed() const;
    int                                GetColorBarColorScheme() const;
    void                               UseLinearColorBar();
    void                               UseLogColorBar();
    int                                IsColorBarLinear() const;
    bool                               IsColorBarNormal() const;
    void                               SetColorBarWidth(int w);
    void                               SetColorBarHeight(int h);
    void                               SetColorBarFontSize(int f);
    void                               SetColorBarOffset(int r);
    void                               SetColorBarTextColor(double r, double g, double b);
    void                               SetColorBarNumberOfLabels(int n);
    void                               SetColorBarLabelPrecision(int d);
    void                               SetColorBarColorScheme(int value);
    int                                GetColorBarWidth() const;
    int                                GetColorBarHeight() const;
    int                                GetColorBarFontSize() const;
    int                                GetColorBarOffset() const;
    const double                      *GetColorBarTextColor() const;
    int                                GetColorBarNumberOfLabels() const;
    int                                GetColorBarLabelPrecision() const;
    std::uint32_t                      GetColorBarFirstCustomColor() const;
    std::uint32_t                      GetColorBarLastCustomColor() const;
    void                               SetColorBarFirstCustomColor(std::uint32_t value);
    void                               SetColorBarLastCustomColor(std::uint32_t value);
    int                                GetColorBarSource() const;
    void                               SetColorBarSource(int value);

    // Lighting
    void                               SetDiffuseLighting(double diffuse);
    void                               SetAmbientLighting(double ambient);
    void                               SetSpecularLighting(double specular);
    void                               SetSpecularPower(double specularPower);
    double                             GetDiffuseLighting() const;
    double                             GetAmbientLighting() const;
    double                             GetSpecularLighting() const;
    double                             GetSpecularPower() const;

    // X-Y-Z Scaling
    void                               SetScale(double xScale, double yScale, double zScale);
    const double                      *GetScale() const;

    // Data Range
    void                               GetScalarDataRange(double *range) const;
    void                               GetVectorMagnitudeRange(double *range) const;

    // Solid Control
    void                               SetSolidDisplayToBlocky();
    void                               SetSolidDisplayToSmooth();
    void                               SetSolidDisplayToBanded();
    void                               SolidThresholdOn();
    void                               SolidThresholdOff();
    int                                IsSolidThresholdOn() const;
    void                               SetSolidThresholdLimits(double minValue, double maxValue);
    SolidDisplayType                   GetSolidDisplayMode() const;
    void                               GetSolidThresholdLimits(double *limits) const;

    // Isosurface Control
    void                               SetRegularIsosurfaces(int numIsosurface, double minValue, double maxValue);
    void                               SetCustomIsosurfaces(int count, const double *values);
    int                                GetNumberOfRegularIsosurfaces() const;
    int                                GetNumberOfCustomIsosurfaces() const;
    void                               GetRegularIsosurfaceRange(double *range) const;
    const double                      *GetCustomIsosurfaceValues() const;
    int                                UsingRegularIsosurfaces() const;

    // Cropping
    void                               Crop(double xmin, double xMax, double yMin, double yMax, double zMin, double zMax, double cropAngle);
    const double                      *GetCropBounds() const { return m_CropBounds; }
    double                             GetHorizontalCropAngle() const;
    void                               ShowCroppedAwayPieces();
    void                               HideCroppedAwayPieces();
    int                                AreCroppedAwayPiecesShown() const;
    void                               SetCroppedAwayPiecesColor(double red, double green, double blue);
    void                               SetCroppedAwayPiecesOpacity(double opacity);
    const double                      *GetCroppedAwayPiecesColor() const;
    double                             GetCroppedAwayPiecesOpacity() const;

    // Serialization
    char                              *Serialize(const char *fileName, mvGUISettings *gui) const;
    void                               Deserialize(const char *fileName, mvGUISettings *gui, std::string &errorMsg);

protected:
    int                                             m_ActivatedGridLinesVisibility;
    int                                             m_ActiveDataType;
    int                                             m_ColorBarDataSource;
    int                                             m_DoVectorThreshold;
    int                                             m_GridLinesActivated[3];
    int                                             m_NumScalarDataTypes;
    int                                             m_PathlineTimeClippingMode;
    int                                             m_ShowCroppedAwayPieces;
    int                                             m_TimePointIndex; // used to keep track of time point when animating
    int                                             m_VectorClippingAngle;
    int                                             m_VectorGlyphActivated;
    int                                             m_VectorLog10Transform;
    int                                             m_VectorSubsampleOffset[3];
    int                                             m_GridDisplayMode;
    double                                          m_CropAngle;
    double                                          m_MinPositiveVector;
    double                                          m_PathlineClipTimeMax;
    double                                          m_PathlineClipTimeMin;
    double                                          m_VectorClippingXMin;
    double                                          m_VectorClippingXMax;
    double                                          m_VectorClippingYMin;
    double                                          m_VectorClippingYMax;
    double                                          m_VectorClippingZMin;
    double                                          m_VectorClippingZMax;
    double                                          m_ActiveScalarRange[2]; // need this to avoid scalar value for inactive cells
    double                                          m_VectorLogMagnitudeRange[2];
    double                                          m_VectorMagnitudeRange[2];
    double                                          m_CropBounds[6]; // These boundaries are relative wrt the bounding box
    double                                          m_VectorBounds[6];
    char                                            m_WarningMessage[500];

    int                                            *m_ColorBarLabelPrecision;
    int                                            *m_DoSolidThreshold;
    int                                            *m_NumColorBarLabels;
    int                                            *m_NumberOfColorBands;
    int                                            *m_NumberOfCustomIsosurfaces;
    int                                            *m_NumberOfRegularIsosurfaces;
    SolidDisplayType                               *m_SolidDisplayMode;
    int                                            *m_UseLogColorBar;
    int                                            *m_UseRegularIsosurface;

    double                                         *m_ColorBarValueBlue;
    double                                         *m_ColorBarValueRed;
    double                                         *m_RegularIsosurfaceMax;
    double                                         *m_RegularIsosurfaceMin;
    double                                         *m_ScaledVectorArray;
    double                                         *m_SolidThresholdMax;
    double                                         *m_SolidThresholdMin;
    double                                         *m_VectorLogMagnitudeArray;
    double                                         *m_VectorMagnitudeArray;
    double                                        **m_CustomIsosurfaceValues;

    // Base class pointer to the data source
    mvDataSource                                   *m_DataSource;

    // Scalar Data Set
    vtkSmartPointer<vtkStructuredGrid>              m_ScalarStructuredGrid;
    vtkSmartPointer<vtkUnstructuredGrid>            m_ScalarLayeredGrid;
    vtkSmartPointer<vtkUnstructuredGrid>            m_ScalarUnstructuredGrid;
    vtkSmartPointer<vtkPoints>                      m_InterpolatedGridPoints;
    vtkSmartPointer<vtkDoubleArray>                 m_PointScalars;
    vtkSmartPointer<vtkDoubleArray>                 m_CellScalars;

    vtkSmartPointer<vtkUnstructuredGrid>            m_StairsteppedGrid;
    vtkSmartPointer<vtkPoints>                      m_StairsteppedGridPoints;

    // Vector Data Set
    vtkSmartPointer<vtkStructuredGrid>              m_VectorDataSet;
    vtkSmartPointer<vtkUnstructuredGrid>            m_VectorUnstructuredGrid;
    vtkSmartPointer<vtkPoints>                      m_VectorGridPoints;
    vtkSmartPointer<vtkDoubleArray>                 m_Vectors;
    vtkSmartPointer<vtkDoubleArray>                 m_VectorMagnitudes;

    // Pathline Data Set
    vtkSmartPointer<vtkPolyData>                    m_PathlineDataSet;
    vtkSmartPointer<vtkPoints>                      m_PathlinePoints;
    vtkSmartPointer<vtkDoubleArray>                 m_PathlineScalars;
    vtkSmartPointer<vtkCellArray>                   m_PathlineLines;

    // Mappers
    vtkSmartPointer<vtkPolyDataMapper>              m_SolidMapper;
    vtkSmartPointer<vtkPolyDataMapper>              m_CroppedAwayPiecesMapper;
    vtkSmartPointer<vtkPolyDataMapper>              m_IsosurfaceMapper;
    vtkSmartPointer<vtkPolyDataMapper>              m_VectorMapper;
    vtkSmartPointer<vtkPolyDataMapper>              m_VectorGlyphMapper;

    // Lookup tables
    vtkLookupTable                                 *m_LutRedToBlue;
    vtkLookupTable                                 *m_LutBlueToRed;
    vtkLookupTable                                 *m_LutModifiedRedToBlue;
    vtkLookupTable                                 *m_LutModifiedBlueToRed;
    vtkLookupTable                                 *m_LutCustomScale;
    vtkLookupTable                                 *m_LutReversedCustomScale;

    vtkLogLookupTable                              *m_LogLutRedToBlue;
    vtkLogLookupTable                              *m_LogLutBlueToRed;
    vtkLogLookupTable                              *m_LogLutModifiedRedToBlue;
    vtkLogLookupTable                              *m_LogLutModifiedBlueToRed;
    vtkLogLookupTable                              *m_LogLutCustomScale;
    vtkLogLookupTable                              *m_LogLutReversedCustomScale;

    // Actors
    vtkSmartPointer<vtkActor>                       m_SolidActor;
    vtkSmartPointer<vtkActor>                       m_CroppedAwayPiecesActor;
    vtkSmartPointer<vtkActor>                       m_IsosurfaceActor;
    vtkSmartPointer<vtkActor>                       m_VectorActor;
    vtkSmartPointer<vtkActor>                       m_VectorGlyphActor;
    vtkSmartPointer<vtkPropCollection>              m_PropCollection;

    // Filters to create grid lines, bounding box, full solid, and isosurfaces
    vtkSmartPointer<vtkExtractGrid>                 m_ExtractStructuredGridForSubgrid;
    vtkSmartPointer<vtkThreshold>                   m_ActiveScalarDataSet;
    vtkSmartPointer<vtkGeometryFilter>              m_ActiveScalarDataSetGeometry;
    vtkSmartPointer<vtkContourFilter>               m_Isosurface;

    // Filter to create Color bands
    vtkSmartPointer<vtkBandedPolyDataContourFilter> m_ColorBandFilter;

    // Filters to create the smooth solid
    vtkSmartPointer<vtkClipPolyData>                m_GridShellClipMin;
    vtkSmartPointer<vtkClipPolyData>                m_GridShellClipMax;
    vtkSmartPointer<vtkContourFilter>               m_SmoothSolidIsosurface;
    mvCustomAppendPolyData                         *m_SmoothSolid;

    // Filters to create the solid cuttings
    mvCustomAppendPolyData                         *m_CroppedAwayPieces;

    // Filters to create the blocky solid
    vtkSmartPointer<vtkThreshold>                   m_BlockySolidThreshold;
    vtkSmartPointer<vtkGeometryFilter>              m_BlockySolid;
    vtkSmartPointer<vtkExtractCells>                m_ExtractCellsForSubgrid;

    // Filters for cropping
    vtkSmartPointer<vtkPlane>                       m_Plane[6];
    vtkSmartPointer<vtkClipPolyData>                m_Cropper[6];

    // Cropped Solid
    vtkSmartPointer<vtkCutter>                      m_ExtractFace[6];
    vtkSmartPointer<vtkClipPolyData>                m_FaceCrop[24];
    mvCustomAppendPolyData                         *m_Faces;
    vtkSmartPointer<vtkClipPolyData>                m_FacesClipMin;
    vtkSmartPointer<vtkClipPolyData>                m_FacesClipMax;
    mvCustomAppendPolyData                         *m_CroppedSolid;
    vtkSmartPointer<vtkThreshold>                   m_FacesThreshold;
    vtkSmartPointer<vtkGeometryFilter>              m_FacesThresholdGeometry;

    // Cropped Isosurface
    vtkSmartPointer<vtkCutter>                      m_IsosurfaceCutter[3];

    // Filter to create vector lines
    vtkSmartPointer<vtkExtractGrid>                 m_ExtractStructuredGridVector;
    //	vtkSmartPointer<vtkMaskPoints> m_ExtractUnstructuredGridVector;
    vtkSmartPointer<vtkExtractGeometry>             m_CropVectors;
    mvClipBox                                      *m_CropBoxForVectors;

    vtkSmartPointer<vtkThresholdPoints>             m_ActiveVectorDataSet;
    vtkSmartPointer<vtkThresholdPoints>             m_VectorThreshold;
    vtkSmartPointer<vtkHedgeHog>                    m_HedgeHog;
    vtkSmartPointer<vtkCubeSource>                  m_CubeSource;
    vtkSmartPointer<vtkGlyph3D>                     m_VectorGlyph;

    mvPathlines                                    *m_Pathlines;
    mvGridLines                                    *m_GridLines[3];

    mvGridLayer                                    *m_GridLayer;

    mvGridShell                                    *m_GridShell;
    mvBoundingBox                                  *m_BoundingBox;
    mvAxes                                         *m_Axes;
    mvModelFeatures                                *m_ModelFeatures;
    mvColorBar                                     *m_ColorBar;
    mvDisplayText                                  *m_TimeLabel;
    mvDisplayText                                  *m_Title;

    mvOverlay                                      *m_Overlay;

    // Protected methods
    //	void SetAllCellsAreActive(int b);
    void                                            ComputeActiveScalarRange();
    void                                            ComputeVectorMagnitudes();
    void                                            OnDataModified();
    void                                            UpdateScaledVectorArray();
    void                                            UpdateColorBands();
    void                                            UpdateCrop();
    void                                            BuildPipelineForSolid();
    void                                            BuildPipelineForIsosurface();
    //	void ReleaseArrayMemory();
    //	void UpdateLogTransformedData();

    friend class CMvDoc;
};

#endif
