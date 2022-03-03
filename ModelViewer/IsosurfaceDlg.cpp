// IsosurfaceDlg.cpp : implementation file
//

#include "ModelViewer.h"
#include "IsosurfaceDlg.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIsosurfaceDlg dialog

CIsosurfaceDlg::CIsosurfaceDlg(CWnd* pParent, CMvDoc* pDoc)
    : CDialog(CIsosurfaceDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CIsosurfaceDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT

    ASSERT(pParent != NULL);

    m_pParent                       = pParent;
    m_pDoc                          = pDoc;
    m_nID                           = CIsosurfaceDlg::IDD;
    m_PropertySheet                 = new CPropertySheet;
    m_RegularIsosurfacePage         = new CRegularIsosurfacePage;
    m_RegularIsosurfacePage->m_pDoc = m_pDoc;
    m_CustomIsosurfacePage          = new CCustomIsosurfacePage;
    m_CustomIsosurfacePage->m_pDoc  = m_pDoc;
    m_PropertySheet->AddPage(m_RegularIsosurfacePage);
    m_PropertySheet->AddPage(m_CustomIsosurfacePage);
}

CIsosurfaceDlg::~CIsosurfaceDlg()
{
    delete m_PropertySheet;
    delete m_RegularIsosurfacePage;
    delete m_CustomIsosurfacePage;
}

void CIsosurfaceDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CIsosurfaceDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIsosurfaceDlg, CDialog)
    //{{AFX_MSG_MAP(CIsosurfaceDlg)
    ON_BN_CLICKED(IDC_APPLY, OnApply)
    ON_BN_CLICKED(IDC_DONE, OnDone)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIsosurfaceDlg message handlers

BOOL CIsosurfaceDlg::Create()
{
    if (!CDialog::Create(m_nID, m_pParent))
    {
        return FALSE;
    }

    m_PropertySheet->Create(this, WS_CHILD | WS_VISIBLE, 0);

    CRect rcSheet;
    GetDlgItem(IDC_PROP_SHEET)->GetWindowRect(&rcSheet);
    ScreenToClient(&rcSheet);

    CTabCtrl* tabctrl = m_PropertySheet->GetTabControl();
    tabctrl->MoveWindow(0, 0, rcSheet.Width(), rcSheet.Height());
    m_PropertySheet->SetWindowPos(NULL, rcSheet.left, rcSheet.top,
                                  rcSheet.Width(), rcSheet.Height(),
                                  SWP_NOZORDER | SWP_NOACTIVATE);

    m_PropertySheet->ModifyStyleEx(0, WS_EX_CONTROLPARENT);
    m_PropertySheet->ModifyStyle(0, WS_TABSTOP);
    // Force the controls on the property pages to be created
    m_PropertySheet->SetActivePage(1);
    m_PropertySheet->SetActivePage(0);

    return TRUE;
}

BOOL CIsosurfaceDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    WINDOWPLACEMENT wndpl;
    GetWindowPlacement(&wndpl);
    int screenWidth               = ::GetSystemMetrics(SM_CXSCREEN);
    int screenHeight              = ::GetSystemMetrics(SM_CYSCREEN);
    int dlgWidth                  = wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left;
    int dlgHeight                 = wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top;
    wndpl.rcNormalPosition.right  = screenWidth - 10;
    wndpl.rcNormalPosition.left   = wndpl.rcNormalPosition.right - dlgWidth;
    wndpl.rcNormalPosition.top    = screenHeight / 2 - 50;
    wndpl.rcNormalPosition.bottom = wndpl.rcNormalPosition.top + dlgHeight;
    wndpl.showCmd                 = SW_HIDE;
    SetWindowPlacement(&wndpl);

    GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);

    return TRUE; // return TRUE unless you set the focus to a control
                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CIsosurfaceDlg::Reinitialize()
{
    m_RegularIsosurfacePage->Reinitialize();
    m_CustomIsosurfacePage->Reinitialize();
    GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
}

void CIsosurfaceDlg::PostNcDestroy()
{
    delete this;
}

void CIsosurfaceDlg::Activate(BOOL b)
{
    m_RegularIsosurfacePage->Activate(b);
    m_CustomIsosurfacePage->Activate(b);
    GetDlgItem(IDC_APPLY)->EnableWindow(b);
}

void CIsosurfaceDlg::OnApply()
{
    switch (m_PropertySheet->GetActiveIndex())
    {
    case 0:
        m_RegularIsosurfacePage->Apply();
        break;
    case 1:
        m_CustomIsosurfacePage->Apply();
        break;
    }
}

void CIsosurfaceDlg::OnDone()
{
    ShowWindow(SW_HIDE);
}
