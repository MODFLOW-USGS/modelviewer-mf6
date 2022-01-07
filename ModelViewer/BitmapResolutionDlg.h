#if !defined(AFX_BITMAPRESOLUTIONDLG_H__664AD1B8_E987_11D3_8103_00C04F61038F__INCLUDED_)
#define AFX_BITMAPRESOLUTIONDLG_H__664AD1B8_E987_11D3_8103_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BitmapResolutionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitmapResolutionDlg dialog

class CBitmapResolutionDlg : public CDialog
{
// Construction
public:
	CBitmapResolutionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBitmapResolutionDlg)
	enum { IDD = IDD_BITMAP_RESOLUTION };
	int		m_SideOption;
	int		m_ResolutionOption;
	//}}AFX_DATA

	double m_WidthInInches;
	double m_HeightInInches;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapResolutionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBitmapResolutionDlg)
	afx_msg void OnBmpHeightOption();
	afx_msg void OnBmpWidthOption();
	afx_msg void OnResolution150();
	afx_msg void OnResolution300();
	afx_msg void OnResolutionScreen();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAPRESOLUTIONDLG_H__664AD1B8_E987_11D3_8103_00C04F61038F__INCLUDED_)
