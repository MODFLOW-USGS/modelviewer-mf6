#if !defined(AFX_BACKGROUNDPAGE_H__B7D1E3C8_F5E8_11D3_8106_00C04F61038F__INCLUDED_)
#define AFX_BACKGROUNDPAGE_H__B7D1E3C8_F5E8_11D3_8106_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BackgroundPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBackgroundPage dialog
class CMvDoc;

class CBackgroundPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CBackgroundPage)

    // Construction
public:
    CBackgroundPage();
    ~CBackgroundPage();
    void    Reinitialize();
    void    Activate(BOOL b);
    void    OnDefault();
    void    UpdateLabels();
    CMvDoc* m_pDoc;

    // Dialog Data
    //{{AFX_DATA(CBackgroundPage)
    enum { IDD = IDD_BACKGROUND };
    CSliderCtrl m_BlueSlider;
    CSliderCtrl m_GreenSlider;
    CSliderCtrl m_RedSlider;
    int         m_Red;
    int         m_Green;
    int         m_Blue;
    int         m_Background;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CBackgroundPage)
protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CBackgroundPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnWhiteBackground();
    afx_msg void OnCustomBackground();
    afx_msg void OnReleasedcaptureRedSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureGreenSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureBlueSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKGROUNDPAGE_H__B7D1E3C8_F5E8_11D3_8106_00C04F61038F__INCLUDED_)
