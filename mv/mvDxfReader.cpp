#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mvDxfReader.h"
#include "mvLinkList.h"
#include "mvUtil.h"

const double PI = 3.14159265;

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvDxfReader::mvDxfReader()
{
    m_FileName                 = 0;
    m_XCoord                   = 0;
    m_YCoord                   = 0;
    m_NumberOfVerticesInEntity = 0;
    m_NumberOfEntities         = 0;
    layer_name                 = 0;
    layer_color                = 0;
}

mvDxfReader::~mvDxfReader()
{
    if (m_FileName)
    {
        delete[] m_FileName;
    }
    ClearData();
}

void mvDxfReader::ClearData()
{
    int i;
    if (m_XCoord)
    {
        for (i = 0; i < m_NumberOfEntities; i++)
        {
            if (m_XCoord[i]) delete[] m_XCoord[i];
        }
        delete[] m_XCoord;
        m_XCoord = 0;
    }
    if (m_YCoord)
    {
        for (i = 0; i < m_NumberOfEntities; i++)
        {
            if (m_YCoord[i]) delete[] m_YCoord[i];
        }
        delete[] m_YCoord;
        m_YCoord = 0;
    }
    if (m_NumberOfVerticesInEntity)
    {
        delete[] m_NumberOfVerticesInEntity;
        m_NumberOfVerticesInEntity = 0;
    }
    m_NumberOfEntities = 0;
}

void mvDxfReader::SetFileName(char *filename)
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
}

int mvDxfReader::GetNumberOfEntities()
{
    return m_NumberOfEntities;
}

int mvDxfReader::GetNumberOfVerticesInEntity(int i)
{
    if (!m_NumberOfVerticesInEntity || i > m_NumberOfEntities)
    {
        return 0;
    }
    else
    {
        return m_NumberOfVerticesInEntity[i];
    }
}

int mvDxfReader::GetCoordinates(int entity, int vertex, double &x, double &y)
{
    if (entity >= m_NumberOfEntities)
    {
        return 0;
    }
    if (!m_NumberOfVerticesInEntity || vertex >= m_NumberOfVerticesInEntity[entity])
    {
        return 0;
    }
    if (!m_XCoord || !m_YCoord || !m_XCoord[entity] || !m_YCoord[entity])
    {
        return 0;
    }
    x = m_XCoord[entity][vertex];
    y = m_YCoord[entity][vertex];
    return 1;
}

int mvDxfReader::Read(const char *errMsg)
{
    double Xoff = 0.0;
    double Yoff = 0.0;
    int    i;
    int    test, code;
    char   buffer[1024];
    num_layers = 0;
    errMsg     = nullptr;
    ClearData();

    // Open DXF file
    if ((fdxf = fopen(m_FileName, "r")) == 0)
    {
        errMsg = "Unable to open DXF file.";
        return 0;
    }

    // Get Layer Colors
    get_layer_info();

    // Read the x and y offsets
    rewind(fdxf);
    int found = 0;
    do
    {
        if (fgets(buffer, 1024, fdxf) == nullptr) break;
        if (mvUtil::stricmp(buffer, "$ucsorg\n") == 0)
        {
            for (;;)
            {
                test = ReadValue(fdxf, &code);
                if (code == 10)
                {
                    ReadValue(fdxf, &Xoff);
                }
                else if (code == 20)
                {
                    ReadValue(fdxf, &Yoff);
                }
                else if (code == 9)
                {
                    break;
                }
                else
                {
                    char* s = fgets(buffer, 1024, fdxf);
                    assert(s);
                }
            }
            found = 1;
        }
    } while (!found);

    //  Find Beginning of Entity Section
    rewind(fdxf);
    if (!find_entity())
    {
        fclose(fdxf);
        errMsg = "No entities in DXF file.";
        return 0;
    }

    // Process Entity Data
    mvLinkList *lines = new mvLinkList;
    do
    {
        if (fgets(buffer, 1024, fdxf) == nullptr)
        {
            break;
        }
        if (!mvUtil::stricmp(buffer, "polyline\n"))
        {
            process_poly(lines);
        }
        else if (!mvUtil::stricmp(buffer, "lwpolyline\n"))
        {
            process_lwpoly(lines);
        }
        else if (!mvUtil::stricmp(buffer, "line\n"))
        {
            process_line(lines);
        }
        else if (!mvUtil::stricmp(buffer, "text\n"))
        {
            process_text();
        }
        else if (!mvUtil::stricmp(buffer, "circle\n"))
        {
            process_circle(lines);
        }
        else if (!mvUtil::stricmp(buffer, "arc\n"))
        {
            process_arc(lines);
        }
        else if (!mvUtil::stricmp(buffer, "endsec\n"))
        {
            break;
        }
        else
        {
            process_none();
        }
    } while (1);
    fclose(fdxf);

    // Extract coordinates
    m_NumberOfEntities         = lines->GetNumberOfItems();
    m_XCoord                   = new double *[m_NumberOfEntities];
    m_YCoord                   = new double *[m_NumberOfEntities];
    m_NumberOfVerticesInEntity = new int[m_NumberOfEntities];
    for (i = 0; i < m_NumberOfEntities; i++)
    {
        m_XCoord[i]                   = 0;
        m_YCoord[i]                   = 0;
        m_NumberOfVerticesInEntity[i] = 0;
    }
    lines->InitTraversal();
    for (i = 0; i < m_NumberOfEntities; i++)
    {
        mvLinkList *aLine             = (mvLinkList *)lines->GetNextItem();
        int         nv                = aLine->GetNumberOfItems();
        m_NumberOfVerticesInEntity[i] = nv;
        m_XCoord[i]                   = new double[nv];
        m_YCoord[i]                   = new double[nv];
        aLine->InitTraversal();
        for (int j = 0; j < nv; j++)
        {
            double *u      = (double *)aLine->GetNextItem();
            m_XCoord[i][j] = u[0] + Xoff;
            m_YCoord[i][j] = u[1] + Yoff;
            delete[] u;
        }
        delete aLine;
    }
    delete lines;
    lines = 0;

    for (i = 0; i < num_layers; i++)
    {
        delete[] layer_name[i];
    }
    delete[] layer_name;
    layer_name = 0;
    delete[] layer_color;
    layer_color = 0;

    return 1;
}
/***********************************************************************/
/*  get_layer_info  ..... The TABLES Section of a DXF file contains    */
/*                        information about LAYERS, such as the color. */
/***********************************************************************/

void mvDxfReader::get_layer_info(void)
{
    int         test;
    int         code;
    char        buffer[1024];

    mvLinkList *names  = new mvLinkList;
    mvLinkList *colors = new mvLinkList;

    do
    {
        if (fgets(buffer, 1024, fdxf) == nullptr) goto ProcessLayers;
        if (mvUtil::stricmp(buffer, "tables\n") == 0)
        {
            do
            {
                if (fgets(buffer, 1024, fdxf) == nullptr) goto ProcessLayers;
                if (mvUtil::stricmp(buffer, "layer\n") == 0)
                {
                    for (;;)
                    {
                        test = ReadValue(fdxf, &code);
                        if (code == 2)
                        {
                            char *name = new char[1024];
                            char *s    = fgets(name, 1024, fdxf);
                            assert(s);
                            if (s != nullptr) names->AddItem(name);
                        }
                        else if (code == 62)
                        {
                            int *color = new int;
                            ReadValue(fdxf, color);
                            colors->AddItem(color);
                        }
                        else if (code == 0)
                        {
                            break;
                        }
                        else
                        {
                            char* s = fgets(buffer, 1024, fdxf);
                            assert(s != nullptr);
                        }
                    }
                }
                else if (mvUtil::stricmp(buffer, "endsec\n") == 0)
                {
                    goto ProcessLayers;
                }
            } while (1);
        }
    } while (1);

ProcessLayers:
    num_layers = names->GetNumberOfItems();
    if (num_layers == 0)
    {
        delete names;
        delete colors;
        return;
    }
    int nnc     = colors->GetNumberOfItems();
    layer_name  = new char *[num_layers];
    layer_color = new int[num_layers];
    names->InitTraversal();
    colors->InitTraversal();
    for (int i = 0; i < num_layers; i++)
    {
        layer_name[i]  = (char *)names->GetNextItem();
        int *c         = (int *)colors->GetNextItem();
        layer_color[i] = *c;
        delete c;
    }
    delete names;
    delete colors;
}

/***********************************************************************/
/*  get_layer     ..... Find color for layer.                          */
/***********************************************************************/
int mvDxfReader::get_layer(char *lname)
{
    int i;

    for (i = 0; i < num_layers; i++)
    {
        if (mvUtil::stricmp(layer_name[i], lname) == 0)
        {
            return layer_color[i];
        }
    }
    return -1;
}

/***********************************************************************/
/*  find_entity     ..... Find the beginning of the ENTITY section of  */
/*                        the DXF file.                                */
/***********************************************************************/
int mvDxfReader::find_entity(void)
{
    int  test;
    char buffer[1024];

    do
    {
        if (fgets(buffer, 1024, fdxf) == nullptr)
        {
            test = feof(fdxf);
            if (test)
            { /*----- END OF FILE, START OVER -----*/
                fclose(fdxf);
                if ((fdxf = fopen(m_FileName, "r")) == 0)
                {
                    return 0;
                }
                do
                {
                    if (fgets(buffer, 1024, fdxf) == nullptr)
                    {
                        return 0;
                    }
                    if (mvUtil::stricmp(buffer, "entities\n") == 0)
                    {
                        char* s = fgets(buffer, 1024, fdxf);
                        assert(s);
                        return 1;
                    }
                } while (1);
            }
            else
            {
                return 0;
            }
        }
        if (mvUtil::stricmp(buffer, "entities\n") == 0)
        {
            char *s = fgets(buffer, 1024, fdxf);
            assert(s);
            return 1;
        }
    } while (1);
    return 1;
}

/***********************************************************************/
/*  process_line     ..... Read critical data about the line entity    */
/***********************************************************************/
void mvDxfReader::process_line(mvLinkList *lines)
{
    int    test;
    int    code;
    int    color;
    char   buffer2[1024];
    double x1, y1;
    double x2, y2;
    char * s;

    color             = 1;

    mvLinkList *aline = new mvLinkList;
    lines->AddItem(aline);

    do
    {
        test = ReadValue(fdxf, &code);

        switch (code)
        {
        case 8: /*----- layer name -----*/
            s = fgets(buffer2, 1024, fdxf);
            assert(s);
            color = get_layer(buffer2);
            break;
        case 10:
            test = ReadValue(fdxf, &x1);
            break;
        case 20:
            test = ReadValue(fdxf, &y1);
            break;
        case 11:
            test = ReadValue(fdxf, &x2);
            break;
        case 21:
            test = ReadValue(fdxf, &y2);
            break;
        case 62:
            test = ReadValue(fdxf, &color);
            break;
        case 0:
            break;
        default:
            s = fgets(buffer2, 1024, fdxf);
            assert(s);
            break;
        }
    } while (code != 0);
    double *c = new double[2];
    c[0]      = x1;
    c[1]      = y1;
    aline->AddItem(c);
    c    = new double[2];
    c[0] = x2;
    c[1] = y2;
    aline->AddItem(c);
}

/***********************************************************************/
/*  process_poly     ..... Read critical data about the poly entity    */
/***********************************************************************/
void mvDxfReader::process_poly(mvLinkList *lines)
{
    int    test;
    int    code;
    int    temp, spline, quit, closed;
    int    vert_num;
    int    color;
    double bulge;
    double xstart, ystart;
    double zz;
    char   buffer2[1024];
    double x1, y1;
    double x2, y2;
    char * s;

    color             = 1;
    quit              = 0;
    spline            = 0;
    closed            = 0;
    int         found = 0;

    mvLinkList *aline = new mvLinkList;
    lines->AddItem(aline);

    do
    {
        test = ReadValue(fdxf, &code);

        switch (code)
        {
        case 8: /*----- layer name -----*/
            s = fgets(buffer2, 1024, fdxf);
            assert(s);
            color = get_layer(buffer2);
            break;

        case 30:
            test  = ReadValue(fdxf, &zz);
            found = 1;
            break;

        case 62:
            test = ReadValue(fdxf, &color);
            break;

        case 70:
            test = ReadValue(fdxf, &temp); /*-- Check for Splines --*/
            if (temp == 4) spline = 1;
            if (temp == 1) closed = 1;
            break;

        case 0:
            break;
        default:
            s = fgets(buffer2, 1024, fdxf);
            assert(s);
            break;
        }
    } while (code != 0);

    /*----- process vertices of polyline ----*/
    vert_num = 0;
    do
    {
        s = fgets(buffer2, 1024, fdxf);
        assert(s);
        if (strstr(mvUtil::strlwr(buffer2), "vertex"))
        {
            vert_num++;
            do
            {
                test = ReadValue(fdxf, &code);
                switch (code)
                {
                case 8: /*----- layer name -----*/
                    s = fgets(buffer2, 1024, fdxf);
                    assert(s);
                    // if(strstr(buffer2,"HARD") != NULL) found = 1;
                    // if(strstr(buffer2,"TOP") != NULL) found = 1;
                    /*                       color = get_layer(buffer2);*/
                    break;
                case 10:
                    test = ReadValue(fdxf, &x2);
                    break;
                case 20:
                    test = ReadValue(fdxf, &y2);
                    break;
                case 38:
                    test  = ReadValue(fdxf, &zz);
                    found = 1;
                    break;
                case 42:
                    test = ReadValue(fdxf, &bulge); /* Not processing */
                    break;                          /* Bulge Data Yet */
                case 62:
                    test = ReadValue(fdxf, &temp);
                    break;
                case 70:
                    test = ReadValue(fdxf, &temp);
                    if (spline == 1 && temp == 16 && vert_num == 2)
                        quit = 1;
                    break;
                case 0:
                    break;
                default:
                    s = fgets(buffer2, 1024, fdxf);
                    assert(s);
                    break;
                }
            } while (code != 0);
            if (color <= 0 || color > 1000) color = 1;
            if (vert_num == 1)
            {
                xstart = x2;
                ystart = y2;
            }
            if (vert_num == 2 && !quit)
            {
                vert_num = 1;
                x1       = x2;
                y1       = y2;
            }
            else
            {
                x1 = x2;
                y1 = y2;
            }
            double *c = new double[2];
            c[0]      = x2;
            c[1]      = y2;
            aline->AddItem(c);
        }
        else
        {
            do
            {
                test = ReadValue(fdxf, &code);
                switch (code)
                {
                case 8: /*----- layer name -----*/
                    s = fgets(buffer2, 1024, fdxf);
                    assert(s);
                    color = get_layer(buffer2);
                    if (color <= 0) color = 1;
                    break;
                case 0:
                    break;
                default:
                    s = fgets(buffer2, 1024, fdxf);
                    assert(s);
                    break;
                }
            } while (code != 0);
            if (color <= 0 || color > 1000) color = 1;
            if (closed)
            {
                double *c = new double[2];
                c[0]      = x2;
                c[1]      = y2;
                aline->AddItem(c);
            }
            return;
        }
    } while (1);
}
/*******************************************************************************/
/*  process_lwpoly     ..... Read critical data about the new lwpoly entity    */
/*******************************************************************************/
void mvDxfReader::process_lwpoly(mvLinkList *lines)
{
    int    test;
    int    code;
    int    temp, spline, quit, closed;
    int    vert_num, firstone;
    int    color;
    double xstart, ystart;
    double zz;
    char   buffer2[1024];
    double x1, y1;
    double x2, y2;
    char * s;

    color             = 1;
    quit              = 0;
    spline            = 0;
    closed            = 0;
    int found         = 0;
    vert_num          = 1;
    firstone          = 1;

    mvLinkList *aline = new mvLinkList;
    lines->AddItem(aline);

    do
    {
        test = ReadValue(fdxf, &code);

        switch (code)
        {
        case 8: /*----- layer name -----*/
            s = fgets(buffer2, 1024, fdxf);
            assert(s);
            color = get_layer(buffer2);
            break;

        case 10:
            test = ReadValue(fdxf, &x2);
            break;
        case 20:
            test = ReadValue(fdxf, &y2);
            break;
        case 30:
            test  = ReadValue(fdxf, &zz);
            found = 1;
            break;

        case 62:
            test = ReadValue(fdxf, &color);
            break;

        case 70:
            test = ReadValue(fdxf, &temp); /*-- Check for Splines --*/
            if (temp == 4) spline = 1;
            if (temp == 1) closed = 1;
            break;

        case 0:
            break;
        default:
            s = fgets(buffer2, 1024, fdxf);
            assert(s);
            break;
        }
        if (code == 20)
        { // Got the Y coordinate, write out line?
            if (firstone == 1)
            {
                firstone = 0;
                xstart   = x2;
                ystart   = y2;
                x1       = x2;
                y1       = y2;
            }
            else if (!firstone && !quit)
            {
                x1 = x2;
                y1 = y2;
            }
            double *c = new double[2];
            c[0]      = x2;
            c[1]      = y2;
            aline->AddItem(c);
        }
    } while (code != 0);

    if (closed)
    {
        double *c = new double[2];
        c[0]      = xstart;
        c[1]      = ystart;
        aline->AddItem(c);
    }
}
/***********************************************************************/
/*  process_text      ..... Read critical data about the text entity   */
/***********************************************************************/
void mvDxfReader::process_text(void)
{
    int    test;
    int    code;
    int    color;
    int    symbol;
    double angle;
    double height;
    char   buffer2[1024];
    double x1, y1;
    char * s;

    angle  = 0.0;
    symbol = -1;
    color  = 1;

    do
    {
        test = ReadValue(fdxf, &code);

        switch (code)
        {
        case 8: /*----- layer name -----*/
            s = fgets(buffer2, 1024, fdxf);
            assert(s);
            color = get_layer(buffer2);
            break;
        case 10:
            test = ReadValue(fdxf, &x1);
            break;
        case 20:
            test = ReadValue(fdxf, &y1);
            break;
        case 50:
            test = ReadValue(fdxf, &angle);
            break;
        case 40:
            test = ReadValue(fdxf, &height);
            break;
        case 62:
            test = ReadValue(fdxf, &color);
            break;
        case 1:
            s = fgets(buffer2, 1024, fdxf);
            assert(s);
            break;
        case 0:
            break;
        default:
            s = fgets(buffer2, 1024, fdxf);
            assert(s);
            break;
        }
    } while (code != 0);
}
/***********************************************************************/
/*  process_circle  ..... Read critical data about the circle entity   */
/***********************************************************************/
void mvDxfReader::process_circle(mvLinkList *lines)
{
    int    test;
    int    code;
    int    color;
    int    i;
    double angle;
    double cx, cy;
    double radius;
    char   buffer2[1024];
    double x1, y1;
    double x2, y2;
    char * s;

    radius = 0.0;
    angle  = 0.0;
    cx = cy           = 0.0;
    color             = 1;

    mvLinkList *aline = new mvLinkList;
    lines->AddItem(aline);

    do
    {
        test = ReadValue(fdxf, &code);

        switch (code)
        {
        case 8: /*----- layer name -----*/
            s = fgets(buffer2, 1024, fdxf);
            assert(s);
            color = get_layer(buffer2);
            break;
        case 10:
            test = ReadValue(fdxf, &cx);
            break;
        case 20:
            test = ReadValue(fdxf, &cy);
            break;
        case 40:
            test = ReadValue(fdxf, &radius);
            break;
        case 62:
            test = ReadValue(fdxf, &color);
            break;
        case 0:
            break;
        default:
            s = fgets(buffer2, 1024, fdxf);
            assert(s);
            break;
        }
    } while (code != 0);

    if (radius == 0.0) return;
    x1 = cx + (radius * cos(angle));
    y1 = cy + (radius * sin(angle));
    for (i = 0; i < 20; i++)
    {
        angle += 0.34906585; /*----- 1/20 degree in radians -----*/
        x2        = cx + (radius * cos(angle));
        y2        = cy + (radius * sin(angle));
        double *c = new double[2];
        c[0]      = x2;
        c[1]      = y2;
        aline->AddItem(c);
        x1 = x2;
        y1 = y2;
    }
}
/***********************************************************************/
/*  process_arc        ..... Read critical data about the arc entity   */
/***********************************************************************/
void mvDxfReader::process_arc(mvLinkList *lines)
{
    int    test;
    int    code;
    int    color;
    int    num, i;
    double cx, cy;
    double radius;
    double radians, t360, t2;
    double astart, aend;
    double angle;
    char   buffer2[1024];
    double x1, y1;
    double x2, y2;
    char * s;

    radius = 0.0;
    aend = astart = 0.0;
    angle         = 0.0;
    cx = cy           = 0.0;
    color             = 1;
    t2                = 2.0;
    t360              = 360.0;
    radians           = (PI * t2) / t360;

    mvLinkList *aline = new mvLinkList;
    lines->AddItem(aline);

    do
    {
        test = ReadValue(fdxf, &code);

        switch (code)
        {
        case 8: /*----- layer name -----*/
            s = fgets(buffer2, 1024, fdxf);
            assert(s);
            color = get_layer(buffer2);
            break;
        case 10:
            test = ReadValue(fdxf, &cx);
            break;
        case 20:
            test = ReadValue(fdxf, &cy);
            break;
        case 40:
            test = ReadValue(fdxf, &radius);
            break;
        case 50:
            test = ReadValue(fdxf, &astart);
            break;
        case 51:
            test = ReadValue(fdxf, &aend);
            break;
        case 62:
            test = ReadValue(fdxf, &color);
            break;
        case 0:
            break;
        default:
            s = fgets(buffer2, 1024, fdxf);
            assert(s);
            break;
        }
    } while (code != 0);

    if (radius == 0.0) return;
    if (aend == astart) return;

    if (aend == 0.0) aend = 360.0;
    if (astart > aend)
    {
        aend += 360.0;
    }

    num = (int)(fabs(astart - aend) / 20.0);
    if (num < 5) num = 5;
    angle = fabs(astart - aend) / ((double)num);
    astart *= radians;
    angle *= radians;
    x1 = cx + (radius * cos(astart));
    y1 = cy + (radius * sin(astart));
    if (color <= 0 || color > 1000) color = 1;
    for (i = 0; i < num; i++)
    {
        astart += angle; /*----- 1/20 degree in radians -----*/
        x2        = cx + (radius * cos(astart));
        y2        = cy + (radius * sin(astart));
        double *c = new double[2];
        c[0]      = x2;
        c[1]      = y2;
        aline->AddItem(c);
    }
}
/***********************************************************************/
/*  process_none      ..... Read critical data about the unknown entity*/
/***********************************************************************/
void mvDxfReader::process_none(void)
{
    int  test;
    int  code;
    char buffer3[275];
    char *s;

    do
    {
        test = ReadValue(fdxf, &code);
        switch (code)
        {
        case 0:
            break;
        default:
            s = fgets(buffer3, 274, fdxf);
            assert(s);
            break;
        }
    } while (code != 0);
}

int mvDxfReader::ReadValue(FILE *f1, int *ival)
{
    char field[80];
    if (!fgets(field, 80, f1))
    {
        return 0;
    }
    // Replace the last character, which should be the line feed character,
    // by the null character
    field[strlen(field) - 1] = '\0';
    mvUtil::TrimLeft(field);
    mvUtil::TrimRight(field);
    *ival = atoi(field);
    return 1;
}

int mvDxfReader::ReadValue(FILE *f1, double *val)
{
    char field[80];
    if (!fgets(field, 80, f1))
    {
        return 0;
    }
    // Replace the last character, which should be the line feed character,
    // by the null character
    field[strlen(field) - 1] = '\0';
    mvUtil::TrimLeft(field);
    mvUtil::TrimRight(field);
    *val = atof(field);
    return 1;
}
