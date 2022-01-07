// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ModelViewer.h"
#include "DisplaySizeDlg.h"

#include "MainFrm.h"
#include "HtmlHelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_DISPLAY_SIZE, OnDisplaySize)
	ON_COMMAND(ID_HELP_CONTENTS, OnHelpContents)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MODEL_NAME, OnUpdateModelName)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_DATA_NAME, OnUpdateDataName)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_MODEL_NAME,
	ID_INDICATOR_DATA_NAME,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnDisplaySize() 
{
	RECT clientRect;
	GetActiveView()->GetClientRect(&clientRect);

	CDisplaySizeDlg dlg;
	dlg.m_DisplayWidth = clientRect.right;
	dlg.m_DisplayHeight = clientRect.bottom;
	if (dlg.DoModal() == IDOK)
	{
		RECT wndRect;
		GetWindowRect(&wndRect);
		int borderWidth = wndRect.right - wndRect.left - clientRect.right;
		int borderHeight = wndRect.bottom - wndRect.top - clientRect.bottom;
		int newWndWidth = dlg.m_DisplayWidth + borderWidth;
		int newWndHeight = dlg.m_DisplayHeight + borderHeight;
		BOOL result = SetWindowPos(&wndTop, 0, 0, newWndWidth, newWndHeight, SWP_NOMOVE);
	}
}

void CMainFrame::OnUpdateModelName(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateDataName(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnHelpContents() 
{
	// Activates HTML Help
	if (!::HtmlHelp(m_hWnd, AfxGetApp()->m_pszHelpFilePath, HH_DISPLAY_TOC, NULL))
	{
		AfxMessageBox("Cannot find the help file or topic.");
	}
	
}
