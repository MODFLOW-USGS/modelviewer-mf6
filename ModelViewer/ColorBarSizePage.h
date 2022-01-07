#if !defined(AFX_COLORBARSIZEPAGE_H__4F439174_3644_11D4_811B_00C04F61038F__INCLUDED_)
#define AFX_COLORBARSIZEPAGE_H__4F439174_3644_11D4_811B_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorBarSizePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorBarSizePage dialog
class CMvDoc;

class CColorBarSizePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CColorBarSizePage)

// Construction
public:
	CColorBarSizePage();
	~CColorBarSizePage();
	void Reinitialize();
	void Activate(BOOL b);
	BOOL CustomUpdateData(BOOL b);
	void OnDefault();
	void Apply();
	CMvDoc *m_pDoc;
	BOOL	m_ExchangeData;
	int		m_Height;
	int		m_Offset;
	int		m_Width;

// Dialog Data
	//{{AFX_DATA(CColorBarSizePage)
	enum { IDD = IDD_COLOR_BAR_SIZE };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CColorBarSizePage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CColorBarSizePage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBARSIZEPAGE_H__4F439174_3644_11D4_811B_00C04F61038F__INCLUDED_)
