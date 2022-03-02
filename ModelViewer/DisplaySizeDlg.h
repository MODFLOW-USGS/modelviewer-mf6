#if !defined(AFX_DISPLAYSIZEDLG_H__1A3C4B10_1482_11D4_810D_00C04F61038F__INCLUDED_)
#define AFX_DISPLAYSIZEDLG_H__1A3C4B10_1482_11D4_810D_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DisplaySizeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDisplaySizeDlg dialog

class CDisplaySizeDlg : public CDialog
{
    // Construction
public:
    CDisplaySizeDlg(CWnd* pParent = NULL); // standard constructor

    // Dialog Data
    //{{AFX_DATA(CDisplaySizeDlg)
    enum { IDD = IDD_DISPLAY_SIZE };
    int m_DisplayHeight;
    int m_DisplayWidth;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDisplaySizeDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CDisplaySizeDlg)
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISPLAYSIZEDLG_H__1A3C4B10_1482_11D4_810D_00C04F61038F__INCLUDED_)
