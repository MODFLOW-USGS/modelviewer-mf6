#if !defined(AFX_ANIMATIONOPTIONSPAGE_H__664AD1BD_E987_11D3_8103_00C04F61038F__INCLUDED_)
#define AFX_ANIMATIONOPTIONSPAGE_H__664AD1BD_E987_11D3_8103_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimationOptionsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnimationOptionsPage dialog

class CAnimationOptionsPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CAnimationOptionsPage)

    // Construction
public:
    CAnimationOptionsPage();
    ~CAnimationOptionsPage();
    void Reinitialize();
    void Activate(BOOL b);

    // Dialog Data
    //{{AFX_DATA(CAnimationOptionsPage)
    enum { IDD = IDD_ANIMATION_OPTIONS };
    double m_Delay;
    double m_Elevate;
    double m_Rotate;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CAnimationOptionsPage)
protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CAnimationOptionsPage)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMATIONOPTIONSPAGE_H__664AD1BD_E987_11D3_8103_00C04F61038F__INCLUDED_)
