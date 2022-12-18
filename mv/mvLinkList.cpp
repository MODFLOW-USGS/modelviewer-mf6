#include "mvLinkList.h"

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvLinkList::mvLinkList()
{
    m_NumberOfItems = 0;
    m_First         = 0;
    m_Last          = 0;
    m_Init          = 0;
    m_Pointer       = 0;
}

mvLinkList::~mvLinkList()
{
    linkListItem *d = m_First;
    while (d)
    {
        linkListItem *nd = d->next;
        delete[] d->item;
        delete d;
        d = nd;
    }
}

void mvLinkList::AddItem(void *item)
{
    if (!m_First)
    {
        m_First = new linkListItem;
        m_Last  = m_First;
    }
    else
    {
        m_Last->next = new linkListItem;
        m_Last       = m_Last->next;
    }
    m_Last->item = item;
    m_Last->next = 0;
    m_NumberOfItems++;
}

void mvLinkList::InitTraversal()
{
    m_Init = 1;
}

int mvLinkList::GetNumberOfItems()
{
    return m_NumberOfItems;
}

void *mvLinkList::GetNextItem()
{
    if (m_Init)
    {
        m_Pointer = m_First;
        m_Init    = 0;
        return m_Pointer->item;
    }
    else if (m_Pointer == 0)
    {
        return 0;
    }
    else
    {
        m_Pointer = m_Pointer->next;
        if (m_Pointer)
        {
            return m_Pointer->item;
        }
        else
        {
            return 0;
        }
    }
}
