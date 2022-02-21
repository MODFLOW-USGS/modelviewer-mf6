// DataDlg.cpp : implementation file
//

#include "ModelViewer.h"
#include "DataDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataDlg dialog


CDataDlg::CDataDlg(CWnd* pParent, CMvDoc * pDoc)
	: CDialog(CDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_nID = CDataDlg::IDD;
	m_pDoc = pDoc;
	m_PropertySheet = new CPropertySheet;
	m_ScalarPage = new CScalarDataPage;
	m_VectorPage = new CVectorDataPage;
	m_PathlinePage = new CPathlineDataPage;
	m_ScalarPage->m_pDoc = pDoc;
	m_PropertySheet->AddPage(m_ScalarPage);
	m_PropertySheet->AddPage(m_VectorPage);
	m_PropertySheet->AddPage(m_PathlinePage);
}

CDataDlg::~CDataDlg()
{
	delete m_PropertySheet;
	delete m_ScalarPage;
	delete m_VectorPage;
	delete m_PathlinePage;
}


void CDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataDlg, CDialog)
	//{{AFX_MSG_MAP(CDataDlg)
	ON_BN_CLICKED(IDC_DONE, OnDone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataDlg message handlers

BOOL CDataDlg::Create()
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

void CDataDlg::PostNcDestroy() 
{
	delete this;
}

BOOL CDataDlg::OnInitDialog() 
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
	wndpl.rcNormalPosition.top = screenHeight - dlgHeight - 30;
	wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgHeight;
	wndpl.showCmd = SW_HIDE;
	SetWindowPlacement(&wndpl);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDataDlg::Reinitialize()
{
	m_ScalarPage->Reinitialize();
	m_VectorPage->Reinitialize();
	m_PathlinePage->Reinitialize();
}

void CDataDlg::Activate(BOOL b)
{
	m_ScalarPage->Activate(b);
}

void CDataDlg::OnDone() 
{
	ShowWindow(SW_HIDE);
}
