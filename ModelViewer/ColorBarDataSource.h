#if !defined(AFX_COLORBARDATASOURCE_H__49E98A36_FAAA_4F09_ADF5_1999150C4584__INCLUDED_)
#define AFX_COLORBARDATASOURCE_H__49E98A36_FAAA_4F09_ADF5_1999150C4584__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorBarDataSource.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorBarDataSource dialog
class CMvDoc;

class CColorBarDataSource : public CPropertyPage
{
	DECLARE_DYNCREATE(CColorBarDataSource)

// Construction
public:
	CColorBarDataSource();
	~CColorBarDataSource();
	void Reinitialize();
	void Activate(BOOL b);
	void OnDefault();
	void Apply();
	BOOL CustomUpdateData(BOOL b);
	BOOL	m_ExchangeData;
	CMvDoc *m_pDoc;

// Dialog Data
	//{{AFX_DATA(CColorBarDataSource)
	enum { IDD = IDD_COLOR_BAR_SOURCE };
	int		m_DataSourceIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CColorBarDataSource)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CColorBarDataSource)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBARDATASOURCE_H__49E98A36_FAAA_4F09_ADF5_1999150C4584__INCLUDED_)
