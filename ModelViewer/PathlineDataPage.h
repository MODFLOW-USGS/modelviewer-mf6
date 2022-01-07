#if !defined(AFX_PATHLINEDATAPAGE_H__B2B05C41_23A7_11D4_A325_000557C10000__INCLUDED_)
#define AFX_PATHLINEDATAPAGE_H__B2B05C41_23A7_11D4_A325_000557C10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PathlineDataPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPathlineDataPage dialog

class CPathlineDataPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPathlineDataPage)

// Construction
public:
	CPathlineDataPage();
	~CPathlineDataPage();
	void Reinitialize();
	void SetRange(double *range);

// Dialog Data
	//{{AFX_DATA(CPathlineDataPage)
	enum { IDD = IDD_PATHLINE_DATA };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPathlineDataPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPathlineDataPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATHLINEDATAPAGE_H__B2B05C41_23A7_11D4_A325_000557C10000__INCLUDED_)
