#ifndef __mvSaveCurrentDirectory_h
#define __mvSaveCurrentDirectory_h

#include <QString>

#include "mvHeader.h"

class MV_EXPORT mvSaveCurrentDirectory
{
public:
    mvSaveCurrentDirectory(void);
    mvSaveCurrentDirectory(const char* szNewPath);
    ~mvSaveCurrentDirectory(void);

    bool               changeDir(const char* szPathName);

    static std::string GetDirName(const char* fullPath);
    static std::string GetFullPath(const char* szMore, const char* szDirectory);
    static std::string GetRelativePath(const char* pszFrom, const char* pszTo);

protected:

#if 0
    char*   m_path;
#else
    QString _path;
#endif
};

#endif
