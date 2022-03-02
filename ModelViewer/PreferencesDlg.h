#if !defined(AFX_PREFERENCESDLG_H__664AD1B6_E987_11D3_8103_00C04F61038F__INCLUDED_)
#define AFX_PREFERENCESDLG_H__664AD1B6_E987_11D3_8103_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreferencesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreferencesDlg dialog

class CPreferencesDlg : public CDialog
{
    // Construction
public:
    CPreferencesDlg(CWnd* pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CPreferencesDlg)
    enum { IDD = IDD_PREFERENCES };
    int m_InteractorStyle;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CPreferencesDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CPreferencesDlg)
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCESDLG_H__664AD1B6_E987_11D3_8103_00C04F61038F__INCLUDED_)
