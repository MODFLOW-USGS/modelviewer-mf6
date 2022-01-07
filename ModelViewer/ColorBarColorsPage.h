#if !defined(AFX_COLORBARCOLORSPAGE_H__9B69A1E1_D694_4E2A_8B4A_9F713ACE40A7__INCLUDED_)
#define AFX_COLORBARCOLORSPAGE_H__9B69A1E1_D694_4E2A_8B4A_9F713ACE40A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ColorBarPreviewListBox.h"
// ColorBarColorsPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorBarColorsPage dialog
class CMvDoc;

class CColorBarColorsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CColorBarColorsPage)

// Construction
public:
	CColorBarColorsPage();
	~CColorBarColorsPage();
	void Reinitialize();
	void Activate(BOOL b);
	void OnDefault();
	void Apply();
	void InitializeDialog();

	CMvDoc *m_pDoc;
	COLORREF m_FirstColor;
	COLORREF m_LastColor;
// Dialog Data
	//{{AFX_DATA(CColorBarColorsPage)
	enum { IDD = IDD_COLOR_BAR_COLORS };
	CColorBarPreviewListBox	m_Last;
	CColorBarPreviewListBox	m_First;
	CComboBox	m_ColorSchemeCombo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CColorBarColorsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CColorBarColorsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboColorScheme();
	afx_msg void OnLowerColorButton();
	afx_msg void OnUpperColorButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBARCOLORSPAGE_H__9B69A1E1_D694_4E2A_8B4A_9F713ACE40A7__INCLUDED_)
