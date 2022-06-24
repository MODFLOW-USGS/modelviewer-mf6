#if !defined(AFX_GRIDDLG_H__5CA7EE9B_F548_11D3_8106_00C04F61038F__INCLUDED_)
#define AFX_GRIDDLG_H__5CA7EE9B_F548_11D3_8106_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridDlg.h : header file
//

#include "GridLinesPage.h"
#include "GridShellPage.h"
#include "SubgridPage.h"
#include "GridDisplayPage.h"

/////////////////////////////////////////////////////////////////////////////
// CGridDlg dialog

class CMvDoc;

class CGridDlg : public CDialog
{
    // Construction
public:
    CGridDlg(CWnd *pParent, CMvDoc *pDoc);
    ~CGridDlg();
    BOOL              Create();
    void              Reinitialize();
    void              Activate(BOOL Activate);

    CPropertySheet   *m_PropertySheet;
    CGridLinesPage   *m_GridLinesPage;
    CGridShellPage   *m_GridShellPage;
    CSubgridPage     *m_SubgridPage;
#ifdef ENABLE_CGRIDDISPLAYPAGE
    CGridDisplayPage *m_GridDisplayPage;
#endif

    // Dialog Data
    //{{AFX_DATA(CGridDlg)
    enum { IDD = IDD_GRID };
    CButton m_ApplyButton;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGridDlg)
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
    //{{AFX_MSG(CGridDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnDone();
    afx_msg void OnApply();
    afx_msg void OnCaptureChanged(CWnd *pWnd);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDDLG_H__5CA7EE9B_F548_11D3_8106_00C04F61038F__INCLUDED_)
