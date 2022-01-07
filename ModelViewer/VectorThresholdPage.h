#if !defined(AFX_VECTORTHRESHOLDPAGE_H__7CE8DB8F_05F5_11D4_8108_00C04F61038F__INCLUDED_)
#define AFX_VECTORTHRESHOLDPAGE_H__7CE8DB8F_05F5_11D4_8108_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VectorThresholdPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVectorThresholdPage dialog
class CMvDoc;

class CVectorThresholdPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CVectorThresholdPage)

// Construction
public:
	CVectorThresholdPage();
	~CVectorThresholdPage();
	void Reinitialize();
	void Activate(BOOL b);
	BOOL CustomUpdateData(BOOL b);
	void Apply();
	void OnDefault();
	CMvDoc *m_pDoc;

	BOOL m_ApplyThreshold;
	double m_VectorMin;
	double m_VectorMax;

// Dialog Data
	//{{AFX_DATA(CVectorThresholdPage)
	enum { IDD = IDD_VECTOR_THRESHOLD };
	CButton	m_VectorThresholdCheckBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CVectorThresholdPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_ExchangeData;

	// Generated message map functions
	//{{AFX_MSG(CVectorThresholdPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnApplyVectorThreshold();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VECTORTHRESHOLDPAGE_H__7CE8DB8F_05F5_11D4_8108_00C04F61038F__INCLUDED_)
