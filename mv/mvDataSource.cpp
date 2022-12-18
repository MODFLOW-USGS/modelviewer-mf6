#include "mvDataSource.h"
#include <string.h>

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvDataSource::mvDataSource()
{
    m_DataFileList                = 0;
    m_NumberOfTimePoints          = 0;
    m_TimePointLabels             = 0;
    m_NumberOfScalarDataTypes     = 0;
    m_DataTypeLabels              = 0;
    m_ScalarGridDim[0]            = 0;
    m_ScalarGridDim[1]            = 0;
    m_ScalarGridDim[2]            = 0;
    m_InterpolatedGridCoordinates = 0;
    m_ScalarArray                 = 0;
    m_ScalarArrayOffset           = 0;
    // The following value of 1.0e30 is repeated in the
    // dll's for reading subsidence in MODFLOW
    m_InactiveCellValue           = 1.0e30;
    m_VectorGridDim[0]            = 0;
    m_VectorGridDim[1]            = 0;
    m_VectorGridDim[2]            = 0;
    m_VectorGridCoordinates       = 0;
    m_VectorArray                 = 0;
    m_NumberOfPathlines           = 0;
    m_NumberOfPathlineCoordinates = 0;
    m_PathlineCoordinates         = 0;
    //	m_PathlinePointArray = 0;
    m_PathlineScalarArray         = 0;
    m_MaxTimePathlineScalarArray  = 0;
    m_MinTimePathlineScalarArray  = 0;
    m_NumberOfModelFeatureTypes   = 0;
    m_ModelFeatureArray           = 0;
    m_ModelFeatureLabels          = 0;
    m_Particle_Coord              = 0;
    m_Particle_Concentrations     = 0;
    m_NumberOfParticles           = 0;
    m_PathsBackwardsInTime        = 0;
    m_PathLineScalarMode          = MP_TRAVEL_TIME;
    m_StairsteppedGridCoordinates = 0;
}

mvDataSource::~mvDataSource()
{
    if (m_DataFileList) delete[] m_DataFileList;
    if (m_NumberOfTimePoints > 0 && m_TimePointLabels)
    {
        for (int i = 0; i < m_NumberOfTimePoints; i++)
        {
            delete[] m_TimePointLabels[i];
        }
        delete m_TimePointLabels;
    }
    if (m_NumberOfScalarDataTypes > 0 && m_DataTypeLabels)
    {
        for (int i = 0; i < m_NumberOfScalarDataTypes; i++)
        {
            delete[] m_DataTypeLabels[i];
        }
        delete m_DataTypeLabels;
    }
    if (m_InterpolatedGridCoordinates) delete[] m_InterpolatedGridCoordinates;
    if (m_ScalarArray) delete[] m_ScalarArray;
    if (m_VectorGridCoordinates) delete[] m_VectorGridCoordinates;
    if (m_VectorArray) delete[] m_VectorArray;
    if (m_PathlineCoordinates) delete[] m_PathlineCoordinates;
    //	if (m_PathlinePointArray) delete [] m_PathlinePointArray;
    if (m_PathlineScalarArray) delete[] m_PathlineScalarArray;
    if (m_MaxTimePathlineScalarArray) delete[] m_MaxTimePathlineScalarArray;
    if (m_MinTimePathlineScalarArray) delete[] m_MinTimePathlineScalarArray;
    if (m_ModelFeatureArray) delete[] m_ModelFeatureArray;
    if (m_ModelFeatureLabels) delete[] m_ModelFeatureLabels;
    if (m_Particle_Coord) delete[] m_Particle_Coord;
    if (m_Particle_Concentrations) delete[] m_Particle_Concentrations;
    if (m_StairsteppedGridCoordinates) delete[] m_StairsteppedGridCoordinates;
}

bool mvDataSource::ParseDataFileList(char *&pList, char *fileName)
{
    char *end = strchr(pList, '\n');
    if (end == NULL)
    {
        return 0;
    }
    else
    {
        int len = end - pList;
        strncpy(fileName, pList, len);
        *(fileName + len) = '\0';
        pList += len + 1;
        return 1;
    }
}

int mvDataSource::GetDataSetToUseForRange()
{
    if (GetPrimaryScalarMode() == ScalarMode::MV_CELL_SCALARS)
    {
        return MV_USE_CELL_DATA_FOR_RANGE;
    }
    else
    {
        return MV_USE_POINT_DATA_FOR_RANGE;
    }
}

double *mvDataSource::GetPathlineScalarArray()
{
    switch (m_PathLineScalarMode)
    {
    case MP_TRAVEL_TIME:
        return m_PathlineScalarArray;
    case MP_MIN_TRAVEL_TIME:
        return m_MinTimePathlineScalarArray;
    case MP_MAX_TRAVEL_TIME:
        return m_MaxTimePathlineScalarArray;
    default:
        return m_PathlineScalarArray;
    }
}
