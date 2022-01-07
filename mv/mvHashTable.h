#ifndef __mvHashTable_h
#define __mvHashTable_h

class mvHashNode 
{
public:
  mvHashNode *next;
  char *key;
  char *value;
};

class mvHashTable
{
public:
  mvHashTable();
  ~mvHashTable();
  mvHashNode *(m_Nodes[64]);
  void AddHashEntry(char *key, char *value);
  int GetHashTableValue(char *key, double &v);
  int GetHashTableValue(char *key, int &v);
  int GetHashTableValue(char *key, char *v);

protected:
  char *GetHashTableValue(char *key);
  int HashFunction(char *str);

};

#endif