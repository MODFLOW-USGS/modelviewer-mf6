#if !defined(AFX_DATADLG_H__68D5DFEC_0812_11D4_8109_00C04F61038F__INCLUDED_)
#define AFX_DATADLG_H__68D5DFEC_0812_11D4_8109_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataDlg dialog
#include "ScalarDataPage.h"
#include "VectorDataPage.h"
#include "PathlineDataPage.h"

class CMvDoc;

class CDataDlg : public CDialog
{
// Construction
public:
	CDataDlg(CWnd* pParent, CMvDoc *pDoc);   // standard constructor
	~CDataDlg();
	BOOL Create();
	void Activate(BOOL b);
	void Reinitialize();

	CPropertySheet *m_PropertySheet;
	CScalarDataPage *m_ScalarPage;
	CVectorDataPage *m_VectorPage;
	CPathlineDataPage *m_PathlinePage;

// Dialog Data
	//{{AFX_DATA(CDataDlg)
	enum { IDD = IDD_DATA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataDlg)
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
	//{{AFX_MSG(CDataDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATADLG_H__68D5DFEC_0812_11D4_8109_00C04F61038F__INCLUDED_)
