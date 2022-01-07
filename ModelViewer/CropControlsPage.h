#if !defined(AFX_CROPCONTROLSPAGE_H__FB5923C1_010E_11D4_8108_00C04F61038F__INCLUDED_)
#define AFX_CROPCONTROLSPAGE_H__FB5923C1_010E_11D4_8108_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CropControlsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCropControlsPage dialog
class CMvDoc;
class CCropDlg;

class CCropControlsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCropControlsPage)

// Construction
public:
	CCropControlsPage();
	~CCropControlsPage();
	void Reinitialize();
	void Activate(BOOL b);
	BOOL CustomUpdateData(BOOL b);
	void OnDefault();
	void Apply();
	CMvDoc *m_pDoc;
	CCropDlg *m_Parent;
	double	m_XDelta;
	double	m_XMax;
	double	m_XMin;
	BOOL	m_XSync;
	double	m_YDelta;
	double	m_YMax;
	double	m_YMin;
	BOOL	m_YSync;
	double	m_ZDelta;
	double	m_ZMax;
	double	m_ZMin;
	BOOL	m_ZSync;
	int		m_CropAngle;
	BOOL	m_ExchangeData;
	BOOL m_IsActive;


// Dialog Data
	//{{AFX_DATA(CCropControlsPage)
	enum { IDD = IDD_CROP_CONTROLS };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCropControlsPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCropControlsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposXminSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposXmaxSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnXsync();
	afx_msg void OnYsync();
	afx_msg void OnZsync();
	afx_msg void OnDeltaposCropAngleSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposYminSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposYmaxSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposZminSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposZmaxSpin(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CROPCONTROLSPAGE_H__FB5923C1_010E_11D4_8108_00C04F61038F__INCLUDED_)
