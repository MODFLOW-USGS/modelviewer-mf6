#ifndef __mvCustomAppendPolyData_h
#define __mvCustomAppendPolyData_h

#include "mvHeader.h"
#include "vtkAppendPolyData.h"

class MV_EXPORT mvCustomAppendPolyData : public vtkAppendPolyData
{
public:
    mvCustomAppendPolyData();
    ~mvCustomAppendPolyData();
    static mvCustomAppendPolyData *New() { return new mvCustomAppendPolyData; };
    const char                    *GetClassName() { return "mvCustomAppendPolyData"; }
    // void PrintSelf(ostream& os, vtkIndent indent);

protected:
    virtual int FillInputPortInformation(int, vtkInformation *);
};

#endif
