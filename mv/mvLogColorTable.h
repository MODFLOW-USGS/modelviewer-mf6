// mvLogColorTable.h: interface for the mvLogColorTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined mvLogColorTable_h
#define mvLogColorTable_h

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cstdint>

#include "vtkLogLookupTable.h"
#include "mvColorTable.h"

class mvLogColorTable : public vtkLogLookupTable
{
protected:
    void          FillColorTable(std::uint32_t lowercolor, std::uint32_t uppercolor);
    bool          m_TransformValues;
    int           m_ColorScheme;
    std::uint32_t m_FirstCustomColor;
    std::uint32_t m_LastCustomColor;

public:
    vtkTypeMacro(mvLogColorTable, vtkLogLookupTable);

    void                    Build() override;
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
    std::uint32_t           GetFirstCustomColor() { return m_FirstCustomColor; };
    std::uint32_t           GetLastCustomColor() { return m_LastCustomColor; };
    void                    SetFirstCustomColor(std::uint32_t value);
    void                    SetLastCustomColor(std::uint32_t value);

protected:
    mvLogColorTable();
    virtual ~mvLogColorTable();
};

#endif // !defined mvLogColorTable_h
