#if !defined(AFX_OVERLAYCONTROLSPAGE_H__572344D0_B829_4D96_A96B_1F31FEF068DA__INCLUDED_)
#define AFX_OVERLAYCONTROLSPAGE_H__572344D0_B829_4D96_A96B_1F31FEF068DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OverlayControlsPage.h : header file
//

class COverlayDlg;
class CMvDoc;
/////////////////////////////////////////////////////////////////////////////
// COverlayControlsPage dialog

class COverlayControlsPage : public CPropertyPage
{
    DECLARE_DYNCREATE(COverlayControlsPage)

    // Construction
public:
    COverlayControlsPage();
    ~COverlayControlsPage();
    void Reinitialize();
    void Activate(BOOL b);

    // Dialog Data
    //{{AFX_DATA(COverlayControlsPage)
    enum { IDD = IDD_OVERLAY_CONTROLS };
    CButton      m_FlatRadioButton;
    CButton      m_TrimCheckBox;
    double       m_XOrig;
    double       m_YOrig;
    double       m_Scale;
    double       m_Elev;
    double       m_DrapeGap;
    BOOL         m_Trim;
    double       m_Angle;
    int          m_Drape;
    BOOL         m_Crop;
    BOOL         m_StructuredGrid;
    //}}AFX_DATA

    COverlayDlg *m_Parent;
    CMvDoc      *m_pDoc;

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(COverlayControlsPage)
protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(COverlayControlsPage)
    afx_msg void OnDrape();
    afx_msg void OnFlat();
    afx_msg void OnOverlayTrim();
    afx_msg void OnOverlayCrop();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OVERLAYCONTROLSPAGE_H__572344D0_B829_4D96_A96B_1F31FEF068DA__INCLUDED_)
