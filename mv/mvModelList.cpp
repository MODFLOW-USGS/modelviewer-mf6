// comment out any of the following definitions to
// exclude supporting the corresponding model
#define MV_MODFLOW6

#include "mvModelList.h"
#include "mvUtil.h"

#ifdef MV_MODFLOW6
#include "Modflow6DataSource.h"
#endif

#include <string.h>

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int mvModelList::GetNumberOfModels()
{
    // This method is not used because this version of Model Viewer supports only Modflow 6
    int n = 0;

#ifdef MV_MODFLOW6
    n++;
#endif

    return n;
}

void mvModelList::GetModelNames(char **buffer)
{
    // This method is not used because this version of Model Viewer supports only Modflow 6
    int n = 0;

#ifdef MV_MODFLOW6
    strcpy(buffer[n], Modflow6DataSource::GetNameStatic());
    n++;
#endif
}

mvDataSource *mvModelList::CreateDataSource(char *model)
{
#ifdef MV_MODFLOW6
    if (mvUtil::stricmp(model, Modflow6DataSource::GetNameStatic()) == 0)
    {
        return new Modflow6DataSource;
    }
#endif

    return 0;
}
