#if !defined(AFX_ANIMATIONCONTROLSPAGE_H__664AD1BC_E987_11D3_8103_00C04F61038F__INCLUDED_)
#define AFX_ANIMATIONCONTROLSPAGE_H__664AD1BC_E987_11D3_8103_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimationControlsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnimationControlsPage dialog
class CMvDoc;
class CAnimationOptionsPage;

class CAnimationControlsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAnimationControlsPage)

// Construction
public:
	CAnimationControlsPage();
	~CAnimationControlsPage();
	void Reinitialize();
	CMvDoc *m_pDoc;
	CAnimationOptionsPage *m_OptionsPage;
	int m_NumberOfTimePoints;
	char **m_TimePointLabels;
	int m_CurrentTimePointIndex;

    // public methods
	void Reset();
	void SetAndDisplayCurrentTime(int timePointIndex);
	void SetAnimationType(int Value); 


// Dialog Data
	//{{AFX_DATA(CAnimationControlsPage)
	enum { IDD = IDD_ANIMATION_CONTROLS };
	int		m_SetToIndex;
	int		m_NumberOfSteps;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAnimationControlsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAnimationControlsPage)
	afx_msg void OnRunAnimation();
	afx_msg void OnStopAnimation();
	afx_msg void OnAdvanceAnimation();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetToTimePoint();
	afx_msg void OnSelchangeComboAnimationType();
	afx_msg void OnChangeEditNumberOfSteps();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMATIONCONTROLSPAGE_H__664AD1BC_E987_11D3_8103_00C04F61038F__INCLUDED_)
