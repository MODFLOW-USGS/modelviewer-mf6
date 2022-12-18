
#include "mvSaveCurrentDirectory.h"

// This must be below vtkStandardNewMacro
////#include <afx.h>
#include <shlwapi.h>
//#if defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

mvSaveCurrentDirectory::mvSaveCurrentDirectory(void)
    : m_path(0)
{
    DWORD len = ::GetCurrentDirectory(0, nullptr);
    if (len)
    {
        m_path = new char[len];
        if (::GetCurrentDirectory(len, m_path) == 0)
        {
            delete[] m_path;
            m_path = nullptr;
        }
    }
}

mvSaveCurrentDirectory::mvSaveCurrentDirectory(const char* lpszNewPath)
    : m_path(0)
{
    DWORD len = ::GetCurrentDirectory(0, nullptr);
    if (len)
    {
        m_path = new char[len];
        if (::GetCurrentDirectory(len, m_path) == 0)
        {
            delete[] m_path;
            m_path = nullptr;
        }
    }

    mvSaveCurrentDirectory::changeDir(GetDirName(lpszNewPath).c_str());
}

mvSaveCurrentDirectory::~mvSaveCurrentDirectory(void)
{
    if (m_path)
    {
        BOOL bRet = ::SetCurrentDirectory(m_path);
        if (bRet == 0)
        {
            //TRACE("::SetCurrentDirectory(%s) failed (%d)\n", m_path, ::GetLastError());
            //ASSERT(false);
            assert(false);
        }
        delete[] m_path;
    }
    m_path = nullptr;
}

bool mvSaveCurrentDirectory::changeDir(const char* lpPathName)
{
    bool bRet = false;
    if (lpPathName)
    {
        //CString strPathName(lpPathName);

        DWORD   len     = ::GetCurrentDirectory(0, nullptr);
        char * current = new char[len];
        if (current)
        {
            ::GetCurrentDirectory(len, current);
            if (::strcmp(current, lpPathName) != 0)
            {
                bRet = ::SetCurrentDirectory(lpPathName);
                if (bRet == 0)
                {
                    //TRACE("::SetCurrentDirectory failed (%d)\n", ::GetLastError());
                    assert(false);
                }
            }
            delete[] current;
        }
        else
        {
            bRet = ::SetCurrentDirectory(lpPathName);
            if (bRet == 0)
            {
                //TRACE("::SetCurrentDirectory failed (%d)\n", ::GetLastError());
                assert(false);
            }
        }
    }
    return bRet;
}

std::string mvSaveCurrentDirectory::GetDirName(const char* fullPath)
{
    char szOut[MAX_PATH];
    char szDrive[_MAX_DRIVE];
    char szDir[_MAX_DIR];
    char szDest[MAX_PATH];

    //ASSERT(!PathIsRelative(fullPath));
    //VERIFY(_tsplitpath_s(fullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, nullptr, 0, nullptr, 0) == 0);
    _splitpath_s(fullPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, nullptr, 0, nullptr, 0);

    char szPath[MAX_PATH];
    //VERIFY(_tmakepath_s(szPath, _MAX_DIR, szDrive, szDir, nullptr, nullptr) == 0);
    _makepath_s(szPath, _MAX_DIR, szDrive, szDir, nullptr, nullptr);

    return std::string(szPath);
}

std::string mvSaveCurrentDirectory::GetRelativePath(const char* pszFrom, const char* pszTo)
{
    char szOut[MAX_PATH] = "";
    if (strlen(pszTo) && PathIsSameRoot(pszFrom, pszTo))
    {
        std::string cpTo(pszTo);
        std::replace(cpTo.begin(), cpTo.end(), '/', '\\');
        //VERIFY(PathCanonicalize(szOut, cpTo.c_str()));
        PathCanonicalize(szOut, cpTo.c_str());
        cpTo = szOut;
        //VERIFY(PathRelativePathTo(szOut, pszFrom, FILE_ATTRIBUTE_NORMAL, cpTo.c_str(), FILE_ATTRIBUTE_NORMAL));
        PathRelativePathTo(szOut, pszFrom, FILE_ATTRIBUTE_NORMAL, cpTo.c_str(), FILE_ATTRIBUTE_NORMAL);
        return std::string(szOut);
    }
    return std::string(pszTo);
}

std::string mvSaveCurrentDirectory::GetFullPath(const char* szMore, const char* szDirectory)
{
    if (PathIsRelative(szMore))
    {
        assert(szDirectory);
        assert(PathIsDirectory(szDirectory));

        char buffer[MAX_PATH];
        strcpy(buffer, szDirectory);
        PathAddBackslash(buffer);
        PathAppend(buffer, szMore);

        char canon[MAX_PATH];
        PathCanonicalize(canon, buffer);

        return std::string(canon);
    }
    return std::string(szMore);
}
