#if !defined(AFX_AXESPAGE_H__5CA7EE9F_F548_11D3_8106_00C04F61038F__INCLUDED_)
#define AFX_AXESPAGE_H__5CA7EE9F_F548_11D3_8106_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxesPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxesPage dialog
class CMvDoc;
class CGeometryDlg;

class CAxesPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CAxesPage)

    // Construction
public:
    CAxesPage();
    ~CAxesPage();
    void          Reinitialize();
    void          Activate(BOOL b);
    BOOL          CustomUpdateData(BOOL b);
    void          Apply();
    CMvDoc       *m_pDoc;
    CGeometryDlg *m_Parent;
    int           m_Representation;
    double        m_AxesSize;
    double        m_TubeDiameter;
    double        m_XPos;
    double        m_YPos;
    double        m_ZPos;
    BOOL          m_IsActive;

    // Dialog Data
    //{{AFX_DATA(CAxesPage)
    enum { IDD = IDD_AXES };
    //}}AFX_DATA

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CAxesPage)
public:
    virtual BOOL OnSetActive();

protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    BOOL         m_ExchangeData;

    // Generated message map functions
    //{{AFX_MSG(CAxesPage)
    afx_msg void OnLine();
    afx_msg void OnTube();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXESPAGE_H__5CA7EE9F_F548_11D3_8106_00C04F61038F__INCLUDED_)
