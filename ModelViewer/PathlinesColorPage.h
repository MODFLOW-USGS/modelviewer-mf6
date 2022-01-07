#if !defined(AFX_PATHLINESCOLORPAGE_H__8C784CDD_23B0_11D4_8113_00C04F61038F__INCLUDED_)
#define AFX_PATHLINESCOLORPAGE_H__8C784CDD_23B0_11D4_8113_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PathlinesColorPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPathlinesColorPage dialog
class CMvDoc;
class CPathlinesDlg;

class CPathlinesColorPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPathlinesColorPage)

// Construction
public:
	CPathlinesColorPage();
	~CPathlinesColorPage();
	void Reinitialize();
	void Activate(BOOL b);
	BOOL CustomUpdateData(BOOL b);
	void Apply();
	void OnDefault();
	CMvDoc *m_pDoc;
	CPathlinesDlg *m_Parent;
	double	m_ValueBlue;
	double	m_ValueRed;
	BOOL m_IsActive;

// Dialog Data
	//{{AFX_DATA(CPathlinesColorPage)
	enum { IDD = IDD_PATHLINES_COLOR };
	BOOL	m_LogTransport;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPathlinesColorPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_ExchangeData;

	// Generated message map functions
	//{{AFX_MSG(CPathlinesColorPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckLogTransport();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATHLINESCOLORPAGE_H__8C784CDD_23B0_11D4_8113_00C04F61038F__INCLUDED_)
