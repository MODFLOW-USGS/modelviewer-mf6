#include "Modflow6DataSource.h"
#include "mvUtil.h"
#include "mvLinkList.h"
#include <stdio.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#if defined(QT_GUI_LIB)
#include <QDebug>
#include <QDir>
#include <QString>
#endif

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
Terminology:

Referring to the Modflow grid (layered):
cell3d:  a three dimensional cell in the modflow grid
layer3d: a layer of cell3d's
cell2d:  a two dimensional cell, that is, the top surface or bottom surface
         of a cell3d.
sheet:   a layer of cell2d's, that is, the top surface or bottom surface
         of a layer3d.
vertex:  a point at a corner of a cell2d
node:    a point a the center of a cell2d

Variables:
m_Nvert = number of vertices in a sheet
m_Ncpl = number of cell2d in a sheet
m_NumberOfCellLayers = number of layer3d's


Referring to the VTK grid:
vtkcell: a cell in the VTK grid
vtkpoint: a point defined by its x-y-z coordinates

Variables:
m_NumberOfVTKPoints = number of vtkpoints
m_NumberOfVTKCells = number of vtkcells. For DIS and DISV grids, includes inactive and pass-through cells.

*/

Modflow6DataSource::Modflow6DataSource()
    : mvDataSource()
{
    m_NumberOfVTKPoints            = 0;
    m_NumberOfVTKCells             = 0;
    m_GridType                     = GridType::MV_GRID_NOT_DEFINED;
    m_NumberOfScalarDataTypes      = 0;
    m_NumberOfCellColumns          = 0;
    m_NumberOfCellRows             = 0;
    m_NumberOfCellLayers           = 0;
    m_NumberOfModflowCells         = 0;
    m_Nvert                        = 0; // number of Modflow vertices in a sheet
    m_Ncpl                         = 0; // number of cell2d's in a sheet
    m_InactiveCellValue            = 1.0e30;
    m_DryCellValue                 = -1.0e30;
    m_NumberOfModelFeatureTypes    = 0;
    m_HasSpecificDischargeData     = 0;
    m_ModelFeatureArraySize        = 0;

    // set array pointers initially to zero
    m_Idomain                      = 0;
    m_Icelltype                    = 0;
    m_Iavert                       = 0;
    m_Javert                       = 0;
    m_Ia                           = 0;
    m_Ja                           = 0;
    m_NumCell2dVert                = 0; // m_NumCell2dVert[i] = number of vertices in the i-th cell2d.
    m_NumFlowConn                  = 0; // m_NumFlowConn[i] = number of flow connections of the i-th cell3d
    m_Delr                         = 0;
    m_Delc_revdir                  = 0;
    m_Elev                         = 0;
    m_xorigin                      = 0.0;
    m_yorigin                      = 0.0;
    m_angrot                       = 0.0;
    m_BudgetText                   = 0;
    m_PeriodStep                   = 0;
    m_Rgba                         = 0;
    m_MfCell2VtkCells              = 0;
    m_ConnectivityArray            = 0;
    m_LayerConnectivity            = 0;
    m_StairsteppedGridConnectivity = 0;
    m_NumberOfVTKCellsInLayer      = 0;
}

Modflow6DataSource::~Modflow6DataSource()
{
    if (m_IfHead.is_open()) m_IfHead.close();
    if (m_Idomain) delete[] m_Idomain;
    if (m_Icelltype) delete[] m_Icelltype;
    if (m_Iavert) delete[] m_Iavert;
    if (m_Javert) delete[] m_Javert;
    if (m_Ia) delete[] m_Ia;
    if (m_Ja) delete[] m_Ja;
    if (m_NumCell2dVert) delete[] m_NumCell2dVert;
    if (m_NumFlowConn) delete[] m_NumFlowConn;
    if (m_Delr) delete[] m_Delr;
    if (m_Delc_revdir) delete[] m_Delc_revdir;
    if (m_Elev) delete[] m_Elev;
    if (m_BudgetText) delete m_BudgetText;
    if (m_PeriodStep) delete m_PeriodStep;
    if (m_ConnectivityArray) delete[] m_ConnectivityArray;
    if (m_LayerConnectivity) delete[] m_LayerConnectivity;
    if (m_StairsteppedGridConnectivity) delete[] m_StairsteppedGridConnectivity;
    if (m_NumberOfVTKCellsInLayer) delete[] m_NumberOfVTKCellsInLayer;
    if (m_Rgba)
    {
        for (int i = 0; i < m_NumberOfModelFeatureTypes; i++)
        {
            delete[] m_Rgba[i];
        }
        delete[] m_Rgba;
    }
    if (m_MfCell2VtkCells)
    {
        for (int i = 0; i < m_NumberOfModflowCells; i++)
        {
            delete[] m_MfCell2VtkCells[i];
        }
        delete[] m_MfCell2VtkCells;
    }
}

void Modflow6DataSource::GetDefaultModelFeatureColor(int i, double *rgba)
{
    if (m_Rgba && i < m_NumberOfModelFeatureTypes)
    {
        rgba[0] = m_Rgba[i][0];
        rgba[1] = m_Rgba[i][1];
        rgba[2] = m_Rgba[i][2];
        rgba[3] = m_Rgba[i][3];
    }
    else
    {
        rgba[0] = 0;
        rgba[1] = 0;
        rgba[2] = 0;
        rgba[3] = 1;
    }
}

const char *Modflow6DataSource::LoadData(char *dataFileList)
{
    char nameFile[256];
    char gridFile[256];
    char headFile[256];
    char budgetFile[256];
    gridFile[0]        = '\0';
    headFile[0]        = '\0';
    budgetFile[0]      = '\0';
    const char *errMsg = nullptr;
    int         i;

    // Save the dataFileList. This is needed for serialization.
    // Also, mvDoc checks this variable to determine if data has been loaded.
    m_DataFileList = new char[strlen(dataFileList) + 1];
    strcpy(m_DataFileList, dataFileList);

    // Parse the data file list;
    char *pList = dataFileList;
    ParseDataFileList(pList, nameFile);

    m_GridType = GridType::MV_GRID_NOT_DEFINED;
    if (strlen(nameFile) > 0)
    {
        // if nameFile is specified, then get the gridFile, headFile, and budgetFile from
        // the nameFile
        errMsg = ExtractModflowOutputFileNames(nameFile, gridFile, headFile, budgetFile);
        if (errMsg)
        {
            delete[] m_DataFileList;
            m_DataFileList = 0;
            return errMsg;
        }
        if (m_GridType == GridType::MV_GRID_NOT_DEFINED)
        {
            delete[] m_DataFileList;
            m_DataFileList = 0;
            return "Error: Unable to determine type of Modflow grid.";
        }
    }
    else
    {
        // If the nameFile is not sepcified, parse the DataFileList to get the gridFile,
        // headFile, and budgetFile
        ParseDataFileList(pList, gridFile);
        ParseDataFileList(pList, headFile);
        ParseDataFileList(pList, budgetFile);
        // read the first line of the grid file to determine the type of grid
        ifstream in(gridFile, ios::in | ios::binary);
        char     aRecord[50];
        in.read(aRecord, sizeof(char) * 50);
        in.close();
        aRecord[49] = '\0';
        if (strstr(aRecord, "DISV"))
        {
            m_GridType = GridType::MV_LAYERED_GRID;
        }
        else if (strstr(aRecord, "DISU"))
        {
            m_GridType = GridType::MV_UNSTRUCTURED_GRID;
        }
        else if (strstr(aRecord, "DIS"))
        {
            m_GridType = GridType::MV_STRUCTURED_GRID;
        }
        else
        {
            return "Error encountered while reading the binary grid file to determine the grid type.";
        }
    }

    // open head file
    m_IfHead.open(headFile, ios::in | ios::binary);
    if (!m_IfHead.is_open())
    {
        return "Error: Unable to open the head file.";
    }

    // open budget file, if it is specified.
    // if budget file is not specified, or error is encountered when opening
    // it, then m_IfBudget will remain closed.
    if (strlen(budgetFile))
    {
        m_IfBudget.open(budgetFile, ios::in | ios::binary);
    }

    // construct the grid
    errMsg = nullptr;
    switch (m_GridType)
    {
    case GridType::MV_STRUCTURED_GRID:
        errMsg = CreateDisGrid(gridFile);
        break;
    case GridType::MV_LAYERED_GRID:
        errMsg = CreateDisvGrid(gridFile);
        break;
    case GridType::MV_UNSTRUCTURED_GRID:
        errMsg = CreateDisuGrid(gridFile);
        break;
    case GridType::MV_GRID_NOT_DEFINED:
        errMsg = "Grid has not been defined";
        break;
    }
    if (errMsg) return errMsg;

    const int dataTypeLabelLength  = 17;
    const int maxNumberOfDataTypes = 4; // temporary
    char    **dataTypeLabel        = new char *[maxNumberOfDataTypes];
    for (i = 0; i < maxNumberOfDataTypes; i++)
    {
        dataTypeLabel[i]    = new char[dataTypeLabelLength];
        dataTypeLabel[i][0] = '\0';
    }

    // Count the head file
    errMsg = CountHead(dataTypeLabel[0]);
    if (m_NumberOfTimePoints == 0)
    {
        for (i = 0; i < maxNumberOfDataTypes; i++)
        {
            delete[] dataTypeLabel[i];
        }
        delete[] dataTypeLabel;
        return "Error: No head data.";
    }
    mvUtil::ToLowerCase(dataTypeLabel[0]);
    m_NumberOfScalarDataTypes = 1;

    // TO DO: count the remaining scalar files, if used

    // Store the data type labels
    m_DataTypeLabels          = new char *[m_NumberOfScalarDataTypes];
    for (i = 0; i < m_NumberOfScalarDataTypes; i++)
    {
        m_DataTypeLabels[i] = new char[20];
        strcpy(m_DataTypeLabels[i], dataTypeLabel[i]);
    }
    for (i = 0; i < maxNumberOfDataTypes; i++)
    {
        delete[] dataTypeLabel[i];
    }
    delete[] dataTypeLabel;

    // Get the time points and store them as strings.
    double *timePoints = new double[m_NumberOfTimePoints];
    int    *periods    = new int[m_NumberOfTimePoints];
    int    *steps      = new int[m_NumberOfTimePoints];
    GetTimePoints(timePoints, periods, steps);
    m_TimePointLabels = new char *[m_NumberOfTimePoints];
    char p[10];
    for (i = 0; i < m_NumberOfTimePoints; i++)
    {
        m_TimePointLabels[i] = new char[35];
        sprintf(m_TimePointLabels[i], "%.4g", timePoints[i]);
        mvUtil::TrimRight(m_TimePointLabels[i]);
        /*
          if (GetTimeUnit() != "")
          {
              strcat(m_TimePointLabels[i], " ");
              strcat(m_TimePointLabels[i], GetTimeUnit());
              strcat(m_TimePointLabels[i], " ");
          }
        */
        if (periods[i] > 0 || steps[i] > 0)
        {
            strcat(m_TimePointLabels[i], " (");
            sprintf(p, "%-4u", periods[i]);
            mvUtil::TrimRight(p);
            strcat(m_TimePointLabels[i], p);
            sprintf(p, "-%-4u", steps[i]);
            mvUtil::TrimRight(p);
            strcat(m_TimePointLabels[i], p);
            strcat(m_TimePointLabels[i], ")");
        }
    }

    delete[] timePoints;
    delete[] periods;
    delete[] steps;

    int scalarArraySize        = m_NumberOfScalarDataTypes * (m_NumberOfVTKPoints + m_NumberOfVTKCells);
    m_ScalarArray              = new double[scalarArraySize];

    // Count budget file and model features
    m_HasSpecificDischargeData = 0;
    m_ModelFeatureArraySize    = 0;
    errMsg                     = nullptr;
    if (m_IfBudget.is_open())
    {
        errMsg = CountBudgetAndFeatures();
        if (m_ModelFeatureArraySize)
        {
            m_ModelFeatureArray = new int[m_ModelFeatureArraySize];
            for (i = 0; i < m_ModelFeatureArraySize; i++)
            {
                m_ModelFeatureArray[i] = 0;
            }
        }
        if (m_HasSpecificDischargeData)
        {
            m_VectorArray = new double[3 * m_NumberOfModflowCells];
        }
    }
    return nullptr;
}

int Modflow6DataSource::GetNumVTKPoints()
{
    return m_NumberOfVTKPoints;
}

int Modflow6DataSource::GetNumVTKCells()
{
    return m_NumberOfVTKCells;
}

int Modflow6DataSource::GetNumModelCells()
{
    return m_NumberOfModflowCells;
}


double Modflow6DataSource::GetXOrigin()const
{
    return m_xorigin;
}

double Modflow6DataSource::GetYOrigin()const
{
    return m_yorigin;
}

double Modflow6DataSource::GetAngRot()const
{
    return m_angrot;
}


const char *Modflow6DataSource::ExtractModflowOutputFileNames(char *nameFile,
                                                              char *gridFile, char *headFile, char *budgetFile)
{
    char  aline[300];
    char  ocFile[300];
    char *p;
    headFile[0]   = '\0';
    budgetFile[0] = '\0';
    gridFile[0]   = '\0';
    m_GridType    = GridType::MV_GRID_NOT_DEFINED;
    ocFile[0]     = '\0';
    ifstream in(nameFile, ios::in);
    if (!in.is_open())
    {
        return "Error: Unable to open the name file.";
    }

    // determine the grid type (DIS, DISV, or DISU), name of the grid file,
    // and name of the OC file.
    while (!in.eof())
    {
        in.getline(aline, 300);
        mvUtil::ReplaceTabWithWhiteSpace(aline);
        mvUtil::TrimLeft(aline);
        mvUtil::TrimRight(aline);
        if (strlen(aline) > 0 && aline[0] != '#')
        {
            if (!mvUtil::strnicmp(aline, "dis6 ", 5))
            {
#if defined(QT_GUI_LIB)
                QString gf(aline + 5);
                gf = gf.trimmed();
                gf = mvUtil::toNativeSeparators(gf);
                strcpy(gridFile, gf.toLocal8Bit().data());
#else
                strcpy(gridFile, aline + 5);
                mvUtil::TrimLeft(gridFile);
#endif
                ExtractFileName(gridFile);
                strcat(gridFile, ".grb");
                m_GridType = GridType::MV_STRUCTURED_GRID;
            }
            else if (!mvUtil::strnicmp(aline, "disv6 ", 6))
            {
#if defined(QT_GUI_LIB)
                QString gf(aline + 6);
                gf = gf.trimmed();
                gf = mvUtil::toNativeSeparators(gf);
                strcpy(gridFile, gf.toLocal8Bit().data());
#else
                strcpy(gridFile, aline + 6);
                mvUtil::TrimLeft(gridFile);
#endif
                ExtractFileName(gridFile);
                strcat(gridFile, ".grb");
                m_GridType = GridType::MV_LAYERED_GRID;
            }
            else if (!mvUtil::strnicmp(aline, "disu6 ", 6))
            {
#if defined(QT_GUI_LIB)
                QString gf(aline + 6);
                gf = gf.trimmed();
                gf = mvUtil::toNativeSeparators(gf);
                strcpy(gridFile, gf.toLocal8Bit().data());
#else
                strcpy(gridFile, aline + 5);
                mvUtil::TrimLeft(gridFile);
#endif
                ExtractFileName(gridFile);
                strcat(gridFile, ".grb");
                m_GridType = GridType::MV_UNSTRUCTURED_GRID;
            }
            if (!mvUtil::strnicmp(aline, "oc6 ", 4))
            {
#if defined(QT_GUI_LIB)
                QString oc(aline + 4);
                oc = oc.trimmed();
                oc = mvUtil::toNativeSeparators(oc);
                strcpy(ocFile, oc.toLocal8Bit().data());
#else
                strcpy(ocFile, aline + 4);
                mvUtil::TrimLeft(ocFile);
#endif
                ExtractFileName(ocFile);
            }
        }
    }
    in.close();

    // open the oc file
    if (strlen(ocFile) == 0)
    {
        return "Error: Unable to determine the oc file.";
    }
#if !defined(NDEBUG)
    qDebug() << "attempting to open " << ocFile;
#endif
    in.open(ocFile, ios::in);
    if (!in.is_open())
    {
        return "Error: Unable to open the oc file.";
    }

    // determine the name of the head file and budget file
    while (!in.eof())
    {
        in.getline(aline, 300);
        mvUtil::ReplaceTabWithWhiteSpace(aline);
        mvUtil::TrimLeft(aline);
        mvUtil::TrimRight(aline);
        if (strlen(aline) > 0 && aline[0] != '#')
        {
            if (!mvUtil::strnicmp(aline, "head ", 5))
            {
                p = mvUtil::NextNonSpaceChar(aline + 5, (int)strlen(aline) - 5);
                if (!mvUtil::strnicmp(p, "fileout ", 8))
                {
                    strcpy(headFile, p + 8);
                    mvUtil::TrimLeft(headFile);
                    mvUtil::RemoveQuotes(headFile);
                    mvUtil::RemoveDoubleQuotes(headFile);
                }
            }
            if (!mvUtil::strnicmp(aline, "budget ", 7))
            {
                p = mvUtil::NextNonSpaceChar(aline + 7, (int)strlen(aline) - 7);
                if (!mvUtil::strnicmp(p, "fileout ", 8))
                {
                    strcpy(budgetFile, p + 8);
                    mvUtil::TrimLeft(budgetFile);
                    mvUtil::RemoveQuotes(budgetFile);
                    mvUtil::RemoveDoubleQuotes(budgetFile);
                }
            }
            if (!mvUtil::strnicmp(aline, "end ", 4))
            {
                p = mvUtil::NextNonSpaceChar(aline + 4, (int)strlen(aline) - 4);
                if (!mvUtil::strnicmp(p, "options ", 8))
                {
                    break;
                }
            }
        }
    }
    in.close();
    return nullptr;
}

const char *Modflow6DataSource::CreateDisGrid(char *gridFile)
{
    int      i, j, k;
    int      nx, ny, nz, nxy;

    ifstream in(gridFile, ios::in | ios::binary);
    if (!in.is_open())
    {
        return "Error: Unable to open the binary grid file.";
    }

    char   *aRecord = new char[50];
    char   *q;
    int     ntxt, lentxt, nja;
    double  angrot;
    double *delr, *delc, *elev;
    double  xorigin, yorigin;

    try
    {
        if (!in.read(aRecord, sizeof(char) * 50)) throw 1;
        // Read the version (not used)
        if (!in.read(aRecord, sizeof(char) * 50)) throw 1;
        // extract the value of ntxt
        if (!in.read(aRecord, sizeof(char) * 50)) throw 1;
        q    = strchr(aRecord + 5, ' ');
        *q   = '\0';
        ntxt = atoi(aRecord + 5);
        // extract the value of lentxt
        if (!in.read(aRecord, sizeof(char) * 50)) throw 1;
        q      = strchr(aRecord + 7, ' ');
        *q     = '\0';
        lentxt = atoi(aRecord + 7);
        // re-allocate aRecord to size lentxt
        delete[] aRecord;
        aRecord  = new char[lentxt];
        // skip the next ntxt records plus comments
        int itxt = 0;
        while (itxt < ntxt)
        {
            if (!in.read(aRecord, sizeof(char) * lentxt)) throw 1;
            if (aRecord[0] != '#') itxt++;
        }
        if (!in.read((char *)(&m_NumberOfModflowCells), sizeof(int))) throw 1;
        if (!in.read((char *)(&m_NumberOfCellLayers), sizeof(int))) throw 1;
        if (!in.read((char *)(&m_NumberOfCellRows), sizeof(int))) throw 1;
        if (!in.read((char *)(&m_NumberOfCellColumns), sizeof(int))) throw 1;
        if (!in.read((char *)(&nja), sizeof(int))) throw 1;
        if (!in.read((char *)(&xorigin), sizeof(double))) throw 1;
        if (!in.read((char *)(&yorigin), sizeof(double))) throw 1;
        if (!in.read((char *)(&angrot), sizeof(double))) throw 1;
        delete[] aRecord;
    }
    catch (...)
    {
        delete[] aRecord;
        return "Error encountered while reading the binary grid file.";
    }

    m_Ncpl             = m_NumberOfCellRows * m_NumberOfCellColumns;
    m_NumberOfVTKCells = m_NumberOfModflowCells;
    m_xorigin          = xorigin;
    m_yorigin          = yorigin;
    m_angrot           = angrot;

    // allocate arrays
    delr               = new double[m_NumberOfCellColumns];
    delc               = new double[m_NumberOfCellRows];
    elev               = new double[m_Ncpl + m_NumberOfModflowCells];
    m_Ia               = new int[m_NumberOfModflowCells + 1];
    m_Ja               = new int[nja];
    m_Idomain          = new int[m_NumberOfModflowCells];
    m_Icelltype        = new int[m_NumberOfModflowCells];
    m_NumFlowConn      = new int[m_NumberOfModflowCells];

    // continue to read
    try
    {
        if (!in.read((char *)delr, m_NumberOfCellColumns * sizeof(double))) throw 1;
        if (!in.read((char *)delc, m_NumberOfCellRows * sizeof(double))) throw 1;
        // elevation of top of grid is read into the first m_Ncpl elements of elev.
        // This is followed by bottom elevations of each layer.
        if (!in.read((char *)elev, (m_Ncpl + m_NumberOfModflowCells) * sizeof(double))) throw 1;
        if (!in.read((char *)m_Ia, (m_NumberOfModflowCells + 1) * sizeof(int))) throw 1;
        if (!in.read((char *)m_Ja, nja * sizeof(int))) throw 1;
        if (!in.read((char *)m_Idomain, m_NumberOfModflowCells * sizeof(int))) throw 1;
        if (!in.read((char *)m_Icelltype, m_NumberOfModflowCells * sizeof(int))) throw 1;
        in.close();
    }
    catch (...)
    {
        in.close();
        delete[] delr;
        delete[] delc;
        delete[] elev;
        delete[] m_Ia;
        delete[] m_Ja;
        delete[] m_Idomain;
        delete[] m_Icelltype;
        delete[] m_NumFlowConn;
        m_Ia          = 0;
        m_Ja          = 0;
        m_Idomain     = 0;
        m_Icelltype   = 0;
        m_NumFlowConn = 0;
        return "Error encountered while reading the binary grid file.";
    }

    // change flow indices to start counting from zero
    for (i = 0; i < m_NumberOfModflowCells + 1; i++)
    {
        m_Ia[i]--;
    }
    for (j = 0; j < nja; j++)
    {
        m_Ja[j]--;
    }

    // compute the number of flow connections for each cell
    for (i = 0; i < m_NumberOfModflowCells; i++)
    {
        m_NumFlowConn[i] = m_Ia[i + 1] - m_Ia[i] - 1; // does not count the cell connected to itself
        if (m_NumFlowConn[i] < 0)
        {
            m_NumFlowConn[i] = 0;
        }
    }

    // Work on coordinates
    nx                  = m_NumberOfCellColumns + 1;
    ny                  = m_NumberOfCellRows + 1;
    nz                  = m_NumberOfCellLayers + 1;
    nxy                 = nx * ny;
    m_NumberOfVTKPoints = nx * ny * nz;
    //	m_TimeUnit = timeunit;
    //   m_TimeUnit = 0;
    m_UnitOffset        = m_NumberOfVTKPoints + m_NumberOfModflowCells;

    m_Delr              = new double[m_NumberOfCellColumns];
    m_Delc_revdir       = new double[m_NumberOfCellRows];
    m_Elev              = new double[m_NumberOfCellColumns * m_NumberOfCellRows * (m_NumberOfCellLayers + 1)];

    for (i = 0; i < m_NumberOfCellColumns; i++)
    {
        m_Delr[i] = delr[i];
    }
    for (i = 0; i < m_NumberOfCellRows; i++)
    {
        m_Delc_revdir[i] = delc[m_NumberOfCellRows - i - 1];
    }

    // m_Elev is stored from the bottom layer upward, and rows counted from south to north
    // botm is stored from top layer downward, and rows are counted from north to south
    for (k = 0; k < m_NumberOfCellLayers; k++)
    {
        for (j = 0; j < m_NumberOfCellRows; j++)
        {
            for (i = 0; i < m_NumberOfCellColumns; i++)
            {
                int idx = (m_NumberOfCellLayers - k - 1) * m_Ncpl + (m_NumberOfCellRows - j - 1) * m_NumberOfCellColumns + i;
                assert(0 <= idx && idx < m_NumberOfCellColumns * m_NumberOfCellRows * (m_NumberOfCellLayers + 1));
                m_Elev[(m_NumberOfCellLayers - k - 1) * m_Ncpl + (m_NumberOfCellRows - j - 1) * m_NumberOfCellColumns + i] = elev[(k + 1) * m_Ncpl + j * m_NumberOfCellColumns + i];
            }
        }
    }
    // The last ncells elements of m_Elev contains the elevations of the top of the grid
    for (j = 0; j < m_NumberOfCellRows; j++)
    {
        for (i = 0; i < m_NumberOfCellColumns; i++)
        {
            int idx = m_NumberOfModflowCells + (m_NumberOfCellRows - j - 1) * m_NumberOfCellColumns + i;
            assert(0 <= idx && idx < m_NumberOfCellColumns * m_NumberOfCellRows * (m_NumberOfCellLayers + 1));
            m_Elev[m_NumberOfModflowCells + (m_NumberOfCellRows - j - 1) * m_NumberOfCellColumns + i] = elev[j * m_NumberOfCellColumns + i];
        }
    }

    // Compute point coordinates in x and y directions
    double  xx, yy;
    double *x = new double[nx];
    double *y = new double[ny];
    double *z = new double[m_NumberOfVTKPoints];
    //	xx        = xorigin;
    xx        = 0;
    x[0]      = xx;
    for (i = 0; i < m_NumberOfCellColumns; i++)
    {
        xx += m_Delr[i];
        x[i + 1] = xx;
    }
    //	yy = yorigin;
    // *********** for debugging only ***************
    yy   = 0;
    y[0] = yy;
    for (i = 0; i < m_NumberOfCellRows; i++)
    {
        yy += m_Delc_revdir[i];
        y[i + 1] = yy;
    }

    // coordinates for stairstep grid
    // These have to correspond to the orientation of structured grid--layers counted from bottom upward
    m_StairsteppedGridCoordinates = new double[24 * m_NumberOfModflowCells];
    int m                         = 0;
    for (k = 0; k < m_NumberOfCellLayers; k++)
    {
        for (j = 0; j < m_NumberOfCellRows; j++)
        {
            for (i = 0; i < m_NumberOfCellColumns; i++)
            {
                int ijk                            = k * m_Ncpl + j * m_NumberOfCellColumns + i;

                m_StairsteppedGridCoordinates[m++] = x[i];
                m_StairsteppedGridCoordinates[m++] = y[j];
                m_StairsteppedGridCoordinates[m++] = m_Elev[ijk];

                m_StairsteppedGridCoordinates[m++] = x[i + 1];
                m_StairsteppedGridCoordinates[m++] = y[j];
                m_StairsteppedGridCoordinates[m++] = m_Elev[ijk];

                m_StairsteppedGridCoordinates[m++] = x[i + 1];
                m_StairsteppedGridCoordinates[m++] = y[j + 1];
                m_StairsteppedGridCoordinates[m++] = m_Elev[ijk];

                m_StairsteppedGridCoordinates[m++] = x[i];
                m_StairsteppedGridCoordinates[m++] = y[j + 1];
                m_StairsteppedGridCoordinates[m++] = m_Elev[ijk];

                ijk += m_Ncpl;

                m_StairsteppedGridCoordinates[m++] = x[i];
                m_StairsteppedGridCoordinates[m++] = y[j];
                m_StairsteppedGridCoordinates[m++] = m_Elev[ijk];

                m_StairsteppedGridCoordinates[m++] = x[i + 1];
                m_StairsteppedGridCoordinates[m++] = y[j];
                m_StairsteppedGridCoordinates[m++] = m_Elev[ijk];

                m_StairsteppedGridCoordinates[m++] = x[i + 1];
                m_StairsteppedGridCoordinates[m++] = y[j + 1];
                m_StairsteppedGridCoordinates[m++] = m_Elev[ijk];

                m_StairsteppedGridCoordinates[m++] = x[i];
                m_StairsteppedGridCoordinates[m++] = y[j + 1];
                m_StairsteppedGridCoordinates[m++] = m_Elev[ijk];
            }
        }
    }

    // Compute coordinates at cell center for drawing vectors.
    // TO DO: m_m_HasSpecificDischargeData has not yet been determined.
    //	if (m_m_HasSpecificDischargeData)
    //	{
    m_VectorGridDim[0]      = nx - 1;
    m_VectorGridDim[1]      = ny - 1;
    m_VectorGridDim[2]      = nz - 1;
    m_VectorGridCoordinates = new double[3 * m_NumberOfModflowCells];

    m                       = 0;
    for (k = 0; k < m_NumberOfCellLayers; k++)
    {
        for (j = 0; j < m_NumberOfCellRows; j++)
        {
            for (i = 0; i < m_NumberOfCellColumns; i++)
            {
                m_VectorGridCoordinates[3 * m]     = (x[i] + x[i + 1]) / 2;
                m_VectorGridCoordinates[3 * m + 1] = (y[j] + y[j + 1]) / 2;
                ;
                m_VectorGridCoordinates[3 * m + 2] = (m_Elev[m] + m_Elev[m + m_Ncpl]) / 2;
                m++;
            }
        }
    }
    //	}

    // Modify cell top/bottom elevation for inactive cells. The modified array is used for interpolation.
    double znull = -1.0e20;
    // For cells in the top most cell layer, set the top elevation to the null value
    // if the cell is inactive.
    for (j = 0; j < m_NumberOfCellRows; j++)
    {
        for (i = 0; i < m_NumberOfCellColumns; i++)
        {
            if (m_Idomain[j * m_NumberOfCellColumns + i] == 0)
            {
                m_Elev[m_NumberOfModflowCells + (m_NumberOfCellRows - j - 1) * m_NumberOfCellColumns + i] = znull;
            }
        }
    }
    // Work from the second from top cell layer downward to the bottom cell layer
    for (k = 1; k < m_NumberOfCellLayers; k++)
    {
        for (j = 0; j < m_NumberOfCellRows; j++)
        {
            for (i = 0; i < m_NumberOfCellColumns; i++)
            {
                int modflow_index = k * m_Ncpl + j * m_NumberOfCellColumns + i;
                // set top elevation to the null value if the cell in this layer and the cell in the layer above are both inactive.
                if (m_Idomain[modflow_index] == 0 && m_Idomain[modflow_index - m_Ncpl] == 0)
                {
                    m_Elev[(m_NumberOfCellLayers - k) * m_Ncpl + (m_NumberOfCellRows - j - 1) * m_NumberOfCellColumns + i] = znull;
                }
            }
        }
    }
    // For the bottom layer of cells, set the cell bottom elevation
    // to the null value if the cell is inactive.
    for (j = 0; j < m_NumberOfCellRows; j++)
    {
        for (i = 0; i < m_NumberOfCellColumns; i++)
        {
            if (m_Idomain[(m_NumberOfCellLayers - 1) * m_Ncpl + j * m_NumberOfCellColumns + i] == 0)
            {
                m_Elev[(m_NumberOfCellRows - j - 1) * m_NumberOfCellColumns + i] = znull;
            }
        }
    }

    m_InterpolatedGridCoordinates = new double[3 * m_NumberOfVTKPoints];
    for (k = 0; k < nz; k++)
    {
        mvUtil::interp2d(x, y, z + k * nxy, m_Delr, m_Delc_revdir, m_Elev + k * m_Ncpl, m_NumberOfCellRows, m_NumberOfCellColumns, znull);
        int m = k * nxy;
        for (j = 0; j < ny; j++)
        {
            for (i = 0; i < nx; i++)
            {
                m_InterpolatedGridCoordinates[3 * m]     = x[i];
                m_InterpolatedGridCoordinates[3 * m + 1] = y[j];
                m_InterpolatedGridCoordinates[3 * m + 2] = z[m];
                m++;
            }
        }
    }
    // Check elevations
    // At layer edge, interpolated elevation in a deeper layer could be higher
    // than interpolated elevation in a shallower layer if the "footprint" of
    // the deeper layer is larger than the shallower layer, and if the deeper
    // layer has a bowl shape.
    int     m1, m2;
    double *c = m_InterpolatedGridCoordinates;
    double  ave;
    for (j = 0; j < ny; j++)
    {
        for (i = 0; i < nx; i++)
        {
            for (k = 0; k < nz - 1; k++)
            {
                m1 = 3 * ((k * nxy) + j * nx + i) + 2;
                m2 = 3 * (((k + 1) * nxy) + j * nx + i) + 2;
                if (c[m1] != znull && c[m2] != znull && c[m1] > c[m2])
                {
                    ave   = (c[m1] + c[m2]) / 2;
                    c[m1] = ave;
                    c[m2] = ave;
                    // propagate the change downward
                    for (int kk = k - 1; kk >= 0; kk--)
                    {
                        m1 = 3 * ((kk * nxy) + j * nx + i) + 2;
                        if (c[m1] != znull && c[m1] > ave)
                        {
                            c[m1] = ave;
                        }
                    }
                }
            }
        }
    }

    // For each layer, replace the znull values by the average of non-null values.
    // These points will not be displayed, but they presence of znull elevation
    // points will affect the size of the bounding box.
    for (k = 0; k < nz; k++)
    {
        // compute the average of non null elevation
        double zave  = 0;
        int    count = 0;
        int    index;
        for (j = 0; j < nxy; j++)
        {
            index = 3 * (k * nxy + j) + 2;
            if (c[index] != znull)
            {
                zave += c[index];
                count++;
            }
        }
        if (count > 0)
        {
            zave /= count;
        }
        for (j = 0; j < nxy; j++)
        {
            index = 3 * (k * nxy + j) + 2;
            if (c[index] == znull)
            {
                c[index] = zave;
            }
        }
    }
    m_ScalarGridDim[0] = nx;
    m_ScalarGridDim[1] = ny;
    m_ScalarGridDim[2] = nz;

    delete[] x;
    delete[] y;
    delete[] z;
    delete[] elev;
    delete[] delr;
    delete[] delc;
    return nullptr;
}

const char *Modflow6DataSource::CreateDisvGrid(char *gridFile)
{
    int      i, j, k, m, index;

    ifstream in(gridFile, ios::in | ios::binary);
    if (!in.is_open())
    {
        return "Error: Unable to open the binary grid file.";
    }

    char   *aRecord = new char[50];
    char   *q;
    int     ntxt, lentxt, njavert, nja;
    double  xorigin, yorigin, angrot;
    double *vert, *cellx, *celly;

    try
    {
        if (!in.read(aRecord, sizeof(char) * 50)) throw 1;
        // Read the version (not used)
        if (!in.read(aRecord, sizeof(char) * 50)) throw 1;
        // determine ntxt
        if (!in.read(aRecord, sizeof(char) * 50)) throw 1;
        q    = strchr(aRecord + 5, ' ');
        *q   = '\0';
        ntxt = atoi(aRecord + 5);
        // determine lentxt
        if (!in.read(aRecord, sizeof(char) * 50)) throw 1;
        q      = strchr(aRecord + 7, ' ');
        *q     = '\0';
        lentxt = atoi(aRecord + 7);
        // re-allocate aRecord to size lentxt
        delete[] aRecord;
        aRecord  = new char[lentxt];
        // skip the next ntxt records plus comments
        int itxt = 0;
        while (itxt < ntxt)
        {
            if (!in.read(aRecord, sizeof(char) * lentxt)) throw 1;
            if (aRecord[0] != '#') itxt++;
        }
        if (!in.read((char *)(&m_NumberOfModflowCells), sizeof(int))) throw 1;
        if (!in.read((char *)(&m_NumberOfCellLayers), sizeof(int))) throw 1;
        if (!in.read((char *)(&m_Ncpl), sizeof(int))) throw 1;
        if (!in.read((char *)(&m_Nvert), sizeof(int))) throw 1;
        if (!in.read((char *)(&njavert), sizeof(int))) throw 1;
        if (!in.read((char *)(&nja), sizeof(int))) throw 1;
        if (!in.read((char *)(&xorigin), sizeof(double))) throw 1;
        if (!in.read((char *)(&yorigin), sizeof(double))) throw 1;
        if (!in.read((char *)(&angrot), sizeof(double))) throw 1;
        delete[] aRecord;
    }
    catch (...)
    {
        delete[] aRecord;
        return "Error encountered while reading the binary grid file.";
    }

    m_xorigin   = xorigin;
    m_yorigin   = yorigin;
    m_angrot    = angrot;

    // allocate arrays to contain read data
    m_Elev      = new double[m_Ncpl + m_NumberOfModflowCells]; // first ncpl elements contains top of grid, remaining entries contain botm of layer
    vert        = new double[2 * m_Nvert];
    cellx       = new double[m_Ncpl];
    celly       = new double[m_Ncpl];
    m_Iavert    = new int[m_Ncpl + 1];
    m_Javert    = new int[njavert];
    m_Ia        = new int[m_NumberOfModflowCells + 1];
    m_Ja        = new int[nja];
    m_Idomain   = new int[m_NumberOfModflowCells];
    m_Icelltype = new int[m_NumberOfModflowCells];

    // continue to read data
    try
    {
        // read top data into first m_Ncpl elements of m_Elev
        if (!in.read((char *)m_Elev, m_Ncpl * sizeof(double))) throw 1;
        // read botm data into remaining elements of m_Elev
        if (!in.read((char *)(m_Elev + m_Ncpl), m_NumberOfModflowCells * sizeof(double))) throw 1;
        // read x and y coordinates of vertices
        if (!in.read((char *)vert, 2 * m_Nvert * sizeof(double))) throw 1;
        if (!in.read((char *)cellx, m_Ncpl * sizeof(double))) throw 1;
        if (!in.read((char *)celly, m_Ncpl * sizeof(double))) throw 1;
        if (!in.read((char *)m_Iavert, (m_Ncpl + 1) * sizeof(int))) throw 1;
        if (!in.read((char *)m_Javert, njavert * sizeof(int))) throw 1;
        if (!in.read((char *)m_Ia, (m_NumberOfModflowCells + 1) * sizeof(int))) throw 1;
        if (!in.read((char *)m_Ja, nja * sizeof(int))) throw 1;
        if (!in.read((char *)m_Idomain, m_NumberOfModflowCells * sizeof(int))) throw 1;
        if (!in.read((char *)m_Icelltype, m_NumberOfModflowCells * sizeof(int))) throw 1;
        in.close();
    }
    catch (...)
    {
        in.close();
        delete[] aRecord;
        delete[] m_Elev;
        delete[] vert;
        delete[] cellx;
        delete[] celly;
        delete[] m_Iavert;
        delete[] m_Javert;
        delete[] m_Ia;
        delete[] m_Ja;
        delete[] m_Idomain;
        delete[] m_Icelltype;
        return "Error encountered while reading the binary grid file.";
    }

    // allocate arrays
    m_NumCell2dVert           = new int[m_Ncpl];
    m_NumFlowConn             = new int[m_NumberOfModflowCells];
    m_NumberOfVTKCellsInLayer = new int[m_NumberOfCellLayers];

    // change indices to start counting from zero
    for (i = 0; i < m_Ncpl + 1; i++)
    {
        m_Iavert[i]--;
    }
    for (i = 0; i < njavert; i++)
    {
        m_Javert[i]--;
    }
    for (i = 0; i < m_NumberOfModflowCells + 1; i++)
    {
        m_Ia[i]--;
    }
    for (i = 0; i < nja; i++)
    {
        m_Ja[i]--;
    }

    // compute the number of vertices for each cell2d, and the maximum number
    int maxVertPerCell2d = 0;
    for (i = 0; i < m_Ncpl; i++)
    {
        m_NumCell2dVert[i] = m_Iavert[i + 1] - m_Iavert[i] - 1; // does not count the cell's last vertex of, which is the same as the first vertes
        if (m_NumCell2dVert[i] > maxVertPerCell2d)
        {
            maxVertPerCell2d = m_NumCell2dVert[i];
        }
    }

    // compute the number of flow connections for each cell
    for (i = 0; i < m_NumberOfModflowCells; i++)
    {
        m_NumFlowConn[i] = m_Ia[i + 1] - m_Ia[i] - 1; // does not count a node connected to itself
        if (m_NumFlowConn[i] < 0)
        {
            m_NumFlowConn[i] = 0;
        }
    }

    // compute weight vectors (used to compute specific discharge at cell center)
    // coordinates for blocky grid
    m_NumberOfVTKPointsForStairsteppedGrid = 0;
    for (i = 0; i < m_Ncpl; i++)
    {
        m_NumberOfVTKPointsForStairsteppedGrid += (2 * m_NumCell2dVert[i]);
    }
    m_NumberOfVTKPointsForStairsteppedGrid *= m_NumberOfCellLayers;
    int cellCenterNodeBegin = m_NumberOfVTKPointsForStairsteppedGrid;
    m_NumberOfVTKPointsForStairsteppedGrid += 2 * m_NumberOfModflowCells;

    m_StairsteppedGridCoordinates = new double[3 * m_NumberOfVTKPointsForStairsteppedGrid];
    m                             = 0;
    // coordinates of vertices (store for each cell in counter clockwise orientation per VTK)
    for (k = 0; k < m_NumberOfCellLayers; k++)
    {
        for (i = 0; i < m_Ncpl; i++)
        {
            for (j = m_NumCell2dVert[i] - 1; j >= 0; j--)
            {
                int p                              = 2 * (m_Javert[m_Iavert[i] + j]);
                m_StairsteppedGridCoordinates[m++] = vert[p];
                m_StairsteppedGridCoordinates[m++] = vert[p + 1];
                m_StairsteppedGridCoordinates[m++] = m_Elev[(k + 1) * m_Ncpl + i]; // bottom face
            }
            for (j = m_NumCell2dVert[i] - 1; j >= 0; j--)
            {
                int p                              = 2 * (m_Javert[m_Iavert[i] + j]);
                m_StairsteppedGridCoordinates[m++] = vert[p];
                m_StairsteppedGridCoordinates[m++] = vert[p + 1];
                m_StairsteppedGridCoordinates[m++] = m_Elev[k * m_Ncpl + i]; // top face
            }
        }
    }
    for (k = 0; k < m_NumberOfCellLayers; k++)
    {
        for (i = 0; i < m_Ncpl; i++)
        {
            m_StairsteppedGridCoordinates[m++] = cellx[i];
            m_StairsteppedGridCoordinates[m++] = celly[i];
            m_StairsteppedGridCoordinates[m++] = m_Elev[(k + 1) * m_Ncpl + i]; // bottom face
            m_StairsteppedGridCoordinates[m++] = cellx[i];
            m_StairsteppedGridCoordinates[m++] = celly[i];
            m_StairsteppedGridCoordinates[m++] = m_Elev[k * m_Ncpl + i]; // top face
        }
    }

    // m_InterpolatedGridCoordinates contains x-y-z coordinates of points that define the grid
    // in VTK. These points are organized in "sheets"; the first sheet is the top of
    // MF layer 1, the second sheet is the bottom of MF layer 1, the third sheet is the bottom
    // of layer MF 2, etc. There are nlay+1 sheets.
    // In each sheet, there are m_Nvert vertices and m_Ncpl nodes.
    // m_InterpolatedGridCoordinates contains the x-y-z coordinates of all the vertices, followed
    // by coordinates of all the nodes.

    m_NumberOfVTKPoints           = (m_NumberOfCellLayers + 1) * (m_Nvert + m_Ncpl);
    m_InterpolatedGridCoordinates = new double[3 * m_NumberOfVTKPoints];

    // load the x-y-z coordinates of the vertices
    double *sum_weighted_z        = new double[m_Nvert];
    double *sum_weights           = new double[m_Nvert];
    double  weight;
    int     use;
    for (k = 0; k < m_NumberOfCellLayers + 1; k++) // loop through each sheet (top to bottom)
    {
        // load the x and y coordinates
        for (j = 0; j < m_Nvert; j++) // loop through each vertex in a sheet
        {
            index                                    = 3 * (k * m_Nvert + j);
            m_InterpolatedGridCoordinates[index]     = vert[2 * j];
            m_InterpolatedGridCoordinates[index + 1] = vert[2 * j + 1];
        }
        // determine the z coordinates of the vertices by averaging the elevations of cell2ds that use these vertices
        memset(sum_weighted_z, 0, m_Nvert * sizeof(double));
        memset(sum_weights, 0, m_Nvert * sizeof(double));
        for (i = 0; i < m_Ncpl; i++)
        {
            // check if the cell2d is associated with a cell3d that is used
            use = 0;
            if (k < m_NumberOfCellLayers) // if this is not the bottom sheet, check the idomain of the underlying cell3d
            {
                if (m_Idomain[k * m_Ncpl + i] > 0)
                {
                    use = 1;
                }
            }
            if (k > 0) // if this is not the top sheet, check the idomain of the overlying cell3d
            {
                if (m_Idomain[(k - 1) * m_Ncpl + i] > 0)
                {
                    use = 1;
                }
            }
            {
                for (j = 0; j < m_NumCell2dVert[i]; j++)
                {
                    index  = m_Javert[m_Iavert[i] + j];
                    weight = 1 / sqrt(pow(cellx[i] - vert[2 * index], 2) + pow(celly[i] - vert[2 * index + 1], 2));
                    sum_weighted_z[index] += m_Elev[k * m_Ncpl + i] * weight;
                    sum_weights[index] += weight;
                }
            }
        }
        for (i = 0; i < m_Nvert; i++)
        {
            if (sum_weights[i] > 0)
            {
                m_InterpolatedGridCoordinates[3 * (k * m_Nvert + i) + 2] = sum_weighted_z[i] / sum_weights[i];
            }
        }
    }
    delete[] sum_weighted_z;
    delete[] sum_weights;

    // load the x-y-z coordinates of node2d's (center of cell2d)

    int istart = 3 * m_Nvert * (m_NumberOfCellLayers + 1);
    for (k = 0; k < m_NumberOfCellLayers + 1; k++)
    {
        for (j = 0; j < m_Ncpl; j++)
        {
            index                                    = istart + 3 * (k * m_Ncpl + j);
            m_InterpolatedGridCoordinates[index]     = cellx[j];
            m_InterpolatedGridCoordinates[index + 1] = celly[j];
            m_InterpolatedGridCoordinates[index + 2] = m_Elev[k * m_Ncpl + j];
        }
    }

    // Determine the number of vtkcells (includes inactive and pass-through cells)
    m_NumberOfVTKCells = 0;
    for (k = 0; k < m_NumberOfCellLayers; k++)
    {
        m_NumberOfVTKCellsInLayer[k] = 0;
        for (i = 0; i < m_Ncpl; i++)
        {
            if (m_Idomain[k * m_Ncpl + i] > 0)
            {
                // if the Modflow cell2d has 6 or less vertices, consider it as
                // a single vtkcell.
                if (m_NumCell2dVert[i] <= 6)
                {
                    m_NumberOfVTKCells++;
                    m_NumberOfVTKCellsInLayer[k]++;
                }
                // if the Modflow cell2d has more than 6 vertices, break it up into triangles, using
                // the node as a common vertex.
                else
                {
                    m_NumberOfVTKCells += m_NumCell2dVert[i];
                    m_NumberOfVTKCellsInLayer[k] += m_NumCell2dVert[i];
                }
            }
            else
            {
                m_NumberOfVTKCells++;
            }
        }
    }

    // Map Modflow cells to VTK cells
    // Also calculate the size of m_ConductivityArray
    m_MfCell2VtkCells         = new int *[m_NumberOfModflowCells];
    m                         = 0;
    int n                     = 0;
    int connectivityArraySize = 0;
    for (k = 0; k < m_NumberOfCellLayers; k++)
    {
        for (i = 0; i < m_Ncpl; i++)
        {
            if (m_Idomain[m] > 0)
            {
                if (m_NumCell2dVert[i] <= 6)
                {
                    m_MfCell2VtkCells[m]    = new int[2];
                    m_MfCell2VtkCells[m][0] = 1;
                    m_MfCell2VtkCells[m][1] = n;
                    n++;
                    connectivityArraySize += (1 + 2 * m_NumCell2dVert[i]);
                }
                else
                {
                    m_MfCell2VtkCells[m]    = new int[m_NumCell2dVert[i] + 1];
                    m_MfCell2VtkCells[m][0] = m_NumCell2dVert[i];
                    for (j = 1; j <= m_NumCell2dVert[i]; j++)
                    {
                        m_MfCell2VtkCells[m][j] = n;
                        n++;
                    }
                    connectivityArraySize += (7 * m_NumCell2dVert[i]);
                }
            }
            else
            {
                // Inactive or pass-through cells don't get mapped
                m_MfCell2VtkCells[m]    = new int[1];
                m_MfCell2VtkCells[m][0] = 0;
                connectivityArraySize++;
            }
            m++;
        }
    }

    // create the coordinates of the cell3d centers. These are used to draw vectors
    m_VectorGridCoordinates = new double[3 * m_NumberOfModflowCells];
    m                       = 0;
    for (k = 0; k < m_NumberOfCellLayers; k++)
    {
        for (i = 0; i < m_Ncpl; i++)
        {
            m_VectorGridCoordinates[m++] = cellx[i];
            m_VectorGridCoordinates[m++] = celly[i];
            m_VectorGridCoordinates[m++] = (m_Elev[k * m_Ncpl + i] + m_Elev[(k + 1) * m_Ncpl + i]) / 2;
        }
    }

    //   m_VectorArray = new double[3*m_NumberOfModflowCells];

    delete[] vert;
    delete[] cellx;
    delete[] celly;

    // build cell connectivity for interpolated grid
    m_ConnectivityArray = new int[connectivityArraySize];
    m                   = 0;
    int p               = 0;
    int mf_cell_index   = 0;
    for (k = 0; k < m_NumberOfCellLayers; k++)
    {
        int mf_vertex_offset = k * m_Nvert;
        for (i = 0; i < m_Ncpl; i++)
        {
            if (m_Idomain[mf_cell_index] > 0)
            {
                if (m_NumCell2dVert[i] <= 6)
                {
                    // do not split cell
                    m_ConnectivityArray[m++] = 2 * m_NumCell2dVert[i];
                    for (j = m_NumCell2dVert[i] - 1; j >= 0; j--)
                    {
                        m_ConnectivityArray[m++] = mf_vertex_offset + m_Nvert + m_Javert[m_Iavert[i] + j];
                    }
                    for (j = m_NumCell2dVert[i] - 1; j >= 0; j--)
                    {
                        m_ConnectivityArray[m++] = mf_vertex_offset + m_Javert[m_Iavert[i] + j];
                    }
                }
                else
                {
                    // split cell into triangular prisms
                    int mf_node_offset = m_Nvert * (m_NumberOfCellLayers + 1) + k * m_Ncpl;
                    for (j = 0; j < m_NumCell2dVert[i]; j++)
                    {
                        m_ConnectivityArray[m++] = 6;
                        m_ConnectivityArray[m++] = mf_node_offset + m_Ncpl + i;
                        m_ConnectivityArray[m++] = mf_vertex_offset + m_Nvert + m_Javert[m_Iavert[i] + j + 1];
                        m_ConnectivityArray[m++] = mf_vertex_offset + m_Nvert + m_Javert[m_Iavert[i] + j];
                        m_ConnectivityArray[m++] = mf_node_offset + i;
                        m_ConnectivityArray[m++] = mf_vertex_offset + m_Javert[m_Iavert[i] + j + 1];
                        m_ConnectivityArray[m++] = mf_vertex_offset + m_Javert[m_Iavert[i] + j];
                    }
                }
            }
            else
            {
                m_ConnectivityArray[m++] = 0;
            }
            mf_cell_index++;
        }
    }

    // Build layer connectivity array
    // m_LayerConnectivity contains data to construct polygons that outlines the bottom faces
    // of cells in a layer.
    // The first element of m_LayerConnectivity is the number of polygons
    // For each polygon, the following data are stored sequentially in m_LayerConnectivity
    // a. The layer index of the cell
    // b. The number of vertices to define the polygon
    // c. The indices of the vertices that define the polygon
    m_LayerConnectivity    = new int[(m_NumberOfCellLayers + 1) * (njavert + m_Ncpl) + 1];
    m_LayerConnectivity[0] = 0;
    m                      = 1;
    for (k = 0; k <= m_NumberOfCellLayers; k++)
    {
        for (i = 0; i < m_Ncpl; i++)
        {
            use = 1;
            // if working on the top sheet, check that the cell in top layer is active
            if (k == 0)
            {
                if (m_Idomain[i] <= 0)
                {
                    use = 0;
                }
            }
            // otherwise, check if the cell above the sheet is active
            else if (m_Idomain[(k - 1) * m_Ncpl + i] <= 0)
            {
                use = 0;
            }
            if (use)
            {
                m_LayerConnectivity[m++] = k;
                m_LayerConnectivity[m++] = m_NumCell2dVert[i];
                for (j = 0; j < m_NumCell2dVert[i]; j++)
                {
                    m_LayerConnectivity[m++] = m_Javert[m_Iavert[i] + j] + m_Nvert * k;
                }
                m_LayerConnectivity[0]++;
            }
        }
    }

    // build connectivity for stairstepped grid
    m_StairsteppedGridConnectivity = new int[connectivityArraySize];
    m                              = 0;
    p                              = 0;
    mf_cell_index                  = 0;
    for (k = 0; k < m_NumberOfCellLayers; k++)
    {
        int mf_vertex_offset = k * m_Nvert;
        for (i = 0; i < m_Ncpl; i++)
        {
            if (m_Idomain[mf_cell_index] > 0)
            {
                if (m_NumCell2dVert[i] <= 6)
                {
                    // do not split cell
                    m_StairsteppedGridConnectivity[m++] = 2 * m_NumCell2dVert[i];
                    for (j = 0; j < 2 * m_NumCell2dVert[i]; j++)
                    {
                        m_StairsteppedGridConnectivity[m++] = p++;
                    }
                }
                else
                {
                    // split cell into triangular prisms
                    for (int pp = 0; pp < m_NumCell2dVert[i]; pp++)
                    {
                        m_StairsteppedGridConnectivity[m++] = 6;
                        m_StairsteppedGridConnectivity[m++] = cellCenterNodeBegin + 2 * (k * m_Nvert + i);
                        m_StairsteppedGridConnectivity[m++] = p + pp;
                        int pp1                             = (pp + 1) % m_NumCell2dVert[i]; // wrap around for the last vertex
                        m_StairsteppedGridConnectivity[m++] = p + pp1;
                        m_StairsteppedGridConnectivity[m++] = cellCenterNodeBegin + 2 * (k * m_Nvert + i) + 1;
                        m_StairsteppedGridConnectivity[m++] = p + m_NumCell2dVert[i] + pp;
                        m_StairsteppedGridConnectivity[m++] = p + m_NumCell2dVert[i] + pp1;
                    }
                    p += 2 * m_NumCell2dVert[i];
                }
            }
            else
            {
                m_StairsteppedGridConnectivity[m++] = 0;
                p += 2 * m_NumCell2dVert[i];
            }
            mf_cell_index++;
        }
    }

    return nullptr;
}

const char *Modflow6DataSource::CreateDisuGrid(char *gridFile)
{
    int      i, j;
    ifstream in(gridFile, ios::in | ios::binary);
    if (!in.is_open())
    {
        return "Error: Unable to open the binary grid file.";
    }
    char   *aRecord = new char[50];
    char   *q;
    int     ntxt, lentxt, njavert, nja;
    double *top, *bot, *vertices, *cellx, *celly;
    double  xorigin, yorigin, angrot;

    try
    {
        if (!in.read(aRecord, sizeof(char) * 50)) throw 1;
        // Read the version (not used)
        if (!in.read(aRecord, sizeof(char) * 50)) throw 1;
        // determine ntxt
        if (!in.read(aRecord, sizeof(char) * 50)) throw 1;
        q    = strchr(aRecord + 5, ' ');
        *q   = '\0';
        ntxt = atoi(aRecord + 5);
        if (ntxt != 15)
        {
            in.close();
            delete[] aRecord;
            return "Unable to display DISU grid because the binary grid file does not contain data on vertices.";
        }
        // determine lentxt
        if (!in.read(aRecord, sizeof(char) * 50)) throw 1;
        q      = strchr(aRecord + 7, ' ');
        *q     = '\0';
        lentxt = atoi(aRecord + 7);
        // re-allocate aRecord to size lentxt
        delete[] aRecord;
        aRecord  = new char[lentxt];
        // skip the next ntxt records plus comments
        int itxt = 0;
        while (itxt < ntxt)
        {
            if (!in.read(aRecord, sizeof(char) * lentxt)) throw 1;
            if (aRecord[0] != '#') itxt++;
            if (!strncmp(aRecord, "VERTICES", 8))
            {
                q       = strchr(aRecord + 25, ' ');
                *q      = '\0';
                m_Nvert = atoi(aRecord + 25);
            }
            else if (!strncmp(aRecord, "JAVERT", 6))
            {
                q       = strchr(aRecord + 22, ' ');
                *q      = '\0';
                njavert = atoi(aRecord + 22);
            }
        }
        if (!in.read((char *)(&m_NumberOfModflowCells), sizeof(int))) throw 1;
        if (!in.read((char *)(&nja), sizeof(int))) throw 1;
        if (!in.read((char *)(&xorigin), sizeof(double))) throw 1;
        if (!in.read((char *)(&yorigin), sizeof(double))) throw 1;
        if (!in.read((char *)(&angrot), sizeof(double))) throw 1;
        delete[] aRecord;
    }
    catch (...)
    {
        delete[] aRecord;
        return "Error encountered while reading the binary grid file.";
    }

    m_xorigin       = xorigin;
    m_yorigin       = yorigin;
    m_angrot        = angrot;

    // allocate arrays
    top             = new double[m_NumberOfModflowCells];
    bot             = new double[m_NumberOfModflowCells];
    vertices        = new double[2 * m_Nvert];
    cellx           = new double[m_NumberOfModflowCells];
    celly           = new double[m_NumberOfModflowCells];
    m_Iavert        = new int[m_NumberOfModflowCells + 1];
    m_Javert        = new int[njavert];
    m_Ia            = new int[m_NumberOfModflowCells + 1];
    m_Ja            = new int[nja];
    m_Icelltype     = new int[m_NumberOfModflowCells];
    m_NumCell2dVert = new int[m_NumberOfModflowCells];
    m_NumFlowConn   = new int[m_NumberOfModflowCells];

    // continue to read data
    try
    {
        if (!in.read((char *)top, m_NumberOfModflowCells * sizeof(double))) throw 1;
        if (!in.read((char *)bot, m_NumberOfModflowCells * sizeof(double))) throw 1;
        if (!in.read((char *)m_Ia, (m_NumberOfModflowCells + 1) * sizeof(int))) throw 1;
        if (!in.read((char *)m_Ja, nja * sizeof(int))) throw 1;
        if (!in.read((char *)m_Icelltype, m_NumberOfModflowCells * sizeof(int))) throw 1;
        if (!in.read((char *)vertices, 2 * m_Nvert * sizeof(double))) throw 1;
        if (!in.read((char *)cellx, m_NumberOfModflowCells * sizeof(double))) throw 1;
        if (!in.read((char *)celly, m_NumberOfModflowCells * sizeof(double))) throw 1;
        if (!in.read((char *)m_Iavert, (m_NumberOfModflowCells + 1) * sizeof(int))) throw 1;
        if (!in.read((char *)m_Javert, njavert * sizeof(int))) throw 1;
        in.close();
    }
    catch (...)
    {
        delete[] top;
        delete[] bot;
        delete[] vertices;
        delete[] cellx;
        delete[] celly;
        delete[] m_Iavert;
        delete[] m_Javert;
        delete[] m_Ia;
        delete[] m_Ja;
        delete[] m_Icelltype;
        delete[] m_NumCell2dVert;
        delete[] m_NumFlowConn;
        in.close();
        return "Error encountered while reading the binary grid file.";
    }

    // change indices to start counting from zero
    for (i = 0; i < m_NumberOfModflowCells + 1; i++)
    {
        m_Iavert[i]--;
    }
    for (i = 0; i < njavert; i++)
    {
        m_Javert[i]--;
    }
    for (i = 0; i < m_NumberOfModflowCells + 1; i++)
    {
        m_Ia[i]--;
    }
    for (i = 0; i < nja; i++)
    {
        m_Ja[i]--;
    }

    // compute the number of vertices for each cell2d, and the maximum number
    // also compute m_NumberOfVTKPoints and m_NumberOfVTKCells
    int maxVertPerCell2d = 0;
    m_NumberOfVTKPoints  = 0;
    m_NumberOfVTKCells   = 0;
    for (i = 0; i < m_NumberOfModflowCells; i++)
    {
        m_NumCell2dVert[i] = m_Iavert[i + 1] - m_Iavert[i] - 1; // does not count the cell's last vertex of, which is the same as the first vertes
        if (m_NumCell2dVert[i] > maxVertPerCell2d)
        {
            maxVertPerCell2d = m_NumCell2dVert[i];
        }
        m_NumberOfVTKPoints += (2 * m_NumCell2dVert[i]);
        if (m_NumCell2dVert[i] <= 6)
        {
            // don't need to split cells
            m_NumberOfVTKCells++;
        }
        else
        {
            // split cells into wedges
            m_NumberOfVTKCells += m_NumCell2dVert[i];
        }
    }
    // include the cell centers at top and bottom cell faces. These nodes might be needed to spilt cells
    int cellCenterNodeBegin = m_NumberOfVTKPoints;
    m_NumberOfVTKPoints += (2 * m_NumberOfModflowCells);

    // compute the number of flow connections for each cell
    for (i = 0; i < m_NumberOfModflowCells; i++)
    {
        m_NumFlowConn[i] = m_Ia[i + 1] - m_Ia[i] - 1; // does not count a node connected to itself
        if (m_NumFlowConn[i] < 0)
        {
            m_NumFlowConn[i] = 0;
        }
    }

    // TO DO: compute weight vectors to compute specific discharge

    // Compute coordinates (stairstepped grid only)
    m_StairsteppedGridCoordinates = new double[3 * m_NumberOfVTKPoints];
    int m                         = 0;
    // coordinates of vertices (store for each cell in counter clockwise orientation per VTK,
    // bottom face first, then top face)
    for (i = 0; i < m_NumberOfModflowCells; i++)
    {
        for (j = m_NumCell2dVert[i] - 1; j >= 0; j--)
        {
            int k                              = 2 * (m_Javert[m_Iavert[i] + j]);
            m_StairsteppedGridCoordinates[m++] = vertices[k];     // x coordinate
            m_StairsteppedGridCoordinates[m++] = vertices[k + 1]; // y coordinate
            m_StairsteppedGridCoordinates[m++] = bot[i];
        }
        for (j = m_NumCell2dVert[i] - 1; j >= 0; j--)
        {
            int k                              = 2 * (m_Javert[m_Iavert[i] + j]);
            m_StairsteppedGridCoordinates[m++] = vertices[k];
            m_StairsteppedGridCoordinates[m++] = vertices[k + 1];
            m_StairsteppedGridCoordinates[m++] = top[i];
        }
    }
    // coordinates of cell centers at bottom and top faces of cells
    for (i = 0; i < m_NumberOfModflowCells; i++)
    {
        m_StairsteppedGridCoordinates[m++] = cellx[i];
        m_StairsteppedGridCoordinates[m++] = celly[i];
        m_StairsteppedGridCoordinates[m++] = bot[i];
        m_StairsteppedGridCoordinates[m++] = cellx[i];
        m_StairsteppedGridCoordinates[m++] = celly[i];
        m_StairsteppedGridCoordinates[m++] = top[i];
    }

    delete[] top;
    delete[] bot;
    delete[] vertices;
    delete[] cellx;
    delete[] celly;

    // build connectivity (stairstepped grid only)
    m_StairsteppedGridConnectivity = new int[m_NumberOfVTKPoints + m_NumberOfVTKCells];
    m                              = 0;
    int p                          = 0;
    for (i = 0; i < m_NumberOfModflowCells; i++)
    {
        if (m_NumCell2dVert[i] <= 6)
        {
            // do not split cell
            m_StairsteppedGridConnectivity[m++] = 2 * m_NumCell2dVert[i];
            for (j = 0; j < 2 * m_NumCell2dVert[i]; j++)
            {
                m_StairsteppedGridConnectivity[m++] = p++;
            }
        }
        else
        {
            // split cell into triangular prisms
            for (int pp = 0; pp < m_NumCell2dVert[i]; pp++)
            {
                m_StairsteppedGridConnectivity[m++] = 6;
                m_StairsteppedGridConnectivity[m++] = cellCenterNodeBegin + 2 * i;
                m_StairsteppedGridConnectivity[m++] = p + pp;
                int pp1                             = (pp + 1) % m_NumCell2dVert[i]; // wrap around for the last prism
                m_StairsteppedGridConnectivity[m++] = p + pp1;
                m_StairsteppedGridConnectivity[m++] = cellCenterNodeBegin + 2 * i + 1;
                m_StairsteppedGridConnectivity[m++] = p + m_NumCell2dVert[i] + pp;
                m_StairsteppedGridConnectivity[m++] = p + m_NumCell2dVert[i] + pp1;
            }
            p += 2 * m_NumCell2dVert[i];
        }
    }

    // Map Modflow cells to VTK cells
    m_MfCell2VtkCells = new int *[m_NumberOfModflowCells];
    m                 = 0;
    int n             = 0;
    for (i = 0; i < m_NumberOfModflowCells; i++)
    {
        if (m_NumCell2dVert[i] <= 6)
        {
            // one-to-one mapping between Mf cell and Vtk cell
            m_MfCell2VtkCells[m]    = new int[2];
            m_MfCell2VtkCells[m][0] = 1;
            m_MfCell2VtkCells[m][1] = n;
            n++;
        }
        else
        {
            // Mf cell split into multiple Vtk cells
            m_MfCell2VtkCells[m]    = new int[m_NumCell2dVert[i] + 1];
            m_MfCell2VtkCells[m][0] = m_NumCell2dVert[i];
            for (j = 1; j <= m_NumCell2dVert[i]; j++)
            {
                m_MfCell2VtkCells[m][j] = n;
                n++;
            }
        }
        m++;
    }

    return nullptr;
}

const char *Modflow6DataSource::CountHead(char *dataTypeLabel)
{
    int    kstp, kper, n1, n2, n3, j, jmax, k, kmax;
    double pertim, totim, value;
    char   text[16];
    m_NumberOfTimePoints = 0;
    if (m_GridType == GridType::MV_UNSTRUCTURED_GRID)
    {
        jmax = m_NumberOfModflowCells;
        kmax = 1;
    }
    else
    {
        jmax = m_Ncpl;
        kmax = m_NumberOfCellLayers;
    }
    m_PeriodStep = new mvLinkList;
    m_IfHead.seekg(0, m_IfHead.end);
    size_t length = m_IfHead.tellg();
    m_IfHead.seekg(0, m_IfHead.beg);
    while (length > 0)
    {
        for (k = 0; k < kmax; k++)
        {
            m_IfHead.read((char *)(&kstp), sizeof(int));
            m_IfHead.read((char *)(&kper), sizeof(int));
            m_IfHead.read((char *)(&pertim), sizeof(double));
            m_IfHead.read((char *)(&totim), sizeof(double));
            m_IfHead.read(text, 16 * sizeof(char));
            m_IfHead.read((char *)(&n1), sizeof(int));
            m_IfHead.read((char *)(&n2), sizeof(int));
            m_IfHead.read((char *)(&n3), sizeof(int));
            for (j = 0; j < jmax; j++)
            {
                m_IfHead.read((char *)(&value), sizeof(double));
            }
            length -= (5 * sizeof(int) + (jmax + 2) * sizeof(double) + 16 * sizeof(char));
            if (k == 0)
            {
                m_NumberOfTimePoints++;
                int *perstp = new int[2];
                perstp[0]   = kper;
                perstp[1]   = kstp;
                m_PeriodStep->AddItem(perstp);
            }
        }
    }
    strncpy(dataTypeLabel, text, 16);
    dataTypeLabel[16] = '\0';
    mvUtil::TrimRight(dataTypeLabel);
    return nullptr;
}

const char *Modflow6DataSource::CountBudgetAndFeatures()
{
    int    kstp, kper, ndim1, ndim2, ndim3, imeth, i, j, nlist, nval, match;
    double delt, pertim, totim, value, t1;
    char   flowType[17], text[16], auxtxt[16];
    flowType[16] = '\0';
    char *pname;
    int  *pm;
    m_BudgetText               = new mvLinkList;
    mvLinkList *vtk_cell_count = new mvLinkList;
    m_PeriodStep->InitTraversal();
    int *perstp = (int *)m_PeriodStep->GetNextItem();
    m_IfBudget.seekg(0, m_IfBudget.end);
    size_t length = m_IfBudget.tellg();
    m_IfBudget.seekg(0, m_IfBudget.beg);
    m_NumberOfBudgetItems = 0;
    int numTimePoints     = 0;
    t1                    = 0;
    while (length > 0 && numTimePoints < m_NumberOfTimePoints)
    {
        m_IfBudget.read((char *)(&kstp), sizeof(int));
        m_IfBudget.read((char *)(&kper), sizeof(int));
        if (kper != perstp[0] || kstp != perstp[1])
        {
            perstp = (int *)m_PeriodStep->GetNextItem();
            if (perstp == 0)
            {
                // error -- execution should not get here
                m_IfBudget.close();
                delete vtk_cell_count;
                m_HasSpecificDischargeData = 0;
                m_ModelFeatureArraySize    = 0;
                return nullptr;
            }
            if (kper != perstp[0] || kstp != perstp[1])
            {
                // times at which flows are saved don't match with times at which heads are saved
                m_IfBudget.close();
                delete vtk_cell_count;
                m_HasSpecificDischargeData = 0;
                m_ModelFeatureArraySize    = 0;
                return nullptr;
            }
            else
            {
                numTimePoints++;
            }
        }
        m_IfBudget.read(flowType, 16 * sizeof(char));
        m_IfBudget.read((char *)(&ndim1), sizeof(int));
        m_IfBudget.read((char *)(&ndim2), sizeof(int));
        m_IfBudget.read((char *)(&ndim3), sizeof(int));
        ndim3 = -ndim3;
        m_IfBudget.read((char *)(&imeth), sizeof(int));
        m_IfBudget.read((char *)(&delt), sizeof(double));
        m_IfBudget.read((char *)(&pertim), sizeof(double));
        m_IfBudget.read((char *)(&totim), sizeof(double));
        if (t1 == 0)
        {
            t1                    = totim;
            m_NumberOfBudgetItems = 1;
        }
        else if (t1 == totim)
        {
            m_NumberOfBudgetItems++;
        }
        length -= (6 * sizeof(int) + 3 * sizeof(double) + 16 * sizeof(char));
        if (imeth == 1)
        {
            for (i = 0; i < ndim1 * ndim2 * ndim3; i++) // used only for intercell flow or storage, which are not displayed by Model Viewer
            {
                m_IfBudget.read((char *)(&value), sizeof(double));
            }
            length -= ndim1 * ndim2 * ndim3 * sizeof(double);
        }
        else if (imeth == 6)
        {
            // read the data
            int num_vtk_cells_for_this_flowType = 0;
            int mf_cell_index;
            int node, id2;
            m_IfBudget.read(text, 16 * sizeof(char));
            m_IfBudget.read(text, 16 * sizeof(char));
            m_IfBudget.read(text, 16 * sizeof(char));
            m_IfBudget.read(text, 16 * sizeof(char));
            m_IfBudget.read((char *)(&nval), sizeof(int));
            for (i = 0; i < nval - 1; i++)
            {
                m_IfBudget.read(auxtxt, 16 * sizeof(char));
            }
            m_IfBudget.read((char *)(&nlist), sizeof(int));
            for (i = 0; i < nlist; i++)
            {
                m_IfBudget.read((char *)(&node), sizeof(int));
                m_IfBudget.read((char *)(&id2), sizeof(int));
                mf_cell_index = node - 1;
                if (m_GridType == GridType::MV_STRUCTURED_GRID)
                {
                    num_vtk_cells_for_this_flowType++;
                }
                else
                {
                    num_vtk_cells_for_this_flowType += m_MfCell2VtkCells[mf_cell_index][0];
                }
                for (j = 0; j < nval; j++)
                {
                    m_IfBudget.read((char *)(&value), sizeof(double));
                }
            }
            length -= (2 * nlist + 2) * sizeof(int) + 16 * (nval + 3) * sizeof(char) + (nval * nlist) * sizeof(double);
            // check if data are specific discharge or model features
            if (strstr(flowType, "DATA-SPDIS"))
            {
                m_HasSpecificDischargeData = 1;
            }
            //         else if (!strstr(flowType, "MVR") && !strstr(flowType, "UZF-GWD")
            //             &&  !strstr(flowType, "UZF-GWET") &&  !strstr(flowType, "LAK")
            //             &&  !strstr(flowType, "FLOW-JA-FACE"))
            else if (IsModelFeature(flowType))
            {
                m_BudgetText->InitTraversal();
                vtk_cell_count->InitTraversal();
                match = 0;
                for (i = 0; i < m_BudgetText->GetNumberOfItems(); i++)
                {
                    pname = (char *)m_BudgetText->GetNextItem();
                    pm    = (int *)vtk_cell_count->GetNextItem();

                    if (!strncmp(text, pname, 16))
                    {
                        match = 1;
                        break;
                    }
                }
                if (match)
                {
                    if (*pm < num_vtk_cells_for_this_flowType)
                    {
                        *pm = num_vtk_cells_for_this_flowType;
                    }
                }
                else
                {
                    pname = new char[16];
                    strncpy(pname, text, 16);
                    m_BudgetText->AddItem(pname);
                    pm = new int[1];
                    *pm = num_vtk_cells_for_this_flowType;
                    vtk_cell_count->AddItem(pm);
                }
            }
        }
    }
    m_NumberOfModelFeatureTypes = m_BudgetText->GetNumberOfItems();
    m_ModelFeatureArraySize     = 0;
    //   if (m_NumberOfModelFeatureTypes)
    //   {
    m_Rgba                      = new double *[m_NumberOfModelFeatureTypes];
    m_BudgetText->InitTraversal();
    vtk_cell_count->InitTraversal();
    m_ModelFeatureLabels = new char[40 * m_NumberOfModelFeatureTypes];
    char *p              = m_ModelFeatureLabels;
    for (i = 0; i < m_NumberOfModelFeatureTypes; i++)
    {
        pname = (char *)m_BudgetText->GetNextItem();
        strncpy(p, pname, 16);
        p[16] = '\0';
        mvUtil::TrimLeft(p);
        for (size_t jj = strlen(p); jj < 40; jj++)
        {
            p[jj] = ' ';
        }
        p += 40;
        pm = (int *)vtk_cell_count->GetNextItem();
        m_ModelFeatureArraySize += ((*pm) + 1);
        m_Rgba[i]         = new double[4];
        double wavelength = ((double)i / (double)(m_NumberOfModelFeatureTypes - 1)) * 250 + 440;
        mvUtil::Wavelength2RGBA(wavelength, m_Rgba[i]);
    }
    //   }
    delete vtk_cell_count;

    // ********* temporary -- no vectors for DISU GRID **********
    if (m_GridType == GridType::MV_UNSTRUCTURED_GRID)
    {
        m_HasSpecificDischargeData = 0;
    }

    return nullptr;
}

void Modflow6DataSource::GetTimePoints(double *timePoints, int *periods, int *steps)
{
    int    kstp, kper, n1, n2, n3, i, j, jmax, k, kmax;
    double pertim, totim, value;
    char   text[16];
    if (m_GridType == GridType::MV_UNSTRUCTURED_GRID)
    {
        jmax = m_NumberOfModflowCells;
        kmax = 1;
    }
    else
    {
        jmax = m_Ncpl;
        kmax = m_NumberOfCellLayers;
    }
    m_IfHead.seekg(0, m_IfHead.beg);
    for (i = 0; i < m_NumberOfTimePoints; i++)
    {
        for (k = 0; k < kmax; k++)
        {
            m_IfHead.read((char *)(&kstp), sizeof(int));
            m_IfHead.read((char *)(&kper), sizeof(int));
            m_IfHead.read((char *)(&pertim), sizeof(double));
            m_IfHead.read((char *)(&totim), sizeof(double));
            m_IfHead.read(text, 16 * sizeof(char));
            m_IfHead.read((char *)(&n1), sizeof(int));
            m_IfHead.read((char *)(&n2), sizeof(int));
            m_IfHead.read((char *)(&n3), sizeof(int));
            for (j = 0; j < jmax; j++)
            {
                m_IfHead.read((char *)(&value), sizeof(double));
            }
            if (k == 0)
            {
                timePoints[i] = totim;
                periods[i]    = kper;
                steps[i]      = kstp;
            }
        }
    }
}

void Modflow6DataSource::SetTimePointTo(int timePointIndex)
{
    int    kstp, kper, n1, n2, n3, ilay, i, j, k, imeth, ivalue, nlist, nval;
    double pertim, totim, value, delt, value1, value2, value3;
    char   text[16], flowType[17];
    flowType[16] = '\0';
    // if not showing the next time point, then backup to the beginning of the file and
    // move to the timePointIndex.
    if (timePointIndex != -1)
    {
        // get to the beginning of the desired time point in the head file
        m_IfHead.seekg(0, m_IfHead.beg);
        for (int itp = 0; itp < timePointIndex - 1; itp++)
        {
            for (k = 0; k < m_NumberOfCellLayers; k++)
            {
                m_IfHead.read((char *)(&kstp), sizeof(int));
                m_IfHead.read((char *)(&kper), sizeof(int));
                m_IfHead.read((char *)(&pertim), sizeof(double));
                m_IfHead.read((char *)(&totim), sizeof(double));
                m_IfHead.read(text, 16 * sizeof(char));
                m_IfHead.read((char *)(&n1), sizeof(int));
                m_IfHead.read((char *)(&n2), sizeof(int));
                int ncpl;
                if (m_GridType == GridType::MV_STRUCTURED_GRID)
                {
                    ncpl = n1 * n2;
                }
                else
                {
                    ncpl = n1;
                }
                m_IfHead.read((char *)(&ilay), sizeof(int));
                for (i = 0; i < ncpl; i++)
                {
                    m_IfHead.read((char *)(&value), sizeof(double));
                }
            }
        }
        // get to the beginning of the desired time point in the budget file
        if (m_IfBudget.is_open())
        {
            m_IfBudget.seekg(0, m_IfBudget.beg);
            for (int itp = 0; itp < (timePointIndex - 1) * (m_NumberOfBudgetItems); itp++)
            {
                m_IfBudget.read((char *)(&kstp), sizeof(int));
                m_IfBudget.read((char *)(&kper), sizeof(int));
                m_IfBudget.read(text, 16 * sizeof(char));
                m_IfBudget.read((char *)(&n1), sizeof(int));
                m_IfBudget.read((char *)(&n2), sizeof(int));
                m_IfBudget.read((char *)(&n3), sizeof(int));
                n3 = -n3;
                m_IfBudget.read((char *)(&imeth), sizeof(int));
                m_IfBudget.read((char *)(&delt), sizeof(double));
                m_IfBudget.read((char *)(&pertim), sizeof(double));
                m_IfBudget.read((char *)(&totim), sizeof(double));
                if (imeth == 1)
                {
                    for (i = 0; i < n1 * n2 * n3; i++)
                    {
                        m_IfBudget.read((char *)(&value), sizeof(double));
                    }
                }
                else if (imeth == 6)
                {
                    m_IfBudget.read(text, 16 * sizeof(char));
                    m_IfBudget.read(text, 16 * sizeof(char));
                    m_IfBudget.read(text, 16 * sizeof(char));
                    m_IfBudget.read(text, 16 * sizeof(char));
                    m_IfBudget.read((char *)(&nval), sizeof(int));
                    for (i = 0; i < nval - 1; i++)
                    {
                        m_IfBudget.read(text, 16 * sizeof(char));
                    }
                    m_IfBudget.read((char *)(&nlist), sizeof(int));
                    for (i = 0; i < nlist; i++)
                    {
                        m_IfBudget.read((char *)(&ivalue), sizeof(int));
                        m_IfBudget.read((char *)(&ivalue), sizeof(int));
                        for (k = 0; k < nval; k++)
                        {
                            m_IfBudget.read((char *)(&value), sizeof(double));
                        }
                    }
                }
            }
        }
    }
    // initialize points
    memset(m_ScalarArray, 0, m_NumberOfVTKPoints * sizeof(double));

    int *modflow_active_cell = new int[m_NumberOfModflowCells];
    memset(modflow_active_cell, 0, m_NumberOfModflowCells * sizeof(int));

    // read head
    if (m_GridType == GridType::MV_STRUCTURED_GRID)
    {
        int     ncol, nrow, ncpl;
        double *pointValues = m_ScalarArray;
        double *cellValues  = pointValues + m_NumberOfVTKPoints;
        for (k = 0; k < m_NumberOfCellLayers; k++)
        {
            m_IfHead.read((char *)(&kstp), sizeof(int));
            m_IfHead.read((char *)(&kper), sizeof(int));
            m_IfHead.read((char *)(&pertim), sizeof(double));
            m_IfHead.read((char *)(&totim), sizeof(double));
            m_IfHead.read(text, 16 * sizeof(char));
            m_IfHead.read((char *)(&ncol), sizeof(int));
            m_IfHead.read((char *)(&nrow), sizeof(int));
            ncpl = ncol * nrow;
            m_IfHead.read((char *)(&ilay), sizeof(int));
            for (j = 0; j < nrow; j++)
            {
                for (i = 0; i < ncol; i++)
                {
                    m_IfHead.read((char *)(&value), sizeof(double));
                    // if the head value indicates it as a dry cell, or if the head is below
                    // the bottom of the cell, set the value to indicate an inactive cell
                    int ijk = k * ncpl + j * ncol + i;
                    int jj  = (m_NumberOfCellLayers - k - 1) * ncpl + (nrow - j - 1) * ncol + i;
                    if (PracticallyEqual(value, m_DryCellValue) || (m_Icelltype[ijk] != 0 && value < m_Elev[jj]))
                    {
                        value = m_InactiveCellValue;
                    }
                    if (!PracticallyEqual(value, m_InactiveCellValue)) modflow_active_cell[jj] = 1;
                    cellValues[jj] = value;
                }
            }
        }
        double znull    = -1.0e20;
        int   *useLayer = 0;
        mvUtil::interp3d(cellValues, pointValues, m_Delr, m_Delc_revdir, m_Elev,
                         ncol, nrow, m_NumberOfCellLayers, m_InactiveCellValue, znull, useLayer);
    }
    else if (m_GridType == GridType::MV_LAYERED_GRID)
    {
        int     p, q, ncpl;
        double  dx, dy, dz;
        double  weight;
        int     m            = 0;
        int     node_offset  = (m_NumberOfCellLayers + 1) * m_Nvert;

        double *sumOfWeights = new double[m_NumberOfVTKPoints];
        memset(sumOfWeights, 0, m_NumberOfVTKPoints * sizeof(double));
        for (k = 0; k < m_NumberOfCellLayers; k++)
        {
            m_IfHead.read((char *)(&kstp), sizeof(int));
            m_IfHead.read((char *)(&kper), sizeof(int));
            m_IfHead.read((char *)(&pertim), sizeof(double));
            m_IfHead.read((char *)(&totim), sizeof(double));
            m_IfHead.read(text, 16 * sizeof(char));
            m_IfHead.read((char *)(&ncpl), sizeof(int));
            m_IfHead.read((char *)(&n2), sizeof(int));
            m_IfHead.read((char *)(&ilay), sizeof(int));
            for (i = 0; i < ncpl; i++)
            {
                m_IfHead.read((char *)(&value), sizeof(double));
                // if the head value indicates it as a dry cell, set the value to indicate an inactive cell
                q = (k * ncpl + i); // q is the index of the cell3d
                if (PracticallyEqual(value, m_DryCellValue) || (m_Icelltype[q] != 0 && value < m_Elev[q + ncpl]))
                {
                    value = m_InactiveCellValue;
                }

                if (!PracticallyEqual(value, m_InactiveCellValue))
                {
                    modflow_active_cell[q] = 1;
                    for (j = 0; j < m_NumCell2dVert[i]; j++)
                    {
                        // vtkpoint value at vertex on upper sheet
                        p      = k * m_Nvert + m_Javert[m_Iavert[i] + j]; // p is the index of the vertex
                        dx     = m_InterpolatedGridCoordinates[3 * p] - m_VectorGridCoordinates[3 * q];
                        dy     = m_InterpolatedGridCoordinates[3 * p + 1] - m_VectorGridCoordinates[3 * q + 1];
                        dz     = m_InterpolatedGridCoordinates[3 * p + 2] - m_VectorGridCoordinates[3 * q + 2];
                        weight = 1 / sqrt(dx * dx + dy * dy + dz * dz);
                        m_ScalarArray[p] += value * weight;
                        sumOfWeights[p] += weight;
                        // vtkpoint value at vertex on lower sheet
                        p += m_Nvert;
                        dx     = m_InterpolatedGridCoordinates[3 * p] - m_VectorGridCoordinates[3 * q];
                        dy     = m_InterpolatedGridCoordinates[3 * p + 1] - m_VectorGridCoordinates[3 * q + 1];
                        dz     = m_InterpolatedGridCoordinates[3 * p + 2] - m_VectorGridCoordinates[3 * q + 2];
                        weight = 1 / sqrt(dx * dx + dy * dy + dz * dz);
                        m_ScalarArray[p] += value * weight;
                        sumOfWeights[p] += weight;
                    }
                    // vtkpoint value at node on upper sheet
                    p      = node_offset + k * ncpl + i;
                    weight = 1 / abs(m_InterpolatedGridCoordinates[3 * p + 2] - m_VectorGridCoordinates[3 * q + 2]);
                    m_ScalarArray[p] += value * weight;
                    sumOfWeights[p] += weight;
                    // vtkpoint value at node on lower sheet
                    p += ncpl;
                    weight = 1 / abs(m_InterpolatedGridCoordinates[3 * p + 2] - m_VectorGridCoordinates[3 * q + 2]);
                    m_ScalarArray[p] += value * weight;
                    sumOfWeights[p] += weight;
                }
                // vtkcell value
                if (m_Idomain[k * ncpl + i] > 0)
                {
                    if (m_NumCell2dVert[i] >= 3 && m_NumCell2dVert[i] <= 6)
                    {
                        if (!PracticallyEqual(value, m_DryCellValue))
                        {
                            m_ScalarArray[m_NumberOfVTKPoints + m] = value;
                        }
                        else
                        {
                            m_ScalarArray[m_NumberOfVTKPoints + m] = m_InactiveCellValue;
                        }
                        m++;
                    }
                    else if (m_NumCell2dVert[i] > 6)
                    {
                        for (j = 0; j < m_NumCell2dVert[i]; j++)
                        {
                            if (!PracticallyEqual(value, m_DryCellValue))
                            {
                                m_ScalarArray[m_NumberOfVTKPoints + m] = value;
                            }
                            else
                            {
                                m_ScalarArray[m_NumberOfVTKPoints + m] = m_InactiveCellValue;
                            }
                            m++;
                        }
                    }
                }
            }
        }
        for (k = 0; k < m_NumberOfVTKPoints; k++)
        {
            if (sumOfWeights[k] > 0)
            {
                m_ScalarArray[k] /= sumOfWeights[k];
            }
        }
        delete[] sumOfWeights;
    }
    else if (m_GridType == GridType::MV_UNSTRUCTURED_GRID)
    {
        int nodes, n1, n2;
        memset(m_ScalarArray, 0, m_NumberOfVTKPoints * sizeof(double));
        m_IfHead.read((char *)(&kstp), sizeof(int));
        m_IfHead.read((char *)(&kper), sizeof(int));
        m_IfHead.read((char *)(&pertim), sizeof(double));
        m_IfHead.read((char *)(&totim), sizeof(double));
        m_IfHead.read(text, 16 * sizeof(char));
        m_IfHead.read((char *)(&nodes), sizeof(int));
        m_IfHead.read((char *)(&n1), sizeof(int));
        m_IfHead.read((char *)(&n2), sizeof(int));
        int m = 0;
        for (i = 0; i < nodes; i++)
        {
            m_IfHead.read((char *)(&value), sizeof(double));
            if (m_NumCell2dVert[i] >= 3 && m_NumCell2dVert[i] <= 6)
            {
                if (!PracticallyEqual(value, m_DryCellValue))
                {
                    m_ScalarArray[m_NumberOfVTKPoints + m] = value;
                }
                else
                {
                    m_ScalarArray[m_NumberOfVTKPoints + m] = m_InactiveCellValue;
                }
                m++;
            }
            else if (m_NumCell2dVert[i] > 6)
            {
                for (j = 0; j < m_NumCell2dVert[i]; j++)
                {
                    if (!PracticallyEqual(value, m_DryCellValue))
                    {
                        m_ScalarArray[m_NumberOfVTKPoints + m] = value;
                    }
                    else
                    {
                        m_ScalarArray[m_NumberOfVTKPoints + m] = m_InactiveCellValue;
                    }
                    m++;
                }
            }
        }
    }
    // read budget
    if (m_IfBudget.is_open())
    {
        int ip = 0;
        for (int ibud = 0; ibud < m_NumberOfBudgetItems; ibud++)
        {
            m_IfBudget.read((char *)(&kstp), sizeof(int));
            m_IfBudget.read((char *)(&kper), sizeof(int));
            m_IfBudget.read(flowType, 16 * sizeof(char));
            m_IfBudget.read((char *)(&n1), sizeof(int));
            m_IfBudget.read((char *)(&n2), sizeof(int));
            m_IfBudget.read((char *)(&n3), sizeof(int));
            n3 = -n3;
            m_IfBudget.read((char *)(&imeth), sizeof(int));
            m_IfBudget.read((char *)(&delt), sizeof(double));
            m_IfBudget.read((char *)(&pertim), sizeof(double));
            m_IfBudget.read((char *)(&totim), sizeof(double));
            if (imeth == 1) // only used for cell-by-cell flow terms, which are not used in Model Viewer
            {
                for (i = 0; i < n1 * n2 * n3; i++)
                {
                    m_IfBudget.read((char *)(&value), sizeof(double));
                }
            }
            else if (imeth == 6)
            {
                int mf_cell_index;
                int node, id2, np;
                //            int modelFeature = (!strstr(flowType, "MVR") && !strstr(flowType, "UZF-GWD")
                //                   &&  !strstr(flowType, "UZF-GWET") &&  !strstr(flowType, "LAK")
                //                   &&  !strstr(flowType, "FLOW-JA-FACE") && !strstr(flowType, "DATA-SPDIS"));

                m_IfBudget.read(text, 16 * sizeof(char));
                m_IfBudget.read(text, 16 * sizeof(char));
                m_IfBudget.read(text, 16 * sizeof(char));
                m_IfBudget.read(text, 16 * sizeof(char));
                m_IfBudget.read((char *)(&nval), sizeof(int));
                for (i = 0; i < nval - 1; i++)
                {
                    m_IfBudget.read(text, 16 * sizeof(char));
                }
                m_IfBudget.read((char *)(&nlist), sizeof(int));
                if (IsModelFeature(flowType))
                {
                    np                      = ip;
                    assert(0 <= np && np < m_ModelFeatureArraySize);
                    m_ModelFeatureArray[np] = 0; // this element holds the number of vtkcells associated with the feature
                    ip++;
                }
                if (strstr(flowType, "DATA-SPDIS") && m_HasSpecificDischargeData /* need to temporarily eliminate DISU grid */)
                {
                    for (i = 0; i < 3 * m_NumberOfModflowCells; i++)
                    {
                        m_VectorArray[i] = m_InactiveCellValue;
                    }
                }
                for (i = 0; i < nlist; i++)
                {
                    m_IfBudget.read((char *)(&node), sizeof(int));
                    m_IfBudget.read((char *)(&id2), sizeof(int));

                    if (IsModelFeature(flowType))
                    {
                        if (m_GridType == GridType::MV_STRUCTURED_GRID)
                        {
                            // find the Modflow col, row, and layer for this node
                            int nm1                 = node - 1;
                            int ncpl                = m_NumberOfCellColumns * m_NumberOfCellRows;
                            int layer               = (nm1) / ncpl + 1;
                            int row                 = (nm1 % ncpl) / m_NumberOfCellColumns + 1;
                            int col                 = ((nm1 % ncpl) % m_NumberOfCellColumns) + 1;
                            // find the VTK index for this node
                            assert(0 <= ip && ip < m_ModelFeatureArraySize);
                            m_ModelFeatureArray[ip] = (m_NumberOfCellLayers - layer) * ncpl + (m_NumberOfCellRows - row) * m_NumberOfCellColumns + col - 1;
                            assert(0 <= np && np < m_ModelFeatureArraySize);
                            m_ModelFeatureArray[np]++;
                            ip++;
                        }
                        else
                        {
                            mf_cell_index = node - 1;
                            for (k = 1; k <= m_MfCell2VtkCells[mf_cell_index][0]; k++)
                            {
                                assert(0 <= ip && ip < m_ModelFeatureArraySize);
                                m_ModelFeatureArray[ip] = m_MfCell2VtkCells[mf_cell_index][k];
                                assert(0 <= np && np < m_ModelFeatureArraySize);
                                m_ModelFeatureArray[np]++;
                                ip++;
                            }
                        }
                    }
                    if (strstr(flowType, "DATA-SPDIS"))
                    {
                        m_IfBudget.read((char *)(&value), sizeof(double));
                        m_IfBudget.read((char *)(&value1), sizeof(double));
                        m_IfBudget.read((char *)(&value2), sizeof(double));
                        m_IfBudget.read((char *)(&value3), sizeof(double));
                        if (m_GridType == GridType::MV_STRUCTURED_GRID)
                        {
                            // find the Modflow col, row, and layer for this node
                            int nm1   = node - 1;
                            int ncpl  = m_NumberOfCellColumns * m_NumberOfCellRows;
                            int layer = (nm1) / ncpl + 1;
                            int row   = (nm1 % ncpl) / m_NumberOfCellColumns + 1;
                            int col   = ((nm1 % ncpl) % m_NumberOfCellColumns) + 1;
                            // find the vtk index for this node
                            int ivtk  = (m_NumberOfCellLayers - layer) * ncpl + (m_NumberOfCellRows - row) * m_NumberOfCellColumns + col - 1;
                            if (modflow_active_cell[ivtk])
                            {
                                m_VectorArray[3 * ivtk]     = value1;
                                m_VectorArray[3 * ivtk + 1] = value2;
                                m_VectorArray[3 * ivtk + 2] = value3;
                            }
                            else
                            {
                                m_VectorArray[3 * ivtk]     = m_InactiveCellValue;
                                m_VectorArray[3 * ivtk + 1] = m_InactiveCellValue;
                                m_VectorArray[3 * ivtk + 2] = m_InactiveCellValue;
                            }
                        }
                        else if (m_GridType == GridType::MV_LAYERED_GRID /* || m_GridType == GridType::MV_UNSTRUCTURED_GRID */)
                        {
                            int nm1 = node - 1;
                            if (modflow_active_cell[nm1])
                            {
                                m_VectorArray[3 * nm1]     = value1;
                                m_VectorArray[3 * nm1 + 1] = value2;
                                m_VectorArray[3 * nm1 + 2] = value3;
                            }
                            else
                            {
                                m_VectorArray[3 * nm1]     = m_InactiveCellValue;
                                m_VectorArray[3 * nm1 + 1] = m_InactiveCellValue;
                                m_VectorArray[3 * nm1 + 2] = m_InactiveCellValue;
                            }
                        }
                    }
                    else // if not specific discharge, just read DATA2D and ignore.
                    {
                        for (k = 0; k < nval; k++)
                        {
                            m_IfBudget.read((char *)(&value), sizeof(double));
                        }
                    }
                }
            }
        } // end block to read each budget item
    }     // end block to read from m_IfBudget
    delete[] modflow_active_cell;
}

int Modflow6DataSource::PracticallyEqual(double value1, double value2)
{
    double tolerance = 1.0e-6;
    if (abs(value1 - value2) / abs((value1 + value2) / 2) < tolerance)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int Modflow6DataSource::GetNumberOfVTKCellsInLayer(int k)
{
    if (m_NumberOfVTKCellsInLayer)
    {
        if (k >= 0 && k < m_NumberOfCellLayers)
        {
            return m_NumberOfVTKCellsInLayer[k];
        }
    }
    return 0;
}

int Modflow6DataSource::IsModelFeature(char *flowType)
{
    return (!strstr(flowType, "MVR") &&
            !strstr(flowType, "UZF-GWD") &&
            !strstr(flowType, "UZF-GWET") &&
            !strstr(flowType, "LAK") &&
            !strstr(flowType, "FLOW-JA-FACE") &&
            !strstr(flowType, "CSUB") &&             //  @TODO fixes crash when loading ex-gwf-csub-p04
            !strstr(flowType, "DATA-SPDIS"));
}

void Modflow6DataSource::ExtractFileName(char *aString)
{
    char *p = strchr(aString, ' ');
    if (p)
    {
        *p = '\0';
    }
}