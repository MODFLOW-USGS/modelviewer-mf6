#if !defined(AFX_DATASELECTIONDLG_H__B2AD1384_EF16_11D3_8105_00C04F61038F__INCLUDED_)
#define AFX_DATASELECTIONDLG_H__B2AD1384_EF16_11D3_8105_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataSelectionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataSelectionDlg dialog

class CDataSelectionDlg : public CDialog
{
// Construction
public:
	CDataSelectionDlg(CWnd* pParent = NULL);   // standard constructor

	int m_NumberOfTimePoints;
	char **m_TimePointLabels;
	int m_NumberOfScalarDataTypes;
	char **m_DataTypeLabels;
	int m_TimeLabelOption;
	int m_InitialDisplayTimePoint;

// Dialog Data
	//{{AFX_DATA(CDataSelectionDlg)
	enum { IDD = IDD_DATA_SELECTION };
	int		m_SelectedDataType;
	int		m_SelectedTimePoint;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataSelectionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDataSelectionDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATASELECTIONDLG_H__B2AD1384_EF16_11D3_8105_00C04F61038F__INCLUDED_)
