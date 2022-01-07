#if !defined(AFX_PATHLINESDLG_H__CF519F14_0804_11D4_8109_00C04F61038F__INCLUDED_)
#define AFX_PATHLINESDLG_H__CF519F14_0804_11D4_8109_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PathlinesDlg.h : header file
//

#include "PathlinesDisplayPage.h"
#include "PathlinesColorPage.h"
#include "PathlinesClippingPage.h"

/////////////////////////////////////////////////////////////////////////////
// CPathlinesDlg dialog
class CMvDoc;

class CPathlinesDlg : public CDialog
{
// Construction
public:
	CPathlinesDlg(CWnd* pParent, CMvDoc *pDoc);
	~CPathlinesDlg();
	BOOL Create();
	void Reinitialize();
	void Activate(BOOL b);

	CPropertySheet *m_PropertySheet;
	CPathlinesDisplayPage *m_DisplayPage;
	CPathlinesColorPage *m_ColorPage;
	CPathlinesClippingPage *m_ClipPage;

// Dialog Data
	//{{AFX_DATA(CPathlinesDlg)
	enum { IDD = IDD_PATHLINES };
	CButton	m_DefaultButton;
	CButton	m_ApplyButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPathlinesDlg)
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
	//{{AFX_MSG(CPathlinesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnDefault();
	afx_msg void OnDone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATHLINESDLG_H__CF519F14_0804_11D4_8109_00C04F61038F__INCLUDED_)
