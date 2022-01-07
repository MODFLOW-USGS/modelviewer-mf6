#if !defined(AFX_SCALEPAGE_H__8932E0A3_F5F2_11D3_8106_00C04F61038F__INCLUDED_)
#define AFX_SCALEPAGE_H__8932E0A3_F5F2_11D3_8106_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScalePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScalePage dialog
class CMvDoc;
class CGeometryDlg;

class CScalePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CScalePage)

// Construction
public:
	CScalePage();
	~CScalePage();
	void Reinitialize();
	void Activate(BOOL b);
	void Apply();
	CMvDoc *m_pDoc;
	CGeometryDlg *m_Parent;
	BOOL m_IsActive;

// Dialog Data
	//{{AFX_DATA(CScalePage)
	enum { IDD = IDD_SCALE };
	double	m_XScale;
	double	m_YScale;
	double	m_ZScale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CScalePage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CScalePage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALEPAGE_H__8932E0A3_F5F2_11D3_8106_00C04F61038F__INCLUDED_)
