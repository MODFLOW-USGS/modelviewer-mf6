#if !defined(AFX_VECTOROPTIONSPAGE_H__121CFCA5_0014_11D4_8108_00C04F61038F__INCLUDED_)
#define AFX_VECTOROPTIONSPAGE_H__121CFCA5_0014_11D4_8108_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VectorOptionsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVectorOptionsPage dialog
class CMvDoc;

class CVectorOptionsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CVectorOptionsPage)

// Construction
public:
	CVectorOptionsPage();
	~CVectorOptionsPage();
	void Reinitialize();
	void Activate(BOOL b);
	BOOL CustomUpdateData(BOOL b);
	void Apply();
	void OnDefault();
	CMvDoc *m_pDoc;

// Dialog Data
	//{{AFX_DATA(CVectorOptionsPage)
	enum { IDD = IDD_VECTOR_OPTIONS };
	//}}AFX_DATA

	double	m_ScaleFactor;
	BOOL	m_ShowGlyph;
	int		m_ColorOption;
	BOOL	m_LogTransform;
	double   m_LineWidth;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CVectorOptionsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_ExchangeData;

	// Generated message map functions
	//{{AFX_MSG(CVectorOptionsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposGlyphSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowGlyph();
	afx_msg void OnBlack();
	afx_msg void OnGray();
	afx_msg void OnWhite();
	afx_msg void OnLogTransform();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VECTOROPTIONSPAGE_H__121CFCA5_0014_11D4_8108_00C04F61038F__INCLUDED_)
