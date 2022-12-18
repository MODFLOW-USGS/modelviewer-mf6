#include "ModpathReader.h"
#include "mvUtil.h"
#include <fstream>
#include <string.h>
#include "vtkIntArray.h"
#include "vtkDoubleArray.h"

using std::ifstream;

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void ModpathReader::ReadData(char *pathlineFile, int &numPathlines, int &numCoordinates,
                             double *&coordinates, double *&scalarArrayTime, double *&scalarArrayMaxTime,
                             double *&scalarArrayMinTime, vtkIdType *&pointArray, bool backwards, double &minPositiveTime)
{
    ifstream in(pathlineFile, std::ifstream::in);
    char     line[1024];
    int      i, j, nlines, ncoord, version; //, dummyInt;
    double   tref, v[4], dummy;
    char     Number[1000];
    version = 1;
    // read the first line.
    // Determine the version and file type in order to get the reference time.
    in.getline(line, 1024);
    if (strncmp(line + 22, "6", 1) == 0)
    {
        version = 6;
        in.getline(line, 1024);
        if (strncmp(line + 1, "1", 1) == 0)
        {
            backwards = false;
        }
        else if (strncmp(line + 1, "2", 1) == 0)
        {
            backwards = true;
        }
        mvUtil::ExtractFirstString(line, Number);
        mvUtil::ExtractFirstString(line, Number);
        tref = mvUtil::Fortran_atof(Number);

        in.getline(line, 1024);
        in.getline(line, 1024);
    }
    else if (strncmp(line + 12, "5.0", 3) == 0)
    {
        version = 5;
        if (strncmp(line + 16, "COMPACT", 7) == 0)
        {
            line[46] = '\0';
            tref     = atof(line + 32);
        }
        else
        {
            line[38] = '\0';
            tref     = atof(line + 24);
        }
    }
    else if (strncmp(line + 20, "4.00", 4) == 0)
    {
        version = 4;
        if (strncmp(line + 49, "COMPACT", 7) == 0)
        {
            line[78] = '\0';
            tref     = atof(line + 63);
        }
        else
        {
            line[70] = '\0';
            tref     = atof(line + 55);
        }
    }
    else if (strncmp(line + 20, "3.00", 4) == 0)
    {
        version = 3;
        if (strncmp(line + 47, "COMPACT", 7) == 0)
        {
            line[76] = '\0';
            tref     = atof(line + 61);
        }
        else
        {
            line[68] = '\0';
            tref     = atof(line + 53);
        }
    }

    // Use vtk arrays to hold input data
    vtkIntArray    *intArray    = vtkIntArray::New();
    vtkDoubleArray *doubleArray = vtkDoubleArray::New();
    doubleArray->SetNumberOfComponents(4);
    nlines = 0;
    while (!in.eof())
    {
        // i = particle index number
        // v[0] = Global coordinate in the x-direction
        // v[1] = Global coordinate in the y-direction
        // v[2] = Global coordinate in the z-direction
        // v[3] = Cumulative tracking time
        if (version <= 5)
        {
            in >> i >> v[0] >> v[1] >> dummy >> v[2] >> v[3];
            // negative tracking time indicates one that is specified by
            // the user to be saved. Change this back to positive
            if (v[3] < 0)
            {
                v[3] = -v[3];
            }
        }
        else
        {
            // in.getline(line, 1024);
            mvUtil::ExtractFirstString(line, Number);
            i = atoi(Number);

            // skip 3 numbers
            mvUtil::ExtractFirstString(line, Number);
            mvUtil::ExtractFirstString(line, Number);
            mvUtil::ExtractFirstString(line, Number);

            mvUtil::ExtractFirstString(line, Number);
            v[3] = mvUtil::Fortran_atof(Number);
            mvUtil::ExtractFirstString(line, Number);
            v[0] = mvUtil::Fortran_atof(Number);
            mvUtil::ExtractFirstString(line, Number);
            v[1] = mvUtil::Fortran_atof(Number);
            mvUtil::ExtractFirstString(line, Number);
            v[2] = mvUtil::Fortran_atof(Number);
        }
        in.getline(line, 1024);
        if (strlen(line) == 0) break;

        // Look for the last point of this pathline to see if
        // the coordinates are the same as the newly read coordinates.
        // If yes, ignore this point.
        int intCount = intArray->GetNumberOfTuples();
        int flag     = 0;
        for (j = intCount - 1; j >= 0 && flag == 0; j--)
        {
            if (intArray->GetValue(j) + 1 == i)
            {
                double w[4];
#if ((VTK_MAJOR_VERSION == 7) && (VTK_MINOR_VERSION <= 1) || (VTK_MAJOR_VERSION < 7))  //  https://vtk.org/doc/nightly/html/VTK-7-1-Changes.html
                doubleArray->GetTupleValue(j, w);
#else
                doubleArray->GetTypedTuple(j, w);
#endif

                if (v[0] == w[0] && v[1] == w[1] && v[2] == w[2])
                {
                    flag = -1; // ignore coincident point
                }
                else
                {
                    flag = 1; // different point
                }
                break;
            }
        }
        if (flag != -1)
        {
            intArray->InsertNextValue(i - 1);
            doubleArray->InsertNextTuple(v);
            if (i > nlines)
            {
                nlines = i;
            }
        }
    }
    in.close();

    ncoord             = intArray->GetNumberOfTuples();
    int *size          = new int[nlines];
    int *locator       = new int[nlines];
    pointArray         = new vtkIdType[ncoord + nlines];
    coordinates        = new double[3 * ncoord];
    scalarArrayTime    = new double[ncoord];
    scalarArrayMaxTime = new double[ncoord];
    scalarArrayMinTime = new double[ncoord];
    // determine the number of points in each pathline. This
    // is stored in the array size, i.e, size[i] = number of
    // points in pathline i
    for (i = 0; i < nlines; i++)
    {
        size[i] = 0;
    }
    for (j = 0; j < ncoord; j++)
    {
        size[intArray->GetValue(j)]++;
    }
    // locator[i] = position to insert next pathline point index for
    // pathline i.
    numPathlines = 0;
    int pos      = 0;
    for (i = 0; i < nlines; i++)
    {
        if (size[i] > 1)
        {
            pointArray[pos] = size[i];
            locator[i]      = pos + 1;
            pos += size[i] + 1;
            numPathlines++;
        }
        else
        {
            locator[i] = -1; // exclude this line because it has less than 2 points
        }
    }
    int k = 0;
    if (numPathlines > 0)
    {
        for (j = 0; j < ncoord; j++)
        {
            i = intArray->GetValue(j);
            if (locator[i] != -1)
            {
                double w[4];
#if ((VTK_MAJOR_VERSION == 7) && (VTK_MINOR_VERSION <= 1) || (VTK_MAJOR_VERSION < 7))  //  https://vtk.org/doc/nightly/html/VTK-7-1-Changes.html
                doubleArray->GetTupleValue(j, w);
#else
                doubleArray->GetTypedTuple(j, w);
#endif
                coordinates[3 * k]     = w[0];
                coordinates[3 * k + 1] = w[1];
                coordinates[3 * k + 2] = w[2];
                if (backwards)
                {
                    scalarArrayTime[k] = tref - w[3];
                }
                else
                {
                    scalarArrayTime[k] = tref + w[3];
                }
                pointArray[locator[i]] = k;
                locator[i]++;
                k++;
            }
        }

        // Determine the maximum and minimum travel time for each pathline.
        double *MaxTimes  = new double[nlines];
        double *MinTimes  = new double[nlines];
        bool   *FoundTime = new bool[nlines];

        for (int index = 0; index < nlines; index++)
        {
            FoundTime[index] = 0;
        }
        k = 0;
        for (int PointIndex = 0; PointIndex < ncoord; PointIndex++)
        {
            int index = intArray->GetValue(PointIndex);
            if (locator[index] != -1)
            {
                if (FoundTime[index])
                {
                    if (MaxTimes[index] < scalarArrayTime[k])
                    {
                        MaxTimes[index] = scalarArrayTime[k];
                    }
                    if (MinTimes[index] > scalarArrayTime[k])
                    {
                        MinTimes[index] = scalarArrayTime[k];
                    }
                }
                else
                {
                    MaxTimes[index]  = scalarArrayTime[k];
                    MinTimes[index]  = scalarArrayTime[k];
                    FoundTime[index] = 1;
                }
                k++;
            }
        }

        minPositiveTime = 0;
        for (int index = 0; index < nlines; index++)
        {
            if (FoundTime[index] && (MinTimes[index] > 0))
            {
                if ((minPositiveTime == 0) || (MinTimes[index] < minPositiveTime))
                {
                    minPositiveTime = MinTimes[index];
                }
            }
        }

        // Store the maximum and minimum travel time for each
        // pathline in alternate scalar arrays.
        k = 0;
        for (int PointIndex = 0; PointIndex < ncoord; PointIndex++)
        {
            int index = intArray->GetValue(PointIndex);
            if (locator[index] != -1)
            {
                scalarArrayMaxTime[k] = MaxTimes[index];
                scalarArrayMinTime[k] = MinTimes[index];
                k++;
            }
        }

        delete[] FoundTime;
        delete[] MinTimes;
        delete[] MaxTimes;
    }

    delete[] size;
    delete[] locator;
    intArray->Delete();
    doubleArray->Delete();
    numCoordinates = k;
}