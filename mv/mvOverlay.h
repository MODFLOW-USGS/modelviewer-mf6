#ifndef __mvOverlay_h
#define __mvOverlay_h

#include "mvDisplayObject.h"

#define MV_ESRI_SHAPEFILE_OVERLAY 1
#define MV_DXF_ASCII_OVERLAY      2

class vtkPolyData;
class vtkClipPolyData;
class vtkStructuredGrid;
class vtkPlane;

class MV_EXPORT mvOverlay : public mvDisplayObject
{
public:
    mvOverlay();
    ~mvOverlay();

    void  SetFileName(const char *filename);
    void  SetType(int type);
    void  SetCoordinatesAtGridOrigin(double xorig, double yorig);
    void  SetAngle(double angle);
    void  SetElevation(double elev);
    void  SetOverlayToGridScale(double scale);
    void  SetDrape(int b);
    void  SetTrim(int b);
    void  SetCrop(int b);
    void  SetDrapeGap(double gap);

    char *GetFileName() { return m_FileName; }
    int   GetType() { return m_Type; }
    void  GetCoordinatesAtGridOrigin(double &xorig, double &yorig)
    {
        xorig = m_XAtGridOrigin;
        yorig = m_YAtGridOrigin;
    }
    double GetAngle() { return m_Angle; }
    double GetElevation() { return m_Elevation; }
    double GetOverlayToGridScale() { return m_Scale; }
    int    GetDrape() { return m_Drape; }
    int    GetTrim() { return m_Trim; }
    int    GetCrop() { return m_Crop; }
    double GetDrapeGap() { return m_DrapeGap; }
    void   GetBounds(double &xmin, double &xmax, double &ymin, double &ymax)
    {
        xmin = m_XMin;
        xmax = m_XMax;
        ymin = m_YMin;
        ymax = m_YMax;
    }

    int  Update(const char *errMsg);
    void ClearData();
    int  HasData();
    void SetFullGrid(vtkStructuredGrid *fg);
    void SetSubgrid(vtkStructuredGrid *sg);
    void SubgridOn();
    void SubgridOff();
    void SetCropperPlaneOrigin(int i, double x, double y, double z);
    void SetCropAngle(double cropAngle);

protected:
    double             m_XAtGridOrigin;
    double             m_YAtGridOrigin;
    double             m_XMin;
    double             m_XMax;
    double             m_YMin;
    double             m_YMax;
    double             m_Angle;
    double             m_Elevation;
    double             m_Scale;
    double             m_DrapeGap;
    double           **m_XCoord;
    double           **m_YCoord;
    int               *m_NumVert;
    int                m_NumEntities;
    int                m_NewFile;
    char              *m_FileName;
    int                m_Drape;
    int                m_Trim; // If m_Trim = 1, then the portions of the overlay outside the bounding box are not shown
    int                m_Crop; // If m_Crop = 1, then the portions of the overlay beyond the cropping planes are not shown
    int                m_Type;
    int                m_SubgridIsActivated;
    vtkPolyData       *m_Lines;
    vtkStructuredGrid *m_FullGrid;
    vtkStructuredGrid *m_Subgrid;
    // objects for trimming
    vtkClipPolyData   *m_ClipXMin;
    vtkClipPolyData   *m_ClipXMax;
    vtkClipPolyData   *m_ClipYMin;
    vtkClipPolyData   *m_ClipYMax;
    vtkPlane          *m_PlaneXMin;
    vtkPlane          *m_PlaneXMax;
    vtkPlane          *m_PlaneYMin;
    vtkPlane          *m_PlaneYMax;
    // objects for cropping
    vtkPlane          *m_Plane[4];
    vtkClipPolyData   *m_Cropper[4];

    int                ReadESRIShapeFile(const char *errMsg);
    int                ReadDxfFile(const char *errMsg);
    void               Build();
};

#endif
