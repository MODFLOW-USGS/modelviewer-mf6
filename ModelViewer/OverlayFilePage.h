#if !defined(AFX_OVERLAYFILEPAGE_H__7AB3C07D_9E00_45EE_8AFC_DF45068B05FE__INCLUDED_)
#define AFX_OVERLAYFILEPAGE_H__7AB3C07D_9E00_45EE_8AFC_DF45068B05FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OverlayFilePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COverlayFilePage dialog

class COverlayFilePage : public CPropertyPage
{
	DECLARE_DYNCREATE(COverlayFilePage)

// Construction
public:
	COverlayFilePage();
	~COverlayFilePage();
	void Reinitialize();
	void Activate(BOOL b);

// Dialog Data
	//{{AFX_DATA(COverlayFilePage)
	enum { IDD = IDD_OVERLAY_FILE };
	CComboBox	m_TypeComboBox;
	CString	m_Filename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COverlayFilePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COverlayFilePage)
	afx_msg void OnBrowseOverlay();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OVERLAYFILEPAGE_H__7AB3C07D_9E00_45EE_8AFC_DF45068B05FE__INCLUDED_)
