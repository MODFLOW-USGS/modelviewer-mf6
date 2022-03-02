#if !defined(AFX_SCALARDATAPAGE_H__68D5DFF1_0812_11D4_8109_00C04F61038F__INCLUDED_)
#define AFX_SCALARDATAPAGE_H__68D5DFF1_0812_11D4_8109_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScalarDataPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScalarDataPage dialog

class CMvDoc;

class CScalarDataPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CScalarDataPage)

    // Construction
public:
    CScalarDataPage();
    ~CScalarDataPage();
    void    Reinitialize();
    void    Activate(BOOL b);
    void    SetRange(double *range);
    CMvDoc *m_pDoc;

    // Dialog Data
    //{{AFX_DATA(CScalarDataPage)
    enum { IDD = IDD_SCALAR_DATA };
    CComboBox m_DataTypeChooser;
    int       m_DataType;
    //}}AFX_DATA

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CScalarDataPage)
protected:
    virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CScalarDataPage)
    afx_msg void OnSelchangeDataTypeCombo();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALARDATAPAGE_H__68D5DFF1_0812_11D4_8109_00C04F61038F__INCLUDED_)
