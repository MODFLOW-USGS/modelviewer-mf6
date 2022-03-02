#ifndef __mvGridLines_h
#define __mvGridLines_h

#include "mvDisplayObject.h"
#include "vtkSmartPointer.h"

class vtkStructuredGrid;
class vtkStructuredGridGeometryFilter;
class vtkThreshold;
class vtkGeometryFilter;
class vtkExtractEdges;
class vtkUnstructuredGrid;
class vtkExtractCells;

class MV_EXPORT mvGridLines : public mvDisplayObject
{
public:
    mvGridLines();
    virtual ~mvGridLines();

    void       SetInputData(vtkStructuredGrid *dataSet);
    void       SetInputData(vtkUnstructuredGrid *dataSet, int numCol, int numRow, int numLay);
    void       SetThresholdMin(double value);
    const int *GetExtent() const;
    void       SetExtent(int imin, int imax, int jmin, int jmax, int kmin, int kmax);
    void       DoThresholdUsingCellData();
    void       DoThresholdUsingPointData();
    void       SetAllCellsAreActive(int b);

protected:
    vtkSmartPointer<vtkStructuredGridGeometryFilter> m_SGGeometryFilter;
    vtkSmartPointer<vtkThreshold>                    m_Threshold;
    vtkSmartPointer<vtkExtractEdges>                 m_WireFrame;
    vtkSmartPointer<vtkGeometryFilter>               m_GeometryFilter;
    vtkSmartPointer<vtkExtractCells>                 m_ExtractCells;
    int                                              m_GridDisplayMode;
    int                                              m_Extent[6];
    int                                              m_NumCol;
    int                                              m_NumRow;
    int                                              m_NumLay;
};

#endif
