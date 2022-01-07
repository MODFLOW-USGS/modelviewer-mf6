#ifndef __mvModelList_h
#define __mvModelList_h

#include "mvDataSource.h"

class MV_EXPORT mvModelList  
{
public:

	static int GetNumberOfModels();

	static void GetModelNames(char **buffer);
	
	static mvDataSource *CreateDataSource(char *model);
};

#endif
