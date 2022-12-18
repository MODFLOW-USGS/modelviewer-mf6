#ifndef __mvUtil_h
#define __mvUtil_h

#include "mvHeader.h"
#include <fstream>

#if defined(QT_GUI_LIB)
#include <QString>
#endif

// class vtkObject;

using std::ifstream;

class MV_EXPORT mvUtil
{
public:
    //	static void DeleteVtkObject(vtkObject *obj);
    static void   interp2d(double *x, double *y, double *z, double *dx, double *dy, double *zc,
                           int numRow, int numCol, double znull);
    static void   interp3d(double *cellDraw, double *ptDraw, double *dx, double *dy, double *zc,
                           int numCol, int numRow, int numLay, double drawNull, double znull, int *useLayer = 0);

    static int    stricmp(const char *a, const char *b);

    static char * strlwr(char *str);

    static int    strnicmp(const char *a, const char *b, size_t n);

    static int    ExtractNumber(ifstream *in, int *buffer);

    static int    ExtractNumber(ifstream *in, double *buffer);

    static int    ExtractString(ifstream *in, char *buffer, int len);

    static int    ExtractLabelAndValue(ifstream *in, char *label, char *value);

    static void   TrimLeft(char *aString);

    static void   TrimRight(char *aString);

    static void   ToLowerCase(char *aString);

#if defined(QT_GUI_LIB)
    static QString PathAppendA(const QString &path, const QString &more);
#else
    static int     PathAppendA(char *path, const char *more);
#endif


#if defined(QT_GUI_LIB)
    static QString PathCanonicalizeA(const QString &path);
#else
    static int     PathCanonicalizeA(char *buf, const char *path);
#endif

#if defined(QT_GUI_LIB)
    static bool PathFileExistsA(const QString &path);
#else
    static int     PathFileExistsA(const char *path);
#endif

    static void   RemoveQuotes(char *sString);

    static void   RemoveDoubleQuotes(char *sString);

    static void   ReplaceTabWithWhiteSpace(char *aString);

    static double Fortran_atof(char *aString);
    static void   ExtractFirstString(char *aString, char *FirstString);

    static char * NextNonSpaceChar(char *str, int len);

    static void   Wavelength2RGBA(double wavelength, double *RGB);

#if defined(QT_GUI_LIB)
    static QString toNativeSeparators(const QString &pathName);
#endif
};

#endif
