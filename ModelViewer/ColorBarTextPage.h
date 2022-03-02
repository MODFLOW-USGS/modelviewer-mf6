#if !defined(AFX_COLORBARTEXTPAGE_H__4F439175_3644_11D4_811B_00C04F61038F__INCLUDED_)
#define AFX_COLORBARTEXTPAGE_H__4F439175_3644_11D4_811B_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorBarTextPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorBarTextPage dialog
class CMvDoc;

class CColorBarTextPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CColorBarTextPage)

    // Construction
public:
    CColorBarTextPage();
    ~CColorBarTextPage();
    void    Reinitialize();
    void    Activate(BOOL b);
    BOOL    CustomUpdateData(BOOL b);
    void    OnDefault();
    void    Apply();
    CMvDoc* m_pDoc;
    BOOL    m_ExchangeData;
    int     m_FontSize;
    int     m_NumLabels;
    int     m_ColorOption;
    int     m_Precision;

    // Dialog Data
    //{{AFX_DATA(CColorBarTextPage)
    enum { IDD = IDD_COLOR_BAR_TEXT };
    //}}AFX_DATA

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CColorBarTextPage)
protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CColorBarTextPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnBlack();
    afx_msg void OnGray();
    afx_msg void OnWhite();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBARTEXTPAGE_H__4F439175_3644_11D4_811B_00C04F61038F__INCLUDED_)
