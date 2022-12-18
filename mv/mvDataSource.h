#ifndef __mvDataSource_h
#define __mvDataSource_h

#include "mvHeader.h"
#include "mvDefine.h"
//#include "vtkCellArray.h"

#include <string>

/**
 * Abstract base class to specify dataset behavior.
 */
class MV_EXPORT mvDataSource
{
public:
    mvDataSource();
    virtual ~mvDataSource();

    /**
     * Returns the name of the dataset.
     */
    virtual const char *GetName()                              = 0;

    /**
     * Returns the "primary mode" of the scalars. This is either
     * cell scalars (MV_CELL_SCALARS), which are defined for cells,
     * or point scalars (MV_POINT_SCALARS), which are defined at
     * points. Cell scalars will also have a secondary dataset which
     * are values interpolated to points (cell corners). This is needed
     * for drawing isosurfaces. Point scalars do not have a secondary dataset.
     */
    virtual ScalarMode  GetPrimaryScalarMode()                 = 0;

    /**
     * Returns the type of grid. The possibilities are: 1) vtkStructuredPoints
     * with all active cells, 2) vtkRectilinearGrid with all active cells,
     * 3) vtkStructuredGrid with all active cells, 4) vtkStructuredGrid that
     * might contain inactive cells, and 5) vtkUnstructuredGrid..
     */
    virtual GridType    GetGridType()                          = 0;

    /**
     * Returns the convention used for indexing.
     */
    //   virtual int GetIndexConvention() = 0;

    /**
     * Returns 1 if all cells in this data source are active
     * Returns 0 if not all cells in this data source are active
     */
    //   virtual int AreAllCellsActive() = 0;

    /**
     * Reads data from data files. Reads the grid information such as dimensions,
     * point coordinates, and number of time points.
     */
    virtual const char *LoadData(char *dataFileList)           = 0;

    /**
     * Reads data for the next time point. Reads scalar, vector, and pathline
     * data for the next time point.
     */
    virtual void        AdvanceOneTimePoint()                  = 0;

    /**
     * Reads data for the specified time point. Reads scalar, vector, and
     * Pathline data for the next time point.
     */
    virtual void        SetTimePointTo(int timePointIndex)     = 0;

    /**
     * Specifies the type of scalar data to be returned.
     */
    virtual void        SetScalarDataTypeTo(int dataTypeIndex) = 0;

    /**
     * Returns the number of time points in the dataset.
     */
    virtual int         GetNumberOfTimePoints() { return m_NumberOfTimePoints; }

    /**
     * Returns the time point labels. This is usually the times in any
     * array of character strings.
     */
    virtual char **     GetTimePointLabels() { return m_TimePointLabels; }

    /**
     * Returns the number of scalar data types in the dataset.
     */
    virtual int         GetNumberOfScalarDataTypes() { return m_NumberOfScalarDataTypes; }

    /**
     * Returns an array of character strings containing the data type labels.
     */
    virtual char **     GetDataTypeLabels() { return m_DataTypeLabels; }

    /**
     * Returns the string containing the data files. The data file names
     * are all in a single string separated by '\n'.
     */
    virtual char *      GetDataFileList() { return m_DataFileList; }

    /**
     * Returns the dimensions (number of columns, rows, layers) of the
     * grid for the scalar dataset.
     */
    virtual int *       GetScalarGridDimensions() { return m_ScalarGridDim; }

    /**
     * Returns the coordinates of grid points for the scalar data set
     */
    virtual double *    GetInterpolatedGridCoordinates() { return m_InterpolatedGridCoordinates; }
    virtual double *    GetStairsteppedGridCoordinates() { return m_StairsteppedGridCoordinates; }
    virtual int         GetStairsteppedGridCoordinatesSize() { return 0; }

    /**
     * Returns the scalar data.
     */
    virtual double *    GetScalarArray() { return m_ScalarArray + m_ScalarArrayOffset; }

    /**
     * Returns the scalar value that indicates that the cell is inactive.
     */
    virtual double      GetInactiveCellValue() { return m_InactiveCellValue; }

    /**
     * Returns the dimensions of the grid for the vector dataset.
     */
    virtual int *       GetVectorGridDimensions() { return m_VectorGridDim; }

    /**
     * Returns the coordinates of grid points for the vector data set.
     */
    virtual double *    GetVectorGridCoordinates() { return m_VectorGridCoordinates; }

    /**
     * Returns the vector data
     */
    virtual double *    GetVectorArray() { return m_VectorArray; }

    /**
     * Returns the number of pathlines in the data set
     */
    virtual int         GetNumberOfPathlines() { return m_NumberOfPathlines; }

    /**
     * Returns the total number of coordinate points used to define all pathlines.
     */
    virtual int         GetNumberOfPathlineCoordinates() { return m_NumberOfPathlineCoordinates; }

    /**
     * Returns the coordinates of pathline points.
     */
    virtual double *    GetPathlineCoordinates() { return m_PathlineCoordinates; }

    /**
     * Returns the array of points defining the pathlines.
     */
    //	virtual vtkIdType *GetPathlinePointArray() {return m_PathlinePointArray;}

    /**
     * Returns the scalar data associated with each pathline point.
     */
    virtual double *    GetPathlineScalarArray();

    /**
     * Returns the number of different types of model features in the model.
     * Model Features are usually boundary conditions.
     */

    virtual int         GetNumberOfModelFeatureTypes() { return m_NumberOfModelFeatureTypes; }

    /**
     * Returns the array describing the locations of the Model Features.
     * The structure of the array is as follows:
     * 1. The first number in the array is the number of Model Features of the first type (n).
     *    For example, if the first type of Model Features is "wells" and there are 17 wells,
     *    the first number in the array would be 17.
     * 2. The next n members of the array would be integers that indicate where the first
     *    type of Model feature is located.  In the example above, the next 17 members of
     *    the array would be the locations of the 17 wells.
     * This structure repeats for each successive type of model features.
     */

    virtual int *       GetModelFeatureArray() { return m_ModelFeatureArray; }

    /**
     * Returns the labels of each of different types of model features in the model.
     */

    virtual char *      GetModelFeatureLabels() { return m_ModelFeatureLabels; }

    /**
     * Returns whether Model Features should be shown or not.
     */

    virtual int         GetModelFeatureDisplayMode()                     = 0;

    /**
     * Returns an array of colors to assign to Model Features by default.
     */

    virtual void        GetDefaultModelFeatureColor(int i, double *rgba) = 0;

    virtual int         GetTimeLabelOption() { return 0; }

    virtual int         GetInitialGridDisplayPolicy() { return MV_INITIAL_DISPLAY_BOUNDING_BOX; }

    virtual int         GetInitialDisplayTimePoint() { return 0; }

    virtual double *    GetParticleCoord() { return m_Particle_Coord; }
    virtual double *    GetParticleConcentrations() { return m_Particle_Concentrations; }
    virtual int         GetParticleCount() { return m_NumberOfParticles; }
    virtual int         GetDataSetToUseForRange();

    /**
     * In data sources that represent unstructured grids, BuildCellsForDisvGrid
     * defines how the grid is connected.
     */
    //	virtual void BuildCellsForDisvGrid(int *types, vtkCellArray* connectivity) {return;};
    virtual int         GetNumVTKPoints()  = 0;
    virtual int         GetNumVTKCells()   = 0;
    virtual int         GetNumModelCells() = 0;

    virtual std::string GetTimeUnit() { return std::string(); }

    virtual int         GetPathlineScalarMode() { return m_PathLineScalarMode; }
    virtual void        SetPathlineScalarMode(int Value)
    {
        m_PathLineScalarMode = Value;
        return;
    }

    virtual int         GetNumberOfCellLayers() { return 0; }
    virtual int         GetNumberOfVTKCellsInLayer(int k) { return 0; }

    virtual int *       GetConnectivityArray() { return 0; }
    virtual int *       GetStairsteppedGridConnectivity() { return 0; }
    virtual int *       GetLayerConnectivity() { return 0; }

protected:
    int     m_NumberOfTimePoints;
    char  **m_TimePointLabels;
    int     m_NumberOfScalarDataTypes;
    char  **m_DataTypeLabels;
    char   *m_DataFileList;
    int     m_ScalarGridDim[3];
    double *m_InterpolatedGridCoordinates;
    double *m_ScalarArray;
    int     m_ScalarArrayOffset;
    double  m_InactiveCellValue;
    int     m_VectorGridDim[3];
    double *m_VectorGridCoordinates;
    double *m_VectorArray;
    int     m_NumberOfPathlines;
    int     m_NumberOfPathlineCoordinates;
    double *m_PathlineCoordinates;
    //	vtkIdType *m_PathlinePointArray;
    double *m_PathlineScalarArray;
    double *m_MaxTimePathlineScalarArray;
    double *m_MinTimePathlineScalarArray;
    int     m_UnitOffset;
    bool    m_PathsBackwardsInTime;
    int     m_PathLineScalarMode;
    double  m_MinPositiveTime;

    int     m_NumberOfModelFeatureTypes;
    int    *m_ModelFeatureArray;
    char   *m_ModelFeatureLabels;

    double *m_StairsteppedGridCoordinates;

    // m_Particle_Coord stores the locations of particles used to calculate solute
    // transport in MOC3D and MF2K-GWT.  m_Particle_Concentrations stores the
    // concentrations associated with those particles. m_Particle_Coord has 3 times
    // as many members as m_Particle_Concentrations because each particle has 3
    // coordinates but only one concentration.
    double *m_Particle_Coord;
    double *m_Particle_Concentrations;
    int     m_NumberOfParticles;

    bool    ParseDataFileList(char *&pList, char *fileName);

public:
    double GetMinPositiveTime() { return m_MinPositiveTime; }
};

#endif
