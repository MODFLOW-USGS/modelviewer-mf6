#if !defined(AFX_ISOSURFACEDLG_H__B7D1E3C3_F5E8_11D3_8106_00C04F61038F__INCLUDED_)
#define AFX_ISOSURFACEDLG_H__B7D1E3C3_F5E8_11D3_8106_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IsosurfaceDlg.h : header file
//

#include "RegularIsosurfacePage.h"
#include "CustomIsosurfacePage.h"

/////////////////////////////////////////////////////////////////////////////
// CIsosurfaceDlg dialog
class CMvDoc;

class CIsosurfaceDlg : public CDialog
{
// Construction
public:
	CIsosurfaceDlg(CWnd* pParent, CMvDoc *pDoc);
	~CIsosurfaceDlg();
	BOOL Create();
	void Reinitialize();
	void Activate(BOOL b);

	CPropertySheet *m_PropertySheet;
	CRegularIsosurfacePage *m_RegularIsosurfacePage;
	CCustomIsosurfacePage *m_CustomIsosurfacePage;

// Dialog Data
	//{{AFX_DATA(CIsosurfaceDlg)
	enum { IDD = IDD_ISOSURFACE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIsosurfaceDlg)
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
	//{{AFX_MSG(CIsosurfaceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnDone();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISOSURFACEDLG_H__B7D1E3C3_F5E8_11D3_8106_00C04F61038F__INCLUDED_)
