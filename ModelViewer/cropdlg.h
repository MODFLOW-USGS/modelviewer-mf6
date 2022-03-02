#if !defined(AFX_CROPDLG_H__5B4B4203_F07B_11D3_8105_00C04F61038F__INCLUDED_)
#define AFX_CROPDLG_H__5B4B4203_F07B_11D3_8105_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CropDlg.h : header file
//

#include "CropControlsPage.h"
#include "CropOptionsPage.h"

/////////////////////////////////////////////////////////////////////////////
// CCropDlg dialog

class CMvDoc;

class CCropDlg : public CDialog
{
    // Construction
public:
    CCropDlg(CWnd *pParent, CMvDoc *pDoc);
    ~CCropDlg();
    BOOL               Create();
    void               Reinitialize();
    void               Activate(BOOL b);

    CPropertySheet    *m_PropertySheet;
    CCropControlsPage *m_ControlsPage;
    CCropOptionsPage  *m_OptionsPage;

    // Dialog Data
    //{{AFX_DATA(CCropDlg)
    enum { IDD = IDD_CROP };
    CButton m_ApplyButton;
    CButton m_DefaultButton;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCropDlg)
protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
    virtual void PostNcDestroy();
    //}}AFX_VIRTUAL

    // Implementation
protected:
    CMvDoc      *m_pDoc;
    CWnd        *m_pParent;
    int          m_nID;

    // Generated message map functions
    //{{AFX_MSG(CCropDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnDone();
    afx_msg void OnDefault();
    afx_msg void OnApply();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CROPDLG_H__5B4B4203_F07B_11D3_8105_00C04F61038F__INCLUDED_)
