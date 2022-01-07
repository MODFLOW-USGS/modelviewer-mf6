// PathlinesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ModelViewer.h"
#include "PathlinesDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPathlinesDlg dialog


CPathlinesDlg::CPathlinesDlg(CWnd* pParent, CMvDoc *pDoc)
	: CDialog(CPathlinesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPathlinesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ASSERT(pParent != NULL);

	m_pParent = pParent;
	m_pDoc = pDoc;
	m_nID = CPathlinesDlg::IDD;
	m_PropertySheet = new CPropertySheet;
	m_DisplayPage = new CPathlinesDisplayPage;
	m_DisplayPage->m_pDoc = pDoc;
	m_DisplayPage->m_Parent = this;
	m_ColorPage = new CPathlinesColorPage;
	m_ColorPage->m_pDoc = pDoc;
	m_ColorPage->m_Parent = this;
	m_ClipPage = new CPathlinesClippingPage;
	m_ClipPage->m_pDoc = pDoc;
	m_ClipPage->m_Parent = this;
	m_PropertySheet->AddPage(m_DisplayPage);
	m_PropertySheet->AddPage(m_ColorPage);
	m_PropertySheet->AddPage(m_ClipPage);
}

CPathlinesDlg::~CPathlinesDlg()
{
	delete m_PropertySheet;
	delete m_DisplayPage;
	delete m_ColorPage;
	delete m_ClipPage;
}


void CPathlinesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPathlinesDlg)
	DDX_Control(pDX, IDC_DEFAULT, m_DefaultButton);
	DDX_Control(pDX, IDC_APPLY, m_ApplyButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPathlinesDlg, CDialog)
	//{{AFX_MSG_MAP(CPathlinesDlg)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_BN_CLICKED(IDC_DONE, OnDone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathlinesDlg message handlers

BOOL CPathlinesDlg::Create()
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

void CPathlinesDlg::PostNcDestroy() 
{
	delete this;
}

BOOL CPathlinesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	WINDOWPLACEMENT wndpl;
	GetWindowPlacement(&wndpl);
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	int dlgWidth = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
	int dlgHeight = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
	wndpl.rcNormalPosition.left = 175;
	wndpl.rcNormalPosition.right = wndpl.rcNormalPosition.left + dlgWidth;
	wndpl.rcNormalPosition.top = screenHeight - dlgHeight - 30;
	wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgHeight;
	wndpl.showCmd = SW_HIDE;
	SetWindowPlacement(&wndpl);

	m_ApplyButton.EnableWindow(FALSE);
	m_DefaultButton.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPathlinesDlg::Reinitialize()
{
	m_DisplayPage->Reinitialize();
	m_ColorPage->Reinitialize();
	m_ClipPage->Reinitialize();
	m_ApplyButton.EnableWindow(FALSE);
	m_DefaultButton.EnableWindow(FALSE);
}

void CPathlinesDlg::Activate(BOOL b)
{
	m_DisplayPage->Activate(b);
	m_ColorPage->Activate(b);
	m_ClipPage->Activate(b);
	if (b)
	{
		BOOL bb;
		switch (m_PropertySheet->GetActiveIndex())
		{
		case 0:
			bb = ((CButton *) m_DisplayPage->GetDlgItem(IDC_TUBE))->GetCheck();
			m_ApplyButton.EnableWindow(bb);
			m_DefaultButton.EnableWindow(FALSE);
			break;
		case 1:
			m_ApplyButton.EnableWindow(TRUE);
			m_DefaultButton.EnableWindow(TRUE);
			break;
		case 2:
			bb = ((CButton *) m_ClipPage->GetDlgItem(IDC_CLIP_BY_TIME_RANGE))->GetCheck();
			m_ApplyButton.EnableWindow(bb);
			m_DefaultButton.EnableWindow(bb);
			break;
		}
	}
	else
	{
		m_ApplyButton.EnableWindow(FALSE);
		m_DefaultButton.EnableWindow(FALSE);
	}
}

void CPathlinesDlg::OnApply() 
{
	switch(m_PropertySheet->GetActiveIndex())
	{
	case 0:
		m_DisplayPage->Apply();
		break;
	case 1:
		m_ColorPage->Apply();
		break;
	case 2:
		m_ClipPage->Apply();
		break;
	}
}

void CPathlinesDlg::OnDefault() 
{
	if (AfxMessageBox("Do you want to use default settings?", MB_YESNO) == IDNO)
	{
		return;
	}
	switch(m_PropertySheet->GetActiveIndex())
	{
	case 1:
		m_ColorPage->OnDefault();
		break;
	case 2:
		m_ClipPage->OnDefault();
		break;
	}
}

void CPathlinesDlg::OnDone() 
{
	ShowWindow(SW_HIDE);
}
