// ModelViewer.h : main header file for the MODELVIEWER application
//

#if !defined(AFX_MODELVIEWER_H__D3A07DD4_F3BF_11D3_8105_00C04F61038F__INCLUDED_)
#define AFX_MODELVIEWER_H__D3A07DD4_F3BF_11D3_8105_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error There's a problem with the cmake target_precompile_headers
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CModelViewerApp:
// See ModelViewer.cpp for the implementation of this class
//

class CModelViewerApp : public CWinApp
{
protected:
	BOOL m_Startup;

public:
	CModelViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelViewerApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CModelViewerApp)
	afx_msg void OnAppAbout();
	afx_msg void OnCustomFileClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELVIEWER_H__D3A07DD4_F3BF_11D3_8105_00C04F61038F__INCLUDED_)
