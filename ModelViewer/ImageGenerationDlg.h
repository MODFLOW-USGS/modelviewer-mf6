#if !defined(AFX_IMAGEGENERATIONDLG_H__664AD1B7_E987_11D3_8103_00C04F61038F__INCLUDED_)
#define AFX_IMAGEGENERATIONDLG_H__664AD1B7_E987_11D3_8103_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageGenerationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageGenerationDlg dialog

class CImageGenerationDlg : public CDialog
{
// Construction
public:
	CImageGenerationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImageGenerationDlg)
	enum { IDD = IDD_IMAGE_GENERATION };
	int		m_ImageQuality;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageGenerationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImageGenerationDlg)
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEGENERATIONDLG_H__664AD1B7_E987_11D3_8103_00C04F61038F__INCLUDED_)
