#if !defined(AFX_MODELFEATURESDLG_H__8DE021E0_1B8A_11D4_9D64_B7DA87A18229__INCLUDED_)
#define AFX_MODELFEATURESDLG_H__8DE021E0_1B8A_11D4_9D64_B7DA87A18229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelFeaturesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModelFeaturesDlg dialog
#include "ModelFeatureListBox.h"

class CMvDoc;

class CModelFeaturesDlg : public CDialog
{
// Construction
public:
	CModelFeaturesDlg(CWnd* pParent, CMvDoc *pDoc);
	BOOL Create();
	void Reinitialize();
	void Activate(BOOL b);
	void ShowGlyphSizeControl(BOOL b);
	void SetNumberOfModelFeatureTypes(int n);
	void SetDisplayOrder(int *displayOrder);
	CModelFeatureListBox	m_HideListBox;
	CModelFeatureListBox	m_ShowListBox;

// Dialog Data
	//{{AFX_DATA(CModelFeaturesDlg)
	enum { IDD = IDD_MODEL_FEATURES };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelFeaturesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd* m_pParent;
	int m_nID;
	CMvDoc *m_pDoc;
	int m_NumberOfModelFeatureTypes;
	int *m_DisplayOrder;

	// Generated message map functions
	//{{AFX_MSG(CModelFeaturesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDone();
	afx_msg void OnSelchangeShowList();
	afx_msg void OnShowFeature();
	afx_msg void OnHideFeature();
	afx_msg void OnMoveToTop();
	afx_msg void OnMoveToBot();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();
	afx_msg void OnSelchangeHideList();
	afx_msg void OnDeltaposModelFeatureSize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnModelFeatureColor();
	afx_msg void OnDblClkHideList();
	afx_msg void OnDblClkShowList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELFEATURESDLG_H__8DE021E0_1B8A_11D4_9D64_B7DA87A18229__INCLUDED_)
