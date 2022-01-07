#ifndef __ModpathReader_h
#define __ModpathReader_h

#include "mvHeader.h"
#include "mvModelFeatures.h"
#include "vtkIdTypeArray.h"

/**
 * Abstract base class to specify dataset behavior. 
 */
class MV_EXPORT ModpathReader  
{
public:
	static void ReadData(char *pathlineFile, int &numPathlines, int &numCoordinates,
			double *&coordinates, double *&scalarArrayTime,  double *&scalarArrayMaxTime, 
			double *&scalarArrayMinTime, vtkIdType *&pointArray, bool backwards, double &minPositiveTime);
};
#endif
