// mvColorTable.h: interface for the mvColorTable class.
//
// mvColorTable differs from vtkLookupTable by applying a transformation
// to the color scaling.  
// The transformation is of the form
// y = x^Exponent for x in the range (0-0.2), (0.4-0.6), and (0.8-1).
// The transformation is of the form
// y = 1-((1-x)^Exponent) for x in the range (0.2-0.4) and (0.6-0.8).
// This has the effect of spreading out the yellow and aqua bands at the 
// expense of the red, green, and blue bands.
// mvColorTable is only suitable for color scales that range from red to blue
// or vice versa.
//////////////////////////////////////////////////////////////////////

#if !defined mvColorTable_h
#define mvColorTable_h

#include "vtkLookupTable.h"

#define MV_CT_UNDEFINED_COLOR_SCHEME -1
#define MV_CT_DEFAULT_C0LOR_SCHEME 0
#define MV_CT_REVERSED_DEFAULT_C0LOR_SCHEME 1
#define MV_CT_MODIFIED_C0LOR_SCHEME 2
#define MV_CT_REVERSED_MODIFIED_C0LOR_SCHEME 3
#define MV_CT_CUSTOM_C0LOR_SCHEME 4
#define MV_CT_REVERSED_CUSTOM_C0LOR_SCHEME 5

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class mvColorTable : public vtkLookupTable  
{
protected:
  void FillColorTable(unsigned long lowercolor, unsigned long uppercolor);
  bool m_TransformValues;
  int m_ColorScheme;
  unsigned long m_FirstCustomColor;
  unsigned long m_LastCustomColor;
public:
	mvColorTable();
	virtual ~mvColorTable();
  virtual void Build();
  static mvColorTable *New();
  bool GetTransformValues() {return m_TransformValues;}
  void SetTransformValues(bool Value);
  int ColorScheme() {return m_ColorScheme;}
  void SetColorScheme(int Value);
  void SetDefaultColorScheme();
  void SetReversedDefaultColorScheme();
  void SetModifiedColorScheme();
  void SetReversedModifiedColorScheme();
  void SetCustomColorScheme();
  void SetReversedCustomColorScheme();
  unsigned long GetFirstCustomColor() {return m_FirstCustomColor;};
  unsigned long GetLastCustomColor() {return m_LastCustomColor;};
  void SetFirstCustomColor(unsigned long value);
  void SetLastCustomColor(unsigned long value);

};

#endif // !defined mvColorTable_h
