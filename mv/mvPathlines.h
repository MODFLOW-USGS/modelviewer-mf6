#ifndef __mvPathlines_h
#define __mvPathlines_h

#include "mvDisplayObject.h"

class vtkCleanPolyData;
class vtkClipPolyData;
class vtkPlane;
class vtkPolyData;
class vtkTransform;
class vtkTransformPolyDataFilter;
class vtkTubeFilter;
class vtkLookupTable;
class vtkLogLookupTable;

#define PATHLINE_LINE 1
#define PATHLINE_TUBE 0

class MV_EXPORT mvPathlines : public mvDisplayObject
{
public:
    mvPathlines();
    virtual ~mvPathlines();

    void   SetInputData(vtkPolyData *polydata);
    void   SetRepresentationToTube();
    void   SetRepresentationToLine();
    int    GetRepresentation() const;
    void   SetDefaultTubeDiameter(double d);
    void   SetNormalizedTubeDiameter(double d);
    double GetNormalizedTubeDiameter() const;
    void   SetScale(double xScale, double yScale, double zScale);
    void   DoCrop(const double *bounds);
    void   CropOff();
    int    IsCroppingOn() const;
    void   TimeClippingOn();
    void   TimeClippingOff();
    int    IsTimeClippingOn() const;
    void   SetTimeClippingRange(double minTime, double maxTime);
    void   SetColorBarEndPoints(double valueBlue, double valueRed);
    double GetTimeRed() const { return m_TimeRed; }
    double GetTimeBlue() const { return m_TimeBlue; }
    int    GetLogTransform() { return m_LogTransform; }
    void   SetLogTransform(int Value);
    void   SetMinPositiveValue(double Value);
    double GetMinPositiveValue() const { return m_MinPositiveValue; }

protected:
    vtkLookupTable             *m_LutRedToBlue;
    vtkLookupTable             *m_LutBlueToRed;
    vtkLogLookupTable          *m_LogLutRedToBlue;
    vtkLogLookupTable          *m_LogLutBlueToRed;
    vtkPolyData                *m_Input;
    vtkClipPolyData            *m_ClipMin;
    vtkClipPolyData            *m_ClipMax;
    vtkTransform               *m_Transform;
    vtkTransformPolyDataFilter *m_TransformFilter;
    vtkTubeFilter              *m_Tube;
    vtkPlane                   *m_Plane[6];
    vtkClipPolyData            *m_Cropper[6];
    int                         m_TimeClipping;
    int                         m_Cropping;
    int                         m_Representation;
    double                      m_DefaultTubeDiameter;
    double                      m_TimeRed;
    double                      m_TimeBlue;
    double                      m_MinPositiveValue;
    int                         m_LogTransform;
    void                        BuildPipeLine();
};

#endif
