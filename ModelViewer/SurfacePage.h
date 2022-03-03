#if !defined(AFX_SURFACEPAGE_H__B7D1E3C4_F5E8_11D3_8106_00C04F61038F__INCLUDED_)
#define AFX_SURFACEPAGE_H__B7D1E3C4_F5E8_11D3_8106_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SurfacePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSurfacePage dialog
class CMvDoc;

class CSurfacePage : public CPropertyPage
{
    DECLARE_DYNCREATE(CSurfacePage)

    // Construction
public:
    CSurfacePage();
    ~CSurfacePage();
    void    Reinitialize();
    void    Activate(BOOL b);
    void    OnDefault();
    void    UpdateLabels();
    CMvDoc* m_pDoc;

    // Dialog Data
    //{{AFX_DATA(CSurfacePage)
    enum { IDD = IDD_SURFACE };
    CSliderCtrl m_SpecularPowerSlider;
    CSliderCtrl m_SpecularSlider;
    CSliderCtrl m_AmbientSlider;
    CSliderCtrl m_DiffuseSlider;
    int         m_Diffuse;
    int         m_Ambient;
    int         m_Specular;
    int         m_SpecularPower;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CSurfacePage)
protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CSurfacePage)
    virtual BOOL OnInitDialog();
    afx_msg void OnReleasedcaptureDiffuseSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureAmbientSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureSpecularSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureSpecularPowerSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SURFACEPAGE_H__B7D1E3C4_F5E8_11D3_8106_00C04F61038F__INCLUDED_)
