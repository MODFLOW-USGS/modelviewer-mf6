#ifndef __mvDxfReader_h
#define __mvDxfReader_h

#include "mvHeader.h"
#include <stdio.h>

class mvLinkList;

class MV_EXPORT mvDxfReader
{
public:
    mvDxfReader();
    ~mvDxfReader();

    void SetFileName(char *filename);
    int  Read(const char *errMsg);
    int  GetNumberOfEntities();
    int  GetNumberOfVerticesInEntity(int i);
    int  GetCoordinates(int entity, int vertex, double &x, double &y);

protected:
    char    *m_FileName;
    double **m_XCoord;
    double **m_YCoord;
    int     *m_NumberOfVerticesInEntity;
    int      m_NumberOfEntities;

    void     ClearData();

    FILE    *fdxf;
    char   **layer_name;
    int     *layer_color;

    int      num_layers;

    int      find_entity(void);
    void     get_layer_info(void);
    int      get_layer(char *lname);
    void     process_line(mvLinkList *lines);
    void     process_poly(mvLinkList *lines);
    void     process_lwpoly(mvLinkList *lines);
    void     process_text(void);
    void     process_circle(mvLinkList *lines);
    void     process_arc(mvLinkList *lines);
    void     process_none(void);
    int      ReadValue(FILE *f1, int *ival);
    int      ReadValue(FILE *f1, double *val);
};

#endif
