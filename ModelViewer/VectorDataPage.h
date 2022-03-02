#if !defined(AFX_VECTORDATAPAGE_H__68D5DFF2_0812_11D4_8109_00C04F61038F__INCLUDED_)
#define AFX_VECTORDATAPAGE_H__68D5DFF2_0812_11D4_8109_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VectorDataPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVectorDataPage dialog

class CVectorDataPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CVectorDataPage)

    // Construction
public:
    CVectorDataPage();
    ~CVectorDataPage();
    void Reinitialize();
    void SetRange(double* range);

    // Dialog Data
    //{{AFX_DATA(CVectorDataPage)
    enum { IDD = IDD_VECTOR_DATA };
    // NOTE - ClassWizard will add data members here.
    //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_DATA

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CVectorDataPage)
protected:
    virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CVectorDataPage)
    // NOTE: the ClassWizard will add member functions here
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VECTORDATAPAGE_H__68D5DFF2_0812_11D4_8109_00C04F61038F__INCLUDED_)
