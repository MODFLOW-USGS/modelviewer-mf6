// VectorDlg.cpp : implementation file
//

#include "ModelViewer.h"
#include "VectorDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVectorDlg dialog


CVectorDlg::CVectorDlg(CWnd* pParent, CMvDoc *pDoc)
	: CDialog(CVectorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVectorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_pDoc = pDoc;
	m_nID = CVectorDlg::IDD;
	m_PropertySheet = new CPropertySheet;
	m_ControlsPage = new CVectorControlsPage;
	m_OptionsPage = new CVectorOptionsPage;
	m_ThresholdPage = new CVectorThresholdPage;
	m_CropVectorsPage = new CCropVectorsPage;
	m_ControlsPage->m_pDoc = pDoc;
	m_OptionsPage->m_pDoc = pDoc;
	m_ThresholdPage->m_pDoc = pDoc;
	m_CropVectorsPage->m_pDoc = pDoc;
	m_PropertySheet->AddPage(m_ControlsPage);
	m_PropertySheet->AddPage(m_OptionsPage);
	m_PropertySheet->AddPage(m_ThresholdPage);
	m_PropertySheet->AddPage(m_CropVectorsPage);
}

CVectorDlg::~CVectorDlg()
{
	delete m_PropertySheet;
	delete m_ControlsPage;
	delete m_OptionsPage;
	delete m_ThresholdPage;
	delete m_CropVectorsPage;
}

void CVectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVectorDlg)
	DDX_Control(pDX, IDC_DEFAULT, m_DefaultButton);
	DDX_Control(pDX, IDC_APPLY, m_ApplyButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVectorDlg, CDialog)
	//{{AFX_MSG_MAP(CVectorDlg)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_DONE, OnDone)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVectorDlg message handlers

BOOL CVectorDlg::Create()
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
	m_PropertySheet->SetActivePage(3);
	m_PropertySheet->SetActivePage(2);
	m_PropertySheet->SetActivePage(1);
	m_PropertySheet->SetActivePage(0);

	return TRUE;
}

void CVectorDlg::PostNcDestroy() 
{
	delete this;
}

BOOL CVectorDlg::OnInitDialog() 
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
	wndpl.rcNormalPosition.top = screenHeight - dlgHeight - 30;
	wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgHeight;
	wndpl.showCmd = SW_HIDE;
	SetWindowPlacement(&wndpl);

	m_ApplyButton.EnableWindow(FALSE);
	m_DefaultButton.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVectorDlg::Reinitialize()
{
	m_ControlsPage->Reinitialize();
	m_OptionsPage->Reinitialize();
	m_ThresholdPage->Reinitialize();
	m_CropVectorsPage->Reinitialize();
	m_ApplyButton.EnableWindow(FALSE);
	m_DefaultButton.EnableWindow(FALSE);
}

void CVectorDlg::Activate(BOOL b)
{
	m_ControlsPage->Activate(b);
	m_OptionsPage->Activate(b);
	m_ThresholdPage->Activate(b);
	m_CropVectorsPage->Activate(b);
	m_ApplyButton.EnableWindow(b);
	m_DefaultButton.EnableWindow(b);
}

void CVectorDlg::OnApply() 
{
	switch(m_PropertySheet->GetActiveIndex())
	{
	case 0:
		m_ControlsPage->Apply();
		break;
	case 1:
		m_OptionsPage->Apply();
		break;
	case 2:
		m_ThresholdPage->Apply();
		break;
	case 3:
		m_CropVectorsPage->Apply();
		break;
	}
}

void CVectorDlg::OnDone() 
{
	ShowWindow(SW_HIDE);
}

void CVectorDlg::OnDefault() 
{
	if (AfxMessageBox("Do you want to use default settings?", MB_YESNO) == IDNO)
	{
		return;
	}
	switch(m_PropertySheet->GetActiveIndex())
	{
	case 0:
		m_ControlsPage->OnDefault();
		break;
	case 1:
		m_OptionsPage->OnDefault();
		break;
	case 2:
		m_ThresholdPage->OnDefault();
		break;
	case 3:
		m_CropVectorsPage->OnDefault();
		break;
	}
}
