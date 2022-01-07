#if !defined(AFX_SOLIDDLG_H__B7D1E3C5_F5E8_11D3_8106_00C04F61038F__INCLUDED_)
#define AFX_SOLIDDLG_H__B7D1E3C5_F5E8_11D3_8106_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SolidDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSolidDlg dialog
class CMvDoc;

class CSolidDlg : public CDialog
{
// Construction
public:
	CSolidDlg(CWnd* pParent, CMvDoc *pDoc);
	BOOL Create();
	void Reinitialize();
	void Activate(BOOL b);

// Dialog Data
	//{{AFX_DATA(CSolidDlg)
	enum { IDD = IDD_SOLID };
	double	m_SolidThresholdMin;
	double	m_SolidThresholdMax;
	BOOL	m_ApplyThreshold;
	int		m_SolidDisplayMode;
	int		m_NumberOfColorBands;
	//}}AFX_DATA
	int m_PrimaryScalarMode;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSolidDlg)
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
	//{{AFX_MSG(CSolidDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSolidDisplaySmooth();
	afx_msg void OnSolidDisplayBlocky();
	afx_msg void OnApplyThreshold();
	afx_msg void OnFullSolid();
	afx_msg void OnApply();
	afx_msg void OnDone();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSolidDisplayBanded();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOLIDDLG_H__B7D1E3C5_F5E8_11D3_8106_00C04F61038F__INCLUDED_)
