#if !defined(AFX_PATHLINESDISPLAYPAGE_H__CF519F15_0804_11D4_8109_00C04F61038F__INCLUDED_)
#define AFX_PATHLINESDISPLAYPAGE_H__CF519F15_0804_11D4_8109_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PathlinesDisplayPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPathlinesDisplayPage dialog
class CMvDoc;
class CPathlinesDlg;

class CPathlinesDisplayPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CPathlinesDisplayPage)

    // Construction
public:
    CPathlinesDisplayPage();
    ~CPathlinesDisplayPage();
    void           Reinitialize();
    void           Activate(BOOL b);
    BOOL           CustomUpdateData(BOOL b);
    void           Apply();
    void           OnDefault();
    CMvDoc        *m_pDoc;
    double         m_TubeDiameter;
    int            m_PathlineRepresentation;
    CPathlinesDlg *m_Parent;
    BOOL           m_IsActive;

    // Dialog Data
    //{{AFX_DATA(CPathlinesDisplayPage)
    enum { IDD = IDD_PATHLINES_DISPLAY };
    //}}AFX_DATA

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CPathlinesDisplayPage)
public:
    virtual BOOL OnSetActive();

protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    BOOL         m_ExchangeData;

    // Generated message map functions
    //{{AFX_MSG(CPathlinesDisplayPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnLine();
    afx_msg void OnTube();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATHLINESDISPLAYPAGE_H__CF519F15_0804_11D4_8109_00C04F61038F__INCLUDED_)
