#if !defined(AFX_GEOMETRYDLG_H__B7D1E3C1_F5E8_11D3_8106_00C04F61038F__INCLUDED_)
#define AFX_GEOMETRYDLG_H__B7D1E3C1_F5E8_11D3_8106_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeometryDlg.h : header file
//
#include "ScalePage.h"
#include "AxesPage.h"
#include "BoundingBoxPage.h"

/////////////////////////////////////////////////////////////////////////////
// CGeometryDlg dialog
class CMvDoc;

class CGeometryDlg : public CDialog
{
// Construction
public:
	CGeometryDlg(CWnd* pParent, CMvDoc *pDoc);
	~CGeometryDlg();
	BOOL Create();
	void Reinitialize();
	void Activate(BOOL b);

	CPropertySheet *m_PropertySheet;
	CScalePage *m_ScalePage;
	CAxesPage *m_AxesPage;
	CBoundingBoxPage *m_BoundingBoxPage;

// Dialog Data
	//{{AFX_DATA(CGeometryDlg)
	enum { IDD = IDD_GEOMETRY };
	CButton	m_ApplyButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGeometryDlg)
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
	//{{AFX_MSG(CGeometryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnDone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GEOMETRYDLG_H__B7D1E3C1_F5E8_11D3_8106_00C04F61038F__INCLUDED_)
