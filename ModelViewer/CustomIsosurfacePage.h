#if !defined(AFX_CUSTOMISOSURFACEPAGE_H__5B4B4204_F07B_11D3_8105_00C04F61038F__INCLUDED_)
#define AFX_CUSTOMISOSURFACEPAGE_H__5B4B4204_F07B_11D3_8105_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomIsosurfacePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomIsosurfacePage dialog

class CMvDoc;

class CCustomIsosurfacePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCustomIsosurfacePage)

// Construction
public:
	CCustomIsosurfacePage();
	~CCustomIsosurfacePage();
	void Reinitialize();
	void Activate(BOOL b);
	void Apply();
	BOOL CustomUpdateData(BOOL b);
	CMvDoc *m_pDoc;
	BOOL m_IsActive;

// Dialog Data
	//{{AFX_DATA(CCustomIsosurfacePage)
	enum { IDD = IDD_CUSTOM_ISOSURFACE };
	CListBox	m_IsosurfaceList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCustomIsosurfacePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_ExchangeData;
	double m_Value;

	// Generated message map functions
	//{{AFX_MSG(CCustomIsosurfacePage)
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnSetfocusValue();
	afx_msg void OnSetfocusIsosurfaceList();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMISOSURFACEPAGE_H__5B4B4204_F07B_11D3_8105_00C04F61038F__INCLUDED_)
