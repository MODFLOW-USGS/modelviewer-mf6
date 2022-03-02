#ifndef __mvDisplayObject_h
#define __mvDisplayObject_h

#include "mvHeader.h"
#include "vtkActor.h"
#include "vtkLODActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkDataSetMapper.h"

class MV_EXPORT mvDisplayObject
{
public:
    mvDisplayObject();
    virtual ~mvDisplayObject();

    vtkActor     *GetActor() { return m_ActiveActor; }
    void          VisibilityOn();
    void          VisibilityOff();
    void          SetVisibility(int v);
    int           GetVisibility();
    void          SetColor(double red, double green, double blue);
    void          SetOpacity(double p);
    void          SetDiffuse(double d);
    void          SetAmbient(double a);
    void          SetSpecular(double s);
    void          SetSpecularPower(double sp);
    const double *GetColor();
    double        GetOpacity();
    double        GetDiffuse();
    double        GetAmbient();
    double        GetSpecular();
    double        GetSpecularPower();
    void          SetScale(double xScale, double yScale, double zScale);
    void          SetImmediateModeRendering(int b);
    void          UseLODActor(int b);
    void          SetNumberOfCloudPoints(int n);
    void          SetScalarRange(double min, double max);

protected:
    vtkMapper   *m_Mapper;
    vtkActor    *m_Actor;
    vtkLODActor *m_LODActor;
    vtkActor    *m_ActiveActor;
    void         SetMapperInputData(vtkPolyData *in);
    void         SetMapperInputData(vtkDataSet *in);
    void         SetMapperInputConnection(vtkAlgorithmOutput *in);
};

// Note: for a display object to be used, its actor must be added
// to m_PropCollection. Usually this is done in mvManager::mvManager().

#endif
