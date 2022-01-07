#if !defined(AFX_COLORBARDLG_H__D5753376_F623_11D3_8106_00C04F61038F__INCLUDED_)
#define AFX_COLORBARDLG_H__D5753376_F623_11D3_8106_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorBarDlg.h : header file
//
#include "ColorBarLimitsPage.h"
#include "ColorBarSizePage.h"
#include "ColorBarTextPage.h"
#include "ColorBarColorsPage.h"
#include "ColorBarDataSource.h"

/////////////////////////////////////////////////////////////////////////////
// CColorBarDlg dialog
class CMvDoc;

class CColorBarDlg : public CDialog
{
// Construction
public:
	CColorBarDlg(CWnd* pParent, CMvDoc *pDoc);
	~CColorBarDlg();
	BOOL Create();
	void Reinitialize();
	void Activate(BOOL b);

	CPropertySheet *m_PropertySheet;
	CColorBarLimitsPage *m_LimitsPage;
	CColorBarSizePage *m_SizePage;
	CColorBarTextPage *m_TextPage;
	CColorBarColorsPage *m_ColorsPage;
	CColorBarDataSource *m_DataSource;

// Dialog Data
	//{{AFX_DATA(CColorBarDlg)
	enum { IDD = IDD_COLOR_BAR };
	CButton	m_DefaultButton;
	CButton	m_ApplyButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorBarDlg)
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
	//{{AFX_MSG(CColorBarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDefault();
	afx_msg void OnApply();
	afx_msg void OnDone();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBARDLG_H__D5753376_F623_11D3_8106_00C04F61038F__INCLUDED_)
