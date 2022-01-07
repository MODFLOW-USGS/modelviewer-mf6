#if !defined(AFX_CROPOPTIONSPAGE_H__FB5923C2_010E_11D4_8108_00C04F61038F__INCLUDED_)
#define AFX_CROPOPTIONSPAGE_H__FB5923C2_010E_11D4_8108_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CropOptionsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCropOptionsPage dialog
class CMvDoc;
class CCropDlg;

class CCropOptionsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCropOptionsPage)

// Construction
public:
	CCropOptionsPage();
	~CCropOptionsPage();
	void Reinitialize();
	void Activate(BOOL b);
	void UpdateLabels();
	void OnDefault();
	CMvDoc *m_pDoc;
	CCropDlg *m_Parent;

// Dialog Data
	//{{AFX_DATA(CCropOptionsPage)
	enum { IDD = IDD_CROP_OPTIONS };
	CSliderCtrl	m_OpacitySlider;
	CSliderCtrl	m_BlueSlider;
	CSliderCtrl	m_GreenSlider;
	CSliderCtrl	m_RedSlider;
	int		m_Red;
	int		m_Green;
	int		m_Blue;
	int		m_Opacity;
	BOOL	m_ShowCroppedAwayPieces;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCropOptionsPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCropOptionsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnReleasedcaptureRedSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureGreenSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureBlueSlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureOpacitySlider(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnShowCroppedAway();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CROPOPTIONSPAGE_H__FB5923C2_010E_11D4_8108_00C04F61038F__INCLUDED_)
