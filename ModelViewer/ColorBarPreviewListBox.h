#if !defined(AFX_COLORBARPREVIEWLISTBOX_H__8CA9D6CA_6E5E_4915_B226_7EFAB43627AC__INCLUDED_)
#define AFX_COLORBARPREVIEWLISTBOX_H__8CA9D6CA_6E5E_4915_B226_7EFAB43627AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorBarPreviewListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorBarPreviewListBox window
// Use a ListBox to display a color

class CColorBarPreviewListBox : public CListBox
{
    // Construction
public:
    CColorBarPreviewListBox();

    // Attributes
public:
    // m_Color is the color that will be displayed.
    COLORREF m_Color;

    // Operations
public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColorBarPreviewListBox)
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CColorBarPreviewListBox();

    // Generated message map functions
protected:
    //{{AFX_MSG(CColorBarPreviewListBox)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBARPREVIEWLISTBOX_H__8CA9D6CA_6E5E_4915_B226_7EFAB43627AC__INCLUDED_)
