// GeometryDlg.cpp : implementation file
//

#include "ModelViewer.h"
#include "GeometryDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeometryDlg dialog


CGeometryDlg::CGeometryDlg(CWnd* pParent, CMvDoc *pDoc)
	: CDialog(CGeometryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGeometryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_pDoc = pDoc;
	m_nID = CGeometryDlg::IDD;
	m_PropertySheet = new CPropertySheet;
	m_ScalePage = new CScalePage;
	m_ScalePage->m_pDoc = pDoc;
	m_ScalePage->m_Parent = this;
	m_AxesPage = new CAxesPage;
	m_AxesPage->m_pDoc = pDoc;
	m_AxesPage->m_Parent = this;
	m_BoundingBoxPage = new CBoundingBoxPage;
	m_BoundingBoxPage->m_pDoc = pDoc;
	m_BoundingBoxPage->m_Parent = this;
	m_PropertySheet->AddPage(m_ScalePage);
	m_PropertySheet->AddPage(m_AxesPage);
	m_PropertySheet->AddPage(m_BoundingBoxPage);
}

CGeometryDlg::~CGeometryDlg()
{
	delete m_PropertySheet;
	delete m_ScalePage;
	delete m_AxesPage;
	delete m_BoundingBoxPage;
}


void CGeometryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeometryDlg)
	DDX_Control(pDX, IDC_APPLY, m_ApplyButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeometryDlg, CDialog)
	//{{AFX_MSG_MAP(CGeometryDlg)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_DONE, OnDone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeometryDlg message handlers

BOOL CGeometryDlg::Create()
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

void CGeometryDlg::PostNcDestroy() 
{
	delete this;
}

BOOL CGeometryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	WINDOWPLACEMENT wndpl;
	GetWindowPlacement(&wndpl);
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	int dlgWidth = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
	int dlgHeight = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
	wndpl.rcNormalPosition.left = 400;
	wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left + dlgWidth;
	wndpl.rcNormalPosition.top = screenHeight - dlgHeight - 30;
	wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgHeight;
	wndpl.showCmd = SW_HIDE;
	SetWindowPlacement(&wndpl);
	m_ApplyButton.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGeometryDlg::Reinitialize()
{
	m_ScalePage->Reinitialize();
	m_AxesPage->Reinitialize();
	m_BoundingBoxPage->Reinitialize();
	m_ApplyButton.EnableWindow(FALSE);
}

void CGeometryDlg::Activate(BOOL b)
{
	m_ScalePage->Activate(b);
	m_AxesPage->Activate(b);
	m_BoundingBoxPage->Activate(b);
}

void CGeometryDlg::OnApply() 
{
	switch(m_PropertySheet->GetActiveIndex())
	{
	case 0:
		m_ScalePage->Apply();
		break;
	case 1:
		m_AxesPage->Apply();
		break;
	}
}

void CGeometryDlg::OnDone() 
{
	ShowWindow(SW_HIDE);
}
