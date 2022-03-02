#if !defined(AFX_GRIDSHELLPAGE_H__5CA7EE9D_F548_11D3_8106_00C04F61038F__INCLUDED_)
#define AFX_GRIDSHELLPAGE_H__5CA7EE9D_F548_11D3_8106_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridShellPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGridShellPage dialog
class CMvDoc;
class CGridDlg;

class CGridShellPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CGridShellPage)

    // Construction
public:
    CGridShellPage();
    ~CGridShellPage();
    void      Reinitialize();
    void      Activate(BOOL b);
    void      UpdateLabels();
    CMvDoc*   m_pDoc;
    CGridDlg* m_Parent;

    // Dialog Data
    //{{AFX_DATA(CGridShellPage)
    enum { IDD = IDD_GRID_SHELL };
    CSliderCtrl m_OpacitySlider;
    CSliderCtrl m_BlueSlider;
    CSliderCtrl m_GreenSlider;
    CSliderCtrl m_RedSlider;
    int         m_Red;
    int         m_Green;
    int         m_Blue;
    int         m_Opacity;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CGridShellPage)
public:
    virtual BOOL OnSetActive();

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CGridShellPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnReleasedcaptureRedSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureGreenSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureBlueSlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnReleasedcaptureOpacitySlider(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDSHELLPAGE_H__5CA7EE9D_F548_11D3_8106_00C04F61038F__INCLUDED_)
