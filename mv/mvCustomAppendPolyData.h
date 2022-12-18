#ifndef __mvCustomAppendPolyData_h
#define __mvCustomAppendPolyData_h

#include "mvHeader.h"
#include "vtkAppendPolyData.h"

class MV_EXPORT mvCustomAppendPolyData : public vtkAppendPolyData
{
public:
    vtkTypeMacro(mvCustomAppendPolyData, vtkAppendPolyData);

    static mvCustomAppendPolyData *New();
    const char                    *GetClassName() { return "mvCustomAppendPolyData"; }
    // void PrintSelf(ostream& os, vtkIndent indent);

protected:
    mvCustomAppendPolyData();
    ~mvCustomAppendPolyData();
    int FillInputPortInformation(int, vtkInformation *) override;
};

#endif
