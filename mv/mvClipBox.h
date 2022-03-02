#ifndef __mvClipBox_h
#define __mvClipBox_h

#include "vtkImplicitFunction.h"

class mvClipBox : public vtkImplicitFunction
{
public:
    mvClipBox();
    static mvClipBox *New() { return new mvClipBox; };
    void              SetModelBounds(double bounds[6]);
    void              SetBounds(double bounds[6]);
    virtual double    EvaluateFunction(double x[3]);
    virtual void      EvaluateGradient(double x[3], double g[3]);
    void              SetAngle(int angle);

protected:
    double m_ModelBounds[6];
    double m_Bounds[6];
    int    m_Angle;
    void   UpdateInternalBounds();

private:
    double m_InternalBounds[6];
    double m_RadianAngle;
};

#endif
