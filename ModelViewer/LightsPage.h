#if !defined(AFX_LIGHTSPAGE_H__B7D1E3C6_F5E8_11D3_8106_00C04F61038F__INCLUDED_)
#define AFX_LIGHTSPAGE_H__B7D1E3C6_F5E8_11D3_8106_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LightsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLightsPage dialog
class CMvDoc;

class CLightsPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CLightsPage)

    // Construction
public:
    CLightsPage();
    ~CLightsPage();
    void    Reinitialize();
    void    Activate(BOOL b);
    void    OnDefault();
    void    UpdateLabels();
    CMvDoc* m_pDoc;

    // Dialog Data
    //{{AFX_DATA(CLightsPage)
    enum { IDD = IDD_LIGHTS };
    CButton     m_HeadlightCheckBox;
    CButton     m_AuxiliaryLightCheckBox;
    CSliderCtrl m_ZSlider;
    CSliderCtrl m_YSlider;
    CSliderCtrl m_XSlider;
    CSliderCtrl m_HeadlightIntensitySlider;
    CSliderCtrl m_AuxiliaryLightIntensitySlider;
    int         m_AuxiliaryLightIntensity;
    int         m_HeadlightIntensity;
    BOOL        m_AuxiliaryLightOn;
    BOOL        m_HeadlightOn;
    int         m_DirX;
    int         m_DirY;
    int         m_DirZ;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CLightsPage)
protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CLightsPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnReleasedcaptureHeadlightIntensitySlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureAuxiliaryLightIntensitySlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureXSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureYSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureZSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnHeadlight();
    afx_msg void OnAuxiliaryLight();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTSPAGE_H__B7D1E3C6_F5E8_11D3_8106_00C04F61038F__INCLUDED_)
