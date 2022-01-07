#if !defined(AFX_EXPORTANIMATIONDLG_H__664AD1B9_E987_11D3_8103_00C04F61038F__INCLUDED_)
#define AFX_EXPORTANIMATIONDLG_H__664AD1B9_E987_11D3_8103_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportAnimationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExportAnimationDlg dialog

class CExportAnimationDlg : public CDialog
{
// Construction
public:
	CExportAnimationDlg(CWnd* pParent = NULL);   // standard constructor
	int m_NumberOfTimePoints;
	char **m_TimePointLabels;
	int m_SpaceUsagePerFile;
	BOOL m_Preview;

// Dialog Data
	//{{AFX_DATA(CExportAnimationDlg)
	enum { IDD = IDD_EXPORT_ANIMATION };
	double	m_Elevate;
	int		m_EndIndex;
	CString	m_FilePrefix;
	CString	m_FileStartNumber;
	CString	m_OutputFolder;
	double	m_Rotate;
	int		m_StartIndex;
	int		m_NumberOfSteps;
	int		m_AnimationType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportAnimationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExportAnimationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	afx_msg void OnPreview();
	afx_msg void OnSelchangeStartIndex();
	afx_msg void OnSelchangeEndIndex();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnSelchangeComboExportAnimationType();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void DoAnimationTypeChange();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTANIMATIONDLG_H__664AD1B9_E987_11D3_8103_00C04F61038F__INCLUDED_)
