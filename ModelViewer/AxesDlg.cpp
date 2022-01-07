// AxesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ModelViewer.h"
#include "AxesDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAxesDlg dialog


CAxesDlg::CAxesDlg(CWnd* pParent, CMvDoc *pDoc)
	: CDialog(CAxesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAxesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_pDoc = pDoc;
	m_nID = CAxesDlg::IDD;

	m_PropertySheet = new CPropertySheet;
	m_AxesPage = new CAxesPage;
	m_AxesPage->m_pDoc = pDoc;
	m_AxesPage->m_Parent = this;
   m_PropertySheet->AddPage(m_AxesPage);


}


CAxesDlg::~CAxesDlg()
{
	delete m_PropertySheet;
	delete m_AxesPage;
}


void CAxesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxesDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxesDlg, CDialog)
	//{{AFX_MSG_MAP(CAxesDlg)
	ON_BN_CLICKED(ID_AxesDone, OnDone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxesDlg message handlers

BOOL CAxesDlg::Create()
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
	//m_PropertySheet->SetActivePage(1);
	m_PropertySheet->SetActivePage(0);
	
	
	
	return TRUE;
}


void CAxesDlg::PostNcDestroy() 
{
	delete this;
}

BOOL CAxesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	WINDOWPLACEMENT wndpl;
	GetWindowPlacement(&wndpl);
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	int dlgWidth = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
	int dlgHeight = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
	wndpl.rcNormalPosition.right = screenWidth - 600;
	wndpl.rcNormalPosition.left = wndpl.rcNormalPosition.right - dlgWidth;
	wndpl.rcNormalPosition.top = 10;
	wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgHeight;
	wndpl.showCmd = SW_HIDE;
	SetWindowPlacement(&wndpl);

//	m_ApplyButton.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CAxesDlg::OnDone() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
	
}

void CAxesDlg::Reinitialize()
{
	m_AxesPage->Reinitialize();
}
