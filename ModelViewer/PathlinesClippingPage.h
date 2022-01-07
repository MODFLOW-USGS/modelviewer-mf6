#if !defined(AFX_PATHLINESCLIPPINGPAGE_H__7396DD62_23B7_11D4_8113_00C04F61038F__INCLUDED_)
#define AFX_PATHLINESCLIPPINGPAGE_H__7396DD62_23B7_11D4_8113_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PathlinesClippingPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPathlinesClippingPage dialog
class CMvDoc;
class CPathlinesDlg;

class CPathlinesClippingPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPathlinesClippingPage)

// Construction
public:
	CPathlinesClippingPage();
	~CPathlinesClippingPage();
	void Reinitialize();
	void Activate(BOOL b);
	BOOL CustomUpdateData(BOOL b);
	void Apply();
	void OnDefault();
	CMvDoc *m_pDoc;
	CPathlinesDlg *m_Parent;
	double	m_MaximumTime;
	double	m_MinimumTime;
	int		m_ClippingMode;
	BOOL m_IsActive;

// Dialog Data
	//{{AFX_DATA(CPathlinesClippingPage)
	enum { IDD = IDD_PATHLINES_CLIPPING };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPathlinesClippingPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_ExchangeData;

	// Generated message map functions
	//{{AFX_MSG(CPathlinesClippingPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnNoPathlineClipping();
	afx_msg void OnClipBySimtime();
	afx_msg void OnClipByTimeRange();
	afx_msg void OnClipByMaximumTime();
	afx_msg void OnClipByMinimumTime();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATHLINESCLIPPINGPAGE_H__7396DD62_23B7_11D4_8113_00C04F61038F__INCLUDED_)
