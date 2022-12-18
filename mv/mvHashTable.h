#ifndef __mvHashTable_h
#define __mvHashTable_h

class mvHashNode
{
public:
    mvHashNode *next;
    char       *key;
    char       *value;
};

class mvHashTable
{
public:
    mvHashTable();
    ~mvHashTable();
    mvHashNode *(m_Nodes[64]);
    void AddHashEntry(const char *key, const char *value);
    int  GetHashTableValue(const char *key, double &v) const;
    int  GetHashTableValue(const char *key, int &v) const;
    int  GetHashTableValue(const char *key, char *v) const;

protected:
    char *GetHashTableValue(const char *key) const;
    int   HashFunction(const char *str) const;
};

#endif
