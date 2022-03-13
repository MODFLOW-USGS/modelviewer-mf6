// mvLogColorTable.h: interface for the mvLogColorTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined mvLogColorTable_h
#define mvLogColorTable_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vtkLogLookupTable.h"
#include "mvColorTable.h"

class mvLogColorTable : public vtkLogLookupTable
{
protected:
    void          FillColorTable(unsigned long lowercolor, unsigned long uppercolor);
    bool          m_TransformValues;
    int           m_ColorScheme;
    unsigned long m_FirstCustomColor;
    unsigned long m_LastCustomColor;

public:
    vtkTypeMacro(mvLogColorTable, vtkLogLookupTable);

    virtual void            Build();
    static mvLogColorTable *New();
    bool                    GetTransformValues() { return m_TransformValues; };
    void                    SetTransformValues(bool Value);
    int                     ColorScheme() { return m_ColorScheme; }
    void                    SetColorScheme(int Value);
    void                    SetDefaultColorScheme();
    void                    SetReversedDefaultColorScheme();
    void                    SetModifiedColorScheme();
    void                    SetReversedModifiedColorScheme();
    void                    SetCustomColorScheme();
    void                    SetReversedCustomColorScheme();
    unsigned long           GetFirstCustomColor() { return m_FirstCustomColor; };
    unsigned long           GetLastCustomColor() { return m_LastCustomColor; };
    void                    SetFirstCustomColor(unsigned long value);
    void                    SetLastCustomColor(unsigned long value);

protected:
    mvLogColorTable();
    virtual ~mvLogColorTable();
};

#endif // !defined mvLogColorTable_h
