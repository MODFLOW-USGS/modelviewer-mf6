// ColorBarDlg.cpp : implementation file
//

#include "ModelViewer.h"
#include "ColorBarDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorBarDlg dialog


CColorBarDlg::CColorBarDlg(CWnd* pParent, CMvDoc *pDoc)
	: CDialog(CColorBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorBarDlg)
	//}}AFX_DATA_INIT

	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_pDoc = pDoc;
	m_nID = CColorBarDlg::IDD;
	m_PropertySheet = new CPropertySheet;
	m_LimitsPage = new CColorBarLimitsPage;
	m_LimitsPage->m_pDoc = pDoc;
	m_SizePage = new CColorBarSizePage;
	m_SizePage->m_pDoc = pDoc;
	m_TextPage = new CColorBarTextPage;
	m_TextPage->m_pDoc = pDoc;
	m_ColorsPage = new CColorBarColorsPage;
	m_ColorsPage->m_pDoc = pDoc;
	m_DataSource = new CColorBarDataSource;
	m_DataSource->m_pDoc = pDoc;

	m_PropertySheet->AddPage(m_DataSource);
	m_PropertySheet->AddPage(m_LimitsPage);
	m_PropertySheet->AddPage(m_SizePage);
	m_PropertySheet->AddPage(m_TextPage);
	m_PropertySheet->AddPage(m_ColorsPage);
}

CColorBarDlg::~CColorBarDlg()
{
	delete m_PropertySheet;
	delete m_LimitsPage;
	delete m_SizePage;
	delete m_TextPage;
	delete m_ColorsPage;
	delete m_DataSource;
}


void CColorBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorBarDlg)
	DDX_Control(pDX, IDC_DEFAULT, m_DefaultButton);
	DDX_Control(pDX, IDC_APPLY, m_ApplyButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorBarDlg, CDialog)
	//{{AFX_MSG_MAP(CColorBarDlg)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_DONE, OnDone)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorBarDlg message handlers

BOOL CColorBarDlg::Create()
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
	m_PropertySheet->SetActivePage(4);
	m_PropertySheet->SetActivePage(3);
	m_PropertySheet->SetActivePage(2);
	m_PropertySheet->SetActivePage(1);
	m_PropertySheet->SetActivePage(0);

	return TRUE;
}

void CColorBarDlg::PostNcDestroy() 
{
	delete this;
}

BOOL CColorBarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	WINDOWPLACEMENT wndpl;
	GetWindowPlacement(&wndpl);
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	int dlgWidth = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
	int dlgHeight = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
	wndpl.rcNormalPosition.right = screenWidth - 10;
	wndpl.rcNormalPosition.left = wndpl.rcNormalPosition.right - dlgWidth;
	wndpl.rcNormalPosition.top = 10;
	wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgHeight;
	wndpl.showCmd = SW_HIDE;
	SetWindowPlacement(&wndpl);

	m_ApplyButton.EnableWindow(FALSE);
	m_DefaultButton.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColorBarDlg::Reinitialize()
{
	m_DataSource->Reinitialize();
	m_LimitsPage->Reinitialize();
	m_SizePage->Reinitialize();
	m_TextPage->Reinitialize();
	m_ColorsPage->Reinitialize();
	m_ApplyButton.EnableWindow(FALSE);
	m_DefaultButton.EnableWindow(FALSE);
}

void CColorBarDlg::Activate(BOOL b)
{
	m_DataSource->Activate(b);
	m_LimitsPage->Activate(b);
	m_SizePage->Activate(b);
	m_TextPage->Activate(b);
	m_ColorsPage->Activate(b);
	m_ApplyButton.EnableWindow(b);
	m_DefaultButton.EnableWindow(b);
}

void CColorBarDlg::OnDefault() 
{

	if (AfxMessageBox("Do you want to use default settings?", MB_YESNO) == IDNO)
	{
		return;
	}

	switch (m_PropertySheet->GetActiveIndex())
	{
	case 0:
		m_DataSource->OnDefault();
		break;
	case 1:
		m_LimitsPage->OnDefault();
		break;
	case 2:
		m_SizePage->OnDefault();
		break;
	case 3:
		m_TextPage->OnDefault();
		break;
	case 4:
		m_ColorsPage->OnDefault();
		break;
	}
}

void CColorBarDlg::OnApply() 
{
	switch(m_PropertySheet->GetActiveIndex())
	{
	case 0:
		m_DataSource->Apply();
		break;
	case 1:
		m_LimitsPage->Apply();
		break;
	case 2:
		m_SizePage->Apply();
		break;
	case 3:
		m_TextPage->Apply();
		break;
	case 4:
		m_ColorsPage->Apply();
		break;
	}
}

void CColorBarDlg::OnDone() 
{
	ShowWindow(SW_HIDE);
}

BOOL CColorBarDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// Inactivates help when F1 is pressed	
	//return CDialog::OnHelpInfo(pHelpInfo);
	return 0;
}
