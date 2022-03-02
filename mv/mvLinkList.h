#ifndef __mvLinkList_h
#define __mvLinkList_h

#include "mvHeader.h"

struct linkListItem
{
    void         *item;
    linkListItem *next;
};

class MV_EXPORT mvLinkList
{
public:
    mvLinkList();
    ~mvLinkList();

    void  AddItem(void *item);
    void  InitTraversal();
    int   GetNumberOfItems();
    void *GetNextItem();

protected:
    int           m_NumberOfItems;
    int           m_Init;
    linkListItem *m_First;
    linkListItem *m_Last;
    linkListItem *m_Pointer;
};

#endif
