#ifndef __mvModelFeatures_h
#define __mvModelFeatures_h

#include "mvDisplayObject.h"
#include "vtkSmartPointer.h"

#define MV_DISPLAY_MODEL_FEATURES_AS_CELLS  1
#define MV_DISPLAY_MODEL_FEATURES_AS_GLYPHS 2

class vtkCubeSource;
class vtkGlyph3D;
class vtkPoints;
class vtkPolyData;
class vtkStructuredGrid;
class vtkUnstructuredGrid;
class vtkThreshold;
class vtkThresholdPoints;
class vtkTransform;
class vtkTransformPolyDataFilter;
class vtkLookupTable;
class vtkPointSet;
class vtkUnsignedCharArray;
class vtkIdTypeArray;
class vtkCellArray;

class MV_EXPORT mvModelFeatures : public mvDisplayObject
{
public:
    mvModelFeatures();
    virtual ~mvModelFeatures();

    void   SetDisplayMode(int displayMode);
    int    GetDisplayMode() const;
    void   SetNumberOfModelFeatureTypes(int numModelFeatureTypes);
    void   SetModelFeatureArray(int *modelFeatureArray);
    void   SetGridPoints(vtkPoints *gridPoints);
    void   SetScale(double xScale, double yScale, double zScale);
    void   Build();
    void   Update();

    void   SetDisplayOrder(int *displayOrder);
    int   *GetDisplayOrder();
    void   GetColor(int i, double *rgba);
    void   SetColor(int i, double *rgba);
    void   UpdateDisplay();
    void   SetDefaultGlyphSize(double s);
    void   EnlargeGlyphs();
    void   ShrinkGlyphs();
    double GetGlyphSize();

    void   SetGridTypeToStructuredGrid();
    void   SetGridTypeToUnstructuredGrid();

    void   SetFullGridDimensions(const int *dim);
    void   SetSubgridExtent(int imin, int imax, int jmin, int jmax, int kmin, int kmax);
    void   SubgridOn();
    void   SubgridOff();

    void   SetCellArrayForUnstructuredGrid(vtkUnsignedCharArray *cellTypes, vtkIdTypeArray *cellLocations, vtkCellArray *cells);

protected:
    int                                         m_DisplayMode;
    int                                         m_NumberOfModelFeatureTypes;
    int                                        *m_ModelFeatureArray;
    int                                        *m_DisplayOrder;
    double                                     *m_CellScalarArray;
    int                                         m_NumberOfGlyphs;
    int                                         m_NumberOfCells;
    double                                      m_DefaultGlyphSize;
    double                                     *m_GlyphScalarArray;

    int                                         m_Dim[3];
    int                                         m_Subgrid[6];
    int                                         m_SubgridIsActivated;

    vtkSmartPointer<vtkLookupTable>             m_Lut;
    vtkSmartPointer<vtkStructuredGrid>          m_StructuredGrid;
    vtkSmartPointer<vtkUnstructuredGrid>        m_UnstructuredGrid;
    vtkSmartPointer<vtkThreshold>               m_ThresholdCells;

    vtkSmartPointer<vtkPoints>                  m_GridPoints;
    vtkSmartPointer<vtkPolyData>                m_PolyData;
    vtkSmartPointer<vtkGlyph3D>                 m_Glyph;
    vtkSmartPointer<vtkCubeSource>              m_CubeSource;
    vtkSmartPointer<vtkThresholdPoints>         m_ThresholdPoints;
    vtkSmartPointer<vtkTransform>               m_Transform;
    vtkSmartPointer<vtkTransformPolyDataFilter> m_TransformFilter;

    vtkUnsignedCharArray                       *m_CellTypes;
    vtkIdTypeArray                             *m_CellLocations;
    vtkCellArray                               *m_Cells;

    void                                        SetDisplayOrderForGlyphs();
    void                                        SetDisplayOrderForCells();
};

#endif
