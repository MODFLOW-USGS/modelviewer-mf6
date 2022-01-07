#if !defined(AFX_AXESDLG_H__FAA2F13A_6F0A_4626_99A4_7D3E71C56254__INCLUDED_)
#define AFX_AXESDLG_H__FAA2F13A_6F0A_4626_99A4_7D3E71C56254__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AxesDlg.h : header file
//

#include "AxesPage.h"

/////////////////////////////////////////////////////////////////////////////
// CAxesDlg dialog

class CMvDoc;

class CAxesDlg : public CDialog
{
// Construction
public:
	CAxesDlg(CWnd* pParent, CMvDoc *pDoc);  
	~CAxesDlg();
	BOOL Create();
	void Reinitialize();
	void Activate();

	CPropertySheet *m_PropertySheet;
	CAxesPage *m_AxesPage;

// Dialog Data
	//{{AFX_DATA(CAxesDlg)
	enum { IDD = IDD_AXES_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMvDoc *m_pDoc;
	CWnd* m_pParent;
	int m_nID;

	// Generated message map functions
	//{{AFX_MSG(CAxesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDone();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXESDLG_H__FAA2F13A_6F0A_4626_99A4_7D3E71C56254__INCLUDED_)
