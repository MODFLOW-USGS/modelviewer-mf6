#include "mvOverlay.h"
#include "mvUtil.h"
#include "shapefil.h"
#include "vtkPolyData.h"
#include "vtkClipPolyData.h"
#include "vtkPlane.h"
#include "vtkStructuredGrid.h"
#include "vtkStructuredGridGeometryFilter.h"
#include "vtkCellLocator.h"
#include "vtkCellArray.h"
#include "mvDxfReader.h"

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvOverlay::mvOverlay()
{
    m_FileName      = 0;
    m_NewFile       = 0;
    m_XAtGridOrigin = 0;
    m_YAtGridOrigin = 0;
    m_XMin          = 0;
    m_XMax          = 0;
    m_YMin          = 0;
    m_YMax          = 0;
    m_XCoord        = 0;
    m_YCoord        = 0;
    m_NumVert       = 0;
    m_NumEntities   = 0;
    m_Elevation     = 0;
    m_Angle         = 0;
    m_Scale         = 1.0;
    m_Drape         = 0;
    m_Trim          = 0;
    m_Crop          = 0;
    m_DrapeGap      = 0;
    m_Type          = MV_ESRI_SHAPEFILE_OVERLAY;
    m_Lines         = 0;
    m_ClipXMin      = vtkClipPolyData::New();
    m_PlaneXMin     = vtkPlane::New();
    m_PlaneXMin->SetNormal(1, 0, 0);
    m_ClipXMin->SetClipFunction(m_PlaneXMin);

    m_ClipXMax  = vtkClipPolyData::New();
    m_PlaneXMax = vtkPlane::New();
    m_PlaneXMax->SetNormal(-1, 0, 0);
    m_ClipXMax->SetClipFunction(m_PlaneXMax);
    m_ClipXMax->SetInputConnection(m_ClipXMin->GetOutputPort());

    m_ClipYMin  = vtkClipPolyData::New();
    m_PlaneYMin = vtkPlane::New();
    m_PlaneYMin->SetNormal(0, 1, 0);
    m_ClipYMin->SetClipFunction(m_PlaneYMin);
    m_ClipYMin->SetInputConnection(m_ClipXMax->GetOutputPort());

    m_ClipYMax  = vtkClipPolyData::New();
    m_PlaneYMax = vtkPlane::New();
    m_PlaneYMax->SetNormal(0, -1, 0);
    m_ClipYMax->SetClipFunction(m_PlaneYMax);
    m_ClipYMax->SetInputConnection(m_ClipYMin->GetOutputPort());
    m_FullGrid           = 0;
    m_Subgrid            = 0;
    m_SubgridIsActivated = 0;

    // The vertical cropping planes. By default, 2 cropping planes
    // are normal to the x axis, and 2 are normal to the y axis. However
    // they can be rotated.
    int i;
    for (i = 0; i < 4; i++)
    {
        m_Plane[i] = vtkPlane::New();
    }
    m_Plane[0]->SetNormal(-1, 0, 0);
    m_Plane[1]->SetNormal(1, 0, 0);
    m_Plane[2]->SetNormal(0, -1, 0);
    m_Plane[3]->SetNormal(0, 1, 0);

    // The cropper is a series of filters to crop the overlay.
    // Cropping means removal of portions that lie beyond
    // vertical cropping planes.
    for (i = 0; i < 4; i++)
    {
        m_Cropper[i] = vtkClipPolyData::New();
        m_Cropper[i]->SetClipFunction(m_Plane[i]);
        m_Cropper[i]->InsideOutOn();
    }
}

mvOverlay::~mvOverlay()
{
    if (m_FileName != 0)
    {
        delete[] m_FileName;
        m_FileName = 0;
    }
    ClearData();
    m_ClipXMin->Delete();
    m_ClipXMax->Delete();
    m_ClipYMin->Delete();
    m_ClipYMax->Delete();
    m_PlaneXMin->Delete();
    m_PlaneXMax->Delete();
    m_PlaneYMin->Delete();
    m_PlaneYMax->Delete();
    for (int i = 0; i < 4; i++)
    {
        m_Cropper[i]->Delete();
        m_Plane[i]->Delete();
    }
}

void mvOverlay::ClearData()
{
    int i;
    if (m_Lines)
    {
        m_Lines->Delete();
        m_Lines = 0;
    }
    if (m_XCoord)
    {
        for (i = 0; i < m_NumEntities; i++)
        {
            if (m_XCoord[i]) delete[] m_XCoord[i];
        }
        delete[] m_XCoord;
        m_XCoord = 0;
    }
    if (m_YCoord)
    {
        for (i = 0; i < m_NumEntities; i++)
        {
            if (m_YCoord[i]) delete[] m_YCoord[i];
        }
        delete[] m_YCoord;
        m_YCoord = 0;
    }
    if (m_NumVert)
    {
        delete[] m_NumVert;
        m_NumVert = 0;
    }
    m_XMin = 0;
    m_XMax = 0;
    m_YMin = 0;
    m_YMax = 0;
}

void mvOverlay::SetFileName(const char *filename)
{
    if (!filename)
    {
        if (m_FileName)
        {
            delete[] m_FileName;
        }
        m_FileName = 0;
        return;
    }
    if (m_FileName != 0)
    {
        if (mvUtil::stricmp(m_FileName, filename) == 0)
        {
            return;
        }
        delete[] m_FileName;
    }
    m_FileName = new char[strlen(filename) + 1];
    strcpy(m_FileName, filename);
    m_NewFile = 1;
}

void mvOverlay::SetType(int type)
{
    if (m_Type == type)
    {
        return;
    }
    m_Type    = type;
    m_NewFile = 1;
}

void mvOverlay::SetCoordinatesAtGridOrigin(double xorig, double yorig)
{
    if (m_XAtGridOrigin == xorig && m_YAtGridOrigin == yorig)
    {
        return;
    }
    m_XAtGridOrigin = xorig;
    m_YAtGridOrigin = yorig;
}

void mvOverlay::SetAngle(double angle)
{
    if (m_Angle == angle)
    {
        return;
    }
    m_Angle = angle;
}

void mvOverlay::SetElevation(double elev)
{
    if (m_Elevation == elev)
    {
        return;
    }
    m_Elevation = elev;
}

void mvOverlay::SetOverlayToGridScale(double scale)
{
    m_Scale = scale;
}

void mvOverlay::SetDrape(int b)
{
    m_Drape = b;
}

void mvOverlay::SetTrim(int b)
{
    m_Trim = b;
}

void mvOverlay::SetCrop(int b)
{
    m_Crop = b;
}

void mvOverlay::SetDrapeGap(double gap)
{
    m_DrapeGap = gap;
}

void mvOverlay::SetSubgrid(vtkStructuredGrid *sg)
{
    m_Subgrid = sg;
}

void mvOverlay::SetFullGrid(vtkStructuredGrid *fg)
{
    m_FullGrid = fg;
}

int mvOverlay::Update(const char *errMsg)
{
    if (m_NewFile)
    {
        switch (m_Type)
        {
        case MV_ESRI_SHAPEFILE_OVERLAY:
            if (!ReadESRIShapeFile(errMsg))
            {
                return 0;
            }
            break;
        case MV_DXF_ASCII_OVERLAY:
            if (!ReadDxfFile(errMsg))
            {
                return 0;
            }
            break;
        default:
            errMsg = "Uncognized overlay file type.";
            return 0;
        }
    }
    Build();
    errMsg = nullptr;
    return 1;
}

int mvOverlay::HasData()
{
    return (m_Lines != 0);
}

void mvOverlay::SubgridOn()
{
    vtkAlgorithmOutput *previousAlgorithmOutput = 0;
    m_SubgridIsActivated                        = 1;
    if (m_Lines)
    {
        if (m_Trim && m_Subgrid)
        {
            double bounds[6];
            //##			m_Subgrid->Update();
            m_Subgrid->ComputeBounds();
            m_Subgrid->GetBounds(bounds);
            m_PlaneXMin->SetOrigin(bounds[0], 0, 0);
            m_PlaneXMax->SetOrigin(bounds[1], 0, 0);
            m_PlaneYMin->SetOrigin(0, bounds[2], 0);
            m_PlaneYMax->SetOrigin(0, bounds[3], 0);
            previousAlgorithmOutput = m_ClipYMax->GetOutputPort();
        }
        if (m_Crop)
        {
            if (previousAlgorithmOutput)
            {
                m_Cropper[0]->SetInputConnection(previousAlgorithmOutput);
            }
            else
            {
                m_Cropper[0]->SetInputData(m_Lines);
            }
            m_Cropper[1]->SetInputConnection(m_Cropper[0]->GetOutputPort());
            m_Cropper[2]->SetInputConnection(m_Cropper[1]->GetOutputPort());
            m_Cropper[3]->SetInputConnection(m_Cropper[2]->GetOutputPort());
            //			SetMapperInputData(m_Cropper[3]->GetOutput());
            SetMapperInputConnection(m_Cropper[3]->GetOutputPort());
        }
        else
        {
            if (previousAlgorithmOutput)
            {
                SetMapperInputConnection(previousAlgorithmOutput);
            }
            else
            {
                SetMapperInputData(m_Lines);
            }
        }
    }
}

void mvOverlay::SubgridOff()
{
    vtkAlgorithmOutput *previousAlgorithmOutput = 0;
    m_SubgridIsActivated                        = 0;
    if (m_Lines)
    {
        if (m_Trim && m_FullGrid)
        {
            double bounds[6];
            m_FullGrid->GetBounds(bounds);
            m_PlaneXMin->SetOrigin(bounds[0], 0, 0);
            m_PlaneXMax->SetOrigin(bounds[1], 0, 0);
            m_PlaneYMin->SetOrigin(0, bounds[2], 0);
            m_PlaneYMax->SetOrigin(0, bounds[3], 0);
            previousAlgorithmOutput = m_ClipYMax->GetOutputPort();
        }
        if (m_Crop)
        {
            if (previousAlgorithmOutput)
            {
                m_Cropper[0]->SetInputConnection(previousAlgorithmOutput);
            }
            else
            {
                m_Cropper[0]->SetInputData(m_Lines);
            }
            m_Cropper[1]->SetInputConnection(m_Cropper[0]->GetOutputPort());
            m_Cropper[2]->SetInputConnection(m_Cropper[1]->GetOutputPort());
            m_Cropper[3]->SetInputConnection(m_Cropper[2]->GetOutputPort());
            SetMapperInputData(m_Cropper[3]->GetOutput());
        }
        else
        {
            if (previousAlgorithmOutput)
            {
                SetMapperInputConnection(previousAlgorithmOutput);
            }
            else
            {
                SetMapperInputData(m_Lines);
            }
        }
    }
}

void mvOverlay::Build()
{
    double ec, es, x, y;
    ec = cos(m_Angle * 3.14159265358979 / 180.0);
    es = sin(m_Angle * 3.14159265358979 / 180.0);
    if (m_Lines)
    {
        m_Lines->Delete();
    }
    m_Lines              = vtkPolyData::New();
    vtkPoints    *points = vtkPoints::New();
    vtkCellArray *lines  = vtkCellArray::New();
    m_Lines->SetPoints(points);
    m_Lines->SetLines(lines);
    points->Delete();
    lines->Delete();
    m_ClipXMin->SetInputData(m_Lines);
    if (m_SubgridIsActivated)
    {
        SubgridOn();
    }
    else
    {
        SubgridOff();
    }
    SetColor(0, 0, 0);

    vtkStructuredGridGeometryFilter *geom    = 0;
    vtkCellLocator                  *locator = 0;
    double                           p1[3], p2[3], t, ptline[3], pcoords[3];
    int                              subId, i, j;
    if (m_Drape)
    {
        geom = vtkStructuredGridGeometryFilter::New();
        int dim[3];
        m_FullGrid->GetDimensions(dim);
        geom->SetInputData(m_FullGrid);
        geom->SetExtent(0, dim[0] - 1, 0, dim[1] - 1, dim[2] - 1, dim[2] - 1);
        geom->Update();
        double bounds[6];
        geom->GetOutput()->GetBounds(bounds);
        if (bounds[5] - bounds[4] != 0)
        {
            // Grid top is non planar
            locator = vtkCellLocator::New();
            locator->SetDataSet(geom->GetOutput());
            locator->BuildLocator();
            m_FullGrid->GetBounds(bounds);
            double thick = bounds[5] - bounds[4];
            p1[2]        = bounds[4] - thick;
            p2[2]        = bounds[5] + thick;
        }
        else
        {
            // Grid Top is planar
            p1[2] = bounds[4];
        }
    }

    int ip = 0;
    for (i = 0; i < m_NumEntities; i++)
    {
        vtkIdType *pts    = new vtkIdType[m_NumVert[i]];
        double    *xp     = new double[m_NumVert[i]];
        double    *yp     = new double[m_NumVert[i]];
        double    *zp     = new double[m_NumVert[i]];
        int       *inside = new int[m_NumVert[i]];
        int        keep   = 0;
        for (j = 0; j < m_NumVert[i]; j++)
        {
            if (i == 0 && j == 0)
            {
                m_XMin = m_XMax = m_XCoord[i][j];
                m_YMin = m_YMax = m_YCoord[i][j];
            }
            else
            {
                if (m_XCoord[i][j] < m_XMin)
                {
                    m_XMin = m_XCoord[i][j];
                }
                else if (m_XCoord[i][j] > m_XMax)
                {
                    m_XMax = m_XCoord[i][j];
                }
                if (m_YCoord[i][j] < m_YMin)
                {
                    m_YMin = m_YCoord[i][j];
                }
                else if (m_YCoord[i][j] > m_YMax)
                {
                    m_YMax = m_YCoord[i][j];
                }
            }

            x     = m_Scale * (m_XCoord[i][j] - m_XAtGridOrigin);
            y     = m_Scale * (m_YCoord[i][j] - m_YAtGridOrigin);
            xp[j] = x * ec + y * es;
            yp[j] = -x * es + y * ec;
            if (m_Drape)
            {
                if (locator)
                {
                    // project the point vertically to see if it intersects the
                    // top surface of the grid
                    p1[0]       = xp[j];
                    p1[1]       = yp[j];
                    p2[0]       = xp[j];
                    p2[1]       = yp[j];
                    int iresult = locator->IntersectWithLine(p1, p2, 0.001, t, ptline, pcoords, subId);
                    if (iresult)
                    {
                        // Point intersects the top surface of grid
                        zp[j]     = ptline[2];
                        inside[j] = 1;
                        keep      = 1;
                    }
                    else
                    {
                        // Point does not intersect top surface of grid
                        zp[j]     = 0;
                        inside[j] = 0;
                    }
                }
                else
                {
                    zp[j] = p1[2];
                    keep  = 1;
                }
            }
            else
            {
                zp[j] = m_Elevation;
                keep  = 1;
            }
        }
        if (keep)
        {
            int k, m1, m2;
            for (j = 0; j < m_NumVert[i]; j++)
            {
                if (m_Drape && locator && !inside[j])
                {
                    m1 = -1;
                    for (k = j - 1; k >= 0; k--)
                    {
                        if (inside[k])
                        {
                            m1 = k;
                            break;
                        }
                    }
                    m2 = -1;
                    for (k = j + 1; k < m_NumVert[i]; k++)
                    {
                        if (inside[k])
                        {
                            m2 = k;
                            break;
                        }
                    }
                    if (m1 == -1 && m2 != -1)
                    {
                        zp[j] = zp[m2];
                    }
                    else if (m2 == -1 && m1 != -1)
                    {
                        zp[j] = zp[m1];
                    }
                    else if (m1 != -1 && m2 != -1)
                    {
                        if (j - m1 <= m2 - j)
                        {
                            zp[j] = zp[m1];
                        }
                        else
                        {
                            zp[j] = zp[m2];
                        }
                    }
                    else
                    {
                        break; // should not happen
                    }
                }
                if (m_Drape)
                {
                    zp[j] += m_DrapeGap;
                }
                points->InsertNextPoint(xp[j], yp[j], zp[j]);
                pts[j] = ip;
                ip++;
            }

            lines->InsertNextCell(m_NumVert[i], pts);
        }
        delete[] pts;
        delete[] xp;
        delete[] yp;
        delete[] zp;
        delete[] inside;
    }

    if (geom)
    {
        geom->Delete();
    }
    if (locator)
    {
        locator->Delete();
    }
}

int mvOverlay::ReadESRIShapeFile(const char *errMsg)
{
    SHPHandle hSHP;
    int       nShapeType, nEntities, i, iPart;
    double    adfMinBound[4], adfMaxBound[4];

    hSHP = SHPOpen(m_FileName, "rb");

    if (!hSHP)
    {
        errMsg = "Cannot Open Shape File";
        return 0;
    }

    SHPGetInfo(hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound);
    if (nShapeType != SHPT_ARC && nShapeType != SHPT_POLYGON)
    {
        errMsg = "Shape file does not contain arcs or polygon";
        return 0;
    }
    // Note: It appears that if the shape file contains polygons, the last
    // vertex of each polygon coincides with the first vertex. Therefore,
    // for the purpose of vtk, polygon data can be treated in the same
    // way as polyline data, because the figure will be closed.
    ClearData();
    m_XCoord  = new double *[nEntities];
    m_YCoord  = new double *[nEntities];
    m_NumVert = new int[nEntities];
    for (i = 0; i < nEntities; i++)
    {
        m_XCoord[i]  = 0;
        m_YCoord[i]  = 0;
        m_NumVert[i] = 0;
    }
    for (i = 0; i < nEntities; i++)
    {
        int        j;
        SHPObject *psShape;

        psShape = SHPReadObject(hSHP, i);

        if (!psShape)
        {
            SHPClose(hSHP);
            ClearData();
            errMsg = "Error encountered while reading shape file";
            return 0;
        }

        m_NumVert[i] = psShape->nVertices;
        m_XCoord[i]  = new double[psShape->nVertices];
        m_YCoord[i]  = new double[psShape->nVertices];

        for (j = 0, iPart = 1; j < psShape->nVertices; j++)
        {
            const char *pszPartType = "";

            if (j == 0 && psShape->nParts > 0)
            {
                pszPartType = SHPPartTypeName(psShape->panPartType[0]);
            }

            if (iPart < psShape->nParts && psShape->panPartStart[iPart] == j)
            {
                pszPartType = SHPPartTypeName(psShape->panPartType[iPart]);
                iPart++;
            }

            m_XCoord[i][j] = psShape->padfX[j];
            m_YCoord[i][j] = psShape->padfY[j];
        }

        SHPDestroyObject(psShape);
    }

    SHPClose(hSHP);

    m_XMin        = adfMinBound[0];
    m_YMin        = adfMinBound[1];
    m_XMax        = adfMaxBound[0];
    m_YMax        = adfMaxBound[1];
    m_NumEntities = nEntities;
    m_NewFile     = 0;
    errMsg        = nullptr;
    return 1;
}

int mvOverlay::ReadDxfFile(const char *errMsg)
{
    int          i;
    mvDxfReader *dxf = new mvDxfReader;
    dxf->SetFileName(m_FileName);
    errMsg = nullptr;
    if (!dxf->Read(errMsg))
    {
        delete dxf;
        return 0;
    }
    m_NumEntities = dxf->GetNumberOfEntities();
    m_XCoord      = new double *[m_NumEntities];
    m_YCoord      = new double *[m_NumEntities];
    m_NumVert     = new int[m_NumEntities];
    for (i = 0; i < m_NumEntities; i++)
    {
        m_XCoord[i]  = 0;
        m_YCoord[i]  = 0;
        m_NumVert[i] = 0;
    }
    for (i = 0; i < m_NumEntities; i++)
    {
        m_NumVert[i] = dxf->GetNumberOfVerticesInEntity(i);
        m_XCoord[i]  = new double[m_NumVert[i]];
        m_YCoord[i]  = new double[m_NumVert[i]];
        for (int j = 0; j < m_NumVert[i]; j++)
        {
            dxf->GetCoordinates(i, j, m_XCoord[i][j], m_YCoord[i][j]);
        }
    }
    delete dxf;
    m_NewFile = 0;
    return 1;
}

void mvOverlay::SetCropperPlaneOrigin(int i, double x, double y, double z)
{
    m_Plane[i]->SetOrigin(x, y, z);
}

void mvOverlay::SetCropAngle(double cropAngle)
{
    double sn = sin(cropAngle * 1.745329e-2);
    double cs = cos(cropAngle * 1.745329e-2);
    m_Plane[0]->SetNormal(-cs, -sn, 0);
    m_Plane[1]->SetNormal(cs, sn, 0);
    m_Plane[2]->SetNormal(sn, -cs, 0);
    m_Plane[3]->SetNormal(-sn, cs, 0);
}
