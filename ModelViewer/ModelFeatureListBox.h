#if !defined(AFX_MODELFEATURELISTBOX_H__E9C87924_10CC_11D5_8157_00C04F61038F__INCLUDED_)
#define AFX_MODELFEATURELISTBOX_H__E9C87924_10CC_11D5_8157_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelFeatureListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModelFeatureListBox window

class CMvDoc;

class CModelFeatureListBox : public CListBox
{
// Construction
public:
	CModelFeatureListBox();

// Attributes
public:
	int m_Type;
	CMvDoc *m_pDoc;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelFeatureListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CModelFeatureListBox();

protected:

	// Generated message map functions
protected:
	//{{AFX_MSG(CModelFeatureListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELFEATURELISTBOX_H__E9C87924_10CC_11D5_8157_00C04F61038F__INCLUDED_)
