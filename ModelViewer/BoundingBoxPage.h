#if !defined(AFX_BOUNDINGBOXPAGE_H__B857EA37_F928_11D3_8107_00C04F61038F__INCLUDED_)
#define AFX_BOUNDINGBOXPAGE_H__B857EA37_F928_11D3_8107_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BoundingBoxPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBoundingBoxPage dialog
class CMvDoc;
class CGeometryDlg;

class CBoundingBoxPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CBoundingBoxPage)

// Construction
public:
	CBoundingBoxPage();
	~CBoundingBoxPage();
	void Reinitialize();
	void Activate(BOOL b);
	CMvDoc *m_pDoc;
	CGeometryDlg *m_Parent;

// Dialog Data
	//{{AFX_DATA(CBoundingBoxPage)
	enum { IDD = IDD_BOUNDING_BOX };
	int		m_ColorOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBoundingBoxPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBoundingBoxPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBlack();
	afx_msg void OnGray();
	afx_msg void OnWhite();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOUNDINGBOXPAGE_H__B857EA37_F928_11D3_8107_00C04F61038F__INCLUDED_)
