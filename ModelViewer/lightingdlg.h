#if !defined(AFX_LIGHTINGDLG_H__FD096985_EEFF_11D3_A325_203757C10000__INCLUDED_)
#define AFX_LIGHTINGDLG_H__FD096985_EEFF_11D3_A325_203757C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LightingDlg.h : header file
//

#include "LightsPage.h"
#include "SurfacePage.h"
#include "BackgroundPage.h"

/////////////////////////////////////////////////////////////////////////////
// CLightingDlg dialog
class CMvDoc;

class CLightingDlg : public CDialog
{
    // Construction
public:
    CLightingDlg(CWnd *pParent, CMvDoc *pDoc);
    ~CLightingDlg();
    BOOL             Create();
    void             Reinitialize();
    void             Activate(BOOL b);

    CPropertySheet  *m_PropertySheet;
    CLightsPage     *m_LightsPage;
    CSurfacePage    *m_SurfacePage;
    CBackgroundPage *m_BackgroundPage;

    // Dialog Data
    //{{AFX_DATA(CLightingDlg)
    enum { IDD = IDD_LIGHTING };
    CButton m_DefaultButton;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CLightingDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
    virtual void PostNcDestroy();
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CWnd        *m_pParent;
    CMvDoc      *m_pDoc;
    int          m_nID;

    // Generated message map functions
    //{{AFX_MSG(CLightingDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnDefault();
    afx_msg void OnDone();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTINGDLG_H__FD096985_EEFF_11D3_A325_203757C10000__INCLUDED_)
