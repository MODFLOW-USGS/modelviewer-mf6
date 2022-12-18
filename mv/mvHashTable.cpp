#include "mvHashTable.h"
#include "mvUtil.h"
#include <string.h>
#include <stdlib.h>

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvHashTable::mvHashTable()
{
    for (int i = 0; i < 64; i++)
    {
        m_Nodes[i] = 0;
    }
}

mvHashTable::~mvHashTable()
{
    mvHashNode *pos;
    mvHashNode *nextpos;

    for (int i = 0; i < 64; i++)
    {
        pos = m_Nodes[i];
        while (pos)
        {
            delete pos->key;
            delete pos->value;
            nextpos = pos->next;
            delete pos;
            pos = nextpos;
        }
    }
}

void mvHashTable::AddHashEntry(const char *key, const char *value)
{
    mvHashNode *pos;
    mvHashNode *newpos;
    int         loc;

    newpos      = new mvHashNode;
    newpos->key = new char[strlen(key) + 1];
    strcpy(newpos->key, key);
    newpos->value = new char[strlen(value) + 1];
    strcpy(newpos->value, value);
    newpos->next = 0;

    loc          = HashFunction(key);

    pos          = m_Nodes[loc];
    if (!pos)
    {
        m_Nodes[loc] = newpos;
        return;
    }
    while (pos->next)
    {
        pos = pos->next;
    }
    pos->next = newpos;
}

char *mvHashTable::GetHashTableValue(const char *key) const
{
    mvHashNode *pos;
    int         loc = HashFunction(key);

    pos             = m_Nodes[loc];

    if (!pos)
    {
        return 0;
    }
    while ((pos) && (mvUtil::stricmp(pos->key, key) != 0))
    {
        pos = pos->next;
    }
    if (pos)
    {
        return pos->value;
    }
    return 0;
}

int mvHashTable::GetHashTableValue(const char *key, double &v) const
{
    char *str = GetHashTableValue(key);
    if (!str)
    {
        return 0;
    }
    v = atof(str);
    return 1;
}

int mvHashTable::GetHashTableValue(const char *key, int &v) const
{
    char *str = GetHashTableValue(key);
    if (!str)
    {
        return 0;
    }
    v = atoi(str);
    return 1;
}

int mvHashTable::GetHashTableValue(const char *key, char *v) const
{
    char *str = GetHashTableValue(key);
    if (!str)
    {
        return 0;
    }
    strcpy(v, str);
    return 1;
}

int mvHashTable::HashFunction(const char *str) const
{
    unsigned int hashval;

    /* we start our hash out at 0 */
    hashval = 0;

    /* for each character, we multiply the old hash by 31 and add the current
     * character.  Remember that shifting a number left is equivalent to
     * multiplying it by 2 raised to the number of places shifted.  So we
     * are in effect multiplying hashval by 32 and then subtracting hashval.
     * Why do we do this?  Because shifting and subtraction are much more
     * efficient operations than multiplication.
     */
    for (; *str != '\0'; str++)
        hashval = *str + (hashval << 5) - hashval;

    /* we then return the hash value mod the hashtable size so that it will
     * fit into the necessary range
     */
    return hashval % 64;
}