#if !defined(AFX_ANIMATIONDLG_H__664AD1BB_E987_11D3_8103_00C04F61038F__INCLUDED_)
#define AFX_ANIMATIONDLG_H__664AD1BB_E987_11D3_8103_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimationDlg.h : header file
//
#include "AnimationControlsPage.h"
#include "AnimationOptionsPage.h"

/////////////////////////////////////////////////////////////////////////////
// CAnimationDlg dialog
class CMvDoc;

class CAnimationDlg : public CDialog
{
// Construction
public:
	CAnimationDlg(CWnd* pParent, CMvDoc *pDoc);
	~CAnimationDlg();
	BOOL Create();
	void Reinitialize();

	CPropertySheet *m_PropertySheet;
	CAnimationControlsPage *m_ControlsPage;
	CAnimationOptionsPage *m_OptionsPage;

// Dialog Data
	//{{AFX_DATA(CAnimationDlg)
	enum { IDD = IDD_ANIMATION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd* m_pParent;
	int m_nID;
	CMvDoc *m_pDoc;

	// Generated message map functions
	//{{AFX_MSG(CAnimationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMATIONDLG_H__664AD1BB_E987_11D3_8103_00C04F61038F__INCLUDED_)
