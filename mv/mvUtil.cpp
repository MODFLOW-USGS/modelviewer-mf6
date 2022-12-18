#include "mvUtil.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

#if defined(QT_GUI_LIB)
#include <QDir>
#endif

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#if !defined(QT_GUI_LIB)
#if defined(_MSC_VER)
// This must be below <afx.h>
#include <shlwapi.h>
#endif
#endif

void mvUtil::interp2d(double *x, double *y, double *z, double *dx, double *dy, double *zc,
                      int numRow, int numCol, double znull)
{
    int     i, j, cellOffset, pointOffset;
    double  w, v;
    int     nx     = numCol + 1;
    int     ny     = numRow + 1;
    int     nxy    = nx * ny;

    // Set up arrays for interpolation
    double *value  = new double[nxy];
    double *weight = new double[nxy];
    for (i = 0; i < nxy; i++)
    {
        value[i]  = 0;
        weight[i] = 0;
    }

    // Do inverse distance interpolation
    for (j = 0; j < numRow; j++)
    {
        cellOffset  = j * numCol;
        pointOffset = j * nx;
        for (i = 0; i < numCol; i++)
        {
            if (zc[cellOffset + i] != znull)
            {
                w = 4 / sqrt(dx[i] * dx[i] + dy[j] * dy[j]);
                weight[pointOffset + i] += w;
                weight[pointOffset + i + 1] += w;
                weight[pointOffset + i + nx] += w;
                weight[pointOffset + i + nx + 1] += w;
                v = zc[cellOffset + i] * w;
                value[pointOffset + i] += v;
                value[pointOffset + i + 1] += v;
                value[pointOffset + i + nx] += v;
                value[pointOffset + i + nx + 1] += v;
            }
        }
    }
    for (i = 0; i < nxy; i++)
    {
        if (weight[i] > 0)
        {
            z[i] = value[i] / weight[i];
        }
        else
        {
            z[i] = znull;
        }
    }
    delete[] value;
    delete[] weight;
}

void mvUtil::interp3d(double *cellDraw, double *ptDraw, double *dx, double *dy, double *zc,
                      int numCol, int numRow, int numLay, double drawNull, double znull, int *useLayer)
{
    int     i, j, k, ic, jc, kc, ip, jp, kp;
    int     nx     = numCol + 1;
    int     ny     = numRow + 1;
    int     nz     = numLay + 1;
    int     nxy    = nx * ny;
    int     nxyz   = nx * ny * nz;
    int     ncr    = numCol * numRow;
    double *value  = new double[nxyz];
    double *weight = new double[nxyz];
    double  v, w, zz, yy, xx;

    for (i = 0; i < nxyz; i++)
    {
        value[i]  = 0;
        weight[i] = 0;
    }

    for (k = 0; k < numLay; k++) // count cell layer
    {
        if (useLayer == 0 || (useLayer && useLayer[k]))
        {
            kp = k * nxy;
            kc = k * ncr;
            for (j = 0; j < numRow; j++) // count cell row
            {
                jp = j * nx + kp;
                jc = j * numCol + kc;
                yy = dy[j] * dy[j];
                for (i = 0; i < numCol; i++)
                {
                    ip = jp + i;
                    ic = jc + i;
                    xx = dx[i] * dx[i];
                    if (zc[ic + ncr] == znull || zc[ic] == znull)
                    {
                        zz = 0;
                    }
                    else
                    {
                        zz = zc[ic + ncr] - zc[ic];
                    }
                    if (cellDraw[ic] != drawNull)
                    {
                        w = 2 / sqrt(xx + yy + zz);
                        weight[ip] += w;
                        weight[ip + 1] += w;
                        weight[ip + nx] += w;
                        weight[ip + nx + 1] += w;
                        weight[ip + nxy] += w;
                        weight[ip + nxy + 1] += w;
                        weight[ip + nxy + nx] += w;
                        weight[ip + nxy + nx + 1] += w;
                        v = w * cellDraw[ic];
                        value[ip] += v;
                        value[ip + 1] += v;
                        value[ip + nx] += v;
                        value[ip + nx + 1] += v;
                        value[ip + nxy] += v;
                        value[ip + nxy + 1] += v;
                        value[ip + nxy + nx] += v;
                        value[ip + nxy + nx + 1] += v;
                    }
                }
            }
        }
    }

    for (i = 0; i < nxyz; i++)
    {
        if (weight[i] != 0)
        {
            ptDraw[i] = value[i] / weight[i];
        }
        else
        {
            ptDraw[i] = 0;
        }
    }
    delete[] weight;
    delete[] value;
}

int mvUtil::ExtractNumber(ifstream *in, int *buffer)
{
    char line[1024];
    in->getline(line, 1024);
    char *code = strchr(line, '=');
    if (code == 0 || code[1] == '\0')
    {
        return 0;
    }
    *buffer = atoi(code + 2);
    return 1;
}

int mvUtil::ExtractNumber(ifstream *in, double *buffer)
{
    char line[1024];
    in->getline(line, 1024);
    char *code = strchr(line, '=');
    if (code == 0 || code[1] == '\0')
    {
        return 0;
    }
    *buffer = atof(code + 2);
    return 1;
}

int mvUtil::ExtractString(ifstream *in, char *buffer, int len)
{
    char line[1024];
    in->getline(line, 1024);
    char *code = strchr(line, '=');
    if (code == 0 || code[1] == '\0')
    {
        buffer[0] = '\0';
        return 0;
    }
    strncpy(buffer, code + 2, len);
    return 1;
}

int mvUtil::ExtractLabelAndValue(ifstream *in, char *label, char *value)
{
    char line[1024], buffer[1024];
    in->getline(line, 1024);
    char *code = strchr(line, '=');
    if (code == 0 || code[1] == '\0')
    {
        return 0;
    }
    strcpy(buffer, code + 1);
    TrimLeft(buffer);
    TrimRight(buffer);
    strcpy(value, buffer);
    *code = '\0';
    TrimRight(line);
    strcpy(label, line);
    return 1;
}

void mvUtil::TrimLeft(char *aString)
{
    if (aString[0] == '\0' || aString[0] != ' ')
    {
        return;
    }
    char *p = aString;
    while (*p == ' ')
    {
        p++;
        // if the end of the string is reached, then this string contains all spaces
        if (*p == '\0')
        {
            aString[0] = '\0';
            return;
        }
    }

    // shift characters forward
    size_t lp = strlen(p);
    for (size_t i = 0; i < lp; i++)
    {
        aString[i] = p[i];
    }
    aString[lp] = '\0';
}

void mvUtil::TrimRight(char *aString)
{
    size_t ls = strlen(aString);
    if (ls == 0 || aString[ls - 1] != ' ')
    {
        return;
    }
    char *p = aString + (ls - 1);
    while (*p == ' ')
    {
        p--;
        if (p == aString && *p == ' ')
        {
            aString[0] = '\0';
            return;
        }
    }
    *(p + 1) = '\0';
}

void mvUtil::ToLowerCase(char *aString)
{
    for (int i = 0; i < (int)strlen(aString); i++)
    {
        aString[i] = tolower(aString[i]);
    }
}

void mvUtil::ReplaceTabWithWhiteSpace(char *aString)
{
    for (int i = 0; i < (int)strlen(aString); i++)
    {
        if (aString[i] == '\t')
        {
            aString[i] = ' ';
        }
    }
}

double mvUtil::Fortran_atof(char *aString)
{
    TrimLeft(aString);
    TrimRight(aString);
    size_t StringLength = strlen(aString);
    for (size_t Index = StringLength - 1; Index >= 1; Index--)
    {
        if ((aString[Index] == '+') || (aString[Index] == '-'))
        {
            if ((aString[Index - 1] != 'e') && (aString[Index - 1] != 'E') && (aString[Index - 1] != 'd') && (aString[Index - 1] != 'D'))
            {
                std::string Source(aString);
                std::string E("E");
                std::string Destination = Source.substr(0, Index); // + E;//  + Source.substr(Index, StringLength-Index);
                Destination += E;
                Destination += Source.substr(Index, StringLength - Index);
                return atof(Destination.c_str());
            }
        }
    }
    return atof(aString);
}

void mvUtil::ExtractFirstString(char *aString, char *FirstString)
{
    TrimLeft(aString);
    size_t ls = strlen(aString);
    strcpy(FirstString, aString);
    for (size_t i = 0; i < ls; i++)
    {
        if (aString[i] == ' ')
        {
            aString[i]     = '\0';
            FirstString[i] = '\0';
            if (i < ls - 1)
            {
                strcpy(aString, aString + i + 1);
                // aString = aString + i+1;
            }
            else
            {
                aString[0] = '\0';
            }
            return;
        }
    }
    *aString = 0;
    return;
}

char *mvUtil::NextNonSpaceChar(char *str, int len)
{
    if (len <= 0)
    {
        return 0;
    }
    int   i = 0;
    char *p = str;
    while (i < len)
    {
        if (*p != ' ')
        {
            return p;
        }
        p++;
        i++;
    }
    return 0;
}

void mvUtil::Wavelength2RGBA(double wavelength, double *RGBA)
{
    RGBA[3] = 1;
    if (wavelength > 780 || wavelength < 380)
    {
        RGBA[0] = 0.5;
        RGBA[1] = 0.5;
        RGBA[2] = 1;
        return;
    }
    if (wavelength < 440)
    {
        RGBA[0] = -(wavelength - 440) / (440 - 380);
        RGBA[1] = 0.5;
        RGBA[2] = 0.5;
    }
    else if (wavelength < 490)
    {
        RGBA[0] = 0.5;
        RGBA[1] = (wavelength - 440) / (490 - 440);
        RGBA[2] = 1;
    }
    else if (wavelength < 510)
    {
        RGBA[0] = 0.5;
        RGBA[1] = 1;
        RGBA[2] = -(wavelength - 510) / (510 - 490);
    }
    else if (wavelength < 580)
    {
        RGBA[0] = (wavelength - 510) / (580 - 510);
        RGBA[1] = 1;
        RGBA[2] = 0.5;
    }
    else if (wavelength < 645)
    {
        RGBA[0] = 1;
        RGBA[1] = -(wavelength - 645) / (645 - 580);
        RGBA[2] = 0.5;
    }
    else
    {
        RGBA[0] = 1;
        RGBA[1] = 0.5;
        RGBA[2] = 0.5;
    }
}

void mvUtil::RemoveQuotes(char* aString)
{
    size_t len = strlen(aString);
    if (aString[0] == '\'' && aString[len - 1] == '\'')
    {
        aString[len - 1] = '\0';
        memmove(aString, aString + 1, len - 1);
    }
}

void mvUtil::RemoveDoubleQuotes(char* aString)
{
    size_t len = strlen(aString);
    if (aString[0] == '"' && aString[len - 1] == '"')
    {
        aString[len - 1] = '\0';
        memmove(aString, aString + 1, len - 1);
    }
}

int mvUtil::stricmp(const char *a, const char *b)
{
#if defined(_MSC_VER)
    return _stricmp(a, b);
#else
    int na;
    int nb;
    int result;
    assert(a != nullptr);
    assert(b != nullptr);
    do
    {
        na     = tolower(*a++);
        nb     = tolower(*b++);
        result = na - nb;
    } while (result == 0 && na != '\0');
    return result;
#endif
}

int mvUtil::strnicmp(const char *a, const char *b, size_t n)
{
#if defined(_MSC_VER)
    return _strnicmp(a, b, n);
#else
    int na;
    int nb;
    int result;
    if (n == 0) return 0;
    assert(a != nullptr);
    assert(b != nullptr);
    size_t m = n;
    do
    {
        na     = tolower(*a++);
        nb     = tolower(*b++);
        result = na - nb;
    } while (result == 0 && na != '\0' && --m != 0);
    return result;
#endif
}

#if defined(QT_GUI_LIB)
QString mvUtil::PathAppendA(const QString &path, const QString &more)
{
    return QDir::cleanPath(path + QDir::separator() + more);
}
#else
int mvUtil::PathAppendA(char *path, const char *more)
{
#if defined(_MSC_VER)
    return ::PathAppendA(path, more);
#else
    // @qt_todo
    return 0;
#endif
}
#endif

#if defined(QT_GUI_LIB)
QString mvUtil::PathCanonicalizeA(const QString &path)
{
    QDir dir(path);
    return dir.canonicalPath();
}
#else
int mvUtil::PathCanonicalizeA(char *buf, const char *path)
{
#if defined(_MSC_VER)
    return ::PathCanonicalizeA(buf, path);
#else
    // @qt_todo
    return 0;
#endif
}
#endif

#if defined(QT_GUI_LIB)
bool mvUtil::PathFileExistsA(const QString &path)
{
    QFileInfo info(path);
    return info.exists() && info.isFile();
}
#else
int mvUtil::PathFileExistsA(const char *path)
{
#if defined(_MSC_VER)
    return ::PathFileExistsA(path);
#else
    // @qt_todo
    return 0;
#endif
}
#endif

char *mvUtil::strlwr(char *str)
{
#if defined(_MSC_VER)
    return _strlwr(str);
#else
    char *c;
    assert(str != nullptr);
    for (c = str; *c != '\0'; ++c)
    {
        *c = tolower(*c);
    }
    return str;
#endif
}

#if defined(QT_GUI_LIB)
//
// modified from QDir::toNativeSeparators
//
QString mvUtil::toNativeSeparators(const QString &pathName)
{
#if defined(Q_OS_WIN)
    int i = pathName.indexOf(QLatin1Char('/'));
    if (i != -1)
    {
        QString      n(pathName);

        QChar *const data = n.data();
        data[i++]         = QLatin1Char('\\');

        for (; i < n.length(); ++i)
        {
            if (data[i] == QLatin1Char('/'))
                data[i] = QLatin1Char('\\');
        }

        return n;
    }
#else
    int i = pathName.indexOf(QLatin1Char('\\'));
    if (i != -1)
    {
        QString      n(pathName);

        QChar *const data = n.data();
        data[i++]         = QLatin1Char('/');

        for (; i < n.length(); ++i)
        {
            if (data[i] == QLatin1Char('\\'))
                data[i] = QLatin1Char('/');
        }

        return n;
    }
#endif
    return pathName;
}
#endif
