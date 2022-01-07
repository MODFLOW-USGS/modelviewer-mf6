#if !defined(AFX_OVERLAYDLG_H__8EA39BE9_DCB0_451C_A23B_F4A61C50B304__INCLUDED_)
#define AFX_OVERLAYDLG_H__8EA39BE9_DCB0_451C_A23B_F4A61C50B304__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OverlayDlg.h : header file
//

#include "OverlayControlsPage.h"
#include "OverlayFilePage.h"
#include "OverlayBoundsPage.h"

/////////////////////////////////////////////////////////////////////////////
// COverlayDlg dialog
class CMvDoc;

class COverlayDlg : public CDialog
{
// Construction
public:
	COverlayDlg(CWnd* pParent, CMvDoc *pDoc);
	~COverlayDlg();
	BOOL Create();
	void Reinitialize();
	void Activate(BOOL b);
	void Apply() {OnApply();}	// public version of OnApply();

	CPropertySheet *m_PropertySheet;
	COverlayControlsPage *m_ControlsPage;
	COverlayFilePage *m_FilePage;
	COverlayBoundsPage *m_BoundsPage;

// Dialog Data
	//{{AFX_DATA(COverlayDlg)
	enum { IDD = IDD_OVERLAY };
	CButton	m_ApplyButton;
	CButton	m_RemoveButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COverlayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd* m_pParent;
	CMvDoc *m_pDoc;
	int m_nID;

	// Generated message map functions
	//{{AFX_MSG(COverlayDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnApply();
	afx_msg void OnDone();
	afx_msg void OnRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OVERLAYDLG_H__8EA39BE9_DCB0_451C_A23B_F4A61C50B304__INCLUDED_)
