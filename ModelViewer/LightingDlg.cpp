// LightingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ModelViewer.h"
#include "LightingDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightingDlg dialog


CLightingDlg::CLightingDlg(CWnd* pParent, CMvDoc *pDoc)
	: CDialog(CLightingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLightingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_pDoc = pDoc;
	m_nID = CLightingDlg::IDD;
	m_PropertySheet = new CPropertySheet;
	m_LightsPage = new CLightsPage;
	m_LightsPage->m_pDoc = pDoc;
	m_SurfacePage = new CSurfacePage;
	m_SurfacePage->m_pDoc = pDoc;
	m_BackgroundPage = new CBackgroundPage;
	m_BackgroundPage->m_pDoc = pDoc;
	m_PropertySheet->AddPage(m_LightsPage);
	m_PropertySheet->AddPage(m_SurfacePage);
	m_PropertySheet->AddPage(m_BackgroundPage);
}

CLightingDlg::~CLightingDlg()
{
	delete m_PropertySheet;
	delete m_SurfacePage;
	delete m_LightsPage;
	delete m_BackgroundPage;
}

void CLightingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLightingDlg)
	DDX_Control(pDX, IDC_DEFAULT, m_DefaultButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLightingDlg, CDialog)
	//{{AFX_MSG_MAP(CLightingDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_DONE, OnDone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightingDlg message handlers

BOOL CLightingDlg::Create()
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
	m_PropertySheet->SetActivePage(2);
	m_PropertySheet->SetActivePage(1);
	m_PropertySheet->SetActivePage(0);

	return TRUE;
}

void CLightingDlg::PostNcDestroy() 
{
	delete this;
}

BOOL CLightingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	WINDOWPLACEMENT wndpl;
	GetWindowPlacement(&wndpl);
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	int dlgWidth = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
	int dlgHeight = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
	wndpl.rcNormalPosition.left = 10;
	wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left + dlgWidth;
	wndpl.rcNormalPosition.top = screenHeight/3;
	wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgHeight;
	wndpl.showCmd = SW_HIDE;
	SetWindowPlacement(&wndpl);

	GetDlgItem(IDC_DEFAULT)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLightingDlg::Reinitialize()
{
	m_SurfacePage->Reinitialize();
	m_LightsPage->Reinitialize();
	m_BackgroundPage->Reinitialize();
	m_DefaultButton.EnableWindow(FALSE);
}

void CLightingDlg::Activate(BOOL b)
{
	m_SurfacePage->Activate(b);
	m_LightsPage->Activate(b);
	m_BackgroundPage->Activate(b);
	m_DefaultButton.EnableWindow(b);
}

void CLightingDlg::OnDefault() 
{
	if (AfxMessageBox("Do you want to use default settings?", MB_YESNO) == IDNO)
	{
		return;
	}

	switch (m_PropertySheet->GetActiveIndex())
	{
	case 0:
		m_LightsPage->OnDefault();
		break;
	case 1:
		m_SurfacePage->OnDefault();
		break;
	case 2:
		m_BackgroundPage->OnDefault();
		break;
	}
}

void CLightingDlg::OnDone() 
{
	ShowWindow(SW_HIDE);
}
