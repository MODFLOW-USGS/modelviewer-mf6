
#include "mvSaveCurrentDirectory.h"
#include "mvUtil.h"

#if defined(QT_GUI_LIB)
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QStringLiteral>
#endif

#include <cassert>

#if defined(_MSC_VER) 
#include <shlwapi.h>
#endif


mvSaveCurrentDirectory::mvSaveCurrentDirectory(void)
#if defined(_AFXDLL)
    : m_path(0)
#else
    : _path(QDir::currentPath())
#endif
{
#if defined(_AFXDLL)

    DWORD len = ::GetCurrentDirectory(0, nullptr);
    if (len)
    {
        m_path = new char[len + 1];
        if (::GetCurrentDirectory(len, m_path) == 0)
        {
            delete[] m_path;
            m_path = nullptr;
        }
    }

#endif
}

mvSaveCurrentDirectory::mvSaveCurrentDirectory(const char* lpszNewPath)
#if defined(_AFXDLL)
    : m_path(0)
#else
    : _path(QDir::currentPath())
#endif
{
#if defined(_AFXDLL)

    DWORD len = ::GetCurrentDirectory(0, nullptr);
    if (len)
    {
        m_path = new char[len + 1];
        if (::GetCurrentDirectory(len + 1, m_path) == 0)
        {
            delete[] m_path;
            m_path = nullptr;
        }
    }

#endif

    mvSaveCurrentDirectory::changeDir(GetDirName(lpszNewPath).c_str());
}

mvSaveCurrentDirectory::~mvSaveCurrentDirectory(void)
{
#if defined(_AFXDLL)

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

#else

    if (_path.length())
    {
        bool b = QDir::setCurrent(_path);
        assert(b);
    }

#endif
}

bool mvSaveCurrentDirectory::changeDir(const char* lpPathName)
{
#if defined(_AFXDLL)
    bool bRet = false;
    if (lpPathName)
    {
        //CString strPathName(lpPathName);

        DWORD len     = ::GetCurrentDirectory(0, nullptr);
        char* current = new char[len + 1];
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

#else

    assert(QDir(lpPathName).exists());
    return QDir::setCurrent(QString(lpPathName));

#endif
}

std::string mvSaveCurrentDirectory::GetDirName(const char* fullPath)
{
#if defined(_AFXDLL)

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

#else
    // this always returns paths with forward slashes
    QString   path(fullPath);
    path = path.trimmed();

    QFileInfo fileInfo(path);
    QString   absolutePath = QDir::cleanPath(fileInfo.canonicalPath());

    Q_ASSERT(QDir(absolutePath).exists());
    return absolutePath.toStdString();
#endif
}

// Assumes pszFrom and pszTo are both files (as opposed to either of them being directories)
// and that they both exist similar to the following on Windows:
// PathRelativePathTo(szOut, pszFrom, FILE_ATTRIBUTE_NORMAL, cpTo.c_str(), FILE_ATTRIBUTE_NORMAL)
std::string mvSaveCurrentDirectory::GetRelativePath(const char* pszFrom, const char* pszTo)
{
#if defined(_AFXDLL)

    char szOut[MAX_PATH + 1] = "";
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

#else
    Q_ASSERT(pszFrom);
    QFileInfo fiFrom = QFileInfo(pszFrom);
    Q_ASSERT(fiFrom.exists() && fiFrom.isFile());
    if (pszTo && strlen(pszTo))
    {
        QFileInfo fiTo = QFileInfo(pszTo);
        Q_ASSERT(fiTo.exists() && fiTo.isFile());
    }

    QFileInfo i   = QFileInfo(pszFrom);
    QDir      dir = i.absoluteDir();
    QString   std = dir.absolutePath();
    Q_ASSERT(dir.exists());

    QString s  = dir.relativeFilePath(pszTo);
    QString ss = QDir::fromNativeSeparators(s);
    return ss.toStdString();
#endif
}

std::string mvSaveCurrentDirectory::GetFullPath(const char* szMore, const char* szDirectory)
{
#if defined(_AFXDLL)
    if (PathIsRelative(szMore))
    {
        assert(szDirectory);
        assert(PathIsDirectory(szDirectory));

        char buffer[MAX_PATH + 1];
        strcpy(buffer, szDirectory);
        PathAddBackslash(buffer);
        PathAppend(buffer, szMore);

        char canon[MAX_PATH + 1];
        PathCanonicalize(canon, buffer);

        return std::string(canon);
    }
    return std::string(szMore);
#else
    // this always returns paths with forward slashes
    if (QDir::isRelativePath(QString(szMore)))
    {
        Q_ASSERT(szDirectory);
        Q_ASSERT(QDir(szDirectory).exists());    

        QDir      dir(szDirectory);
        QString   abs = dir.absoluteFilePath(szMore);

        QFileInfo fileInfo(abs);
        abs = fileInfo.canonicalFilePath();

        return abs.toStdString();
    }
    std::string more = QDir::fromNativeSeparators(szMore).toStdString();
    return more;
#endif
}
