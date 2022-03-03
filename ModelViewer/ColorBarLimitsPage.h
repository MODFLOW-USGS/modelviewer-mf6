#if !defined(AFX_COLORBARLIMITSPAGE_H__4F439173_3644_11D4_811B_00C04F61038F__INCLUDED_)
#define AFX_COLORBARLIMITSPAGE_H__4F439173_3644_11D4_811B_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorBarLimitsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorBarLimitsPage dialog
class CMvDoc;

class CColorBarLimitsPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CColorBarLimitsPage)

    // Construction
public:
    CColorBarLimitsPage();
    ~CColorBarLimitsPage();
    void    Reinitialize();
    void    Activate(BOOL b);
    BOOL    CustomUpdateData(BOOL b);
    void    OnDefault();
    void    Apply();
    CMvDoc* m_pDoc;
    BOOL    m_ExchangeData;
    double  m_ValueBlue;
    double  m_ValueRed;

    // Dialog Data
    //{{AFX_DATA(CColorBarLimitsPage)
    enum { IDD = IDD_COLOR_BAR_LIMITS };
    CButton m_LogScaleCheckBox;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CColorBarLimitsPage)
protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CColorBarLimitsPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnReverse();
    afx_msg void OnLogScale();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBARLIMITSPAGE_H__4F439173_3644_11D4_811B_00C04F61038F__INCLUDED_)
