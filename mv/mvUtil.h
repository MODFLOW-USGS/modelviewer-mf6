#ifndef __mvUtil_h
#define __mvUtil_h

#include "mvHeader.h"
#include <fstream>

//class vtkObject;

using std::ifstream;

class MV_EXPORT mvUtil  
{
public:
//	static void DeleteVtkObject(vtkObject *obj);
    static void interp2d(double *x, double *y, double *z, double *dx, double *dy, double *zc,
			int numRow, int numCol, double znull);
	static void interp3d(double *cellDraw, double *ptDraw, double *dx, double *dy, double *zc,
			  int numCol, int numRow, int numLay, double drawNull, double znull, int *useLayer = 0);

	static int ExtractNumber(ifstream *in, int *buffer);

	static int ExtractNumber(ifstream *in, double *buffer);

	static int ExtractString(ifstream *in, char *buffer, int len);

	static int ExtractLabelAndValue(ifstream *in, char *label, char *value);

	static void TrimLeft(char *aString);

	static void TrimRight(char *aString);

	static void ToLowerCase(char *aString);

	static void RemoveQuotes(char* sString);

	static void RemoveDoubleQuotes(char* sString);

   static void ReplaceTabWithWhiteSpace(char *aString);

    static double mvUtil::Fortran_atof(char *aString);
	static void ExtractFirstString(char *aString, char *FirstString);

   static char *NextNonSpaceChar(char *str, int len);

   static void Wavelength2RGBA(double wavelength, double *RGB);

};

#endif
