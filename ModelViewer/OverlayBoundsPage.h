#if !defined(AFX_OVERLAYBOUNDSPAGE_H__9C709F2E_4FBE_4155_8A5F_A53BCD360D44__INCLUDED_)
#define AFX_OVERLAYBOUNDSPAGE_H__9C709F2E_4FBE_4155_8A5F_A53BCD360D44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OverlayBoundsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COverlayBoundsPage dialog

class COverlayBoundsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COverlayBoundsPage)

// Construction
public:
	COverlayBoundsPage();
	~COverlayBoundsPage();
	void Reinitialize();
	void SetBounds(double xmin, double xmax, double ymin, double ymax);

// Dialog Data
	//{{AFX_DATA(COverlayBoundsPage)
	enum { IDD = IDD_OVERLAY_BOUNDS };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COverlayBoundsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COverlayBoundsPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OVERLAYBOUNDSPAGE_H__9C709F2E_4FBE_4155_8A5F_A53BCD360D44__INCLUDED_)
