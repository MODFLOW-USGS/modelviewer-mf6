#if !defined(AFX_GRIDLINESPAGE_H__5CA7EE9C_F548_11D3_8106_00C04F61038F__INCLUDED_)
#define AFX_GRIDLINESPAGE_H__5CA7EE9C_F548_11D3_8106_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridLinesPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGridLinesPage dialog
class CMvDoc;
class CGridDlg;

class CGridLinesPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGridLinesPage)

// Construction
public:
	CGridLinesPage();
	~CGridLinesPage();
	void Reinitialize();
	void Activate(BOOL Activate);
	BOOL CustomUpdateData(BOOL b);
	void Apply();
	CMvDoc *m_pDoc;
	CGridDlg *m_Parent;
	int m_XMin;
	int m_XMax;
	int m_YMin;
	int m_YMax;
	int m_ZMin;
	int m_ZMax;
	int	m_PositionX;
	int	m_PositionY;
	int	m_PositionZ;
	int m_ColorOption;
	BOOL m_IsActive;
	BOOL m_StructuredGrid;


// Dialog Data
	//{{AFX_DATA(CGridLinesPage)
	enum { IDD = IDD_GRID_LINES };
	CButton	m_ActivateZCheckBox;
	CButton	m_ActivateYCheckBox;
	CButton	m_ActivateXCheckBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGridLinesPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_ExchangeData;

	// Generated message map functions
	//{{AFX_MSG(CGridLinesPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnActivateX();
	afx_msg void OnActivateY();
	afx_msg void OnActivateZ();
	afx_msg void OnDeltaposPositionXSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposPositionYSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposPositionZSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGray();
	afx_msg void OnBlack();
	afx_msg void OnWhite();
//	afx_msg void OnActivateOutline();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDLINESPAGE_H__5CA7EE9C_F548_11D3_8106_00C04F61038F__INCLUDED_)
