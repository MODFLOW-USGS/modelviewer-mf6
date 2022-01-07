#if !defined(AFX_VECTORDLG_H__9D276238_0011_11D4_8108_00C04F61038F__INCLUDED_)
#define AFX_VECTORDLG_H__9D276238_0011_11D4_8108_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VectorDlg.h : header file
//
#include "VectorControlsPage.h"
#include "VectorOptionsPage.h"
#include "VectorThresholdPage.h"
#include "CropVectorsPage.h"

/////////////////////////////////////////////////////////////////////////////
// CVectorDlg dialog
class CMvDoc;

class CVectorDlg : public CDialog
{
// Construction
public:
	CVectorDlg(CWnd* pParent, CMvDoc *pDoc);
	~CVectorDlg();
	BOOL Create();
	void Reinitialize();
	void Activate(BOOL b);

	CPropertySheet *m_PropertySheet;
	CVectorControlsPage *m_ControlsPage;
	CVectorOptionsPage *m_OptionsPage;
	CVectorThresholdPage *m_ThresholdPage;
	CCropVectorsPage *m_CropVectorsPage;

// Dialog Data
	//{{AFX_DATA(CVectorDlg)
	enum { IDD = IDD_VECTOR };
	CButton	m_DefaultButton;
	CButton	m_ApplyButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVectorDlg)
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
	//{{AFX_MSG(CVectorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnDone();
	afx_msg void OnDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VECTORDLG_H__9D276238_0011_11D4_8108_00C04F61038F__INCLUDED_)
