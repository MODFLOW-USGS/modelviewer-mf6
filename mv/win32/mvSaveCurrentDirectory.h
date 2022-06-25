#ifndef __mvSaveCurrentDirectory_h
#define __mvSaveCurrentDirectory_h

class mvSaveCurrentDirectory
{
public:
    mvSaveCurrentDirectory(void);
    mvSaveCurrentDirectory(const char* szNewPath);
    ~mvSaveCurrentDirectory(void);

    bool changeDir(const char* szPathName);

    static std::string GetDirName(const char* fullPath);
    static std::string GetFullPath(const char* szMore, const char* szDirectory);
    static std::string GetRelativePath(const char* pszFrom, const char* pszTo);

protected:
    char* m_path;
};

#endif
