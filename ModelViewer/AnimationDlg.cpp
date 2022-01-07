// AnimationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ModelViewer.h"
#include "AnimationDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimationDlg dialog


CAnimationDlg::CAnimationDlg(CWnd* pParent, CMvDoc *pDoc)
	: CDialog(CAnimationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnimationDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_pDoc = pDoc;
	m_nID = CAnimationDlg::IDD;
	m_PropertySheet = new CPropertySheet;
	m_ControlsPage = new CAnimationControlsPage;
	m_OptionsPage = new CAnimationOptionsPage;
	m_ControlsPage->m_pDoc = pDoc;
	m_ControlsPage->m_OptionsPage = m_OptionsPage;
	m_PropertySheet->AddPage(m_ControlsPage);
	m_PropertySheet->AddPage(m_OptionsPage);
}

CAnimationDlg::~CAnimationDlg()
{
	delete m_PropertySheet;
	delete m_ControlsPage;
	delete m_OptionsPage;
}


void CAnimationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimationDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimationDlg, CDialog)
	//{{AFX_MSG_MAP(CAnimationDlg)
	ON_BN_CLICKED(IDC_DONE, OnDone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimationDlg message handlers

BOOL CAnimationDlg::Create()
{
	if (!CDialog::Create(m_nID, m_pParent))
	{
		return FALSE;
	}

	m_PropertySheet->Create(this, WS_CHILD | WS_VISIBLE, 0);

	CRect rcSheet;
	GetDlgItem( IDC_PROP_SHEET )->GetWindowRect( &rcSheet );
	ScreenToClient( &rcSheet );
        
	CTabCtrl* tabctrl = m_PropertySheet->GetTabControl();
	tabctrl->MoveWindow(0, 0, rcSheet.Width(), rcSheet.Height());
	m_PropertySheet->SetWindowPos( NULL, rcSheet.left, rcSheet.top, 
									rcSheet.Width(), rcSheet.Height(),
									SWP_NOZORDER | SWP_NOACTIVATE );

	m_PropertySheet->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	m_PropertySheet->ModifyStyle(0, WS_TABSTOP);
	// Force the controls on the property pages to be created
	m_PropertySheet->SetActivePage(1);
	m_PropertySheet->SetActivePage(0);

	return TRUE;
}

void CAnimationDlg::PostNcDestroy() 
{
	delete this;
}

BOOL CAnimationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	WINDOWPLACEMENT wndpl;
	GetWindowPlacement(&wndpl);
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	int dlgWidth = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
	int dlgHeight = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
	wndpl.rcNormalPosition.right = screenWidth - 480;
	wndpl.rcNormalPosition.left = wndpl.rcNormalPosition.right - dlgWidth;
	wndpl.rcNormalPosition.top = 10;
	wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgHeight;
	wndpl.showCmd = SW_HIDE;
	SetWindowPlacement(&wndpl);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnimationDlg::Reinitialize()
{
	m_ControlsPage->Reinitialize();
	m_OptionsPage->Reinitialize();
}

void CAnimationDlg::OnDone() 
{
	if (m_pDoc->IsAnimating())
	{
		m_pDoc->StopAnimation();
	}
	ShowWindow(SW_HIDE);
}
