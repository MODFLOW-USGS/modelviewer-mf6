#if !defined(AFX_REGULARISOSURFACEPAGE_H__5B4B4206_F07B_11D3_8105_00C04F61038F__INCLUDED_)
#define AFX_REGULARISOSURFACEPAGE_H__5B4B4206_F07B_11D3_8105_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegularIsosurfacePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegularIsosurfacePage dialog

class CMvDoc;

class CRegularIsosurfacePage : public CPropertyPage
{
    DECLARE_DYNCREATE(CRegularIsosurfacePage)

    // Construction
public:
    CRegularIsosurfacePage();
    ~CRegularIsosurfacePage();
    void    Reinitialize();
    void    Activate(BOOL b);
    void    Apply();
    BOOL    CustomUpdateData(BOOL b);
    CMvDoc* m_pDoc;
    double  m_ValueMax;
    double  m_ValueMin;
    int     m_IsosurfaceCount;
    BOOL    m_IsActive;

    // Dialog Data
    //{{AFX_DATA(CRegularIsosurfacePage)
    enum { IDD = IDD_REGULAR_ISOSURFACE };
    //}}AFX_DATA

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CRegularIsosurfacePage)
public:
    virtual BOOL OnSetActive();

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    BOOL         m_ExchangeData;

    // Generated message map functions
    //{{AFX_MSG(CRegularIsosurfacePage)
    afx_msg void OnFullRange();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGULARISOSURFACEPAGE_H__5B4B4206_F07B_11D3_8105_00C04F61038F__INCLUDED_)
